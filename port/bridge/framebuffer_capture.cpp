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
 * box-downsampling to N64 native resolution (320x240), with a Y-flip
 * for OpenGL since glReadPixels uses bottom-left origin.
 *
 * Cost: a single GPU->CPU readback per call. Only invoked at scene-
 * boundary moments (stage clear init, transition setup), not per-frame.
 */

#include "framebuffer_capture.h"

#include <fast/Fast3dWindow.h>
#include <fast/Interpreter.h>
#include <fast/backends/gfx_rendering_api.h>
#include <ship/Context.h>

#include <cstring>
#include <vector>

static uint16_t sCaptureBuf[PORT_FB_CAPTURE_W * PORT_FB_CAPTURE_H];

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

    /* mGameFb is created via mRapi->CreateFramebuffer() directly at init
     * time, not via Interpreter::CreateFrameBuffer, so it's not entered
     * into the mFrameBuffers map (that map is for game-driven aux FBs).
     * Its dimensions track mCurDimensions (the upscaled native res).
     * If the renderer is in mRendersToFb=false mode (Windows default
     * when the imgui game viewport matches the resolution), draws went
     * to FB 0 instead, so read from there. On macOS the __APPLE__ guard
     * in ViewportMatchesRendererResolution forces mRendersToFb=true. */
    int fbId = interp->mRendersToFb ? interp->mGameFb : 0;
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

    /* OpenGL's glReadPixels has its origin at bottom-left while N64
     * framebuffers store top-down; D3D11/Metal already match N64. */
    bool flipY = false;
    const char *apiName = interp->mRapi->GetName();
    if (apiName != nullptr && std::strcmp(apiName, "OpenGL") == 0) {
        flipY = true;
    }

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
        if (flipY) {
            sy = srcH - 1 - sy;
        }
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
