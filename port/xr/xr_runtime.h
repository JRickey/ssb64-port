#pragma once

/**
 * xr_runtime — OpenXR integration for the SSB64 PC port.
 *
 * Tier 1 ("cinema mode"): the existing game framebuffer is presented as a
 * floating quad in front of the user, in stereo. Game rendering is unchanged;
 * we just composite the same image to two eyes through OpenXR's quad layer
 * support. No gameplay or comfort impact.
 *
 * Status: SCAFFOLD ONLY. The lifecycle hooks below compile and integrate with
 * the existing port loop, but the actual OpenXR session bring-up is stubbed.
 * To finish:
 *
 *   1. Add OpenXR loader as a dependency (Windows: openxr_loader.lib from
 *      Khronos OpenXR-SDK 1.0.x; Linux: libopenxr_loader.so). Either bundle
 *      it via vcpkg (`xrcore` port) or rely on system install.
 *
 *   2. Replace the `xr_runtime_init_stub` body with real session init:
 *        - xrCreateInstance with XR_KHR_D3D11_enable (or _opengl_enable)
 *        - xrGetSystem(XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY)
 *        - xrGetD3D11GraphicsRequirementsKHR
 *        - xrCreateSession bound to the libultraship D3D11 device
 *        - xrCreateReferenceSpace(XR_REFERENCE_SPACE_TYPE_LOCAL)
 *        - xrCreateSwapchain x2 (one per eye) — but for cinema mode we need
 *          only the quad-layer swapchain (single image)
 *
 *   3. xr_runtime_present(): wrap each game frame between xrWaitFrame /
 *      xrBeginFrame / xrEndFrame. Submit one quad-layer per eye showing the
 *      libultraship final framebuffer (Fast3dWindow::GetGfxFrameBuffer())
 *      copied into the XR swapchain image.
 *
 *   4. Mirror logic: when XR is active, optionally suppress the desktop
 *      window's swap (or run it at lower priority) to avoid double-presenting
 *      and rendering twice.
 *
 *   5. Quad placement: world-locked floating screen ~2 m in front of the user,
 *      ~16:9 aspect, ~3 m wide (configurable via CVars).
 *
 * Tier 2/3 (true stereo / first-person VR) reuse the recording layer
 * (frame_interpolation.{h,cpp}) — per-eye projection matrices go through
 * the same `mtx_replacements` map that frame interpolation already uses.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Lifecycle. All return 0 on success, non-zero on failure. Failures are
 * non-fatal: on any failure xr_runtime_is_active() returns 0 and the rest
 * of the port loop runs as if XR were never enabled. */

int  xr_runtime_init(void);
int  xr_runtime_shutdown(void);

/* True iff a session is live and we should be submitting frames to it.
 * Cheap — used as a gate from the port frame loop. */
int  xr_runtime_is_active(void);

/* Per-frame hooks. Both no-op when xr_runtime_is_active() is 0.
 *
 *   xr_runtime_begin_frame: called from PortPushFrame just before the game
 *     coroutine runs. Opportunity for xrWaitFrame / xrBeginFrame.
 *
 *   xr_runtime_end_frame: called after the desktop swap has happened.
 *     Composites the framebuffer to the XR swapchain images, submits the
 *     quad layer, and calls xrEndFrame. */
void xr_runtime_begin_frame(void);
void xr_runtime_end_frame(void);

/* Diagnostics. Returns a static string; never null. */
const char *xr_runtime_status(void);

#ifdef __cplusplus
}
#endif
