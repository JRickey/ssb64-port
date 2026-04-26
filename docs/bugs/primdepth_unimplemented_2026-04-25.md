# gDPSetPrimDepth Unimplemented in Fast3D — 2D-Sprite Layering Broken

**Resolved 2026-04-25.** Fix lives in libultraship's interpreter. Latent
since the original Emill Fast3D engine; upstream `Kenix3/libultraship`
still carries the same `// TODO Implement this command...` stub. SoH /
2Ship / Starship don't lean on `G_ZS_PRIM`, so the gap has been
dormant. SSB64 hits it constantly because every 2D-sprite-as-background
goes through this path.

## Symptom

Any sprite that sets `gDPSetDepthSource(G_ZS_PRIM)` and a non-zero Z via
`gDPSetPrimDepth(z, dz)` ended up at clip-space z=0 (front of the
frustum) instead of the intended depth. Game-side breakage was
port-wide:

- Fighter description / portrait scenes: the 2D logo sprite drew **in
  front of** the 3D fighter model when it was meant to sit behind.
- Item / HUD overlays that lean on PrimDepth for layering
  ordered the wrong way.
- mvOpeningRoom transition wallpaper sprite — see related notes below.

User-visible across many scenes; reproducible by entering the
"Description" / "How to Play" sequence and watching the character
portrait card.

## Root cause

`libultraship/src/fast/interpreter.cpp:gfx_set_prim_depth_handler_rdp`
was a stub:

```cpp
bool gfx_set_prim_depth_handler_rdp(F3DGfx** cmd) {
    // TODO Implement this command...
    return false;
}
```

Nothing stored the value, and `Interpreter::GfxSpTri1` did not consult
`other_mode_l & G_ZS_PRIM` either — every triangle's Z came from
`v_arr[i]->z` regardless of the depth source the game requested. For
the libultra sprite path (`src/libultra/sp/sprite.c:207`,
`gDPSetPrimDepth(gl++, s->zdepth, 0)` under `SP_Z`) this meant the
sprite's `zdepth` was thrown away and the rectangle's per-vertex Z
(typically `-1.0` from `Interpreter::GfxDrawRectangle`) was used —
i.e. NEAR plane = always in front.

Worth noting: the same Emill-era TODO is still in upstream
`Kenix3/libultraship` `src/fast/interpreter.cpp` at the same callsite,
so any libultraship-lineage port that needs PrimDepth has to fix this
locally.

## Fix

Three small hunks in `libultraship` (branch `ssb64`):

1. **`include/fast/interpreter.h`** — add `prim_depth_z` and
   `prim_depth_dz` (both `uint16_t`) to the `RDP` struct, alongside
   `prim_lod_fraction`.

2. **`src/fast/interpreter.cpp::gfx_set_prim_depth_handler_rdp`** —
   actually parse the command:
   ```cpp
   gfx->mRdp->prim_depth_z = (uint16_t)((cmd->words.w1 >> 16) & 0xFFFF);
   gfx->mRdp->prim_depth_dz = (uint16_t)(cmd->words.w1 & 0xFFFF);
   ```

3. **`src/fast/interpreter.cpp::GfxSpTri1`** — when `other_mode_l &
   G_ZS_PRIM` is set, override per-vertex Z with `prim_depth_z` mapped
   linearly into clip space:
   ```cpp
   bool use_prim_depth = (mRdp->other_mode_l & G_ZS_PRIM) == G_ZS_PRIM;
   float prim_depth_ndc = (float)mRdp->prim_depth_z / 65535.0f;
   if (!clip_parameters.z_is_from_0_to_1) {
       prim_depth_ndc = prim_depth_ndc * 2.0f - 1.0f;
   }
   // ... in the per-vertex loop:
   if (use_prim_depth) {
       z = prim_depth_ndc * w;  // multiply by w so perspective divide produces prim_depth_ndc
   } else if (clip_parameters.z_is_from_0_to_1) {
       z = (z + w) / 2.0f;
   }
   ```

`dz` is parsed and stored but not consumed — the depth-slope is
irrelevant once we're producing post-projection Z directly. Linear
mapping is "good enough" for sprite layering; if a future scene needs
the N64's non-linear Z encoding to match exactly, reach for a
piecewise-linear LUT.

## Diagnostic chain that landed on this

Traced from the explosion-iris transition (the wallpaper sprite at
`gDPSetPrimDepth(36863, 1)`). Verified the explosion's geometry tris
reached `GfxSpTri1` with correct prim color, combine, viewport,
scissor, depth state. Magenta-override probe at the GBI level applied
correctly — fb=1 should have had magenta pixels. Composite path via
`ImGui::Image(GetGfxFrameBuffer(), size)` should have shown them. User
visually verified: nothing visible.

That ruled out shader compile, blender, frustum, scissor, FB binding.
The wallpaper-skip experiment (skip `mvOpeningRoomWallpaperProcDisplay`
during the transition window) made the explosion visible — confirming
the wallpaper was Z-occluding the transition tris. Wallpaper combine
+ render mode pointed straight at `gDPSetPrimDepth(36863, 1)`; checking
the handler exposed the stub.

## Verification

Run the build, watch the fighter description / portrait sequence after
the tutorial in attract mode. Before the fix the 2D card sprite drew
in front of the 3D model; after the fix the model is correctly in
front of the card.

The mvOpeningRoom desk→arena transition explosion is **still hidden**
after this fix — there's a second, separate gap (alpha-cutout coverage
emulation for the wallpaper's window-shaped alpha=0 regions). Tracked
in `docs/intro_explosion_alpha_cutout_handoff_2026-04-25.md`.

## Curious observation worth keeping in mind

While iterating on the explosion, two earlier-flagged port-wide
artifacts came up:

- **DK's intro stage briefly had Mushroom Kingdom as its background**
  in some build states. In one mid-investigation build (wallpaper-skip
  + libultraship debug instrumentation), the user reported DK as
  *correct*. Reverting back without the wallpaper-skip, DK is *back to
  Mushroom Kingdom*. Mechanically the wallpaper-skip is scoped to
  `mvopeningroom.c` only, so this should be unrelated — but it's an
  open question. Independent observation: if the game is paused before
  it ever reaches DK's intro frame and then resumed once that frame is
  current, DK's stage loads correctly. So whatever the DK bug is, it's
  not steady-state — it's a startup-/timing-/scene-transition-state
  thing.
- The PrimDepth fix did **not** introduce a regression here — DK was
  already broken before, and the fix doesn't touch DK's code path
  (`mvopeningjungle.c` does not call `gDPSetPrimDepth`).

## Files

- `libultraship/include/fast/interpreter.h` — `RDP::prim_depth_z`,
  `RDP::prim_depth_dz`.
- `libultraship/src/fast/interpreter.cpp` —
  `gfx_set_prim_depth_handler_rdp`, `Interpreter::GfxSpTri1` Z-source
  override.
- `docs/bugs/primdepth_unimplemented_2026-04-25.md` — this writeup.
- `docs/intro_explosion_alpha_cutout_handoff_2026-04-25.md` — followup.

## Class-of-bug lesson

Fast3D inherits a couple of "TODO Implement this command" stubs from
its Emill ancestor that the dominant ports never exercised. When a
game-side render mode "should obviously work" but produces wildly
wrong layering / depth / blending output, grep
`libultraship/src/fast/interpreter.cpp` for `TODO Implement` — there's
non-zero chance the bit you need is sitting unimplemented behind a
return-false handler.
