#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPPikachuThunderHeadWeaponDesc =
{
    0x02,                                      // Render flags?
    nWPKindThunderHead,                        // Weapon Kind
    &gFTDataPikachuMain,                       // Pointer to character's loaded files?
    &llPikachuMainThunderHeadWeaponAttributes, // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,          // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0                                    // ???
    },

    wpPikachuThunderHeadProcUpdate,             // Proc Update
    wpPikachuThunderHeadProcMap,                // Proc Map
    NULL,                                       // Proc Hit
    NULL,                                       // Proc Shield
    NULL,                                       // Proc Hop
    NULL,                                       // Proc Set-Off
    NULL,                                       // Proc Reflector
    NULL                                        // Proc Absorb
};

WPDesc dWPPikachuThunderTrailWeaponDesc =
{
    0x02,                                       // Render flags?
    nWPKindThunderTrail,                       // Weapon Kind
    &gFTDataPikachuMain,                        // Pointer to character's loaded files?
    &llPikachuMainThunderTrailWeaponAttributes,    // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,          // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0                                       // ???
    },

    wpPikachuThunderTrailProcUpdate,            // Proc Update
    NULL,                                       // Proc Map
    wpPikachuThunderTrailProcHit,               // Proc Hit
    wpPikachuThunderTrailProcHit,               // Proc Shield
    NULL,                                       // Proc Hop
    wpPikachuThunderTrailProcHit,               // Proc Set-Off
    NULL,                                       // Proc Reflector
    wpPikachuThunderTrailProcHit                // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016A640
void wpPikachuThunderHeadSetDestroy(GObj *weapon_gobj, sb32 is_destroy)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (!(wp->weapon_vars.thunder.thunder_state & nWPPikachuThunderStatusDestroy))
    {
        FTStruct *fp = ftGetStruct(wp->owner_gobj);

        if (fp->player_num == wp->player_num) // Check number of player that spawned Thunder
        {
            fp->passive_vars.pikachu.is_thunder_destroy |= is_destroy;
        }
    }
}

// 0x8016A680
void wpPikachuThunderHeadMakeTrailEffect(GObj *weapon_gobj, s32 arg1)
{
    s32 unused;
    Vec3f pos;

    pos.x = DObjGetStruct(weapon_gobj)->translate.vec.f.x;
    pos.y = DObjGetStruct(weapon_gobj)->translate.vec.f.y;
    pos.z = 0.0F;

    if (arg1 == 3)
    {
        efManagerPikachuThunderTrailMakeEffect(&pos, 10, 3);
    }
    else if (arg1 == -1)
    {
        efManagerPikachuThunderTrailMakeEffect(&pos, 6, 0);
    }
    else wpPikachuThunderTrailMakeWeapon(weapon_gobj, &pos);
}

// 0x8016A700
sb32 wpPikachuThunderHeadProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->weapon_vars.thunder.thunder_state == nWPPikachuThunderStatusCollide)
    {
        wpPikachuThunderHeadMakeTrailEffect(weapon_gobj, 3);

        return TRUE;
    }
    else if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);
        wpPikachuThunderHeadSetDestroy(weapon_gobj, TRUE);
        wpPikachuThunderHeadMakeTrailEffect(weapon_gobj, 3);

        return TRUE;
    }
    else wpPikachuThunderHeadMakeTrailEffect(weapon_gobj, 0);

    return FALSE;
}

// 0x8016A794
sb32 wpPikachuThunderHeadProcMap(GObj *weapon_gobj)
{
    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        wpPikachuThunderHeadSetDestroy(weapon_gobj, TRUE);
        efManagerQuakeMakeEffect(1);
        efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

        return TRUE;
    }
    return FALSE;
}

// 0x8016A7E8
sb32 wpPikachuThunderHeadProcDead(GObj *weapon_gobj)
{
    wpPikachuThunderHeadSetDestroy(weapon_gobj, TRUE);

    return TRUE;
}

// 0x8016A80C
GObj* wpPikachuThunderHeadMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel)
{
    s32 unused;
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPPikachuThunderHeadWeaponDesc, pos, WEAPON_FLAG_PARENT_FIGHTER);
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->proc_dead = wpPikachuThunderHeadProcDead;

    wp->lifetime = WPPIKACHUTHUNDER_SPAWN_LIFETIME;

    wp->physics.vel_air = *vel;

    wp->attack_coll.attack_state = nGMAttackStateOff;
    wp->weapon_vars.thunder.thunder_state = nWPPikachuThunderStatusActive;

    wp->group_id = wpManagerGetGroupID();

    DObjGetStruct(weapon_gobj)->mobj->texture_id_curr = 3;

    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 0.5F;
    DObjGetStruct(weapon_gobj)->scale.vec.f.y = 0.5F;
    DObjGetStruct(weapon_gobj)->scale.vec.f.z = 0.5F;

    return weapon_gobj;
}

// 0x8016A8D8
sb32 wpPikachuThunderTrailProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        return TRUE;
    }
    else if (wp->lifetime < WPPIKACHUTHUNDER_EXPIRE)
    {
        wpPikachuThunderHeadMakeTrailEffect(weapon_gobj, -1);

        return TRUE;
    }
    else DObjGetStruct(weapon_gobj)->mobj->texture_id_curr = syUtilsRandIntRange(WPPIKACHUTHUNDER_TEXTURES_NUM - 1);

    return FALSE;
}

// 0x8016A950
sb32 wpPikachuThunderTrailProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerImpactShockMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->attack_coll.damage);

    return FALSE;
}

// 0x8016A980
GObj* wpPikachuThunderTrailMakeWeapon(GObj *weapon_gobj, Vec3f *pos)
{
    s32 unused[2];
    WPStruct *spawn_wp = wpGetStruct(weapon_gobj);
    GObj *trail_gobj = wpManagerMakeWeapon(weapon_gobj, &dWPPikachuThunderTrailWeaponDesc, pos, WEAPON_FLAG_PARENT_WEAPON);
    WPStruct *trail_wp;
    s32 i;

    if (trail_gobj == NULL)
    {
        return NULL;
    }
    trail_wp = wpGetStruct(trail_gobj);

    trail_wp->lifetime = WPPIKACHUTHUNDER_TRAIL_LIFETIME;
    trail_wp->group_id = spawn_wp->group_id;

    for (i = 0; i < ARRAY_COUNT(spawn_wp->attack_coll.attack_records); i++)
    {
        trail_wp->attack_coll.attack_records[i] = spawn_wp->attack_coll.attack_records[i];
    }
    DObjGetStruct(trail_gobj)->scale.vec.f.x = 0.5F;
    DObjGetStruct(trail_gobj)->scale.vec.f.y = 0.5F;
    DObjGetStruct(trail_gobj)->scale.vec.f.z = 0.5F;

    return trail_gobj;
}
