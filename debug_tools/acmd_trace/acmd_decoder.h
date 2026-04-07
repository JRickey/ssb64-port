/**
 * acmd_decoder.h — Shared N64 Audio Binary Interface (ABI) command decoder
 *
 * Decodes raw Acmd commands into structured text.
 * Used by both the port-side audio trace and the Mupen64Plus RSP trace plugin.
 *
 * All functions are pure C for portability.
 */
#ifndef ACMD_DECODER_H
#define ACMD_DECODER_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/*  Audio microcode opcodes (from PR/abi.h)                                  */
/* ========================================================================= */

#define ACMD_SPNOOP       0
#define ACMD_ADPCM        1
#define ACMD_CLEARBUFF    2
#define ACMD_ENVMIXER     3
#define ACMD_LOADBUFF     4
#define ACMD_RESAMPLE     5
#define ACMD_SAVEBUFF     6
#define ACMD_SEGMENT      7
#define ACMD_SETBUFF      8
#define ACMD_SETVOL       9
#define ACMD_DMEMMOVE    10
#define ACMD_LOADADPCM   11
#define ACMD_MIXER       12
#define ACMD_INTERLEAVE  13
#define ACMD_POLEF       14
#define ACMD_SETLOOP     15

#define ACMD_OPCODE_COUNT 16

/* ========================================================================= */
/*  Audio command flags (from PR/abi.h)                                      */
/* ========================================================================= */

#define ACMD_F_INIT       0x01
#define ACMD_F_LOOP       0x02
#define ACMD_F_OUT        0x02
#define ACMD_F_LEFT       0x02
#define ACMD_F_VOL        0x04
#define ACMD_F_AUX        0x08
#define ACMD_F_MIX        0x10

/* ========================================================================= */
/*  Decoder API                                                              */
/* ========================================================================= */

/**
 * Return the human-readable name for an audio opcode (0-15).
 * Returns "A_UNKNOWN_XX" for unrecognized opcodes.
 */
const char *acmd_opcode_name(uint8_t opcode);

/**
 * Decode audio command flags into a human-readable string.
 * Writes into buf (up to buf_size bytes). Returns buf.
 */
char *acmd_decode_flags(uint8_t flags, char *buf, int buf_size);

/**
 * Decode a single Acmd (w0, w1) into a human-readable line.
 * Writes into buf (up to buf_size bytes). Returns number of chars written.
 *
 * Output format:
 *   A_OPCODE_NAME     w0=XXXXXXXX w1=XXXXXXXX  decoded params...
 */
int acmd_decode_cmd(uint32_t w0, uint32_t w1, char *buf, int buf_size);

/**
 * Returns nonzero if the opcode bears a DRAM address in w1
 * (useful for --ignore-addresses filtering in the diff tool).
 */
int acmd_is_address_opcode(uint8_t opcode);

#ifdef __cplusplus
}
#endif

#endif /* ACMD_DECODER_H */
