#include <wp/weapon.h>
#include <it/item.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPNessPKFireWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindPKFire,                          // Weapon Kind
    &gFTNessFileSpecial1,                   // Pointer to character's loaded files?
    &llNessSpecial1PKFireWeaponAttributes,  // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTra,                // Main matrix transformations
        0x2E,                            // Secondary matrix transformations?
        0                                // ???
    },

    wpNessPKFireProcUpdate,                 // Proc Update
    wpNessPKFireProcMap,                    // Proc Map
    wpNessPKFireProcHit,                    // Proc Hit
    wpNessPKFireProcAbsorb,                 // Proc Shield
    wpNessPKFireProcHop,                    // Proc Hop
    wpNessPKFireProcHit,                    // Proc Set-Off
    wpNessPKFireProcReflector,              // Proc Reflector
    wpNessPKFireProcAbsorb                  // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016AA40
sb32 wpNessPKFireProcUpdate(GObj *weapon_gobj)
{
    if (wpMainDecLifeCheckExpire(wpGetStruct(weapon_gobj)) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else return FALSE;
}

// 0x8016AA88
sb32 wpNessPKFireProcMap(GObj *weapon_gobj)
{
    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else return FALSE;
}

// 0x8016AACC
sb32 wpNessPKFireProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f pos;
    Vec3f vel;

    vel.x = 0.0F;
    vel.y = 0.0F;
    vel.z = 0.0F;

    pos = wp->physics.vel_air;

    syVectorNorm3D(&pos);

    pos.x = (pos.x * WPPKFIRE_POS_MUL) + DObjGetStruct(weapon_gobj)->translate.vec.f.x;
    pos.y = (pos.y * WPPKFIRE_POS_MUL) + DObjGetStruct(weapon_gobj)->translate.vec.f.y;
    pos.z = (pos.z * WPPKFIRE_POS_MUL) + DObjGetStruct(weapon_gobj)->translate.vec.f.z;

    itNessPKFireMakeItem(weapon_gobj, &pos, &vel);

    return TRUE;
}

// 0x8016AB84
sb32 wpNessPKFireProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);
    wpMainVelSetLR(weapon_gobj);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z *= -1.0F;

    return FALSE;
}

// 0x8016ABF0
sb32 wpNessPKFireProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPPKFIRE_LIFETIME;

    wpMainReflectorSetLR(wp, fp);
    wpMainVelSetLR(weapon_gobj);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z *= -1.0F;

    return FALSE;
}

// 0x8016AC4C
sb32 wpNessPKFireProcAbsorb(GObj *weapon_gobj)
{
    efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

    return TRUE;
}

// 0x8016AC78
GObj* wpNessPKFireMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel, f32 angle)
{
    s32 unused;
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPNessPKFireWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPPKFIRE_LIFETIME;

    wp->physics.vel_air = *vel;

    wpMainVelSetLR(weapon_gobj);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = (angle + F_CST_DTOR32(90.0F)) * wp->lr;

    return weapon_gobj;
}
