# MBall / Kirby-copy effect `*p_file` reads 8 bytes from a 4-byte reloc token — RESOLVED (2026-04-21)

## Symptom

Selecting Pikachu (or Jigglypuff) would SIGSEGV the game as the fighter transitioned from the hidden "entry" status into the visible "appear" status (where the Poké-Ball-break effect plays). Fault signature:

```
SSB64: !!!! CRASH SIGSEGV fault_addr=0x3a3000003a2
SSB64: pc=… gcSetupCustomDObjs + 120
SSB64: x1=0x3a3000003a2
  0 gcSetupCustomDObjs + 120
  1 efManagerMakeEffect + 808
  2 efManagerMakeEffectNoForce + 28
  3 efManagerMBallThrownMakeEffect + 160
  4 ftCommonAppearSetStatus + 380
  5 sc1PGameWaitStageCommonUpdate + 236
```

The fault-address "two-halves" pattern `0x3a3_0000_03a2` decomposes into two adjacent 32-bit values (`0x3a2 = 930`, `0x3a3 = 931`), matching `portRelocRegisterPointer`'s monotonically-increasing token numbering (`port/resource/RelocPointerTable.cpp:49`, tokens start at 1 and count up per-load).

## Root cause

`efManagerMBallThrownMakeEffect` (and the two Kirby-copy-star companions) need to derive the base address of the `ITCommonData` file from a pointer stored inside the file itself. The decomp does it with the idiom:

```c
void **p_file;
p_file = lbRelocGetFileData(void**, gITManagerCommonData, llITCommonDataMBallThrownFileHead);
file = (void *)((uintptr_t)*p_file - (intptr_t)llITCommonDataMBallThrownDObjDesc);
```

On N64 this works because pointers in a file are 4 bytes, and the reloc-table pass overwrites each 4-byte reloc slot with a 4-byte resolved `void *`.

On the PORT build the layout diverges:

1. `void *` is 8 bytes.
2. The port's reloc bridge (`port/bridge/lbreloc_bridge.cpp:445-451`) does **not** widen the slot — it resolves the target to a real pointer, interns it via `portRelocRegisterPointer` to get a `u32` token, and writes the **4-byte token** back into the slot (`*slot = token;`).
3. The adjacent 4 bytes still hold whatever lives at the next file offset — in `ITCommonData` that is another reloc slot, so it contains another 4-byte token.

`*p_file` on the port therefore reads 4 bytes of token **plus** 4 bytes of the neighbouring token as a single 64-bit `void *`. Subtracting `llITCommonDataMBallThrownDObjDesc` from that garbage still produces garbage. `gcSetupCustomDObjs` then dereferences the garbage as a `DObjDesc *` and faults.

The same pattern appears three times in `src/ef/efmanager.c`, all only exercised via the gameplay paths that were not warming up in prior smoke tests:

| Call site | Fighter path |
|---|---|
| `efManagerMBallThrownMakeEffect`      | Pikachu / Jigglypuff entry animation |
| `efManagerCaptureKirbyStarMakeEffect` | Kirby inhales an opponent |
| `efManagerLoseKirbyStarMakeEffect`    | Kirby's copy ability gets knocked out |

The first is the one that crashed in the user's log. The other two would have the same failure as soon as Kirby's copy mechanic is used; fixed pre-emptively to avoid repeating the investigation.

## Fix

Per call site, read the reloc slot as a `u32` and resolve it through `PORT_RESOLVE` (which calls `portRelocResolvePointer`). Non-PORT builds keep the original `void **` dereference.

```c
#ifdef PORT
    u32 *p_file;
#else
    void **p_file;
#endif
    ...
#ifdef PORT
    p_file = lbRelocGetFileData(u32*, gITManagerCommonData, llITCommonDataMBallThrownFileHead);
    file = (void *)((uintptr_t)PORT_RESOLVE(*p_file) - (intptr_t)llITCommonDataMBallThrownDObjDesc);
#else
    p_file = lbRelocGetFileData(void**, gITManagerCommonData, llITCommonDataMBallThrownFileHead);
    file = (void *)((uintptr_t)*p_file - (intptr_t)llITCommonDataMBallThrownDObjDesc);
#endif
```

Applied at `src/ef/efmanager.c` lines 5254-5272 (MBall), 5884-5911 (Capture Kirby Star), and 5962-5989 (Lose Kirby Star).

## Class-of-bug note

Any `*p = lbRelocGetFileData(T**, file, off)` followed by `*p` where `T` is a pointer type is an LP64 widening trap on this codebase. Under `PORT`, the reloc slot is always a `u32` token, so the pattern is:

```c
u32 *slot = lbRelocGetFileData(u32*, file, off);
T *target = (T *)PORT_RESOLVE(*slot);
```

Three other patterns are already correct and should not be changed:

- `lbRelocGetFileData(T*, file, off)` where `T` is **not** a pointer type: computes `file + off` and yields a typed pointer into the file. No dereference of a reloc slot happens.
- `file_head = &global_void_star;` (used by Samus/DK/Yoshi/Fox/Link entry effects): the "file head" is an already-resolved 8-byte `void *` global (`gFTDataSamusSpecial2` etc.), set by `ftmanager.c:294`. `efManagerMakeEffect` reads `*file_head` → 8 bytes of real pointer. Correct.
- `lbCommonAddDObjAnimJointAll(dobj, lbRelocGetFileData(AObjEvent32**, …), …)`: the file offset points to an array of tokens, and `lbCommonAddDObjAnimJointAll` walks it with `PORT_RESOLVE_ARRAY`, not `*`. Correct.

## Verification

Build `cmake --build build --target ssb64` is clean. Run with Pikachu or Jigglypuff selected and confirm the Poké-Ball-break appear animation plays without faulting.
