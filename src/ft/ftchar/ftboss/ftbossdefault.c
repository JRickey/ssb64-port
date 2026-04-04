#include <ft/fighter.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80158750
void ftBossDefaultProcInterrupt(GObj *fighter_gobj)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        ftBossWaitSetStatus(fighter_gobj);
    }
}

// 0x80158784
void ftBossDefaultSetStatus(GObj *fighter_gobj) // Unused
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusDefault, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}
