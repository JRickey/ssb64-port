#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPKirbyCutterWeaponDesc =
{
    0x03,                                  // Render flags?
    nWPKindCutter,                         // Weapon Kind
    &gFTDataKirbyMain,                     // Pointer to character's loaded files?
    &llKirbyMainCutterWeaponAttributes,    // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,      // Main matrix transformations
        nGCMatrixKindNull,               // Secondary matrix transformations?
        0                                   // ???
    },

    wpKirbyCutterProcUpdate,                // Proc Update
    wpKirbyCutterProcMap,                   // Proc Map
    wpKirbyCutterProcHit,                   // Proc Hit
    wpKirbyCutterProcShield,                // Proc Shield
    NULL,                                   // Proc Hop
    wpKirbyCutterProcSetOff,                // Proc Set-Off
    wpKirbyCutterProcReflector,             // Proc Reflector
    wpKirbyCutterProcShield                 // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016BC50
sb32 wpKirbyCutterProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else if (wp->ga == nMPKineticsGround)
    {
        DObjGetStruct(weapon_gobj)->rotate.vec.f.z = -syUtilsArcTan2(wp->coll_data.floor_angle.x, wp->coll_data.floor_angle.y);
    }
    return FALSE;
}

// 0x8016BCC8
sb32 wpKirbyCutterProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->ga == nMPKineticsAir)
    {
        if (wpMapTestAllCheckFloor(weapon_gobj) == TRUE)
        {
            wpMapSetGround(wp);
        }
    }
    else if (wpMapTestLRWallCheckFloor(weapon_gobj) == FALSE)
    {
        wpMapSetAir(wp);

        wp->physics.vel_air.x = __cosf(DObjGetStruct(weapon_gobj)->rotate.vec.f.z) * wp->physics.vel_ground;
        wp->physics.vel_air.y = __sinf(DObjGetStruct(weapon_gobj)->rotate.vec.f.z) * wp->physics.vel_ground;

        if (DObjGetStruct(weapon_gobj)->rotate.vec.f.y < 0.0F)
        {
            wp->physics.vel_air.x = -wp->physics.vel_air.x;
            wp->physics.vel_air.y = -wp->physics.vel_air.y;
        }
    }

    if (wp->coll_data.mask_curr & (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL))
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else return FALSE;
}

// 0x8016BDD0
sb32 wpKirbyCutterProcHit(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return FALSE;
}

// 0x8016BE08
sb32 wpKirbyCutterProcShield(GObj *weapon_gobj)
{
    return FALSE;
}

// 0x8016BE14
sb32 wpKirbyCutterProcSetOff(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x8016BE4C
sb32 wpKirbyCutterProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPFINALCUTTER_LIFETIME;

    wpMainReflectorSetLR(wp, fp);
    wpMainVelSetModelPitch(weapon_gobj);

    return FALSE;
}

// 0x8016BE8C
GObj* wpKirbyCutterMakeWeapon(GObj *fighter_gobj, Vec3f *pos)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPKirbyCutterWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPFINALCUTTER_LIFETIME;

    wp->physics.vel_air.x = wp->lr * WPFINALCUTTER_VEL;

    if (mpCollisionGetFCCommonFloor(fp->coll_data.floor_line_id, pos, NULL, NULL, &wp->coll_data.floor_angle) != FALSE)
    {
        wp->coll_data.floor_line_id = fp->coll_data.floor_line_id;

        wpMapSetGround(wp);
    }
    wpMainVelSetModelPitch(weapon_gobj);

    return weapon_gobj;
}
