# Envelope-mixer ramp int32 overflow corrupts dominant channel at extreme pan

**Date:** 2026-05-08
**Status:** RESOLVED — issue #108 ("odd sounding hit SFX" v0.7-beta)
**Class:** N64 RSP microcode reimplemented as scalar CPU code where a "saturate-immediately" sentinel rate, paired with a value already near `INT32_MAX`, causes `value += step` to wrap silently and break the existing reach check.

## Symptom

Hit SFX in any battle stage with a wide horizontal extent (Sector Z, Hyrule Castle, Saffron, etc.) sounded harshly distorted whenever fighters were near the screen edge. Confirmed character- and move-independent: Link F-smash on Sector Z's left cockpit area, Mario F-smash on the right below-wing area, both reproduce. Mid-stage smashes were clean.

The corruption tracked the dominant pan channel: fighter on screen-left → harsh noise on the left speaker only; fighter on screen-right → harsh noise on the right speaker only. Mid-screen pan = clean both channels. Forcing all FGM pans to centre (`pan=64`) fully eliminated the noise.

WAV signature in the corrupted window:

| Metric | Buggy | Fixed |
|---|---|---|
| L huge jumps (sample-to-sample \|Δ\| > 15000) over 30 s | **178** | **0** |
| L rail hits (±32767) | 13 | 0 |
| L peak | 32767 | 27720 |

The bus int32 `dry_left_acc[i]` formed a sawtooth waveform with period exactly 16 samples (= 32 kHz / 2 kHz). Each 16-sample group started at a large negative value, ramped smoothly up to a large positive value, then hard-flipped back to negative — i.e. a textbook 2 kHz buzz overlaid on the legitimate audio.

## Root cause

`port/audio/mixer.c::ramp_step` is the per-sample envelope ramp consumed by `aEnvMixerImpl`. Each sample it does:

```c
ramp->value += ramp->step;
reached = (ramp->step <= 0) ? (ramp->value <= ramp->target)
                            : (ramp->value >= ramp->target);
if (reached) {
    ramp->value = ramp->target;
    ramp->step  = 0;
}
return (int16_t)(ramp->value >> 16);
```

`value` and `step` are `int32_t`; `value` carries the s16 volume in Q15.16 fixed-point.

Upstream in `n_env.c::_getRate` (the n_micro variant), the `count == 0` branch returns the sentinel `*ratel = 0xFFFF; return 0x7FFF;`. The N64 RSP envelope ucode interprets this as **"saturate to target this sample"** — a one-sample volume jump.

Hits that path whenever `_getRate` is asked to ramp from `cvol` to `tgt` over zero samples and `tgt >= cvol`, which is the standard shape for a voice that has **just** been started/retargeted at an already-correct volume — `em_first` is set, `em_segEnd` is 0, the math says "no ramp needed, just apply." `n_alSynStartVoiceParams_Alt` and SET_PAN updates both produce this configuration on every voice start and on every cross-screen pan change.

In the port's `aEnvMixerImpl` A_INIT path, that sentinel rate is unpacked as:

```c
int32_t rate_left = ((int32_t)rspa.rate_left_m << 16) | rspa.rate_left_l;
ramps[0].step = rate_left / 8;
```

For `ratem=0x7FFF, ratel=0xFFFF`:

```
rate_left = (0x7FFF << 16) | 0xFFFF = 0x7FFFFFFF = INT32_MAX
step      = INT32_MAX / 8           = 0x0FFFFFFF = 268,435,455
```

Now consider a voice panned hard to one channel with `em_volume * n_eqpower[em_pan] >> 15 ≈ em_volume`. For a typical mid-loud SFX with `em_volume = 29542`, `target = 29542 << 16 = 1,936,654,336` — within striking distance of `INT32_MAX`.

Per-sample iteration of `ramp_step`:

```
sample 0: value = 1,936,654,336 + 268,435,455 = 2,205,089,791
            > INT32_MAX (2,147,483,647) → signed-int32 OVERFLOW (UB; wraps in practice)
            value reads back as -2,089,877,505 (negative)
sample 0: reach check: step > 0, value >= target?
            -2,089,877,505 >= 1,936,654,336 ?  NO → not reached
            value stays at -2,089,877,505 (no clamp to target)
            return (int16_t)(-2,089,877,505 >> 16) = -31,898  (negative!)

sample 1..15: value += 268,435,455 each, climbs back through zero
              into positive territory.  At sample 15, value reaches
              ~1,936,064,496 — once again just under INT32_MAX, the
              reach check almost catches… but step puts it just over
              again.

sample 16: value overflows again, wraps to ~-2,090,467,345
            return l_vol = -31,899  (negative again)
```

The cycle repeats every ~16 samples — exactly the period needed for `+268M/sample` to traverse the full int32 range (`2³² / 268M ≈ 16`). The sign flip is what produces the audible 2 kHz buzz: the dry-bus contribution `(s * l_dry) >> 15` with l_vol oscillating between full-positive and full-negative gives a sawtooth twice the amplitude of the legitimate signal, which then dominates the per-channel sum and rails the bus → soft-clip → harsh noise.

R bus is unaffected because the same voice's `em_rtgt` at extreme L-pan is `em_volume * n_eqpower[127] / 32768 ≈ 0`. The ramp on the silent side never gets close to INT32_MAX, never overflows, and the legitimate-low contribution is noise-free.

## Fix

`port/audio/mixer.c::ramp_step` — accumulate through `int64_t`, clamp to int32 range before the existing reach check, and let the reach check catch the overshoot and clamp to target on the same sample:

```c
int64_t next = (int64_t)ramp->value + (int64_t)ramp->step;
if (next > INT32_MAX) next = INT32_MAX;
if (next < INT32_MIN) next = INT32_MIN;
ramp->value = (int32_t)next;
reached = (ramp->step <= 0) ? (ramp->value <= ramp->target)
                            : (ramp->value >= ramp->target);
if (reached) {
    ramp->value = ramp->target;
    ramp->step  = 0;
}
return (int16_t)(ramp->value >> 16);
```

The int64 path means the "saturate-immediately" sentinel — INT32_MAX rate added to a near-INT32_MAX value — produces an int64 sum well above INT32_MAX, gets clamped to INT32_MAX, the reach check correctly sees `value >= target`, and value snaps to target on the very first sample. From sample 1 onward `step = 0` and the per-sample contribution is the constant target gain the game intended.

For ramps that don't start near the rail (most real attack/decay envelopes), the int64 path is bit-equivalent to the original int32 add — the clamp branches never fire — so this is a pure fix-the-edge-case change with no behavior delta on healthy data.

## Why this hadn't surfaced earlier in port-side audio work

Multiple unrelated audio fixes during the port's bring-up (`audio_adpcm_codebook_stale_slots`, `audio_alparam_variant_overrun`, `audio_mixer_macro_clobber`, `audio_int32_acc_save_load_bridge`, `audio_output_filter_overshoot_clip`) all addressed loud-SFX or extreme-pan symptoms one layer or another upstream of `ramp_step`. With those fixes in place, the only remaining failure mode that matched issue #108's "harsh noise on hit SFX" symptom was at the per-sample ramp itself — but the bug only triggers when both:

1. `em_volume * n_eqpower[em_pan] >> 15` lands within `INT32_MAX - step` of the rail (= the dominant channel at extreme pan)
2. `_getRate` produces the sentinel rate (= count==0 with tgt>=vol, common on voice start / SET_PAN)

A fighter at mid-stage on any stage hits condition (2) constantly but never (1), so the wrap never happens. A voice with a real attack ramp (segEnd > 0) hits (1) on edge stages but never (2). It takes both — and that combination only occurs reliably on screen-edge gameplay.

## Class

N64 RSP microcode operations reimplemented as scalar CPU code where a "magic" rate or count value was historically interpreted by ucode as an instant-saturate signal but the port's plain integer arithmetic interprets it as a literal large step. Sibling: `audio_adpcm_codebook_stale_slots_2026-05-02` (CPU `memcpy` of `count` book bytes left the unused tail in a state the original ucode never produced).

Audit hook: any time port code reimplements an N64 audio op that takes a "rate" / "step" / "count" parameter, check whether the original ucode special-cases boundary values (zero, INT32_MAX, etc.) and whether the port's arithmetic tolerates being driven near the int32 rail with that magic value applied.
