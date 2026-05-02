#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kTapJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.TapJumpDisabled.P1",
    "gEnhancements.TapJumpDisabled.P2",
    "gEnhancements.TapJumpDisabled.P3",
    "gEnhancements.TapJumpDisabled.P4",
};

constexpr const char* kHitboxViewCVar = "gEnhancements.HitboxView";
constexpr const char* kStageClearFrozenWallpaperCVar = "gEnhancements.StageClearFrozenWallpaper";

constexpr const char* kCStickSmashCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.CStickSmash.P1",
    "gEnhancements.CStickSmash.P2",
    "gEnhancements.CStickSmash.P3",
    "gEnhancements.CStickSmash.P4",
};

constexpr const char* kDPadJumpCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.DPadJump.P1",
    "gEnhancements.DPadJump.P2",
    "gEnhancements.DPadJump.P3",
    "gEnhancements.DPadJump.P4",
};

// N64 button bitmasks. Duplicated here so the C++ layer doesn't have to pull
// in PR/os.h. Kept narrow (only what these enhancements need); if more
// enhancements grow C++-side button decoding, hoist into a shared header.
constexpr unsigned short A_BUTTON  = 0x8000;
constexpr unsigned short U_CBUTTON = 0x0008;
constexpr unsigned short D_CBUTTON = 0x0004;
constexpr unsigned short L_CBUTTON = 0x0002;
constexpr unsigned short R_CBUTTON = 0x0001;
constexpr unsigned short U_JPAD    = 0x0800;
constexpr unsigned short D_JPAD    = 0x0400;
constexpr unsigned short L_JPAD    = 0x0200;
constexpr unsigned short R_JPAD    = 0x0100;

// Mirrors dbObjectDisplayMode in src/sys/develop.h. Duplicated here to keep the
// C ABI of port_enhancement_hitbox_display_override() free of game headers.
enum {
    kDisplayModeMaster = 0,
    kDisplayModeHitCollisionFill = 1,
    kDisplayModeHitAttackOutline = 2,
    kDisplayModeMapCollision = 3,
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

extern "C" int port_enhancement_stage_clear_frozen_wallpaper_enabled(void) {
    return CVarGetInteger(kStageClearFrozenWallpaperCVar, 1) != 0;
}

extern "C" void port_enhancement_c_stick_smash(int player_index, unsigned short* button_hold, unsigned short* button_tap, signed char* stick_x, signed char* stick_y, unsigned short tap_pre_remap) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kCStickSmashCVars[player_index], 0)) return;

    if (*button_hold & U_CBUTTON) {
        *button_hold &= ~U_CBUTTON;
        *button_tap &= ~U_CBUTTON;
        *stick_y = 80;
        *button_hold |= A_BUTTON;
        if (tap_pre_remap & U_CBUTTON) *button_tap |= A_BUTTON;
    } else if (*button_hold & D_CBUTTON) {
        *button_hold &= ~D_CBUTTON;
        *button_tap &= ~D_CBUTTON;
        *stick_y = -80;
        *button_hold |= A_BUTTON;
        if (tap_pre_remap & D_CBUTTON) *button_tap |= A_BUTTON;
    } else if (*button_hold & L_CBUTTON) {
        *button_hold &= ~L_CBUTTON;
        *button_tap &= ~L_CBUTTON;
        *stick_x = -80;
        *button_hold |= A_BUTTON;
        if (tap_pre_remap & L_CBUTTON) *button_tap |= A_BUTTON;
    } else if (*button_hold & R_CBUTTON) {
        *button_hold &= ~R_CBUTTON;
        *button_tap &= ~R_CBUTTON;
        *stick_x = 80;
        *button_hold |= A_BUTTON;
        if (tap_pre_remap & R_CBUTTON) *button_tap |= A_BUTTON;
    }
}

extern "C" void port_enhancement_dpad_jump(int player_index, unsigned short* button_hold, unsigned short* button_tap, unsigned short tap_pre_remap) {
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

const char* TapJumpCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kTapJumpCVars[0];
    }
    return kTapJumpCVars[playerIndex];
}

const char* HitboxViewCVarName() {
    return kHitboxViewCVar;
}

const char* StageClearFrozenWallpaperCVarName() {
    return kStageClearFrozenWallpaperCVar;
}

const char* CStickSmashCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kCStickSmashCVars[0];
    }
    return kCStickSmashCVars[playerIndex];
}

const char* DPadJumpCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kDPadJumpCVars[0];
    }
    return kDPadJumpCVars[playerIndex];
}

}
} // namespace ssb64::enhancements
