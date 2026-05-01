#!/usr/bin/env bash
# Builds BattleShip as a Linux AppImage.
#
# Output: <repo-root>/dist/BattleShip-x86_64.AppImage
#
# AppDir layout produced (before appimagetool packs it):
#   AppDir/
#     usr/bin/BattleShip                 — main executable
#     usr/bin/torch                      — sidecar for first-run extraction
#     usr/share/BattleShip/f3d.o2r       — Fast3D shaders (ROM-independent)
#     usr/share/BattleShip/config.yml    — Torch extraction config
#     usr/share/BattleShip/yamls/us/*.yml — Torch extraction recipes
#     usr/share/BattleShip/gamecontrollerdb.txt — SDL controller mappings
#     BattleShip.desktop                 — XDG desktop entry
#     AppRun                             — entry-point shim
#     BattleShip.png                     — application icon (256x256)
#
# Built with NON_PORTABLE=ON so saves and config land in
# $XDG_DATA_HOME/BattleShip/ (or ~/.local/share/BattleShip/) instead of cwd.
# BattleShip.o2r is NOT bundled — extracted on first launch via the ImGui
# wizard from the user's ROM into the app-data dir.
#
# Requires:
#   appimagetool — packs the AppDir into a runnable AppImage.
#     https://github.com/AppImage/appimagetool/releases
#   linuxdeploy  — walks the binary's NEEDED .so list and copies the
#     actual library files into AppDir/usr/lib/ so the AppImage runs
#     on distros that don't have the build host's exact .so versions.
#     https://github.com/linuxdeploy/linuxdeploy/releases
#
# If linuxdeploy isn't in PATH the script warns and skips library
# bundling — the resulting AppImage will only run on systems whose
# .so versions match the build host. If appimagetool isn't available,
# the script still produces the AppDir under dist/ for manual packing.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build-bundle-linux"
DIST_DIR="$ROOT/dist"
APPDIR="$DIST_DIR/BattleShip.AppDir"
APP_NAME="BattleShip"
APPIMAGE="$DIST_DIR/${APP_NAME}-x86_64.AppImage"
JOBS="${JOBS:-$(nproc 2>/dev/null || echo 4)}"

step() { printf '\n\033[36m=== %s ===\033[0m\n' "$1"; }
fail() { printf '\033[31mERROR: %s\033[0m\n' "$1" >&2; exit 1; }

[[ "$(uname -s)" == "Linux" ]] || fail "package-linux.sh runs on Linux only"

# ── 0. Run codegen scripts that don't need the ROM ──
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

# ── 1. Configure + build with NON_PORTABLE=ON ──
step "Configuring release build with NON_PORTABLE=ON"
cmake -B "$BUILD_DIR" "$ROOT" \
    -DCMAKE_BUILD_TYPE=Release \
    -DNON_PORTABLE=ON \
    >/dev/null

step "Building BattleShip + torch"
cmake --build "$BUILD_DIR" -j"$JOBS"

# ── 2. Build f3d.o2r (zip of LUS shaders) ──
step "Packaging Fast3D shader archive"
F3D_O2R="$BUILD_DIR/f3d.o2r"
rm -f "$F3D_O2R"
( cd "$ROOT/libultraship/src/fast" && zip -rq "$F3D_O2R" shaders )
[[ -f "$F3D_O2R" ]] || fail "f3d.o2r was not created"

# ── 3. Locate built artifacts ──
GAME_BIN="$BUILD_DIR/BattleShip"
TORCH_BIN="$BUILD_DIR/TorchExternal/src/TorchExternal-build/torch"
[[ -x "$GAME_BIN" ]] || fail "BattleShip binary not found at $GAME_BIN"
[[ -x "$TORCH_BIN" ]] || fail "torch binary not found at $TORCH_BIN"

# ── 4. Assemble AppDir ──
step "Assembling $APPDIR"
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin" "$APPDIR/usr/share/$APP_NAME/yamls/us"

cp "$GAME_BIN"   "$APPDIR/usr/bin/$APP_NAME"
cp "$TORCH_BIN"  "$APPDIR/usr/bin/torch"
cp "$F3D_O2R"    "$APPDIR/usr/share/$APP_NAME/f3d.o2r"
cp "$ROOT/gamecontrollerdb.txt" "$APPDIR/usr/share/$APP_NAME/gamecontrollerdb.txt"
cp "$ROOT/config.yml" "$APPDIR/usr/share/$APP_NAME/config.yml"
cp "$ROOT/yamls/us/"*.yml "$APPDIR/usr/share/$APP_NAME/yamls/us/"

# Bundle the ESC menu fonts. Menu.cpp::FindMenuAssetPath walks up from
# RealAppBundlePath() (= /proc/self/exe parent = AppDir/usr/bin inside
# the AppImage) and from current_path() (= AppDir/usr/share/BattleShip
# after AppRun's cd). Fonts placed under the cwd-rooted walker hit on
# the first iteration. Without this the menu falls back to ImGui's
# default font silently.
mkdir -p "$APPDIR/usr/share/$APP_NAME/assets/custom/fonts"
cp "$ROOT/assets/custom/fonts/Montserrat-Regular.ttf"  "$APPDIR/usr/share/$APP_NAME/assets/custom/fonts/"
cp "$ROOT/assets/custom/fonts/Inconsolata-Regular.ttf" "$APPDIR/usr/share/$APP_NAME/assets/custom/fonts/"

# ── 5. .desktop + icon (AppRun written after linuxdeploy) ──
# linuxdeploy reads .desktop + icon from the AppDir, so they have to
# exist before we invoke it. AppRun is written *after* linuxdeploy
# runs because linuxdeploy will overwrite whatever AppRun is there
# with its own wrapper; we want our cd-to-data-dir + LD_LIBRARY_PATH
# version to be the final one.
cat > "$APPDIR/$APP_NAME.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=BattleShip
Exec=$APP_NAME
Icon=$APP_NAME
Categories=Game;ArcadeGame;
Terminal=false
EOF

# Application icon. AppImage looks for <Icon>.png at the AppDir root and
# (for hicolor integration) under usr/share/icons/hicolor/<size>/apps/.
# Source is assets/icon.png; we downscale a 256x256 copy for the AppDir
# root (kept small to keep the AppImage lean) and ship the full-resolution
# PNG in the hicolor 512x512 slot.
ICON_SRC="$ROOT/assets/icon.png"
[[ -f "$ICON_SRC" ]] || fail "missing assets/icon.png"
mkdir -p "$APPDIR/usr/share/icons/hicolor/512x512/apps" \
         "$APPDIR/usr/share/icons/hicolor/256x256/apps"
ICON_ROOT="$APPDIR/$APP_NAME.png"
ICON_HI256="$APPDIR/usr/share/icons/hicolor/256x256/apps/$APP_NAME.png"
ICON_HI512="$APPDIR/usr/share/icons/hicolor/512x512/apps/$APP_NAME.png"
if command -v magick >/dev/null 2>&1; then
    magick "$ICON_SRC" -resize 256x256 "$ICON_ROOT"
    magick "$ICON_SRC" -resize 256x256 "$ICON_HI256"
elif command -v convert >/dev/null 2>&1; then
    convert "$ICON_SRC" -resize 256x256 "$ICON_ROOT"
    convert "$ICON_SRC" -resize 256x256 "$ICON_HI256"
elif python3 -c "import PIL" 2>/dev/null; then
    python3 - "$ICON_SRC" "$ICON_ROOT" "$ICON_HI256" <<'PY'
import sys
from PIL import Image
src, *outs = sys.argv[1:]
img = Image.open(src).convert("RGBA").resize((256, 256), Image.LANCZOS)
for o in outs:
    img.save(o)
PY
else
    cp "$ICON_SRC" "$ICON_ROOT"
    cp "$ICON_SRC" "$ICON_HI256"
fi
cp "$ICON_SRC" "$ICON_HI512"

# ── 6. Bundle .so dependencies via linuxdeploy ──
# linuxdeploy populates AppDir/usr/lib/ with the binary's NEEDED libs
# (minus glibc / libGL / other system-driver libs on its excludelist).
# Without this, the AppImage links dynamically against whatever .so
# versions happen to be on the user's distro and fails on anything
# but the exact build host's distro+version.
#
# linuxdeploy ships patchelf 0.15 + a binutils-2.34-era strip. Both predate
# DT_RELR (compact relative relocations, glibc 2.36+). On hosts that link with
# DT_RELR — Fedora 38+, Ubuntu 23.10+, Arch, Steam Deck Holo — bundling host
# libs with those tools corrupts .relr.dyn: patchelf 0.15 mis-rewrites the
# dynamic table, and strip refuses the section type outright. Result: AppImage
# builds, then SIGSEGVs in _dl_init the moment ld.so loads the first bundled
# lib. Detect a DT_RELR-era host (sniff libcrypto for the section) and wire
# the host's modern toolchain in: PATCHELF env var overrides linuxdeploy's
# bundled patchelf, NO_STRIP=1 skips the broken strip pass entirely. CI builds
# on jammy (glibc 2.35) — pre-DT_RELR — and is unaffected.
LIBCRYPTO_HOST="$(ldconfig -p 2>/dev/null | awk '/libcrypto\.so\.3/ && /x86-64/ {print $NF; exit}')"
if [[ -n "$LIBCRYPTO_HOST" ]] && readelf -d "$LIBCRYPTO_HOST" 2>/dev/null | grep -q '(RELR)'; then
    export NO_STRIP=1
    if command -v patchelf >/dev/null 2>&1; then
        export PATCHELF="$(command -v patchelf)"
        printf '\033[33m! Host uses DT_RELR — overriding linuxdeploy patchelf with %s and setting NO_STRIP=1\033[0m\n' "$PATCHELF"
    else
        printf '\033[31mWARNING: host uses DT_RELR but patchelf is not installed — bundled patchelf 0.15 will corrupt .relr.dyn and the AppImage will segfault at launch. Install patchelf >=0.18.\033[0m\n' >&2
    fi
fi

if command -v linuxdeploy >/dev/null 2>&1; then
    step "Bundling shared libraries via linuxdeploy"
    linuxdeploy \
        --appdir "$APPDIR" \
        --executable "$APPDIR/usr/bin/$APP_NAME" \
        --executable "$APPDIR/usr/bin/torch" \
        --desktop-file "$APPDIR/$APP_NAME.desktop" \
        --icon-file "$APPDIR/$APP_NAME.png"
else
    printf '\n\033[33m! linuxdeploy not in PATH — skipping .so bundling.\033[0m\n'
    printf '   The AppImage will only run on systems with matching .so versions.\n'
    printf '   Install from https://github.com/linuxdeploy/linuxdeploy/releases\n'
fi

# ── 7. AppRun ──
# Linuxdeploy makes AppRun a *symlink* to usr/bin/BattleShip and
# expects users to embed their AppRun-equivalent logic (env, cwd) in
# the binary's launch path. Our binary doesn't do that, and torch
# needs cwd=usr/share/BattleShip to find config.yml + yamls/. Replace
# the symlink (rm first — `cat >` follows symlinks and would clobber
# the game binary at usr/bin/BattleShip) with a shell script that
# sets LD_LIBRARY_PATH for the bundled libs, cd's into the data dir,
# then execs the binary.
rm -f "$APPDIR/AppRun"
cat > "$APPDIR/AppRun" <<EOF
#!/bin/sh
HERE="\$(dirname "\$(readlink -f "\${0}")")"
export PATH="\$HERE/usr/bin:\$PATH"
export LD_LIBRARY_PATH="\$HERE/usr/lib\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}"
cd "\$HERE/usr/share/$APP_NAME" || exit 1
exec "\$HERE/usr/bin/$APP_NAME" "\$@"
EOF
chmod +x "$APPDIR/AppRun"

# ── 8. Pack into AppImage if appimagetool is available ──
if command -v appimagetool >/dev/null 2>&1; then
    step "Packing AppImage"
    rm -f "$APPIMAGE"
    appimagetool "$APPDIR" "$APPIMAGE"
    [[ -f "$APPIMAGE" ]] || fail "appimagetool did not produce $APPIMAGE"
    chmod +x "$APPIMAGE"
    # Drop the standalone Python save editor next to the AppImage so
    # users can edit $XDG_DATA_HOME/BattleShip/ssb64_save.bin without
    # extracting the AppImage. Pure stdlib, runs as
    # `python3 save_editor.py …`.
    cp "$ROOT/tools/save_editor.py" "$DIST_DIR/save_editor.py"
    APP_KB=$(du -k "$APPIMAGE" | awk '{print $1}')
    printf '\n\033[32m✓ AppImage ready: %s (%s KB)\033[0m\n' "$APPIMAGE" "$APP_KB"
    printf '\033[32m✓ Save editor:    %s\033[0m\n' "$DIST_DIR/save_editor.py"
else
    APP_KB=$(du -sk "$APPDIR" | awk '{print $1}')
    printf '\n\033[33m! appimagetool not in PATH — produced AppDir only.\033[0m\n'
    printf '   AppDir: %s (%s KB)\n' "$APPDIR" "$APP_KB"
    printf '   Install appimagetool from https://github.com/AppImage/AppImageKit/releases\n'
    printf '   then run: appimagetool "%s" "%s"\n' "$APPDIR" "$APPIMAGE"
fi
printf '   App-data: $XDG_DATA_HOME/BattleShip/ (or ~/.local/share/BattleShip/)\n'
printf '   First launch will prompt for your ROM via the ImGui wizard.\n'
