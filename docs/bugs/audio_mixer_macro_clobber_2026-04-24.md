# Mixer macro overrides clobbered by late `n_abi.h` include

**Date:** 2026-04-24
**Status:** Fix shipped. Verified: BGM 33 produces audible non-zero samples on the attract demo.
**Class:** Header-include-order silently undoing macro overrides. Sibling of LUS-vs-decomp typename shadowing — the symptom is "the function I see in the source isn't the one that runs."

## Symptom

After the bank-swap (`docs/bugs/audio_bridge_bank_swap_2026-04-24.md`) and `ALParam` variant-overrun fixes (`docs/bugs/audio_alparam_variant_overrun_2026-04-24.md`), the audio pipeline ran end-to-end without crashing — voices allocated, `START_VOICE_ALT` set `em_motion = AL_PLAYING`, the envmixer pull chain ran, `n_alResamplePull` and `n_alAdpcmPull` fired, even `aLoadADPCM` (codebook load) reached `aLoadADPCMImpl` — but `aLoadBufferImpl` (sample data load) and `aADPCMdecImpl` (decoder) never executed. The mixer output buffer stayed all-zero and the existing one-shot `first non-zero sample detected` log never tripped.

## Root cause

`include/PR/abi.h` finished its declarations and then auto-included `port/audio/mixer.h`:

```c
#endif /* _LANGUAGE_C */

#ifdef PORT
#include "audio/mixer.h"
#endif

#endif /* !_ABI_H_ */
```

`mixer.h` `#undef`s the standard `aLoadADPCM`, `aLoadBuffer`, etc. **and** the N_MICRO `n_aLoadBuffer`, `n_aADPCMdec`, `n_aResample`, `n_aLoadADPCM`, `n_aSetVolume`, `n_aEnvMixer`, `n_aInterleave`, `n_aSaveBuffer`, `n_aPoleFilter` macros, then redefines them all to call the `aXxxImpl()` CPU functions in `port/audio/mixer.c`.

But `n_abi.h` (the file that defines the `n_a*` originals) is included via `n_audio/n_synthInternals.h` — **after** `abi.h` returns. The preprocessor sequence ended up:

1. `abi.h` defines standard `aXxx` macros.
2. `abi.h` auto-includes `mixer.h`. `mixer.h` `#undef`s the `aXxx` originals (existed) and the `n_a*` originals (did **not** exist yet — these `#undef`s become no-ops). `mixer.h` redefines both families.
3. `abi.h` finishes, returns to includer.
4. Some later `#include` chain reaches `n_audio/n_abi.h`.
5. `n_abi.h`'s include guard hadn't been set, so it runs — defining `n_aLoadBuffer`, `n_aADPCMdec`, etc. as the **original** Acmd-packing macros, **silently overriding** mixer.h's CPU-impl versions.
6. `n_env.c` later compiles `_decodeChunk`. The `n_aLoadBuffer(ptr++, …)` call expands to the original Acmd-write macro (just `_a->words.w0 = …; _a->words.w1 = (unsigned int)(s);`) — never calls `aLoadBufferImpl`.
7. The Acmd words written into the command buffer are never consumed (we have no RSP), the mixer output buffer stays zeroed, audio is silent.

Because `mixer.h`'s own include guard prevented re-inclusion when `n_env.c` later did `#include "audio/mixer.h"` directly, there was no "second pass" that could re-#undef.

The standard `aXxx` family (`aLoadADPCM`, `aSetLoop`, etc.) worked fine because it was defined *before* mixer.h ran and never redefined afterward. Only the `n_a*` family was clobbered.

## Why the diagnostic was hard

Every upstream check passed:

- `_decodeChunk` was confirmed entered with `nbytes=90` (logged).
- `dramLoc = f->dc_dma(…)` returned a valid host pointer (passthrough — fix shipped earlier).
- The preprocessed source from a hand-rolled `cc -E …` invocation **showed `aLoadBufferImpl(...)` in the call body** — but only because the hand command differed from the cmake build. With the actual cmake compile flags, `cc -E -dD` showed `n_aLoadBuffer` redefined twice — once by mixer.h, then again by n_abi.h.
- `nm build/ssb64 | grep aLoadBufferImpl` confirmed the impl was linked.

The smoking gun was disassembling `__decodeChunk` from the binary and finding only Acmd-word stores between `dc_dma` and `aSetLoop` — no `bl _aLoadBufferImpl`.

## Fix

`include/PR/abi.h` — drop the auto-include:

```c
/* PORT: do NOT auto-include "audio/mixer.h" here.  TUs that use the
 * n_a* macros must include mixer.h themselves AFTER pulling in n_abi.h
 * (e.g. via n_audio/n_synthInternals.h). */
```

`port/audio/mixer.h` — comment block at the top of the N_MICRO section explaining the include-order requirement.

`src/libultra/n_audio/n_env.c` already does `#include "audio/mixer.h"` after the n_audio headers, so its `n_aLoadBuffer` calls now resolve to the CPU impl (verified by re-disassembling `__decodeChunk` after rebuild).

## Verified

`SSB64 Audio: first non-zero sample detected (idx=0 v=17)` fires within seconds of `syAudioPlayBGM` being called. 30 s smoke test stable, no crashes, BGM 33 audible end-to-end.

## Fingerprint for next time

- An impl-replacement mixer/codec macro family is silently inactive on PORT, even though the impl symbols exist in the binary and the source clearly shows the macro call.
- `cc -E -dD` against the **exact** cmake compile flags reveals whether macros get redefined more than once.
- Disassemble the suspected caller and check whether the impl function is actually `bl`-called — Acmd-only writes between expected impl calls means the original macro is winning.
- Whenever a port-side header `#undef`s a decomp macro family, **trace which header re-includes it later via include guards**. If the redefiner can't be guaranteed to run after the last macro definition, the override is fragile.
