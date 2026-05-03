#ifndef PORT_ENHANCEMENTS_H
#define PORT_ENHANCEMENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_ENHANCEMENT_MAX_PLAYERS 4

int port_enhancement_tap_jump_disabled(int player_index);

// Hitbox-view debug overlay. Returns the dbObjectDisplayMode the caller should
// use for the current frame. When the cvar is off this is the entity's own
// stored display_mode; when on it overrides to a hitbox-display mode so
// existing fighters/items/weapons flip to hitbox view immediately without a
// match restart.
int port_enhancement_hitbox_display_override(int current_mode);

// 1P stage-clear "frozen frame" wallpaper. Default on. When off, the stage-
// clear bonus screen reverts to the asset's stored solid-black background
// (matches the pre-#57 behaviour). Provides an escape hatch for backends
// where the GPU-readback bridge is unavailable or unreliable.
int port_enhancement_stage_clear_frozen_wallpaper_enabled(void);

// C-Stick → smash/aerial attack remap. When enabled for a player, the C-button
// directional inputs are translated into the corresponding smash/aerial attack
// inputs (mirroring later Smash games' right-stick behaviour) instead of the
// stock jump assignments. Active only during gameplay so CSS palette cycling
// with C buttons is unaffected.
//
// `tap_pre_remap` is a snapshot of `*button_tap` taken BEFORE this helper
// rewrites it, so callees can still ask "was this C-button tapped this frame
// (rising edge)?" after the C-bit has been cleared from the working tap mask.
// The caller passes the same value it copies into `*button_tap`.
void port_enhancement_c_stick_smash(int player_index, unsigned short* button_hold, unsigned short* button_tap, signed char* stick_x, signed char* stick_y, unsigned short tap_pre_remap);

// D-Pad → C-button remap. Companion to the C-Stick smash option for players
// who also have tap-jump disabled and would otherwise lose their jump input.
// `tap_pre_remap` has the same meaning as above.
void port_enhancement_dpad_jump(int player_index, unsigned short* button_hold, unsigned short* button_tap, unsigned short tap_pre_remap);

#ifdef __cplusplus
}

namespace ssb64 {
namespace enhancements {
const char* TapJumpCVarName(int playerIndex);
const char* HitboxViewCVarName();
const char* StageClearFrozenWallpaperCVarName();
const char* CStickSmashCVarName(int playerIndex);
const char* DPadJumpCVarName(int playerIndex);
}
}
#endif

#endif
