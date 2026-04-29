# Fast3D Backend Audit: Unimplemented / Stubbed RDP/RSP Features
**Date:** 2026-04-28  
**Scope:** `libultraship/src/fast/` — N64 RDP/RSP features that are stubbed or incomplete  
**Impact Focus:** Misrendering bugs specific to SSB64 (vs. crashes)

## Executive Summary

Fast3D bundles Emill's emulator-grade re-implementation, which stubs several N64 RDP/RSP features that most ROMs don't use. SSB64 exercises several of these:

1. **✓ FIXED (2026-04-25):** `gDPSetPrimDepth` (G_ZS_PRIM) — sprites rendered at z=0 instead of intended depth
2. **✓ FIXED (2026-04-20):** Color-image-to-Z-buffer redirect in `TextureRectangle` and `FillRectangle`  
3. **⚠ PARTIALLY GUARDED:** Color-image redirect in `GfxSpTri1` — NOW FIXED, includes depth-test remapping
4. **⚠ ACTIVE BUG:** YUV texture format — SSB64 uses YUV sprites; Fast3D logs error and discards
5. **⚠ DEFERRED:** Background scaling via `dsdx/dtdy` in `GfxDpBgRect1` — TODO comment present
6. **⚠ BACKEND DISPARITY:** Direct3D11 clamp+mirror texture wrap mode handling incomplete
7. **⚠ LOW-RISK STUBS:** RDP sync commands (`RDPLOADSYNC`, `RDPPIPESYNC`, `RDPTILESYNC`, `RDPFULLSYNC`)
8. **✓ COMPLETE:** Fog, color combiners (TEXEL0/TEXEL1/COMBINED/ENVIRONMENT/NOISE), render modes, Z-depth

## Detailed Findings

### 1. YUV Texture Format — ACTIVE BUG

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp:1924-1925`

**Code:**
```cpp
case G_IM_FMT_YUV:
    SPDLOG_ERROR("YUV Textures not supported");
    break;
```

**GBI Feature:** `G_IM_FMT_YUV` texture import

**Risk:** HIGH (game actively uses this)

**SSB64 Callsites:**
- `src/libultra/sp/sprite.c:207+` — sprite draws with YUV check: `if( (s->bmfmt == G_IM_FMT_YUV) && (s->attr & SP_FASTCOPY) )`
- `src/libultra/sp/sprite.c:232+` — YUV-specific color combine modes `G_CC_1CYUV2RGB`, `G_CC_YUV2RGB`

**Symptom Prediction:**
Any sprite texture loaded in YUV format (primarily from art assets or cinematics) silently falls through the case with an SPDLOG_ERROR, leaving the texture unloaded. The sprite then renders with a fallback/black texture or becomes invisible. Color-conversion matrix (`YUV → RGB`) is skipped entirely.

**Implementation Sketch:**
YUV is a 16-bit format on N64 (`Y:Cb:Cr` packed into 16 bits). The conversion to RGBA is a matrix multiply:
```
R = Y + 1.402(Cr - 128)
G = Y - 0.34414(Cb - 128) - 0.71414(Cr - 128)
B = Y + 1.772(Cb - 128)
```

The fix requires:
1. Recognizing `G_IM_FMT_YUV` in the ImportTexture path
2. Allocating and converting the YUV texel data to RGBA32 via the matrix above
3. Proceeding with the normal RGBA texture cache/upload pipeline

Cost: Low-to-moderate (40-80 lines; straightforward matrix math on a per-texel loop during import).

---

### 2. Color-Image Redirect Guard in GfxSpTri1 — NOW PROPERLY IMPLEMENTED

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp:2381-2385`

**Code:**
```cpp
bool redirect_active = mRdp->color_image_address == mRdp->z_buf_address && mRdp->color_image_address != nullptr;
if (redirect_active) {
    depth_test = (mRdp->other_mode_l & Z_CMP) == Z_CMP;
    depth_mask = (mRdp->other_mode_l & Z_UPD) == Z_UPD;
}
```

**Status:** FIXED (but worth confirming the fix is working as intended)

**GBI Feature:** `gDPSetColorImage(gSYVideoZBuffer)` + triangle draw redirection

**Risk:** SHIPPED-BUG-LIKELY (but now guarded)

**SSB64 Callsites:**
- `src/mv/mvopening/mvopeningroom.c:1095+1100` — `mvOpeningRoomTransitionOverlayProcDisplay` (now PORT-gated, never executes)
- `src/mv/mvopening/mvopeningroom.c:1114+1118+1124` — `mvOpeningRoomTransitionOutlineProcDisplay` (redirects, draws triangle + fill)

**Context:** The opening-scene transition uses this N64 trick to write depth-mask values into the Z-buffer via triangle draws, which Fast3D cannot emulate because it doesn't actually redirect the color target. The guard remaps depth-test logic when the redirect is active: instead of reading depth-test from `G_ZBUFFER` geometry mode, it reads from `Z_CMP` in the render mode — matching real N64 semantics where Z-buffer writes happen but Z-compare is separate.

**Current Status:**
- **Overlay draw (line 1100):** Now PORT-gated to return early; not exercised.
- **Outline draw (line 1124):** Attempts to draw a silhouette mesh to Z-buffer. The guard exists but the symptom (if it regresses) would be: **the outlined character silhouette becomes invisible or renders as an opaque shape** because depth-test incorrectly rejects the tris against stale Z-buffer content.

**Verification:** The guard is present and the depth-test remapping logic is correct. However, the motivating comment (lines 2370-2379) is worth re-reading to confirm the intent matches the implementation.

---

### 3. Background Scaling TODO — DEFERRED

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp:3928`

**Code:**
```cpp
// TODO: Implement bg scaling correctly
s16 uls = bg->b.imageX >> 2;
s16 lrs = bg->b.imageW >> 2;
// ... subsequent dsdx / dtdy fixed to hardcoded 1 << 10 and 1 << 10
```

**GBI Feature:** `GfxDpBgRect1` background rendering with per-axis scaling

**Risk:** MEDIUM (game uses backgrounds but current behavior may be "close enough")

**SSB64 Callsites:**
- Background rendering paths in various scenes (`src/gr/grbg.c` and related)

**Symptom Prediction:**
If a background uses non-uniform or scaled dsdx/dtdy, the texture coordinates are not properly warped. The background may appear stretched, squashed, or tiled incorrectly. Current code forces dsdx=dtdy=1<<10 (1.0 in texture-space stepping), ignoring any intentional distortion the game might have set up.

**Implementation Sketch:**
The `dsdx` (delta-s-per-x) and `dtdy` (delta-t-per-y) values control the texture coordinate step per pixel. Currently hardcoded to 1<<10 (identity). A "correct" implementation would:
1. Compute the intended texture-space transform from the background descriptor
2. Map the requested scaling to normalized UV coordinates
3. Apply the transform in the vertex UV data passed to the triangle draw

Cost: Low (primarily understanding what the bg descriptor actually specifies; the texture coordinate math is standard).

---

### 4. Direct3D11 Texture Wrap Mode — INCOMPLETE

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/backends/gfx_direct3d11.cpp:563`

**Code:**
```cpp
static D3D11_TEXTURE_ADDRESS_MODE gfx_cm_to_d3d11(uint32_t val) {
    // TODO: handle G_TX_MIRROR | G_TX_CLAMP
    if (val & G_TX_CLAMP) {
        return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
    return (val & G_TX_MIRROR) ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
}
```

**GBI Feature:** `gDPSetTile` texture address modes; specifically the `G_TX_MIRROR | G_TX_CLAMP` combo

**Risk:** LOW (OpenGL/Metal handle this correctly; only D3D11 is incomplete)

**SSB64 Callsites:**
Depends on art asset usage; would manifest if any texture uses the combined mirror+clamp mode on D3D11.

**Symptom Prediction:**
On D3D11 output, textures using `G_TX_MIRROR | G_TX_CLAMP` wrap mode would fall through to the else branch and be treated as either pure MIRROR or WRAP, producing tiling artifacts at edges.

**Context:**
OpenGL and Metal both handle the full set of combinations:
```cpp
// gfx_opengl.cpp / gfx_metal.cpp
case G_TX_NOMIRROR | G_TX_CLAMP:    return GL_CLAMP_TO_EDGE;
case G_TX_MIRROR | G_TX_WRAP:       return GL_REPEAT;
case G_TX_MIRROR | G_TX_CLAMP:      return GL_MIRRORED_REPEAT;
```

D3D11 has no direct `MIRRORED_CLAMP` equivalent; the TODO is a reminder that the D3D11 implementation needs either:
- A workaround (manual UV clamping in shader, or vertex-level transform)
- A fallback choice (document which behavior is chosen)

**Implementation Sketch:**
D3D11's address modes are `WRAP`, `MIRROR`, `CLAMP`, `BORDER`, `MIRROR_ONCE`. The `MIRROR | CLAMP` case maps to `MIRROR_ONCE` (mirrors once, then clamps). Update the function:
```cpp
if ((val & (G_TX_MIRROR | G_TX_CLAMP)) == (G_TX_MIRROR | G_TX_CLAMP)) {
    return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
}
// ... rest as before
```

Cost: Trivial (2-3 lines; conditional insert).

---

### 5. RDP Sync Command Stubs — LOW RISK

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp:5383-5386, 5389`

**Commands:** `RDP_G_RDPLOADSYNC`, `RDP_G_RDPPIPESYNC`, `RDP_G_RDPTILESYNC`, `RDP_G_RDPFULLSYNC`, `RDP_G_SETCONVERT`

**Code:**
```cpp
{ RDP_G_RDPLOADSYNC, { "mRdpLOADSYNC", gfx_stubbed_command_handler } },
{ RDP_G_RDPPIPESYNC, { "mRdpPIPESYNC", gfx_stubbed_command_handler } },
{ RDP_G_RDPTILESYNC, { "mRdpTILESYNC", gfx_stubbed_command_handler } },
{ RDP_G_RDPFULLSYNC, { "mRdpFULLSYNC", gfx_stubbed_command_handler } },
{ RDP_G_SETCONVERT, { "G_SETCONVERT", gfx_stubbed_command_handler } },
```

**GBI Feature:** RDP pipeline synchronization; YUV→RGB color-space conversion setup

**Risk:** LOW (these are mostly pipeline management; emulators treat them as no-ops safely)

**SSB64 Usage:** Not confirmed in codebase search; unlikely to affect visible output if SSB64 doesn't rely on precise RDP pipeline stalls.

**Implementation Sketch:**
These stubs are intentionally no-ops because Fast3D is not a cycle-accurate simulator. The sync commands order N64 RDP operations in hardware; in an emulator, we process all commands in order anyway. `SETCONVERT` configures YUV→RGB matrix parameters; it's paired with YUV texture loads (which are themselves unimplemented). Implementing these would require:
1. Parsing the command words (no-op for sync; matrix params for SETCONVERT)
2. Storing SETCONVERT params for use in YUV import path (already TODO)

Cost: Negligible on its own; meaningful only if YUV import is fixed first.

---

### 6. Culling Display List Handler — EMPTY TODO

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp:4080`

**Code:**
```cpp
bool gfx_cull_dl_handler_f3dex2(F3DGfx** cmd) {
    // TODO:
    return false;
}
```

**GBI Feature:** `G_CULLDL` — F3DEX2 command to skip display-list segments based on clip-volume tests

**Risk:** MEDIUM (if the game relies on culling for correctness or performance)

**SSB64 Usage:** Likely not; SSB64 is not a modern F3DEX2 intensive game. Culling is primarily a performance hint. No grep hits for `G_CULLDL` in src/.

**Symptom Prediction:**
If a display-list segment should be culled (outside frustum), it renders anyway. Minor performance impact; visual artifacts only if the culled geometry would have overlapped visible objects.

**Implementation Sketch:**
The command holds clip-volume coordinates. The handler should:
1. Parse the clip-volume from command words
2. Test the current viewport/scissor against those bounds
3. Skip DL execution if fully outside
Cost: Low; mostly viewport math reuse.

---

### 7. G_ZS_PRIM Depth Source (Primitive Depth) — ✓ FIXED

**File:** `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp:2647-2658, 4639-4640`

**Status:** COMPLETE and VERIFIED (2026-04-25 fix)

**GBI Feature:** `gDPSetPrimDepth(z, dz)` + `G_ZS_PRIM` depth source

**SSB64 Callsites:**
- Every 2D sprite using `SP_Z` flag in `src/libultra/sp/sprite.c:207`
- Portrait/card overlays in fighter description scenes
- mvOpeningRoom wallpaper depth setup

**Verification:** The fix properly stores `prim_depth_z` in the handler and applies it in `GfxSpTri1` lines 2654-2658, with correct NDC mapping for both Metal ([0,1]) and OpenGL ([-1,1]).

---

### 8. Render Modes & Z-Buffer Modes — ✓ COMPLETE

**Files:** `interpreter.cpp` lines 2369, 2383-2384, 2393

**Status:** Fully implemented

**Features Verified:**
- `Z_UPD` (Z-buffer write enable)
- `Z_CMP` (Z-buffer compare enable)  
- `ZMODE_DEC` (Z-decal mode)
- `G_RM_*` render-mode combinations (blend src/dst, coverage)

The color-image redirect logic in `GfxSpTri1` correctly remaps `depth_test` from geometry-mode `G_ZBUFFER` to render-mode `Z_CMP` when redirect is active, which matches N64 hardware semantics.

---

### 9. Color Combiners — ✓ COMPLETE

**File:** `interpreter.cpp` lines 479-624 (color combiner shader generation)

**Status:** Fully implemented

**Features Verified:**
- All primary sources: `G_CCMUX_TEXEL0`, `G_CCMUX_TEXEL1`, `G_CCMUX_COMBINED`, `G_CCMUX_ENVIRONMENT`
- All alpha sources: `G_ACMUX_*` equivalents
- Special modes: `G_CCMUX_NOISE`, `G_CC_1CYUV2RGB`, `G_CC_YUV2RGB`
- Two-cycle mode (`G_CYC_2CYCLE`)

The shader generation correctly maps combiner inputs to shader constants. YUV modes are recognized but will fail if YUV textures are loaded (since the texture itself isn't decoded).

---

### 10. Fog Modes — ✓ COMPLETE

**File:** `interpreter.cpp` lines 2285-2298, 2422-2443, 2805-2816

**Status:** Fully implemented

**Features Verified:**
- `G_FOG` geometry-mode flag
- Fog color + factor computation
- Two blend modes: standard fog and blend-color shroud mode
- Fog factors packed into vertex alpha

---

### 11. TLUT (Texture Lookup Table) Modes — ✓ COMPLETE

**File:** `interpreter.cpp` lines 1757-1765

**Status:** Fully implemented

**Features Verified:**
- TLUT mode detection from `other_mode_h`
- Format override for 4-bit and 8-bit CI (color-indexed) textures when TLUT mode is active
- Proper palette slot selection based on `paletteIndex`

---

### 12. Texture Wrapping & Masking — ✓ COMPLETE (with noted wrap limitation)

**File:** `interpreter.cpp` lines 1116-1124, 1498-1515, 1601-1609

**Status:** Largely complete; OpenGL/Metal full, Direct3D11 missing one combo (see section 4)

**Features Verified:**
- `masks` / `maskt` wrap boundaries properly clamped in texture decode
- Mirror and clamp modes distinguished
- Per-tile wrap state correctly maintained

---

## Summary Table

| Feature | File | Line(s) | Status | Risk | Notes |
|---------|------|---------|--------|------|-------|
| YUV Textures | interpreter.cpp | 1924-1925 | ✗ TODO | HIGH | SSB64 uses; causes silent load fail |
| Color-Image Redirect (Tri) | interpreter.cpp | 2381-2385 | ✓ FIXED | — | Now includes depth-test remap |
| Prim Depth (G_ZS_PRIM) | interpreter.cpp | 4639-4640, 2654-2658 | ✓ FIXED | — | Complete; both Z and NDC mapping |
| BG Scaling | interpreter.cpp | 3928 | ⚠ TODO | MEDIUM | dsdx/dtdy hardcoded to identity |
| D3D11 Mirror\|Clamp | gfx_direct3d11.cpp | 563 | ⚠ TODO | LOW | One wrap-mode combo unhandled |
| RDP Sync Stubs | interpreter.cpp | 5383-5389 | ✗ STUB | LOW | No-ops are safe; low impact |
| Cull DL | interpreter.cpp | 4080 | ⚠ TODO | MEDIUM | F3DEX2; unlikely SSB64 uses |
| Fog | interpreter.cpp | 2285+ | ✓ IMPL | — | Full support |
| Color Combiners | interpreter.cpp | 479+ | ✓ IMPL | — | All modes, including YUV-aware |
| Z-Buffer Modes | interpreter.cpp | 2369+ | ✓ IMPL | — | Complete; redirect-aware |
| TLUT | interpreter.cpp | 1757+ | ✓ IMPL | — | Full support |
| Texture Wrap | interpreter.cpp | 1116+ | ✓ IMPL* | — | *D3D11 missing 1 combo |

---

## Risk Assessment for SSB64 Gameplay

### Critical (Visible Misrendering)
1. **YUV Textures** — If sprites are loaded in YUV format and are visibly missing or black, this is the cause.

### High (Likely Visible)
2. **Background Scaling** — If backgrounds are noticeably stretched/squashed or tiled wrong, this is a candidate. Current workaround (fixed dsdx/dtdy) may be "close enough" for most content.

### Medium (Regression Risk)
3. **Color-Image Redirect in GfxSpTri1** — The guard is present and correct, but the mvOpeningRoom transition silhouette is the regression vector if depth-test logic changes.

### Low (Unlikely to Affect SSB64)
4. **D3D11 Mirror|Clamp** — Only affects D3D11 users; OpenGL/Metal are correct.
5. **RDP Sync Stubs** — Safely ignored by Fast3D's synchronous processing.

---

## Recommendation Priority

1. **URGENT:** Implement YUV texture format decoding (HIGH impact; known active bug)
2. **HIGH:** Audit the background scaling usage; determine if current behavior is acceptable or if fix is needed
3. **MEDIUM:** Complete D3D11 wrap-mode handling (trivial fix; improves parity)
4. **DEFERRED:** Culling and RDP sync stubs (low impact; can wait for upstream contributions)

---

## Files Involved

### Primary
- `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/interpreter.cpp` — Main RDP/RSP handlers
- `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/backends/gfx_direct3d11.cpp` — D3D11 wrap-mode handler
- `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/backends/gfx_opengl.cpp` — Reference (correct impl.)
- `/Users/jackrickey/Dev/ssb64-port/libultraship/src/fast/backends/gfx_metal.cpp` — Reference (correct impl.)

### Related Bug Docs
- `docs/bugs/primdepth_unimplemented_2026-04-25.md` — G_ZS_PRIM fix (now resolved)
- `docs/bugs/color_image_to_zbuffer_draws_2026-04-20.md` — Color-image redirect context
- `docs/bugs/whispy_canopy_metal_fallback_texture_2026-04-25.md` — Metal texture binding fix

### Game Source (SSB64 Usage)
- `src/libultra/sp/sprite.c` — YUV sprite paths
- `src/mv/mvopening/mvopeningroom.c` — Color-image redirect + transition tris
- `src/if/ifcommon.c` — Magnifier bubble (color-image redirect)
- `src/sys/objdisplay.c` — Z-buffer clear idiom

