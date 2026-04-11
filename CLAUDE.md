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
- Build scripts:
  - **Windows**: `build.ps1` (PowerShell) — supports `-Clean`, `-SkipExtract`, `-ExtractOnly`
  - **macOS / Linux**: `build.sh` (bash) — supports `--clean`, `--skip-extract`, `--extract-only`, `--release`
- Manual build: `cmake -S . -B build && cmake --build build --target ssb64`
- macOS generator: uses Ninja if installed, otherwise Unix Makefiles (single-config, so binary is `build/ssb64`, not `build/Debug/ssb64`)

### Regenerating the reloc symbol table
`include/reloc_data.h` is **generated** (it's in `.gitignore`). It declares ~3900 `ll*` linker symbols that the decomp references as file offsets inside the compressed `relocData` ROM blob. The port serves file contents through libultraship's RelocFile resource factory keyed by `file_id`, so the scalar values don't matter at runtime, but the symbols still have to exist at compile time.

Regenerate after adding new decomp sources with:

```bash
python3 tools/generate_reloc_stubs.py
```

The script scans `src/` for `ll[A-Z_][A-Za-z0-9_]*` identifiers and emits `#define <name> ((intptr_t)0)` entries. `#define` is required (not `extern intptr_t`) because the decomp uses these symbols as file-scope struct-literal initializers, which C11 rejects for non-constant expressions.

### Runtime Logs
After running the game:
- **Game trace log**: `ssb64.log` in the cwd — `port_log()` output (boot sequence, thread creation, frame milestones). Overwritten each run.
- **LUS/spdlog log**: `logs/Super Smash Bros. 64.log` — libultraship logging (resource loading, rendering, errors). Cumulative.
- On Windows the binary lives at `build/Debug/ssb64.exe` (multi-config MSVC), on macOS/Linux at `build/ssb64` (single-config Make/Ninja).

### IDO 7.1 Compiler Patterns
The decompiled C code contains patterns that are artifacts of the original IDO 7.1 MIPS compiler. These are intentional and should be preserved in decomp code:
- Specific register allocation patterns may produce odd-looking variable usage
- `goto` statements used to match branch patterns
- Unusual cast chains or temporary variables to match instruction sequences
- `do { } while (0)` wrappers
- These exist to produce matching assembly output against the original ROM and should NOT be "cleaned up" in the decomp source files

### Compiler Compatibility

**The LP64 `long` class of bug.** The N64 SDK headers type several fixed-width 32-bit fields as `long`, which is 4 bytes under MSVC LLP64 and 8 bytes under clang/gcc LP64. Every such field silently doubles in size on macOS/Linux, corrupting every struct that mirrors N64 file data or that gets shared with libultraship (which uses 32-bit field types internally). A full sweep of `include/` and `src/` under the `\blong\b` pattern found only three real offenders — all fixed:

- **`include/PR/ultratypes.h`** — `u32`/`s32`/`vu32`/`vs32` were `unsigned long`/`long`. Every decomp struct that packed N64 file data is affected. Fixed under `#ifdef PORT && !defined(_MSC_VER)` to use `unsigned int`/`int`. MSVC keeps the original definitions.
- **`include/PR/gbi.h`** — `Mtx_t` was `long[4][4]`. Doubles the matrix from 64 to 128 bytes on LP64, so `guMtxF2L`'s 4-byte-stride int writes land in alternating padding slots and Fast3D's reader sees garbage. The smoking gun was that `libultraship/include/fast/types.h:3` *already* defined `Mtx_t` as `int[4][4]` — LUS and the decomp headers disagreed about the matrix element size, and MSVC accidentally reconciled them because `long == int` under LLP64. On LP64 the two headers diverge and every 3D transform collapses to `x==y, z=0, w=0`. Fixed by aligning `gbi.h`'s `Mtx_t` to `int[4][4]` under `PORT && !_MSC_VER`.
- **`include/PR/gbi.h`** — `Gsetcolor::color`, `TexRect::w{0..3}`, `Hilite::force_structure_alignment`, `Gsetcolor`-family typed union views — all declared as `long` but **never accessed by field name** anywhere in `src/` / `port/` / `libultraship/`. The decomp builds display lists exclusively through the raw `Gwords` view (`g->words.w0`/`w1`), so the typed-struct LP64 drift is invisible and harmless. Left as-is.

**Audit clean at time of writing:**
- `src/` contains no `long`-typed struct fields (comment / macro-name matches only).
- `include/PR/ramrom.h`, `u64gio.h`, `PRimage.h`, `stdlib.h::ldiv_t` are `long`-typed but none of them are referenced by any compiled translation unit — the port never pulls in the N64 ramrom debug I/O path or the SGI image format.
- `include/PR/gu.h`'s `FTOFIX32(x)` macro returns `long`, but every caller assigns the result to an `int` before further use, so the extra width is truncated harmlessly.
- `stdarg.h`'s `(long)list` alignment macros use `long` as a pointer-sized integer; LP64 actually makes that *more* correct than LLP64.
- Every `_Static_assert(sizeof(X) == N)` struct (Bitmap, Sprite, DObjDesc, MObjSub, FTAccessPart, …) compiles clean, which means the reconciled types produce the correct byte layouts on both ABIs.

When adding new decomp sources or vendored SDK headers, re-run this sweep (`grep -rn '\blong\b' include/ src/ | grep -v 'long long'`) before assuming the new code is LP64-safe.

Other compiler-compat notes:
- `ultratypes.h` defines `u32`/`s32` as `unsigned int`/`int` under `#ifdef PORT && !defined(_MSC_VER)` (and as `unsigned long`/`long` everywhere else). This is the LP64 fix: on clang/gcc `long` is 8 bytes, which silently corrupts every file-backed N64 struct the decomp touches. MSVC (LLP64) keeps the original SDK definitions.
- `size_t` is provided in the same header via `__SIZE_TYPE__` (GCC/Clang builtin) when `_MIPS_SZLONG` isn't defined, so the decomp can use `size_t` without pulling in the system `<stddef.h>`.
- The `__attribute__((aligned(x)))` macro in `macros.h` is immediately undefined by `#define __attribute__(x)` — this is an IDO compatibility hack. The port will need to fix this for GCC/Clang/MSVC.
- `#ifdef __sgi` guards IDO-specific code paths. The port uses `__GNUC__` or `_MSC_VER` paths.
- Clang ≥16 promotes `-Wimplicit-function-declaration`, `-Wint-conversion`, `-Wincompatible-pointer-types`, `-Wreturn-mismatch`, etc. to errors by default. The decomp relies on IDO-era loose typing for all of these, so `ssb64_game` (the decomp object library) carries a bundle of `-Wno-…` overrides in `CMakeLists.txt`. Keep new decomp compile errors contained to that target — do not spread the overrides to the port layer, which should stay strict.
- `gmColCommandGotoS2` / `SubroutineS2` / `ParallelS2` narrow a 64-bit address to `u32` inside a file-scope `u32[]` initializer. That's legal on N64 (32-bit `void*`) and tolerated by MSVC, but clang/gcc C11 rejects it as a non-constant initializer. Under `PORT && !_MSC_VER` these macros expand to `((u32)0)` — color-script goto targets would need to be patched at runtime if that feature is ever re-enabled.

---

## Debug Tools — GBI Display List Trace & Diff

The `debug_tools/` directory contains tools for capturing and comparing F3DEX2/RDP display list commands between the PC port and a Mupen64Plus emulator. This enables data-driven rendering debugging without relying on visual descriptions.

### Architecture

```
Port (Fast3D interpreter)  →  debug_traces/port_trace.gbi
                                                              → gbi_diff.py → structured diff
Empen (M64P trace plugin)  →  debug_traces/emu_trace.gbi
```

Both sides produce identically-formatted trace files. The Python diff tool aligns by frame and reports divergences at the GBI command level.

### Port-Side Trace (`debug_tools/gbi_trace/`)

Captures every GBI command as Fast3D's interpreter executes it.

**Files:**
- `gbi_decoder.h` / `gbi_decoder.c` — Shared F3DEX2/RDP command decoder (pure C, used by both port and M64P plugin)
- `gbi_trace.h` / `gbi_trace.c` — Port-side trace lifecycle (frame begin/end, file I/O)

**How to enable:**
```bash
# Set env var before launching the port
SSB64_GBI_TRACE=1 ./build/Debug/ssb64.exe

# Optional: control frame limit (default 300 = 5 seconds)
SSB64_GBI_TRACE_FRAMES=60 SSB64_GBI_TRACE=1 ./build/Debug/ssb64.exe

# Optional: custom output directory
SSB64_GBI_TRACE_DIR=my_traces SSB64_GBI_TRACE=1 ./build/Debug/ssb64.exe
```

**Output:** `debug_traces/port_trace.gbi` — one file with frame delimiters.

**Trace format:**
```
=== FRAME 0 ===
[0000] d=0 G_MTX              w0=DA380007 w1=0C000000  PROJ LOAD PUSH seg=0 off=0xC000000
[0001] d=0 G_VTX              w0=01020040 w1=06001234  n=2 v0=0 addr=06001234
[0002] d=0 G_TRI1             w0=05060C12 w1=00000000  v=(3,6,9)
=== END FRAME 0 — 3 commands ===
```

### Mupen64Plus Trace Plugin (`debug_tools/m64p_trace_plugin/`)

A minimal M64P video plugin that renders nothing but captures display lists from RDRAM.

**Build (standalone):**
```bash
cd debug_tools/m64p_trace_plugin
cmake -S . -B build && cmake --build build
# Produces: mupen64plus-video-trace.dll (or .so)
```

**Usage with Mupen64Plus:**
```bash
mupen64plus --gfx /path/to/mupen64plus-video-trace.dll baserom.us.z64

# Optional: control output
M64P_TRACE_DIR=debug_traces M64P_TRACE_FRAMES=60 mupen64plus --gfx ...
```

**Output:** `emu_trace.gbi` (or `$M64P_TRACE_DIR/emu_trace.gbi`)

### Diff Tool (`debug_tools/gbi_diff/gbi_diff.py`)

Compares two `.gbi` trace files and produces structured divergence reports.

**Usage:**
```bash
# Full diff
python debug_tools/gbi_diff/gbi_diff.py debug_traces/port_trace.gbi debug_traces/emu_trace.gbi

# Single frame
python debug_tools/gbi_diff/gbi_diff.py port.gbi emu.gbi --frame 12

# Frame range with summary only
python debug_tools/gbi_diff/gbi_diff.py port.gbi emu.gbi --frame-range 0-10 --summary

# Ignore address differences (useful since port/emu use different address spaces)
python debug_tools/gbi_diff/gbi_diff.py port.gbi emu.gbi --ignore-addresses

# Write to file
python debug_tools/gbi_diff/gbi_diff.py port.gbi emu.gbi --output diff_report.txt
```

**Diff output example:**
```
========================================================================
FRAME 12: 3 divergences (port=847 cmds, emu=847 cmds, 844 matching)
========================================================================

  DIVERGENCE at cmd #203 — w1: FD100000 vs FD080000
  PORT: [0203] d=1 G_SETTIMG          w0=FD100000 w1=05004000  fmt=RGBA siz=16b w=1 seg=5 off=0x004000
  EMU : [0203] d=1 G_SETTIMG          w0=FD080000 w1=05004000  fmt=CI siz=8b w=1 seg=5 off=0x004000
```

### Interpreting Results

- **Opcode mismatch**: Game code is producing different commands — check the function that builds this part of the display list.
- **w0 mismatch (same opcode)**: Parameters differ — check render mode setup, geometry mode, texture settings.
- **w1 mismatch on non-address opcodes**: Data values differ — check resource extraction or runtime state.
- **w1 mismatch on address opcodes** (G_VTX, G_DL, G_MTX, G_SETTIMG): Expected — use `--ignore-addresses` unless investigating resource loading.
- **Missing commands**: One side has more commands — look for conditional code paths or early G_ENDDL.

---

## Known Bugs & Resolved Issues

This section documents significant bugs encountered during the port, their symptoms, root causes, and fixes. When fixing a new bug, add an entry here so future sessions can recognize the same class of issue.

### DL Normalization Guard False Positive (2026-04-07) — FIXED

**Symptoms:** Game hangs at first frame of mvOpeningRoom scene (frame 55). DX11 shader compile error `variable 'texel' used without having been completely initialized`. Appears as a hang because DX11 error path had a blocking `MessageBoxA`.

**Root cause:** `portNormalizeDisplayListPointer` had a guard (`probe[1] == 0`) to detect native 16-byte display lists within reloc file address ranges. Packed 8-byte DLs commonly start with sync commands (`gDPPipeSync`, `gDPSetFogColor(0)`, `gSPGeometryMode` with no set bits) which all have `w1=0`, causing false positives. When normalization was skipped, the interpreter read packed data at wrong stride — garbled combiner state, half of commands skipped.

**Fix:** Disabled the heuristic guard entirely (data inspection can't reliably distinguish packed vs native when many GBI commands have w1=0). Added shader compile failure resilience: `sFailedShaderIds` cache prevents infinite retry, null-program check skips broken draws, `MessageBoxA` calls removed from error paths.

**Files:** `libultraship/src/fast/interpreter.cpp`, `libultraship/src/fast/backends/gfx_direct3d11.cpp`, `libultraship/src/ship/debug/CrashHandler.cpp`, `port/gameloop.cpp`

### Display List Widening (2026-04-06) — FIXED

**Symptoms:** Unhandled opcodes (0xEC, 0x84, 0x94, 0xCD), crashes in renderer.

**Root cause:** Resource DLs in reloc files are 8 bytes/entry (N64 format) but PC interpreter expects 16 bytes/entry. Segment 0x0E addresses within packed DLs resolved against wrong base.

**Fix:** `portNormalizeDisplayListPointer` widens packed DLs to native format with segment 0x0E rewriting, opcode validation, and G_ENDDL guarantee.

**Files:** `libultraship/src/fast/interpreter.cpp`

### Segment 0x0E G_DL Resolution (2026-04-08) — FIXED

**Symptoms:** Unhandled opcodes (values vary with ASLR: 0x96, 0xC6, 0xE1, etc.) at ~5.5s during mvOpeningRoom scene. Interpreter reads game data structures and RGBA5551 pixel data as GBI commands. Game hangs (timeout kill, exit code 124).

**Root cause:** `portNormalizeDisplayListPointer` had a special exception (`isRuntimeSegRef`) that prevented resolving G_DL commands with segment 0x0E to absolute file addresses. The rationale was that these might reference the runtime graphics heap, but in practice all G_DL segment 0x0E references in resource DLs are intra-file sub-DL branches. The MObj system only changes segment 0x0E for texture references (G_SETTIMG), not display list branches. With the exception active, `0x0E000018` was resolved at runtime via `SegAddr()` using whatever segment 0x0E currently pointed to — often the graphics heap instead of the reloc file — causing the interpreter to branch into random heap data.

**Fix:** Removed the `isRuntimeSegRef` G_DL exception. All segment 0x0E references (including G_DL) in resource DLs are now resolved to `fileBase + offset` at normalization time. Also replaced `syDebugPrintf` + `while(TRUE)` in DL buffer overflow detection with visible `port_log` output under `#ifdef PORT`.

**Files:** `libultraship/src/fast/interpreter.cpp`, `port/bridge/lbreloc_bridge.cpp`, `src/sys/taskman.c`

### Particle Bank ROM DMA Segfault (2026-04-08) — FIXED

**Symptoms:** Segfault in scene 30 (nSCKindOpeningMario) during `itManagerInitItems()` → `efParticleGetLoadBankID()`. Crash handler didn't fire. No log output after "about to call func_start".

**Root cause:** `efParticleGetLoadBankID` uses `&lITManagerParticleScriptBankLo/Hi` linker symbol addresses as ROM offsets to DMA-read particle bank data. On PC: (1) symbols are stubs in `port/stubs/segment_symbols.c` (all = 0), so `&symbol` gives meaningless PC addresses; (2) `hi - lo` = 8 bytes (adjacent vars); (3) `syTaskmanMalloc(8)` allocates from the general heap which contains **stale data from the previous scene** (heap pointer resets between scenes but memory isn't zeroed); (4) `syDmaReadRom()` is a no-op so the buffer keeps stale data; (5) `lbParticleSetupBankID()` casts it as `LBScriptDesc*`, reads garbage `scripts_num` → iterates into a segfault. Scene 28 didn't crash because the static heap was zero-initialized on first use.

**Fix:** `#ifdef PORT` guard in `efParticleGetLoadBankID()` skips ROM DMA path entirely. Registers a dummy empty bank so callers get valid bank IDs. Particle emission code already has bounds checks that handle empty banks gracefully.

**Files:** `src/ef/efparticle.c`

### Ground Geometry PORT_RESOLVE + Collision Byte-Swap (2026-04-08) — FIXED

**Symptoms:** Scene 30 (nSCKindOpeningMario) segfaults on first game-loop frame in `gcSetupCustomDObjs` (R11=0xCDCDCDCDCDCDCDCD). After fixing, subsequent crashes in `mpCollisionGetMapObjPositionID` (Castle bumper lookup), `mpCollisionInitYakumonoAll` (line info iteration), and `ifCommonPlayerStockMakeStockSnap` (fighter death due to wrong death boundary).

**Root cause — PORT_RESOLVE:** `grDisplayMakeGeometryLayer()` passed raw `u32` token values from `MPGroundDesc` fields directly to API functions (`gcSetupCustomDObjs`, `gcAddMObjAll`, `gcAddAnimAll`, `grDisplayDObjSetNoAnimXObj`) that expect resolved pointers.

**Root cause — byte-swap:** Multiple all-u16 data structures in map/collision files are corrupted by the blanket `u32` byte-swap. After swap, u16 pairs within each u32 word are position-swapped. Affected: `MPGeometryData` (yakumono_count, mapobj_count), `MPLineInfo`, `MPMapObjData`, `MPVertexLinks` arrays, and `MPGroundData` camera/map bounds (s16 fields). The bounds corruption caused fighters to die immediately (wrong death boundary), crashing the uninitialized HUD stock display. Hard-coded byte offsets were wrong due to MSVC struct padding differences — using runtime pointer arithmetic (`&field - base`) to compute offsets is required.

**Fix:**
1. `grdisplay.c`: Wrapped all `MPGroundDesc` token field accesses with `PORT_RESOLVE()` + appropriate casts
2. `mpcollision.c`: Added `portFixupStructU16` calls in `mpCollisionInitGroundData` for: `MPGeometryData` u16 fields, `MPMapObjData` array, `MPLineInfo` array, `MPVertexLinks` array, and `MPGroundData` camera/map/team bounds (using runtime offsetof via pointer arithmetic). `MPVertexData` fixup deferred (needs safe vertex count).

**Files:** `src/gr/grdisplay.c`, `src/mp/mpcollision.c`

### OSMesg Union/Pointer Type Split (2026-04-11) — FIXED

**Symptoms:** On macOS/arm64, the scheduler crashed in `sySchedulerExecuteTasksAll + 0x200` with SIGBUS: `blr x8` where `x8 = curr->fnCheck` pointed to an invalid code address. `curr` itself was garbage — not a heap pointer but a stack-range address with low bits looking like `0x16f...01`. This surfaced immediately after "Boot sequence yielded — entering frame loop" as the first task dispatch. The same build worked on Windows because MSVC-x64 happened to leave the relevant stack slots zero.

**Root cause:** `OSMesg` has **two conflicting definitions** visible in the same program:
- `include/PR/os.h` (decomp): `typedef void* OSMesg;`
- `libultraship/include/libultraship/libultra/message.h`: `typedef union { u8 data8; u16 data16; u32 data32; void* ptr; } OSMesg;`

Both are 8 bytes with 8-byte alignment, so the ABI matches across translation units. But the semantics of a **C-style cast from integer to OSMesg** differ by a mile:
- In C TUs that see `void*`: `(OSMesg)INTR_VRETRACE` → well-defined int-to-pointer conversion, always zero-extends.
- In C++ TUs that see the union (e.g. `port/gameloop.cpp` via the LUS header chain): clang treats `(OSMesg)1` as a brace-init-like conversion that writes the low union member (`data8`/`data32`) and leaves the remaining bytes **uninitialised** — i.e., whatever the register/stack happened to hold. On MSVC/x64 those bytes were zero by luck; on macOS/arm64 they were a fresh stack pointer.

So `PortPushFrame()` posted `{data32 = 1, upper_bytes = <stack garbage>}` to `gSYSchedulerTaskMesgQueue`. The scheduler read it back, saw a value that was neither `INTR_VRETRACE (1)` nor any other interrupt code, and fell through to `default:` where it casts to `SYTaskInfo*`. The "task pointer" pointed into the stack, `curr->type`/`priority`/`fnCheck` were all garbage, and the first `curr->fnCheck(curr)` call jumped to an invalid address.

**Fix:** `port/gameloop.cpp` grew a `port_make_os_mesg_int(uint32_t code)` helper that `OSMesg{}`-zero-initialises a fresh union and then sets `data32`. Every integer-to-OSMesg send in the port (C++) layer should go through that helper so all 8 bytes are well-defined on every platform. C callers of `osSendMesg` keep using `(OSMesg)(intptr_t)code` because they see the `void*` typedef.

**Why it also matters on Windows:** It doesn't crash today but the same UB is present — any future code change that shifts the stack layout could unmask it. The helper makes it deterministic.

**Files:**
- `port/gameloop.cpp` — `port_make_os_mesg_int()` + call site in `PortPushFrame()`.

**Diagnostic that cracked it:** `port_log` inside the scheduler's task loop printed `task=0x16f...2d01 type=24103981 fnCheck=0x10000000...` — obvious "stack pointer OR'd with an int". Then logging `osSendMesg`/`osRecvMesg` on the task queue showed the queue was round-tripping that same value, meaning the sender was writing it. Adding a dedicated send debug print at `PortPushFrame` confirmed the C-style cast was producing garbage upper bytes.

### bzero Infinite Recursion on macOS/arm64 (2026-04-11) — FIXED

**Symptoms:** On macOS Apple Silicon the port segfaulted inside `std::make_shared<Fast::Fast3dWindow>()` → `Interpreter::Interpreter()` → first `new RSP()`. The crash report showed dozens of stack frames of `bzero → bzero → bzero → …` and terminated with `EXC_BAD_ACCESS: Could not determine thread index for stack guard region` (i.e. thread stack overflow).

**Root cause:** `port/stubs/libc_compat.c` previously provided `void bzero(void *p, unsigned int n) { memset(p, 0, n); }`. Two problems:
1. The final macOS binary exported `_bzero`, shadowing libSystem's `bzero` for every module that dynamically resolved it.
2. Clang lowers `memset(ptr, 0, len)` back to a `bzero()` call as a peephole optimization. With the shadowing stub in place, `bzero → memset → bzero → memset → …` recursed forever. The value-initialized `new RSP()` (which the compiler implements via zero-fill memset) was the first caller large enough to trip the stack guard.

**Fix:** Delete the bzero stub entirely — every libc we target (glibc, musl, libSystem, MSVC's BSD compatibility shims) already provides `bzero`, so there is nothing to emulate. Decomp call sites that use `bzero` resolve to the platform version via `<PR/os.h>` as they did before.

**Files:** `port/stubs/libc_compat.c`.

### Sprite Texel Byteswap + TMEM Line Swizzle (2026-04-10) — FIXED

**Symptoms:** All sprite-rendered content (N64 logo, HUD, menus) had garbled textures on the port — colors were roughly right but the image had a sheared/zigzag/sawtooth pattern. Letters were unreadable. The 3D N64 logo's diagonals stairstepped wrong. Looked like a Fast3D rendering bug or texture filter issue.

**Diagnostic path:**
1. Captured GBI traces from both port and Mupen64Plus (via the `mupen64plus-rsp-trace` plugin) and diffed with `debug_tools/gbi_diff/gbi_diff.py`. Across the entire N64 logo scene (54 frames) the port emitted **byte-perfect identical** GBI commands (0 opcode/w0/w1 mismatches with `--ignore-addresses`). That ruled out game code's display list construction.
2. Logged the actual texel bytes for `bitmap[0].buf` at runtime in `lbCommonMakeSObjForGObj` and compared against the raw .o2r file. After the first BSWAP32 fix the bytes matched the file but the rendered image was still wrong.
3. Forced point sampling and 4x internal resolution — neither helped, ruling out filter/upscale.
4. Found a real but unrelated DX11 filter-default bug (`gfx_direct3d_common.h` defaulted `mCurrentFilterMode = FILTER_NONE` so `linear_filter && mCurrentFilterMode == FILTER_LINEAR` was always false → DX11 always point-sampled regardless of game request). Fixed independently by changing the default to `FILTER_LINEAR`.
5. Compared mupen64plus screenshot (with glide64mk2) — clean and crisp — with the port. The port matched the .o2r contents exactly, but the .o2r contents themselves were wrong relative to mupen.
6. Verified Torch's vpk0 decompression by comparing ROM-decompressed bytes vs .o2r contents — perfect match. So Torch wasn't corrupting data; the issue was data interpretation.
7. Extracted the texture as a Python PNG and tested several swap patterns. Found that **XOR4 on odd rows (strip-local)** produces a clean, mupen-matching image: `debug_traces/logo_xor4_strip_local.png`.

**Root causes (two stacked issues):**

1. **Pass2 doesn't see sprite textures.** `portRelocByteSwapBlob` pass2 finds in-file textures by walking stored display lists for `G_SETTIMG` → `G_LOADBLOCK` pairs. Sprite files (e.g. `N64Logo`) build their LOAD blocks at **runtime** in `lbCommonDrawSObjBitmap` from `bitmap.buf` — those addresses never appear in any stored DL. Pass2 misses the texel data and it's left in pass1's u32-byteswapped state. Fast3D's `ImportTextureRgba16` reads texels as N64 BE u16 (`(addr[0] << 8) | addr[1]`), so it needs the bytes in original BE order — another BSWAP32 restores them.

2. **N64 RDP TMEM line swizzle.** Even with bytes in correct BE order, sprite textures still rendered with a shear because the N64 stores RGBA16 textures in DRAM **pre-swizzled** to avoid TMEM bank conflicts when sampled. The hardware XORs the byte address based on row parity: for 16bpp/IA/CI, odd rows have the byte address XOR'd with 0x4 (i.e., the two 4-byte halves of each 8-byte qword are swapped). `LOAD_BLOCK` with `dxt=0` loads the swizzled data into TMEM as-is; the sampler unscrambles it during reads. Fast3D doesn't emulate TMEM addressing, so it would render the swizzled data linearly and produce a sheared image.

**Fix:** Added `portFixupSpriteBitmapData(sprite, bitmaps)` in `port/bridge/lbreloc_byteswap.cpp`. Walks the bitmap array, resolves each `bm->buf` via `portRelocResolvePointer`, then per bitmap:
1. Apply BSWAP32 over `width_img × actualHeight × bpp / 8` bytes to restore N64 BE byte order.
2. For 16bpp textures, apply the inverse TMEM line swizzle: on every odd row of the bitmap, swap the two 4-byte halves within each 8-byte qword. Strip-local row indexing (each LOAD_BLOCK is independent in TMEM).

Idempotent via the existing `sStructU16Fixups` tracking set. Called from `lbCommonMakeSObjForGObj` right after `portFixupBitmapArray`. Sprite struct field offsets read directly with hard-coded byte offsets — `bmsiz` is at `0x31` (not `0x32`): the C struct packs `u8 bmfmt; u8 bmsiz;` followed by 2 bytes of padding before the next u32, and the field offset must match the C compiler's natural placement.

**Files:**
- `port/bridge/lbreloc_byteswap.cpp` — `portFixupSpriteBitmapData`
- `port/bridge/lbreloc_byteswap.h` — declaration + doc
- `src/lb/lbcommon.c` — call site in `lbCommonMakeSObjForGObj`
- `libultraship/include/fast/backends/gfx_direct3d_common.h` — separate DX11 filter-default fix (`FILTER_NONE` → `FILTER_LINEAR`); was hiding the bug because every sample was point-sampled regardless of game request.

**Latent issues not yet addressed:**
- `apply_fixup_tex_u16` in pass2 uses `rotate16` for in-file 16bpp textures and palettes, which is wrong for the same reason — Fast3D reads textures as BE u16. Most game textures are CI4/CI8 (which use `apply_fixup_tex_bytes` = bswap32 = correct) or 32bpp, so it hasn't surfaced. If any non-sprite RGBA16 textures or LUT palettes show shear, that's the same bug class.
- The TMEM line swizzle applies to **all** 16bpp textures the RDP samples, not just sprites. If other rendering paths load 16bpp via `LOAD_BLOCK` with `dxt=0` and aren't sprite-routed, they'll need the same unswizzle.

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
