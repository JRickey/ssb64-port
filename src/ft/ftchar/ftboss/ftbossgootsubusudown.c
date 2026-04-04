#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159E20
void ftBossGootsubusuDownProcMap(GObj *fighter_gobj)
{
    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        ftBossGootsubusuEndSetStatus(fighter_gobj);
    }
}

// 0x80159E50
void ftBossGootsubusuDownSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTBossStatusGootsubusuDown, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_air.x = 0.0F;
    fp->physics.vel_air.y = -400.0F;
}
