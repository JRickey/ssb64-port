/**
 * gbi_decoder.h — Shared F3DEX2 / RDP display list decoder
 *
 * Decodes raw N64 GBI commands into structured text.
 * Used by both the port-side trace hook and the Mupen64Plus trace plugin.
 *
 * All functions are pure C for portability.
 */
#ifndef GBI_DECODER_H
#define GBI_DECODER_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/*  F3DEX2 opcodes                                                           */
/* ========================================================================= */

/* RSP geometry commands */
#define GBI_G_NOOP            0x00
#define GBI_G_VTX             0x01
#define GBI_G_MODIFYVTX       0x02
#define GBI_G_CULLDL          0x03
#define GBI_G_BRANCH_Z        0x04
#define GBI_G_TRI1            0x05
#define GBI_G_TRI2            0x06
#define GBI_G_QUAD            0x07
#define GBI_G_LINE3D          0x08

/* RSP immediate commands */
#define GBI_G_SPNOOP          0xE0
#define GBI_G_RDPHALF_1       0xE1
#define GBI_G_SETOTHERMODE_L  0xE2
#define GBI_G_SETOTHERMODE_H  0xE3
#define GBI_G_DL              0xDE
#define GBI_G_ENDDL           0xDF
#define GBI_G_LOAD_UCODE      0xDD
#define GBI_G_MOVEMEM         0xDC
#define GBI_G_MOVEWORD        0xDB
#define GBI_G_MTX             0xDA
#define GBI_G_GEOMETRYMODE    0xD9
#define GBI_G_POPMTX          0xD8
#define GBI_G_TEXTURE         0xD7
#define GBI_G_DMA_IO          0xD6
#define GBI_G_SPECIAL_1       0xD5
#define GBI_G_SPECIAL_2       0xD4
#define GBI_G_SPECIAL_3       0xD3

/* RDP commands */
#define GBI_G_SETCIMG         0xFF
#define GBI_G_SETZIMG         0xFE
#define GBI_G_SETTIMG         0xFD
#define GBI_G_SETCOMBINE      0xFC
#define GBI_G_SETENVCOLOR     0xFB
#define GBI_G_SETPRIMCOLOR    0xFA
#define GBI_G_SETBLENDCOLOR   0xF9
#define GBI_G_SETFOGCOLOR     0xF8
#define GBI_G_SETFILLCOLOR    0xF7
#define GBI_G_FILLRECT        0xF6
#define GBI_G_SETTILE         0xF5
#define GBI_G_LOADTILE        0xF4
#define GBI_G_LOADBLOCK       0xF3
#define GBI_G_SETTILESIZE     0xF2
#define GBI_G_RDPHALF_2       0xF1
#define GBI_G_LOADTLUT        0xF0
#define GBI_G_RDPSETOTHERMODE 0xEF
#define GBI_G_SETPRIMDEPTH    0xEE
#define GBI_G_SETSCISSOR      0xED
#define GBI_G_SETCONVERT      0xEC
#define GBI_G_SETKEYR         0xEB
#define GBI_G_SETKEYGB        0xEA
#define GBI_G_RDPFULLSYNC     0xE9
#define GBI_G_RDPTILESYNC     0xE8
#define GBI_G_RDPPIPESYNC     0xE7
#define GBI_G_RDPLOADSYNC     0xE6
#define GBI_G_TEXRECTFLIP     0xE5
#define GBI_G_TEXRECT         0xE4

/* ========================================================================= */
/*  Image format names                                                       */
/* ========================================================================= */

#define GBI_IM_FMT_RGBA  0
#define GBI_IM_FMT_YUV   1
#define GBI_IM_FMT_CI    2
#define GBI_IM_FMT_IA    3
#define GBI_IM_FMT_I     4

/* ========================================================================= */
/*  Geometry mode flags                                                      */
/* ========================================================================= */

#define GBI_G_ZBUFFER             0x00000001
#define GBI_G_SHADE               0x00000004
#define GBI_G_CULL_FRONT          0x00000200
#define GBI_G_CULL_BACK           0x00000400
#define GBI_G_FOG                 0x00010000
#define GBI_G_LIGHTING            0x00020000
#define GBI_G_TEXTURE_GEN         0x00040000
#define GBI_G_TEXTURE_GEN_LINEAR  0x00080000
#define GBI_G_SHADING_SMOOTH      0x00200000
#define GBI_G_CLIPPING            0x00800000

/* ========================================================================= */
/*  Decoder API                                                              */
/* ========================================================================= */

/**
 * Return the human-readable name for an F3DEX2/RDP opcode.
 * Returns "UNKNOWN_XX" for unrecognized opcodes.
 */
const char *gbi_opcode_name(uint8_t opcode);

/**
 * Return the human-readable name for an image format (0-4).
 */
const char *gbi_img_fmt_name(uint32_t fmt);

/**
 * Return the pixel size string for a siz value (0-3).
 */
const char *gbi_img_siz_name(uint32_t siz);

/**
 * Decode geometry mode flags into a human-readable string.
 * Writes into buf (up to buf_size bytes). Returns buf.
 */
char *gbi_decode_geom_mode(uint32_t mode, char *buf, int buf_size);

/**
 * Decode a single GBI command (w0, w1) into a human-readable line.
 * Writes into buf (up to buf_size bytes). Returns number of chars written.
 *
 * The output format is:
 *   OPCODE_NAME      w0=XXXXXXXX w1=XXXXXXXX  decoded params...
 *
 * For the port side (64-bit words), pass the lower 32 bits of each word.
 * The raw 64-bit values can be logged separately if needed.
 */
int gbi_decode_cmd(uint32_t w0, uint32_t w1, char *buf, int buf_size);

/**
 * Returns nonzero if the opcode is G_DL (display list call/branch).
 */
int gbi_is_dl_call(uint8_t opcode);

/**
 * Returns nonzero if the opcode is G_ENDDL.
 */
int gbi_is_dl_end(uint8_t opcode);

/**
 * For a G_DL command, returns nonzero if it's a branch (no return)
 * vs a call (pushes return address).
 * In F3DEX2: w0 bit 0 == 1 means branch, 0 means call (push).
 */
int gbi_dl_is_branch(uint32_t w0);

#ifdef __cplusplus
}
#endif

#endif /* GBI_DECODER_H */
