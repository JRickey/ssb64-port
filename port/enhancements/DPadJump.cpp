#include "enhancements.h"
#include "internal/buttons.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kDPadJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.DPadJump.P1",
    "gEnhancements.DPadJump.P2",
    "gEnhancements.DPadJump.P3",
    "gEnhancements.DPadJump.P4",
};

} // namespace

extern "C" void port_enhancement_dpad_jump(int player_index, unsigned short* button_hold, unsigned short* button_tap, unsigned short tap_pre_remap) {
    using namespace ssb64::enhancements::internal;

    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kDPadJumpCVars[player_index], 0)) return;

    // If any D-Pad direction is pressed, inject a C-Up (Jump) into the engine.
    if (*button_hold & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
        *button_hold |= U_CBUTTON;
        if (tap_pre_remap & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
            *button_tap |= U_CBUTTON;
        }
    }
}

namespace ssb64 {
namespace enhancements {

const char* DPadJumpCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kDPadJumpCVars[0];
    }
    return kDPadJumpCVars[playerIndex];
}

} // namespace enhancements
} // namespace ssb64
