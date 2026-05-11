#include "enhancements.h"
#include "internal/buttons.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kCStickSmashCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.CStickSmash.P1",
    "gEnhancements.CStickSmash.P2",
    "gEnhancements.CStickSmash.P3",
    "gEnhancements.CStickSmash.P4",
};

} // namespace

extern "C" void port_enhancement_c_stick_smash(int player_index, unsigned short* button_hold, unsigned short* button_tap, signed char* stick_x, signed char* stick_y, unsigned short tap_pre_remap) {
    using namespace ssb64::enhancements::internal;

    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kCStickSmashCVars[player_index], 0)) return;

    // Always strip C-bits from both hold/tap so the engine never sees the raw
    // C-button input on this player when the remap is on (otherwise the stock
    // jump assignments would also fire alongside the synthesised attack).
    const unsigned short cbits = U_CBUTTON | D_CBUTTON | L_CBUTTON | R_CBUTTON;
    *button_hold &= ~cbits;
    *button_tap  &= ~cbits;

    // Fire the smash/aerial only on the rising edge of the C-button. A real
    // C-stick is analog and self-centers; translating it from a digital button
    // means as long as the C-button is pressed, the engine would otherwise see
    // stick pinned at ±80 — which manifests as drift in the air / running on
    // the ground (issue #97 [1]). Holding A across multiple frames also isn't
    // useful in SSB64 (no smash-attack charging), so a single-frame edge is
    // enough to register the attack.
    //
    // Cross-axis is forced to 0 so the input lands as a pure-axis tilt rather
    // than a 45° diagonal when the L-stick is already held sideways — without
    // this, e.g. C-up while running right reads as (80, 80) and the engine's
    // dominant-axis logic picks fair/bair instead of upair (issue #97 [2]).
    if (tap_pre_remap & U_CBUTTON) {
        *stick_x = 0;
        *stick_y = 80;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    } else if (tap_pre_remap & D_CBUTTON) {
        *stick_x = 0;
        *stick_y = -80;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    } else if (tap_pre_remap & L_CBUTTON) {
        *stick_x = -80;
        *stick_y = 0;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    } else if (tap_pre_remap & R_CBUTTON) {
        *stick_x = 80;
        *stick_y = 0;
        *button_hold |= A_BUTTON;
        *button_tap  |= A_BUTTON;
    }
}

namespace ssb64 {
namespace enhancements {

const char* CStickSmashCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kCStickSmashCVars[0];
    }
    return kCStickSmashCVars[playerIndex];
}

} // namespace enhancements
} // namespace ssb64
