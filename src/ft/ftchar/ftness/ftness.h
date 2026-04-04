#ifndef _FTNESS_H_
#define _FTNESS_H_

#include <wp/wpvars.h>

#include "ftnessfunctions.h"

// Special move attributes

#define FTNESS_PKFIRE_SPAWN_JOINT nFTPartsJointTopN
#define FTNESS_PKFIRE_SPAWN_OFF_X 100.0F
#define FTNESS_PKFIRE_SPAWN_OFF_Y 180.0F
#define FTNESS_PKFIRE_SPARK_ANGLE_AIR F_CLC_DTOR32(-38.0F) // -0.6632251F
#define FTNESS_PKFIRE_SPARK_ANGLE_GROUND F_CLC_DTOR32(-3.6F) // -0.06283185F
#if defined(REGION_US)
#define FTNESS_PKFIRE_SPARK_VEL_AIR 95.0F
#define FTNESS_PKFIRE_SPARK_VEL_GROUND 73.0F
#else
#define FTNESS_PKFIRE_SPARK_VEL_AIR 120.0F
#define FTNESS_PKFIRE_SPARK_VEL_GROUND 90.0F
#endif

#define FTNESS_PKTHUNDER_TRAIL_POS_COUNT ((WPPKTHUNDER_PARTS_COUNT * 2) + ((WPPKTHUNDER_PARTS_COUNT * 2) / WPPKTHUNDER_PARTS_COUNT))
#define FTNESS_PKTHUNDER_SPAWN_JOINT 12
#define FTNESS_PKTHUNDER_SPAWN_VEL_Y 60.0F
#define FTNESS_PKTHUNDER_COLLIDE_X 250.0F  // Width  of Ness + PK Thunder collision detection box
#define FTNESS_PKTHUNDER_COLLIDE_Y 370.0F  // Height of Ness + PK Thunder collision detection box
#if defined(REGION_US)
#define FTNESS_PKTHUNDER_END_DELAY 30
#define FTNESS_PKTHUNDER_LANDING_LAG F_PCT_TO_DEC(17.0F) // Divides landing animation length by this value
#else
#define FTNESS_PKTHUNDER_END_DELAY 20
#define FTNESS_PKTHUNDER_LANDING_LAG F_PCT_TO_DEC(24.0F) // Divides landing animation length by this value
#endif
#define FTNESS_PKTHUNDER_GRAVITY_DELAY 25
#define FTNESS_PKTHUNDER_FALLSPECIAL_DRIFT 0.6F
#define FTNESS_PKJIBAKU_DELAY 30
#define FTNESS_PKJIBAKU_ANIM_LENGTH 28
#define FTNESS_PKJIBAKU_HALT_ANGLE F_CLC_DTOR32(155.0F) // Maximum angle of stage collision that doesn't cancel PK Thunder Blast? (2.7052603F)
#define FTNESS_PKJIBAKU_VEL 200.0F
#define FTNESS_PKJIBAKU_DECELERATE ((float)(43.0F / 7.0F))
#define FTNESS_PKJIBAKU_PASS_FRAME_END ((float)(FTNESS_PKJIBAKU_ANIM_LENGTH - 3))
#define FTNESS_PKJIBAKU_REBOUND_VEL_MAG 0.5F

#define FTNESS_PSYCHICMAGNET_RELEASE_LAG 30
#define FTNESS_PSYCHICMAGNET_GRAVITY_DELAY 4
#define FTNESS_PSYCHICMAGNET_COLANIM_ID 0x3F
#define FTNESS_PSYCHICMAGNET_COLANIM_LENGTH 0

extern FTStatusDesc dFTNessSpecialStatusDescs[/* */];

extern void *gFTNessFileMain;
extern void *gFTNessFileMainMotion;
extern void *gFTNessFileModel;
extern void *gFTNessFileSpecial1;
extern void *gFTNessFileSpecial2;
extern void *gFTNessFileSpecial3;
extern s32 gFTNessParticleBankID;

// Moveset script IDs
typedef enum ftNessMotion
{
    nFTNessMotionAttack13 = nFTCommonMotionSpecialStart,
    nFTNessMotionAppearRStart,
    nFTNessMotionAppearLStart,
    nFTNessMotionAppearWait,
    nFTNessMotionAppearREnd,
    nFTNessMotionAppearLEnd,
    nFTNessMotionSpecialN,
    nFTNessMotionSpecialAirN,
    nFTNessMotionSpecialHiStart,
    nFTNessMotionSpecialHiHold,
    nFTNessMotionSpecialHiEnd,
    nFTNessMotionSpecialHiJibaku,
    nFTNessMotionSpecialAirHiStart,
    nFTNessMotionSpecialAirHiHold,
    nFTNessMotionSpecialAirHiEnd,
    nFTNessMotionSpecialAirHiBound,
    nFTNessMotionSpecialAirHiJibaku,
    nFTNessMotionSpecialLwStart,
    nFTNessMotionSpecialLwHold,
    nFTNessMotionSpecialLwHit,
    nFTNessMotionSpecialLwEnd,
    nFTNessMotionSpecialAirLwStart,
    nFTNessMotionSpecialAirLwHold,
    nFTNessMotionSpecialAirLwHit,
    nFTNessMotionSpecialAirLwEnd

} ftNessMotion;

// Status IDs
typedef enum ftNessStatus
{
    nFTNessStatusAttack13 = nFTCommonStatusSpecialStart,
    nFTNessStatusAppearRStart,
    nFTNessStatusAppearLStart,
    nFTNessStatusAppearWait,
    nFTNessStatusAppearREnd,
    nFTNessStatusAppearLEnd,
    nFTNessStatusSpecialN,
    nFTNessStatusSpecialAirN,
    nFTNessStatusSpecialHiStart,
    nFTNessStatusSpecialHiHold,
    nFTNessStatusSpecialHiEnd,
    nFTNessStatusSpecialHiJibaku,
    nFTNessStatusSpecialAirHiStart,
    nFTNessStatusSpecialAirHiHold,
    nFTNessStatusSpecialAirHiEnd,
    nFTNessStatusSpecialAirHiBound,
    nFTNessStatusSpecialAirHiJibaku,

    nFTNessStatusSpecialLwScopeStart,                                  // Start of SpecialLw actions
    nFTNessStatusSpecialLwStart = nFTNessStatusSpecialLwScopeStart,
    nFTNessStatusSpecialLwHold,
    nFTNessStatusSpecialLwHit,
    nFTNessStatusSpecialLwEnd,
    nFTNessStatusSpecialAirLwStart,
    nFTNessStatusSpecialAirLwHold,
    nFTNessStatusSpecialAirLwHit,
    nFTNessStatusSpecialAirLwEnd,
    nFTNessStatusSpecialLwScopeEnd = nFTNessStatusSpecialAirLwEnd     // End of SpecialLw actions

} ftNessStatus;

typedef struct FTNessPassiveVars
{
    sb32 is_thunder_destroy;      // Bool to check if PK Thunder exists
    s32 pkthunder_trail_id;
    s16 pkthunder_trail_x[FTNESS_PKTHUNDER_TRAIL_POS_COUNT];
    s16 pkthunder_trail_y[FTNESS_PKTHUNDER_TRAIL_POS_COUNT];

} FTNessPassiveVars;

typedef struct ftNessSpecialHiStatusVars
{
    s32 pkjibaku_delay;            // Frames before PK Thunder Blast can be initiated (PK Thunder will go through Ness while this timer is greater than 0)
    s32 pkthunder_end_delay;       // Frames before PK Thunder transitions to its ending animation after the projectile despawns 
    s32 pkthunder_gravity_delay;   // Frames before Ness is affected by gravity while performing PK Thunder
    GObj *pkthunder_gobj;          // Pointer to the PK Thunder object
    s32 pkjibaku_anim_length;      // Number of frames PK Thunder Blast will loop
    f32 pkjibaku_angle;            // Angle of PK Thunder Blast (converted to 2D velocity vector with sin(angle) and cos(angle))
    Vec3f pkthunder_pos;           // Might be PK Thunder's collision detection when checking if Ness is hitting himself with it?

} ftNessSpecialHiStatusVars;

typedef struct ftNessSpecialLwStatusVars
{
    s32 release_lag;               // Frames PSI Magnet automatically stays in the hold animation after initiating the move
    sb32 is_release;               // Bool to check if PSI Magnet should be dropped (alas the B button is no longer being held)
    s32 gravity_delay;             // Frames before Ness is affected by gravity after initiating PSI Magnet

} ftNessSpecialLwStatusVars;

typedef union FTNessStatusVars
{
    ftNessSpecialHiStatusVars specialhi;
    ftNessSpecialLwStatusVars speciallw;

} FTNessStatusVars;

#endif
