#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015AE80
void ftBossDeadRightSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->lr = -1;

    ftMainSetStatus(fighter_gobj, nFTBossStatusDeadRight, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_air.x = fp->physics.vel_air.y = fp->physics.vel_air.z = 0.0F;
}
