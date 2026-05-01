#!/usr/bin/env bash
# android-env.sh — source this to populate the Android dev environment.
#
# Usage:  source scripts/android-env.sh
#
# Sets ANDROID_HOME / ANDROID_SDK_ROOT / ANDROID_NDK_HOME / JAVA_HOME and
# prepends platform-tools, emulator, and JDK17 bins to PATH so adb /
# emulator / sdkmanager work from a plain shell.
#
# Installed via:
#   brew install --cask android-ndk android-commandlinetools
#   brew install openjdk@17

export JAVA_HOME=/opt/homebrew/opt/openjdk@17
export ANDROID_NDK_HOME=/opt/homebrew/share/android-ndk
export ANDROID_SDK_ROOT=/opt/homebrew/share/android-commandlinetools
export ANDROID_HOME="$ANDROID_SDK_ROOT"

export PATH="$JAVA_HOME/bin:$ANDROID_SDK_ROOT/platform-tools:$ANDROID_SDK_ROOT/emulator:$PATH"
