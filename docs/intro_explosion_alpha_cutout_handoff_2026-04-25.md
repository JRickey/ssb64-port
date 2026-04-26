# Intro Explosion-Through-Window — Handoff (2026-04-25, updated 2026-04-26)

The mvOpeningRoom desk→arena transition explosion is **still not
visible** correctly on the port. After a long iteration session this
file replaces the original handoff with a corrected understanding of
the bug. Several prior assumptions in the original write-up were
wrong; the corrections are below.

## TL;DR — corrected

The intended visual (verified against emulator screenshot at
~frame 1060) is:

- Wallpaper sprite **visible everywhere** (the dim desk-room image)
- A spiky red-and-white explosion mesh **drawn on top of** the wallpaper
- The wallpaper underneath is *not* punched through; it's just under
  a smaller animated overlay

The bug on the port: the wallpaper sprite draws *after* the
transition Outline+Overlay gobjs and **covers the silhouette
entirely**. Skipping the wallpaper makes the silhouette visible
against a red FILLCOLOR background — hence the original (now
contradicted) framing of "explosion shows through windows in the
wallpaper". There are **no windows in the wallpaper**; it's a
flat dim image of the desk room.

## What we ruled out this session

Several plausible-looking Fast3D fixes were tried and rejected:

1. **Treat `AA_EN | ALPHA_CVG_SEL` as alpha-cutout.** Original
   handoff hypothesised the wallpaper had alpha=0 cutouts.
   Confirmed false: combiner alpha is `0,0,0,TEXEL0`, but the
   wallpaper texture is opaque everywhere. There's no cutout to
   discard.

2. **Force `use_alpha = true` whenever `alpha_threshold` is set**
   (so `lbCommonStartSprite`'s `gDPSetAlphaCompare(G_AC_THRESHOLD)`
   actually fires the shader's alpha-discard branch). Built and
   tested — visual unchanged. The wallpaper texture has no
   alpha-zero pixels for the discard to drop.

3. **Clear depth to 0 for the redirect-fill in 1-cycle mode** (so
   the wallpaper's `Z_CMP` at PrimDepth z=0.56 fails everywhere
   during the transition window). Threading a `depth_value`
   parameter through `ClearFramebuffer` for all three backends.
   Built and tested — wallpaper *was* successfully Z-rejected, but
   the visible result was the transition camera's FILLCOLOR (red)
   over the entire screen with a tiny silhouette mesh in the
   middle. Not the intended visual at all.

4. **Switch `depth_test` derivation from `geometry_mode.G_ZBUFFER`
   to `other_mode_l.Z_CMP`.** This is actually a principled change
   (Z_CMP is the RDP's depth-test gate; G_ZBUFFER is for the RSP
   vertex pipe) and it does fix the case where rectangles bypass
   Z_CMP. But combined with the Z=0 clear it gave the same
   wrong-direction result as #3. The change might still be worth
   pursuing on its own terms — it's a port-wide correctness fix
   independent of this transition — but only after verifying it
   doesn't break other scenes.

5. **Suppress framebuffer color writes for redirect-active tris**
   (force `use_alpha=true` + `invisible=true` so alpha=0 leaves dst
   unchanged, while depth still updates). Intended to mimic the
   N64 redirect's "color goes to Z, no FB side-effect". Tested —
   prevented the white blob that would otherwise appear at the
   Overlay mesh, but did not produce the intended visual either,
   because the Overlay redirect-tris on N64 actually write
   *combiner-output-as-16-bit-Z* into the Z buffer, and our
   approximation of "write vertex Z" doesn't reproduce that
   gating.

All five were reverted. **The libultraship submodule is back to
its session-start state** as of this update.

## What the actual mechanism appears to be

Reading the Outline / Overlay display callbacks against the
emulator visual:

- **Outline** (`mvopeningroom.c:1105`):
    1. Redirects color image to Z buffer.
    2. Issues a full-screen `gDPFillRectangle` in 1-cycle mode with
       PRIM = (0,0,0,0xFF) and `G_RM_AA_XLU_SURF`. The XLU blender
       resolves to writing `0x0001` (≈ 0) into every Z buffer pixel.
       *On real hardware Z=0 means closest depth.*
    3. Restores color image to FB.
    4. Sets `G_RM_AA_OPA_SURF` (no Z_CMP / no Z_UPD) + combine =
       SHADE, draws the visible silhouette mesh. *On real hardware
       this draws unconditionally, ignoring depth entirely.*

- **Overlay** (`mvopeningroom.c:1074`):
    1. Redirects color image to Z buffer.
    2. Sets PRIM = white, combine = `0,0,0,PRIM` for both color
       and alpha, render mode `G_RM_PASS | G_RM_OPA_SURF2`.
    3. Draws the explosion-shape mesh. *On real hardware, with
       redirect active, the combiner output (white = `0xFFFE`) gets
       written into each Z buffer pixel covered by the mesh —
       making Z near 1.0 (far) inside the explosion shape.*
    4. Restores color image, restores render mode.

- **Wallpaper** (drawn afterwards) sets `gDPSetDepthSource(G_ZS_PRIM)`,
  PrimDepth z=0.56 (`36863/65535`), `Z_CMP` enabled. Real hardware
  Z-test:
    - Inside Overlay shape → buffer Z = 1.0 → `0.56 < 1.0` →
      wallpaper passes → wallpaper drawn → desk visible
    - Outside Overlay shape → buffer Z = 0 (from Outline-fill,
      untouched by Overlay) → `0.56 < 0` → wallpaper rejected →
      whatever's behind the wallpaper shows

Combined with the silhouette mesh drawing on top of all of it, the
emulator visual makes sense: most of the screen shows the dim
wallpaper (because the Overlay shape covers most of the screen
EXCEPT the silhouette area), and the silhouette mesh's red-and-white
spike is overlaid in the center.

In other words: the **Overlay mesh shape is roughly the inverse
of the explosion** — a "where wallpaper should remain visible"
mask. The Outline-fill clears Z=0 everywhere (default = "wallpaper
hidden"), then Overlay writes Z=1 over the parts where wallpaper
should remain.

## Why none of the partial port fixes worked

All the variants we tried emulate part of this mechanism without
the full color-image-redirect. The fundamental missing piece is:

> When `mRdp->color_image_address == mRdp->z_buf_address`, draws
> should write their **combiner-output RGBA reinterpreted as a
> 16-bit Z value** to the depth attachment, with no framebuffer
> side-effect.

That's three concrete changes to libultraship:

1. **Suppress framebuffer color writes** for redirect-active draws
   — partially attempted via `invisible=true`, mostly works for
   the FB suppression side.

2. **Synthesize a depth value from the combiner output** for
   redirect-active draws. The combiner output is per-fragment, not
   per-vertex, so this would need a custom shader path that maps
   `result.rgb` (5+5+5 bits) and `result.a` (1 bit) into a 16-bit
   Z value, then writes that to gl_FragDepth / equivalent, instead
   of using the interpolated vertex Z.

3. **Override `depth_mask` to true** for redirect-active draws so
   the synthesised depth actually lands in the depth attachment.

Plus, for the `gDPFillRectangle` case (Outline-fill), the
`fill_color`-derived value (or in 1-cycle mode the combiner output
of the rect) should clear the depth attachment to that synthesised
value — which is *value-dependent*, so plumbing through
`ClearFramebuffer` with a depth-value parameter is required (or a
fullscreen quad draw at the right Z).

## Current state of the port

After the reverts:

- Wallpaper covers the silhouette (baseline bug).
- The pre-existing PrimDepth fix and redirect-fill clear (to 1.0)
  are intact.
- No partial fixes from this session are committed; the only diff
  vs the previous commit is this docs update.

## Suggested next steps

In rough order of effort:

1. **Game-side workaround** (small, scoped, doesn't generalize):
   skip `mvOpeningRoomWallpaperProcDisplay`'s body during the
   transition window (`sMVOpeningRoomTotalTimeTics ∈ [1040, 1080)`).
   Confirmed visually-equivalent in an earlier experiment — the
   silhouette renders against the FILLCOLOR background, which is
   close enough to "explosion front and center" to pass for a
   first-cut fix.

2. **Game-side reorder** (also small): swap the creation order so
   the wallpaper inserts *before* the transition camera at frame
   1040. Verify whether insertion order or camera priority
   determines draw order — if reorder works, silhouette will
   render on top of wallpaper without any libultraship changes.

3. **`depth_test = Z_CMP` change in libultraship** (medium,
   port-wide): this is the principled fix for "rectangles ignore
   Z_CMP because GfxDrawRectangle clears geometry_mode". Ship it
   on its own merits and regression-sweep title / fighter-select
   / gameplay layering. With this in place, several other
   sprite-vs-3D layering bugs may also resolve.

4. **Color-image-redirect emulation in libultraship** (large,
   high-leverage): implement real depth-attachment writes for
   redirect-active draws. This is what the original N64 code
   relies on; once it works, the explosion transition lights up
   correctly with no game-side workarounds. It also helps the
   `ifCommonPlayerMagnifyUpdateRender` magnifier bubble (currently
   handled via TextureRectangle skip) and any other scene using
   the same idiom.

## Pointers

| Subject | Location |
|---|---|
| Outline display callback | `src/mv/mvopening/mvopeningroom.c:1105` |
| Overlay display callback | `src/mv/mvopening/mvopeningroom.c:1074` |
| Transition + wallpaper creation (frame 1040) | `src/mv/mvopening/mvopeningroom.c:1369-1373` |
| Wallpaper sprite render setup | `src/mv/mvopening/mvopeningroom.c:653` (`mvOpeningRoomWallpaperProcDisplay`) |
| Wallpaper sprite resource | `lbRelocGetFileData(Sprite*, sMVOpeningRoomFiles[7], llMVOpeningRoomWallpaperSprite)` |
| Per-frame Z clear (real depth-clear via redirect-fill) | `src/sys/objdisplay.c:2994-3001` |
| `lbCommonStartSprite` (sets `G_AC_THRESHOLD`) | `src/lb/lbcommon.c:3145` |
| Existing redirect-active depth-test override | `libultraship/src/fast/interpreter.cpp:2199-2203` (commit `5fe2efe`) |
| Existing redirect-fill depth clear | `libultraship/src/fast/interpreter.cpp:3585-3603` (commit `0148b85`) |
| TextureRectangle redirect skip | `libultraship/src/fast/interpreter.cpp:3529-3531` (commit `4e5fe49`) |
| AA / texture_edge approximation | `libultraship/src/fast/interpreter.cpp:2256-2270` |
| Fragment shader alpha-threshold | `libultraship/src/fast/shaders/metal/default.shader.metal:280-281` |
| ImGui game-FB composite | `libultraship/src/ship/window/gui/Gui.cpp:716-722` |

## Don't break

- The PrimDepth fix (`docs/bugs/primdepth_unimplemented_2026-04-25.md`)
  correctly puts the wallpaper at z=0.56 and the fighter portrait
  card behind the model. Any revisit of this transition that
  touches depth handling needs to regression-check the fighter
  description scene to make sure the 2D card stays behind the
  model.
- The per-frame Z clear at `src/sys/objdisplay.c:2999-3001` uses
  the redirect-fill mechanism in `G_CYC_FILL` mode with
  `fill_color = GPACK_ZDZ(G_MAXFBZ, 0)` — anything that changes
  redirect-fill semantics must preserve that case as a
  clear-to-1.0 (far) operation, or the entire game's depth
  buffer goes wrong.
