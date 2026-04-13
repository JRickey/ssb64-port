# Fighter Intro Animation Investigation (2026-04-12, updated)

## 2026-04-12 evening update — figatree pipeline verified correct

**The figatree byteswap pipeline is NOT the bug.** Verified byte-perfect across
all 40 fighter figatree files loaded during a 3000-frame intro run:

- Pre-byteswap data identical to ROM extraction (Torch is lossless)
- Post-`portRelocByteSwapBlob` + chain walk + `portRelocFixupFighterFigatree`
  produces u16 reads that match raw BE u16 reads from ROM for every non-slot
  word, across every file
- Zero `SetTranslateInterp` (opcode 12) occurrences in any loaded figatree
  stream — rules out the f32 inline interpolation corruption hypothesis
- `sFTAnimTraICommandLogCount` never fires either — no TraI track flags set
  in any command seen by the parser

Verification script lives inline in the session transcript; key technique is
diffing `rom_*.bin` (from `debug_tools/reloc_extract/reloc_extract.py`) against
`port_file_*.bin` (from the bridge's `SSB64_DUMP_ALL_FIGATREE=1` mode), then
simulating the full bswap32 → chain walk → ROT16 pipeline in Python and
comparing u16 views.

**Diagnostic infrastructure added:**
- `SSB64_DUMP_ALL_FIGATREE=1` — dumps every `reloc_animations/FT*` and
  `reloc_submotions/FT*` file pre-byteswap to `debug_traces/port_file_<id>.bin`
- `SSB64_LOG_LBRELOC_LOAD=1` — logs every reloc file load (file_id, loc,
  path, is_fighter_figatree)
- `extern void port_log(...)` declarations added to `ftanim.c`, `main.c`,
  `taskman.c`, `mvopeningroom.c`, `scsubsysfighter.c`, `mnstartup.c`,
  `lbcommon.c`, `lbreloc_bridge.cpp` — required for correct variadic calls
  on ARM64 macOS (see `project_fighter_intro_anim.md` memory)
- Fixed a symptom of the missing extern: `mvOpeningRoomFuncStart` was logging
  `figatree heaps allocated size=0xfffffff0` (ARM64 variadic-register garbage).
  After the fix it correctly logs `size=0x3630`.

**Screenshot capture:** Fast3D's `portFastCaptureBackbufferPNG` is only wired
up on the D3D11 backend. On macOS (Metal) it's a no-op stub. Visual verification
of the intro scenes has to go through `screencapture -x` against the full
desktop with the game window raised.

**What was NOT changed:** the figatree byteswap code itself is correct as-is.
Do not modify `portRelocFixupFighterFigatree` in `lbreloc_bridge.cpp:133-157`
— the Python simulation confirms it produces the right u16 sequence for every
opcode position in every loaded fighter figatree.

## Three leads for the next session

### Lead 1: Material animation (matanim event32) parsing

**The hypothesis.** Many fighter intro visuals aren't joint rotations — they're
material swaps. Yoshi's mouth opening is a texture-ID change
(`nGCAnimTrackTextureIDCurrent` writes `mobj->texture_id_curr`), not an arm
bone rotation. Fox's eye blink is the same class. If the matanim parser is
silently skipping or mis-advancing over opcodes, the joint animation still
plays correctly (via the figatree path, now verified) while the texture never
switches off the default frame — which matches the "mouth stays closed"
symptom exactly.

**Where the data lives.** Matanim `AObjEvent32` streams are attached to
`MObj`s via `gcAddMObjMatAnimJoint` (`src/sys/objanim.c:155-167`), which stores
a pointer at `mobj->matanim_joint.event32`. The source pointers come from
`costume_matanim_joints[]` arrays reached through the fighter's
`FTCommonPart::p_costume_matanim_joints` / `FTModelPart::costume_matanim_joints`
(`src/ft/fttypes.h:152-170, 186, 217`). These arrays live inside
`reloc_fighters_main/XxxMain` and `reloc_fighters_main/XxxModel` — **not
figatree files** — so they receive pass1 `bswap32` only, **not** the
figatree `ROT16` fixup. The u32 bitfield view of each event32 should be
correct post-`bswap32`.

**Where to look first.**
1. `src/sys/objanim.c:968 gcParseMObjMatAnimJoint` — the event32 opcode
   parser. Opcodes 0-15 are defined in `src/sys/objdef.h:187-213` as
   `AObjEvent32Kind`. Opcode `nGCAnimEvent32SetVal*` (1..11) write into
   `mat_aobjs[track-MaterialStart]` which are later played by
   `gcPlayMObjMatAnim`.
2. `src/sys/objanim.c:1382 gcPlayMObjMatAnim` — the player. Already has two
   PORT fixes:
   - Color interp byte-order (`25edeb0`) at lines 1513-1569
   - `color = white` init fix (`a7024aa`) at line 1401
   Non-color tracks (TextureIDCurrent/Next, TraU/V, ScaU/V, ScrU/V, LFrac,
   PaletteID) pass through lines 1452-1496 **without any PORT-specific fixup**
   — they just cast `value = value_target` interpolation from f32. Verify:
   - For `nGCAnimTrackTextureIDCurrent` (track 13), `value = aobj->value_target`
     gets written to `mobj->texture_id_curr` at line 1455. The f32→s16-ish
     conversion assumes `value_target` already holds a decodable integer
     loaded from the event32 stream. Check whether the event32 SetVal*
     opcodes read the correct `->f` field — if they happen to read the
     bitfield-overlapping `command.payload` u32 instead, textures would
     stay fixed at index 0 (common "closed mouth" default).
3. The mapping from AObjEvent32 struct to actual bytes. The union places `f`,
   `s`, `u`, and `command` at the same 4-byte slot:

   ```c
   union AObjEvent32 {
       f32 f; s32 s; u32 u; u32 p;  // port: token
       struct { u32 payload:15; flags:10; opcode:7; } command;  // LE
   };
   ```

   On N64, `->f` reads the same bytes that `->command.opcode` reads. On
   port, `bswap32` produces the correct u32 bit pattern → `.command.opcode`
   decodes correctly AND `.f` re-interprets those bits as an IEEE 754 float.
   So the payload float and the opcode/flags should both be right.
   **BUT**: if the event32 stream was ever constructed with per-u16 writes
   assuming BE byte order, and then `bswap32` reorders the bytes, the f32
   value would be corrupted while the u32 integer view would still look
   correct. This is worth verifying by extracting an actual matanim stream
   from ROM, feeding it through `bswap32`, and decoding the f32 values as
   texture IDs (expected small integers in 0..20 range).

**Diagnostic to add.** In `gcParseMObjMatAnimJoint`, add a PORT-only log at
the top of the do-while loop that dumps `command.opcode`, `command.flags`,
`command.payload`, `->f`, and the raw u32 for the first 32 events parsed per
MObj. Compare against a ROM-extracted matanim stream to verify opcode
sequencing and payload decoding.

### Lead 2: Effect/projectile spawning via motion event `SetFlagN` opcodes

**The hypothesis.** Projectile spawns (Fox laser, Yoshi egg, DK shockwave,
Samus missile) are NOT joint animations. They're spawned by per-fighter
status functions that check `fp->motion_vars.flags.flagN` and call
`wpXxxMakeWeapon`. For example
`src/ft/ftchar/ftfox/ftfoxspecialn.c:11-31`:

```c
void ftFoxSpecialNProcUpdate(GObj *fighter_gobj) {
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    if (fp->motion_vars.flags.flag0 != 0) {       // ← gated by flag
        fp->motion_vars.flags.flag0 = 0;
        pos.x = FTFOX_BLASTER_SPAWN_OFF_X;
        gmCollisionGetFighterPartsWorldPosition(fp->joints[FTFOX_BLASTER_HOLD_JOINT], &pos);
        wpFoxBlasterMakeWeapon(fighter_gobj, &pos);
    }
    ...
}
```

`flag0` is set by the motion script event `nFTMotionEventSetFlag0`
(`src/ft/ftmain.c:659`):

```c
case nFTMotionEventSetFlag0:
    fp->motion_vars.flags.flag0 = ftMotionEventCast(ms, FTMotionEventDefault)->value;
    ftMotionEventAdvance(ms, FTMotionEventDefault);
    break;
```

If the motion event parser doesn't fire `SetFlag0` — whether because the
opcode decode is wrong, or the event advance is wrong, or the script pointer
is wrong — the flag never gets set, and the spawn never happens. The figatree
animation still plays (arm swings, mouth opens) because that's a separate
subsystem, but no projectile appears. **This matches "action triggers but
no visual" better than any joint-animation theory.**

**Where the data lives.** Motion scripts are plain `u32[]` streams inside
`reloc_fighters_main/XxxMainMotion` (and `xxxMain` for common actions).
These files get pass1 `bswap32` only. Each `u32` word is one `FTMotionEvent*`
struct. Opcode is in the top 6 bits of the word (bit 26-31 on BE, or
`command.opcode` on either layout via the `IS_BIG_ENDIAN` switch in
`src/ft/fttypes.h:271-500`).

**Where to look first.**
1. **Verify the FTMotionEvent* bitfield LE-reversals in `src/ft/fttypes.h`.**
   There are ~40 such structs starting at line 271. Each one has a
   `#if IS_BIG_ENDIAN ... #else ... #endif` switch with fields in reversed
   order. Every struct must have the reversed-order LE case such that bit
   positions in the raw u32 match between BE and LE. Audit:
   - `FTMotionEventDefault` (line 291): opcode:6 + value:26. ✓
   - `FTMotionEventMakeAttack1..5` (line 315-386): multi-word, opcode only
     in word 1. Audit word 2-5 which have no opcode. ✓
   - Any struct where the total bit count per u32 is not exactly 32 — the
     compiler may pack differently on LE than BE if padding is implicit.
2. **`ftMotionEventCastAdvance` is 4-byte-only.** From `src/ft/ftdef.h:131`:
   ```c
   // WARNING: Only advances 4 bytes at a time
   #define ftMotionEventCastAdvance(event, type) ((type*)(event)->p_script++)
   ```
   Multi-word events like `FTMotionEventMakeAttack` (5 u32 words) must use
   `ftMotionEventAdvance(event, FTMotionEventMakeAttack)` (which advances by
   `sizeof(type)` = 20 bytes). Grep for call sites of
   `ftMotionEventCastAdvance` vs `ftMotionEventAdvance` and verify the right
   one is used for each opcode. An incorrect advance desyncs the entire
   subsequent event stream for that motion — every downstream opcode then
   parses random bytes, and `SetFlag0` may silently never appear at its
   expected offset.
3. **Motion script base offset.** `ftMainSetStatus` at line 4878 computes
   `event_script_ptr = (u8*)event_file_head + motion_desc->offset`. The
   `motion_desc->offset` field in `FTMotionDesc` is `intptr_t`. On LP64
   it's 8 bytes, on N64 it's 4. For port, this field is zero-extended from
   an N64 `u32` (or read from a compile-time C initializer with a 4-byte
   literal like `0x80000000`). Compare against 0x80000000 sentinel works
   cleanly either way, but verify that any motion_desc loaded from a
   RELOC file (not a compile-time C array) has the right offset — a
   mis-sized struct would read the wrong field.

**Diagnostic to add.** In `ftMainUpdateMotion` / `ftMainPlayAnimEventsAll`
(search for the switch over motion event opcodes around `ftmain.c:500-800`),
add a PORT-only log at the top of the switch that dumps `opcode`,
`p_script` byte offset within the source file, and the next u32 raw value.
Compare opcodes across an entire intro run against expected sequences from
Fox's SpecialN / Yoshi's SpecialN motion scripts (extractable from the
fighter main file via the same `SSB64_DUMP_ALL_FIGATREE=1` technique —
extend the dump gate to include main/mainmotion files, or add a second
`SSB64_DUMP_FILE_ID` per-file hook).

### Lead 3: FTMotionDesc layout / submotion array (LP64 landmine)

**The hypothesis.** `src/ft/fttypes.h:92-102` defines:

```c
struct FTMotionDesc {
    u32 anim_file_id;      // 4 bytes
    intptr_t offset;       // 4 bytes N64, 8 bytes LP64 (!)
    FTAnimDesc anim_desc;  // 4 bytes (u32 wrapped union)
};
```

`sizeof(FTMotionDesc)` is **12 bytes on N64** and **24 bytes on LP64**
(clang/gcc macOS/Linux) due to 8-byte `intptr_t` alignment. On LLP64 (MSVC
Windows), `intptr_t` is still 4 bytes so the struct remains 12 bytes.

**Why it might matter.** The `FTMotionDescArray::motion_desc[j]` indexed
in `ftmain.c:4724` uses the C struct stride. If the array source is a
compile-time initializer (like `dFTMarioMotionDescs[]` in
`src/ft/ftdata.c`), the compiler builds the array at the current-ABI stride
and indexing works fine — every write and every read uses 24-byte stride
on LP64. **BUT** if any array was loaded from a reloc file blob — where the
original N64 bytes are packed at 12-byte stride — then indexing via
`motion_desc[j]` reads two N64 entries per LP64 slot, producing garbage.

**What to check.**
1. **Confirm no FTMotionDesc arrays are file-backed.** Grep for
   `lbRelocGet*` calls that return `FTMotionDescArray *` or `FTMotionDesc *`.
   If any file-backed arrays exist (even indirectly via
   `*data->p_file_mainmotion`), the port is silently reading wrong
   `anim_file_id` and `offset` values for every motion that isn't index 0.
2. **Verify the `fp->data->mainmotion` pointer.** `FTData::mainmotion` is
   `FTMotionDescArray *`. In `src/ft/ftdata.c` / `src/sc/scsubsys/scsubsys*.c`
   these are initialized to addresses of static arrays in per-fighter C
   source files (e.g. `dFTMarioMotionDescs`). Confirm that every `FTData`
   entry uses a compile-time pointer, not a file-backed one.
3. **Check every `sizeof(FTMotionDesc)` usage.** The port's runtime
   advancement of the motion_desc pointer must use the current ABI's
   `sizeof`, not a hard-coded 12.

**If a file-backed array is found**, the fix is one of:
- Add a PORT-only conversion pass at load time that walks the file's 12-byte
  N64 entries and copies them into a 24-byte LP64 struct array in the heap,
  rewriting `FTData::mainmotion` to point at the heap copy.
- Or reduce the struct to 12 bytes on LP64 by replacing `intptr_t offset`
  with `u32 offset` under `#ifdef PORT`. This is cleaner but requires
  auditing every reader of `motion_desc->offset`.

**Diagnostic to add.** In `ftMainSetStatus` at line 4724, log
`script_array=%p script_array->motion_desc=%p sizeof(FTMotionDesc)=%zu
motion_id=%d motion_desc=%p anim_file_id=0x%x offset=0x%lx anim_desc.word=0x%x`.
Compare the `motion_desc` address arithmetic against expected values. For
Mario motion 0, the expected `anim_file_id` is `llFTMarioAnimWaitFileID`
which the reloc stub table maps to some known value. If `anim_file_id`
comes out as `llFTMarioAnim002FileID` instead, the stride bug is active.

---

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
