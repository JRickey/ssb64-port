# Opening-Sequence Residuals — Handoff (2026-04-25)

Three intro-scene defects investigated this session. One landed cleanly,
two remain visibly broken on PC despite the fixes that ship as
groundwork. Use this doc as a starting point next time.

## TL;DR

| # | Issue | Status |
|---|-------|--------|
| 1 | Samus too close to DK in `mvOpeningJungle`; `RelocPointerTable: invalid token 537984998` | **Fixed** by `1c5cd49` (ftanim SetTranslateInterp parser) |
| 2 | `mvOpeningRoom` desk→fighter-stage transition explosion sprite invisible | **Still broken** — fixes landed (Fast3D depth-test override + colour-image-redirect FillRectangle now does an actual depth clear) but the silhouette/explosion still does not appear in-game |
| 3 | Fighter intros (Yoshi tongue / Samus grapple / DK smash) skip the brief freeze-frame at the climax of each move | **Still broken** — mechanism understood, foundation landed (VI framebuffer rotation in port stubs), but the visible freeze does not reproduce |

Also flagged: persistent OpenGL warning  
`UNSUPPORTED (log once): POSSIBLE ISSUE: unit 1 GLD_TEXTURE_INDEX_2D is unloadable and bound to sampler type (Float) - using zero texture because texture unloadable`  
Apple GL one-shot complaint about a TEXEL1 sampler bound while no texture is uploaded. Benign — Fast3D's combine analysis correctly marks the sampler unused for combines that don't reference TEXEL1, the shader still runs. Logged separately so it stops being conflated with the explosion bug.

## Issue 1 — Samus/DK reloc token (FIXED)

`src/ft/ftanim.c:445` `nGCAnimEvent16SetTranslateInterp` was reading the
post-cmd halfword as if it were a 32-bit `PORT_RESOLVE` reloc token,
then advancing 4 bytes — i.e. two `AObjEvent16` slots — corrupting the
opcode stream from that point onward. The opcode is followed by a single
`AObjEvent16` whose `s16 .s` field is a halfword offset back into the
figatree where the interpolation curve lives. Fix matches the non-PORT
branch: `interp = event16 + (event16->s / 2)`, then `AObjAnimAdvance`
once.

Source-level fingerprint of the broken read: `537984998 = 0x2010ffe6`,
i.e. cmd word `0x0801`/flags `0x0040` followed by signed offset
`0xffe6 = -26 halfwords`, read as one fake `u32`.

Commit: `1c5cd49`. Memory note in
`memory/project_fighter_intro_anim.md` worth refreshing once the rest
of the intro stabilises.

## Issue 2 — Transition explosion sprite

### Mechanism (real N64)

`mvOpeningRoomMakeTransition()` (frame 1040) creates two GObjs:

- **Outline** — `gcAddDObjAnimJoint(... TransitionOutlineAnimJoint)`,
  display callback at DL link 30. The callback (`mvopeningroom.c:1105`)
  redirects the colour image to the Z buffer, fills it black with
  `gDPFillRectangle(0, 0, 320, 240)` (depth-mask clear), restores the
  colour image to the framebuffer, sets `G_RM_AA_OPA_SURF` + SHADE
  combine, and draws the visible silhouette tris.

- **Overlay** — `gcAddDObjAnimJoint(... TransitionOverlayAnimJoint)`,
  display callback also at DL link 30 (`mvopeningroom.c:1074`). This
  one redirects colour-image to ZB, sets PRIMITIVE white with
  `G_RM_PASS, G_RM_OPA_SURF2`, draws tris that write Z-mask values into
  the redirected target, then restores. On real hardware these tris are
  invisible to the framebuffer — they only manipulate Z, and a
  subsequent draw uses the resulting depth mask to "iris" the new
  scene through the explosion shape.

Both gobjs are scaled from ~0.052 → ~1.0 over 40 frames — the
"explosion" expanding outward.

### What we fixed

- **`libultraship/src/fast/interpreter.cpp:GfxSpTri1`** (commit
  `5fe2efe` on `JRickey/libultraship` `ssb64`): when the colour image
  is currently redirected to the Z buffer, gate `depth_test` on
  `Z_CMP` from `other_mode_l` rather than on `G_ZBUFFER` from
  `geometry_mode`. Verified at runtime: the Overlay's tris do hit
  `GfxSpTri1` (`hit#1024 v1=(829.6,-219.6,757.7 w=1000.00) gm=0x405
  other_l=0xf0a4000`) with `clip_rej = 0`, so they make it past
  rejection and culling. Without this, they were Z-rejected against
  stale wallpaper depth.

- **`libultraship/src/fast/interpreter.cpp:GfxDpFillRectangle`**
  (commit `0148b85`): when the colour image is redirected to the Z
  buffer, issue `mRapi->ClearFramebuffer(false, true)` instead of
  silently returning. The previous `// already did it with glClear`
  skip is only correct for the start-of-frame Z clear; the Outline's
  mid-frame redirect-fill is meant to wipe Z before the OPA tris draw,
  and the wallpaper had already filled the depth buffer with its own
  values (`G_RM_AA_ZB_OPA_SURF` includes `Z_UPD`).

### What's still wrong

The user reports the explosion / silhouette is **still not visible**
after both fixes. Tris definitely reach the rasteriser — confirmed by
`fprintf(stderr, "zb-redirect tri hit#%d ...")` instrumentation
(removed before commit). So either:

1. **The output pixels are emitted but immediately overwritten.**
   The Wallpaper GObj is created on the same frame (1040), runs at DL
   link 28 under a separate camera (link 16, priority 90, mask
   `COBJ_MASK_DLLINK(28)`). The Transition camera (link 16, priority
   95, mask `COBJ_MASK_DLLINK(30)`) was created right after. Both
   cameras share `link_id=16` so they're traversed in insertion order
   under the same parent list, and the relative draw order between
   "wallpaper sprite" and "transition tris" depends on the
   `priority_display` field interaction with the gobj-list traversal
   — which I did not fully trace.

2. **The blender produces transparent or dest-coloured output.**
   `G_RM_PASS | G_RM_OPA_SURF2` in 1-cycle mode: cycle-2 fields are
   used by Fast3D for the blender. We saw `other_mode_l = 0xF0A4000`,
   `gm = 0x405`. Fast3D's `invisible` heuristic at
   `interpreter.cpp:2101` checks for `G_BL_0` in m1b and `G_BL_CLR_MEM`
   in m2a — our value is `G_BL_CLR_IN`, so `invisible=false` and a
   visible white pixel should be emitted. But this hasn't been
   confirmed pixel-by-pixel; Fast3D may have a separate path that
   short-circuits to no-op on the redirect.

3. **The DObj's vertex stream is already cleaned by the camera frustum
   despite passing trivial clip-rej.** Our log showed `w=1000`,
   `z=757.7` for the early tris (eye-space depth ~1000, NDC z ~0.76),
   well inside the camera's `near=128 / far=16384` frustum. But later
   tris drift to e.g. `(2096.2, 0.0, 757.7)` — the explosion mesh is
   genuinely large at full scale, and may straddle scissor.

### Suggested next steps

- **GBI-trace one full Outline+Overlay draw** (frame 1064 is good — the
  scale is mid-animation at ~0.7). Tooling lives at
  `port/debug_tools/gbi_trace/` per `docs/debug_gbi_trace.md`. Look
  for any `gDPSetTextureImage`/scissor changes that could hide output,
  and confirm the actual blender bits Fast3D is interpreting.

- **Force a blender-output dump in `GfxSpTri1`** for the redirect
  path: bind a magenta render target so any output from these tris is
  obvious. If magenta also doesn't appear, the tris are post-rasteriser
  invisible (blender or framebuffer-write logic dropping them).

- **Walk the gobj draw order at frame 1040** by adding a one-shot log
  to each scene-camera's display callback (`func_80017EC0`,
  `lbCommonDrawSprite`) so we can see the actual relative timing of
  the wallpaper sprite vs the transition draw.

- **Look at the priorities passed to `gcMakeCameraGObj`** for
  Wallpaper (90) vs Transition (95) more carefully. The 6th argument
  is `priority_display`, and `objhelper.c:525` is the constructor —
  trace what list it sorts into and whether higher means "earlier" or
  "later" in the per-link DL chain.

## Issue 3 — Fighter-intro freeze-frame

### Mechanism (real N64)

The brief pause at the climax of Yoshi-tongue / Samus-grapple /
DK-smash isn't a figatree wait or a `gcSetAnimSpeed(0.0F)` call. It's
the natural by-product of the gfx pipeline running long.

Each gfx task carries an `fnCheck` callback. The default is
`sySchedulerCheckReadyFramebuffer` (`src/sys/scheduler.c:182`); the
opening room scene installs `mvOpeningRoomCheckSetFramebuffer`. The
scheduler refuses to start a paused gfx task when the framebuffer slot
the game wants to draw into is the one VI is currently displaying or
has queued for next swap. A heavy fighter-intro frame can take longer
than one VI retrace; on the next retrace the scheduler sees no free
slot, the task stays paused, the game thread blocks on its retVal
`osRecvMesg`, and the user sees the same framebuffer for two retraces
— the freeze.

### What landed

- **`port/stubs/n64_stubs.c`** — `osViGet*Framebuffer` and
  `osViSwapBuffer` were no-ops returning `NULL`. They now track a
  current/next framebuffer pair, with a new `port_vi_simulate_vblank()`
  propagating queued → current. (`66a7726`)

- **`port/gameloop.cpp:PortPushFrame`** — calls
  `port_vi_simulate_vblank()` once per frame, before posting
  `INTR_VRETRACE`, so the rotation is observable to
  `sySchedulerCheckReadyFramebuffer` and the scene-specific siblings.
  (`66a7726`)

After this groundwork, `sySchedulerCheckReadyFramebuffer` and
`mvOpeningRoomCheckSetFramebuffer` correctly see two of the three
`gSYFramebufferSets[]` slots as "in use by VI" each frame instead of
all three slots looking free. Verified with diagnostic logging (now
removed): `fnCheck` invocations across a 1500-frame run produce **zero
FALSE returns**.

### Why it still doesn't freeze

The fnCheck only returns FALSE when the rotation can't find a free
slot, and the rotation only fills up when a gfx task fails to complete
before the next VBlank. On modern hardware Fast3D finishes every
frame's draw in well under 16ms, so the rotation always has room.
There is no "intentional stall" call from the game side — the freeze
emerges naturally from RSP overload.

### Suggested next steps

Two viable directions:

1. **Workload-cost model** (architecturally correct, multi-day):
   Estimate per-frame RSP cost as the gfx task is built (count tris
   submitted, factor in fillrects and texture loads, perhaps weight
   by render-mode complexity). If the estimate exceeds the N64
   per-frame budget (~16.6ms), withhold the rotation in
   `port_vi_simulate_vblank()` for one tick — that pins the previous
   framebuffer, `fnCheck` returns FALSE, the gfx task stays paused,
   the game thread blocks, the user sees the freeze.

2. **Heuristic per-scene rate cap** (one-day fix):
   In each fighter-intro `mv*StartScene()`, install a one-shot hook
   that artificially holds the previous framebuffer for N frames at
   each known climax tic (the user can hand-tune by character). This
   is much simpler, doesn't require cost modelling, and matches the
   visual goal even if it isn't authentic to the original mechanism.

Either approach plugs cleanly into the existing
`port_vi_simulate_vblank()` — the rotation gate is the single point
where a "stall this VBlank" decision needs to live.

### Useful breadcrumbs while debugging this

- `gSYTaskmanTaskCount = tscene->contexts_num` — fighter-intro scenes
  set `contexts_num=2` (e.g., `mvopeningyoshi.c:555`). Two slots is
  what makes the natural freeze possible: one frame's gfx in flight
  + one queued + zero free → next frame stalls. With the 3 slots
  `gSYFramebufferSets` provides, the freeze is a function of how long
  gfx takes vs how fast VI propagates.

- `syTaskmanSetFuncSwapBuffer` (`src/sys/taskman.c:241`) is how a
  scene installs a custom fnCheck. Only `mvOpeningRoom` uses this;
  every other scene rides the default
  `sySchedulerCheckReadyFramebuffer`.

- `sSYSchedulerCustomFramebuffer` is suspicious — it's set when
  `framebuffer_id != -1` on a task and only cleared by the unused
  `SC_TASK_TYPE_11` task. If a non-default scene path ever sets
  `framebuffer_id`, custom would latch on and `fnCheck` would return
  FALSE forever — worth knowing if the freeze later behaves
  pathologically (full game stall) instead of briefly.

## Files / commits index

| Path | Commit | Branch |
|------|--------|--------|
| `src/ft/ftanim.c` | `1c5cd49` | main |
| `src/mv/mvopening/mvopeningroom.c` | `6f826c4` | main (one-shot transition display logging) |
| `port/stubs/n64_stubs.c`, `port/gameloop.cpp` | `66a7726` | main |
| `libultraship/src/fast/interpreter.cpp` (depth-test override) | `5fe2efe` | `JRickey/libultraship` `ssb64` |
| `libultraship/src/fast/interpreter.cpp` (FillRectangle depth-clear) | `0148b85` | `JRickey/libultraship` `ssb64` |
| Outer submodule pointer bump | `6f826c4` (depth fix), pending for FillRect fix | main |

Push order if landing on remotes: submodule first
(`git -C libultraship push origin ssb64`), then bump the outer
pointer, then push outer.
