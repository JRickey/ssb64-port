/**
 * framebuffer_capture.cpp -- GPU framebuffer readback for SSB64 PORT.
 *
 * SSB64's 1P stage clear screen and lbtransition photo wipe both grab a
 * snapshot of the current N64 framebuffer (gSYSchedulerCurrentFramebuffer)
 * and copy its pixels into a wallpaper sprite asset for re-display. On
 * real hardware the RDP renders into RDRAM so the CPU can just memcpy
 * the bytes out. Under libultraship the RDP is replaced by a GPU
 * rasterizer and gSYFramebufferSets[] never receives any pixels -- the
 * wallpaper copy reads zeros, hence the "all-black background" reported
 * in GitHub issue #57.
 *
 * This shim bridges the gap by reading the live GPU game framebuffer
 * back to a CPU buffer (via GfxRenderingAPI::ReadFramebufferToCPU) and
 * box-downsampling to N64 native resolution (320x240). All three
 * backends (OpenGL / D3D11 / Metal) deliver pixels in N64 top-down
 * order, so no row flip is needed; see the orientation comment around
 * the readback below for the OpenGL specifics.
 *
 * Cost: a single GPU->CPU readback per call. Only invoked at scene-
 * boundary moments (stage clear init, transition setup), not per-frame.
 */

#include "framebuffer_capture.h"

#include <fast/Fast3dWindow.h>
#include <fast/interpreter.h>
#include <fast/backends/gfx_rendering_api.h>
#include <ship/Context.h>

#include <vector>

static uint16_t sCaptureBuf[PORT_FB_CAPTURE_W * PORT_FB_CAPTURE_H];

/* Cached desire from the port (CVar / ESC-menu toggle). Mirrored onto the
 * live LUS interpreter as soon as one is reachable; we re-apply on every
 * capture in case the interpreter was recreated (e.g. backend switch). */
static bool sForceRenderToFbDesired = false;

static void apply_force_render_to_fb_to_interp(Fast::Interpreter *interp) {
    if (interp != nullptr) {
        interp->SetForceRenderToFb(sForceRenderToFbDesired);
    }
}

extern "C" void port_capture_set_force_render_to_fb(int enable) {
    sForceRenderToFbDesired = (enable != 0);
    auto ctx = Ship::Context::GetInstance();
    if (!ctx) {
        return;
    }
    auto win = std::dynamic_pointer_cast<Fast::Fast3dWindow>(ctx->GetWindow());
    if (!win) {
        return;
    }
    auto interp = win->GetInterpreterWeak().lock();
    apply_force_render_to_fb_to_interp(interp.get());
}

extern "C" int port_capture_game_framebuffer(void) {
    auto ctx = Ship::Context::GetInstance();
    if (!ctx) {
        return -2;
    }
    auto win = std::dynamic_pointer_cast<Fast::Fast3dWindow>(ctx->GetWindow());
    if (!win) {
        return -3;
    }
    auto interp = win->GetInterpreterWeak().lock();
    if (!interp || interp->mRapi == nullptr) {
        return -4;
    }

    /* Re-apply the desired force-render flag in case the interpreter was
     * recreated after our last call (e.g. user changed video backend).
     * NOTE: this only takes effect on the NEXT frame's StartFrame, so it's
     * here purely as a safety belt — the port should call
     * port_capture_set_force_render_to_fb(1) once at boot so mGameFb is
     * already populated by the time this capture runs. */
    apply_force_render_to_fb_to_interp(interp.get());

    /* mGameFb / mGameFbMsaaResolved are created via mRapi->CreateFramebuffer()
     * directly at init time, not via Interpreter::CreateFrameBuffer, so they
     * aren't entered into the mFrameBuffers map (that map is for game-driven
     * aux FBs). Their dimensions track mCurDimensions (the upscaled native
     * res) when ViewportMatchesRendererResolution() returns false; otherwise
     * mGameFb tracks the window dims with mMsaaLevel.
     *
     * FB 0 is the swap-chain back buffer. By the time this runs (scene
     * transition), Present has already rotated the back buffer; under DXGI
     * FLIP_DISCARD on D3D11 its contents are undefined. Worse, sourcing
     * CopyResource against FB 0 with the renderer's mCurDimensions can
     * dimension-mismatch the swap-chain texture and trigger a device-lost
     * event. So we never read FB 0 — instead bail with -7 if no off-screen
     * game FB is populated, and let the caller leave the wallpaper as
     * solid black (graceful degradation, matches pre-fix behaviour).
     *
     * On macOS the __APPLE__ guard in ViewportMatchesRendererResolution
     * forces mRendersToFb=true, so this always picks mGameFb. On Windows
     * D3D11 / Linux GL, mRendersToFb=true is reached when the user has
     * MSAA enabled or has set a non-1:1 internal resolution multiplier
     * (the imgui game viewport then no longer matches the renderer res).
     * In the default 1x / no-MSAA Windows config, mRendersToFb=false and
     * we bail. */
    if (!interp->mRendersToFb) {
        return -7;
    }

    int fbId;
    if (interp->mMsaaLevel > 1) {
        /* MSAA path: mGameFb is multi-sampled and not directly CPU-readable.
         * When the imgui game viewport doesn't match the renderer res, the
         * interpreter resolves into mGameFbMsaaResolved at end-of-frame. When
         * it does match, the resolve target is FB 0 (swap-chain) instead and
         * mGameFbMsaaResolved is left stale — see Interpreter.cpp:5905. We
         * can't read FB 0 (post-Present, undefined contents), so bail. */
        if (interp->ViewportMatchesRendererResolution()) {
            return -8;
        }
        fbId = interp->mGameFbMsaaResolved;
    } else {
        /* Single-sampled mGameFb at mCurDimensions; safe to read directly. */
        fbId = interp->mGameFb;
    }

    uint32_t srcW = interp->mCurDimensions.width;
    uint32_t srcH = interp->mCurDimensions.height;
    if (srcW == 0 || srcH == 0) {
        return -6;
    }

    /* Drain any pending GBI commands so the readback sees the latest
     * pixels written by the prior frame. Cheap when there's nothing
     * pending. */
    interp->Flush();

    std::vector<uint16_t> src(static_cast<size_t>(srcW) * srcH);
    interp->mRapi->ReadFramebufferToCPU(fbId, srcW, srcH, src.data());

    /* Row order is N64 top-down on every backend after this readback.
     * D3D11 / Metal already store textures top-left so their CopyResource /
     * compute-shader paths return row 0 = top of image. OpenGL's glReadPixels
     * returns row 0 = bottom in raw GL memory order, but mGameFb is created
     * with invertY=true (interpreter.cpp:5893), which negates vertex Y at
     * draw time so the framebuffer's OGL-bottom row IS the N64 top row. The
     * MSAA-resolved target follows the same effective convention through the
     * resolve step. Verified empirically: A/B-tested all three flip rules
     * (always-flip-on-OpenGL / no-flip / per-FB-invertY) on OGL — only the
     * no-flip variant produced an upright stage-clear wallpaper. */

    /* Nearest-neighbor box downsample. Acceptable for a wallpaper that
     * only shows once at scene boundaries; bilinear would smear the
     * already-aliased N64 output.
     *
     * Fast3D's RGBA16 texel reader does `(addr[0] << 8) | addr[1]` --
     * each pixel is BIG-ENDIAN per-byte. ReadFramebufferToCPU returns
     * native-endian uint16, so on an LE host every pixel needs a byte
     * swap before it can be served from the texture cache. (See
     * `gfx_read_fb_handler_custom` in libultraship/src/fast/interpreter.cpp:5024
     * for the matching `BE16SWAP` in the GBI-driven readback path.) */
    for (uint32_t dy = 0; dy < PORT_FB_CAPTURE_H; dy++) {
        uint32_t sy = (dy * srcH) / PORT_FB_CAPTURE_H;
        const uint16_t *srcRow = src.data() + static_cast<size_t>(sy) * srcW;
        uint16_t *dstRow = sCaptureBuf + static_cast<size_t>(dy) * PORT_FB_CAPTURE_W;
        for (uint32_t dx = 0; dx < PORT_FB_CAPTURE_W; dx++) {
            uint32_t sx = (dx * srcW) / PORT_FB_CAPTURE_W;
            uint16_t px = srcRow[sx];
            dstRow[dx] = (uint16_t)((px >> 8) | (px << 8));
        }
    }

    return 0;
}

extern "C" const uint16_t *port_get_captured_framebuffer(void) {
    return sCaptureBuf;
}
