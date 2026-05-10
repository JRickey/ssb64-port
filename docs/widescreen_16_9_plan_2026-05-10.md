# 16:9 Native Widescreen — Implementation Plan

**Status:** Planning (2026-05-10)
**Worktree:** `agent/widescreen-16-9` at `.claude/worktrees/widescreen-16-9/`
**Toggle:** menu option (CVar-gated; default off until proven stable)

## Goal

**Native 16:9**, not stretched. The camera shows *more world horizontally* — same vertical FOV as 4:3, just a wider window. HUD anchors to the new edges; authored full-screen 2D effects (freeze frames, fades, transitions) get a sensible policy (letterbox or scale, decided per-class).

Non-goal: arbitrary aspect ratios (21:9 etc.). 16:9 first; the architecture should generalize but we won't validate beyond 16:9 in this PR.

---

## Constraints we must honor

1. **Decomp preservation (CLAUDE.md rule 5):** changes to `decomp/src/` should be wrapped in `#ifdef PORT` *only when the change alters decomp call sites*. Prefer keeping decomp pristine and intercepting at the GBI / port boundary where possible.
2. **Toggle-gated:** every behavior change reads `CVarGetInteger(kWidescreenCVar, 0)`; CVar=0 ⇒ exact current 4:3 behavior, byte-for-byte. This lets us ship incrementally and bisect regressions.
3. **MIT attribution:** any file we lift from another port (Ship of Harkinian, Starship, 2 Ship 2 Harkinian, etc.) gets the SPDX/Portions-derived header that `port/audio/mixer.c:1-8` uses.

---

## Architecture overview

There are three planes that have to agree for native 16:9 to look right:

| Plane | What sets it today | What 16:9 needs |
|-------|---------------------|-----------------|
| **Window / framebuffer aspect** | libultraship reads window size each frame (`Fast3dWindow.cpp:254` `GetAspectRatio()`) | Already aspect-aware. No change needed. |
| **Game viewport (`syRdpSetViewport`)** | Hardcoded 4:3 bounds (e.g. `10..310, 10..230`) per overlay | Widen X bounds when CVar on. Source of truth: a port-side `port_widescreen_get_viewport_bounds()` helper. |
| **Projection matrix aspect** | Computed from viewport dimensions in `gmcamera.c` style code (`(lrx-ulx)/(lry-uly)`); also a hardcoded `15.0F / 11.0F` baseline (`gmcamera.c:18-19`) | Widens automatically once viewport widens, **but** the hardcoded baseline needs override. |
| **HUD / 2D screen-space** | Hardcoded constants `160`, `120`, `320`, `240` scattered across `mp/`, `mvopening*`, `sc1p*` files | Anchor system: each HUD draw declares "anchor: left / right / center / stretch". Centered/stretch unchanged; left/right offset by `(widescreen_width - 320)/2 * sign`. |
| **Authored full-screen 2D (fades, wipes, freeze frames)** | Draw a textured rect across full FB | **Default policy: letterbox** (constrain to 4:3 region, draw black bars). Reason: re-authoring these in widescreen is out of scope. |

Two render decisions follow from this:

- **Camera (3D world):** widen viewport ⇒ wider FOV horizontally. Same vertical content as 4:3. Stage edges that were off-screen become visible. We accept that some stages have visible authored boundaries (e.g. Hyrule Castle has scenery cutoffs); document any stages where this looks bad.
- **2D screen overlays:** anchor-aware. Most HUD already centers around `x=160`; those become `x = screen_center` and stay correct. Per-corner sprites (stock icons, timer, ready/go) need explicit anchor metadata.

---

## Phase plan

### Phase 0 — Plumbing (no behavior change)

Goal: land all infrastructure with CVar default 0, so this is a no-op merge that future phases build on.

Files to add (all auto-globbed by `CMakeLists.txt:182-183`):

- `port/widescreen/widescreen.h`
  - `bool port_widescreen_enabled();`
  - `float port_widescreen_aspect();` — returns `16.0f/9.0f` if enabled, else `4.0f/3.0f`
  - `void port_widescreen_get_viewport(s32* ulx, s32* uly, s32* lrx, s32* lry, s32 base_ulx, s32 base_uly, s32 base_lrx, s32 base_lry);` — given the decomp-baked 4:3 bounds, returns widened bounds (or pass-through when off)
  - `s32 port_widescreen_anchor_x(s32 base_x, port_widescreen_anchor_t anchor);` — for HUD anchor offsets
  - `bool port_widescreen_should_letterbox_2d();` — flag for full-screen authored effects
- `port/widescreen/widescreen.cpp` — implements the helpers, reads `CVAR_WIDESCREEN_ENABLED`
- `port/enhancements/enhancements.h` (existing) — add `const char* WidescreenCVarName();`
- `port/widescreen/widescreen.cpp` — owns the CVar string `gEnhancements.Widescreen` (mirroring `kStageHazardsDisabledCVar` pattern from `port/enhancements/DisableStageHazards.cpp:8`)
- `port/gui/PortMenu.cpp` — register checkbox under the existing **Enhancements** sidebar, mirroring the `StageHazardsDisabledCVarName()` registration at `PortMenu.cpp:314`. Tooltip: "Renders the game in native 16:9 widescreen. Some stage backgrounds may show visible edges. Restart not required."

Verification: build clean; toggle the menu item; `CVarGet` reads correct values. No rendering change yet.

### Phase 1 — Projection / viewport (3D world)

This phase makes the camera widescreen. HUD will misalign in this phase — that's expected and gets fixed in Phase 2.

Two integration strategies, in decreasing order of preference:

1. **Decomp-side: replace the hardcoded `15.0F / 11.0F` baseline.** `decomp/src/gm/gmcamera.c:18-19` is the only literal aspect; everywhere else computes from viewport bounds. Wrap the literal in `#ifdef PORT` and read from `port_widescreen_aspect()`.
2. **Viewport-side: widen `syRdpSetViewport` calls.** The decomp computes aspect from `(lrx - ulx) / (lry - uly)` (sample at `sc1pgameboss.c`), so widening the viewport propagates to projection automatically. But there are *many* viewport call sites and each is per-overlay. We don't want to touch them all — instead intercept `syRdpSetViewport` itself in `decomp/src/sys/rdp.c:71-82` with a `#ifdef PORT` widen step that calls `port_widescreen_get_viewport()`.

The combination of (1)+(2) gives correct projection across all overlays without touching individual stage code.

Risk: scissor in `syRdpResetSettings` (`rdp.c:106-114`) already scales by `gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT`. Verify that widening the *logical* (320-space) viewport bounds is the right place to inject the change vs. widening the *resolved* pixel bounds. Likely the logical bounds — the existing scaling math should then propagate.

Per-stage acceptance: cycle through every stage (single-player + VS) on widescreen, capture a screenshot, eyeball for severe edge artifacts. Document stages where authored backgrounds run out (likely candidates: Saffron, Sector Z, possibly Hyrule).

### Phase 2 — HUD anchoring

Once the camera is widescreen, the HUD that was centered on x=160 in a 320-wide screen now sits on the *left* of a wider screen. Two modes:

- **Centered HUD elements** (damage %, "go!" banners): no code change needed if they compute from a screen-center constant — but they don't, they hardcode `160`. We change them to anchor.
- **Edge-anchored elements** (stock icons in corners, timer top-right): need explicit anchoring.

Approach: introduce `PORT_WIDESCREEN_ANCHOR_X(base, anchor)` macro that's a no-op when CVar is off. Audit and update call sites by category, not en masse:

1. **Multiplayer match HUD** (`decomp/src/mp/`): damage%, stock icons, timer, ready-set-go, victory pose backdrops. Highest user value — do this first.
2. **Single-player intermission screens** (`sc1pintro`, `sc1pgameboss`, `sc1pbonusstage`): timer digits, score, character intros.
3. **Menus** (CSS, stage select, options): mostly already centered; verify and patch outliers.
4. **Opening / attract / credits** (`mvopening*`): low priority. Likely letterbox these wholesale — Phase 3.

Per category, list the screen-space coordinate constants, classify each as `LEFT / RIGHT / CENTER / STRETCH`, then apply.

Gotcha: the CSS panel garble investigation (`docs/css_panel_garble_investigation_2026-04-29.md`) and CSS selected-anim VFX investigation may interact with anchor changes — read those first before touching CSS.

### Phase 3 — Full-screen 2D effect policy (letterbox or scale)

Authored full-screen 2D draws — fades, wipes, freeze-frame (`docs/freeze_frame_rcp_clock_design_2026-04-26.md`), the VS results photo wipe (commit `b86aab6`) — were authored as 320×240 textures. They cannot widen without re-authoring or visible stretch.

Default policy: **letterbox** these to the 4:3 sub-region of a widescreen frame. Implement at the GBI level by detecting full-screen `gSPTextureRectangle` / large-tri draws and clipping the scissor to the centered 4:3 region for the duration of the draw.

`docs/audit_fast3d_gaps_2026-04-28.md` may have prior notes on which draw categories are worth gating. Read before designing the gate.

The freeze-frame retune work (memory: `Freeze-frame RE-LAND in PR #62`) already exposes a `SSB64_RCP_RECT_GATE` env. Reuse the same machinery: a `letterbox region` filter that activates only when widescreen is on.

Stretch goal (post-merge): any single full-screen effect we want to actually widen gets a per-call carve-out (e.g. Smash logo on title can probably stretch without harm).

### Phase 4 — Polish

- Stage-by-stage audit. Document edge artifacts in `docs/widescreen_stage_audit.md`.
- Default on? Probably not for first PR — keep gated, advertise as opt-in. Reassess after a week of use.
- `docs/bugs/widescreen_<topic>_2026-XX-XX.md` for any non-obvious fixes during implementation.

---

## Code we may borrow (with attribution)

Strong candidates from MIT-licensed N64 PC ports:

- **Ship of Harkinian (HarbourMasters/Shipwright)** — `OTRGlobals::HasMasterQuest`, `gSPGrayscale`, `OTRGlobals::IsGameMasterQuest` patterns; `update_screen_aspect_ratio` if present. License: MIT. URL: `https://github.com/HarbourMasters/Shipwright`.
- **2 Ship 2 Harkinian (HarbourMasters/2ship2harkinian)** — Majora's Mask port, has more recent widescreen plumbing.
- **Starship (HarbourMasters/Starship)** — Star Fox 64 port. Already a referenced source for `port/audio/mixer.c`. License: MIT. Likely has the cleanest projection-matrix widescreen patches because SF64's render path is the closest to SSB64's (both use F3DEX).

If we lift any helper file (e.g. an aspect-correction matrix utility), it gets the standard header — sample from `port/audio/mixer.c:1-8`:

```c
/*
 * SPDX-License-Identifier: MIT
 *
 * Portions of this file are derived from the Starship (Star Fox 64) PC port
 *   Copyright (c) The Harbour Masters
 *   https://github.com/HarbourMasters/Starship
 * Licensed under the MIT License; see LICENSE at repository root.
 */
```

For derived but heavily-rewritten code, attribute as "Derived from"; for verbatim file lifts, "Originally from" with the original copyright line preserved alongside ours.

---

## Open questions to revisit during implementation

1. **Is there a `15.0F/11.0F` somewhere else, not just `gmcamera.c:18-19`?** Grep before we assume one literal. (`Fast3D` may also bake an aspect into a fallback ortho.)
2. **Does the freeze-frame gate (`SSB64_RCP_RECT_GATE`) compose cleanly with a letterbox scissor?** Both filter "big rects" — we want the letterbox to *not* gate freeze frames out.
3. **What's the behavior when the user toggles widescreen mid-match?** CVar is read every frame, so it should "just work", but viewport-bound caches in camera structs may need invalidation. Decide: either invalidate camera caches on toggle, or document that toggle takes effect at next stage load.
4. **Internal resolution scaling (`CVAR_INTERNAL_RESOLUTION`, `PortMenu.cpp:225`) interaction:** widescreen is independent of internal res; verify the math doesn't double-scale.
5. **Splitscreen / 4-player stock icons:** these have implicit anchoring (each player's stock cluster is in a corner). Audit during Phase 2.
6. **Widescreen + netplay:** `docs/netplay_architecture.md` — does desync care about viewport bounds? Almost certainly not (rendering is post-sim), but confirm.

---

## Acceptance criteria for the first PR

- CVar off ⇒ pixel-identical output to current main on at least 3 reference scenes (title, CSS, mid-match).
- CVar on ⇒ camera 3D scenes render at 16:9 with no severe edge artifacts on the four most-played stages (Dream Land, Saffron, Hyrule, Battlefield Past — or the equivalents we have).
- HUD on widescreen: damage%, stock icons, timer, ready/go all correctly anchored on a VS match.
- Authored full-screen 2D effects do not stretch (letterboxed).
- Menu toggle works; toggling mid-frame does not crash.
- Build clean; no new warnings.
