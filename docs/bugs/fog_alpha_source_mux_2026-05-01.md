# Fighter colour-anim flashes invisible (issue #31)

**RESOLVED 2026-05-01** in libultraship `ssb64@e608ec09`. The respawn-platform white flash, Mario fireball red self-tint, Pikachu hit red/yellow/black flicker, and Pikachu F-Smash white-body flash were all invisible because libultraship's Fast3D blender ignored the cycle-1 alpha-source mux when feeding the shader's fog-blend factor.

## Symptom

A whole class of "flash" effects on fighters never appeared. Each one is driven by a `dGMColScripts*` bytecode (interpreted by `ftMainUpdateColAnim`) that sweeps `colanim->color1.{r,g,b,a}` over time; the renderer translates that into `gDPSetFogColor(r,g,b,a)` plus the render mode `G_RM_FOG_PRIM_A` for fighter polys, expecting the polys to be tinted with `(r,g,b)` by the alpha factor `a`. None of that tint reached the screen.

Repro: training mode with Mario, walk off the stage to ring out → no white flash on the rebirth platform; throw a fireball → no red self-flash; let a CPU get hit by a fireball → no red/yellow/black hit flicker.

## Root cause

`G_RM_FOG_PRIM_A` expands to `GBL_c1(G_BL_CLR_FOG, G_BL_A_FOG, G_BL_CLR_IN, G_BL_1MA)`. On hardware the cycle-1 blender computes `FOG_COLOR * FOG_A + IN_COLOR * (1 - FOG_A)`, where `FOG_A` is the alpha component of the **fog colour register** (set by `gDPSetFogColor`). The `_PRIM_A` in the macro name distinguishes it from `G_RM_FOG_SHADE_A` (`G_BL_A_SHADE` — per-vertex shade alpha, used by OoT for depth haze when the RSP fills `vertex.color.a` from Z under `G_FOG` geomode).

LUS Fast3D's `Interpreter::GfxSpTri*` only inspected the cycle-1 *colour* source (`other_mode_l >> 30`) when deciding what to put into the per-vertex fog VBO slot. It never read the *alpha* source bits (cycle-1 P alpha at bits 26..27). Both `G_RM_FOG_SHADE_A` and `G_RM_FOG_PRIM_A` produced the same code path:

```cpp
mBufVbo[mBufVboLen++] = v_arr[i]->color.a / 255.0f;   // shade alpha — wrong for G_BL_A_FOG
```

The fragment shader (`default.shader.glsl:253`) does `mix(texel.rgb, vFog.rgb, vFog.a)`, so when the game sweeps `gDPSetFogColor(255,255,255,A)` the shader instead used vertex shade alpha (typically 0xFF for opaque fighters → near-pure-white flush, or 0 → no tint), producing nothing that resembled the intended ramp.

OoT/SoH never noticed because they only use `G_RM_FOG_SHADE_A`, where the shade-alpha path is correct.

## Diagnosis path

Built `agent/issue31-colflash` worktree with port-side instrumentation in `ftMainUpdateColAnim`, `ftDisplayMainCalcFogColor`, `ftDisplayMainSetFogColor`, and `ftDisplayMainDecideFogDraw`. Drove training mode interactively. The trace showed:

| Stage | Output | Interpretation |
|---|---|---|
| `COLANIM` | dispatched opcodes match `dGMColScriptsFighterRebirth` byte-for-byte; alpha sweeps from 255 down to ~10, back up to ~180 | bytecode interpreter healthy under PORT |
| `FOGCALC` | `c1=(255,255,255,A) → rgba=(255,255,255,A)` with `shade=0`, `fog_attr_r=255` | calc passes the colour straight through |
| `SETFOG` | `gDPSetFogColor(255,255,255,A)` emitted with the right A | render-side emission correct |
| `FOGDRAW` | `NOFOG=0`, `is_use_color1=1`, mode `G_RM_FOG_PRIM_A` selected | renderer asks for the blend |

Three different scenarios captured (Mario rebirth white, Mario hit black α=128, Pikachu hit yellow `(255,240,120,170)`) all showed identical end-to-end correctness on the game side, ruling out the bytecode/bitfield/PORT-layout class of bugs (which is where #30 and similar lived).

That moved the search into libultraship. `git diff kenix/main..ssb64 -- src/fast/interpreter.cpp` showed zero meaningful fog changes between our fork and Kenix3 upstream — we hadn't broken it; the bug was in shipping LUS. Reading the blender bit decoder (`Interpreter::DrawDisplayList` ≈ line 2431) showed it only reads `>> 30` (colour source); reading the per-vertex fog packing (line 2823 onward) showed the alpha slot is unconditionally fed from shade.

## Fix

`libultraship/src/fast/interpreter.cpp`, in the per-vertex fog packing branch:

```cpp
} else {
    mBufVbo[mBufVboLen++] = mRdp->fog_color.r / 255.0f;
    mBufVbo[mBufVboLen++] = mRdp->fog_color.g / 255.0f;
    mBufVbo[mBufVboLen++] = mRdp->fog_color.b / 255.0f;
    uint8_t fog_alpha_src = (mRdp->other_mode_l >> 26) & 3;
    float fog_a = (fog_alpha_src == G_BL_A_FOG) ? mRdp->fog_color.a / 255.0f
                                                : v_arr[i]->color.a / 255.0f;
    mBufVbo[mBufVboLen++] = fog_a;
}
```

`G_BL_A_FOG = 1`, `G_BL_A_SHADE = 2`, `G_BL_A_IN = 0`, `G_BL_0 = 3` (per `libultra/gbi.h`). Default branch (shade alpha) preserves all existing OoT/SoH behaviour; only `G_BL_A_FOG`-selecting modes (SSB64) get the new path.

## Class

This is the same family as `primdepth_unimplemented_2026-04-25`: an entire bit-field selector in the N64 RDP that LUS Fast3D collapses to one fixed value because the only consumers it had (OoT/MM) used that value. Other potentially-uncovered selectors worth auditing if rendering looks wrong:

- Cycle-2 P/A/M/B selectors — currently always treated as the standard alpha-over-mem chain.
- Blender M (cycle-1 bits 22..23, cycle-2 bits 20..21): only `G_BL_CLR_IN` and `G_BL_CLR_MEM` are exercised; `G_BL_CLR_BL` and `G_BL_CLR_FOG` as the *destination* slot are unverified.
- Blender B (cycle-1 bits 18..19): `G_BL_A_MEM` may not be honoured if a render mode ever needs it.

## Files touched

- `libultraship/src/fast/interpreter.cpp` (3 lines + helpful comment) — pushed as `JRickey/libultraship#ssb64@e608ec09`
- Submodule pointer bump in outer worktree

## Validation

Tints visible in training mode for: Mario rebirth (white), Mario fireball self-tint (red), Pikachu hit reaction (red/yellow), Pikachu F-Smash (white). User-confirmed.
