# CSS Gate Palette Overlap Fixup

## Summary

**Resolved issue #3.** Character Select gate panels could render with garbled colors or patterns depending on previous HMN/CPU settings and scene re-entry history.

## Symptoms

- The panel behind a player slot could pick up colors or stripes that belonged to a previous panel state.
- Re-entering Character Select or changing a slot between HMN and CPU made the result state-dependent.
- Reusing the same red-card bitmap while swapping only `Sprite.LUT` made the failure look like a stale texture-cache or TLUT invalidation problem.

## Root Cause

The CSS gate palettes are stored as adjacent small TLUT blocks in `MNPlayersCommon`, roughly `0x28` bytes apart. The red-card `Sprite` asks Fast3D to load a 256-entry TLUT, so each panel requests a 512-byte runtime palette fixup even though the meaningful CI4 palette data is much smaller.

`portRelocFixupTextureAtRuntime` was idempotent only by the request's starting address. Loading panel 1 fixed a range that overlapped panel 2/3/4 palette bytes. Loading panel 2 then started at a different address, so the old guard treated it as new work and byte-swapped the overlap a second time, putting those palette bytes back into the wrong order. Later panels repeated the same overlap/toggle pattern, which explains why the bug depended on previous player-kind state and scene history.

The earlier TLUT invalidation change in libultraship is still correct for decoded CI texture cache behavior, but by itself it did not resolve issue #3 because the palette bytes being re-imported were already corrupted by overlapping runtime fixups.

## Fix

Runtime texture/TLUT fixups now track fixed texture words individually in `sTexFixupWords`, mirroring the per-vertex idempotency used by `portRelocFixupVertexAtRuntime`. Overlapping loads from different starts skip words already fixed by an earlier load, while still allowing genuinely new words in a larger or shifted request to be fixed.

The fix also preserves the existing exact-base skip for pass2/chain-fixed texture data and continues clipping TLUT fixup ranges before protected sprite/bitmap struct ranges.

## Verification

- User re-tested the CSS panel repro and confirmed the issue is fixed.
- Built successfully:

```sh
cmake --build /Users/jackrickey/Dev/ssb64-port/build --target ssb64
```

Signed-off-by: GPT-5.5 <gpt-5.5@openai.com>
