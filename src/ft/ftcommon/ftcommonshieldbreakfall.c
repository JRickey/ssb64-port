#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTCOMMON_SHIELDBREAKFALL_STATUS_FLAGS (FTSTATUS_PRESERVE_DAMAGEPLAYER | FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80149720
void ftCommonShieldBreakFallProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftCommonShieldBreakDownSetStatus);
}

// 0x80149744
void ftCommonShieldBreakFallSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusShieldBreakFall, 0.0F, 1.0F, FTCOMMON_SHIELDBREAKFALL_STATUS_FLAGS); // 0x10B2
    ftPhysicsClampAirVelXMax(fp);
    ftParamMakeRumble(fp, 3, 0);
}
