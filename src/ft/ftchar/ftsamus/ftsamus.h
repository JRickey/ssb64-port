#ifndef _FTSAMUS_H_
#define _FTSAMUS_H_

#include "ftsamusfunctions.h"

#define FTSAMUS_CHARGE_JOINT 16             // Bone to attach Charge Shot
#define FTSAMUS_CHARGE_EFFECT_JOINT FTSAMUS_CHARGE_JOINT
#define FTSAMUS_CHARGE_MAX 7                // Maximum charge level
#define FTSAMUS_CHARGE_INT 20               // Charge interval, level increrments once this timer hits zero, then begins counting the next iteration
#define FTSAMUS_CHARGE_COLANIM_ID 0x6
#define FTSAMUS_CHARGE_COLANIM_LENGTH 0
#define FTSAMUS_CHARGE_OFF_X 180.0F         // X-Offset of Charge Shot
#define FTSAMUS_CHARGE_RECOIL_BASE 10.0F    // Base unit of recoil taken from firing Charge Shot
#define FTSAMUS_CHARGE_RECOIL_MUL 2.0F      // Charge Recoil multiplier
#define FTSAMUS_CHARGE_RECOIL_ADD 20.0F     // Added velocity

#define FTSAMUS_SCREWATTACK_PASS_STICK_RANGE_MIN (-44)
#define FTSAMUS_SCREWATTACK_DRIFT_MUL 0.5F
#define FTSAMUS_SCREWATTACK_DRIFT_CLAMP 20.0F
#define FTSAMUS_SCREWATTACK_VEL_X_BASE 10.0F
#define FTSAMUS_SCREWATTACK_VEL_Y_BASE 62.0F
#define FTSAMUS_SCREWATTACK_FALLSPECIAL_DRIFT 0.66F
#define FTSAMUS_SCREWATTACK_LANDING_LAG 0.4F

#define FTSAMUS_BOMB_OFF_Y 60.0F
#define FTSAMUS_BOMB_VEL_Y_BASE 40.0F
#define FTSAMUS_BOMB_VEL_Y_SUB 10.0F
#define FTSAMUS_BOMB_DRIFT 0.66F

extern FTStatusDesc dFTSamusSpecialStatusDescs[/* */];

extern void *gFTDataSamusMain;
extern void *gFTDataSamusMainMotion;
extern void *gFTDataSamusModel;
extern void *gFTDataSamusSpecial1;
extern void *gFTDataSamusSpecial2;
extern void *gFTDataSamusSpecial3;
extern s32 gFTDataSamusParticleBankID;

typedef enum ftSamusMotion
{
    nFTSamusMotionAppearR = nFTCommonMotionSpecialStart,
    nFTSamusMotionAppearL,
    nFTSamusMotionSpecialNStart,
    nFTSamusMotionSpecialNLoop,
    nFTSamusMotionSpecialNEnd,
    nFTSamusMotionSpecialAirNStart,
    nFTSamusMotionSpecialAirNEnd,
    nFTSamusMotionSpecialHi,
    nFTSamusMotionSpecialAirHi,
    nFTSamusMotionSpecialLw,
    nFTSamusMotionSpecialAirLw

} ftSamusMotion;

typedef enum ftSamusStatus
{
    nFTSamusStatusAppearR = nFTCommonStatusSpecialStart,
    nFTSamusStatusAppearL,
    nFTSamusStatusSpecialNStart,
    nFTSamusStatusSpecialNLoop,
    nFTSamusStatusSpecialNEnd,
    nFTSamusStatusSpecialAirNStart,
    nFTSamusStatusSpecialAirNEnd,
    nFTSamusStatusSpecialHi,
    nFTSamusStatusSpecialAirHi,
    nFTSamusStatusSpecialLw,
    nFTSamusStatusSpecialAirLw

} ftSamusStatus;

typedef struct FTSamusPassiveVars
{
    s32 charge_level;
    s32 charge_recoil;

} FTSamusPassiveVars;

typedef struct ftSamusSpecialNStatusVars
{
    sb32 is_release;
    s32 charge_int;
    GObj *charge_gobj;

} ftSamusSpecialNStatusVars;

typedef struct ftSamusSpecialLwStatusVars
{
    sb32 unused;

} ftSamusSpecialLwStatusVars;

typedef union FTSamusStatusVars
{
    ftSamusSpecialNStatusVars specialn;
    ftSamusSpecialLwStatusVars speciallw;

} FTSamusStatusVars;

#endif
