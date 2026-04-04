#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015A2F0
void ftBossOkukoukiProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWaitSetStatus);
}

// 0x8015A314
void ftBossOkukoukiSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusOkukouki, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftBossCommonCheckPlayerInvertLR(fighter_gobj);
}
