#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159DB0
void ftBossGootsubusuEndProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWaitSetStatus);
}

// 0x80159DD4
void ftBossGootsubusuEndSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTBossStatusGootsubusuEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_air.y = 0.0F;
}
