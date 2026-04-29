# Endian-conditional bitfield struct audit (2026-04-28)

## Purpose

Audit the SSB64 PC port for endian-conditional bitfield structs that may be initialized with positional (non-designated) C initializers. When a struct's field order differs between `#if IS_BIG_ENDIAN` and `#else`, positional initializers like `{1, 2, 3}` bind by *declaration order*, not by bit position. On LE, values can land in the wrong fields.

Known bug class example: `GMRumbleEventDefault` (fixed 2026-04-19) — positional init stored opcode as param, causing battle-start hang.

## Structs with Endian-Conditional Bitfields (Full Inventory)

### Fighter Motion / Animation Events (src/ft/fttypes.h)

**Union with endian-conditional bitfield:**
- `union FTAnimDesc` (lines 59-90) — flags union with reordered bitfields
  - Contains: `is_use_xrotn_joint`, `is_use_transn_joint`, `is_use_yrotn_joint`, `is_enabled_joints`, `is_use_submotion_script`, `is_anim_joint`, `is_have_translate_scale`, `is_use_shieldpose`, `is_use_animlocks`
  - **Status**: SAFE — never positionally initialized. Loaded from ROM file data via motion descriptors.

**Struct with endian-conditional bitfields (nested in FTStatusDesc):**
- `struct FTMotionFlags` (lines 271-290)
  - BE: `s16 motion_id : 10; u16 attack_id : 6`
  - LE: `u16 attack_id : 6; s16 motion_id : 10`
  - **Status**: SAFE (FIXED) — Under `#ifdef PORT`, declares as plain fields `s16 motion_id; u16 attack_id` (not bitfields). Size bumps to 4 bytes but fixes positional init issue. All FTStatusDesc initializations in fighter character files use positional inits and rely on this fix.
  - **Location**: All `dFT*SpecialStatusDescs[]` arrays in `src/ft/ftchar/*/*.h`
  - **Example**: `src/ft/ftchar/ftmario/ftmariostatus.h:7-27`

**FTMotionEvent* structs (motion script bytecode):**
- `struct FTMotionEventDefault` (lines 301-310): opcode:6 | value:26
- `struct FTMotionEventDouble` (lines 312-323): opcode:6 + 2 pad words
- `struct FTMotionEventMakeAttack1` (lines 325-344): opcode:6 | attack_id:3 | group_id:3 | joint_id:7 | damage:8 | can_rebound:1 | element:4
- `struct FTMotionEventMakeAttack2` (lines 346-355): size:16 | off_x:16
- `struct FTMotionEventMakeAttack3` (lines 357-366): off_y:16 | off_z:16
- `struct FTMotionEventMakeAttack4` (lines 368-381): angle:10 | knockback_scale:10 | knockback_weight:10 | is_hit_ground_air:2
- `struct FTMotionEventMakeAttack5` (lines 383-404): shield_damage:8 | fgm_level:3 | fgm_kind:4 | knockback_base:10 | pad:7 (LE has leading pad)
- **Plus 30+ other FTMotionEvent* variants** (FTMotionEventSetAttackOffset*, FTMotionEventSetDamageCollPartID*, FTMotionEventSetHitStatusPartID, FTMotionEventSetColAnimID, FTMotionEventSetSlopeContour, FTMotionEventSetAfterImage, FTMotionEventMakeRumble, FTMotionEventStopRumble, etc.)
- **Status**: SAFE — These are loaded from ROM motion script data files, never initialized with positional initializers in C code.
- **Note**: Some have leading pad bits on LE (e.g., FTMotionEventMakeAttack5, FTMotionEventSetAttackOffset1) to align fields to same physical bit positions as BE.

### Game Manager Color Animation Scripts (src/gm/gmtypes.h)

- `union GMStatFlags` (lines 65-87) — bitfield struct inside union
  - BE: `unused:3 | is_smash_attack:1 | ga:1 | is_projectile:1 | attack_id:10`
  - LE: `attack_id:10 | is_projectile:1 | ga:1 | is_smash_attack:1 | unused:3`
  - **Status**: SAFE — Embedded in FTStatusDesc but FTMotionFlags (the first field) is PORT-safe. GMStatFlags is only initialized as part of FTStatusDesc positional init in character status files, and works correctly because the struct layout matches physical bit positions.

- `struct GMColEventDefault` (lines 161-170): opcode:6 | value:26
- `struct GMColEventSetRGBA2` (lines 237-250): r:8 | g:8 | b:8 | a:8 (byte order reversed on LE)
- `struct GMColEventBlendRGBA1` (lines 258-267): opcode:6 | blend_frames:26
- `struct GMColEventBlendRGBA2` (lines 269-282): r:8 | g:8 | b:8 | a:8 (byte order reversed on LE)
- `struct GMColEventMakeEffect1` (lines 290-305): opcode:6 | joint_id:7 | effect_id:9 | flag:10
- **Plus GMColEventMakeEffect2-4, GMColEventSetLight, etc.**
- **Status**: SAFE — All initialized via `gmColCommand*` macros in `src/gm/gmdef.h` that use manual `GC_FIELDSET()` bit packing, not relying on struct bitfield layout.
- **Location**: `src/gm/gmcolscripts.c` — all color animation event tables use these macros.

### Rumble Event Scripts (src/gm/gmtypes.h)

- `struct GMRumbleEventDefault` (lines 338-345): opcode:3 | param:13
  - BE: opcode first
  - LE: param first
  - **Status**: SAFE (FIXED) — `src/gm/gmrumble.c` uses `#define GMRUMBLE_EV(op, p) { .opcode = (op), .param = (p) }` under PORT, wrapping positional init with designated form.
  - **Fixed**: 2026-04-19 (see `docs/bugs/rumble_event_bitfield_init_2026-04-19.md`)
  - **Example**: All `dGMRumbleEventN[]` tables in `src/gm/gmrumble.c`

### Item Hitbox Events (src/it/ittypes.h)

- `struct ITAttackEvent` (lines 115-138): timer:8 (u8) | angle:10 | damage:8 | pad:14 | size:16
  - BE: `u8 timer` followed by `s32 angle:10 | u32 damage:8 | u32 :14 | u16 size`
  - LE: Complex packing with `u32 :6 | u32 damage:8 | u32 angle:10 | u32 timer:8 | u16 _pad_0x04 | u16 size`
  - **Status**: SAFE — Loaded from file data, never positionally initialized in C.

- `struct ITMonsterEvent` (lines 140-173): Same layout as ITAttackEvent plus additional fields
  - **Status**: SAFE — Loaded from file data.

### Animation Script Events (sys/objtypes.h)

- `union AObjEvent16` (lines 78-99): toggle:1 | flags:10 | opcode:5
  - **Status**: SAFE — Loaded from ROM file data (figatree scripts), never positionally initialized.

- `union AObjEvent32` (lines 102-128): payload:15 | flags:10 | opcode:7
  - **Status**: SAFE — Loaded from ROM file data, never positionally initialized.

## Verification of Safe Practices

### Designated Initializer Usage

**FTStatusDesc arrays** (all character status descriptors):
- Pattern: Positional initializers with inline comments marking each field
- Risk: Early concern, but FTMotionFlags was pre-emptively fixed under PORT
- Example: `src/ft/ftchar/ftmario/ftmariostatus.h:10-27`
```c
{
    nFTMarioMotionAttack13,           // Motion ID → FTMotionFlags.motion_id
    nFTMotionAttackIDAttack13,        // Attack ID → FTMotionFlags.attack_id
    0,                                // GMStatFlags fields...
    FALSE,
    nMPKineticsGround,
    FALSE,
    nFTStatusAttackIDAttack13,
    ftAnimEndSetWait,                 // Callbacks...
    NULL,
    ftPhysicsApplyGroundVelFriction,
    mpCommonSetFighterFallOnEdgeBreak
}
```
- **Result**: Safe because FTMotionFlags uses plain fields on PORT.

**Color Animation Scripts** (gmcolscripts.c):
- Pattern: `gmColCommand*()` macros that manually pack bits
- Example: `gmColCommandSetColor1(0xFF, 0xFF, 0xFF, 0x30)` expands to macro-based packing
- **Result**: Safe — no reliance on bitfield layout.

**Rumble Event Tables** (gmrumble.c):
- Pattern: `GMRUMBLE_EV(opcode, param)` macro wrapper
- **Result**: Safe (fixed with designated init).

## Endian-Conditional Structs WITHOUT Bitfields (Not at Risk)

- `struct SYInterpDesc` (src/sys/interp.h) — conditional byte ordering, not bitfields
- `struct FTModelPart` (src/ft/fttypes.h) — conditional u8 flag padding position
- `struct FTCommonPart` (src/ft/fttypes.h) — conditional u8 flag padding position

## Conclusion

**No critical hits found.** The port has either:
1. **Already fixed** problematic structs (FTMotionFlags, GMRumbleEventDefault)
2. **Avoided positional init** by loading from file data (FTMotionEvent*, AObjEvent*, ITEvent*)
3. **Used manual bit packing** via macros (GMColEvent*, etc.)

All endian-conditional bitfield structs are either safe or have been pre-emptively addressed. The codebase does not exhibit the bug pattern at scale.

### Structs Initialized with Positional Inits (All Safe)

| Struct | File | Initializer | Status |
|--------|------|-------------|--------|
| FTMotionFlags (nested in FTStatusDesc) | src/ft/fttypes.h:271 | FTStatusDesc arrays (ftchar/*/*.h) | SAFE (PORT fix: plain fields) |
| GMStatFlags (nested in FTStatusDesc) | src/gm/gmtypes.h:65 | FTStatusDesc arrays (ftchar/*/*.h) | SAFE (correct bit positions) |
| GMRumbleEventDefault | src/gm/gmtypes.h:338 | gmrumble.c:dGMRumbleEventN[] | SAFE (designated init macro) |

### Structs NOT Initialized in C Code (Safe by Design)

All other endian-conditional bitfield structs are loaded from ROM file data or initialized via bit-packing macros and present no risk of positional-init bugs.

## Recommendations

1. **Document the PORT fix in FTMotionFlags** — consider adding a comment flagging the size difference (2 → 4 bytes) as intentional.
2. **No action needed** — the codebase is audit-clean.

---

Generated: 2026-04-28
