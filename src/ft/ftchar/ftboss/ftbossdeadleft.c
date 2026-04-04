#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015AD50
void ftBossDeadLeftProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossDeadCenterSetStatus);
}

// 0x8015AD74
void ftBossDeadLeftSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp;

    ftMainSetStatus(fighter_gobj, nFTBossStatusDeadLeft, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp = ftGetStruct(fighter_gobj);
    
    fp->physics.vel_air.x = fp->physics.vel_air.y = fp->physics.vel_air.z = 0.0F;
}
