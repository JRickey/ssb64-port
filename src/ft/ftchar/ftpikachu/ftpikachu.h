#ifndef _FTPIKACHU_H_
#define _FTPIKACHU_H_

#include <ft/fttypes.h>

#include "ftpikachufunctions.h"

#define FTPIKACHU_THUNDERJOLT_SPAWN_JOINT 11					// Spawn Thunder Jolt relative to this bone
#define FTPIKACHU_THUNDERJOLT_SPAWN_ANGLE F_CLC_DTOR32(-45.0F)	// Launch angle of Thunder Jolt in Radians (-0.7853982F)
#define FTPIKACHU_THUNDERJOLTVEL 40.0F							// Constant velocity of aerial Thunder Jolt
#define FTPIKACHU_THUNDERJOLT_COLANIM_ID 0x3B					// ID of color animation applied to Pikachu's model
#define FTPIKACHU_THUNDERJOLT_COLANIM_LENGTH 0

#define FTPIKACHU_QUICKATTACK_BASE_JOINT 4						// Apply calculations to this joint (i.e. rotation, scale)
#define FTPIKACHU_QUICKATTACK_COLANIM_ID 0x39					// ID of color animation applied to Pikachu's model
#define FTPIKACHU_QUICKATTACK_COLANIM_LENGTH 0
#define FTPIKACHU_QUICKATTACK_SCALE_X 0.8F						// X scale of Pikachu's model
#define FTPIKACHU_QUICKATTACK_SCALE_Y 0.8F						// Y scale of Pikachu's model
#define FTPIKACHU_QUICKATTACK_SCALE_Z 1.2F						// Z scale of Pikachu's model
#define FTPIKACHU_QUICKATTACK_HALT_ANGLE F_CLC_DTOR32(135.0F)	// Maximum angle of stage collision that doesn't cancel Quick Attack? (2.3561945F)
#define FTPIKACHU_QUICKATTACK_START_TIME 20						// Number of frames Pikachu spends in the startup animation of Quick Attack
#define FTPIKACHU_QUICKATTACK_ZIP_TIME 5						// Number of frames Pikachu spends in a Quick Attack zip
#define FTPIKACHU_QUICKATTACK_PASS_BUFFER_MAX 2
#define FTPIKACHU_QUICKATTACK_STICK_RANGE_MIN 60.0F				// Minimum stick range required for input read?
#define FTPIKACHU_QUICKATTACK_ANGLE_DIFF_MIN F_CLC_DTOR32(42.0F)// Minimum angle difference required for subsequent zips (0.7330383F)
#define FTPIKACHU_QUICKATTACK_VEL_BASE 3.0F						// Base velocity of grounded Quick Attack
#define FTPIKACHU_QUICKATTACK_VEL_ADD 90.0F						// Added to ground velocity
#define FTPIKACHU_QUICKATTACK_VEL_MUL 0.9F						// Multiplies ground velocity on some condition
#define FTPIKACHU_QUICKATTACK_AIR_ACCEL_MUL 0.5F
#define FTPIKACHU_QUICKATTACK_AIR_SPEED_MUL 0.5F
#define FTPIKACHU_QUICKATTACK_VEL_Y_DIV 9.0F
#define FTPIKACHU_QUICKATTACK_VEL_BAK_MUL 0.2F					// Multiplies backed up velocity?
#define FTPIKACHU_QUICKATTACK_FALLSPECIAL_DRIFT 0.4F			// Air drift multiplier once Pikachu enters freefall after Quick Attack
#define FTPIKACHU_QUICKATTACK_LANDING_LAG 0.4F					// Landing lag

#define FTPIKACHU_THUNDER_SPAWN_JOINT 11						// Spawn Thunder relative to this bone
#define FTPIKACHU_THUNDER_SPAWN_OFF_Y 500.0F					// Thunder spawns at (top map_bound - this value)
#define FTPIKACHU_THUNDER_VEL_Y -450.0F							// Vertical velocity of Thunder
#define FTPIKACHU_THUNDER_COLL_OFF_Y 225.0F						// Added to Thunder's Y-position before calculating distance from Pikachu
#define FTPIKACHU_THUNDER_COLLIDE_X 200.0F						// Width of Thunder collision box
#define FTPIKACHU_THUNDER_COLLIDE_Y	800.0F						// Height of Thunder collision box
#define FTPIKACHU_THUNDER_HIT_GRAVITY 0.5F
#define FTPIKACHU_THUNDER_HITVEL_Y	20.0F						// Vertical velocity gained if Thunder connects with Pikachu

extern FTStatusDesc dFTPikachuSpecialStatusDescs[/* */];

extern void *gFTDataPikachuMain;
extern void *gFTDataPikachuMainMotion;
extern void *gFTDataPikachuModel;
extern void *gFTDataPikachuSpecial1;
extern void *gFTDataPikachuSpecial2;
extern void *gFTDataPikachuSpecial3;
extern s32 gFTDataPikachuParticleBankID;

typedef enum ftPikachuMotion
{
	// Missing SpecialHiStart and SpecialAirHiStart; these action states don't have a script, so their ID is -1

	nFTPikachuMotionAppearR = nFTCommonMotionSpecialStart,
	nFTPikachuMotionAppearL,
	nFTPikachuMotionSpecialN,
	nFTPikachuMotionSpecialAirN,
	nFTPikachuMotionSpecialLwStart,
	nFTPikachuMotionSpecialLwLoop,
	nFTPikachuMotionSpecialLwHit,
	nFTPikachuMotionSpecialLwEnd,
	nFTPikachuMotionSpecialAirLwStart,
	nFTPikachuMotionSpecialAirLwLoop,
	nFTPikachuMotionSpecialAirLwHit,
	nFTPikachuMotionSpecialAirLwEnd,
	nFTPikachuMotionSpecialHi,
	nFTPikachuMotionSpecialHiEnd,
	nFTPikachuMotionSpecialAirHi,
	nFTPikachuMotionSpecialAirHiEnd

} ftPikachuMotion;

typedef enum ftPikachuStatus
{
	nFTPikachuStatusAppearR = nFTCommonStatusSpecialStart,
	nFTPikachuStatusAppearL,
	nFTPikachuStatusSpecialN,
	nFTPikachuStatusSpecialAirN,
	nFTPikachuStatusSpecialLwStart,
	nFTPikachuStatusSpecialLwLoop,
	nFTPikachuStatusSpecialLwHit,
	nFTPikachuStatusSpecialLwEnd,
	nFTPikachuStatusSpecialAirLwStart,
	nFTPikachuStatusSpecialAirLwLoop,
	nFTPikachuStatusSpecialAirLwHit,
	nFTPikachuStatusSpecialAirLwEnd,
	nFTPikachuStatusSpecialHiStart,
	nFTPikachuStatusSpecialHi,
	nFTPikachuStatusSpecialHiEnd,
	nFTPikachuStatusSpecialAirHiStart,
	nFTPikachuStatusSpecialAirHi,
	nFTPikachuStatusSpecialAirHiEnd

} ftPikachuStatus;

typedef struct FTPikachuPassiveVars
{
	sb32 is_thunder_destroy;

} FTPikachuPassiveVars;

typedef struct ftPikachuSpecialHiStatusVars
{
	s32 anim_frames;
	sb32 is_subsequent_zip;
	u32 unk_0x8;
	Vec2i stick_range;
	s32 pass_timer;
	f32 vel_x_bak;
	f32 vel_y_bak;
	f32 vel_ground_bak;

} ftPikachuSpecialHiStatusVars;

typedef struct ftPikachuSpecialLwStatusVars
{
	GObj *thunder_gobj;

} ftPikachuSpecialLwStatusVars;

typedef union FTPikachuStatusVars
{
	ftPikachuSpecialHiStatusVars specialhi;
	ftPikachuSpecialLwStatusVars speciallw;

} FTPikachuStatusVars;

#endif
