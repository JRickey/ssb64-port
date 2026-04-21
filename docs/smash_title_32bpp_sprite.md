# SMASH Title Screen 32bpp Sprite — Rendering Artifact

## Status: **RESOLVED** — see [docs/bugs/sprite_32bpp_tmem_swizzle_2026-04-20.md](bugs/sprite_32bpp_tmem_swizzle_2026-04-20.md).

Root cause: 32bpp sprites needed their own TMEM-line-swizzle granularity
(16-byte group, 8-byte half = 2-pixel pair swap on odd rows), not the
4b/8b/16b 8-byte-group / 4-byte-half granularity that was being tested. The
`bpp < 32` guard in `portFixupSpriteBitmapData` now falls through to a
bpp-dependent group size. The rest of this file is kept for historical
context only.

---

## (original, superseded) Status: OPEN — root cause unknown; two fixup approaches tried and ineffective

## Symptom

The "SMASH" text on the title screen (scene 1, `nSCKindTitle`) renders with fine horizontal combing/interlacing lines through the letters. The gradient colors (red top to yellow bottom) and letter shapes are correct — only the scanline-level structure is wrong. All other sprites on the title screen (SUPER, BROS, TM, copyright, background wallpaper) render correctly.

The visual pattern is similar to the XOR-4 TMEM deswizzle artifact seen on other sprites before that fix was applied.

## Sprite Details

- Source: `llMNTitleSmashSprite` in `sMNTitleFiles[0]`, created via `lbCommonMakeSObjForGObj` in `mnTitleMakeLabels`
- Format: **RGBA32 (bmsiz=3, bpp=32)**
- Layout: 21 bitmap strips, each `w=172 h=4`, total sprite ~172x84
- The sprite scales during the title animation (gets larger, shrinks back)

## Confirmed Facts

### portFixupSpriteBitmapData IS called

`SSB64_TEX_FIXUP_LOG=1` output from a title-screen session (file=167) shows all 21 RGBA32 bitmaps logged as `[sprite.bitmap]` entries (bpp=32, w=172, h=4). BSWAP32 is applied. `first16=FF001100FF001100...` — correct N64 big-endian RGBA byte order.

### BSWAP is correct

Pixel bytes like `FF0011FF` decode to R=255,G=0,B=17,A=255 (opaque dark-red), consistent with the visible gradient. Fast3D's `ImportTextureRgba32` reads `addr[0]=R, addr[1]=G, addr[2]=B, addr[3]=A` sequentially.

### XOR-4 TMEM deswizzle is NOT applied to 32bpp — this is correct

`include/PR/gbi.h` defines:
```c
#define G_IM_SIZ_32b_LOAD_BLOCK  G_IM_SIZ_32b   // NOT G_IM_SIZ_16b
```

4b/8b/16b sprites all use `G_IM_SIZ_*_LOAD_BLOCK = G_IM_SIZ_16b`, which triggers the DRAM pre-swizzle. 32bpp uses its own load format (`G_IM_SIZ_32b`), which the hardware handles differently. The XOR-4 deswizzle in `portFixupSpriteBitmapData` correctly excludes `bpp >= 32` via the `bpp < 32` guard.

Empirical test confirmed: applying XOR-4 to 32bpp data reversed the gradient on odd rows, corrupting the image.

### `line_size *= 2` fix in interpreter.cpp is present and correct

`G_IM_SIZ_32b_LINE_BYTES = 2` means the SetTile `line` field is half the actual DRAM stride. The fix at `libultraship/src/fast/interpreter.cpp:2149-2152` doubles `line_size` for G_IM_SIZ_32b tiles before computing tex_width/tex_height:
- line=43 bytes → ×8 = 344 → ×2 = 688 → ÷4 = tex_width=172 ✓
- tex_size_bytes=2752 / 688 = tex_height=4 ✓

This fix is necessary and correct. It predates the current investigation.

## Approaches Tried (Both Ineffective)

### Fix A: Separate tracking sets for portFixupSprite / portFixupBitmap

`portFixupSprite` and `portFixupBitmap` both used `sStructU16Fixups` for idempotency, keyed on the struct's base address. Since `sStructU16Fixups` also tracks bitmap buf addresses for BSWAP, a struct pointer coinciding with a buf address could theoretically suppress a BSWAP. Added `sSpriteTouched` and `sBitmapStructTouched` sets to separate these concerns. Applied, tested, no visible change. **Reverted to HEAD.**

### Fix B: Apply XOR-4 deswizzle to 32bpp

Removed the `bpp < 32` guard so XOR-4 applies to RGBA32 bitmaps. Applied, tested, no visible change. Confirmed analytically incorrect per the G_IM_SIZ_32b hardware path. **Reverted to HEAD.**

## Current Code State

All code is at HEAD. No pending changes. The deswizzle condition in `portFixupSpriteBitmapData` is:
```cpp
if (bpp > 0 && bpp < 32 && bmsiz != 4 && width_img > 0 && actualHeight > 0 && !sDeswizzle4cFixups.count(key))
```
This is correct. No change needed here.

## Files Involved

- `src/mn/mncommon/mntitle.c` — title screen sprite creation
- `src/lb/lbcommon.c:2629-2675` — `lbCommonDrawSObjBitmap` 32bpp LOAD_BLOCK + SetTile GBI sequence
- `include/PR/gbi.h:455,460` — `G_IM_SIZ_32b_LINE_BYTES=2`, `G_IM_SIZ_32b_LOAD_BLOCK=G_IM_SIZ_32b`
- `libultraship/src/fast/interpreter.cpp:2143-2173` — `line_size *= 2` for G_IM_SIZ_32b (present, correct)
- `libultraship/src/fast/interpreter.cpp:863-912` — `ImportTextureRgba32`
- `port/bridge/lbreloc_byteswap.cpp` — `portFixupSpriteBitmapData` (BSWAP + deswizzle logic)

## Next Investigation Steps

1. Capture `SSB64_TEX_FIXUP_LOG=1` log while rendering the SMASH sprite specifically (not just title scene load — include animation frames where combing is visible).
2. Capture `SSB64_GBI_TRACE=1` GBI trace for the SMASH draw call and diff against expected N64 reference to check for dimension, format, or load sequence discrepancies.
3. Investigate `ImportTextureRgba32` in interpreter.cpp — verify it correctly handles the flat LOAD_BLOCK case (width=1, full strip in one block) for all 21 bitmaps. Check whether `GetEffectiveLineSize` returns 688 consistently.
4. Check whether the combing is a UV mapping issue (wrong tex_width/tex_height at render time) vs. a texel data issue (wrong bytes reaching Import).
