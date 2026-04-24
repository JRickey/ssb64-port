# Struct Audit — `ITAttackColl` and `WPAttributes` (2026-04-24)

Per the handoff in `docs/struct_rom_layout_audit_handoff_2026-04-24.md`, I ran
steps 1–5 of the ROM-layout audit method against these two structs.
**No code rewrites** — findings only.

## Summary

- **`ITAttackColl` (`src/it/ittypes.h`)** — **Not a ROM-loaded struct.** It is
  allocated and populated field-by-field in C (`itmain.c`, `wpmanager.c`, event
  readers). There is no reader that interprets raw ROM bytes through this
  struct, so the IDO-bit-layout audit does not apply. Layout is correct by
  construction. No rewrite needed.
- **`WPAttributes` (`src/wp/wptypes.h`)** — **Layout OK, fully verified.** Every
  field's declared LE bit position matches the IDO-emitted extraction in
  `wpManagerMakeWeapon` at VRAM `0x801655C8`. Sanity-checked against the
  Mario Fireball record (file 204, offset 0) — all fields produce
  gameplay-sensible values. No rewrite needed.

---

## 1. `ITAttackColl`

### Finding
Runtime-only struct; **ROM-layout audit does not apply**. No fix needed and no
fixup currently exists (nor is one called for).

### Evidence
`grep -rn 'ip->attack_coll\|wp->attack_coll' src/` returns 431 hits — all
scalar field reads/writes, no `memcpy` or pointer-cast from a ROM buffer.
Construction sites:

- `itmain.c` — `ip->attack_coll.attack_state = …`, `.damage = …`, `.size = …`,
  `.angle = BITFIELD_SEXT10(ev[…].angle)`, etc. Values come from
  `ITAttackEvent` / `ITMonsterEvent` (which are ROM-loaded and already audited
  — see `ittypes.h:117-172`) or from direct constants.
- `wpmanager.c:200-253` — the weapon side populates its own `attack_coll` from
  `WPAttributes` fields.
- `itprocess.c`, `wpprocess.c`, `gm/gmcollision.c` — only read individual
  scalar fields.

Only oddity: `itmain.c:423` does `memcpy(&ip->attack_coll.stat_flags, &stat_flags, sizeof(u16))`.
That's a single `GMStatFlags` u16 copy; its correctness depends on
`GMStatFlags` layout, which is a separate struct in `gm/gmtypes.h`. Not a
concern for `ITAttackColl` itself.

### Bitfield sub-section (runtime only)
Inside `ITAttackColl`:

```c
ub32 can_setoff : 1;
ub32 can_rehit_item : 1;
ub32 can_rehit_fighter : 1;
ub32 can_rehit_shield : 1;
ub32 can_hop : 1;
ub32 can_reflect : 1;
ub32 can_shield : 1;
u32 motion_attack_id : 6;
```

These are **not** wrapped in `#if IS_BIG_ENDIAN`, so clang LE packs them
LSB-first while IDO BE would pack them MSB-first. **That is fine** because
every read/write in the port is a named field access — BE and LE layouts are
each self-consistent, and nothing deserializes a raw ROM u32 into this
section. If a future patch ever introduces a raw u32 pun against
`&attack_coll.<flag>`, audit this sub-section — until then it's inert.

### Rewrite priority
**None.** Runtime struct, no ROM reader, no fixup needed.

---

## 2. `WPAttributes`

### Reader VRAM anchor
`wpManagerMakeWeapon` at `0x801655C8` (loaded via
`lbRelocGetFileData(WPAttributes*, *wp_desc->p_weapon, wp_desc->o_attributes)`
→ `$v1` is the attr pointer).

### IDO extraction sequences (hand-traced from `/tmp/wpmgr.asm`)

| Offset / load | Asm idiom | Bit window | Width | Signed | Field |
|---|---|---|---|---|---|
| `0x10` `lh` | (plain half-word) | — | 16 | yes | `attack_offsets[0].x` |
| `0x12` `lh` | plain | — | 16 | yes | `attack_offsets[0].y` |
| `0x14` `lh` | plain | — | 16 | yes | `attack_offsets[0].z` |
| `0x16` `lh` | plain | — | 16 | yes | `attack_offsets[1].x` |
| `0x18` `lh` | plain | — | 16 | yes | `attack_offsets[1].y` |
| `0x1A` `lh` | plain | — | 16 | yes | `attack_offsets[1].z` |
| `0x1C` `lh` | plain | — | 16 | yes | `map_coll_top` |
| `0x1E` `lh` | plain | — | 16 | yes | `map_coll_center` |
| `0x20` `lh` | plain | — | 16 | yes | `map_coll_bottom` |
| `0x22` `lh` | plain | — | 16 | yes | `map_coll_width` |
| `0x24` `lhu` | plain | — | 16 | no | `size` |
| `0x26` `lh; sra 6` | top 10 of u16 | [15:6] of u16 @ 0x26 | 10 | yes | `angle` |
| `0x28` `lw; sll 10; srl 24` | mid | [21:14] | 8 | no | `damage` |
| `0x28` `lw; sll 18; srl 28` | mid | [13:10] | 4 | no | `element` |
| `0x28` `lw; srl 22` | top | [31:22] | 10 | no | `knockback_scale` |
| `0x28` `lw; andi 0x3FF` | low | [9:0] | 10 | no | `knockback_weight` |
| `0x2C` `lb` | signed top byte | [31:24] | 8 | yes | `shield_damage` |
| `0x2D` `lbu; srl 6` | top 2 of byte 0x2D | [23:22] | 2 | no | `attack_count` |
| `0x2C` `lw; sll 10; srl 31` | [21] | 1 | no | `can_setoff` |
| `0x2C` `lw; sll 11; srl 22` | [20:11] | 10 | no | `sfx` |
| `0x2E` `lbu; andi 0x7` | low 3 of byte 0x2E | [10:8] | 3 | no | `priority` |
| `0x2F` `lbu; srl 7` | top bit of byte 0x2F | [7] | 1 | no | `can_rehit_item` |
| `0x2C` `lw; sll 25; srl 31` | [6] | 1 | no | `can_rehit_fighter` |
| `0x2C` `lw; sll 26; srl 31` | [5] | 1 | no | `can_hop` |
| `0x2C` `lw; sll 27; srl 31` | [4] | 1 | no | `can_reflect` |
| `0x2C` `lw; sll 28; srl 31` | [3] | 1 | no | `can_absorb` |
| `0x2C` `lw; sll 29; srl 31` | [2] | 1 | no | `can_shield` |
| `0x30` `lw; srl 22` | top | [31:22] | 10 | no | `knockback_base` |

Bits [1:0] of word 0x2C are `unused_0x2F_b7` / `unused_0x2F_b6`; they are
never read in the reader (confirmed by absence from the disasm).

### Port comparison

| Field | IDO bit window | Port LE declaration | Match? |
|---|---|---|---|
| `attack_offsets[0..1].{x,y,z}` @ 0x10..0x1B | plain s16 | `Vec3h attack_offsets[2]` | yes |
| `map_coll_top` @ 0x1C | plain s16 | `s16 map_coll_top` | yes |
| `map_coll_center` @ 0x1E | plain s16 | `s16 map_coll_center` | yes |
| `map_coll_bottom` @ 0x20 | plain s16 | `s16 map_coll_bottom` | yes |
| `map_coll_width` @ 0x22 | plain s16 | `s16 map_coll_width` | yes |
| `size` @ 0x24 | plain u16 | `u16 size` | yes |
| `angle` @ 0x26 | u16 storage, [15:6] signed | `u16 _angle_raw`, extracted via `((s16)_angle_raw) >> 6` in `wpmanager.c:220` | yes |
| `knockback_weight` | 0x28 [9:0] | `u32 knockback_weight : 10` at bits 9-0 | yes |
| `element` | 0x28 [13:10] | `u32 element : 4` at bits 13-10 | yes |
| `damage` | 0x28 [21:14] | `u32 damage : 8` at bits 21-14 | yes |
| `knockback_scale` | 0x28 [31:22] | `u32 knockback_scale : 10` at bits 31-22 | yes |
| `unused_0x2F_b7` | 0x2C [0] (inferred) | bit 0 | yes |
| `unused_0x2F_b6` | 0x2C [1] (inferred) | bit 1 | yes |
| `can_shield` | 0x2C [2] | bit 2 | yes |
| `can_absorb` | 0x2C [3] | bit 3 | yes |
| `can_reflect` | 0x2C [4] | bit 4 | yes |
| `can_hop` | 0x2C [5] | bit 5 | yes |
| `can_rehit_fighter` | 0x2C [6] | bit 6 | yes |
| `can_rehit_item` | 0x2C [7] | bit 7 | yes |
| `priority` | 0x2C [10:8] | bits 10-8 | yes |
| `sfx` | 0x2C [20:11] | bits 20-11 | yes |
| `can_setoff` | 0x2C [21] | bit 21 | yes |
| `attack_count` | 0x2C [23:22] | bits 23-22 | yes |
| `shield_damage` | 0x2C [31:24] signed | bits 31-24 + `BITFIELD_SEXT8()` at read site | yes |
| `knockback_base` | 0x30 [31:22] | bits 31-22 | yes |
| pad | 0x30 [21:0] | `u32 : 22` low | yes |

**Zero mismatches.**

### Sanity check: Mario Fireball (file 204, offset 0)

Raw bytes (BE, file size 64, WPAttributes sizeof 0x34):

```
0x00: 00 01 00 6a ff ff 00 36 00 00 00 00 00 00 00 00
0x10: 00 00 00 00 00 00 00 00 00 00 00 00 00 32 00 00
0x20: ff ce 00 32 00 c8 5a 40 06 41 c4 00 01 60 e1 3c
0x30: 02 80 00 00
```

Decoded:

| Field | Value | Gameplay sanity |
|---|---|---|
| `map_coll_top` | 50 | reasonable projectile hull |
| `map_coll_center` | 0 | yes |
| `map_coll_bottom` | -50 | yes, symmetric with top |
| `map_coll_width` | 50 | yes |
| `size` | 200 | canonical fireball hitbox diameter |
| `angle` (0x5A40 sra 6) | 361 | 10-bit angle (0-1023 = 0-360°) |
| `knockback_scale` | 25 | small pro |
| `damage` | **7** | Mario fireball = 7% (matches canonical frame data) |
| `element` | 1 | fire element id |
| `knockback_weight` | 0 | typical weight-dep KB off for small proj |
| `shield_damage` | 1 | yes |
| `attack_count` | 1 | one hitbox |
| `can_setoff` | 1 | projectile vs. projectile interact — yes |
| `sfx` | 28 | plausible FGM ID |
| `priority` | 1 | yes |
| `can_rehit_item/fighter` | 0 / 0 | fireball is single-hit |
| `can_hop/reflect/absorb/shield` | 1 / 1 / 1 / 1 | standard projectile |
| `knockback_base` | 10 | yes |

Every field is a gameplay-plausible number. Damage = 7 matches the canonical
value; this is the field that was reading 64 before the 2026-04-20 fix. The
fix held.

### u32-word bit sums (sanity)

| Word | Fields (widths) | Sum | OK? |
|---|---|---|---|
| 0x28 | 10+8+4+10 | 32 | yes |
| 0x2C | 8+2+1+10+3+1+1+1+1+1+1+1+1 | 32 | yes |
| 0x30 | 22(pad)+10 | 32 | yes |

No suspicious words. The `u16 _angle_raw` at 0x26 plus `u16 size` at 0x24
occupy one u32-aligned storage word that pass1 BSWAP32 swaps; this is
correctly undone by `portFixupStructU16(attr, 0x10, 6)` in
`wpmanager.c:119` (6 words = 0x10..0x27, covering every plain-u16 slot up to
and including `_angle_raw`). The three bitfield-only u32s at 0x28/0x2C/0x30
don't need fixup — BSWAP32 already lands the raw u32 value where the port's
bit masks expect it.

### Rewrite priority
**None.** Port layout, read-site extraction, and fixup range all match IDO
ground truth.

---

## Notes for future audit passes

- The `wpManagerMakeWeapon` disassembly (`/tmp/wpmgr.asm`) cleanly demonstrates
  IDO's canonical pad-gap packing: `angle : 10` lands in the u16 gap between
  `size` (0x24) and the bitfield u32 at 0x28 — the port handles this via
  explicit `u16 _angle_raw` + shift-at-read.
- `ITAttackColl`, `ITAttackPos`, `GMAttackRecord` are all **runtime** structs.
  The ROM-backed item attack data lives in `ITAttackEvent` / `ITMonsterEvent`
  (audited and fixed earlier — see `ittypes.h:117-172` and the `timer`-byte
  pad-gap packing note).
- Candidates remaining for this audit pass, unchanged from the handoff:
  `FTAttributes`, `FTAttackEvent` / `FTMotionState` / `FTStatusEvent`,
  `GRAttackColl` / `GRSectorDesc`, per-fighter `WeaponAttributes` variants
  (Yoshi egg throw, Pikachu thunderjolt). None were re-checked here.
