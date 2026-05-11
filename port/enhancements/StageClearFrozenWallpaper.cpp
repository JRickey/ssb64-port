#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kStageClearFrozenWallpaperCVar = "gEnhancements.StageClearFrozenWallpaper";

} // namespace

extern "C" int port_enhancement_stage_clear_frozen_wallpaper_enabled(void) {
    return CVarGetInteger(kStageClearFrozenWallpaperCVar, 1) != 0;
}

namespace ssb64 {
namespace enhancements {

const char* StageClearFrozenWallpaperCVarName() {
    return kStageClearFrozenWallpaperCVar;
}

} // namespace enhancements
} // namespace ssb64
