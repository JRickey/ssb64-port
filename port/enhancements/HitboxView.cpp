#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kHitboxViewCVar = "gEnhancements.HitboxView";

// Mirrors dbObjectDisplayMode in src/sys/develop.h. Duplicated here to keep the
// C ABI of port_enhancement_hitbox_display_override() free of game headers.
enum {
    kDisplayModeMaster = 0,
    kDisplayModeHitCollisionFill = 1,
    kDisplayModeHitAttackOutline = 2,
    kDisplayModeMapCollision = 3,
};

} // namespace

extern "C" int port_enhancement_hitbox_display_override(int current_mode) {
    int setting = CVarGetInteger(kHitboxViewCVar, 0);
    switch (setting) {
        case 1:
            return kDisplayModeHitCollisionFill;
        case 2:
            return kDisplayModeHitAttackOutline;
        default:
            return current_mode;
    }
}

namespace ssb64 {
namespace enhancements {

const char* HitboxViewCVarName() {
    return kHitboxViewCVar;
}

} // namespace enhancements
} // namespace ssb64
