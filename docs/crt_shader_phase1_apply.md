# Phase 1 CRT/Post-Process Shader — Applying the libultraship Patch

This branch implements Phase 1 of `docs/crt_shader_plan_2026-05-11.md`.
The LUS-side changes live in `docs/crt_shader_phase1_libultraship.patch`
rather than as a submodule SHA bump, because the session that authored
the patch could not push to `JRickey/libultraship` over the local proxy.

## Applying

```bash
cd libultraship
git checkout -b claude/postprocess-shaders        # or feature/postprocess-shaders
git am ../docs/crt_shader_phase1_libultraship.patch
git push -u origin claude/postprocess-shaders
cd ..
git add libultraship
git commit -m "Bump libultraship: post-process shaders (Phase 1, OpenGL)"
```

## What the patch adds

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

## Not yet implemented (follow-ups)

- D3D11 backend implementation.
- Metal backend implementation.
- SPIRV-Cross + glslang transpiler glue (Phase 1 ships the
  `PostProcessSource.{hlsl,msl}` fields as reserved-and-empty).
- Multi-pass support (`.glslp`, `.slangp`).

See `docs/crt_shader_plan_2026-05-11.md` §10 for the effort breakdown.

## Trying it

After applying:

```bash
cmake --build build --target ssb64 -j 4
./build/ssb64
# In the in-game console:
gPostProcessShader = scanlines
gPostProcessEnabled = 1
```
