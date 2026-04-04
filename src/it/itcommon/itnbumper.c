#include <it/item.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018A690
ITDesc dITNBumperItemDesc =
{
    nITKindNBumper,                         // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataNBumperItemAttributes,   // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTra,                    // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0                                    // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itNBumperFallProcUpdate,                // Proc Update
    itNBumperFallProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018A6C4
ITStatusDesc dITNBumperStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itNBumperWaitProcMap,               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itNBumperFallProcUpdate,            // Proc Update
        itNBumperFallProcMap,               // Proc Map
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
        itNBumperThrownProcUpdate,          // Proc Update
        itNBumperThrownProcMap,             // Proc Map
        itNBumperThrownProcHit,             // Proc Hit
        itNBumperThrownProcShield,          // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        NULL,                               // Proc Set-Off
        itNBumperThrownProcReflector,       // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itNBumperThrownProcUpdate,          // Proc Update
        itNBumperThrownProcMap,             // Proc Map
        itNBumperThrownProcHit,             // Proc Hit
        itNBumperThrownProcShield,          // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        NULL,                               // Proc Set-Off
        itNBumperThrownProcReflector,       // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 5 (Ground Active Wait)
    {
        itNBumperAttachedProcUpdate,        // Proc Update
        itNBumperAttachedProcMap,           // Proc Map
        itNBumperAttachedProcHit,           // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        itNBumperAttachedProcReflector,     // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 6 (Airborne after Ground Active Wait)
    {
        itNBumperHitAirProcUpdate,          // Proc Update
        itNBumperThrownProcMap,             // Proc Map
        itNBumperThrownProcHit,             // Proc Hit
        itNBumperThrownProcShield,          // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        itNBumperThrownProcReflector,       // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 7 (Despawn)
    {
        itNBumperGDisappearProcUpdate,      // Proc Update
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

enum itNBumperStatus
{
    nITNBumperStatusWait,                   // Ground neutral
    nITNBumperStatusFall,                   // Airborne neutral
    nITNBumperStatusHold,                   // Fighter hold
    nITNBumperStatusThrown,                 // Fighter throw
    nITNBumperStatusDropped,                // Fighter drop
    nITNBumperStatusAttached,                // Ground active
    nITNBumperStatusHitAir,                   // Airborne hit
    nITNBumperStatusGDisappear,             // Ground despawn
    nITNBumperStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017B430
sb32 itNBumperFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITBUMPER_GRAVITY_NORMAL, ITBUMPER_TVEL);

    if (ip->multi != 0)
    {
        dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = (2.0F - (10 - ip->multi) * 0.1F);

        ip->multi--;
    }
    else dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = 1.0F;
    
    if (!ip->item_vars.bumper.damage_all_delay)
    {
        itMainClearOwnerStats(item_gobj);

        ip->item_vars.bumper.damage_all_delay = -1;
    }
    if (ip->item_vars.bumper.damage_all_delay != -1)
    {
        ip->item_vars.bumper.damage_all_delay--;
    }
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x8017B520
sb32 itNBumperWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itNBumperFallSetStatus);

    return FALSE;
}

// 0x8017B548
sb32 itNBumperFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITBUMPER_MAP_REBOUND_COMMON, ITBUMPER_MAP_REBOUND_GROUND, itNBumperWaitSetStatus);
}

// 0x8017B57C
sb32 itNBumperThrownProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->scale.vec.f.x = 2.0F;
    dobj->scale.vec.f.y = 2.0F;
    dobj->scale.vec.f.z = 2.0F;

    ip->item_vars.bumper.hit_anim_length = ITBUMPER_HIT_ANIM_LENGTH;

    dobj->mobj->palette_id = 1.0F;

    ip->physics.vel_air.x = ITBUMPER_REBOUND_AIR_X * ip->hit_lr;
    ip->physics.vel_air.y = ITBUMPER_REBOUND_AIR_Y;

    ip->multi = ITBUMPER_HIT_SCALE;

    itNBumperHitAirSetStatus(item_gobj);

    return FALSE;
}

// 0x8017B600
void itNBumperWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusWait);
}

// 0x8017B634
void itNBumperFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusFall);
}

// 0x8017B678
void itNBumperHoldSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusHold);
}

// 0x8017B6A0
sb32 itNBumperThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITBUMPER_GRAVITY_NORMAL, ITBUMPER_TVEL);

    if (!(ip->item_vars.bumper.damage_all_delay))
    {
        itMainClearOwnerStats(item_gobj);

        ip->item_vars.bumper.damage_all_delay = -1;
    }
    if (ip->item_vars.bumper.damage_all_delay != -1)
    {
        ip->item_vars.bumper.damage_all_delay--;
    }
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x8017B720
sb32 itNBumperThrownProcMap(GObj *item_gobj)
{
    return itMapCheckMapReboundProcNoFloor(item_gobj, 0.8F, itNBumperAttachedSetStatus);
}

// 0x8017B74C
sb32 itNBumperThrownProcShield(GObj *item_gobj)
{
    itMainVelSetRebound(item_gobj);
    itMainClearOwnerStats(item_gobj);

    return FALSE;
}

// 0x8017B778
sb32 itNBumperThrownProcReflector(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp = ftGetStruct(ip->owner_gobj);

    if ((ip->physics.vel_air.x * fp->lr) < 0.0F)
    {
        ip->physics.vel_air.x = -ip->physics.vel_air.x;
    }
    itMainClearOwnerStats(item_gobj);

    return FALSE;
}

// 0x8017B7DC
void itNBumperThrownSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->item_vars.bumper.damage_all_delay = ITBUMPER_DAMAGE_ALL_WAIT;

    ip->coll_data.map_coll.top = ITBUMPER_COLL_SIZE;
    ip->coll_data.map_coll.bottom = -ITBUMPER_COLL_SIZE;

    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusThrown);
}

// 0x8017B828
void itNBumperDroppedSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->item_vars.bumper.damage_all_delay = ITBUMPER_DAMAGE_ALL_WAIT;

    ip->coll_data.map_coll.top = ITBUMPER_COLL_SIZE;
    ip->coll_data.map_coll.bottom = -ITBUMPER_COLL_SIZE;

    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusDropped);
}

// 0x8017B874
void itNBumperAttachedSetModelPitch(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    s32 unused;
    Vec3f floor_angle;
    DObj *dobj = DObjGetStruct(item_gobj);

    floor_angle = ip->coll_data.floor_angle;

    ip->attach_line_id = ip->coll_data.floor_line_id;

    dobj->rotate.vec.f.z = syUtilsArcTan2(floor_angle.y, floor_angle.x) - F_CLC_DTOR32(90.0F);
}

// 0x8017B8DC
void itNBumperAttachedInitVars(GObj *item_gobj)
{
    s32 unused[2];
    DObj *dobj;
    ITStruct *ip;
    MObjSub *mobjsub;
    Gfx *dl;

    ip = itGetStruct(item_gobj);
    dobj = DObjGetStruct(item_gobj);

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    dl = itGetPData(ip, &llITCommonDataNBumperDataStart, &llITCommonDataNBumperWaitDisplayList); // (uintptr_t)((uintptr_t)ip->attr->data - (intptr_t)&llITCommonDataNBumperDataStart) + (intptr_t)&llITCommonDataNBumperWaitDisplayList; Linker thing

    dobj->dl = dl;

    mobjsub = itGetPData(ip, &llITCommonDataNBumperDataStart, &llITCommonDataNBumperWaitMObjSub); // ((uintptr_t)((uintptr_t)ip->attr->data - (intptr_t)&llITCommonDataNBumperDataStart) + (intptr_t)&llITCommonDataNBumperWaitMObjSub);

    gcRemoveMObjAll(dobj);
    gcAddMObjForDObj(dobj, mobjsub);

    dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = 1.0F;

    ip->coll_data.map_coll.top = ITBUMPER_COLL_SIZE;
    ip->coll_data.map_coll.bottom = -ITBUMPER_COLL_SIZE;

    itNBumperAttachedSetModelPitch(item_gobj);

    ip->is_attach_surface = TRUE;

    ip->lifetime = ITBUMPER_LIFETIME;

    itMainClearOwnerStats(item_gobj);
}

// 0x8017B9C8
sb32 itNBumperAttachedProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->scale.vec.f.x = 2.0F;
    dobj->scale.vec.f.z = 2.0F;

    ip->item_vars.bumper.hit_anim_length = ITBUMPER_HIT_ANIM_LENGTH;

    dobj->mobj->palette_id = 1.0F;

    ip->lr = -ip->hit_lr;

    ip->physics.vel_air.x = ip->hit_lr * ITBUMPER_REBOUND_VEL_X;

    ip->multi = ITBUMPER_HIT_SCALE;

    return FALSE;
}

// 0x8017BA2C
sb32 itNBumperAttachedProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    ITAttributes *attr = ip->attr;
    DObj *dobj = DObjGetStruct(item_gobj);
    Vec3f edge_pos;

    if ((ip->item_vars.bumper.hit_anim_length == 0) && (dobj->mobj->palette_id == 1.0F))
    {
        dobj->mobj->palette_id = 0.0F;
    }
    else ip->item_vars.bumper.hit_anim_length--;

    if (mpCollisionCheckExistLineID(ip->coll_data.floor_line_id) != FALSE)
    {
        if (ip->lr == -1)
        {
            mpCollisionGetFloorEdgeL(ip->coll_data.floor_line_id, &edge_pos);

            if (edge_pos.x >= (dobj->translate.vec.f.x - attr->map_coll_width))
            {
                ip->physics.vel_air.x = 0.0F;
            }
        }
        else
        {
            mpCollisionGetFloorEdgeR(ip->coll_data.floor_line_id, &edge_pos);

            if (edge_pos.x <= (dobj->translate.vec.f.x + attr->map_coll_width))
            {
                ip->physics.vel_air.x = 0.0F;
            }
        }
    }
    if (ip->multi < ITBUMPER_STOPVEL_WAIT)
    {
        ip->physics.vel_air.x = 0.0F;
    }
    if (ip->multi != 0)
    {
        dobj->scale.vec.f.x = dobj->scale.vec.f.z = 2.0F - ((10 - ip->multi) * 0.1F);

        ip->multi--;
    }
    else dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = 1.0F;
    
    if (ip->lifetime == 0)
    {
        itNBumperGDisappearSetStatus(item_gobj);
    }
    ip->lifetime--;

    return FALSE;
}

// 0x8017BBFC
sb32 itNBumperAttachedProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *joint = DObjGetStruct(item_gobj);

    if (itMapCheckLRWallProcNoFloor(item_gobj, itNBumperDroppedSetStatus) != FALSE)
    {
        if (mpCollisionCheckExistLineID(ip->attach_line_id) == FALSE)
        {
            ip->is_attach_surface = FALSE;

            itNBumperDroppedSetStatus(item_gobj);

#if defined(REGION_US)
            joint->scale.vec.f.x = joint->scale.vec.f.y = joint->scale.vec.f.z = 1.0F;

            joint->mobj->palette_id = 0.0F;
#endif
        }
        else if (ip->multi == 0)
        {
            itNBumperAttachedSetModelPitch(item_gobj);
        }
    }
    return FALSE;
}

// 0x8017BCC0
sb32 itNBumperAttachedProcReflector(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp = ftGetStruct(ip->owner_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->scale.vec.f.x = 2.0F;
    dobj->scale.vec.f.z = 2.0F;

    ip->item_vars.bumper.hit_anim_length = 3;

    dobj->mobj->palette_id = 1.0F;

    ip->physics.vel_air.x = (-fp->lr * ITBUMPER_REBOUND_VEL_X);

    ip->lr = fp->lr;

    ip->multi = ITBUMPER_HIT_SCALE;

    itMainClearOwnerStats(item_gobj);

    return FALSE;
}

// 0x8017BD4C
void itNBumperAttachedSetStatus(GObj *item_gobj)
{
    itNBumperAttachedInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusAttached);
}

// 0x8017BD80
sb32 itNBumperHitAirProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    if ((ip->item_vars.bumper.hit_anim_length == 0) && (dobj->mobj->palette_id == 1.0F))
    {
        dobj->mobj->palette_id = 0.0F;
    }
    else ip->item_vars.bumper.hit_anim_length--;

    itMainApplyGravityClampTVel(ip, ITBUMPER_GRAVITY_HIT, ITBUMPER_TVEL);

    if (ip->multi != 0)
    {
        dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = (2.0F - (10 - ip->multi) * 0.1F);

        ip->multi--;
    }
    else dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = 1;
    
    if (!ip->item_vars.bumper.damage_all_delay)
    {
        itMainClearOwnerStats(item_gobj);

        ip->item_vars.bumper.damage_all_delay = -1;
    }
    if (ip->item_vars.bumper.damage_all_delay != -1)
    {
        ip->item_vars.bumper.damage_all_delay--;
    }
    return FALSE;
}

// 0x8017BEA0
void itNBumperHitAirSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->item_vars.bumper.damage_all_delay = ITBUMPER_DAMAGE_ALL_WAIT;

    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusHitAir);
}

// 0x8017BED4
sb32 itNBumperGDisappearProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->lifetime == 0)
    {
        return TRUE;
    }
    else if ((ip->lifetime % 2) != 0)
    {
        DObj *dobj = DObjGetStruct(item_gobj);

        dobj->flags ^= DOBJ_FLAG_HIDDEN;
    }
    ip->lifetime--;

    return FALSE;
}

// 0x8017BF1C
void itNBumperGDisappearSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->mobj->palette_id = 0;

    dobj->scale.vec.f.x = 1.0F;
    dobj->scale.vec.f.y = 1.0F;
    dobj->scale.vec.f.z = 1.0F;

    ip->lifetime = ITBUMPER_DESPAWN_TIMER;

    dobj->flags = DOBJ_FLAG_NONE;

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    itMainSetStatus(item_gobj, dITNBumperStatusDescs, nITNBumperStatusGDisappear);
}

// 0x8017BF8C
GObj* itNBumperMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITNBumperItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip;
#if defined(REGION_US)
        Vec3f translate = dobj->translate.vec.f;
#endif

        ip = itGetStruct(item_gobj);

        ip->multi = 0;

        ip->attack_coll.interact_mask = GMHITCOLLISION_FLAG_FIGHTER;

        ip->attack_coll.can_rehit_shield = TRUE;

        dobj->mobj->palette_id = 0.0F;

        gcAddXObjForDObjFixed(dobj, 0x2E, 0);

#if defined(REGION_US)
        dobj->translate.vec.f = translate;
#else
        dobj->translate.vec.f = *pos;
#endif

        dobj->rotate.vec.f.z = 0.0F;

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}
