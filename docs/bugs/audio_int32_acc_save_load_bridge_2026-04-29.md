# Audio: reverb wet bus invisible to FX delay-line save/load — 2026-04-29

## Symptom

BGM and SFX played but sounded "dry" — missing reverb. Multiple users (incl.
jtl3d / MarioReincarnate) flagged the absence; the decomp author had previously
guessed "it sounds like I'm just hearing the raw samples." On title music
(BGM 33, scene 28) the difference is subtle but on stage music with intentional
reverb (e.g. Hyrule Castle) it's pronounced.

## Root cause

Two separate memory regions back the audio mixer:

| Region | What it stores | Used by |
|---|---|---|
| `BUF_U8` / `BUF_S16` (s16 DMEM) | s16 samples at any DMEM offset | `aLoadBufferImpl` / `aSaveBufferImpl` (DRAM ↔ DMEM) |
| `rspa.{dry,wet}_{left,right}_acc[]` (int32) | per-bus mix accumulators | `aEnvMixerImpl` / `aMixImpl` via `sample_read/write` |

`fixed_bus_acc(addr)` maps the four bus addresses (`PORT_NAUDIO_DRY_LEFT/RIGHT`,
`PORT_NAUDIO_WET_LEFT/RIGHT`) to the int32 accumulator arrays, so any sample-level
op (envmix, mix, clear) that touches those addresses goes through the int32 path.

The leak: `aLoadBufferImpl` and `aSaveBufferImpl` only memcpy through
`BUF_U8` — they don't consult `fixed_bus_acc`. So when `n_alFxPull` did

```c
_n_saveBuffer(r, r->input, N_AL_AUX_L_OUT, ptr);   // wet bus → delay-line DRAM
```

`aSaveBufferImpl` read `BUF_U8(0xCB0)` (an unused s16 region at that offset) and
copied zeros to the FX delay line. The reverb processed zeros, the delay output
was zero, and `n_alMainBusPull`'s `aMix(N_AL_AUX_L_OUT → N_AL_MAIN_L_OUT)` added
zero to the dry bus — even though the wet accumulator did contain real envmix
output upstream.

The same break the other direction: `_n_loadBuffer(DRAM → DMEM at TEMP_0)` is
fine because TEMP_0 isn't a fixed-bus address, but any wet-mapped load would
similarly fail to reach the int32 acc.

## Fingerprint via runtime telemetry

`port/audio/mixer.c:aInterleaveImpl` instrumented (since reverted) to log peak
abs values across all four bus accumulators every ~120 audio frames:

```
Before fix
  BUS peaks @ frame 1200: dryL=2944  dryR=3129   wetL=0     wetR=0     outL=2944
                                                 ^^^^^^^^^^^^^^^^^^^^^
                                                 wet bus exactly zero in every frame

After fix
  BUS peaks @ frame 1200: dryL=3629  dryR=3814   wetL=1024  wetR=1024  outL=3629
                                                 ^^^^^^^^^^^^^^^^^^^^^
                                                 wet bus carries real reverb output;
                                                 dryL ~23% louder because aMix(wet→dry)
                                                 finally has signal to add
```

`wetL == wetR` after fix is correct: `n_alFxPull` mixes L+R to mono in the wet
bus, runs the delay sections, then `aDMEMMove(N_AL_AUX_R_OUT → N_AL_AUX_L_OUT)`
copies right back to left.

## What was *not* the bug (ruled out along the way)

- **FGM bytecode interpreter (the decomp author's first guess).** Both
  `func_80027460_28060` (fgm.tbl articulation interp) and
  `func_80026B90_27790` (fgm.ucd voice script interp) run on valid pointers
  and parse correct opcodes — first-byte dumps matched upstream `audio.md`'s
  `nSYAudioFGMExplodeS` example exactly (`de 00 d1 07 d2 ff d3 44`).
- **FX type wiring.** `n_alSynNew` selected `n_alFxPull` (reverb chain) when
  scManagerRunLoop's `syAudioSetFXType(AL_FX_CUSTOM)` propagated to fxType=6.
- **Output-stage low-pass.** A/B with `SSB64_AUDIO_OUTPUT_FILTER=0` was
  audibly identical, so the 14.5 kHz biquad in `audio_playback.cpp` wasn't
  what was masking the reverb.
- **Address translation between `N_AL_*` and `PORT_NAUDIO_*`.** The
  `PORT_AUDIO_DMEM(a)` macro adds `PORT_NAUDIO_MAIN` (0x4f0) so
  `N_AL_AUX_L_OUT (0x7c0) → 0xcb0 = PORT_NAUDIO_WET_LEFT`. That mapping is
  correct; the names alias the same int32 accumulator from `aMix`'s view.
  The break is at the point where the FX chain crosses the int32-acc / s16-DMEM
  boundary via save/load.

## Fix

`port/audio/mixer.c`:

- `aLoadBufferImpl`: after the DRAM → `BUF_U8` memcpy, if `rspa.out` aliases
  a fixed-bus accumulator, copy the just-loaded s16 mirror into the int32
  accumulator so subsequent `sample_read` ops see the loaded data.
- `aSaveBufferImpl`: before the `BUF_U8` → DRAM memcpy, if `rspa.in` aliases
  a fixed-bus accumulator, clamp16 each int32 sample into the s16 mirror so
  the memcpy carries the actual bus data instead of the stale s16 region.

28 lines added, no other behavior changes. Verified by re-running the same
trace: wet bus peaks went from 0 → ~1k, dry bus picked up the wet contribution,
audible reverb returned.

## Sibling bugs

- `audio_mixer_macro_clobber_2026-04-24.md` — same family: "correct data,
  wrong endpoint." There the `n_a*` macros' CPU implementations were silently
  shadowed by `n_abi.h`'s Acmd-write originals; here the bus accumulator
  arrays were silently invisible to save/load.
- `audio_mixer_n_micro_dmem_addresses_2026-04-24.md` — also a routing
  mismatch (standard vs. N_MICRO DMEM layout); resolved by adopting the
  `PORT_AUDIO_DMEM(a)` translator that this fix relies on.

## Credits

Symptom flagged by **jtl3d / MarioReincarnate** (community feedback);
their decomp work also let us narrow this down by ruling out incorrect
hypothesis paths quickly.
