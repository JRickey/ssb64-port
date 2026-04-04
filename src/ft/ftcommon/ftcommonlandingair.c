#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80150E80
void ftCommonLandingAirSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, fp->status_id + (nFTCommonStatusLandingAirEnd - nFTCommonStatusLandingAirStart), 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}
