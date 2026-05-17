/**
 * framebuffer_capture.cpp -- GPU framebuffer-passthrough bridge for SSB64.
 *
 * Replaces the old CPU-readback path (ReadFramebufferToCPU + box
 * downsample + BE16 swap + texture-cache evict) with a much cleaner
 * model based on libultraship's RegisterFbTexture API
 * (Kenix3/libultraship#1046):
 *
 *   - At "photo time" we copy the live mGameFb into an internal
 *     snapshot FB once and tell Fast3D's ImportTexture to bypass the
 *     CPU upload for a given CPU address and instead bind the snapshot
 *     FB directly via SelectTextureFb.
 *
 *   - The snapshot lives at mCurDimensions (whatever the user's
 *     internal-resolution scale currently is), so the captured frame
 *     looks identical to the on-screen rendering -- no quality loss.
 *
 *   - No per-pixel work on the CPU; just one GPU->GPU CopyFramebuffer
 *     and two map-insert calls per capture.
 *
 * See port/bridge/framebuffer_capture.h for the public API.
 */

#include "framebuffer_capture.h"

#include <fast/Fast3dWindow.h>
#include <fast/interpreter.h>
#include <fast/backends/gfx_rendering_api.h>
#include <ship/Context.h>

namespace {

/* Cached desire from the port (CVar / ESC-menu toggle). Mirrored onto the
 * live LUS interpreter as soon as one is reachable; we re-apply on every
 * capture in case the interpreter was recreated (e.g. backend switch). */
bool sForceRenderToFbDesired = false;

/* Internal snapshot FB. -1 = not yet created. Sized to track
 * mCurDimensions; resized lazily when the user changes internal
 * resolution. The same FB is shared across all registered CPU
 * addresses -- each register call re-blits mGameFb into it, so the
 * snapshot reflects the moment of capture. */
int sSnapshotFbId = -1;
uint32_t sSnapshotW = 0;
uint32_t sSnapshotH = 0;

void apply_force_render_to_fb_to_interp(Fast::Interpreter *interp) {
    if (interp != nullptr) {
        interp->SetForceRenderToFb(sForceRenderToFbDesired);
    }
}

Fast::Interpreter *get_interp() {
    auto ctx = Ship::Context::GetInstance();
    if (!ctx) {
        return nullptr;
    }
    auto win = std::dynamic_pointer_cast<Fast::Fast3dWindow>(ctx->GetWindow());
    if (!win) {
        return nullptr;
    }
    auto interp = win->GetInterpreterWeak().lock();
    return interp.get();
}

/* Lazily create / resize the internal snapshot FB to match mCurDimensions.
 * Returns the FB id, or -1 on failure. */
int ensure_snapshot_fb(Fast::Interpreter *interp) {
    uint32_t w = interp->mCurDimensions.width;
    uint32_t h = interp->mCurDimensions.height;
    if (w == 0 || h == 0) {
        return -1;
    }

    if (sSnapshotFbId < 0) {
        /* resize=false: this is a fixed-size capture FB whose contents are
         * the unscaled game framebuffer. Widescreen aspect-ratio correction
         * (which only fires for resize=true off-screen FBs) would stretch
         * the photo and break the transition mesh's UV mapping. */
        sSnapshotFbId = interp->CreateFrameBuffer(w, h, w, h, /*resize=*/0);
        sSnapshotW = w;
        sSnapshotH = h;
    } else if (sSnapshotW != w || sSnapshotH != h) {
        interp->mRapi->UpdateFramebufferParameters(sSnapshotFbId, w, h, 1, /*opengl_invertY=*/true,
                                                   /*render_target=*/true, /*has_depth_buffer=*/true,
                                                   /*can_extract_depth=*/true);
        sSnapshotW = w;
        sSnapshotH = h;
    }
    return sSnapshotFbId;
}

} // namespace

extern "C" void port_capture_set_force_render_to_fb(int enable) {
    sForceRenderToFbDesired = (enable != 0);
    apply_force_render_to_fb_to_interp(get_interp());
}

extern "C" int port_capture_register_fb_for_subrect(const void *base, unsigned int size_bytes,
                                                    float u0, float v0, float u1, float v1) {
    if (base == nullptr || size_bytes == 0) {
        return -1;
    }

    Fast::Interpreter *interp = get_interp();
    if (interp == nullptr || interp->mRapi == nullptr) {
        return -2;
    }

    /* Re-apply the desired force-render flag in case the interpreter was
     * recreated after our last call (e.g. user changed video backend).
     * NOTE: SetForceRenderToFb only takes effect on the NEXT frame's
     * StartFrame, so the port should call port_capture_set_force_render_to_fb(1)
     * once at boot so mGameFb is populated by the time this runs. */
    apply_force_render_to_fb_to_interp(interp);

    /* mGameFb is created via mRapi->CreateFramebuffer() at init time and
     * receives the game's draws every frame when mRendersToFb=true. Without
     * mRendersToFb, draws go to FB 0 (swap chain back buffer) whose contents
     * are undefined post-Present under DXGI FLIP_DISCARD. We can't sample
     * FB 0 reliably, so bail; the caller's draw will fall back to whatever
     * bytes live at cpu_addr (typically zeros, rendering as black -- same as
     * the pre-fix "all-black background" behaviour). */
    if (!interp->mRendersToFb) {
        return -3;
    }

    /* Always source from mGameFb. Two reasons:
     *
     * 1. mGameFb has opengl_invertY=true (Y-negated rendering matches the
     *    snapshot FB's invertY=true). CopyFramebuffer therefore performs a
     *    straight 1:1 row copy. If we sourced from mGameFbMsaaResolved
     *    instead (created with opengl_invertY=false in the interpreter),
     *    CopyFramebuffer's metadata mismatch would insert an unwanted Y-flip
     *    during the blit, leaving the snapshot's storage row 0 holding
     *    game-bottom instead of game-top — i.e. 1P stage-clear wallpaper
     *    renders upside-down when MSAA > 1. (VS results photo wipe happens
     *    to cancel that flip via its bottom-up mesh authoring.)
     *
     * 2. glBlitFramebuffer can read from an MSAA color buffer and resolve
     *    into a single-sample destination in one call when dimensions match
     *    (OpenGL spec: unscaled MSAA→non-MSAA blit performs an implicit
     *    resolve). We don't need mGameFbMsaaResolved as a staging buffer.
     *
     * The legacy "ViewportMatchesRendererResolution + MSAA" bailout is also
     * unnecessary now: that case was about mGameFbMsaaResolved being stale
     * (resolve goes to FB 0 instead). mGameFb itself is always populated
     * mid-frame, so it's a valid blit source regardless of viewport.
     */
    int src_fb = interp->mGameFb;

    int snap_fb = ensure_snapshot_fb(interp);
    if (snap_fb < 0) {
        return -5;
    }

    /* Drain pending GBI so the blit sees the final pixels of the prior
     * frame. Cheap when there's nothing pending. */
    interp->Flush();

    /* GPU->GPU blit, 1:1 size match. CopyFramebuffer is the per-backend
     * blit primitive (glBlitFramebuffer / D3D11 CopyResource / Metal blit
     * encoder). Sub-millisecond on any modern GPU. */
    interp->mRapi->CopyFramebuffer(snap_fb, src_fb,
                                   /*srcX0=*/0, /*srcY0=*/0,
                                   /*srcX1=*/(int)sSnapshotW, /*srcY1=*/(int)sSnapshotH,
                                   /*dstX0=*/0, /*dstY0=*/0,
                                   /*dstX1=*/(int)sSnapshotW, /*dstY1=*/(int)sSnapshotH);

    interp->RegisterFbTexture(base, size_bytes, snap_fb, u0, v0, u1, v1);
    return 0;
}

extern "C" void port_capture_release_range(const void *base) {
    if (base == nullptr) {
        return;
    }
    Fast::Interpreter *interp = get_interp();
    if (interp == nullptr) {
        return;
    }
    interp->UnregisterFbTexture(base);
}

extern "C" void port_capture_release_all(void) {
    Fast::Interpreter *interp = get_interp();
    if (interp == nullptr) {
        return;
    }
    interp->ClearFbTextures();
}
