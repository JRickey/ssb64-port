# build.ps1 - Build SSB64 port and extract assets (Windows)
#
# Usage:
#   .\build.ps1                # Full build + extract (Debug)
#   .\build.ps1 -SkipExtract   # Build only, skip asset extraction
#   .\build.ps1 -ExtractOnly   # Extract assets only (assumes Torch already built)
#   .\build.ps1 -Clean         # Clean build from scratch
#   .\build.ps1 -Release       # Release config (default: Debug)
#   .\build.ps1 -Jobs 8        # Parallel build job count (default: NUMBER_OF_PROCESSORS)
#   .\build.ps1 -Diagnose      # Print environment diagnostics and exit (paste into bug reports)
#   .\build.ps1 -Help          # Show this help

param(
    [switch]$SkipExtract,
    [switch]$ExtractOnly,
    [switch]$Clean,
    [switch]$Release,
    [switch]$Debug,
    [int]$Jobs = 0,
    [switch]$Diagnose,
    [switch]$Help
)

if ($Help) {
    Get-Content $MyInvocation.MyCommand.Path | Select-Object -First 12 | ForEach-Object { Write-Host $_ }
    exit 0
}

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $MyInvocation.MyCommand.Path

$Config = if ($Release) { "Release" } else { "Debug" }
if ($Jobs -le 0) {
    $Jobs = [int]($env:NUMBER_OF_PROCESSORS)
    if ($Jobs -le 0) { $Jobs = 4 }
}

$BuildDir = Join-Path $Root "build"
$ROM = $null
foreach ($ext in @("z64", "n64", "v64")) {
    $candidate = Join-Path $Root "baserom.us.$ext"
    if (Test-Path $candidate) { $ROM = $candidate; break }
}
if (-not $ROM) { $ROM = Join-Path $Root "baserom.us.z64" }
$O2R = Join-Path $Root "BattleShip.o2r"
$F3DO2R = Join-Path $Root "f3d.o2r"
$Fast3DShaderDir = Join-Path $Root "libultraship\src\fast\shaders"
$TorchExe = Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\$Config\torch.exe"
$GameExe = Join-Path $BuildDir "$Config\BattleShip.exe"
$ExeDir = Split-Path $GameExe -Parent

function Write-Step($msg) {
    Write-Host "`n=== $msg ===" -ForegroundColor Cyan
}

function Write-Warn($msg) {
    Write-Host "WARNING: $msg" -ForegroundColor Yellow
}

function Get-CommandSourceOrNull($name) {
    $cmd = Get-Command $name -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source } else { return $null }
}

# Probe `python --version` then `py -3 --version`. Returns a hashtable with
# keys: Cmd (string[]), Version (string). Exits with a clear error message
# when neither is a working Python 3 — including a specific call-out for
# the Microsoft Store stub (a zero-byte python.exe in WindowsApps that
# launches the Store and exits 9009 with no stdout).
function Resolve-Python {
    # 1. Try `python` directly.
    $pythonPath = Get-CommandSourceOrNull 'python'
    $isStoreStub = $pythonPath -and ($pythonPath -like '*WindowsApps*')

    if ($pythonPath -and -not $isStoreStub) {
        $verRaw = & python --version 2>&1
        if ($LASTEXITCODE -eq 0 -and "$verRaw" -match 'Python 3\.') {
            return @{ Cmd = @('python'); Version = "$verRaw"; Source = $pythonPath }
        }
    }

    # 2. Try `py -3` (python.org launcher; preferred fallback when only `py`
    #    is on PATH or `python` is the Store stub).
    $pyPath = Get-CommandSourceOrNull 'py'
    if ($pyPath) {
        $verRaw = & py -3 --version 2>&1
        if ($LASTEXITCODE -eq 0 -and "$verRaw" -match 'Python 3\.') {
            return @{ Cmd = @('py', '-3'); Version = "$verRaw"; Source = $pyPath }
        }
    }

    # No working Python 3. Print an actionable error.
    Write-Host "ERROR: Python 3 not found." -ForegroundColor Red
    if ($isStoreStub) {
        Write-Host "  'python' on PATH points to the Microsoft Store stub:" -ForegroundColor Red
        Write-Host "    $pythonPath" -ForegroundColor Red
        Write-Host "  This zero-byte stub opens the Store rather than running Python." -ForegroundColor Red
        Write-Host "  Either:" -ForegroundColor Red
        Write-Host "    (a) install Python 3 from https://www.python.org/downloads/ and tick" -ForegroundColor Red
        Write-Host "        'Add Python to PATH' in the installer, or" -ForegroundColor Red
        Write-Host "    (b) disable the Store stub at" -ForegroundColor Red
        Write-Host "        Settings > Apps > Advanced app settings > App execution aliases" -ForegroundColor Red
        Write-Host "        (uncheck 'python.exe' and 'python3.exe')." -ForegroundColor Red
    } else {
        Write-Host "  Tried: 'python', 'py -3'. Neither found a working Python 3." -ForegroundColor Red
        Write-Host "  Install Python 3 from https://www.python.org/downloads/ and tick" -ForegroundColor Red
        Write-Host "  'Add Python to PATH' in the installer, then re-run build.ps1." -ForegroundColor Red
    }
    Write-Host "  Run '.\build.ps1 -Diagnose' for a full environment dump." -ForegroundColor Red
    exit 1
}

# Invoke whatever Python the script is configured to use, splatting any
# additional args. Set $script:Python earlier via Resolve-Python.
function Invoke-Python {
    $py = $script:Python.Cmd
    $head = $py[0]
    $tail = if ($py.Length -gt 1) { $py[1..($py.Length - 1)] } else { @() }
    & $head @tail @args
}

# Read a single string value from CMakeCache.txt (e.g. "CMAKE_GENERATOR:INTERNAL").
# Returns $null if the cache or the key is missing.
function Get-CMakeCacheValue($cacheFile, $key) {
    if (-not (Test-Path $cacheFile)) { return $null }
    $line = Select-String -Path $cacheFile -Pattern "^$([regex]::Escape($key))[^=]*=" -SimpleMatch:$false |
            Select-Object -First 1
    if (-not $line) { return $null }
    return ($line.Line -split '=', 2)[1]
}

# -Diagnose: dump environment + tool versions + repo path + (if present)
# the CMake cache's generator/compiler choice. Exits without building.
# Designed so users can paste the output verbatim into bug reports.
if ($Diagnose) {
    Write-Step "Environment"
    Write-Host "PSVersionTable:"
    $PSVersionTable | Format-Table | Out-String | Write-Host
    Write-Host "Execution policy (per scope):"
    Get-ExecutionPolicy -List | Format-Table | Out-String | Write-Host

    Write-Step "Tools"
    foreach ($tool in @('python', 'py', 'cmake', 'msbuild', 'cl', 'git', 'ninja')) {
        $src = Get-CommandSourceOrNull $tool
        if (-not $src) {
            Write-Host ("  {0,-9} : NOT FOUND" -f $tool)
            continue
        }
        $isStub = ($tool -eq 'python') -and ($src -like '*WindowsApps*')
        if ($isStub) {
            Write-Host ("  {0,-9} : {1}  [Microsoft Store stub — Python WILL NOT RUN]" -f $tool, $src) -ForegroundColor Yellow
            continue
        }
        $verArgs = if ($tool -eq 'msbuild') { @('-version') } else { @('--version') }
        $ver = $null
        try { $ver = (& $tool @verArgs 2>&1 | Select-Object -First 1) } catch { $ver = "(error: $_)" }
        Write-Host ("  {0,-9} : {1}" -f $tool, $src)
        Write-Host ("            {0}" -f $ver)
    }

    Write-Step "Repository"
    Write-Host "  Path:       $Root"
    Write-Host "  Length:     $($Root.Length) chars"
    if ($env:OneDrive -and ($Root.StartsWith($env:OneDrive, [StringComparison]::OrdinalIgnoreCase))) {
        Write-Warn "Repo is inside OneDrive ($env:OneDrive). OneDrive sync can lock files mid-build."
        Write-Host "  Move to a non-synced path like C:\src\ssb64\ for reliable builds." -ForegroundColor Yellow
    }
    if ($Root.Length -gt 100) {
        Write-Warn "Repo path is long ($($Root.Length) chars). Submodule clones may hit Windows MAX_PATH (260)."
        Write-Host "  Move to a short path like C:\src\ssb64\ if 'Filename too long' errors appear." -ForegroundColor Yellow
    }

    Write-Step "CMake cache (if configured)"
    $cacheFile = Join-Path $BuildDir "CMakeCache.txt"
    if (Test-Path $cacheFile) {
        Write-Host "  Cache:      $cacheFile"
        foreach ($k in @('CMAKE_GENERATOR:INTERNAL', 'CMAKE_GENERATOR_PLATFORM:INTERNAL',
                         'CMAKE_C_COMPILER:FILEPATH', 'CMAKE_CXX_COMPILER:FILEPATH',
                         'CMAKE_LINKER:FILEPATH', 'CMAKE_MAKE_PROGRAM:FILEPATH')) {
            $v = Get-CMakeCacheValue $cacheFile $k
            if ($v) { Write-Host ("  {0,-36} = {1}" -f $k, $v) }
        }
        $gen = Get-CMakeCacheValue $cacheFile 'CMAKE_GENERATOR:INTERNAL'
        if ($gen -and ($gen -notlike 'Visual Studio*')) {
            Write-Warn "Generator '$gen' is single-config — build.ps1 assumes a Visual Studio multi-config layout."
            Write-Host "  Run '.\build.ps1 -Clean' then re-build to force re-configure with the default generator." -ForegroundColor Yellow
            Write-Host "  Or set `$env:CMAKE_GENERATOR='Visual Studio 17 2022' before running build.ps1." -ForegroundColor Yellow
        }
    } else {
        Write-Host "  No build cache yet (build.ps1 has not been run)." -ForegroundColor DarkGray
    }

    exit 0
}

# Resolve Python BEFORE any codegen step, so users hit a clear error early
# instead of "reloc_data.h generation failed" with no further context.
$script:Python = $null
if (-not $ExtractOnly) {
    $script:Python = Resolve-Python
}

# Surface long-path / OneDrive concerns at every run (not just -Diagnose).
# These don't fail the build outright — they just steer triage when the build
# does fail downstream.
if ($env:OneDrive -and ($Root.StartsWith($env:OneDrive, [StringComparison]::OrdinalIgnoreCase))) {
    Write-Warn "Repo is inside OneDrive ($env:OneDrive)."
    Write-Host "  OneDrive sync can lock build artifacts; consider moving to C:\src\ssb64\." -ForegroundColor Yellow
}
if ($Root.Length -gt 100) {
    Write-Warn "Repo path is $($Root.Length) chars. Long paths trip 'Filename too long' in submodules."
    Write-Host "  If submodule init fails, move the checkout to a shorter path (e.g. C:\src\ssb64\)." -ForegroundColor Yellow
}

# ── Clean ──
if ($Clean) {
    Write-Step "Cleaning build directory"
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
    }
    if (Test-Path $O2R) {
        Remove-Item -Force $O2R
    }
    if (Test-Path $F3DO2R) {
        Remove-Item -Force $F3DO2R
    }
}

# ── Validate ROM ──
if (-not (Test-Path $ROM)) {
    Write-Host "ERROR: ROM not found at $ROM" -ForegroundColor Red
    Write-Host "Place your NTSC-U v1.0 ROM in the project root as baserom.us.z64,"
    Write-Host "baserom.us.n64, or baserom.us.v64 (Torch will normalize byte order)."
    exit 1
}

# ── Submodules ──
if (-not $ExtractOnly) {
    Write-Step "Initializing submodules"
    git -C $Root submodule update --init --recursive
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Submodule init failed (exit code $LASTEXITCODE)" -ForegroundColor Red
        Write-Host "Common Windows causes:" -ForegroundColor Red
        Write-Host "  - 'Filename too long' on a deeply-nested checkout: move to C:\src\ssb64\." -ForegroundColor Red
        Write-Host "  - 'Permission denied (publickey)' on SSH submodules: SSH agent not running." -ForegroundColor Red
        Write-Host "    Switch to HTTPS via 'git config --global url.https://github.com/.insteadOf git@github.com:'" -ForegroundColor Red
        Write-Host "  - HTTPS auth prompts: run 'git config --global credential.helper manager-core'." -ForegroundColor Red
        Write-Host "Run '.\build.ps1 -Diagnose' for an environment dump." -ForegroundColor Red
        exit 1
    }
}

# ── Generate reloc_data.h, Torch YAML configs, RelocFileTable.cpp ──
# All three are downstream of tools/reloc_data_symbols.us.txt and the ROM.
#
# Pipeline:
#   reloc_data_symbols.us.txt
#     -> generate_reloc_stubs.py  -> include/reloc_data.h
#          -> generate_yamls.py   -> yamls/us/reloc_*.yml       (gitignored)
#               -> generate_reloc_table.py -> port/resource/RelocFileTable.cpp
#                    -> Torch               -> ssb64.o2r
#
# reloc_data.h + yamls/us/reloc_*.yml are gitignored and must be rebuilt on
# every fresh checkout. RelocFileTable.cpp is committed, but we still
# regenerate it here so it stays in lock-step with whatever the YAML
# generator emitted — if the two ever disagree at runtime, the resource
# loader falls back to the file_NNNN fallback names and every fighter /
# sprite / icon lookup silently returns NULL.
if (-not $ExtractOnly) {
    Write-Host "Using Python: $($script:Python.Cmd -join ' ')  ($($script:Python.Version))" -ForegroundColor DarkGray

    Write-Step "Regenerating include/reloc_data.h"
    Invoke-Python "$Root\tools\generate_reloc_stubs.py"
    if ($LASTEXITCODE -ne 0) { Write-Host "reloc_data.h generation failed (run '.\build.ps1 -Diagnose' for env)" -ForegroundColor Red; exit 1 }

    Write-Step "Regenerating Torch YAML extraction configs"
    Invoke-Python "$Root\tools\generate_yamls.py"
    if ($LASTEXITCODE -ne 0) { Write-Host "generate_yamls.py failed (run '.\build.ps1 -Diagnose' for env)" -ForegroundColor Red; exit 1 }

    Write-Step "Regenerating port\resource\RelocFileTable.cpp"
    Push-Location $Root
    try {
        Invoke-Python "$Root\tools\generate_reloc_table.py"
        if ($LASTEXITCODE -ne 0) { Write-Host "generate_reloc_table.py failed (run '.\build.ps1 -Diagnose' for env)" -ForegroundColor Red; exit 1 }
    } finally { Pop-Location }
}

# ── Encode credits text ──
# staff/titles/info/companies credit strings are #include'd directly
# into scstaffroll.c via .credits.encoded / .credits.metadata files,
# which are gitignored. Every fresh checkout has to rerun the encoder.
# staff/titles use the default title font; info/companies need the
# paragraph font for digits, punctuation and accents. The tool is
# idempotent — it overwrites the outputs unconditionally.
if (-not $ExtractOnly) {
    Write-Step "Encoding credits text"
    Push-Location (Join-Path $Root "src\credits")
    try {
        foreach ($name in @("staff.credits.us.txt", "titles.credits.us.txt")) {
            Invoke-Python "$Root\tools\creditsTextConverter.py" $name | Out-Null
            if ($LASTEXITCODE -ne 0) { Write-Host "credits encode failed: $name (run '.\build.ps1 -Diagnose' for env)" -ForegroundColor Red; exit 1 }
        }
        foreach ($name in @("info.credits.us.txt", "companies.credits.us.txt")) {
            Invoke-Python "$Root\tools\creditsTextConverter.py" -paragraphFont $name | Out-Null
            if ($LASTEXITCODE -ne 0) { Write-Host "credits encode failed: $name (run '.\build.ps1 -Diagnose' for env)" -ForegroundColor Red; exit 1 }
        }
    } finally {
        Pop-Location
    }
}

# ── CMake configure ──
if (-not $ExtractOnly) {
    Write-Step "Configuring CMake ($Config)"
    cmake -S $Root -B $BuildDir
    if ($LASTEXITCODE -ne 0) {
        Write-Host "CMake configure failed (exit code $LASTEXITCODE)" -ForegroundColor Red
        Write-Host "Common Windows causes:" -ForegroundColor Red
        Write-Host "  - No supported compiler found: install Visual Studio 2022 (Build Tools is enough)" -ForegroundColor Red
        Write-Host "    with the 'Desktop development with C++' workload." -ForegroundColor Red
        Write-Host "  - vcpkg auto-bootstrap failure: re-run with '-DUSE_AUTO_VCPKG=OFF' in CMake," -ForegroundColor Red
        Write-Host "    or check that GitHub.com is reachable from this machine." -ForegroundColor Red
        Write-Host "Run '.\build.ps1 -Diagnose' for an environment dump." -ForegroundColor Red
        exit 1
    }

    # Sanity-check the picked generator. CMake's default-generator selection
    # on Windows can pick Ninja (single-config) if it's on PATH — e.g. when
    # vcpkg / scoop / Chocolatey installs Ninja. This script's path layout
    # ($BuildDir\$Config\BattleShip.exe) only works for multi-config Visual
    # Studio generators. Warn loudly if the cache says otherwise so the user
    # at least knows where the exe actually lives.
    $cacheFile = Join-Path $BuildDir "CMakeCache.txt"
    $generator = Get-CMakeCacheValue $cacheFile 'CMAKE_GENERATOR:INTERNAL'
    if ($generator) {
        Write-Host "  Generator: $generator" -ForegroundColor DarkGray
        if ($generator -notlike 'Visual Studio*') {
            Write-Warn "Generator '$generator' is single-config; build.ps1's path layout assumes Visual Studio."
            Write-Host "  The exe will land at $BuildDir\BattleShip.exe (not $BuildDir\$Config\BattleShip.exe)" -ForegroundColor Yellow
            Write-Host "  and the auto-copy of *.o2r next to the exe may be skipped." -ForegroundColor Yellow
            Write-Host "  To force the Visual Studio generator, run:" -ForegroundColor Yellow
            Write-Host "    .\build.ps1 -Clean" -ForegroundColor Yellow
            Write-Host "    `$env:CMAKE_GENERATOR='Visual Studio 17 2022'" -ForegroundColor Yellow
            Write-Host "    .\build.ps1" -ForegroundColor Yellow

            # Adjust the path layout assumptions so the rest of the script
            # at least produces a working build.
            $script:GameExe = Join-Path $BuildDir "BattleShip.exe"
            $script:ExeDir = $BuildDir
            $GameExe = $script:GameExe
            $ExeDir = $script:ExeDir
        }
    }
}

# ── Build game ──
if (-not $ExtractOnly) {
    Write-Step "Building ssb64 ($Config, j=$Jobs)"
    cmake --build $BuildDir --target ssb64 --config $Config --parallel $Jobs
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Game build failed (exit code $LASTEXITCODE)" -ForegroundColor Red
        Write-Host "Common Windows causes:" -ForegroundColor Red
        Write-Host "  - msbuild not found / wrong msbuild on PATH: run from a 'Developer PowerShell" -ForegroundColor Red
        Write-Host "    for VS 2022' window, or run 'Launch-VsDevShell.ps1' to set up the env." -ForegroundColor Red
        Write-Host "  - Compile errors: scroll up for the first cl.exe / clang error before this line." -ForegroundColor Red
        Write-Host "Run '.\build.ps1 -Diagnose' for an environment dump." -ForegroundColor Red
        exit 1
    }
    Write-Host "Game built: $GameExe" -ForegroundColor Green
}

# ── Build Torch + Extract assets ──
if (-not $SkipExtract) {
    # Build Torch via ExternalProject
    Write-Step "Building Torch ($Config, j=$Jobs)"
    cmake --build $BuildDir --target TorchExternal --config $Config --parallel $Jobs
    if ($LASTEXITCODE -ne 0) { Write-Host "Torch build failed" -ForegroundColor Red; exit 1 }

    # Try several known torch.exe locations (mirrors build.sh fallback list)
    $TorchExe = $null
    foreach ($cand in @(
        (Join-Path $BuildDir "torch-install\bin\torch.exe"),
        (Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\$Config\torch.exe"),
        (Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\Debug\torch.exe"),
        (Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\Release\torch.exe"),
        (Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\torch.exe")
    )) {
        if (Test-Path $cand) { $TorchExe = $cand; break }
    }

    if (-not $TorchExe) {
        Write-Host "ERROR: torch.exe not found after build" -ForegroundColor Red
        exit 1
    }

    Write-Step "Extracting assets from ROM"
    Push-Location $Root
    & $TorchExe o2r $ROM 2>&1 | ForEach-Object { $_ }
    $exitCode = $LASTEXITCODE
    Pop-Location

    if ($exitCode -ne 0) {
        Write-Host "Asset extraction failed (exit code $exitCode)" -ForegroundColor Red
        exit 1
    }

    if (-not (Test-Path $O2R)) {
        Write-Host "ERROR: BattleShip.o2r was not created" -ForegroundColor Red
        exit 1
    }

    $o2rSize = (Get-Item $O2R).Length / 1MB
    Write-Host ("Assets extracted: BattleShip.o2r ({0:N1} MB)" -f $o2rSize) -ForegroundColor Green

    # Copy o2r next to exe
    if ((Test-Path $ExeDir) -and ($ExeDir -ne $Root)) {
        Copy-Item $O2R (Join-Path $ExeDir "BattleShip.o2r") -Force
        Write-Host "Copied BattleShip.o2r to $ExeDir"
    }
}

# ── Package Fast3D shader archive ──
Write-Step "Packaging Fast3D shader archive"
if (-not (Test-Path $Fast3DShaderDir)) {
    Write-Host "ERROR: Fast3D shader directory not found at $Fast3DShaderDir" -ForegroundColor Red
    exit 1
}

if (Test-Path $F3DO2R) {
    Remove-Item -Force $F3DO2R
}

# The resource manager loads shaders as "shaders/opengl/...", "shaders/directx/...", etc.
# ZipFile::CreateFromDirectory on Windows stores entries with backslashes, which breaks
# zip_name_locate (exact forward-slash match). Write entries individually with explicit
# forward-slash paths using ZipArchive.
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$stream = [System.IO.File]::Open($F3DO2R, [System.IO.FileMode]::Create)
$zip = New-Object System.IO.Compression.ZipArchive($stream, [System.IO.Compression.ZipArchiveMode]::Create)
Get-ChildItem -Recurse -File $Fast3DShaderDir | ForEach-Object {
    $rel = "shaders/" + $_.FullName.Substring($Fast3DShaderDir.Length + 1).Replace('\', '/')
    $entry = $zip.CreateEntry($rel)
    $es = $entry.Open()
    $fs = [System.IO.File]::OpenRead($_.FullName)
    $fs.CopyTo($es)
    $fs.Close()
    $es.Close()
}
$zip.Dispose()
$stream.Close()

if (-not (Test-Path $F3DO2R)) {
    Write-Host "ERROR: f3d.o2r was not created" -ForegroundColor Red
    exit 1
}

$f3dSizeKB = [math]::Round((Get-Item $F3DO2R).Length / 1KB, 1)
Write-Host ("Packaged f3d.o2r ({0:N1} KB)" -f $f3dSizeKB) -ForegroundColor Green

if ((Test-Path $ExeDir) -and ($ExeDir -ne $Root)) {
    Copy-Item $F3DO2R (Join-Path $ExeDir "f3d.o2r") -Force
    Write-Host "Copied f3d.o2r to $ExeDir"
}

# ── Done ──
Write-Host "`n" -NoNewline
Write-Step "Build complete"
if (Test-Path $GameExe) {
    Write-Host "  Executable: $GameExe"
}
if (Test-Path $O2R) {
    Write-Host "  Assets:     $O2R"
}
if (Test-Path $F3DO2R) {
    Write-Host "  Fast3D:     $F3DO2R"
}
Write-Host ""
