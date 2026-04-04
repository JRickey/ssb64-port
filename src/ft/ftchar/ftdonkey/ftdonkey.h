#ifndef _FTDONKEY_H_
#define _FTDONKEY_H_

#include "ftdonkeyfunctions.h"

#define FTDONKEY_CHARGE_EFFECT_JOINT 16

#define FTDONKEY_GIANTPUNCH_CHARGE_MAX 10
#define FTDONKEY_GIANTPUNCH_CHARGE_DAMAGE_MUL 2 // Uncharged Giant Punch damage = base hitbox damage + charge level * this value
#define FTDONKEY_GIANTPUNCH_CHARGE_COLANIM_ID 6
#define FTDONKEY_GIANTPUNCH_CHARGE_COLANIM_LENGTH 0
#define FTDONKEY_GIANTPUNCH_CHRAGE_ANIM_SPEED 2.0F
#define FTDONKEY_GIANTPUNCH_VEL_MUL 8.0F        // Multiplies horizontal ground velocity only

#define FTDONKEY_SPINNINGKONG_GROUND_ACCEL 0.025F
#define FTDONKEY_SPINNINGKONG_GROUND_VEL_MAX 26.0F
#if defined(REGION_US)
#define FTDONKEY_SPINNINGKONG_AIR_VEL_Y 20.3F
#else
#define FTDONKEY_SPINNINGKONG_AIR_VEL_Y 18.0F
#endif
#define FTDONKEY_SPINNINGKONG_AIR_ACCEL 0.05F
#define FTDONKEY_SPINNINGKONG_AIR_VEL_MAX 38.0F
#define FTDONKEY_SPINNINGKONG_START_GRAVITY_MUL 0.07F
#define FTDONKEY_SPINNINGKONG_END_GRAVITY_MUL 1.0F
#define FTDONKEY_SPINNINGKONG_FALLSPECIAL_DRIFT 1.0F
#define FTDONKEY_SPINNINGKONG_LANDING_LAG 0.3F

extern FTStatusDesc dFTDonkeySpecialStatusDescs[/* */];

extern void *gFTDataDonkeyMain;
extern void *gFTDataDonkeyMainMotion;
extern void *gFTDataDonkeyModel;
extern void *gFTDataDonkeySpecial2;
extern s32 gFTDataDonkeyParticleBankID;

typedef enum ftDonkeyMotion
{
	nFTDonkeyMotionAppearR = nFTCommonMotionSpecialStart,
	nFTDonkeyMotionAppearL,
	nFTDonkeyMotionSpecialNStart,
	nFTDonkeyMotionSpecialAirNStart,
	nFTDonkeyMotionSpecialNLoop,
	nFTDonkeyMotionSpecialAirNLoop,
	nFTDonkeyMotionSpecialNEnd,
	nFTDonkeyMotionSpecialAirNEnd,
	nFTDonkeyMotionSpecialNFull,
	nFTDonkeyMotionSpecialAirNFull,
	nFTDonkeyMotionSpecialHi,
	nFTDonkeyMotionSpecialAirHi,
	nFTDonkeyMotionSpecialLwStart,
	nFTDonkeyMotionSpecialLwLoop,
	nFTDonkeyMotionSpecialLwEnd,
	nFTDonkeyMotionThrowFWait,
	nFTDonkeyMotionThrowFWalkSlow,
	nFTDonkeyMotionThrowFWalkMiddle,
	nFTDonkeyMotionThrowFWalkFast,
	nFTDonkeyMotionThrowFTurn,
	nFTDonkeyMotionThrowFKneeBend,
	nFTDonkeyMotionThrowFFall,
	nFTDonkeyMotionThrowFLanding,
	nFTDonkeyMotionThrowFDamage,
	nFTDonkeyMotionThrowFF,
	nFTDonkeyMotionThrowAirFF,

	// These last few are probably nonexistent

	nFTDonkeyMotionHeavyThrowF,
	nFTDonkeyMotionHeavyThrowB,
	nFTDonkeyMotionHeavyThrowF4,
	nFTDonkeyMotionHeavyThrowB4

} ftDonkeyMotion;

typedef enum ftDonkeyStatus
{
	nFTDonkeyStatusAppearR = nFTCommonStatusSpecialStart,
	nFTDonkeyStatusAppearL,
	nFTDonkeyStatusSpecialNStart,
	nFTDonkeyStatusSpecialAirNStart,
	nFTDonkeyStatusSpecialNLoop,
	nFTDonkeyStatusSpecialAirNLoop,
	nFTDonkeyStatusSpecialNEnd,
	nFTDonkeyStatusSpecialAirNEnd,
	nFTDonkeyStatusSpecialNFull,
	nFTDonkeyStatusSpecialAirNFull,
	nFTDonkeyStatusSpecialHi,
	nFTDonkeyStatusSpecialAirHi,
	nFTDonkeyStatusSpecialLwStart,
	nFTDonkeyStatusSpecialLwLoop,
	nFTDonkeyStatusSpecialLwEnd,

	nFTDonkeyStatusThrowFStart,

	nFTDonkeyStatusThrowFWait = nFTDonkeyStatusThrowFStart,
	nFTDonkeyStatusThrowFWalkSlow,
	nFTDonkeyStatusThrowFWalkMiddle,
	nFTDonkeyStatusThrowFWalkFast,
	nFTDonkeyStatusThrowFTurn,
	nFTDonkeyStatusThrowFKneeBend,
	nFTDonkeyStatusThrowFFall,
	nFTDonkeyStatusThrowFLanding,
	nFTDonkeyStatusThrowFDamage,
	nFTDonkeyStatusThrowFEnd = nFTDonkeyStatusThrowFDamage,
	nFTDonkeyStatusThrowFF,
	nFTDonkeyStatusThrowAirFF,

	nFTDonkeyStatusHeavyThrowStart,	// Start of HeavyThrow action states
	nFTDonkeyStatusHeavyThrowF = nFTDonkeyStatusHeavyThrowStart,
	nFTDonkeyStatusHeavyThrowB,
	nFTDonkeyStatusHeavyThrowF4,
	nFTDonkeyStatusHeavyThrowB4,
	nFTDonkeyStatusHeavyThrowEnd = nFTDonkeyStatusHeavyThrowB4,

} ftDonkeyStatus;

typedef struct FTDonkeyPassiveVars
{
	s32 charge_level;

} FTDonkeyPassiveVars;

typedef struct ftDonkeySpecialNStatusVars
{
	sb32 is_release;
	s32 charge_level;
	sb32 is_charging;
	sb32 is_cancel;

} ftDonkeySpecialNStatusVars;

typedef struct ftDonkeySpecialHiStatusVars
{
	s32 unk_0x0;

} ftDonkeySpecialHiStatusVars;

typedef struct ftDonkeySpecialLwStatusVars
{
	sb32 is_loop;

} ftDonkeySpecialLwStatusVars;

typedef union FTDonkeyStatusVars
{
	ftDonkeySpecialNStatusVars specialn;
	ftDonkeySpecialHiStatusVars specialhi;
	ftDonkeySpecialLwStatusVars speciallw;

} FTDonkeyStatusVars;

#endif
