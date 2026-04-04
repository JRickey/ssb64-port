#include <it/item.h>
#include <ft/fighter.h>
#include <gr/ground.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018B220
ITDesc dITSawamuraItemDesc =
{
    nITKindSawamura,                        // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataSawamuraItemAttributes,  // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itSawamuraCommonProcUpdate,             // Proc Update
    itSawamuraCommonProcMap,                // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018B254
ITStatusDesc dITSawamuraStatusDescs[/* */] =
{
    // Status 0 (Air Fall)
    {
        itSawamuraFallProcUpdate,           // Proc Update
        itSawamuraFallProcMap,              // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Fall)
    {
        itSawamuraWaitProcUpdate,           // Proc Update
        itSawamuraWaitProcMap,              // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 2 (Neutral Attack)
    {
        itSawamuraAttackProcUpdate,         // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itSawamuraStatus
{
    nITSawamuraStatusFall,
    nITSawamuraStatusWait,
    nITSawamuraStatusAttack,
    nITSawamuraStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80182630
sb32 itSawamuraFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITSAWAMURA_GRAVITY, ITSAWAMURA_TVEL);

    return FALSE;
}

// 0x80182660
sb32 itSawamuraFallProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;

        itSawamuraWaitSetStatus(item_gobj);
    }
    return FALSE;
}

// 0x801826A8
void itSawamuraFallSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITSawamuraStatusDescs, nITSawamuraStatusFall);
}

// 0x801826D0
sb32 itSawamuraWaitProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        itSawamuraAttackSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x80182714
sb32 itSawamuraWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itSawamuraFallSetStatus);

    return FALSE;
}

// 0x8018273C
void itSawamuraWaitSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITSawamuraStatusDescs, nITSawamuraStatusWait);
}

// 0x80182764
sb32 itSawamuraAttackProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITSAWAMURA_GRAVITY, ITSAWAMURA_TVEL);

    if ((ip->lr == +1) && (dobj->translate.vec.f.x >= (gMPCollisionGroundData->map_bound_right - ITSAWAMURA_DESPAWN_OFF_X)))
    {
        return TRUE;
    }
    else if ((ip->lr == -1) && (dobj->translate.vec.f.x <= (gMPCollisionGroundData->map_bound_left + ITSAWAMURA_DESPAWN_OFF_X)))
    {
        return TRUE;
    }
    else if (ip->multi == 0)
    {
        return TRUE;
    }
    ip->multi--;

    return FALSE;
}

// 0x8018285C
void itSawamuraAttackSetFollowPlayerLR(GObj *item_gobj, GObj *fighter_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *ij = DObjGetStruct(item_gobj);
    DObj *fj = DObjGetStruct(fighter_gobj);
    s32 unused;
    Vec3f dist;
    Vec3f target_pos;

    target_pos = fj->translate.vec.f;

    target_pos.y += ITSAWAMURA_TARGET_POS_OFF_Y - fp->coll_data.map_coll.bottom;

    syVectorDiff3D(&dist, &target_pos, &ij->translate.vec.f);

    ip->physics.vel_air.y = ip->physics.vel_air.z = 0.0F;
    ip->physics.vel_air.x = ITSAWAMURA_KICK_VEL_X;

    syVectorRotate3D(&ip->physics.vel_air, SYVECTOR_AXIS_Z, syUtilsArcTan2(dist.y, dist.x));

    ip->lr = (dist.x < 0.0F) ? -1 : +1;

    if (ip->lr == +1)
    {
        ij->rotate.vec.f.y = F_CST_DTOR32(180.0F);
    }
}

// 0x80182958
void itSawamuraAttackInitVars(GObj *item_gobj)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
#if defined(REGION_US)
    ITStruct *ip = itGetStruct(item_gobj);
    GObj *victim_gobj;
    s32 unused2[3];
    DObj *dobj = DObjGetStruct(item_gobj);
    f32 square_xy;
    f32 dist_x;
    f32 dist_xy;
    Vec3f dist;
#else
    // TODO: regswap
    s32 unused1;
    GObj *victim_gobj;
    s32 unused2[2];
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    FTStruct *owner_fp = ftGetStruct(ip->owner_gobj);
    f32 square_xy;
    f32 dist_x;
    f32 dist_xy;
    Vec3f dist;
#endif
    s32 players = 0;

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

#if defined(REGION_US)
        if ((fighter_gobj != ip->owner_gobj) && (fp->team != ip->team))
#else
        if ((fighter_gobj != ip->owner_gobj) && (fp->team != owner_fp->team))
#endif
        {
            syVectorDiff3D(&dist, &DObjGetStruct(fighter_gobj)->translate.vec.f, &dobj->translate.vec.f);

            if (players == 0)
            {
                dist_xy = SQUARE(dist.x) + SQUARE(dist.y);
            }
            players++;

            square_xy = SQUARE(dist.x) + SQUARE(dist.y);

            if (square_xy <= dist_xy)
            {
                dist_xy = square_xy;

                victim_gobj = fighter_gobj;
            }
        }
        fighter_gobj = fighter_gobj->link_next;

        continue;
    }
    itSawamuraAttackSetFollowPlayerLR(item_gobj, victim_gobj);

    if (ip->kind == nITKindSawamura)
    {
        Gfx *dl = (Gfx*) itGetPData(ip, &llITCommonDataSawamuraDataStart, &llITCommonDataSawamuraDisplayList);

        dobj->dl = dl;

        func_800269C0_275C0(nSYAudioVoiceMBallSawamuraKick);
    }
    ip->multi = ITSAWAMURA_LIFETIME;

    ip->attack_coll.size = ITSAWAMURA_KICK_SIZE;
}

// 0x80182AAC
void itSawamuraAttackSetStatus(GObj *item_gobj)
{
    itSawamuraAttackInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITSawamuraStatusDescs, nITSawamuraStatusAttack);
}

// 0x80182AE0
sb32 itSawamuraCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        ip->multi = ITSAWAMURA_KICK_WAIT;

        ip->physics.vel_air.y = 0.0F;

        itSawamuraFallSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x80182B34
sb32 itSawamuraCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x80182B74
GObj* itSawamuraMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITSawamuraItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        gcAddXObjForDObjFixed(dobj, 0x48, 0);

        dobj->translate.vec.f = *pos;

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataSawamuraDataStart), 0.0F);

        func_800269C0_275C0(nSYAudioVoiceMBallSawamuraAppear);

        gcMoveGObjDLHead(item_gobj, 18, item_gobj->dl_link_priority);
    }
    return item_gobj;
}
