#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018AA90
ITDesc dITIwarkItemDesc =
{
    nITKindIwark,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataWarkItemAttributes,      // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                  // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itIwarkCommonProcUpdate,                // Proc Update
    itIwarkCommonProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018AAC4
ITStatusDesc dITIwarkStatusDescs[/* */] = 
{
    // Status 0 (Neutral Fly)
    {
        itIwarkFlyProcUpdate,               // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Neutral Attack)
    {
        itIwarkAttackProcUpdate,            // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// 0x8018AB04
WPDesc dITIwarkWeaponRockWeaponDesc =
{
    0x01,                                   // Render flags?
    nWPKindIwarkRock,                       // Weapon Kind
    &gITManagerCommonData,                  // Pointer to weapon's loaded files?
    &llITCommonDataWarkRockWeaponAttributes,// Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindNull,                  // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    itIwarkWeaponRockProcUpdate,            // Proc Update
    itIwarkWeaponRockProcMap,               // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    itIwarkWeaponRockProcHop,               // Proc Hop
    NULL,                                   // Proc Set-Off
    itIwarkWeaponRockProcReflector,         // Proc Reflector
    NULL                                    // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itIwarkStatus
{
    nITIwarkStatusFly,
    nITIwarkStatusAttack,
    nITIwarkStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017D740
void itIwarkAttackUpdateRock(GObj *iwark_gobj)
{
    ITStruct *ip = itGetStruct(iwark_gobj);
    DObj *dobj = DObjGetStruct(iwark_gobj);

    if (ip->item_vars.iwark.rock_spawn_wait <= 0)
    {
        WPStruct *wp;
        GObj *rock_gobj;
        Vec3f pos = dobj->translate.vec.f;

        pos.x += (ITIWARK_ROCK_SPAWN_OFF_X_MUL * syUtilsRandFloat()) + ITIWARK_ROCK_SPAWN_OFF_X_ADD;

        rock_gobj = itIwarkWeaponRockMakeWeapon(iwark_gobj, &pos, syUtilsRandIntRange(WPIWARK_ROCK_RANDOM_VEL_MAX));

        if (rock_gobj != NULL)
        {
            wp = wpGetStruct(rock_gobj);

        #if !defined (DAIRANTOU_OPT0)
            wp->weapon_vars.rock.unk_0xC = ip->item_vars.iwark.rock_spawn_max - ip->item_vars.iwark.rock_spawn_remain;
        #endif

            ip->item_vars.iwark.rock_spawn_remain--;

        #if !defined (DAIRANTOU_OPT0)
            if (ip->item_vars.iwark.rock_spawn_remain == 0)
            {
                wp->weapon_vars.rock.unk_0xC = -1;
            }
        #endif
            ip->item_vars.iwark.rock_spawn_wait = syUtilsRandIntRange(ITIWARK_ROCK_SPAWN_WAIT_MAX) + ITIWARK_ROCK_SPAWN_WAIT_MIN;
        }
    }
}

// 0x8017D820
sb32 itIwarkAttackProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
#if defined(REGION_US)
    f32 pos_y = gMPCollisionGroundData->map_bound_top - ITIWARK_FLY_STOP_Y;

    if (dobj->translate.vec.f.y >= pos_y)
    {
        dobj->translate.vec.f.y = pos_y;
#else
    if (dobj->translate.vec.f.y >= gMPCollisionGroundData->map_bound_top - ITIWARK_FLY_STOP_Y)
    {
#endif

        ip->physics.vel_air.y = 0.0F;

        if (ip->item_vars.iwark.rock_spawn_remain != 0)
        {
            itIwarkAttackUpdateRock(item_gobj);
        }
        else if (ip->item_vars.iwark.rock_spawn_count == ip->item_vars.iwark.rock_spawn_max)
        {
            return TRUE;
        }
        if ((ip->item_vars.iwark.rumble_wait == 0) && (ip->item_vars.iwark.rumble_frame != 0))
        {
            efManagerQuakeMakeEffect(0);

            ip->item_vars.iwark.rumble_wait = ITIWARK_ROCK_RUMBLE_WAIT;
        }
        if (ip->item_vars.iwark.rumble_frame != 0)
        {
            ip->item_vars.iwark.rumble_wait--;
        }
        ip->item_vars.iwark.rock_spawn_wait--;
    }
    if (ip->multi == ITIWARK_MODEL_ROTATE_WAIT)
    {
        dobj->rotate.vec.f.y += F_CST_DTOR32(180.0F);

        ip->multi = 0;
    }
    ip->multi++;

    return FALSE;
}

// 0x8017D948
void itIwarkAttackInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    Gfx *dl;
    Vec3f pos;

#if defined(REGION_US)
    ip->ga = nMPKineticsAir;
#endif

    ip->physics.vel_air.y = ITIWARK_FLY_VEL_Y;

    ip->item_vars.iwark.rock_spawn_remain = syUtilsRandIntRange(ITIWARK_ROCK_SPAWN_COUNT_RANDOM) + ITIWARK_ROCK_SPAWN_COUNT_MIN;
    ip->item_vars.iwark.rock_spawn_max = ip->item_vars.iwark.rock_spawn_remain;
    ip->item_vars.iwark.rock_spawn_count = 0;
    ip->item_vars.iwark.rock_spawn_wait = 0;
    ip->item_vars.iwark.rumble_frame = 0;
    ip->item_vars.iwark.rumble_wait = 0;

    ip->multi = 0;

    pos = dobj->translate.vec.f;

    if (ip->kind == nITKindIwark)
    {
        dobj->dl = dl = (Gfx*) itGetPData(ip, &llITCommonDataWarkDataStart, &llITCommonDataWarkDisplayList);

        pos.y += ITIWARK_IWARK_ADD_POS_Y;
    }
    else pos.y += ITIWARK_OTHER_ADD_POS_Y;

    efManagerDustHeavyDoubleMakeEffect(&pos, -1, 1.0F);

    if (ip->kind == nITKindIwark)
    {
        func_800269C0_275C0(nSYAudioVoiceMBallIwarkAppear);
    }
}

// 0x8017DA60
void itIwarkAttackSetStatus(GObj *item_gobj)
{
    itIwarkAttackInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITIwarkStatusDescs, nITIwarkStatusAttack);
}

// 0x8017DA94
sb32 itIwarkFlyProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        itIwarkAttackSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017DAD8
void itIwarkFlySetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = ITIWARK_FLY_WAIT;

    ip->physics.vel_air.x = ip->physics.vel_air.y = 0.0F;

    itMainSetStatus(item_gobj, dITIwarkStatusDescs, nITIwarkStatusFly);
}

// 0x8017DB18
sb32 itIwarkCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        itIwarkFlySetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017DB5C
sb32 itIwarkCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;

        itMapSetGround(ip);
    }
    return FALSE;
}

// 0x8017DBA0
GObj* itIwarkMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITIwarkItemDesc, pos, vel, flags);
    DObj *dobj;
    ITStruct *ip;

    if (item_gobj != NULL)
    {
        itMainClearOwnerStats(item_gobj);

        dobj = DObjGetStruct(item_gobj);

        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
        gcAddXObjForDObjFixed(dobj, 0x48, 0);

        dobj->translate.vec.f = *pos;

        ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->attack_coll.interact_mask = GMHITCOLLISION_FLAG_FIGHTER;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataWarkDataStart), 0.0F);
    }
    return item_gobj;
}

// 0x8017DCAC
sb32 itIwarkWeaponRockProcDead(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    ITStruct *ip = itGetStruct(wp->weapon_vars.rock.owner_gobj);

    ip->item_vars.iwark.rock_spawn_count++;

    return TRUE;
}

// 0x8017DCCC
sb32 itIwarkWeaponRockProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    DObj *dobj;

    wpMainApplyGravityClampTVel(wp, WPIWARK_ROCK_GRAVITY, WPIWARK_ROCK_TVEL);

    dobj = DObjGetStruct(weapon_gobj);

    dobj->rotate.vec.f.z += WPIWARK_ROCK_ROTATE_STEP;

    return FALSE;
}

// 0x8017DD18
sb32 itIwarkWeaponRockProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    ITStruct *ip = itGetStruct(wp->weapon_vars.rock.owner_gobj);
    MPCollData *coll_data = &wp->coll_data;
    Vec3f pos = DObjGetStruct(weapon_gobj)->translate.vec.f;
    s32 line_id = wp->weapon_vars.rock.floor_line_id;

    wpMapTestAllCheckCollEnd(weapon_gobj);

    if (coll_data->mask_curr & MAP_FLAG_FLOOR)
    {
        if (line_id != coll_data->floor_line_id)
        {
            lbCommonReflect2D(&wp->physics.vel_air, &coll_data->floor_angle);
            lbCommonScale2D(&wp->physics.vel_air, WPIWARK_ROCK_COLLIDE_MUL_VEL_Y);

            wp->weapon_vars.rock.floor_line_id = coll_data->floor_line_id;

            func_800269C0_275C0(nSYAudioFGMIwarkRockMake);

            pos.y += WPIWARK_ROCK_COLLIDE_ADD_VEL_Y;

            efManagerDustLightMakeEffect(&pos, wp->lr, 1.0F);

            wp->lr = -wp->lr;

            ip->item_vars.iwark.rumble_frame++;
        }
    }
    return FALSE;
}

// 0x8017DE10
sb32 itIwarkWeaponRockProcHop(GObj *weapon_gobj)
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

// 0x8017DEB8
sb32 itIwarkWeaponRockProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    wp->lr = -wp->lr;

    return FALSE;
}

// 0x8017DF28
GObj* itIwarkWeaponRockMakeWeapon(GObj *parent_gobj, Vec3f *pos, u8 random)
{
    u32 random32;
    GObj *weapon_gobj = wpManagerMakeWeapon(parent_gobj, &dITIwarkWeaponRockWeaponDesc, pos, WEAPON_FLAG_PARENT_ITEM);
    DObj *dobj;
    f32 vel_y;
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->weapon_vars.rock.floor_line_id = -1;

    random32 = random;

    if (random32 == 0)
    {
        wp->physics.vel_air.y = WPIWARK_ROCK_VEL_Y_START_A;
    }
    else wp->physics.vel_air.y = vel_y = (random32 == 1) ? WPIWARK_ROCK_VEL_Y_START_B : WPIWARK_ROCK_VEL_Y_START_C;

    if (syUtilsRandIntRange(2) == 0)
    {
        wp->lr = -1;
    }
    else wp->lr = +1;

    dobj = DObjGetStruct(weapon_gobj);

    gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
    gcAddXObjForDObjFixed(dobj, 0x46, 0);

    dobj->translate.vec.f = *pos;

    dobj->child->mobj->texture_id_curr = random;

    wp->weapon_vars.rock.owner_gobj = parent_gobj;

    wp->is_hitlag_victim = TRUE;

    wp->proc_dead = itIwarkWeaponRockProcDead;

    return weapon_gobj;
}
