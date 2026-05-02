# OpeningJungle TotalTimeTics state leak — 2026-05-02

GH issue: [#71 — Intro sequence breaks on second watch](https://github.com/JRickey/ssb64-port/issues/71)

## Symptom

The opening attract sequence plays correctly on first launch. After the
loop returns to title (via the demo cycle, or via Title→ModeSelect→Title
followed by demo timeout), the next iteration of the attract intro
"breaks" — the user reported the same regression both via natural
demo-cycle progression and via Title→ModeSelect→Title.

No crash, no log error. The scene chain stalls silently mid-intro.

## Root cause

`src/mv/mvopening/mvopeningjungle.c` declares
`s32 sMVOpeningJungleTotalTimeTics;` (BSS) and:

- increments it every tic in `mvOpeningJungleFuncRun` (line 361)
- gates the only scene-exit on `if (TotalTimeTics == 320)` →
  `nSCKindOpeningYoster` (line 370)

There is no `mvOpeningJungleInitTotalTimeTics()` function and
`mvOpeningJungleFuncStart` does **not** zero the counter. Every other
opening overlay that uses the same idiom (`mvopeningcliff.c`,
`mvopeningsector.c`, `mvopeningstandoff.c`, `mvopeningyamabuki.c`,
`mvopeningyoster.c`, `mvopeningclash.c`) does have an explicit
`*InitTotalTimeTics()` call. Jungle is the lone omission.

On real N64 this never manifested: each scene transition re-DMAs the
overlay's BSS to zero on entry, so `sMVOpeningJungleTotalTimeTics`
implicitly resets to 0 every time. On the PC port, overlays are
statically linked — BSS is allocated once at process start and retains
whatever value the previous attract-loop iteration left it at.

The previous iteration always exits at exactly 320 (that's the
transition condition), so the second iteration enters with
`TotalTimeTics == 320` — the equality check at line 370 has already
fired-once-and-passed, the counter rolls past 320 on the next tic and
never returns. Jungle dead-ends; the chain never reaches Yoster /
Newcomers / Title, and the user perceives the intro as "broken".

This is the same class of bug as the BTT result-screen state leak
(`docs/bugs/...` — counters from a prior 1P stage clear leaked into a
later BTT result screen). The general rule from
`docs/feedback_overlay_bss_audit.md`: any global written by an overlay
on PC must be re-zeroed by the overlay's own init path; the BSS-zero
on overlay re-DMA that happened "for free" on N64 does not.

## Fix

`src/mv/mvopening/mvopeningjungle.c`:

```c
void mvOpeningJungleInitTotalTimeTics(void)
{
    sMVOpeningJungleTotalTimeTics = 0;
}

void mvOpeningJungleFuncStart(void)
{
    mvOpeningJungleInitTotalTimeTics();
    sMVOpeningJungleBattleState = dSCManagerDefaultBattleState;
    /* …unchanged below… */
}
```

Matches the cliff/sector/yamabuki pattern verbatim. Not PORT-gated:
on N64 the explicit zero-write is a no-op (BSS is already zero on
re-entry) so the additional call is harmless and brings Jungle in
line with its peers. Worth proposing upstream to
`VetriTheRetri/ssb-decomp-re` as a decomp consistency fix.

A `port_log` was added inside the new init function under `#ifdef
PORT` to make second-iteration behaviour observable — the log line
`SSB64: mvOpeningJungleInitTotalTimeTics entry tics_before=N` shows
N==0 on first entry and N==320 (the leaked value) on every subsequent
entry, confirming the leak existed and the fix neutralises it.

## Audit context

A subagent audit of all `src/mv/mvopening/mvopening*.c` files
verified that Jungle is the only opening scene with this defect.
Every other opening overlay either has a dedicated
`*InitTotalTimeTics()` / `*InitVars()` call from its `FuncStart`, or
unconditionally re-assigns each leak-prone variable before the first
read. See worktree `agent/attract-loop-debug` for the audit report.

## Verification

Direct-entry test (`SSB64_START_SCENE=45 SSB64_MAX_FRAMES=600
./BattleShip`):

```
SSB64: scManagerRunLoop — entering scene 45
SSB64: mvOpeningJungleInitTotalTimeTics entry tics_before=0
```

The init log fires on first entry. End-to-end attract-loop
verification — observing `tics_before=320` on the second iteration
and the chain progressing past Jungle on each iteration — is
left to the parent session's longer attract-cycle run.

## Related

- `docs/feedback_overlay_bss_audit.md` — the general audit discipline.
- `project_btt_result_state_leak.md` (memory) — sibling instance of the
  same class.
- `docs/intro_residuals_2026-04-25.md` — separate intro defects
  (transition explosion, freeze-frame) that are NOT addressed by this
  fix; tracked under issues #72 (camera-blocked Yoshi segment, freeze
  frame mechanism) and #73 (transition explosion + sound).
