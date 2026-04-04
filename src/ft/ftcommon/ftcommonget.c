#include <ft/fighter.h>
#include <it/item.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80145990
GObj* ftCommonGetFindItem(GObj *fighter_gobj, u8 pickup_mask)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *pickup_gobj = NULL;
    FTItemPickup *item_pickup = &fp->attr->item_pickup;
    GObj *item_gobj = gGCCommonLinks[nGCCommonLinkIDItem];
    f32 closest_item_dist = F32_MAX;
    sb32 is_pickup;
    f32 current_item_dist;
    Vec2f pickup_range;

    while (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if (ip->is_allow_pickup)
        {
            if (fp->coll_data.floor_line_id == ip->coll_data.floor_line_id)
            {
                Vec3f *ft_translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;
                Vec3f *it_translate = &DObjGetStruct(item_gobj)->translate.vec.f;
                MPObjectColl *map_coll = &ip->coll_data.map_coll;

                is_pickup = FALSE;

                if ((ip->weight == nITWeightLight) && (pickup_mask & FTCOMMON_GET_MASK_LIGHT))
                {
                    pickup_range.x = ft_translate->x + (fp->lr * item_pickup->pickup_offset_light.x);
                    pickup_range.y = ft_translate->y + item_pickup->pickup_offset_light.y;

                    if ((((pickup_range.x - item_pickup->pickup_range_light.x) - map_coll->width) < it_translate->x) && (it_translate->x < (item_pickup->pickup_range_light.x + pickup_range.x + map_coll->width)))
                    {
                        if ((((pickup_range.y - item_pickup->pickup_range_light.y) - map_coll->top) < it_translate->y) && (it_translate->y < ((item_pickup->pickup_range_light.y + pickup_range.y) - map_coll->bottom)))
                        {
                            is_pickup = TRUE;
                        }
                    }
                }
                if ((ip->weight == nITWeightHeavy) && (pickup_mask & FTCOMMON_GET_MASK_HEAVY))
                {
                    pickup_range.x = ft_translate->x + (fp->lr * item_pickup->pickup_offset_heavy.x);
                    pickup_range.y = ft_translate->y + item_pickup->pickup_offset_heavy.y;

                    if ((((pickup_range.x - item_pickup->pickup_range_heavy.x) - map_coll->width) < it_translate->x) && (it_translate->x < (item_pickup->pickup_range_heavy.x + pickup_range.x + map_coll->width)))
                    {
                        if ((((pickup_range.y - item_pickup->pickup_range_heavy.y) - map_coll->top) < it_translate->y) && (it_translate->y < ((item_pickup->pickup_range_heavy.y + pickup_range.y) - map_coll->bottom)))
                        {
                            is_pickup = TRUE;
                        }
                    }
                }
                if (is_pickup != FALSE)
                {
                    current_item_dist = (pickup_range.x < it_translate->x) ? -(pickup_range.x - it_translate->x) : (pickup_range.x - it_translate->x);

                    if (current_item_dist < closest_item_dist)
                    {
                        closest_item_dist = current_item_dist;
                        pickup_gobj = item_gobj;
                    }
                }
            }
        }
        item_gobj = item_gobj->link_next;
    }
    return pickup_gobj;
}

// 0x80145BE4
void ftCommonLightGetProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *item_gobj = fp->item_gobj;

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if (ip->type == nITTypeConsume)
        {
            switch (ip->kind)
            {
            case nITKindTomato:
                ftParamSetHealDamage(fp, ITTOMATO_DAMAGE_HEAL);
                itMainDestroyItem(item_gobj);

                if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (fp->player == gSCManagerSceneData.player) && (gSC1PGameBonusTomatoCount < U8_MAX))
                {
                    gSC1PGameBonusTomatoCount++;
                }
                break;

            case nITKindHeart:
                ftParamSetHealDamage(fp, ITHEART_DAMAGE_HEAL);
                itMainDestroyItem(item_gobj);

                if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (fp->player == gSCManagerSceneData.player) && (gSC1PGameBonusHeartCount < U8_MAX))
                {
                    gSC1PGameBonusHeartCount++;
                }
                break;

            case nITKindHammer:
                fp->hammer_tics = ITHAMMER_TIME;

                ftParamTryPlayItemMusic(nSYAudioBGMHammer);
                break;

            default:
                break;
            }
        }
    }
}

// 0x80145D28
void ftCommonHeavyGetProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->item_gobj != NULL)
    {
        ftSetupDropItem(fp);
    }
}

// 0x80145D70
void ftCommonGetProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *item_gobj;

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        item_gobj = ftCommonGetFindItem(fighter_gobj, ((fp->status_id == nFTCommonStatusHeavyGet) ? FTCOMMON_GET_MASK_HEAVY : FTCOMMON_GET_MASK_LIGHT));

        if (item_gobj != NULL)
        {
            itMainSetFighterHold(item_gobj, fighter_gobj);
        }
    }
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        if (fp->status_id == nFTCommonStatusHeavyGet)
        {
            if (fp->item_gobj != NULL)
            {
                if ((fp->fkind == nFTKindDonkey) || (fp->fkind == nFTKindNDonkey) || (fp->fkind == nFTKindGDonkey))
                {
                    ftDonkeyThrowFWaitSetStatus(fighter_gobj);
                }
                else ftCommonLiftWaitSetStatus(fighter_gobj);
            }
            else ftCommonWaitSetStatus(fighter_gobj);
        }
        else
        {
            item_gobj = fp->item_gobj;

            if (item_gobj != NULL)
            {
                ITStruct *ip = itGetStruct(item_gobj);

                if (ip->type == nITTypeConsume)
                {
                    ftCommonLightGetProcDamage(fighter_gobj);

                    if (ip->kind == nITKindHammer)
                    {
                        ftHammerSetStatusHammerWait(fighter_gobj);

                        return;
                    }
                }
            }
            ftCommonWaitSetStatus(fighter_gobj);
        }
    }
}

// 0x80145ED8
void ftCommonLightGetProcMap(GObj *fighter_gobj)
{
    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        ftCommonLightGetProcDamage(fighter_gobj);
        ftCommonFallSetStatus(fighter_gobj);
    }
}

// 0x80145F10
void ftCommonHeavyGetProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        if (fp->item_gobj != NULL)
        {
            ftSetupDropItem(fp);
        }
        ftCommonFallSetStatus(fighter_gobj);
    }
}

// 0x80145F74
void ftCommonHeavyThrowProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnEdge(fighter_gobj) == FALSE)
    {
        if (fp->item_gobj != NULL)
        {
            ftSetupDropItem(fp);
        }
        ftCommonFallSetStatus(fighter_gobj);
    }
}

// 0x80145FD8
void ftCommonGetSetStatus(GObj *fighter_gobj, GObj *item_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    ITStruct *ip = itGetStruct(item_gobj);

    fp->motion_vars.flags.flag1 = 0;

    ftMainSetStatus(fighter_gobj, ((ip->weight == nITWeightHeavy) ? nFTCommonStatusHeavyGet : nFTCommonStatusLightGet), 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    if (fp->status_id == nFTCommonStatusHeavyGet)
    {
        fp->proc_damage = ftCommonHeavyGetProcDamage;
    }
    else fp->proc_damage = ftCommonLightGetProcDamage;
}

// 0x80146064
sb32 ftCommonGetCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->item_gobj == NULL)
    {
        GObj *item_gobj = ftCommonGetFindItem(fighter_gobj, (FTCOMMON_GET_MASK_LIGHT | FTCOMMON_GET_MASK_HEAVY));

        if (item_gobj != NULL)
        {
            ftCommonGetSetStatus(fighter_gobj, item_gobj);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x801460B8
void ftCommonLiftWaitProcInterrupt(GObj *fighter_gobj)
{
    if (ftCommonHeavyThrowCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        ftCommonLiftTurnCheckInterruptLiftWait(fighter_gobj);
    }
}

// 0x801460E8
void ftCommonLiftWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusLiftWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_SLOPECONTOUR);

    fp->proc_damage = ftCommonHeavyGetProcDamage;
}

// 0x80146130
void ftCommonLiftTurnUpdateModelYaw(FTStruct *fp)
{
    fp->status_vars.common.lift.turn_tics--;

    fp->joints[nFTPartsJointTopN]->rotate.vec.f.y += FTCOMMON_LIFT_TURN_STEP;

    ftParamsUpdateFighterPartsTransformAll(fp->joints[nFTPartsJointTopN]);

    if (fp->status_vars.common.lift.turn_tics == (FTCOMMON_LIFT_TURN_FRAMES / 2))
    {
        fp->lr = -fp->lr;
        fp->physics.vel_ground.x = -fp->physics.vel_ground.x;
    }
}

// 0x801461A8
void ftCommonLiftTurnProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonLiftTurnUpdateModelYaw(fp);

    if (fp->status_vars.common.lift.turn_tics == 0)
    {
        ftCommonLiftWaitSetStatus(fighter_gobj);
    }
}

// 0x801461E8
void ftCommonLiftTurnProcInterrupt(GObj *fighter_gobj)
{
    ftCommonHeavyThrowCheckInterruptCommon(fighter_gobj);
}

// 0x80146208
void ftCommonLiftTurnSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusLiftTurn, 0.0F, 1.0F, FTSTATUS_PRESERVE_SLOPECONTOUR);

    fp->proc_damage = ftCommonHeavyGetProcDamage;

    fp->status_vars.common.lift.turn_tics = FTCOMMON_LIFT_TURN_FRAMES;

    ftCommonLiftTurnUpdateModelYaw(fp);
}

// 0x8014625C
sb32 ftCommonLiftTurnCheckInterruptLiftWait(GObj *fighter_gobj)
{
    if (ftCommonTurnCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftCommonLiftTurnSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}