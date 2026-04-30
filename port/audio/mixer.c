/**
 * mixer.c — CPU-side Acmd audio interpreter for SSB64 PC port
 *
 * Replaces N64 RSP audio microcode with scalar CPU implementations.
 * Standard N64 ABI: 16 opcodes (A_SPNOOP through A_SETLOOP).
 *
 * Simulates the RSP's 4KB DMEM as a flat byte array. Each opcode
 * implementation reads/writes this buffer identically to the RSP
 * microcode, producing bit-exact audio output.
 *
 * Ported from Starship (Star Fox 64) PC port mixer.c, adapted for
 * SSB64's standard N64 audio ABI (stereo, no 5.1 surround).
 *
 * Key differences from Starship:
 *   - A_ENVMIXER (3): Standard N64 envelope mixer (stereo dry/wet),
 *     not Starship's extended 5.1 surround version (opcode 19)
 *   - A_SETVOL (9): Volume/target/rate setup (Starship uses EnvSetup1/2)
 *   - A_POLEF (14): IIR pole filter (Starship uses FIR A_FILTER/7)
 *   - A_MIXER uses rspa.nbytes, not an explicit count parameter
 *   - A_INTERLEAVE is stereo-only (2 channel)
 *   - A_LOADBUFF/A_SAVEBUFF use rspa.out/in from prior A_SETBUFF
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mixer.h"

/* ------------------------------------------------------------------ */
/*  Rounding helpers                                                  */
/* ------------------------------------------------------------------ */

#define ROUND_UP_16(v)   (((v) + 15) & ~15)
#define ROUND_UP_32(v)   (((v) + 31) & ~31)
#define ROUND_UP_8(v)    (((v) + 7) & ~7)

/* ------------------------------------------------------------------ */
/*  Simulated DMEM — 4KB, matching N64 RSP data memory                */
/* ------------------------------------------------------------------ */

#define DMEM_SIZE 0x1000  /* 4096 bytes */

/* DMEM buffer starts at address 0 (standard N64 audio ucode).
 * No offset subtraction needed, unlike Starship's custom ucode. */
#define BUF_U8(a)  (rspa.buf + (a))
#define BUF_S16(a) ((int16_t*)(rspa.buf + (a)))

/* ------------------------------------------------------------------ */
/*  Mixer state — persists across Acmd calls within a frame           */
/* ------------------------------------------------------------------ */

static struct {
	/* Buffer parameters (set by A_SETBUFF) */
	uint16_t in;
	uint16_t out;
	uint16_t nbytes;

	/* Volume state (set by A_SETVOL, consumed by A_ENVMIXER) */
	int16_t  vol_left;
	int16_t  vol_right;
	int16_t  tgt_left;
	int16_t  tgt_right;
	int16_t  rate_left_m;   /* ramp rate mantissa */
	uint16_t rate_left_l;   /* ramp rate low bits */
	int16_t  rate_right_m;
	uint16_t rate_right_l;
	int16_t  dry_amt;
	int16_t  wet_amt;

	/* ADPCM state */
	int16_t *adpcm_loop_state;
	int16_t  adpcm_table[8][2][8];

	/* Pole filter coefficients (loaded via A_LOADADPCM for POLEF) */
	int16_t  polef_coefs[8];

	/* Simulated DMEM */
	uint8_t  buf[DMEM_SIZE];

	/* The RSP mixes into wider internal accumulators and clamps late.  Keep
	 * fixed N_AUDIO output buses wide too; otherwise loud layered SFX clip
	 * once per voice before the aux/main bus passes can rebalance them. */
	int32_t  dry_left_acc[PORT_NAUDIO_COUNT / sizeof(int16_t)];
	int32_t  dry_right_acc[PORT_NAUDIO_COUNT / sizeof(int16_t)];
	int32_t  wet_left_acc[PORT_NAUDIO_COUNT / sizeof(int16_t)];
	int32_t  wet_right_acc[PORT_NAUDIO_COUNT / sizeof(int16_t)];
} rspa;

/* ------------------------------------------------------------------ */
/*  Resample table — 64-entry 4-tap Lagrange interpolation            */
/* ------------------------------------------------------------------ */

static const int16_t resample_table[64][4] = {
	{ 0x0c39, 0x66ad, 0x0d46, 0xffdf }, { 0x0b39, 0x6696, 0x0e5f, 0xffd8 },
	{ 0x0a44, 0x6669, 0x0f83, 0xffd0 }, { 0x095a, 0x6626, 0x10b4, 0xffc8 },
	{ 0x087d, 0x65cd, 0x11f0, 0xffbf }, { 0x07ab, 0x655e, 0x1338, 0xffb6 },
	{ 0x06e4, 0x64d9, 0x148c, 0xffac }, { 0x0628, 0x643f, 0x15eb, 0xffa1 },
	{ 0x0577, 0x638f, 0x1756, 0xff96 }, { 0x04d1, 0x62cb, 0x18cb, 0xff8a },
	{ 0x0435, 0x61f3, 0x1a4c, 0xff7e }, { 0x03a4, 0x6106, 0x1bd7, 0xff71 },
	{ 0x031c, 0x6007, 0x1d6c, 0xff64 }, { 0x029f, 0x5ef5, 0x1f0b, 0xff56 },
	{ 0x022a, 0x5dd0, 0x20b3, 0xff48 }, { 0x01be, 0x5c9a, 0x2264, 0xff3a },
	{ 0x015b, 0x5b53, 0x241e, 0xff2c }, { 0x0101, 0x59fc, 0x25e0, 0xff1e },
	{ 0x00ae, 0x5896, 0x27a9, 0xff10 }, { 0x0063, 0x5720, 0x297a, 0xff02 },
	{ 0x001f, 0x559d, 0x2b50, 0xfef4 }, { 0xffe2, 0x540d, 0x2d2c, 0xfee8 },
	{ 0xffac, 0x5270, 0x2f0d, 0xfedb }, { 0xff7c, 0x50c7, 0x30f3, 0xfed0 },
	{ 0xff53, 0x4f14, 0x32dc, 0xfec6 }, { 0xff2e, 0x4d57, 0x34c8, 0xfebd },
	{ 0xff0f, 0x4b91, 0x36b6, 0xfeb6 }, { 0xfef5, 0x49c2, 0x38a5, 0xfeb0 },
	{ 0xfedf, 0x47ed, 0x3a95, 0xfeac }, { 0xfece, 0x4611, 0x3c85, 0xfeab },
	{ 0xfec0, 0x4430, 0x3e74, 0xfeac }, { 0xfeb6, 0x424a, 0x4060, 0xfeaf },
	{ 0xfeaf, 0x4060, 0x424a, 0xfeb6 }, { 0xfeac, 0x3e74, 0x4430, 0xfec0 },
	{ 0xfeab, 0x3c85, 0x4611, 0xfece }, { 0xfeac, 0x3a95, 0x47ed, 0xfedf },
	{ 0xfeb0, 0x38a5, 0x49c2, 0xfef5 }, { 0xfeb6, 0x36b6, 0x4b91, 0xff0f },
	{ 0xfebd, 0x34c8, 0x4d57, 0xff2e }, { 0xfec6, 0x32dc, 0x4f14, 0xff53 },
	{ 0xfed0, 0x30f3, 0x50c7, 0xff7c }, { 0xfedb, 0x2f0d, 0x5270, 0xffac },
	{ 0xfee8, 0x2d2c, 0x540d, 0xffe2 }, { 0xfef4, 0x2b50, 0x559d, 0x001f },
	{ 0xff02, 0x297a, 0x5720, 0x0063 }, { 0xff10, 0x27a9, 0x5896, 0x00ae },
	{ 0xff1e, 0x25e0, 0x59fc, 0x0101 }, { 0xff2c, 0x241e, 0x5b53, 0x015b },
	{ 0xff3a, 0x2264, 0x5c9a, 0x01be }, { 0xff48, 0x20b3, 0x5dd0, 0x022a },
	{ 0xff56, 0x1f0b, 0x5ef5, 0x029f }, { 0xff64, 0x1d6c, 0x6007, 0x031c },
	{ 0xff71, 0x1bd7, 0x6106, 0x03a4 }, { 0xff7e, 0x1a4c, 0x61f3, 0x0435 },
	{ 0xff8a, 0x18cb, 0x62cb, 0x04d1 }, { 0xff96, 0x1756, 0x638f, 0x0577 },
	{ 0xffa1, 0x15eb, 0x643f, 0x0628 }, { 0xffac, 0x148c, 0x64d9, 0x06e4 },
	{ 0xffb6, 0x1338, 0x655e, 0x07ab }, { 0xffbf, 0x11f0, 0x65cd, 0x087d },
	{ 0xffc8, 0x10b4, 0x6626, 0x095a }, { 0xffd0, 0x0f83, 0x6669, 0x0a44 },
	{ 0xffd8, 0x0e5f, 0x6696, 0x0b39 }, { 0xffdf, 0x0d46, 0x66ad, 0x0c39 },
};

/* ------------------------------------------------------------------ */
/*  Clamping helpers                                                  */
/* ------------------------------------------------------------------ */

static inline int16_t clamp16(int32_t v) {
	if (v < -0x8000) return -0x8000;
	if (v > 0x7fff) return 0x7fff;
	return (int16_t)v;
}

static int32_t *fixed_bus_acc(uint16_t addr) {
	switch (addr) {
	case PORT_NAUDIO_DRY_LEFT:
		return rspa.dry_left_acc;
	case PORT_NAUDIO_DRY_RIGHT:
		return rspa.dry_right_acc;
	case PORT_NAUDIO_WET_LEFT:
		return rspa.wet_left_acc;
	case PORT_NAUDIO_WET_RIGHT:
		return rspa.wet_right_acc;
	default:
		return NULL;
	}
}

static void fixed_bus_clear(uint16_t addr, int nbytes) {
	int count = nbytes / (int)sizeof(int16_t);
	int offset;

	if (addr < PORT_NAUDIO_DRY_LEFT || count <= 0) {
		return;
	}

	offset = (addr - PORT_NAUDIO_DRY_LEFT) / (int)sizeof(int16_t);
	if (offset < 0 || offset >= (int)(PORT_NAUDIO_COUNT * 4 / sizeof(int16_t))) {
		return;
	}

	while (count > 0 && offset < (int)(PORT_NAUDIO_COUNT * 4 / sizeof(int16_t))) {
		int bus = offset / (PORT_NAUDIO_COUNT / (int)sizeof(int16_t));
		int bus_index = offset % (PORT_NAUDIO_COUNT / (int)sizeof(int16_t));
		int run = (PORT_NAUDIO_COUNT / (int)sizeof(int16_t)) - bus_index;
		int32_t *acc;

		if (run > count) {
			run = count;
		}

		switch (bus) {
		case 0: acc = rspa.dry_left_acc; break;
		case 1: acc = rspa.dry_right_acc; break;
		case 2: acc = rspa.wet_left_acc; break;
		case 3: acc = rspa.wet_right_acc; break;
		default: return;
		}

		memset(acc + bus_index, 0, (size_t)run * sizeof(*acc));
		offset += run;
		count -= run;
	}
}

static int32_t sample_read(uint16_t base, int index) {
	int32_t *acc = fixed_bus_acc(base);
	return acc ? acc[index] : BUF_S16(base)[index];
}

static void sample_write(uint16_t base, int index, int32_t value) {
	int32_t *acc = fixed_bus_acc(base);
	if (acc) {
		acc[index] = value;
	} else {
		BUF_S16(base)[index] = clamp16(value);
	}
}

static void sample_add(uint16_t base, int index, int32_t value) {
	int32_t *acc = fixed_bus_acc(base);
	if (acc) {
		acc[index] += value;
	} else {
		BUF_S16(base)[index] = clamp16((int32_t)BUF_S16(base)[index] + value);
	}
}

static inline int16_t adpcm_predict_sample(uint8_t byte, uint8_t mask,
                                           unsigned lshift, unsigned rshift) {
	int16_t sample = (uint16_t)(byte & mask) << lshift;
	return sample >> rshift;
}

/* ================================================================== */
/*  A_CLEARBUFF (opcode 2) — Zero a region of DMEM                    */
/* ================================================================== */

void aClearBufferImpl(uint16_t addr, int nbytes) {
	int32_t *acc = fixed_bus_acc(addr);
	nbytes = ROUND_UP_16(nbytes);
	if (acc) {
		fixed_bus_clear(addr, nbytes);
	}
	memset(BUF_U8(addr), 0, nbytes);
}

/* ================================================================== */
/*  A_SETBUFF (opcode 8) — Configure in/out/count for next command    */
/* ================================================================== */

void aSetBufferImpl(uint8_t flags, uint16_t in, uint16_t out, uint16_t nbytes) {
	rspa.in = in;
	rspa.out = out;
	rspa.nbytes = nbytes;
}

/* ================================================================== */
/*  A_LOADBUFF (opcode 4) — Load from DRAM into DMEM                  */
/*  Uses rspa.out as dest DMEM addr, rspa.nbytes as count.            */
/* ================================================================== */

void aLoadBufferImpl(uintptr_t source_addr) {
	if (source_addr == 0) return;
	memcpy(BUF_U8(rspa.out), (const void*)source_addr, ROUND_UP_8(rspa.nbytes));
	/* Mirror: if the destination DMEM addr aliases a fixed-bus int32
	 * accumulator (wet/dry), populate that accumulator from the loaded
	 * s16 bytes so subsequent sample_read calls on this addr see the
	 * data. Otherwise the FX chain's "save → process → load → mix"
	 * delay-line round-trip silently drops anything that flows through
	 * a wet/dry-mapped DMEM address. */
	{
		int32_t *acc = fixed_bus_acc(rspa.out);
		if (acc) {
			int count = ROUND_UP_16(rspa.nbytes) / (int)sizeof(int16_t);
			int16_t *src = BUF_S16(rspa.out);
			int i;
			for (i = 0; i < count; i++) acc[i] = src[i];
		}
	}
}

/* ================================================================== */
/*  A_SAVEBUFF (opcode 6) — Save from DMEM to DRAM                    */
/*  Uses rspa.in as source DMEM addr, rspa.nbytes as count.           */
/* ================================================================== */

void aSaveBufferImpl(uintptr_t dest_addr) {
	if (dest_addr == 0) return;
	/* Mirror: if the source DMEM addr aliases a fixed-bus int32
	 * accumulator (wet/dry), clamp16 each int32 sample and copy
	 * into the s16 mirror first so memcpy actually carries the
	 * accumulated bus data, not the stale s16 region underneath. */
	{
		int32_t *acc = fixed_bus_acc(rspa.in);
		if (acc) {
			int count = ROUND_UP_16(rspa.nbytes) / (int)sizeof(int16_t);
			int16_t *dst = BUF_S16(rspa.in);
			int i;
			for (i = 0; i < count; i++) dst[i] = clamp16(acc[i]);
		}
	}
	memcpy((void*)dest_addr, BUF_U8(rspa.in), ROUND_UP_8(rspa.nbytes));
}

/* ================================================================== */
/*  A_DMEMMOVE (opcode 10) — Move data within DMEM                    */
/* ================================================================== */

void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes) {
	int32_t *in_acc = fixed_bus_acc(in_addr);
	int32_t *out_acc = fixed_bus_acc(out_addr);
	nbytes = ROUND_UP_16(nbytes);

	if (in_acc || out_acc) {
		int count = nbytes / (int)sizeof(int16_t);
		int i;
		if (in_acc && out_acc) {
			memmove(out_acc, in_acc, (size_t)count * sizeof(*out_acc));
		} else if (out_acc) {
			int16_t *in = BUF_S16(in_addr);
			for (i = 0; i < count; i++) {
				out_acc[i] = in[i];
			}
		} else {
			int16_t *out = BUF_S16(out_addr);
			for (i = 0; i < count; i++) {
				out[i] = clamp16(in_acc[i]);
			}
		}
		return;
	}

	memmove(BUF_U8(out_addr), BUF_U8(in_addr), nbytes);
}

/* ================================================================== */
/*  A_LOADADPCM (opcode 11) — Load ADPCM codebook from DRAM           */
/* ================================================================== */

void aLoadADPCMImpl(int count, uintptr_t book_addr) {
	if (book_addr == 0) return;
	memcpy(rspa.adpcm_table, (const void*)book_addr, count);
}

/* ================================================================== */
/*  A_SETLOOP (opcode 15) — Set ADPCM loop state pointer              */
/* ================================================================== */

void aSetLoopImpl(uintptr_t adpcm_loop_state) {
	rspa.adpcm_loop_state = (int16_t*)adpcm_loop_state;
}

/* ================================================================== */
/*  A_ADPCM (opcode 1) — ADPCM decompression                         */
/*  Scalar implementation from Starship.                              */
/* ================================================================== */

void aADPCMdecImpl(uint8_t flags, int16_t *state) {
	uint8_t *in = BUF_U8(rspa.in);
	int16_t *out = BUF_S16(rspa.out);
	int nbytes = ROUND_UP_32(rspa.nbytes);
	int i, j, k;

	/* PORT: dump first ADPCM call to disk for offline reference comparison.
	 * Captures: book (rspa.adpcm_table), input bytes (DMEM at rspa.in), prior
	 * state (16 s16), flags, nbytes — all the inputs needed to reproduce the
	 * decode in a Python reference.  Output samples are written after the
	 * decode finishes.  Set SSB64_ADPCM_DUMP=1 to enable. */
	{
		extern void port_log(const char *fmt, ...);
		static int dumped = -1;
		if (dumped == -1) {
			dumped = (getenv("SSB64_ADPCM_DUMP") && getenv("SSB64_ADPCM_DUMP")[0] == '1') ? 0 : -2;
		}
		if (dumped == 0) {
			dumped = 1;
			FILE *f = fopen("/tmp/adpcm_dump.bin", "wb");
			if (f) {
				/* Header (32 bytes): magic 'ADPCMDP1', flags(u32), nbytes(s32),
				 * in_offset(u16) [DMEM addr], out_offset(u16), reserved×16 */
				fwrite("ADPCMDP1", 1, 8, f);
				uint32_t fl = flags;            fwrite(&fl, 4, 1, f);
				int32_t  nb = nbytes;           fwrite(&nb, 4, 1, f);
				uint16_t in_o = rspa.in;        fwrite(&in_o, 2, 1, f);
				uint16_t out_o = rspa.out;      fwrite(&out_o, 2, 1, f);
				uint8_t pad[12] = {0};          fwrite(pad, 1, 12, f);
				/* Codebook: 8 × 2 × 8 = 128 s16 = 256 bytes */
				fwrite(rspa.adpcm_table, sizeof(int16_t), 8 * 2 * 8, f);
				/* Loop state pointer (8 bytes) — write its content if non-NULL */
				if (rspa.adpcm_loop_state) {
					fwrite(rspa.adpcm_loop_state, sizeof(int16_t), 16, f);
				} else {
					int16_t zero[16] = {0};
					fwrite(zero, sizeof(int16_t), 16, f);
				}
				/* Prior decoder state (16 s16) — what the impl will load */
				fwrite(state, sizeof(int16_t), 16, f);
				/* Input bytes from DMEM */
				fwrite(in, 1, nbytes, f);
				fclose(f);
				port_log("SSB64 Audio: ADPCM dump → /tmp/adpcm_dump.bin (flags=0x%02x nbytes=%d in=0x%04x out=0x%04x)\n",
				         (unsigned)flags, nbytes, (unsigned)rspa.in, (unsigned)rspa.out);
			}
		}
	}

	if (flags & 0x01) { /* A_INIT */
		memset(out, 0, 16 * sizeof(int16_t));
	} else if (flags & 0x02) { /* A_LOOP */
		memcpy(out, rspa.adpcm_loop_state, 16 * sizeof(int16_t));
	} else {
		memcpy(out, state, 16 * sizeof(int16_t));
	}
	out += 16;

	while (nbytes > 0) {
		int shift = *in >> 4;
		int table_index = *in++ & 0xf;
		int16_t (*tbl)[8] = rspa.adpcm_table[table_index];

		for (i = 0; i < 2; i++) {
			int16_t ins[8];
			int16_t prev1 = out[-1];
			int16_t prev2 = out[-2];

			if (flags & 4) {
				/* 2-bit ADPCM */
				unsigned rshift = (shift < 14) ? (14 - shift) : 0;
				for (j = 0; j < 2; j++) {
					uint8_t byte = *in++;
					ins[j * 4]     = adpcm_predict_sample(byte, 0xc0, 8, rshift);
					ins[j * 4 + 1] = adpcm_predict_sample(byte, 0x30, 10, rshift);
					ins[j * 4 + 2] = adpcm_predict_sample(byte, 0x0c, 12, rshift);
					ins[j * 4 + 3] = adpcm_predict_sample(byte, 0x03, 14, rshift);
				}
			} else {
				/* 4-bit ADPCM */
				unsigned rshift = (shift < 12) ? (12 - shift) : 0;
				for (j = 0; j < 4; j++) {
					uint8_t byte = *in++;
					ins[j * 2]     = adpcm_predict_sample(byte, 0xf0, 8, rshift);
					ins[j * 2 + 1] = adpcm_predict_sample(byte, 0x0f, 12, rshift);
				}
			}

			for (j = 0; j < 8; j++) {
				int32_t acc = tbl[0][j] * prev2 + tbl[1][j] * prev1 + (ins[j] << 11);
				for (k = 0; k < j; k++) {
					acc += tbl[1][((j - k) - 1)] * ins[k];
				}
				acc >>= 11;
				*out++ = clamp16(acc);
			}
		}
		nbytes -= 16 * sizeof(int16_t);
	}
	memcpy(state, out - 16, 16 * sizeof(int16_t));

	/* Append output samples to the dump file (only on first call). */
	{
		static int output_dumped = 0;
		if (!output_dumped && getenv("SSB64_ADPCM_DUMP") && getenv("SSB64_ADPCM_DUMP")[0] == '1') {
			output_dumped = 1;
			FILE *f = fopen("/tmp/adpcm_dump_output.bin", "wb");
			if (f) {
				/* Restart from rspa.out — total samples decoded =
				 * 16 (initial state copy) + (nbytes_orig / 9) * 16 (per ADPCM frame).
				 * Just write everything from rspa.out for the original nbytes_orig
				 * worth of decoded samples. */
				int16_t *out_base = BUF_S16(rspa.out);
				int decoded_samples = (int)(out - out_base);
				fwrite(out_base, sizeof(int16_t), decoded_samples, f);
				fclose(f);
			}
		}
	}
}

/* ================================================================== */
/*  A_RESAMPLE (opcode 5) — 4-tap interpolated resampling             */
/*  Scalar implementation from Starship.                              */
/* ================================================================== */

void aResampleImpl(uint8_t flags, uint16_t pitch, int16_t *state) {
	int16_t tmp[16];
	int16_t *in_initial = BUF_S16(rspa.in);
	int16_t *in = in_initial;
	int16_t *out = BUF_S16(rspa.out);
	int nbytes = ROUND_UP_16(rspa.nbytes);
	uint32_t pitch_accumulator;
	int i;
	const int16_t *tbl;
	int32_t sample;

	/* PORT: dump first resample call to disk for offline reference comparison.
	 * Captures: flags, pitch, prior state (16 s16), DMEM contents around the
	 * input area (the resampler reads in[-4..nbytes/8+4] roughly), rspa.in/out/
	 * nbytes — enough for a Python reference to reproduce the math.
	 * Set SSB64_RESAMPLE_DUMP=1 to enable. */
	{
		extern void port_log(const char *fmt, ...);
		static int dumped = -1;
		if (dumped == -1) {
			dumped = (getenv("SSB64_RESAMPLE_DUMP") &&
			          getenv("SSB64_RESAMPLE_DUMP")[0] == '1') ? 0 : -2;
		}
		if (dumped == 0) {
			dumped = 1;
			FILE *f = fopen("/tmp/resample_dump.bin", "wb");
			if (f) {
				/* Header (32 bytes): magic 'RSAMPDP1', flags(u32), pitch(u32),
				 * nbytes(s32), in(u16), out(u16), reserved(12) */
				fwrite("RSAMPDP1", 1, 8, f);
				uint32_t fl = flags;          fwrite(&fl, 4, 1, f);
				uint32_t pt = pitch;          fwrite(&pt, 4, 1, f);
				int32_t  nb = nbytes;         fwrite(&nb, 4, 1, f);
				uint16_t in_o = rspa.in;      fwrite(&in_o, 2, 1, f);
				uint16_t out_o = rspa.out;    fwrite(&out_o, 2, 1, f);
				uint8_t pad[10] = {0};        fwrite(pad, 1, 10, f);
				/* Prior state: 16 s16 */
				fwrite(state, sizeof(int16_t), 16, f);
				/* Full DMEM (4 KB) — captures any input range the resampler
				 * may touch including lookbehind / lookahead. */
				fwrite(rspa.buf, 1, sizeof(rspa.buf), f);
				fclose(f);
				port_log("SSB64 Audio: resample dump → /tmp/resample_dump.bin (flags=0x%02x pitch=0x%04x nbytes=%d in=0x%04x out=0x%04x)\n",
				         (unsigned)flags, (unsigned)pitch, nbytes,
				         (unsigned)rspa.in, (unsigned)rspa.out);
			}
		}
	}

	if (flags & 0x01) { /* A_INIT */
		memset(tmp, 0, 5 * sizeof(int16_t));
	} else {
		memcpy(tmp, state, 16 * sizeof(int16_t));
	}
	if (flags & 0x02) { /* A_LOOP */
		memcpy(in - 8, tmp + 8, 8 * sizeof(int16_t));
		in -= tmp[5] / (int)sizeof(int16_t);
	}
	in -= 4;
	pitch_accumulator = (uint16_t)tmp[4];
	memcpy(in, tmp, 4 * sizeof(int16_t));

	do {
		for (i = 0; i < 8; i++) {
			tbl = resample_table[pitch_accumulator * 64 >> 16];
			sample = ((int32_t)in[0] * tbl[0] +
			          (int32_t)in[1] * tbl[1] +
			          (int32_t)in[2] * tbl[2] +
			          (int32_t)in[3] * tbl[3]) >> 15;
			*out++ = clamp16(sample);

			pitch_accumulator += (pitch << 1);
			in += pitch_accumulator >> 16;
			pitch_accumulator %= 0x10000;
		}
		nbytes -= 8 * sizeof(int16_t);
	} while (nbytes > 0);

	state[4] = (int16_t)pitch_accumulator;
	memcpy(state, in, 4 * sizeof(int16_t));
	i = (int)(in - in_initial + 4) & 7;
	in -= i;
	if (i != 0) {
		i = -8 - i;
	}
	state[5] = (int16_t)i;
	memcpy(state + 8, in, 8 * sizeof(int16_t));

	/* Append output samples to the dump on first call only. */
	{
		static int output_dumped = 0;
		if (!output_dumped && getenv("SSB64_RESAMPLE_DUMP") &&
		    getenv("SSB64_RESAMPLE_DUMP")[0] == '1') {
			output_dumped = 1;
			FILE *f = fopen("/tmp/resample_dump_output.bin", "wb");
			if (f) {
				int16_t *out_base = BUF_S16(rspa.out);
				int n_samples = (int)(out - out_base);
				/* Also write final state (post-call) so reference can
				 * verify state save matches. */
				fwrite(&n_samples, 4, 1, f);
				fwrite(out_base, sizeof(int16_t), n_samples, f);
				fwrite(state, sizeof(int16_t), 16, f);
				fclose(f);
			}
		}
	}
}

/* ================================================================== */
/*  A_INTERLEAVE (opcode 13) — Stereo channel interleave              */
/*  Reads separate L/R buffers, writes interleaved L-R-L-R to out.    */
/* ================================================================== */

void aInterleaveImpl(uint16_t left, uint16_t right) {
	int count;
	int16_t *d;
	int i;

	if (rspa.nbytes == 0) return;

	count = rspa.nbytes / (2 * sizeof(int16_t));
	d = BUF_S16(rspa.out);

	for (i = 0; i < count; i++) {
		*d++ = clamp16(sample_read(left, i));
		*d++ = clamp16(sample_read(right, i));
	}
}

/* ================================================================== */
/*  A_SETVOL (opcode 9) — Set volume parameters for A_ENVMIXER        */
/*                                                                    */
/*  Flag combos from pull chain (n_env.c):                            */
/*    A_LEFT  | A_VOL  (0x06): set left current volume                */
/*    A_RIGHT | A_VOL  (0x04): set right current volume               */
/*    A_LEFT  | A_RATE (0x02): set left target + ramp rate            */
/*    A_RIGHT | A_RATE (0x00): set right target + ramp rate           */
/*    A_AUX          (0x08): set dry/wet amounts                      */
/* ================================================================== */

void aSetVolumeImpl(uint16_t flags, uint16_t vol, uint16_t voltgt, uint16_t volrate) {
	if (flags & 0x08) { /* A_AUX */
		rspa.dry_amt = (int16_t)vol;
		rspa.wet_amt = (int16_t)volrate;
	} else if (flags & 0x04) { /* A_VOL */
		if (flags & 0x02) { /* A_LEFT */
			rspa.vol_left = (int16_t)vol;
		} else { /* A_RIGHT */
			rspa.vol_right = (int16_t)vol;
		}
	} else { /* A_RATE */
		if (flags & 0x02) { /* A_LEFT */
			rspa.tgt_left = (int16_t)vol;
			rspa.rate_left_m = (int16_t)voltgt;
			rspa.rate_left_l = volrate;
		} else { /* A_RIGHT */
			rspa.tgt_right = (int16_t)vol;
			rspa.rate_right_m = (int16_t)voltgt;
			rspa.rate_right_l = volrate;
		}
	}
}

/* ================================================================== */
/*  A_ENVMIXER (opcode 3) — N_AUDIO envelope mixer                    */
/*                                                                    */
/*  SSB64 builds with N_MICRO, whose compact command fields are       */
/*  relative offsets into the N_AUDIO ucode workspace.  The envmixer  */
/*  consumes the fixed main input buffer and accumulates into fixed   */
/*  dry/wet output buses.  Its persisted state is an 80-byte block:   */
/*  wet/dry amounts, left/right targets, left/right ramp steps, and   */
/*  left/right ramp values.                                           */
/* ================================================================== */
#define MIX_MAIN_L PORT_NAUDIO_DRY_LEFT
#define MIX_MAIN_R PORT_NAUDIO_DRY_RIGHT
#define MIX_AUX_L  PORT_NAUDIO_WET_LEFT
#define MIX_AUX_R  PORT_NAUDIO_WET_RIGHT

typedef struct {
	int32_t value;
	int32_t target;
	int32_t step;
} AudioRamp;

static inline int16_t ramp_step(AudioRamp *ramp) {
	int reached;

	ramp->value += ramp->step;
	reached = (ramp->step <= 0) ? (ramp->value <= ramp->target)
	                            : (ramp->value >= ramp->target);
	if (reached) {
		ramp->value = ramp->target;
		ramp->step = 0;
	}
	return (int16_t)(ramp->value >> 16);
}

static inline int32_t load_s32_from_s16(const int16_t *p) {
	int32_t v;
	memcpy(&v, p, sizeof(v));
	return v;
}

static inline void store_s32_to_s16(int16_t *p, int32_t v) {
	memcpy(p, &v, sizeof(v));
}

void aEnvMixerImpl(uint8_t flags, int16_t *state) {
	int16_t *in = BUF_S16(rspa.in);
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int count = nbytes / (int)sizeof(int16_t);
	int i;
	int16_t dry_amt, wet_amt;
	AudioRamp ramps[2];
	int16_t save[40];

	if (flags & 0x01) { /* A_INIT — load from SETVOL params */
		int32_t rate_left = ((int32_t)rspa.rate_left_m << 16) | rspa.rate_left_l;
		int32_t rate_right = ((int32_t)rspa.rate_right_m << 16) | rspa.rate_right_l;

		dry_amt = rspa.dry_amt;
		wet_amt = rspa.wet_amt;
		ramps[0].value = (int32_t)rspa.vol_left << 16;
		ramps[0].target = (int32_t)rspa.tgt_left << 16;
		ramps[0].step = rate_left / 8;
		ramps[1].value = (int32_t)rspa.vol_right << 16;
		ramps[1].target = (int32_t)rspa.tgt_right << 16;
		ramps[1].step = rate_right / 8;
	} else { /* A_CONTINUE — restore from ENVMIX_STATE */
		memcpy(save, state, sizeof(save));
		wet_amt = save[0];
		dry_amt = save[2];
		ramps[0].target = (int32_t)save[4] << 16;
		ramps[1].target = (int32_t)save[6] << 16;
		ramps[0].step = load_s32_from_s16(save + 8);
		ramps[1].step = load_s32_from_s16(save + 10);
		ramps[0].value = load_s32_from_s16(save + 16);
		ramps[1].value = load_s32_from_s16(save + 18);
	}

	for (i = 0; i < count; i++) {
		int16_t s = in[i];
		int16_t l_vol = ramp_step(&ramps[0]);
		int16_t r_vol = ramp_step(&ramps[1]);
		int16_t l_dry = clamp16(((int32_t)l_vol * dry_amt + 0x4000) >> 15);
		int16_t r_dry = clamp16(((int32_t)r_vol * dry_amt + 0x4000) >> 15);
		int16_t l_wet = clamp16(((int32_t)l_vol * wet_amt + 0x4000) >> 15);
		int16_t r_wet = clamp16(((int32_t)r_vol * wet_amt + 0x4000) >> 15);
		sample_add(MIX_MAIN_L, i, ((int32_t)s * l_dry) >> 15);
		sample_add(MIX_MAIN_R, i, ((int32_t)s * r_dry) >> 15);
		sample_add(MIX_AUX_L, i, ((int32_t)s * l_wet) >> 15);
		sample_add(MIX_AUX_R, i, ((int32_t)s * r_wet) >> 15);
	}

	memset(save, 0, sizeof(save));
	save[0] = wet_amt;
	save[2] = dry_amt;
	save[4] = (int16_t)(ramps[0].target >> 16);
	save[6] = (int16_t)(ramps[1].target >> 16);
	store_s32_to_s16(save + 8, ramps[0].step);
	store_s32_to_s16(save + 10, ramps[1].step);
	store_s32_to_s16(save + 16, ramps[0].value);
	store_s32_to_s16(save + 18, ramps[1].value);
	memcpy(state, save, sizeof(save));
}

/* ================================================================== */
/*  A_MIXER (opcode 12) — Gain-weighted mix                           */
/*  out = clamp(out + ((in * gain) >> 15))                            */
/*  Count from rspa.nbytes (set by prior A_SETBUFF).                  */
/* ================================================================== */

void aMixImpl(uint8_t flags, int16_t gain, uint16_t in_addr, uint16_t out_addr) {
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int32_t sample;
	int index = 0;

	(void)flags;

	if (gain == -0x8000) {
		/* Special case: subtract only */
		while (nbytes > 0) {
			int i;
			for (i = 0; i < 16 && nbytes > 0; i++) {
				sample = sample_read(out_addr, index) - sample_read(in_addr, index);
				sample_write(out_addr, index, sample);
				index++;
				nbytes -= sizeof(int16_t);
			}
		}
		return;
	}

	while (nbytes > 0) {
		int i;
		for (i = 0; i < 16 && nbytes > 0; i++) {
			sample = sample_read(out_addr, index) + ((sample_read(in_addr, index) * gain) >> 15);
			sample_write(out_addr, index, sample);
			index++;
			nbytes -= sizeof(int16_t);
		}
	}
}

/* ================================================================== */
/*  A_POLEF (opcode 14) — N_AUDIO pole / IIR filter                   */
/* ================================================================== */

static inline int32_t vmulf(int16_t x, int16_t y) {
	return (((int32_t)x * (int32_t)y) + 0x4000) >> 15;
}

static int32_t reverse_dot(int n, const int16_t *x, const int16_t *y) {
	int32_t acc = 0;
	y += n;
	while (n != 0) {
		acc += *x++ * *--y;
		n--;
	}
	return acc;
}

static void aPoleFilterPoleImpl(uint8_t flags, uint16_t gain, int16_t *state) {
	int16_t *src = BUF_S16(rspa.in);
	int16_t *dst = BUF_S16(rspa.out);
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int16_t *table = &rspa.adpcm_table[0][0][0];
	int16_t *h1 = table;
	int16_t *h2 = table + 8;
	int16_t h2_before[8];
	int16_t l1 = 0;
	int16_t l2 = 0;
	int i;

	if (!(flags & 0x01)) {
		l1 = state[2];
		l2 = state[3];
	}

	for (i = 0; i < 8; i++) {
		h2_before[i] = h2[i];
		h2[i] = (int16_t)(((int32_t)h2[i] * gain) >> 14);
	}

	while (nbytes > 0) {
		int16_t frame[8];
		for (i = 0; i < 8; i++) {
			frame[i] = src[i];
		}
		for (i = 0; i < 8; i++) {
			int32_t acc = (int32_t)frame[i] * gain;
			acc += (int32_t)h1[i] * l1 + (int32_t)h2_before[i] * l2;
			acc += reverse_dot(i, h2, frame);
			dst[i] = clamp16(acc >> 14);
		}
		l1 = dst[6];
		l2 = dst[7];
		src += 8;
		dst += 8;
		nbytes -= 16;
	}

	memcpy(state, dst - 4, 4 * sizeof(int16_t));
}

static void aPoleFilterIirImpl(uint8_t flags, int16_t *state) {
	int16_t *src = BUF_S16(rspa.in);
	int16_t *dst = BUF_S16(rspa.out);
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int16_t *table = &rspa.adpcm_table[0][0][0];
	int16_t frame[8] = {0};
	int16_t ibuf[4] = {0};
	uint16_t index = 7;
	int32_t prev;
	int i;

	if (!(flags & 0x01)) {
		frame[6] = state[2];
		frame[7] = state[3];
		ibuf[1] = state[4];
		ibuf[2] = state[5];
	}

	prev = vmulf(table[9], frame[6]) * 2;
	while (nbytes > 0) {
		for (i = 0; i < 8; i++) {
			int32_t acc;
			ibuf[index & 3] = *src++;
			acc = prev +
			      vmulf(table[0], ibuf[index & 3]) +
			      vmulf(table[1], ibuf[(index - 1) & 3]) +
			      vmulf(table[0], ibuf[(index - 2) & 3]);
			acc += vmulf(table[8], frame[index]) * 2;
			prev = vmulf(table[9], frame[index]) * 2;
			dst[i] = frame[i] = (int16_t)acc;
			index = (index + 1) & 7;
		}
		dst += 8;
		nbytes -= 16;
	}

	state[2] = frame[6];
	state[3] = frame[7];
	state[4] = ibuf[(index - 2) & 3];
	state[5] = ibuf[(index - 1) & 3];
}

void aPoleFilterImpl(uint8_t flags, uint16_t gain, int16_t *state) {
	int16_t *table = &rspa.adpcm_table[0][0][0];
	if (table[0] == 0 && table[1] == 0) {
		aPoleFilterPoleImpl(flags, gain, state);
	} else {
		aPoleFilterIirImpl(flags, state);
	}
}
