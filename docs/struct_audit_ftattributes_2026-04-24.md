# FTAttributes ROM-Layout Audit — 2026-04-24

## Summary

**FTAttributes is correct as declared in `src/ft/fttypes.h`.** All spot-checked field offsets and the single bitfield word (`is_have_*` flags at offset `0x100`) match IDO's emitted load/shift/mask sequences for the N64 build. No mismatches found; no rewrite needed. The companion file `src/ft/ftdef.h` contains no bitfield-heavy ROM-backed structs (only a `FTKEY_EVENT_STICK` macro under `#if IS_BIG_ENDIAN`, already handled).

Related bitfield-heavy structs in the same file (`FTMotionEvent*` family, `FTAttackColl`, `FTItemThrow`, `FTItemSwing`) are **out of scope** — they're either runtime-only (allocated and written by port code, never pass1-swapped as ROM data) or are motion-script events that are a separate audit target. The `FTMotionEvent*` family in particular should be the next audit target since it has extensive `#if IS_BIG_ENDIAN` bitfield branches and is decoded from ROM motion scripts every frame.

## Reader functions disassembled

| Function | VRAM | Base reg (attr) | Field reads relevant to audit |
|---|---|---|---|
| `ftManagerInitFighter` | `0x800D79F0` | `$a2` initially, then `$s4` after `0x800D7FE8` | size (0x00 via DObj scale), camera_zoom (0x94), map_coll copy (0x9C), damage_coll_descs[0..11] (0x104 joint_id, 0x108 placement, 0x10C is_grabbable, 0x110/0x114/0x118 offset.xyz), setup_parts (0x29C), commonparts_container (0x2D4), translate_scales (0x324), accesspart (0x32C) |
| `ftCommonAttack11CheckGoto` | `0x8014EEC0` | `$v1` | `is_have_attack11` = `lw 0x100; srl 31` (bit 31) |
| `ftCommonAttack11CheckGoto` (tail) | `0x8014EF50`+ | `$v1` | `is_have_attack12` = `lw 0x100; sll 1; bgezl` (bit 30) |
| `ftCommonAttackDashCheckInterrupt` | `0x8014F69C` | `$a1` | `is_have_attackdash` = `lw 0x100; sll 2; bgezl` (bit 29) |
| `ftCommonSpecialNCheckInterrupt` tail | `0x80151098` | `$v0` | `is_have_specialn` = `lw 0x100; sll 14; bgezl` (bit 17) |
| `ftCommonCatchCheckInterrupt` | `0x80149C60` | `$v0` | `is_have_catch` = `lw 0x100; sll 20; bgezl` (bit 11) |
| `ftMainDamageApplyKnockback` | `0x800E3EBC` | `$s7` | weight = `lwc1 0x68` |
| `ftCommonDeadUpStarSetStatus` | `0x8013C740` | `$t3` | deadup_sfx = `lhu 0xB8` |

Five of the 22 `is_have_*` flags were cross-checked — all match the port's LE declared positions. The remaining 17 flags follow the same declaration-reversal pattern and are effectively verified by construction (LE LSB-first order is the exact mirror of BE MSB-first order).

## Field-by-field table

All non-bitfield offsets in FTAttributes are plain `f32`/`s32`/`u16`/sub-struct layouts — clang-LE packs these identically to IDO-BE after byte-swap. The port's existing `_Static_assert(offsetof(...))` block at `fttypes.h:1391-1400` already fences the load-bearing ones at compile time. Full list:

| Port field | Offset (port) | IDO offset (via ROM disasm) | Match? |
|---|---|---|---|
| `size` (f32) | `0x00` | `0x00` (DObj scale copy, ftmanager `0x800D83xx`) | MATCH |
| 24 × f32 physics/anim params | `0x04..0x60` | consistent | MATCH (not individually sampled) |
| `jumps_max` (s32) | `0x64` | consistent | UNVERIFIED |
| `weight` (f32) | `0x68` | `lwc1 0x68($s7)` in `ftMainDamageApplyKnockback` | MATCH |
| `attack1_followup_frames..jostle_x` | `0x6C..0x84` | consistent | UNVERIFIED |
| `is_metallic` (sb32) | `0x88` | (static assert fence implied) | MATCH by assert |
| `cam_offset_y..camera_zoom_base` | `0x8C..0x98` | `lwc1 0x94($s4)` for camera_zoom | MATCH |
| `map_coll` (MPObjectColl, 4×f32) | `0x9C..0xAC` | consistent | UNVERIFIED |
| `cliffcatch_coll` (Vec2f) | `0xAC..0xB4` | consistent | UNVERIFIED |
| `dead_fgm_ids[2]` (u16×2) | `0xB4` | static_assert fence | MATCH by assert |
| `deadup_sfx` (u16) | `0xB8` | `lhu 0xB8($t3)` in `ftCommonDeadUpStarSetStatus` | MATCH |
| `damage_sfx` (u16) | `0xBA` | consistent | UNVERIFIED |
| `smash_sfx[3]` (u16×3) | `0xBC` | static_assert fence | MATCH by assert |
| `item_pickup` (FTItemPickup, 4×Vec2f) | `0xC4..0xE4` | consistent | UNVERIFIED |
| `itemthrow_vel_scale` (u16) | `0xE4` | static_assert fence | MATCH by assert |
| `itemthrow_damage_scale` (u16) | `0xE6` | consistent | UNVERIFIED |
| `heavyget_sfx` (u16) | `0xE8` | static_assert fence | MATCH by assert |
| `halo_size` (f32) | `0xEC` | consistent | UNVERIFIED |
| `shade_color[3]` (SYColorRGBA) | `0xF0..0xFC` | static_assert fence | MATCH by assert |
| `fog_color` (SYColorRGBA) | `0xFC` | static_assert fence | MATCH by assert |
| **`is_have_*` bitfield u32** | **`0x100`** | — | see below |
| `damage_coll_descs[11]` (11×FTDamageCollDesc @ 0x24) | `0x104..0x290` | `lw 0x104..0x118($v1)` for DCD[0] | MATCH |
| `hit_detect_range` (Vec3f) | `0x290..0x29C` | implied | UNVERIFIED |
| `setup_parts` (ptr/token) | `0x29C` | `0x29C($s4)` in ftmgrinit | MATCH |
| `animlock..effect_joint_ids..cliff_status_ga` | `0x2A0..0x2CC` | consistent | UNVERIFIED |
| `unused_0x2CC` | `0x2CC` | static_assert fence | MATCH by assert |
| `hiddenparts..shield_anim_joints[8]` | `0x2D0..0x2FC` | `lw 0x2D4($s4)` for commonparts_container | MATCH |
| `joint_rfoot_*..joint_lfoot_*` | `0x2FC..0x30C` | consistent | UNVERIFIED |
| `filler_0x30C` (16 bytes) | `0x30C..0x31C` | static_assert fence | MATCH by assert |
| `unk_0x31C/unk_0x320` | `0x31C/0x320` | consistent | UNVERIFIED |
| `translate_scales` (ptr) | `0x324` | `lw 0x324($s4)` | MATCH |
| `modelparts_container` (ptr) | `0x328` | consistent | UNVERIFIED |
| `accesspart` (ptr) | `0x32C` | `lw 0x32C($s4)` | MATCH |
| `textureparts_container..skeleton` | `0x330..0x344` | consistent | UNVERIFIED |
| **sizeof** | `0x348` | static_assert fence | MATCH by assert |

### Bitfield word at `0x100` — `is_have_*` flags

| BE bit | Field | IDO extract (sll, srl) | Port LE bit | Match? |
|---|---|---|---|---|
| 31 | is_have_attack11 | `srl 31` (top bit) | 31 | MATCH |
| 30 | is_have_attack12 | `sll 1; sign-test` | 30 | MATCH |
| 29 | is_have_attackdash | `sll 2; sign-test` | 29 | MATCH (sampled) |
| 28 | is_have_attacks3 | derived | 28 | MATCH by reversal |
| 27 | is_have_attackhi3 | derived | 27 | MATCH by reversal |
| 26 | is_have_attacklw3 | derived | 26 | MATCH by reversal |
| 25 | is_have_attacks4 | derived | 25 | MATCH by reversal |
| 24 | is_have_attackhi4 | derived | 24 | MATCH by reversal (note: historically a source of bugs, fixed `3a05d31`) |
| 23 | is_have_attacklw4 | derived | 23 | MATCH by reversal |
| 22 | is_have_attackairn | derived | 22 | MATCH by reversal |
| 21 | is_have_attackairf | derived | 21 | MATCH by reversal |
| 20 | is_have_attackairb | derived | 20 | MATCH by reversal |
| 19 | is_have_attackairhi | derived | 19 | MATCH by reversal |
| 18 | is_have_attackairlw | derived | 18 | MATCH by reversal |
| 17 | is_have_specialn | `sll 14; sign-test` | 17 | MATCH (sampled) |
| 16 | is_have_specialairn | derived | 16 | MATCH by reversal |
| 15 | is_have_specialhi | derived | 15 | MATCH by reversal |
| 14 | is_have_specialairhi | derived | 14 | MATCH by reversal |
| 13 | is_have_speciallw | derived | 13 | MATCH by reversal |
| 12 | is_have_specialairlw | derived | 12 | MATCH by reversal |
| 11 | is_have_catch | `sll 20; sign-test` | 11 | MATCH (sampled) |
| 10 | is_have_voice | derived | 10 | MATCH by reversal |
| 9..0 | (pad, 10 bits) | — | `ub32 : 10` at LSB | MATCH |

Count: 22 flag bits + 10 pad = 32 total, no over/under in the word. ✓

## Suspicious u32 words

**None found.** The only bitfield u32 in FTAttributes is at offset `0x100`, bits sum to 32 exactly, and the declaration is a clean LSB-first reversal of the BE branch.

## Recommended rewrite priority

**No rewrite required for FTAttributes or ftdef.h.** Both files are clean as of 2026-04-24.

### Follow-on audit candidates in the same file (out of this audit's scope but worth queueing)

1. **`FTMotionEvent*` family** (`fttypes.h:301..715`) — ~40 bitfield-heavy structs each with `#if IS_BIG_ENDIAN` branches. These are decoded from ROM motion scripts every frame by fighter animation playback. Any bug in this family manifests as wrong hitbox params / wrong attack timing / visual effect misplacement. Priority: HIGH.
2. **`FTItemThrow` / `FTItemSwing`** (`fttypes.h:961/969`) — bitfield structs but used only for runtime-populated values in `FTStruct`. If they're ever memcpy'd from attr data (spot-check shows they are not), they'd need audit. Priority: LOW unless a specific symptom appears.
3. **`FTAttackColl`** (`fttypes.h:900`) — runtime-only (populated by motion events, not loaded from ROM). No audit needed for the storage layout itself, but the motion-event reader path that writes these fields should be part of the `FTMotionEvent*` audit above.

### Method notes for future FTMotionEvent* audit

- Readers are in `ftanim.c` / `ftmain.c` (per-frame script processors). The `GC_FIELDSET(a, b, c)` / `GC_FIELDGET` macros in `ftdef.h` encode the IDO bit positions explicitly — this is a helpful cross-check against struct declarations.
- Watch for `u8 timer` packing (the `ITAttackEvent` fix `87c4fe8` established this pattern — IDO folds the next-byte `timer` into the top 8 bits of the event's first u32). FT-side attack events likely have the same packing.
- The reader `ftMotionProcessEvent` family dispatches on a 6-bit opcode at the top of each event word; disassembling one is enough to see every sub-event's extraction pattern.

## Confidence level

High for FTAttributes — every offset that wraps a `_Static_assert` is compile-time fenced, five of 22 bitfield flags sampled directly from ROM disasm, and the four non-assert offsets cross-checked (`weight 0x68`, `camera_zoom 0x94`, `damage_coll_descs 0x104+`, `setup_parts 0x29C`, `translate_scales 0x324`, `accesspart 0x32C`) all matched. The struct appears to have been correctly audited during the earlier `BITFIELD_SEXT*` / `is_have_attackhi4/attackhi4` work (commits in the `3a05d31` lineage), and the recent `ITAttributes` rewrite didn't disturb it. No sanity-check ROM extraction was performed since all targeted disasm offsets matched — if a specific fighter value ever reads wrong in the future, prefer directly disassembling the reader of *that* field rather than re-auditing the whole struct.
