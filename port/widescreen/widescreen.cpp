#include "widescreen.h"

#include "../enhancements/enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>
#include <libultraship/bridge/gfxbridge.h>

namespace {

constexpr const char* kWidescreenCVar = "gEnhancements.Widescreen";

bool widescreen_enabled() {
    return CVarGetInteger(kWidescreenCVar, 0) != 0;
}

} // namespace

extern "C" int port_widescreen_enabled(void) {
    return widescreen_enabled() ? 1 : 0;
}

extern "C" float port_widescreen_clip_x_scale(void) {
    if (!widescreen_enabled()) {
        return 1.0f;
    }
    return GfxGetWidescreenClipXScale();
}

extern "C" void port_widescreen_tick(void) {
    /* Mirror the CVar state into libultraship's Fast3D interpreter via the
     * GfxSetWidescreenActive bridge. When active, Interpreter::AdjXForAspectRatio
     * compresses post-projection clip-space X by (4/3)/window_aspect, expanding
     * the visible 4:3 frustum into the wider window. The pillarbox/letterbox
     * for non-battle scenes is handled passively: the game emits 2D rect ops
     * (HUD, backgrounds, menus) which libultraship leaves at their authored
     * 4:3 NDC range, while 3D vertex ops get the FOV expansion. So title
     * screens and menus end up 4:3-centered with black side strips, while
     * battles render native widescreen — no per-scene gating required.
     *
     * AdjXForAspectRatio derived from Ship of Harkinian's libultraship fork
     *   Copyright (c) The Harbour Masters
     *   https://github.com/HarbourMasters/libultraship
     * Licensed under MIT; see LICENSE at repository root. */
    const int active = widescreen_enabled() ? 1 : 0;
    static int s_last = -1;
    if (active != s_last) {
        s_last = active;
        GfxSetWidescreenActive(active);
    }
}

namespace ssb64 {
namespace enhancements {

const char* WidescreenCVarName() {
    return kWidescreenCVar;
}

} // namespace enhancements
} // namespace ssb64
