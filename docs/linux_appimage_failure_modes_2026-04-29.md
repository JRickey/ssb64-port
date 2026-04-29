# Linux AppImage Doesn't Run — Investigation Handoff (2026-04-29)

**Issue:** [#11 Linux build doesn't work](https://github.com/JRickey/BattleShip/issues/11)
— `BattleShip-x86_64.AppImage` from the latest CI release fails to
launch. Issue is a placeholder; no logs, no specific error yet.

**Status:** Not fixed. Examined the build pipeline
(`.github/workflows/release.yml` build-linux job +
`scripts/package-linux.sh`) and the runtime path resolution in
libultraship's `Context.cpp`. There are several real, separate
problems any one of which can prevent the AppImage from running.
Listed in rough order of likelihood.

## What the pipeline produces

CI job `build-linux` runs on **`ubuntu-24.04`** (glibc 2.39),
installs apt deps, builds with `clang`, then runs
`scripts/package-linux.sh` which:

1. `cmake -B build-bundle-linux -DCMAKE_BUILD_TYPE=Release -DNON_PORTABLE=ON`
2. `cmake --build build-bundle-linux` (Ninja-style implicit, since
   `ninja-build` is installed)
3. Zips `libultraship/src/fast/shaders/` into `f3d.o2r`
4. Copies `BattleShip`, `torch`, `f3d.o2r`, `gamecontrollerdb.txt`,
   `config.yml`, `yamls/us/*.yml` into `dist/BattleShip.AppDir/`
5. Writes `AppRun` (a `cd $HERE/usr/share/BattleShip && exec
   $HERE/usr/bin/BattleShip "$@"` shim), `BattleShip.desktop`, icons
6. Runs `appimagetool dist/BattleShip.AppDir
   dist/BattleShip-x86_64.AppImage`

## Failure modes ranked by likelihood

### A — No `.so` bundling. AppImage relies on the user's distro libs

`scripts/package-linux.sh` doesn't run `linuxdeploy` (or any
equivalent) to bundle the binary's runtime dependencies into
`AppDir/usr/lib/`. The shipped AppImage links dynamically against:

- `libSDL2-2.0.so.0`
- `libGLEW.so.2.2` (Ubuntu 24.04 ships 2.2; older distros ship 2.1)
- `libzip.so.5` (Ubuntu 24.04; older distros may have .4)
- `libspdlog.so.1`
- `libtinyxml2.so.10` (24.04) / `.9` / `.8`
- `libstdc++.so.6` with **GLIBCXX_3.4.32** symbols (clang on 24.04 emits these)
- `libc.so.6` with **GLIBC_2.39** symbols

`AppRun` doesn't set `LD_LIBRARY_PATH` either, so even if libs *were*
in `usr/lib/`, the loader wouldn't find them.

The release.yml comment already acknowledges this: *"AppImages built
on noble (glibc 2.39) won't run on distros older than ~Debian 13."*

**This is almost certainly the bug for any user not on Ubuntu 24.04 /
Debian 13+ / Fedora 40+ / Arch.** Symptom on the reporter's terminal
will be one of:

```
./BattleShip-x86_64.AppImage: error while loading shared libraries:
  libGLEW.so.2.2: cannot open shared object file: No such file or directory
```

```
./BattleShip-x86_64.AppImage: /lib/x86_64-linux-gnu/libstdc++.so.6:
  version `GLIBCXX_3.4.32' not found (required by ./BattleShip-x86_64.AppImage)
```

```
./BattleShip-x86_64.AppImage: /lib/x86_64-linux-gnu/libc.so.6:
  version `GLIBC_2.39' not found (required by ./BattleShip-x86_64.AppImage)
```

**Recommended fix:** install `linuxdeploy` in the CI job and run it
between the `cmake --build` step and `appimagetool`. linuxdeploy
populates `AppDir/usr/lib/` with the binary's `.so` deps minus a
known-system excludelist (libc, libGL, etc.) and rewrites `AppRun` to
set `LD_LIBRARY_PATH=$HERE/usr/lib`. That's the standard AppImage
recipe; the script's TODO-comment ("linuxdeploy is optional but
useful") understates how essential it is for a portable AppImage.

Adjacent: switch the build runner from `ubuntu-24.04` to
`ubuntu-22.04` (jammy, glibc 2.35) for broader compat. The release
workflow's comment about jammy not having `libzip-tools` is true but
already worked around (`touch /usr/bin/{zipcmp,zipmerge,ziptool}`),
so jammy could be the build base.

### B — FUSE 2 vs FUSE 3 on the user's distro

Classic AppImages use libfuse 2 to mount themselves at runtime.
Modern Ubuntu (23.04+) and Fedora (35+) ship `libfuse3` only by
default. On those distros, running an AppImage gives:

```
dlopen(): error loading libfuse.so.2
AppImages require FUSE to run.
```

User has to either install `libfuse2t64` (Ubuntu 24.04) / `fuse2fs`
(Fedora) or extract the AppImage with `--appimage-extract` and run
the `AppRun` from there.

**Recommended fix:** ship the static `runtime-fuse3` (an `appimagetool
--runtime-file` option as of `appimagetool` continuous build 2024-04+)
so the AppImage uses fuse3 directly and runs unmodified on modern
distros. Or document the `libfuse2` install requirement in the
release notes.

### C — `NON_PORTABLE=ON` resolves resource paths wrong inside the AppImage

`Context::GetAppBundlePath()` (`libultraship/src/ship/Context.cpp:436`)
on Linux with `NON_PORTABLE` defined returns the literal
`CMAKE_INSTALL_PREFIX` (defaults to `/usr/local`). Inside the
AppImage the binary actually lives at `/tmp/.mount_XYZ/usr/bin/`, but
`GetAppBundlePath()` ignores that and returns the build-time
hardcoded `/usr/local`.

`LocateFileAcrossAppDirs("f3d.o2r")` then searches:

1. `~/.local/share/BattleShip/f3d.o2r` (app data — empty on first run)
2. `/usr/local/f3d.o2r` (bundle — wrong, points at the build host's
   prefix, not the AppImage mount)
3. `./f3d.o2r` (cwd)

Step 3 is the only thing that saves us — `AppRun` does `cd
$HERE/usr/share/BattleShip` before exec-ing the binary, so cwd is
the right directory and `./f3d.o2r` resolves. So this **isn't a fatal
issue today**, but it's a foot-gun: any future code that calls
`GetPathRelativeToAppBundle()` directly (instead of going through
`LocateFileAcrossAppDirs`'s cwd fallback) will look at `/usr/local`
and hit a missing file.

**Recommended fix:** for Linux + NON_PORTABLE inside an AppImage,
prefer `readlink("/proc/self/exe")` over the hardcoded
`CMAKE_INSTALL_PREFIX`. `Context.cpp:441-453` already has that code
path — it just lives in the `else` branch of `#ifdef NON_PORTABLE`.
Move the `readlink` before the `NON_PORTABLE` check, fall back to
`CMAKE_INSTALL_PREFIX` only if `/proc/self/exe` resolution fails.

### D — Missing AppImage runtime when not embedded

`appimagetool` from the continuous release embeds the AppImage
runtime into the output by default. If the runner's `appimagetool`
binary is older or stripped of the runtime, `appimagetool` falls back
to producing a `.AppDir` zip without the magic header, and running
the file gives `bash: ./BattleShip-x86_64.AppImage: cannot execute
binary file`.

The release workflow grabs `appimagetool` via `wget` from
`https://github.com/AppImage/appimagetool/releases/download/continuous/`
which should always have the runtime embedded, but this is worth
verifying on a CI re-run.

### E — `chmod +x` missing on the AppImage artifact

The script does `chmod +x "$APPIMAGE"` (line 152), but only **inside
package-linux.sh**. The CI uploads the artifact via
`actions/upload-artifact@v4` which preserves Unix mode bits in the
artifact archive… mostly. The user downloads the asset from the
release and may need to `chmod +x BattleShip-x86_64.AppImage` again
themselves (that's documented in the issue body, fine), but if
they're missing the bit there's a clear error.

Less likely than A/B/C but worth ruling out from the user's report.

### F — Wayland-only desktop without SDL2 Wayland support

SDL2 from `apt install libsdl2-dev` on 24.04 is built with Wayland
support, so the resulting binary should work on both X11 and
Wayland. But if SDL2 from older systems lacks Wayland, the AppImage
fails to create a window on a Wayland-only session with:

```
SDL_Init failed: No available video device
```

User can work around by setting `SDL_VIDEODRIVER=x11` (which uses
XWayland). Worth documenting if/when this surfaces.

### G — `clang` codegen depends on the build host's libc

The CI runner explicitly sets `CC=clang CXX=clang++`. Clang's
`-fstack-protector` glibc references and any `__libc_start_main`
versioned symbol can pin to the build host's glibc (2.39 on noble).
Same root cause as failure mode A but a different symptom — the
binary load fails before even reaching the `dlopen` lookup.

The release.yml's comment "the runtime is unchanged (still libstdc++
/ glibc) so AppImage compat is the same as a GCC-built binary" is
correct in *kind* but understates that GCC and Clang both pin to the
build host's glibc.

## Quick triage when the reporter's logs land

When the user replies with their exact failure output, walk down
this checklist in order:

| Symptom in error log | Likely mode |
|----------------------|--------------|
| `error while loading shared libraries: libGLEW.so.2.X` or `libzip.so.X` | A |
| `version 'GLIBCXX_3.4.32' not found` or `GLIBC_2.39 not found` | A / G |
| `AppImages require FUSE to run` or `libfuse.so.2: cannot open` | B |
| `cannot execute binary file` | D |
| `Permission denied` on the chmod'd file | E |
| `SDL_Init failed` / black screen on Wayland | F |
| Crash / SIGSEGV in resource path code | C |

## Followup actions (not done in this pass)

1. **Land linuxdeploy in CI.** This is the largest correctness win.
   The pattern is:
   ```yaml
   - name: Install linuxdeploy
     run: |
       wget -qO /usr/local/bin/linuxdeploy \
         https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
       chmod +x /usr/local/bin/linuxdeploy
   ```
   Then in `package-linux.sh`, after `cmake --build`:
   ```bash
   linuxdeploy --appdir "$APPDIR" \
     --executable "$GAME_BIN" \
     --executable "$TORCH_BIN" \
     --desktop-file "$APPDIR/$APP_NAME.desktop" \
     --icon-file "$APPDIR/$APP_NAME.png"
   ```
   linuxdeploy will populate `usr/lib/`, write a proper `AppRun` that
   sets `LD_LIBRARY_PATH`, and apply the standard `excludelist` so
   we don't ship libc / libGL.
2. **Move `readlink("/proc/self/exe")` ahead of the `NON_PORTABLE`
   branch** in `Context::GetAppBundlePath()` so AppImage-mounted
   binaries find their bundle dir correctly (see mode C).
3. **Switch CI runner to `ubuntu-22.04`** (jammy, glibc 2.35) for
   wider compat, keeping the `touch /usr/bin/{zipcmp,...}` workaround
   that's already in the release.yml.
4. **Embed fuse3 runtime** via `appimagetool --runtime-file` so users
   on 23.04+ Ubuntu / 35+ Fedora don't need libfuse2 installed.
5. **Document `chmod +x` and `libfuse2`** in the release notes.

## What I am NOT doing

Patching the workflow / package-linux.sh blind. Mode A is the most
likely root cause and the fix shape is well understood, but landing
linuxdeploy in CI without an iterative test on a real Linux runner
(can't validate from macOS) risks shipping a release that fails in a
new way. When the reporter's logs surface, we'll know which one of
A-G to fix first and can land the corresponding workflow change in a
verifiable PR.
