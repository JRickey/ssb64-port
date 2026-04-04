#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITLGunItemDesc =
{
    nITKindLGun,                            // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataLGunItemAttributes,      // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itLGunFallProcUpdate,                   // Proc Update
    itLGunFallProcMap,                      // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITLGunStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itLGunWaitProcMap,                  // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itLGunFallProcUpdate,               // Proc Update
        itLGunFallProcMap,                  // Proc Map
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
        itLGunFallProcUpdate,               // Proc Update
        itLGunThrownProcMap,                // Proc Map
        itLGunCommonProcHit,                // Proc Hit
        itLGunCommonProcHit,                // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itLGunCommonProcHit,                // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itLGunFallProcUpdate,               // Proc Update
        itLGunDroppedProcMap,               // Proc Map
        itLGunCommonProcHit,                // Proc Hit
        itLGunCommonProcHit,                // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itLGunCommonProcHit,                // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    }
};

WPDesc dITLGunAmmoWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindLGunAmmo,                        // Weapon Kind
    &gITManagerCommonData,                  // Pointer to character's loaded files?
    &llITCommonDataLGunAmmoWeaponAttributes,// Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    itLGunWeaponAmmoProcUpdate,             // Proc Update
    itLGunWeaponAmmoProcMap,                // Proc Map
    itLGunWeaponAmmoProcHit,                // Proc Hit
    itLGunWeaponAmmoProcHit,                // Proc Shield
    itLGunWeaponAmmoProcHop,                // Proc Hop
    itLGunWeaponAmmoProcHit,                // Proc Set-Off
    itLGunWeaponAmmoProcReflector,          // Proc Reflector
    itLGunWeaponAmmoProcHit                 // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itLGunStatus
{
    nITLGunStatusWait,
    nITLGunStatusFall,
    nITLGunStatusHold,
    nITLGunStatusThrown,
    nITLGunStatusDropped,
    nITLGunStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801754F0
sb32 itLGunFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITLGUN_GRAVITY, ITLGUN_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80175528
sb32 itLGunWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itLGunFallSetStatus);

    return FALSE;
}

// 0x80175550
sb32 itLGunFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITLGUN_MAP_REBOUND_COMMON, ITLGUN_MAP_REBOUND_GROUND, itLGunWaitSetStatus);
}

// 0x80175584
void itLGunWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITLGunStatusDescs, nITLGunStatusWait);
}

// 0x801755B8
void itLGunFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITLGunStatusDescs, nITLGunStatusFall);
}

// 0x801755FC
void itLGunHoldSetStatus(GObj *item_gobj)
{
    DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(0.0F);

    itMainSetStatus(item_gobj, dITLGunStatusDescs, nITLGunStatusHold);
}

// 0x80175630
sb32 itLGunThrownProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        return itMapCheckDestroyLanding(item_gobj, ITLGUN_MAP_REBOUND_COMMON);
    }
    else return itMapCheckDestroyDropped(item_gobj, ITLGUN_MAP_REBOUND_COMMON, ITLGUN_MAP_REBOUND_GROUND, itLGunWaitSetStatus);
}

// 0x80175684
sb32 itLGunCommonProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x801756AC
void itLGunThrownSetStatus(GObj *item_gobj)
{
    s32 lr = ftGetStruct(itGetStruct(item_gobj)->owner_gobj)->lr;

    itMainSetStatus(item_gobj, dITLGunStatusDescs, nITLGunStatusThrown);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = (lr == -1) ? F_CST_DTOR32(-90.0F) : F_CST_DTOR32(90.0F);
}

// 0x8017572C
sb32 itLGunDroppedProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        return itMapCheckDestroyLanding(item_gobj, ITLGUN_MAP_REBOUND_COMMON);
    }
    else return itMapCheckDestroyDropped(item_gobj, ITLGUN_MAP_REBOUND_COMMON, ITLGUN_MAP_REBOUND_GROUND, itLGunWaitSetStatus);
}

// 0x80175780
void itLGunDroppedSetStatus(GObj *item_gobj)
{
    s32 lr = ftGetStruct(itGetStruct(item_gobj)->owner_gobj)->lr;

    itMainSetStatus(item_gobj, dITLGunStatusDescs, nITLGunStatusDropped);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = (lr == -1) ? F_CST_DTOR32(-90.0F) : F_CST_DTOR32(90.0F);
}

// 0x80175800
GObj* itLGunMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITLGunItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        ip->multi = ITLGUN_AMMO_MAX;

        DObjGetStruct(item_gobj)->rotate.vec.f.y = ((syUtilsRandUShort() % 2) != 0) ? F_CST_DTOR32(90.0F) : F_CST_DTOR32(-90.0F);

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}

// Ray Gun ammo functions
sb32 itLGunWeaponAmmoProcUpdate(GObj *weapon_gobj)
{
    DObj *dobj = DObjGetStruct(weapon_gobj);

    if (dobj->scale.vec.f.x < ITLGUN_AMMO_CLAMP_SCALE_X)
    {
        dobj->scale.vec.f.x += ITLGUN_AMMO_STEP_SCALE_X;

    #if !defined (DAIRANTOU_OPT0)
        dobj = DObjGetStruct(weapon_gobj); // Y tho lol
    #endif

        if (dobj->scale.vec.f.x > ITLGUN_AMMO_CLAMP_SCALE_X)
        {
            dobj->scale.vec.f.x = ITLGUN_AMMO_CLAMP_SCALE_X;
        }
    }
    return FALSE;
}

// 0x80175914
sb32 itLGunWeaponAmmoProcMap(GObj *weapon_gobj)
{
    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else return FALSE;
}

// 0x80175958
sb32 itLGunWeaponAmmoProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerImpactShockMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->attack_coll.damage);

    return TRUE;
}

// 0x80175988
sb32 itLGunWeaponAmmoProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    return FALSE;
}

// 0x80175A00
sb32 itLGunWeaponAmmoProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    return FALSE;
}

// 0x80175A60
GObj* itLGunWeaponAmmoMakeWeapon(GObj *fighter_gobj, Vec3f *pos)
{
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dITLGunAmmoWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->physics.vel_air.x = wp->lr * ITLGUN_AMMO_VEL_X;

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);

    return weapon_gobj;
}

// 0x80175AD8
void itLGunMakeAmmo(GObj *fighter_gobj, Vec3f *pos)
{
    ITStruct *ip = itGetStruct(ftGetStruct(fighter_gobj)->item_gobj);

    itLGunWeaponAmmoMakeWeapon(fighter_gobj, pos);

    ip->multi--;
}
