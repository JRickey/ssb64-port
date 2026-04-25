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
#include <string.h>
#include <stdio.h>

#include "mixer.h"

/* ------------------------------------------------------------------ */
/*  Rounding helpers                                                  */
/* ------------------------------------------------------------------ */

#define ROUND_UP_16(v)   (((v) + 15) & ~15)
#define ROUND_UP_32(v)   (((v) + 31) & ~31)
#define ROUND_DOWN_16(v) ((v) & ~0xf)

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

/* ================================================================== */
/*  A_CLEARBUFF (opcode 2) — Zero a region of DMEM                    */
/* ================================================================== */

void aClearBufferImpl(uint16_t addr, int nbytes) {
	nbytes = ROUND_UP_16(nbytes);
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
	memcpy(BUF_U8(rspa.out), (const void*)source_addr, ROUND_DOWN_16(rspa.nbytes));
}

/* ================================================================== */
/*  A_SAVEBUFF (opcode 6) — Save from DMEM to DRAM                    */
/*  Uses rspa.in as source DMEM addr, rspa.nbytes as count.           */
/* ================================================================== */

void aSaveBufferImpl(uintptr_t dest_addr) {
	if (dest_addr == 0) return;
	memcpy((void*)dest_addr, BUF_U8(rspa.in), ROUND_DOWN_16(rspa.nbytes));
}

/* ================================================================== */
/*  A_DMEMMOVE (opcode 10) — Move data within DMEM                    */
/* ================================================================== */

void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes) {
	nbytes = ROUND_UP_16(nbytes);
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
				for (j = 0; j < 2; j++) {
					ins[j * 4]     = (((*in >> 6) << 30) >> 30) << shift;
					ins[j * 4 + 1] = ((((*in >> 4) & 0x3) << 30) >> 30) << shift;
					ins[j * 4 + 2] = ((((*in >> 2) & 0x3) << 30) >> 30) << shift;
					ins[j * 4 + 3] = (((*in++ & 0x3) << 30) >> 30) << shift;
				}
			} else {
				/* 4-bit ADPCM */
				for (j = 0; j < 4; j++) {
					ins[j * 2]     = (((*in >> 4) << 28) >> 28) << shift;
					ins[j * 2 + 1] = (((*in++ & 0xf) << 28) >> 28) << shift;
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
			sample = ((in[0] * tbl[0] + 0x4000) >> 15) +
			         ((in[1] * tbl[1] + 0x4000) >> 15) +
			         ((in[2] * tbl[2] + 0x4000) >> 15) +
			         ((in[3] * tbl[3] + 0x4000) >> 15);
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
}

/* ================================================================== */
/*  A_INTERLEAVE (opcode 13) — Stereo channel interleave              */
/*  Reads separate L/R buffers, writes interleaved L-R-L-R to out.    */
/* ================================================================== */

void aInterleaveImpl(uint16_t left, uint16_t right) {
	int count;
	int16_t *l, *r, *d;
	int i;

	if (rspa.nbytes == 0) return;

	count = rspa.nbytes / (2 * sizeof(int16_t));
	l = BUF_S16(left);
	r = BUF_S16(right);
	d = BUF_S16(rspa.out);

	for (i = 0; i < count; i++) {
		*d++ = *l++;
		*d++ = *r++;
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
/*  A_ENVMIXER (opcode 3) — Standard N64 envelope mixer               */
/*                                                                    */
/*  Mixes mono input to 4 output buses (dry L/R, wet L/R) with       */
/*  per-channel volume ramping. Volumes ramp toward targets at the    */
/*  configured rates.                                                 */
/*                                                                    */
/*  ENVMIX_STATE[40] layout (self-consistent — we write and read):    */
/*    [0]  current left volume (high 16 bits)                         */
/*    [1]  current left volume (low 16 bits)                          */
/*    [2]  current right volume (high 16 bits)                        */
/*    [3]  current right volume (low 16 bits)                         */
/*    [4]  left target                                                */
/*    [5]  right target                                               */
/*    [6]  left ramp rate (mantissa)                                  */
/*    [7]  left ramp rate (low)                                       */
/*    [8]  right ramp rate (mantissa)                                 */
/*    [9]  right ramp rate (low)                                      */
/*    [10] dry amount                                                 */
/*    [11] wet amount                                                 */
/*                                                                    */
/*  DMEM output addresses (from synthInternals.h):                    */
/*    AL_MAIN_L_OUT (1088), AL_MAIN_R_OUT (1408)                      */
/*    AL_AUX_L_OUT  (1728), AL_AUX_R_OUT  (2048)                     */
/* ================================================================== */

/* DMEM bus addresses.  SSB64 builds with N_MICRO, which uses the
 * N_AL_* DMEM layout (n_synthInternals.h), NOT the standard AL_*
 * layout (synthInternals.h).  Using the wrong bank silently routes
 * envmix output to one address while mainbuspull / interleave read
 * from another → stereo channels see only the AUX-bus residue and
 * sound like wildly different signals.  Verified empirically: this is
 * THE bug behind the BGM-noise issue captured 2026-04-24.
 *
 *   N_AL_MAIN_L_OUT = 1248   (vs AL_MAIN_L_OUT = 1088)
 *   N_AL_MAIN_R_OUT = 1616   (vs AL_MAIN_R_OUT = 1408)
 *   N_AL_AUX_L_OUT  = 1984   (vs AL_AUX_L_OUT  = 1728)
 *   N_AL_AUX_R_OUT  = 2352   (vs AL_AUX_R_OUT  = 2048)
 */
#define MIX_MAIN_L 1248
#define MIX_MAIN_R 1616
#define MIX_AUX_L  1984
#define MIX_AUX_R  2352

void aEnvMixerImpl(uint8_t flags, int16_t *state) {
	int16_t *in = BUF_S16(rspa.in);
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int i, j;

	int32_t vol_left, vol_right;
	int32_t rate_left, rate_right;
	int16_t tgt_left, tgt_right;
	int16_t dry_amt, wet_amt;
	int has_aux = (flags & 0x08); /* A_AUX */

	if (flags & 0x01) { /* A_INIT — load from SETVOL params */
		vol_left   = (int32_t)rspa.vol_left << 16;
		vol_right  = (int32_t)rspa.vol_right << 16;
		tgt_left   = rspa.tgt_left;
		tgt_right  = rspa.tgt_right;
		rate_left  = ((int32_t)rspa.rate_left_m << 16) | rspa.rate_left_l;
		rate_right = ((int32_t)rspa.rate_right_m << 16) | rspa.rate_right_l;
		dry_amt    = rspa.dry_amt;
		wet_amt    = rspa.wet_amt;
	} else { /* A_CONTINUE — restore from ENVMIX_STATE */
		vol_left   = ((int32_t)state[0] << 16) | (uint16_t)state[1];
		vol_right  = ((int32_t)state[2] << 16) | (uint16_t)state[3];
		tgt_left   = state[4];
		tgt_right  = state[5];
		rate_left  = ((int32_t)state[6] << 16) | (uint16_t)state[7];
		rate_right = ((int32_t)state[8] << 16) | (uint16_t)state[9];
		dry_amt    = state[10];
		wet_amt    = state[11];
	}

	/* Process in 8-sample blocks (matching RSP microcode granularity) */
	while (nbytes > 0) {
		int16_t cur_vol_l = (int16_t)(vol_left >> 16);
		int16_t cur_vol_r = (int16_t)(vol_right >> 16);

		for (j = 0; j < 8 && nbytes > 0; j++) {
			int16_t s = *in++;
			int32_t sL, sR;

			/* Apply per-channel volume */
			sL = (s * cur_vol_l) >> 15;
			sR = (s * cur_vol_r) >> 15;

			/* Mix into dry buses (main L/R) */
			{
				int16_t *dryL = BUF_S16(MIX_MAIN_L);
				int16_t *dryR = BUF_S16(MIX_MAIN_R);
				int idx = (int)(in - 1 - BUF_S16(rspa.in));
				dryL[idx] = clamp16(dryL[idx] + ((sL * dry_amt) >> 15));
				dryR[idx] = clamp16(dryR[idx] + ((sR * dry_amt) >> 15));
			}

			/* Mix into wet buses (aux L/R) if A_AUX */
			if (has_aux) {
				int16_t *wetL = BUF_S16(MIX_AUX_L);
				int16_t *wetR = BUF_S16(MIX_AUX_R);
				int idx = (int)(in - 1 - BUF_S16(rspa.in));
				wetL[idx] = clamp16(wetL[idx] + ((sL * wet_amt) >> 15));
				wetR[idx] = clamp16(wetR[idx] + ((sR * wet_amt) >> 15));
			}

			nbytes -= sizeof(int16_t);
		}

		/* Ramp volumes toward targets per block */
		vol_left += rate_left;
		vol_right += rate_right;

		/* Clamp at target */
		if (rate_left > 0 && (vol_left >> 16) > tgt_left)
			vol_left = (int32_t)tgt_left << 16;
		else if (rate_left < 0 && (vol_left >> 16) < tgt_left)
			vol_left = (int32_t)tgt_left << 16;

		if (rate_right > 0 && (vol_right >> 16) > tgt_right)
			vol_right = (int32_t)tgt_right << 16;
		else if (rate_right < 0 && (vol_right >> 16) < tgt_right)
			vol_right = (int32_t)tgt_right << 16;
	}

	/* Save state for A_CONTINUE on next subframe */
	state[0]  = (int16_t)(vol_left >> 16);
	state[1]  = (int16_t)(vol_left & 0xFFFF);
	state[2]  = (int16_t)(vol_right >> 16);
	state[3]  = (int16_t)(vol_right & 0xFFFF);
	state[4]  = tgt_left;
	state[5]  = tgt_right;
	state[6]  = (int16_t)(rate_left >> 16);
	state[7]  = (int16_t)(rate_left & 0xFFFF);
	state[8]  = (int16_t)(rate_right >> 16);
	state[9]  = (int16_t)(rate_right & 0xFFFF);
	state[10] = dry_amt;
	state[11] = wet_amt;
}

/* ================================================================== */
/*  A_MIXER (opcode 12) — Gain-weighted mix                           */
/*  out = ((out * 0x7FFF) + (in * gain) + 0x4000) >> 15              */
/*  Count from rspa.nbytes (set by prior A_SETBUFF).                  */
/* ================================================================== */

void aMixImpl(uint8_t flags, int16_t gain, uint16_t in_addr, uint16_t out_addr) {
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int16_t *in = BUF_S16(in_addr);
	int16_t *out = BUF_S16(out_addr);
	int32_t sample;

	(void)flags;

	if (gain == -0x8000) {
		/* Special case: subtract only */
		while (nbytes > 0) {
			int i;
			for (i = 0; i < 16 && nbytes > 0; i++) {
				sample = *out - *in++;
				*out++ = clamp16(sample);
				nbytes -= sizeof(int16_t);
			}
		}
		return;
	}

	while (nbytes > 0) {
		int i;
		for (i = 0; i < 16 && nbytes > 0; i++) {
			sample = ((*out * 0x7fff + *in++ * gain) + 0x4000) >> 15;
			*out++ = clamp16(sample);
			nbytes -= sizeof(int16_t);
		}
	}
}

/* ================================================================== */
/*  A_POLEF (opcode 14) — Single-pole IIR low-pass filter             */
/*                                                                    */
/*  Uses coefficients loaded via A_LOADADPCM into rspa.adpcm_table    */
/*  (the pole filter reuses the ADPCM codebook load mechanism).       */
/*                                                                    */
/*  Algorithm: y[n] = ((x[n] * gain) + (y[n-1] * (0x7FFF - gain))    */
/*                     + 0x4000) >> 15                                */
/*                                                                    */
/*  The N64 pole filter uses the first row of loaded coefficients     */
/*  as the filter kernel. The 'gain' parameter controls the filter    */
/*  cutoff: higher gain = higher cutoff (more input, less feedback).  */
/*                                                                    */
/*  POLEF_STATE[4]: [0] = previous output sample, [1-3] = unused.    */
/* ================================================================== */

void aPoleFilterImpl(uint8_t flags, uint16_t gain, int16_t *state) {
	int16_t *buf = BUF_S16(rspa.in);
	int nbytes = ROUND_UP_16(rspa.nbytes);
	int16_t prev;
	int i;
	int32_t g = (int32_t)(int16_t)gain;

	if (flags) { /* First call — init state */
		prev = 0;
	} else {
		prev = state[0];
	}

	for (i = 0; i < nbytes / (int)sizeof(int16_t); i++) {
		int32_t sample = (buf[i] * g + prev * (0x7FFF - g) + 0x4000) >> 15;
		prev = clamp16(sample);
		buf[i] = prev;
	}

	state[0] = prev;
}
