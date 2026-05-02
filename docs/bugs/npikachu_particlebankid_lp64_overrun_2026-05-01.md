# NPikachu ParticleBankID LP64 BSS overrun — 2026-05-01

## Symptom

Polygon Pikachu (`fkind=23`, `nFTKindNPikachu`) crashes during fighter spawn
on stage 12 (Fighting Polygon Team) when `ftMainSetStatus` reads
`*fp->data->p_file_submotion`. Fault address has the LP64 token-aliased-as-pointer
shape — observed values `0x400000004`, `0xb0000001b` etc. (`<heap-prefix>_<small>_<small>`).

Crash backtrace (foreground sample):

```
ftMainUpdateMotionEventsAll + 312      ; ldr w8,[x8] where x8 = ms->p_script
ftMainPlayAnimEventsAll
ftMainSetStatus + 5184
ftCommonEntrySetStatus
ftManagerMakeFighter
sc1PGameFuncStart
```

## Root cause

`dFTNPikachuData` declared `p_file_special2 = (void**)&gFTDataNPikachuParticleBankID`.
The slot was originally typed `s32` (4 bytes), so on N64 the cast was harmless —
the special2 file pointer (4 bytes on MIPS) fit perfectly. On LP64, however,
`*p_file_special2 = lbRelocGetStatusBufferFile(...)` writes 8 bytes through that
`void**`. With BSS laid out by the linker as

```
0x100780f50: gFTDataNPikachuMain        (8B)
0x100780f58: gFTDataNPikachuModel       (8B)
0x100780f60: gFTDataNPikachuPad         (4B)
0x100780f64: gFTDataNPikachuParticleBankID (4B s32)   ← p_file_special2
0x100780f68: gFTDataNPikachuSubMotion   (8B)
```

the 8-byte write at `0x100780f64` straddles into `gFTDataNPikachuSubMotion`'s
low 4 bytes. After `ftManagerSetupFilesKind(NPikachu)` runs, the submotion
file pointer reads as

```
high half: 0x00000004    ← left over from the heap pointer's high bits sitting at +4
low half:  0x00000004    ← high half of the special2 ptr that overflowed in
```

i.e. `0x400000004`. `ftMainSetStatus` then computes
`event_script_ptr = motion_desc->offset + (intptr_t)file_head` and stores that
garbage into `ms->p_script`; the first opcode load in
`ftMainUpdateMotionEventsAll` faults.

## Fix

`gFTDataNPikachuParticleBankID` is exclusively used as a Special2-file slot
(no code reads it as a particle bank id — search confirms the only references
are the BSS declaration and the cast in `dFTNPikachuData`). Widen it to
`void *` so the 8-byte write fits properly, and drop the `(void**)` cast in
`dFTNPikachuData`.

Files changed:

- `src/ft/ftchar/ftnpikachu/ftnpikachu.c` — `s32 → void *`
- `src/ft/ftchar/ftnpikachu/ftnpikachu.h` — `s32 → void *`
- `src/ft/ftdata.c` — drop `(void**)` cast

## Repro

Play 1P Game far enough to reach the Fighting Polygon Team stage (or
short-circuit the scene chain for testing).

Before the fix: SIGBUS / SIGSEGV at polygon-fighter spawn with
`fault_addr` of shape `0x<small>_<small>0000`. After the fix: stage
loads and runs.

## Related

- Same family as the AObjEvent32, MBall/Kirby file_head, and audio
  ALParam-overrun LP64 bugs — a 4-byte ROM-layout slot referenced through
  a wider PORT type causes neighbouring BSS to be clobbered.
- `nm BattleShip | grep gFTDataNPikachu` shows the alignment fix moved
  the slot from `0x...f64` (4-byte aligned) to `0x...f68` (8-byte aligned).

## Issue #53 follow-up — sprite/lag still pending

After the crash fix, stage 12 loads but the renderer floods 280k+ frames
with `RelocPointerTable: invalid/stale token` errors at `lbcommon.c:2427`.
A single SObj reports `nbitmaps=36` and walks a Bitmap array whose entries
contain f32-shaped junk (`0x3F800000`, `0x41A00000`, …) and aliased heap
pointer fragments. Likely a separate ROM-layout / sprite-loader bug; this
is the visible "lag" + "stock icons in bottom-left" the reporter saw on
DX11. Tracked separately.
