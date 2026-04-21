# 32bpp Sprite TMEM Line Swizzle — Wrong Swap Granularity (2026-04-20) — FIXED

**Symptom:** Every RGBA32 sprite rendered with fine horizontal combing / diagonal
shear artifacts. Colors and letter shapes were correct, but the scanline-level
pixel positions were scrambled on odd rows. Affected at least:

- SMASH title-screen logo — 21 strips of `172×4` RGBA32, `file=167` (sprite
  `llMNTitleSmashSprite` in `sMNTitleFiles[0]` created by
  `mnTitleMakeLabels`).
- Fighter-select character portraits — 12 characters × 3 strips of `48×21` +
  `48×21` + `48×3` RGBA32, `file=19` (sprites accessed through
  `llMNPlayersPortraitsFileID` and drawn by `mnplayersvs.c` /
  `mnplayers1pgame.c` `mnPlayers*MakePortrait`).
- Any other sprite with `bmsiz=3` (`G_IM_SIZ_32b`).

Visually identical to the shear pattern that 16bpp sprites had before the
[sprite_texel_tmem_swizzle_2026-04-10](sprite_texel_tmem_swizzle_2026-04-10.md)
fix landed. But that fix explicitly **excluded 32bpp** via a `bpp < 32` guard,
so 32bpp sprites never received any TMEM unswizzle and were left rendered
as-swizzled.

## Prior investigation and what was ruled out

Two dead-end writeups covered this before the root cause was found:

- `docs/smash_title_32bpp_sprite.md` — the SMASH-specific handoff.
- `docs/sprite_broken_textures_handoff_2026-04-19.md` — the broader set,
  which also (incorrectly) lumped in tutorial CI8/RGBA16 artifacts and a
  misidentified "portraits" file (`file=168 = llMNTitleFireAnimFileID`, which
  really is a fire animation — "just fire" was literal, not an artifact).

Those handoffs tried:

- **Fix A — separate tracking sets for struct vs. buf:** no effect. Idempotency
  wasn't the bug.
- **Fix B — apply the existing XOR-4 deswizzle to 32bpp:** swapped adjacent
  pixels on odd rows (the fix was for 16bpp qword halves where one half = 4
  16bpp pixels; applied to 32bpp it means "swap pixel 0 with pixel 1 in every
  8-byte qword"), which produced a different-but-still-wrong result (`reversed
  gradient on odd rows`). Reverted.

The contradiction — "pattern looks like XOR-4 not applied, but applying XOR-4
makes it worse" — was the whole impasse, because **32bpp needs a different
swap granularity than 4b/8b/16b, not just a different on/off flag.**

## Root cause

Sprite `LOAD_BLOCK` sequences in `lbCommonDrawSObjBitmap`
(`src/lb/lbcommon.c:2570-2680`) differ between ≤16bpp and 32bpp:

| bpp    | LoadBlock size                              | SetTile `line`                              | TMEM layout |
|--------|---------------------------------------------|---------------------------------------------|-------------|
| 4/8/16 | `G_IM_SIZ_16b` (all `*_LOAD_BLOCK` alias to 16b) | `G_IM_SIZ_16b`, `line = ⌈w·2/8⌉`            | Linear: one TMEM word per 16bpp-equivalent group |
| 32     | `G_IM_SIZ_32b_LOAD_BLOCK = G_IM_SIZ_32b`         | `G_IM_SIZ_32b`, `line = ⌈w·2/8⌉` (still half) | **Bank-split**: low-bank holds `[R G]` halves, high-bank holds `[B A]` halves; each texel consumes **two** TMEM words |

On real hardware the sampler XORs the TMEM word address with 1 on every odd
row to defeat bank conflicts. At DRAM level that becomes a byte-address XOR:

- 4b/8b/16b: each TMEM word = 4 bytes, one word-address XOR swaps the two
  4-byte halves of an 8-byte qword on odd rows (the existing XOR-4 logic).
- 32b: each texel occupies 2 consecutive TMEM words, so the same word XOR
  spans 8 DRAM bytes. At DRAM level the swap granularity doubles: swap the
  two 8-byte halves of each 16-byte group (= 2 RGBA32 pixels with the next
  2) on odd rows.

`G_IM_SIZ_32b_LINE_BYTES = 2` (not 4, see `include/PR/gbi.h:455`) and the
compensating `line_size *= 2` at `libultraship/src/fast/interpreter.cpp:2149-
2151` both corroborate this bank-split layout — the tile's line stride is
encoded at half the real DRAM stride precisely because the encoding is in
low-bank 2-byte words.

Fix B applied the **4b/8b/16b** granularity (8-byte group, 4-byte half) to
32bpp. That swaps pixel 0 with pixel 1 in every qword on odd rows, which
scrambles the data further. The correct 32bpp granularity is **16-byte group,
8-byte half** (2-pixel pair swap).

## Verification

Offline analysis of the post-BSWAP32 TGA dumps produced by
`tex_dump_known_dims` (see `SSB64_TEX_DUMP=1`):

```
python3 ... neighbor_similarity test (avg adjacent-row byte delta, lower = smoother)

                SMASH strip 10         Portrait 0
  no swizzle:       29.88                 29.88
  XOR-4  (qw halves, 1-pixel swap):  28.86   (barely changes)
  XOR-16 (group halves, 2-pixel swap):  9.05   (3× smoother)
  XOR-32:           35.90   (worse)
```

Visual confirmation via
`debug_traces/sprite_deswizzle_32bpp/{SMASH,Portraits}_*.png` — XOR-16
produces readable SMASH letters and recognisable fighter-head portraits;
XOR-4 produces the documented "reversed gradient / adjacent-pixel scramble"
artifact.

## Fix

Extended the existing sprite deswizzle in `portFixupSpriteBitmapData`
(`port/bridge/lbreloc_byteswap.cpp`) to pick `group_size` based on bpp:

```cpp
size_t group_size = (bpp == 32) ? 16 : 8;
size_t half       = group_size / 2;
```

The loop then swaps `half`-byte halves of each `group_size`-byte chunk on
every odd row, same as before. `bpp < 32` guard removed so 32bpp now falls
through the same path; 4c compressed is still excluded (separate post-decode
deswizzle).

## Files

- `port/bridge/lbreloc_byteswap.cpp` — `portFixupSpriteBitmapData`, extended
  deswizzle for bpp=32.
- `docs/smash_title_32bpp_sprite.md` — superseded by this writeup.
- `docs/sprite_broken_textures_handoff_2026-04-19.md` — superseded by this
  writeup for items 1, 5 (SMASH, portraits). Items 2–4 (tutorial CI8/RGBA16
  "How to Play" / "HereText" / textbox) and item 6 (unknown "player card") are
  separate investigations; the 32bpp fix does not address them.

## Diagnostic tooling (kept for future swizzle questions)

- `SSB64_TEX_DUMP=1` writes post-BSWAP, pre-deswizzle TGAs into `tex_dump/`
  using the sprite's real `width_img × actualHeight`.
- `debug_traces/sprite_deswizzle_32bpp/` holds the before/after PNGs
  generated during this investigation. The generator script was ad-hoc
  (`uv`-based venv with `pillow`, inline in a shell heredoc) — see this
  doc's Verification section for the neighbor-similarity metric idea.

## Class-of-bug lesson

When porting an RDP feature that claims to be format-independent ("all formats
use the same XOR-4 swizzle"), check the per-format **TMEM word size** first.
The `G_IM_SIZ_*_LINE_BYTES` / `*_LOAD_BLOCK` macros are the smoking gun —
any format whose `LOAD_BLOCK` alias differs, or whose `LINE_BYTES` doesn't
match its bytes-per-pixel, is a candidate for a different DRAM layout even
when the GBI commands look identical.
