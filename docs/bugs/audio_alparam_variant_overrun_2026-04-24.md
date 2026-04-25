# ALParam variant-struct overrun corrupts envmixer freelist (LP64)

**Date:** 2026-04-24
**Status:** Fix shipped. Verified: 30 s smoke test runs without crash; safety-net diagnostics removed.
**Class:** Variant-struct overlay where one variant grew on LP64 due to embedded pointer widening, but the underlying allocator sizes from the smallest variant.

## Symptom

Immediately after the audio_bridge bank-swap fix (`docs/bugs/audio_bridge_bank_swap_2026-04-24.md`) — which finally let music NoteOns find real instruments — the synthesizer crashed within a few audio frames:

```
SSB64: !!!! CRASH SIGSEGV fault_addr=0x7f0d
…
0   ssb64                               0x...n_alEnvmixerPull + 100
1   ssb64                               0x...n_alEnvmixerPull + 2132
2   ssb64                               0x...n_alAuxBusPull + 152
…
```

`n_alEnvmixerPull + 100` is the `ldr w8, [x8, #0x8]` that reads `e->em_ctrlList->delta`. `x8` was 0x7f05, so `em_ctrlList` itself had been written to that small value.

## Root cause

`ALParam` is the freelist allocation unit. Many call sites (`n_synstartvoiceparam.c`, `n_synfreevoice.c`, `n_synset*.c`) request a slot via `__n_allocParam()` and then **cast** the returned pointer to a variant struct (`ALStartParamAlt`, `ALStartParam`, `ALFreeParam`) before writing the type-specific fields and linking it into a voice's `em_ctrlList`.

On N64 every variant fit comfortably inside `sizeof(ALParam) = 32`. On LP64:

| Struct           | N64 | PORT | Reason for growth                              |
|------------------|-----|------|------------------------------------------------|
| ALParam          | 32  | 40   | `next *` widened 4→8                            |
| ALStartParam     | 16  | 32   | `next *` and `wave *` widened, alignment pad   |
| **ALStartParamAlt** | **30** | **48** | **`next *` + `wave *` widened, plus 4-byte pad before wave to 8-align it; pushed `unk1C/unk1D` to offsets 40/41** |
| ALFreeParam      | 16  | 24   | `next *` and `pvoice *` widened                |

`ALStartParamAlt` is **8 bytes larger** than the freelist slot. `n_alSynStartVoiceParams_Alt`:

```c
update = (ALStartParamAlt *)__n_allocParam();   /* gets 40-byte ALParam slot */
…
update->wave  = w;          /* writes 8 bytes at offsets 32..39 */
update->unk1C = arg7;       /* writes 1 byte at offset 40       */
update->unk1D = arg8;       /* writes 1 byte at offset 41       */
```

Offsets 40/41 land on bytes 0/1 of the **next** ALParam in the heap-allocated array — i.e. on the low two bytes of *its* `next` pointer. Subsequent allocators or consumers walked into that corruption.

The exact byte pattern is diagnostic. With `arg7 = 0x05`, `arg8 = 0x7F` (typical chan-state `unk_0x12`/`unk_0x13` values), the corrupted next pointer became `00 00 00 00 00 00 7F 05` LE = **`0x0000000000007F05`**, which is precisely the small fault address we observed.

`em_ctrlList` then walked into that fake pointer on the next frame and SIGSEGV'd at the first field load.

## Fix

`include/n_audio/synthInternals.h` — pad `ALParam` to at least the size of the largest variant under `#ifdef PORT`, and add `_Static_assert`s so any future variant growth is caught at compile time:

```c
typedef struct ALParam_s {
    …
    s32 unk1C;
#ifdef PORT
    u8  _port_overlay_pad[12];   /* sizeof(ALParam) becomes 48 */
#endif
} ALParam;

#ifdef PORT
_Static_assert(sizeof(ALStartParamAlt) <= sizeof(ALParam), …);
_Static_assert(sizeof(ALStartParam)    <= sizeof(ALParam), …);
_Static_assert(sizeof(ALFreeParam)     <= sizeof(ALParam), …);
#endif
```

Verified by 30-second smoke test on the attract-demo path that previously crashed within a few audio frames; no SIGSEGV, no safety-net hits, audio thread keeps ticking through scene transitions.

## Why memory/docs should have flagged this

This is a sibling of the well-documented LP64 patterns:

- `project_port_token_vs_raw_pointer` — port-side widening of pointer-bearing fields.
- `project_itstruct_arrow_trunc` / `project_mball_effect_filehead_lp64` — pointer-width changes shifting struct offsets.
- `project_file_pointer_array_lp64_stride` — array stride mismatch when a pointer field widens.

The new instance is the *type-overlay* form of the same family: a freelist slot sized to one variant gets cast to a larger variant. Memory note added: when a struct is allocated via a generic header type and downstream code casts to a wider variant, **check `sizeof(variant) <= sizeof(header)` on PORT** — and if it fails, either pad the header or rework the allocator. Static asserts are cheap and prevent this class entirely.

## Open follow-up

Audio still emits silence (no `first non-zero sample detected` log). Crash and corruption are gone, notes successfully look up sounds, but the mixer output buffer stays zero. Next: investigate downstream synth — `dc_memin` truncating `(s32) dc_table->base` casts at `n_env.c:566/628/683` are unwrapped LP64 traps that would feed garbage to the ADPCM decoder; also `port/audio/mixer.c` implementations of the `aXxx` macros. See task #6 and `docs/audio_buildout_2026-04-24.md`.
