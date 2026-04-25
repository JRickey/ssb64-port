# Audio mixer used standard AL_* DMEM addresses instead of N_MICRO N_AL_* layout

**Date:** 2026-04-24
**Status:** Fix shipped. Verified empirically: stereo L-R correlation jumped from ~0 (wildly different signals) to +0.94 (coherent stereo with spatial offset); waveform shows clear periodic musical structure where it was incoherent hash before.
**Class:** Texture-swizzle-style consumer/producer address mismatch — bytes are correct, the consumer reads them from the wrong location because two layouts share field names but differ in concrete addresses.

## Symptom

After the bridge bank-swap, ALParam variant-overrun, and mixer-macro-clobber fixes turned silent BGM into audible output, the user reported the audio as "scratchy, noisy, unrecognizable" — but with note onsets that matched the expected attract-demo BGM 33 sequence. Orchestration correct, timbre wrong.

WAV dump captured 10 s of PCM via `portAudioSubmitFrame` and inspected with a Python/scipy script. Two diagnostic findings stood out:

1. **L vs R channels visualized at 5-ms zoom looked like two completely different signals.** Stretches where L was flat near zero coincided with R saturating at -32k. Real stereo audio tracks closely between channels — they should look like the same waveform with mild spatial offset.

2. **Spectrum had narrow peaks clustered at 3000-3343 Hz** that aligned roughly with synthesis-chunk-rate harmonics (chunk_rate = 32000 / 184 ≈ 173.91 Hz; 18× ≈ 3130, 19× ≈ 3304). Suggested per-chunk discontinuity from a state/buffer mismatch.

## Root cause

`port/audio/mixer.c` hard-coded the four output-bus DMEM addresses against the **standard `AL_*` constants** from `include/n_audio/synthInternals.h`:

```c
#define MIX_MAIN_L 1088   /* AL_MAIN_L_OUT */
#define MIX_MAIN_R 1408   /* AL_MAIN_R_OUT */
#define MIX_AUX_L  1728   /* AL_AUX_L_OUT  */
#define MIX_AUX_R  2048   /* AL_AUX_R_OUT  */
```

But SSB64 builds with `N_MICRO=1`, which swaps in the **`N_AL_*` constants** from `include/n_audio/n_synthInternals.h`:

```c
#define N_AL_MAIN_L_OUT 1248
#define N_AL_MAIN_R_OUT 1616
#define N_AL_AUX_L_OUT  1984
#define N_AL_AUX_R_OUT  2352
```

Throughout the synth pull chain (n_alMainBusPull, n_alAuxBusPull, n_alSavePull / aInterleave), every `aClearBuffer`/`aMix`/`aInterleave` macro call uses the `N_AL_*` constants. The standard `AL_*` family is only used in the `#ifndef N_MICRO` branches that don't compile.

So:

- Each voice's `aEnvMixerImpl` call accumulated mixed samples into the wrong DMEM addresses (1088/1408/1728/2048).
- `n_alMainBusPull`'s clear+aMix worked on the right DMEM addresses (1248/1616/1984/2352), but those buffers had only the cleared bytes plus whatever `aMix(... AUX → MAIN)` carried over from the AUX bus.
- `aInterleaveImpl(N_AL_MAIN_L_OUT, N_AL_MAIN_R_OUT)` then read from 1248/1616 and wrote interleaved garbage to the output — entirely missing the actual voice mix that had landed at 1088/1408.

The output PCM was effectively whatever leaked from the AUX bus residue plus uninitialized DMEM, with each output sample independently depending on which bytes happened to be in those address ranges. Hence:

- L and R channels had no shared content (each pulled from a different uninitialized region).
- High-frequency hash from sample-to-sample address aliasing matched chunk-boundary discontinuities.
- Saturation reached ~3.4% of samples (occasional bytes interpreted as full-scale s16 values).

## Fix

`port/audio/mixer.c` — change the four `MIX_*` constants to the `N_AL_*` values, with a comment explaining the trap so the next person doesn't re-introduce it.

## Verification

| Metric                    | Before fix | After fix |
|---------------------------|------------|-----------|
| L-R cross-correlation     | (visually incoherent) | **+0.9391** |
| L peak / R peak           | mismatched | both 32767 (matched) |
| L RMS / R RMS             | uneven     | 10776 / 10790 (matched) |
| L saturation %            | 1.71%      | 1.64% |
| R saturation %            | 1.68%      | 1.65% |
| Time-domain waveform      | dense / noisy / no visible period | clear periodic musical structure |

Audible BGM is now recognizably musical (still some clipping; see follow-up).

## Fingerprint for next time

When a port-side replacement implements an N64 RSP microcode replacement, **verify which DMEM-layout constant family the build uses**. SSB64 has two: standard (`AL_*` in `synthInternals.h`) for full audio microcode, and N_MICRO (`N_AL_*` in `n_synthInternals.h`) for the compact microcode. The two pre-define constants of the same role at different addresses. The build defines `N_MICRO=1` in `CMakeLists.txt` to select the latter, but a port-side mixer that hard-codes the standard addresses silently routes to nowhere.

Diagnostic: if stereo channels look completely independent on a 5-ms zoom, the mixer's output is being read from a different DMEM region than it was written to. Check both producer (envmix) and consumer (interleave/save) for matching constant-family use.

This is a sibling of `audio_mixer_macro_clobber_2026-04-24.md` — both involve correct data routed to the wrong address by a port/decomp interface mismatch.

## Open follow-up

Saturation rate is still ~1.65% per channel. With N voices summing into a saturating bus accumulator, transients from chord-like simultaneous notes can clip even when individual voice volumes are small. The original RSP audio uses 32-bit accumulators internally with saturation only at the final output stage — our `aEnvMixerImpl` saturates per-voice mix-in. Whether this matters in practice depends on how loud the user perceives the residual distortion. If it remains audible, widen the DMEM bus to `int32_t` accumulator and saturate only at the `aSaveBufferImpl` step.
