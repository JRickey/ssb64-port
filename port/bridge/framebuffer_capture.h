#ifndef PORT_BRIDGE_FRAMEBUFFER_CAPTURE_H
#define PORT_BRIDGE_FRAMEBUFFER_CAPTURE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_FB_CAPTURE_W 320
#define PORT_FB_CAPTURE_H 240

/**
 * SSB64 uses a CPU-side framebuffer-readback trick on real hardware to
 * "photograph" the prior gameplay frame and stamp it into a wallpaper
 * sprite (1P stage clear background, scene transition photos). On the
 * port the RDP is replaced by a GPU rasterizer, so the CPU-side frame
 * buffers in `gSYFramebufferSets[]` stay zeroed -- this helper bridges
 * that gap by reading the actual GPU framebuffer back into RDRAM-shaped
 * pixels at native resolution.
 *
 * Reads the live game framebuffer from the GPU and writes it into a
 * caller-private static 320x240 RGBA5551 buffer with N64 orientation
 * (origin top-left, row stride = 320 u16). On success the buffer
 * pointer can be retrieved via port_get_captured_framebuffer().
 *
 * @return 0 on success, negative if the LUS interpreter / framebuffer
 *         is not addressable (e.g. headless boot, gui-only mode).
 *         On non-zero return the capture buffer is left zeroed (the
 *         wallpaper just renders black -- same as the pre-fix behavior,
 *         not a visible regression).
 */
int port_capture_game_framebuffer(void);

/**
 * Returns a pointer to the most recent captured framebuffer
 * (PORT_FB_CAPTURE_W x PORT_FB_CAPTURE_H, RGBA5551, top-left origin).
 * Always non-null; if no successful capture has happened yet, returns
 * a zero-filled buffer.
 */
const uint16_t *port_get_captured_framebuffer(void);

/**
 * Pin the LUS interpreter to off-screen rendering (mRendersToFb=true) so the
 * prior gameplay frame is preserved in mGameFb at scene-transition time.
 * Required by the GPU-readback bridge on backends where FB 0 is the swap-
 * chain back buffer and contents are undefined post-Present (Windows D3D11
 * with DXGI FLIP_DISCARD, Linux GL with similar swap semantics).
 *
 * Cost when enabled: one extra full-screen blit per frame from mGameFb into
 * FB 0; sub-millisecond on any modern GPU. No effect on macOS — the platform
 * already pins this on via the __APPLE__ guard inside the interpreter.
 *
 * Idempotent. Safe to call before the LUS context exists (will become
 * effective once it does).
 */
void port_capture_set_force_render_to_fb(int enable);

#ifdef __cplusplus
}
#endif

#endif
