#ifndef _WPVARS_H_
#define _WPVARS_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <wp/wpdef.h>

#define WPBLASTER_VEL_X 160.0F
#define WPBLASTER_ADD_SCALE_X (16.0F / 3.0F)
#define WPBLASTER_CLAMP_SCALE_X (160.0F / 3.0F)

#define WPCHARGESHOT_GFX_SIZE_DIV 30.0F
#define WPCHARGESHOT_ROTATE_SPEED F_CLC_DTOR32(18.0F)

#define WPSAMUSBOMB_EXPLODE_LIFETIME 6
#define WPSAMUSBOMB_EXPLODE_SIZE 180.0F
#define WPSAMUSBOMB_WAIT_LIFETIME 100					 			// Countdown of Bomb until it explodes
#define WPSAMUSBOMB_WAIT_VEL_Y 10.0F					 			// Initial velocity of Bomb
#define WPSAMUSBOMB_WAIT_ROTATE_SPEED_AIR F_CLC_DTOR32(20.0F)	 	// Bomb spins at this rate while airborne
#define WPSAMUSBOMB_WAIT_ROTATE_SPEED_GROUND F_CLC_DTOR32(10.0F) 	// Bomb spins at this rate while grounded
#define WPSAMUSBOMB_WAIT_COLLIDE_MOD_VEL 0.9F			 			// Modify velocity when colliding with an obstruction?
#define WPSAMUSBOMB_WAIT_GRAVITY 1.0F					 			// Bomb's gravity
#define WPSAMUSBOMB_WAIT_TVEL 50.0F					 			// Bomb's terminal velocity
#define WPSAMUSBOMB_WAIT_BLINK_SLOW                                                                                    \
	40 // The light in the center of the bomb blinks at its "slow" rate while
	   // lifetime is greater than this value
#define WPSAMUSBOMB_WAIT_BLINK_MID                                                                                     \
	20 // The light in the center of the bomb blinks at its "mid" rate while
	   // lifetime is greater than this value
#define WPSAMUSBOMB_WAIT_BLINK_TIMER_SLOW 8 // Switch out Bomb's texture animation frame when this timer reaches 0
#define WPSAMUSBOMB_WAIT_BLINK_TIMER_MID 5	// Switch out Bomb's texture animation frame when this timer reaches 0
#define WPSAMUSBOMB_WAIT_BLINK_TIMER_FAST 3 // Switch out Bomb's texture animation frame when this timer reaches 0

#define WPPIKACHUJOLT_VEL 55.0F
#define WPPIKACHUJOLT_GRAVITY 0.0F
#if defined(REGION_US)
#define WPPIKACHUJOLT_LIFETIME 100
#define WPPIKACHUJOLT_TVEL 50.0F
#else
#define WPPIKACHUJOLT_LIFETIME 120
#define WPPIKACHUJOLT_TVEL 45.0F
#endif
#define WPPIKACHUJOLT_ROTATE_ANGLE_MAX F_CLC_DTOR32(100.0F)
#define WPPIKACHUJOLT_ANIM_PUSH_FRAME 7.5F // Frame on which Thunder Jolt gets pushed ahead to simulate movement?
#define WPPIKACHUJOLT_COLL_GROUND 0		   // ID of Thunder Jolt collision type
#define WPPIKACHUJOLT_COLL_RWALL 1		   // ID of Thunder Jolt collision type
#define WPPIKACHUJOLT_COLL_CEIL 2		   // ID of Thunder Jolt collision type
#define WPPIKACHUJOLT_COLL_LWALL 3		   // ID of Thunder Jolt collision type

#define WPPIKACHUTHUNDER_TEXTURES_NUM 4    // Number of textures Thunder contains
#define WPPIKACHUTHUNDER_SPAWN_LIFETIME 40 // Duration of inital Thunder projectile?
#define WPPIKACHUTHUNDER_TRAIL_LIFETIME 10 // Duration of subsequent Thunder segments?
#define WPPIKACHUTHUNDER_EXPIRE 6 // Thunder can no longer damage opponents once its lifetime has dipped below this number

#define WPPKFIRE_LIFETIME 20
#define WPPKFIRE_POS_MUL 160.0F

#define WPPKTHUNDER_LIFETIME 160
#define WPPKTHUNDER_SPAWN_TRAIL_FRAME (WPPKTHUNDER_LIFETIME - 2) // Subtracted from PK Thunder's maximum lifetime to determine when to begin spawning trails
#define WPPKTHUNDER_TURN_STICK_THRESHOLD 45 // Minimum stick range required to steer PK Thunder
#define WPPKTHUNDER_ANGLE_STEP F_CLC_DTOR32(6.0F) // If there is a difference between PK Thunder and the control stick's current angle, step this amount
#define WPPKTHUNDER_ANGLE_DIV (45.0F / 6.0F) // Divide angle difference then add to current PK Thunder angle if less than 45 degrees
#define WPPKTHUNDER_VEL 60.0F
#define WPPKTHUNDER_REFLECT_POS_Y_ADD 250.0F // Added to Y position when PK Thunder is reflected
#define WPPKTHUNDER_PARTS_COUNT 5
#define WPPKTHUNDER_TEXTURES_NUM 4

#define WPFINALCUTTER_LIFETIME 20
#define WPFINALCUTTER_VEL 100.0F

#define WPEGGTHROW_LIFETIME 50
#define WPEGGTHROW_EXPLODE_LIFETIME 10
#define WPEGGTHROW_EXPLODE_SIZE 340.0F
#define WPEGGTHROW_TRAJECTORY_DIV 65.0F
#define WPEGGTHROW_TRAJECTORY_SUB_FORWARD F_CLC_DTOR32(73.0F)
#define WPEGGTHROW_TRAJECTORY_SUB_BEHIND F_CLC_DTOR32(107.0F)					   
#define WPEGGTHROW_ANGLE_MUL F_CLC_DTOR32(20.0F)   // Default: 20 degrees
#define WPEGGTHROW_ANGLE_CLAMP F_CLC_DTOR32(6.0F) // Default: 6 degrees; if less than this value, angle becomes 0.0
#define WPEGGTHROW_VEL_ADD 50.0F
#define WPEGGTHROW_VEL_FORCE_MUL 2.3F // Multiplies number of frames the B button was held
#define WPEGGTHROW_ANGLE_FORCE_MUL (-2.1F)
#define WPEGGTHROW_ANGLE_ADD (-1.5F)
#define WPEGGTHROW_GRAVITY 2.7F
#define WPEGGTHROW_TVEL 120.0F

#define WPYOSHISTAR_LIFETIME 16
#define WPYOSHISTAR_LIFETIME_SCALE_MUL 0.175F
#define WPYOSHISTAR_LIFETIME_SCALE_ADD 0.3F
#define WPYOSHISTAR_ROTATE_SPEED 0.24F
#define WPYOSHISTAR_VEL_CLAMP 1.8F
#define WPYOSHISTAR_ANGLE F_CLC_DTOR32(30.0F)
#define WPYOSHISTAR_VEL 30.0F
#define WPYOSHISTAR_OFF_X 300.0F
#define WPYOSHISTAR_OFF_Y 20.0F

#define WPSPINATTACK_LIFETIME 100
#define WPSPINATTACK_EXTEND_POS_COUNT 4
#define WPSPINATTACK_VEL 28.0F
#define WPSPINATTACK_VEL_CLAMP 0.4F
#define WPSPINATTACK_OFF_X 40.0F
#define WPSPINATTACK_OFF_Y 80.0F
#define WPSPINATTACK_ANGLE F_CLC_DTOR32(10.0F) // 0.17453294F

#define WPBOOMERANG_OFF_X 150.0F
#define WPBOOMERANG_OFF_Y 290.0F
#define WPBOOMERANG_HOMING_ANGLE_MAX F_CST_DTOR32( 1.5F)
#define WPBOOMERANG_HOMING_ANGLE_MIN F_CST_DTOR32(0.75F)
#define WPBOOMERANG_VEL_SMASH 114.0F
#define WPBOOMERANG_VEL_TILT 85.0F
#define WPBOOMERANG_RETURN_DAMAGE 8
#define WPBOOMERANG_ANGLE_STICK_THRESHOLD 10 // Minimum stick Y range required to angle Boomerang up or down
#define WPBOOMERANG_LIFETIME_SMASH 190
#define WPBOOMERANG_LIFETIME_TILT 160
#define WPBOOMERANG_LIFETIME_REFLECT 100

#define WPYUBIBULLET_EXPLODE_LIFETIME 6
#define WPYUBIBULLET_EXPLODE_SIZE 180.0F
#define WPYUBIBULLETVEL_X 160.0F
#define WPYUBIBULLETVEL_Y -25.0F

typedef enum wpPikachuThunderCollide
{
	nWPPikachuThunderStatusActive,
	nWPPikachuThunderStatusCollide,
	nWPPikachuThunderStatusDestroy

} wpPikachuThunderCollide;

typedef enum wpPikachuThunderJoltStatus
{
    nWPPikachuThunderJoltStatusNull,
    nWPPikachuThunderJoltStatusActive,
    nWPPikachuThunderJoltStatusDestroy

} wpPikachuThunderJoltStatus;

typedef enum wpNessPKThunderCollide
{
	nWPNessPKThunderStatusActive,	// PK Thunder is active
	nWPNessPKThunderStatusDestroy, // PK Thunder despawns
	nWPNessPKThunderStatusCollide	// PK Thunder collides with Ness

} wpNessPKThunderCollide;

typedef struct wpMarioFireballAttributes
{
	s32 lifetime;
	f32 vel_terminal;
	f32 vel_min; // Minimum velocity required for Fireball to stay active
	f32 gravity;
	f32 collide_rebound;
	f32 rotate_speed;
	f32 angle_ground;
	f32 angle_air;
	f32 vel_base;
	void* p_weapon;
	intptr_t offset;
	f32 anim_frame; // Frame of texture animation to use? 0 = Mario Fireball, 1
					// = Luigi Fireball

} wpMarioFireballAttributes;

typedef struct wpSamusChargeShotAttributes
{
	f32 gfx_size;
	f32 vel_x;
	s32 damage;
	s32 attack_size;
	s32 coll_size;
	u32 shoot_fgm_id;
	u32 charge_fgm_id;
	u32 attack_fgm_id;
	s32 priority;

} wpSamusChargeShotAttributes;

typedef struct wpSamusWeaponVarsBomb
{
	s32 bomb_blink_timer;

} wpSamusWeaponVarsBomb;

typedef struct wpMarioWeaponVarsFireball
{
	s32 index; // Index of Fireball description to use; 0 = Mario, 1 = Luigi

} wpMarioWeaponVarsFireball;

typedef struct wpPikachuWeaponVarsThunderJolt
{
	s32 line_type;
	Vec3f rotate;

} wpPikachuWeaponVarsThunderJolt;

typedef struct wpPikachuWeaponVarsThunder // Pikachu's Thunder
{
	s32 thunder_state;

} wpPikachuWeaponVarsThunder;

typedef struct _wpNessWeaponVarsPKThunder
{
	s32 status;
	f32 angle;
	GObj* parent_gobj; // PK Thunder's original owner
	GObj* trail_gobj[WPPKTHUNDER_PARTS_COUNT];

} wpNessWeaponVarsPKThunder;

typedef struct _wpNessWeaponVarsPKThunderTrail
{
	s32 status;
	s32 trail_id;  // Also key of RGB struct to use to set color of PK
					  // Thunder trails?
	GObj* parent_gobj; // Original owner?
	GObj* head_gobj;

} wpNessWeaponVarsPKThunderTrail;

typedef struct wpSamusWeaponVarsChargeShot
{
	sb32 is_release;
	sb32 is_full_charge;
	s32 charge_size;
	GObj* owner_gobj;

} wpSamusWeaponVarsChargeShot;

typedef struct wpLinkWeaponVarsSpinAttack
{
	s16 pos_x[WPSPINATTACK_EXTEND_POS_COUNT];
	s16 pos_y[WPSPINATTACK_EXTEND_POS_COUNT];
	Vec2f vel;
	sb8 is_destroy;
	s8 pos_index;

} wpLinkWeaponVarsSpinAttack;

typedef struct wpLinkWeaponVarsBoomerang
{
	GObj* parent_gobj; // GObj that spawned Boomerang
	u8 flyforward_timer;
	u8 homing_delay;
	u8 flags;
	u8 adjust_angle_delay;
	f32 default_angle;
	f32 homing_angle;

} wpLinkWeaponVarsBoomerang;

typedef struct wpYoshiWeaponVarsEggThrow
{
	sb8 is_spin; // Also determines whether Egg Throw is affected by gravity,
				 // not sure what to name it
	sb8 is_throw;
	s8 stick_range;
	s8 lr;
	s16 throw_force;
	f32 angle;

} wpYoshiWeaponVarsEggThrow;

typedef struct wpStarRodWeaponVarsStar // Star Rod projectile
{
	s32 lifetime;

} wpStarRodWeaponVarsStar;

typedef struct wpIwarkWeaponVarsRock
{
	s32 unk_0x0; // Unused?
	s32 floor_line_id;
	s32 unk_0x8; // Unused?
	s32 unk_0xC; // Set but never used?
	GObj* owner_gobj;

} wpIwarkWeaponVarsRock;

typedef struct wpNyarsWeaponVarsCoin
{
	s32 lifetime;

} wpNyarsWeaponVarsCoin;

typedef struct wpKamexWeaponVarsHydro
{
	s32 unk_0x0; // Set to 0 but never used
	s32 unk_0x4; // Set to 0 but never used

} wpKamexWeaponVarsHydro;

typedef struct wpDogasWeaponVarsSmog
{
	WPAttributes *attr;

} wpDogasWeaponVarsSmog;

#endif
