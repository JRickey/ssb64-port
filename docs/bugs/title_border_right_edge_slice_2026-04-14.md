# Title-Screen Border — Missing Right-Edge Slice & MARIO Title-Card Smear

**Date:** 2026-04-14
**Files touched:** `libultraship/src/fast/interpreter.cpp` (`ImportTextureIA8`, `ImportTextureI4`, new `ClampUploadWidthToTile` helper); `port/bridge/lbreloc_byteswap.cpp` — revert of the ce89700 trailing-column workaround.
**Supersedes:** `ce89700` ("Zero sprite trailing columns to kill MARIO title-card bleed") — the workaround is removed because the root cause is fixed in Fast3D.
**Related:** [sprite_trailing_column_bleed](../sprite_trailing_column_bleed.md)

## Symptom

Two bugs with the same underlying cause:

1. **Title-screen top border** — `llMNTitleBorderUpperSprite` in `src/mn/mncommon/mntitle.c:95`, a 300×10 I4 sprite drawn at `(160, 15)`, rendered with a ~1-texel vertical slice of its rightmost column visibly dimmed. Introduced by the ce89700 workaround.
2. **Fighter-intro title cards** ("MARIO", "FOX", "PIKACHU", …) — per-letter IA8 sprites from `IFCommonAnnounceCommon` (file 37) drawn by `mvOpening<Fighter>MakeName`. Rendered with faint horizontal smears extending right out of every letter. Fixed by ce89700 by zero-filling trailing columns, then the fix was revised to edge-replicate, which fixed the border but reintroduced the letter smear.

## Root cause

Fast3D uploads sprite textures at the **TMEM line stride** (rounded up from `bitmap->width` to qword alignment, which is typically `bitmap->width_img`), but `GfxSpTri1` normalises UV coordinates by the **SetTileSize-clamped** `tex_width[t]` = `(texture_tile.lrs - texture_tile.uls + 4) / 4` = `bitmap->width`.

Concretely, for letter M (IA8, `width=37`, `width_img=40`):
- `ImportTextureIA8` uploads a **40×37** GPU texture by iterating `x ∈ [0, 40)` against source stride `full_image_line_size_bytes = 40`.
- `GfxSpTri1` at line 2135 clamps `tex_width[i] = min(40, tex_width2[i]) = 37`.
- At the rightmost vertex, `v_arr[i]->u = 37 * 32` (S10.5), so `normalized_u = 37 / tex_width[i] = 37/37 = 1.0`.
- OpenGL then samples `normalized_u * GPU_width = 1.0 × 40 = col 40` (clamped to 39 by `GL_CLAMP_TO_EDGE`).

That is, the rightmost drawn output pixel samples **GPU col 39** instead of the intended **col 36** — a ~40/37 horizontal stretch that pulls the trailing-column bytes into the render. The MARIO smear was those bytes being visible, the border dim was the bilinear filter's interaction with the zero-fill workaround.

### Offline proof

Letter M (IA8) post port_current deswizzle, row 33:

```
col  35  36  37  38  39
val  7F  0F  0F  0F  0F   ← cols 37..39 hold serif-continuation bytes
```

With Fast3D's stretch, screen pixel 36 of the drawn M samples GPU col ~38.9, so the "tail" of the serif (which on N64 hardware is clamped away) becomes visible as the rendered right-edge pixel.

Border row 0 (I4), cols 295..303 after decoding:

```
zero-mode  (ce89700):  221 221 221 221 221   0   0   0   0
edge-mode  (revision): 221 221 221 221 221 221 221 221 221
```

Zero-mode leaves a brightness cliff between pixel 299 and 300; Fast3D's bilinear sampler at the stretched right edge averages the cliff and produces the dim sliver. Edge-mode preserves the border — but the same replication for letter M is a no-op because cols 37..39 already equal col 36 in the serif rows, so the MARIO smear returns.

## Fix

Clamp the upload width to the SetTileSize extent in `ImportTextureIA8` / `ImportTextureI4` so the GPU texture is exactly `width` pixels wide instead of `width_img`.

New helper `ClampUploadWidthToTile(naturalWidthPixels, tile_uls, tile_lrs)` in `interpreter.cpp` returns `min(naturalWidthPixels, (tile_lrs - tile_uls + 4) / 4)`. Call it right after `GetEffectiveLineSize` and use the result as the inner loop's `width`, keeping `full_image_line_size_bytes` as the source stride so the loop iterates over only the drawable cols.

With the upload clamped, every pipeline stage agrees on the same width:
- GPU texture is `width × height`.
- `GfxSpTri1`'s `tex_width[i]` clamp is a no-op (already matches).
- UV normalisation at the right edge maps to GPU col `width - 1`.
- `GL_CLAMP_TO_EDGE` handles any bilinear fetch past `width - 1` by returning the edge pixel — exactly N64 tile-clamp behaviour.

The fix is applied in two format paths for now (IA8 for font glyphs, I4 for the border). Other `ImportTexture*` functions should grow the same call if analogous regressions surface — the mechanical fix is to wrap whatever width they compute via `GetEffectiveLineSize` with `ClampUploadWidthToTile`, preserving the unclamped stride for source indexing.

## Why the port-side workaround is removed

With the Fast3D upload clamped, cols `[width, width_img)` of the sprite data are never read. Whatever value they hold — original serif continuation, zero-filled, or edge-replicated — has no effect on rendering. `portFixupSpriteBitmapData`'s trailing-column fix from ce89700 is deleted; only the earlier BSWAP + TMEM-unswizzle block remains.

## Files

- `libultraship/src/fast/interpreter.cpp` — `ClampUploadWidthToTile` helper + calls from `ImportTextureIA8`, `ImportTextureI4`.
- `port/bridge/lbreloc_byteswap.cpp` — trailing-column block removed from `portFixupSpriteBitmapData`.
- `docs/sprite_trailing_column_bleed.md` — marked superseded, points here.
