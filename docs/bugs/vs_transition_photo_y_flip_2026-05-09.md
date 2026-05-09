# VS-mode transition photo wipe rendered upside-down + OpenGL FB-passthrough failure

**Date:** 2026-05-09
**Class:** Decomp/port-bridge orientation mismatch + OpenGL FBO sample-direction asymmetry
**Issue:** GitHub #157
**Files:** `decomp/src/lb/lbtransition.c`, `libultraship/src/fast/{interpreter.cpp, backends/gfx_opengl.cpp}`, `libultraship/include/fast/backends/{gfx_rendering_api.h, gfx_opengl.h}`
**Status:** RESOLVED on D3D11, Metal, and OpenGL.

## Symptom

VS-match → results-screen transition (the "photo wipe" — paper plane / falling
board / star / etc.): the gameplay frame visible through the transition mesh
was rendered **vertically inverted** relative to gameplay. Reproduced on
D3D11 in v0.8-beta. 1P stage-clear was unaffected (correct orientation on
the same builds).

## Root cause

The VS photo capture and the 1P photo capture call the same bridge entry
point (`port_capture_register_fb_for_subrect`) but the original N64 memcpy
loops they replace walk the framebuffer in **opposite directions**:

`decomp/src/sc/sc1pmode/sc1pstageclear.c:2268-2300` (1P stage clear):

```c
framebuffer_pixels = ... + SYVIDEO_BORDER_SIZE(320, 10, u16) + SYVIDEO_BORDER_SIZE(1, 10, u16);
/* starts at (row 10, col 10) — TOP of photo region */
for (i = 0; i < 220; i++) {
    ...
    framebuffer_pixels = row_pixels + 160;  /* advance one full row */
}
```

→ heap row 0 = N64 row 10 (TOP of photo).

`decomp/src/lb/lbtransition.c:326-338` (VS transition):

```c
framebuffer_pixels = ... + SYVIDEO_BORDER_SIZE(320, 10, u16)
                         + SYVIDEO_BORDER_SIZE(320, 220, u16)   /* skip 220 rows down */
                         + SYVIDEO_BORDER_SIZE(1, 10, u16);
/* starts at (row 230, col 10) — BOTTOM of photo region */
for (i = 0; i < 220; i++) {
    for (j = 0; j < 150; j++) { *heap_pixels++ = *framebuffer_pixels++; }
    framebuffer_pixels -= 310;  /* go BACK one row */
}
```

→ heap row 0 = N64 row 230 (BOTTOM of photo). The transition mesh's vertex
UVs (baked into the relocData transition files, e.g.
`dLBTransitionAeroplane*`) are authored against this bottom-up heap layout.

Our pre-fix bridge call mapped consumer's local UV (0..1) onto the
**top-down** N64 sub-rect:

```c
port_capture_register_fb_for_subrect(sLBTransitionPhotoHeap, 300u*220u*2,
                                     10.0f/320.0f,  10.0f/240.0f,    /* u0, v0 */
                                     310.0f/320.0f, 230.0f/240.0f);  /* u1, v1 */
```

mGameFb stores rows N64-top-down (row 0 at v=0). With v0 < v1 the consumer
saw the photo right-side-up *relative to the GPU framebuffer* but
upside-down *relative to what the transition mesh's UVs were authored to
expect* — i.e. visibly inverted on screen.

## Fix

Swap v0 ↔ v1 so consumer normV=0 (mesh top) samples the high V end of the
sub-rect (= N64 row 230, the bottom of the photo region — which is where
heap byte 0 lives in the original heap):

```c
port_capture_register_fb_for_subrect(sLBTransitionPhotoHeap, 300u*220u*2,
                                     10.0f/320.0f,  230.0f/240.0f,
                                     310.0f/320.0f,  10.0f/240.0f);
```

Inside libultraship, `Interpreter::ImportTexture` packs this into
`FbUvTransform{ scaleU, scaleV, offsetU, offsetV }` as
`{ u1-u0, v1-v0, u0, v0 }`. With v0 > v1 the `scaleV` goes negative;
`GfxSpTri1` applies `normV * scaleV + offsetV` which Y-flips the sample —
exactly equivalent to the original heap's bottom-up byte order.

The slice math at `interpreter.cpp:1972-1985` works for negative `vRange`
without changes (scales offRows/loadRows linearly across whatever sign
`r.v1 - r.v0` has). 1P's call site keeps v0 < v1 unchanged.

## OpenGL: two stacked backend bugs in addition to the v-swap

The v-swap above fixes D3D11 + Metal but leaves OpenGL broken in two
ways. Pre-v-swap, 1P stage-clear was already black on OGL; post-v-swap
VS rendered black and halted the game mid-transition. Both root causes
are in the OpenGL backend, not the bridge / decomp:

### 1. FB color attachments had width/height = 0 in `textures[]`

`GfxRenderingAPIOGL::CreateFramebuffer` allocates a GL texture for the
color attachment via `glGenTextures` but never registers it in the
`textures` vector that `SetPerDrawUniforms` reads from. For non-FB
textures, `UploadTexture` populates `.width/.height` (and would also
override `.filtering` later via `SetSamplerParameters`); for FB
textures these stay at the zero-initialized default.

Result: when the FB-passthrough hook runs `SelectTextureFb` and a draw
follows, the GLSL shader's `texture_width[] / texture_height[]`
uniforms are 0, so `0.5 / texSize` and `texCoord * texSize` in
`filter3point` (and the `clamp(..., 0.5/texSize, ...)` in the
clamp-S/T paths) divide by zero. NaN UVs come out → driver returns
black for the sample, and at least one Windows GL driver wedges on the
NaN flow during a multi-stripe transition (the apparent VS "halt").

D3D11 has no equivalent bug because `UpdateFramebufferParameters`
writes `tex.width/height` on the FB's texture entry. Metal's MSL
shader queries `uTex.get_width()` directly from the texture.

Fix: register `clrbuf` in `textures` at `CreateFramebuffer` time
(default `.filtering = FILTER_LINEAR` so `filter3point` doesn't fire
before `SetSamplerParameters` runs), and keep
`textures[clrbuf].width/height` in sync inside
`UpdateFramebufferParameters`.

### 2. OpenGL FBO sample direction is opposite of its render direction

The interpreter pre-negates vertex Y at draw time when rendering to an
`invertY=true` FB (`interpreter.cpp:2960`,
`clip_parameters.invertY ? -v_arr[i]->y : v_arr[i]->y`), so on OGL the
storage row that holds "game-top" is at the GL pixel-bottom of the
FBO. But sampling the same FBO as a `GL_TEXTURE_2D` returns rows in
the opposite direction — `V=10/240` reads close to game-bottom (≈ N64
row 230), not N64 row 10. D3D11 and Metal have no Y negation on the
render side and their texture sample direction matches their storage,
so they don't have the asymmetry. With the texSize fix above 1P
rendered, but inverted (and VS post-v-swap rendered the photo from
the wrong end of the mesh).

Fix: the rendering API now exposes
`GfxRenderingAPI::FbNeedsSampleVFlip(int fbId)` — default `false`,
overridden on OpenGL to return the FB's `invertY`. Inside
`Interpreter::ImportTexture`'s FB-passthrough hook, after assembling
`mFbUvTransform` from the registered `(u0, v0, u1, v1)` and the
multi-stripe slice, apply a post-transform V-flip when the backend
reports it's needed:

```cpp
if (mRapi->FbNeedsSampleVFlip(it->second.fbId)) {
    mFbUvTransform[i].scaleV = -mFbUvTransform[i].scaleV;
    mFbUvTransform[i].offsetV = 1.0f - mFbUvTransform[i].offsetV;
}
```

That cancels the OGL sample-direction asymmetry and leaves D3D11/Metal
untouched. `GfxSpTri1`'s existing `normV * scaleV + offsetV` math
handles the resulting negative `scaleV` without changes.

## Verification

- D3D11 (Windows release v0.8-beta): VS results photo wipe shows
  right-side-up gameplay frame. 1P unaffected (no regression).
- Metal: confirmed correct (analytically — same FbUvTransform path,
  `FbNeedsSampleVFlip` defaults to `false`; user observation pending).
- OpenGL: post-fix 1P stage-clear wallpaper renders right-side-up and
  VS results photo wipe renders right-side-up without halting.

## Audit hook

Any future port-bridge consumer that registers an N64 framebuffer mirror
needs to check the **direction** of the original RDRAM-write loop, not just
the rectangle. If the original walk decrements rows (or columns), pass
the corresponding UV bounds **swapped** so the FbUvTransform's `scaleV`
(or `scaleU`) goes negative. The simple test: if `heap[0]` would be the
bottom of the N64 photo region on real hardware, `v0 > v1` in the
registration. The 1P stripe-array case in `sc1pstageclear.c` and the VS
single-block case in `lbtransition.c` are now the two reference patterns.
