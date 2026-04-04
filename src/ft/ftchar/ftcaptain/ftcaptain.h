#ifndef _FTCAPTAIN_H_
#define _FTCAPTAIN_H_

#include "ftcaptainfunctions.h"

#define FTCAPTAIN_FALCONPUNCH_VEL_BASE 65.0F
#define FTCAPTAIN_FALCONPUNCH_VEL_MUL 0.92

#define FTCAPTAIN_FALCONDIVE_UNK_TIMER 15
#define FTCAPTAIN_FALCONDIVE_TURN_STICK_RANGE_MIN 18
#define FTCAPTAIN_FALCONDIVE_FALLSPECIAL_DRIFT 0.72F
#if defined(REGION_US)
#define FTCAPTAIN_FALCONDIVE_AIR_ACCEL_MUL 1.1F
#define FTCAPTAIN_FALCONDIVE_AIR_SPEED_MAX_MUL 0.8F
#else
#define FTCAPTAIN_FALCONDIVE_AIR_ACCEL_MUL 1.2F
#define FTCAPTAIN_FALCONDIVE_AIR_SPEED_MAX_MUL 0.84F
#endif
#define FTCAPTAIN_FALCONDIVE_LANDING_LAG 0.65F

#define FTCAPTAIN_FALCONKICK_VEL_SCALE_APPLY_TIME 6
#define FTCAPTAIN_FALCONKICK_VEL_SCALE_DIV 2.0F

extern FTStatusDesc dFTCaptainSpecialStatusDescs[/* */];

extern void *gFTDataCaptainMain;
extern void *gFTDataCaptainMainMotion;
extern void *gFTDataCaptainModel;
extern void *gFTDataCaptainSpecial2;
extern void *gFTDataCaptainSpecial3;
extern s32 gFTDataCaptainParticleBankID;

typedef enum ftCaptainMotion
{
    nFTCaptainMotionAttack13 = nFTCommonMotionSpecialStart,
    nFTCaptainMotionAttack100Start,
    nFTCaptainMotionAttack100Loop,
    nFTCaptainMotionAttack100End,
    nFTCaptainMotionAppearRStart,
    nFTCaptainMotionAppearLStart,
    nFTCaptainMotionAppearREnd,
    nFTCaptainMotionAppearLEnd,
    nFTCaptainMotionSpecialN,
    nFTCaptainMotionSpecialAirN,
    nFTCaptainMotionSpecialLw,                                 // Grounded Falcon Kick
    nFTCaptainMotionSpecialLwAir,                              // Grounded -> aerial Falcon Kick
    nFTCaptainMotionSpecialLwLanding,                          // Landing from aerial Falcon Kick                             
    nFTCaptainMotionSpecialAirLw,                              // Aerial Falcon Kick
    nFTCaptainMotionSpecialLwBound,                            // Wall collision from Falcon Kick
    nFTCaptainMotionSpecialHi,
    nFTCaptainMotionSpecialHiCatch,
    nFTCaptainMotionSpecialHiThrow,
    nFTCaptainMotionSpecialAirHi

} ftCaptainMotion;

typedef enum ftCaptainStatus
{
    nFTCaptainStatusAttack13 = nFTCommonStatusSpecialStart,
    nFTCaptainStatusAttack100Start,
    nFTCaptainStatusAttack100Loop,
    nFTCaptainStatusAttack100End,
    nFTCaptainStatusAppearRStart,
    nFTCaptainStatusAppearLStart,
    nFTCaptainStatusAppearREnd,
    nFTCaptainStatusAppearLEnd,
    nFTCaptainStatusSpecialN,
    nFTCaptainStatusSpecialAirN,
    nFTCaptainStatusSpecialLw,                                 // Grounded Falcon Kick
    nFTCaptainStatusSpecialLwAir,                              // Grounded -> aerial Falcon Kick
    nFTCaptainStatusSpecialLwLanding,                          // Landing from aerial Falcon Kick                             
    nFTCaptainStatusSpecialAirLw,                              // Aerial Falcon Kick
    nFTCaptainStatusSpecialLwBound,                            // Wall collision from Falcon Kick
    nFTCaptainStatusSpecialHi,
    nFTCaptainStatusSpecialHiCatch,
    nFTCaptainStatusSpecialHiThrow,
    nFTCaptainStatusSpecialAirHi

} ftCaptainStatus;

typedef struct FTCaptainPassiveVars
{
    s32 falcon_punch_unk;

} FTCaptainPassiveVars;

typedef struct ftCaptainSpecialHiStatusVars
{
    u16 flags;
    Vec3f vel;

} ftCaptainSpecialHiStatusVars;

typedef struct ftCaptainSpecialLwStatusVars
{
    u16 scale_apply_timer;
    f32 vel_scale;

} ftCaptainSpecialLwStatusVars;

typedef union FTCaptainStatusVars
{
    ftCaptainSpecialHiStatusVars specialhi;
    ftCaptainSpecialLwStatusVars speciallw;

} FTCaptainStatusVars;

#endif
