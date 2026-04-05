# SSB64 PC Port — Claude Session Context

This is a PC port of Super Smash Bros. 64, built from the complete decompilation at github.com/Killian-C/ssb-decomp-re.

## Project Status
- Decompilation is complete: 23/27 functions matched, 4 non-matching (debug only, excluded from port)
- All ~950 function placeholders and ~88 data placeholders renamed
- All 24 debug struct types renamed to descriptive names
- Port repo created, planning phase
- Target integration: libultraship (LUS) + Torch asset pipeline

## ROM Info
- File: `baserom.us.z64` — Z64 big-endian, NTSC-U v1.0
- SHA1: `e2929e10fccc0aa84e5776227e798abc07cedabf`
- MD5: `f7c52568a31aadf26e14dc2b6416b2ed`
- Game code: NALE, internal name "SMASH BROTHERS"

## Key Dependencies
- **libultraship** (Kenix3/libultraship) — replaces libultra with PC-native rendering, audio, input, resource management
- **Torch** (HarbourMasters/Torch) — asset extraction from ROM into .o2r archives via YAML configs
- **Fast3D** — translates N64 F3DEX2 GBI display lists to OpenGL/D3D11/D3D12/Metal
- **SDL2** — windowing, input, audio backend
- Reference ports: HarbourMasters/Starship (Star Fox 64), HarbourMasters/SpaghettiKart (Mario Kart 64)

## What NOT to Include in Port
- `src/ovl8/` — Debug overlay (developer tools)
- `src/db/` — Debug battle/menu testing
- Assembly files (`asm/`) — Port uses C code only
- N64 ROM or copyrighted assets — Users provide their own ROM

## Architecture

### Source Organization
```
src/
  sys/    — System: main loop, DMA, scheduling, audio, controllers, threading
  ft/     — Fighters: per-character logic (ftmario/, ftkirby/, ftfox/, etc.)
  sc/     — Scene management
  gm/     — Game modes
  gr/     — Graphics/stage rendering
  mn/     — Menus
  it/     — Items
  ef/     — Effects/particles
  lb/     — Library utilities
  mp/     — Maps
  wp/     — Weapons/projectiles
  mv/     — Movie/cutscene
  if/     — Interface/HUD
  credits/ — Credits sequence
  libultra/ — Decompiled N64 SDK (replaced by LUS in port)
```

### Port Target Structure
```
port/         — Modern C++ port layer (Ship::Context, resource factories, bridges)
yamls/us/     — Torch YAML asset extraction configs
libultraship/ — Git submodule
torch/        — Git submodule
CMakeLists.txt
```

---

## C Language Conventions

### Type System
The codebase uses N64 SDK types from `PR/ultratypes.h`. Use these consistently:

| Type | Meaning | Size |
|------|---------|------|
| `u8, u16, u32, u64` | Unsigned integers | 1/2/4/8 bytes |
| `s8, s16, s32, s64` | Signed integers | 1/2/4/8 bytes |
| `f32, f64` | Float / double | 4/8 bytes |
| `sb8, sb16, sb32` | Signed booleans | 1/2/4 bytes |
| `ub8, ub16, ub32` | Unsigned booleans | 1/2/4 bytes |

**Do not use** `int`, `short`, `long`, `float`, `double` in game code. Use the SDK typedefs.

Custom vector/color types from `ssb_types.h`:
- `Vec2f`, `Vec2h`, `Vec2i`, `Vec3f`, `Vec3h`, `Vec3i`
- `SYColorRGB`, `SYColorRGBA`, `SYColorRGBPair`, `SYColorPack`
- `Mtx44f` — 4x4 float matrix

### Naming Conventions (Decomp Style)
The decomp uses a consistent prefix system. Preserve it in all original game code:

- **Module prefixes**: `sy` (system), `ft` (fighter), `sc` (scene), `gm` (game mode), `gr` (graphics), `mn` (menu), `it` (item), `ef` (effect), `lb` (library), `mp` (map), `wp` (weapon), `if` (interface), `mv` (movie)
- **Global variables**: `gXXYyyy` — `g` prefix + module prefix + name (e.g., `gSYMainThread5`)
- **Static variables**: `sXXYyyy` — `s` prefix + module prefix + name (e.g., `sSYMainThread1Stack`)
- **Data (initialized)**: `dXXYyyy` — `d` prefix + module prefix + name (e.g., `dSYMainSceneManagerOverlay`)
- **Functions**: `xxYyyy` — module prefix lowercase + name (e.g., `syMainSetImemStatus`)
- **Enums**: `nXXYyyy` — `n` prefix + module prefix + name (e.g., `nSYColorRGBAIndexR`)
- **Structs/Types**: `XXYyyy` — module prefix uppercase + name (e.g., `SYOverlay`, `SYColorRGB`)

Port-specific code (in `port/`) may use modern C/C++ naming but should maintain clean boundaries with decomp code.

### Code Style
- **Indentation**: Tabs (matching decomp)
- **Braces**: GNU/Allman style — opening brace on its own line for function bodies
- **Section banners**: The decomp uses decorated comment blocks to separate sections:
  ```c
  // // // // // // // // // // // //
  //                               //
  //       EXTERNAL VARIABLES      //
  //                               //
  // // // // // // // // // // // //
  ```
  Preserve these in existing files. Not required in new port-specific code.
- **Boolean values**: Use `TRUE` / `FALSE` (defined as 1/0), not `true`/`false`
- **NULL**: Defined as `0`, not `((void*)0)`

### Macro Conventions
Key macros from `macros.h` — use these instead of rolling your own:
- `ARRAY_COUNT(arr)` — element count of static arrays
- `ALIGN(x, align)` — align value up
- `ABS(x)` / `ABSF(x)` — absolute value (int / float)
- `SQUARE(x)`, `CUBE(x)`, `BIQUAD(x)` — power macros
- `PI32`, `HALF_PI32`, `DOUBLE_PI32` — float pi constants
- `DTOR32` / `RTOD32` — degrees-to-radians / radians-to-degrees
- `F_CST_DTOR32(x)` / `F_CLC_DTOR32(x)` — degree-to-radian conversion (use CST for const multiplication, CLC for step-by-step calculation)
- `UPDATE_INTERVAL` (60) — ticks per second
- `TIME_SEC`, `TIME_MIN`, `TIME_HRS` — timing constants
- `I_SEC_TO_TICS(q)`, `F_SEC_TO_TICS(q)` — time conversion macros
- `PHYSICAL_TO_ROM(x)` — convert physical address to 0xB0 ROM address

---

## Nintendo 64 Technical Reference

### Memory Architecture
- **RDRAM**: 4 MB (8 MB with expansion pak). All game data lives here.
- **Segmented addressing**: The N64 uses segment registers. Addresses like `0x06001234` mean segment 6, offset 0x1234. The segment table maps segment IDs to physical RDRAM addresses.
- **DMA**: Data is transferred from ROM cartridge to RDRAM via DMA (Direct Memory Access) through the PI (Peripheral Interface). All ROM access is async DMA, not memory-mapped reads.
- **Overlays**: Code and data are loaded from ROM into RDRAM on demand. SSB64 uses overlays extensively (see `SYOverlay` struct with ROM_START/END, VRAM, TEXT/DATA/BSS segments).

### Graphics Pipeline (Reality Co-Processor)
- **RSP** (Reality Signal Processor): Programmable MIPS-based coprocessor that runs "microcode" (ucode). SSB64 uses **F3DEX2** microcode for geometry processing.
- **RDP** (Reality Display Processor): Fixed-function rasterizer. Handles texturing, blending, z-buffer, anti-aliasing.
- **Display lists**: GPU commands are built as arrays of `Gfx` structs (64-bit words each). The GBI macros (`gSPVertex`, `gDPSetTextureImage`, `gSPDisplayList`, etc.) write into these arrays.
- **Framebuffer**: 320x240 (NTSC) at 16-bit or 32-bit color. Double-buffered.
- **TMEM**: 4 KB of texture memory on the RDP. Textures must be loaded into TMEM before use, limiting texture size per draw call.

### GBI (Graphics Binary Interface)
Display list commands fall into three categories:
- **SP commands** (`gSP*`): RSP geometry commands — vertex loading, matrix operations, display list calls, lighting
- **DP commands** (`gDP*`): RDP rasterization commands — texture loading, color combiners, blend modes, fill/rect operations
- **DMA commands**: Bulk data transfer commands

When porting, these GBI calls are intercepted by libultraship's Fast3D renderer, which translates them to modern GPU API calls. The decomp code continues to call GBI macros normally.

### Audio
- **N64 audio**: Software-mixed on the RSP using audio microcode. Audio banks contain instrument definitions, samples (ADPCM compressed), and sequences (MIDI-like).
- The audio subsystem (`src/sys/audio.c`, `include/n_audio/`) manages sound effects, music, and mixing.
- In the port, audio processing routes through SDL2 instead of the RSP.

### Threading Model
SSB64 uses the N64 OS threading system:
- **Thread 0**: Idle thread (lowest priority)
- **Thread 1**: Boot/init
- **Thread 3**: Scheduler (priority 120) — manages RSP/RDP task submission
- **Thread 4**: Audio (priority 110) — processes audio DMA and mixing
- **Thread 5**: Game logic (priority 50) — main game loop
- **Thread 6**: Controller polling (priority 115)

In the port, this threading model is collapsed. libultraship runs a single main loop with explicit calls for graphics, audio, and input at the appropriate points.

### Controller Input
- N64 controller: analog stick (s8 x/y, range ~-80 to +80), 14 digital buttons, D-pad
- `I_CONTROLLER_RANGE_MAX` = 80, `F_CONTROLLER_RANGE_MAX` = 80.0f
- Controller data read via `OSContPad` struct
- In the port, libultraship's ControlDeck maps modern gamepad/keyboard input to `OSContPad` format

### Save Data
- SSB64 uses **SRAM** for save data (battery-backed cartridge RAM)
- In the port, SRAM read/write calls are redirected to filesystem operations

### Endianness
- N64 MIPS R4300i is **big-endian**. All multi-byte values in ROM and RDRAM are big-endian.
- The decomp's C code already handles this correctly (the compiler managed byte ordering).
- On PC (little-endian x86), libultraship handles any necessary byte swapping transparently through the resource system. Data loaded from .o2r archives is already in native host byte order.
- **Do not** add manual byte-swap code in game logic. If you encounter endianness issues, it means the asset extraction or resource loading layer needs fixing, not the game code.

---

## Build & Tooling Rules

### Build System
- CMake is the build system
- libultraship and Torch are git submodules
- MSVC on Windows, Apple Clang on macOS, GCC/Clang on Linux
- The decomp's original MIPS toolchain (IDO 7.1) is NOT used for the port
- Build script: `build.ps1` (PowerShell) — supports `-Clean`, `-SkipExtract`, `-ExtractOnly`
- Manual build: `cmake -S . -B build && cmake --build build --target ssb64 --config Debug`

### Runtime Logs
After running `build/Debug/ssb64.exe`:
- **Game trace log**: `build/Debug/ssb64.log` — `port_log()` output (boot sequence, thread creation, frame milestones)
- **LUS/spdlog log**: `build/Debug/logs/Super Smash Bros. 64.log` — libultraship logging (resource loading, rendering, errors)
- The game trace log is overwritten each run; the spdlog log is cumulative

### IDO 7.1 Compiler Patterns
The decompiled C code contains patterns that are artifacts of the original IDO 7.1 MIPS compiler. These are intentional and should be preserved in decomp code:
- Specific register allocation patterns may produce odd-looking variable usage
- `goto` statements used to match branch patterns
- Unusual cast chains or temporary variables to match instruction sequences
- `do { } while (0)` wrappers
- These exist to produce matching assembly output against the original ROM and should NOT be "cleaned up" in the decomp source files

### Compiler Compatibility
When modifying decomp code for the port:
- `ultratypes.h` defines `u32` as `unsigned long` and `s32` as `long` — on modern 64-bit compilers, `long` is 8 bytes on LP64 (Linux/macOS) but 4 bytes on LLP64 (Windows/MSVC). This must be addressed in the port's type definitions.
- The `__attribute__((aligned(x)))` macro in `macros.h` is immediately undefined by `#define __attribute__(x)` — this is an IDO compatibility hack. The port will need to fix this for GCC/Clang/MSVC.
- `#ifdef __sgi` guards IDO-specific code paths. The port uses `__GNUC__` or `_MSC_VER` paths.

---

## Agent Directives

### Pre-Work

1. **THE "STEP 0" RULE**: Before any structural refactor on a file >300 LOC, first remove dead code, unused exports, unused imports, and debug logs. Commit cleanup separately.

2. **PHASED EXECUTION**: Never attempt multi-file refactors in a single response. Break work into phases. Complete Phase 1, run verification, wait for approval before Phase 2. Max 5 files per phase.

### Code Quality

3. **THE SENIOR DEV OVERRIDE**: If architecture is flawed, state is duplicated, or patterns are inconsistent — propose and implement structural fixes. Ask: "What would a senior, experienced, perfectionist dev reject in code review?" Fix all of it.

4. **FORCED VERIFICATION**: Do not report a task complete until you have run the build and fixed all errors. If no build is configured yet, state that explicitly.

5. **DECOMP PRESERVATION**: Never "clean up" or "modernize" decompiled game code in `src/` unless it is necessary for compilation on modern toolchains. IDO patterns (goto, odd casts, temp variables) exist for matching and must be preserved. Port-specific modifications should be wrapped in `#ifdef PORT` / `#endif` guards where possible.

### Context Management

6. **SUB-AGENT SWARMING**: For tasks touching >5 independent files, launch parallel sub-agents. Each agent gets its own context window.

7. **CONTEXT DECAY AWARENESS**: After 10+ messages, re-read any file before editing. Do not trust memory of file contents.

8. **FILE READ BUDGET**: For files over 500 LOC, use offset and limit parameters to read in chunks.

9. **EDIT INTEGRITY**: Before every edit, re-read the file. After editing, verify the change applied correctly. Never batch >3 edits to the same file without a verification read.

10. **NO SEMANTIC SEARCH**: When renaming or changing any function/type/variable, search separately for: direct calls, type references, string literals, dynamic references, re-exports, and tests.
