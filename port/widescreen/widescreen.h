#ifndef PORT_WIDESCREEN_H
#define PORT_WIDESCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

// Returns 1 when the user has the widescreen toggle enabled in the menu.
int port_widescreen_enabled(void);

// Per-frame tick called from the gameloop. Mirrors the CVar state into
// libultraship's Fast3D interpreter so AdjXForAspectRatio applies clip-x
// compression (FOV expansion) when on. Cheap; only writes when state flips.
void port_widescreen_tick(void);

// Returns the post-projection clip-space X scale factor libultraship is
// currently applying to 3D vertices via AdjXForAspectRatio (1.0f when off
// or not in a >4:3 window). Decomp consumers that project world points to
// 2D screen space for HUD overlays (player tag, item arrow, fighter
// magnify, boomerang off-screen check) multiply the projected x by this
// factor so the overlay tracks the rendered character/item position.
float port_widescreen_clip_x_scale(void);

#ifdef __cplusplus
}

namespace ssb64 {
namespace enhancements {
const char* WidescreenCVarName();
}
}
#endif

#endif
