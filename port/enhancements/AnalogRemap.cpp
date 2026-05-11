#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>
#include <ship/Context.h>
#include <ship/controller/controldeck/ControlDeck.h>
#include <ship/controller/controldevice/controller/Controller.h>
#include <ship/controller/controldevice/controller/ControllerStick.h>
#include <ship/controller/controldevice/controller/mapping/ControllerAxisDirectionMapping.h>

#include <algorithm>

namespace {

// Per-player NRage-style analog-stick remap. When the enable cvar for a player
// is on, src/sys/controller.c discards the libultraship Process()'d int8_t
// stick output and recomputes the value from the raw per-direction axis
// magnitudes via the per-axis formula in
// usbh_xpad.c (Ownasaurus/USBtoN64v2). Disabled by default; the LUS pipeline
// (circular deadzone + octagonal gate + notch snap) runs unchanged when off.
// Enable key has to live UNDER PX (as a sibling of Deadzone/Range), not AT PX.
// The config layer stores cvars in flattened JSON-pointer form and unflattens
// on every save; if PX were both a scalar (the enable flag) and a parent of
// PX.Deadzone / PX.Range, nlohmann's unflatten throws type_error.313 on the
// next save and Config::Save truncates the file to empty before serializing,
// wiping the user's settings (issue #96).
constexpr const char* kAnalogRemapEnableCVars[PORT_ENHANCEMENT_MAX_PLAYERS] = {
    "gEnhancements.AnalogRemap.P1.Enabled",
    "gEnhancements.AnalogRemap.P2.Enabled",
    "gEnhancements.AnalogRemap.P3.Enabled",
    "gEnhancements.AnalogRemap.P4.Enabled",
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

} // namespace

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

    // libultraship hands us per-direction magnitudes in [0, RAW_MAX] where
    // RAW_MAX is MAX_AXIS_RANGE (85.0f). The Ownasaurus/USBtoN64v2 firmware,
    // by contrast, runs the formula on the int16 USB stick value directly
    // (XPAD_MAX = 32767). Run the formula in the firmware's domain so it
    // is byte-faithful to usbh_xpad.c — including the `-(in+1)` asymmetry,
    // which is meaningful at int16 scale (one LSB of 32767, present to
    // protect against negating INT16_MIN) but would explode to ~1% of full
    // scale if applied to LUS's [-85, +85] floats directly. Output naturally
    // lands in [-127, +127] like the firmware.
    const float in_x = (right - left) * (32767.0f / 85.0f);
    const float in_y = (up - down)    * (32767.0f / 85.0f);

    constexpr float XPAD_MAX = 32767.0f;
    const float dz_pct =
        std::clamp(CVarGetFloat(kAnalogRemapDeadzoneCVars[player_index], kAnalogRemapDeadzoneDefault), 0.0f, 0.99f);
    const float rng_pct =
        std::clamp(CVarGetFloat(kAnalogRemapRangeCVars[player_index], kAnalogRemapRangeDefault), 0.50f, 1.50f);

    const float n64_max     = 127.0f * rng_pct;
    const float dz_val      = dz_pct * XPAD_MAX;
    const float dz_relation = (XPAD_MAX > dz_val) ? XPAD_MAX / (XPAD_MAX - dz_val) : 1.0f;

    // Verbatim usbh_xpad.c L260-L303 formula. The firmware truncates with a
    // (uint8_t) cast then unary-negates; at range > ~100% that truncation
    // wraps a >127 result into a small negative value (firmware bug). We
    // clamp to s8 instead so the user's range slider stays monotonic past
    // 100%.
    auto remap_axis = [&](float stick) -> signed char {
        float out;
        if (stick >= dz_val) {
            float unscaled = (stick - dz_val) * dz_relation;
            out = unscaled * (n64_max / XPAD_MAX);
        } else if (stick <= -dz_val) {
            float temp     = -(stick + 1.0f);
            float unscaled = (temp - dz_val) * dz_relation;
            out            = -(unscaled * (n64_max / XPAD_MAX));
        } else {
            return 0;
        }
        if (out >  127.0f) out =  127.0f;
        if (out < -127.0f) out = -127.0f;
        return static_cast<signed char>(out);
    };

    *stick_x = remap_axis(in_x);
    *stick_y = remap_axis(in_y);
}

namespace ssb64 {
namespace enhancements {

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

} // namespace enhancements
} // namespace ssb64
