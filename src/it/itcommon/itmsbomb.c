#include <it/item.h>
#include <ft/fighter.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITMSBombItemDesc =
{
    nITKindMSBomb,                          // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataMSBombItemAttributes,    // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                  // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itMSBombFallProcUpdate,                 // Proc Update
    itMSBombFallProcMap,                    // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITMSBombStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itMSBombWaitProcMap,                // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itMSBombFallProcUpdate,             // Proc Update
        itMSBombFallProcMap,                // Proc Map
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
        itMSBombThrownProcUpdate,           // Proc Update
        itMSBombThrownProcMap,              // Proc Map
        itMSBombCommonProcHit,              // Proc Hit
        itMSBombCommonProcHit,              // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itMSBombCommonProcHit,              // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itMSBombFallProcUpdate,             // Proc Update
        itMSBombDroppedProcMap,             // Proc Map
        itMSBombCommonProcHit,              // Proc Hit
        itMSBombCommonProcHit,              // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itMSBombCommonProcHit,              // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 5 (Ground Attach)
    {
        itMSBombAttachedProcUpdate,         // Proc Update
        itMSBombAttachedProcMap,            // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itMSBombCommonProcDamage            // Proc Damage
    },

    // Status 6 (Air Detach from Surface)
    {
        itMSBombDetachedProcUpdate,         // Proc Update
        itMSBombDroppedProcMap,             // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itMSBombCommonProcDamage            // Proc Damage
    },

    // Status 7 (Neutral Explosion)
    {
        itMSBombExplodeProcUpdate,          // Proc Update
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


enum itMSBombStatus
{
    nITMSBombStatusWait,
    nITMSBombStatusFall,
    nITMSBombStatusHold,
    nITMSBombStatusThrown,
    nITMSBombStatusDropped,
    nITMSBombStatusAttached,
    nITMSBombStatusDetached,
    nITMSBombStatusExplode,
    nITMSBombStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80176450
sb32 itMSBombFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITMSBOMB_GRAVITY, ITMSBOMB_TVEL);
    itVisualsUpdateSpin(item_gobj);

    dobj->child->sib_next->rotate.vec.f.z = dobj->rotate.vec.f.z;

    return FALSE;
}

// 0x801764A8
sb32 itMSBombWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itMSBombFallSetStatus);

    return FALSE;
}

// 0x801764D0
sb32 itMSBombFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITMSBOMB_MAP_REBOUND_COMMON, ITMSBOMB_MAP_REBOUND_GROUND, itMSBombWaitSetStatus);
}

// 0x80176504
void itMSBombWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusWait);
}

// 0x80176538
void itMSBombFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusFall);
}

// 0x8017657C
void itMSBombHoldSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusHold);
}

// 0x801765A4
sb32 itMSBombThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITMSBOMB_GRAVITY, ITMSBOMB_TVEL);
    itVisualsUpdateSpin(item_gobj);

    dobj->child->sib_next->rotate.vec.f.z = dobj->rotate.vec.f.z;

    return FALSE;
}

// 0x801765FC
sb32 itMSBombThrownProcMap(GObj *item_gobj)
{
    return itMapCheckMapProcAll(item_gobj, itMSBombAttachedSetStatus);
}

// 0x80176620
sb32 itMSBombCommonProcHit(GObj *item_gobj)
{
    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x80176644
void itMSBombThrownSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->coll_data.map_coll.top = ITMSBOMB_COLL_SIZE;
    ip->coll_data.map_coll.center = 0.0F;
    ip->coll_data.map_coll.bottom = -ITMSBOMB_COLL_SIZE;
    ip->coll_data.map_coll.width = ITMSBOMB_COLL_SIZE;

    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusThrown);
}

// 0x80176694
sb32 itMSBombDroppedProcMap(GObj *item_gobj)
{
    return itMapCheckMapProcAll(item_gobj, itMSBombAttachedSetStatus);
}

// 0x801766B8
void itMSBombDroppedSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->coll_data.map_coll.top = ITMSBOMB_COLL_SIZE;
    ip->coll_data.map_coll.center = 0.0F;
    ip->coll_data.map_coll.bottom = -ITMSBOMB_COLL_SIZE;
    ip->coll_data.map_coll.width = ITMSBOMB_COLL_SIZE;

    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusDropped);
}

// 0x80176708
void itMSBombAttachedUpdateSurface(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    MPCollData *coll_data = &ip->coll_data;
    Vec3f angle;
    DObj *dobj = DObjGetStruct(item_gobj);

    if ((coll_data->mask_curr & MAP_FLAG_CEIL) || (coll_data->mask_curr & MAP_FLAG_FLOOR))
    {
        if (coll_data->mask_curr & MAP_FLAG_CEIL)
        {
            angle = coll_data->ceil_angle;

            ip->attach_line_id = coll_data->ceil_line_id;
        }
        if (coll_data->mask_curr & MAP_FLAG_FLOOR)
        {
            angle = coll_data->floor_angle;

            ip->attach_line_id = coll_data->floor_line_id;
        }
    }
    else
    {
        if (coll_data->mask_curr & MAP_FLAG_LWALL)
        {
            angle = coll_data->lwall_angle;

            ip->attach_line_id = coll_data->lwall_line_id;
        }
        if (coll_data->mask_curr & MAP_FLAG_RWALL)
        {
            angle = coll_data->rwall_angle;

            ip->attach_line_id = coll_data->rwall_line_id;
        }
    }
    dobj->rotate.vec.f.z = syUtilsArcTan2(angle.y, angle.x) - F_CST_DTOR32(90.0F);
}

// 0x80176840
void itMSBombAttachedInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    ip->coll_data.map_coll.top = ITMSBOMB_COLL_SIZE;
    ip->coll_data.map_coll.center = 0.0F;
    ip->coll_data.map_coll.bottom = -ITMSBOMB_COLL_SIZE;
    ip->coll_data.map_coll.width = ITMSBOMB_COLL_SIZE;

    ip->physics.vel_air.x = ip->physics.vel_air.y = ip->physics.vel_air.z = 0;

    dobj->child->flags = DOBJ_FLAG_NONE;
    dobj->child->sib_next->flags = DOBJ_FLAG_HIDDEN;

    itMSBombAttachedUpdateSurface(item_gobj);

    ip->is_attach_surface = TRUE;

    ip->damage_coll.hitstatus = nGMHitStatusNormal;

    ip->attack_coll.attack_state = nGMAttackStateOff;

    if ((ip->player != -1) && (ip->player != GMCOMMON_PLAYERS_MAX))
    {
        GObj *fighter_gobj = gSCManagerBattleState->players[ip->player].fighter_gobj;

        if (fighter_gobj != NULL)
        {
            ftParamMakeRumble(ftGetStruct(fighter_gobj), 6, 0);
        }
    }
    func_800269C0_275C0(nSYAudioFGMMSBombAttach);

    itMainClearOwnerStats(item_gobj);
}

// 0x80176934
void itMSBombExplodeMakeEffect(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    ITAttributes *attr = ip->attr;
    DObj *dobj = DObjGetStruct(item_gobj);
    s32 unused[4];

    if (ip->coll_data.mask_curr & MAP_FLAG_FLOOR)
    {
        Vec3f translate = dobj->translate.vec.f;

        translate.y += attr->map_coll_bottom;

        efManagerDustHeavyDoubleMakeEffect(&translate, ip->lr, 1.0F);
    }
}

// 0x801769AC
void itMSBombExplodeInitStatusVars(GObj *item_gobj, sb32 is_make_effect)
{
    LBParticle *pc;
    DObj *dobj = DObjGetStruct(item_gobj);

    if (is_make_effect != FALSE)
    {
        itMSBombExplodeMakeEffect(item_gobj);
    }
    pc = efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);

    if (pc != NULL)
    {
        pc->xf->scale.x = ITMSBOMB_EXPLODE_SCALE;
        pc->xf->scale.y = ITMSBOMB_EXPLODE_SCALE;
        pc->xf->scale.z = ITMSBOMB_EXPLODE_SCALE;
    }
    efManagerQuakeMakeEffect(1);
    itMainRefreshAttackColl(item_gobj);
    itMSBombExplodeSetStatus(item_gobj);

    DObjGetStruct(item_gobj)->flags = DOBJ_FLAG_HIDDEN;
}

// 0x80176A34
sb32 itMSBombCommonProcDamage(GObj *item_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeL);
    itMSBombExplodeInitStatusVars(item_gobj, FALSE);

    return FALSE;
}

// 0x80176A68
sb32 itMSBombAttachedProcUpdate(GObj *item_gobj)
{
    s32 unused[2];
    GObj *fighter_gobj;
    Vec3f *translate;
    Vec3f dist;
    Vec3f fighter_pos;
    DObj *item_dobj = DObjGetStruct(item_gobj);
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi < ITMSBOMB_DETECT_FIGHTER_DELAY)
    {
        ip->multi++;
    }
    else
    {
        fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

        translate = &item_dobj->translate.vec.f;

        while (fighter_gobj != NULL)
        {
            FTStruct *fp = ftGetStruct(fighter_gobj);
            DObj *fighter_dobj = DObjGetStruct(fighter_gobj);
            f32 var = fp->attr->map_coll.top * 0.5F;

            fighter_pos = fighter_dobj->translate.vec.f;

            fighter_pos.y += var;

            syVectorDiff3D(&dist, &fighter_pos, translate);

            if ((SQUARE(dist.x) + SQUARE(dist.y) + SQUARE(dist.z)) < ITMSBOMB_DETECT_FIGHTER_RADIUS)
            {
                itMSBombExplodeInitStatusVars(item_gobj, TRUE); // We might want to break out of the loop here
            }
            fighter_gobj = fighter_gobj->link_next;
        }
    }
    return FALSE;
}

// 0x80176B94
void itMSBombAttachedSetStatus(GObj *item_gobj)
{
    itMSBombAttachedInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusAttached);
}

// 0x80176BC8
sb32 itMSBombAttachedProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (mpCollisionCheckExistLineID(ip->attach_line_id) == FALSE)
    {
        ip->is_attach_surface = FALSE;

        itMSBombDetachedSetStatus(item_gobj);
    }
    return FALSE;
}

void itMSBombExplodeUpdateAttackEvent(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    ITAttackEvent *ev = itGetAttackEvent(dITMSBombItemDesc, &llITCommonDataMSBombAttackEvents); // (ITAttackEvent *)((uintptr_t)*dITMSBombItemDesc.p_file + &llITCommonDataMSBombAttackEvents); - Linker thing

    if (ip->multi == ev[ip->event_id].timer)
    {
        ip->attack_coll.angle  = ev[ip->event_id].angle;
        ip->attack_coll.damage = ev[ip->event_id].damage;
        ip->attack_coll.size   = ev[ip->event_id].size;

        ip->attack_coll.can_rehit_item = TRUE;
        ip->attack_coll.can_hop = FALSE;
        ip->attack_coll.can_reflect = FALSE;
        ip->attack_coll.can_setoff = FALSE;

        ip->attack_coll.element = nGMHitElementFire;

        ip->event_id++;

        if (ip->event_id == 4)
        {
            ip->event_id = 3;
        }
    }
}

// 0x80176D00
void itMSBombDetachedInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->damage_coll.hitstatus = nGMHitStatusNormal;
    ip->attack_coll.attack_state = nGMAttackStateOff;

    itMainClearOwnerStats(item_gobj);
}

// 0x80176D2C
sb32 itMSBombDetachedProcUpdate(GObj *item_gobj)
{
    s32 unused[2];
    GObj *fighter_gobj;
    Vec3f *translate;
    Vec3f dist;
    Vec3f fighter_pos;
    DObj *item_dobj = DObjGetStruct(item_gobj);
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITMSBOMB_GRAVITY, ITMSBOMB_TVEL);

    if (ip->multi < ITMSBOMB_DETECT_FIGHTER_DELAY)
    {
        ip->multi++;
    }
    else
    {
        fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

        translate = &item_dobj->translate.vec.f;

        while (fighter_gobj != NULL)
        {
            FTStruct *fp = ftGetStruct(fighter_gobj);
            DObj *fighter_dobj = DObjGetStruct(fighter_gobj);
            f32 offset_y = fp->attr->map_coll.top * 0.5F;

            fighter_pos = fighter_dobj->translate.vec.f;

            fighter_pos.y += offset_y;

            syVectorDiff3D(&dist, &fighter_pos, translate);

            if ((SQUARE(dist.x) + SQUARE(dist.y) + SQUARE(dist.z)) < ITMSBOMB_DETECT_FIGHTER_RADIUS)
            {
                itMSBombExplodeInitStatusVars(item_gobj, FALSE); // We might want to break out of the loop here
            }
            fighter_gobj = fighter_gobj->link_next;
        }
    }
    return FALSE;
}

// 0x80176E68
void itMSBombDetachedSetStatus(GObj *item_gobj)
{
    itMSBombDetachedInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusDetached);
}

// 0x80176E9C
void itMSBombExplodeInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = 0;

    ip->event_id = 0;

    ip->attack_coll.throw_mul = ITEM_THROW_DEFAULT;
    ip->attack_coll.fgm_id = nSYAudioFGMExplodeL;

    ip->damage_coll.hitstatus = nGMHitStatusNone;

    itMSBombExplodeUpdateAttackEvent(item_gobj);
}

// 0x80176EE4
sb32 itMSBombExplodeProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMSBombExplodeUpdateAttackEvent(item_gobj);

    ip->multi++;

    if (ip->multi == ITMSBOMB_EXPLODE_LIFETIME)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80176F2C
void itMSBombExplodeSetStatus(GObj *item_gobj)
{
    itMSBombExplodeInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITMSBombStatusDescs, nITMSBombStatusExplode);
}

// 0x80176F60
GObj* itMSBombMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITMSBombItemDesc, pos, vel, flags);
    DObj *dobj;
    ITStruct *ip;
#if defined(REGION_US)
    Vec3f translate;
#endif

    if (item_gobj != NULL)
    {
        dobj = DObjGetStruct(item_gobj);

        dobj->child->flags = DOBJ_FLAG_HIDDEN;
        dobj->child->sib_next->flags = DOBJ_FLAG_NONE;

#if defined(REGION_US)
        translate = dobj->translate.vec.f;
#endif

        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
        gcAddXObjForDObjFixed(dobj->child->sib_next, 0x46, 0);

#if defined(REGION_US)
        dobj->translate.vec.f = translate;
#else
        dobj->translate.vec.f = *pos;
#endif

        ip = itGetStruct(item_gobj);

        ip->multi = 0;

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);

        dobj->rotate.vec.f.z = 0.0F;
    }
    return item_gobj;
}
