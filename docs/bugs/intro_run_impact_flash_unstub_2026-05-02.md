# Intro OpeningRun Impact Flash + Explosion Sound (issue #73)

**Status:** RESOLVED 2026-05-02

## Symptom

User reported (gh #73): the attract-loop sequence with characters running
together (Mario, Kirby, Link, Yoshi, Fox, DK, Samus, Pikachu) is missing
both the visual flash effect and the explosion boom sound at the climax,
just before the scene transitions out to the Cliff (Link on dark hill)
scene. Reference emulator (RMG with GLideN64 + Azimer HLE + Pure
Interpreter) shows the explosion mesh + plays the boom sound.

The user described the scene as "after Mario and Kirby fight (the
OpeningClash or OpeningStandoff scene)" — but those scenes (41/43) have
their own white-out ending that already works on port. The actual missing
explosion is in `nSCKindOpeningRun` (scene 38, "Characters running on
purple background"), at tic 190 of `mvOpeningRunFuncRun`.

## Root cause

`src/mv/mvopening/mvopeningrun.c:323-336` had a `#ifdef PORT` block
introduced in commit `66e108c` (2026-04-11) that stubbed out both
`mvOpeningRunMakeCrash()` (the impact-flash DObj) and the paired
`func_800269C0_275C0(nSYAudioFGMExplodeL)` (the explosion FGM trigger).

The stub was added to dodge a Fast3D deadlock: the `MVOpeningRunCrash`
reloc file (file id 0x4B) loads a sprite-based MObjSub material chain
whose palette / bitmap structures were not being byte-order fixed up
correctly. At first draw the bitmap pointers resolved as 0x09000000 etc.
and the decomp `while(TRUE)` panic fired.

The same commit (`66e108c`) shipped two infrastructure fixes that
*together* address the underlying byte-order issue:

1. `port/bridge/lbreloc_byteswap.cpp` — `portFixupBitmapArray` and
   `portFixupSpriteBitmapData` now register protected struct ranges so
   `portRelocFixupTextureAtRuntime` won't re-BSWAP previously fixed-up
   memory when an over-sized TLUT load extends past the palette.
2. `src/lb/lbcommon.c` — NULL-bitmap log-and-skip safety net replaces
   the decomp `while(TRUE)` panic.

Those fixes were sufficient to make `mvOpeningRunMakeCrash`'s draw path
safe. The PORT-side stub in `mvopeningrun.c` remained as belt-and-suspenders
but it suppressed the visual effect entirely. The `func_800269C0_275C0`
stub in `n_env.c` (item 3 in the original commit) was already removed
by later audio work (post-`016fb7e5`).

## Fix

Remove the `#ifdef PORT` block in `mvOpeningRunFuncRun` so the original
`mvOpeningRunMakeCrash()` + `func_800269C0_275C0(nSYAudioFGMExplodeL)`
calls fire on PORT just like on N64. Add an `extern void *func_800269C0_275C0(u16 id);`
forward declaration alongside the existing `extern u32 sySchedulerGetTicCount();`.

```diff
diff --git a/src/mv/mvopening/mvopeningrun.c b/src/mv/mvopening/mvopeningrun.c
@@ -7,6 +7,7 @@
 #include <reloc_data.h>

 extern u32 sySchedulerGetTicCount();
+extern void *func_800269C0_275C0(u16 id);
 #ifdef PORT
 extern void port_coroutine_yield(void);
 #endif
@@ -321,18 +322,8 @@ void mvOpeningRunFuncRun(GObj *gobj)
 		}
 		if (sMVOpeningRunTotalTimeTics == 190)
 		{
-#ifdef PORT
-			/* Skip the scene-38 "impact flash" effect on PC.  Drawing its
-			 * DObj the first time deadlocks Fast3D because the crash-file
-			 * MObjSub / material-animation chain isn't running through the
-			 * byte-order fixups that the other scene DObjs go through, and
-			 * the resulting DL reads garbage texture state.  The effect is
-			 * cosmetic (a 30-tick explosion flash between OpeningRun and
-			 * OpeningCliff) — we keep the audio stubbed to match. */
-#else
 			mvOpeningRunMakeCrash();
 			func_800269C0_275C0(nSYAudioFGMExplodeL);
-#endif
 		}
```

## Verification

`SSB64_START_SCENE=38 ./BattleShip` and waiting through the natural
2250-tic pre-roll (~37s) to tic 190 (~40.7s) renders the impact-flash
mesh — purple/yellow concentric rays expanding outward — then transitions
to OpeningCliff at tic 220 (~41.2s). No deadlock. No NULL-bitmap warnings
in `ssb64.log`.

`func_800269C0_275C0(nSYAudioFGMExplodeL)` (FGM ~85, `ExplodeL`) is the
same SFX bound to bombs / capsules / boxes / eggs in gameplay; if those
play, this plays.

## Pointers

| Subject | Path |
|---|---|
| Stubbed call site | `src/mv/mvopening/mvopeningrun.c:322-326` |
| `mvOpeningRunMakeCrash` | `src/mv/mvopening/mvopeningrun.c:207` |
| Crash file reloc id | `llMVOpeningRunCrashFileID = 0x4B` |
| Reloc data offsets | `llMVOpeningRunCrashDObjDesc / MObjSub / MatAnimJoint` in `include/reloc_data.h` |
| Original stubbing commit | `66e108c` (2026-04-11) |
| Protected-range fix | `port/bridge/lbreloc_byteswap.cpp` (`portFixupBitmapArray`) |
| NULL-bitmap safety net | `src/lb/lbcommon.c:lbCommonStartSprite` |
