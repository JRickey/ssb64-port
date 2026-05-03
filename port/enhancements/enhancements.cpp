#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>
#include <ship/Context.h>
#include <ship/controller/controldeck/ControlDeck.h>
#include <ship/controller/controldevice/controller/Controller.h>
#include <ship/controller/controldevice/controller/ControllerStick.h>
#include <ship/controller/controldevice/controller/mapping/ControllerAxisDirectionMapping.h>

#include <algorithm>
#include <cmath>

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

// Per-player NRage-style analog-stick remap. When the enable cvar for a player
// is on, src/sys/controller.c discards the libultraship Process()'d int8_t
// stick output and recomputes the value from the raw per-direction axis
// magnitudes via the per-axis formula in
// usbh_xpad.c (Ownasaurus/USBtoN64v2). Disabled by default; the LUS pipeline
// (circular deadzone + octagonal gate + notch snap) runs unchanged when off.
constexpr const char* kAnalogRemapEnableCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1",
    "gEnhancements.AnalogRemap.P2",
    "gEnhancements.AnalogRemap.P3",
    "gEnhancements.AnalogRemap.P4",
};
constexpr const char* kAnalogRemapDeadzoneCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1.Deadzone",
    "gEnhancements.AnalogRemap.P2.Deadzone",
    "gEnhancements.AnalogRemap.P3.Deadzone",
    "gEnhancements.AnalogRemap.P4.Deadzone",
};
constexpr const char* kAnalogRemapRangeCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1.Range",
    "gEnhancements.AnalogRemap.P2.Range",
    "gEnhancements.AnalogRemap.P3.Range",
    "gEnhancements.AnalogRemap.P4.Range",
};

constexpr float kAnalogRemapDeadzoneDefault = 0.10f;
constexpr float kAnalogRemapRangeDefault    = 1.00f;

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

// NRage-style per-axis stick remap. See header comment for the design.
//
// Reads the raw per-direction normalized magnitudes from libultraship
// (0..MAX_AXIS_RANGE per direction) and reconstructs a signed per-axis value,
// then applies the verbatim usbh_xpad.c formula independently to X and Y:
//   - linear deadzone subtraction with rescale gain
//   - asymmetric negative branch (`-(in+1)`), faithful to the source
//   - linear range scale capped at 127
// No octagonal gate, no radial deadzone, no notch snap.
//
// When the enable cvar is off, this is a no-op so libultraship's Process()
// output flows through to the game unchanged.
extern "C" void port_enhancement_analog_remap(int player_index, signed char* stick_x, signed char* stick_y) {
    if (player_index < 0 || player_index >= PORT_ENHANCEMENT_MAX_PLAYERS) return;
    if (!CVarGetInteger(kAnalogRemapEnableCVars[player_index], 0)) return;

    auto ctx = Ship::Context::GetInstance();
    if (!ctx) return;
    auto deck = ctx->GetControlDeck();
    if (!deck) return;
    auto controller = deck->GetControllerByPort(static_cast<uint8_t>(player_index));
    if (!controller) return;
    auto stick = controller->GetLeftStick();
    if (!stick) return;

    const float right = stick->GetAxisDirectionValue(Ship::RIGHT);
    const float left  = stick->GetAxisDirectionValue(Ship::LEFT);
    const float up    = stick->GetAxisDirectionValue(Ship::UP);
    const float down  = stick->GetAxisDirectionValue(Ship::DOWN);

    // Per-axis signed input in [-RAW_MAX, +RAW_MAX].
    const float in_x = right - left;
    const float in_y = up - down;

    // RAW_MAX matches libultraship's MAX_AXIS_RANGE (85.0f) — the per-direction
    // normalized cap GetAxisDirectionValue() can return.
    constexpr float RAW_MAX = 85.0f;
    const float dz_pct =
        std::clamp(CVarGetFloat(kAnalogRemapDeadzoneCVars[player_index], kAnalogRemapDeadzoneDefault), 0.0f, 0.99f);
    const float rng_pct =
        std::clamp(CVarGetFloat(kAnalogRemapRangeCVars[player_index], kAnalogRemapRangeDefault), 0.50f, 1.50f);

    const float dz_val      = dz_pct * RAW_MAX;
    const float dz_relation = (RAW_MAX > dz_val) ? RAW_MAX / (RAW_MAX - dz_val) : 1.0f;
    const float n64_max     = 127.0f * rng_pct;
    const float scale       = n64_max / RAW_MAX;

    auto remap_axis = [&](float in) -> signed char {
        if (in >= dz_val) {
            float out = (in - dz_val) * dz_relation * scale;
            if (out > 127.0f) out = 127.0f;
            return static_cast<signed char>(out);
        } else if (in <= -dz_val) {
            // Verbatim source asymmetry: temp = -(in+1) before deadzone math.
            float temp = -(in + 1.0f);
            float out  = (temp - dz_val) * dz_relation * scale;
            if (out > 127.0f) out = 127.0f;
            return static_cast<signed char>(-out);
        }
        return 0;
    };

    *stick_x = remap_axis(in_x);
    *stick_y = remap_axis(in_y);
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

const char* AnalogRemapCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kAnalogRemapEnableCVars[0];
    }
    return kAnalogRemapEnableCVars[playerIndex];
}

const char* AnalogRemapDeadzoneCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kAnalogRemapDeadzoneCVars[0];
    }
    return kAnalogRemapDeadzoneCVars[playerIndex];
}

const char* AnalogRemapRangeCVarName(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= PORT_ENHANCEMENT_MAX_PLAYERS) {
        return kAnalogRemapRangeCVars[0];
    }
    return kAnalogRemapRangeCVars[playerIndex];
}

}
} // namespace ssb64::enhancements
