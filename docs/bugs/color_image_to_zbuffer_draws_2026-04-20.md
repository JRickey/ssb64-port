# Color-Image-to-Z-Buffer Draws Render as Opaque Rectangles (2026-04-20) — FIXED (TextureRectangle only)

**Symptom:** The off-screen fighter magnifier bubble in
`ifCommonPlayerMagnifyUpdateRender` rendered as a solid **opaque black
square** with the mini-fighter icon visible inside. The intended visual is a
translucent circular outline (bubble) with the mini-fighter drawn inside it;
on the port the outline's transparent edges came out black, giving the
"square with circle inside" artifact.

## Root cause

SSB64 uses an N64 hardware trick to build an alpha-masked stencil in the
Z buffer before drawing the visible frame:

```c
// ifcommon.c — ifCommonPlayerMagnifyUpdateRender
gDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, w, gSYVideoZBuffer);  // redirect colour output to Z
gDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2);
gDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA);
gDPSetAlphaCompare(G_AC_NONE);
// LoadBlock the IA16 frame texture, SetTile as IA8 (mirror-wrap)
gSPTextureRectangle(...);                                          // write depth values
gDPSetColorImage(G_IM_FMT_RGBA, depth, w, 0x0F000000);             // back to primary FB
gDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
gDPSetAlphaCompare(G_AC_THRESHOLD);
gDPSetBlendColor(0, 0, 0, 0x8);
gSPTextureRectangle(...);                                          // draw visible frame
```

On real N64 the first `TextureRectangle` writes depth values derived from
the IA texture's alpha into Z-buffer memory. The second draw then uses
`G_RM_AA_XLU_SURF` + `G_AC_THRESHOLD` to sample those Z values and blend
the visible frame with soft edges.

Fast3D does **not** emulate color-image address redirection — every draw
unconditionally targets the primary framebuffer regardless of what
`GfxDpSetColorImage` was last called with. `Interpreter::
color_image_address` is stored but read in exactly one place
(`GfxDpFillRectangle`) and ignored by every other draw. So the first
"write a mask to the Z buffer" `TextureRectangle` actually landed **on the
primary FB as an opaque black rectangle** (the IA texture's low-intensity
pixels drawn opaque with `G_CC_DECALRGBA` + `G_RM_OPA_SURF`), and the
subsequent XLU draw blended the visible bubble on top of it. Hence the
"black square with circle inside."

`GfxDpFillRectangle` already had a matching skip for this exact idiom
("Don't clear Z buffer here since we already did it with glClear"); the
rest of the draw paths didn't.

## Fix

`libultraship/src/fast/interpreter.cpp` — add the same guard to
`Interpreter::GfxDpTextureRectangle`:

```cpp
if (mRdp->color_image_address == mRdp->z_buf_address) {
    return;
}
```

This treats any TextureRectangle whose current color target matches the
Z-buffer address as a no-op, which is semantically what real N64 would
have produced (a depth-buffer side-effect that Fast3D doesn't read
anyway) with the port's visible-output consequence removed.

## Caveats — where the same idiom is NOT yet guarded

Two other SSB64 sites use the same redirection pattern. They're not known
to be visibly broken today, but if a future regression appears around
them, the fix is mechanical — add the same guard to the relevant Fast3D
draw entry point.

1. **`src/mv/mvopening/mvopeningroom.c` — `mvOpeningRoomTransitionOverlayProcDisplay`**
   (line 1074) and **`mvOpeningRoomTransitionOutlineProcDisplay`** (line
   1092). These redirect to `gSYVideoZBuffer` and then:
   - Call `gcDrawDObjDLHead0` → triangle draws through `GfxSpTri1`
     (**not** guarded; a future symptom would be "solid rectangle behind
     the transition fighter silhouette").
   - Do a `gDPFillRectangle(0, 0, 320, 240)` (line 1100) — already
     guarded by the existing `GfxDpFillRectangle` skip.

2. **`src/sys/objdisplay.c:2994-3002`** — conditional `gDPFillRectangle`
   used as a Z-clear. Already guarded by the existing skip.

If the opening-scene transition animation (Fight Now / Ready To Fight /
character intro sequences) ever shows a rectangular opaque silhouette
behind the outlined fighter, the matching guard needs to land in
`GfxSpTri1` (or `GfxDrawRectangle`, which is shared by Tri /
TextureRectangle / FillRectangle — a guard at that helper would cover
all three in one place, at the cost of coupling the three paths).

Deliberately keeping the fix narrow (only `TextureRectangle`, only the
one obvious symptom) because:
- The guard semantics for tri draws aren't 100% identical to
  TextureRectangle's — tri draws could in principle be used with color
  redirection for meaningful rendering tricks that Fast3D might be able
  to partially emulate. A blanket "skip all draws to Z buffer" risks
  masking a different bug.
- The opening-room transition code isn't in any known regression list as
  of 2026-04-20.

## Files

- `libultraship/src/fast/interpreter.cpp` — `GfxDpTextureRectangle`
  guard.
- `docs/bugs/color_image_to_zbuffer_draws_2026-04-20.md` — this
  writeup.

## Class-of-bug lesson

Fast3D preserves several `SetXImage` parameters in its state but only
consults `color_image_address` in one place. Any N64 rendering idiom
that works by **swapping** the color target between the primary FB and
another buffer (Z buffer, auxiliary framebuffer, offscreen texture) is
a candidate for this class — the intermediate draws land on the primary
FB instead of the intended target, producing extra visible geometry.
Mechanical check: grep for `gDPSetColorImage(..., gSYVideoZBuffer)` or
`gDPSetColorImage(..., 0x0F000000)` in game code; each such pair
bounds a region where Fast3D's target redirection is absent and the
enclosed draws need to either be skipped (if purely used for a Z/mask
side-effect) or handled specially.
