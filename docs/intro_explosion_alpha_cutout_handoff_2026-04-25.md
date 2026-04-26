# Intro Explosion-Through-Window — Handoff (2026-04-25)

The mvOpeningRoom desk→arena transition explosion is **still not
visible** after the PrimDepth fix
(`docs/bugs/primdepth_unimplemented_2026-04-25.md`). This is the
remaining gap. I have a strong root-cause hypothesis but no fix yet.

## TL;DR

The N64 game uses a stencil-through-windows trick: render an explosion
into the Z buffer (via the color-image-redirect idiom), then draw the
wallpaper sprite with `AA_EN | ALPHA_CVG_SEL` and alpha-zero pixels in
the window areas. On real hardware, alpha=0 → coverage=0 → no Z/color
write, so the explosion's Z is preserved at window pixels and the
explosion shows through.

Fast3D approximates AA via per-pixel src-alpha blending. It does
**not** treat alpha-zero as "no write" the way coverage does; the
wallpaper writes an opaque color (or near-opaque) at every pixel
including the window cutouts, so the explosion is fully occluded.

The explosion redirect-tris machinery (Outline silhouette + Overlay
burst) is otherwise wired up correctly: tris reach `GfxSpTri1` (~1600
hits per transition), the depth-test override for redirect-active
draws lands them past Z rejection, the redirect-fill path clears
depth before the silhouette tris draw. Frame-by-frame instrumentation
confirmed all callbacks fire, animation progresses 0.052→1.000 across
40 ticks, dl pointer is stable, no shader-compile failures.

The PrimDepth fix that just shipped is necessary but not sufficient
— it gets the wallpaper to its **intended** depth (z≈0.56 instead of
front-plane z=0), but the wallpaper still has full alpha at window
cutouts on the port, so it still occludes.

## What's already known and ruled out

From the diagnostic session in `docs/intro_residuals_2026-04-25.md`
plus follow-up:

- ✅ Both transition GObjs (Outline / Overlay) animate and call their
  display callbacks every tick during frames 1040-1080. Logged scale
  growth from 0.052 → 1.000.
- ✅ Outline silhouette tris (post-restore from redirect, OPA_SURF +
  SHADE) reach `GfxSpTri1`. Captured via ring-buffer:
  `prim=(00,00,00,ff) shade=(ff,00,00,00) combine=0x00dff9fffdff9fff
  other_l=0x00552048 gm=0x405 ndc≈(±0.3,±0.3,0.76)`. `use_alpha`
  decoded false; shader hardcodes alpha=1.0.
- ✅ Overlay burst tris (redirect-active, PASS + PRIMITIVE) reach
  `GfxSpTri1`. ~1600 hits per transition. With env-gated magenta
  prim-color override they should produce magenta. **They don't appear
  on screen.**
- ✅ No shader compile failures (always-on `prg==NULL` log was clean).
- ✅ All Metal `DrawTriangles` calls go to `mFramebuffers[fb=1]` (the
  game framebuffer), which is composited to fb=0 (screen) at end of
  frame via `Gui.cpp:721 ImGui::Image(GetGfxFrameBuffer(), size)`.
- ✅ `ClearFramebuffer` (called by the redirect-fill path on
  `0148b85`) properly invalidates the per-fb encoder caches at
  `gfx_metal.cpp:940-950` — the encoder restart is not the bug.
- ✅ Wallpaper-skip experiment confirmed: comment out
  `mvOpeningRoomWallpaperProcDisplay`'s body during the transition
  window and the explosion *does* appear on screen. So the explosion
  geometry IS being drawn into fb=1; the wallpaper just covers it.

## Working hypothesis: alpha-cutout coverage emulation

The wallpaper sprite's render mode (`src/mv/mvopening/mvopeningroom.c:653`):

```c
gDPSetRenderMode(...,
    AA_EN | Z_CMP | IM_RD | CVG_DST_CLAMP | ZMODE_OPA |
        ALPHA_CVG_SEL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM),
    AA_EN | Z_CMP | IM_RD | CVG_DST_CLAMP | ZMODE_OPA |
        ALPHA_CVG_SEL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM));
```

`ALPHA_CVG_SEL` means **the combiner alpha output replaces the
hardware coverage value** (instead of the rasterizer-computed coverage
from anti-aliasing). With `AA_EN` set, low coverage causes the blender
to skip / partial-write the pixel. For an opaque sprite with
alpha-cutout windows (alpha=0 at window pixels), this means:

- Opaque pixels: coverage=255 → full write. Color and Z written.
- Window pixels: coverage=0 → no write. Z preserved (the explosion's
  Z that was written by the redirect-tris). Color preserved (whatever
  was there before the wallpaper).

Fast3D does not implement this. The Metal/GL pipeline state for
`use_alpha=true` (which `AA_EN | ZMODE_OPA | GBL_c1(CLR_IN, A_IN, CLR_MEM, A_MEM)`
maps to) enables standard alpha blending with `setSourceRGBBlendFactor(SourceAlpha)`
and `setDestinationRGBBlendFactor(OneMinusSourceAlpha)`. Result for
alpha=0 src: `result.rgb = 0*src + 1*dst = dst`. Color preserved (good)
— but **alpha** is also written, and Z is **written unconditionally**
based on the depth-test outcome. The Z pass-through that the original
N64 trick depends on doesn't happen.

That doesn't directly explain why the explosion is invisible (since
color preserves dst at alpha-zero pixels — good). The deeper issue is
that `Interpreter::GfxSpTri1` for the redirect-active draws **never
writes Z at all** (depth_mask=false in the redirect override). So the
explosion-shape-Z trick is broken too: the explosion's Z is never in
the buffer for the wallpaper to compare against. Even if alpha-cutout
coverage worked, there'd be no explosion-Z preserved at window
pixels.

So this is actually a **two-sided** gap:

1. **Redirect-tris don't actually write to the Z buffer.** On real
   hardware they wrote Z values "into" what the color image was
   pointed at (the Z buffer). The fix `5fe2efe` only made them
   *visible* on the FB; it didn't reproduce the Z-write side-effect.
2. **Alpha-cutout coverage isn't emulated**, so the wallpaper draws
   opaque everywhere.

The original mechanism cannot be faithfully reproduced without
either: (a) routing the redirect-tris to the depth attachment instead
of the color attachment, or (b) some explicit "stencil through
window" emulation done game-side under `#ifdef PORT`.

## Suggested next steps

There are roughly three directions, in increasing complexity:

### Option A: Game-side hack — order the wallpaper behind the explosion

Re-order the gobjs so the wallpaper draws **before** the explosion
(swap DLLink so wallpaper at link 30, explosion at link 28? Or change
priorities so wallpaper-camera draws before transition-camera).
Combined with the PrimDepth fix that's already shipped, the wallpaper
ends up at z=0.56 and the explosion at z≈0.88 (back). With wallpaper
drawing first, then explosion drawing later with `Z_CMP` enabled and
its z=0.88 > existing z=0.56 in the buffer → wait, that fails Z-test.

Doesn't work cleanly. The explosion really does need to be drawn
**before** the wallpaper for the Z-stencil trick. And on the port,
the wallpaper has no alpha-cutout coverage so it'd cover the
explosion anyway. Not a real solution.

### Option B: Game-side override — disable wallpaper Z-test during transition

Under `#ifdef PORT`, change the wallpaper's render mode for the
transition window so it does not write color over the explosion
shape. Concretely: skip the wallpaper sprite render entirely while
the transition gobjs are alive (the wallpaper-skip experiment that
verified the diagnosis). Visually-equivalent to the N64 result
because the wallpaper's only purpose during the transition is to
provide a window-cutout for the explosion to show through; if we
just skip the wallpaper during those 40 frames, the explosion is
visible against the cleared FB.

Pros: small, scoped, only touches `mvopeningroom.c`. Builds and runs
today.
Cons: hardcoded to one scene. Doesn't generalize. If the same idiom
appears elsewhere (likely, on stage transitions or victory poses?),
each site needs its own bypass.

### Option C: Fast3D-side — emulate `ALPHA_CVG_SEL + AA_EN` as alpha discard

In `libultraship/src/fast/interpreter.cpp::GfxSpTri1`, detect the
`AA_EN | ALPHA_CVG_SEL | (m1b == G_BL_A_IN)` pattern and turn it
into a `SHADER_OPT(ALPHA_THRESHOLD)` so the fragment shader does
`if (alpha < threshold) discard_fragment()` (or similar). This makes
the wallpaper's alpha-zero pixels not write color OR depth — closest
real-hardware equivalent.

Pros: generalizes to every game-side use of this idiom.
Cons: have to be careful not to break other use cases of the same
flags. Anti-aliased edges on regular geometry use AA_EN+ALPHA_CVG_SEL
too; you don't want them to get hard-discarded, you want them
soft-blended. Probably need to look at whether the wallpaper texture
actually has alpha=0 at the cutout pixels (likely, since that's how
real N64 distinguished window from frame), then key the discard on
alpha < some threshold. Existing `texture_edge` heuristic at
`interpreter.cpp:2108-2113` is similar — extend it.

The corollary item-1 (redirect-tris not writing Z) is harder. To do
it properly you'd need to bind the depth attachment as the color
target during redirect-active draws — non-trivial on Metal. Unless
the game's design works with "redirect-tris just visible on FB"
without actually needing the Z stencil — in which case fixing
ALPHA_CVG_SEL alone might be enough.

## Quick test to validate Option C scope

Before committing to a real Fast3D change, verify the wallpaper
sprite texture's alpha channel:

1. Add a one-shot log in libultraship at ImportTexture that dumps
   the texture's alpha histogram for the wallpaper sprite (any IA8 /
   IA4 / RGBA16 with mostly-opaque + sparse-alpha-zero pixels).
2. Confirm alpha=0 corresponds to the window shapes.
3. If yes, Option C is well-scoped. If no (e.g., it's coverage from
   AA edges, not literal alpha=0 cutouts), the fix is harder.

## Pointers

| Subject | Location |
|---|---|
| Wallpaper render mode | `src/mv/mvopening/mvopeningroom.c:653` (`mvOpeningRoomWallpaperProcDisplay`) |
| Wallpaper sprite resource | `lbRelocGetFileData(Sprite*, sMVOpeningRoomFiles[7], llMVOpeningRoomWallpaperSprite)` |
| Outline display callback | `src/mv/mvopening/mvopeningroom.c:1105` |
| Overlay display callback | `src/mv/mvopening/mvopeningroom.c:1074` |
| Transition + wallpaper creation | `src/mv/mvopening/mvopeningroom.c:1370-1373` (frame 1040 trigger) |
| Redirect depth-test override | `libultraship/src/fast/interpreter.cpp:2048-2052` (commit `5fe2efe`) |
| Redirect-fill = depth clear | `libultraship/src/fast/interpreter.cpp:3338` (commit `0148b85`) |
| TextureRectangle redirect skip | `libultraship/src/fast/interpreter.cpp:3222` (commit `4e5fe49`) |
| AA / texture_edge approximation | `libultraship/src/fast/interpreter.cpp:2089-2102, 2252-2253` |
| Fragment shader alpha-threshold | `libultraship/src/fast/shaders/metal/default.shader.metal:280-281` |
| ImGui game-FB composite | `libultraship/src/ship/window/gui/Gui.cpp:716-722` |

## Don't break

The PrimDepth fix shipped today (`docs/bugs/primdepth_unimplemented_2026-04-25.md`)
correctly puts the wallpaper at z=0.56 and the fighter portrait card
behind the model. Any further change to depth/coverage handling
should regression-check the fighter description scene to make sure
the 2D card stays behind the model.
