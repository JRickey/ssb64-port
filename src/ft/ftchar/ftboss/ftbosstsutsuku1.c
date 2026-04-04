#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159EA0
void ftBossTsutsuku1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossTsutsuku2SetStatus);
}

// 0x80159EC4
void ftBossTsutsuku1SetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusTsutsuku1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}
