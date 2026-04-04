#ifndef _GRVARS_H_
#define _GRVARS_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <gr/grdef.h>
#include <ef/efdef.h>

#define GRPUPUPU_WHISPY_BLINK_WAIT_BASE 30
#define GRPUPUPU_WHISPY_BLINK_WAIT_RANDOM 270
#define GRPUPUPU_WHISPY_WAIT_DURATION_BASE 960
#define GRPUPUPU_WHISPY_WAIT_DURATION_RANDOM 1140
#define GRPUPUPU_WHISPY_WIND_DURATION_BASE 240
#define GRPUPUPU_WHISPY_WIND_DURATION_RANDOM 80
#define GRPUPUPU_WHISPY_WIND_RUMBLE_WAIT 18
#define GRPUPUPU_WHISPY_POS_X (-525.0F)
#define GRPUPUPU_WHISPY_WIND_VEL_BASE 6.0F
#if defined(REGION_US)
#define GRPUPUPU_WHISPY_WIND_DIST_DECAY 0.0006F
#else
#define GRPUPUPU_WHISPY_WIND_DIST_DECAY 0.0003F
#endif
#define GRPUPUPU_WHISPY_WINDBOX_TOP 1000.0F
#define GRPUPUPU_WHISPY_WINDBOX_BOTTOM (-10.0F)
#define GRPUPUPU_WHISPY_WINDBOX_EDGELEFT (-2325.0F)
#define GRPUPUPU_WHISPY_WINDBOX_EDGERIGHT 2275.0F

#define GRYAMABUKI_MONSTER_WEAPON_NONE      (0)
#define GRYAMABUKI_MONSTER_WEAPON_WAIT      (1)
#define GRYAMABUKI_MONSTER_WEAPON_INSTANT   (2)
#define GRYAMABUKI_MONSTER_WEAPON_ALL       (3)
#define GRYAMABUKI_MONSTER_WEAPON_MAX       (4U)

typedef struct GRPupupuEffect
{
	Vec3f pos;
	f32 rotate;

} GRPupupuEffect;

typedef struct GRCommonGroundVarsPupupu
{
	void *map_head;
	GObj *map_gobj[4];
	s32 particle_bank_id;
	LBTransform *leaves_xf;
	LBTransform *dust_xf;
	u16 whispy_wind_wait;
	u16 whispy_wind_duration;
	s16 whispy_blink_wait;
	u8 whispy_status;
	u8 flowers_back_wait;
	u8 flowers_front_wait;
	u8 rumble_wait;
	s8 lr_players;
	u8 flowers_back_status;
	u8 flowers_front_status;
	s8 whispy_eyes_status;
	s8 whispy_mouth_status;
	s8 whispy_mouth_texture;
	s8 whispy_eyes_texture;

} GRCommonGroundVarsPupupu;

typedef struct GRSectorDesc
{
	AObjEvent32 *anim_joint_0x0;
	u8 filler_0x4[0x1C - 0x4];
	AObjEvent32 *anim_joint_0x1C;
	void *unk_sectordesc_0x20;
	AObjEvent32 *anim_joint_0x24;
	void *unk_sectordesc_0x28;
	AObjEvent32 *anim_joint_0x2C;

} GRSectorDesc;

typedef struct GRCommonGroundVarsSector
{
	void *map_head;
	GObj *map_gobj;
	DObj *map_dobjs[12];
	void *weapon_head;
	void *map_file;
	f32 arwing_target_x;
	u16 arwing_appear_timer;
	u16 arwing_state_timer;
	u8 arwing_status;
	s8 arwing_flight_pattern;
	u8 arwing_type_cycle;
	u8 arwing_laser_ammo;
	s8 unk_sector_0x4C;
	s8 unk_sector_0x4D;
	u16 unk_sector_0x4E;
	u16 arwing_laser_timer;
	u8 unk_sector_0x52;
	s8 arwing_pilot_curr; // Line IDs?
	u8 arwing_pilot_prev;
	u8 arwing_laser_count; // Laser shot count? e.g. if set to 2, the arwing
						   // will fire 2 lasers at once (though this is
						   // hardcoded to fire two shots in the weapon make
						   // function)
	ub8 is_arwing_z_near;
	ub8 is_arwing_z_collision;
	ub8 is_arwing_line_active;
	ub8 is_arwing_line_collision;

} GRCommonGroundVarsSector;

typedef struct GRZebesAcid
{
	u16 acid_wait_base;
	u16 acid_random_min, acid_random_max;
	f32 acid_level;

} GRZebesAcid;

typedef struct GRCommonGroundVarsZebes
{
	void *map_head;
	GObj *map_gobj;
	GRAttackColl *attack_coll;
	f32 acid_level_curr;
	f32 acid_level_step;
	u16 acid_level_wait;
	u8 acid_status;
	u8 acid_attr_id;
	u8 rumble_wait;

} GRCommonGroundVarsZebes;

typedef struct GRYosterCloud
{
	GObj *gobj;
	DObj *dobj[3];
	f32 altitude;
	f32 pressure; // 0x18
	u8 status;
	s8 anim_id;
	ub8 is_cloud_line_active;
	s8 pressure_timer;
	u8 evaporate_wait; // 0x1C

} GRYosterCloud;

typedef struct GRCommonGroundVarsYoster
{
	void *map_head;
	GRYosterCloud clouds[3];
	s32 particle_bank_id;

} GRCommonGroundVarsYoster;

typedef struct GRInishieScale
{
	DObj *platform_dobj;
	DObj *string_dobj;
	f32 string_length;
	f32 platform_base_y;

} GRInishieScale;

typedef struct GRCommonGroundVarsInishie
{
	void *map_head;
	void *item_head;
	GRInishieScale scale[2];
	f32 splat_alt;	  			// Scale platform uh... thing I don't know how to explain, splat_accelerate gets added to this
	f32 splat_accelerate; 		// Scale platform fall acceleration
	u16 splat_wait;		  		// Wait this many frames before retracting if the platforms have fallen
	u8 splat_status;   			// Current state of the platforms
	u8 players_tt[4]; 	 		// Players timer, something to do with being GRounded/airborne
	ub8 players_ga[4]; 			// Players GRound or air
	GObj *pblock_gobj;
	GRAttackColl *attack_coll;
	u16 pblock_appear_wait;
	u8 pblock_pos_count;
	u8 *pblock_pos_ids;
	u8 pblock_status;
	GObj *pakkun_gobj[2];

} GRCommonGroundVarsInishie;

typedef struct GRCommonGroundVarsJungle
{
	void *map_head;
	GObj *tarucann_gobj;
	u8 tarucann_status;
	u16 tarucann_wait;
	f32 tarucann_rotate_step;

} GRCommonGroundVarsJungle;

typedef struct GRCommonGroundVarsHyrule
{
	void *map_head;
	GObj *twister_gobj;
	LBTransform *twister_xf;
	f32 twister_leftedge_x;
	f32 twister_rightedge_x;
	f32 twister_vel;
	u8 *twister_pos_ids;
	s32 particle_bank_id;
	u16 twister_wait;
	u16 twister_speed_wait;
	u16 twister_turn_wait;
	u16 twister_line_id;
	u8 twister_status;
	u8 twister_pos_count;

} GRCommonGroundVarsHyrule;

typedef struct GRCommonGroundVarsYamabuki
{
	void *map_head;
	void *item_head;
	GObj *monster_gobj;
	GObj *gate_gobj;
	Vec3f gate_pos;
	u8 gate_status;
	ub8 gate_noentry;
	u16 monster_wait;
	u16 gate_wait;
	u8 monster_id_prev;

} GRCommonGroundVarsYamabuki;

typedef struct GRCommonGroundVarsCastle
{
	void *map_head;
	void *item_head;
	GObj *bumper_gobj;
	Vec3f bumper_pos;

} GRCommonGroundVarsCastle;

typedef struct GRBonusGroundVarsBonus1
{
	s32 unk_bonus1_0x0;
	GObj *interface_gobj; // GObj for target count HUD at the top of the screen
	s32 unk_bonus1_0x8;
	u8 target_count;

} GRBonusGroundVarsBonus1;

typedef struct GRBonusTarget
{
	intptr_t start;
	intptr_t dobjdesc;
	intptr_t anim_joint;

} GRBonusTarget;

typedef struct GRBonusGroundVarsBonus2
{
	void *unk_bonus2_0x0;
	void *file;
	void *unk_bonus2_0x8;
	GObj *interface_gobj; // GObj for platform count HUD at the top of the screen
	u8 platform_count;

} GRBonusGroundVarsBonus2;

typedef struct GRBonusGroundVarsBonus3
{
	void *map_head;
	void *item_head;
	Vec3f tarubomb_make_pos;
	s32 tarubomb_make_wait;

} GRBonusGroundVarsBonus3;

#endif
