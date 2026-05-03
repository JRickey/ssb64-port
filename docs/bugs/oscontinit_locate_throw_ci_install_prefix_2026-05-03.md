# `osContInit` filesystem_error from CI-baked install prefix — Windows boot crash

**Status:** RESOLVED 2026-05-03

**Where:** `libultraship` — `src/ship/Context.cpp`, `src/libultraship/libultra/os.cpp`. Commit `d388467d` on `JRickey/libultraship#ssb64`. Submodule pointer bumped in this repo.

## Symptom

Windows v0.7.2 release crashed during boot on user machines, shortly after audio and the controller-init thread came up. From the user log:

```
*** C++ THROW (first-chance) ***
    type="class std::filesystem::filesystem_error"
    what=exists: The device is not ready.:
         "D:/a/BattleShip/BattleShip/BattleShip/gamecontrollerdb.txt"

*** C++ THROW (first-chance) ***
    type="class spdlog::spdlog_ex"
    what=async flush: thread pool doesn't exist anymore

[critical] Exception: 0xe06d7363
```

The `D:\a\<owner>\<repo>` path is the GitHub Actions runner's workspace — meaningless on a user machine. The crash always landed at the controller-init stage, shortly after `osCreateThread id=6`. Spdlog's async pool teardown was a downstream symptom, not the cause.

## Root cause

Two compounding bugs.

### 1. CI bakes the runner workspace path into `install_config.h`

`scripts/package-windows.ps1` invokes:

```powershell
cmake -DNON_PORTABLE=ON -DCMAKE_INSTALL_PREFIX=BattleShip -B build-bundle-win
```

CMake resolves a relative `CMAKE_INSTALL_PREFIX` against the configure-time cwd, so on the GitHub runner the prefix becomes `D:/a/BattleShip/BattleShip/BattleShip`. libultraship's `NON_PORTABLE` build path bakes that absolute prefix into `install_config.h` as the app-bundle path. `Ship::Context::GetAppBundlePath()` returns the runner path verbatim at runtime — every subsequent file probe under that prefix targets a `D:` drive the user's machine has no media in.

The port already routes around this for `f3d.o2r` and `BattleShip.o2r` via the noexcept `ssb64::PortLocateFile` (issue #58, see `port/`). `gamecontrollerdb.txt` was the one remaining caller, loaded by libultraship's own `osContInit` through the upstream throwing helper.

### 2. Throwing `std::filesystem::exists(p)` overload in `LocateFileAcrossAppDirs`

`Ship::Context::LocateFileAcrossAppDirs` walked three candidate paths with the throwing overload:

```cpp
fpath = GetPathRelativeToAppDirectory(path, appName);
if (std::filesystem::exists(fpath)) { return fpath; }      // throws on non-ENOENT
fpath = GetPathRelativeToAppBundle(path);
if (std::filesystem::exists(fpath)) { return fpath; }      // throws on non-ENOENT
return "./" + std::string(path);
```

On Windows, `std::filesystem::exists` calls `GetFileAttributesExW`. A missing file returns `ERROR_FILE_NOT_FOUND` and the throwing overload returns `false`. **Anything else** — `ERROR_NOT_READY` for a drive with no media, `ERROR_PATH_NOT_FOUND` deep in a UNC tree, etc. — gets rethrown as `std::filesystem::filesystem_error`. The throw unwound out of the SSB64 controller-init thread, no catch was in scope, `std::terminate` fired, and spdlog's async pool tore down on the way out — leaving the `0xE06D7363` crash signature with no useful traceback.

## Why it hit later than `f3d.o2r`

`f3d.o2r` and `BattleShip.o2r` are loaded much earlier through `ssb64::PortLocateFile`, which already uses the noexcept overload. The controller thread doesn't init until after audio comes up — that's when the unguarded LUS helper finally got hit.

## Fix

Both hunks live in `JRickey/libultraship` on branch `ssb64`.

### `src/ship/Context.cpp`

Add `<filesystem>` and `<system_error>` to the includes and switch `LocateFileAcrossAppDirs` to the noexcept `exists(p, ec)` overload:

```cpp
std::error_code ec;
std::string fpath;

fpath = GetPathRelativeToAppDirectory(path, appName);
if (std::filesystem::exists(fpath, ec)) { return fpath; }
fpath = GetPathRelativeToAppBundle(path);
if (std::filesystem::exists(fpath, ec)) { return fpath; }
return "./" + std::string(path);
```

For path probing, "false" is the right answer for any failure — the caller falls through to the next candidate or the cwd-relative fallback. `ec` is intentionally ignored.

### `src/libultraship/libultra/os.cpp`

Defense-in-depth: wrap the `gamecontrollerdb.txt` load in `osContInit` with try/catch so a future regression in any path helper can never kill the controller thread. `gamecontrollerdb.txt` is a quality-of-life mappings file — failure to find or parse it should be non-fatal, and SDL has builtin mappings for everything the project supports anyway.

```cpp
try {
    std::string controllerDb = Ship::Context::LocateFileAcrossAppDirs("gamecontrollerdb.txt");
    int mappingsAdded = SDL_GameControllerAddMappingsFromFile(controllerDb.c_str());
    if (mappingsAdded >= 0) {
        SPDLOG_INFO("Added SDL game controllers from \"{}\" ({})", controllerDb, mappingsAdded);
    } else {
        SPDLOG_ERROR("Failed add SDL game controller mappings from \"{}\" ({})", controllerDb, SDL_GetError());
    }
} catch (const std::exception& e) {
    SPDLOG_ERROR("osContInit: skipping gamecontrollerdb.txt — {}", e.what());
} catch (...) {
    SPDLOG_ERROR("osContInit: skipping gamecontrollerdb.txt — unknown exception");
}
```

The `PreInitRaphnet` / `SDL_Init(SDL_INIT_GAMECONTROLLER)` / `ControlDeck->Init` calls below the try/catch are unchanged.

## Verification

Repro condition without the fix is "any user machine whose `D:` drive has no media when running a release-mode `NON_PORTABLE` Windows build configured with a relative `CMAKE_INSTALL_PREFIX`." Forcing the repro locally with a bogus drive letter:

```powershell
cmake -B build-test-bogus -DCMAKE_BUILD_TYPE=Release -DNON_PORTABLE=ON `
      -DCMAKE_INSTALL_PREFIX=Z:/a/BattleShip/BattleShip/BattleShip
cmake --build build-test-bogus --config Release -j 4
```

After applying the fix the launch reaches the title screen with no crash. The log shows one of:

- `Added SDL game controllers from "<…>" (NNN)` — happy path, mappings file was found in `%APPDATA%\BattleShip\` or in the app-bundle dir.
- `Failed add SDL game controller mappings from "./gamecontrollerdb.txt" (Couldn't open …)` — cwd-relative fallback hit because no candidate path resolved. SDL falls back to its builtin mappings; controllers still bind through the menu.
- `osContInit: skipping gamecontrollerdb.txt — exists: The device is not ready.: "…"` — the new try/catch caught a throw that the noexcept switch missed (e.g. inside `GetPathRelativeToAppBundle` itself). Also non-fatal.

The crash signatures must not appear:
- `class std::filesystem::filesystem_error` — gone after the `Context.cpp` fix.
- `Exception: 0xe06d7363` at the controller-init stage — gone.
- `async flush: thread pool doesn't exist anymore` — gone (was a downstream symptom of the terminate path).

## Class

Third-party path helper throws on non-`ENOENT` failure → unhandled exception on a worker thread → `terminate`. Same family as `oscontpad_lus_sizeof_overrun_2026-04-24.md`: LUS code shipping in the binary that the port doesn't have direct authority over until a submodule bump lands.

## Audit hooks

- Any `libultraship` API probing the filesystem with the throwing `std::filesystem::exists(p)` (no `error_code`) is a latent Windows crash on non-ENOENT failures. Worth a sweep across `Ship::Context`, `ResourceManager`, `ArchiveManager`, factory loaders. Same applies to `is_directory`, `is_regular_file`, `file_size` — every one of those has a noexcept overload that should be preferred for probes.
- Any `osContInit`-class entry point that runs on a non-main thread and performs file I/O needs a catch-all so a throw doesn't kill the process before the GUI is up. Scan thread entry points in `port/` and `libultraship/src/` that don't terminate cleanly on exception.
- The "CI bakes the runner workspace path into a release artifact" failure mode applies to any `NON_PORTABLE` Windows packaging script. Consider passing `-DCMAKE_INSTALL_PREFIX=.` (or omitting `NON_PORTABLE` on Windows since the port already overrides the app-bundle resolution via `RealAppBundlePath()` in `port/`).

## Diagnostic fingerprint

- `class std::filesystem::filesystem_error` thrown on a worker thread shortly after `osCreateThread id=6`.
- `what=` cites a path under a drive letter that doesn't exist or has no media (`D:/...`, `Z:/...`).
- Unwind path tears down spdlog's async pool, producing a secondary `async flush: thread pool doesn't exist anymore` throw and a final `Exception: 0xe06d7363` (Windows C++ exception code).
- Same machine boots fine if the named drive becomes ready (DVD inserted, SD card loaded, etc.).
