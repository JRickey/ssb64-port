# Builds BattleShip as a self-contained Windows release zip.
#
# Output: <repo-root>\dist\BattleShip-windows.zip
#
# Layout produced (extracted):
#   BattleShip\
#     BattleShip.exe             — main executable
#     torch.exe                  — sidecar for first-run extraction
#     f3d.o2r                    — Fast3D shader archive (ROM-independent)
#     config.yml                 — Torch extraction config
#     yamls\us\*.yml             — Torch extraction recipes
#     gamecontrollerdb.txt       — SDL controller mappings
#     SDL2.dll                   — runtime dependency (vcpkg-bundled)
#     <other vcpkg DLLs>         — picked up by Get-ChildItem from build dir
#
# Built with NON_PORTABLE=ON so saves and config land in
# %APPDATA%\BattleShip\ instead of next to the .exe — same as macOS bundle.
# BattleShip.o2r is NOT bundled; the first-run wizard extracts it from the
# user's ROM into %APPDATA%\BattleShip\BattleShip.o2r.

$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$BuildDir = Join-Path $Root "build-bundle-win"
$DistDir = Join-Path $Root "dist"
$AppName = "BattleShip"
$StageDir = Join-Path $DistDir $AppName
$ZipPath = Join-Path $DistDir "$AppName-windows.zip"
$Jobs = if ($env:NUMBER_OF_PROCESSORS) { [int]$env:NUMBER_OF_PROCESSORS } else { 4 }

function Write-Step($msg) { Write-Host "`n=== $msg ===" -ForegroundColor Cyan }
function Fail($msg) { Write-Host "ERROR: $msg" -ForegroundColor Red; exit 1 }

# ── 0. Run codegen scripts that don't need the ROM ──
# Encoded credit files are gitignored (input text is in src/credits/),
# so a fresh checkout (CI or otherwise) must run the encoder before
# cmake builds scstaffroll.c. ROM-independent — same step CMake's
# GenerateCreditsAssets target runs.
Write-Step "Encoding credits text"
Push-Location (Join-Path $Root "src/credits")
foreach ($f in @("staff.credits.us.txt", "titles.credits.us.txt")) {
    & python "$Root/tools/creditsTextConverter.py" $f | Out-Null
    if ($LASTEXITCODE -ne 0) { Pop-Location; Fail "credits encode failed: $f" }
}
foreach ($f in @("info.credits.us.txt", "companies.credits.us.txt")) {
    & python "$Root/tools/creditsTextConverter.py" -paragraphFont $f | Out-Null
    if ($LASTEXITCODE -ne 0) { Pop-Location; Fail "credits encode failed: $f" }
}
Pop-Location

# ── 1. Configure + build with NON_PORTABLE=ON (Release) ──
Write-Step "Configuring release build with NON_PORTABLE=ON"
# CMAKE_INSTALL_PREFIX is baked into libultraship's install_config.h at
# configure time and returned by Ship::Context::GetAppBundlePath() under
# NON_PORTABLE. CMake's Windows default is $ENV{ProgramFiles(x86)}/<project>
# (e.g. "C:\Program Files (x86)\ssb64") which is meaningless for a zip the
# user extracts to an arbitrary directory. We never run `cmake --install`,
# so the value is cosmetic — the runtime path resolution lives in
# port/app_paths.cpp::RealAppBundlePath() (GetModuleFileNameW). Set a
# readable label so log lines make sense.
cmake -B $BuildDir $Root `
    -DCMAKE_BUILD_TYPE=Release `
    -DNON_PORTABLE=ON `
    -DCMAKE_INSTALL_PREFIX=BattleShip `
    | Out-Null
if ($LASTEXITCODE -ne 0) { Fail "cmake configure failed" }

Write-Step "Building BattleShip + torch"
cmake --build $BuildDir --config Release -j $Jobs
if ($LASTEXITCODE -ne 0) { Fail "build failed" }

# ── 2. Build f3d.o2r (zip of LUS shaders, ROM-independent) ──
Write-Step "Packaging Fast3D shader archive"
$F3DO2R = Join-Path $BuildDir "f3d.o2r"
if (Test-Path $F3DO2R) { Remove-Item $F3DO2R -Force }
$ShaderSrc = Join-Path $Root "libultraship\src\fast"
Push-Location $ShaderSrc
Compress-Archive -Path "shaders" -DestinationPath $F3DO2R -CompressionLevel Optimal
Pop-Location
if (-not (Test-Path $F3DO2R)) { Fail "f3d.o2r was not created" }

# ── 3. Locate built artifacts ──
$GameExe = Join-Path $BuildDir "Release\BattleShip.exe"
if (-not (Test-Path $GameExe)) {
    # Fall back to non-multi-config layout (Ninja).
    $GameExe = Join-Path $BuildDir "BattleShip.exe"
}
$TorchExe = $null
foreach ($cand in @(
    "TorchExternal\src\TorchExternal-build\Release\torch.exe",
    "TorchExternal\src\TorchExternal-build\torch.exe",
    "torch-install\bin\torch.exe"
)) {
    $p = Join-Path $BuildDir $cand
    if (Test-Path $p) { $TorchExe = $p; break }
}
if (-not (Test-Path $GameExe))   { Fail "BattleShip.exe not found at $GameExe" }
if (-not $TorchExe)              { Fail "torch.exe not found in $BuildDir" }

# ── 4. Stage the release tree ──
Write-Step "Staging $StageDir"
if (Test-Path $StageDir) { Remove-Item -Recurse -Force $StageDir }
New-Item -ItemType Directory -Path $StageDir | Out-Null
New-Item -ItemType Directory -Path (Join-Path $StageDir "yamls\us") | Out-Null

Copy-Item $GameExe        (Join-Path $StageDir "$AppName.exe")
Copy-Item $TorchExe      (Join-Path $StageDir "torch.exe")
Copy-Item $F3DO2R        (Join-Path $StageDir "f3d.o2r")
Copy-Item (Join-Path $Root "gamecontrollerdb.txt") $StageDir
Copy-Item (Join-Path $Root "config.yml") $StageDir
Copy-Item (Join-Path $Root "yamls\us\*.yml") (Join-Path $StageDir "yamls\us")
# Standalone .ico for shortcut/installer use — the icon is also embedded
# directly in BattleShip.exe via port/ssb64.rc, so Explorer picks it up
# without this file. Keep it bundled for future installer work.
Copy-Item (Join-Path $Root "assets\icon.ico") (Join-Path $StageDir "$AppName.ico")

# Bundle the ESC menu fonts. Menu.cpp::FindMenuAssetPath walks up from
# RealAppBundlePath() and from current_path(); placing the TTFs at
# <staging>\assets\custom\fonts\ next to the .exe matches the first
# iteration of the walker rooted at the .exe's directory. Without this
# the menu falls back to ImGui's default font silently.
$FontsDir = Join-Path $StageDir "assets\custom\fonts"
New-Item -ItemType Directory -Path $FontsDir -Force | Out-Null
Copy-Item (Join-Path $Root "assets\custom\fonts\Montserrat-Regular.ttf")  $FontsDir
Copy-Item (Join-Path $Root "assets\custom\fonts\Inconsolata-Regular.ttf") $FontsDir

# Bundle DLLs that landed next to BattleShip.exe (vcpkg drops SDL2.dll, etc.).
$ExeBuildDir = Split-Path $GameExe -Parent
Get-ChildItem -Path $ExeBuildDir -Filter "*.dll" | ForEach-Object {
    Copy-Item $_.FullName $StageDir
}

# ── 5. Zip ──
Write-Step "Compressing $ZipPath"
if (Test-Path $ZipPath) { Remove-Item $ZipPath -Force }
Compress-Archive -Path "$StageDir\*" -DestinationPath $ZipPath -CompressionLevel Optimal
if (-not (Test-Path $ZipPath)) { Fail "zip was not created" }

$ZipKB = [int]((Get-Item $ZipPath).Length / 1024)
Write-Host "`n✓ Release zip ready: $ZipPath ($ZipKB KB)" -ForegroundColor Green
Write-Host "   App-data: %APPDATA%\BattleShip\"
Write-Host "   First launch will prompt for your ROM via the ImGui wizard."
