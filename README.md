# ssb64-port

A PC port of **Super Smash Bros. (N64, NTSC-U v1.0)** built on top of the [VetriTheRetri/ssb-decomp-re](https://github.com/vetritheretri/ssb-decomp-re) decompilation, using [libultraship](https://github.com/Kenix3/libultraship) for PC-native rendering / audio / input and [Torch](https://github.com/HarbourMasters/Torch) for extracting assets out of the ROM at build time.

Runs natively on macOS (Apple Silicon + Intel), Linux, and Windows.

## No copyrighted assets are included in this repository

**None of Nintendo's assets (code, textures, audio, models, text, ROM data) are checked into this repo or distributed with builds.** The port is a pure C/C++ source tree; every byte of Nintendo-owned data is extracted at build time from a ROM that *you* supply. If you do not own a legal copy of Super Smash Bros. for the Nintendo 64, you cannot build or run this project.

The required ROM is **NTSC-U v1.0** (game code `NALE`, internal name `SMASH BROTHERS`):

| Hash   | Value                                      |
|--------|--------------------------------------------|
| SHA‑1  | `e2929e10fccc0aa84e5776227e798abc07cedabf` |
| MD5    | `f7c52568a31aadf26e14dc2b6416b2ed`         |

If your dump does not match those hashes, it will not work.

## About the project

This is a one-person hobby project — I'm doing it for fun, and to learn about N64 internals, graphics, and porting. There is no team, no roadmap, and no release schedule. Expect bugs, expect the port to sit idle for weeks at a time, and expect things to get rewritten when I learn they were wrong.

[Claude](https://claude.com/claude-code) is credited as a co-contributor on most commits. That's not a novelty — Claude has genuinely done a large share of the work here: reading disassembly, diagnosing endian / bitfield / LP64 issues, writing fix patches, and keeping the documentation under `docs/` current. Treating that labor as "mine" would be dishonest. If you're curious what that collaboration looks like in practice, the `docs/bugs/` folder is a reasonable tour — most of those write-ups are joint work.

## Prerequisites

You will need:

- **Git** (with submodule support)
- **CMake** ≥ 3.20
- **Python 3** (used by the reloc-stub / YAML / credits generators in `tools/`)
- **zip** (used to pack the Fast3D shader archive)
- A **C/C++ toolchain**:
  - macOS: Xcode Command Line Tools (`xcode-select --install`)
  - Linux: `gcc`/`clang`, plus the usual SDL2/OpenGL dev headers that libultraship pulls in
  - Windows: Visual Studio 2022 with the "Desktop development with C++" workload
- A **legal NTSC-U v1.0 ROM** (see hash table above)

Optional but recommended: `ninja` on macOS/Linux — the build script will use it automatically if it's on `PATH`, otherwise it falls back to Unix Makefiles.

## Build instructions

### 1. Clone the repo

```bash
git clone https://github.com/JRickey/ssb64-port.git
cd ssb64-port
```

### 2. Initialize submodules

```bash
git submodule update --init --recursive
```

### 3. Provide the ROM

Drop your NTSC-U v1.0 ROM into the project root and name it exactly:

```
baserom.us.z64
```

It must be a big-endian `.z64` dump (not `.v64` / `.n64`). The build will refuse to start if the file is missing.

### 4. Run the build script

**macOS / Linux:**

```bash
./build.sh
```

**Windows (PowerShell):**

```powershell
.\build.ps1
```

Common flags (both scripts):

| Flag | Behavior |
|------|----------|
| *(none)* | Full build + asset extraction (Debug) |
| `--release` / *(edit script for ps1)* | Release build |
| `--skip-extract` / `-SkipExtract` | Build the executable, skip Torch asset extraction |
| `--extract-only` / `-ExtractOnly` | Re-extract assets only (assumes Torch is already built) |
| `--clean` / `-Clean` | Wipe `build/`, `ssb64.o2r`, `f3d.o2r` and start over |

The script will, in order:

1. Initialize submodules (`libultraship`, `torch`)
2. Regenerate `include/reloc_data.h`, the `yamls/us/reloc_*.yml` extraction configs, and `port/resource/RelocFileTable.cpp` from `tools/reloc_data_symbols.us.txt`
3. Encode the credits text strings (`src/credits/*.credits.us.txt` → `.credits.encoded` / `.credits.metadata`)
4. Configure CMake and build the `ssb64` executable
5. Build Torch as an ExternalProject and run it against your ROM to produce `ssb64.o2r`
6. Zip `libultraship/src/fast/shaders/` into `f3d.o2r`
7. Copy both archives next to the executable

When it finishes you'll have:

```
build/ssb64           # the executable (ssb64.exe on Windows)
build/ssb64.o2r       # game assets extracted from your ROM
build/f3d.o2r         # Fast3D shader archive
```

### 5. Run it

```bash
./build/ssb64
```

On first launch the game creates `ssb64.cfg.json` next to the executable. Default keyboard controls are documented in [`CONTROLS.md`](CONTROLS.md); you can rebind them in-game via the controller settings menu, or plug in a gamepad (SDL2 controller support is on by default).

## Why the submodules are forks

Both `libultraship` and `torch` are pinned to **my personal forks** rather than the upstream Harbour Masters repos, because each needs SSB64-specific changes that don't exist upstream.

### [`libultraship`](https://github.com/JRickey/libultraship/tree/ssb64) — fork of [Kenix3/libultraship](https://github.com/Kenix3/libultraship)

SSB64 drives the RDP differently than the Zelda / Mario 64 / Star Fox 64 titles that libultraship was originally built for — in particular around how tile masks, `SetTileSize` extents, and IA/I4 texture uploads interact during fighter rendering. Upstreaming those fixes cleanly is on the list, but until then the fork carries changes such as:

- Clamping `ImportTexture*` upload width/height to the active `SetTileSize` extent (fixes the fighter "black squares" bug and the IA8/I4 stretch bug)
- Honouring `SetTile` masks/maskt in the Import* path
- A no-logging path in `IResource`'s destructor to prevent a shutdown-time crash
- Improved Fast3D texture error logging for diagnostics

### [`torch`](https://github.com/JRickey/Torch/tree/ssb64) — fork of [HarbourMasters/Torch](https://github.com/HarbourMasters/Torch)

Torch is the tool that reads the ROM and emits `ssb64.o2r`. The upstream repo supports OoT, MM, SF64, MK64, PM64, etc., but has no knowledge of SSB64's file formats. The fork adds:

- An `SSB64` build flag and game target
- A reloc-file factory for SSB64's relocatable data blobs (fighters, items, effects, sprites)
- `libvpk0` integration for VPK0-compressed segments

In short: modifying these projects was the *only* way to get game-specific asset layouts and rendering quirks handled correctly. They live as submodules so their history stays their own, and so upstream changes can be merged in cleanly when/if the fixes are accepted upstream.

## Repo layout

```
src/          decompiled C source (unchanged game logic)
  sys/        main loop, DMA, scheduling, audio, controllers, threading
  ft/         fighters (ftmario/, ftkirby/, ftfox/, …)
  sc/ gm/ gr/ scene / game modes / stage rendering
  mn/ it/ ef/ menus / items / effects
  …
port/         modern C++ port layer — Ship::Context, resource factories,
              bridges between decomp code and libultraship
include/      headers (some generated: reloc_data.h)
libultraship/ submodule — PC-native render / audio / input / resource mgr
torch/        submodule — asset extractor
yamls/us/     Torch YAML extraction configs (some generated)
tools/        Python helpers: reloc stubs, YAML gen, credits encoder
docs/         architecture notes, bug write-ups, debugging guides
debug_tools/  optional disasm / diff utilities (not required for a build)
```

Further reading:

- [`docs/architecture.md`](docs/architecture.md) — project structure, ROM info, dependencies
- [`docs/c_conventions.md`](docs/c_conventions.md) — decomp naming prefixes, code style
- [`docs/n64_reference.md`](docs/n64_reference.md) — RDRAM, RSP/RDP, GBI, audio, endianness
- [`docs/build_and_tooling.md`](docs/build_and_tooling.md) — CMake details, reloc stub regen, runtime logs
- [`docs/bugs/README.md`](docs/bugs/README.md) — per-bug root-cause and fix write-ups

## Contributing

PRs are welcome but please don't be offended if responses are slow — this is a side project. If you're opening a bug report, the most useful things to include are:

- SHA-1 of your `baserom.us.z64`
- OS + architecture (especially macOS ARM64 vs x86_64, since LP64 bitfield layout has bitten us several times — see `docs/bugs/`)
- The contents of `ssb64.log` from the run that reproduces the issue

## Credits & licensing

- Game code, data, sound, textures, models, and trademarks: **© Nintendo / HAL Laboratory.** Not included in this repository, not redistributed, and not endorsed by them.
- Decompilation: [Killian-C/ssb-decomp-re](https://github.com/Killian-C/ssb-decomp-re) and its contributors.
- Runtime framework: [libultraship](https://github.com/Kenix3/libultraship) (Kenix3 and the Harbour Masters team).
- Asset pipeline: [Torch](https://github.com/HarbourMasters/Torch) (Harbour Masters).
- Reference ports I learned from: [Starship](https://github.com/HarbourMasters/Starship) (SF64), [SpaghettiKart](https://github.com/HarbourMasters/SpaghettiKart) (MK64).
- Port work: me ([JRickey](https://github.com/JRickey)), with an enormous amount of help from [Claude](https://claude.com/claude-code).

This project is **not affiliated with, endorsed by, or authorized by Nintendo.** It is a personal, non-commercial research and preservation effort. Do not upload ROMs, extracted `.o2r` archives, or any other Nintendo-owned data to issues or pull requests.
