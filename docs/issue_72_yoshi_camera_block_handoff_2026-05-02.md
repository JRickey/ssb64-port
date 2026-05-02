# Issue #72 — "Camera blocked by something" in Yoshi intro segment — Handoff (2026-05-02)

## Summary

GH #72 (filed by `Kriix08`, debug build of `a3e242fd4afe`, Win10 + DX11)
reports that during the Yoshi segment of the opening attract loop, the
camera is "blocked by something for a few frames." The reference correct
behaviour is RMG + GLideN64 + Azimer-HLE + Pure Interpreter (no flash /
no occlusion).

The session task description re-summarised the symptom as a
**1-frame full-framebuffer pink/magenta flash** during Yoshi's tongue
sequence. The **GH issue title and body** call it a "camera blocked"
(occlusion of a few frames), not a colour flash. The two interpretations
may be of the same underlying bug, but I never saw the user's video and
could not confirm pixel-level appearance.

This handoff documents the investigation done this session — the
discovery that **the attract chain is silently skipping two whole scenes
on the port** is the dominant finding and likely the real root cause.

## Critical finding: attract chain silently skips Link (scene 33) and Kirby (scene 36)

The user's `ssb64.log` (attached to the GH issue) shows the
following scene-entry sequence in the opening attract:

```
27 → 28 → 29 → 30 → 31 → 34 → 32 → 35 → 37 → 33
   Startup, Room, Portraits, Mario, Donkey, **Yoshi**, Samus, Pikachu, Run, Fox
```

Per the decomp source the chain should be:

```
27 → 28 → 29 → 30 → 31 → 33 → 32 → 34 → 36 → 33 → 35 → 37
   Startup, Room, Portraits, Mario, Donkey, Link, Samus, Yoshi, Kirby, Fox, Pikachu, Run
```

So on the port:

- After Donkey (31), the chain jumps to **Yoshi (34)** instead of **Link (33)**.
- After Yoshi (34), the chain jumps to **Samus (32)** instead of **Kirby (36)**.
- After Samus (32), the chain jumps to **Pikachu (35)** — correct? No: it should be Yoshi (34) per source.

The **Link** scene and the **Kirby** scene never load. The Yoshi scene
also runs in the wrong slot in the chain (Donkey → Yoshi instead of
Samus → Yoshi).

The chain is hard-coded inside each `mvOpening*FuncRun`:

| File | Line | Transition |
|------|------|-----------|
| `src/mv/mvopening/mvopeningmario.c` | 485 | Mario → Donkey ✓ |
| `src/mv/mvopening/mvopeningdonkey.c` | 523 | Donkey → **Link** (source) — port reaches Yoshi instead |
| `src/mv/mvopening/mvopeninglink.c` | 472 | Link → Samus (source) — never runs |
| `src/mv/mvopening/mvopeningsamus.c` | 509 | Samus → Yoshi (source) — port reaches Pikachu instead |
| `src/mv/mvopening/mvopeningyoshi.c` | 492 | Yoshi → **Kirby** (source) — port reaches Samus instead |
| `src/mv/mvopening/mvopeningkirby.c` | 487 | Kirby → Fox (source) — never runs |
| `src/mv/mvopening/mvopeningfox.c`   | 478 | Fox → Pikachu ✓ |
| `src/mv/mvopening/mvopeningpikachu.c` | 474 | Pikachu → Run ✓ |

These line-number `scene_curr = nSCKind*` writes are unconditional and
correct in source. The fact that Link and Kirby are skipped while
Donkey-→-Yoshi happens means **somewhere between
`scene_curr = nSCKindOpeningLink` and the scene dispatcher's enum
branch in `scManagerRunLoop`, the value is being mis-routed**.

### Wallpaper-stage evidence

`[wallpaper] MakeCommon` log lines for Yoshi (scene 34) show:

```
[ground] InitGroundData scene=34 gkind=4 file_id=265 …
[wallpaper] … path=reloc_stages/StageCastle …
particle_bank[GRHyrule]: bank_id=2 …
```

`gkind=4` is `nGRKindHyrule` (Castle) — but `mvOpeningYoshi` source sets
`gSCManagerBattleState->gkind = nGRKindYoster` (gkind=5). So either:

1. The Yoshi scene itself is corrupt and reads the prior scene's
   gkind, or
2. We are seeing the "wallpaper for the **previous**'s gkind being
   re-resolved during the brief loading interval where scene_curr has
   already advanced" — i.e. an attract-chain transition window.

Given the scene mapping confusion above, hypothesis (1) is unlikely;
hypothesis (2) is consistent with the user's "camera blocked for a few
frames" symptom — the wrong stage's wallpaper appears for a few frames
at the start of the Yoshi segment.

## Where to look next

1. **`src/sc/scmanager.c:scManagerRunLoop`** — the enum dispatch table
   that maps `gSCManagerSceneData.scene_curr` → `*StartScene()` call.
   See line 1179 (`mvOpeningYoshiStartScene`) and the surrounding
   `case nSCKindOpening*` branches.  Is `nSCKindOpeningLink` reachable
   here, or is it falling through?
2. **`scSubsysControllerGetPlayerTapButtons` — A/B/START skip logic.**
   Each opening scene's `FuncRun` checks for player input and short-
   circuits to `nSCKindTitle`. If a stale tap from a previous scene is
   bleeding through (controller-state retention bug), it would explain
   skips, but not the *specific* L/K skips. Worth ruling out.
3. **`mvOpeningDonkey` end conditions.** Check what
   `sMVOpeningDonkeyTotalTimeTics` reaches when `scene_curr = …Link`;
   is it set to a value that triggers an immediate-end branch on the
   *next* scene (Link)?
4. **PR #71's `InitTotalTimeTics` fix for Jungle.** This task was framed
   as "the Jungle fix is already shipped," but a sister bug (where
   another scene's TotalTimeTics is leaking on initial entry to the
   next scene) could cause a scene to immediately exit on tic 0 → see
   the user's "few frames blocked" symptom. Specifically:
   - `mvOpeningLink`'s `InitTotalTimeTics` audit
   - `mvOpeningKirby`'s `InitTotalTimeTics` audit
   - Any scene whose `FuncRun` advances `…TotalTimeTics` on tic 0
     could exit before its `FuncStart` finishes loading.

## Investigation steps tried this session

- Read `src/mv/mvopening/mvopeningyoshi.c`, `mvopeningyoster.c`,
  `mvopeningroom.c`, `interpreter.cpp`, and the `gfx_metal.cpp`
  Clear/Fallback paths. No magenta-fill code, no debug magenta
  fallback, no missing redirect-active guard for the Yoshi scene.
  The "color-image-redirect-to-Z" idiom only appears in
  `mvOpeningRoom` and is correctly guarded post-PR `0148b85`/`5fe2efe`.
- Captured a 2400-frame `port_trace.gbi` of the natural attract chain
  (`SSB64_GBI_TRACE=1`, no `SSB64_START_SCENE`).  Hunted for full-screen
  `(0,0)-(320,240)` `G_FILLRECT` outside the room transition (none),
  for magenta or pink `G_SETPRIMCOLOR` / `G_SETFILLCOLOR` (none —
  only black and 0xFFFCFFFC max-Z).
- Determined that `0xFFFFBE5A` peach `gDPSetPrimColor` in
  `mvOpeningYoshiPosedWallpaperProcDisplay` is the lower-panel posed-
  fighter wallpaper, not full-screen, and nowhere near magenta.
- Observed via the user's log that the attract chain skips scenes 33
  and 36 — see "Critical finding" above. **This is not magenta but
  matches "camera blocked for a few frames" if the wrong stage's data
  is briefly visible during the load.**
- Did **not** view the user's two video attachments (cannot fetch
  images in this session); a future session should view them to
  confirm whether the symptom is colour-flash, occlusion, or both.

## Files referenced

- `/Users/jackrickey/Dev/ssb64-port/.claude/worktrees/attract-loop-debug/.claude/worktrees/issue-72-yoshi-freeze/src/mv/mvopening/mvopeningyoshi.c`
- `…/mvopeningroom.c`, `…/mvopeningyoster.c`, `…/mvopeningdonkey.c`,
  `…/mvopeninglink.c`, `…/mvopeningsamus.c`, `…/mvopeningkirby.c`
- `/Users/jackrickey/Dev/ssb64-port/.claude/worktrees/attract-loop-debug/.claude/worktrees/issue-72-yoshi-freeze/src/sc/scmanager.c` — `scManagerRunLoop` (line ~880-1200)
- `/Users/jackrickey/Dev/ssb64-port/.claude/worktrees/attract-loop-debug/.claude/worktrees/issue-72-yoshi-freeze/libultraship/src/fast/interpreter.cpp` — redirect guards
- `/Users/jackrickey/Dev/ssb64-port/.claude/worktrees/attract-loop-debug/.claude/worktrees/issue-72-yoshi-freeze/libultraship/src/fast/backends/gfx_metal.cpp` — Clear/Fallback
- User's log: <https://github.com/user-attachments/files/27304337/ssb64.log>
- User's videos (port + emulator):
  <https://github.com/user-attachments/assets/edf1c7b1-686c-4128-a726-d49b4266cbc9>
  <https://github.com/user-attachments/assets/89d4f50f-c3a2-47b0-890a-37aa89a505f1>

## Suggested next-session prompt

> Issue #72: pick up handoff `docs/issue_72_yoshi_camera_block_handoff_2026-05-02.md`.
> Step 1: view both attached videos in the GH issue and write a 1-line
> visual summary of the bug (colour flash vs camera-blocked vs both).
> Step 2: instrument `scManagerRunLoop` to log every
> `scene_prev → scene_curr` write (scene_id, source func, tic) so we
> can see why the port walks 30→31→34 instead of 30→31→33. The
> divergence has to be in either scene-id dispatch or in the
> end-of-Donkey transition write. Output an ssb64.log run on the
> attract chain and diff scene order vs source-defined chain.
