#include <gr/ground.h>
#include <ft/fighter.h>
#include <ef/effect.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grHyruleTwisterStatus
{
    nGRHyruleTwisterStatusSleep,
    nGRHyruleTwisterStatusWait,
    nGRHyruleTwisterStatusSummon,
    nGRHyruleTwisterStatusMove,
    nGRHyruleTwisterStatusTurn,
    nGRHyruleTwisterStatusStop,
    nGRHyruleTwisterStatusSubside
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8010A140
LBParticle* grHyruleTwisterMakeEffect(Vec3f *pos, s32 effect_id)
{
    LBParticle *pc = lbParticleMakeScriptID(gGRCommonStruct.hyrule.particle_bank_id | LBPARTICLE_MASK_GENLINK(0), effect_id);

    if (pc != NULL)
    {
        LBTransform *xf = lbParticleAddTransformForStruct(pc, nLBTransformStatusDefault);

        if (xf == NULL)
        {
            lbParticleEjectStruct(pc);

            return NULL;
        }
        LBParticleProcessStruct(pc);

        if (xf->users_num == 0)
        {
            return NULL;
        }
        xf->translate = *pos;
    }
    return pc;
}

// 0x8010A1E4
GObj* grHyruleMakeTwister(Vec3f *pos)
{
    s32 line_id;
    f32 floor_dist;
    GObj *twister_gobj;
    DObj *twister_dobj;
    LBParticle *pc;
    Vec3f edge_pos;
    s32 edge_under;

    if (mpCollisionCheckProjectFloor(pos, &line_id, &floor_dist, NULL, NULL) == FALSE)
    {
        return NULL;
    }
    twister_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    if (twister_gobj != NULL)
    {
        twister_dobj = gcAddDObjForGObj(twister_gobj, NULL);

        twister_dobj->translate.vec.f = *pos;

        twister_dobj->translate.vec.f.y += floor_dist;

        pc = grHyruleTwisterMakeEffect(&twister_dobj->translate.vec.f, 3);

        if (pc == NULL)
        {
            gGRCommonStruct.hyrule.twister_xf = NULL;

            gcEjectGObj(twister_gobj);

            return NULL;
        }
        gGRCommonStruct.hyrule.twister_line_id = line_id;

        mpCollisionGetFloorEdgeL(line_id, &edge_pos);

        edge_under = mpCollisionGetEdgeUnderLLineID(line_id);

        if ((edge_under == -1) || (mpCollisionGetLineTypeID(edge_under) != nMPLineKindRWall))
        {
            gGRCommonStruct.hyrule.twister_leftedge_x = edge_pos.x;
        }
        else gGRCommonStruct.hyrule.twister_leftedge_x = edge_pos.x + 300.0F;
        
        mpCollisionGetFloorEdgeR(line_id, &edge_pos);

        edge_under = mpCollisionGetEdgeUnderRLineID(line_id);

        if ((edge_under == -1) || (mpCollisionGetLineTypeID(edge_under) != nMPLineKindLWall))
        {
            gGRCommonStruct.hyrule.twister_rightedge_x = edge_pos.x;
        }
        else gGRCommonStruct.hyrule.twister_rightedge_x = edge_pos.x - 300.0F;
        
        gGRCommonStruct.hyrule.twister_xf = pc->xf;
    }
    return twister_gobj;
}

// 0x8010A36C
void grHyruleTwisterUpdateSleep(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusWait;
        gGRCommonStruct.hyrule.twister_wait = syUtilsRandIntRange(1200) + 1600;
    }
}

// 0x8010A3B4
void grHyruleTwisterUpdateWait(void)
{
    Vec3f pos;
    GObj *twister_gobj;

    gGRCommonStruct.hyrule.twister_wait--;

    if (gGRCommonStruct.hyrule.twister_wait == 0)
    {
        mpCollisionGetMapObjPositionID(gGRCommonStruct.hyrule.twister_pos_ids[syUtilsRandIntRange(gGRCommonStruct.hyrule.twister_pos_count)], &pos);

        twister_gobj = grHyruleMakeTwister(&pos);

        if (twister_gobj == NULL)
        {
            gGRCommonStruct.hyrule.twister_wait = syUtilsRandIntRange(1200) + 1600;
        }
        else
        {
            gGRCommonStruct.hyrule.twister_wait = 80;
            gGRCommonStruct.hyrule.twister_gobj = twister_gobj;
            gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusSummon;
        }
    }
}

// 0x8010A444
void grHyruleTwisterUpdateSummon(void)
{
    s32 lr;

    gGRCommonStruct.hyrule.twister_wait--;

    if (gGRCommonStruct.hyrule.twister_wait == 0)
    {
        gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusMove;
        gGRCommonStruct.hyrule.twister_wait = syUtilsRandIntRange(600) + 520;

        lr = ((syUtilsRandUShort() % 2) != 0) ? +1 : -1;

        gGRCommonStruct.hyrule.twister_turn_wait = 0;
        gGRCommonStruct.hyrule.twister_vel = lr * 10.0F;
        gGRCommonStruct.hyrule.twister_speed_wait = syUtilsRandIntRange(120) + 180;

        ftMainCheckAddGroundObstacle(gGRCommonStruct.hyrule.twister_gobj, grHyruleTwisterCheckGetDamageKind);

        func_800269C0_275C0(nSYAudioFGMHyruleTwisterAppear);
    }
}

// 0x8010A4F4
sb32 grHyruleTwisterDecLifetimeCheckStop(void)
{
    gGRCommonStruct.hyrule.twister_wait--;

    if (gGRCommonStruct.hyrule.twister_wait == 0)
    {
        gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusStop;

        return TRUE;
    }
    else return FALSE;
}

// 0x8010A52C
s32 grHyruleTwisterGetLR(void)
{
    s32 players_rside = 0;
    s32 players_lside = 0;
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    f32 twister_pos_x = DObjGetStruct(gGRCommonStruct.hyrule.twister_gobj)->translate.vec.f.x;

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if ((fp->ga == nMPKineticsGround) && (fp->coll_data.floor_line_id == gGRCommonStruct.hyrule.twister_line_id))
        {
            if (fp->joints[nFTPartsJointTopN]->translate.vec.f.x > twister_pos_x)
            {
                players_rside++;
            }
            else players_lside++;
        }
        fighter_gobj = fighter_gobj->link_next;
    }
    if ((players_lside != 0) || (players_rside != 0))
    {
        if (players_lside == players_rside)
        {
            return ((syUtilsRandUShort() % 2) != 0) ? -1 : +1;
        }
        else if (players_rside < players_lside)
        {
            return -1;
        }
        else return +1;
    }
    else return 0;
}

// 0x8010A610
void grHyruleTwisterUpdateMove(void)
{
    Vec3f *pos = &DObjGetStruct(gGRCommonStruct.hyrule.twister_gobj)->translate.vec.f;
    f32 ground_level;
    f32 pos_x;
    s32 lr;

    if (grHyruleTwisterDecLifetimeCheckStop() == FALSE)
    {
        if (gGRCommonStruct.hyrule.twister_turn_wait != 0)
        {
            gGRCommonStruct.hyrule.twister_turn_wait--;

            if (gGRCommonStruct.hyrule.twister_turn_wait == 0)
            {
                if (gGRCommonStruct.hyrule.twister_vel < 0.0F)
                {
                    gGRCommonStruct.hyrule.twister_vel = -10.0F;
                }
                else gGRCommonStruct.hyrule.twister_vel = 10.0F;
            }
        }
        else
        {
            gGRCommonStruct.hyrule.twister_speed_wait--;

            if (gGRCommonStruct.hyrule.twister_speed_wait == 0)
            {
                lr = grHyruleTwisterGetLR();

                if ((lr != 0) && (syUtilsRandIntRange(5) == 0))
                {
                    gGRCommonStruct.hyrule.twister_turn_wait = syUtilsRandIntRange(180) + 300;
                    gGRCommonStruct.hyrule.twister_vel = lr * 50.0F;
                }
            }
        }
        pos_x = pos->x + gGRCommonStruct.hyrule.twister_vel;

        if ((gGRCommonStruct.hyrule.twister_rightedge_x < pos_x) || (pos_x < gGRCommonStruct.hyrule.twister_leftedge_x))
        {
            if (gGRCommonStruct.hyrule.twister_rightedge_x < pos_x)
            {
                pos->x = (gGRCommonStruct.hyrule.twister_rightedge_x - 10.0F);
            }
            else pos->x = (gGRCommonStruct.hyrule.twister_leftedge_x + 10.0F);

            gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusTurn;
            gGRCommonStruct.hyrule.twister_turn_wait = 120;
        }
        else pos->x = pos_x;

        mpCollisionGetFCCommonFloor(gGRCommonStruct.hyrule.twister_line_id, pos, &ground_level, NULL, NULL);

        pos->y += ground_level;

        gGRCommonStruct.hyrule.twister_xf->translate = *pos;
    }
}

// 0x8010A7CC
void grHyruleTwisterUpdateTurn(void)
{
    if (grHyruleTwisterDecLifetimeCheckStop() == FALSE)
    {
        gGRCommonStruct.hyrule.twister_turn_wait--;

        if (gGRCommonStruct.hyrule.twister_turn_wait == 0)
        {
            gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusMove;
            gGRCommonStruct.hyrule.twister_turn_wait = 0;
            gGRCommonStruct.hyrule.twister_vel = -gGRCommonStruct.hyrule.twister_vel;
        }
    }
}

// 0x8010A824
void grHyruleTwisterUpdateStop(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (fp->status_id != nFTCommonStatusTwister)
        {
            fighter_gobj = fighter_gobj->link_next;
        }
        else return;
    }
    gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusSubside;
    gGRCommonStruct.hyrule.twister_wait = 32;

    ftMainClearGroundObstacle(gGRCommonStruct.hyrule.twister_gobj);

    if (gGRCommonStruct.hyrule.twister_xf != NULL)
    {
        grHyruleTwisterMakeEffect(&gGRCommonStruct.hyrule.twister_xf->translate, 7);
    }
    gcEjectGObj(gGRCommonStruct.hyrule.twister_gobj);
}

// 0x8010A8B4
void grHyruleTwisterUpdateSubside(void)
{
    gGRCommonStruct.hyrule.twister_wait--;

    if (gGRCommonStruct.hyrule.twister_wait == 0)
    {
        gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusWait;
        gGRCommonStruct.hyrule.twister_wait = syUtilsRandIntRange(1200) + 1600;

        if (gGRCommonStruct.hyrule.twister_xf != NULL)
        {
            lbParticleEjectStructID(gGRCommonStruct.hyrule.twister_xf->generator_id, 1);
        }
    }
}

// 0x8010A91C
void grHyruleTwisterProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.hyrule.twister_status)
    {
    case nGRHyruleTwisterStatusSleep:
        grHyruleTwisterUpdateSleep();
        break;

    case nGRHyruleTwisterStatusWait:
        grHyruleTwisterUpdateWait();
        break;

    case nGRHyruleTwisterStatusSummon:
        grHyruleTwisterUpdateSummon();
        break;

    case nGRHyruleTwisterStatusMove:
        grHyruleTwisterUpdateMove();
        break;

    case nGRHyruleTwisterStatusTurn:
        grHyruleTwisterUpdateTurn();
        break;

    case nGRHyruleTwisterStatusStop:
        grHyruleTwisterUpdateStop();
        break;

    case nGRHyruleTwisterStatusSubside:
        grHyruleTwisterUpdateSubside();
        break;
    }
}

// 0x8010A9C8
void grHyruleTwisterInitVars(void)
{
    s32 i;
    s32 pos_count;
    s32 pos_ids[10];

    gGRCommonStruct.hyrule.twister_pos_count = pos_count = mpCollisionGetMapObjCountKind(nMPMapObjKindTwister);

    if ((pos_count == 0) || (pos_count > ARRAY_COUNT(pos_ids)))
    {
        while (TRUE)
        {
            syDebugPrintf("Twister positions are error!\n");
            scManagerRunPrintGObjStatus();
        }
    }
    gGRCommonStruct.hyrule.twister_pos_ids = (u8*) syTaskmanMalloc(pos_count * sizeof(*gGRCommonStruct.hyrule.twister_pos_ids), 0x0);

    mpCollisionGetMapObjIDsKind(nMPMapObjKindTwister, pos_ids);

    for (i = 0; i < pos_count; i++)
    {
        gGRCommonStruct.hyrule.twister_pos_ids[i] = pos_ids[i];
    }
    gGRCommonStruct.hyrule.twister_status = nGRHyruleTwisterStatusSleep;
    gGRCommonStruct.hyrule.particle_bank_id = efParticleGetLoadBankID
    (
        (uintptr_t)&lGRHyruleParticleScriptBankLo,
        (uintptr_t)&lGRHyruleParticleScriptBankHi,
        (uintptr_t)&lGRHyruleParticleTextureBankLo,
        (uintptr_t)&lGRHyruleParticleTextureBankHi
    );
}

// 0x8010AB20
GObj* grHyruleMakeGround(void)
{
    GObj *ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjProcess(ground_gobj, grHyruleTwisterProcUpdate, nGCProcessKindFunc, 4);
    grHyruleTwisterInitVars();

    return ground_gobj;
}

// 0x8010AB74
sb32 grHyruleTwisterCheckGetDamageKind(GObj *ground_gobj, GObj *fighter_gobj, s32 *kind)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 dist_y;
    f32 dist_x;

    if 
    (
        (fp->twister_wait == 0)                                && 
        (fp->status_id != nFTCommonStatusTwister)              && 
        !(fp->capture_immune_mask & FTCATCHKIND_MASK_TWISTER)  &&
        (ftParamGetBestHitStatusAll(fighter_gobj) == nGMHitStatusNormal)
    )
    {
        DObj *gr_dobj = DObjGetStruct(ground_gobj);
        DObj *ft_dobj = DObjGetStruct(fighter_gobj);

        if (gr_dobj->translate.vec.f.x < ft_dobj->translate.vec.f.x)
        {
            dist_x = -(gr_dobj->translate.vec.f.x - ft_dobj->translate.vec.f.x);
        }
        else dist_x = (gr_dobj->translate.vec.f.x - ft_dobj->translate.vec.f.x);

        dist_y = ft_dobj->translate.vec.f.y - gr_dobj->translate.vec.f.y;

        if ((dist_x < 300.0F) && (dist_y < 600.0F) && (dist_y > -300.0F))
        {
            *kind = nGMHitEnvironmentTwister;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x8010AC70
sb32 grHyruleTwisterCheckGetPosition(Vec3f *pos)
{
    if ((gGRCommonStruct.hyrule.twister_status == nGRHyruleTwisterStatusMove) || (gGRCommonStruct.hyrule.twister_status == nGRHyruleTwisterStatusTurn))
    {
        *pos = DObjGetStruct(gGRCommonStruct.hyrule.twister_gobj)->translate.vec.f;

        return TRUE;
    }
    else return FALSE;
}
