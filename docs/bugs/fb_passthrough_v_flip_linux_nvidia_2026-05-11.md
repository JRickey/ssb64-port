# FB-as-texture V-flip rendered passthrough wallpapers upside-down on Linux/NVIDIA OpenGL

**Date:** 2026-05-11
**Class:** Backend-quirk over-correction
**Platform:** Linux + NVIDIA proprietary OpenGL driver (observed on 595.58.03)
**Status:** RESOLVED on Linux/NVIDIA OGL; D3D11/Metal/macOS-OGL/Windows-OGL unaffected (default path matches their previous behavior).
**Files:** `libultraship/src/fast/backends/gfx_opengl.cpp` (FbNeedsSampleVFlip)
**Related:** issue #157 (the original V-flip introduction), `docs/bugs/vs_transition_photo_y_flip_2026-05-09.md`

## Symptom

On Linux + NVIDIA proprietary OpenGL driver:

- 1P stage-clear frozen wallpaper rendered **upside-down**.
- VS results photo wipe rendered **right-side-up**.

Both use the same `port_capture_register_fb_for_subrect` bridge into
libultraship's FB-as-texture passthrough path
(`Interpreter::ImportTexture`'s `mFbUvTransform` setup). The bridge call
shapes differ by historical artifact:

- VS path (`lbtransition.c`) passes `v0 > v1` because its mesh's UVs are
  authored against the original N64 bottom-up RDRAM memcpy.
- 1P path (`sc1pstageclear.c`) passes `v0 < v1` because the wallpaper
  stripe bitmaps stack top-to-bottom in memory.

D3D11 and Windows-OGL render both correctly. macOS Metal and (likely)
macOS OpenGL also render both correctly.

## Root cause

Commit 19cc6c03 (issue #157) added `GfxRenderingAPIOGL::FbNeedsSampleVFlip`
returning `mFrameBuffers[fb_id].invertY` — i.e. `true` for every snapshot
FB on OGL. The rationale, per the original bug doc:

> "the interpreter pre-negates vertex Y so storage row 0 holds game-top,
> but GL_TEXTURE_2D sampling reads rows in the opposite order — `V=10/240`
> reads close to game-bottom (≈ N64 row 230), not N64 row 10."

`Interpreter::ImportTexture` then negates `scaleV` and flips `offsetV`
(`offsetV = 1.0f - offsetV`) when the backend reports it's needed.

**The premise was wrong on Linux NVIDIA OGL.** Empirically on this
driver, sampling an invertY FBO does NOT reverse the V direction relative
to its rendered storage. V=0 already reads "game top". With the V-flip
applied:

- 1P stripe consumer normV=0 (top of rect) gets `V_sample = 1 - v0` ≈
  230/240 → reads game-row 230 (BOTTOM of photo). Top of stripe shows
  bottom of photo → upside-down on screen.
- VS mesh consumer normV=0 (top of mesh) gets `V_sample = 1 - v0` =
  10/240 → reads game-row 10 (TOP of photo). Mesh-top shows photo-top,
  but the mesh's UV authoring expects mesh-top → photo-BOTTOM (bottom-up
  RDRAM layout). The mesh's authoring cancels with the V-flip → photo
  ends up right-side-up by accident.

So the V-flip cancelled the mesh authoring of one consumer and inverted
the other. Removing the V-flip makes both render right-side-up because
GL_TEXTURE_2D sampling of an invertY FBO on Linux NVIDIA already
matches the consumer's expected orientation.

## Fix

`GfxRenderingAPIOGL::FbNeedsSampleVFlip` returns `false` unconditionally.
The interpreter-side flip is unchanged — it just no longer fires. The
`fb_id` bounds check is preserved so the method remains a safe per-FB
hook for any future driver-specific re-introduction (none known yet).

D3D11 and Metal were already at the default `false`. Mac/Windows OpenGL
were untested here; if their behavior turns out to be the opposite of
Linux NVIDIA and the FB-passthrough renders upside-down there, the fix
is to add a runtime detection or platform/vendor gate inside
`FbNeedsSampleVFlip`. Until then, the cross-backend default-false is the
safer position because at least one Linux user (the primary developer on
this branch) saw it as broken.

## Verification

Tested on Linux/Fedora 43 + NVIDIA proprietary 595.58.03:

- 1P stage clear frozen wallpaper: right-side-up after fix.
- VS results photo wipe: right-side-up after fix.

The OG v0.8.1 AppImage (with the V-flip enabled) shows the same upside-
down 1P wallpaper on the same machine — confirming this isn't a build
quirk of the developer's local config but the actual shipped behavior.

## Audit hook

When a backend's `FbNeedsSampleVFlip` decision turns out wrong for a
specific user / driver, the first diagnostic is whether their
FB-passthrough wallpaper (1P stage clear, easy to reach) renders
upside-down or right-side-up. The two consumers have OPPOSITE
sensitivity to the V-flip (1P's bridge passes top-down v's, VS passes
bottom-up v's), so testing both pins the V-flip direction exactly:

| 1P wallpaper | VS photo wipe | Diagnosis           |
|--------------|---------------|---------------------|
| right-side-up | right-side-up | V-flip OFF is correct (current default) |
| upside-down  | right-side-up | V-flip ON needed for this backend       |
| right-side-up | upside-down  | V-flip ON applied but inverted (broken) |
| upside-down  | upside-down  | mFbUvTransform pipeline broken upstream |
