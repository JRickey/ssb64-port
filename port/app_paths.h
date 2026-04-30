#pragma once

#include <string>

namespace ssb64 {

// Resolve the directory containing this process's executable.
//
// Ship::Context::GetAppBundlePath() on Linux/Windows + NON_PORTABLE
// returns the literal CMAKE_INSTALL_PREFIX, which is wrong for any
// distribution that doesn't install to that prefix (AppImage, Windows
// portable zip, dev cmake builds). This wrapper queries the OS for the
// actual binary location:
//   Linux   — readlink /proc/self/exe → parent dir
//   Windows — GetModuleFileNameW(NULL) → parent dir
//   macOS   — NSBundle resourcePath via Ship::Context (already correct
//             for both .app bundles and ad-hoc binaries)
// Falls back to Ship::Context::GetAppBundlePath() if the OS query fails.
std::string RealAppBundlePath();

} // namespace ssb64
