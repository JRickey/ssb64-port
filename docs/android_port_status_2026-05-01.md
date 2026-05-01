# Android Port — Status (Phase 7 complete)

Supersedes the spike doc (`android_port_spike_2026-05-01.md`) — that was
written when only `libmain.so` compiled and nothing ran. As of this entry
the attract demo plays end-to-end on the `ssb64test` AVD with audio,
on-screen controls, and stable rendering.

## What works

End-to-end first-run flow on the emulator:

1. Launch from system launcher → `BootActivity` extracts bundled assets
   (`f3d.o2r`, `ssb64.o2r`, `config.yml`, `yamls/us/*.yml`) into
   `getExternalFilesDir()`.
2. SAF picker (`ACTION_OPEN_DOCUMENT`) accepts the user's `baserom.us.z64`.
3. `libtorch_runner.so`'s `torch_extract_o2r` runs on-device, producing
   `BattleShip.o2r` next to the bundled assets (~5s on the emulator).
4. `BattleShipActivity` (subclass of `org.libsdl.app.SDLActivity`) loads
   `libSDL2.so` + `libmain.so`, calls `SDL_main`.
5. Game cycles through the SSB64 attract demo: character intros (Mario,
   Link, Yoshi, DK, Kirby, Samus, Fox, Pikachu...), stage demos (Hyrule
   Castle, Peach's Castle, Kirby's Dream Land, Sector Z...).
6. Audio plays via SDL2's OpenSL ES backend; `AudioFlinger` mixer streams
   continuously. (Confirmed via logcat — emulator host audio passthrough.)
7. On-screen overlay renders above the GLES surface: drag-anywhere analog
   stick on the left half, A/B/Z/Start cluster on the right.
8. Touch input drives a virtual SDL gamepad (`SDL_JoystickAttachVirtualEx`
   with Xbox-360 vendor/product signature) that LUS's existing
   `SDLButtonToAnyMapping` picks up unchanged.

## Architecture decisions worth documenting

### GFX render hops to the SDL_main thread (Phase 6.1)

The defining architectural choice for Android. SSB64's cooperative
scheduler runs as `port_coroutine` aarch64 fibers stack-switched on the
SDL_main thread. ART's CheckJNI tracks JNI transition frames per OS
thread via a `ManagedStack` linked list whose head lives on the native
stack — when our coroutine swap moves SP to a different fiber, the head
dangles, and any `jobject` allocated by Binder IPC from the fiber comes
back as "invalid JNI transition frame reference".

ImGui's SDL2 backend calls `SDL_GetDisplayUsableBounds` every frame in
`ImGui_ImplSDL2_UpdateMonitors`, which on Android falls through to
`SDL_getenv` → `Android_JNI_GetManifestEnvironmentVariables` →
`PackageManager.getApplicationInfo` (Binder IPC). On the GFX coroutine
this trips CheckJNI within seconds of boot. We tried surgical guards
(skip cursor tick, hoist HID init, pre-set the hint, warm `getenv`) —
each one fixed one site but more kept surfacing.

Fix: `port_submit_display_list` now stages the DL pointer; the actual
`Fast3dWindow::DrawAndRunGraphicsCommands` call happens later in
`PortPushFrame` after `port_resume_service_threads` — on the SDL_main
thread proper, not on a fiber. The scheduler's task-completion
signalling is decoupled from GPU work, so the deferral is invisible to
the cooperative scheduler.

This pattern preserves the coroutine model for game logic (where it's
useful) while keeping JNI-touching rendering on the JVM-attached
thread. Smaller blast radius than collapsing the scheduler à la
Ship-of-Harkinian's single-`RunFrame` refactor.

Companion mitigations that are still required (live in
`libultraship` submodule's `ssb64` branch + `port/port.cpp`):

- `MouseStateManager::CursorVisibilityTimeoutTick` short-circuits on
  `__ANDROID__` — touch devices don't have cursors and the JNI path
  used to fire from the GFX fiber.
- Pre-init `SDL_INIT_GAMECONTROLLER` from `port.cpp` main on Android
  before any coroutine spawns. The HID init's JNI handshake happens
  on a real thread; the `osContInit` redo from the controller fiber
  is then a refcount no-op.
- `SDL_SetHint(SDL_HINT_DISPLAY_USABLE_BOUNDS, "0,0,1920,1080")` and
  `SDL_getenv` warmup at the same place — populate SDL2's per-program
  hint store and `bHasEnvironmentVariables` cache on the real thread
  so subsequent `SDL_GetHint` reads short-circuit.

### Touch input → SDL virtual gamepad

`port/android_touch_overlay.cpp` exposes JNI methods
`Java_com_jrickey_battleship_TouchOverlay_setButton` and
`Java_com_jrickey_battleship_TouchOverlay_setAxis`. On first call,
`ensureAttached()` runs `SDL_JoystickAttachVirtualEx` with vendor 0x045E
/ product 0x028E (Xbox 360). SDL2 promotes the virtual joystick to
`SDL_GameController` and LUS's existing controller pipeline picks it up
without code changes — user-configurable remappings via the LUS UI
still work.

### ROM extraction architecture

We **never ship Nintendo content**. `BattleShip.o2r` is generated
on-device from the user's ROM by `libtorch_runner.so` (cross-compiled
Torch, ~34 MB). `f3d.o2r` (open-source Fast3D shaders) and `ssb64.o2r`
(port-local custom assets) are bundled in APK `assets/` and extracted
to `getExternalFilesDir()` on first launch.

## Performance / known issues

- **Audio**: works via OpenSL ES default. Could move to AAudio (Android
  8.0+) for ~5ms lower latency; not blocking.
- **Display orientation**: BootActivity portrait + BattleShipActivity
  sensorLandscape. SDL2's setOrientation kicks in mid-launch and the
  surface ends up letterboxed in the device's portrait viewport. Phase
  8 polish: force landscape from BootActivity onward.
- **Per-button visual press feedback**: not implemented; current
  buttons are static circles. Phase 8.
- **Auto-hide overlay**: not yet — wire to LUS's controller-detection
  hook.
- **`HIDDeviceManager.hid_init` RECEIVER_EXPORTED warning**: SDL2 main
  has a fix newer than 2.32.10. Non-fatal, logs once at boot.

## Phase summary

| Phase | Commit | What |
|---|---|---|
| 1 | 19417a4 | aarch64 asm coroutine swap (replaces bionic ucontext) |
| 2 | 752a01f | Gradle/AGP 8.13 scaffold, externalNativeBuild → CMake |
| 3 | d8eceeb | SDLActivity bridge — `SDL_main` runs on Android |
| 4.1+4.2 | 77d8fa4 | Cross-compile Torch as `libtorch_runner.so` |
| 4.3 | 68dbd47 | Bundle non-copyrighted assets in APK + first-run extract |
| 4.4 | edcc424 | SAF ROM picker + JNI bridge → on-device asset extraction |
| 5 | d94b940 | First-boot triage (HID init hoist, cursor guard) |
| 6.1 | eb31202 | Defer GFX render to SDL_main thread (fixes JNI-from-fiber) |
| 6.3 | 6e678e0 | Touch overlay — virtual SDL gamepad via JNI |
| 6.4 | 87e632f | Full layout — analog stick + A/B/Z/Start |
| 7 | (no code) | Audio + ghost-fb verified, both fine on emulator |

## Reproducer (current)

```bash
brew install --cask android-ndk android-commandlinetools
brew install openjdk@17 gradle
source scripts/android-env.sh

yes | sdkmanager --licenses
sdkmanager --install "platform-tools" "emulator" \
                     "platforms;android-34" \
                     "system-images;android-34;google_apis;arm64-v8a"
scripts/android-emulator.sh

# Build the desktop port once first to get f3d.o2r + ssb64.o2r:
cmake -B build && cmake --build build --target GenerateF3DO2R GeneratePortO2R

# Then build + install the Android APK:
cd android && ./gradlew assembleDebug
adb install -r app/build/outputs/apk/debug/app-debug.apk

# Launch with dev_rom shortcut (skip SAF picker for fast iteration):
adb push baserom.us.z64 \
  /storage/emulated/0/Android/data/com.jrickey.battleship.debug/files/baserom.us.z64
adb shell am start -n com.jrickey.battleship.debug/com.jrickey.battleship.BootActivity \
  --es ssb64.dev_rom \
  /storage/emulated/0/Android/data/com.jrickey.battleship.debug/files/baserom.us.z64
```

## What's next (Phase 8: polish)

- Force landscape across both Activities; fix the portrait-letterbox
  surface mid-launch glitch
- App icon + adaptive launcher icon
- Auto-hide touch overlay when paired Bluetooth/USB controller detected
- Per-button press visual feedback (state-list drawables)
- Layout configurability via res/values/dimens.xml
- Release signing config + AAB build for Play Store
- In-game SAF picker for re-extraction (currently you have to wipe
  BattleShip.o2r manually to re-trigger)
- Investigate AAudio backend for lower audio latency
- Bump SDL2 to a release with the `RECEIVER_EXPORTED` fix
