#ifndef _FTMARIO_H_
#define _FTMARIO_H_

#include "ftmariofunctions.h"

#define FTMARIO_FIREBALL_SPAWN_JOINT 16

#define FTMARIO_SUPERJUMP_TURN_STICK_THRESHOLD 50   // Minimum stick range required for direction change (velocity)
#define FTMARIO_SUPERJUMP_TURN_MODEL_THRESHOLD 20   // Minimum stick range required for direction change (model rotation)
#define FTMARIO_SUPERJUMP_AIR_DRIFT 0.6F            // Aerial drift multiplier during freefall (FallSpecial)
#define FTMARIO_SUPERJUMP_LANDING_LAG 0.28F         // Divides animation length and rounds it up (7 / 0.28 = 25 frames of landing lag) 
#define FTMARIO_SUPERJUMP_STICK_Y_UNK -44           // ???

#define FTMARIO_TORNADO_VEL_X_GROUND 0.025F         // Base X-velocity on ground
#define FTMARIO_TORNADO_VEL_X_AIR 0.03F             // Base X-velocity in midair
#define FTMARIO_TORNADO_VEL_X_CLAMP 17.0F           // Maximum X-velocity
#define FTMARIO_TORNADO_VEL_Y_CLAMP 40.0F           // Maximum Y-velocity during Tornado
#define FTMARIO_TORNADO_VEL_Y_BASE 15.0F            // Subtract 0.0 or FTMARIO_TORNADO_VEL_Y_TAP based on whether tornado charge is empty or full, respectively
#define FTMARIO_TORNADO_VEL_Y_TAP 22.0F             // Y-velocity added per B-tap during Mario Tornado

extern FTStatusDesc dFTMarioSpecialStatusDescs[/* */];
extern FTMotionDesc dFTMarioMotionDescs[/* */];

extern void *gFTMarioFileMain;
extern void *gFTMarioFileMainMotion;
extern void *gFTMarioFileModel;
extern void *gFTMarioFileSpecial1;
extern void *gFTMarioFileSpecial2;
extern void *gFTMarioFileSpecial3;
extern s32 gFTMarioParticleBankID;

typedef enum ftMarioMotion
{
    nFTMarioMotionAttack13 = nFTCommonMotionSpecialStart,
    nFTMarioMotionAppearR,
    nFTMarioMotionAppearL,
    nFTMarioMotionSpecialN,
    nFTMarioMotionSpecialAirN,
    nFTMarioMotionSpecialHi,
    nFTMarioMotionSpecialAirHi,
    nFTMarioMotionSpecialLw,
    nFTMarioMotionSpecialAirLw

} ftMarioMotion;

typedef enum ftMarioStatus
{
    nFTMarioStatusAttack13 = nFTCommonStatusSpecialStart,
    nFTMarioStatusAppearR,
    nFTMarioStatusAppearL,
    nFTMarioStatusSpecialN,
    nFTMarioStatusSpecialAirN,
    nFTMarioStatusSpecialHi,
    nFTMarioStatusSpecialAirHi,
    nFTMarioStatusSpecialLw,
    nFTMarioStatusSpecialAirLw

} ftMarioStatus;

typedef struct FTMarioPassiveVars
{
    sb32 is_expend_tornado;

} FTMarioPassiveVars;

typedef struct ftMarioSpecialHiStatusVars
{
    sb32 is_air_bool;

} ftMarioSpecialHiStatusVars;

typedef struct ftMarioSpecialLwStatusVars
{
    f32 friction; // Ground drift of Mario Tornado
    s32 dust_effect_int;  // Interval between dust GFX kicked up during Mario Tornado

} ftMarioSpecialLwStatusVars;

typedef union FTMarioStatusVars
{
    ftMarioSpecialHiStatusVars specialhi;
    ftMarioSpecialLwStatusVars speciallw;

} FTMarioStatusVars;

#endif
