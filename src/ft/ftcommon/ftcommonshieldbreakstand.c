#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTCOMMON_SHIELDBREAKSTAND_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801497A0
void ftCommonShieldBreakStandProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonFuraFuraSetStatus);
}

// 0x801497C4
void ftCommonShieldBreakStandSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id = (fp->status_id == nFTCommonStatusShieldBreakDownD) ? nFTCommonStatusShieldBreakStandD : nFTCommonStatusShieldBreakStandU;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTCOMMON_SHIELDBREAKSTAND_STATUS_FLAGS);
}
