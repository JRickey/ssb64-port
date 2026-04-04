#ifndef _FTFOX_H_
#define _FTFOX_H_

#include "ftfoxfunctions.h"

#define FTFOX_BLASTER_HOLD_JOINT 17                     // Joint to attach Blaster
#define FTFOX_BLASTER_SPAWN_OFF_X 60.0F                 // X-Offset of Blaster spawn

#define FTFOX_FIREFOX_LAUNCH_DELAY 35                   // Startup frames required to pass for Firefox to take off 
#define FTFOX_FIREFOX_GRAVITY_DELAY 15                  // Frames required for gravity to take effect
#define FTFOX_FIREFOX_DECELERATE_DELAY 2                // Frames required to pass for Firefox to being deceleration
#define FTFOX_FIREFOX_DECELERATE_VEL 3.03571438789F     // Value removed from velocity per frame on deceleration
#define FTFOX_FIREFOX_DECELERATE_END 1.5F               // Value removed from velocity per frame on end animation (ground only?)
#define FTFOX_FIREFOX_BOUND_ANGLE F_CLC_DTOR32(20.0F)   // Maximum angle difference between Firefox angle and ground angle for rebound
#define FTFOX_FIREFOX_TRAVEL_TIME 30                    // Frames Firefox travels
#define FTFOX_FIREFOX_VEL 115.0F
#define FTFOX_FIREFOX_ANGLE_STICK_THRESHOLD 45          // Minimum stick range required to angle Firefox
#define FTFOX_FIREFOX_MODEL_STICK_THRESHOLD 11          // Minimum stick range required to change Fox's model rotation
#define FTFOX_FIREFOX_AIR_DRIFT 1.0F                    // Aerial drift multiplier during freefall (FallSpecial)
#define FTFOX_FIREFOX_LANDING_LAG 0.34F                 // Divides animation length and rounds it up (7 / 0.34 = 21 frames of landing lag)

#define FTFOX_REFLECTOR_RELEASE_LAG 18                  // Number of frames Fox is automatically locked in Reflector before the B button is read
#define FTFOX_REFLECTOR_TURN_FRAMES 4                   // Number of frames it takes Fox to turn around during Reflector
#define FTFOX_REFLECTOR_GRAVITY_DELAY 4                 // Frames required for gravity to take effect
#define FTFOX_REFLECTOR_GRAVITY 0.8F

extern FTStatusDesc dFTFoxSpecialStatusDescs[/* */];

extern void *gFTDataFoxMain;
extern void *gFTDataFoxMainMotion;
extern void *gFTDataFoxModel;
extern void *gFTDataFoxSpecial1;
extern void *gFTDataFoxSpecial2;
extern void *gFTDataFoxSpecial3;
extern void *gFTDataFoxSpecial4;
extern s32 gFTDataFoxParticleBankID;

typedef enum ftFoxMotion
{
    nFTFoxMotionAttack100Start = nFTCommonMotionSpecialStart,
    nFTFoxMotionAttack100Loop,
    nFTFoxMotionAttack100End,
    nFTFoxMotionAppearR,
    nFTFoxMotionAppearL,
    nFTFoxMotionSpecialN,
    nFTFoxMotionSpecialAirN,
    nFTFoxMotionSpecialHiStart,
    nFTFoxMotionSpecialAirHiStart,
    nFTFoxMotionSpecialHiHold,
    nFTFoxMotionSpecialAirHiHold,
    nFTFoxMotionSpecialHi,
    nFTFoxMotionSpecialAirHi,
    nFTFoxMotionSpecialHiEnd,
    nFTFoxMotionSpecialAirHiEnd,
    nFTFoxMotionSpecialAirHiBound,
    nFTFoxMotionSpecialLwStart,
    nFTFoxMotionSpecialLwLoop,
    nFTFoxMotionSpecialLwHit,
    nFTFoxMotionSpecialLwEnd,
    nFTFoxMotionSpecialAirLwStart,
    nFTFoxMotionSpecialAirLwLoop,
    nFTFoxMotionSpecialAirLwHit,
    nFTFoxMotionSpecialAirLwEnd

} ftFoxMotion;

typedef enum ftFoxStatus
{
    nFTFoxStatusAttack100Start = nFTCommonStatusSpecialStart,
    nFTFoxStatusAttack100Loop,
    nFTFoxStatusAttack100End,
    nFTFoxStatusAppearR,
    nFTFoxStatusAppearL,
    nFTFoxStatusSpecialN,
    nFTFoxStatusSpecialAirN,
    nFTFoxStatusSpecialHiStart,
    nFTFoxStatusSpecialAirHiStart,
    nFTFoxStatusSpecialHiHold,
    nFTFoxStatusSpecialAirHiHold,
    nFTFoxStatusSpecialHi,
    nFTFoxStatusSpecialAirHi,
    nFTFoxStatusSpecialHiEnd,
    nFTFoxStatusSpecialAirHiEnd,
    nFTFoxStatusSpecialAirHiBound,

    nFTFoxStatusSpecialLwScopeStart,                               // Start of SpecialLw actions
    nFTFoxStatusSpecialLwStart = nFTFoxStatusSpecialLwScopeStart,
    nFTFoxStatusSpecialLwHit,
    nFTFoxStatusSpecialLwEnd,
    nFTFoxStatusSpecialLwLoop,
    nFTFoxStatusSpecialLwTurn,
    nFTFoxStatusSpecialAirLwStart,
    nFTFoxStatusSpecialAirLwHit,
    nFTFoxStatusSpecialAirLwEnd,
    nFTFoxStatusSpecialAirLwLoop,
    nFTFoxStatusSpecialAirLwTurn,
    nFTFoxStatusSpecialLwScopeEnd = nFTFoxStatusSpecialAirLwTurn  // End of SpecialLw actions

} ftFoxStatus;

typedef struct ftFoxSpecialHiStatusVars
{
    s32 launch_delay;
    s32 gravity_delay;
    f32 angle;
    s32 anim_frames;
    s32 decelerate_wait;
    s32 pass_timer;

} ftFoxSpecialHiStatusVars;

typedef struct ftFoxSpecialLwStatusVars
{
    s32 release_lag;
    s32 turn_tics;
    sb32 is_release;
    GObj *effect_gobj;
    s32 gravity_delay;

} ftFoxSpecialLwStatusVars;

typedef union FTFoxStatusVars
{
    ftFoxSpecialHiStatusVars specialhi;
    ftFoxSpecialLwStatusVars speciallw;

} FTFoxStatusVars;

#endif
