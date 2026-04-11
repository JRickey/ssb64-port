#!/usr/bin/env bash
# build.sh - Build SSB64 port and extract assets (macOS / Linux)
#
# Usage:
#   ./build.sh                 # Full build + extract
#   ./build.sh --skip-extract  # Build only, skip asset extraction
#   ./build.sh --extract-only  # Extract assets only (assumes Torch already built)
#   ./build.sh --clean         # Clean build from scratch
#   ./build.sh --release       # Release config (default: Debug)
#
# Environment:
#   CMAKE_GENERATOR            # Override generator (default: Ninja if available, else Unix Makefiles)
#   JOBS                       # Parallel job count (default: sysctl/nproc)

set -euo pipefail

SKIP_EXTRACT=0
EXTRACT_ONLY=0
CLEAN=0
CONFIG="Debug"

for arg in "$@"; do
    case "$arg" in
        --skip-extract) SKIP_EXTRACT=1 ;;
        --extract-only) EXTRACT_ONLY=1 ;;
        --clean)        CLEAN=1 ;;
        --release)      CONFIG="Release" ;;
        --debug)        CONFIG="Debug" ;;
        -h|--help)
            sed -n '2,13p' "$0"
            exit 0
            ;;
        *)
            echo "Unknown argument: $arg" >&2
            exit 1
            ;;
    esac
done

ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT/build"
ROM="$ROOT/baserom.us.z64"
O2R="$ROOT/ssb64.o2r"
F3D_O2R="$ROOT/f3d.o2r"
FAST3D_SHADER_DIR="$ROOT/libultraship/src/fast/shaders"
GAME_EXE="$BUILD_DIR/ssb64"

if [[ -z "${JOBS:-}" ]]; then
    if command -v sysctl >/dev/null 2>&1; then
        JOBS="$(sysctl -n hw.ncpu 2>/dev/null || echo 4)"
    elif command -v nproc >/dev/null 2>&1; then
        JOBS="$(nproc)"
    else
        JOBS=4
    fi
fi

if [[ -z "${CMAKE_GENERATOR:-}" ]]; then
    if command -v ninja >/dev/null 2>&1; then
        CMAKE_GENERATOR="Ninja"
    else
        CMAKE_GENERATOR="Unix Makefiles"
    fi
fi

step() {
    printf '\n\033[36m=== %s ===\033[0m\n' "$1"
}

fail() {
    printf '\033[31mERROR: %s\033[0m\n' "$1" >&2
    exit 1
}

# ── Clean ──
if [[ $CLEAN -eq 1 ]]; then
    step "Cleaning build directory"
    rm -rf "$BUILD_DIR"
    rm -f "$O2R" "$F3D_O2R"
fi

# ── Validate ROM ──
if [[ ! -f "$ROM" ]]; then
    fail "ROM not found at $ROM
Place your NTSC-U v1.0 ROM as baserom.us.z64 in the project root."
fi

# ── Submodules ──
if [[ $EXTRACT_ONLY -eq 0 ]]; then
    step "Initializing submodules"
    git -C "$ROOT" submodule update --init --recursive
fi

# ── Generate reloc_data.h ──
# `include/reloc_data.h` is gitignored — rebuild it from the vendored
# symbols table before configuring CMake so every compiler sees the
# real file_id / offset values.
if [[ $EXTRACT_ONLY -eq 0 ]]; then
    step "Regenerating include/reloc_data.h"
    python3 "$ROOT/tools/generate_reloc_stubs.py"
fi

# ── Encode credits text ──
# The staff/titles/info/companies credit strings are included directly
# into scstaffroll.c via `#include "credits/<name>.credits.encoded"` /
# `.metadata`. Those generated artefacts are gitignored, so every fresh
# checkout needs to re-run tools/creditsTextConverter.py. Info and
# companies use the paragraph font (digits, punctuation, accents);
# staff and titles use the default title font. Idempotent — the tool
# overwrites the output files unconditionally.
if [[ $EXTRACT_ONLY -eq 0 ]]; then
    step "Encoding credits text"
    (
        cd "$ROOT/src/credits"
        for f in staff.credits.us.txt titles.credits.us.txt; do
            python3 "$ROOT/tools/creditsTextConverter.py" "$f" > /dev/null
        done
        for f in info.credits.us.txt companies.credits.us.txt; do
            python3 "$ROOT/tools/creditsTextConverter.py" -paragraphFont "$f" > /dev/null
        done
    )
fi

# ── CMake configure ──
if [[ $EXTRACT_ONLY -eq 0 ]]; then
    step "Configuring CMake ($CMAKE_GENERATOR, $CONFIG)"
    cmake -S "$ROOT" -B "$BUILD_DIR" \
        -G "$CMAKE_GENERATOR" \
        -DCMAKE_BUILD_TYPE="$CONFIG"
fi

# ── Build game ──
if [[ $EXTRACT_ONLY -eq 0 ]]; then
    step "Building ssb64"
    cmake --build "$BUILD_DIR" --target ssb64 --config "$CONFIG" -j "$JOBS"
    printf '\033[32mGame built: %s\033[0m\n' "$GAME_EXE"
fi

# ── Build Torch + Extract assets ──
if [[ $SKIP_EXTRACT -eq 0 ]]; then
    step "Building Torch"
    cmake --build "$BUILD_DIR" --target TorchExternal --config "$CONFIG" -j "$JOBS"

    # Try several known locations for the torch binary:
    #   1. Installed prefix (set by CMake's ExternalProject install step)
    #   2. Build dir directly (single-config generators)
    TORCH_EXE=""
    for candidate in \
        "$BUILD_DIR/torch-install/bin/torch" \
        "$BUILD_DIR/TorchExternal/src/TorchExternal-build/torch" \
        "$BUILD_DIR/TorchExternal/src/TorchExternal-build/$CONFIG/torch"; do
        if [[ -x "$candidate" ]]; then
            TORCH_EXE="$candidate"
            break
        fi
    done

    if [[ -z "$TORCH_EXE" ]]; then
        fail "torch binary not found after build (checked torch-install and TorchExternal build dirs)"
    fi

    step "Extracting assets from ROM"
    ( cd "$ROOT" && "$TORCH_EXE" o2r "$ROM" )

    if [[ ! -f "$O2R" ]]; then
        fail "ssb64.o2r was not created"
    fi

    O2R_SIZE_MB="$(awk "BEGIN {printf \"%.1f\", $(stat -f%z "$O2R" 2>/dev/null || stat -c%s "$O2R") / 1048576}")"
    printf '\033[32mAssets extracted: ssb64.o2r (%s MB)\033[0m\n' "$O2R_SIZE_MB"
fi

# ── Package Fast3D shader archive ──
step "Packaging Fast3D shader archive"
if [[ ! -d "$FAST3D_SHADER_DIR" ]]; then
    fail "Fast3D shader directory not found at $FAST3D_SHADER_DIR"
fi

rm -f "$F3D_O2R"

# Zip with forward-slash paths (zip_name_locate requires exact forward-slash match).
# Running `zip` from libultraship/src/fast with arg `shaders` produces entries
# prefixed "shaders/..." which is exactly what the resource manager expects.
( cd "$ROOT/libultraship/src/fast" && zip -rq "$F3D_O2R" shaders )

if [[ ! -f "$F3D_O2R" ]]; then
    fail "f3d.o2r was not created"
fi

F3D_SIZE_KB="$(awk "BEGIN {printf \"%.1f\", $(stat -f%z "$F3D_O2R" 2>/dev/null || stat -c%s "$F3D_O2R") / 1024}")"
printf '\033[32mPackaged f3d.o2r (%s KB)\033[0m\n' "$F3D_SIZE_KB"

# ── Copy assets next to the executable ──
# On single-config generators (Make/Ninja), the binary lives at $BUILD_DIR/ssb64.
# On multi-config generators (Xcode), it lives at $BUILD_DIR/$CONFIG/ssb64.
EXE_DIRS=()
for cand in "$BUILD_DIR" "$BUILD_DIR/$CONFIG"; do
    if [[ -x "$cand/ssb64" ]]; then
        EXE_DIRS+=("$cand")
    fi
done

for d in "${EXE_DIRS[@]:-}"; do
    [[ -z "$d" ]] && continue
    [[ -f "$O2R" ]] && cp -f "$O2R" "$d/ssb64.o2r"
    [[ -f "$F3D_O2R" ]] && cp -f "$F3D_O2R" "$d/f3d.o2r"
    printf 'Copied assets to %s\n' "$d"
done

# ── Done ──
step "Build complete"
[[ -f "${EXE_DIRS[0]:-$BUILD_DIR}/ssb64" ]] && printf '  Executable: %s/ssb64\n' "${EXE_DIRS[0]:-$BUILD_DIR}"
[[ -f "$O2R" ]] && printf '  Assets:     %s\n' "$O2R"
[[ -f "$F3D_O2R" ]] && printf '  Fast3D:     %s\n' "$F3D_O2R"
printf '\n'
