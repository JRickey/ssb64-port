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

    void port_enhancement_c_stick_smash(int player_index, unsigned short* button_hold, unsigned short* button_tap, signed char* stick_x, signed char* stick_y, unsigned short raw_tap);
    void port_enhancement_dpad_jump(int player_index, unsigned short* button_hold, unsigned short* button_tap, unsigned short raw_tap);

    #ifdef __cplusplus
}

namespace ssb64 {
namespace enhancements {
    const char* TapJumpCVarName(int playerIndex);
    const char* HitboxViewCVarName();
    const char* CStickSmashCVarName(int playerIndex);
    const char* DPadJumpCVarName(int playerIndex);
}
}
#endif

#endif
