#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITTaruItemDesc =
{
    nITKindTaru,                            // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataTaruItemAttributes,      // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itTaruFallProcUpdate,                   // Proc Update
    itTaruFallProcMap,                      // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITTaruStatusDescs[/* */] = 
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itTaruWaitProcMap,                  // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itTaruCommonProcDamage              // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itTaruFallProcUpdate,               // Proc Update
        itTaruFallProcMap,                  // Proc Map
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
        itTaruFallProcUpdate,               // Proc Update
        itTaruThrownProcMap,                // Proc Map
        itTaruCommonProcHit,                // Proc Hit
        itTaruCommonProcHit,                // Proc Shield
        NULL,                               // Proc Hop
        itTaruCommonProcHit,                // Proc Set-Off
        itTaruCommonProcHit,                // Proc Reflector
        itTaruCommonProcDamage              // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itTaruFallProcUpdate,               // Proc Update
        itTaruThrownProcMap,                // Proc Map
        itTaruCommonProcHit,                // Proc Hit
        itTaruCommonProcHit,                // Proc Shield
        NULL,                               // Proc Hop
        itTaruCommonProcHit,                // Proc Set-Off
        itTaruCommonProcHit,                // Proc Reflector
        itTaruCommonProcDamage              // Proc Damage
    },

    // Status 5 (Neutral Explosion)
    {
        itTaruExplodeProcUpdate,            // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 6 (Ground Roll)
    {
        itTaruRollProcUpdate,               // Proc Update
        itTaruRollProcMap,                  // Proc Map
        itTaruCommonProcHit,                // Proc Hit
        itTaruCommonProcHit,                // Proc Shield
        NULL,                               // Proc Hop
        itTaruCommonProcHit,                // Proc Set-Off
        itTaruCommonProcHit,                // Proc Reflector
        itTaruCommonProcDamage              // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itTaruStatus
{
    nITTaruStatusWait,
    nITTaruStatusFall,
    nITTaruStatusHold,
    nITTaruStatusThrown,
    nITTaruStatusDropped,
    nITTaruStatusExplode,
    nITTaruStatusRoll,
    nITTaruStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80179BA0
sb32 itTaruFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITTARU_GRAVITY, ITTARU_TVEL);

    DObjGetStruct(item_gobj)->rotate.vec.f.z += ip->item_vars.taru.roll_rotate_step;

    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80179BF8
sb32 itTaruWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itTaruFallSetStatus);

    return FALSE;
}

// 0x80179C20
sb32 itTaruCommonProcHit(GObj *item_gobj)
{
    func_800269C0_275C0(nSYAudioFGMContainerSmash);

    itBoxContainerSmashMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);

    if (itMainMakeContainerItem(item_gobj) != FALSE)
    {
        return TRUE;
    }
    else itTaruExplodeMakeEffectGotoSetStatus(item_gobj);

    return FALSE;
}

// 0x80179C78
sb32 itTaruCommonProcDamage(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->percent_damage >= ITTARU_HEALTH_MAX)
    {
        return itTaruCommonProcHit(item_gobj);
    }
    else return FALSE;
}

// 0x80179CB8
sb32 itTaruFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITTARU_MAP_REBOUND_COMMON, ITTARU_MAP_REBOUND_GROUND, itTaruWaitSetStatus);
}

// 0x80179CE8
void itTaruWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusWait);
}

// 0x80179D1C
void itTaruFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusFall);
}

// 0x80179D60
void itTaruHoldSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusHold);
}

// 0x80179D88
sb32 itTaruThrownCheckMapCollision(GObj *item_gobj, f32 common_rebound)
{
    s32 unused;
    ITStruct *ip;
    sb32 is_collide_floor = itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR);

    if (itMapCheckCollideAllRebound(item_gobj, (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL), common_rebound, NULL) != FALSE)
    {
        itMainSetSpinVelLR(item_gobj);
    }
    if (is_collide_floor != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80179DEC
void itTaruRollSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->lifetime = ITTARU_LIFETIME;

    ip->physics.vel_air.y = 0.0F;

    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusRoll);
}

// 0x80179E28
sb32 itTaruThrownProcMap(GObj *item_gobj)
{
    if (itTaruThrownCheckMapCollision(item_gobj, 0.5F) != FALSE)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if (ip->physics.vel_air.y >= 90.0F)       // Is it even possible to meet this condition? Didn't they mean ABSF(ip->physics.vel_air.y)?
        {
            itTaruCommonProcHit(item_gobj);       // This causes the barrel to smash on impact when landing from too high; doesn't seem possible to trigger

            return TRUE;
        }
        else if (ip->physics.vel_air.y < 30.0F)
        {
            itTaruRollSetStatus(item_gobj);
        }
        else
        {
            lbCommonReflect2D(&ip->physics.vel_air, &ip->coll_data.floor_angle);

            ip->physics.vel_air.y *= 0.2F;

            itMainSetSpinVelLR(item_gobj);
        }
        itMainClearOwnerStats(item_gobj);
    }
    return FALSE;
}

// 0x80179EF0
void itTaruThrownInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.x = F_CST_DTOR32(90.0F);

    ip->coll_data.map_coll.top = ip->coll_data.map_coll.width;
    ip->coll_data.map_coll.bottom = -ip->coll_data.map_coll.width;
}

// 0x80179F1C
void itTaruThrownSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusThrown);
    itTaruThrownInitVars(item_gobj);
}

// 0x80179F50 - Unused
sb32 func_ovl3_80179F50(GObj *item_gobj)
{
    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x80179F74
void itTaruDroppedSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusDropped);
    itTaruThrownInitVars(item_gobj);
}

// 0x80179FA8
sb32 itTaruExplodeProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi++;

    if (ip->multi == ITTARU_EXPLODE_LIFETIME)
    {
        return TRUE;
    }
    else itMainUpdateAttackEvent(item_gobj, itGetAttackEvent(dITTaruItemDesc, &llITCommonDataTaruAttackEvents));

    return FALSE;
}

// 0x8017A004
sb32 itTaruRollProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    f32 roll_rotate_step;
    f32 sqrt_vel;

    ip->physics.vel_air.x += (-(syUtilsArcTan2(ip->coll_data.floor_angle.y, ip->coll_data.floor_angle.x) - F_CLC_DTOR32(90.0F)) * ITTARU_MUL_VEL_X);

    ip->lr = (ip->physics.vel_air.x >= 0.0F) ? +1 : -1;

    sqrt_vel = sqrtf(SQUARE(ip->physics.vel_air.x) + SQUARE(ip->physics.vel_air.y));

    if (sqrt_vel < ITTARU_VEL_MIN)
    {
        ip->lifetime--;

        if (ip->lifetime < ITTARU_DESPAWN_FLASH_START)
        {
            if (ip->lifetime == 0)
            {
                return TRUE;
            }
            else if ((ip->lifetime % 2) != 0)
            {
                DObjGetStruct(item_gobj)->flags ^= DOBJ_FLAG_HIDDEN;
            }
        }
    }
    roll_rotate_step = ((ip->lr == -1) ? ITTARU_ROLL_ROTATE_MUL : -ITTARU_ROLL_ROTATE_MUL) * sqrt_vel;

    ip->item_vars.taru.roll_rotate_step = roll_rotate_step;

    DObjGetStruct(item_gobj)->rotate.vec.f.z += roll_rotate_step;

    return FALSE;
}

// 0x8017A148
sb32 itTaruRollProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestLRWallCheckFloor(item_gobj) == FALSE)
    {
        itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusDropped);
    }
    else if (ip->coll_data.mask_curr & (MAP_FLAG_RWALL | MAP_FLAG_LWALL))
    {
        return itTaruCommonProcHit(item_gobj);
    }
    return FALSE;
}

// 0x8017A1B8
GObj* itTaruMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITTaruItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        ip->item_vars.taru.roll_rotate_step = 0.0F;

        ip->is_damage_all = TRUE;

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}

// 0x8017A240
void itTaruExplodeInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = 0;
    ip->event_id = 0;

    ip->attack_coll.fgm_id = nSYAudioFGMExplodeL;

    ip->attack_coll.can_rehit_item = TRUE;
    ip->attack_coll.can_reflect = FALSE;

    ip->attack_coll.throw_mul = ITEM_THROW_DEFAULT;
    ip->attack_coll.element = nGMHitElementFire;

    ip->attack_coll.can_setoff = FALSE;

    ip->damage_coll.hitstatus = nGMHitStatusNone;

    itMainClearOwnerStats(item_gobj);
    itMainRefreshAttackColl(item_gobj);
    itMainUpdateAttackEvent(item_gobj, itGetAttackEvent(dITTaruItemDesc, &llITCommonDataTaruAttackEvents));
}

// 0x8017A2D8
void itTaruExplodeSetStatus(GObj *item_gobj)
{
    itTaruExplodeInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITTaruStatusDescs, nITTaruStatusExplode);
}

// 0x8017A30C
void itTaruExplodeMakeEffectGotoSetStatus(GObj *item_gobj)
{
    LBParticle *pc;
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    pc = efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);

    if (pc != NULL)
    {
        pc->xf->scale.x = pc->xf->scale.y = pc->xf->scale.z = ITTARU_EXPLODE_SCALE;
    }
    efManagerQuakeMakeEffect(1);

    DObjGetStruct(item_gobj)->flags = DOBJ_FLAG_HIDDEN;

    itTaruExplodeSetStatus(item_gobj);
}
