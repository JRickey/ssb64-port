# Post-Process Shader — Status & Clean-Room Recap (2026-05-14)

Snapshot of where the post-process shader feature stands now that
Phase 3 (.slang / .slangp) has shipped through 3F + 3E. Read this
before touching `libultraship/src/fast/postprocess/` or any
`libultraship/tests/postprocess_*.cpp` test file — it's the index
into what's done, what's tested, and the clean-room rules that
constrain how we extend either side.

For deeper background: the original plan is
`docs/crt_shader_plan_2026-05-11.md` (architecture, schema choice,
licensing, §8 clean-room rules). The UX follow-up plan is
`docs/crt_shader_ux_plan_2026-05-11.md`. The Phase 1 apply-notes
are `docs/crt_shader_phase1_apply.md`. This file supersedes none of
them — it's a status sheet.

---

## What shipped (chronological, since Phase 2I)

LUS submodule, on `JRickey/libultraship@claude/postprocess-shaders`,
bumped into the outer tree commit-by-commit:

| Branch step | LUS commit | Outer bump | Surface |
|---|---|---|---|
| 2I  external PNG textures via `.glslp textures` | `f0273ef` | `e849051` | preset textures + chain upload |
| —   user-data shaders dir + picker walker         | `dbcfeb8` | (port-side) | `ListUserPostProcessShaders` |
| 2.1 emit `<alias>Size` uniforms                   | `368c68b` | (in 1fede08+) | normalizer + transpiler UBO tail |
| 2.2 mipmap_input per-pass                          | `e0cfc8a` | (later 3-series) | preset stores; chain consumes |
| 2.3 `#pragma parameter` sliders                    | `a28e48c` then revert `eba77d5` | reverted | n/a |
| 3A  `.slangp` preset parser                        | `368a5e1` | `144a986` | `ParseSlangPreset` + `parameterOverrides` |
| 3B  `.slang` stage splitter + `#pragma` extractor | `5afdb35` | `dd2790f` | `ParseSlangSource` + `#pragma parameter` |
| 3C  `.slang` → SPIR-V → backend transpile          | `ac3f02e` | `352363f` | `PostProcessSlangTranspiler::Compile` |
| 3D-1 slang program create/destroy API + GL impl   | `0fec78a` | `66c81c8` | `GfxRenderingAPI::CreateSlangPostProcessProgram` |
| 3D-2 GL slang Run path (single-pass)              | `952f412` | `3168b25` | `RunSlangPostProcess` (GL) |
| 3F  source loader + builtin slang                  | `058f54d` | `832f9b8` | `LoadPostProcessSlangShader` + bundled slang |
| 3D-3 D3D11 + Metal slang backends                  | `d1c3b65` | `9835c04` | `RunSlangPostProcess` (D3D11/Metal) |
| —   clean-room hygiene comment fixes               | `f039d81` | `2110296` | provenance comments only |
| 3E  `OriginalHistoryN` + `PassFeedbackN` rings     | `ab1250c` | `e24c3ff` | chain ring allocator + `MatchSemanticIndex` |
| —   WIP shader UX (downloader / diag / sidecar)    | `53c4b42` | `f0fa284` | `GetPostProcess{ShaderInfo,RuntimeDiagnostics}` |

## What's tested today

All counts after the Phase 3E bump (commit `e24c3ff`):

```
$ build/libultraship/tests/lus_tests --gtest_brief=1
[==========] 106 tests from 24 test suites ran. (65 ms total)
[  PASSED  ] 105 tests.
[  SKIPPED ] 1 test.   # LUS_TEST_USER_SHADER not set
```

| File | Surface |
|---|---|
| `postprocess_preset_tests.cpp` | `.glslp` + `.slangp` parsing — passes, scale/wrap/srgb/float/mipmap/alias/textures/parameter overrides, error cases |
| `postprocess_transpiler_tests.cpp` | Normalizer + GLSL→HLSL/MSL transpile — single-pass, libretro single-file, alias bindings, `<alias>Size` uniforms, env-gated user shader |
| `postprocess_slang_source_tests.cpp` | `.slang` stage splitter + `#pragma stage/name/format/parameter` extractor — comprehensive |
| `postprocess_slang_transpiler_tests.cpp` | `.slang` → SPIR-V → backend transpile — single Source sampler, UBO reflection, malformed-glsl rejection |

## Normalizer fixes landed on this branch (2026-05-14)

Three bugs that together drove the libretro single-file pack reject
rate from 100% on the realistic-shader subset down through
intermediate states to a 33% remaining tail:

1. **`ExtractVsVaryings` first-`#endif` walk** —
   `PostProcessGlslNormalizer.cpp` used to find the first `#endif`
   after `#if defined(VERTEX)`, which usually closes the inner
   `#if __VERSION__ >= 130 ... #endif` block. The VS body was then
   truncated before any `COMPAT_VARYING` declaration, no `#define`
   macros were emitted, and the FS rejected `sinangle` / `onex`
   / `invdims` etc. as undeclared. Replaced with a depth-tracking
   walk that pairs `#if`/`#ifdef`/`#ifndef` with `#endif` and stops
   at depth 0 (or `#elif`/`#else` at depth 1).

2. **`PreprocessForVulkan` left unknown samplers unbound** —
   libretro shaders frequently declare history-style samplers
   (`PassPrev3Texture`, `LUT0`, mask textures) that aren't in the
   `.glslp` `aliasN` list. The Vulkan-targeted preprocessor stripped
   schema/alias samplers and re-emitted them with explicit bindings,
   but kept other `uniform sampler2D <name>;` lines untouched —
   glslang under `EShClientVulkan` requires `layout(binding=N)` on
   every sampler. Now the preprocessor catches every leftover
   `uniform sampler2D <name>;`, strips it, and re-emits with an
   auto-assigned binding past the alias range. The runtime never
   binds data there in Phase 1, so sampling returns zero — a visual
   artifact instead of a hard load failure.

3. **`ExtractVsVaryings` ran on the pre-rewrite source** — the
   identifier-rewrite pass turned `TextureSize` → `SourceSize` and
   `TEX0` → `vTexCoord` in the FS body. But `ExtractVsVaryings`
   captured the macro RHS expressions from the ORIGINAL source,
   leaving `#define invdims (1.0/TextureSize)` etc. When the FS
   expanded the macro, it reintroduced an identifier the rewrite
   had already removed. Fix: run extraction on the post-rewrite
   buffer so macros and FS body share one name-space.

**Downloader install gate**: `port/enhancements/ShaderDownloader.cpp`
now runs each candidate through the same
`Fast::NormalizeUserGlsl` + `Fast::PostProcessTranspiler::SynthesizeMissing`
pipeline the picker uses, BEFORE copying the file into
`<user-data>/shaders/libretro/`. Anything that doesn't transpile
under the current LUS build is dropped with a single counter bump;
the completion status reads `Installed N shaders (skipped M
unsupported by this build)`. The check is self-maintaining — when
the normalizer learns to handle a new shader shape, the next
download automatically picks up the additional files without code
changes in the downloader.

Corpus walker progression on `shaders/libretro/` (176 single-file
`.glsl` shaders):

| State | Pass | Pct |
|---|---|---|
| Before any fix | 85 / 176 | 48.3% |
| After fixes 1 + 2 | 105 / 176 | 59.7% |
| After fix 3 (final) | 117 / 176 | 66.5% |

The remaining 59 failures are dominated by VS computations the
line-walker can't handle (intermediate locals, multi-statement
varying derivations, conditionals) — properly fixing them needs a
real GLSL parser or per-shader workarounds, both out of scope for
this pass. A handful of files are genuinely malformed under
glslang's strict mode (e.g. `crt-consumer-classic.glsl` syntax
errors, `zfast_crt.glsl` vec2/vec4 arithmetic mismatch) and would
not load in any clean-room runtime without targeted patches.

## Coverage gaps the next pass should fill

These are the holes I'm filling on this branch (see new tests under
`libultraship/tests/`):

1. **Normalizer ExtractVsVaryings — depth-aware `#endif` walk.**
   FIXED — see "Normalizer fixes landed" above.
2. **Slang reflection of `OriginalHistoryN` / `PassFeedbackN` /
   `PassOutputN`** — Phase 3E added `MatchSemanticIndex` in the
   chain, but no test confirms those names actually surface from the
   transpiler's reflection step. COVERED by
   `PostProcessSlangTranspiler.ReflectsHistoryAndFeedbackSamplers`.
3. **`#pragma parameter` → UBO member round-trip** — COVERED by
   `PostProcessSlangTranspiler.ParameterDeclMatchesUboMember`.
4. **Runtime diagnostics setter/getter** — COVERED by
   `PostProcessRuntimeDiagnostics.*` (7 tests).
5. **Real-corpus reject rate** — env-gated walker
   (`LUS_TEST_USER_SHADER_DIR=…`) at
   `PostProcessTranspiler.AcceptsUserSuppliedShaderDirectory` —
   informational summary, never fails. Run on this branch:
   117/176 (66.5%) accept rate.

What's still NOT testable from a unit-test seat (and why):

- The bundle loaders (`LoadSinglePassBundle` / `LoadPresetBundle` /
  `LoadSlangSinglePassBundle` / `LoadSlangPresetBundle`) live in an
  anonymous namespace inside `PostProcessSourceLoader.cpp` — the
  external entry points (`LoadPostProcessShader` /
  `LoadPostProcessSlangShader`) call into `Ship::Context` for the
  user-data path, which we can't stand up under gtest. If we want
  coverage here, expose the inner functions in the header (no
  semantic change, just visibility), or mock the context.
- Backend Run paths (`RunPostProcess` / `RunSlangPostProcess`) need
  real GL / D3D11 / Metal device contexts — covered today only by
  the in-game smoke test.
- `GetPostProcessShaderInfo` reads sidecars from `UserShaderRoots()`,
  also Ship::Context-bound. Same fix as the bundle loaders if we
  want unit coverage.

---

## Clean-room rules — recap (don't skip)

The full rules are in `docs/crt_shader_plan_2026-05-11.md` §8. The
hot path when adding code or tests:

1. **Spec, not source.** When a slang/.slangp behavior is unclear,
   read `libretro/slang-shaders/README.md` and the `docs/` files in
   that repo. Do NOT open `RetroArch/gfx/drivers_shader/*.c` —
   it's GPL, and any line we transcribe poisons our MIT posture.
2. **Tests use synthetic inputs.** Every test in
   `libultraship/tests/postprocess_*.cpp` uses a hand-authored
   minimal shader stub. The corpus-walker test is the one
   exception, and it loads real shader files as opaque data — it
   never quotes their content into our source. Keep this contract:
   if a new test needs a multi-pass example, write the smallest
   possible synthetic one.
3. **Naming is ours.** Our types are `PostProcessPreset`,
   `PostProcessSlangArtifact`, `PostProcessChain`, etc. — not
   `slang_preset`, `pass_data`, `glsl_filter_chain`. If you find
   yourself reaching for a libretro/RetroArch type name because
   "that's what they call it," pause and pick a name that
   describes what our code does.
4. **Provenance comment at the top of every new file.** Pattern:
   `// Implemented from the public libretro <which-spec> docs at <URL>.`
   `// No code copied from RetroArch or any GPL-licensed shader runtime.`
   Every new file in `src/fast/postprocess/` and
   `tests/postprocess_*` carries this. If you copy-paste a header
   from a sibling file, double-check the URL still applies (a slang
   parser file shouldn't claim to be implemented from the .glslp
   spec).
5. **Behavioral parity is fine; line-by-line parity is not.** If a
   shader renders wrong, it is OK to load the same `.slangp` in
   RetroArch and observe the difference. It is NOT OK to open
   RetroArch's `slang_process.cpp` next to ours and "translate" the
   relevant function.
6. **GPL shader files are user data.** We never bundle a GPL `.glsl`
   or `.slang` inside `f3d.o2r`. The downloader pulls them at
   runtime into `<user-data>/shaders/libretro/`, which is data the
   user installs — same legal posture as RetroArch's own shader
   updater. The two bundled shaders (`scanlines`, `crt-lottes`) are
   CC0 originals authored in this repo.

If a future agent is unsure whether something crosses the line, the
default is to ask — not to peek at the GPL source "just for
reference." A single transcribed line costs more than a half-day
spent re-deriving it.

---

## Suggested next phases (when we pick this back up)

The Phase 3 stack is functional but the picker's reject rate is
high. In rough order of leverage:

1. **Fix `ExtractVsVaryings` `#endif` walk** (described above).
   This single change should unblock most rejected libretro `crt-*`
   single-file shaders.
2. **Parse `#ifdef PARAMETER_UNIFORM` blocks correctly.** Even with
   the VS-walk fix, several shaders ship parameter defaults inside
   `#ifdef PARAMETER_UNIFORM` — we currently leave the block in but
   don't define `PARAMETER_UNIFORM`, so `uniform COMPAT_PRECISION
   float ...;` lines reach the FS as plain identifiers (with no
   declaration) when they're referenced from extracted VS varyings.
   Either define `PARAMETER_UNIFORM` in the preamble (and rewrite
   `COMPAT_PRECISION` to nothing) or pre-substitute the
   `#ifndef PARAMETER_UNIFORM` defaults block.
3. **Parameter sliders re-land.** Phase 2.3 was reverted; the
   reflection plumbing for slang `#pragma parameter` is in place
   already. Surface them in the menu.
4. **Multi-pass slang tests.** 3D-2 / 3D-3 ship the runtime; only
   single-pass artifacts have direct test coverage today.
