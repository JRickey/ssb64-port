# Attract Freeze VI Idle Present

**Status:** Fix implemented; trace-verified on direct portrait-scene boot and natural attract-demo path. User visual check against RMG/GLideN64 ground truth confirmed the result.

## Symptoms

- Issue #9: attract-demo freeze moments advanced to the next scene/camera cut too quickly, cutting off the intended audio phrase.
- The easiest visible benchmark is the opening portraits sequence: in ground-truth emulation, the first Kirby banner has fully left the screen before the held frame; in the port, the held frame could still show the red banner tail.
- Earlier fixes that only added a sleep/pacing hold could make the scene pause, but the held image was one frame stale or one frame early.

## Root Cause

The game can intentionally go through a VI period with no new display list submission. On original hardware, VI keeps scanning the current RDRAM framebuffer regardless of whether the CPU/RSP/RDP submitted new graphics that tick.

Fast3D only presented through `DrawAndRunGraphicsCommands()`. On a zero-submit VI, the port either slept or relied on the host swapchain front buffer, which is not necessarily the most recently completed Fast3D framebuffer. The scheduler/game state was at the right tic, but the visible host image was stale.

The portrait trace made this clear:

- tic 149 submits the last display list for the outgoing banner row.
- tic 150 updates the cover to the fully offscreen/black state and requests the next scene.
- `syTaskmanRunTask()` sees `LoadScene` before the draw callback, so no display list is submitted for tic 150. This matches decomp behavior and should not be patched in scene code.
- The final submitted display list already contains the black/zero-width banner draw. The visible red tail was a presentation-layer issue, not a scene-timer issue.

## Fix

Add a VI-style idle present path:

- `Fast3dWindow::PresentCurrentFramebuffer()` draws the cached game framebuffer texture through the normal GUI/window path.
- `Interpreter::PresentCurrentFramebuffer()` starts a backend frame targeting the swapchain framebuffer without re-running any display list or touching game memory.
- `PortPushFrame()` calls this helper on zero-submit VI frames before falling back to the old 16.67 ms sleep pacing.

This models the hardware property we need: no new gfx task is required for the display to continue presenting the current VI framebuffer.

## Verification

Diagnostic direct-portrait trace result from the investigation build:

```text
portraits display port_frame=152 tic=149 row=1 cover_x=-646.0
portraits update port_frame=153 tic=150 row=1 cover_x=-656.0
gfx_per_vi frame=154 submits=0
vi_idle_present frame=154 ok=1
scManagerRunLoop — entering scene 30
```

Natural-attract trace result from the investigation build:

```text
portraits display port_frame=1487 tic=149 row=1 cover_x=-646.0
portraits update port_frame=1488 tic=150 row=1 cover_x=-656.0
gfx_per_vi frame=1489 submits=0
vi_idle_present frame=1489 ok=1
scManagerRunLoop — entering scene 30
```

## Notes

- Do not fix this by adding a final portrait draw or by adjusting portrait-scene timers. The game-side short-circuit on scene load is the behavior to preserve.
- The shipped fix does not keep the temporary portrait probes or idle-present trace hooks.
