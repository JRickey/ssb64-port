#include "widescreen.h"

#include "../enhancements/enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

namespace {

constexpr const char* kWidescreenCVar = "gEnhancements.Widescreen";

constexpr float kBaseAspect = 4.0f / 3.0f;
constexpr float kWideAspect = 16.0f / 9.0f;
constexpr float kBaseLogicalWidth = 320.0f;
constexpr float kBaseLogicalHeight = 240.0f;

bool widescreen_enabled() {
    return CVarGetInteger(kWidescreenCVar, 0) != 0;
}

} // namespace

extern "C" int port_widescreen_enabled(void) {
    return widescreen_enabled() ? 1 : 0;
}

extern "C" float port_widescreen_aspect(void) {
    return widescreen_enabled() ? kWideAspect : kBaseAspect;
}

extern "C" float port_widescreen_logical_width(void) {
    return widescreen_enabled() ? (kBaseLogicalHeight * kWideAspect) : kBaseLogicalWidth;
}

extern "C" int port_widescreen_should_letterbox_2d(void) {
    return widescreen_enabled() ? 1 : 0;
}

extern "C" void port_widescreen_widen_viewport_f(float* ulx, float* uly, float* lrx, float* lry) {
    if (!widescreen_enabled()) {
        return;
    }
    const float old_w = *lrx - *ulx;
    const float old_h = *lry - *uly;
    if (old_w <= 0.0f || old_h <= 0.0f) {
        return;
    }
    const float old_aspect = old_w / old_h;
    const float scale = kWideAspect / old_aspect;
    const float center = (*ulx + *lrx) * 0.5f;
    const float new_half = (old_w * scale) * 0.5f;
    *ulx = center - new_half;
    *lrx = center + new_half;
}

extern "C" float port_widescreen_anchor_x_f(float base_x, port_widescreen_anchor_t anchor) {
    if (!widescreen_enabled()) {
        return base_x;
    }
    const float wide_w = kBaseLogicalHeight * kWideAspect;
    const float margin = (wide_w - kBaseLogicalWidth) * 0.5f;
    switch (anchor) {
        case PORT_WIDESCREEN_ANCHOR_LEFT:
            return base_x - margin;
        case PORT_WIDESCREEN_ANCHOR_RIGHT:
            return base_x + margin;
        case PORT_WIDESCREEN_ANCHOR_STRETCH:
            return base_x * (wide_w / kBaseLogicalWidth) - margin;
        case PORT_WIDESCREEN_ANCHOR_CENTER:
        default:
            return base_x;
    }
}

extern "C" int port_widescreen_anchor_x(int base_x, port_widescreen_anchor_t anchor) {
    return (int)port_widescreen_anchor_x_f((float)base_x, anchor);
}

namespace ssb64 {
namespace enhancements {

const char* WidescreenCVarName() {
    return kWidescreenCVar;
}

} // namespace enhancements
} // namespace ssb64
