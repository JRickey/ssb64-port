#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159380
void ftBossHippatakuProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWaitSetStatus);
}

// 0x801593A4
void ftBossHippatakuSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusHippataku, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftBossCommonCheckEdgeInvertLR(fighter_gobj);
}
