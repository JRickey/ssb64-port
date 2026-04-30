#pragma once

#include <string>

namespace ssb64 {

// Resolve the directory containing this process's executable.
//
// Ship::Context::GetAppBundlePath() on Linux + NON_PORTABLE returns the
// literal CMAKE_INSTALL_PREFIX (default /usr/local), which is wrong for
// AppImages (binary actually lives at /tmp/.mount_XYZ/usr/bin/) and for
// any non-prefix install. This wrapper uses /proc/self/exe on Linux to
// get the real directory and falls back to the upstream API only if
// the readlink fails. On non-Linux platforms it just delegates.
std::string RealAppBundlePath();

} // namespace ssb64
