#include "app_paths.h"

#include <libultraship/libultraship.h>

#include <filesystem>
#include <system_error>

namespace ssb64 {

std::string RealAppBundlePath() {
#if defined(__linux__)
    std::error_code ec;
    std::filesystem::path exe = std::filesystem::read_symlink("/proc/self/exe", ec);
    if (!ec && !exe.empty()) {
        return exe.parent_path().string();
    }
#endif
    return Ship::Context::GetAppBundlePath();
}

} // namespace ssb64
