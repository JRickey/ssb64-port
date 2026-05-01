#!/usr/bin/env bash
# Builds BattleShip as a self-contained macOS .app bundle.
#
# Output: <repo-root>/dist/BattleShip.app
#
# Layout produced:
#   BattleShip.app/
#     Contents/
#       Info.plist
#       MacOS/
#         BattleShip                 — main executable
#         torch                      — sidecar for first-run extraction
#       Resources/
#         f3d.o2r                    — Fast3D shader archive (ROM-independent)
#         config.yml                 — Torch extraction config
#         yamls/us/*.yml             — Torch extraction recipes
#         gamecontrollerdb.txt       — SDL controller mappings
#
# The bundle is built with NON_PORTABLE=ON so the runtime resolves saves,
# BattleShip.cfg.json, and the user's extracted BattleShip.o2r out of the
# OS app-data dir (~/Library/Application Support/BattleShip/) instead of cwd.
#
# Notes:
# - The .app does NOT include BattleShip.o2r — that's ROM-derived and gets
#   extracted on first launch via the ImGui wizard.
# - Code signing / notarization is not handled here; expect Gatekeeper to
#   warn on first launch. A future pass should sign with `codesign --deep`
#   and notarize via `notarytool`.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build-bundle"
DIST_DIR="$ROOT/dist"
APP_NAME="BattleShip"
APP="$DIST_DIR/$APP_NAME.app"
JOBS="${JOBS:-$(sysctl -n hw.ncpu 2>/dev/null || echo 4)}"

step() { printf '\n\033[36m=== %s ===\033[0m\n' "$1"; }
fail() { printf '\033[31mERROR: %s\033[0m\n' "$1" >&2; exit 1; }

[[ "$(uname -s)" == "Darwin" ]] || fail "package-macos.sh runs on macOS only"

# ── 0. Run codegen scripts that don't need the ROM ──
# Encoded credit files are gitignored (input text is in src/credits/),
# so a fresh checkout (CI or otherwise) must run the encoder before
# cmake builds scstaffroll.c. ROM-independent — same step CMake's
# GenerateCreditsAssets target runs.
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

# Build the f3d.o2r shader archive (ROM-independent, just zips the LUS
# shaders directory). CMake's GenerateF3DO2R target produces this at
# $ROOT/f3d.o2r — reuse the same recipe rather than re-implement.
step "Packaging Fast3D shader archive"
F3D_O2R="$BUILD_DIR/f3d.o2r"
rm -f "$F3D_O2R"
( cd "$ROOT/libultraship/src/fast" && zip -rq "$F3D_O2R" shaders )
[[ -f "$F3D_O2R" ]] || fail "f3d.o2r was not created"

# ── 2. Locate built artifacts ──
SSB64_BIN="$BUILD_DIR/BattleShip"
TORCH_BIN="$BUILD_DIR/TorchExternal/src/TorchExternal-build/torch"
[[ -x "$SSB64_BIN" ]] || fail "BattleShip binary not found at $SSB64_BIN"
[[ -x "$TORCH_BIN" ]] || fail "torch binary not found at $TORCH_BIN"

# ── 3. Assemble the bundle ──
step "Assembling $APP"
rm -rf "$APP"
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources/yamls/us"

cp "$SSB64_BIN"  "$APP/Contents/MacOS/$APP_NAME"
cp "$TORCH_BIN"  "$APP/Contents/MacOS/torch"
cp "$F3D_O2R"    "$APP/Contents/Resources/f3d.o2r"
cp "$ROOT/gamecontrollerdb.txt" "$APP/Contents/Resources/gamecontrollerdb.txt"
cp "$ROOT/config.yml" "$APP/Contents/Resources/config.yml"
cp "$ROOT/yamls/us/"*.yml "$APP/Contents/Resources/yamls/us/"
cp "$ROOT/assets/icon.icns" "$APP/Contents/Resources/AppIcon.icns"

# Bundle the ESC menu fonts. Menu.cpp::FindMenuAssetPath walks up from
# RealAppBundlePath() (= Contents/Resources inside an .app on macOS)
# checking each parent for assets/custom/fonts/<name> — so placing the
# TTFs at Contents/Resources/assets/custom/fonts/ matches first
# iteration. Without this the menu silently falls back to ImGui's
# default font.
mkdir -p "$APP/Contents/Resources/assets/custom/fonts"
cp "$ROOT/assets/custom/fonts/Montserrat-Regular.ttf"  "$APP/Contents/Resources/assets/custom/fonts/"
cp "$ROOT/assets/custom/fonts/Inconsolata-Regular.ttf" "$APP/Contents/Resources/assets/custom/fonts/"

# ── 4. Info.plist ──
# Minimal but sufficient: bundle ID, version, executable name, high-DPI flag.
# CFBundleIdentifier picks the same reverse-DNS the user's app-data dir
# is scoped to (battleship), keeping save state stable across signed/unsigned
# rebuilds.
cat > "$APP/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
  "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key>                <string>BattleShip</string>
    <key>CFBundleDisplayName</key>         <string>BattleShip</string>
    <key>CFBundleIdentifier</key>          <string>com.ssb-decomp-re.battleship</string>
    <key>CFBundleVersion</key>             <string>1.0</string>
    <key>CFBundleShortVersionString</key>  <string>1.0</string>
    <key>CFBundlePackageType</key>         <string>APPL</string>
    <key>CFBundleSignature</key>           <string>????</string>
    <key>CFBundleExecutable</key>          <string>$APP_NAME</string>
    <key>CFBundleIconFile</key>            <string>AppIcon</string>
    <key>LSMinimumSystemVersion</key>      <string>11.0</string>
    <key>NSHighResolutionCapable</key>     <true/>
    <key>NSSupportsAutomaticGraphicsSwitching</key> <true/>
</dict>
</plist>
EOF

# Make the binaries executable (cp preserves mode, but be defensive).
chmod +x "$APP/Contents/MacOS/$APP_NAME" "$APP/Contents/MacOS/torch"

# ── 4a. Bundle Homebrew dylib dependencies (fixes #43) ──
# CMake links BattleShip / torch against Homebrew dylibs (SDL2, GLEW,
# libzip, tinyxml2, spdlog, fmt, …) using their absolute install paths
# (`/opt/homebrew/opt/<pkg>/lib/lib*.dylib`). On a developer machine the
# .app launches fine because every load command resolves verbatim. On
# any user's Mac without Homebrew at the same prefix and exact package
# versions, dyld bails before main() with `Library not loaded:
# /opt/homebrew/*/libSDL2-2.0.0.dylib`. This is the macOS analogue of
# bundling SDL2.dll on Windows / `.so` deps via linuxdeploy on Linux —
# the package script must walk the binaries' transitive Homebrew deps,
# stage them into Contents/Frameworks/, rewrite each dylib's id to
# `@rpath/lib*.dylib`, retarget the binaries' references via
# `install_name_tool`, and add an `LC_RPATH` of `@executable_path/../Frameworks`.
#
# `dylibbundler` (Homebrew package) automates all of that. `-of` =
# overwrite-files (idempotent re-runs), `-b` = bundle transitive deps,
# `-x` = files to fix (repeatable for torch alongside BattleShip),
# `-d` = destination directory, `-p` = install_name prefix, `-cd` =
# create destination, `-ns` = skip dylibbundler's own adhoc codesign
# pass since the bundle-level `codesign --deep --force` below resigns
# everything in one shot.
#
# `-p @executable_path/../Frameworks/` makes the rewritten install_names
# fully self-resolving (dyld substitutes `@executable_path` with the
# directory of the loading executable — Contents/MacOS — so the path
# lands at Contents/Frameworks/lib*.dylib).  Setting `-p @rpath/` would
# require an LC_RPATH on the binary, and dylibbundler's existing-rpath
# rewrite stomps on that path with literal `@rpath/`, producing a
# recursive load command that dyld can't resolve.  Using the absolute
# `@executable_path` form sidesteps that.
step "Bundling Homebrew dylib dependencies"
command -v dylibbundler >/dev/null \
    || fail "dylibbundler not in PATH — install with: brew install dylibbundler"
dylibbundler -of -b -cd -ns \
    -x "$APP/Contents/MacOS/$APP_NAME" \
    -x "$APP/Contents/MacOS/torch" \
    -d "$APP/Contents/Frameworks/" \
    -p "@executable_path/../Frameworks/"

# Sanity-check: no /opt/homebrew or /usr/local references should remain in
# the binaries' load commands. Catches the case where dylibbundler missed a
# transitive dep (rare, but worth failing loudly here rather than letting
# the .app ship and hit the user with a runtime "Library not loaded:").
for bin in "$APP/Contents/MacOS/$APP_NAME" "$APP/Contents/MacOS/torch"; do
    if otool -L "$bin" | grep -qE '(/opt/homebrew|/usr/local/Cellar)'; then
        echo "  remaining unbundled refs in $bin:" >&2
        otool -L "$bin" | grep -E '(/opt/homebrew|/usr/local/Cellar)' >&2
        fail "dylibbundler left non-portable load commands in $(basename "$bin")"
    fi
done

# ── 4b. Adhoc-sign the bundle as a unit ──
# Modern Gatekeeper (Sequoia / 15.x+) flags downloaded adhoc-signed
# bundles as "damaged" if the signature isn't deep enough to cover
# every Mach-O inside. cp preserved each binary's linker-signature
# individually, but the bundle as a whole isn't sealed — so the .app
# refuses to install / launch from a quarantined DMG.
#
# `codesign --deep --force --sign -` walks the bundle, adhoc-signs
# every executable and dylib, and writes a bundle-level signature
# referencing all of them. Doesn't need an Apple Developer cert; it's
# enough to satisfy Gatekeeper's structural check on a quarantined
# download. For full "no warnings, no right-click-Open" we'd need a
# real Developer ID Apple Distribution cert + notarization, but that
# costs $99/yr and isn't tractable for an unsigned community port.
#
# Users who still hit "damaged" can run:
#   xattr -dr com.apple.quarantine /Applications/BattleShip.app
step "Adhoc-signing bundle"
codesign --deep --force --sign - "$APP"
codesign --verify --deep --strict "$APP" \
    && echo "  signature verified" \
    || fail "codesign verify failed on $APP"

# ── 5. Build a drag-and-drop DMG ──
# Drives `create-dmg` (homebrew: `brew install create-dmg`) rather than
# rolling our own Finder/AppleScript dance. The community tool handles
# the well-known macOS quirks that make hand-rolled DMG styling
# unreliable: Finder view caching across remounts of the same volume
# name, alias resolution back to the writable scratch image, .DS_Store
# write timing, and quarantine xattr stripping. Reinventing these
# workarounds isn't worth the maintenance.
#
# Background image: we cap the long edge at 600px so the installer
# window matches the standard macOS DMG footprint instead of the source
# artwork's full 1586x992. The 1x and 2x renders are combined into a
# single multi-representation TIFF — Apple's documented mechanism for
# resolution-independent DMG backgrounds; Finder picks the right rep
# based on the user's display.
DMG_VOLNAME="BattleShip"
DMG_BG_SRC="$ROOT/assets/macos_dmg_banner.png"
DMG_BG_LONG=600
DMG="$DIST_DIR/$APP_NAME.dmg"
DMG_STAGE="$DIST_DIR/dmg-stage"
DMG_BG_DIR="$DIST_DIR/dmg-bg"

command -v create-dmg >/dev/null \
    || fail "create-dmg not in PATH — install with: brew install create-dmg"

step "Building DMG"
rm -rf "$DMG_STAGE" "$DMG_BG_DIR" "$DMG"
mkdir -p "$DMG_STAGE" "$DMG_BG_DIR"
# create-dmg expects a source folder containing only the artifacts the
# user should see in the DMG window. The Applications shortcut is
# injected by --app-drop-link, not staged here.
cp -R "$APP" "$DMG_STAGE/"

# Bundle the standalone Python save editor next to the .app so users
# can edit ~/Library/Application Support/BattleShip/ssb64_save.bin
# (binary save game format documented in the script's docstring).
# Pure stdlib, no install needed — runs as `python3 save_editor.py …`.
cp "$ROOT/tools/save_editor.py" "$DMG_STAGE/save_editor.py"

sips -Z $((DMG_BG_LONG * 2)) "$DMG_BG_SRC" --out "$DMG_BG_DIR/bg@2x.png" >/dev/null
sips -Z $DMG_BG_LONG          "$DMG_BG_SRC" --out "$DMG_BG_DIR/bg.png"    >/dev/null
DMG_BG_W=$(sips -g pixelWidth  "$DMG_BG_DIR/bg.png" | awk '/pixelWidth/  {print $2}')
DMG_BG_H=$(sips -g pixelHeight "$DMG_BG_DIR/bg.png" | awk '/pixelHeight/ {print $2}')
# Multi-rep TIFF: tile the 1x at 72 dpi and the 2x at 144 dpi into a
# single file. Finder reads the matching rep on retina vs non-retina.
tiffutil -cathidpicheck "$DMG_BG_DIR/bg.png" "$DMG_BG_DIR/bg@2x.png" \
    -out "$DMG_BG_DIR/background.tiff" >/dev/null

# Detach any stale mount of the same volume name before invoking
# create-dmg — otherwise hdiutil's auto-mount step will fail.
if [[ -d "/Volumes/$DMG_VOLNAME" ]]; then
    hdiutil detach "/Volumes/$DMG_VOLNAME" -force >/dev/null 2>&1 || true
fi

create-dmg \
    --volname "$DMG_VOLNAME" \
    --background "$DMG_BG_DIR/background.tiff" \
    --window-pos 200 120 \
    --window-size "$DMG_BG_W" "$DMG_BG_H" \
    --icon-size 128 \
    --icon "$APP_NAME.app" $((DMG_BG_W / 4))     $((DMG_BG_H * 3 / 5)) \
    --app-drop-link            $((DMG_BG_W * 3 / 4)) $((DMG_BG_H * 3 / 5)) \
    --hide-extension "$APP_NAME.app" \
    --hdiutil-quiet \
    --no-internet-enable \
    "$DMG" \
    "$DMG_STAGE"

rm -rf "$DMG_STAGE" "$DMG_BG_DIR"
[[ -f "$DMG" ]] || fail "DMG was not created"

# ── 6. Report ──
APP_KB=$(du -sk "$APP" | awk '{print $1}')
DMG_KB=$(du -sk "$DMG" | awk '{print $1}')
printf '\n\033[32m✓ Bundle: %s (%s KB)\033[0m\n' "$APP" "$APP_KB"
printf '\033[32m✓ DMG:    %s (%s KB)\033[0m\n' "$DMG" "$DMG_KB"
printf '   To run from the bundle:        open "%s"\n' "$APP"
printf '   To install from the DMG:       open "%s"  (then drag to Applications)\n' "$DMG"
printf '   App-data: ~/Library/Application Support/BattleShip/\n'
printf '   First launch will prompt for your ROM via the ImGui wizard.\n'
