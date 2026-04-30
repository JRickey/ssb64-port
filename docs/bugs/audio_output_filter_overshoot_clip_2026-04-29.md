# Output Low-Pass Biquad Overshoot Clipped Loud SFX — 2026-04-29

**Status:** RESOLVED

## Symptom

DK's win-pose growl (CSS lock-in voice `nSYAudioVoiceDonkeySmash1`, FGM 0x146) played as garbled noise when the output low-pass filter (`SSB64_AUDIO_OUTPUT_FILTER=1`, default) was active. With `SSB64_AUDIO_OUTPUT_FILTER=0`, the same voice was clean. Yoshi's win-pose voice (`nSYAudioVoiceYoshiAppeal`, FGM 0x247) was clean either way.

The reverb-bridge fix (`audio_int32_acc_save_load_bridge_2026-04-29`) restored the wet bus but did not address this — DK still garbled with reverb working correctly.

## Root cause

`port/audio/audio_playback.cpp::applyOutputFilter` runs a Butterworth biquad LPF at fc = 14.5 kHz, fs = 32 kHz. That cutoff lands at **0.91 × Nyquist**, where the bilinear transform's frequency warp distorts the discrete-time response away from the analog Butterworth target.

Hand-computed step response with the original coefficients:

```
y[0] = 0.812
y[1] = 1.146   ← 14.6% overshoot
y[2] = 0.893
y[3] = 1.075
y[4] = 0.952
y[5] = 1.028
...
```

vs the analog Butterworth Q = 1/√2 target of 4.3% overshoot. The bilinear warp inflates overshoot by ~3×.

For input near int16 max (loud SFX — DK's deep growl peaks at the rails), the overshoot drives the filter output to ~37500 → `clampS16FromDouble` flat-tops at ±32767 → square-wave harmonic injection → audible "garbled noise."

Yoshi's voice peaks lower; the overshot output stays under int16; no clamping; clean output. Same explanation for why BGM-only mixes weren't garbled — none of the BGM peaks crossed the overshoot threshold.

The diagnostic that pinpointed this:

1. `SSB64_AUDIO_OUTPUT_FILTER=0` → DK clean.
2. Filter forced to passthrough (`sFilteredBuf[i] = input[i]` instead of biquad math) → DK clean.
3. Filter math restored → DK garbled.

That isolated the issue to the biquad math itself, not buffer routing or upstream saturation. Hand-derivation of the step response then made the overshoot quantitative.

## Fix

`port/audio/audio_playback.cpp`: pre-attenuate the numerator coefficients by `kHeadroom = 0.85`. Filter step response now peaks at `0.85 × 1.146 ≈ 0.974` of input — just under int16 max even on int16-max input, so `clampS16FromDouble` never flat-tops.

```cpp
constexpr double kHeadroom = 0.85;
constexpr double b0 = 0.8118317459078658 * kHeadroom;
constexpr double b1 = 1.6236634918157316 * kHeadroom;
constexpr double b2 = 0.8118317459078658 * kHeadroom;
constexpr double a1 = 1.5879371063123660;
constexpr double a2 = 0.6593898773190974;
```

Cost: ~1.4 dB drop on the filtered output path. Acceptable for an anti-imaging filter — the user perceives this as marginally quieter, but no individual sample is lost. Anti-imaging effectiveness near 14.5 kHz is unchanged (numerator scaling is frequency-flat).

## Why earlier theories were wrong

- **Bus saturation in `aInterleave`**: I tried replacing the `clamp16` at the bus → s16 transition with a tanh soft-clip. Didn't help because the sum into the int32 accumulators *doesn't* exceed int16 by enough to flat-top there — the saturation that was producing flat-tops was happening one stage later, *inside the filter*, after the filter's own gain peak.
- **Reverb wet-bus zero (jtl3d)**: real bug, fix was correct, but it explains "missing reverb" not "garbled DK voice."
- **Per-instrument codebook / sample data corruption**: ruled out because filter-off was clean — DK voice arrived intact at `applyOutputFilter`'s input.

## Class of bug

A bilinear-transform IIR filter designed for fc/fs near the upper edge of the passable range (>~0.85 × Nyquist) needs the numerator scaled down to absorb step-response overshoot, OR a different topology (e.g. cascaded first-order stages, or a windowed FIR) that has no overshoot. The "design as analog Butterworth, bilinear-transform to discrete" workflow silently assumes fc ≪ Nyquist and produces over-peaking when that assumption breaks.

When tuning a discrete LPF for very-near-Nyquist cutoffs in the future: hand-check the discrete step response against your assumed analog target, not just the magnitude response.
