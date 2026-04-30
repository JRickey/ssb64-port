# Audio fgm_unk flat-array parse + f32 byteswap (2026-04-30)

## Symptom

The Star-KO chime (FGM `0xc`, the rising chime that plays when a fighter is launched into the top blast zone) sounded as a sustained high-pitched ring instead of a clean rising chime. Other SFX exhibited similar "no decay / hold to end" symptoms but the Star-KO chime made it most audible because of its 150-frame sustain.

## Root cause

`fgm_unk` is one of three FGM packages loaded by `portAudioLoadAssets`. Decompilation initially treated it the same as `fgm_tbl` and `fgm_ucd` — a `parsePackage`-style blob with a count word followed by a u32 pointer-table, where each entry points at a payload elsewhere in the blob. Under that interpretation, `unk44` was set to `pkg->data` (the pointer table itself).

The actual consumer side (`func_80026A6C_2766C` and friends in `n_env.c`) reads `D_8009EDD0_406D0.unk_alsound_0x24[i]` with `sizeof(ALWhatever8009EE0C_3) = 16`-byte stride. So when `unk44` was a `uintptr_t[count]` pointer table at 8-byte stride, indexing `[i]` for `i ≥ 1` walked into the pointer table's tail and then into adjacent heap. Most entries' u8 fields read as low bytes of pointers (sometimes valid-looking, mostly garbage); the f32 fields read as high bytes of pointers (typically values like `0x00000001` or `0x00000000` — denormal/zero floats that produce a flat envelope shape).

Result: every SFX voice played at constant amplitude (no envelope decay) for the duration of its bytecode. Most SFX are short enough that this isn't audible; the Star-KO chime's long sustain made it ring continuously.

## Layout (verified by diagnostic dump)

`fgm_unk` is a flat array of 16-byte structs with a single u32 header — **no offset table**:

```
[BE u32 count][16-byte struct[count]]
```

Each struct is `ALWhatever8009EE0C_3`:

```c
typedef struct {
    u8  unk0, unk1, unk2, unk3;
    f32 unk4, unk8, unkC;
} ALWhatever8009EE0C_3;
```

The four u8 fields are byte-ordered as-is in the ROM (no per-byte swap needed on LE). The three f32 fields are stored BE and need per-word `bswap32` on LE.

For our extracted blob: `count = 100`, structs occupy `100 × 16 = 1600` bytes starting at file offset 4. Blob size 2096 bytes leaves 492 bytes of trailing padding.

## Fix

`portAudioLoadAssets`'s `fgm_unk` branch:

1. Allocate `count × 16` bytes from the audio heap.
2. For each struct: copy the 4 u8 fields raw, `bswap32` each of the 3 f32 fields.
3. Set `unk44` to the flat-array base (with bounds check on `count` and `4 + count*16 ≤ blob_size`).

Bounds rejected → emit an `spdlog::error` and skip; SFX path silently degrades to the unfixed-but-non-crashing state.

## Earlier draft

A previous draft assumed an interleaved offset-table layout (`[count][offset[count]][structs]` like `fgm_tbl`/`fgm_ucd`). Reading bytes 4-7 as a BE u32 offset returned `0x070C0000` (which is actually `struct[0]`'s u8 fields interpreted as a BE u32 — `unk0=0x07, unk1=0x0C, unk2=0x00, unk3=0x00`). The first iteration's `raw + 0x070C0000` faulted, producing **SIGBUS at audio thread setup** on every launch (exit code 138 on macOS). That draft was reverted; this fix uses direct file-offset stride, matching the actual layout.

## Verification

- Build: clean.
- Boot: no SIGBUS at audio thread creation. `audio_bridge: fgm_unk parsed as flat envelope array (100 entries, 1600 bytes)` log line confirms successful parse.
- Game flow: scenes 27 → 28 → 1 → 7 → 8 → 9 → 16 (VS character select) traversed without crash.
- The original audio symptom (Star-KO chime ringing) was diagnosed by the audio agent before they wrote this fix; verifying the audible result in-game is left as a follow-up by anyone who can drive a Star-KO scenario.

## Class

Layout-mismatch via "looks like a similar package, must use the same parser." `fgm_unk` shares the `[count][...]` header pattern with `fgm_tbl`/`fgm_ucd` but the rest of the layout is genuinely different. Diagnostic dumping the first few entries' bytes is the way to verify before assuming.
