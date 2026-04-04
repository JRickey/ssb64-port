#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTCOMMON_SHIELDBREAKDOWN_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80149680 - Probably its own file?
void ftCommonShieldBreakDownProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonShieldBreakStandSetStatus);
}

// 0x801496A4
void ftCommonShieldBreakDownSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterGround(fp);
    }
    status_id = (ftCommonDownBounceCheckUpOrDown(fighter_gobj) != 0) ? nFTCommonStatusShieldBreakDownD : nFTCommonStatusShieldBreakDownU;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTCOMMON_SHIELDBREAKDOWN_STATUS_FLAGS);
    ftCommonDownBounceUpdateEffects(fighter_gobj);
}
