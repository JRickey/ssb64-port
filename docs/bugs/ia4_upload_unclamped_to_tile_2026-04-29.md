# IA4 Upload Width Not Clamped to SetTileSize (2026-04-29) — FIXED

**Symptom (issue #2):** VS Record screen digits ("0–9") render as
near-vertical strokes only ~1 pixel wide instead of the full 4-pixel
glyph shapes. The digit columns and grid layout are otherwise correct
— only the digit *content* is squished horizontally. Reference image
on N64 hardware / Mupen+GLideN64 shows full-width 4×7 digits.

## Root cause

VS Record digit sprites are 4×7 IA4 tiles. The DL sequence the game
emits per digit is:

```
G_SETTIMG          fmt=IA siz=16b w=1   seg=10 off=...
G_SETTILE tile=7   fmt=IA siz=16b line=0 tmem=0 cms=2 cmt=2   ; LOAD tile
G_LOADBLOCK tile=7 ul=(0,0) texels=28 dxt=0                   ; 56 bytes
G_SETTILE tile=0   fmt=IA siz=4b  line=1 tmem=0 cms=2 cmt=2   ; RENDER tile
G_SETTILESIZE tile=0 ul=(0,0) lr=(3.0,6.0)                    ; 4×7 clamp
G_TEXRECT tile=0 (62,65)-(66,72)                              ; 4×7 dest
```

The render tile's `line=1` means 8 bytes per TMEM line, which at IA4
(0.5 byte/px) is 16 pixels wide. So TMEM holds a 16×7 buffer with the
useful 4×7 digit in the leftmost columns and the rest unused. The
SetTileSize tells the RDP to clamp rendering to the leftmost 4×7
square.

`Interpreter::ImportTextureIA4` in `libultraship/src/fast/interpreter.cpp`
took the line stride (8 bytes → 16 pixels wide) and uploaded that as
the GPU texture width *without* applying the SetTileSize clamp:

```cpp
uint32_t width = widthBytes * 2;            // 16, the unclamped natural width
...
mRapi->UploadTexture(mTexUploadBuffer, width, height);   // GPU texture is 16×7
```

`GfxSpTri1` (and `GfxDpTextureRectangle` via the same path) then
normalises UVs by the *clamped* `tex_width = 4`. UVs running 0..lrs=4
divided by `tex_width=4` produce normalised u ∈ [0,1]. The GPU sampler
multiplies that back by the GPU texture width (16), so the four output
pixels of the TexRect end up sampling GPU columns ≈ 0, 4, 8, 12 of a
16-column upload whose *useful* digit data lives in columns 0–3. Three
of the four output pixels miss the digit entirely; only column 0 of
the digit is ever visible — hence the "squished, ~1 pixel wide" look.

This is the same root cause and same fix shape as
[title_border_right_edge_slice_2026-04-14](title_border_right_edge_slice_2026-04-14.md)
(IA8 / I4) and
[sprite_decode_stride_mismatch_2026-04-20](sprite_decode_stride_mismatch_2026-04-20.md)
(RGBA16 / RGBA32 / CI8). IA4 was simply missed in those passes.

## Fix

`libultraship/src/fast/interpreter.cpp` — apply
`ClampUploadWidthToTile` to the IA4 path, mirroring the IA8 / I4
implementation:

```cpp
uint32_t naturalWidth = widthBytes * 2;
...
if (fullImageLineSizeBytes == sizeBytes) {
    fullImageLineSizeBytes = widthBytes;     // unchanged — source stride
}

uint32_t width = ClampUploadWidthToTile(naturalWidth,
                                        mRdp->texture_tile[tile].uls,
                                        mRdp->texture_tile[tile].lrs);
```

`naturalWidth` (the TMEM-line-derived 16 in our digit case) stays the
source-indexing stride for the decode loop's `clrIdx` calculation, but
the upload width — and the inner `for x in [0, width)` extent — comes
from the SetTileSize clamp, so the GPU receives a 4×7 texture and the
UV normalisation is consistent end-to-end.

## Class of bug — audit target

Same hazard as the previous Clamp fixes: any `ImportTexture*` that
does

```
uint32_t width = (function of widthBytes);
... loop bound by `width` ...
mRapi->UploadTexture(buf, width, height);
```

without first clamping `width` to the SetTileSize extent will render
"squished" output for any sprite where TMEM line stride is wider than
the SetTileSize clamp.

Remaining unclamped paths in this file (still latent — fix
defensively if a similar regression surfaces):

- `ImportTextureI8` (line ~1410) — uses raw `width = GetEffectiveLineSize(...)`
  without clamp.
- `ImportTextureIA16` (line ~1305) — uses raw `width = widthBytes / 2`
  without clamp.

These will exhibit the same squish for any sprite whose `bitmap->width
< bitmap->width_img`. Not patching them in this commit because no
in-game asset has been observed using them with a clamp narrower than
the natural width — but the fix shape is mechanical when one shows up.

## Files

- `libultraship/src/fast/interpreter.cpp` — `ImportTextureIA4` patched
  to call `ClampUploadWidthToTile`.

## Not related to

- Issue #1 (training-mode sprite init writes pre-fixup) — those were
  field-offset bugs in C code; this is a Fast3D decode-side bug.
