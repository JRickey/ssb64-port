#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018ACA0
ITDesc dITNyarsItemDesc =
{
    nITKindNyars,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataNyarsItemAttributes,     // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itNyarsCommonProcUpdate,                // Proc Update
    itNyarsCommonProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018ACD4
ITStatusDesc dITNyarsStatusDescs[/* */] =
{
    // Status 0 (Neutral Attack)
    {
        itNyarsAttackProcUpdate,            // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// 0x8018ACF4
WPDesc dITNyarsWeaponCoinWeaponDesc =
{
    0x01,                                     // Render flags?
    nWPKindNyarsCoin,                         // Weapon Kind
    &gITManagerCommonData,                    // Pointer to character's loaded files?
    &llITCommonDataNyarsCoinWeaponAttributes, // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindNull,                  // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    itNyarsWeaponCoinProcUpdate,            // Proc Update
    NULL,                                   // Proc Map
    itNyarsWeaponCoinProcHit,               // Proc Hit
    itNyarsWeaponCoinProcHit,               // Proc Shield
    itNyarsWeaponCoinProcHop,               // Proc Hop
    itNyarsWeaponCoinProcHit,               // Proc Set-Off
    itNyarsWeaponCoinProcReflector,         // Proc Reflector
    itNyarsWeaponCoinProcHit                // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itNyarsStatus
{
    nITNyarsStatusAttack,
    nITNyarsStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017EEB0
sb32 itNyarsAttackProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        return TRUE;
    }
    if (ip->multi == ip->item_vars.nyars.coin_spawn_wait)
    {
        itNyarsAttackMakeCoin(item_gobj, ip->item_vars.nyars.coin_rotate_step * ITNYARS_COIN_ANGLE_STEP);

        ip->item_vars.nyars.coin_rotate_step++;
        ip->item_vars.nyars.coin_spawn_wait = ip->multi - ITNYARS_COIN_SPAWN_WAIT;

        func_800269C0_275C0(nSYAudioFGMNyarsCoin);
    }
    if (ip->item_vars.nyars.model_rotate_wait == 0)
    {
        dobj->rotate.vec.f.y += F_CST_DTOR32(180.0F);

        ip->item_vars.nyars.model_rotate_wait = ITNYARS_MODEL_ROTATE_WAIT;
    }
    ip->item_vars.nyars.model_rotate_wait--;

    ip->multi--;

    return FALSE;
}

// 0x8017EFA0
void itNyarsAttackInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = ITNYARS_LIFETIME;

    ip->item_vars.nyars.coin_spawn_wait = ip->multi - (ITNYARS_COIN_SPAWN_WAIT / 2);
    ip->item_vars.nyars.coin_rotate_step = 0;
    ip->item_vars.nyars.model_rotate_wait = ITNYARS_MODEL_ROTATE_WAIT;
}

// 0x8017EFC4
void itNyarsAttackSetStatus(GObj *item_gobj)
{
    itNyarsAttackInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITNyarsStatusDescs, nITNyarsStatusAttack);
}

// 0x8017EFF8
sb32 itNyarsCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        ip->physics.vel_air.x = ip->physics.vel_air.y = 0.0F;

        itNyarsAttackSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017F04C
sb32 itNyarsCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x8017F08C
GObj* itNyarsMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITNyarsItemDesc, pos, vel, flags);
    DObj *dobj;
    ITStruct *ip;

    if (item_gobj != NULL)
    {
        dobj = DObjGetStruct(item_gobj);

        gcAddXObjForDObjFixed(dobj, 0x48, 0);

        dobj->translate.vec.f = *pos;

        ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataNyarsAnimJoint), 0.0F);
    }
    return item_gobj;
}

// 0x8017F17C
sb32 itNyarsWeaponCoinProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->weapon_vars.coin.lifetime == 0)
    {
        return TRUE;
    }
    wp->weapon_vars.coin.lifetime--;

    return FALSE;
}

// 0x8017F1A4
sb32 itNyarsWeaponCoinProcHit(GObj *weapon_gobj)
{
    efManagerDamageCoinMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x8017F1CC
sb32 itNyarsWeaponCoinProcHop(GObj *weapon_gobj)
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

// 0x8017F274
sb32 itNyarsWeaponCoinProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    wp->lr = -wp->lr;

    return FALSE;
}

// 0x8017F2E4
GObj* itNyarsWeaponCoinMakeWeapon(GObj *item_gobj, u8 coin_number, f32 rotate_angle)
{
    WPStruct *wp;
    GObj *weapon_gobj = wpManagerMakeWeapon(item_gobj, &dITNyarsWeaponCoinWeaponDesc, &DObjGetStruct(item_gobj)->translate.vec.f, WEAPON_FLAG_PARENT_ITEM);
    DObj *dobj;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->weapon_vars.coin.lifetime = ITNYARS_COIN_LIFETIME;

    wp->physics.vel_air.y = wp->physics.vel_air.z = 0.0F;
    wp->physics.vel_air.x = ITNYARS_COIN_VEL_X;

    syVectorRotate3D(&wp->physics.vel_air, SYVECTOR_AXIS_Z, F_CLC_DTOR32((coin_number * ITNYARS_COIN_ANGLE_DIFF) + rotate_angle));

    dobj = DObjGetStruct(weapon_gobj);

    gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyRSca, 0);
    gcAddXObjForDObjFixed(dobj, 0x46, 0);

    dobj->translate.vec.f = DObjGetStruct(item_gobj)->translate.vec.f;

    return weapon_gobj;
}

// 0x8017F408
void itNyarsAttackMakeCoin(GObj *item_gobj, f32 angle)
{
    s32 coin_count;

    for (coin_count = 0; coin_count < ITNYARS_COIN_SPAWN_MAX; coin_count++)
    {
        itNyarsWeaponCoinMakeWeapon(item_gobj, coin_count, angle);
    }
}
