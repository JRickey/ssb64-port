#include <ft/fighter.h>
#include <sc/scene.h>
#include <sys/develop.h>
#include <lb/library.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern FTFileSize gSCManagerFighterFileSizes[nFTKindEnumCount];

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80130D80
FTStruct *sFTManagerStructsAllocFree;

// 0x80130D84
FTStruct *sFTManagerStructsAllocBuf;

// 0x80130D88
FTParts *sFTManagerPartsAllocFree;

// 0x80130D8C
FTParts *sFTManagerPartsAllocBuf;

// 0x80130D90
u32 gFTManagerPlayersNum;

// 0x80130D94
u16 gFTManagerMotionCount;

// 0x80130D96
u16 gFTManagerStatUpdateCount;

// 0x80130D98
void *gFTManagerCommonFile;

// 0x80130D9C
size_t gFTManagerFigatreeHeapSize;

// 0x80130DA0
LBFileNode sFTManagerForceStatusBuffer[7];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D6FE0
void ftManagerSetupFileSize(void)
{
    s32 i, j;
    size_t largest_size;
    size_t current_anim_size;
    FTData *data;
    FTFileSize *file_size;
    LBRelocSetup rl_setup;
    FTMotionDesc *motion_desc;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = NULL;
    rl_setup.status_buffer_size = 0;
    rl_setup.force_status_buffer = sFTManagerForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sFTManagerForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);

    for (i = 0; i < nFTKindEnumCount; i++)
    {
        file_size = &gSCManagerFighterFileSizes[i];
        data = dFTManagerDataFiles[i];

        largest_size = 0;

        file_size->main = lbRelocGetFileSize(data->file_main_id);

        for (j = 0; j < data->mainmotion_array_count; j++)
        {
            motion_desc = &data->mainmotion->motion_desc[j];

            current_anim_size = data->mainmotion->motion_desc[j].anim_file_id;

            if (motion_desc->anim_file_id != 0)
            {
                if (!(motion_desc->anim_desc.flags.is_use_shieldpose))
                {
                    current_anim_size = lbRelocGetFileSize(motion_desc->anim_file_id);

                    if (largest_size < current_anim_size)
                    {
                        largest_size = current_anim_size;
                    }
                }
            }
        }
        file_size->mainmotion_largest_anim = largest_size;

        for (j = 0; j < *data->submotion_array_count; j++)
        {
            motion_desc = &data->submotion->motion_desc[j];

            current_anim_size = data->submotion->motion_desc[j].anim_file_id;

            if (motion_desc->anim_file_id != 0)
            {
                if (!(motion_desc->anim_desc.flags.is_use_shieldpose))
                {
                    current_anim_size = lbRelocGetFileSize(motion_desc->anim_file_id);

                    if (largest_size < current_anim_size)
                    {
                        largest_size = current_anim_size;
                    }
                }
            }
        }
        file_size->submotion_largest_anim = largest_size;
    }
}

// 0x800D7194
void ftManagerAllocFighter(u32 data_flags, s32 allocs_num)
{
    size_t largest_size;
    s32 i;
    size_t current_size;
    FTData *data;
    FTFileSize *file_size;
    size_t heap_size;

    heap_size = 0;

    sFTManagerStructsAllocBuf = sFTManagerStructsAllocFree = syTaskmanMalloc(sizeof(FTStruct) * allocs_num, 0x8);

    bzero(sFTManagerStructsAllocBuf, sizeof(FTStruct) * allocs_num);

    for (i = 0; i < (allocs_num - 1); i++)
    {
        sFTManagerStructsAllocBuf[i].next = &sFTManagerStructsAllocBuf[i + 1];
    }
    sFTManagerStructsAllocBuf[i].next = NULL;

    sFTManagerPartsAllocFree = sFTManagerPartsAllocBuf = syTaskmanMalloc(sizeof(FTParts) * allocs_num * FTPARTS_JOINT_NUM_MAX, 0x8);

    for (i = 0; i < ((allocs_num * FTPARTS_JOINT_NUM_MAX) - 1); i++)
    {
        sFTManagerPartsAllocBuf[i].next = &sFTManagerPartsAllocBuf[i + 1];
    }
    sFTManagerPartsAllocBuf[i].next = NULL;

    gFTManagerPlayersNum = 1;
    gFTManagerMotionCount = 1;
    gFTManagerStatUpdateCount = 1;

    gFTManagerCommonFile = lbRelocGetExternHeapFile((u32)&llFTManagerCommonFileID, syTaskmanMalloc(lbRelocGetFileSize((u32)&llFTManagerCommonFileID), 0x10));

    lbRelocGetExternHeapFile((u32)&ll_201_FileID, syTaskmanMalloc(lbRelocGetFileSize((u32)&ll_201_FileID), 0x10));

    for (i = 0; i < (nFTKindEnumCount + ARRAY_COUNT(gSCManagerFighterFileSizes)) / 2; i++)
    {
        data = dFTManagerDataFiles[i];
        file_size = &gSCManagerFighterFileSizes[i];

        largest_size = 0;

        *data->p_file_main = NULL;

        data->file_main_size = file_size->main;

        if (data_flags & FTDATA_FLAG_MAINMOTION)
        {
            current_size = file_size->mainmotion_largest_anim;

            if (current_size != 0)
            {
                largest_size = current_size;
            }
        }
        if (data_flags & FTDATA_FLAG_SUBMOTION)
        {
            current_size = file_size->submotion_largest_anim;

            if (largest_size < current_size)
            {
                largest_size = current_size;
            }
        }
        data->file_anim_size = largest_size;

        if (heap_size < data->file_anim_size)
        {
            heap_size = data->file_anim_size;
        }
    }
    gFTManagerFigatreeHeapSize = heap_size;

    if (data_flags & FTDATA_FLAG_SUBMOTION)
    {
        scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

// 0x800D7594
FTStruct* ftManagerGetNextStructAlloc(void)
{
    FTStruct *current_fighter;
    FTStruct *new_fighter = sFTManagerStructsAllocFree;

    if (new_fighter == NULL)
    {
        while (TRUE)
        {
            syDebugPrintf("couldn\'t get Fighter struct.\n");
            scManagerRunPrintGObjStatus();
        }
    }
    else current_fighter = new_fighter;

    sFTManagerStructsAllocFree = new_fighter->next;

    return current_fighter;
}

// 0x800D75EC
void ftManagerSetPrevStructAlloc(FTStruct *fp)
{
    fp->next = sFTManagerStructsAllocFree;
    sFTManagerStructsAllocFree = fp;
}

// 0x800D7604
FTParts* ftManagerGetNextPartsAlloc(void)
{
    FTParts *current_part;
    FTParts *new_part;

    new_part = sFTManagerPartsAllocFree;

    if (new_part == NULL)
    {
        while (TRUE)
        {
            syDebugPrintf("couldn\'t get FighterParts struct.\n");
            scManagerRunPrintGObjStatus();
        }
    }
    current_part = new_part;

    sFTManagerPartsAllocFree = new_part->next;

    new_part->transform_update_mode =
    new_part->unk_dobjtrans_0x5 =
    new_part->unk_dobjtrans_0x7 = 0;
    new_part->unk_dobjtrans_0x6 = 0;

    new_part->gobj = NULL;
    new_part->is_have_anim = FALSE;

    return current_part;
}

// 0x800D767C
void ftManagerSetPrevPartsAlloc(FTParts *parts)
{
    parts->next = sFTManagerPartsAllocFree;
    sFTManagerPartsAllocFree = parts;
}

// 0x800D7694
void ftManagerSetupFilesMainKind(s32 fkind)
{
    FTData *data = dFTManagerDataFiles[fkind];

    *data->p_file_main = lbRelocGetExternHeapFile(data->file_main_id, syTaskmanMalloc(lbRelocGetFileSize(data->file_main_id), 0x10));

    if (data->particles_script_lo != 0x0)
    {
        *data->p_particle = efParticleGetLoadBankID
        (
            data->particles_script_lo, 
            data->particles_script_hi, 
            data->particles_texture_lo, 
            data->particles_texture_hi
        );
    }
}

// 0x800D7710
void ftManagerSetupFilesKind(s32 fkind)
{
    FTData *data = dFTManagerDataFiles[fkind];

    if (data->file_mainmotion_id != 0)
    {
        *data->p_file_mainmotion = lbRelocGetStatusBufferFile(data->file_mainmotion_id);
    }
    if (data->file_submotion_id != 0)
    {
        *data->p_file_submotion = lbRelocGetStatusBufferFile(data->file_submotion_id);
    }
    *data->p_file_model = lbRelocGetStatusBufferFile(data->file_model_id);

    if (data->file_shieldpose_id != 0)
    {
        data->p_file_shieldpose = lbRelocGetStatusBufferFile(data->file_shieldpose_id);
    }
    if (data->file_special1_id != 0)
    {
        *data->p_file_special1 = lbRelocGetStatusBufferFile(data->file_special1_id);
    }
    if (data->file_special2_id != 0)
    {
        *data->p_file_special2 = lbRelocGetStatusBufferFile(data->file_special2_id);
    }
    if (data->file_special3_id != 0)
    {
        *data->p_file_special3 = lbRelocGetStatusBufferFile(data->file_special3_id);
    }
    if (data->file_special4_id != 0)
    {
        *data->p_file_special4 = lbRelocGetStatusBufferFile(data->file_special4_id);
    }
    if (data->particles_script_lo != 0x0)
    {
        *data->p_particle = efParticleGetBankID(data->particles_script_lo);
    }
}

// 0x800D782C
void ftManagerSetupFilesPlayablesAll(void)
{
    s32 i;

    for (i = 0; i <= nFTKindPlayableEnd; i++)
    {
        ftManagerSetupFilesKind(i);
    }
}

// 0x800D786C
void ftManagerSetupFilesAllKind(s32 fkind)
{
    FTData *data = dFTManagerDataFiles[fkind];

    if (*data->p_file_main == NULL)
    {
        ftManagerSetupFilesMainKind(fkind);
        ftManagerSetupFilesKind(fkind);
    }
}

// 0x800D78B4
void* ftManagerAllocFigatreeHeapKind(s32 fkind)
{
    FTData *data = dFTManagerDataFiles[fkind];

    return syTaskmanMalloc(data->file_anim_size, 0x10);
}

// 0x800D78E8
void ftManagerDestroyFighter(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 i;

    if (fp->is_effect_attach)
    {
        ftParamProcStopEffect(fighter_gobj);
    }
    for (i = 0; i < ARRAY_COUNT(fp->joints); i++)
    {
        if (fp->joints[i] != NULL)
        {
            FTParts *parts = fp->joints[i]->user_data.p;

            if (parts->gobj != NULL)
            {
                gcEjectGObj(parts->gobj);
            }
            ftManagerSetPrevPartsAlloc(parts);
        }
    }
    ftManagerSetPrevStructAlloc(fp);
    gcEjectGObj(fighter_gobj);
}

// 0x800D7994
void ftManagerDestroyFighterWeapons(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    switch (fp->fkind)
    {
    case nFTKindKirby:
    case nFTKindNKirby:
        ftKirbyCopyLinkSpecialNDestroyBoomerang(fighter_gobj);
        break;

    case nFTKindLink:
    case nFTKindNLink:
        ftLinkSpecialNDestroyBoomerang(fighter_gobj);
        break;
    }
}

// 0x800D79F0
void ftManagerInitFighter(GObj *fighter_gobj, FTDesc *desc)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    f32 scale;

    fp->lr = desc->lr;
    fp->percent_damage = desc->damage;

    if (fp->pkind != nFTPlayerKindDemo)
    {
        gSCManagerBattleState->players[fp->player].stock_damage_all = fp->percent_damage;
    }
    fp->shield_health = (fp->fkind == nFTKindYoshi) ? 55 : 55;

#ifdef BUGFIX_CRASH_SELFDESTRUCT
    /*
     * shield_player is never explicitly initialized during creation, so its value is 0.
     * This can result in a crash if a player other than P1 self-destructs directly
     * from a shield break without getting their shield hit, as shield_player
     * is moved to damage_player, which is then used to award points
     * to the player who caused the shield break.
     * This results in an invalid GObj* being accessed without a NULL check, crashing the game.
     */
    fp->shield_player = -1;
#endif

    fp->unk_ft_0x38 = 0.0F;
    fp->hitlag_tics = 0;
    fp->is_knockback_paused = FALSE;

    ftPhysicsStopVelAll(fighter_gobj);

    fp->jumps_used = 0;
    fp->is_reflect = FALSE;
    fp->is_absorb = FALSE;
    fp->is_shield = FALSE;
    fp->is_effect_attach = FALSE;
    fp->is_jostle_ignore = FALSE;

    fp->cliffcatch_wait = 0;
    fp->tics_since_last_z = 0;

    fp->acid_wait = fp->twister_wait = fp->tarucann_wait = fp->damagefloor_wait = 0;

    fp->unk_ft_0x7AC = 0;
    fp->attack_damage = 0;
    fp->attack_count = 0;
    fp->attack_shield_push = 0;
    fp->shield_damage = 0;
    fp->damage_lag = 0;
    fp->damage_queue = 0;
    fp->damage_player = -1;
    fp->damage_object_class = 0;
    fp->damage_object_kind = 0;
    fp->damage_count = 0;
    fp->damage_kind = nFTDamageKindDefault;
    fp->damage_heal = 0;
    fp->damage_joint_id = 0;
    fp->invincible_tics = 0;
    fp->intangible_tics = 0;
    fp->star_invincible_tics = 0;

    fp->hitstatus = nGMHitStatusNormal;
    fp->star_hitstatus = nGMHitStatusNormal;
    fp->special_hitstatus = nGMHitStatusNormal;

    fp->throw_gobj = NULL;
    fp->catch_gobj = NULL;
    fp->capture_gobj = NULL;
    fp->is_catch_or_capture = FALSE;

    fp->item_gobj = NULL;

    fp->reflect_lr = 0;
    fp->absorb_lr = 0;

    fp->reflect_damage = 0;

    fp->special_coll = NULL;

    fp->attack1_followup_frames = 0.0F;
    fp->unk_ft_0x7A0 = 0.0F;
    fp->attack_knockback = 0.0F;
    fp->attack_rebound = 0.0F;
    fp->damage_knockback_stack = 0.0F;
    fp->knockback_resist_status = 0.0F;
    fp->knockback_resist_passive = 0.0F;
    fp->damage_knockback = 0.0F;
    fp->hitlag_mul = 1.0F;
    fp->shield_heal_wait = 10.0F;

    fp->is_fastfall = FALSE;

    fp->player_num = gFTManagerPlayersNum++;

    fp->public_knockback = 0.0F;

    fp->is_hitstun = FALSE;
    fp->is_use_animlocks = FALSE;

    fp->shuffle_frame_index = fp->shuffle_index_max = 0;

    fp->is_use_fogcolor = FALSE;

    fp->is_shuffle_electric = FALSE;
    fp->shuffle_tics = 0;

    fp->motion_attack_id = nFTMotionAttackIDNone;
    fp->motion_count = 0;
    fp->stat_flags.attack_id = nFTStatusAttackIDNone;
    fp->stat_flags.is_smash_attack = fp->stat_flags.ga = fp->stat_flags.is_projectile = 0;

    fp->stat_count = fp->damage_stat_count = 0;
    fp->damage_stat_flags = fp->stat_flags;

    fp->afterimage.desc_id = 0;

    DObjGetStruct(fighter_gobj)->translate.vec.f = desc->pos;
    DObjGetStruct(fighter_gobj)->scale.vec.f.x = DObjGetStruct(fighter_gobj)->scale.vec.f.y = DObjGetStruct(fighter_gobj)->scale.vec.f.z = attr->size;

    if (fp->pkind != nFTPlayerKindDemo)
    {
        sb32 is_collide_floor = mpCollisionCheckProjectFloor
        (
            &DObjGetStruct(fighter_gobj)->translate.vec.f,
            &fp->coll_data.floor_line_id,
            &fp->coll_data.floor_dist,
            &fp->coll_data.floor_flags,
            &fp->coll_data.floor_angle
        );

        if (is_collide_floor == FALSE)
        {
            fp->coll_data.floor_line_id = -1;
        }
        if ((is_collide_floor != FALSE) && (fp->coll_data.floor_dist > -300.0F) && (fp->fkind != nFTKindBoss))
        {
            fp->ga = nMPKineticsGround;

            DObjGetStruct(fighter_gobj)->translate.vec.f.y += fp->coll_data.floor_dist;

            fp->coll_data.floor_dist = 0;
        }
        else
        {
            fp->ga = nMPKineticsAir;
            fp->jumps_used = 1;
        }
    }
    else
    {
        fp->ga = nMPKineticsAir;
        fp->jumps_used = 1;
    }
    fp->coll_data.pos_prev = DObjGetStruct(fighter_gobj)->translate.vec.f;

    switch (fp->fkind)
    {
    case nFTKindMMario:
        fp->knockback_resist_passive = 30.0F;

        /* fallthrough */

    case nFTKindMario:
    case nFTKindNMario:
        fp->passive_vars.mario.is_expend_tornado = FALSE;
        break;

    case nFTKindGDonkey:
        fp->knockback_resist_passive = 48.0F;

        /* fallthrough */

    case nFTKindDonkey:
    case nFTKindNDonkey:
        fp->passive_vars.donkey.charge_level = 0;
        break;

    case nFTKindSamus:
    case nFTKindNSamus:
        fp->passive_vars.samus.charge_level = 0;
        fp->passive_vars.samus.charge_recoil = 0;
        break;

    case nFTKindLuigi:
    case nFTKindNLuigi:
        fp->passive_vars.mario.is_expend_tornado = FALSE;
        break;

    case nFTKindCaptain:
    case nFTKindNCaptain:
        fp->passive_vars.captain.falcon_punch_unk = 0;
        break;

    case nFTKindKirby:
    case nFTKindNKirby:
        fp->passive_vars.kirby.copy_id = desc->copy_kind;

        fp->passive_vars.kirby.copysamus_charge_level = 0;
        fp->passive_vars.kirby.copysamus_charge_recoil = 0;
        fp->passive_vars.kirby.copydonkey_charge_level = 0;
        fp->passive_vars.kirby.copycaptain_falcon_punch_unk = 0;
        fp->passive_vars.kirby.copypurin_unk = 0;
        fp->passive_vars.kirby.copylink_boomerang_gobj = NULL;

        if (desc->copy_kind == nFTKindKirby)
        {
            fp->passive_vars.kirby.is_ignore_losecopy = FALSE;
        }
        else fp->passive_vars.kirby.is_ignore_losecopy = TRUE;

        if (fp->fkind == nFTKindKirby)
        {
            FTKirbyCopy *copy = lbRelocGetFileData(FTKirbyCopy*, gFTDataKirbyMainMotion, &llKirbyMainMotionSpecialNFTKirbyCopy);

            ftParamSetModelPartDefaultID(fighter_gobj, FTKIRBY_COPY_MODELPARTS_JOINT, copy[fp->passive_vars.kirby.copy_id].copy_modelpart_id);
        }
        break;

    case nFTKindLink:
    case nFTKindNLink:
        fp->passive_vars.link.boomerang_gobj = NULL;

        ftParamSetModelPartDefaultID(fighter_gobj, 21, -1);
        ftParamSetModelPartDefaultID(fighter_gobj, 19, 0);
        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        fp->passive_vars.purin.unk_0x0 = 0;
        break;

    case nFTKindBoss:
        fp->passive_vars.boss.p = &fp->passive_vars.boss.s;
        fp->passive_vars.boss.p->wait_div = 1.0F;
        fp->passive_vars.boss.p->status_id = -1;
        fp->passive_vars.boss.p->status_id_random = -1;
        fp->passive_vars.boss.p->status_id_guard = 0;

        if (fp->pkind != nFTPlayerKindDemo)
        {
            ftBossCommonSetNextAttackWait(fighter_gobj);
            ftBossCommonSetDefaultLineID(fighter_gobj);
        }
        break;
    }
    ftParamClearAttackCollAll(fighter_gobj);
    ftParamSetHitStatusPartAll(fighter_gobj, nGMHitStatusNormal);
    ftParamResetFighterColAnim(fighter_gobj);
}

// 0x800D7F3C
GObj* ftManagerMakeFighter(FTDesc *desc) // Create fighter
{
    FTStruct *fp;
    GObj *fighter_gobj;
    s32 i;
    FTParts *parts;
    FTAttributes *attr;
    s32 unused;
    DObj *topn_joint;
    FTAccessPart *accesspart;

    fighter_gobj = gcMakeGObjSPAfter(nGCCommonKindFighter, NULL, nGCCommonLinkIDFighter, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(fighter_gobj, desc->proc_display, FTDISPLAY_DLLINK_DEFAULT, GOBJ_PRIORITY_DEFAULT, ~0);

    fp = ftManagerGetNextStructAlloc();

    fighter_gobj->user_data.p = fp;

    fp->pkind = desc->pkind;
    fp->fighter_gobj = fighter_gobj;
    fp->fkind = desc->fkind;
    fp->data = dFTManagerDataFiles[fp->fkind];
    attr = fp->attr = lbRelocGetFileData(FTAttributes*, *fp->data->p_file_main, fp->data->o_attributes);
    fp->figatree_heap = desc->figatree_heap;
    fp->team = desc->team;
    fp->player = desc->player;
    fp->stock_count = desc->stock_count;

    if (fp->pkind != nFTPlayerKindDemo)
    {
        gSCManagerBattleState->players[fp->player].stock_count = desc->stock_count;
    }
    fp->detail_curr = fp->detail_base = desc->detail;

    fp->costume = desc->costume;
    fp->shade = desc->shade;

    fp->shade_color.r = (attr->shade_color[fp->shade - 1].r * attr->shade_color[fp->shade - 1].a) / 0xFF;
    fp->shade_color.g = (attr->shade_color[fp->shade - 1].g * attr->shade_color[fp->shade - 1].a) / 0xFF;
    fp->shade_color.b = (attr->shade_color[fp->shade - 1].b * attr->shade_color[fp->shade - 1].a) / 0xFF;

    fp->handicap = desc->handicap;
    fp->level = desc->level;

    fp->card_anim_frame_id = 0;
    fp->unk_ft_0x3C = 0;
    fp->anim_desc.word = 0;

    fp->p_sfx = NULL;
    fp->sfx_id = 0;
    fp->p_voice = NULL;
    fp->voice_id = 0;
    fp->p_loop_sfx = NULL;
    fp->loop_sfx_id = 0;

    fp->effect_joint_array_id = 0;

    fp->is_invisible = FALSE;
    fp->is_shadow_hide = FALSE;

    fp->display_mode = nDBDisplayModeMaster;

    fp->is_muted = FALSE;
    fp->is_events_forward = FALSE;

    fp->proc_status = NULL;

    fp->unk_ft_0x149 = desc->unk_rebirth_0x1C;
    fp->team_order = desc->team_order;
    fp->dl_link = FTDISPLAY_DLLINK_DEFAULT;

    fp->is_magnify_ignore = desc->is_magnify_ignore;

    fp->status_total_tics = 0;

    fp->camera_zoom_frame = attr->camera_zoom;
    fp->camera_zoom_range = 1.0F;

    fp->is_playertag_bossend = FALSE;
    fp->is_limit_map_bounds = FALSE;

    fp->is_have_translate_scale = (attr->translate_scales != NULL) ? TRUE : FALSE;

    for (i = 0; i < ARRAY_COUNT(fp->joints); i++)
    {
        fp->joints[i] = NULL;
    }
    topn_joint = gcAddDObjForGObj(fighter_gobj, NULL);
    fp->joints[nFTPartsJointTopN] = topn_joint;

    lbCommonInitDObj3Transforms(topn_joint, 0x4B, nGCMatrixKindNull, nGCMatrixKindNull);

    fp->joints[nFTPartsJointTopN]->xobjs[0]->unk05 = desc->unk_rebirth_0x1D;

    lbCommonSetupFighterPartsDObjs
    (
        DObjGetStruct(fighter_gobj),
        attr->commonparts_container,
        fp->detail_curr,
        &fp->joints[nFTPartsJointCommonStart],
        attr->setup_parts,
        0x4B,
        nGCMatrixKindNull,
        nGCMatrixKindNull,
        fp->costume,
        fp->unk_ft_0x149
    );
    for (i = 0; i < ARRAY_COUNT(fp->joints); i++)
    {
        if (fp->joints[i] != NULL)
        {
            fp->joints[i]->user_data.p = ftManagerGetNextPartsAlloc();

            parts = fp->joints[i]->user_data.p;
            parts->flags = attr->commonparts_container->commonparts[fp->detail_curr - nFTPartsDetailStart].flags;
            parts->joint_id = i;

            if (fp->costume != 0)
            {
                if ((attr->accesspart != NULL) && (i == attr->accesspart->joint_id))
                {
                    accesspart = attr->accesspart;

                    parts->gobj = gcMakeGObjSPAfter(nGCCommonKindFighterParts, NULL, nGCCommonLinkIDFighterParts, GOBJ_PRIORITY_DEFAULT);

                    gcAddDObjForGObj(parts->gobj, accesspart->dl);
                    lbCommonAddMObjForFighterPartsDObj(DObjGetStruct(parts->gobj), accesspart->mobjsubs, accesspart->costume_matanim_joints, NULL, fp->costume);
                }
            }
        }
    }
    for (i = nFTPartsJointCommonStart; i < ARRAY_COUNT(fp->joints); i++)
    {
        if (fp->joints[i] != NULL)
        {
            fp->modelpart_status[i - nFTPartsJointCommonStart].modelpart_id_base = 
            fp->modelpart_status[i - nFTPartsJointCommonStart].modelpart_id_curr = (fp->joints[i]->dl != NULL) ? 0 : -1;
        }
    }
    for (i = 0; i < ARRAY_COUNT(fp->texturepart_status); i++)
    {
        fp->texturepart_status[i].texture_id_base = fp->texturepart_status[i].texture_id_curr = 0;
    }
    ftParamSetAnimLocks(fp);

    fp->input.pl.stick_range.x = fp->input.pl.stick_range.y = fp->input.pl.stick_prev.x = fp->input.pl.stick_prev.y = fp->input.cp.stick_range.x = fp->input.cp.stick_range.y = 0;
    fp->input.pl.button_hold = fp->input.pl.button_tap = fp->input.cp.button_inputs = 0;

    fp->input.controller = desc->controller;

    fp->input.button_mask_a = desc->button_mask_a;
    fp->input.button_mask_b = desc->button_mask_b;
    fp->input.button_mask_z = desc->button_mask_z;
    fp->input.button_mask_l = desc->button_mask_l;

    fp->tap_stick_x = fp->tap_stick_y = fp->hold_stick_x = fp->hold_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;

    for (i = 0; i < ARRAY_COUNT(fp->damage_colls); i++)
    {
        if (attr->damage_coll_descs[i].joint_id != -1)
        {
            fp->damage_colls[i].hitstatus = nGMHitStatusNormal;
            fp->damage_colls[i].joint_id = attr->damage_coll_descs[i].joint_id;
            fp->damage_colls[i].joint = fp->joints[fp->damage_colls[i].joint_id];
            fp->damage_colls[i].placement = attr->damage_coll_descs[i].placement;
            fp->damage_colls[i].is_grabbable = attr->damage_coll_descs[i].is_grabbable;
            fp->damage_colls[i].offset = attr->damage_coll_descs[i].offset;
            fp->damage_colls[i].size = attr->damage_coll_descs[i].size;

            fp->damage_colls[i].size.x *= 0.5F;
            fp->damage_colls[i].size.y *= 0.5F;
            fp->damage_colls[i].size.z *= 0.5F;
        }
        else fp->damage_colls[i].hitstatus = nGMHitStatusNone;
    }
    fp->coll_data.p_translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;
    fp->coll_data.p_lr = &fp->lr;
    fp->coll_data.map_coll = attr->map_coll;
    fp->coll_data.p_map_coll = &fp->coll_data.map_coll;
    fp->coll_data.cliffcatch_coll = attr->cliffcatch_coll;
    fp->coll_data.ignore_line_id = -1;
    fp->coll_data.update_tic = gMPCollisionUpdateTic;
    fp->coll_data.mask_curr = 0;

    if (fp->pkind != nFTPlayerKindDemo)
    {
        gcAddGObjProcess(fighter_gobj, ftMainProcUpdateInterrupt, nGCProcessKindFunc, 5);
        gcAddGObjProcess(fighter_gobj, ftMainProcPhysicsMapDefault, nGCProcessKindFunc, 4);
        gcAddGObjProcess(fighter_gobj, ftMainProcPhysicsMapCapture, nGCProcessKindFunc, 3);
        gcAddGObjProcess(fighter_gobj, ftMainProcSearchCatch, nGCProcessKindFunc, 2);
        gcAddGObjProcess(fighter_gobj, ftMainProcSearchHitAll, nGCProcessKindFunc, 1);
        gcAddGObjProcess(fighter_gobj, ftMainProcParams, nGCProcessKindFunc, 0);
    }
    else gcAddGObjProcess(fighter_gobj, scSubsysFighterProcUpdate, nGCProcessKindFunc, 5);

    ftManagerInitFighter(fighter_gobj, desc);

    if (fp->pkind == nFTPlayerKindCom)
    {
        ftComputerSetupAll(fighter_gobj);
    }
    if ((fp->pkind == nFTPlayerKindKey) || (fp->pkind == nFTPlayerKindGameKey))
    {
        fp->key.script = NULL;
        fp->key.input_wait = 0;
    }
    switch (fp->pkind)
    {
    case nFTPlayerKindDemo:
        scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusNull);
        break;

    case nFTPlayerKindKey:
        mpCommonSetFighterWaitOrFall(fighter_gobj);
        break;

    default:
        if (desc->is_skip_entry)
        {
            mpCommonSetFighterWaitOrFall(fighter_gobj);
            ftParamLockPlayerControl(fighter_gobj);
        }
        else
        {
            ftCommonEntrySetStatus(fighter_gobj);
            ftParamLockPlayerControl(fighter_gobj);
        }
        break;
    }
    if ((fp->pkind == nFTPlayerKindMan) || (fp->pkind == nFTPlayerKindCom))
    {
        ftComputerSetFighterDamageDetectSize(fighter_gobj);
    }
    if ((fp->pkind != nFTPlayerKindDemo) && !(desc->is_skip_shadow_setup))
    {
        ftShadowMakeShadow(fighter_gobj);
    }
    return fighter_gobj;
}
