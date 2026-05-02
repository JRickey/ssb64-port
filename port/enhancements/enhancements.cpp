#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

// Manually define the N64 button bitmasks since the C++ layer cannot see PR/os.h
namespace {
    // A and C-Buttons
    constexpr unsigned short A_BUTTON  = 0x8000;
    constexpr unsigned short U_CBUTTON = 0x0008;
    constexpr unsigned short D_CBUTTON = 0x0004;
    constexpr unsigned short L_CBUTTON = 0x0002;
    constexpr unsigned short R_CBUTTON = 0x0001;

    // D-Pad
    constexpr unsigned short U_JPAD = 0x0800;
    constexpr unsigned short D_JPAD = 0x0400;
    constexpr unsigned short L_JPAD = 0x0200;
    constexpr unsigned short R_JPAD = 0x0100;

    // Add the CVars for D-Pad Jump
    constexpr const char* kDPadJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
        "gEnhancements.DPadJump.P1",
        "gEnhancements.DPadJump.P2",
        "gEnhancements.DPadJump.P3",
        "gEnhancements.DPadJump.P4",
    };
}

namespace {

constexpr const char* kTapJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.TapJumpDisabled.P1",
    "gEnhancements.TapJumpDisabled.P2",
    "gEnhancements.TapJumpDisabled.P3",
    "gEnhancements.TapJumpDisabled.P4",
};

constexpr const char* kHitboxViewCVar = "gEnhancements.HitboxView";

// Mirrors dbObjectDisplayMode in src/sys/develop.h. Duplicated here to keep the
// C ABI of port_enhancement_hitbox_display_override() free of game headers.
enum {
    kDisplayModeMaster = 0,
    kDisplayModeHitCollisionFill = 1,
    kDisplayModeHitAttackOutline = 2,
    kDisplayModeMapCollision = 3,
};

constexpr const char* kCStickSmashCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.CStickSmash.P1",
    "gEnhancements.CStickSmash.P2",
    "gEnhancements.CStickSmash.P3",
    "gEnhancements.CStickSmash.P4",
};

} // namespace

extern "C" int port_enhancement_tap_jump_disabled(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return 0;
    }
    return CVarGetInteger(kTapJumpCVars[playerIndex], 0) != 0;
}

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

extern "C" void port_enhancement_c_stick_smash(int player_index, unsigned short* button_hold, unsigned short* button_tap, signed char* stick_x, signed char* stick_y, unsigned short raw_tap) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kCStickSmashCVars[player_index], 0)) return;

    if (*button_hold & U_CBUTTON) {
        *button_hold &= ~U_CBUTTON;
        *button_tap &= ~U_CBUTTON;
        *stick_y = 80;
        *button_hold |= A_BUTTON;
        if (raw_tap & U_CBUTTON) *button_tap |= A_BUTTON;
    } else if (*button_hold & D_CBUTTON) {
        *button_hold &= ~D_CBUTTON;
        *button_tap &= ~D_CBUTTON;
        *stick_y = -80;
        *button_hold |= A_BUTTON;
        if (raw_tap & D_CBUTTON) *button_tap |= A_BUTTON;
    } else if (*button_hold & L_CBUTTON) {
        *button_hold &= ~L_CBUTTON;
        *button_tap &= ~L_CBUTTON;
        *stick_x = -80;
        *button_hold |= A_BUTTON;
        if (raw_tap & L_CBUTTON) *button_tap |= A_BUTTON;
    } else if (*button_hold & R_CBUTTON) {
        *button_hold &= ~R_CBUTTON;
        *button_tap &= ~R_CBUTTON;
        *stick_x = 80;
        *button_hold |= A_BUTTON;
        if (raw_tap & R_CBUTTON) *button_tap |= A_BUTTON;
    }
}

extern "C" void port_enhancement_dpad_jump(int player_index, unsigned short* button_hold, unsigned short* button_tap, unsigned short raw_tap) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kDPadJumpCVars[player_index], 0)) return;

    // If any D-Pad direction is pressed, inject a C-Up (Jump) into the engine
    if (*button_hold & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
        *button_hold |= U_CBUTTON;
        if (raw_tap & (U_JPAD | D_JPAD | L_JPAD | R_JPAD)) {
            *button_tap |= U_CBUTTON;
        }
    }
}

namespace ssb64 {
namespace enhancements {
    const char* TapJumpCVarName(int playerIndex) {
        if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
            return kTapJumpCVars[0];
        }
        return kTapJumpCVars[playerIndex];
    }

    const char* HitboxViewCVarName() {
        return kHitboxViewCVar;
    }

    const char* CStickSmashCVarName(int playerIndex) {
        if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
            return kCStickSmashCVars[0];
        }
        return kCStickSmashCVars[playerIndex];
    }

    const char* DPadJumpCVarName(int playerIndex) {
        if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) return kDPadJumpCVars[0];
        return kDPadJumpCVars[playerIndex];
    }
}
} // namespace ssb64::enhancements
