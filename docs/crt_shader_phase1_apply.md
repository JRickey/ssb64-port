# CRT/Post-Process Shader — What Shipped

This branch implements Phase 1 of `docs/crt_shader_plan_2026-05-11.md`,
plus the SPIRV-Cross transpiler glue and libretro single-file
normalizer originally tracked as Phase 1 follow-ups.

LUS-side changes live on `JRickey/libultraship@claude/postprocess-shaders`;
the outer tree's submodule pointer is bumped on each integration
commit (`aa3a2f1`, `508457c`, `942fe0e`, `99119e1`, `11fb63f`,
`184275f`, plus the transpiler/normalizer follow-ups).

## What Phase 1 adds

LUS feature work (game-agnostic, ready for upstream PR consideration):

- `Fast::GfxRenderingAPI` gains four non-pure virtuals with no-op
  defaults — `SupportsPostProcess`, `CreatePostProcessProgram`,
  `DestroyPostProcessProgram`, `RunPostProcess`. Existing backends stay
  buildable without touching them.
- A new `Fast::PostProcessChain` helper owns one sampleable output FBO
  and the currently-loaded compiled program. The interpreter holds one
  instance.
- `Interpreter::ComposeFinalFrame()` is a refactor of the
  end-of-frame MSAA-resolve block (duplicated between `Run` and
  `RunGuiOnly`) that now also runs the chain when active.
- `PostProcessSourceLoader` resolves shader names against
  `./shaders/<name>.glsl` on the filesystem first, then
  `shaders/postprocess/<name>.glsl` in `f3d.o2r`.
- OpenGL backend implements the four virtuals fully (~240 LOC):
  fullscreen-triangle VAO, fragment-program compile/link/uniform
  caching, MSAA-aware FlipY uniform, scissor/blend/depth-off pass with
  `mOpenglVbo`/`mOpenglVao` restoration so the regular draw path is
  untouched.
- `gPostProcessEnabled` and `gPostProcessShader` CVars added to
  `libultraship/cmake/cvars.cmake`. `Interpreter::StartFrame` reads
  them each frame and compiles/unloads only on transitions.
- Bundled builtin: `libultraship/src/fast/shaders/postprocess/scanlines.glsl`,
  original MIT work in the repo, demonstrating the standard uniform
  schema.

## Verifying

```bash
cmake -B build
cmake --build build --target libultraship -j 4
```

The libultraship target builds cleanly with my changes applied.

## Shipped follow-ups (since the original Phase 1 cut)

- D3D11 backend implementation.
- Metal backend implementation.
- SPIRV-Cross + glslang transpiler glue
  (`PostProcessTranspiler::SynthesizeMissing`). Fills the
  `PostProcessSource.{hlsl,msl}` slots from `.glsl` at load time;
  hand-tuned siblings on disk still win when present.
- Libretro single-file GLSL normalizer
  (`PostProcessGlslNormalizer::NormalizeUserGlsl`). Lets a raw
  `crt-*.glsl` from `libretro/glsl-shaders` (combined VS+FS,
  `Texture` / `TextureSize` / `TEX0` / `FragColor`, GLSL 120 idioms)
  load without manual adaptation.

## Still not implemented

- Multi-pass support (`.glslp`, `.slangp`) — `crt-royale`,
  `crt-guest-advanced`, and most "premium" CRT presets need this.
  See `docs/crt_shader_plan_2026-05-11.md` §4 / §10 for the
  Phase 2 / Phase 3 plan.
- UI for `#pragma parameter` tunables. They are accepted by the
  normalizer (stripped) and the shaders' `#define` fall-back blocks
  carry the defaults, but the menu doesn't surface per-shader knobs
  yet.
- Custom shader picker in the menu. `gPostProcessShader` is a string
  CVar, so any file dropped in `./shaders/<name>.glsl` is reachable
  by name via the console — but the picker only enumerates built-ins.

## Trying it

```bash
cmake --build build --target ssb64 -j 4
./build/BattleShip
# In the in-game console (built-ins from f3d.o2r):
gPostProcessShader = scanlines    # or crt-lottes
gPostProcessEnabled = 1
```

Drop a custom shader file at `./shaders/<name>.glsl` next to the
binary, then `gPostProcessShader = <name>`. Single-file libretro
shaders (e.g. `libretro/glsl-shaders/crt/shaders/hyllian/
crt-hyllian-curvature-glow.glsl`) load unmodified — the normalizer
handles the schema translation.
