# OpenGL stage-clear wallpaper rendered upside-down — double Y-invert

**Date:** 2026-05-03
**Class:** Backend-specific framebuffer-readback orientation
**Files:** `port/bridge/framebuffer_capture.cpp`

## Symptom

On the OpenGL backend the 1P "STAGE CLEAR" frozen-frame wallpaper rendered
upside-down. D3D11 and Metal looked correct.

## Root cause

The original WIP that introduced `port_capture_game_framebuffer`
(`a2697fb`) was verified on Metal only. It speculatively added a
"glReadPixels uses bottom-left origin" Y-flip on OpenGL on the assumption
that the OGL framebuffer memory was stored bottom-up.

That assumption is wrong for `mGameFb`. Look at libultraship:

* `interpreter.cpp:5893` — `UpdateFramebufferParameters(mGameFb, …, /*opengl_invertY=*/true, …)`
* `interpreter.cpp:2761` — when drawing geometry to an `invertY=true` FB,
  the engine pre-negates the vertex Y coordinate
  (`clip_parameters.invertY ? -v_arr[i]->y : v_arr[i]->y`).

The net effect: the OGL framebuffer ends up with its bottom row holding
the N64 *top* of the rendered image. `glReadPixels` returns rows in OGL
memory order (row 0 = bottom), which on this FB is **already** N64
top-down. The wrapper's "compensating" flip then inverted a buffer that
was already in N64 orientation.

D3D11 / Metal don't have an analogous flag — their `ReadFramebufferToCPU`
implementations (`gfx_direct3d11.cpp:1100`, `gfx_metal.cpp:1188`) walk the
texture in native top-left memory order. So they were correct without
any flip and the OGL flip never applied to them.

Corroboration from the GBI-driven readback path: `gfx_read_fb_handler_custom`
(`interpreter.cpp:5036`) calls `ReadFramebufferToCPU` on game-driven aux
FBs (also created with `invertY=true`) and never row-reverses, expecting
the result to be N64 top-down on every backend.

## A/B verification

A short-lived runtime harness (`SSB64_FB_FLIP=always|never|auto` env var)
ran three rules from a single binary:

* `always` — flip on OpenGL (legacy behaviour) → upside-down ✗
* `never` — no flip on any backend → upright ✓
* `auto` — flip iff backend=OGL and source FB has `invertY=false` → upright ✓
  (would have only changed behaviour for the OGL+MSAA-resolved path)

`never` was the simplest rule that matched observation across the OGL
non-MSAA case the user reproduces. Locked in as the unconditional default.

## Fix

`port/bridge/framebuffer_capture.cpp`: drop the OpenGL-only Y-flip and the
associated `<cstring>` include / `apiName` lookup. Comment captures the
non-obvious reason (no per-backend branching is needed).

## Audit hook

Any future port-side bridge that calls `mRapi->ReadFramebufferToCPU` on
a libultraship-managed FB whose `invertY` was set to `true` (i.e.,
`mGameFb` or any FB created via `Interpreter::CreateFrameBuffer`) gets
N64 top-down rows back on every backend without further work. Only
`mGameFbMsaaResolved` is constructed with `invertY=false`; if a future
caller reads it on OpenGL and treats the result as N64 top-down without
testing, suspect this same class of bug.
