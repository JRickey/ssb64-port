# Sprite Trailing-Column Bleed — Fighter-Intro "MARIO" Title Card

**Date:** 2026-04-11
**Files touched:** `port/bridge/lbreloc_byteswap.cpp` (`portFixupSpriteBitmapData`)

## Symptom

Fighter-intro title cards in `mvOpening<Fighter>` scenes (the per-letter
"MARIO", "FOX", "PIKACHU", … sprites drawn by `mvOpening<Fighter>MakeName`)
rendered with faint white/gray horizontal smears extending to the right of
every letter. The letter shapes themselves were correct — these were thin
tails, most obvious along the bottom serif of each glyph. The earlier
`portFixupSpriteBitmapData` deswizzle (commits `c8c855d` and `8fe90df`) had
fixed the shear on these same sprites but left this trailing-edge artifact
behind.

## Root cause

It is **not** a deswizzle bug — the existing XOR4 odd-row unswizzle produces
clean letter shapes. The problem is that each font sprite has two widths:

| Letter | `bitmap->width` (draw) | `bitmap->width_img` (storage) |
|--------|------------------------|-------------------------------|
| M      | 37                     | 40                            |
| A      | 35                     | 40                            |
| R      | 27                     | 32                            |
| I      |  9                     | 16                            |
| O      | 34                     | 40                            |
| F      | 20                     | 24                            |
| N      | 29                     | 32                            |
| …      | …                      | …                             |

`width_img` is the DRAM/TMEM row stride (rounded up for qword alignment and
to satisfy `LOAD_BLOCK`'s pixel-count encoding). `width` is the tile's drawn
extent. The extra columns at the right of each row are *unused* in the
N64's render pipeline — `lbCommonDrawSObjBitmap` calls
`gDPSetTileSize(…, (width-1) << 2, …)`, so the sampler clamps sampling to
`s ∈ [0, width-1]` and those trailing bytes are never read on real hardware.

But the on-disk font data is **not** zero in those columns. A survey after
the current deswizzle runs:

```
Letter   w   w_img   trailing-nonzero-bytes
  M     37     40            21
  A     35     40            15
  R     27     32            10
  I      9     16           252   ← nearly all 7 unused cols are non-zero
  O     34     40            66
  F     20     24            16
  X     31     32             2
  K     27     32            35
  Y     29     32             6
  N     29     32           111
  D     28     32            48
  L     20     24            12
```

Letter M's trailing bytes contain faint-alpha gray — the continuation of
the letter's bottom serif across rows 29–35. Letter I's trailing bytes
contain `0x0F` (intensity 0, alpha F) — fully-opaque-black padding, visible
as gray when the linear sampler averages it with a white letter pixel at
the edge.

On PC, Fast3D's bilinear filter fetches one texel *past* the rightmost
sampled pixel while rendering the right edge of the letter — that's
fundamental to how bilinear filtering works at tile boundaries. The extra
texel lands in the unused column, its non-zero value gets averaged with the
letter pixel, and the result is a faint smear one texel wide bleeding
rightward out of the letter. Visible across every letter in the screenshot;
particularly obvious along the baseline of each glyph.

On real N64 hardware the tile-clamp machinery kills the read before the
sampler ever touches the unused column, so the smear does not appear.

## Fix

> **Superseded:** the port-side trailing-column fix has been **removed**.
> The actual root cause was a Fast3D upload/UV-normalisation mismatch in
> libultraship (GPU texture uploaded at `width_img`, UV normalised by
> `width`, producing a subtle right-edge stretch).  The fix now lives in
> `libultraship/src/fast/interpreter.cpp` as a new
> `ClampUploadWidthToTile` helper called from `ImportTextureIA8` and
> `ImportTextureI4`.  See `docs/bugs/title_border_right_edge_slice_2026-04-14.md`
> for the full writeup.  The two earlier port-side revisions (zero-fill
> and edge-replicate) are kept below for historical reference — both
> were workarounds around Fast3D's stretch and produced conflicting
> regressions (zero-fill dimmed the title-screen border; edge-replicate
> brought the MARIO smear back because letter M's rightmost col and its
> trailing serif bytes happen to be identical).  With the libultraship
> fix, cols `[width, width_img)` of the source data are never read and
> no port-side manipulation is needed.

## Diagnostic workflow

Offline reproduction lives in `debug_tools/sprite_deswizzle/sprite_deswizzle.py`,
modeled on the N64 logo / stage background workflow from commit `c8c855d`.

Usage:

```bash
python3 debug_tools/sprite_deswizzle/sprite_deswizzle.py letter baserom.us.z64 M
```

The script:
1. Uses `reloc_extract.py` to pull `IFCommonAnnounceCommon` (file_id 37)
   straight from the ROM, including VPK0 decompression.
2. Walks the intern reloc chain to resolve `Sprite->bitmap` → `Bitmap[]` and
   each `Bitmap->buf` → raw texel byte range, entirely from the big-endian
   on-disk representation (no runtime byteswap simulation needed).
3. Applies a configurable deswizzle strategy (`none`, `port_current`,
   `xor4`, `xor2`, `xor1`, `xor8`) per odd row.
4. Decodes IA8/IA4/RGBA16/… to RGBA and writes a PNG via Pillow.

Supports three modes:
- `letter`   — IFCommonAnnounceCommon per-letter sprites (file 37, offsets
               `0x05E0`–`0x7AE8` for A–Z).
- `portrait` — 300×55 RGBA16 multi-strip fighter portraits from
               `MVOpeningPortraitsSet1/Set2` (file ids 0x35, 0x36).
- `charname` — `CharacterNames` per-fighter name sprites (file id 0x0C),
               used by the title-screen auto-demo.

Verification artifacts generated during this investigation:

- `debug_traces/sprite_deswizzle/M_analysis_fullwidth.png` — M after
  `port_current` deswizzle, no mask. Dark background; the faint smear
  extending from the bottom-right of the letter past col 37 is visible.
- `debug_traces/sprite_deswizzle/M_analysis_bleed_zeroed.png` — same
  deswizzle + the trailing-column mask applied. Clean letter, no tail.

## Lessons

1. **"Deswizzle produces the right letter" ≠ "the texture is fit to render."**
   The XOR4 fix landed the pixels in the right places but left a strip of
   live data that Fast3D was happy to sample. The test that would have
   caught this is "run the deswizzled bytes through a hex dump and check
   the bytes at `col ∈ [width, width_img)` for non-zero content," which
   trivially flags this bug across every letter in the font.

2. **Port-only bugs can look like pipeline bugs.** The in-game screenshot
   shows a stable, consistent artifact that looks very much like incorrect
   filtering or a swizzle off-by-one. The actual cause was authentic N64
   texture data that the hardware quietly clipped away — a PC-only problem
   created by the bilinear filter's boundary behavior. When the
   deswizzle-dump PNG looked clean at the letter's visible width but had
   data beyond it, that was the tell.

3. **The surgical fix is the right one.** An alternative would have been
   to dig into Fast3D's `gfx_direct3d11` / `gfx_metal` sampler clamp and
   fix the tile-boundary behavior there. That's a much bigger change,
   affects other code paths, and is hard to verify across all backends.
   Zeroing the bytes in the port fixup path is local, targeted, and
   semantically identical to the hardware's tile clamp (neither hardware
   nor mask touches those texels at render time).
