# build.ps1 - Build SSB64 port and extract assets
#
# Usage:
#   .\build.ps1              # Full build + extract
#   .\build.ps1 -SkipExtract # Build only, skip asset extraction
#   .\build.ps1 -ExtractOnly # Extract assets only (assumes Torch already built)
#   .\build.ps1 -Clean       # Clean build from scratch

param(
    [switch]$SkipExtract,
    [switch]$ExtractOnly,
    [switch]$Clean
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $Root "build"
$ROM = Join-Path $Root "baserom.us.z64"
$O2R = Join-Path $Root "ssb64.o2r"
$F3DO2R = Join-Path $Root "f3d.o2r"
$Fast3DShaderDir = Join-Path $Root "libultraship\src\fast\shaders"
$TorchExe = Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\Debug\torch.exe"
$GameExe = Join-Path $BuildDir "Debug\ssb64.exe"
$ExeDir = Split-Path $GameExe -Parent

function Write-Step($msg) {
    Write-Host "`n=== $msg ===" -ForegroundColor Cyan
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
    Write-Host "Place your NTSC-U v1.0 ROM as baserom.us.z64 in the project root."
    exit 1
}

# ── Submodules ──
if (-not $ExtractOnly) {
    Write-Step "Initializing submodules"
    git -C $Root submodule update --init --recursive
    if ($LASTEXITCODE -ne 0) { Write-Host "Submodule init failed" -ForegroundColor Red; exit 1 }
}

# ── CMake configure ──
if (-not $ExtractOnly) {
    Write-Step "Configuring CMake"
    cmake -S $Root -B $BuildDir
    if ($LASTEXITCODE -ne 0) { Write-Host "CMake configure failed" -ForegroundColor Red; exit 1 }
}

# ── Build game ──
if (-not $ExtractOnly) {
    Write-Step "Building ssb64"
    cmake --build $BuildDir --target ssb64 --config Debug
    if ($LASTEXITCODE -ne 0) { Write-Host "Game build failed" -ForegroundColor Red; exit 1 }
    Write-Host "Game built: $GameExe" -ForegroundColor Green
}

# ── Build Torch + Extract assets ──
if (-not $SkipExtract) {
    # Build Torch via ExternalProject
    Write-Step "Building Torch"
    cmake --build $BuildDir --target TorchExternal
    if ($LASTEXITCODE -ne 0) { Write-Host "Torch build failed" -ForegroundColor Red; exit 1 }

    if (-not (Test-Path $TorchExe)) {
        # Try Release config
        $TorchExe = Join-Path $BuildDir "TorchExternal\src\TorchExternal-build\Release\torch.exe"
    }

    if (-not (Test-Path $TorchExe)) {
        Write-Host "ERROR: torch.exe not found after build" -ForegroundColor Red
        Write-Host "Searched in Debug/ and Release/ under TorchExternal build dir"
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
        Write-Host "ERROR: ssb64.o2r was not created" -ForegroundColor Red
        exit 1
    }

    $o2rSize = (Get-Item $O2R).Length / 1MB
    Write-Host ("Assets extracted: ssb64.o2r ({0:N1} MB)" -f $o2rSize) -ForegroundColor Green

    # Copy o2r next to exe
    if ((Test-Path $ExeDir) -and ($ExeDir -ne $Root)) {
        Copy-Item $O2R (Join-Path $ExeDir "ssb64.o2r") -Force
        Write-Host "Copied ssb64.o2r to $ExeDir"
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

# Compress-Archive only accepts .zip; use ZipFile directly so we can name it .o2r
Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::CreateFromDirectory($Fast3DShaderDir, $F3DO2R)

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
