#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPFoxBlasterWeaponDesc = 
{
    0x00,                                  // Render flags?
    nWPKindBlaster,                        // Weapon Kind
    &gFTDataFoxSpecial1,                   // Pointer to character's loaded files?
    &llFoxSpecial1BlasterWeaponAttributes, // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,      // Main matrix transformations
        nGCMatrixKindNull,               // Secondary matrix transformations?
        0                                // ???
    },

    wpFoxBlasterProcUpdate,                 // Proc Update
    wpFoxBlasterProcMap,                    // Proc Map
    wpFoxBlasterProcHit,                    // Proc Hit
    wpFoxBlasterProcHit,                    // Proc Shield
    wpFoxBlasterProcHop,                    // Proc Hop
    wpFoxBlasterProcHit,                    // Proc Set-Off
    wpFoxBlasterProcReflector,              // Proc Reflector
    wpFoxBlasterProcHit                     // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801688D0
sb32 wpFoxBlasterProcUpdate(GObj *weapon_gobj)
{
    if (DObjGetStruct(weapon_gobj)->scale.vec.f.x < WPBLASTER_CLAMP_SCALE_X)
    {
        DObjGetStruct(weapon_gobj)->scale.vec.f.x += WPBLASTER_ADD_SCALE_X;

        if (DObjGetStruct(weapon_gobj)->scale.vec.f.x > WPBLASTER_CLAMP_SCALE_X)
        {
            DObjGetStruct(weapon_gobj)->scale.vec.f.x = WPBLASTER_CLAMP_SCALE_X;
        }
    }
    return FALSE;
}

// 0x80168924
sb32 wpFoxBlasterProcMap(GObj *weapon_gobj)
{
    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        efManagerFoxBlasterGlowMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

        return TRUE;
    }
    else return FALSE;
}

// 0x80168964
sb32 wpFoxBlasterProcHit(GObj *weapon_gobj)
{
    efManagerFoxBlasterGlowMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x8016898C
sb32 wpFoxBlasterProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    efManagerFoxBlasterGlowMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return FALSE;
}

// 0x80168A14
sb32 wpFoxBlasterProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    return FALSE;
}

// 0x80168A74
GObj* wpFoxBlasterMakeWeapon(GObj *fighter_gobj, Vec3f *pos)
{
    WPStruct *wp;
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPFoxBlasterWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->physics.vel_air.x = wp->lr * WPBLASTER_VEL_X;

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);

    efManagerFoxBlasterGlowMakeEffect(pos);

    return weapon_gobj;
}
