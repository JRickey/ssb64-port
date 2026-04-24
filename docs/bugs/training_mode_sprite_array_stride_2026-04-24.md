# Training Mode Entry SIGSEGV #2 — LP64 Pointer-Array Stride Sweep (2026-04-24) — FIXED

## TL;DR

Training-mode entry crashed in three successive places, all the same
class: a pointer field that widens from 4B on N64 to 8B on LP64 inside
a reloc-loaded array or struct, breaking stride-matched indexing.
Swept the entire training-mode file in one pass after the third hit.

Three distinct targets fixed:
- `SC1PTrainingModeSprites::sprite` (struct-internal pointer, 8B on N64
  / 16B on LP64). Pinned to `u32` under PORT.
- `SC1PTrainingModeMenu::display_option_sprites` (`Sprite**`,
  pointer-to-pointer array loaded from reloc file). Pinned to `u32*`
  under PORT.
- `SC1PTrainingModeMenu::menu_option_sprites` — same as above.

All three follow the same pattern as
[mball_thrown_effect_filehead_lp64](mball_thrown_effect_filehead_lp64_2026-04-21.md)
and the bonus-stage `anim_joints` case (see memory `project_file_pointer_array_lp64_stride`).


**Symptom:** After the [wallpaper token fix](training_mode_wallpaper_token_truncation_2026-04-24.md),
Training Mode now crashes deeper in `sc1PTrainingModeFuncStart`:

```
SSB64: !!!! CRASH SIGSEGV fault_addr=0x1fe00140024
SSB64: x0=0x1fe00140024 ...
0  _ZL14fixup_rotate16Pj + 12
1  portFixupSprite + 108
2  lbCommonMakeSObjForGObj + 28
3  sc1PTrainingModeMakeStatDisplay + 36
4  sc1PTrainingModeMakeStatDisplayText + 120
5  sc1PTrainingModeMakeStatDisplayAll + 12
6  sc1PTrainingModeFuncStart + 792
```

The "fault address" `0x1fe00140024` is diagnostic: two adjacent 32-bit
values read as a single 64-bit pointer (high 0x1fe = low bits of a
token; low 0x140024 = a pos word or similar).

## Root cause

`src/sc/sctypes.h`:

```c
struct SC1PTrainingModeSprites
{
    Vec2h pos;         // 4 bytes
    Sprite *sprite;    // 4 bytes on N64, 8 bytes on LP64
};
```

Sizeof differs between N64 (8 bytes) and LP64 (16 bytes due to
alignment of the widened pointer). The array is loaded straight from
a reloc file whose entries are laid out at the **N64** 8-byte stride.
With the port treating it as 16-byte entries:

- `ts[0].pos`     reads entry 0's pos ✓ (at offset 0)
- `ts[0].sprite`  reads 8 bytes at offset 8 — which is entry 1's
  `{pos, sprite_token}` concatenated → bogus 64-bit "pointer"
- `ts[1]`+ are accessed at the wrong addresses entirely

The crash came out of `portFixupSprite` dereferencing that 0x1fe...
value. This is the same failure class as the MBall/Kirby
`file_head` LP64 bug (`docs/bugs/mball_thrown_effect_filehead_lp64_2026-04-21.md`):
an `#ifdef PORT` branch forgot to downgrade the pointer field to a
`u32` reloc token, leaving the struct too wide on LP64.

The sprite slots themselves are **already tokenized** correctly by the
intra-file reloc chain in
`port/bridge/lbreloc_bridge.cpp:452`, so no separate token
registration is needed — only the struct layout had to be fixed.

## Fix

### 1. Pin struct width to 8 bytes on port

`src/sc/sctypes.h`:

```c
struct SC1PTrainingModeSprites
{
    Vec2h pos;
#ifdef PORT
    u32 sprite;      // reloc token (intra-file reloc chain pre-tokenized)
#else
    Sprite *sprite;
#endif
};
```

### 2. Route every reader through a resolver helper

`src/sc/sc1pmode/sc1ptrainingmode.c`:

```c
#ifdef PORT
static inline Sprite* sc1PTrainingModeResolveTS(SC1PTrainingModeSprites *ts)
{
    // pass1 BSWAP32 scrambled the {s16 x, s16 y} pos word —
    // rotate16 restores the fields. Idempotent via sStructU16Fixups.
    portFixupStructU16(ts, 0, 1);
    return (Sprite*)PORT_RESOLVE(ts->sprite);
}
#else
#define sc1PTrainingModeResolveTS(ts) ((ts)->sprite)
#endif
```

Applied at every `ts->sprite` / `array[i].sprite` read site in
`sc1ptrainingmode.c`. The helper does two jobs: resolves the token to
the real `Sprite*`, and fixes up the `Vec2h pos` word (which would
otherwise read with `x`/`y` swapped because pass1's u32 byte-swap
leaves the two `s16` pairs reversed on LE).

## Class of bug — audit target

`#ifdef PORT` branches that tokenize **scalar** pointer fields but
forget struct-level consequences. Any struct that:

1. Contains one or more `Type*` fields
2. Is loaded as a **C array** directly from a reloc file (not built at
   runtime)
3. Has `sizeof` that changes between N64 (32-bit ptrs) and LP64 (64-bit
   ptrs)

...will read garbage off the second entry onward unless every
pointer-width field is downgraded to a `u32` token under PORT. Grep
target: `struct.*{.*Vec.*Sprite\s*\*` and similar patterns where a
small leading field + pointer adds up to 8 bytes on N64.

## Supersedes / relates to

- Does **not** supersede
  [training_mode_wallpaper_token_truncation](training_mode_wallpaper_token_truncation_2026-04-24.md)
  — that was a separate earlier crash in the same scene entry path
  (`grWallpaperMakeStatic`). Both had to be fixed in sequence before
  training mode could reach frame 1.
- Same mechanism class as
  [mball_thrown_effect_filehead_lp64](mball_thrown_effect_filehead_lp64_2026-04-21.md)
  but on a struct array instead of a single struct.
