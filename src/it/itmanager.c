#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <lb/library.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80189450 - 0 spawns random, non-0 spawns Pokémon item ID of dITManagerForceMonsterKind + (nITMBallMonsterStart - 1)
s32 dITManagerForceMonsterKind = 0;

// 0x80189454
u16 dITManagerAppearanceRatesMin[/* */] = 
{
    I_SEC_TO_TICS(0), 
    I_SEC_TO_TICS(30),
    I_SEC_TO_TICS(25),
    I_SEC_TO_TICS(20),
    I_SEC_TO_TICS(15),
    I_SEC_TO_TICS(10)
};

// 0x80189460
u16 dITManagerAppearanceRatesMax[/* */] =
{
    I_SEC_TO_TICS(0),
    I_SEC_TO_TICS(30) + 90,
    I_SEC_TO_TICS(25) + 75,
    I_SEC_TO_TICS(20) + 60,
    I_SEC_TO_TICS(15) + 45,
    I_SEC_TO_TICS(10) + 30
};

// 0x8018946C
GObj* (*dITManagerProcMakeList[/* */])(GObj*, Vec3f*, Vec3f*, u32) = 
{
    // Containers
    itBoxMakeItem,          // Box
    itTaruMakeItem,         // Barrel
    itCapsuleMakeItem,      // Capsule
    itEggMakeItem,          // Egg

    // Usable items
    itTomatoMakeItem,       // Maxim Tomato
    itHeartMakeItem,        // Heart Container
    itStarMakeItem,         // Star Man
    itSwordMakeItem,        // Beam Sword
    itBatMakeItem,          // Home Run Bat
    itHarisenMakeItem,      // Fan
    itStarRodMakeItem,      // Star Rod
    itLGunMakeItem,         // Ray Gun
    itFFlowerMakeItem,      // Fire Flower
    itHammerMakeItem,       // Hammer
    itMSBombMakeItem,       // Motion-sensor Bomb
    itBombHeiMakeItem,      // Bob-omb
    itNBumperMakeItem,      // Normal Bumper
    itGShellMakeItem,       // Green Shell
    itRShellMakeItem,       // Red Shell
    itMBallMakeItem,        // Poké Ball

    // Fighter items
    NULL,                   // Ness' PK Fire
    NULL,                   // Link's Bomb

    // Stage items
    itPowerBlockMakeItem,   // Mushroom Kingdom POW Block
    itGBumperMakeItem,      // Common Stage Bumper
    itPakkunMakeItem,       // Mushroom Kingdom Piranha Plant
    itTargetMakeItem,       // Bonus Stage Target
    itTaruBombMakeItem,     // Race to the Finish Bomb
    itGLuckyMakeItem,       // Saffron City Chansey
    itMarumineMakeItem,     // Saffron City Electrode
    itHitokageMakeItem,     // Saffron City Charmander
    itFushigibanaMakeItem,  // Saffron City Venusaur
    itPorygonMakeItem,      // Saffron City Porygon

    // Pokémon items
    itIwarkMakeItem,        // Poké Ball Onix
    itKabigonMakeItem,      // Poké Ball Snorlax
    itTosakintoMakeItem,    // Poké Ball Goldeen
    itNyarsMakeItem,        // Poké Ball Meowth
    itLizardonMakeItem,     // Poké Ball Charizard
    itSpearMakeItem,        // Poké Ball Beedrill
    itKamexMakeItem,        // Poké Ball Blastoise
    itMLuckyMakeItem,       // Poké Ball Chansey
    itStarmieMakeItem,      // Poké Ball Starmie
    itSawamuraMakeItem,     // Poké Ball Hitmonlee
    itDogasMakeItem,        // Poké Ball Koffing
    itPippiMakeItem,        // Poké Ball Clefairy
    itMewMakeItem           // Poké Ball Mew
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018D040
void *gITManagerCommonData;

// 0x8018D044
s32 gITManagerParticleBankID;

// 0x8018D048
ITRandomWeights gITManagerRandomWeights;

// 0x8018D060
ITMonsterData gITManagerMonsterData;

// 0x8018D090
s32 gITManagerDisplayMode;

// 0x8018D094 - points to next available item struct
ITStruct *gITManagerStructsAllocFree;

// 0x8018D098
ITAppearActor gITManagerAppearActor;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016DEA0
void itManagerInitItems(void) // Many linker things here
{
    ITStruct *ip;
    s32 i;

    gITManagerStructsAllocFree = ip = syTaskmanMalloc(sizeof(ITStruct) * ITEM_ALLOC_MAX, 0x8);

    for (i = 0; i < (ITEM_ALLOC_MAX - 1); i++)
    {
        ip[i].next = &ip[i + 1];
    }
    if (ip != NULL)
    {
        ip[i].next = NULL;
    }
    gITManagerCommonData = lbRelocGetExternHeapFile((u32)&llITCommonDataFileID, syTaskmanMalloc(lbRelocGetFileSize((u32)&llITCommonDataFileID), 0x10));

    gITManagerParticleBankID = efParticleGetLoadBankID
    (
        &lITManagerParticleScriptBankLo,
        &lITManagerParticleScriptBankHi,
        &lITManagerParticleTextureBankLo,
        &lITManagerParticleTextureBankHi
    );
    itManagerSetupContainerDrops();
    itManagerInitMonsterVars();
    ifCommonItemArrowSetAttr();

    gITManagerDisplayMode = nDBDisplayModeMaster;
}

// 0x8016DFAC
ITStruct* itManagerGetNextStructAlloc(void) // Set global Item user_data link pointer to next member
{
    ITStruct *new_item = gITManagerStructsAllocFree;
    ITStruct *get_item;

    if (new_item == NULL)
    {
        return NULL;
    }
    get_item = new_item;

    gITManagerStructsAllocFree = new_item->next;

    return get_item;
}

// 0x8016DFDC
void itManagerSetPrevStructAlloc(ITStruct *ip) // Set global Item user_data link pointer to previous member
{
    ip->next = gITManagerStructsAllocFree;

    gITManagerStructsAllocFree = ip;
}

// 0x8016DFF4
void itManagerSetupItemDObjs(GObj *gobj, DObjDesc *dobjdesc, DObj **dobjs, u8 transform_kind)
{
    s32 i, id;
    DObj *dobj, *array_dobjs[DOBJ_ARRAY_MAX];

    for (i = 0; i < ARRAY_COUNT(array_dobjs); i++)
    {
        array_dobjs[i] = NULL;
    }
    for (i = 0; dobjdesc->id != ARRAY_COUNT(array_dobjs); i++, dobjdesc++)
    {
        id = dobjdesc->id & 0xFFF;

        if (id != 0)
        {
            dobj = array_dobjs[id] = gcAddChildForDObj(array_dobjs[id - 1], dobjdesc->dl);
        }
        else dobj = array_dobjs[0] = gcAddDObjForGObj(gobj, dobjdesc->dl);
        
        if (i == 1)
        {
            gcDecideDObj3TransformsKind(dobj, transform_kind, nGCMatrixKindNull, nGCMatrixKindNull, nGCMatrixKindNull);
        }
        else if (transform_kind != nGCMatrixKindNull)
        {
            gcAddXObjForDObjFixed(dobj, transform_kind, nGCMatrixKindNull);
        }
        dobj->translate.vec.f = dobjdesc->translate;
        dobj->rotate.vec.f = dobjdesc->rotate;
        dobj->scale.vec.f = dobjdesc->scale;

        if (dobjs != NULL)
        {
            dobjs[i] = dobj;
        }
    }
}

// 0x8016E174
GObj* itManagerMakeItem(GObj *parent_gobj, ITDesc *item_desc, Vec3f *pos, Vec3f *vel, u32 flags)
{
    ITStruct *ip = itManagerGetNextStructAlloc();
    GObj *item_gobj;
    ITAttributes *attr;
    void (*proc_display)(GObj*);
    s32 unused[4];

    if (ip == NULL)
    {
        return NULL;
    }
    item_gobj = gcMakeGObjSPAfter(nGCCommonKindItem, NULL, nGCCommonLinkIDItem, GOBJ_PRIORITY_DEFAULT);

    if (item_gobj == NULL)
    {
        itManagerSetPrevStructAlloc(ip);

        return NULL;
    }
    attr = lbRelocGetFileData(ITAttributes*, *item_desc->p_file, item_desc->o_attributes);

    if (attr->is_display_colanim)
    {
        proc_display = (attr->is_display_xlu) ? itDisplayColAnimXLUProcDisplay : itDisplayColAnimOPAProcDisplay;
    }
    else proc_display = (attr->is_display_xlu) ? itDisplayXLUProcDisplay : itDisplayOPAProcDisplay;

    gcAddGObjDisplay(item_gobj, proc_display, 11, GOBJ_PRIORITY_DEFAULT, ~0);

    item_gobj->user_data.p = ip;

    ip->item_gobj = item_gobj;
    ip->owner_gobj = NULL;

    ip->kind = item_desc->kind;
    ip->type = attr->type;

    ip->physics.vel_air = *vel;
    ip->physics.vel_ground = 0.0F;

    ip->attr = attr;

    itMainSetSpinVelLR(item_gobj);
    itMainResetPlayerVars(item_gobj);

    ip->is_allow_pickup     = FALSE;
    ip->is_hold             = FALSE;
    ip->is_allow_knockback  = FALSE;
    ip->is_unused_item_bool = FALSE;
    ip->is_static_damage    = FALSE;

    ip->pickup_wait         = ITEM_PICKUP_WAIT_DEFAULT;

    ip->percent_damage      = 0;
    ip->hitlag_tics         = 0;
    ip->damage_highest      = 0;
    ip->damage_knockback    = 0.0F;
    ip->damage_queue        = 0;
    ip->damage_lag          = 0;

    ip->times_landed        = 0;
    ip->times_thrown        = 0;

    ip->weight              = attr->weight;
    ip->is_hitlag_victim    = attr->is_give_hitlag;
    ip->drop_sfx            = attr->drop_sfx;
    ip->throw_sfx           = attr->throw_sfx;
    ip->smash_sfx           = attr->smash_sfx;

    ip->vel_scale           = F_PCT_TO_DEC(attr->vel_scale);

    ip->is_damage_all       = FALSE;
    ip->is_thrown           = FALSE; // Applies magnitude and stale multiplier if TRUE and hitbox is active?
    ip->is_attach_surface   = FALSE;

    ip->spin_step         = 0.0F;

    ip->arrow_gobj          = NULL;
    ip->arrow_timer         = 0;

    ip->attack_coll.attack_state     = item_desc->attack_state;
    ip->attack_coll.damage           = attr->damage;
    ip->attack_coll.throw_mul        = 1.0F;
    ip->attack_coll.stale            = 1.0F;
    ip->attack_coll.element          = attr->element;
    ip->attack_coll.offsets[0].x     = attr->attack_offset0_x;
    ip->attack_coll.offsets[0].y     = attr->attack_offset0_y;
    ip->attack_coll.offsets[0].z     = attr->attack_offset0_z;
    ip->attack_coll.offsets[1].x     = attr->attack_offset1_x;
    ip->attack_coll.offsets[1].y     = attr->attack_offset1_y;
    ip->attack_coll.offsets[1].z     = attr->attack_offset1_z;
    ip->attack_coll.size             = attr->size * 0.5F;
    ip->attack_coll.angle            = attr->angle;
    ip->attack_coll.knockback_scale  = attr->knockback_scale;
    ip->attack_coll.knockback_weight = attr->knockback_weight;
    ip->attack_coll.knockback_base   = attr->knockback_base;
    ip->attack_coll.can_setoff       = attr->can_setoff;
    ip->attack_coll.shield_damage    = attr->shield_damage;
    ip->attack_coll.fgm_id           = attr->hit_sfx;
    ip->attack_coll.priority         = attr->priority;
    ip->attack_coll.can_rehit_item   = attr->can_rehit_item;
    ip->attack_coll.can_rehit_fighter= attr->can_rehit_fighter;
    ip->attack_coll.can_rehit_shield = FALSE;
    ip->attack_coll.can_hop          = attr->can_hop;
    ip->attack_coll.can_reflect      = attr->can_reflect;
    ip->attack_coll.can_shield       = attr->can_shield;
    ip->attack_coll.attack_count     = attr->attack_count;
    ip->attack_coll.interact_mask    = GMHITCOLLISION_FLAG_ALL;

    ip->attack_coll.motion_attack_id           = nFTMotionAttackIDNone;
    ip->attack_coll.motion_count               = ftParamGetMotionCount();
    ip->attack_coll.stat_flags.attack_id       = nFTStatusAttackIDNull;
    ip->attack_coll.stat_flags.is_smash_attack = ip->attack_coll.stat_flags.ga = ip->attack_coll.stat_flags.is_projectile = 0;
    ip->attack_coll.stat_count                 = ftParamGetStatUpdateCount();

    itMainClearAttackRecord(ip);

    ip->damage_coll.hitstatus     = attr->hitstatus;
    ip->damage_coll.offset.x      = attr->damage_coll_offset.x;
    ip->damage_coll.offset.y      = attr->damage_coll_offset.y;
    ip->damage_coll.offset.z      = attr->damage_coll_offset.z;
    ip->damage_coll.size.x        = attr->damage_coll_size.x * 0.5F;
    ip->damage_coll.size.y        = attr->damage_coll_size.y * 0.5F;
    ip->damage_coll.size.z        = attr->damage_coll_size.z * 0.5F;
    ip->damage_coll.interact_mask = GMHITCOLLISION_FLAG_ALL;

    ip->shield_collide_angle = 0.0F;
    ip->shield_collide_dir.x = 0.0F;
    ip->shield_collide_dir.y = 0.0F;
    ip->shield_collide_dir.z = 0.0F;

    ip->hit_normal_damage  = 0;
    ip->hit_refresh_damage = 0;
    ip->hit_attack_damage  = 0;
    ip->hit_shield_damage  = 0;

    ip->reflect_gobj = NULL;

    if (attr->data != NULL)
    {
        if (!(attr->is_item_dobjs))
        {
            gcSetupCustomDObjsWithMObj
            (
                item_gobj,
                attr->data,
                attr->p_mobjsubs,
                NULL,
                item_desc->transform_types.tk1,
                item_desc->transform_types.tk2,
                item_desc->transform_types.tk3
            );
        }
        else
        {
            itManagerSetupItemDObjs(item_gobj, attr->data, NULL, item_desc->transform_types.tk1);

            if (attr->p_mobjsubs != NULL)
            {
                gcAddMObjAll(item_gobj, attr->p_mobjsubs);
            }
        }
        if ((attr->anim_joints != NULL) || (attr->p_matanim_joints != NULL))
        {
            gcAddAnimAll(item_gobj, attr->anim_joints, attr->p_matanim_joints, 0.0F);
            gcPlayAnimAll(item_gobj);
        }
        lbCommonEjectTreeDObj(DObjGetStruct(item_gobj));
    }
    else gcAddDObjForGObj(item_gobj, NULL);
    
    ip->coll_data.p_translate       = &DObjGetStruct(item_gobj)->translate.vec.f;
    ip->coll_data.p_lr              = &ip->lr;
    ip->coll_data.map_coll.top      = attr->map_coll_top;
    ip->coll_data.map_coll.center   = attr->map_coll_center;
    ip->coll_data.map_coll.bottom   = attr->map_coll_bottom;
    ip->coll_data.map_coll.width    = attr->map_coll_width;
    ip->coll_data.p_map_coll        = &ip->coll_data.map_coll;
    ip->coll_data.ignore_line_id    = -1;
    ip->coll_data.update_tic   = gMPCollisionUpdateTic;
    ip->coll_data.mask_curr    = 0;
    ip->coll_data.vel_push.x        = 0.0F;
    ip->coll_data.vel_push.y        = 0.0F;
    ip->coll_data.vel_push.z        = 0.0F;

    gcAddGObjProcess(item_gobj, itProcessProcItemMain, nGCProcessKindFunc, 3);
    gcAddGObjProcess(item_gobj, itProcessProcSearchHitAll, nGCProcessKindFunc, 1);
    gcAddGObjProcess(item_gobj, itProcessProcHitCollisions, nGCProcessKindFunc, 0);

    ip->proc_update    = item_desc->proc_update;
    ip->proc_map       = item_desc->proc_map;
    ip->proc_hit       = item_desc->proc_hit;
    ip->proc_shield    = item_desc->proc_shield;
    ip->proc_hop       = item_desc->proc_hop;
    ip->proc_setoff    = item_desc->proc_setoff;
    ip->proc_reflector = item_desc->proc_reflector;
    ip->proc_damage    = item_desc->proc_damage;
    ip->proc_dead      = NULL;

    ip->coll_data.pos_prev = DObjGetStruct(item_gobj)->translate.vec.f = *pos;

    if (flags & ITEM_FLAG_COLLPROJECT)
    {
        switch (flags & ITEM_MASK_PARENT)
        {
        case ITEM_FLAG_PARENT_GROUND:
        case ITEM_FLAG_PARENT_DEFAULT: // Default?
            break;

        case ITEM_FLAG_PARENT_FIGHTER:
            mpCommonRunItemCollisionDefault(item_gobj, ftGetStruct(parent_gobj)->coll_data.p_translate, &ftGetStruct(parent_gobj)->coll_data);
            break;

        case ITEM_FLAG_PARENT_WEAPON:
            mpCommonRunItemCollisionDefault(item_gobj, wpGetStruct(parent_gobj)->coll_data.p_translate, &wpGetStruct(parent_gobj)->coll_data);
            break;

        case ITEM_FLAG_PARENT_ITEM:
            mpCommonRunItemCollisionDefault(item_gobj, itGetStruct(parent_gobj)->coll_data.p_translate, &itGetStruct(parent_gobj)->coll_data);
            break;

        default:
            break;
        }
    }
    ip->ga = nMPKineticsAir;

    itProcessUpdateAttackPositions(item_gobj);
    itMainClearColAnim(item_gobj);

    return item_gobj;
}

// 0x8016EA78
GObj* itManagerMakeItemSetupCommon(GObj *parent_gobj, s32 index, Vec3f *pos, Vec3f *vel, u32 spawn_flags)
{
    GObj *item_gobj = dITManagerProcMakeList[index](parent_gobj, pos, vel, spawn_flags);

    if (item_gobj != NULL)
    {
        if (index <= nITKindCommonEnd)
        {
            efManagerItemSpawnSwirlMakeEffect(pos);
            itMainSetAppearSpin(item_gobj, FALSE);
        }
    }
    return item_gobj;
}

// 0x8016EB00
ITStruct* itManagerGetCurrentAlloc(void)
{
    return gITManagerStructsAllocFree;
}

// 0x8016EB0C
void itManagerSetItemSpawnWait(void)
{
    gITManagerAppearActor.spawn_wait = 
    dITManagerAppearanceRatesMin[gSCManagerBattleState->item_appearance_rate] + 
    syUtilsRandIntRange
    (
        dITManagerAppearanceRatesMax[gSCManagerBattleState->item_appearance_rate] - dITManagerAppearanceRatesMin[gSCManagerBattleState->item_appearance_rate]
    );
}

// 0x8016EB78
void itManagerAppearActorProcUpdate(GObj *item_gobj)
{
    s32 unused;
    s32 kind;
    Vec3f pos;
    Vec3f vel;

    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        if (gITManagerAppearActor.spawn_wait > 0)
        {
            gITManagerAppearActor.spawn_wait--;

            return;
        }
        if (itManagerGetCurrentAlloc() != NULL)
        {
            kind = itMainGetWeightedItemKind(&gITManagerAppearActor.weights);

            mpCollisionGetMapObjPositionID(gITManagerAppearActor.mapobjs[syUtilsRandIntRange(gITManagerAppearActor.mapobjs_num)], &pos);

            vel.x = vel.y = vel.z = 0.0F;

            func_800269C0_275C0(nSYAudioFGMItemSpawn1);

            itManagerMakeItemSetupCommon(NULL, kind, &pos, &vel, ITEM_FLAG_PARENT_DEFAULT);
        }
        itManagerSetItemSpawnWait();
    }
}

// 0x8016EC40 - create item spawner GObj
GObj* itManagerMakeAppearActor(void)
{
    GObj *gobj;
    s32 i;
    s32 item_any_weights;   // Sum of all toggled item weights of ANY value
    MPItemWeights *p_any_weights;
    s32 weights_sum;
    s32 mapobjs_num;
    s32 item_mapobj_ids[30];
    s32 unused;
    s32 item_valid_weights; // Sum of all toggled NON-ZERO item weights
    u32 item_valid_toggles;
    MPItemWeights *p_valid_weights;
    u32 item_any_toggles;

    if (gSCManagerBattleState->item_appearance_rate != nSCBattleItemSwitchNone)
    {
        if (gSCManagerBattleState->item_toggles != 0)
        {
            if (gMPCollisionGroundData->item_weights != NULL)
            {
                p_any_weights = gMPCollisionGroundData->item_weights;
                item_any_toggles = gSCManagerBattleState->item_toggles;

                item_any_weights = 0;

                for (i = nITKindCommonStart; i <= nITKindCommonEnd; i++, item_any_toggles >>= 1)
                {
                    if (item_any_toggles & 1)
                    {
                        item_any_weights += p_any_weights->values[i];
                    }
                }
                if (item_any_weights == 0)
                {
                    return NULL;
                }
                gITManagerAppearActor.weights.weights_sum = item_any_weights;

                mapobjs_num = mpCollisionGetMapObjCountKind(nMPMapObjKindItem);

                if (mapobjs_num == 0)
                {
                    return NULL;
                }
                if (mapobjs_num > ARRAY_COUNT(item_mapobj_ids))
                {
                    while (TRUE)
                    {
                        syDebugPrintf("Item positions are over %d!\n", ARRAY_COUNT(item_mapobj_ids));
                        scManagerRunPrintGObjStatus();
                    }
                }
                gITManagerAppearActor.mapobjs_num = mapobjs_num;
                gITManagerAppearActor.mapobjs = (u8*) syTaskmanMalloc(mapobjs_num * sizeof(*gITManagerAppearActor.mapobjs), 0);

                mpCollisionGetMapObjIDsKind(nMPMapObjKindItem, item_mapobj_ids);

                for (i = 0; i < mapobjs_num; i++)
                {
                    gITManagerAppearActor.mapobjs[i] = item_mapobj_ids[i];
                }
                gobj = gcMakeGObjSPAfter(nGCCommonKindItem, NULL, nGCCommonLinkIDItemActor, GOBJ_PRIORITY_DEFAULT);

                gcAddGObjProcess(gobj, itManagerAppearActorProcUpdate, nGCProcessKindFunc, 3);

                item_valid_toggles = gSCManagerBattleState->item_toggles;
                p_valid_weights = gMPCollisionGroundData->item_weights;

                for (i = nITKindCommonStart, item_valid_weights = 0; i <= nITKindCommonEnd; i++, item_valid_toggles >>= 1)
                {
                    if ((item_valid_toggles & 1) && (p_valid_weights->values[i] != 0))
                    {
                        item_valid_weights++;
                    }
                }
                gITManagerAppearActor.weights.valids_num = item_valid_weights;
                gITManagerAppearActor.weights.kinds = (u8*) syTaskmanMalloc(item_valid_weights * sizeof(*gITManagerAppearActor.weights.kinds), 0x0);
                gITManagerAppearActor.weights.blocks = (u16*) syTaskmanMalloc(item_valid_weights * sizeof(*gITManagerAppearActor.weights.blocks), 0x2);

                item_valid_toggles = gSCManagerBattleState->item_toggles;
                weights_sum = 0;

                for (i = nITKindCommonStart, item_valid_weights = 0; i <= nITKindCommonEnd; i++, item_valid_toggles >>= 1)
                {
                    if ((item_valid_toggles & 1) && (p_valid_weights->values[i] != 0))
                    {
                        gITManagerAppearActor.weights.kinds[item_valid_weights] = i;
                        gITManagerAppearActor.weights.blocks[item_valid_weights] = weights_sum;
                        weights_sum += p_valid_weights->values[i];

                        item_valid_weights++;
                    }
                }
                itManagerSetItemSpawnWait();

                return gobj;
            }
        }
    }
    return NULL;
}

// 0x8016EF40
void itManagerSetupContainerDrops(void)
{
    s32 item_tenth_round;
    s32 item_tenth_floor;
    s32 item_any_weights;   // Sum of all toggled item weights of ANY value
    u32 item_any_toggles;
    u32 item_valid_toggles;
    s32 item_valid_weights; // Sum of all toggled NON-ZERO item weights
    s32 weights_sum;
    MPItemWeights *p_any_weights;
    MPItemWeights *p_valid_weights;
    s32 i;

    if ((gSCManagerBattleState->item_appearance_rate != nSCBattleItemSwitchNone) && (gSCManagerBattleState->item_toggles != 0) && (gMPCollisionGroundData->item_weights != NULL))
    {
        item_any_toggles = gSCManagerBattleState->item_toggles >> nITKindUtilityStart;
        p_any_weights = gMPCollisionGroundData->item_weights;

        item_any_weights = 0;

        for (i = nITKindUtilityStart; i <= nITKindUtilityEnd; i++, item_any_toggles >>= 1)
        {
            if (item_any_toggles & 1)
            {
                item_any_weights += p_any_weights->values[i];
            }
        }
        gITManagerRandomWeights.weights_sum = item_any_weights;

        if (item_any_weights != 0)
        {
            item_valid_toggles = gSCManagerBattleState->item_toggles >> nITKindUtilityStart;
            p_valid_weights = gMPCollisionGroundData->item_weights;

            for (item_valid_weights = 0, i = nITKindUtilityStart; i <= nITKindUtilityEnd; i++, item_valid_toggles >>= 1)
            {
                if ((item_valid_toggles & 1) && (p_valid_weights->values[i] != 0))
                {
                    item_valid_weights++;
                }
            }
            gITManagerRandomWeights.valids_num = ++item_valid_weights;
            gITManagerRandomWeights.kinds = (u8*) syTaskmanMalloc(item_valid_weights * sizeof(*gITManagerRandomWeights.kinds), 0x0);
            gITManagerRandomWeights.blocks = (u16*) syTaskmanMalloc(item_valid_weights * sizeof(*gITManagerRandomWeights.blocks), 0x2);

            item_valid_toggles = gSCManagerBattleState->item_toggles >> nITKindUtilityStart;
            weights_sum = 0;

            for (item_valid_weights = 0, i = nITKindUtilityStart; i <= nITKindUtilityEnd; i++, item_valid_toggles >>= 1)
            {
                if ((item_valid_toggles & 1) && (p_valid_weights->values[i] != 0))
                {
                    gITManagerRandomWeights.kinds[item_valid_weights] = i;
                    gITManagerRandomWeights.blocks[item_valid_weights] = weights_sum;

                    weights_sum += p_valid_weights->values[i];
                    item_valid_weights++;
                }
            }
            gITManagerRandomWeights.kinds[item_valid_weights] = nITKindMBallMonsterStart;
            gITManagerRandomWeights.blocks[item_valid_weights] = weights_sum;

            item_tenth_round = (gITManagerRandomWeights.weights_sum * 0.1F);

            if (item_tenth_round != 0)
            {
                item_tenth_floor = item_tenth_round;
            }
            else item_tenth_floor = 1;

            gITManagerRandomWeights.weights_sum += item_tenth_floor;
        }
    }
    else gITManagerRandomWeights.weights_sum = 0;
}

// 0x8016F218
void itManagerInitMonsterVars(void)
{
    gITManagerMonsterData.monster_curr = gITManagerMonsterData.monster_prev = U8_MAX;
    gITManagerMonsterData.monsters_num = (nITKindMBallMonsterEnd - nITKindMBallMonsterStart);
}

// 0x8016F238
GObj* itManagerMakeItemKind(GObj *parent_gobj, s32 kind, Vec3f *pos, Vec3f *vel, u32 flags)
{
    return dITManagerProcMakeList[kind](parent_gobj, pos, vel, flags);
}
