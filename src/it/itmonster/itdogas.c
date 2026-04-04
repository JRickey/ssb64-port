#include <it/item.h>
#include <wp/weapon.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018B2C0
ITDesc dITDogasItemDesc =
{
    nITKindDogas,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataDogasItemAttributes,     // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                   // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0,                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itDogasCommonProcUpdate,                // Proc Update
    itDogasCommonProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x80182BF4
ITStatusDesc dITDogasStatusDescs[/* */] =
{
    // Status 0 (Neutral Active)
    {
        itDogasAttackProcUpdate,            // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Neutral Disappear)
    {
        itDogasDisappearProcUpdate,         // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// 0x8018B334
WPDesc dITDogasWeaponSmogWeaponDesc = 
{
    0x03,                                   // Render flags?
    nWPKindDogasSmog,                       // Weapon Kind
    &gITManagerCommonData,                    // Pointer to weapon's loaded files?
    &llITCommonDataDogasSmogWeaponAttributes,    // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,             // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0,                                  // ???
    },

    itDogasWeaponSmogProcUpdate,            // Proc Update
    NULL,                                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itDogasStatus
{
    itDogasStatusAttack,
    itDogasStatusDisappear,
    itDogasStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80182C80
sb32 itDogasDisappearProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        return TRUE;
    }
    ip->multi--;

    return FALSE;
}

// 0x80182CA8
void itDogasDisappearSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = ITDOGAS_DESPAWN_WAIT;

    itMainSetStatus(item_gobj, dITDogasStatusDescs, itDogasStatusDisappear);
}

// 0x80182CDC
void itDogasAttackUpdateSmog(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    Vec3f pos;
    Vec3f vel;

    if (ip->item_vars.dogas.smog_spawn_wait <= 0)
    {
        vel.x = ITDOGAS_SMOG_VEL;
        vel.y = ITDOGAS_SMOG_VEL;
        vel.z = 0.0F;

        pos = dobj->translate.vec.f;

        pos.x += (syUtilsRandFloat() * ITDOGAS_SMOG_MUL_OFF_X) - ITDOGAS_SMOG_SUB_OFF_X;
        pos.y += (syUtilsRandFloat() * ITDOGAS_SMOG_MUL_OFF_Y) - ITDGOAS_SMOG_SUB_OFF_Y;

        if (pos.x < dobj->translate.vec.f.x)
        {
            vel.x = -vel.x;
        }
        if (pos.y < dobj->translate.vec.f.y)
        {
            vel.y = -vel.y;
        }
        itDogasWeaponSmogMakeWeapon(item_gobj, &pos, &vel);
        func_800269C0_275C0(nSYAudioFGMDogasSmog);

        ip->item_vars.dogas.smog_spawn_wait = ITDOGAS_SMOG_SPAWN_WAIT;

        ip->multi--;
    }
}

// 0x80182E1C
sb32 itDogasAttackProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itDogasAttackUpdateSmog(item_gobj);

    if (ip->multi == 0)
    {
        itDogasDisappearSetStatus(item_gobj);

        return FALSE;
    }
    ip->item_vars.dogas.smog_spawn_wait--;

    return FALSE;
}

// 0x80182E78
void itDogasAttackInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    ip->multi = ITDOGAS_SMOG_SPAWN_COUNT;

    ip->item_vars.dogas.smog_spawn_wait = 0;

    if (ip->kind == nITKindDogas)
    {
        ip->item_vars.dogas.pos = dobj->translate.vec.f;

        gcAddDObjAnimJoint(dobj->child, itGetPData(ip, &llITCommonDataDogasDataStart, &llITCommonDataDogasAnimJoint), 0.0F);

        gcPlayAnimAll(item_gobj);
        func_800269C0_275C0(nSYAudioVoiceMBallDogasAppear);
    }
}

// 0x80182F0C
void itDogasAttackSetStatus(GObj *item_gobj)
{
    itDogasAttackInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITDogasStatusDescs, itDogasStatusAttack);
}

// 0x80182F40
sb32 itDogasCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {   
        ip->physics.vel_air.x = ip->physics.vel_air.y = 0.0F;

        itDogasAttackSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x80182F94
sb32 itDogasCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x80182FD4
GObj* itDogasMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITDogasItemDesc, pos, vel, flags);
    DObj *dobj;
    ITStruct *ip;

    if (item_gobj != NULL)
    {
        dobj = DObjGetStruct(item_gobj);

        gcAddXObjForDObjFixed(dobj, 0x28, 0);
        gcAddXObjForDObjFixed(dobj->child, nGCMatrixKindTraRotRpyRSca, 0);

        dobj->translate.vec.f = *pos;

        ip = itGetStruct(item_gobj);

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = 0.0F;
        ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        gcAddDObjAnimJoint(dobj->child, itGetMonsterAnimNode(ip, &llITCommonDataDogasDataStart), 0.0F);
    }
    return item_gobj;
}

// 0x801830DC
sb32 itDogasWeaponSmogProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    DObj *dobj = DObjGetStruct(weapon_gobj)->child;

    wp->attack_coll.size = dobj->scale.vec.f.x * wp->weapon_vars.smog.attr->size;

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80183144
GObj* itDogasWeaponSmogMakeWeapon(GObj *item_gobj, Vec3f *pos, Vec3f *vel)
{
    WPDesc *weapon_desc = &dITDogasWeaponSmogWeaponDesc;
    GObj *weapon_gobj = wpManagerMakeWeapon(item_gobj, &dITDogasWeaponSmogWeaponDesc, pos, WEAPON_FLAG_PARENT_ITEM);
    DObj *dobj;
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lifetime = ITDOGAS_SMOG_LIFETIME;

    wp->weapon_vars.smog.attr = (WPAttributes*) ((uintptr_t)*weapon_desc->p_weapon + (intptr_t)weapon_desc->o_attributes); // Dude I had a stroke trying to match this

    dobj = DObjGetStruct(weapon_gobj);

    wp->physics.vel_air = *vel;

    gcAddXObjForDObjFixed(dobj->child, 0x2C, 0);

    dobj->translate.vec.f = *pos;

    return weapon_gobj;
}
