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

#ifdef __cplusplus
}

namespace ssb64 {
namespace enhancements {
const char* WidescreenCVarName();
}
}
#endif

#endif
