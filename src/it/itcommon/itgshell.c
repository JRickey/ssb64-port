#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITGShellItemDesc =
{
    nITKindGShell,                          // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataGShellItemAttributes,    // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                  // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itGShellFallProcUpdate,                 // Proc Update
    itGShellFallProcMap,                    // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITGShellStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itGShellWaitProcMap,                // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itGShellCommonProcDamage            // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itGShellFallProcUpdate,             // Proc Update
        itGShellFallProcMap,                // Proc Map
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
        itGShellThrownProcUpdate,           // Proc Update
        itGShellThrownProcMap,              // Proc Map
        itGShellCommonProcHit,              // Proc Hit
        itGShellCommonProcShield,           // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itGShellCommonProcShield,           // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        itGShellCommonProcDamage            // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itGShellFallProcUpdate,             // Proc Update
        itGShellThrownProcMap,              // Proc Map
        itGShellCommonProcHit,              // Proc Hit
        itGShellCommonProcShield,           // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itGShellCommonProcShield,           // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        itGShellCommonProcDamage            // Proc Damage
    },

    // Status 5 (Ground Spin)
    {
        itGShellSpinProcUpdate,             // Proc Update
        itGShellSpinProcMap,                // Proc Map
        itGShellCommonProcHit,              // Proc Hit
        itGShellCommonProcHit,              // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        itGShellSpinProcDamage              // Proc Damage
    },

    // Status 6 (Air Spin)
    {
        itGShellFallProcUpdate,             // Proc Update
        itGShellThrownProcMap,              // Proc Map
        itGShellCommonProcHit,              // Proc Hit
        itGShellCommonProcHit,              // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        itGShellSpinProcDamage              // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itGShellStatus
{
    nITGShellStatusWait,
    nITGShellStatusFall,
    nITGShellStatusHold,
    nITGShellStatusThrown,
    nITGShellStatusDropped,
    nITGShellStatusSpin,
    nITGShellStatusSpinAir,
    nITGShellStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801785E0
void itGShellSpinUpdateEffect(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    Vec3f pos;

    if (ip->item_vars.shell.dust_effect_int == 0)
    {
        pos = dobj->translate.vec.f;

        pos.y += ip->attr->map_coll_bottom;

        efManagerDustLightMakeEffect(&pos, ip->lr, 1.0F);

        ip->item_vars.shell.dust_effect_int = ITGSHELL_EFFECT_SPAWN_INT;
    }
    ip->item_vars.shell.dust_effect_int--;
}

// 0x80178670
void itGShellSpinAddAnim(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    s32 unused[2];

    gcAddDObjAnimJoint(dobj, itGetPData(ip, &llITCommonDataShellDataStart, &llITCommonDataShellAnimJoint), 0.0F);
    gcAddMObjMatAnimJoint(dobj->mobj, itGetPData(ip, &llITCommonDataShellDataStart, &llITCommonDataShellMatAnimJoint), 0.0F);
    gcPlayAnimAll(item_gobj);
}

// 0x80178704
void itGShellCommonClearAnim(GObj *item_gobj)
{
    DObjGetStruct(item_gobj)->mobj->matanim_joint.event32 = NULL;
    DObjGetStruct(item_gobj)->anim_joint.event32 = NULL;
}

// 0x8017871C
sb32 itGShellFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITGSHELL_GRAVITY, ITGSHELL_TVEL);

    return FALSE;
}

// 0x8017874C
sb32 itGShellWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itGShellFallSetStatus);

    return FALSE;
}

// 0x80178774
sb32 itGShellFallProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->item_vars.shell.health == 0)
    {
        return itMapCheckDestroyLanding(item_gobj, ITGSHELL_MAP_REBOUND_COMMON);
    }
    else itMapCheckDestroyDropped(item_gobj, ITGSHELL_MAP_REBOUND_COMMON, ITGSHELL_MAP_REBOUND_GROUND, itGShellWaitSetStatus);

    return FALSE;
}

// 0x801787CC
void itGShellWaitInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMapSetGround(ip);

    if (ABSF(ip->physics.vel_air.x) < ITGSHELL_STOP_VEL_X)
    {
        itMainSetGroundAllowPickup(item_gobj);

        ip->item_vars.shell.is_damage = FALSE;

        ip->is_damage_all = TRUE;

        ip->damage_coll.hitstatus = nGMHitStatusNormal;
        ip->attack_coll.attack_state = nGMAttackStateOff;

        ip->physics.vel_air.x = 0.0F;

        itGShellCommonClearAnim(item_gobj);
        itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusWait);
    }
    else if (ip->item_vars.shell.is_damage != FALSE)
    {
        ip->attack_coll.attack_state = nGMAttackStateNew;

        itProcessUpdateAttackPositions(item_gobj);
        itGShellSpinSetStatus(item_gobj);
    }
    else
    {
        itMainSetGroundAllowPickup(item_gobj);

        ip->is_damage_all = TRUE;

        ip->damage_coll.hitstatus = nGMHitStatusNormal;
        ip->attack_coll.attack_state = nGMAttackStateOff;

        ip->physics.vel_air.x = 0.0F;

        itGShellCommonClearAnim(item_gobj);
        itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusWait);
    }
}

// 0x80178910
void itGShellWaitSetStatus(GObj *item_gobj)
{
    itGShellWaitInitVars(item_gobj);
}

// 0x80178930
void itGShellFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->damage_coll.hitstatus = nGMHitStatusNone;

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusFall);
}

// 0x8017897C
sb32 itGShellCommonProcDamage(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->physics.vel_air.x = (ip->damage_queue * ITGSHELL_DAMAGE_MUL_NORMAL * -ip->damage_lr);

    if (ABSF(ip->physics.vel_air.x) > ITGSHELL_STOP_VEL_X)
    {
        ip->item_vars.shell.is_damage = TRUE;

        ip->attack_coll.attack_state = nGMAttackStateNew;

        itProcessUpdateAttackPositions(item_gobj);

        ip->damage_coll.hitstatus = nGMHitStatusNone;

        itMainCopyDamageStats(item_gobj);

        if (ip->ga != nMPKineticsGround)
        {
            itGShellSpinAirSetStatus(item_gobj);
        }
        else itGShellSpinSetStatus(item_gobj);
    }
    else
    {
        ip->physics.vel_air.x = 0.0F;

        if (ip->ga != nMPKineticsGround)
        {
            itGShellFallSetStatus(item_gobj);
        }
        else itGShellWaitSetStatus(item_gobj);
    }
    return FALSE;
}

// 0x80178A90
void itGShellHoldSetStatus(GObj *item_gobj)
{
    DObjGetStruct(item_gobj)->rotate.vec.f.y = 0.0F;

    itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusHold);
}

// 0x80178AC4
sb32 itGShellThrownProcMap(GObj *item_gobj)
{
    itMapCheckLanding(item_gobj, ITGSHELL_MAP_REBOUND_COMMON, ITGSHELL_MAP_REBOUND_GROUND, itGShellWaitSetStatus);

    return FALSE;
}

// 0x80178AF8
sb32 itGShellThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITGSHELL_GRAVITY, ITGSHELL_TVEL);

    return FALSE;
}

// 0x80178B28
void itGShellThrownSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->item_vars.shell.health = 1;
    ip->item_vars.shell.is_damage = TRUE;

    itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusThrown);
}

// 0x80178B60
void itGShellDroppedSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->item_vars.shell.health = 1;
    ip->item_vars.shell.is_damage = TRUE;

    itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusDropped);
}

// 0x80178B98
sb32 itGShellSpinProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itGShellSpinUpdateEffect(item_gobj);

    if (!(ip->item_vars.shell.damage_all_delay))
    {
        ip->is_damage_all = TRUE;

        ip->item_vars.shell.damage_all_delay = -1;
    }
    if (ip->item_vars.shell.damage_all_delay != -1)
    {
        ip->item_vars.shell.damage_all_delay--;
    }
    if (ip->lifetime == 0)
    {
        return TRUE;
    }
    else ip->lifetime--;

    return FALSE;
}

// 0x80178C10
sb32 itGShellSpinProcMap(GObj *item_gobj)
{
    /*
     * OVERSIGHT (?): This sets the state of the shell to "Fall" when transitioning from the grounded spinning state,
     * causing the shell's hitbox to deactivate when flying off platforms.
     * 
     * Solution: itMapCheckLRWallProcNoFloor(item_gobj, itGShellSpinAirSetStatus);
     */
    itMapCheckLRWallProcNoFloor(item_gobj, itGShellFallSetStatus);

    if (itMapCheckCollideAllRebound(item_gobj, (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL), 0.2F, NULL) != FALSE)
    {
        itMainSetSpinVelLR(item_gobj);
        itMainClearOwnerStats(item_gobj);
    }
    return FALSE;
}

// 0x80178C6C
sb32 itGShellCommonProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->damage_coll.hitstatus = nGMHitStatusNormal;

    ip->item_vars.shell.health = syUtilsRandIntRange(ITGSHELL_HEALTH_MAX);

    ip->physics.vel_air.y = ITGSHELL_REBOUND_VEL_Y;

    ip->physics.vel_air.x = syUtilsRandFloat() * (-ip->physics.vel_air.x * ITGSHELL_REBOUND_MUL_X);

    itMainClearOwnerStats(item_gobj);
    itGShellCommonClearAnim(item_gobj);
    itGShellFallSetStatus(item_gobj);

    return FALSE;
}

// 0x80178CF8
sb32 itGShellSpinProcDamage(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->physics.vel_air.x += (ip->damage_queue * ITGSHELL_DAMAGE_MUL_ADD * -ip->damage_lr);

    if (ABSF(ip->physics.vel_air.x) > ITGSHELL_STOP_VEL_X)
    {
        ip->attack_coll.attack_state = nGMAttackStateNew;

        itProcessUpdateAttackPositions(item_gobj);
        itMainCopyDamageStats(item_gobj);

        if (ip->ga != nMPKineticsGround)
        {
            itGShellSpinAirSetStatus(item_gobj);
        }
        else itGShellSpinSetStatus(item_gobj);
    }
    else
    {
        ip->physics.vel_air.x = 0.0F;

        if (ip->ga != nMPKineticsGround)
        {
            itGShellFallSetStatus(item_gobj);
        }
        else itGShellWaitSetStatus(item_gobj);
    }
    return FALSE;
}

// 0x80178E04
void itGShellSpinInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    ip->pickup_wait = ITEM_PICKUP_WAIT_DEFAULT;

    if (ip->physics.vel_air.x > ITGSHELL_CLAMP_VEL_X)
    {
        ip->physics.vel_air.x = ITGSHELL_CLAMP_VEL_X;
    }
    if (ip->physics.vel_air.x < -ITGSHELL_CLAMP_VEL_X)
    {
        ip->physics.vel_air.x = -ITGSHELL_CLAMP_VEL_X;
    }
    ip->physics.vel_air.y = 0.0F;

    if (ip->physics.vel_air.x < 0.0F)
    {
        ip->lr = -1;
    }
    else ip->lr = +1;

    ip->item_vars.shell.dust_effect_int = ITGSHELL_EFFECT_SPAWN_INT;
    ip->item_vars.shell.damage_all_delay = ITGSHELL_DAMAGE_ALL_WAIT;

    itGShellSpinAddAnim(item_gobj);

    ip->is_damage_all = FALSE;

    itMainRefreshAttackColl(item_gobj);
    func_800269C0_275C0(nSYAudioFGMBombHeiWalkStart);
}

// 0x80178EDC
void itGShellSpinSetStatus(GObj *item_gobj)
{
    itGShellSpinInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusSpin);
}

// 0x80178F10
void itGShellSpinAirInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->physics.vel_air.x > ITGSHELL_CLAMP_VEL_X)
    {
        ip->physics.vel_air.x = ITGSHELL_CLAMP_VEL_X;
    }
    if (ip->physics.vel_air.x < -ITGSHELL_CLAMP_VEL_X)
    {
        ip->physics.vel_air.x = -ITGSHELL_CLAMP_VEL_X;
    }
    if (ip->physics.vel_air.x < 0.0F)
    {
        ip->lr = -1;
    }
    else ip->lr = +1;

    ip->is_damage_all = FALSE;

    itMainRefreshAttackColl(item_gobj);
}

// 0x80178FA8
void itGShellSpinAirSetStatus(GObj *item_gobj)
{
    itGShellSpinAirInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITGShellStatusDescs, nITGShellStatusSpinAir);
}

// 0x80178FDC
GObj* itGShellMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITGShellItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip;
#if defined(REGION_US)
        Vec3f translate = dobj->translate.vec.f;
#endif

        dobj->rotate.vec.f.y = F_CST_DTOR32(90.0F);

        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
        gcAddXObjForDObjFixed(dobj, 0x48, 0);

#if defined(REGION_US)
        dobj->translate.vec.f = translate;
#else
        dobj->translate.vec.f = *pos;
#endif

        dobj->mobj->palette_id = 1.0F;

        ip = itGetStruct(item_gobj);

        ip->attack_coll.can_rehit_shield = TRUE;

        ip->item_vars.shell.health = 1;
        ip->item_vars.shell.is_damage = FALSE;

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);

        ip->lifetime = ITGSHELL_LIFETIME;
    }
    return item_gobj;
}

// 0x801790F4
sb32 itGShellCommonProcShield(GObj *item_gobj)
{
    itMainVelSetRebound(item_gobj);

    return FALSE;
}
