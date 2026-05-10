#ifndef PORT_WIDESCREEN_H
#define PORT_WIDESCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

// Anchor mode for 2D screen-space draws in the 4:3 logical space (0..320 X,
// 0..240 Y). Used by Phase 2 HUD anchoring once Phase 1's viewport widening
// is in place.
//   LEFT     : base_x is offset from logical x=0 → new left edge.
//   RIGHT    : base_x is offset from logical x=320 → new right edge.
//   CENTER   : base_x is offset from logical x=160 (no change needed; the
//              widened logical space stays centered around 160).
//   STRETCH  : base_x is a 0..320 coordinate that should fill the widened
//              horizontal extent (used for full-screen backgrounds we opt to
//              stretch instead of letterbox).
typedef enum {
    PORT_WIDESCREEN_ANCHOR_LEFT = 0,
    PORT_WIDESCREEN_ANCHOR_RIGHT = 1,
    PORT_WIDESCREEN_ANCHOR_CENTER = 2,
    PORT_WIDESCREEN_ANCHOR_STRETCH = 3
} port_widescreen_anchor_t;

int port_widescreen_enabled(void);
float port_widescreen_aspect(void);
float port_widescreen_logical_width(void);

// Widens a 4:3-authored viewport so its X span matches port_widescreen_aspect().
// Vertical extent is preserved; horizontal center is preserved. New X bounds
// may go negative or exceed 320 in logical space — consumed downstream by the
// same scissor/projection path that already scales 320x240 logical units to
// physical pixels. Pass-through when CVar is off.
void port_widescreen_widen_viewport_f(float* ulx, float* uly, float* lrx, float* lry);

// Maps a base x coordinate (4:3 logical, 0..320) into the widescreen logical
// space using the supplied anchor. Pass-through when CVar is off.
int port_widescreen_anchor_x(int base_x, port_widescreen_anchor_t anchor);
float port_widescreen_anchor_x_f(float base_x, port_widescreen_anchor_t anchor);

// Phase-3 hook: should authored full-screen 2D draws (fades, wipes, freeze
// frames) be clipped to a centered 4:3 region rather than stretched across
// the widened frame? Currently equal to port_widescreen_enabled(); split out
// so a sub-toggle can be added without churning call sites.
int port_widescreen_should_letterbox_2d(void);

#ifdef __cplusplus
}

namespace ssb64 {
namespace enhancements {
const char* WidescreenCVarName();
}
}
#endif

#endif
