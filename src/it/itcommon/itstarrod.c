#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITStarRodItemDesc =
{
    nITKindStarRod,                         // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataStarRodItemAttributes,   // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itStarRodFallProcUpdate,                // Proc Update
    itStarRodFallProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITStarRodStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itStarRodWaitProcMap,               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itStarRodFallProcUpdate,            // Proc Update
        itStarRodFallProcMap,               // Proc Map
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
        itStarRodThrownProcUpdate,          // Proc Update
        itStarRodThrownProcMap,             // Proc Map
        itStarRodThrownProcHit,             // Proc Hit
        itStarRodThrownProcHit,             // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itStarRodThrownProcHit,             // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itStarRodFallProcUpdate,            // Proc Update
        itStarRodDroppedProcMap,            // Proc Map
        itStarRodThrownProcHit,             // Proc Hit
        itStarRodThrownProcHit,             // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itStarRodThrownProcHit,             // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    }
};

WPDesc dITStarRodWeaponStarWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindStarRodStar,                     // Weapon Kind
    &gITManagerCommonData,                  // Pointer to character's loaded files?
    &llITCommonDataStarRodWeaponAttributes, // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    itStarRodWeaponStarProcUpdate,          // Proc Update
    itStarRodWeaponStarProcMap,             // Proc Map
    itStarRodWeaponStarProcHit,             // Proc Hit
    itStarRodWeaponStarProcHit,             // Proc Shield
    itStarRodWeaponStarProcHop,             // Proc Hop
    itStarRodWeaponStarProcHit,             // Proc Set-Off
    itStarRodWeaponStarProcReflector,       // Proc Reflector
    itStarRodWeaponStarProcHit              // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itStarRodStatus
{
    nITStarRodStatusWait,
    nITStarRodStatusFall,
    nITStarRodStatusHold,
    nITStarRodStatusThrown,
    nITStarRodStatusDropped,
    nITStarRodStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80177E80
sb32 itStarRodFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITSTARROD_GRAVITY, ITSTARROD_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80177EBC
sb32 itStarRodWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itStarRodFallSetStatus);

    return FALSE;
}

// 0x80177EE4
sb32 itStarRodFallProcMap(GObj *item_gobj)
{
    itMapCheckDestroyDropped(item_gobj, ITSTARROD_MAP_REBOUND_COMMON, ITSTARROD_MAP_REBOUND_GROUND, itStarRodWaitSetStatus);

    return FALSE;
}

// 0x80177F18
void itStarRodWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITStarRodStatusDescs, nITStarRodStatusWait);
}

// 0x80177F4C
void itStarRodFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITStarRodStatusDescs, nITStarRodStatusFall);
}

// 0x80177F90
void itStarRodHoldSetStatus(GObj *item_gobj)
{
    DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(0.0F);

    itMainSetStatus(item_gobj, dITStarRodStatusDescs, nITStarRodStatusHold);
}

// 0x80177FC4
sb32 itStarRodThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITSTARROD_GRAVITY, ITSTARROD_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80178000
sb32 itStarRodThrownProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITSTARROD_MAP_REBOUND_COMMON, ITSTARROD_MAP_REBOUND_GROUND, itStarRodWaitSetStatus);
}

// 0x80178030
sb32 itStarRodThrownProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x80178058
void itStarRodThrownSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITStarRodStatusDescs, nITStarRodStatusThrown);
    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(90.0F);
}

// 0x8017809C
sb32 itStarRodDroppedProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        return itMapCheckDestroyLanding(item_gobj, ITSTARROD_MAP_REBOUND_COMMON);
    }
    else return itMapCheckDestroyDropped(item_gobj, ITSTARROD_MAP_REBOUND_COMMON, ITSTARROD_MAP_REBOUND_GROUND, itStarRodWaitSetStatus);
}

// 0x801780F0
void itStarRodDroppedSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITStarRodStatusDescs, nITStarRodStatusDropped);
    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(90.0F);
}

// 0x80178134
GObj* itStarRodMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITStarRodItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        ip->multi = ITSTARROD_AMMO_MAX;

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}

// 0x801781B0
sb32 itStarRodWeaponStarProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f pos;
    DObj *dobj;

    if (wp->weapon_vars.star.lifetime == 0)
    {
        DObjGetStruct(weapon_gobj)->flags = DOBJ_FLAG_HIDDEN;

        efManagerSparkleWhiteScaleMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }

    wp->weapon_vars.star.lifetime--;

    dobj = DObjGetStruct(weapon_gobj);

    dobj->rotate.vec.f.z += (-0.2F * wp->lr);

    if (wp->weapon_vars.star.lifetime % 2)
    {
        pos.x = DObjGetStruct(weapon_gobj)->translate.vec.f.x;
        pos.y = syUtilsRandIntRange(250) + (DObjGetStruct(weapon_gobj)->translate.vec.f.y - 125.0F);
        pos.z = 0.0F;

        efManagerStarRodSparkMakeEffect(&pos, wp->lr * -1.0F);
    }
    return FALSE;
}

// 0x801782D4
sb32 itStarRodWeaponStarProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        efManagerStarSplashMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->lr);

        func_800269C0_275C0(nSYAudioFGMStarMapCollide);

        return TRUE;
    }
    else return FALSE;
}

// 0x8017832C
sb32 itStarRodWeaponStarProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerStarSplashMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->lr);

    return TRUE;
}

// 0x8017835C
sb32 itStarRodWeaponStarProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    if (wp->physics.vel_air.x > 0.0F)
    {
        wp->lr = +1;
    }
    else wp->lr = -1;

    return FALSE;
}

// 0x80178404
sb32 itStarRodWeaponStarProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    wp->lr = -wp->lr;

    return FALSE;
}

// 0x80178474
GObj* itStarRodWeaponStarMakeWeapon(GObj *fighter_gobj, Vec3f *pos, ub8 is_smash)
{
    GObj *weapon_gobj;
    DObj *dobj;
    WPStruct *wp;

    if (is_smash == TRUE)
    {
        dITStarRodWeaponStarWeaponDesc.o_attributes = (intptr_t)&llITCommonDataStarRodSmashWeaponAttributes; // Set attribute data on smash input - Linker thing
    }
    weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dITStarRodWeaponStarWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    dobj = DObjGetStruct(weapon_gobj);
    wp = wpGetStruct(weapon_gobj);

    wp->physics.vel_air.x = ((!(is_smash)) ? ITSTARROD_AMMO_TILTVEL_X : ITSTARROD_AMMO_SMASH_VEL_X) * wp->lr;

    wp->weapon_vars.star.lifetime = (!(is_smash)) ? ITSTARROD_AMMO_TILT_LIFETIME : ITSTARROD_AMMO_SMASH_LIFETIME; // Why float lol

    gcAddXObjForDObjFixed(dobj, 0x2E, 0);

    dobj->translate.vec.f = *pos;
    dobj->translate.vec.f.z = 0.0F;

    return weapon_gobj;
}

// 0x80178594
void itStarRodMakeStar(GObj *fighter_gobj, Vec3f *pos, ub8 is_smash)
{
    ITStruct *ip = itGetStruct(ftGetStruct(fighter_gobj)->item_gobj);

    itStarRodWeaponStarMakeWeapon(fighter_gobj, pos, is_smash);

    ip->multi--;
}
