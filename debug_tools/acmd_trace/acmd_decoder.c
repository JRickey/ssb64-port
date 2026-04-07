/**
 * acmd_decoder.c — Shared N64 Audio Binary Interface (ABI) command decoder
 *
 * Pure C, no dependencies beyond libc. Used by both the port-side
 * audio trace and the Mupen64Plus RSP trace plugin.
 *
 * Acmd format (8 bytes per command):
 *   w0: opcode (bits 31-24) + flags/parameters (bits 23-0)
 *   w1: address or packed parameters
 *
 * Field layouts derived from the Acmd union in PR/abi.h.
 */
#include "acmd_decoder.h"
#include <string.h>

/* ========================================================================= */
/*  Opcode name table                                                        */
/* ========================================================================= */

static const char *sOpcodeNames[ACMD_OPCODE_COUNT] = {
	"A_SPNOOP",       /*  0 */
	"A_ADPCM",        /*  1 */
	"A_CLEARBUFF",    /*  2 */
	"A_ENVMIXER",     /*  3 */
	"A_LOADBUFF",     /*  4 */
	"A_RESAMPLE",     /*  5 */
	"A_SAVEBUFF",     /*  6 */
	"A_SEGMENT",      /*  7 */
	"A_SETBUFF",      /*  8 */
	"A_SETVOL",       /*  9 */
	"A_DMEMMOVE",     /* 10 */
	"A_LOADADPCM",   /* 11 */
	"A_MIXER",        /* 12 */
	"A_INTERLEAVE",   /* 13 */
	"A_POLEF",        /* 14 */
	"A_SETLOOP",      /* 15 */
};

/* ========================================================================= */
/*  Public helpers                                                           */
/* ========================================================================= */

const char *acmd_opcode_name(uint8_t opcode)
{
	static char unknown[16];
	if (opcode < ACMD_OPCODE_COUNT) return sOpcodeNames[opcode];
	snprintf(unknown, sizeof(unknown), "A_UNKNOWN_%02X", opcode);
	return unknown;
}

char *acmd_decode_flags(uint8_t flags, char *buf, int buf_size)
{
	int pos = 0;
	buf[0] = '\0';

#define APPEND_FLAG(mask, name) do { \
	if (flags & (mask)) { \
		pos += snprintf(buf + pos, buf_size - pos, "%s%s", pos ? "|" : "", name); \
	} \
} while(0)

	APPEND_FLAG(ACMD_F_INIT, "INIT");
	APPEND_FLAG(ACMD_F_LOOP, "LOOP");  /* also OUT, LEFT — context-dependent */
	APPEND_FLAG(ACMD_F_VOL,  "VOL");
	APPEND_FLAG(ACMD_F_AUX,  "AUX");
	APPEND_FLAG(ACMD_F_MIX,  "MIX");

#undef APPEND_FLAG

	if (pos == 0) snprintf(buf, buf_size, "NONE");
	return buf;
}

int acmd_is_address_opcode(uint8_t opcode)
{
	switch (opcode) {
	case ACMD_ADPCM:
	case ACMD_LOADBUFF:
	case ACMD_SAVEBUFF:
	case ACMD_LOADADPCM:
	case ACMD_SETLOOP:
	case ACMD_ENVMIXER:   /* w1 = ENVMIX_STATE address */
	case ACMD_POLEF:      /* w1 = POLEF_STATE address */
	case ACMD_RESAMPLE:   /* w1 = RESAMPLE_STATE address */
	case ACMD_SEGMENT:    /* w1 = segment base address */
		return 1;
	default:
		return 0;
	}
}

/* ========================================================================= */
/*  Command decoder                                                          */
/* ========================================================================= */

int acmd_decode_cmd(uint32_t w0, uint32_t w1, char *buf, int buf_size)
{
	uint8_t opcode = (uint8_t)(w0 >> 24);
	const char *name = acmd_opcode_name(opcode);
	int n = 0;

	/* Common prefix: opcode name + raw words */
	n = snprintf(buf, buf_size, "%-16s w0=%08X w1=%08X", name, w0, w1);
	if (n >= buf_size) return n;

	/* Per-opcode parameter decoding */
	switch (opcode) {

	case ACMD_SPNOOP:
		/* No parameters */
		break;

	case ACMD_ADPCM: {
		/* Aadpcm: cmd:8, flags:8, gain:16, addr:32 */
		uint8_t  flags = (w0 >> 16) & 0xFF;
		uint16_t gain  = w0 & 0xFFFF;
		char fbuf[64];
		acmd_decode_flags(flags, fbuf, sizeof(fbuf));
		n += snprintf(buf + n, buf_size - n, "  flags=%s gain=%u state=%08X",
		              fbuf, gain, w1);
		break;
	}

	case ACMD_CLEARBUFF: {
		/* Aclearbuff: cmd:8, pad:8, dmem:16, pad:16, count:16 */
		uint16_t dmem  = w0 & 0xFFFF;
		uint16_t count = w1 & 0xFFFF;
		n += snprintf(buf + n, buf_size - n, "  dmem=0x%04X count=%u", dmem, count);
		break;
	}

	case ACMD_ENVMIXER: {
		/* Aenvmixer: cmd:8, flags:8, pad:16, addr:32 */
		uint8_t flags = (w0 >> 16) & 0xFF;
		char fbuf[64];
		acmd_decode_flags(flags, fbuf, sizeof(fbuf));
		n += snprintf(buf + n, buf_size - n, "  flags=%s state=%08X", fbuf, w1);
		break;
	}

	case ACMD_LOADBUFF: {
		/* Aloadbuff: cmd:8, pad:24, addr:32 */
		n += snprintf(buf + n, buf_size - n, "  dram=%08X", w1);
		break;
	}

	case ACMD_RESAMPLE: {
		/* Aresample: cmd:8, flags:8, pitch:16, addr:32 */
		uint8_t  flags = (w0 >> 16) & 0xFF;
		uint16_t pitch = w0 & 0xFFFF;
		char fbuf[64];
		acmd_decode_flags(flags, fbuf, sizeof(fbuf));
		n += snprintf(buf + n, buf_size - n, "  flags=%s pitch=0x%04X(%.4f) state=%08X",
		              fbuf, pitch, (double)pitch / 0x8000, w1);
		break;
	}

	case ACMD_SAVEBUFF: {
		/* Asavebuff: cmd:8, pad:24, addr:32 */
		n += snprintf(buf + n, buf_size - n, "  dram=%08X", w1);
		break;
	}

	case ACMD_SEGMENT: {
		/* Asegment: cmd:8, pad:24, pad:2, number:4, base:24 */
		/* Note: w1 layout = [pad:2][seg:4][base:24] but the macro packs as
		 * _SHIFTL(s,24,8) | _SHIFTL(b,0,24), so seg is in bits 27-24 of w1 */
		uint8_t  seg  = (w1 >> 24) & 0x0F;
		uint32_t base = w1 & 0x00FFFFFF;
		n += snprintf(buf + n, buf_size - n, "  seg=%u base=0x%06X", seg, base);
		break;
	}

	case ACMD_SETBUFF: {
		/* Asetbuff: cmd:8, flags:8, dmemin:16, dmemout:16, count:16 */
		uint8_t  flags   = (w0 >> 16) & 0xFF;
		uint16_t dmemin  = w0 & 0xFFFF;
		uint16_t dmemout = (w1 >> 16) & 0xFFFF;
		uint16_t count   = w1 & 0xFFFF;
		char fbuf[64];
		acmd_decode_flags(flags, fbuf, sizeof(fbuf));
		n += snprintf(buf + n, buf_size - n, "  flags=%s in=0x%04X out=0x%04X count=%u",
		              fbuf, dmemin, dmemout, count);
		break;
	}

	case ACMD_SETVOL: {
		/* Asetvol: cmd:8, flags:8, vol:16, voltgt:16, volrate:16 */
		uint8_t  flags   = (w0 >> 16) & 0xFF;
		uint16_t vol     = w0 & 0xFFFF;
		uint16_t voltgt  = (w1 >> 16) & 0xFFFF;
		uint16_t volrate = w1 & 0xFFFF;
		char fbuf[64];
		acmd_decode_flags(flags, fbuf, sizeof(fbuf));
		n += snprintf(buf + n, buf_size - n, "  flags=%s vol=%u tgt=%u rate=%u",
		              fbuf, vol, voltgt, volrate);
		break;
	}

	case ACMD_DMEMMOVE: {
		/* Admemmove: cmd:8, pad:8, dmemin:16, dmemout:16, count:16 */
		uint16_t dmemin  = w0 & 0xFFFF;
		uint16_t dmemout = (w1 >> 16) & 0xFFFF;
		uint16_t count   = w1 & 0xFFFF;
		n += snprintf(buf + n, buf_size - n, "  in=0x%04X out=0x%04X count=%u",
		              dmemin, dmemout, count);
		break;
	}

	case ACMD_LOADADPCM: {
		/* Aloadadpcm: cmd:8, pad:8, count:16, addr:32 */
		uint16_t count = w0 & 0xFFFF;
		n += snprintf(buf + n, buf_size - n, "  count=%u dram=%08X", count, w1);
		break;
	}

	case ACMD_MIXER: {
		/* Amixer: cmd:8, flags:8, gain:16, dmemi:16, dmemo:16 */
		uint8_t  flags = (w0 >> 16) & 0xFF;
		int16_t  gain  = (int16_t)(w0 & 0xFFFF);
		uint16_t dmemi = (w1 >> 16) & 0xFFFF;
		uint16_t dmemo = w1 & 0xFFFF;
		n += snprintf(buf + n, buf_size - n, "  flags=0x%02X gain=%d in=0x%04X out=0x%04X",
		              flags, gain, dmemi, dmemo);
		break;
	}

	case ACMD_INTERLEAVE: {
		/* Ainterleave: cmd:8, pad:8, pad:16, inL:16, inR:16 */
		uint16_t inL = (w1 >> 16) & 0xFFFF;
		uint16_t inR = w1 & 0xFFFF;
		n += snprintf(buf + n, buf_size - n, "  inL=0x%04X inR=0x%04X", inL, inR);
		break;
	}

	case ACMD_POLEF: {
		/* Apolef: cmd:8, flags:8, gain:16, addr:32 */
		uint8_t  flags = (w0 >> 16) & 0xFF;
		uint16_t gain  = w0 & 0xFFFF;
		char fbuf[64];
		acmd_decode_flags(flags, fbuf, sizeof(fbuf));
		n += snprintf(buf + n, buf_size - n, "  flags=%s gain=%u state=%08X",
		              fbuf, gain, w1);
		break;
	}

	case ACMD_SETLOOP: {
		/* Asetloop: cmd:8, pad:24, addr:32 */
		n += snprintf(buf + n, buf_size - n, "  loop=%08X", w1);
		break;
	}

	default:
		/* Unknown opcode — raw words already printed */
		break;
	}

	return n;
}
