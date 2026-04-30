# BTT/BTP result-screen state-machine leak across stages (2026-04-30)

## Symptom

Reported in GitHub issue #27 (linux x86_64 / OpenGL) and reproduced on macOS arm64. After a 10/10 perfect Break the Targets clear in 1P classic mode:

- The "TIMER" tally step never renders. The result screen displays TARGETS: count + bonus stats but skips straight past the "TIMER × 200 = N" animation that should appear between them.
- The screen then "freezes" — the bonus tally doesn't auto-advance through pages and pressing A doesn't exit. Audio (the BTT victory jingle) loops forever; the game thread is still running normally and submitting display lists, but the result-screen state machine has wedged.

The reporter described it as "won't count the bonus score and advance to the next screen by itself."

Build at v0.4-beta does not exhibit the bug; the symptom appeared after additional 1P content became playable.

## Investigation

The state machine in `sc1PStageClearUpdateResultScore` (`src/sc/sc1pmode/sc1pstageclear.c:1987`) advances the BTT/BTP "Result" path through:

1. `tic = 10`: `MakeTargetTextSObjs()` — "TARGETS:" caption.
2. `tic = 20`: `func_ovl56_80132F78()` spawns one target sprite per cleared target; sets `BaseIntervalTic = 20 + cleared * 10` (= 120 for a 10/10 clear).
3. `tic = BaseIntervalTic + 10`: `MakeTimerTextSObjs(126.0F)` — "TIMER:" caption. Only fires for perfect clears (`cleared == 10`).
4. `tic = BaseIntervalTic + 30`: `MakeTimerDigits(126.0F)` — multiplier digits.
5. `tic = BaseIntervalTic + 50`: eject `TimerMultiplierGObj`, append `GetAppendTotalTimeScore` to running total.
6. `tic = BaseIntervalTic + 70`: eject `ScoreTextGObj`, recompute `UpdateBonusScore`. If `IsHaveBonusStats`, set `CommonAdvanceTic = BonusShowNextTic = BonusAdvanceTic = tic + 10` to chain into the bonus-tally page rotator. Otherwise set `IsAllowProceedNext = TRUE`.

Per-frame state log on a fresh build (instrumentation since stripped before commit) showed for a Mario 10/10 BTT after clearing Link, Yoshi, Fox:

```
ResultScore tic=10  stage=3 cleared=10 BaseInt=0   CommonAdv=130 BonusShowNext=170 BonusAdv=190 BonusNum=2 ...
ResultScore tic=20  ... BaseInt=0   CommonAdv=130 BonusShowNext=170 BonusAdv=190 BonusNum=2 ...
ResultScore tic=30  ... BaseInt=120 CommonAdv=130 BonusShowNext=170 BonusAdv=190 BonusNum=2 ...
ResultScore tic=130 ... BaseInt=120 CommonAdv=130 BonusShowNext=170 BonusAdv=190 BonusNum=2 ...
MakeTimerTextSObjs y=126 tic=130 stage=3 cleared=10                              ← timer text built
MakeBonusTable    tic=130 BonusID=0 BonusNum=2                                   ← cleanup-and-transition fires SAME FRAME
BonusStatAll CommonAdv-fire tic=130 BonusID=0 BonusNum=2 IsAdv=0
```

`CommonAdvanceTic = 130 / BonusShowNextTic = 170 / BonusAdvanceTic = 190 / BonusNum = 2` are stale — they were set by Fox's stage clear, the previous result screen, and inherited into BTT.

When `TotalTimeTics` reached 130 (= BaseIntervalTic + 10 = the timer-text step), it _also_ matched the stale `CommonAdvanceTic`. Two mutually-incompatible branches fired on the same frame:

- The boundary-tic branch built the timer-text GObj.
- The cleanup-and-transition block at `sc1pstageclear.c:2110` (`if ((IsHaveBonusStats != FALSE) && (BonusTextGObj != NULL) && (CommonAdvanceTic == TotalTimeTics))`) ejected the just-built `TimerTextGObj`, ejected `TargetGObj`, and ran `MakeBonusTable`.
- `sc1PStageUpdateBonusStatAll`'s `CommonAdv-fire` branch then ran the bonus-stats stat enumeration prematurely.

After this, `BonusShowNextTic = (BonusNum * 10) + tic + 20 = 1*10 + 130 + 20 = 160`, in the past relative to the current tic — the auto-advance comparison `BonusShowNextTic == TotalTimeTics` never triggers again. The screen sits there forever.

## Root cause

PC overlay vs. N64 overlay.

On N64, `sc1PManagerRunLoop` (`src/sc/sc1pmode/sc1pmanager.c:471`) calls `syDmaLoadOverlay(&dSC1PManager1PStageClearOverlay)` before each stage-clear scene. `syDmaLoadOverlay` re-DMAs the overlay's `.bss` image, zeroing every BSS global the overlay owns — including `sSC1PStageClearCommonAdvanceTic`, `sSC1PStageClearBonusShowNextTic`, `sSC1PStageClearBonusAdvanceTic`, and `sSC1PStageClearBonusNum`.

The PC port has overlays statically linked. `syDmaLoadOverlay` is a no-op-equivalent stub. BSS persists across stage-clear scene entries.

`sc1PStageClearInitVars` (`sc1pstageclear.c:1656`) explicitly zeroes most globals on every scene entry — `BonusID`, `BaseIntervalTic`, `IsAdvance`, `IsAllowProceedNext`, `IsSetCommonAdvanceTic`, the GObj arrays, the timer/damage tic targets — but not the four advance-tic globals. They were never an issue on N64 because the overlay re-DMA cleared them.

## Fix

`src/sc/sc1pmode/sc1pstageclear.c:1746` — under `#ifdef PORT`, explicitly zero the four globals at the start of each `InitVars`:

```c
sSC1PStageClearCommonAdvanceTic = 0;
sSC1PStageClearBonusShowNextTic = 0;
sSC1PStageClearBonusAdvanceTic  = 0;
sSC1PStageClearBonusNum         = 0;
```

The `Timer*Tic` and `Damage*Tic` globals don't need this treatment — they're consumed only inside `UpdateGameClearScore` / `UpdateStageClearScore` (Stage/Game kinds) which already hard-set them inside the same `InitVars` call.

## Verification

- 10/10 BTT result screen on a Mario 1P run reaches `tic = 130` with `CommonAdv = 0`, builds the timer text, builds the timer digits at `tic = 150`, ejects the multiplier and computes the time bonus at `tic = 170`, then transitions cleanly into the bonus-stats tally without prematurely ejecting any GObj.
- Auto-advance through `BonusShowNextTic` works: `tic=200 CommonAdv-fire → tic=210 BonusShowNext-fire → tic=230 BonusAdvance-fire → AllowProceedNext = TRUE`. A press exits the scene.
- Regular per-stage clear (Link → Yoshi → Fox) is unaffected — the same `InitVars` runs for `Stage` kind and the four resets are no-ops there since the value-on-entry doesn't matter when `Update*ClearScore` overwrites them via the `Timer*Tic`/`Damage*Tic` schedule.

## Class

This is one instance of a broader PC-port-only category: anywhere the decomp's per-scene `InitVars` relies on overlay-load to zero BSS, statically-linked overlays leak state. See `feedback_overlay_bss_audit` in user memory. The fix pattern is a single `#ifdef PORT` block in `InitVars` listing the missed globals.
