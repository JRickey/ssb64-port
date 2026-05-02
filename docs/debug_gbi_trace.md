# Debug Tools — GBI Display List Trace & Diff

The `debug_tools/` directory contains tools for capturing and comparing F3DEX2/RDP display list commands between the PC port and a Mupen64Plus emulator. This enables data-driven rendering debugging without relying on visual descriptions.

## Architecture

```
Port (Fast3D interpreter)  →  debug_traces/port_trace.gbi
                                                              → gbi_diff.py → structured diff
Empen (M64P trace plugin)  →  debug_traces/emu_trace.gbi
```

Both sides produce identically-formatted trace files. The Python diff tool aligns by frame and reports divergences at the GBI command level.

## Port-Side Trace (`debug_tools/gbi_trace/`)

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

## Mupen64Plus Trace Plugin (`debug_tools/m64p_trace_plugin/`)

The plugin is implemented as an **RSP plugin** (not a video plugin): it intercepts `DoRspCycles`, walks the display list / audio command list out of RDRAM, then forwards `ProcessDList` / `ProcessAList` to a real video/audio plugin so the game keeps running. A no-op stub video plugin is also built for cases where you don't want any rendering.

**Build (standalone):**
```bash
cd debug_tools/m64p_trace_plugin
cmake -S . -B build && cmake --build build -j 4
# Produces (extension is platform-specific):
#   mupen64plus-rsp-trace.{dylib,so,dll}    <- the actual capture plugin
#   mupen64plus-video-trace.{dylib,so,dll}  <- no-op video stub
```

**Usage with stock Mupen64Plus (smoke-test on macOS, homebrew):**
```bash
PLUGIN_DIR=debug_tools/m64p_trace_plugin/build
M64P_TRACE_DIR=debug_traces M64P_TRACE_FRAMES=600 \
  mupen64plus --noosd \
    --rsp   $PLUGIN_DIR/mupen64plus-rsp-trace.dylib \
    --gfx   /opt/homebrew/Cellar/mupen64plus/2.6.0/lib/mupen64plus/mupen64plus-video-rice.dylib \
    --audio dummy --input dummy \
    baserom.us.z64
```

**Env vars:**
- `M64P_TRACE_DIR=path` — output directory (default `.`)
- `M64P_TRACE_FRAMES=N` — max GFX frames (`0` = unlimited; default 300)
- `M64P_ACMD_TRACE=1` — also capture audio command lists
- `M64P_ACMD_TRACE_TASKS=N` — max audio tasks (default 600)

**Output:** `emu_trace.gbi` (and optionally `emu_acmd_trace.acmd`).

### Using the plugin under Rosalie's Mupen GUI (RMG)

RMG bundles its own M64P core, but it loads RSP plugins from a per-install `Plugin/` tree, so our `.dylib`/`.so`/`.dll` is drop-in. The reference plugin combo from issues #71/#72/#73 is:

| Slot     | Plugin                            |
|----------|-----------------------------------|
| RSP      | **GBI + Acmd Trace RSP Plugin** (ours) |
| Video    | GLideN64                          |
| Audio    | any (SDL is fine)                 |
| Input    | any                               |
| CPU core | **Pure Interpreter**              |

**Plugin drop-in path:**
- **Linux portable / Windows / AppImage:** `<RMG-install>/Bin/<Config>/Plugin/RSP/`
- **Linux system install:** `${CMAKE_INSTALL_LIBDIR}/RMG/Plugin/RSP/` (typically `/usr/lib/RMG/Plugin/RSP/` or `/usr/local/lib/RMG/Plugin/RSP/`)
- **macOS:** RMG has no dedicated macOS install logic; it uses the Linux system layout. Inside the .app bundle this typically resolves to `RMG.app/Contents/Resources/Plugin/RSP/` (verify via `--plugin-path` override below).
- **Override:** RMG accepts a `--plugin-path <dir>` CLI flag (see `Source/RMG/main.cpp`) which switches to a custom plugin tree containing `RSP/`, `GFX/`, `Audio/`, `Input/` subdirs.

**Settings to set in RMG:**
- *Settings → Plugins*: pick "GBI + Acmd Trace RSP Plugin" for RSP, GLideN64 for Video.
- *Settings → Core*: set CPU emulator to **Pure Interpreter** (matches the reference traces in issues #71–#73; required because dynarec reorders RSP-trigger sites and drifts cmd indices).
- Set `M64P_TRACE_DIR=<absolute path>` in RMG's environment (`env M64P_TRACE_DIR=... rmg ...` on Linux/macOS) before launch — RMG itself doesn't expose env vars in its UI. The plugin will write `emu_trace.gbi` there.

## Capture-and-diff harness (`debug_tools/gbi_diff/capture_and_diff.sh`)

Glue script that runs the port for N seconds with `SSB64_GBI_TRACE=1`, then diffs against an emulator reference. See the script header for full usage; quick form:

```bash
debug_tools/gbi_diff/capture_and_diff.sh intro_first 10
# emits debug_traces/intro_first_port.gbi  +  intro_first_diff.txt
# expects an emulator trace at debug_traces/intro_first_emu.gbi
# (or falls back to debug_traces/emu_trace.gbi from a prior mupen64plus run)
```

## Diff Tool (`debug_tools/gbi_diff/gbi_diff.py`)

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

## Interpreting Results

- **Opcode mismatch**: Game code is producing different commands — check the function that builds this part of the display list.
- **w0 mismatch (same opcode)**: Parameters differ — check render mode setup, geometry mode, texture settings.
- **w1 mismatch on non-address opcodes**: Data values differ — check resource extraction or runtime state.
- **w1 mismatch on address opcodes** (G_VTX, G_DL, G_MTX, G_SETTIMG): Expected — use `--ignore-addresses` unless investigating resource loading.
- **Missing commands**: One side has more commands — look for conditional code paths or early G_ENDDL.
