#ifndef _GMTYPES_H_
#define _GMTYPES_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <sys/matrix.h>
#include <sys/vector.h>
#include <sys/utils.h>
#include <lb/library.h>

#include <gm/gmdef.h>

struct GMCamera
{
	s32 status_default;
	s32 status_curr;
	s32 status_prev;
	void (*func_camera)(GObj*);
	f32 target_dist;
	Vec3f vel_at;
    s32 viewport_ulx;
    s32 viewport_uly;
    s32 viewport_lrx;
    s32 viewport_lry;
    s32 viewport_center_x;
    s32 viewport_center_y;
    s32 viewport_width;
    s32 viewport_height;
	f32 fovy;
	GObj *pzoom_fighter_gobj; // Guess: this is a struct from here...
	f32 pzoom_eye_x;
	f32 pzoom_eye_y;
	f32 pzoom_dist;
	f32 pzoom_pan_scale;
	f32 pzoom_fov;
	Vec3f zoom_origin_pos;
	Vec3f zoom_target_pos; // ...to here
	GObj *pfollow_fighter_gobj;	 // ...and there is an array of it
	f32 pfollow_eye_x;
	f32 pfollow_eye_y;
	f32 pfollow_dist;
	f32 pfollow_pan_scale;
	f32 pfollow_fov;
	Vec3f vel_all;
	LookAt look_at;
};

struct GMHitFlags
{
	u32 is_interact_hurt : 1;
	u32 is_interact_shield : 1;
	u32 is_interact_reflect : 1;
	u32 is_interact_absorb : 1;
	u32 group_id : 3; // Number of hitbox groups this object has been hit by /
					  // can be hit by? Its implementation feels abandoned.
	u32 timer_rehit : 6;
};

struct GMAttackRecord
{
	GObj *victim_gobj;
	GMHitFlags victim_flags;
};

union GMStatFlags
{
	struct
	{
		u16 unused : 3;
		ub16 is_smash_attack : 1;
		ub16 ga : 1;
		ub16 is_projectile : 1;
		u16 attack_id : 10;
	};
	u16 halfword;
};

struct GMColScript
{
	u32 *p_script; // Pointer to Color Animation script?
	u16 color_event_timer;
	u16 script_id;
	void *p_subroutine[1];
	s32 loop_count[1];
	void *p_goto[2];
	s32 unk_ca_timer;
};

struct GMColKeys
{
	u8 r, g, b, a;
	s16 ir, ig, ib, ia; 		// Interpolation step
};

struct GMColAnim
{
	GMColScript cs[2];
	s32 length;
	s32 colanim_id;
	GMColKeys color1; 		// Used as both PrimColor and EnvColor? Screen flashes use it as Prim and items as Env
	f32 light_angle_x;
	f32 light_angle_y;
	GMColKeys color2;
	ub8 is_use_color1 : 1;
	ub8 is_use_light : 1;
	ub8 is_use_color2 : 1;
	u8 skeleton_id : 2; 		// ID of skeleton model to use during electric shock ColAnim?
};

struct GMColDesc
{
	void *p_script;
	u8 priority;				// If this is >= current ColAnim's priority, the new ColAnim gets applied
	ub8 is_unlocked;			// If TRUE, ColAnim can be interrupted on fighter status change
};

struct GMColEventDefault
{
	u32 opcode : 6;
	u32 value : 26;
};

struct GMColEventGoto1
{
	u32 opcode : 6;
};

struct GMColEventGoto2
{
	void* p_goto;
};

struct GMColEventGoto
{
	GMColEventGoto1 s1;
	GMColEventGoto2 s2;
};

struct GMColEventSubroutine1
{
	u32 opcode : 6;
};

struct GMColEventSubroutine2
{
	void* p_subroutine;
};

struct GMColEventSubroutine
{
	GMColEventSubroutine1 s1;
	GMColEventSubroutine2 s2;
};

struct GMColEventParallel1
{
	u32 opcode : 6;
};

struct GMColEventParallel2
{
	void* p_script;
};

struct GMColEventParallel
{
	GMColEventParallel1 s1;
	GMColEventParallel2 s2;
};

struct GMColEventSetRGBA1
{
	u32 opcode : 6;
};

struct GMColEventSetRGBA2
{
	u32 r : 8;
	u32 g : 8;
	u32 b : 8;
	u32 a : 8;
};

struct GMColEventSetRGBA
{
	GMColEventSetRGBA1 s1;
	GMColEventSetRGBA2 s2;
};

struct GMColEventBlendRGBA1
{
	u32 opcode : 6;
	u32 blend_frames : 26;
};

struct GMColEventBlendRGBA2
{
	u32 r : 8;
	u32 g : 8;
	u32 b : 8;
	u32 a : 8;
};

struct GMColEventBlendRGBA
{
	GMColEventBlendRGBA1 s1;
	GMColEventBlendRGBA2 s2;
};

struct GMColEventMakeEffect1
{
	u32 opcode : 6;
	s32 joint_id : 7;
	u32 effect_id : 9;
	u32 flag : 10;
};

struct GMColEventMakeEffect2
{
	s32 off_x : 16;
	s32 off_y : 16;
};

struct GMColEventMakeEffect3
{
	s32 off_z : 16;
	s32 rng_x : 16;
};

struct GMColEventMakeEffect4
{
	s32 rng_y : 16;
	s32 rng_z : 16;
};

struct GMColEventMakeEffect
{
	GMColEventMakeEffect1 s1;
	GMColEventMakeEffect2 s2;
	GMColEventMakeEffect3 s3;
	GMColEventMakeEffect4 s4;
};

struct GMColEventSetLight
{
	u32 opcode : 6;
	s32 light1 : 13;
	s32 light2 : 13;
};

union GMColEventAll
{
	GMColEventDefault ca_base;
	GMColEventGoto1 ca_goto1;
	GMColEventGoto2 ca_goto2;
	GMColEventDefault ca_loopstart;
	GMColEventSubroutine1 ca_sub1;
	GMColEventSubroutine2 ca_sub2;
	GMColEventParallel1 ca_par1;
	GMColEventParallel2 ca_par2;
	GMColEventSetRGBA1 ca_rgba1;
	GMColEventSetRGBA2 ca_rgba2;
	GMColEventBlendRGBA1 ca_blend1;
	GMColEventBlendRGBA2 ca_blend2;
	GMColEventMakeEffect1 ca_effect1;
	GMColEventMakeEffect2 ca_effect2;
	GMColEventMakeEffect3 ca_effect3;
	GMColEventMakeEffect4 ca_effect4;
	GMColEventSetLight ca_light;
	GMColEventDefault ca_sfx;
};

struct GMRumbleEventDefault
{
	u16 opcode : 3;
	u16 param : 13;
};

struct GMRumbleScript
{
	u8 rumble_id;
	ub8 is_rumble_active;
	u16 rumble_status;
	u16 loop_count;
	s32 rumble_timer;
	void *p_goto;
	GMRumbleEventDefault *p_event;
};

struct GMRumbleLink
{
	GMRumbleScript *p_script;
	GMRumbleLink *rnext;
    GMRumbleLink *rprev;
};

struct GMRumblePlayer
{
	ub8 is_active;
	GMRumbleLink *rlink;
};

#endif
