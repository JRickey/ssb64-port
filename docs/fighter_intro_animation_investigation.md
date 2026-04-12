# Fighter Intro Animation Investigation (2026-04-12)

## Problem

During opening/intro scenes, fighters perform wrong animations despite triggering the correct action states. Examples:
- Fox: neutral-B triggers but no laser visual (was doing reflector before fix)
- DK: down-B triggers but animation looks like a brief crouch/roll, not full Hand Slap
- Yoshi: moves forward but mouth stays closed (tongue should extend)
- Mario: jab combo fires but final down-tilt kick was wrong direction

## Fixes Applied (This Session)

### Fix 1: FTKEY_EVENT_STICK Endianness (`src/ft/ftdef.h`)

The `FTKEY_EVENT_STICK(x, y, t)` macro packs stick coordinates into a u16 as `(x << 8) | y`, assuming big-endian byte layout. The `FTKeyEvent` union's `Vec2b stick_range` member reads this via byte-level access. On little-endian PC, the byte order within the u16 is reversed, swapping x and y.

**Impact:** Every stick direction in every key event script was swapped. Fox's left+B became down+B (reflector instead of laser). DK's down+B became left+B (Giant Punch instead of Hand Slap).

**Fix:** `#if IS_BIG_ENDIAN` guard with swapped packing order on LE.

**Verified by logs:** All stick values now decode correctly (Fox: x=-50 y=0, DK: x=0 y=-80, etc.)

### Fix 2: FTAttributes `is_have_*` Bitfield Endianness (`src/ft/fttypes.h`)

22 one-bit `ub32` bitfields control which moves each fighter can perform. These are loaded from file data (bswap32'd). After bswap32, the u32 value is numerically identical, but BE allocates bitfields MSB-first while LE allocates LSB-first — different bits map to different field names.

**Impact:** Move availability flags (`is_have_speciallw`, `is_have_specialhi`, `is_have_catch`, etc.) read wrong bits. Fighters couldn't access certain moves.

**Fix:** `#if IS_BIG_ENDIAN` reversed field declarations with explicit 10-bit padding, matching the pattern used throughout `fttypes.h` for motion event bitfields.

**Verified by logs:** Raw bitfield word `0xFFFFFC00` for Mario/Fox, all 22 flags read correctly.

## Diagnostic Infrastructure Added

- `extern void port_log(...)` declarations added to `ftmanager.c`, `ftmain.c`, `ftkey.c`, `ftcommonspecialn.c`, `ftcommonspecialhi.c`, `ftcommonspeciallw.c` — required on ARM64 macOS where undeclared variadic functions use wrong calling convention (registers vs stack for va_args).
- Key event logging in `ftkey.c` (stick values, button values, raw u16)
- Special move detection logging in `ftcommonspecial{n,hi,lw}.c` (conditions checked, pass/fail)
- Hidden parts guard logging in `ftmain.c:ftMainUpdateHiddenPartID` (token resolution, joint IDs)
- FTAttributes bitfield dump in `ftmanager.c` (raw word + sizeof/offsetof validation)

## Verified Working

| System | Status | Evidence |
|--------|--------|----------|
| Stick direction encoding | FIXED | Log: Fox x=-50 y=0, DK x=0 y=-80 |
| Button detection | OK | Log: B_BUTTON=0x4000, A_BUTTON=0x8000 |
| Move availability flags | FIXED | Raw bitfield 0xFFFFFC00, all flags=1 |
| Special move dispatch | OK | SpecialLwCheck PASS for DK, SpecialNCheck PASS for Fox |
| Hidden parts / joint creation | OK | No BAIL entries, all joints created with valid IDs |
| Animation file loading | OK | Non-NULL figatree pointers, valid file IDs from reloc_data.h |
| Status transitions | OK | Correct status IDs (0xE8=SpecialLwStart, 0xE9=Loop, 0xEA=End) |

## Remaining Issue: Animation Data Visuals

The correct actions trigger and animations load, but **joint transforms are visually wrong**. Fighters do approximately the right thing (correct body movement direction) but specific joints don't animate properly (Yoshi's mouth stays closed, Fox's arm doesn't extend for laser).

### Suspected Root Cause: Figatree Byte-Swap Pipeline

`portRelocFixupFighterFigatree` in `port/bridge/lbreloc_bridge.cpp:133-157` applies ROT16 `((word<<16)|(word>>16))` to all non-relocated u32 words in figatree files. This correctly fixes u16 event pair ordering after bswap32.

**Potential corruption vectors:**
1. **f32 interpolation data** — `nGCAnimEvent16SetTranslateInterp` commands reference `SYInterpDesc` structures with f32 fields. ROT16 corrupts f32 values (they need plain bswap32, not ROT16).
2. **Cross-stream boundary** — If a joint's AObjEvent16 stream has an odd u16 count, the last u16 gets paired with the first u16 of the next stream. ROT16 would swap them across the boundary.
3. **Non-u16 embedded data** — Any data within the figatree that isn't pairs of u16 values (e.g., u32 values, u8 values) would be corrupted by ROT16.

### Next Steps

1. **Dump figatree data** — Use the `SSB64_DUMP_FILE_ID` mechanism in `lbreloc_bridge.cpp` to dump a specific fighter animation file (e.g., Fox neutral-B figatree). Compare byte-for-byte against ROM extraction.
2. **Trace AObjEvent16 parsing** — Add logging to `ftAnimParseDObjFigatree` for one specific action to see decoded opcodes, flags, and target values. Compare against expected values.
3. **Check ROT16 boundary alignment** — Verify that each joint's event stream in figatree files starts at a u32-aligned offset.
4. **Audit for non-u16 data in figatree** — Check if any figatree files contain f32 or u32 data that ROT16 would corrupt.

### Discovered Side Issue: port_log ARM64 Calling Convention

On ARM64 macOS (Apple Silicon), calling `port_log` without a visible `extern void port_log(const char *fmt, ...)` declaration causes the compiler to use implicit function declaration with non-variadic calling convention. The callee (`vfprintf` via `va_start`) reads arguments from the wrong location (stack vs registers), producing garbled output. This affected ALL existing `port_log` calls in decomp `.c` files that don't include `port_log.h`. Fix: add `extern` declarations in each file under `#ifdef PORT`.
