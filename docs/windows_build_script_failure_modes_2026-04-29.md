# Windows PowerShell `build.ps1` Failure Modes — Preliminary Examination (2026-04-29)

**Issue:** [#12 Windows PowerShell build script doesn't work for some
users](https://github.com/JRickey/BattleShip/issues/12). Reported user
profile: Windows + MSVC installed. No further detail. This document
audits `build.ps1` against common Windows-specific pitfalls and
highlights the likeliest failure points so that when reproducer
details land we can short-list the cause quickly.

## Script under review

`build.ps1` (project root, 261 lines). It does, in order:
1. Param parsing + ROM probe.
2. `git submodule update --init --recursive`.
3. Three Python codegen steps (`generate_reloc_stubs.py`,
   `generate_yamls.py`, `generate_reloc_table.py`).
4. Four credits-encoder Python invocations.
5. `cmake -S $Root -B $BuildDir` (no generator specified).
6. `cmake --build $BuildDir --target ssb64 --config $Config`.
7. Build Torch via the ExternalProject target, run torch.exe to
   produce `BattleShip.o2r`.
8. Manual ZIP archive of the Fast3D shader directory into `f3d.o2r`
   using `System.IO.Compression.ZipArchive`.
9. Copy both `.o2r` files next to the built exe.

## Failure modes ranked by likelihood

### A — PowerShell execution policy blocks the unsigned script

Default Windows policy is **`Restricted`** (or `RemoteSigned` on some
SKUs). Running `.\build.ps1` produces:

```
File ...\build.ps1 cannot be loaded because running scripts is disabled on this system.
```

This is the single most common Windows-PowerShell-script-doesn't-run
report. Workarounds the user needs:

```powershell
# session-scoped, doesn't persist
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\build.ps1

# or, single invocation
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

The script has no signing block, so signing isn't an option.
**Recommended fix:** add a one-line README / build doc note
("`If you see 'running scripts is disabled', start with
`Set-ExecutionPolicy -Scope Process Bypass`'"), or detect at script
top and print a clearer error.

### B — `python` not on PATH / WSA stub instead

`build.ps1` calls `python` (line 101, 105, 111, 128, 132). The bash
counterpart calls `python3`. On Windows:

- **Microsoft Store stub**: a fresh Windows install ships a
  zero-byte `python.exe` in `%LOCALAPPDATA%\Microsoft\WindowsApps\`
  that opens the Store when invoked. It returns exit code 9009 and
  no stdout. The script's `if ($LASTEXITCODE -ne 0)` would fire
  with a generic "reloc_data.h generation failed".
- **python.org installer with "Add to PATH" unchecked**: only
  `py.exe` is on PATH; `python` is not.
- **Anaconda not activated**: `conda activate` not run, `python`
  unresolved.

If the user installed Python via the python.org installer with default
options, `python` works. If via the Store stub, it fails silently. If
via Anaconda without activation, it fails.

**Recommended fix:** probe `python --version` at script top, fall back
to `py -3` if the first fails; print an explicit "Python 3 is required
and `python` was not found / pointed at the Microsoft Store stub" if
neither works.

### C — `cmake` invokes a non-MSVC generator behind the user's back

Line 143: `cmake -S $Root -B $BuildDir` does **not** specify
`-G "Visual Studio 17 2022"` (or whatever the user has). CMake's
default-generator selection on Windows is:

1. If `CMAKE_GENERATOR` env var is set, use it.
2. Else, try the *highest installed* Visual Studio.
3. Else, fall back to NMake / MinGW Makefiles depending on what's on
   PATH.

So a user with **MSVC installed via Build Tools (no full VS)** plus
**Ninja on PATH** (e.g., installed via vcpkg, scoop, Chocolatey) gets
a Ninja build, which is single-config. The script's `--config $Config`
does nothing for Ninja, the build dir layout becomes
`$BuildDir\BattleShip.exe` instead of `$BuildDir\$Config\BattleShip.exe`,
and:

- `Test-Path $GameExe` fails → "Game built" never prints.
- `Copy-Item $O2R (Join-Path $ExeDir "BattleShip.o2r")` quietly skips
  because `$ExeDir` doesn't exist.
- The user runs `BattleShip.exe`, which boots from the build dir but
  finds no `BattleShip.o2r`, hits the wizard flow, drops a different
  copy of o2r into `~\Library\Application Support\BattleShip\` (well,
  the Windows AppData equivalent), and may or may not realize anything
  went wrong.

**Recommended fix:** explicitly pass `-G` based on a probe (e.g., look
for `cl.exe`, then `vswhere`, then default to "Visual Studio 17
2022"), OR fail early if the configured generator is single-config and
print "this build script assumes a multi-config generator".

### D — Long path / OneDrive / Defender interactions

Windows `MAX_PATH` is 260 characters by default. A submodule clone
(libultraship has its own submodules) deep inside an existing
nested-path checkout (`C:\Users\Foo\OneDrive - SomeOrg\Documents\
GitHub\BattleShip\libultraship\extern\imgui\misc\fonts\droid_sans...`
or similar) blows past 260 characters in `git`'s own internal pack
unpacking and aborts with `Filename too long`.

Mitigations:
- `git config --global core.longpaths true` (only works if Win10 has
  the longpath registry key flipped).
- Move the checkout to a shallow path like `C:\src\ssb64\`.

OneDrive-mirrored folders also trigger random "file in use" errors
when OneDrive's sync agent has a build artifact open at the moment
CMake tries to write it. Defender real-time scan does the same for
the freshly-built torch.exe.

**Recommended fix:** the README / build doc should explicitly
recommend a short path outside OneDrive. Also potentially detect
`$env:OneDrive` and warn.

### E — `cmake --build` without VS Developer environment

When CMake picks the Visual Studio generator, `cmake --build` invokes
**msbuild.exe**. msbuild is shipped under each VS install (e.g.
`C:\Program Files\Microsoft Visual Studio\2022\BuildTools\
MSBuild\Current\Bin\msbuild.exe`) and is **not** added to a normal
PowerShell `$Env:Path` by default — only the "Developer PowerShell for
VS 2022" shortcut sets it up via `Launch-VsDevShell.ps1`.

Symptoms:
- `cmake --build` falls back to whatever `msbuild` is on PATH.
- Without VS env activated, `msbuild` may resolve to:
  - A stale `.NET Framework` msbuild (`C:\Windows\Microsoft.NET\
    Framework64\v4.0.30319\MSBuild.exe`), which can build but doesn't
    pull in the right toolchain → cl.exe missing, fail.
  - Nothing → "msbuild is not recognized", build fails.

CMake's VS generator usually invokes msbuild via a full path through
the VS install probed at configure time, so this is **less common**
than I initially thought, but I've seen it bite users with multiple
VS versions where CMake configure picked one and `cmake --build`
picked another.

### F — `USE_AUTO_VCPKG=ON` first-run vcpkg bootstrap fails

`CMakeLists.txt:25` defaults `USE_AUTO_VCPKG=ON`. That triggers a
vcpkg manifest-mode dep install at configure time, which on Windows
needs:

- Internet access to GitHub (vcpkg clones).
- `cmake.exe` already on PATH (yes, since we're running CMake).
- Sometimes Visual Studio English language pack (vcpkg toolchain
  scripts have hit issues with non-English locales).

A corporate firewall blocking GitHub or vcpkg's binary cache CDN can
make this step take 10+ minutes or fail with non-obvious errors.

**Recommended fix:** if reproduction shows vcpkg failures, document
the manual workaround (`-DUSE_AUTO_VCPKG=OFF` + use system vcpkg).

### G — `git submodule update --init --recursive` over SSH without keys

The repo uses SSH submodule URLs (per `CLAUDE.md`'s worktree workflow
note about pushing to `JRickey/libultraship`). Users who clone with
HTTPS and don't override the submodule URLs hit auth prompts on every
submodule. If they cloned the SSH URL but their SSH agent isn't
running on Windows, the init hangs or fails.

**Recommended fix:** the bash script has the same issue. Document
HTTPS clone via `.gitconfig`'s `insteadOf` rewrite, or ship a
`url.https://github.com/.insteadOf = git@github.com:` config locally.

### H — `Add-Type -AssemblyName System.IO.Compression` flaky on PS7

PowerShell 5.1 (built into Windows) ships `System.IO.Compression` as
part of the .NET Framework GAC. PS7 ships .NET Core / .NET 8, where
`System.IO.Compression.FileSystem` was merged into the runtime. The
script does both `Add-Type` calls (lines 220-221), which works on PS5
but on PS7 may print a "type already loaded" warning that's harmless,
or, in older PS7 betas, a hard error.

Less likely than A/B/C but worth noting if the user's reproducer is
PS7-specific.

## Quick triage when the reproducer comes in

When the user replies with `$PSVersionTable` + error log, walk down
this checklist in order:

| Symptom in error log | Likely cause |
|----------------------|--------------|
| "running scripts is disabled" | A — execution policy |
| Generic "reloc_data.h generation failed", no Python output | B — python missing / Store stub |
| "ninja: error" or build dir layout has no `Debug\`/`Release\` | C — Ninja picked behind back |
| "Filename too long" during `git submodule` | D — long path |
| "msbuild is not recognized" | E — VS Dev env |
| vcpkg / fetch errors during `cmake -S -B` | F — vcpkg bootstrap |
| `Permission denied (publickey)` during submodule | G — SSH auth |
| "Add-Type: failed to load type" | H — PS7 |

## Followup actions (not done in this pass)

- `build.ps1` should auto-detect Python (probe `python --version` then
  `py -3 --version`).
- `build.ps1` should explicitly pass `-G` to CMake or fail loudly when
  it picks Ninja with this script's multi-config assumptions.
- README / build doc on Windows needs an "if you see X, do Y" section
  for the top three failure modes.
- Add a `--diagnose` switch to `build.ps1` that prints `$PSVersionTable`,
  `where.exe python`, `where.exe cmake`, `where.exe msbuild`,
  `git --version`, `cmake --version`, and the chosen generator, so
  reporters can paste it into the issue verbatim.

## What I am NOT doing

Patching `build.ps1` blindly. Each fix above changes behavior; without
the user's reproducer (which environment did fail, what error
message), a speculative fix could mask the real cause or break a
working setup. The triage table above is the actionable artifact —
when the reporter responds, this lets us go from "open issue" to "PR
that fixes the right thing" quickly.
