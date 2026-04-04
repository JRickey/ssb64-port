#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159AA0
void ftBossWalkShootProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWaitSetStatus);
}

// 0x80159AC4
void ftBossWalkShootSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusWalkShoot, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}
