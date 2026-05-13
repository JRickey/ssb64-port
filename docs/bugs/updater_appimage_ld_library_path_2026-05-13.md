# Updater AppImage loader path poisoning — Fedora 44 KDE

**Status:** RESOLVED 2026-05-13

**Issue:** [#174 Updater doesn't work in Fedora 44 KDE](https://github.com/JRickey/BattleShip/issues/174)

**Where:** `port/enhancements/Updater.cpp`, `port/gui/PortMenu.cpp`

## Symptom

The built-in updater did not reach GitHub from the Linux AppImage on Fedora 44 KDE. The reporter saw system `curl` fail before any network request:

```text
curl: /tmp/.mount_Battle.../usr/lib/libcrypto.so.3: version `OPENSSL_3.3.0' not found
curl: /tmp/.mount_Battle.../usr/lib/libcrypto.so.3: version `OPENSSL_3.4.0' not found
```

The About menu then presented the failed check as if the game were up to date.

## Root Cause

The AppImage `AppRun` script prepends its bundled library directory to `LD_LIBRARY_PATH` so `BattleShip` and `torch` resolve against the libraries packaged with the release. That environment is correct for the game process, but poisonous for host tools launched from inside the AppImage.

The updater used plain `popen("curl ...")` for both the release check and the artifact download. On Fedora, `/usr/bin/curl` loaded the AppImage's bundled `libcrypto.so.3` from `/tmp/.mount_.../usr/lib` while also resolving the host distro's `libssl.so.3`. The host `libssl` expected newer OpenSSL symbol versions than the bundled `libcrypto` provided, so `curl` died during dynamic loading.

`port/native_dialog.cpp` had already fixed the same class for `zenity` / `kdialog` by launching them under `env -u LD_LIBRARY_PATH -u LD_PRELOAD`. The updater was missing that guard.

Two adjacent updater bugs made the failure mode worse:

- The update check queried `/tags` and trusted the first tag, even when no release asset existed for the platform.
- Downloads used `curl -L` without `--fail`, so a 404 or rate-limit HTML page could be treated as a successful update payload.

## Fix

- Route Linux updater `curl` and `xdg-open` commands through `env -u LD_LIBRARY_PATH -u LD_PRELOAD`.
- Query GitHub's `releases/latest` API instead of `/tags`, and use the matching platform asset's `browser_download_url`.
- Add `curl -f` to check and download commands so HTTP errors produce non-zero exits.
- Track update-check status separately from download status so network/API/parse failures are visible in the About menu instead of appearing as "Up to date."
- Keep PR #175's Windows no-console path by running updater subprocesses through `CreateProcessA(..., CREATE_NO_WINDOW, ...)` and using `ShellExecuteA` for browser / updater-script launches.

## Verification

- Clean macOS Debug build:

```bash
cmake --build /Users/jackrickey/Dev/ssb64-port/.claude/worktrees/updater-fixes-174/build --target ssb64 -j 8
```

The first attempt failed only because the sandbox blocked Torch's FetchContent clone of `libgfxd`; rerunning with network access completed successfully.

Linux AppImage runtime behavior still needs verification on Fedora 44 KDE because this worktree was built on macOS.

## Audit Hooks

- Any AppImage code that spawns host tools must strip `LD_LIBRARY_PATH` and `LD_PRELOAD` unless the child intentionally needs bundled libraries.
- Updaters should consume release metadata, not raw tags, when downloadable assets are required.
- Curl-based downloads must use `-f` or equivalent and should validate the artifact before replacing an executable.
