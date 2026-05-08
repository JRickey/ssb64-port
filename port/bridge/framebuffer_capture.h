#ifndef PORT_BRIDGE_FRAMEBUFFER_CAPTURE_H
#define PORT_BRIDGE_FRAMEBUFFER_CAPTURE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SSB64 framebuffer-capture bridge for libultraship Fast3D.
 *
 * SSB64 uses a CPU-side framebuffer-readback trick on real hardware to
 * "photograph" the prior gameplay frame and stamp it into a wallpaper
 * sprite (1P stage clear background) or a heap buffer bound to RSP
 * segment 1 (lbtransition photo wipe -> VS results screen). On real N64
 * hardware the RDP renders into RDRAM so the CPU can just memcpy the
 * bytes out. Under libultraship the RDP is replaced by a GPU rasterizer
 * and gSYFramebufferSets[] never receives any pixels -- the wallpaper
 * copy reads zeros, hence the "all-black background" reported in
 * GitHub issues #57 (1P) and #81 (VS).
 *
 * This bridge replaces the readback path with a direct GPU FB
 * passthrough using libultraship's RegisterFbTexture API
 * (Kenix3/libultraship#1046):
 *
 *   1. port_capture_register_fb_for_addr(cpu_addr) at "photo time":
 *      copies the live mGameFb into an internal snapshot FB and tells
 *      Fast3D that any gsDPSetTextureImage(cpu_addr) should sample the
 *      snapshot directly (no CPU readback, no byte-swap, no texture-
 *      cache eviction, full GPU resolution).
 *
 *   2. port_capture_release_addr(cpu_addr) when the consumer is done.
 */

/**
 * Pin the LUS interpreter to off-screen rendering (mRendersToFb=true) so
 * the prior gameplay frame is preserved in mGameFb at scene-transition
 * time. Required on backends where FB 0 is the swap-chain back buffer
 * and contents are undefined post-Present (Windows D3D11 with DXGI
 * FLIP_DISCARD, Linux GL with similar swap semantics).
 *
 * Cost when enabled: one extra full-screen blit per frame from mGameFb
 * into FB 0; sub-millisecond on any modern GPU. No effect on macOS --
 * the platform already pins this on via the __APPLE__ guard inside the
 * interpreter.
 *
 * Idempotent. Safe to call before the LUS context exists (will become
 * effective once it does).
 */
void port_capture_set_force_render_to_fb(int enable);

/**
 * Capture the current GPU game framebuffer into an internal snapshot FB
 * and register the CPU range [base, base+size_bytes) as a mirror of a
 * sub-rect of that snapshot. Any subsequent gsDPSetTextureImage(addr)
 * processed by Fast3D where addr falls inside the range will sample the
 * snapshot directly via SelectTextureFb, with the consumer's local UV
 * (0..1) remapped to [u0,u1] x [v0,v1] of the snapshot.
 *
 * The sub-rect is essential for multi-tile sprites: N64 TMEM is 4 KB so
 * any framebuffer-sized region must be loaded as N tiles, each its own
 * gsDPSetTextureImage with an offset pointer. Each tile's mesh samples
 * UV (0..1) of its own small slice. Without per-registration UV
 * remapping the GPU would sample the WHOLE big snapshot for every tile,
 * giving N copies of the captured frame instead of N slices of it.
 *
 * The snapshot is shared across all registered ranges (the underlying
 * GPU FB is reused). Every call re-blits mGameFb into the snapshot,
 * so consecutive register calls all observe the frame at register-
 * call time.
 *
 * @param base        Start of the CPU range covered by the registration.
 * @param size_bytes  Number of bytes the range covers.
 * @param u0, v0, u1, v1  Source-FB sub-rect in normalized [0,1] coords
 *                        (top-left origin). For the trivial single-tile
 *                        case (consumer wants the whole snapshot mapped
 *                        to its UV (0..1)), pass (0, 0, 1, 1).
 * @return 0 on success, negative on failure. On failure the address is
 *         NOT registered; the consumer's draw will fall back to whatever
 *         bytes live at the range (typically zeros, rendering as black).
 */
int port_capture_register_fb_for_subrect(const void *base, unsigned int size_bytes,
                                         float u0, float v0, float u1, float v1);

/**
 * Unregister a range previously registered via
 * port_capture_register_fb_for_subrect. Safe to call with an
 * unregistered address. The internal snapshot FB is retained for
 * future captures.
 */
void port_capture_release_range(const void *base);

/**
 * Drop ALL registered FB-mirror addresses. Called at scene transitions
 * (lbRelocInitSetup) when the bump-reset heaps invalidate every prior
 * heap-keyed registration -- otherwise a fresh asset that happens to
 * land at the same address as a stale FB-mirror entry would render the
 * old snapshot instead of its own pixels.
 *
 * Same role as portResetStructFixups for the byteswap-fixup tracker.
 */
void port_capture_release_all(void);

#ifdef __cplusplus
}
#endif

#endif
