# CPU Series-Icon Renders Cyan — `unused` Field Missed by GroundData Fixup (2026-04-29) — FIXED

**Symptom (issue #6):** CPU fighters' damage-display series icons
(DK logo, Star Fox emblem, Triforce, Smash Bros logo, etc.) render
cyan/teal instead of their proper colors. Human (HMN) players'
emblems render correctly. Reporter screenshots show 1P/2P (HMN) with
saturated colors and 3P/4P (CPU) with consistent cyan tinting across
all stages tested.

## Root cause

Two pieces fit together:

### 1. Original game's "5th emblem color" trick

`MPGroundData` (`src/mp/mptypes.h:193`) declares emblem colors as a
4-entry array:

```c
SYColorRGB emblem_colors[GMCOMMON_PLAYERS_MAX]; // = 4 entries
s32 unused;
Vec3f light_angle;
```

But `ifCommonPlayerDamageInitInterface` reads
`gMPCollisionGroundData->emblem_colors[player.color]`, and
`mnPlayersVSStartGame` (`src/mn/mnplayers/mnplayersvs.c:4424`) sets
`players[i].color = GMCOMMON_PLAYERS_MAX` (= 4) for non-team-battle
**CPU** players. That's a 5th index into a 4-entry array.

On N64 this is intentional: the level data files store five 3-byte
colors back-to-back at the `fog_alpha + emblem_colors` byte run, and
the C code reads the 5th by indexing past the declared array. The
decomp's `s32 unused` field is just the C struct framing for what the
level designer treats as `emblem_colors[4]` — typically a neutral
gray that flags "this player is CPU-controlled".

### 2. Port-side byteswap fixup stops one word short

`mpCollisionFixGroundDataLayout` un-BSWAP32s the packed-byte block
covering `fog_color` + `fog_alpha` + `emblem_colors[0..3]`:

```c
unsigned int fog_off    = ... &fog_color ...;
unsigned int emblem_end = ... &emblem_colors[GMCOMMON_PLAYERS_MAX - 1] + 1 ...;
unsigned int aligned_off = fog_off & ~3U;
portFixupStructU32(ground_data, aligned_off, (emblem_end - aligned_off + 3) / 4);
```

`emblem_end` lands exactly at the end of `emblem_colors[3]` — i.e.
the start of `unused`. The word containing `unused`'s bytes is
**not** included in the fixup, so it stays in pass1's bytes-reversed
state. When the C code reads `emblem_colors[4].r/.g/.b` (which are
just the first three bytes of `unused`), it gets the BSWAP32 byte
permutation of whatever the level designer wrote.

Most stages set the CPU emblem color to a near-gray like
`(80, 80, 80)`. The BSWAP32 of the containing u32 puts a high byte
(usually 0xFF or 0x80) into the middle slots, producing a triple like
`(00, FF, FF)` — i.e. cyan. Hence "every CPU emblem is cyan".

## Fix

`src/mp/mpcollision.c` — extend the fixup range to include the
`unused` word:

```c
unsigned int unused_end = (unsigned int)((uintptr_t)(&ground_data->unused + 1) - (uintptr_t)ground_data);
unsigned int aligned_off = fog_off & ~3U;
portFixupStructU32(ground_data, aligned_off, (unused_end - aligned_off + 3) / 4);
```

`portFixupStructU32` is idempotent (keyed in `sStructU16Fixups`) so
re-entering a stage doesn't double-flip. No struct layout change —
the array is still declared with 4 entries and the C reader still
indexes `[4]` for CPU. The byte at offset 12-of-emblem-colors (= the
first byte of `unused`) is now in N64 BE order, which is what the
emblem-color reader expects.

## Why we don't extend the C array

Renaming `unused` → `cpu_emblem_color` and declaring
`emblem_colors[GMCOMMON_PLAYERS_MAX + 1]` would be cleaner, but the
struct's next field (`Vec3f light_angle`) requires 4-byte alignment
and `5 * sizeof(SYColorRGB) = 15` bytes. We'd have to insert a 1-byte
padding to keep the rest of the struct at the same offsets — a
load-bearing constraint because reloc files write directly into this
struct's byte layout. Easier to leave the decomp's framing alone and
fix the byteswap.

## Class of bug

Same family as
`mpgrounddata_layer_mask_u8_byteswap_2026-04-22` — a u8/s32 inside a
larger struct that pass1 BSWAP32 mangles, and the fixup pass missed
one word's worth of coverage. Audit target: any
`portFixupStructU32(...)` that bounds its `num_words` by the *end of
a sub-struct field* rather than the next aligned offset of an actual
boundary.

## Files

- `src/mp/mpcollision.c` — `mpCollisionFixGroundDataLayout` extended.
