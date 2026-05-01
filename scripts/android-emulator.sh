#!/usr/bin/env bash
# android-emulator.sh — create + launch the SSB64 test AVD.
#
# Usage:
#   scripts/android-emulator.sh           # create AVD if missing, then boot
#   scripts/android-emulator.sh --recreate # nuke AVD and rebuild fresh
#   scripts/android-emulator.sh --shell    # launch + drop into adb shell
#
# Prerequisites: scripts/android-env.sh sourced (NDK, SDK, JDK17 in PATH).

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=android-env.sh
source "$SCRIPT_DIR/android-env.sh"

AVD_NAME="ssb64test"
SYSIMG="system-images;android-34;google_apis;arm64-v8a"
DEVICE="pixel_6"  # 1080x2400, hwlevel matches what most users have

RECREATE=0
DROP_SHELL=0
for arg in "$@"; do
    case "$arg" in
        --recreate) RECREATE=1 ;;
        --shell)    DROP_SHELL=1 ;;
        -h|--help)  sed -n '2,12p' "$0"; exit 0 ;;
    esac
done

if (( RECREATE )); then
    avdmanager delete avd -n "$AVD_NAME" 2>/dev/null || true
fi

if ! avdmanager list avd 2>/dev/null | grep -q "Name: $AVD_NAME$"; then
    echo "Creating AVD '$AVD_NAME' from $SYSIMG..."
    echo no | avdmanager create avd \
        --name "$AVD_NAME" \
        --package "$SYSIMG" \
        --device "$DEVICE" \
        --force
fi

echo "Booting emulator (Ctrl+C to stop)..."
emulator -avd "$AVD_NAME" -gpu host -no-snapshot-save &
EMU_PID=$!

if (( DROP_SHELL )); then
    echo "Waiting for device..."
    adb wait-for-device
    adb shell
fi

wait "$EMU_PID"
