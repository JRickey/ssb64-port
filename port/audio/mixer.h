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

#define aSegment(pkt, s, b)         do { (void)(pkt); } while (0)
#define aClearBuffer(pkt, d, c)     aClearBufferImpl(d, c)
#define aSetBuffer(pkt, f, i, o, c) aSetBufferImpl(f, i, o, c)
#define aLoadBuffer(pkt, s)         aLoadBufferImpl((uintptr_t)(s))
#define aSaveBuffer(pkt, s)         aSaveBufferImpl((uintptr_t)(s))
#define aInterleave(pkt, l, r)      aInterleaveImpl(l, r)
#define aDMEMMove(pkt, i, o, c)     aDMEMMoveImpl(i, o, c)
#define aLoadADPCM(pkt, c, d)       aLoadADPCMImpl(c, (uintptr_t)(d))
#define aSetLoop(pkt, a)            aSetLoopImpl((uintptr_t)(a))
#define aADPCMdec(pkt, f, s)        aADPCMdecImpl(f, (int16_t*)(uintptr_t)(s))
#define aResample(pkt, f, p, s)     aResampleImpl(f, p, (int16_t*)(uintptr_t)(s))
#define aSetVolume(pkt, f, v, t, r) aSetVolumeImpl(f, v, t, r)
#define aEnvMixer(pkt, f, s)        aEnvMixerImpl(f, (int16_t*)(uintptr_t)(s))
#define aMix(pkt, f, g, i, o)       aMixImpl(f, g, i, o)
#define aPoleFilter(pkt, f, g, s)   aPoleFilterImpl(f, g, (int16_t*)(uintptr_t)(s))
#define aPan(pkt, f, d, s)          do { (void)(pkt); } while (0)

#ifdef __cplusplus
}
#endif

#endif /* PORT */
#endif /* PORT_AUDIO_MIXER_H */
