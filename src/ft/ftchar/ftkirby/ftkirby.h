#ifndef _FTKIRBY_H_
#define _FTKIRBY_H_

#include "ftkirbyfunctions.h"

#define FTKIRBY_JUMPAERIAL_VEL_MUL 0.8F

#define FTKIRBY_COPY_MODELPARTS_JOINT 6

#define FTKIRBY_COPYDAMAGE_LOSECOPY_RANDOM (1.0F / 12.0F)  // 0.083333336F

#define FTKIRBY_CHARGE_EFFECT_JOINT 16

#define FTKIRBY_VACUUM_RELEASE_LAG 40                   // Automatic lag frames before Inhale can be released
#define FTKIRBY_VACUUM_COPY_STICK_RANGE_MIN -40         // Minimum control stick threshold on Y-axis to get copy ability
#define FTKIRBY_VACUUM_TURN_STICK_RANGE_MIN 28
#define FTKIRBY_VACUUM_THROW_DAMAGE 10                  // Damage dealt to inhaled victim upon being shot out
#define FTKIRBY_VACUUM_COPY_DAMAGE 6                    // Damage dealt to inhaled victim upon being copied
#define FTKIRBY_VACUUM_COPY_ANGLE F_CLC_DTOR32(75.0F)   // 1.308997F
#define FTKIRBY_VACUUM_COPY_VEL_BASE 100.0F
#define FTKIRBY_VACUUM_THROW_VEL_BASE 120.0F
#define FTKIRBY_VACUUM_STOPGFX_DIST_MIN 9216.0F
#define FTKIRBY_VACUUM_SPECIALNWAIT_DIST_MIN 1024.0F
#define FTKIRBY_VACUUM_GRAVITY_MUL 2.0F                 
#define FTKIRBY_VACUUM_FALL_MAX_MUL 2.0F

#define FTKIRBY_FINALCUTTER_BEAM_SPAWN_JOINT 0          // Spawn Final Cutter Beam at the position of this bone
#define FTKIRBY_FINALCUTTER_OFF_X 200.0F                // X-Offset of Final Cutter Beam spawn position
#define FTKIRBY_FINALCUTTER_AIR_ACCEL_MUL 0.5F          // Multiplies air acceleration during Final Cutter

#define FTKIRBY_STONE_DURATION_MAX 160                  // Maximum time Kirby can spend in his Stone form
#if defined(REGION_US)
#define FTKIRBY_STONE_DURATION_MIN 18                   // Minimum time Kirby must spend in his Stone form before he can turn back
#else
#define FTKIRBY_STONE_DURATION_MIN 14                   // Minimum time Kirby must spend in his Stone form before he can turn back
#endif
#define FTKIRBY_STONE_FALL_VEL -140.0F                  // Stone terminal velocity in midair
#define FTKIRBY_STONE_SLIDE_ANGLE F_CLC_DTOR32(25.0F)   // Minimum surface angle required to slide? (Radians) (0.43633232F)
#define FTKIRBY_STONE_SLIDE_TRACTION_MUL 1.15F          // Traction multiplier?
#define FTKIRBY_STONE_SLIDE_VEL_MUL 36.0F               // Slide velocity multiplier
#define FTKIRBY_STONE_SLIDE_CLAMP_VEL_X 30.0F           // Maximum slide velocity?
#if defined(REGION_US)
#define FTKIRBY_STONE_HEALTH_MAX 38                     // Maximum % damage Kirby can withstand while transformed
#define FTKIRBY_STONE_HEALTH_MID 22                     // Begin flashing more rapidly when remaining Stone HP is compromised 
#else
#define FTKIRBY_STONE_HEALTH_MAX 50                     // Maximum % damage Kirby can withstand while transformed
#define FTKIRBY_STONE_HEALTH_MID 25                     // Begin flashing more rapidly when remaining Stone HP is compromised 
#endif
#define FTKIRBY_STONE_HEALTH_LOW 10                     // Begin flashing fast when remaining Stone HP is critical
#define FTKIRBY_STONE_COLANIM_ID_HIGH 0x33              // Color Animation ID of high-HP Stone
#define FTKIRBY_STONE_COLANIM_LENGTH_HIGH 0
#define FTKIRBY_STONE_COLANIM_ID_MID 0x34               // Color Animation ID of mid-HP Stone
#define FTKIRBY_STONE_COLANIM_LENGTH_MID 0
#define FTKIRBY_STONE_COLANIM_ID_LOW 0x35               // Color Animation ID of low-HP Stone
#define FTKIRBY_STONE_COLANIM_LENGTH_LOW 0

#define FTKIRBY_COPYMARIO_FIREBALL_SPAWN_JOINT 17

#define FTKIRBY_COPYFOX_BLASTER_SPAWN_JOINT 17
#define FTKIRBY_COPYFOX_BLASTER_SPAWN_OFF_X 70.0F

#define FTKIRBY_COPYDONKEY_GIANTPUNCH_CHARGE_MAX 10
#define FTKIRBY_COPYDONKEY_GIANTPUNCH_CHARGE_DAMAGE_MUL 2 // Uncharged Giant Punch damage = base hitbox damage + charge level * this value
#define FTKIRBY_COPYDONKEY_GIANTPUNCH_CHARGE_COLANIM_ID 6
#define FTKIRBY_COPYDONKEY_GIANTPUNCH_CHARGE_COLANIM_LENGTH 0
#define FTKIRBY_COPYDONKEY_GIANTPUNCH_CHRAGE_ANIM_SPEED 2.0F
#define FTKIRBY_COPYDONKEY_GIANTPUNCH_VEL_MUL 8.0F        // Multiplies horizontal ground velocity only

#define FTKIRBY_COPYSAMUS_CHARGE_JOINT 0
#define FTKIRBY_COPYSAMUS_CHARGE_OFF_Y 200.0F
#define FTKIRBY_COPYSAMUS_CHARGE_OFF_Z 210.0F
#define FTKIRBY_COPYSAMUS_CHARGE_MAX 7                  // Maximum charge level
#define FTKIRBY_COPYSAMUS_CHARGE_INT 20                 // Charge interval, level increrments once this timer hits zero, then begins counting the next iteration
#define FTKIRBY_COPYSAMUS_CHARGE_COLANIM_ID 6
#define FTKIRBY_COPYSAMUS_CHARGE_COLANIM_LENGTH 0
#define FTKIRBY_COPYSAMUS_CHARGE_RECOIL_BASE 10.0F      // Base unit of recoil taken from firing Charge Shot
#define FTKIRBY_COPYSAMUS_CHARGE_RECOIL_MUL 2.0F        // Charge Recoil multiplier
#define FTKIRBY_COPYSAMUS_CHARGE_RECOIL_ADD 20.0F       // Added velocity

#define FTKIRBY_COPYLINK_BOOMERANG_SPAWN_JOINT 0        // Spawn Boomerang at the position of this bone
#define FTKIRBY_COPYLINK_BOOMERANG_SMASH_STICK_MIN 56
#define FTKIRBY_COPYLINK_BOOMERANG_SMASH_BUFFER 8

#define FTKIRBY_COPYCAPTAIN_FALCONPUNCH_VEL_BASE 65.0F  // 
#define FTKIRBY_COPYCAPTAIN_FALCONPUNCH_VEL_MUL 0.92F   //

#define FTKIRBY_COPYPIKACHU_THUNDERJOLT_SPAWN_JOINT 0
#define FTKIRBY_COPYPIKACHU_THUNDERJOLT_SPAWN_ANGLE F_CLC_DTOR32(-45.0F) // -0.7853982F
#define FTKIRBY_COPYPIKACHU_THUNDERJOLT_SPAWN_OFF_X 200.0F
#define FTKIRBY_COPYPIKACHU_THUNDERJOLT_SPAWN_OFF_Y 200.0F
#define FTKIRBY_COPYPIKACHU_THUNDERJOLTVEL 40.0F
#define FTKIRBY_COPYPIKACHU_THUNDERJOLT_COLANIM_ID 0x3B
#define FTKIRBY_COPYPIKACHU_THUNDERJOLT_COLANIM_LENGTH 0

#define FTKIRBY_COPYPURIN_POUND_VEL_BASE 65.0F          //
#define FTKIRBY_COPYPURIN_POUND_VEL_MUL 0.92F           //

#define FTKIRBY_COPYNESS_PKFIRE_SPAWN_JOINT 0
#define FTKIRBY_COPYNESS_PKFIRE_SPAWN_OFF_X 240.0F
#define FTKIRBY_COPYNESS_PKFIRE_SPAWN_OFF_Y 190.0F
#define FTKIRBY_COPYNESS_PKFIRE_SPARK_ANGLE_AIR F_CLC_DTOR32(-38.0F) // -0.6632251F
#define FTKIRBY_COPYNESS_PKFIRE_SPARK_ANGLE_GROUND F_CLC_DTOR32(-3.6F) // -0.06283185F
#if defined(REGION_US)
#define FTKIRBY_COPYNESS_PKFIRE_SPARK_VEL_AIR 95.0F
#define FTKIRBY_COPYNESS_PKFIRE_SPARK_VEL_GROUND 73.0F
#else
#define FTKIRBY_COPYNESS_PKFIRE_SPARK_VEL_AIR 120.0F
#define FTKIRBY_COPYNESS_PKFIRE_SPARK_VEL_GROUND 90.0F
#endif

extern intptr_t lFTKirbySpecialNCopyData;

extern FTStatusDesc dFTKirbySpecialStatusDescs[/* */];

extern void *gFTDataKirbyMain;
extern void *gFTDataKirbyMainMotion;
extern void *gFTDataKirbyModel;
extern void *gFTDataKirbySpecial2;
extern s32 gFTDataKirbyParticleBankID;

typedef enum ftKirbyMotion
{
    nFTKirbyMotionAttack100Start = nFTCommonMotionSpecialStart,
    nFTKirbyMotionAttack100Loop,
    nFTKirbyMotionAttack100End,
    nFTKirbyMotionJumpAerialF1,
    nFTKirbyMotionJumpAerialF2,
    nFTKirbyMotionJumpAerialF3,
    nFTKirbyMotionJumpAerialF4,
    nFTKirbyMotionJumpAerialF5,
    nFTKirbyMotionThrowF,
    nFTKirbyMotionThrowFFall,
    nFTKirbyMotionThrowFLanding,
    nFTKirbyMotionCopyMarioSpecialN,
    nFTKirbyMotionCopyMarioSpecialAirN,
    nFTKirbyMotionCopyLuigiSpecialN,
    nFTKirbyMotionCopyLuigiSpecialAirN,
    nFTKirbyMotionCopyFoxSpecialN,
    nFTKirbyMotionCopyFoxSpecialAirN,
    nFTKirbyMotionCopySamusSpecialNStart,
    nFTKirbyMotionCopySamusSpecialNLoop,
    nFTKirbyMotionCopySamusSpecialNEnd,
    nFTKirbyMotionCopySamusSpecialAirNStart,
    nFTKirbyMotionCopySamusSpecialAirNEnd,
    nFTKirbyMotionCopyDonkeySpecialNStart,
    nFTKirbyMotionCopyDonkeySpecialAirNStart,
    nFTKirbyMotionCopyDonkeySpecialNLoop,
    nFTKirbyMotionCopyDonkeySpecialAirNLoop,
    nFTKirbyMotionCopyDonkeySpecialNEnd,
    nFTKirbyMotionCopyDonkeySpecialAirNEnd,
    nFTKirbyMotionCopyDonkeySpecialNFull,
    nFTKirbyMotionCopyDonkeySpecialAirNFull,
    nFTKirbyMotionAppearR,
    nFTKirbyMotionAppearL,
    nFTKirbyMotionCopyPikachuSpecialN,
    nFTKirbyMotionCopyPikachuSpecialAirN,
    nFTKirbyMotionCopyNessSpecialN,
    nFTKirbyMotionCopyNessSpecialAirN,
    nFTKirbyMotionSpecialHi,
    nFTKirbyMotionSpecialHiLanding,
    nFTKirbyMotionSpecialAirHi,
    nFTKirbyMotionSpecialAirHiFall,
    nFTKirbyMotionSpecialLwStart,
    nFTKirbyMotionSpecialLwUnk,
    nFTKirbyMotionSpecialLwHold,
    nFTKirbyMotionSpecialLwEnd,
    nFTKirbyMotionSpecialAirLwStart,
    nFTKirbyMotionSpecialAirLwHold,
    nFTKirbyMotionSpecialAirLwLanding,
    nFTKirbyMotionSpecialAirLwFall,
    nFTKirbyMotionSpecialAirLwEnd,
    nFTKirbyMotionSpecialNStart,
    nFTKirbyMotionSpecialNLoop,
    nFTKirbyMotionSpecialNEnd,
    nFTKirbyMotionSpecialNEat,
    nFTKirbyMotionSpecialNThrow,
    nFTKirbyMotionSpecialNWait,
    nFTKirbyMotionSpecialNTurn,
    nFTKirbyMotionSpecialNCopy,
    nFTKirbyMotionSpecialAirNStart,
    nFTKirbyMotionSpecialAirNLoop,
    nFTKirbyMotionSpecialAirNEnd,
    nFTKirbyMotionSpecialAirNEat,
    nFTKirbyMotionSpecialAirNThrow,
    nFTKirbyMotionSpecialAirNWait,
    nFTKirbyMotionSpecialAirNTurn,
    nFTKirbyMotionSpecialAirNCopy,
    nFTKirbyMotionCopyLinkSpecialN,
    nFTKirbyMotionCopyLinkSpecialNGet,
    nFTKirbyMotionCopyLinkSpecialNEmpty,
    nFTKirbyMotionCopyLinkSpecialAirN,
    nFTKirbyMotionCopyLinkSpecialAirNReturn,
    nFTKirbyMotionCopyLinkSpecialAirNEmpty,
    nFTKirbyMotionCopyPurinSpecialN,
    nFTKirbyMotionCopyPurinSpecialAirN,
    nFTKirbyMotionCopyCaptainSpecialN,
    nFTKirbyMotionCopyCaptainSpecialAirN,
    nFTKirbyMotionCopyYoshiSpecialN,
    nFTKirbyMotionCopyYoshiSpecialNCatch,
    nFTKirbyMotionCopyYoshiSpecialNRelease,
    nFTKirbyMotionCopyYoshiSpecialAirN,
    nFTKirbyMotionCopyYoshiSpecialAirNCatch,
    nFTKirbyMotionCopyYoshiSpecialAirNRelease

} ftKirbyMotion;

typedef enum ftKirbyStatus
{
    nFTKirbyStatusAttack100Start = nFTCommonStatusSpecialStart,
    nFTKirbyStatusAttack100Loop,
    nFTKirbyStatusAttack100End,
    nFTKirbyStatusJumpAerialF1,
    nFTKirbyStatusJumpAerialF2,
    nFTKirbyStatusJumpAerialF3,
    nFTKirbyStatusJumpAerialF4,
    nFTKirbyStatusJumpAerialF5,
    nFTKirbyStatusThrowF,
    nFTKirbyStatusThrowFFall,
    nFTKirbyStatusThrowFLanding,
    nFTKirbyStatusCopyMarioSpecialN,
    nFTKirbyStatusCopyMarioSpecialAirN,
    nFTKirbyStatusCopyLuigiSpecialN,
    nFTKirbyStatusCopyLuigiSpecialAirN,
    nFTKirbyStatusCopyFoxSpecialN,
    nFTKirbyStatusCopyFoxSpecialAirN,
    nFTKirbyStatusCopySamusSpecialNStart,
    nFTKirbyStatusCopySamusSpecialNLoop,
    nFTKirbyStatusCopySamusSpecialNEnd,
    nFTKirbyStatusCopySamusSpecialAirNStart,
    nFTKirbyStatusCopySamusSpecialAirNEnd,
    nFTKirbyStatusCopyDonkeySpecialNStart,
    nFTKirbyStatusCopyDonkeySpecialAirNStart,
    nFTKirbyStatusCopyDonkeySpecialNLoop,
    nFTKirbyStatusCopyDonkeySpecialAirNLoop,
    nFTKirbyStatusCopyDonkeySpecialNEnd,
    nFTKirbyStatusCopyDonkeySpecialAirNEnd,
    nFTKirbyStatusCopyDonkeySpecialNFull,
    nFTKirbyStatusCopyDonkeySpecialAirNFull,
    nFTKirbyStatusAppearR,
    nFTKirbyStatusAppearL,
    nFTKirbyStatusCopyPikachuSpecialN,
    nFTKirbyStatusCopyPikachuSpecialAirN,
    nFTKirbyStatusCopyNessSpecialN,
    nFTKirbyStatusCopyNessSpecialAirN,
    nFTKirbyStatusSpecialHi,
    nFTKirbyStatusSpecialHiLanding,
    nFTKirbyStatusSpecialAirHi,
    nFTKirbyStatusSpecialAirHiFall,
    nFTKirbyStatusSpecialLwStart,
    nFTKirbyStatusSpecialLwUnk,
    nFTKirbyStatusSpecialLwHold,
    nFTKirbyStatusSpecialLwEnd,
    nFTKirbyStatusSpecialAirLwStart,
    nFTKirbyStatusSpecialAirLwHold,
    nFTKirbyStatusSpecialAirLwLanding,
    nFTKirbyStatusSpecialAirLwFall,
    nFTKirbyStatusSpecialAirLwEnd,
    nFTKirbyStatusSpecialNStart,
    nFTKirbyStatusSpecialNLoop,
    nFTKirbyStatusSpecialNEnd,
    nFTKirbyStatusSpecialNCatch,
    nFTKirbyStatusSpecialNEat,
    nFTKirbyStatusSpecialNThrow,
    nFTKirbyStatusSpecialNWait,
    nFTKirbyStatusSpecialNTurn,
    nFTKirbyStatusSpecialNCopy,
    nFTKirbyStatusSpecialAirNStart,
    nFTKirbyStatusSpecialAirNLoop,
    nFTKirbyStatusSpecialAirNEnd,
    nFTKirbyStatusSpecialAirNCatch,
    nFTKirbyStatusSpecialAirNEat,
    nFTKirbyStatusSpecialAirNThrow,
    nFTKirbyStatusSpecialAirNWait,
    nFTKirbyStatusSpecialAirNTurn,
    nFTKirbyStatusSpecialAirNCopy,
    nFTKirbyStatusCopyLinkSpecialN,
    nFTKirbyStatusCopyLinkSpecialNGet,
    nFTKirbyStatusCopyLinkSpecialNEmpty,
    nFTKirbyStatusCopyLinkSpecialAirN,
    nFTKirbyStatusCopyLinkSpecialAirNReturn,
    nFTKirbyStatusCopyLinkSpecialAirNEmpty,
    nFTKirbyStatusCopyPurinSpecialN,
    nFTKirbyStatusCopyPurinSpecialAirN,
    nFTKirbyStatusCopyCaptainSpecialN,
    nFTKirbyStatusCopyCaptainSpecialAirN,
    nFTKirbyStatusCopyYoshiSpecialN,
    nFTKirbyStatusCopyYoshiSpecialNCatch,
    nFTKirbyStatusCopyYoshiSpecialNRelease,
    nFTKirbyStatusCopyYoshiSpecialAirN,
    nFTKirbyStatusCopyYoshiSpecialAirNCatch,
    nFTKirbyStatusCopyYoshiSpecialAirNRelease

} ftKirbyStatus;

typedef struct FTKirbyCopy
{
    u16 copy_id;
    s16 copy_modelpart_id;
    f32 effect_scale; // Random position offset used for blue sparkle effect?
    s32 star_damage;

} FTKirbyCopy;

typedef struct ftKirbyAttack100Effect
{
    Vec3f offset;
    f32 rotate;
    f32 vel;
    f32 add;

} ftKirbyAttack100Effect;

typedef struct FTKirbyPassiveVars
{
    s32 copy_id;
    s32 copysamus_charge_level;
    s32 copysamus_charge_recoil;
    s32 copydonkey_charge_level;
    s32 copycaptain_falcon_punch_unk;
    s32 copypurin_unk;
    sb32 is_ignore_losecopy;
    GObj *copylink_boomerang_gobj;

} FTKirbyPassiveVars;

typedef struct ftKirbySpecialNStatusVars
{
    s16 copy_id;
    s16 release_lag;
    Vec3f dist;

} ftKirbySpecialNStatusVars;

typedef struct ftKirbySpecialLwStatusVars
{
    s16 duration;
    s16 unk_0x2;
    s16 colanim_id;

} ftKirbySpecialLwStatusVars;

typedef struct FTKirbyCopyDonkeySpecialNStatusVars
{
    sb32 is_release;
    s32 charge_level;
    sb32 is_charging;
    sb32 is_cancel;

} ftKirbyCopyDonkeySpecialNStatusVars;

typedef struct FTKirbyCopySamusSpecialNStatusVars
{
    sb32 is_release;
    s32 charge_int;
    GObj *charge_gobj;

} ftKirbyCopySamusSpecialNStatusVars;

typedef struct FTKirbyCopyLinkSpecialNStatusVars
{
    sb32 is_smash;

} ftKirbyCopyLinkSpecialNStatusVars;

typedef union FTKirbyStatusVars
{
    ftKirbySpecialNStatusVars specialn;
    ftKirbySpecialLwStatusVars speciallw;
    ftKirbyCopyDonkeySpecialNStatusVars copydonkey_specialn;
    ftKirbyCopySamusSpecialNStatusVars copysamus_specialn;
    ftKirbyCopyLinkSpecialNStatusVars copylink_specialn;

} FTKirbyStatusVars;

#endif
