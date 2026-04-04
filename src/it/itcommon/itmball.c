#include <it/item.h>
#include <ft/fighter.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITMBallItemDesc =
{
    nITKindMBall,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataMBallItemAttributes,     // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                  // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itMBallFallProcUpdate,                  // Proc Update
    itMBallFallProcMap,                     // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITMBallStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itMBallWaitProcMap,                 // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itMBallFallProcUpdate,              // Proc Update
        itMBallFallProcMap,                 // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 2 (Fighter Hold)
    {
        NULL,                               // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 3 (Fighter Throw)
    {
        itMBallThrownProcUpdate,            // Proc Update
        itMBallThrownProcMap,               // Proc Map
        itMBallCommonProcHit,               // Proc Hit
        itMBallCommonProcHit,               // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itMBallCommonProcHit,               // Proc Set-Off
        itMBallCommonProcReflector,         // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itMBallFallProcUpdate,              // Proc Update
        itMBallThrownProcMap,               // Proc Map
        itMBallCommonProcHit,               // Proc Hit
        itMBallCommonProcHit,               // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itMBallCommonProcHit,               // Proc Set-Off
        itMBallCommonProcReflector,         // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 5 (Ground Open)
    {
        itMBallOpenProcUpdate,             // Proc Update
        itMBallOpenProcMap,                // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 6 (Air Open)
    {
        itMBallOpenAirProcUpdate,             // Proc Update
        itMBallOpenAirProcMap,                // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        itMBallCommonProcReflector,         // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itMBallStatus
{
    nITMBallStatusWait,
    nITMBallStatusFall,
    nITMBallStatusHold,
    nITMBallStatusThrown,
    nITMBallStatusDropped,
    nITMBallStatusOpen,
    nITMBallStatusOpenAir,
    nITMBallStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017C690
void itMBallOpenAddAnim(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    void *matanim_joint = itGetPData(ip, &llITCommonDataMBallDataStart, &llITCommonDataMBallMatAnimJoint);

    gcAddMObjMatAnimJoint(dobj->child->child->sib_next->mobj, matanim_joint, 0.0F);
    gcPlayAnimAll(item_gobj);
}

// 0x8017C6F8
void itMBallOpenClearAnim(GObj *item_gobj)
{
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->child->sib_next->mobj->matanim_joint.event32 = NULL;
}

// 0x8017C710
sb32 itMBallFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITMBALL_GRAVITY, ITMBALL_TVEL);
    itVisualsUpdateSpin(item_gobj);

    dobj->child->sib_next->rotate.vec.f.z = dobj->rotate.vec.f.z;

    return FALSE;
}

// 0x8017C768
sb32 itMBallWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itMBallFallSetStatus);

    return FALSE;
}

// 0x8017C790
sb32 itMBallFallProcMap(GObj *item_gobj)
{
    itMapCheckDestroyDropped(item_gobj, ITMBALL_MAP_REBOUND_COMMON, ITMBALL_MAP_REBOUND_GROUND, itMBallWaitSetStatus);

    return FALSE;
}

// 0x8017C7C8
void itMBallWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusWait);
}

// 0x8017C7FC
void itMBallFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusFall);
}

// 0x8017C840
void itMBallHoldSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    DObjGetStruct(item_gobj)->rotate.vec.f.y = 0.0F;

    ip->item_vars.mball.owner_gobj = ip->owner_gobj;

    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusHold);
}

// 0x8017C880
sb32 itMBallThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITMBALL_GRAVITY, ITMBALL_TVEL);
    itVisualsUpdateSpin(item_gobj);

    dobj->child->sib_next->rotate.vec.f.z = dobj->rotate.vec.f.z;

    return FALSE;
}

// 0x8017C8D8
sb32 itMBallThrownProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->item_vars.mball.is_rebound != FALSE)
    {
        itMapCheckLanding(item_gobj, ITMBALL_MAP_REBOUND_COMMON, ITMBALL_MAP_REBOUND_GROUND, itMBallOpenSetStatus);
    }
    else itMapCheckDestroyDropped(item_gobj, ITMBALL_MAP_REBOUND_COMMON, ITMBALL_MAP_REBOUND_GROUND, itMBallOpenSetStatus);
    
    return FALSE;
}

// 0x8017C94C
sb32 itMBallCommonProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->item_vars.mball.is_rebound = TRUE;

    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x8017C97C
sb32 itMBallCommonProcReflector(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp;
    GObj *fighter_gobj;

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->item_vars.mball.is_rebound = TRUE;

    itMainVelSetRebound(item_gobj);

    fighter_gobj = ip->item_vars.mball.owner_gobj;
    ip->owner_gobj = fighter_gobj;
    fp = ftGetStruct(fighter_gobj);

    ip->team = fp->team;
    ip->player = fp->player;
    ip->player_num = fp->player_num;
    ip->handicap = fp->handicap;

    return FALSE;
}

// 0x8017C9E0
void itMBallThrownSetStatus(GObj *item_gobj)
{
    itMBallOpenAddAnim(item_gobj);
    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusThrown);
}

// 0x8017CA14
void itMBallDroppedSetStatus(GObj *item_gobj)
{
    itMBallOpenAddAnim(item_gobj);
    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusDropped);
}

// 0x8017CA48
sb32 itMBallOpenProcUpdate(GObj *mball_gobj)
{
    ITStruct *mball_ip = itGetStruct(mball_gobj);
    ITStruct *monster_ip;
    GObj *monster_gobj;
    Vec3f vel;
    s32 unused[2];

    if (mball_ip->multi == 0)
    {
        vel.x = vel.y = vel.z = 0.0F;

        if (dITManagerForceMonsterKind == 0)
        {
            itMainMakeMonster(mball_gobj);

            return TRUE;
        }
        monster_gobj = itManagerMakeItemKind(mball_gobj, dITManagerForceMonsterKind + (nITKindMBallMonsterStart - 1), &DObjGetStruct(mball_gobj)->translate.vec.f, &vel, (ITEM_FLAG_COLLPROJECT | ITEM_FLAG_PARENT_ITEM));

        if (monster_gobj != NULL)
        {
            monster_ip = itGetStruct(monster_gobj);

            monster_ip->owner_gobj = mball_ip->owner_gobj;
            monster_ip->team = mball_ip->team;
            monster_ip->player = mball_ip->player;
            monster_ip->handicap = mball_ip->handicap;
            monster_ip->player_num = mball_ip->player_num;
            monster_ip->display_mode = mball_ip->display_mode;
        }
        return TRUE;
    }
    mball_ip->multi--;

    if (mball_ip->item_vars.mball.effect_gobj != NULL)
    {
        DObjGetStruct(mball_ip->item_vars.mball.effect_gobj)->translate.vec.f = DObjGetStruct(mball_gobj)->translate.vec.f;
    }
    return FALSE;
}

// 0x8017CB38
sb32 itMBallOpenProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (mpCollisionCheckExistLineID(ip->attach_line_id) == FALSE)
    {
        ip->is_attach_surface = FALSE;

        itMBallOpenAirSetStatus(item_gobj);
    }
    return FALSE;
}

// 0x8017CB84
void itMBallOpenInitVars(GObj *item_gobj)
{
    s32 unused[2];
    DObj *dobj = DObjGetStruct(item_gobj);
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *child;
    DObj *sibling;

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    child = dobj->child;
    child->flags ^= DOBJ_FLAG_HIDDEN;

    sibling = dobj->child->sib_next;
    sibling->flags ^= DOBJ_FLAG_HIDDEN;

    func_800269C0_275C0(nSYAudioFGMMBallOpen);

    ip->attach_line_id = ip->coll_data.floor_line_id;

    ip->is_attach_surface = TRUE;

    if ((ip->player != -1) && (ip->player != GMCOMMON_PLAYERS_MAX))
    {
        GObj *fighter_gobj = gSCManagerBattleState->players[ip->player].fighter_gobj;

        if (fighter_gobj != NULL)
        {
            FTStruct *fp = ftGetStruct(fighter_gobj);

            ftParamMakeRumble(fp, 8, 20);
        }
    }
    ip->item_vars.mball.effect_gobj = efManagerMBallRaysMakeEffect(&dobj->translate.vec.f);

    itMBallOpenClearAnim(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;
    ip->attack_coll.can_reflect = FALSE;
}

// 0x8017CC88
void itMBallOpenSetStatus(GObj *item_gobj)
{
    itMBallOpenInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusOpen);
}

// 0x8017CCBC
sb32 itMBallOpenAirProcUpdate(GObj *mball_gobj)
{
    ITStruct *mball_ip = itGetStruct(mball_gobj);
    ITStruct *monster_ip;
    GObj *monster_gobj;
    Vec3f vel;
    s32 unused[2];

    if (mball_ip->multi == 0)
    {
        vel.x = vel.y = vel.z = 0.0F;

        if (dITManagerForceMonsterKind == 0)
        {
            itMainMakeMonster(mball_gobj);

            return TRUE;
        }
        monster_gobj = itManagerMakeItemKind(mball_gobj, dITManagerForceMonsterKind + (nITKindMBallMonsterStart - 1), &DObjGetStruct(mball_gobj)->translate.vec.f, &vel, (ITEM_FLAG_COLLPROJECT | ITEM_FLAG_PARENT_ITEM));

        if (monster_gobj != NULL)
        {
            monster_ip = itGetStruct(monster_gobj);

            monster_ip->owner_gobj = mball_ip->owner_gobj;
            monster_ip->team = mball_ip->team;
            monster_ip->player = mball_ip->player;
            monster_ip->handicap = mball_ip->handicap;
            monster_ip->player_num = mball_ip->player_num;
            monster_ip->display_mode = mball_ip->display_mode;
        }
        return TRUE;
    }
    mball_ip->multi--;

    if (mball_ip->item_vars.mball.effect_gobj != NULL)
    {
        DObjGetStruct(mball_ip->item_vars.mball.effect_gobj)->translate.vec.f = DObjGetStruct(mball_gobj)->translate.vec.f;
    }
    return FALSE;
}

// 0x8017CDAC
sb32 itMBallOpenAirProcMap(GObj *item_gobj)
{
    itMapCheckDestroyDropped(item_gobj, ITMBALL_MAP_REBOUND_COMMON, ITMBALL_MAP_REBOUND_GROUND, itMBallOpenSetStatus);

    return FALSE;
}

// 0x8017CDE4
void itMBallOpenAirSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITMBallStatusDescs, nITMBallStatusOpenAir);
}

// 0x8017CE0C
GObj* itMBallMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITMBallItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip = itGetStruct(item_gobj);
#if defined(REGION_US)
        Vec3f translate = dobj->translate.vec.f;
#endif

        dobj->child->flags = DOBJ_FLAG_HIDDEN;
        dobj->child->sib_next->flags = DOBJ_FLAG_NONE;

        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
        gcAddXObjForDObjFixed(dobj->child->sib_next, 0x46, 0);

#if defined(REGION_US)
        dobj->translate.vec.f = translate;
#else
        dobj->translate.vec.f = *pos;
#endif

        ip->multi = ITMBALL_SPAWN_WAIT;

        ip->item_vars.mball.is_rebound = FALSE;

        ip->is_unused_item_bool = TRUE;

        dobj->rotate.vec.f.z = 0.0F;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}
