#include <ft/fighter.h>
#include <sc/scene.h>

extern alSoundEffect* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018CF90
s32 sFTPublicCommonTicsPast;                // Frames passed since last audience reaction

// 0x8018CF94
u32 sFTPublicCommonPlayerNum;               // Can go beyond actual max player num? Also, either this...

// 0x8018CF98
f32 sFTPublicCommonKnockback;               // Last knockback value that prompted audience reaction?

// 0x8018CF9C
alSoundEffect *sFTPublicCommonALSound;      // Current common reaction sound struct

// 0x8018CFA0
u16 sFTPublicCommonOrder;                   // Current common reaction's sound effect number

// 0x8018CFA4
sb32 sFTPublicCallIsInterrupt;              // If TRUE, stop player's crowd chant

// 0x8018CFA8
s32 sFTPublicCallWait;                      // Wait this much before new audience cheer can occur?

// 0x8018CFAC
s32 sFTPublicCallPlayerNum;                 // ...or this is u32 and the other s32, not entirely sure which

// 0x8018CFB0
alSoundEffect *sFTPublicCallALSound;        // Fighter's chant sound struct

// 0x8018CFB4
u16 sFTPublicCallOrder;                     // Current chant's sound effect number

// 0x8018CFB8
s32 sFTPublicCallCount;                     // Number of times fighter's name has been chanted

// 0x8018CFBC
u32 sFTPublicCallID;                        // Sound effect ID of audience chant for fighter

// 0x8018CFC0
s32 sFTPublicPlayersDown;                   // Number of players too close to the bottom blast zone

// 0x8018CFC4
// s32 D_ovl3_8018CFC4;                     // Might be required padding? Not sure

// 0x8018CFC8                       
u16 sFTPublicDefeatedVoiceIDs[10];          // Array of announcer voices to play for when a player is defeated  

// 0x8018CFDC
u32 sFTPublicDefeatedQueueCurrent;          // Current array index to play from queued "<player> defeated" announcement voice IDs

// 0x8018CFE0
u32 sFTPublicDefeatedQueueEnd;              // Last array index of queued "<player> defeated" announcement voice IDs

// 0x8018CFE4
alSoundEffect *sFTPublicDefeatedALSound;    // "<player> defeated" announcement voice sound struct

// 0x8018CFE8
u16 sFTPublicDefeatedCurrentOrder;          // Current "<player> defeated" announcement's sound effect number

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80164AB0
sb32 ftPublicTryStartCall(GObj *gobj, f32 knockback, s32 player_num)
{
    FTStruct *fp;
    GObj *fighter_gobj = ftParamGetPlayerNumGObj(player_num);
    u16 sfx_id;

    if (fighter_gobj == NULL)
    {
        return FALSE;
    }
    else if ((ftGetStruct(fighter_gobj)->percent_damage < 100) || (sFTPublicCallWait < 1200))
    {
        return FALSE;
    }
    else if (player_num == sFTPublicCallPlayerNum)
    {
        return FALSE;
    }
    else sFTPublicCallID = dFTCommonDataPublicFighterCallFGMs[ftGetStruct(fighter_gobj)->fkind];

    if (sFTPublicCallID == nSYAudioFGMVoiceEnd)
    {
        return FALSE;
    }
    if (sFTPublicCallALSound != NULL)
    {
        sfx_id = sFTPublicCallALSound->sfx_id;

        if ((sfx_id != 0) && (sfx_id == sFTPublicCallOrder))
        {
            func_80026738_27338(sFTPublicCallALSound);
        }
    }
    sFTPublicCallALSound = func_800269C0_275C0((knockback >= 160.0F) ? nSYAudioVoicePublicCheer : nSYAudioVoicePublicAmazed);

    if (sFTPublicCallALSound != NULL)
    {
        sFTPublicCallOrder = sFTPublicCallALSound->sfx_id;
        sFTPublicCallPlayerNum = player_num;
        sFTPublicCallCount = 0;

        return TRUE;
    }
    return FALSE;
}

// 0x80164C18
void ftPublicTryInterruptCall(void)
{
    if ((sFTPublicCallCount < 9) && (sFTPublicCallCount >= 3))
    {
        sFTPublicCallIsInterrupt = TRUE;
    }
}

// 0x80164C44
void ftPublicPlayCommon(u16 new_sfx)
{
    if (sFTPublicCommonALSound != NULL)
    {
        u16 current_sfx = sFTPublicCommonALSound->sfx_id;

        if ((current_sfx != 0) && (current_sfx == sFTPublicCommonOrder))
        {
            func_80026738_27338(sFTPublicCommonALSound);
        }
    }
    sFTPublicCommonALSound = func_800269C0_275C0(new_sfx);

    if (sFTPublicCommonALSound != NULL)
    {
        sFTPublicCommonOrder = sFTPublicCommonALSound->sfx_id;
    }
}

// 0x80164CBC
void ftPublicCommonStop(void)
{
    if (sFTPublicCommonALSound != NULL)
    {
        u16 sfx_id = sFTPublicCommonALSound->sfx_id;

        if ((sfx_id != 0) && (sfx_id == sFTPublicCommonOrder))
        {
            func_80026738_27338(sFTPublicCommonALSound);
        }
    }
}

// 0x80164D08
void ftPublicDecideCall(GObj *gobj, s32 player_num, f32 knockback)
{
    if (knockback >= 130.0F)
    {
        if (ftPublicTryStartCall(gobj, knockback, player_num) != FALSE)
        {
            ftPublicCommonStop();
            return;
        }
        else if (knockback >= 160.0F)
        {
            ftPublicTryInterruptCall();
            ftPublicPlayCommon(nSYAudioVoicePublicCheer);
            return;
        }
        else if (player_num == sFTPublicCallPlayerNum)
        {
            ftPublicTryInterruptCall();
        }
        ftPublicPlayCommon(nSYAudioVoicePublicAmazed);
    }
    else if (knockback >= 100.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspClap);
    }
}

// 0x80164DED4
void ftPublicDecideCommon(GObj *fighter_gobj, s32 player_num, f32 knockback, sb32 is_force_curr_knockback)
{
    if (is_force_curr_knockback != FALSE)
    {
        ftPublicDecideCall(fighter_gobj, player_num, knockback);
    }
    else if ((player_num == sFTPublicCommonPlayerNum) && (sFTPublicCommonTicsPast < 60))
    {
        ftPublicDecideCall(fighter_gobj, player_num, (knockback > sFTPublicCommonKnockback) ? knockback : sFTPublicCommonKnockback);
    }
    else if (knockback >= 160.0F)
    {
        ftPublicTryInterruptCall();
        ftPublicPlayCommon(nSYAudioVoicePublicDamageL);
    }
    else if (knockback >= 130.0F)
    {
        if (player_num == sFTPublicCallPlayerNum)
        {
            ftPublicTryInterruptCall();
        }
        ftPublicPlayCommon(nSYAudioVoicePublicDamageM);
    }
    else if (knockback >= 100.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicDamageS);
    }
    sFTPublicCommonTicsPast = 0;
    sFTPublicCommonPlayerNum = player_num;
    sFTPublicCommonKnockback = knockback;
}

// 0x80164F2C
void ftPublicCommonCheck(GObj *fighter_gobj, f32 knockback, sb32 is_force_curr_knockback)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (knockback >= 100.0F) // Check if knockback is over 100 units
    {
        ftPublicDecideCommon(fighter_gobj, fp->damage_player_num, knockback, is_force_curr_knockback); // Play crowd SFX
    }
}

// 0x80164F70 - Play audience gasp when fighter successfully recovers
void ftPublicPlayCliffReact(GObj *fighter_gobj, f32 knockback)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (sFTPublicCallPlayerNum == fp->player_num)
    {
        ftPublicTryInterruptCall();
    }
    if (knockback >= 160.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspL);
    }
    else if (knockback >= 130.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspM);
    }
    else if (knockback >= 100.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspS);
    }
}

// 0x80165024
void ftPublicTryPlayFallSpecialReact(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 pos_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    if ((pos_y >= gMPCollisionBounds.current.bottom) || (pos_y < -2400.0F))
    {
        return;
    }
    else if (pos_y >= -300.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspL);
    }
    else if (pos_y >= -900.0F)
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspM);
    }
    else ftPublicPlayCommon(nSYAudioVoicePublicGaspS);

    if (sFTPublicCallPlayerNum == fp->player_num)
    {
        ftPublicTryInterruptCall();
    }
}

// 0x801650F8
void ftPublicDefeatedAddID(u16 sfx_id)
{
    sFTPublicDefeatedVoiceIDs[sFTPublicDefeatedQueueEnd] = sfx_id;

    sFTPublicDefeatedQueueEnd++;

    if (sFTPublicDefeatedQueueEnd == ARRAY_COUNT(sFTPublicDefeatedVoiceIDs))
    {
        sFTPublicDefeatedQueueEnd = 0;
    }
}

// 0x80165134
void ftPublicProcUpdate(GObj *public_gobj)
{
    s32 players_down_bak;
    s32 unused[2];
    GObj *down_gobj;
    GObj *fighter_gobj;

    if (sFTPublicCommonTicsPast < (U16_MAX + 1))
    {
        sFTPublicCommonTicsPast++;
    }
    players_down_bak = sFTPublicPlayersDown;
    sFTPublicPlayersDown = 0;
    fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    down_gobj = NULL;

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (!(gSCManagerBattleState->game_rules & SCBATTLE_GAMERULE_STOCK) || (fp->stock_count != -1))
        {
            if (DObjGetStruct(fighter_gobj)->translate.vec.f.y < (gMPCollisionBounds.current.bottom - 100.0F))
            {
                sFTPublicPlayersDown++;
            }
            else down_gobj = fighter_gobj;
        }
        fighter_gobj = fighter_gobj->link_next;
    }
    if ((players_down_bak < 3) && (sFTPublicPlayersDown >= 3))
    {
        ftPublicPlayCommon(nSYAudioVoicePublicGaspL);

        if ((down_gobj != NULL) && (sFTPublicCallPlayerNum == ftGetStruct(down_gobj)->player_num))
        {
            ftPublicTryInterruptCall();
        }
    }
    if (sFTPublicCallCount < 9)
    {
        if ((sFTPublicCallALSound == NULL) || (sFTPublicCallALSound->sfx_id == 0) || (sFTPublicCallALSound->sfx_id != sFTPublicCallOrder))
        {
            sFTPublicCallCount++;

            if (sFTPublicCallCount < 9)
            {
                if (sFTPublicCallIsInterrupt != FALSE)
                {
                    sFTPublicCallIsInterrupt = FALSE;
                    sFTPublicCallWait = 0;
                    sFTPublicCallCount = 9;
                }
                else
                {
                    sFTPublicCallALSound = func_800269C0_275C0(sFTPublicCallID);

                    if (sFTPublicCallALSound != NULL)
                    {
                        sFTPublicCallOrder = sFTPublicCallALSound->sfx_id;
                    }
                }
            }
            else
            {
                sFTPublicCallWait = 0;

                ftPublicPlayCommon(nSYAudioVoicePublicCheer);
            }
        }
    }
    else if (sFTPublicCallWait < 1200)
    {
        sFTPublicCallWait++;
    }
    if (sFTPublicDefeatedQueueCurrent != sFTPublicDefeatedQueueEnd)
    {
        if ((sFTPublicDefeatedALSound == NULL) || (sFTPublicDefeatedCurrentOrder == 0) || (sFTPublicDefeatedALSound->sfx_id != sFTPublicDefeatedCurrentOrder))
        {
            sFTPublicDefeatedALSound = func_800269C0_275C0(sFTPublicDefeatedVoiceIDs[sFTPublicDefeatedQueueCurrent]);

            if (sFTPublicDefeatedALSound != NULL)
            {
                sFTPublicDefeatedCurrentOrder = sFTPublicDefeatedALSound->sfx_id;
            }
            else sFTPublicDefeatedCurrentOrder = 0;

            sFTPublicDefeatedQueueCurrent++;

            if (sFTPublicDefeatedQueueCurrent == ARRAY_COUNT(sFTPublicDefeatedVoiceIDs))
            {
                sFTPublicDefeatedQueueCurrent = 0;
            }
        }
    }
}

// 0x801653E0
void ftPublicMakeActor(void)
{
    gcAddGObjProcess
    (
        gcMakeGObjSPAfter(nGCCommonKindPublic, NULL, 13, GOBJ_PRIORITY_DEFAULT), 
        ftPublicProcUpdate, 
        nGCProcessKindFunc, 
        0
    );
    sFTPublicCommonTicsPast = U16_MAX + 1;
    sFTPublicCommonPlayerNum = -1;
    sFTPublicCommonKnockback = 0.0F;
    sFTPublicCommonALSound = NULL;
    sFTPublicCommonOrder = 0;
    sFTPublicCallIsInterrupt = 0;
    sFTPublicCallWait = 1200;
    sFTPublicCallPlayerNum = -1;
    sFTPublicCallCount = 9;
    sFTPublicPlayersDown = 0;
    sFTPublicDefeatedALSound = NULL;
    sFTPublicDefeatedCurrentOrder = 0;
    sFTPublicDefeatedQueueCurrent = sFTPublicDefeatedQueueEnd = 0;
}
