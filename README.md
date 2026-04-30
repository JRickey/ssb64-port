# BattleShip

**BattleShip** is a PC port of **Super Smash Bros. (N64, NTSC-U v1.0)** built on top of the [VetriTheRetri/ssb-decomp-re](https://github.com/vetritheretri/ssb-decomp-re) decompilation, using [libultraship](https://github.com/Kenix3/libultraship) for PC-native rendering / audio / input and [Torch](https://github.com/HarbourMasters/Torch) for extracting assets out of the ROM at build time.

Runs natively on macOS (Apple Silicon), Linux, and Windows.

## No copyrighted assets are included in this repository

**None of Nintendo's assets (code, textures, audio, models, text, ROM data) are checked into this repo or distributed with builds.** The port is a pure C/C++ source tree; every byte of Nintendo-owned data is extracted at build time from a ROM that *you* supply. If you do not own a legal copy of Super Smash Bros. for the Nintendo 64, you cannot build or run this project.

The required ROM is **NTSC-U v1.0** (game code `NALE`, internal name `SMASH BROTHERS`):

| Hash   | Value                                      |
|--------|--------------------------------------------|
| SHA‑1  | `e2929e10fccc0aa84e5776227e798abc07cedabf` |
| MD5    | `f7c52568a31aadf26e14dc2b6416b2ed`         |

If your dump does not match those hashes, it will not work.

## Author's notes

### About the project

This is a 100% AI-generated modern port. It took a little over 25 days, with me, Opus 4.6, Opus 4.7, and GPT 5.5 as the only contributors. As of 4/28, there are no >2 day gaps in development. At many points, agents were dispatched and worked to build and test autonomously while I did other things.

I wanted to do this project for two reasons.

**First, to personally get experience with developing software in C/C++ and learning the motions of how to develop software (git, worktrees, cmake, macros, etc).** I am not a software developer at my job, nor was I educated in computer science. C is a hobby for me, since I only write Python and TypeScript at work. So part of the motivation was to learn how things are actually made in C. How the sausage is made, if you will.

I have been using AI for a long time at this point. I started with Sonnet 3.5 and 3.7 to rewrite my Python code in graduate school to structure it better. I've been through all the motions: pasting files into chat on the web, copying responses, prompting *continue*, then everything breaking because the model couldn't output the whole file in a single prompt. As you probably know, a lot of things have changed since then — and that leads me to the second reason.

**Second, as a proof of concept that AI can be used for a task of this magnitude.** That is not to say that I gave AI an N64 cartridge and got a PC port out of it — years of work from many people went into the decomp, the 3D engine, asset extraction, and everything else this port stands on. The point I am trying to make is that there are tons of cases like this that are low-hanging fruit and can be done with a little bit of your time using AI.

I am not sponsored or endorsed by OpenAI or Anthropic — in fact I've spent ~$500 in subscription fees and extra usage on this project. I want it to serve as proof that the barrier to making really cool things is incredibly low. Humans have always progressed our knowledge and capabilities by expanding on the work of others. Agentic coding is the newest frontier of that principle. I hope this project serves as inspiration for other people to learn by doing, make something for themselves, and give it freely to others.

---

### Working with AI on a project like this

#### Timeline

First boot and scaffolding took 2 days. Rendering took about 20 and was the most difficult part of the project. Audio fell in nicely after the agents finally had a good grasp of the common pitfalls. Opus did the vast majority of the work, but GPT 5.5 takes credit for a few big bug fixes, and sole credit for audio.

Before working on the port I tried my hand at assisting the decomp. Multiple failed attempts there built a lot of working knowledge for the agents on IDO / MIPS specific quirks and issues that paid off later. We tore into the decomp and the IDO decomp a lot. Failures there gave me inspiration to working on a port instead.

#### Proper dev tools

I did not use a debugger once. Instead all the debugging kit was written by the agents, for the agents to use:

- **`gbi_trace` + `m64p_trace_plugin` + `gbi_diff`** was the workhorse when developing rendering. These allowed the agents to compare the display list in our port vs the game running on an emulator. Instructions on how to use the debug tools were also written by the agents and dropped in `docs/`. `CLAUDE.md` steered them to reach for these tools when appropriate instead of static analysis.
- **`acmd_trace` + `acmd_diff` + `m64p_audio_dump_plugin` + `adpcm_diff`** are the audio analogues to the rendering tools.
- **`rom_disasm/decode_bitfields.py` + `disasm.py`** compile a snippet with IDO, disassemble with rabbitizer, and read off the actual bit positions. This is how every bitfield rewrite under `#ifdef PORT` was verified instead of guessed.
- **`sprite_deswizzle.py`** is a standalone TMEM-swizzle reference implementation, used to visually verify the LUS-side swizzle fixes in an independent environment instead of in the renderer that was being debugged.
- **`reloc_extract`** pulls relocatable-data layouts from the ROM for verifying the Torch factory output.

The lesson here is to never debug the port in isolation. Every truly difficult issue was eventually solved by capturing the same signal from the emulator, diffing the two, then looking at the first divergence. This workflow was exceptionally performant and enabled unsupervised development.

#### Documentation as a workflow primitive

The main strategy I used to actually do this was to force the agents to document all bug patterns whenever they made a fix, and to document how they resolved it. These are all indexed through their respective `CLAUDE.md` and `AGENTS.md` files. This workflow was invaluable for gaining momentum and fixing stray issues as they came down the line later. `HANDOFF.md` files were also used by agents to carry context across multiple debugging sessions.

#### Bug highlights

What follows are bug highlights as per Claude. Some may appear a bit silly, but part of the point is to document this entire project. Yes, if I was more experienced with C I would have seen these issues coming. Yes, if I knew more about N64 porting, I might not have made these mistakes. That is all true but not the point — the point is to teach how to avoid these pitfalls for any brave person who wants to do something similar.

1. **Endianness — halfswap on loaded data blocks (9 bugs).** The decomp's halfswap files are halfswapped at load; any pointer reached through them needs a per-stream un-halfswap. Recurred across animation events, splines, vertex data, sprite TMEM, and collision data.
2. **LP64 pointer truncation (9 bugs).** The single most expensive lesson: `-Wno-implicit-function-declaration` was silently truncating 64-bit pointer returns to 32-bit `int`. Fingerprint: a fault address < 4 GiB, or weird patterns like `0xAAA000000BBB` from adjacent reloc-token pairs.
3. **IDO BE bitfield physical layout (4 bugs).** IDO packs small bitfields MSB-first into preceding `u16` pad gaps; modern compilers don't. The bug always came back if patched in game-code reads instead of the struct layout itself — codified into a standing rule.
4. **Fast3D unimplemented stubs (13 rendering bugs total).** `gDPSetPrimDepth` was a `TODO Implement` upstream — every 2D sprite using `G_ZS_PRIM` rendered at z=0 (front). The pattern "grep `TODO Implement` in `libultraship/src/fast/interpreter.cpp` when rendering looks wrong" is now a memory entry.
5. **LUS-vs-decomp typename shadowing (2 bugs, big blast radius).** libultraship redefines `OSContPad`, `OSMesg`, `OSContStatus` with larger layouts than the decomp; `sizeof()` in a C++ stub overran the C caller's buffer and zeroed unrelated state. Caused the menu double-input bug.

#### Closing

The load-bearing fix isn't always where you think. It's very helpful to turn down the reasoning ability on these agents and prompt them to get a big-picture view first.

And with that, I hope you enjoy playing the game, finding bugs that I missed, and that you might've learned something about what is possible today.

Take it away, Claude:

---

## Building

If you want to manually compile BattleShip, please consult the [building instructions](docs/BUILDING.md).

## Architecture

The port has three layers and they are kept deliberately separate:

```
┌──────────────────────────────────────────────────────────────┐
│  decompiled game code  (src/)                                │
│  Unmodified C produced by the decomp project. Talks to the   │
│  N64 the same way the original ROM did: GBI display lists,   │
│  ALSeqPlayer audio, OS threads, OSContPad input.             │
├──────────────────────────────────────────────────────────────┤
│  port layer            (port/)                               │
│  Modern C++ glue. Translates N64-shaped APIs into LUS calls, │
│  fixes endianness on freshly-loaded data, owns Ship::Context │
│  and the resource factories, and quarantines every change    │
│  the decomp doesn't need to know about.                      │
├──────────────────────────────────────────────────────────────┤
│  libultraship         (libultraship/)                        │
│  PC-native runtime: Fast3D renderer (OpenGL / Metal / D3D),  │
│  SDL2 input, miniaudio output, OTR/O2R resource manager,     │
│  ImGui overlay.                                              │
└──────────────────────────────────────────────────────────────┘
```

### Asset pipeline

`baserom.us.z64` is never read at runtime. At build time, **Torch** walks the ROM with the YAMLs under `yamls/us/` and emits `BattleShip.o2r` — a zip-format archive of typed resources (textures, sequences, sample banks, animations, reloc files). At launch, libultraship's resource manager mounts `BattleShip.o2r` + `f3d.o2r` and the port code requests resources by path. This is the same pipeline used by Ship of Harkinian, Starship, SpaghettiKart, etc.

The relocatable-data files (fighter tables, item tables, effects, sprites) are SSB64-specific and required custom factories on the Torch side and a custom loader (`port/resource/RelocFileFactory.cpp`) on the runtime side.

### Build-time codegen

A small amount of generated code lives outside the source tree (gitignored) and is regenerated on every build from `tools/reloc_data_symbols.us.txt`:

- `include/reloc_data.h` — extern declarations for every relocatable symbol
- `yamls/us/reloc_*.yml` — Torch extraction configs
- `port/resource/RelocFileTable.cpp` — the runtime symbol table

If you ever see "undefined reference to `dFooBarReloc`" you regenerated the table without rebuilding, or vice versa.

---

## Code conventions

### `#ifdef PORT` — what it is and what it isn't

Every meaningful change to a decomp source file is wrapped in `#ifdef PORT` / `#else` / `#endif`. The discipline this enforces:

- **The original decomp code path stays intact and compilable** under the IDO toolchain on a real N64 build. This is non-negotiable — if it ever stops being true, upstreaming improvements back to the decomp project becomes impossible.
- **The PORT branch is allowed to be ugly** — an explicit endian conversion, a struct rewrite, a function shim — as long as the contract it presents to the rest of the file is the same as the N64 branch.
- **Reloc tokens vs. raw pointers**: a field declared `u32` under `#ifdef PORT` where the N64 branch declared `T*` is a *reloc token*, not a raw pointer. Resolving it requires `PORT_RESOLVE(token)`. Assigning a real pointer with `(uintptr_t)ptr` will silently truncate on LP64 — wrap post-load writes in `PORT_REGISTER`.

### Decomp preservation: behavior, not bytes

The repo follows a single principle for changes to `src/`:

> **Accuracy to game behavior > accuracy to ROM bytes.**

That means IDO idioms that encode original N64 semantics — odd casts, `goto` flow, deliberate temporaries — are load-bearing and stay. But **compiler-compat shims** (warning suppressions, permissive flags, header shortcuts) that mask real bugs on modern LP64 toolchains do *not* survive. The most expensive lesson of the project was that `-Wno-implicit-function-declaration` was silently truncating 64-bit pointer returns to 32-bit `int` in dozens of places — see `docs/bugs/item_arrow_gobj_implicit_int_2026-04-20.md`. The shim is gone; the real declarations are in.

### Naming prefixes

The decomp uses two-letter module prefixes throughout. Knowing them makes the source tree navigable:

| Prefix | Meaning |
|--------|---------|
| `ft`   | Fighter (`ftMario`, `ftKirby`, `ftFox`, …) |
| `it`   | Item (`itAttribute`, `itManager`) |
| `wp`   | Weapon |
| `ef`   | Effect / particle |
| `gm`   | Game mode |
| `gr`   | Stage (ground) |
| `mp`   | Map / collision |
| `mn`   | Menu |
| `sc`   | Scene |
| `sy`   | System (engine internals) |
| `sf`   | Saved-state / save file |
| `db`   | Debug |
| `cm`   | Camera |
| `lb`   | Library (low-level utilities) |
| `obj`  | GObj / DObj / OMObj — game-object wrappers |

Full reference: [`docs/c_conventions.md`](docs/c_conventions.md).

### Endianness handling

The N64 is big-endian; PC targets are little-endian. The port handles this in three layers:

1. **Gross byteswap at load** — `lbRelocLoadAndRelocFile` byteswaps relocatable files word-by-word during load.
2. **Per-struct fixups** — small `portFixupStructU16` / `portFixupStructU8` helpers fix sub-word fields that the gross swap got wrong (e.g., `{u16, u8, u8}` patterns where the two `u8`s end up swapped).
3. **Per-stream walkers** — animation events, spline interpolators, and other variable-length streams are halfswapped at file-bake time and need a per-stream un-halfswap on first access. These live in `port/port_aobj_fixup.{h,cpp}` and friends.

If you find a new struct that reads garbage, the playbook in `docs/n64_reference.md` will tell you which layer it belongs in.

### Bitfield layout

The IDO compiler packs small bitfields into preceding `u16` pad gaps, MSB-first. Modern compilers (Clang, GCC) on LE targets pack LSB-first into the next storage unit. Bitfield structs that travel through file data must be **rewritten under `#ifdef PORT`** to match the IDO physical layout — see `docs/debug_ido_bitfield_layout.md` for the workflow (compile + rabbitizer disasm to verify bit positions before porting).

Patching the *reads* in game code instead of the *layout* is forbidden by team policy; the bug always comes back. See `feedback_struct_rewrite_over_overrides.md` in the project's memory for the long version.

---

## Why the submodules are forks

Both `libultraship` and `torch` are pinned to **personal forks** rather than the upstream Harbour Masters repos, because each needs SSB64-specific changes that don't exist upstream.

### [`libultraship`](https://github.com/JRickey/libultraship/tree/ssb64) — fork of [Kenix3/libultraship](https://github.com/Kenix3/libultraship)

SSB64 drives the RDP differently than the Zelda / Mario 64 / Star Fox 64 titles libultraship was originally built for — in particular around tile masks, `SetTileSize` extents, IA/I4 texture uploads, `gDPSetPrimDepth` for 2D layering, and Metal sampler binding for matanim CCs. Upstreaming the fixes is on the list, but until then the fork carries:

- Clamping `ImportTexture*` upload width/height to the active `SetTileSize` extent (fixes the fighter "black squares" bug and the IA8/I4 stretch bug)
- Honouring `SetTile` mask/maskt in the Import* path
- A real implementation of `gDPSetPrimDepth` (was a `TODO Implement` stub upstream — broke every `G_ZS_PRIM` 2D sprite)
- A 1×1 black fallback texture for sampler slots that would otherwise alias the screen drawable on Metal (fixed the Whispy canopy stripe)
- A no-logging path in `IResource`'s destructor to prevent a shutdown-time crash

### [`torch`](https://github.com/JRickey/Torch/tree/ssb64) — fork of [HarbourMasters/Torch](https://github.com/HarbourMasters/Torch)

Torch is the tool that reads the ROM and emits `BattleShip.o2r`. Upstream supports OoT, MM, SF64, MK64, PM64, etc., but has no knowledge of SSB64's file formats. The fork adds:

- An `SSB64` build flag and game target
- A reloc-file factory for SSB64's relocatable data blobs (fighters, items, effects, sprites)
- `libvpk0` integration for VPK0-compressed segments

Both forks live as submodules so their history stays their own and so upstream changes can be merged in cleanly when/if the fixes are accepted.

---

## Repo layout

```
src/          decompiled C source (largely unchanged game logic)
  sys/        main loop, DMA, scheduling, audio, controllers, threading
  ft/         fighters (ftmario/, ftkirby/, ftfox/, …)
  sc/ gm/ gr/ scene / game modes / stage rendering
  mn/ it/ ef/ menus / items / effects
  …
port/         modern C++ port layer — Ship::Context, resource factories,
              endian fixups, bridges between decomp code and libultraship
include/      headers (some generated: reloc_data.h)
libultraship/ submodule — PC-native render / audio / input / resource mgr
torch/        submodule — asset extractor
yamls/us/     Torch YAML extraction configs (some generated)
tools/        Python helpers: reloc stubs, YAML gen, credits encoder
docs/         architecture notes, bug write-ups, debugging guides
debug_tools/  optional disasm / diff utilities (not required for a build)
scripts/      packaging (.dmg / .AppImage / .zip), worktree helper
```

### Further reading

- [`docs/architecture.md`](docs/architecture.md) — project status, ROM info, dependency map, source-tree layout
- [`docs/c_conventions.md`](docs/c_conventions.md) — decomp naming prefixes, IDO idioms to preserve, code style, macros
- [`docs/n64_reference.md`](docs/n64_reference.md) — RDRAM, RSP/RDP, GBI, audio, threading, endianness primer
- [`docs/build_and_tooling.md`](docs/build_and_tooling.md) — CMake details, reloc stub regen, runtime logs, LP64 compat notes
- [`docs/debug_gbi_trace.md`](docs/debug_gbi_trace.md) — capturing GBI traces from the port and a M64P plugin, diffing with `gbi_diff.py`
- [`docs/debug_ido_bitfield_layout.md`](docs/debug_ido_bitfield_layout.md) — verifying ported struct bit positions against IDO output via rabbitizer
- [`docs/bugs/README.md`](docs/bugs/README.md) — index of resolved bugs with per-bug root-cause and fix write-ups (~45 entries)

---

## Contributing

PRs are welcome but please don't be offended if responses are slow — this is a side project. If you're opening a bug report, the most useful things to include are:

- SHA-1 of your `baserom.us.z64`
- OS + architecture (especially macOS ARM64 vs x86_64, since LP64 bitfield layout has bitten us several times — see `docs/bugs/`)
- The contents of `ssb64.log` from the run that reproduces the issue
- A GBI trace if the issue is rendering-related (see `docs/debug_gbi_trace.md`)

## Credits & licensing

- Game code, data, sound, textures, models, and trademarks: **© Nintendo / HAL Laboratory.** Not included in this repository, not redistributed, and not endorsed by them.
- Decompilation: [VetriTheRetri/ssb-decomp-re](https://github.com/VetriTheRetri/ssb-decomp-re) and its contributors.
- Runtime framework: [libultraship](https://github.com/Kenix3/libultraship) (Kenix3 and the Harbour Masters team).
- Asset pipeline: [Torch](https://github.com/HarbourMasters/Torch) (Harbour Masters).
- Menu fonts: [Montserrat](https://github.com/JulietaUla/Montserrat) and [Inconsolata](https://github.com/cyrealtype/Inconsolata), both bundled under the [SIL Open Font License 1.1](https://openfontlicense.org). License texts ship alongside the font files in [`assets/custom/fonts/`](assets/custom/fonts/).
- Reference ports I learned from: [Starship](https://github.com/HarbourMasters/Starship) (SF64), [SpaghettiKart](https://github.com/HarbourMasters/SpaghettiKart) (MK64).
- Port work: me ([JRickey](https://github.com/JRickey)), with an enormous amount of help from [Claude](https://claude.com/claude-code).

This project is **not affiliated with, endorsed by, or authorized by Nintendo.** It is a personal, non-commercial research and preservation effort. Do not upload ROMs, extracted `.o2r` archives, or any other Nintendo-owned data to issues or pull requests.

This project is **not affiliated with, endorsed by, or authorized by Harbour Masters** either. It uses libultraship (originated by the Harbour Masters team and now maintained at [Kenix3/libultraship](https://github.com/Kenix3/libultraship)) and Torch (the [HarbourMasters/Torch](https://github.com/HarbourMasters/Torch) asset extractor) as upstream dependencies via personal forks, but it is an independent fan effort. Issues, bugs, and support questions about this port should not be directed to the Harbour Masters team.

## License

Source code in this repository (everything outside `libultraship/`, `torch/`, and `src/` decomp content carrying its own attribution) is released under the [MIT License](LICENSE) — free to use, modify, and redistribute, with no warranty and no liability. See [`LICENSE`](LICENSE) for the full text.

The MIT grant covers only the port-specific code (the `port/` layer, build scripts, tools, docs). It does **not** extend to:
- Game assets, code, audio, textures, models, or any other content owned by Nintendo / HAL Laboratory — none of which is in this repository.
- The decompilation in `src/`, which carries its own license from the [VetriTheRetri/ssb-decomp-re](https://github.com/VetriTheRetri/ssb-decomp-re) project.
- The `libultraship` and `torch` submodules, which carry their own upstream licenses.
- The bundled menu fonts under `assets/custom/fonts/`, which are licensed under the SIL Open Font License 1.1 (per-font license files in that directory).
