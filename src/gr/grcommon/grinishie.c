#include <gr/ground.h>
#include <ft/fighter.h>
#include <it/item.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012EB30
u16 dGRInishieScaleMapObjKinds[/* */] = { nMPMapObjKindScaleL, nMPMapObjKindScaleR };

// 0x8012EB34
u8 dGRInishieScaleLineGroups[/* */] = { 0x01, 0x02 };

// 0x8012EB38
DObjTransformTypes dGRInishieScaleTransformKinds[/* */] =
{
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x01 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x01 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x00 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x01 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x00 }
};

// 0xx8012EB48
u16 dGRInishiePakkunMapObjKinds[/* */] = { nMPMapObjKindPakkunL, nMPMapObjKindPakkunR };

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grInishieScaleStatus
{
    nGRInishieScaleStatusWait,
    nGRInishieScaleStatusFall,
    nGRInishieScaleStatusSleep,
    nGRInishieScaleStatusRetract
};

enum grInishiePowerBlockStatus
{
    nGRInishiePowerBlockStatusWait,
    nGRInishiePowerBlockStatusMake,
    nGRInishiePowerBlockStatusSleep,
    nGRInishiePowerBlockStatusDamage
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80108CD0
void grInishieScaleUpdateFighterStatsGA(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);
        s32 player = fp->player;

        if (fp->ga == nMPKineticsGround)
        {
            if (gGRCommonStruct.inishie.players_ga[player] != nMPKineticsGround)
            {
                gGRCommonStruct.inishie.players_tt[player] = 1;
            }
            else if (gGRCommonStruct.inishie.players_tt[player] != 0)
            {
                gGRCommonStruct.inishie.players_tt[player]--;
            }
        }
        else gGRCommonStruct.inishie.players_tt[player] = 0;

        gGRCommonStruct.inishie.players_ga[player] = fp->ga;

        fighter_gobj = fighter_gobj->link_next;
    }
}

// 0x80108D50
f32 grInishieScaleGetPressure(s32 line_id)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    f32 pressure = 0.0F;

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (fp->ga == nMPKineticsGround)
        {
            if ((fp->coll_data.floor_line_id != -2) && (mpCollisionSetDObjNoID(fp->coll_data.floor_line_id) == line_id))
            {
                f32 weight = (1.0F - fp->attr->weight) + 1.4F;

                if (gGRCommonStruct.inishie.players_tt[fp->player] != 0)
                {
                    pressure += (weight * 8.0F);
                }
                else pressure += weight;
            }
        }
        fighter_gobj = fighter_gobj->link_next;
    }
    return pressure;
}

// 0x801085E0
void grInishieScaleUpdateWait(void)
{
    DObj *l_dobj;
    DObj *r_dobj;
    f32 l_weight;
    f32 r_weight;
    f32 alt;
    sb32 ud;

    grInishieScaleUpdateFighterStatsGA();

    l_weight = grInishieScaleGetPressure(dGRInishieScaleLineGroups[0]);
    r_weight = grInishieScaleGetPressure(dGRInishieScaleLineGroups[1]);

    if ((l_weight == 0.0F) && (r_weight == 0.0F))
    {
        if (gGRCommonStruct.inishie.splat_alt != 0.0F)
        {
            if (gGRCommonStruct.inishie.splat_alt < 0.0F)
            {
                gGRCommonStruct.inishie.splat_alt += 8.0F;

                if (gGRCommonStruct.inishie.splat_alt > 0.0F)
                {
                    gGRCommonStruct.inishie.splat_alt = 0.0F;
                }
            }
            else
            {
                gGRCommonStruct.inishie.splat_alt -= 8.0F;

                if (gGRCommonStruct.inishie.splat_alt < 0.0F)
                {
                    gGRCommonStruct.inishie.splat_alt = 0.0F;
                }
            }
        }
        gGRCommonStruct.inishie.splat_accelerate = 0.0F;
    }
    else
    {
        gGRCommonStruct.inishie.splat_accelerate += (r_weight - l_weight);

        if ((l_weight != 0.0F) && (r_weight != 0.0F) && (gGRCommonStruct.inishie.splat_accelerate != 0.0F))
        {
            gGRCommonStruct.inishie.splat_accelerate *= 0.93F;
        }
        else if (gGRCommonStruct.inishie.splat_accelerate > 0.0F)
        {
            gGRCommonStruct.inishie.splat_accelerate -= 0.9F;

            if (gGRCommonStruct.inishie.splat_accelerate < 0.0F)
            {
                gGRCommonStruct.inishie.splat_accelerate = 0.0F;
            }
        }
        else if (gGRCommonStruct.inishie.splat_accelerate < 0.0F)
        {
            gGRCommonStruct.inishie.splat_accelerate += 0.9F;

            if (gGRCommonStruct.inishie.splat_accelerate > 0.0F)
            {
                gGRCommonStruct.inishie.splat_accelerate = 0.0F;
            }
        }
        gGRCommonStruct.inishie.splat_alt += gGRCommonStruct.inishie.splat_accelerate;
    }
    alt = ABSF(gGRCommonStruct.inishie.splat_alt);

    l_dobj = gGRCommonStruct.inishie.scale[0].platform_dobj;
    r_dobj = gGRCommonStruct.inishie.scale[1].platform_dobj;

    if (alt > 1100.0F)
    {
        ud = 0;

        if (gGRCommonStruct.inishie.splat_alt < 0.0F)
        {
            ud = 1;

            if (gGRCommonStruct.inishie.splat_accelerate != 0.0F)
            {
                // ??? The permuter found this but I really can't match it otherwise
            }
        }
        gGRCommonStruct.inishie.splat_accelerate = 0.0F;

        if (ud != 0)
        {
            gGRCommonStruct.inishie.splat_alt = -1100.0F;
        }
        else gGRCommonStruct.inishie.splat_alt = 1100.0F;

        gGRCommonStruct.inishie.splat_status = nGRInishieScaleStatusFall;

        efManagerSparkleWhiteScaleMakeEffect(&l_dobj->translate.vec.f, 1.0F);
        efManagerSparkleWhiteScaleMakeEffect(&r_dobj->translate.vec.f, 1.0F);
    }
    l_dobj->translate.vec.f.y = gGRCommonStruct.inishie.scale[0].platform_base_y + gGRCommonStruct.inishie.splat_alt;
    r_dobj->translate.vec.f.y = gGRCommonStruct.inishie.scale[1].platform_base_y - gGRCommonStruct.inishie.splat_alt;

    gGRCommonStruct.inishie.scale[0].string_dobj->translate.vec.f.y = l_dobj->translate.vec.f.y - gGRCommonStruct.inishie.scale[0].string_length;
    gGRCommonStruct.inishie.scale[1].string_dobj->translate.vec.f.y = r_dobj->translate.vec.f.y - gGRCommonStruct.inishie.scale[1].string_length;
}

// 0x80109118
void grInishieScaleUpdateFall(void)
{
    f32 deadzone;

    gGRCommonStruct.inishie.splat_accelerate += 3.0F;

    if (gGRCommonStruct.inishie.splat_accelerate > 70.0F)
    {
        gGRCommonStruct.inishie.splat_accelerate = 70.0F;
    }
    gGRCommonStruct.inishie.scale[0].platform_dobj->translate.vec.f.y -= gGRCommonStruct.inishie.splat_accelerate;
    gGRCommonStruct.inishie.scale[1].platform_dobj->translate.vec.f.y -= gGRCommonStruct.inishie.splat_accelerate;

    deadzone = gMPCollisionGroundData->map_bound_bottom + (-1000.0F);

    if ((gGRCommonStruct.inishie.scale[0].platform_dobj->translate.vec.f.y < deadzone) && (gGRCommonStruct.inishie.scale[1].platform_dobj->translate.vec.f.y < deadzone))
    {
        gGRCommonStruct.inishie.splat_status = nGRInishieScaleStatusSleep;
        gGRCommonStruct.inishie.splat_accelerate = 0.0F;

        mpCollisionSetYakumonoOffID(dGRInishieScaleLineGroups[0]);
        mpCollisionSetYakumonoOffID(dGRInishieScaleLineGroups[1]);

        gGRCommonStruct.inishie.splat_wait = 180;
    }
}

// 0x80109220
void grInishieScaleUpdateStep(void)
{
    gGRCommonStruct.inishie.splat_wait--;

    if (gGRCommonStruct.inishie.splat_wait == 0)
    {
        gGRCommonStruct.inishie.splat_status = nGRInishieScaleStatusRetract;

        gcAddDObjAnimJoint(gGRCommonStruct.inishie.scale[0].platform_dobj, (AObjEvent32*) ((intptr_t)&llGRInishieMapScaleRetractAnimJoint + (uintptr_t)gGRCommonStruct.inishie.map_head), 0.0F);
        gcAddDObjAnimJoint(gGRCommonStruct.inishie.scale[1].platform_dobj, (AObjEvent32*) ((intptr_t)&llGRInishieMapScaleRetractAnimJoint + (uintptr_t)gGRCommonStruct.inishie.map_head), 0.0F);
    }
}

// 0x8010929C
void grInishieScaleUpdateRetract(void)
{
    DObj *l_dobj;
    DObj *r_dobj;
    sb32 is_complete = FALSE;

    if (gGRCommonStruct.inishie.splat_alt != 0.0F)
    {
        if (gGRCommonStruct.inishie.splat_alt < 0.0F)
        {
            gGRCommonStruct.inishie.splat_alt += 10.0F;

            if (gGRCommonStruct.inishie.splat_alt >= 0.0F)
            {
                is_complete = TRUE;
            }
        }
        else
        {
            gGRCommonStruct.inishie.splat_alt -= 10.0F;

            if (gGRCommonStruct.inishie.splat_alt <= 0.0F)
            {
                is_complete = TRUE;
            }
        }
    }
    l_dobj = gGRCommonStruct.inishie.scale[0].platform_dobj;
    r_dobj = gGRCommonStruct.inishie.scale[1].platform_dobj;

    if (is_complete != FALSE)
    {
        gGRCommonStruct.inishie.splat_alt = 0.0F;

        l_dobj->anim_wait = AOBJ_ANIM_NULL;
        l_dobj->flags = DOBJ_FLAG_NONE;

        mpCollisionSetYakumonoOnID(dGRInishieScaleLineGroups[0]);

        r_dobj->anim_wait = AOBJ_ANIM_NULL;
        r_dobj->flags = DOBJ_FLAG_NONE;

        mpCollisionSetYakumonoOnID(dGRInishieScaleLineGroups[1]);

        gGRCommonStruct.inishie.splat_status = nGRInishieScaleStatusWait;
    }
    l_dobj->translate.vec.f.y = gGRCommonStruct.inishie.scale[0].platform_base_y + gGRCommonStruct.inishie.splat_alt;
    r_dobj->translate.vec.f.y = gGRCommonStruct.inishie.scale[1].platform_base_y - gGRCommonStruct.inishie.splat_alt;

    gGRCommonStruct.inishie.scale[0].string_dobj->translate.vec.f.y = l_dobj->translate.vec.f.y - gGRCommonStruct.inishie.scale[0].string_length;
    gGRCommonStruct.inishie.scale[1].string_dobj->translate.vec.f.y = r_dobj->translate.vec.f.y - gGRCommonStruct.inishie.scale[1].string_length;
}

// 0x801093EC
void grInishieScaleProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.inishie.splat_status)
    {
    case nGRInishieScaleStatusWait:
        grInishieScaleUpdateWait();
        break;

    case nGRInishieScaleStatusFall:
        grInishieScaleUpdateFall();
        break;

    case nGRInishieScaleStatusSleep:
        grInishieScaleUpdateStep();
        break;

    case nGRInishieScaleStatusRetract:
        grInishieScaleUpdateRetract();
        break;
    }
    mpCollisionSetYakumonoPosID(dGRInishieScaleLineGroups[0], &gGRCommonStruct.inishie.scale[0].platform_dobj->translate.vec.f);
    mpCollisionSetYakumonoPosID(dGRInishieScaleLineGroups[1], &gGRCommonStruct.inishie.scale[1].platform_dobj->translate.vec.f);
}

// 0x801094A0
void grInishieMakeScale(void)
{
    void *map_head;
    GObj *ground_gobj;
    DObj *map_dobjs[5];
    DObj *platform_dobj;
    s32 i;
    s32 mapobj;
    Vec3f yakumono_pos;

    map_head = gGRCommonStruct.inishie.map_head;
    ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(ground_gobj, gcDrawDObjTreeForGObj, 6, GOBJ_PRIORITY_DEFAULT, ~0);
    grModelSetupGroundDObjs(ground_gobj, (DObjDesc*) ((uintptr_t)map_head + (intptr_t)&llGRInishieMapScaleDObjDesc), map_dobjs, dGRInishieScaleTransformKinds);

    gGRCommonStruct.inishie.scale[0].string_dobj = map_dobjs[4];
    gGRCommonStruct.inishie.scale[0].string_length = map_dobjs[0]->translate.vec.f.y + map_dobjs[3]->translate.vec.f.y;

    gGRCommonStruct.inishie.scale[1].string_dobj = map_dobjs[2];
    gGRCommonStruct.inishie.scale[1].string_length = map_dobjs[0]->translate.vec.f.y + map_dobjs[1]->translate.vec.f.y;

    for (i = 0; i < ARRAY_COUNT(gGRCommonStruct.inishie.scale); i++)
    {
        ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);
        gcAddGObjDisplay(ground_gobj, gcDrawDObjDLHead0, 6, GOBJ_PRIORITY_DEFAULT, ~0);

        platform_dobj = gcAddDObjForGObj(ground_gobj, (void*) ((uintptr_t)map_head + (intptr_t)&llGRInishieMapMapHead));
        gGRCommonStruct.inishie.scale[i].platform_dobj = platform_dobj;

        gcAddXObjForDObjFixed(platform_dobj, nGCMatrixKindTra, 0);
        gcAddGObjProcess(ground_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);

        mpCollisionGetMapObjIDsKind(dGRInishieScaleMapObjKinds[i], &mapobj);
        mpCollisionGetMapObjPositionID(mapobj, &yakumono_pos);

        platform_dobj->translate.vec.f = yakumono_pos;

        gGRCommonStruct.inishie.scale[i].platform_base_y = yakumono_pos.y;

        mpCollisionSetYakumonoOnID(dGRInishieScaleLineGroups[i]);
    }
    gcAddGObjProcess(ground_gobj, grInishieScaleProcUpdate, nGCProcessKindFunc, 4);

    // WARNING: gGRCommonStruct MUST be defined as extern in ground.h in order for this to match
    gGRCommonStruct.inishie.splat_status = nGRInishieScaleStatusWait;
    gGRCommonStruct.inishie.splat_alt = 0.0F;
    gGRCommonStruct.inishie.splat_accelerate = 0.0F;

    for (i = 0; i < (ARRAY_COUNT(gGRCommonStruct.inishie.players_ga) + ARRAY_COUNT(gGRCommonStruct.inishie.players_tt)) / 2; i++)
    {
        gGRCommonStruct.inishie.players_tt[i] = 0;
        gGRCommonStruct.inishie.players_ga[i] = 0;
    }
}

// 0x8010972C
void grInishiePakkunSetWaitFighter(void)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gGRCommonStruct.inishie.pakkun_gobj); i++)
    {
        itPakkunCommonSetWaitFighter(gGRCommonStruct.inishie.pakkun_gobj[i]);
    }
}

// 0x80109774
void grInishieMakePakkun(void)
{
    s32 i;
    Vec3f pos;
    Vec3f vel;
    s32 mapobj;

    for (i = 0; i < ARRAY_COUNT(gGRCommonStruct.inishie.pakkun_gobj); i++)
    {
        mpCollisionGetMapObjIDsKind(dGRInishiePakkunMapObjKinds[i], &mapobj);
        mpCollisionGetMapObjPositionID(mapobj, &pos);

        vel.x = vel.y = vel.z = 0.0F;

        gGRCommonStruct.inishie.pakkun_gobj[i] = itManagerMakeItemSetupCommon(NULL, nITKindPakkun, &pos, &vel, ITEM_FLAG_PARENT_GROUND);
    }
}

// 0x80109838
void grInishiePowerBlockUpdateWait(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        gGRCommonStruct.inishie.pblock_status = nGRInishiePowerBlockStatusMake;
        gGRCommonStruct.inishie.pblock_appear_wait = 1800;
    }
}

// 0x8010986C
void grInishiePowerBlockSetWait(void)
{
    gGRCommonStruct.inishie.pblock_status = nGRInishiePowerBlockStatusMake;
    gGRCommonStruct.inishie.pblock_appear_wait = 1800;
}

// 0x80109888
void grInishiePowerBlockUpdateMake(void)
{
    GObj *pblock_gobj;
    Vec3f pos;
    Vec3f vel;

    gGRCommonStruct.inishie.pblock_appear_wait--;

    if (gGRCommonStruct.inishie.pblock_appear_wait == 0)
    {
        s32 pblock_pos_id = gGRCommonStruct.inishie.pblock_pos_ids[syUtilsRandIntRange(gGRCommonStruct.inishie.pblock_pos_count)];

        mpCollisionGetMapObjPositionID(pblock_pos_id, &pos);

        vel.x = vel.y = vel.z = 0.0F;

        pblock_gobj = itManagerMakeItemSetupCommon(NULL, nITKindPowerBlock, &pos, &vel, ITEM_FLAG_PARENT_GROUND);

        if (pblock_gobj != NULL)
        {
            gGRCommonStruct.inishie.pblock_gobj = pblock_gobj;
            gGRCommonStruct.inishie.pblock_status = nGRInishiePowerBlockStatusSleep;
        }
        else grInishiePowerBlockSetWait();
    }
}

// 0x8010992C
void grInishiePowerBlockUpdateDamage(void)
{
    gGRCommonStruct.inishie.pblock_appear_wait--;

    if (gGRCommonStruct.inishie.pblock_appear_wait == 0)
    {
        ftMainClearHazard(gGRCommonStruct.inishie.pblock_gobj);
    }
}

// 0x80109968
void grInishiePowerBlockProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.inishie.pblock_status)
    {
    case nGRInishiePowerBlockStatusWait:
        grInishiePowerBlockUpdateWait();
        break;

    case nGRInishiePowerBlockStatusMake:
        grInishiePowerBlockUpdateMake();
        break;

    case nGRInishiePowerBlockStatusDamage:
        grInishiePowerBlockUpdateDamage();
        break;
    }
}

// 0x801099D4
void grInishieMakePowerBlock(void)
{
    s32 pos_count, i, pos_ids[10];

    gcAddGObjProcess(gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT), grInishiePowerBlockProcUpdate, nGCProcessKindFunc, 4);

    gGRCommonStruct.inishie.pblock_pos_count = pos_count = mpCollisionGetMapObjCountKind(nMPMapObjKindPowerBlock);

    if ((pos_count == 0) || (pos_count > ARRAY_COUNT(pos_ids)))
    {
        while (TRUE)
        {
            syDebugPrintf("PowerBlock positions are error!\n");
            scManagerRunPrintGObjStatus();
        }
    }
    gGRCommonStruct.inishie.pblock_pos_ids = (u8*) syTaskmanMalloc(pos_count * sizeof(*gGRCommonStruct.inishie.pblock_pos_ids), 0x0);

    mpCollisionGetMapObjIDsKind(nMPMapObjKindPowerBlock, pos_ids);

    for (i = 0; i < pos_count; i++)
    {
        gGRCommonStruct.inishie.pblock_pos_ids[i] = pos_ids[i];
    }
    gGRCommonStruct.inishie.pblock_status = nGRInishiePowerBlockStatusWait;
    gGRCommonStruct.inishie.attack_coll = (GRAttackColl*) (((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRInishieMapMapHeader) + (intptr_t)&llGRInishieMapPowerBlockGRAttackColl);
}

// 0x80109B4C
void grInishiePowerBlockSetDamage(void)
{
    ftMainCheckAddGroundHazard(gGRCommonStruct.inishie.pblock_gobj, grInishiePowerBlockCheckGetDamageKind);

    gGRCommonStruct.inishie.pblock_appear_wait = 2;
    gGRCommonStruct.inishie.pblock_status = nGRInishiePowerBlockStatusDamage;
}

// 0x80109B8C
sb32 grInishiePowerBlockCheckGetDamageKind(GObj *item_gobj, GObj *fighter_gobj, GRAttackColl **gr_attack_coll, s32 *kind)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if (fighter_gobj != ip->damage_gobj)
        {
            *gr_attack_coll = gGRCommonStruct.inishie.attack_coll;
            *kind = nGMHitEnvironmentPowerBlock;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80109BD4
void grInishieInitHeaders(void)
{
    gGRCommonStruct.inishie.map_head = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRInishieMapMapHead);
    gGRCommonStruct.inishie.item_head = (void*) ((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRInishieMapMapHeader);
}

// 0x80109C0C
GObj* grInishieMakeGround(void)
{
    grInishieInitHeaders();
    grInishieMakeScale();
    grInishieMakePakkun();
    grInishieMakePowerBlock();

    return NULL;
}

// 0x80109C48
void grInishieScaleGetPlatformInfo(f32 *alt, f32 *accel) // Unused?
{
    *alt = 1100.0F - ((gGRCommonStruct.inishie.splat_alt < 0.0F) ? -gGRCommonStruct.inishie.splat_alt : gGRCommonStruct.inishie.splat_alt);

    *accel = (gGRCommonStruct.inishie.splat_accelerate < 0.0F) ? -gGRCommonStruct.inishie.splat_accelerate : gGRCommonStruct.inishie.splat_accelerate;
}
