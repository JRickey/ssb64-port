#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159830
void ftBossWalkProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWalkLoopSetStatus);
}

// 0x80159854
void ftBossWalkSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTBossStatusWalk, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftBossCommonCheckEdgeInvertLR(fighter_gobj);

    fp->physics.vel_air.x = 0.0F;
    fp->physics.vel_air.y = fp->coll_data.floor_dist / 60.0F;
}
