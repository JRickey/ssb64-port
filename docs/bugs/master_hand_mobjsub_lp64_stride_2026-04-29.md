## Master Hand Boss-Wallpaper MObjSub Walker LP64 Stride — 2026-04-29

**Status:** RESOLVED (issue [#15](https://github.com/JRickey/BattleShip/issues/15))

## Symptom

SIGSEGV on entry to the 1P Master Hand fight (`nSC1PGameStageBoss`). Crash log:

```
SSB64: !!!! CRASH SIGSEGV fault_addr=0x840002c400000000
0  sc1PGameBossSetupBackgroundDObjs + 512
1  sc1PGameBossSetupBackgroundDObjs + 392
2  sc1PGameBossMakeWallpaperEffect  + 328
3  sc1PGameBossAdvanceWallpaper     + 332
4  sc1PGameBossWallpaperProcUpdate  + 72
```

Fault address `0x840002c400000000` — bytes `0x840002c4` (a PORT reloc token) sitting in the high half of an 8-byte pointer. Classic LP64 stride/token signature.

## Root cause

`sc1PGameBossMakeWallpaperEffect` raw-casts file bytes to a triple pointer:

```c
sc1PGameBossSetupBackgroundDObjs(
    effect_gobj,
    (DObjDesc*)(addr + o_dobjdesc),
    (o_mobjsub != 0) ? (MObjSub***)(addr + o_mobjsub) : NULL,
    nGCMatrixKindTraRotRpyRSca);
```

`sc1PGameBossSetupBackgroundDObjs` then walks both pointer levels with native pointer arithmetic:

```c
mobjsubs = *p_mobjsubs;   /* 8-byte read of a 4-byte file slot */
mobjsub  = *mobjsubs;     /* same problem at inner level */
...
mobjsubs++;               /* strides 8 bytes over 4-byte slots */
p_mobjsubs++;             /* same, outer array */
```

The file region holds two nested arrays of 4-byte tokens. On LP64 `MObjSub**` and `MObjSub*` are both 8 bytes, so each `++` skips a slot and each `*` pulls in half of the next slot — the high bytes of `0x840002c400000000` are the next `MObjSub*` token bleeding into the current dereference.

Same family as the existing **File pointer-array LP64 stride** class (sc1pbonusstage `anim_joints` Break-the-Targets fix).

## Relation to commit `db533f0`

`db533f0` ("Fix sc1pgameboss boss-wallpaper MObjSub fixup", 2026-04-29) added `portFixupMObjSub(mobjsub)` inside the loop. The fixup itself is correct, but it runs *after* the loop has already strided through the array using LP64-sized pointers — so we crash before reaching a real `MObjSub*`. The fix in this entry is upstream of `db533f0`'s call site; `db533f0` stays in place (now reachable via stride-correct walks).

## Fix

`src/sc/sc1pmode/sc1pgameboss.c::sc1PGameBossSetupBackgroundDObjs`: under `#ifdef PORT`, walk both pointer levels as `u32*` (4-byte stride) and resolve every slot through `PORT_RESOLVE`. Mirrors the `anim_joints` walker in `sc1pbonusstage.c::sc1PBonusStageMakeTargets`. Non-PORT path is byte-identical to the original (decomp preserved).

## Test hook (also landed)

To make this stage trivially reproducible without playing through the whole 1P run, the boot-time scene override at `src/sc/scmanager.c` was extended with `SSB64_SPGAME_STAGE`, and `src/mn/mnplayers/mnplayers1pgame.c::mnPlayers1PGameSetSceneData` re-applies it after the menu unconditionally zeros `spgame_stage`:

```bash
SSB64_START_SCENE=17 SSB64_SPGAME_STAGE=13 ./BattleShip
```

`SSB64_START_SCENE=17` lands at the 1P character select (so the 1P-game overlays load); `SSB64_SPGAME_STAGE=13` (`nSC1PGameStageBoss`) selects Master Hand. Stage indices are in `nSC1PGameStageKind` (`src/sc/scdef.h`).

## Verification

- Master Hand stage loads cleanly, no SIGSEGV.
- BGM and stage geometry render correctly. (Audio-effect coverage tracked separately as a follow-up issue.)

## Class of bug

**File pointer-array LP64 stride.** Any function that raw-casts file bytes to `T**` / `T***` and walks them with native pointer arithmetic will stride over 4-byte file slots in 8-byte steps on LP64. Audit pattern: grep for `(T**)` / `(T***)` casts of `file_base + offset` or `addr + o_*` and confirm the walker either uses `u32*` + `PORT_RESOLVE` or applies a struct fixup that widens the slots up front.
