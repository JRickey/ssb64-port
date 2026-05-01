# Android Port — Compile Spike (2026-05-01)

## TL;DR

`libmain.so` (67 MB, ELF aarch64, dynamically linked) builds clean against
NDK r29 / Android API 26 / arm64-v8a from the `agent/android-port` worktree.
**The artifact compiles and links, but it is not a working game** — the
coroutine system, JNI/Java glue, and APK asset pipeline are all unwritten.

## What works

- Top-level CMake graph configures with the NDK toolchain file once
  `USE_OPENGLES=ON` is forced and `ExternalProject_Add(TorchExternal)` is
  gated off (Torch is a host build tool, can't cross-compile).
- libultraship's existing Android scaffolding (`cmake/dependencies/android.cmake`,
  `find_library(GLESv3)`, `USE_OPENGLES` define) works unmodified.
- All FetchContent dependencies cross-compile cleanly: SDL2 2.32.10,
  spdlog, libzip (zlib found, optional bzip2/lzma/zstd disabled),
  tinyxml2, nlohmann_json, prism, ImGui.
- All ssb64 game C sources (1052 TUs) compile under clang from the NDK with
  the existing decomp warning flags. No bitfield, endianness, or LP64
  surprises beyond what we've already handled for Apple Silicon.
- Port C++ glue (port_watchdog, gameloop, RelocFileTable, etc.) compiles
  with two narrow `#ifdef __ANDROID__` patches (see "Spike stubs" below).

## What's blocked

### 1. Coroutines (load-bearing — no runtime without this)

Bionic libc dropped `getcontext` / `makecontext` / `swapcontext`. They were
never supported on aarch64. `coroutine_posix.cpp` is gated off on Android
and replaced by `coroutine_android.cpp`, which currently aborts on first call.

The cooperative scheduler (Thread1/4/5/6, GObj process coroutines) won't
run a single tic without these. **Real fix options:**

- **Aarch64 asm swapcontext (~30 LOC).** Save/restore x19–x30 + fp/lr + sp;
  same approach as `boost::context::fcontext_t`. Cleanest, no extra deps.
  Recommended.
- **pthread + condvar ping-pong.** Heavier per-coroutine (one OS thread
  each); ~50–80 active coroutines in normal play, may bloat memory and
  scheduling overhead.
- **libco (single-file MIT).** Has aarch64 backend; trivial to vendor.

### 2. Backtrace (cosmetic — crash diagnostics only)

`<execinfo.h>` exists in NDK sysroot but `backtrace`/`backtrace_symbols_fd`
are gated `__INTRODUCED_IN(33)`. We target API 26 → linker errors.
Stubbed in `port_watchdog.cpp` under `__ANDROID_API__ < 33`. Either bump
minSdk to 33 (cuts off Android 12 and below) or wire libunwind directly.

### 3. JNI / Activity glue (not started)

LUS has no Java side. SDL2's prebuilt SDLActivity wants a `libmain.so` (we
produce one) but also needs:
- `android/SDLActivity.java` subclass with the app's package name
- `AndroidManifest.xml` declaring activity, permissions, OpenGL ES feature
- Gradle wrapper or `ndk-build`-driven APK assembly
- A Kotlin/Java bridge for `AAssetManager` (see #4)

None of this exists in either repo today. Estimated ~2 days of plumbing.

### 4. Asset loading (architectural — touches LUS)

LUS's `Context::GetAppDirectoryPath()` calls `readlink("/proc/self/exe")`
on `__linux__` (Android matches), which on Android resolves to
`/data/app/.../base.apk!/lib/arm64/libmain.so`. The .o2r files won't be
next to that path — they live inside the APK's `assets/` and need
`AAssetManager_open` (JNI) or one-time extraction to
`Context.getExternalFilesDir()` at first launch.

LUS has zero `AAssetManager` / `<android/...>` references today. Either
extend LUS with an Android-aware FS abstraction, or copy the three .o2r
files from APK assets to a writable dir during the Activity's
`onCreate` and pass that path to LUS via env var or config override.

### 5. Touch input UI (not started)

SDL2 reports touches as mouse + finger events, but the game expects N64
controller input. Need an on-screen overlay (D-pad + 4 buttons + C-stick)
or require a paired Bluetooth gamepad. Not a compile blocker, but the
game is unplayable without it.

### 6. Asset extraction (architectural — host-only)

Torch is a build-time host binary. The Android build skips
`ExtractAssets` and `GeneratePortO2R`; the .o2r files must be produced on
a host (macOS/Linux/Windows) and dropped into Android's `assets/`. This
is fine — it just needs to be wired into the Gradle build's `merge`
phase.

## Files changed in this spike (all in `agent/android-port` worktree)

```
CMakeLists.txt                        # Android branches: USE_OPENGLES, SHARED lib, gate Torch + post-build copy
port/coroutine_posix.cpp              # exclude when __ANDROID__
port/coroutine_android.cpp            # NEW — abort-stub for spike
port/port_watchdog.cpp                # gate <execinfo.h> on API <33
docs/android_port_spike_2026-05-01.md # this file
```

## Reproducer

```bash
brew install --cask android-ndk    # installs r29 to /opt/homebrew/share/android-ndk

cmake -S .claude/worktrees/android-port \
      -B .claude/worktrees/android-port/build-android -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/opt/homebrew/share/android-ndk/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-26 \
      -DCMAKE_BUILD_TYPE=Release -DUSE_AUTO_VCPKG=OFF

cmake --build .claude/worktrees/android-port/build-android --target ssb64 -j 4
# → build-android/libmain.so   (67 MB, aarch64 ELF)
```

## Prioritized roadmap to a runnable APK

1. **Coroutines (aarch64 asm)** — 1 day. Without this nothing runs.
2. **JNI/Activity scaffolding + APK build glue** — 2 days. Gradle, manifest,
   SDLActivity subclass, asset packaging.
3. **AAssetManager bridge or first-run extraction** — 1 day.
4. **Boot to title screen** — debugging / triage. Likely 2–5 days of
   surprises (file paths, GLES vs GL shader differences, Metal-only code paths).
5. **Touch overlay UX** — 1–2 days. Required before "playable."
6. **Audio latency tuning** — open-ended; OpenSL/AAudio differs from
   CoreAudio.

Realistic estimate to a buggy-but-running APK: **2 weeks of focused work**
on top of this spike. Polished release with controller config UI, save
folder management, Play Store assets, etc.: **4–6 weeks total**.
