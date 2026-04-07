# Build the Mupen64Plus GBI trace plugin
# Requires: CMake, MSVC (Visual Studio Build Tools)
#
# Usage: .\build.ps1

param(
    [switch]$Clean
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

if ($Clean -and (Test-Path "$ScriptDir\build")) {
    Remove-Item -Recurse -Force "$ScriptDir\build"
}

cmake -S $ScriptDir -B "$ScriptDir\build"
cmake --build "$ScriptDir\build" --config Release

$dll = Get-ChildItem "$ScriptDir\build" -Recurse -Filter "mupen64plus-video-trace.dll" | Select-Object -First 1
if ($dll) {
    Write-Host "`nPlugin built: $($dll.FullName)" -ForegroundColor Green
    Write-Host "Usage: mupen64plus --gfx `"$($dll.FullName)`" baserom.us.z64"
} else {
    $so = Get-ChildItem "$ScriptDir\build" -Recurse -Filter "mupen64plus-video-trace.so" | Select-Object -First 1
    if ($so) {
        Write-Host "`nPlugin built: $($so.FullName)" -ForegroundColor Green
    }
}
