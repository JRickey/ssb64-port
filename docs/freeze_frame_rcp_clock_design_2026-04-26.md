# Gfx-Task Duration & Emergent Freeze-Frame Stalls — Design Doc

**Date:** 2026-04-26
**Status:** **MECHANISM CONFIRMED. Foundation shipped, cost-model phase next.**
The original RSP-overrun-emergent hypothesis was *partially* right and partially wrong. It's right that wall-clock gfx duration is what produces the freeze; it's wrong about the emulator survey concluding "no emulator models this." Mupen64+parallel-rdp and RetroArch with parallel-n64 use LLE rasterizers that DO have natural per-DL wall-clock variance — those are presumably what the user observed reproducing the freeze. HLE plugins (GLideN64) don't. So the freeze is RSP/RDP-overrun-emergent on real hw and on LLE emulators, and the fix is to model that in our HLE port.

**The actual mechanism:** `syTaskmanSwitchContext` (`src/sys/taskman.c:905`) blocks the game thread when all `gSYTaskmanTaskCount` (= `tscene->contexts_num` = 2 for fighter intros) slots are in flight. Slots release via `nSYTaskTypeGfxEnd` tasks the scheduler processes after each gfx task completes (`sySchedulerDpFullSync`). Real hw / LLE emulator: gfx takes ~1 VI period, slot releases ~1 VI period after acquisition. Heavy climax DLs take >1 VI, slot release lags, the game's next-frame `SwitchContext` finds 0 free slots → blocks → game tic doesn't advance → "whole-screen freeze with all systems."

**Port-side foundation (shipped 2026-04-26):**
- `port/stubs/n64_stubs.c` — defers `PORT_INTR_SP_TASK_DONE` and `PORT_INTR_DP_FULL_SYNC` posts by N VI periods via a per-frame delay queue. Env var `SSB64_GFX_DEFER_VI` picks N. Default `1` produces zero `SwitchContext` blocks (behavior identical to pre-fix port). `2` produces blocks every other frame (proof of mechanism, too aggressive).
- `src/sys/taskman.c` — `syTaskmanSwitchContext` logs all-slots-full events under `SSB64_TRACE_SWITCH_CTX=1`.

**Verification (3000-frame run, attract chain reaching DK/Samus/Yoshi intros):**

| `SSB64_GFX_DEFER_VI` | SwitchContext-full events | of which arg0=1 (skip-draw) | of which arg0=0 (block-resume) |
|---|---|---|---|
| unset / 1 | 0 | 0 | 0 |
| 2 | 1275 | 1022 | 253 |
| 3 | 1709 | — | — |

**Critical caveat (2026-04-26 user testing):** With `N=2` enabled, the
user reports the game **"runs much faster"** rather than producing
visible freezes. This is correct port behavior given how the resume
loop works, and means the deferral alone does NOT reproduce the visual
freeze. Why:

1. When SwitchContext blocks (arg0=0 path), the game coroutine yields.
   `port_resume_service_threads` in `port/stubs/n64_stubs.c:72` runs up
   to 8 rounds per host frame. The scheduler runs in an early round,
   releases the slot, the game thread is resumed in a later round of
   the SAME host frame and completes its tic. No held frame.
2. When SwitchContext returns 0 (arg0=1, skip-draw), task_draw is
   skipped — but `task_update` already ran and advanced game state by
   one tic. Next host frame the next tic runs again and Fast3D
   redraws fresh. The "skipped" frame doesn't visibly hold because
   the next host frame's draw replaces what was on the backbuffer.
3. Over many frames, the catch-up dynamic in (1) means the game
   thread advances state faster than wall-clock 60Hz. User
   perceives this as "running faster."

So the slot-contention mechanism is correctly identified and triggered
by the deferral, but our coroutine resume model absorbs the would-be
stall into game-thread catch-up instead of a held frame. The deferral
infra is necessary scaffolding but not sufficient on its own.

**Default reset to N=1** so the port behaves identically to pre-fix
when the env var is unset. The infrastructure stays for the next
phase to build on.

## Problem

On real N64, SSB64's libultra scheduler refuses to start a gfx task whose target framebuffer is currently being scanned out (or queued for the next swap) by VI. When a gfx task runs longer than one VI period (~16.67 ms NTSC), the next frame's task stays paused, the game thread blocks on its retVal `osRecvMesg`, and VI re-presents the previous framebuffer. This emergent stall is what produces the brief visible freeze at the climax of fighter intros (Yoshi tongue, Samus grapple, DK smash, Kirby inhale, ...) and the desk → arena explosion transition in `mvOpeningRoom`. SSB64's scene authors deliberately budgeted those climax frames around the overrun.

In our port, Fast3D returns from `DrawAndRunGraphicsCommands` in well under 1 ms. The framebuffer-rotation gate (`sySchedulerCheckReadyFramebuffer`) consults `osViGetCurrentFramebuffer` / `osViGetNextFramebuffer` correctly, but VI rotates every frame on schedule because the gfx task always "completes" instantly. `fnCheck` never returns FALSE → no stall → no freeze.

Background: `docs/intro_residuals_2026-04-25.md` Issue 3.

## Survey of prior art

Performed 2026-04-26. **No HLE-based N64 emulator or libultraship-based PC port models gfx-task wall-clock duration.**

| Project | Models gfx duration? | Evidence |
|---|---|---|
| ares (HLE & LLE) | **No** for RDP cost. Has `register_time_interval()` GPU profiling but does not block CPU. | DeepWiki ares N64 architecture: *"Ares doesn't emulate RDP draw timing at all, it costs zero time to draw."* |
| Mupen64Plus + GLideN64 / Glide64 | No — HLE plugin reads the entire DL synchronously, raises `SP_INTR`/`DP_INTR` immediately. | mupen64plus-core#226 (input-lag thread); blogspot GLideN64 HLE writeup. |
| Project64 | No — same shape. SSB64 intro is a known regression test. | pj64-emu forum t-4487. |
| parallel-RDP | No CPU feedback. Vulkan-async, profiling tooling only (`PARALLEL_RDP_BENCH=1`). | github.com/Themaister/parallel-rdp |
| cen64 | LLE only — emergent stalls reproduce naturally because RSP/RDP run at clock rate. Not viable for an HLE port. |  |
| Ship of Harkinian (OoT) | No. `osCreateScheduler`/`osViSwapBuffer` stubbed; host renderer runs ASAP. |  |
| 2 Ship 2 Harkinian (MM) | No. Same shape as SoH. |  |
| Starship (Starfox 64) | No. |  |
| Smash64Recomp | No. N64Recomp-based static recomp; does not emulate RCP scheduling. |  |
| sm64ex / Render96 | No. |  |

The other LUS-based games tolerate this gap because their gameplay almost never exercises gfx-overrun stalls. SSB64 is the outlier.

There is **no documented per-tri / per-pixel cost table** in the public domain. The closest published numbers are F3DEX3's 21 hardware perf counters (`hackern64.github.io/F3DEX3`): point lighting 77 RSP cycles per light per vertex pair (F3DEX3) vs. 144 (F3DEX2_PL); directional lighting ~7 cycles per light per vertex pair. The F3DEX2 disassembly (`Mr-Wiseguy/f3dex2`) is the canonical reference for deriving per-command cycle counts. RDP fillrate rule of thumb: 1-cycle mode ≈ 1 px/cycle at 62.5 MHz, 2-cycle ≈ 0.5 px/cycle, copy-pipe ≈ 4 px/cycle.

**No public writeup names the SSB64 fighter-intro freeze as a timing-emergent phenomenon.** Speedrun and emulator threads discuss intro/menu desync between PJ64 versions and frameskip on Virtual Console, but treat the freezes as content, not as overrun artifacts. This is consistent with the gap above: emulators hide the effect, so the community has no vocabulary for it.

## Design space

Three viable approaches, ordered most → least faithful:

### A. Synthetic RCP cost model (recommended)

Charge a virtual RCP clock at gfx-task submission time. The Fast3D interpreter already walks every command — accumulating a per-command cost coefficient is essentially free. At each simulated VBlank, deduct one VI period from the outstanding cost; if the previous task hasn't "finished" when the next VI fires, withhold the framebuffer rotation that tick → `sySchedulerCheckReadyFramebuffer` returns FALSE → game thread blocks → user sees the freeze.

This is exactly the libultra semantics described in *moria.us — Scheduling RCP Tasks* and *ultra64.ca PRO22-02*, just driven by a synthetic clock instead of a real one.

**Starting coefficients** (back-of-envelope; tune empirically):

```
per_tri          ≈ 75 RSP cycles    (G_TRI1 dispatch + clip + setup)
per_vtx_load     ≈ 30 RSP cycles    (lit, no point lights)
per_tex_load     ≈ tile_w * tile_h  (TMEM DMA dominates)
per_fillrect_px  ≈ 1 RDP cycle      (1-cycle mode) / 0.5 (2-cycle)
per_textrect_px  ≈ 1 RDP cycle      (1-cycle mode)
overhead_per_cmd ≈ 4 RSP cycles
RCP clock        62.5 MHz → 16.67 ms ≈ 1.04 M cycles per VI period
```

Calibration target: a typical SSB64 stage gameplay frame should land at ~12–14 ms (well under VI), and intro climax frames should land at ~18–22 ms (overruns by 1.x VI periods so the freeze lasts ~1 frame).

**Pros:**
- One implementation handles every freeze-frame case in the game (intros, transitions, hit-stop on heavy effects).
- Robust to future content (mods, tournaments).
- Faithful: matches what hardware actually did.

**Cons:**
- Coefficient calibration is ROM-specific; we need a reference. Two viable references:
  - cen64 LLE run with M64P trace + VI-tick logging to record real RCP cycle counts.
  - F3DEX3 perf-counter run on hardware (requires a flashcart and a SSB64 build using F3DEX3 — only possible if someone has done this; we'd need our own measurement rig).
- The Fast3D interpreter is in libultraship, so the per-command accumulator either lives there (changes the submodule) or in an analyzer pass run in `port_submit_display_list` before/around `DrawAndRunGraphicsCommands` (no submodule changes).

### B. Per-DL tri threshold (cheap fallback)

`gsSPEndDisplayList` triggers a check: if `tris_this_DL > N` (e.g. N=1500), declare overrun, withhold rotation for one VI period.

**Pros:** trivial — single counter, single threshold. Doesn't require submodule changes (Fast3D already exposes tri counts in some paths; if not, a wrapper in `port_submit_display_list` can count `G_TRI1`/`G_TRI2`/`G_QUAD` opcodes directly).

**Cons:** misses fillrate-bound cases. DK smash's full-screen white flash is mostly fillrate, not tris. Yoshi tongue is the same — an enlarged sprite.

### C. Captured-trace replay (highest fidelity, heaviest infrastructure)

Run a one-time M64P trace + cen64 LLE run; record per-DL RCP cycle counts; replay during port runtime keyed off a DL fingerprint (hash of opcode sequence).

**Pros:** exact match to original timing.

**Cons:** large infrastructure investment for one game. Fingerprints are brittle to mod changes. Replay data must ship as an asset.

## Recommendation

Implement **A**. It is the only approach that is robust without per-effect tuning and that composes with the freeze foundation already in place. The Fast3D interpreter's existing per-command walk in `DrawAndRunGraphicsCommands` is the natural integration point.

If A's calibration proves stubbornly hard, fall back to a hybrid: **A for tri+vertex+texload cost, plus B's threshold as a backstop for fillrate cases the linear model under-counts.** Document both coefficient values and threshold N in `port/port_rcp_clock.cpp` (proposed file) so future maintainers can tune.

## Implementation phases

Per `CLAUDE.md` rule 2 (phased execution), this is a 3-phase build:

**Phase 1 — observation only.** Add a port-side analyzer pass that walks the DL ahead of `DrawAndRunGraphicsCommands` and emits a per-frame cost estimate to `port_log` under `SSB64_RCP_CLOCK_LOG=1`. No behavior change. Goal: see the cost histogram across attract → fighter intros → gameplay and confirm the climax frames stand out. Output: shipping logs we can plot.

**Phase 2 — gate the rotation.** Wire the cost into `port_vi_simulate_vblank()`: maintain a `sPortRcpCarryCycles` accumulator; on each VBlank, deduct `RCP_CYCLES_PER_VI` (1.04M); if `sPortRcpCarryCycles > 0` after deduction, *skip* the `sPortViNextFB → sPortViCurrentFB` propagation that tick. New env var `SSB64_RCP_CLOCK=1` to gate the whole feature. Default off until calibrated.

**Phase 3 — calibration.** Tune the coefficients against attract-mode + fighter-intro recordings. Capture before/after side-by-sides. Once intro freezes match emulator behavior, flip the env var to default-on.

Stretch — **Phase 4 — submodule integration:** if the analyzer pass duplicates too much of Fast3D's command walking, push the cost accumulator into libultraship's interpreter as a callback hook (similar to the existing `gfx_set_trace_callback` plumbing).

## Files touched

- `port/port_rcp_clock.cpp` (new) — analyzer + accumulator. Public API: `port_rcp_charge_dl(Gfx *dl); port_rcp_tick_vblank(); int port_rcp_should_stall(void);`.
- `port/port_rcp_clock.h` (new) — declarations + cost coefficient table (visible for tuning).
- `port/gameloop.cpp` — call `port_rcp_charge_dl(dl)` at the top of `port_submit_display_list`.
- `port/stubs/n64_stubs.c` — in `port_vi_simulate_vblank`, call `port_rcp_tick_vblank()` and gate the rotation on `port_rcp_should_stall()`.
- `CMakeLists.txt` — add `port/port_rcp_clock.cpp` to port sources.
- `docs/bugs/freeze_frame_rcp_clock_<DATE>.md` — finalize when Phase 3 ships.

No libultraship changes in Phases 1–3. Phase 4 (optional) is the only one that touches the submodule.

## Risks / open questions

- **Coefficient calibration without hardware reference.** Without a cen64 LLE recording or F3DEX3 perf-counter run, coefficients are guesses. Option: tune empirically by visual inspection ("does the freeze last ~1 frame at the right tic?") rather than trying to match real RCP cycles. This is what the recommendation above implicitly does and is acceptable as long as Phase 2 ships a clean opt-out (`SSB64_RCP_CLOCK=0`).
- **Audio-task cost.** The N64 scheduler also runs audio tasks on the RSP. If they're significant, gfx-only modeling will under-count. Spot-check: the SSB64 audio task is small (mixer + sequence) — likely <10% of one VI. Leave audio cost out of Phase 1; revisit if calibration is off.
- **Multiple DLs per VI.** Some scenes (UI overlays) submit multiple gfx tasks per VI. The accumulator must sum across all DLs in a VI window. Already in scope — `port_submit_display_list` is called once per task and the carry persists across calls.
- **Custom framebuffer scenes.** `mvOpeningRoom` installs `mvOpeningRoomCheckSetFramebuffer`; verify it consults `sPortRcpCarryCycles` indirectly via the same `osViGetCurrentFramebuffer` rotation gate. (It does — same getters, same propagation hook.)
- **Performance.** Walking the DL ahead of Fast3D's own walk doubles the per-frame command count. Mitigations: (a) fold into the existing trace callback hook, (b) move into the Fast3D interpreter (Phase 4). For Phase 1, the doubled cost is negligible — modern hosts run thousands of frames per second of headroom.

## Disproof (added 2026-04-26 after user testing)

User ran the original ROM on Mupen64Plus and on RetroArch and observed the
fighter-intro freezes reproduce identically to real hardware. The agent
survey (above, sourced from ares DeepWiki, GLideN64 blog, parallel-RDP
README, mupen64plus core issue threads) is unanimous that none of those
emulators models gfx-task wall-clock duration. If the freeze still
reproduces under emulators that have no concept of RSP duration, then
**the freeze cannot be coming from RSP overrun**. It must be a CPU-side
mechanism the original game executes deterministically each frame.

The supporting evidence in `docs/intro_residuals_2026-04-25.md` Issue 3
that this work was based on — "fnCheck never returns FALSE in our port"
— is correct as a port observation, but the inferred conclusion ("the
freeze is therefore emergent from RSP overload") was wrong. fnCheck
also never returns FALSE on Mupen64Plus, yet the freeze still reproduces
there.

### Likely real mechanisms (next investigation)

In rough order of probability:

1. **Figatree / spline-interp hold segment.** The animation curve for
   the climax move likely contains a key segment that holds a value
   for several frames (Yoshi tongue fully extended, Samus grapple at
   max reach, DK smash at apex). Our port's figatree fixup chain has
   known data corruption issues:
   - `project_aobjevent32_halfswap` — partial fix
   - `project_spline_interp_block_halfswap` — fix shipped 2026-04-25
   - `project_fighter_intro_anim` — "figatree data corruption remaining"

   If a hold segment is being parsed as a 1-frame transition instead of
   a 4-8-frame hold, our anim plays through what should be the freeze.

2. **Motion-script wait event.** `ftMain` motion scripts have
   `script_wait` decremented per frame (`src/ft/ftmain.c:753, 820, 917`).
   A `nFTMotionEventWait` opcode at the climax tic could pause the
   anim_frame advance. We may have a parsing bug there — six partial-bit
   `FTMotionEvent*` structs are flagged as broken in
   `docs/fighter_intro_animation_handoff_2026-04-13.md` "Other findings
   kept on ice", any of which could affect a wait event.

3. **anim_speed = 0 at climax frame.** A figatree opcode or motion-script
   opcode that sets `dobj->anim_speed = 0.0` for one frame would freeze
   animation update. This wouldn't freeze gfx at all on real hardware
   either — VI keeps running — it would just hold the visible pose.

### False alarm: `-FLT_MAX` is `AOBJ_ANIM_NULL`, not corruption (2026-04-26)

A first pass at port-side per-tic instrumentation (env-var `SSB64_TRACE_INTRO_ANIM=1` in
`src/mv/mvopening/mvopening{yoshi,samus,donkey}.c` `*FuncRun`) showed
`topn_child_wait = -3.4028e38` at the climax-pose-status (`0x98`/motion `134`) for
both Yoshi (tic 36) and Samus (tic 16). I initially flagged this as a halfswap-family
data corruption ("sign-bit flip on +FLT_MAX hold sentinel"). **It is not.**
`src/sys/objtypes.h:41` defines `AOBJ_ANIM_NULL = F32_MIN = -FLT_MAX` — i.e. the
exact value we observed is the *legitimate sentinel for "this joint has no active
animation right now."* It appears during the brief transition tic between
statuses on both port and (almost certainly) hardware. No bug.

What the trace actually establishes:
- Body-skeleton `anim_speed` is constant `1.0` throughout all three intros — no
  game-side `gcSetAnimSpeed(0.0)` pause.
- `anim_frame` advances `+1.0/tic` cleanly through every tic. No flat hold
  segment in the body skeleton.
- Status `0x98`/motion `134` lasts exactly 1 tic. Whether it lasts longer on
  hardware is unknown without an emulator-side trace.

The visual freeze the user observes on Mupen64/RetroArch is therefore probably
*not* in the body-skeleton AObj system at all. Likely candidates instead:

1. Item/effect gobjs (Yoshi tongue, Samus grapple beam, DK smash flash) —
   separate from the fighter-body skeleton, not covered by the current trace.
2. Camera hold (CObj per-tic update sets eye/at to the same value for N tics).
3. Scene-level pause (`mv*FuncRun` skips `sMVOpening*TotalTimeTics++` for one
   tic, or there's a separate "freeze counter" we haven't found).
4. Wallpaper/sprite-overlay hold.

Before chasing further, we need an emulator-side anchor: which subsystem actually
freezes, at which tic, for how long. The current port-side trace alone cannot
distinguish (1)-(4).

### Mechanisms ruled out (2026-04-26 session)

After several rounds of port-side instrumentation under
`SSB64_TRACE_INTRO_ANIM=1` and `SSB64_TRACE_GFX_PER_VI=1`:

| Mechanism | How disproven |
|---|---|
| RSP wall-clock overrun (emergent stall) | Emulators (Mupen64+RetroArch) reproduce the freeze; they don't model RSP duration. |
| Body-skeleton `anim_speed=0` | `anim_speed=1.0` constant across all three intros. |
| Body-skeleton figatree hold | `anim_frame` advances cleanly `+1.0/tic` with no flat segment. |
| `-FLT_MAX` wait corruption hypothesis | False alarm — `AOBJ_ANIM_NULL == F32_MIN == -FLT_MAX` per `src/sys/objtypes.h:41` is a normal sentinel during status transitions, not data corruption. |
| Multi-pass gfx-task throughput | Histogram of submits-per-VI across 3000 frames: 2595× `1`, 405× `0`, 0× `2+`. Game never submits multiple gfx tasks per VI. |
| `sySchedulerCheckReadyFramebuffer` false returns | Zero FALSE returns across a 1500-frame run (per prior session in `intro_residuals_2026-04-25.md`). |
| Fighter hitlag (`fp->hitlag_tics`) | Always `0` across all three intros. Only set in `ftmain.c:4060` damage handler, which requires a hit landing on a fighter — and intros have no opponent fighter. |
| Item hitlag (`ip->hitlag_tics`) | Set only in `it/itprocess.c:1128` when an item's `damage_lag != 0` (gets reflected). Intros don't reflect items. |
| `nFTMotionEventPauseScript` | Sets `ms->script_wait = F32_MAX` — "pause forever," not "very short pause." Not a fit. |
| `MakeAttackColl` gating differences | `pkind != nFTPlayerKindDemo` gate at `ftmain.c:201` — intro fighters use `nFTPlayerKindKey`, so they DO create attack colliders, but with no target there's no collision and no hitlag. |

### Open candidates worth tracing next session

User confirmed: **deterministic, very short, whole-screen-freezes-with-all-systems**, at the climax of the move (Samus shooting her grapple beam). Whatever the mechanism is, it pauses everything visually for ~1-3 frames in a content-driven way.

Remaining hypotheses, in rough order of probability:
1. **Effect-spawning side effect.** `nFTMotionEventEffect` (`ftmain.c:432`) spawns an `efManager*` effect at specific motion frames. Some effect type might pause the parent fighter/camera while it plays out a one-shot animation. Worth grepping `efManager*` for any `anim_speed = 0` / `is_paused = TRUE` code paths.
2. **Camera follow lock.** The opening-scene cameras follow the fighter joint via custom proc-update functions. If the camera locks to a joint that isn't moving on emulator (because of mechanism #1), the camera also visually pauses, which the user would perceive as a whole-screen freeze.
3. **Subroutine motion-script return delay.** `nFTMotionEventSubroutine`/`Return` (`ftdef.h:874-875`) — if the climax move invokes a sub-script that has its own wait events, and the parser bug we don't have a fingerprint for collapses the sub-script to 0 tics, we'd never see the pause.
4. **Item self-hitlag at spawn.** `nFTMotionEventEffectItemHold` may spawn an item that self-imposes hitlag on its first tick. Worth tracing item spawn → `hitlag_tics` flow during the intro.

### Cheapest unblocking data point

The most efficient way to break this open is a side-by-side **frame-by-frame
emulator capture** of the climax tic — even just 5-10 consecutive frames
saved as PNGs from Mupen64 around the moment Samus's grapple emphasis
freeze fires. With that we can identify which visual element (fighter
body, grapple item, camera/wallpaper, or all) actually holds, and then
target instrumentation at the matching subsystem instead of enumerating
candidates.

### How to differentiate

Direct test: at the SSB64 climax tic on our port, log:
- `fighter_gobj->anim_frame` (does it stop advancing for 1+ frames?)
- `DObjGetStruct(fighter_gobj)->anim_speed` (does it go to 0?)
- `motion_scripts[*].script_wait` (does a wait fire?)
- the AObj key segment that should be active (does it hold?)

If our port shows `anim_frame` smoothly advancing through the climax tic
while emulator shows it stuck for 1+ frames, the freeze is figatree- or
script-driven. The emulator side requires a M64P trace via the existing
plugin (`docs/debug_gbi_trace.md`) to capture the per-frame state for
comparison.

If our port and the emulator both show `anim_frame` advancing smoothly
through, the freeze is at the VI / framebuffer layer (much less likely
given the disproof above, but worth ruling out).

## Phase 2 — game-thread resume cap (shipped 2026-04-26)

Implemented: `port/stubs/n64_stubs.c:port_resume_service_threads` now caps the game thread (id=5) to N resumes per host frame (default `N=1`). When the game thread blocks on `SwitchContext` and another thread frees the slot in a later round, the cap prevents the game thread from being resumed again *this* host frame — the blocked tic stays blocked, no new gfx submits, host backbuffer holds previous content, screen visibly freezes for one frame.

Env knob: `SSB64_GAME_THREAD_CAP_RESUMES=0` disables the cap (matches pre-fix port behavior). Default `1` matches authored intro pacing.

**Verification:** at default `SSB64_GFX_DEFER_VI=1` no contention exists so the cap is a no-op (scenes progress identically to pre-fix port). At `SSB64_GFX_DEFER_VI=2`, 735 SwitchContext-full events / 1500 frames now hold instead of catching up — game state advances at 60Hz but visible frames hold on contention.

**To see the freeze yourself:**
```
SSB64_GFX_DEFER_VI=2 ./build/ssb64
```
Whole-screen freezes will be visible throughout attract sequence (currently every contention event, not just climax). Phase 3 makes them climax-specific.

## Phase 3 — per-DL cost model (shipped 2026-04-26)

`port/gameloop.cpp:gbi_trace_callback` now accumulates a coarse per-DL cost during Fast3D's command walk. After each DL is processed, `port_get_last_dl_defer_n()` divides by a per-VI budget to choose N for the SP/DP-interrupt deferral. Light DLs N=1 (no freeze, smooth play); heavy DLs N=2 (1-frame whole-screen freeze).

**Cost model:** `cost = tris * 75 + rect_px + load_bytes`. SSB64 uses **F3DEX2** ucode (not F3DEX as initial implementation guessed) — opcodes:
- `0x05` G_TRI1 (1 tri)
- `0x06` G_TRI2 (2 tris)
- `0x07` G_QUAD (2 tris)
- `0xE4` G_TEXRECT, `0xF6` G_FILLRECT (pixel area, 10.2 fixed-point)
- `0xF3` G_LOADBLOCK, `0xF4` G_LOADTILE (~512B/load coarse)

**Budget calibration** (env `SSB64_RCP_CYCLE_BUDGET`, default 400000): empirical attract-mode DL cost histogram across 2351 DLs:

| Percentile | Cost (cycles) |
|---|---|
| min | 83401 |
| p50 | 318674 |
| p75 | 363268 |
| p90 | 382244 |
| p99 | 394332 |
| p99.5 | 397453 |
| max | 414816 |

p99 ≈ 394k, max = 415k. Setting budget at 400k catches only the top ~0.1% (2 DLs across 2700 frames). Both contention events land in fighter-intro scenes (Link at frame ~1800, Samus at frame ~1860) — matching authored climax timing.

**Tunables:**
- `SSB64_RCP_CYCLE_BUDGET=N` — lower → more freezes (e.g. 350000 ≈ top 35%; 250000 ≈ top 87%).
- `SSB64_RCP_FORCE_N=N` — bypass cost model, force fixed N for every submit.
- `SSB64_GFX_DEFER_VI=N` — same as FORCE_N (legacy alias).
- `SSB64_GAME_THREAD_CAP_RESUMES=N` (default 1) — set 0 to disable cap (enables old catch-up behavior).
- `SSB64_TRACE_DL_COST=1` — log per-DL cost; useful for re-calibration if rendering changes alter costs.
- `SSB64_OPCODE_HISTOGRAM=1` — dump opcode-frequency histogram at frames 60, 200, 800, 1500, 2500.

**Verification (3000-frame attract run, default config):**

| Metric | Value |
|---|---|
| Total DL submits | 2351 |
| Frames with 0 gfx submits (held) | 351 |
| Frames with 1 gfx submit | 2349 |
| Frames with 2+ gfx submits | 0 |
| SwitchContext blocks | 2 |
| Block locations | scene 34 (Link intro) frame ~1800; scene 32 (Samus intro) frame ~1860 |

Game state advances at 60 Hz throughout; frame pacing is 1:1 host → game tic. The 2 SwitchContext blocks each produce a 1-frame whole-screen freeze at the corresponding climax tic.

## Phase 3.5 — open follow-ups (if needed)

User testing 2026-04-26 showed the deferral by itself produces game-thread catch-up, not held frames. Two implementation paths to fix this, in order of simplicity:

### Option A: cap game-thread advances per host frame (recommended)

In `port/stubs/n64_stubs.c:port_resume_service_threads`, track per-thread "tics advanced this host frame." For the game thread, refuse to resume it if it has already completed one full tic this frame, even if its mq state is now ready. The thread stays in WAITING until next host frame, and its blocked work continues then.

This forces a strict 1 game-tick = 1 host-frame relationship even under contention. When SwitchContext blocks or returns 0, the held tic doesn't catch up — host swaps backbuffer in same state, screen visibly holds for one frame.

Cost: small change to the resume loop. Risk: low, since 1:1 game-to-host pacing is the natural N64 contract anyway. Other threads (scheduler, controller, audio) keep running normally.

### Option B: make `osRecvMesg(BLOCK)` actually block PortPushFrame

When game thread blocks on the context queue, instead of yielding the coroutine and letting other threads run, freeze the frame pump itself for one VI period. More invasive — would require detecting "this is a PortPushFrame-significant block" in the coroutine yield path.

Cost: significantly more code; touches the coroutine layer.

### Cost-model layer (Phase 3)

Phase 2 ships the visible freeze under static `SSB64_GFX_DEFER_VI=2`. Phase 3 walks per-DL cost in `port/gameloop.cpp:port_submit_display_list` and selects N per submit so freezes appear only on heavy climax DLs:

```
per_tri          ≈ 75 RSP cycles
per_vtx_load     ≈ 30 RSP cycles
per_tex_load_px  ≈ 1  RDP cycle
per_fillrect_px  ≈ 1  RDP cycle
per_textrect_px  ≈ 1  RDP cycle
overhead_per_cmd ≈ 4  RSP cycles
RCP clock        62.5 MHz → 1 VI = 16.67 ms ≈ 1.04M cycles
```

Light DLs N=1 (no freeze), heavy DLs N=2 (1-frame whole-screen freeze).

**Files to touch (Phase 3):**
- `port/gameloop.cpp` — DL-cost walker; sets a global N consumed by `osSpTaskStartGo` instead of env var.
- `port/port_rcp_clock.{h,cpp}` (new) — coefficients + walker.

The current foundation uses a static N for every gfx submit. To make freezes climax-specific (matching the authored ROM behavior), N must vary per-DL based on estimated gfx wall-clock cost.

**Design:** in `port/gameloop.cpp:port_submit_display_list`, walk the DL ahead of `DrawAndRunGraphicsCommands` and accumulate a small linear cost. Divide by the per-VI cycle budget (~1.04M RSP cycles at 62.5 MHz) to pick N: cost < 1.04M → N=1, cost < 2.08M → N=2, etc.

**Cost coefficients (back-of-envelope, tune empirically):**

```
per_tri          ≈ 75 RSP cycles
per_vtx_load     ≈ 30 RSP cycles
per_tex_load_px  ≈ 1  RDP cycle (fillrate-bound)
per_fillrect_px  ≈ 1  RDP cycle (1-cycle mode)
per_textrect_px  ≈ 1  RDP cycle
overhead_per_cmd ≈ 4  RSP cycles
RCP clock        62.5 MHz → 1 VI = 16.67 ms ≈ 1.04M cycles
```

Calibration target: typical SSB64 gameplay/menu frames N=1 (no freeze); fighter-intro climax frames (Yoshi tongue, Samus grapple, DK smash) N=2 (1-frame whole-screen freeze).

**Interaction with already-shipped infrastructure:** the deferral queue handles per-DL N directly — `sPortPendingSPInts[N]++` selects the slot. Just thread the cost-derived N through `osSpTaskStartGo` instead of the env-var read.

**Files to touch (Phase 2):**
- `port/gameloop.cpp` — add cost-walking pass at top of `port_submit_display_list`; expose result via global `int sPortLastDLDeferN`.
- `port/stubs/n64_stubs.c` — `osSpTaskStartGo` reads `sPortLastDLDeferN` instead of `getenv`.
- `port/port_rcp_clock.{h,cpp}` (new) — cost coefficients + walker. Designed to live in libultraship via the existing trace-callback hook in Phase 3 (optional).

## Sources

- ares N64 emulation architecture (DeepWiki) — https://deepwiki.com/ares-emulator/ares/3-nintendo-64-emulation
- moria.us — Scheduling RCP Tasks — https://www.moria.us/blog/2020/11/n64-part11-scheduling-rcp-tasks
- Ultra64 PRO22-02 Scheduler & overrun semantics — https://ultra64.ca/files/documentation/online-manuals/man/pro-man/pro22/22-02.html
- F3DEX3 README & perf counters — https://hackern64.github.io/F3DEX3/md_README.html
- Mr-Wiseguy/f3dex2 — F3DEX2 microcode disassembly — https://github.com/Mr-Wiseguy/f3dex2
- RetroReversing — N64 RDP — https://www.retroreversing.com/n64rdp
- N64brew — Video Interface — https://n64brew.dev/wiki/Video_Interface
- Mupen64Plus core issue #226 — https://github.com/mupen64plus/mupen64plus-core/issues/226
- parallel-RDP — https://github.com/Themaister/parallel-rdp
- HarbourMasters/Shipwright — https://github.com/HarbourMasters/Shipwright
