# AObjEvent32 figatree halfswap corruption (2026-04-18)

## Symptom

Title-screen idle into How-to-Play (scene 60, Explain) froze with a
macOS beach ball. After the scheduler-yield and SCExplainPhase LP64
fixes in e6df265 removed the freeze, Mario and Luigi appeared in the
Explain stage but didn't perform any tutorial gestures — the scene
advanced through all 22 phases on the timer but characters stood still.

## Root cause

Fighter reloc files (`reloc_animations/FT*`, `reloc_submotions/FT*`)
are loaded through two port-layer byte-swap passes:

1. **Blanket `BSWAP32`** over the whole blob in
   `port/bridge/lbreloc_byteswap.cpp:pass1_swap_u32`. Makes u32 reads
   produce the same value as on N64 (BE-native).
2. **u16 half-swap** of every non-reloc u32 slot in
   `portRelocFixupFighterFigatree` (`port/bridge/lbreloc_bridge.cpp:134`):
   `w = (w << 16) | (w >> 16)`. Reloc-chain slots (pointer tokens) are
   skipped by this pass.

Pass 2 is **correct** for `AObjEvent16` figatree data — a file u32 slot
holds two u16 commands side-by-side, the blanket u32 swap inverts the
u16 pair order, and the half-swap restores it. That path is covered by
`ftAnimParseDObjFigatree` (`src/ft/ftanim.c`) and confirmed good across
all 40 fighter files (see `memory/project_fighter_intro_anim.md`).

Pass 2 **corrupts** `AObjEvent32` data in the same file. `AObjEvent32`
uses the u32 as a bitfield `{ payload:15, flags:10, opcode:7 }` (LE
layout in `src/sys/objtypes.h`). After the half-swap, the opcode field
lands at bits 9-15 instead of 25-31, and flags split across the
half-swap boundary in a way no bitfield can express. The in-place fix
at read time has to happen on the full u32 value, not the bitfield.

Concrete example: an `nGCAnimEvent32Jump` command (opcode=1, flags=0x210,
payload=128) is stored as BE `0x03080080` (bytes `03 08 00 80`). After
pass 1, RAM contains the same value `0x03080080` (LE u32 read yields the
BE value). After pass 2 half-swap, RAM contains `0x80000803`. LE
bitfield extraction reads opcode from bits 25-31 and gets `0x40 = 64`.

`gcParseDObjAnimJoint` (`src/sys/objanim.c`) switches on opcode 0-17 and
falls into `default: break;` for 18+. The original N64 `break` leaves
`event32` unadvanced and `anim_wait` still ≤ 0, so the outer
`while (anim_wait <= 0)` loop spins forever — the beach-ball freeze.
Commit e6df265 replaced that with an AOBJ_ANIM_END / return stopgap to
prevent the hang but rendered EVENT32 animations dead on arrival.

A file-load-time fix is blocked because the same figatree file contains
both EVENT16 and EVENT32 streams; which tokens point to which type is
decided at runtime via `fp->anim_desc.flags.is_anim_joint` per motion.

## Fix

Lazy per-stream un-half-swap at the EVENT32 reader entry points, in a
new module `port/port_aobj_fixup.{h,cpp}`:

- `port_aobj_register_halfswapped_range(base, size)` — called from
  `portRelocByteSwapBlob`'s caller in `lbreloc_bridge.cpp` after
  `portRelocFixupFighterFigatree` runs, so the walker knows which RAM
  regions were half-swapped and doesn't touch anything else.
- `port_aobj_event32_unhalfswap_stream(head)` — called at the top of
  `gcParseDObjAnimJoint` and `gcParseMObjMatAnimJoint`. Two-phase:
  1. **Scan**: walk the stream opcode-by-opcode, compute each slot's
     un-half-swapped value *without writing*, collect the address list.
     Validate every opcode is 0-23; validate flag-field popcount bounds.
  2. **Apply**: if the scan reached a proper terminator (End, Jump,
     SetAnim) with all-valid opcodes, apply the half-swap fix to every
     collected address in a second pass. Otherwise mark the head in
     `sRejectedHeads` and leave data untouched.
  Idempotent via `sUnswappedHeads` visited set. Recurses through
  Jump/SetAnim targets with cycle protection.

The event-size table mirrors the decomp parser's switch in
`src/sys/objanim.c:455` (DObj) and `:1049` (MObj). Token slots inside
Jump/SetAnim/SetInterp are left alone — the reloc chain already wrote
native u32 token indices there.

The `default:` case in `gcParseDObjAnimJoint` retains a log-and-end
safety net under `#ifdef PORT`: if the walker ever mis-parses (or a
rejected stream gets read), the parser terminates the animation instead
of hanging.

## Verification

`SSB64_MAX_FRAMES=12000` run:

- No `WATCHDOG HANG` messages.
- Scene progression `27 → 28 → 29 → … → 46 → 1 → 60 → 26` — all 22
  Explain phases plus transition to Characters.
- 24 `UNHANDLED opcode` messages — these are the safety-net firing on
  streams the walker rejected as non-EVENT32. The walker did not
  modify those streams (validate-first-apply-later), so they're left
  in their original half-swapped state for whatever reads them next.
- User-visible: Mario and Luigi perform the tutorial gestures in
  Explain; opening-scene fighter animations still look the same as
  before (they go through the EVENT16 path, which the walker never
  touches).

## Known limitations

- A given stream is assumed to only ever be read as EVENT32. If the
  same stream bytes are walked once (un-half-swapped) and then read as
  EVENT16 later, the u16 reads would be broken. Not expected — stream
  type is authored per-offset — but worth flagging.
- The walker's event-size table must match the decomp parser. New
  opcodes added to `gcParseDObjAnimJoint` require a matching change
  in `port_aobj_fixup.cpp`.
- The 24 UNHANDLED-opcode streams we currently reject probably include
  legitimate EVENT32 data that uses an opcode or flag-layout the scan
  doesn't recognise. If any of those are essential for gameplay, the
  scan validator needs widening (separately investigable via log).
