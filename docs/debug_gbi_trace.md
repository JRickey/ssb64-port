# Debug Tools — GBI Display List Trace & Diff

The `debug_tools/` directory contains tools for capturing F3DEX2/RDP display list commands from the PC port and diffing them against a reference trace. This enables data-driven rendering debugging without relying on visual descriptions.

## Architecture

```
Port (Fast3D interpreter)  →  debug_traces/port_trace.gbi
                                                              → gbi_diff.py → structured diff
Reference trace            →  debug_traces/emu_trace.gbi
```

Both sides produce identically-formatted trace files. The Python diff tool aligns by frame and reports divergences at the GBI command level. Reference traces previously came from an in-tree Mupen64Plus RSP plugin; that plugin was removed in the licensing-compliance pass (it was derived from GPLv2 mupen64plus headers and is incompatible with this repository's MIT scope). Capture references from a separately-licensed external tool if needed.

## Port-Side Trace (`debug_tools/gbi_trace/`)

Captures every GBI command as Fast3D's interpreter executes it.

**Files:**
- `gbi_decoder.h` / `gbi_decoder.c` — Shared F3DEX2/RDP command decoder (pure C)
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

## Reference traces

The in-tree Mupen64Plus RSP trace plugin (`debug_tools/m64p_trace_plugin/`) and audio-dump plugin (`debug_tools/m64p_audio_dump_plugin/`) were removed in the licensing-compliance pass: they were derived from GPLv2 mupen64plus headers and could not live under this repository's MIT scope. If you need an emulator-side reference trace at `debug_traces/emu_trace.gbi` (matching the format produced by `debug_tools/gbi_trace/`), build a separately-licensed capture tool out of tree.

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
