#!/usr/bin/env bash
# capture_and_diff.sh — glue script for capturing a port-side GBI trace and
# diffing it against an emulator-side reference trace.
#
# Usage:
#   capture_and_diff.sh <label> [seconds]
#
# Examples:
#   capture_and_diff.sh intro_first 10
#   capture_and_diff.sh intro_second 10 && diff intro_first vs intro_second
#
# What it does:
#   1. Runs the port for <seconds> with SSB64_GBI_TRACE=1, writes
#      debug_traces/<label>_port.gbi
#   2. Looks for an emulator-side reference at debug_traces/<label>_emu.gbi
#      (or the generic debug_traces/emu_trace.gbi). You produce that file
#      either with mupen64plus + our RSP plugin (see below) or via RMG.
#   3. Runs gbi_diff.py and writes debug_traces/<label>_diff.txt
#
# Generating the emulator-side reference (mupen64plus, on this Mac):
#
#   PLUGIN_DIR=<repo>/debug_tools/m64p_trace_plugin/build
#   M64P_TRACE_DIR=debug_traces M64P_TRACE_FRAMES=600 \
#     mupen64plus --noosd \
#       --rsp   $PLUGIN_DIR/mupen64plus-rsp-trace.dylib \
#       --gfx   /opt/homebrew/Cellar/mupen64plus/2.6.0/lib/mupen64plus/mupen64plus-video-rice.dylib \
#       --audio dummy --input dummy \
#       baserom.us.z64
#
# Generating the emulator-side reference (RMG, the issue-71 reference combo):
#   - Install our plugin into the RMG plugin dir (see docs/debug_gbi_trace.md)
#   - In RMG settings: RSP=GBI+Acmd Trace RSP Plugin, GFX=GLideN64,
#     Audio=any, Input=any, CPU=Pure Interpreter (in core settings)
#   - Set M64P_TRACE_DIR=<absolute path to debug_traces> in RMG's environment
#     before launching, then start the ROM. emu_trace.gbi appears in that dir.
#   - Move/rename it to debug_traces/<label>_emu.gbi.

set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "usage: $0 <label> [seconds]" >&2
    exit 2
fi

LABEL="$1"
SECONDS_TO_RUN="${2:-10}"

# Resolve repo root: this script lives at <repo>/debug_tools/gbi_diff/.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

TRACE_DIR="$REPO_ROOT/debug_traces"
mkdir -p "$TRACE_DIR"

PORT_TRACE="$TRACE_DIR/${LABEL}_port.gbi"
EMU_TRACE="$TRACE_DIR/${LABEL}_emu.gbi"
DIFF_OUT="$TRACE_DIR/${LABEL}_diff.txt"

# ----- Step 1: port-side capture -----
PORT_BIN=""
for cand in \
    "$REPO_ROOT/build/Debug/ssb64" \
    "$REPO_ROOT/build/ssb64" \
    "$REPO_ROOT/build/Release/ssb64"
do
    if [[ -x "$cand" ]]; then PORT_BIN="$cand"; break; fi
done

if [[ -z "$PORT_BIN" ]]; then
    echo "[capture_and_diff] WARNING: no ssb64 binary found under $REPO_ROOT/build/" >&2
    echo "  Skipping port capture. Build first: cmake --build $REPO_ROOT/build --target ssb64 -j 4" >&2
else
    echo "[capture_and_diff] capturing port trace -> $PORT_TRACE (${SECONDS_TO_RUN}s)"
    rm -f "$PORT_TRACE" "$TRACE_DIR/port_trace.gbi"
    (
        cd "$(dirname "$PORT_BIN")"
        SSB64_GBI_TRACE=1 \
        SSB64_GBI_TRACE_FRAMES=$((SECONDS_TO_RUN * 60)) \
        SSB64_GBI_TRACE_DIR="$TRACE_DIR" \
            "$PORT_BIN" >"$TRACE_DIR/${LABEL}_port.log" 2>&1 &
        PORT_PID=$!
        sleep "$SECONDS_TO_RUN"
        kill -TERM "$PORT_PID" 2>/dev/null || true
        sleep 1
        kill -KILL "$PORT_PID" 2>/dev/null || true
        wait "$PORT_PID" 2>/dev/null || true
    )
    # Port writes to debug_traces/port_trace.gbi by convention; rename per label.
    if [[ -f "$TRACE_DIR/port_trace.gbi" ]]; then
        mv "$TRACE_DIR/port_trace.gbi" "$PORT_TRACE"
    fi
fi

# ----- Step 2: emulator-side trace (manual) -----
if [[ ! -f "$EMU_TRACE" ]]; then
    if [[ -f "$TRACE_DIR/emu_trace.gbi" ]]; then
        echo "[capture_and_diff] reusing $TRACE_DIR/emu_trace.gbi as $EMU_TRACE"
        cp "$TRACE_DIR/emu_trace.gbi" "$EMU_TRACE"
    else
        echo "[capture_and_diff] no emulator trace found at $EMU_TRACE" >&2
        echo "  Generate one with mupen64plus or RMG (see header of this script)." >&2
        exit 0
    fi
fi

# ----- Step 3: diff -----
if [[ ! -f "$PORT_TRACE" ]]; then
    echo "[capture_and_diff] no port trace produced — skipping diff" >&2
    exit 0
fi

echo "[capture_and_diff] diffing -> $DIFF_OUT"
python3 "$REPO_ROOT/debug_tools/gbi_diff/gbi_diff.py" \
    "$PORT_TRACE" "$EMU_TRACE" \
    --ignore-addresses \
    --output "$DIFF_OUT" || true

# Print short summary
echo "----- summary -----"
grep -E "^(FRAME|Total|Matching|Diverg)" "$DIFF_OUT" | head -40 || true
echo "Full report: $DIFF_OUT"
