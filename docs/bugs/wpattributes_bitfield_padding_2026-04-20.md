# WPAttributes bitfield layout mismatch with IDO BE packing — RESOLVED (2026-04-20)

## Symptom

All fighter-weapon projectiles (Mario Fireball, Samus Charge Shot, Fox Blaster, Pikachu Thunder Jolt, Link Arrow, etc.) were broken in one of three ways:

- **Mario Fireball:** dealt exactly **64% damage per hit** (expected ~6%).
- **Fox Blaster:** hit animation played but dealt **0 damage** (expected ~6%).
- **Samus Charge Shot:** **passed through opponents** without hitting (expected to hit + knock back).

Melee attacks hit normally; the bug was isolated to projectile-vs-fighter hit detection and damage magnitude.

## Root cause

The decomp's `WPAttributes` struct declares bitfields like this:

```c
u16 size;                          // 0x24
s32 angle : 10;
u32 knockback_scale : 10;
u32 damage : 8;
u32 element : 4;
u32 knockback_weight : 10;
// ... Word 2, Word 3 bitfields
```

**This was not the layout IDO BE actually produced.** Reading the IDO-compiled code for the decomp's field accessors (reproduced with the IDO 5.3 compiler bundled in `ssb-decomp-re/tools/ido-recomp/5.3`), the real physical bit positions are:

| Offset | Field | Bits |
|---|---|---|
| 0x24 (u16) | `size` | 15-0 |
| 0x26 (s16) | `angle` | 15-6 (bits 5-0 unused) |
| 0x28 (u32) | `knockback_scale` | 31-22 |
| 0x28 (u32) | `damage` | 21-14 |
| 0x28 (u32) | `element` | 13-10 |
| 0x28 (u32) | `knockback_weight` | 9-0 |
| 0x2C (u32) | `shield_damage` (signed) | 31-24 |
| 0x2C (u32) | `attack_count` | 23-22 |
| 0x2C (u32) | `can_setoff` | 21 |
| 0x2C (u32) | `sfx` | 20-11 |
| 0x2C (u32) | `priority` | 10-8 |
| 0x2C (u32) | `can_rehit_item` | 7 |
| 0x2C (u32) | `can_rehit_fighter` | 6 |
| 0x2C (u32) | `can_hop` | 5 |
| 0x2C (u32) | `can_reflect` | 4 |
| 0x2C (u32) | `can_absorb` | 3 |
| 0x2C (u32) | `can_shield` | 2 |
| 0x2C (u32) | `unused_0x2F_b6` | 1 |
| 0x2C (u32) | `unused_0x2F_b7` | 0 |
| 0x30 (u32) | `knockback_base` | 31-22 (bits 21-0 pad) |

Two things IDO BE did that the port's earlier struct did not anticipate:

1. **`s32 angle : 10` was packed into the 2 pad bytes between `u16 size` and the natural u32 alignment at 0x28** — using them as a 16-bit storage unit with angle occupying bits 15-6 (MSB-first). IDO's generated reader: `lh $t, 0x26($attr); sra $t, 6` (load halfword signed, arithmetic shift right by 6).
2. **`knockback_weight` belongs to Word 1** (alongside kbs/damage/element) at bits 9-0 of the u32 at 0x28 — not to Word 2 as the port's struct assumed. Word 2 begins with `shield_damage` at offset 0x2C.

The port's earlier `WPAttributes` layout read:

- `damage` at bits 11-4 of u32 at 0x28 (wrong — real position is 21-14)
- `element` at bits 3-0 (wrong — real position is 13-10)
- `knockback_scale` at bits 21-12 (wrong — real is 31-22)
- `angle` at bits 31-22 (wrong — real is 15-6 of u16 at 0x26)
- `knockback_weight` at bits 31-22 of u32 at 0x2C (wrong — real is bits 9-0 of u32 at 0x28)
- All Word 2/3 flags and offsets similarly misaligned

For Mario Fireball's ROM Word 1 `0x0641c400`:

- Old port decode: `damage = (0x0641c400 >> 4) & 0xFF = 0x40 = 64` → 64% per hit
- IDO layout decode: `damage = (0x0641c400 >> 14) & 0xFF = 0x07 = 7` → ~7% per hit (matches N64)

For Fox Blaster's Word 1 `0x19018001`:

- Old port decode: `damage = (0x19018001 >> 4) & 0xFF = 0x00 = 0` → 0% per hit
- IDO layout decode: `damage = (0x19018001 >> 14) & 0xFF = 0x06 = 6` → ~6%

For Samus Charge Shot's Word 2 `0x0160013c`:

- Old port decode: `attack_count = (0x0160013c >> 12) & 0x3 = 0` → no hitbox iterations → pass-through
- IDO layout decode: `attack_count = (0x0160013c >> 22) & 0x3 = 0x1` → 1 hitbox iteration → hit registers

## Fix

1. **Replace `u16 _pad_before_combat_bits;` with `u16 _angle_raw;`** at offset 0x26 in the PORT struct. The 2 pad bytes are not padding — they contain the `angle` bitfield. Extract at the read site via arithmetic shift on the signed interpretation:

   ```c
   wp->attack_coll.angle = ((s16)attr->_angle_raw) >> 6;
   ```

2. **Rewrite Word 1 PORT bitfield order** (clang LSB-first on LE) to match the physical bit positions IDO chose, declared in reverse:

   ```c
   u32 knockback_weight : 10;   // bits 9-0
   u32 element : 4;             // bits 13-10
   u32 damage : 8;              // bits 21-14
   u32 knockback_scale : 10;    // bits 31-22
   ```

3. **Rewrite Word 2 PORT bitfield order** with the 13 fields in reverse declaration order, so LSB-first packing places each field at the correct bit positions:

   ```c
   u32 unused_0x2F_b7 : 1;      // bit 0
   u32 unused_0x2F_b6 : 1;      // bit 1
   u32 can_shield : 1;          // bit 2
   u32 can_absorb : 1;          // bit 3
   u32 can_reflect : 1;         // bit 4
   u32 can_hop : 1;             // bit 5
   u32 can_rehit_fighter : 1;   // bit 6
   u32 can_rehit_item : 1;      // bit 7
   u32 priority : 3;            // bits 10-8
   u32 sfx : 10;                // bits 20-11
   u32 can_setoff : 1;          // bit 21
   u32 attack_count : 2;        // bits 23-22
   u32 shield_damage : 8;       // bits 31-24 (read as unsigned; use BITFIELD_SEXT8 at site)
   ```

4. **Rewrite Word 3 PORT bitfield order** — `knockback_base` now at the top 10 bits with 22 pad bits below (the old struct had many flag fields here that actually belong in Word 2):

   ```c
   u32 : 22;                    // bits 21-0 pad
   u32 knockback_base : 10;     // bits 31-22
   ```

The BE branch is unchanged — the decomp's original declaration order is what IDO BE compiles byte-for-byte against the ROM.

`sizeof(WPAttributes)` remains `0x34` (52 bytes). The `_Static_assert` still passes.

### How IDO's peculiar packing was reverse-engineered

Compiled the decomp's `WPAttributes` struct with IDO 5.3 (bundled at `tools/ido-recomp/5.3/cc`) using the same flags as the decomp build (`-Wo,-loopunroll,0 -G0 -mips2 -O2`). A hand-written reader function extracted every field into an output array. Disassembling the result with `rabbitizer` gave the exact load/shift/mask instructions IDO emits for each bitfield access. Those load offsets and shift counts are the ground truth for the physical bit layout, which we then replicate in clang via reversed LSB-first bitfield declarations.

Example reader for `damage`:

```
8c890028  lw     $t1, 0x28($a0)     ; load u32 at attr + 0x28
00095280  sll    $t2, $t1, 10       ; eliminate top 10 bits (knockback_scale)
000a5e02  srl    $t3, $t2, 24       ; keep next 8 bits (damage, at original bits 21-14)
```

So `damage = (*(u32*)(attr + 0x28) >> 14) & 0xFF`.

## Verified decode for 5+ weapons (after fix)

| Weapon | damage | element | angle | atk_cnt | kbs | flags of note |
|---|---|---|---|---|---|---|
| Mario Fireball | 7 | 1 (Fire) | 361 (Sakurai) | 1 | 25 | can_hop/reflect/absorb/shield ✓ |
| Fox Blaster | 6 | 0 (Normal) | 10° | 1 | 100 | can_hop/reflect/absorb/shield ✓ |
| Samus Charge Shot | 0 (overridden) | 2 (Electric) | 361 | 1 | 100 | can_hop/reflect/absorb/shield ✓ |
| Samus Bomb | 9 | 1 (Fire) | 361 | 1 | 65 | (explosion) |
| Pika Thunder Jolt Air | 10 | 2 (Electric) | 361 | 1 | 30 | |
| Pika Thunder Jolt Gnd | 7 | 2 (Electric) | 361 | 1 | 20 | |

All values are now in the expected ranges; the earlier port's values (damage=64, 0, etc.) were artifacts of wrong bit positions.

## Files touched

- `src/wp/wptypes.h` — rename `_pad_before_combat_bits` → `_angle_raw`; rewrite Word 1/2/3 PORT bitfield order to match IDO's physical layout.
- `src/wp/wpmanager.c` — extract angle via `((s16)attr->_angle_raw) >> 6` instead of the (incorrect) `BITFIELD_SEXT10(attr->angle)`.

## Related

- `docs/bugs/itattributes_type_field_offset_2026-04-20.md` — sibling bug in `ITAttributes` (same struct pattern, type field was at wrong Word). The analogous Word 1 bitfield layout in `ITAttributes` is also wrong in the same way — see "Followups" below.
- `docs/bugs/samus_charge_shot_hit_detection_2026-04-13.md` — superseded. The charge-shot-specific `attack_count = 0` theorized from the old (wrong) bit layout was never in the ROM; real decoded `attack_count = 1` lets the hit loop iterate. Any prior Launch-time workaround (explicit `attack_count = 1`, pos init) can be removed.
- `docs/mario_fireball_passthrough_2026-04-19.md` — closed by this fix.

## Followups

**`ITAttributes` has the same bug pattern.** The Word B1/B2/B3 bitfield declarations in `src/it/ittypes.h` are structured identically to the pre-fix `WPAttributes` (angle at the "top" of Word B1, knockback_weight at start of Word B2, etc.). Items currently don't crash (the `type` field fix addressed the Pokeball/Capsule crash), but damage/element/angle/attack_count decode for items is almost certainly as wrong as weapons were before this fix. Mechanical application of the same pattern — `_angle_raw` in place of `_pad_before_combat_bits`, reversed bitfield order for clang LE — should fix items analogously. Defer until after weapons are verified in-game.
