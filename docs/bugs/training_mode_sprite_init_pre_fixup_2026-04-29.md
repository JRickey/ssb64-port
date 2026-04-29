# Training Mode Sprite Init Writes Land Pre-Fixup (2026-04-29) — FIXED

**Symptom (issue #1):** Training mode runs, but most of the per-frame
sprites render wrong. Damage `%` digits and combo digits have wrong
tint / missing alpha and several digits don't render at all. The
top-right held-item indicator (brackets + item icon, or "NONE" when
empty) is gone entirely. The CP behavior, game speed, and view-mode
display sprites are similarly off. Inside the pause menu, only the
first option of each row renders correctly; the others vanish or render
garbage data depending on what the menu was last in. ([Reporter
screenshots in
issue #1](https://github.com/JRickey/BattleShip/issues/1).)

The wallpaper, the four "DAMAGE/COMBO/ENEMY/SPEED" labels, and the
orange menu-row labels (which go through a different code path) all
render correctly — narrowing the bug to the
`display_option_sprites` and `menu_option_sprites` arrays.

## Root cause

Training mode stores its per-glyph / per-option sprites as two arrays
of reloc tokens:

```c
// src/sc/sctypes.h (PORT branch)
u32 *display_option_sprites;   // 39 entries — digits 0–9, item icons,
                               // speed/CP/view labels, ENEMY anchor
u32 *menu_option_sprites;      // 31 entries — pause-menu options +
                               // arrows + cursor
```

Both arrays come straight from a reloc file. Pass1's blanket u32
BSWAP32 has run by the time C code touches the sprites, so each Sprite
struct is in a "bytes scrambled within each u32 word" intermediate
state. The fields are restored to native LE only when
`lbCommonMakeSObjForGObj` runs `portFixupSprite` (rotate16 on each
{s16, s16} word, bswap32 on each u8 quad word).

The training-mode init pass writes scalar fields **before** any
`lbCommonMakeSObjForGObj` call:

```c
// sc1PTrainingModeInitStatDisplayCharacterSprites — runs first, on all 39 sprites
sprite->red   = 0x6C;  // offset 0x18
sprite->green = 0xFF;  // offset 0x19
sprite->blue  = 0x6C;  // offset 0x1A
sprite->attr  = SP_TEXSHUF | SP_TRANSPARENT;  // offset 0x14

// sc1PTrainingModeInitMenuOptionSpriteAttrs — same hazard, all 31 menu sprites
sprite->attr  = SP_TEXSHUF | SP_TRANSPARENT;
```

In post-pass1 / pre-fixup byte order, those C field offsets point at
the *original* alpha (0x18 holds what was at file-offset 0x1B), original
blue (0x19 ← 0x1A), original green (0x1A ← 0x19), and original zdepth
(0x14 ← original 0x16). The Init writes therefore land in the wrong
file slots.

`lbCommonMakeSObjForGObj` later runs `portFixupSprite` on whichever
sprite it's first handed (digits[0] for the damage/combo/speed/etc.
displays). Its bswap32 of word 6 (the rgba quad) and rotate16 of word 5
({attr, zdepth}) shuffle the bytes by exactly the inverse of the
pass1 swap — moving the writes one final time. Net result:

| Field write              | Lands post-fixup in        |
|--------------------------|----------------------------|
| `sprite->red = 0x6C`     | `sprite->green` byte       |
| `sprite->green = 0xFF`   | `sprite->blue` byte        |
| `sprite->blue = 0x6C`    | `sprite->alpha` byte       |
| `sprite->attr = 0x0201`  | `sprite->zdepth`           |
| (untouched original alpha) → `sprite->red`   |
| (untouched original attr)  → `sprite->attr`  |

So the SObjs end up using the **ROM's original** attr (which lacks
`SP_TRANSPARENT` / `SP_TEXSHUF` for the digit / option sprites — hence
"missing alpha", "no tint") with arbitrary RGB, and `alpha = 0x6C`.

Two compounding hazards on top of the wrong-offset write:

1. **`portFixupSprite` is idempotent per Sprite pointer.** Once a
   single sprite from `display_option_sprites` is handed to
   `lbCommonMakeSObjForGObj`, every subsequent bswap32/rotate16 pass on
   *that one* sprite is skipped. The other 38 entries never reach the
   Make path — they're only ever consumed via `sobj->sprite =
   *display_option_sprites[modulo]` struct copies in
   `sc1PTrainingModeUpdateDamageDisplay`,
   `sc1PTrainingModeUpdateComboDisplay`, etc. — so they stay in
   post-pass1 byte order forever. That's why the held-item indicator
   (`[37]`, `[36]`, `[10..18]`) and digits `[1..9]` render with
   garbage; only the digit-0 / item-NONE / first-row-option slots see a
   fixup.

2. **Same hazard for `menu_option_sprites`.** Every option past the
   first one of each menu row is reached via a `sobj->sprite =
   *menu_option_sprites[i]` copy at navigation time. If the source
   sprite never went through the fixup chain, the SObj inherits the
   scrambled headers and (more visibly) the un-byteswapped /
   un-deswizzled bitmap data — so menu options 1+ "disappear or render
   garbage data".

## Fix

`src/sc/sc1pmode/sc1ptrainingmode.c` — add a port-side helper that runs
the full Sprite + Bitmap-array + texel-data fixup chain on a sprite
loaded from a reloc array, and call it at the top of the Init loops
that touch every sprite in those two arrays:

```c
#ifdef PORT
extern void portFixupSprite(void *sprite);
extern void portFixupBitmapArray(void *bitmaps, unsigned int count);
extern void portFixupSpriteBitmapData(void *sprite, void *bitmaps);

static void sc1PTrainingModeFixupSprite(Sprite *sprite)
{
    if (sprite == NULL) return;
    portFixupSprite(sprite);
    Bitmap *bitmaps = (Bitmap*)PORT_RESOLVE(sprite->bitmap);
    if (bitmaps != NULL) {
        portFixupBitmapArray(bitmaps, sprite->nbitmaps);
        portFixupSpriteBitmapData(sprite, bitmaps);
    }
}
#endif
```

Then in `sc1PTrainingModeInitStatDisplayCharacterSprites` (39 sprites)
and `sc1PTrainingModeInitMenuOptionSpriteAttrs` (31 sprites), do the
fixup before the field writes:

```c
for (i = 0; i < 39; i++) {
    Sprite *sprite = sc1PTrainingModeResolveOpt(
        sSC1PTrainingModeMenu.display_option_sprites, i);
#ifdef PORT
    sc1PTrainingModeFixupSprite(sprite);
#endif
    sprite->red = 0x6C;
    sprite->green = 0xFF;
    sprite->blue = 0x6C;
    sprite->attr = SP_TEXSHUF | SP_TRANSPARENT;
}
```

These two loops happen to enumerate every slot the rest of the file
ever indexes into, so a single fixup pass at Init time is enough — the
sub-range Init functions
(`sc1PTrainingModeInitCPOptionSpriteColors`,
`sc1PTrainingModeInitItemOptionSpriteColors`,
`sc1PTrainingModeInitSpeedOptionSpriteColors`) all run *after*
`sc1PTrainingModeInitMenuOptionSpriteAttrs`, so by the time their RGB
writes hit, the sprite layouts are already correct.

`portFixupSprite`, `portFixupBitmapArray`, and
`portFixupSpriteBitmapData` are each idempotent (keyed in
`sStructU16Fixups` / `sDeswizzle4cFixups`), so it's safe for the later
`lbCommonMakeSObjForGObj` paths to call them again without effect.

## Class of bug — audit target

Any code that:

1. Resolves a sprite straight from a reloc array (token lookup),
2. **Writes** to scalar fields (`red`/`green`/`blue`/`attr`/`zdepth`/
   `width`/`height`/etc.) before any `lbCommonMakeSObjForGObj` /
   `portFixupSprite` call on that pointer,

…will leave the writes at the wrong offsets and have them later moved
by the deferred fixup. Grep target: `sprite->\(red\|green\|blue\|attr\|
zdepth\)\s*=` in `src/{sc,if,gm,mn,gr,ef,it,ft}/` for any path that
reaches the resolved Sprite via a `u32*` reloc-token array (rather
than via a fresh `lbCommonMakeSObjForGObj`-returned SObj).

The same hazard also applies to `sobj->sprite = *sprites[i]` struct
copies when `sprites[i]` was never passed through the fixup chain —
both the headers and the texel-data side need to be armed before the
copy.

## Not related to

- `training_mode_wallpaper_token_truncation_2026-04-24.md`: that bug
  was about a raw host pointer stored in a `u32` reloc-token field; the
  wallpaper sprite still goes through the normal Make path and gets
  fixed. This one is about every *non-wallpaper* training-mode sprite.
- `training_mode_sprite_array_stride_2026-04-24.md`: that fixed the
  `Sprite**` → `u32*` stride downgrade so the array reads work at all.
  Without it the bug here couldn't be reproduced because reads landed
  on bogus pointers; with it, the reads work but the writes still land
  pre-fixup, which is what this entry covers.
