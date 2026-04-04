#include <ft/fighter.h>
#include <if/interface.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013D580
sb32 ftCommonSleepCheckIgnorePauseMenu(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 steal_from_player[GMCOMMON_PLAYERS_MAX];
    s32 active_teammate_count;
    s32 player;
    s32 stock_count;

    if ((gSCManagerBattleState->game_rules & SCBATTLE_GAMERULE_STOCK) && (gSCManagerBattleState->is_team_battle == TRUE) && (fp->status_id == nFTCommonStatusSleep))
    {
        if (fp->status_vars.common.sleep.stock_steal_wait == 0)
        {
            for (active_teammate_count = 0, stock_count = 0, player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
            {
                if ((player != fp->player) && (gSCManagerBattleState->players[player].pkind != nFTPlayerKindNot) && (fp->team == gSCManagerBattleState->players[player].player))
                {
                    if (gSCManagerBattleState->players[player].stock_count > 0)
                    {
                        if (stock_count < gSCManagerBattleState->players[player].stock_count)
                        {
                            active_teammate_count = 0;
                            stock_count = gSCManagerBattleState->players[player].stock_count;
                        }
                        steal_from_player[active_teammate_count] = player;

                        active_teammate_count++;
                    }
                }
            }
            if (active_teammate_count != 0)
            {
                return TRUE; // Do not bring up pause menu
            }
        }
    }
    return FALSE; // Bring up pause menu
}

// 0x8013D6D0
void ftCommonSleepProcUpdate(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *steal_gobj;
    s32 active_teammate_count;
    s32 steal_from_player[GMCOMMON_PLAYERS_MAX];
    s32 player;
    s32 random_steal_target;
    s32 stock_count;

    if ((gSCManagerBattleState->game_rules & SCBATTLE_GAMERULE_STOCK) && (gSCManagerBattleState->is_team_battle == TRUE))
    {
        if (this_fp->status_vars.common.sleep.stock_steal_wait != 0)
        {
            this_fp->status_vars.common.sleep.stock_steal_wait--;

            if (this_fp->status_vars.common.sleep.stock_steal_wait == 0)
            {
                this_fp->stock_count = 0;
                gSCManagerBattleState->players[this_fp->player].stock_count = 0;

                ftCommonRebirthDownSetStatus(fighter_gobj);
            }
        }
        else
        {
            if (this_fp->input.pl.button_tap & START_BUTTON)
            {
                for (active_teammate_count = 0, stock_count = 0, player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
                {
                    if ((player != this_fp->player) && (gSCManagerBattleState->players[player].pkind != nFTPlayerKindNot) && (this_fp->team == gSCManagerBattleState->players[player].player)) 
                    {
                        if (gSCManagerBattleState->players[player].stock_count > 0)
                        {
                            if (stock_count < gSCManagerBattleState->players[player].stock_count)
                            {
                                active_teammate_count = 0;
                                stock_count = gSCManagerBattleState->players[player].stock_count;
                            }
                            steal_from_player[active_teammate_count] = player;
                            active_teammate_count++;
                        }
                    }
                }
                if (active_teammate_count != 0)
                {
                    random_steal_target = syUtilsRandIntRange(active_teammate_count);

                    gSCManagerBattleState->players[steal_from_player[random_steal_target]].stock_count--;

                    steal_gobj = gSCManagerBattleState->players[steal_from_player[random_steal_target]].fighter_gobj;

                    ftGetStruct(steal_gobj)->stock_count--;

                    this_fp->stock_count = -2;

                    gSCManagerBattleState->players[this_fp->player].stock_count = -2;

                    this_fp->status_vars.common.sleep.stock_steal_wait = FTCOMMON_SLEEP_STOCK_STEAL_WAIT;

                    ifCommonPlayerStockStealMakeInterface(this_fp->player, steal_from_player[random_steal_target]);

                    func_800269C0_275C0(nSYAudioFGMStockSteal);
                }
            }
        }
    }
}

// 0x8013D8B0
void ftCommonSleepSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusSleep, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_invisible = TRUE;
    fp->is_shadow_hide = TRUE;
    fp->is_ghost = TRUE;
    fp->is_menu_ignore = TRUE;

    fp->status_vars.common.sleep.stock_steal_wait = 0;

    fp->camera_mode = nFTCameraModeGhost;

    fp->is_playertag_hide = TRUE;
}
