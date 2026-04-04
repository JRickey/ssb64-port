#ifndef _IFTYPES_H_
#define _IFTYPES_H_

#include <ssb_types.h>
#include <macros.h>
#include <sys/obj.h>
#include <gm/generic.h>

#include <if/ifdef.h>

struct IFPlayerCommon
{
	ub8 is_magnify_display;
	u8 magnify_mode;
	f32 magnify_scale;
	s32* player_pos_x; // Pointer to each player's HUD X-position
	u16 player_pos_y;  // Player HUD's Y-positions - subtracted from center
						  // Y-position?
	u8 arrows_flags;
	u8 arrows_left_status;
	u8 arrows_right_status;
};

struct IFACharacter // Announcer text character (e.g. SUCCESS)
{
	Vec2h pos;
	intptr_t offset;
};

struct IFDCharacter // Damage text character
{
	Vec2f pos;
	Vec2f vel;
	u8 image_id; // e.g. the % symbol is 0xA
	u8 is_lock_movement;
};

struct IFPlayerDamage
{
	s32 damage;
	s32 pos_adjust_wait;  // Wait this many frames before snapping numbers back
						  // to original position?
	s32 flash_reset_wait; // Frames until damage display's white flash is
						  // reset; tied to pos_adjust_wait?
	f32 scale;
	IFDCharacter chars[4];
	GObj* interface_gobj; // Fighter whom this HUD is assigned to
	u8 color_id;		  // ??? This changes the damage HUD's color to white if set to 4
	u8 is_update_anim;
	u8 char_display_count;
	u8 break_anim_frame;
	u8 dead_stopupdate_wait; // This counts down if the player loses their last
							 // stock?
	u8 is_show_interface;
};

struct IFPlayerSteal
{
	u8 anim_frames;
	u16 steal_pos_x;  // X-Position of stock icon this player is stealing from
	u16 steal_pos_y;  // Y-Position of stock icon this player is stealing from
	u16 target_pos_x; // X-Position the stolen stock will fly to
};

struct IFPlayerMagnify
{
	Vec2f pos;
	Vp viewport;
	GObj *interface_gobj;
	u8 color_id;
};

struct IFTraffic // This is exclusively for the color of the countdown lamps, the black main body is not included
{
	Vec2h pos;
	u8 color_id;
};

struct IFPauseDecal
{
	intptr_t offset;
	Vec2h pos;
	SYColorRGBPair colors;
};

#endif
