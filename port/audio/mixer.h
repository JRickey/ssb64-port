/*
 * SPDX-License-Identifier: MIT
 *
 * Portions of this file are derived from the Starship (Star Fox 64) PC port
 *   Copyright (c) The Harbour Masters
 *   https://github.com/HarbourMasters/Starship
 * Licensed under the MIT License; see LICENSE at repository root.
 */

/**
 * mixer.h — CPU-side Acmd audio interpreter for SSB64 PC port
 *
 * Replaces N64 RSP audio microcode with CPU implementations.
 * Standard N64 ABI (opcodes 0-15), stereo only.
 *
 * Strategy: macro replacement — the pull chain (n_alAudioFrame / n_env.c)
 * calls GBI-style macros (aSetBuffer, aADPCMdec, etc.) that normally build
 * an Acmd command list for RSP submission. We #undef those macros and
 * redefine them to call CPU functions directly. The 'pkt' parameter
 * (command list pointer) is evaluated but discarded.
 *
 * Reference: Starship (Star Fox 64) PC port — src/audio/mixer.c
 */

#ifndef PORT_AUDIO_MIXER_H
#define PORT_AUDIO_MIXER_H

#ifdef PORT

#include <stdint.h>

/* Pull in the state typedefs (ADPCM_STATE, RESAMPLE_STATE, etc.)
 * These are defined in abi.h which is included before this header. */

/* Acmd trace: log each command we execute so traces can be diffed
 * against the N64 emulator.  acmd_trace_log_cmd() is a no-op when
 * tracing is disabled, so this adds no overhead in normal builds. */
#include "acmd_trace/acmd_trace.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  Function declarations                                             */
/* ------------------------------------------------------------------ */

void aClearBufferImpl(uint16_t addr, int nbytes);
void aLoadBufferImpl(uintptr_t source_addr);
void aSaveBufferImpl(uintptr_t dest_addr);
void aLoadADPCMImpl(int count, uintptr_t book_addr);
void aSetBufferImpl(uint8_t flags, uint16_t in, uint16_t out, uint16_t nbytes);
void aInterleaveImpl(uint16_t left, uint16_t right);
void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes);
void aSetLoopImpl(uintptr_t adpcm_loop_state);
void aADPCMdecImpl(uint8_t flags, int16_t *state);
void aResampleImpl(uint8_t flags, uint16_t pitch, int16_t *state);
void aSetVolumeImpl(uint16_t flags, uint16_t vol, uint16_t voltgt, uint16_t volrate);
void aEnvMixerImpl(uint8_t flags, int16_t *state);
void aMixImpl(uint8_t flags, int16_t gain, uint16_t in_addr, uint16_t out_addr);
void aPoleFilterImpl(uint8_t flags, uint16_t gain, int16_t *state);

/* SSB64 uses the compact N_AUDIO ucode.  In that ABI, the buffer fields
 * emitted by n_abi.h are offsets relative to the ucode's internal main
 * workspace, not absolute DMEM addresses.  These constants match the N_AUDIO
 * interpreter used by mupen64plus-rsp-hle.
 */
#define PORT_NAUDIO_COUNT     0x170
#define PORT_NAUDIO_MAIN      0x4f0
#define PORT_NAUDIO_MAIN2     0x660
#define PORT_NAUDIO_DRY_LEFT  0x9d0
#define PORT_NAUDIO_DRY_RIGHT 0xb40
#define PORT_NAUDIO_WET_LEFT  0xcb0
#define PORT_NAUDIO_WET_RIGHT 0xe20

#ifdef N_MICRO
#define PORT_AUDIO_DMEM(a) ((uint16_t)(PORT_NAUDIO_MAIN + (uint16_t)(a)))
#define PORT_AUDIO_PREP_MIX() aSetBufferImpl(0, 0, 0, PORT_NAUDIO_COUNT)
#else
#define PORT_AUDIO_DMEM(a) ((uint16_t)(a))
#define PORT_AUDIO_PREP_MIX() ((void)0)
#endif

/* ------------------------------------------------------------------ */
/*  Undef standard N64 ABI macros (from include/PR/abi.h)             */
/* ------------------------------------------------------------------ */

#undef aADPCMdec
#undef aPoleFilter
#undef aClearBuffer
#undef aEnvMixer
#undef aInterleave
#undef aLoadBuffer
#undef aMix
#undef aPan
#undef aResample
#undef aSaveBuffer
#undef aSegment
#undef aSetBuffer
#undef aSetVolume
#undef aSetLoop
#undef aDMEMMove
#undef aLoadADPCM

/* ------------------------------------------------------------------ */
/*  Macro replacements — call CPU functions, discard pkt              */
/*                                                                    */
/*  State pointers arrive through osVirtualToPhysical() which returns */
/*  uintptr_t on PORT. We cast back to the appropriate pointer type.  */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Trace-logging helpers: encode the Acmd w0/w1 exactly as the N64   */
/*  ABI macros do (see include/PR/abi.h), then call the CPU impl.     */
/*  _SHIFTL and A_* constants are already visible from abi.h.         */
/* ------------------------------------------------------------------ */

#define aSegment(pkt, s, b) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_SEGMENT, 24, 8), \
	                        _SHIFTL(s, 24, 8) | _SHIFTL(b, 0, 24)); \
	} while (0)

#define aClearBuffer(pkt, d, c) \
	do { acmd_trace_log_cmd(_SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24), \
	                        (uint32_t)(c)); \
	     aClearBufferImpl(PORT_AUDIO_DMEM(d), c); \
	} while (0)

#define aSetBuffer(pkt, f, i, o, c) \
	do { acmd_trace_log_cmd(_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16), \
	                        _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16)); \
	     aSetBufferImpl(f, i, o, c); \
	} while (0)

#define aLoadBuffer(pkt, s) \
	do { acmd_trace_log_cmd(_SHIFTL(A_LOADBUFF, 24, 8), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aLoadBufferImpl((uintptr_t)(s)); \
	} while (0)

#define aSaveBuffer(pkt, s) \
	do { acmd_trace_log_cmd(_SHIFTL(A_SAVEBUFF, 24, 8), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aSaveBufferImpl((uintptr_t)(s)); \
	} while (0)

#define aInterleave(pkt, l, r) \
	do { acmd_trace_log_cmd(_SHIFTL(A_INTERLEAVE, 24, 8), \
	                        _SHIFTL(l, 16, 16) | _SHIFTL(r, 0, 16)); \
	     aInterleaveImpl(l, r); \
	} while (0)

#define aDMEMMove(pkt, i, o, c) \
	do { acmd_trace_log_cmd(_SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24), \
	                        _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16)); \
	     aDMEMMoveImpl(PORT_AUDIO_DMEM(i), PORT_AUDIO_DMEM(o), c); \
	} while (0)

#define aLoadADPCM(pkt, c, d) \
	do { acmd_trace_log_cmd(_SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24), \
	                        (uint32_t)(uintptr_t)(d)); \
	     aLoadADPCMImpl(c, (uintptr_t)(d)); \
	} while (0)

#define aSetLoop(pkt, a) \
	do { acmd_trace_log_cmd(_SHIFTL(A_SETLOOP, 24, 8), \
	                        (uint32_t)(uintptr_t)(a)); \
	     aSetLoopImpl((uintptr_t)(a)); \
	} while (0)

#define aADPCMdec(pkt, f, s) \
	do { acmd_trace_log_cmd(_SHIFTL(A_ADPCM, 24, 8) | _SHIFTL(f, 16, 8), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aADPCMdecImpl(f, (int16_t*)(uintptr_t)(s)); \
	} while (0)

#define aResample(pkt, f, p, s) \
	do { acmd_trace_log_cmd(_SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(p, 0, 16), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aResampleImpl(f, p, (int16_t*)(uintptr_t)(s)); \
	} while (0)

#define aSetVolume(pkt, f, v, t, r) \
	do { acmd_trace_log_cmd(_SHIFTL(A_SETVOL, 24, 8) | _SHIFTL(f, 16, 16) | _SHIFTL(v, 0, 16), \
	                        _SHIFTL(t, 16, 16) | _SHIFTL(r, 0, 16)); \
	     aSetVolumeImpl(f, v, t, r); \
	} while (0)

#define aEnvMixer(pkt, f, s) \
	do { acmd_trace_log_cmd(_SHIFTL(A_ENVMIXER, 24, 8) | _SHIFTL(f, 16, 8), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aEnvMixerImpl(f, (int16_t*)(uintptr_t)(s)); \
	} while (0)

#define aMix(pkt, f, g, i, o) \
	do { acmd_trace_log_cmd(_SHIFTL(A_MIXER, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16), \
	                        _SHIFTL(i, 16, 16) | _SHIFTL(o, 0, 16)); \
	     PORT_AUDIO_PREP_MIX(); \
	     aMixImpl(f, g, PORT_AUDIO_DMEM(i), PORT_AUDIO_DMEM(o)); \
	} while (0)

#define aPoleFilter(pkt, f, g, s) \
	do { acmd_trace_log_cmd(_SHIFTL(A_POLEF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aPoleFilterImpl(f, g, (int16_t*)(uintptr_t)(s)); \
	} while (0)

#define aPan(pkt, f, d, s)          do { (void)(pkt); } while (0)

/* ================================================================== */
/*  N_MICRO (n_abi.h) macro replacements                              */
/*                                                                    */
/*  SSB64 uses N_MICRO (compact audio microcode).  The n_a* macros    */
/*  pack buffer setup (DMEM in/out/count) into the command word       */
/*  itself, unlike standard macros which use separate aSetBuffer.     */
/*  We #undef the n_abi.h originals and redefine them to:             */
/*    1. Call aSetBufferImpl to configure I/O state                   */
/*    2. Call the corresponding CPU impl function                     */
/*    3. Log the N_MICRO-encoded w0/w1 for trace comparison           */
/*                                                                    */
/*  IMPORTANT: TU must include this header AFTER both abi.h AND       */
/*  n_abi.h so the n_a* originals exist to be #undef'd.  abi.h used   */
/*  to auto-include this header, but that ran BEFORE n_abi.h was      */
/*  pulled in via n_synthInternals.h, so n_abi.h's later #defines     */
/*  silently clobbered our redefinitions of n_aLoadBuffer /           */
/*  n_aADPCMdec / etc.  The pull chain wrote Acmd words that nothing  */
/*  consumed → silent BGM.  Now n_env.c explicitly includes mixer.h   */
/*  after the n_audio headers, and abi.h no longer auto-includes us.  */
/* ================================================================== */

#undef n_aADPCMdec
#undef n_aEnvMixer
#undef n_aInterleave
#undef n_aLoadBuffer
#undef n_aResample
#undef n_aSaveBuffer
#undef n_aSetVolume
#undef n_aLoadADPCM
#undef n_aPoleFilter

/* n_aLoadBuffer(pkt, c, d, s) — load c bytes from DRAM s to DMEM d */
#define n_aLoadBuffer(pkt, c, d, s) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_LOADBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aSetBufferImpl(0, 0, PORT_AUDIO_DMEM(d), (c)); \
	     aLoadBufferImpl((uintptr_t)(s)); \
	} while (0)

/* n_aSaveBuffer(pkt, c, d, s) — save c bytes from DMEM d to DRAM s */
#define n_aSaveBuffer(pkt, c, d, s) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_SAVEBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12), \
	                        (uint32_t)(uintptr_t)(s)); \
	     aSetBufferImpl(0, PORT_AUDIO_DMEM(d), 0, (c)); \
	     aSaveBufferImpl((uintptr_t)(s)); \
	} while (0)

/* n_aADPCMdec(pkt, s, f, c, a, d) — ADPCM decode:
 *   s=state, f=flags, c=count, a=dramAlign(=input offset), d=output DMEM */
#define n_aADPCMdec(pkt, s, f, c, a, d) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_ADPCM, 24, 8) | _SHIFTL(s, 0, 24), \
	                        _SHIFTL(f, 28, 4) | _SHIFTL(c, 16, 12) | \
	                        _SHIFTL(a, 12, 4) | _SHIFTL(d, 0, 12)); \
	     aSetBufferImpl(0, PORT_AUDIO_DMEM(a), PORT_AUDIO_DMEM(d), (c)); \
	     aADPCMdecImpl((uint8_t)(f), (int16_t*)(uintptr_t)(s)); \
	} while (0)

/* n_aResample(pkt, s, f, p, i, o) — pitch resample:
 *   s=state, f=flags, p=pitch, i=input DMEM, o=output selector (2 bits) */
#define n_aResample(pkt, s, f, p, i, o) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(s, 0, 24), \
	                        _SHIFTL(f, 30, 2) | _SHIFTL(p, 14, 16) | \
	                        _SHIFTL(i, 2, 12) | _SHIFTL(o, 0, 2)); \
	     aSetBufferImpl(0, PORT_AUDIO_DMEM(i), \
	                    ((o) & 0x3) ? PORT_NAUDIO_MAIN2 : PORT_NAUDIO_MAIN, \
	                    PORT_NAUDIO_COUNT); \
	     aResampleImpl((uint8_t)(f), (uint16_t)(p), (int16_t*)(uintptr_t)(s)); \
	} while (0)

/* n_aSetVolume(pkt, f, v, t, r) — N_MICRO repurposes the flags:
 *   0x00 (A_RATE):      sets LEFT target + ramp (standard: A_LEFT|A_RATE = 0x02)
 *   0x06 (A_LEFT|A_VOL): sets LEFT volume + packs dry/wet into t,r
 *   0x04 (A_RIGHT|A_VOL): sets RIGHT target + ramp (standard: A_RIGHT|A_RATE = 0x00)
 */
#define n_aSetVolume(pkt, f, v, t, r) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_SETVOL, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(v, 0, 16), \
	                        _SHIFTL(t, 16, 16) | _SHIFTL(r, 0, 16)); \
	     if (((f) & 0x0E) == 0x00) { \
	         /* N_MICRO A_RATE(0x00) → standard LEFT rate */ \
	         aSetVolumeImpl(0x02, v, t, r); \
	     } else if (((f) & 0x0E) == 0x06) { \
	         /* N_MICRO A_LEFT|A_VOL(0x06) → LEFT vol + dry/wet */ \
	         aSetVolumeImpl(0x06, v, 0, 0); \
	         aSetVolumeImpl(0x08, t, 0, r); \
	     } else if (((f) & 0x0E) == 0x04) { \
	         /* N_MICRO A_RIGHT|A_VOL(0x04) → RIGHT target + rate */ \
	         aSetVolumeImpl(0x00, v, t, r); \
	     } else { \
	         aSetVolumeImpl(f, v, t, r); \
	     } \
	} while (0)

/* n_aEnvMixer(pkt, f, t, s) — envelope mixer:
 *   f=flags, t=cvolR (on A_INIT) or 0, s=state
 *   N_MICRO packs cvolR into the command; standard path sets it via
 *   a separate aSetVolume(A_RIGHT|A_VOL) call before aEnvMixer.
 *
 * FIXED_SAMPLE = SAMPLES = 184 (per n_synthInternals.h N_MICRO config). */
#define n_aEnvMixer(pkt, f, t, s) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_ENVMIXER, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(t, 0, 16), \
	                        (uint32_t)(uintptr_t)(s)); \
	     if ((f) & 0x01) { /* A_INIT */ \
	         aSetVolumeImpl(0x04 /* A_RIGHT|A_VOL */, (uint16_t)(t), 0, 0); \
	     } \
	     aSetBufferImpl(0, PORT_NAUDIO_MAIN, 0, PORT_NAUDIO_COUNT); \
	     aEnvMixerImpl((uint8_t)((f) | 0x08 /* A_AUX */), (int16_t*)(uintptr_t)(s)); \
	} while (0)

/* n_aInterleave(pkt) — interleave using fixed N_MICRO DMEM addresses */
#define n_aInterleave(pkt) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_INTERLEAVE, 24, 8), 0); \
	     aSetBufferImpl(0, 0, PORT_NAUDIO_MAIN, PORT_NAUDIO_COUNT << 1); \
	     aInterleaveImpl(PORT_NAUDIO_DRY_LEFT, PORT_NAUDIO_DRY_RIGHT); \
	} while (0)

/* n_aLoadADPCM(pkt, c, d) — same semantics as standard aLoadADPCM */
#define n_aLoadADPCM(pkt, c, d) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24), \
	                        (uint32_t)(uintptr_t)(d)); \
	     aLoadADPCMImpl(c, (uintptr_t)(d)); \
	} while (0)

/* n_aPoleFilter(pkt, f, g, t, s) — IIR pole filter:
 *   f=flags, g=gain, t=output shift, s=state */
#define n_aPoleFilter(pkt, f, g, t, s) \
	do { (void)(pkt); \
	     acmd_trace_log_cmd(_SHIFTL(A_POLEF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16), \
	                        _SHIFTL(t, 24, 8) | _SHIFTL((uint32_t)(uintptr_t)(s), 0, 24)); \
	     aSetBufferImpl(0, ((t) == 0) ? PORT_NAUDIO_MAIN : PORT_NAUDIO_MAIN2, \
	                    ((t) == 0) ? PORT_NAUDIO_MAIN : PORT_NAUDIO_MAIN2, \
	                    PORT_NAUDIO_COUNT); \
	     aPoleFilterImpl(f, g, (int16_t*)(uintptr_t)(s)); \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* PORT */
#endif /* PORT_AUDIO_MIXER_H */
