# CRT / Post-Process Shader Plan — 2026-05-11

Branch: `claude/plan-crt-shader-dJzz9`

Goal: add a generic, user-supplied post-process shader stage to the LUS-based
Battleship port. Targets all three Fast3D backends (OpenGL, DirectX 11, Metal)
at v1. Optimize for **maximum compatibility with existing CRT/scanline shader
libraries** rather than bundling a single look. Keep the binary's MIT license
clean by not redistributing GPL shader source.

This is a planning doc, not an implementation. Nothing in `src/` changes yet.

---

## 1. Why this is non-trivial

LUS's `Fast::GfxRenderingAPI` interface
(`libultraship/include/fast/backends/gfx_rendering_api.h`) is entirely scoped
to N64-GBI combiner emulation. Every shader the backend manages is a
*dynamically-generated combiner program* keyed by `(shader_id0, shader_id1)`
and rebuilt per combiner state via the Prism templated default shader
(`libultraship/src/fast/shaders/{opengl,metal,directx}/default.shader.*`).
There is no:

- "Static fullscreen-pass shader" concept.
- Cross-backend shader loader for user-authored source.
- FBO chain abstraction (the interpreter manages exactly two FBOs:
  `mGameFb` and `mGameFbMsaaResolved`).

A user-shader runtime has to add all three.

## 2. Rendering pipeline today (confirmed)

End-of-frame path inside `Fast::Interpreter::Run`
(`libultraship/src/fast/interpreter.cpp:6373-6442`):

1. Game GBI commands render into `mGameFb` (offscreen FBO at internal
   resolution; created in `Interpreter::Init` at line 6193).
2. After the last GBI command, the MSAA path branches:
   - MSAA > 1 and `!ViewportMatchesRendererResolution`: resolve
     `mGameFb -> mGameFbMsaaResolved`; expose resolved texture as
     `mGfxFrameBuffer` (interpreter.cpp:6356-6357).
   - MSAA > 1 and matched: resolve `mGameFb -> 0` directly (no texture
     handle) (interpreter.cpp:6359).
   - MSAA == 1: expose `mGameFb`'s color texture as `mGfxFrameBuffer`
     (interpreter.cpp:6362).
3. `Fast3dWindow` returns control to the GUI, which calls
   `ImGui::Image(mGfxFrameBuffer, size)` inside `Gui::DrawGame`
   (`libultraship/src/ship/window/gui/Gui.cpp:718-722`).
4. ImGui renders its draw data to FB 0; `EndFrame()` swaps buffers.

The single, clean injection point is step 2: between MSAA resolve and the
moment `mGfxFrameBuffer` is exposed to ImGui.

## 3. Integration shape (the LUS-side scaffolding)

This part is the same regardless of which shader schema we adopt.

### 3.1 Always render to a sampleable texture

The matched-resolution MSAA fast path that resolves directly to FB 0 must be
removed when the post-process stage is active. With CRT on, always resolve to
`mGameFbMsaaResolved` so the post-process pass has a real texture handle to
sample.

### 3.2 New rendering-API surface

Add to `Fast::GfxRenderingAPI`:

```cpp
struct PostProcessParams {
    uint32_t srcWidth, srcHeight;       // game FB native dimensions
    uint32_t dstWidth, dstHeight;       // window dimensions
    uint32_t frameCount;
    float    frameDeltaSeconds;
    // Plus a parameter blob for user uniforms.
};

virtual int  CreatePostProcessProgram(const PostProcessSource& src) = 0;
virtual void DestroyPostProcessProgram(int progId) = 0;
virtual void RunPostProcess(int progId, int srcFb, int dstFb,
                            const PostProcessParams& p) = 0;
```

`PostProcessSource` carries per-backend shader text (already-transpiled GLSL
for OpenGL, HLSL for D3D11, MSL for Metal) — the transpiler runs in the
*core layer*, not inside each backend, so each backend only needs a native
compile path.

Each backend (`gfx_opengl.cpp`, `gfx_direct3d11.cpp`, `gfx_metal.cpp`)
already has the primitives needed (FBO/RTV creation, sampler creation,
fullscreen-triangle vertex buffer). New code per backend is on the order of
200–400 LOC.

### 3.3 New FBO chain

Add `mPostProcessFb[N]` to the interpreter for ping-pong between passes.
Final pass writes into `mPostProcessFb[last]`; the interpreter sets
`mGfxFrameBuffer` to that texture before returning.

### 3.4 Where shaders live on disk

`f3d.o2r` is already a `.zip` of `libultraship/src/fast/shaders/`
(CMakeLists.txt:450-456). The bundled *built-in* post-process passes
(see §6) live under `libultraship/src/fast/shaders/postprocess/`
and ride along inside `f3d.o2r`. User-supplied shaders load from a
sibling directory (`shaders/` next to the executable) — not in
`f3d.o2r`, since users need to drop files there at install time.

## 4. Schema choice — the real decision

The user wants maximum compatibility with existing shader libraries. Three
realistic formats to support, in increasing order of complexity:

### Option S1: Single-file plain GLSL fragment, our own uniform schema

User drops a `.glsl` file (Mednafen-style header optional). We define a
fixed set of uniforms:

```glsl
uniform sampler2D Source;
uniform vec2  TextureSize;       // game FB resolution
uniform vec2  OutputSize;        // window resolution
uniform vec2  InputSize;
uniform int   FrameCount;
uniform float FrameDirection;
```

Single pass only. We compile the GLSL → SPIR-V via glslang/shaderc at load
time, then transpile to HLSL/MSL via SPIRV-Cross for the D3D11 and Metal
backends. OpenGL uses the source as-is.

- **Library compatibility**: ~30% of the wild. Most simple CRT/scanline
  shaders on Shadertoy or in indie repos are single-file fragment shaders
  and port trivially. The big libretro presets (royale, guest, easymode-
  halation) are multipass and won't load.
- **Effort**: small. ~1–2 weeks.
- **Dependencies added**: SPIRV-Cross (Apache 2.0), glslang or shaderc
  (Apache 2.0). Adds ~3–4 MB to the binary.

### Option S2: RetroArch `.glslp` (multipass legacy GLSL)

`.glslp` is an INI-style preset listing multiple `.glsl` passes with FBO
scale/filter/wrap parameters. Only legacy GLSL inside (no Vulkan-GLSL
extensions). Native fit for OpenGL; D3D11/Metal need SPIRV-Cross.

- **Library compatibility**: ~50% of the wild. Covers `libretro/glsl-
  shaders` (including crt-easymode-halation, crt-geom-halation, scanlines-
  sine-abs, hyllian-shadow-mask, etc.). Does NOT cover `libretro/slang-
  shaders` which is where most active development moved post-2019 (crt-
  royale and crt-guest-advanced exist there only).
- **Effort**: medium. `.glslp` parser is a few hundred LOC; multipass FBO
  chain we need anyway.
- **Bonus**: most `.glslp` shaders also exist in libretro/glsl-shaders as
  individual files, so users can grab either.

### Option S3: RetroArch `.slangp` (Vulkan-GLSL preset format)

The current libretro standard. Vulkan-flavored GLSL with semantic UBO
blocks, `#pragma format`, history/feedback textures. Authored once,
transpiled per backend via SPIRV-Cross. `.slangp` is an INI-style preset.

- **Library compatibility**: ~95% of the wild — including crt-royale,
  crt-guest-advanced, crt-lottes-fast, every modern preset. This is "the
  shader format" in 2026.
- **Effort**: significant. Beyond what S2 needs we have to:
  - Parse `.slangp` (parameters, scale_type, scale_xN, history/feedback,
    `alias`, `srgb_framebuffer`, `float_framebuffer`).
  - Honor the libretro semantic blocks (`UBO`, `PushConstant`) and texture
    names (`Source`, `Original`, `OriginalHistoryN`, `PassFeedback`,
    `MVP`).
  - Track per-pass scaled FBO chains with history rings and feedback
    targets.
  - Build VS/FS pairs from a single source where both `#pragma stage
    vertex` and `#pragma stage fragment` are present.

  ~3–4 weeks plus a real test pass against the upstream library.
- **License caveats**: SPIRV-Cross / glslang are Apache 2.0, fine. The
  *shaders themselves* are largely GPL (see §5). As long as we never
  redistribute them, our binary stays MIT.

### Recommendation

Stage them — they share the same scaffolding (post-process FBO chain +
SPIRV-Cross + multipass), so the work is mostly additive:

- **Phase 1**: §3 scaffolding + Option S1 (single-file `.glsl`). Useful on
  day one, covers the simple-CRT case, validates the per-backend code.
- **Phase 2**: extend to multipass — implement `.glslp` (S2). Most of this
  is the preset parser + the FBO chain extension; both are reusable for
  S3.
- **Phase 3**: `.slangp` (S3). Adds slang semantic block handling and
  history/feedback support. This is what unlocks the full libretro/slang-
  shaders library.

If we only have time for one phase ever, do Phase 1 + Phase 3 and skip
`.glslp`. The `.glslp` library is mostly a subset of what's available in
slang form.

## 5. Licensing — what we ship vs what users supply

The Battleship binary and LUS are both **MIT**
(`LICENSE`, `libultraship/LICENSE`).

The shader library landscape, from the research pass:

| Shader | License | Passes |
|---|---|---|
| crt-lottes | **Public domain** | Single |
| crt-mattias | **Public domain** (upstream `crtview` repo) | Single |
| crt-geom | GPLv2+ | Single |
| crt-easymode | GPL | Single |
| crt-easymode-halation | GPL | Multi |
| crt-royale | GPLv2+ | Multi (~8) |
| crt-guest-advanced | GPLv2+ | Multi (many) |
| crt-hyllian | GPL | Single/Multi |
| crt-pi | GPL | Single |

**The shipping strategy this implies:**

1. **Bundle only public-domain shaders** in `f3d.o2r`. Realistic candidates:
   crt-lottes (canonical Lottes shader, public domain header), crt-mattias
   (Mattias Gustavsson, "do whatever"). Both are single-pass so they work
   with Phase 1.
2. **Do not bundle GPL shaders.** Users who want crt-royale or crt-guest
   download them from `libretro/slang-shaders` themselves and drop them
   in the runtime `shaders/` directory. This matches how RetroArch itself
   ships (RetroArch core is GPL, but the shader repo is loose data the
   user installs). Since we never redistribute the GPL files, our MIT
   binary distribution is clean.
3. **Document the policy** in `README.md` and in the UI's shader-picker
   menu: "Built-in shaders are public-domain. Drop additional .slangp
   shaders into `shaders/` — see libretro/slang-shaders for a catalog."

This is the legally cleanest path. RetroArch operates under the same
assumption (shaders as data, not linked code).

**One caveat**: glslang and SPIRV-Cross are *runtime dependencies* — they
ARE linked into the binary. Both are Apache 2.0, which is MIT-compatible.
No issue.

## 6. Upstreamability — keep this generic, not SSB64-specific

This feature is general-purpose; **the runtime must land in LUS, not in
`port/`**, so SoH / 2S2H / Starship / Battleship can all consume it.
Concrete rules for the implementing agent:

### 6.1 What lives where

| Layer | Goes in | Why |
|---|---|---|
| `RunPostProcess` / `CreatePostProcessProgram` API additions on `GfxRenderingAPI` | `libultraship/include/fast/backends/gfx_rendering_api.h` + the three backend `.cpp` | Pure rendering primitive |
| Post-process FBO chain (allocation, ping-pong, resize on window change) | `libultraship/src/fast/interpreter.cpp` (new helper class `PostProcessChain` next to `Interpreter`) | Belongs with the other FBO bookkeeping |
| `.slangp` / `.glslp` parser + shader-source loader + SPIRV-Cross / glslang glue | new `libultraship/src/fast/postprocess/` dir | LUS feature, not port-specific |
| Public-domain bundled shaders (crt-lottes, crt-mattias) | `libultraship/src/fast/shaders/postprocess/` (rides along inside `f3d.o2r`) | LUS-managed asset |
| User-supplied shader directory location, CVar names | LUS — use namespace `gShaders.*` or `gPostProcess.*`, **not** `gBattleShipCrt.*` | Cross-port reusable |
| Menu wiring (the actual `ImGui::MenuItem` entries) | LUS exposes an enumerable shader list + parameter introspection API; **each downstream port writes its own menu** in `port/gui/PortMenu.cpp` | Ports already own their menu styling |
| LUS-side menu hook (if LUS wants its built-in "Enhancements" menu to gain a Shaders submenu) | LUS — but optional, behind a config flag, so ports that already have their own menu don't get duplicates | Cooperative, not forced |

### 6.2 "Generic" means no game-specific assumptions

- No SSB64 paths, ROM constants, or `decomp/` references anywhere in the
  new code.
- No assumption that the game FB has 4:3 aspect — query
  `mCurDimensions` like the existing code does.
- No assumption about MSAA level — the post-process pass must work at
  `MSAA = 1, 2, 4, 8`. If MSAA > 1, the resolve to `mGameFbMsaaResolved`
  is the input texture; if MSAA == 1, `mGameFb` is. Both already exist.
- No assumption about widescreen — `mWidescreenActive` is an SSB64 concept
  injected from the port; the LUS runtime should treat the input FB as a
  generic 2D image with `srcWidth × srcHeight` regardless.
- No reliance on Battleship's ConsoleVariableBridge naming — use the
  generic `Ship::Context::GetInstance()->GetConsoleVariables()` interface
  that already exists in LUS.

### 6.3 Upstreaming workflow

1. Implement on a `feature/postprocess-shaders` branch of
   `JRickey/libultraship` (the fork we already pin).
2. Add a Battleship-side menu PR on `port-patches` consuming the LUS
   feature.
3. Open a PR against `Kenix3/libultraship` main (upstream LUS) with just
   the LUS-side changes. Include before/after screenshots and a note that
   none of the LUS-family ports currently ship a CRT shader.
4. Until upstream merges, Battleship pins to the JRickey fork branch
   (current setup).

## 7. Backend implementation references for the implementing agent

When the implementing agent reads this plan, it should already know to
look at the existing combiner shader path in each backend as the
template — but here are the exact files and external specs to keep open.

### 7.1 Inside LUS (read these first)

- **`libultraship/src/fast/backends/gfx_opengl.cpp`** — full GL backend.
  Look at:
  - `GfxRenderingAPIOGL::CreateAndLoadNewShader` (line ~399) — how a
    shader gets compiled, linked, and registered.
  - `GfxRenderingAPIOGL::DrawTriangles` (search for it) — the vertex
    layout and `glDrawArrays` pattern. A fullscreen quad/tri is a
    degenerate case of this.
  - `GfxRenderingAPIOGL::UpdateFramebufferParameters` and `CreateFramebuffer`
    (~lines 711, 778) — how FBOs are allocated, including MSAA color
    attachments and resolve targets.
- **`libultraship/src/fast/backends/gfx_direct3d11.cpp`** — D3D11 backend.
  Look at:
  - `CreateAndLoadNewShader` for D3DCompile and ID3D11VertexShader /
    ID3D11PixelShader creation.
  - Framebuffer creation: search for `D3D11_TEXTURE2D_DESC` and
    `CreateRenderTargetView`.
  - Sampler/state setup.
- **`libultraship/src/fast/backends/gfx_metal.cpp`** + **`gfx_metal_shader.cpp`**
  — Metal backend (Metal-cpp via the macOS framework).
  - Shader compilation: `MTLLibrary` / `MTLFunction` creation in
    `gfx_metal_shader.cpp`.
  - Render pipeline state in `gfx_metal.cpp`.
- **`libultraship/include/fast/backends/gfx_rendering_api.h`** — the
  abstract interface (currently 96 lines). New virtuals go here.
- **`libultraship/src/fast/interpreter.cpp:6193+`** — FBO allocation and
  the end-of-frame MSAA resolve choice. The post-process pass injects
  here.
- **`libultraship/src/ship/window/gui/Gui.cpp:718-722`** — the
  `ImGui::Image(mGfxFrameBuffer)` call that consumes the result.

### 7.2 External specs / docs

- **OpenGL 3.3 Core Profile** — Khronos spec
  https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf
  (LUS's minimum GL version is what `gfx_opengl.cpp` already targets;
  4.1 is the macOS legacy cap, but Metal is preferred there anyway).
  Specifically need: framebuffer objects (chapter 4), GLSL 330 fragment
  shaders, vertex array objects (3.6).
- **Direct3D 11** — MSDN
  https://learn.microsoft.com/en-us/windows/win32/api/d3d11/
  Specifically: `ID3D11DeviceContext::OMSetRenderTargets`,
  `D3D11_RENDER_TARGET_VIEW_DESC`, `ID3D11SamplerState`. HLSL 5.0 SM5
  is the target (`vs_5_0` / `ps_5_0`).
- **Metal Shading Language Spec** — Apple
  https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf
  Plus `MTLRenderCommandEncoder`, `MTLRenderPipelineDescriptor`,
  `MTLTexture` usage docs. macOS 10.15+ MSL 2.2 is a safe baseline.
- **SPIRV-Cross** — https://github.com/KhronosGroup/SPIRV-Cross
  README documents the C and C++ APIs. We need
  `spirv_cross::CompilerGLSL`, `CompilerHLSL`, `CompilerMSL`.
- **glslang** — https://github.com/KhronosGroup/glslang
  `glslang::TShader` + `glslang::TProgram` + `glslang::GlslangToSpv`.
  Alternative: **shaderc** (https://github.com/google/shaderc) which
  wraps glslang with a friendlier C API; either works.
- **libretro slang shader spec** — the authoritative format docs:
  https://github.com/libretro/slang-shaders/blob/master/README.md
  https://github.com/libretro/slang-shaders/tree/master/docs
  This describes `.slangp` syntax (INI), `#pragma stage`,
  `#pragma format`, semantic UBO blocks, history/feedback, and the
  bind-name conventions (`Source`, `Original`, `OriginalHistoryN`,
  `PassOutput0`, `PassFeedback0`, `MVP`).
- **libretro Vulkan / slang spec** — same repo, especially
  `slang-shaders/docs/slang_shaders.md` if present; otherwise the
  glsl-shaders README covers `.glslp` syntax:
  https://github.com/libretro/glsl-shaders/blob/master/README.md
- **RetroArch slang-shader runtime (read for behavior reference ONLY,
  see §8 — do not copy code)**:
  `RetroArch/gfx/drivers_shader/shader_glsl.c`,
  `shader_vulkan.cpp`, `slang_process.cpp`. These are GPL and must not
  be transcribed.

### 7.3 Existing reference implementations (look at design, not code)

- **Dolphin emulator's post-processing system**
  (`Source/Core/VideoCommon/PostProcessing.cpp`) — GPL. Useful only as a
  black-box example of how an emulator integrates a `.slangp`-ish
  multipass chain into its native renderer. **Do not copy code.**
- **Mednafen's OpenGL shader path** — license varies, mostly GPL. Same
  caveat.
- **PCSX2's post-processing** — GPL.

If we want a *permissive*-licensed reference to read freely, there isn't
one I'm aware of. SPIRV-Cross's own sample programs (Apache 2.0) are the
closest thing for the transpiler-glue layer.

## 8. Legal — is our runtime a derivative of RetroArch's GPL shader runtime?

Short answer: **no, if we follow the clean-room rules below, and the file
format / API conventions are not themselves copyrightable.**

### 8.1 What copyright actually covers

Software copyright protects *expression*: the specific source code,
including variable names, structure, comments, the particular sequence of
statements implementing an algorithm. It does **not** protect:

- File formats (`.slangp` INI syntax, `#pragma stage` directives,
  semantic block names like `MVP`, `Source`, `OriginalHistoryN`).
- APIs and naming conventions — settled in *Google v. Oracle* (SCOTUS
  2021), declaring code and naming conventions are non-protectable /
  fair use when reimplemented.
- General algorithms and rendering patterns ("render a fullscreen
  triangle sampling a previous FBO", "ping-pong between two render
  targets", "parse INI describing a multipass chain"). These are facts
  about how rendering works.

The libretro shader format conventions are documented as a public spec.
Reading the spec and implementing a parser/runtime from that spec is
unambiguously fine. The same way WINE reimplements the Win32 API,
Mesa reimplements OpenGL, and dozens of clean-room JVMs exist.

### 8.2 Clean-room rules the implementing agent must follow

1. **Authoritative references are spec docs, not source.** When in doubt,
   read `libretro/slang-shaders/README.md` and the format docs, not
   `RetroArch/gfx/drivers_shader/slang_process.cpp`.
2. **Behavioral verification is fine; source transcription is not.**
   It's legal — and recommended for correctness — to load the same
   `.slangp` preset in RetroArch and in our runtime and compare the
   rendered output. It is **not** legal to open RetroArch's
   `slang_process.cpp` next to our parser and translate it function by
   function.
3. **Test against the libretro shader corpus.** Use `libretro/slang-
   shaders` shader files as black-box test inputs (loading them is fine
   — they are data, GPL applies to redistribution, and we never
   redistribute them). Validate that crt-royale, crt-guest, crt-lottes-
   fast, etc. produce plausible output.
4. **Name things ourselves.** Class names, function names, variable
   names, and code structure should come from our design, not be
   transliterations of RetroArch's. (E.g. our parser's main type is
   `PostProcessPreset`, not `slang_preset`.)
5. **Comments and documentation are ours.** Don't paste comments from
   GPL sources.
6. **No "look at RetroArch's code to fix this bug" shortcuts.** If a
   shader doesn't render right, read the spec and debug from first
   principles. If it's truly opaque, file a question to the libretro
   forums or read the *spec discussion* threads, not the source.
7. **Document the clean-room provenance.** Top of each new file:
   `// Implemented from the public libretro slang/slangp format spec.
   // No code copied from RetroArch or any GPL-licensed shader runtime.`
   This is belt-and-braces, but useful if anyone ever asks.

### 8.3 The dependencies are fine

- **SPIRV-Cross** — Apache 2.0 + MIT. MIT-compatible. Linking is fine.
- **glslang** — BSD-3 + Apache 2.0. MIT-compatible. Linking is fine.
- **shaderc** (optional alternative to glslang) — Apache 2.0.
  MIT-compatible.

None of these are GPL. The transpiler glue and parser we author sit on
top of them and stay MIT alongside LUS.

### 8.4 Could the format itself be a problem?

No. `.slangp` is an INI file. `.slang` is Vulkan-GLSL with semantic
conventions. Neither the file format nor the convention names are
copyrightable. The shaders *themselves* are — which is why we never
redistribute GPL shaders, only load them as user data at runtime (see §5).

### 8.5 Is this too specific to LUS to be copyrightable on our side?

Our runtime's interface (`GfxRenderingAPI::RunPostProcess` plus a
`PostProcessChain` helper inside the interpreter) is generic enough that
it would land cleanly in any rendering abstraction. But that genericity
*helps* us — it means our code is not "substantially similar to RetroArch
even by accident," because RetroArch's runtime is tightly coupled to
its own `gfx_ctx` driver model, while ours plugs into LUS's
`GfxRenderingAPI`. Different architectures, different code shape, no
accidental similarity.

### 8.6 Bottom line

We can ship our runtime as MIT, link it against Apache-2.0 third-party
libs, and load user-supplied GPL shaders at runtime — without our binary
or our source taking on any GPL obligation, as long as the implementing
agent follows the clean-room rules in §8.2. This is the same legal
posture RetroArch's shader format users (Dolphin, PCSX2, Citra
historically) all relied on; the difference is that those projects
*chose* to be GPL for other reasons. We don't have to.

If we ever want a stronger legal posture, the agent can keep a short
"clean-room log" in `docs/crt_shader_provenance.md` listing which spec
sections were consulted for each major feature, and confirming no GPL
source was read while implementing it. Optional but cheap.

## 9. Open questions for the next planning round

1. **Built-in defaults**: do we ship crt-lottes + crt-mattias inside
   `f3d.o2r` so users get a CRT look out of the box (toggled in the
   PortMenu), or do we ship only the runtime and require users to
   provide their own shaders? Bundling Lottes by default has no license
   downside and is the friendlier UX.
2. **UI**: a CRT submenu in `port/gui/PortMenu.cpp` with "Off / Lottes /
   Mattias / Custom..." plus per-shader parameter sliders
   (Phase 1) and a `.slangp` preset picker (Phase 3). Wire to ConsoleVars
   under `gPostProcess.*`.
3. **Performance budget**: at 1440p × 60 fps, even crt-royale costs ~1–2 ms
   on modern GPUs. We should still expose an MSAA + post-process
   interaction option since multipass shaders prefer MSAA=1 input.
4. **macOS-specific**: ImGui-Metal driver pairing — confirm SPIRV-Cross
   MSL output compiles against the `MTLLibrary` API we already use in
   `gfx_metal.cpp` (it should; it's standard MSL).
5. **Asset packaging**: should user-supplied shaders go in a sibling
   `shaders/` directory next to the executable (filesystem),
   or do we let users repack `user.o2r`? Filesystem is simpler and
   matches RetroArch convention.
6. **Upstream LUS contribution**: this feature is general-purpose. We
   should ask kenix3 / the SoH / 2S2H / Starship maintainers whether
   they'd accept it upstream — none of them have a CRT shader today
   (confirmed by `repo:HarbourMasters/Shipwright` and the Starship and
   2S2H repos: no `crt`/`scanline`/`post_process` hits). Landing this
   upstream means we don't carry a LUS fork delta.

## 10. Estimated effort

| Phase | Adds | Effort |
|---|---|---|
| 1 | `RunPostProcess` API + 3 backend implementations + single-`.glsl` loader + SPIRV-Cross integration + crt-lottes default | 1–2 weeks |
| 2 | `.glslp` parser + multipass FBO chain | 1 week |
| 3 | `.slangp` parser + libretro semantic block handling + history/feedback | 2–3 weeks |

Phases 2 and 3 share most of their scaffolding with Phase 1, so the total
from-zero-to-S3 is roughly 4–5 weeks of focused work, not 6–8.

## 11. References

- libretro/slang-shaders (the modern catalog): https://github.com/libretro/slang-shaders
- libretro/glsl-shaders: https://github.com/libretro/glsl-shaders
- crt-lottes.glsl (public domain): https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-lottes.glsl
- Mattias Gustavsson MattiasCRT: https://www.shadertoy.com/view/Ms23DR
- libretro docs — crt shaders: https://docs.libretro.com/shader/crt/
- SPIRV-Cross: https://github.com/KhronosGroup/SPIRV-Cross (Apache 2.0)
- glslang: https://github.com/KhronosGroup/glslang (BSD-3, Apache 2.0)
