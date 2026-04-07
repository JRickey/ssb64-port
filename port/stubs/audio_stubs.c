/**
 * audio_stubs.c — Stub implementations for audio functions not yet ported.
 *
 * Functions now provided by the decomp n_audio sources (n_env.c, n_csp*.c,
 * n_seq*.c, cents2ratio.c) have been REMOVED from this file:
 *   n_alInit, n_alClose, n_alAudioFrame, n_alCSPNew, n_alCSPPlay,
 *   n_alCSPStop, n_alCSPSetBank, n_alCSPSetSeq, n_alCSPSetVol,
 *   n_alCSPSetChlFXMix, n_alCSPSetChlPriority, n_alCSeqNew, alCents2Ratio,
 *   func_80026070..func_80026A10 (all FGM/SFX functions in n_env.c)
 *
 * Only OS stubs needed by the n_audio code and data definitions remain.
 */

#include <ssb_types.h>
#include <PR/os.h>
#include <PR/os_internal.h>
#include <PR/libaudio.h>
#include <sys/audio.h>

#include <stdarg.h>
#include <string.h>

/* ========================================================================= */
/*  N64 OS / SDK functions needed by the n_audio decomp sources              */
/* ========================================================================= */

OSIntMask osSetIntMask(OSIntMask mask)
{
	(void)mask;
	return 0;
}

void __osError(s16 code, s16 nargs, ...)
{
	va_list ap;
	va_start(ap, nargs);
	va_end(ap);
	(void)code;
}

/* alCopy — N64 SDK memcpy for audio structures */
void alCopy(void *src, void *dest, s32 len)
{
	memcpy(dest, src, (size_t)len);
}

/* ========================================================================= */
/*  Audio settings data                                                      */
/* ========================================================================= */

/*
 * dSYAudioPublicSettings2 and dSYAudioPublicSettings3 are initialised data
 * tables for audio configuration.  They are referenced by audio.c but
 * defined in game-specific code outside the n_audio library.
 * Zero-initialised stubs allow linking.
 */
SYAudioSettings dSYAudioPublicSettings2 = { 0 };
SYAudioSettings dSYAudioPublicSettings3 = { 0 };
