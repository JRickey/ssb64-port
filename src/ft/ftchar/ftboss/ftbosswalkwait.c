#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159A30
void ftBossWalkWaitProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWaitSetStatus);
}

// 0x80159A54
void ftBossWalkWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTBossStatusWalkWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_air.x = fp->physics.vel_air.y = fp->physics.vel_air.z = 0;
}
