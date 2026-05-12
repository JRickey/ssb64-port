#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kTapJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.TapJumpDisabled.P1",
    "gEnhancements.TapJumpDisabled.P2",
    "gEnhancements.TapJumpDisabled.P3",
    "gEnhancements.TapJumpDisabled.P4",
};

} // namespace

extern "C" int port_enhancement_tap_jump_disabled(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return 0;
    }
    return CVarGetInteger(kTapJumpCVars[playerIndex], 0) != 0;
}

namespace ssb64 {
namespace enhancements {

const char* TapJumpCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kTapJumpCVars[0];
    }
    return kTapJumpCVars[playerIndex];
}

} // namespace enhancements
} // namespace ssb64
