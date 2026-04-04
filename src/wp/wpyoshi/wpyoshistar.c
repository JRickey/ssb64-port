#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPYoshiStarWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindYoshiStar,                       // Weapon Kind
    &gFTDataYoshiMain,                      // Pointer to character's loaded files?
    &llYoshiMainStarWeaponAttributes,       // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,      // Main matrix transformations
        nGCMatrixKindNull,               // Secondary matrix transformations?
        0                                // ???
    },

    wpYoshiStarProcUpdate,                  // Proc Update
    wpYoshiStarProcMap,                     // Proc Map
    wpYoshiStarProcHit,                     // Proc Hit
    wpYoshiStarProcShield,                  // Proc Shield
    wpYoshiStarProcHop,                     // Proc Hop
    wpYoshiStarProcHit,                     // Proc Set-Off
    wpYoshiStarProcReflector,               // Proc Reflector
    wpYoshiStarProcShield                   // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016C540
f32 wpYoshiStarGetScale(WPStruct *wp)
{
    f32 scale = (wp->lifetime * WPYOSHISTAR_LIFETIME_SCALE_MUL) + WPYOSHISTAR_LIFETIME_SCALE_ADD;

    if (scale > 1.0F)
    {
        scale = 1.0F;
    }
    return scale;
}

// 0x8016C588
sb32 wpYoshiStarProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    f32 scale;
    f32 vel_sqrt;
    f32 vel_mul;

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    scale = wpYoshiStarGetScale(wp);

    DObjGetStruct(weapon_gobj)->scale.vec.f.x = scale;
    DObjGetStruct(weapon_gobj)->scale.vec.f.y = scale;

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z += (WPYOSHISTAR_ROTATE_SPEED * wp->lr);

    vel_sqrt = sqrtf(SQUARE(wp->physics.vel_air.x) + SQUARE(wp->physics.vel_air.y));

    if (vel_sqrt > 0.0F)
    {
        if (vel_sqrt < WPYOSHISTAR_VEL_CLAMP)
        {
            vel_mul = 0.0F;
        }
        else vel_mul = vel_sqrt - WPYOSHISTAR_VEL_CLAMP;
        
        wp->physics.vel_air.x = (wp->physics.vel_air.x * vel_mul) / vel_sqrt;
        wp->physics.vel_air.y = (wp->physics.vel_air.y * vel_mul) / vel_sqrt;
    }
    return FALSE;
}

// 0x8016C6A0
sb32 wpYoshiStarProcMap(GObj *weapon_gobj)
{
    return FALSE;
}

// 0x8016C6AC
sb32 wpYoshiStarProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    if (wp->hit_normal_damage != 0)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8016C6F0
sb32 wpYoshiStarProcShield(GObj *weapon_gobj)
{
    efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x8016C718
sb32 wpYoshiStarProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);

    if (wp->physics.vel_air.x > 0.0F)
    {
        wp->lr = +1;
    }
    else wp->lr = -1;

    return FALSE;
}

// 0x8016C7B0
sb32 wpYoshiStarProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPYOSHISTAR_LIFETIME;

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);

    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(weapon_gobj)->scale.vec.f.y = 1.0F;

    wp->lr = -wp->lr;

    return FALSE;
}

// 0x8016C834
#if !defined (DAIRANTOU_OPT0)
    GObj* wpYoshiStarMakeWeapon(GObj *fighter_gobj, Vec3f *pos, s32 lr)
#else
    void wpYoshiStarMakeWeapon(GObj *fighter_gobj, Vec3f *pos, s32 lr)
#endif
{
    GObj *weapon_gobj;
    WPStruct *wp;
    Vec3f offset = *pos;

    offset.y += WPYOSHISTAR_OFF_Y;

    if (lr == +1)
    {
        offset.x += WPYOSHISTAR_OFF_X;
    }
    else offset.x -= WPYOSHISTAR_OFF_X;
    
    weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPYoshiStarWeaponDesc, &offset, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lr = lr;

    wp->lifetime = WPYOSHISTAR_LIFETIME;

    wp->physics.vel_air.x = __cosf(WPYOSHISTAR_ANGLE) * (WPYOSHISTAR_VEL * wp->lr);
    wp->physics.vel_air.y = __sinf(WPYOSHISTAR_ANGLE) * WPYOSHISTAR_VEL;

    #if !defined (AVOID_UB) || defined (DAIRANTOU_OPT0)
        return; // Undefined behavior here, no return value
    #else
        return weapon_gobj;
    #endif
}

// 0x8016C954
GObj* wpYoshiStarMakeStars(GObj *fighter_gobj, Vec3f *pos)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    wpYoshiStarMakeWeapon(fighter_gobj, pos, fp->lr);
    wpYoshiStarMakeWeapon(fighter_gobj, pos, -fp->lr);

    return NULL;
}
