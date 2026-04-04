#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITEggItemDesc =
{
    nITKindEgg,                             // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataEggItemAttributes,       // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,          // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0                                    // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itEggFallProcUpdate,                    // Proc Update
    itEggFallProcMap,                       // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    itEggCommonProcHit                      // Proc Damage
};

ITStatusDesc dITEggStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itEggWaitProcMap,                   // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itEggCommonProcHit                  // Proc Damage
    },

    // Status 1 (Air Fall Wait)
    {
        itEggFallProcUpdate,                // Proc Update
        itEggFallProcMap,                   // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itEggCommonProcHit                  // Proc Damage
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
        itEggThrownProcUpdate,              // Proc Update
        itEggThrownProcMap,                 // Proc Map
        itEggCommonProcHit,                 // Proc Hit
        itEggCommonProcHit,                 // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itEggCommonProcHit,                 // Proc Set-Off
        itEggCommonProcHit,                 // Proc Reflector
        itEggCommonProcHit                  // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itEggFallProcUpdate,                // Proc Update
        itEggDroppedProcMap,                // Proc Map
        itEggCommonProcHit,                 // Proc Hit
        itEggCommonProcHit,                 // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itEggCommonProcHit,                 // Proc Set-Off
        itEggCommonProcHit,                 // Proc Reflector
        itEggCommonProcHit                  // Proc Damage
    },

    // Status 5 (Neutral Explosion)
    {
        itEggExplodeProcUpdate,             // Proc Update
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

enum itEggStatus
{
    nITEggStatusWait,
    nITEggStatusFall,
    nITEggStatusHold,
    nITEggStatusThrown,
    nITEggStatusDropped,
    nITEggStatusExplode,
    nITEggStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801815C0
sb32 itEggFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITEGG_GRAVITY, ITEGG_TVEL);
    itVisualsUpdateSpin(item_gobj);

    dobj->child->rotate.vec.f.z = dobj->rotate.vec.f.z;

    return FALSE;
}

// 0x80181618
sb32 itEggWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itEggFallSetStatus);

    return FALSE;
}

// 0x80181640
sb32 itEggCommonProcHit(GObj *item_gobj)
{
    if (itMainMakeContainerItem(item_gobj) != FALSE)
    {
        efManagerEggBreakMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);

        return TRUE;
    }
    else itEggExplodeMakeEffectGotoSetStatus(item_gobj);

    return FALSE;
}

// 0x80181688
sb32 itEggFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITEGG_MAP_REBOUND_COMMON, ITEGG_MAP_REBOUND_GROUND, itEggWaitSetStatus);
}

// 0x801816B8
void itEggWaitSetModelVars(GObj *item_gobj)
{
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = 1.0F;

    dobj->child->rotate.vec.f.z = dobj->rotate.vec.f.z;
}

// 0x801816E0
void itEggWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itEggWaitSetModelVars(item_gobj);
    itMainSetStatus(item_gobj, dITEggStatusDescs, nITEggStatusWait);
}

// 0x8018171C
void itEggFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    ip->damage_coll.hitstatus = nGMHitStatusNormal;
    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->is_damage_all = TRUE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITEggStatusDescs, nITEggStatusFall);
}

// 0x80181778
void itEggHoldSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITEggStatusDescs, nITEggStatusHold);
}

// 0x801817A0
sb32 itEggThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITEGG_GRAVITY, ITEGG_TVEL);
    itVisualsUpdateSpin(item_gobj);

    dobj->child->rotate.vec.f.z = dobj->rotate.vec.f.z;

    return FALSE;
}

// 0x801817F8
sb32 itEggThrownProcMap(GObj *item_gobj)
{
    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_MAIN_MASK) != FALSE)
    {
        if (itMainMakeContainerItem(item_gobj) != FALSE)
        {
            efManagerEggBreakMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);

            return TRUE;
        }
        else itEggExplodeMakeEffectGotoSetStatus(item_gobj);
    }
    return FALSE;
}

// 0x80181854
void itEggThrownSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_damage_all = TRUE;

    ip->damage_coll.hitstatus = nGMHitStatusNormal;

    itMainSetStatus(item_gobj, dITEggStatusDescs, nITEggStatusThrown);
}

// 0x80181894
sb32 func_ovl3_80181894(GObj *item_gobj) // Unused
{
    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x801818B8
sb32 itEggDroppedProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITEGG_MAP_REBOUND_COMMON, ITEGG_MAP_REBOUND_GROUND, itEggWaitSetStatus);
}

// 0x801818E8
void itEggDroppedSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_damage_all = TRUE;

    ip->damage_coll.hitstatus = nGMHitStatusNormal;

    itMainSetStatus(item_gobj, dITEggStatusDescs, nITEggStatusDropped);
}

// 0x80181928
sb32 itEggExplodeProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi++;

    if (ip->multi == ITEGG_EXPLODE_EFFECT_WAIT)
    {
        efManagerEggBreakMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);

        return TRUE;
    }
    itMainUpdateAttackEvent(item_gobj, itGetAttackEvent(dITEggItemDesc, &llITCommonDataEggAttackEvents));

    return FALSE;
}

// 0x80181998
GObj* itEggMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITEggItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *egg_ip = itGetStruct(item_gobj);

        egg_ip->is_unused_item_bool = TRUE;

        egg_ip->arrow_gobj = ifCommonItemArrowMakeInterface(egg_ip);

        gcAddXObjForDObjFixed(dobj->child, 0x2E, 0);

        dobj->translate.vec.f = *pos;

        if (flags & ITEM_FLAG_PARENT_ITEM)
        {
            ITStruct *spawn_ip = itGetStruct(parent_gobj);

            if ((spawn_ip->kind == nITKindMLucky) && (syUtilsRandIntRange(2) == 0))
            {
                dobj->child->rotate.vec.f.y = F_CST_DTOR32(180.0F);

                egg_ip->physics.vel_air.x = -egg_ip->physics.vel_air.x;

                egg_ip->lr = -egg_ip->lr;
            }
        }
    }
    return item_gobj;
}

// 0x80181AA8
void itEggExplodeInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = 0;

    ip->event_id = 0;

    ip->attack_coll.fgm_id = nSYAudioFGMExplodeL;
    ip->attack_coll.throw_mul = ITEM_THROW_DEFAULT;

    func_800269C0_275C0(nSYAudioFGMExplodeL);

    ip->attack_coll.can_rehit_item = TRUE;
    ip->attack_coll.can_hop = FALSE;
    ip->attack_coll.can_reflect = FALSE;
    ip->attack_coll.can_setoff = FALSE;
    ip->attack_coll.element = nGMHitElementFire;

    ip->damage_coll.hitstatus = nGMHitStatusNone;

    itMainClearOwnerStats(item_gobj);
    itMainRefreshAttackColl(item_gobj);
    itMainUpdateAttackEvent(item_gobj, itGetAttackEvent(dITEggItemDesc, &llITCommonDataCapsuleAttackEvents)); // Should this be llITCommonDataEggAttackEvents?
}

// 0x80181B5C
void itEggExplodeSetStatus(GObj *item_gobj)
{
    itEggExplodeInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITEggStatusDescs, nITEggStatusExplode);
}

// 0x80181B90
void itEggExplodeMakeEffectGotoSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    LBParticle *ep;

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    ep = efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);

    if (ep != NULL)
    {
        ep->xf->scale.x = ep->xf->scale.y = ep->xf->scale.z = ITEGG_EXPLODE_EFFECT_SCALE;
    }
    efManagerQuakeMakeEffect(1);

    DObjGetStruct(item_gobj)->flags = DOBJ_FLAG_HIDDEN;

    itEggExplodeSetStatus(item_gobj);
}
