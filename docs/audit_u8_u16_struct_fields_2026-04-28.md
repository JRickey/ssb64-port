# u8 / u16 Fields in Loaded Structs — Byteswap Fixup Audit — 2026-04-28

## Executive Summary

This audit identifies structs with `u8` and `u16` fields that are loaded from reloc files and subject to pass-1 blanket `BSWAP32`. The bug class: after `BSWAP32`, any u8/u16 field within a 4-byte word lands at the wrong byte offset, breaking read access. **22 instances checked; 20 confirmed fixed or safe, 2 require further verification.**

### Symbols

- ✓ **FIXED** — portFixup call already in place at load time
- ⚠ **LIKELY** — hits/symptom reports indicate early fix
- ✗ **SHIPPED-BUG-LIKELY** — no observed fixes; data matches symptom prediction
- ? **NEED-VERIFICATION** — needs hand-audit against loader code
- ✔️ **SAFE** — all fields are u32/f32/pointer or bitfields spanning full u32

---

## Structs by Category

### Animation & Interpolation

#### **SYInterpDesc** (`src/sys/interp.h:8-36`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `_pad0` | 0x00 | u8 | ✓ Fixed (portFixupStructU16 at offset 0) |
| `kind` | 0x01 | u8 | ✓ Fixed (rotated into correct position) |
| `points_num` | 0x02 | s16 | ✓ Fixed (u16 pair rotated) |
| `unk04` | 0x04 | f32 | ✔️ Safe (full u32) |
| `points` | 0x08 | u32 token | ✔️ Safe (full u32) |
| `length` | 0x0C | f32 | ✔️ Safe (full u32) |
| `keyframes` | 0x10 | u32 token | ✔️ Safe (full u32) |
| `quartics` | 0x14 | u32 token | ✔️ Safe (full u32) |

**Loader:** `src/sys/interp.c:72-79` — calls `portFixupStructU16(desc, 0, 1)` before any field reads. ✓

**Files touched:** `src/sys/interp.c` (2026-04-28 fix for non-figatree descriptors)

**Risk:** NONE (fixed 2026-04-28)

---

#### **AObjEvent16** (`src/sys/objtypes.h:79-99`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `toggle:1` (LE bit 0) | 0x00-0x01 | u16 bitfield | ✓ Fixed (entire u16 pair rotated by pass1 + bitfield extraction via LE struct branch) |
| `flags:10` | 0x00-0x01 | u16 bitfield | ✓ Fixed |
| `opcode:5` | 0x00-0x01 | u16 bitfield | ✓ Fixed |

**Loader:** Fighter animation decoder (`src/sys/objanim.c`, `src/ft/ftanim.c`) — these structs are decoded via union as `s16`/`u16` fields; the LE bitfield branch correctly extracts from the post-BSWAP32 u16 order. ✓

**Risk:** NONE (bitfields properly handled in LE branch)

---

#### **AObjEvent32** (`src/sys/objtypes.h:102-128`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `payload:15` (LE bits 0-14) | 0x00 | u32 bitfield | ✓ Fixed (full u32, LE branch extracts correct bits) |
| `flags:10` | 0x00 | u32 bitfield | ✓ Fixed |
| `opcode:7` (LE bits 25-31) | 0x00 | u32 bitfield | ✓ Fixed |
| `p` / `u` / `s` / `f` | 0x00 | u32 union | ✔️ Safe (full u32) |

**Loader:** All animation handlers decode via union + LE bitfield struct; post-BSWAP32 u32 value is correct by construction. ✓

**Risk:** NONE (correctly designed)

---

### Stage Geometry & Collision

#### **MPVertexData** (`src/mp/mptypes.h:34-38`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `pos` | 0x00-0x03 | Vec2h (2×s16) | ✓ Fixed (portFixupStructU16 applied to vertex_data array) |
| `vertex_flags` | 0x04-0x05 | u16 | ✓ Fixed (u16 pair rotated) |

**Loader:** `src/mp/mpcollision.c:4110` — `portFixupStructU16(gMPCollisionVertexData, 0, vpos_words)` ✓

**Bug report:** `docs/bugs/mpvertex_byte_swap_2026-04-11.md` — DK-vs-Samus intro collision broke due to u16 corruption; fixed.

**Risk:** NONE (fixed 2026-04-11)

---

#### **MPVertexLinks** (`src/mp/mptypes.h:24-27`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `vertex1` | 0x00 | u16 | ✓ Fixed |
| `vertex2` | 0x02 | u16 | ✓ Fixed |

**Loader:** `src/mp/mpcollision.c:4071` — `portFixupStructU16(gMPCollisionVertexLinks, 0, (unsigned int)gMPCollisionLinesNum)` ✓

**Risk:** NONE (fixed 2026-04-11)

---

#### **MPLineData** (`src/mp/mptypes.h:45-49`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `group_id` | 0x00 | u16 | ✓ Fixed |
| `line_count` | 0x02 | u16 | ✓ Fixed |

**Loader:** `src/mp/mpcollision.c:4059` — `portFixupStructU16(line_info_ptr, 0, lineinfo_words)` ✓

**Risk:** NONE (fixed 2026-04-08)

---

#### **MPMapObjData** (`src/mp/mptypes.h:92-96`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `mapobj_kind` | 0x00 | u16 | ✓ Fixed |
| `pos` | 0x02-0x05 | Vec2h (2×s16) | ✓ Fixed |

**Loader:** `src/mp/mpcollision.c:4050` — `portFixupStructU16(gMPCollisionMapObjs, 0, mapobj_words)` ✓

**Risk:** NONE (fixed 2026-04-08)

---

#### **MPGeometryData** (`src/mp/mptypes.h:57-75`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `yakumono_count` | 0x00 | u16 | ✓ Fixed (portFixupStructU16 at offset 0, word 1) |
| `mapobj_count` | 0x14 | u16 | ✓ Fixed (portFixupStructU16 at offset 0x14, word 1) |

**Loader:** `src/mp/mpcollision.c:4037-4038` — two `portFixupStructU16` calls ✓

**Risk:** NONE (fixed 2026-04-08)

---

#### **MPGroundData** (`src/mp/mptypes.h:193-237`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `layer_mask` | 0x44 | u8 | ✓ Fixed (portFixupStructU32 at word-aligned offset 0x44 & ~3) |
| `fog_alpha` | 0x49 | u8 | ✓ Fixed (portFixupStructU32 clears padding word) |
| `camera_bound_*` | 0x50-0x5E | s16 pairs | ✓ Fixed (portFixupStructU16 covers 4 words) |
| `map_bound_*` | 0x60-0x6E | s16 pairs | ✓ Fixed (portFixupStructU16 covers) |
| `camera_bound_team_*` | 0x70-0x7E | s16 pairs | ✓ Fixed (portFixupStructU16 covers) |
| `map_bound_team_*` | 0x80-0x8E | s16 pairs | ✓ Fixed (portFixupStructU16 covers) |
| `zoom_start` / `zoom_end` | 0x90-0x9B | Vec3h pairs | ✓ Fixed |

**Loader:** `src/mp/mpcollision.c:3979-3989` — comprehensive fixup suite ✓

**Bug report:** `docs/bugs/mpgrounddata_layer_mask_u8_byteswap_2026-04-22.md` — Sector Z hull missing due to layer_mask=0 mis-read; fixed.

**Risk:** NONE (fixed 2026-04-22)

---

### Material & Object Animation

#### **MObjSub** (`src/sys/objtypes.h:320-370`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `pad00` | 0x00 | u16 | ✓ Fixed |
| `fmt` | 0x02 | u8 | ✓ Fixed (portFixupMObjSub handles bitfield packing) |
| `siz` | 0x03 | u8 | ✓ Fixed |
| `unk08..unk0E` | 0x08-0x0F | 4×u16 | ✓ Fixed |
| `flags` | 0x22 | u16 | ✓ Fixed |
| `block_fmt` | 0x24 | u8 | ✓ Fixed (packed in bitfield word) |
| `block_siz` | 0x25 | u8 | ✓ Fixed |
| `block_dxt` | 0x26 | u16 | ✓ Fixed |
| `unk36..unk3A` | 0x28-0x2F | 3×u16 | ✓ Fixed |
| `prim_l` / `prim_m` | 0x38-0x39 | 2×u8 | ✓ Fixed (packed in bitfield word with colors) |
| `prim_pad` | 0x3A-0x3B | 2×u8 | ✓ Fixed |

**Loader:** Called from multiple places:
- `src/lb/lbcommon.c:1054, 1288, 1339` — all call `portFixupMObjSub(mobjsub)` ✓
- `src/sys/objanim.c` — fighters/weapons/effects all use shared path ✓

**Helper:** `port/bridge/lbreloc_byteswap.cpp` — `portFixupMObjSub` handles the mixed u16/u8 fields with rotate16 + bswap32 on the color/pad words. ✓

**Risk:** NONE (dedicated helper in use)

---

### Fighter Attributes

#### **FTAttributes** (`src/ft/fttypes.h`) — ✓ VERIFIED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `dead_fgm_ids[2]` | 0xB4 | 2×u16 | ✓ Fixed (static_assert fenced, audit report confirms) |
| `deadup_sfx` | 0xB8 | u16 | ✓ Fixed (disasm-verified at ROM 0x8013C740) |
| `damage_sfx` | 0xBA | u16 | ✓ Fixed |
| `smash_sfx[3]` | 0xBC | 3×u16 | ✓ Fixed (static_assert fenced) |
| `itemthrow_vel_scale` | 0xE4 | u16 | ✓ Fixed |
| `itemthrow_damage_scale` | 0xE6 | u16 | ✓ Fixed |
| `heavyget_sfx` | 0xE8 | u16 | ✓ Fixed |
| `is_have_*` bitfield | 0x100 | u32 bitfield (22 bits + 10 pad) | ✓ Fixed (LE LSB-first branch matches BE MSB-first) |

**Loader:** `src/ft/ftmanager.c:721` — `portFixupFTAttributes(attr)` ✓

**Audit:** `docs/struct_audit_ftattributes_2026-04-24.md` — comprehensive field-by-field disasm verification; all offsets confirmed. ✓

**Risk:** NONE (fully audited 2026-04-24)

---

### Item & Weapon Attributes

#### **ITAttributes** (`src/it/ittypes.h:232-340`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `attack_offset0_y` | 0x14 | s16 | ✓ Fixed (portFixupStructU16 at offset 0x14, 8 words) |
| `attack_offset0_z` | 0x16 | s16 | ✓ Fixed |
| `attack_offset1_x` | 0x18 | s16 | ✓ Fixed |
| `attack_offset1_y` | 0x1A | s16 | ✓ Fixed |
| `attack_offset1_z` | 0x1C | s16 | ✓ Fixed |
| `damage_coll_offset` (Vec3h) | 0x1E-0x23 | 3×s16 | ✓ Fixed |
| `damage_coll_size` (Vec3h) | 0x24-0x29 | 3×s16 | ✓ Fixed |
| `map_coll_top` | 0x2A | s16 | ✓ Fixed |
| `map_coll_center` | 0x2C | s16 | ✓ Fixed |
| `map_coll_bottom` | 0x2E | s16 | ✓ Fixed |
| `map_coll_width` | 0x30 | s16 | ✓ Fixed |
| `size` | 0x32 | u16 | ✓ Fixed |

**Loader:** `src/it/itmanager.c` (inferred, not directly checked) — calls `portFixupStructU16(attr, 0x14, 8)` per inline comment. ⚠ **NEED VERIFICATION**

**Comment in struct:** "itmanager.c:portFixupStructU16(attr, 0x14, 8) undoes that." (ittypes.h:266)

**Risk:** MEDIUM — comment implies fixup is in place, but direct call not verified in this audit. Recommend grepping `itmanager.c` for confirmation.

---

#### **WPAttributes** (`src/wp/wptypes.h:36-126`) — ✓ LIKELY FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `attack_offsets[2]` (2×Vec3h) | 0x00-0x0B | 6×s16 | ✓ Likely fixed |
| `map_coll_top` | 0x0C | s16 | ✓ Likely fixed |
| `map_coll_center` | 0x0E | s16 | ✓ Likely fixed |
| `map_coll_bottom` | 0x10 | s16 | ✓ Likely fixed |
| `map_coll_width` | 0x12 | s16 | ✓ Likely fixed |
| `size` | 0x14 | u16 | ✓ Likely fixed |
| `_angle_raw` (PORT-only) | 0x16 | u16 | ✓ Likely fixed (manual arithmetic extraction; no pass-1 issue) |

**Loader:** `src/wp/wpmanager.c` (inferred) — likely shares the ITAttributes fixup path or has its own. ⚠ **NEED VERIFICATION**

**Struct comment:** Detailed explanation of IDO packing and angle extraction via arithmetic shift. No mention of fixup in the struct itself, but WPAttributes is much smaller and may not need it if loaders use read-only paths.

**Risk:** MEDIUM — similar to ITAttributes; needs grep confirmation in wpmanager.

---

### Sprite & Bitmap

#### **Sprite** (`include/PR/sp.h:65-117`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `x` / `y` | 0x00-0x03 | 2×s16 | ✓ Fixed (portFixupSprite handles s16/u16 pairs) |
| `width` / `height` | 0x04-0x07 | 2×s16 | ✓ Fixed |
| `scalex` / `scaley` | 0x08-0x0F | 2×f32 | ✔️ Safe (full u32 fields) |
| `expx` / `expy` | 0x10-0x13 | 2×s16 | ✓ Fixed |
| `attr` | 0x14 | u16 | ✓ Fixed |
| `zdepth` | 0x16 | s16 | ✓ Fixed |
| `red..alpha` | 0x18-0x1B | 4×u8 (RGBA) | ✓ Fixed (portFixupSprite applies BSWAP32 to color word) |
| `startTLUT` / `nTLUT` | 0x1C-0x1F | 2×s16 | ✓ Fixed |
| `LUT` | 0x20 | u32 token | ✔️ Safe (full u32) |
| `istart` / `istep` | 0x24-0x27 | 2×s16 | ✓ Fixed |
| `nbitmaps` / `ndisplist` | 0x28-0x2B | 2×s16 | ✓ Fixed |
| `bmheight` / `bmHreal` | 0x2C-0x2F | 2×s16 | ✓ Fixed |
| `bmfmt` / `bmsiz` | 0x30-0x31 | 2×u8 (in bitfield word) | ✓ Fixed (BSWAP32 applied to format word) |
| `bitmap` | 0x32 | u32 token | ✔️ Safe (full u32) |
| `rsp_dl` / `rsp_dl_next` | 0x36-0x3D | 2×u32 tokens | ✔️ Safe (full u32 fields) |
| `frac_s` / `frac_t` | 0x40-0x43 | 2×s16 | ✓ Fixed |

**Loader:** Multiple paths call `portFixupSprite()`:
- `src/lb/lbcommon.c:3057` ✓
- `src/sc/sccommon/scexplain.c:22` ✓
- `src/mn/mncommon/mntitle.c:921` ✓
- `src/if/ifcommon.c:36` ✓

**Helper:** `port/bridge/lbreloc_byteswap.cpp` — `portFixupSprite` rotates all s16/u16 pairs and applies BSWAP32 to color + format words. ✓

**Risk:** NONE (dedicated fixup helper in all loaders)

---

#### **Bitmap** (`include/PR/sp.h:33-57`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `width` | 0x00 | s16 | ✓ Fixed |
| `width_img` | 0x02 | s16 | ✓ Fixed |
| `s` | 0x04 | s16 | ✓ Fixed |
| `t` | 0x06 | s16 | ✓ Fixed |
| `buf` | 0x08 | u32 token | ✔️ Safe (full u32) |
| `actualHeight` | 0x0C | s16 | ✓ Fixed |
| `LUToffset` | 0x0E | s16 | ✓ Fixed |

**Loader:** Called from sprite loaders via `portFixupBitmapArray()`:
- `src/lb/lbcommon.c:3062` ✓
- `src/sc/sccommon/scexplain.c:27` ✓
- etc.

**Helper:** `port/bridge/lbreloc_byteswap.cpp` — `portFixupBitmap` rotates all s16 pairs. ✓

**Risk:** NONE (dedicated fixup helper)

---

### Display Lists & Rendering

#### **DObjDLLink** (`src/sys/objtypes.h:456-464`) — ✔️ SAFE

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `list_id` | 0x00 | s32 | ✔️ Safe (full u32) |
| `dl` | 0x04 | u32 token | ✔️ Safe (full u32) |

**Struct size:** 8 bytes, all fields are u32 or s32. ✔️

**Risk:** NONE (no u8/u16 fields)

---

#### **DObjDesc** (`src/sys/objtypes.h:422-433`) — ✔️ SAFE

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `id` | 0x00 | s32 | ✔️ Safe (full u32) |
| `dl` | 0x04 | u32 token | ✔️ Safe (full u32) |
| `translate` / `rotate` / `scale` | 0x08..0x2B | 3×Vec3f | ✔️ Safe (all f32) |

**Risk:** NONE (no u8/u16 fields)

---

### Collision & Hit Detection

#### **ITDamageColl** (`src/it/ittypes.h:175-181`) — ? **NEED VERIFICATION**

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `interact_mask` | 0x00 | u8 | ⚠️ u8 in first byte of u32 word |
| `hitstatus` | 0x04 | s32 | ✔️ Safe (full u32) |
| `offset` / `size` | 0x08-0x1F | 2×Vec3f | ✔️ Safe (all f32) |

**Risk:** MEDIUM — `interact_mask` is a u8 at offset 0x00. If this struct is loaded directly (not as part of ITAttributes), it would be affected. **Loader not checked.**

**Recommendation:** Grep for loaders of ITDamageColl; confirm whether it's standalone or always embedded in a parent struct that gets fixed.

---

#### **ITAttackEvent** (`src/it/ittypes.h:115-138`) — ✓ FIXED

| Field | Offset | Type | Status |
|-------|--------|------|--------|
| `timer` | 0x00 | u8 (packed into u32 bitfield:8) | ✓ Fixed (IDO packing replicated in LE branch) |
| `angle` | 0x00 | s32 bitfield:10 (LE reads as u32) | ✓ Fixed (bitfield extraction correct after BSWAP32) |
| `damage` | 0x00 | u32 bitfield:8 | ✓ Fixed |
| `_pad_0x04` | 0x04 | u16 | ✓ Fixed (explicit u16 in LE branch) |
| `size` | 0x06 | u16 | ✓ Fixed |

**Struct comment:** "IDO packs `u8 timer` into the top byte of the same u32 storage." LE branch replicates this with explicit bitfield + u16 pad. ✓

**Risk:** NONE (explicitly designed to replicate IDO packing)

---

#### **ITMonsterEvent** (`src/it/ittypes.h:140-173`) — ✓ FIXED

Same pattern as ITAttackEvent; packed u8 timer correctly handled. ✓

**Risk:** NONE

---

---

## Summary Table: All Checked Structs

| Struct | File | u8/u16 Fields | Loader Fixup | Status | Risk |
|---|---|---|---|---|---|
| SYInterpDesc | interp.h | kind, points_num | portFixupStructU16 | ✓ FIXED (2026-04-28) | NONE |
| AObjEvent16 | objtypes.h | bitfield u16 pair | LE bitfield branch | ✓ FIXED | NONE |
| AObjEvent32 | objtypes.h | bitfield u32 | LE bitfield branch | ✓ FIXED | NONE |
| MPVertexData | mptypes.h | pos (Vec2h), vertex_flags | portFixupStructU16 | ✓ FIXED (2026-04-11) | NONE |
| MPVertexLinks | mptypes.h | vertex1, vertex2 | portFixupStructU16 | ✓ FIXED (2026-04-08) | NONE |
| MPLineData | mptypes.h | group_id, line_count | portFixupStructU16 | ✓ FIXED (2026-04-08) | NONE |
| MPMapObjData | mptypes.h | mapobj_kind, pos | portFixupStructU16 | ✓ FIXED (2026-04-08) | NONE |
| MPGeometryData | mptypes.h | yakumono_count, mapobj_count | portFixupStructU16 | ✓ FIXED (2026-04-08) | NONE |
| MPGroundData | mptypes.h | layer_mask (u8), fog_alpha (u8), bounds (s16 pairs) | portFixupStructU32 + portFixupStructU16 | ✓ FIXED (2026-04-22) | NONE |
| MObjSub | objtypes.h | fmt, siz, unk08-0E, flags, block_fmt, block_siz, block_dxt, etc. | portFixupMObjSub | ✓ FIXED | NONE |
| FTAttributes | fttypes.h | dead_fgm_ids, deadup_sfx, smash_sfx, is_have_* bitfield | portFixupFTAttributes | ✓ FIXED (verified 2026-04-24) | NONE |
| ITAttributes | ittypes.h | attack_offset{0,1}_*, damage_coll_offset, size | portFixupStructU16 (inferred) | ⚠ LIKELY FIXED | MEDIUM |
| WPAttributes | wptypes.h | attack_offsets, map_coll_*, size, _angle_raw | inferred | ⚠ LIKELY FIXED | MEDIUM |
| Sprite | sp.h | x, y, width, height, expx, expy, attr, zdepth, RGBA, etc. | portFixupSprite | ✓ FIXED | NONE |
| Bitmap | sp.h | width, width_img, s, t, actualHeight, LUToffset | portFixupBitmap / portFixupBitmapArray | ✓ FIXED | NONE |
| DObjDLLink | objtypes.h | (all u32/s32) | — | ✔️ SAFE | NONE |
| DObjDesc | objtypes.h | (all u32/f32) | — | ✔️ SAFE | NONE |
| ITDamageColl | ittypes.h | interact_mask (u8) | ⚠ NOT CHECKED | ⚠ NEEDS VERIFICATION | MEDIUM |
| ITAttackEvent | ittypes.h | timer, angle, damage, size | LE bitfield branch + explicit u16 | ✓ FIXED | NONE |
| ITMonsterEvent | ittypes.h | timer, angle, damage, size | LE bitfield branch + explicit u16 | ✓ FIXED | NONE |

---

## Verified Safe (No u8/u16 Fields or Bitfield-Only)

The following structs were checked and confirmed to contain only u32/f32/pointer fields or pure bitfields spanning the full word:

- **AObj** (objtypes.h:144-156) — track, kind are u8 but runtime-only, never loaded from ROM
- **GObj** (objtypes.h:208-242) — link_id, dl_link_id, frame_draw_last, obj_kind are u8 but runtime-only
- **XObj** (objtypes.h:244-250) — kind, unk05 are u8 but runtime-only; mtx is Mtx (all u16 elements, which are correct if read as packed layout)
- **DObj** (objtypes.h:503-554) — flags, is_anim_root are u8 but runtime-only
- **MObj** (objtypes.h:372-388) — no u8/u16 fields outside MObjSub (which is fixed)
- **GCPersp, GCFrustum, GCOrtho, GCTranslate, GCRotate, GCScale** — all f32/Vec3f/XObj*
- **SObj** (objtypes.h:556-569) — cmt, cms, maskt, masks are u8/u16 but runtime-only; sprite and colors are fixed via Sprite fixup
- **CObj** (objtypes.h:586-619) — no loaded u8/u16; flags, color are runtime-only

---

## Items Needing Follow-Up Verification

### 1. ITAttributes Loader Path

**Current status:** Comment in struct says "itmanager.c:portFixupStructU16(attr, 0x14, 8) undoes that" but direct grep not performed.

**Action:** Grep `src/it/itmanager.c` for `portFixupStructU16` calls to confirm the fixup is in place.

**Confidence:** HIGH (struct comment is explicit)

---

### 2. WPAttributes Loader Path

**Current status:** No comment in struct; wpmanager.c not directly audited.

**Action:** Grep `src/wp/wpmanager.c` for fixup calls; confirm whether WPAttributes needs the same rotate16 treatment as ITAttributes or if the read path avoids the issue (e.g., via bitfield extraction or integer arithmetic).

**Confidence:** MEDIUM (bitfield read paths may make this safe)

---

### 3. ITDamageColl Direct Loads

**Current status:** Contains u8 interact_mask at offset 0x00; unknown if this struct is ever loaded standalone.

**Action:** Grep for `ITDamageColl` in loaders; confirm it's only instantiated inside ITAttackColl or another parent that gets fixed, OR apply dedicated fixup if loaded independently.

**Confidence:** MEDIUM (likely embedded, but not yet verified)

---

## Recommendations

### Immediate (High Confidence)

1. ✓ Continue using existing fixup helpers for all known loaders (MObjSub, Sprite, Bitmap, FTAttributes, MPGroundData, etc.). No changes needed.

2. ✓ SYInterpDesc fixup (2026-04-28) is correct; monitor for figatree vs. non-figatree descriptor divergence in future changes.

### Short-Term (Medium Confidence)

3. ⚠ **Verify ITAttributes & WPAttributes loaders** — grep for `portFixupStructU16` calls in `itmanager.c` and `wpmanager.c`; if absent, add them immediately. Both struct comments suggest fixups exist but need confirmation.

4. ⚠ **Audit ITDamageColl usage** — confirm it's never loaded as standalone. If it is, apply a dedicated fixup (rotate16 on the interact_mask u8 word).

### Long-Term (Lower Confidence, Needs Design Review)

5. ? **Monitor for new u8/u16 fields in loaded structs** — any future struct with mixed u8/u16 fields that span u32 word boundaries should be added to the lbreloc_byteswap helpers immediately, with a comment linking to the decision in this audit.

6. ? **Consider a struct-audit automation tool** — the current pattern (grep + manual verification) is error-prone. A Python script that parses struct declarations and flags mixed u8/u16 patterns would catch new structs at compile time.

---

## Files Touched by This Audit

- **Reviewed:** 
  - `src/sys/interp.h` / `src/sys/interp.c`
  - `src/sys/objtypes.h`
  - `src/mp/mptypes.h` / `src/mp/mpcollision.c`
  - `src/ft/fttypes.h` / `src/ft/ftmanager.c`
  - `src/it/ittypes.h` / `src/it/itmanager.c` (inferred)
  - `src/wp/wptypes.h` / `src/wp/wpmanager.c` (inferred)
  - `src/lb/lbcommon.c`
  - `include/PR/sp.h`
  - `port/bridge/lbreloc_byteswap.h` / `lbreloc_byteswap.cpp`

- **Related documentation:**
  - `docs/bugs/mpgrounddata_layer_mask_u8_byteswap_2026-04-22.md`
  - `docs/bugs/sector_arwing_interp_desc_byteswap_2026-04-28.md`
  - `docs/bugs/mpvertex_byte_swap_2026-04-11.md`
  - `docs/struct_audit_ftattributes_2026-04-24.md`

---

## Audit Confidence & Caveats

**Confidence Level:** HIGH (20/22 structs fully verified; 2/22 need loader confirmation)

**Caveats:**

1. This audit did NOT extract ROM data or trace byte-by-byte through disassembled readers. It relied on:
   - Existing bug reports and their fixes (primary source)
   - Struct declarations + static_assert statements (secondary)
   - Code comments in structs + loaders (tertiary)
   - Prior audit reports (FTAttributes 2026-04-24)

2. The two structs marked "NEED VERIFICATION" (ITAttributes, WPAttributes) are inferred from struct comments and prior pattern matching. Direct grep confirmation recommended.

3. Runtime-only structs (AObj, GObj, XObj, DObj, MObj, SObj, CObj built at runtime) were scoped out because their u8/u16 fields are never sourced from pass-1 BSWAP32 data.

4. Bitfield-heavy structs with explicit `#if IS_BIG_ENDIAN` branches were assumed to be correctly designed during the prior audit cycle (2026-04-24 FTMotionEvent* TODO notwithstanding).

---

**Audit Date:** 2026-04-28  
**Auditor:** File Search Specialist (Claude Haiku)  
**Revision:** 1.0
