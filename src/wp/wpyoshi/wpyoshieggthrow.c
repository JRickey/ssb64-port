#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPYoshiEggThrowWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindEggThrow,                        // Weapon Kind
    &gFTDataYoshiMain,                      // Pointer to character's loaded files?
    &llYoshiMainEggThrowWeaponAttributes,      // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        0x2E,                               // Secondary matrix transformations?
        0                                   // ???
    },

    wpYoshiEggThrowProcUpdate,              // Proc Update
    wpYoshiEggThrowProcMap,                 // Proc Map
    wpYoshiEggThrowProcHit,                 // Proc Hit
    wpYoshiEggThrowProcHit,                 // Proc Shield
    wpYoshiEggThrowProcHop,                 // Proc Hop
    wpYoshiEggThrowProcHit,                 // Proc Set-Off
    wpYoshiEggThrowProcReflector,           // Proc Reflector
    NULL                                    // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016BF50
sb32 wpYoshiEggThrowProcDead(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->weapon_vars.egg_throw.is_throw == FALSE)
    {
        return FALSE;
    }
    else return TRUE;
}

// 0x8016BF74
sb32 wpYoshiEggExplodeProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8016BFA0
void wpYoshiEggHitInitVars(GObj *weapon_gobj) // Egg Throw explodes from landing successfully
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPEGGTHROW_EXPLODE_LIFETIME;

    wp->attack_coll.can_hop = FALSE;
    wp->attack_coll.can_reflect = FALSE;

    wp->physics.vel_air.x = wp->physics.vel_air.y = wp->physics.vel_air.z = 0.0F;

    wp->attack_coll.size = WPEGGTHROW_EXPLODE_SIZE;

    DObjGetStruct(weapon_gobj)->dl = NULL;

    wp->proc_update = wpYoshiEggExplodeProcUpdate;
    wp->proc_map = NULL;
    wp->proc_hit = NULL;
    wp->proc_shield = NULL;
    wp->proc_hop = NULL;
    wp->proc_setoff = NULL;
    wp->proc_reflector = NULL;
}

// 0x8016C00C
void wpYoshiEggExpireInitVars(GObj *weapon_gobj) // Egg Throw explodes from expiring 
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPEGGTHROW_EXPLODE_LIFETIME;

    wp->attack_coll.can_reflect = FALSE;
    wp->attack_coll.can_shield = FALSE;

    wp->physics.vel_air.x = wp->physics.vel_air.y = wp->physics.vel_air.z = 0.0F;

    wp->attack_coll.size = WPEGGTHROW_EXPLODE_SIZE;

    DObjGetStruct(weapon_gobj)->dl = NULL;

    wp->proc_update = wpYoshiEggExplodeProcUpdate;
    wp->proc_map = NULL;
    wp->proc_hit = NULL;
    wp->proc_shield = NULL;
    wp->proc_hop = NULL;
    wp->proc_setoff = NULL;
    wp->proc_reflector = NULL;
}

// 0x8016C07C
void wpYoshiEggThrowInitVars(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    f32 angle = ABS(wp->weapon_vars.egg_throw.stick_range) / WPEGGTHROW_TRAJECTORY_DIV;

    if (angle > 1.0F)
    {
        angle = 1.0F;
    }

    angle *= WPEGGTHROW_ANGLE_MUL;

    if (angle < WPEGGTHROW_ANGLE_CLAMP)
    {
        angle = 0.0F;
    }

    angle *= ((wp->weapon_vars.egg_throw.stick_range < 0) ? -1 : +1);

    angle = (wp->weapon_vars.egg_throw.lr > 0) ? (WPEGGTHROW_TRAJECTORY_SUB_FORWARD - angle) : (WPEGGTHROW_TRAJECTORY_SUB_BEHIND - angle);

    wp->physics.vel_air.y = __sinf(angle) * ((wp->weapon_vars.egg_throw.throw_force * WPEGGTHROW_VEL_FORCE_MUL) + WPEGGTHROW_VEL_ADD);
    wp->physics.vel_air.x = __cosf(angle) * ((wp->weapon_vars.egg_throw.throw_force * WPEGGTHROW_VEL_FORCE_MUL) + WPEGGTHROW_VEL_ADD);

    wp->weapon_vars.egg_throw.angle = F_CLC_DTOR32((wp->weapon_vars.egg_throw.throw_force * WPEGGTHROW_ANGLE_FORCE_MUL) + WPEGGTHROW_ANGLE_ADD);

    DObjGetStruct(weapon_gobj)->translate.vec.f.z = 0.0F;

    wp->attack_coll.attack_state = nGMAttackStateNew;

    wpProcessUpdateHitPositions(weapon_gobj);
}

// 0x8016C218
sb32 wpYoshiEggThrowProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->weapon_vars.egg_throw.is_spin != FALSE)
    {
        if (wpMainDecLifeCheckExpire(wp) != FALSE)
        {
            func_800269C0_275C0(nSYAudioFGMYoshiEggShatter1);

            efManagerYoshiEggExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
            efManagerEggBreakMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

            wpYoshiEggExpireInitVars(weapon_gobj);

            return FALSE;
        }
        else
        {
            DObjGetStruct(weapon_gobj)->rotate.vec.f.z += wp->weapon_vars.egg_throw.angle;
            wpMainApplyGravityClampTVel(wp, WPEGGTHROW_GRAVITY, WPEGGTHROW_TVEL);

            return FALSE;
        }
    }
    else if (wp->weapon_vars.egg_throw.is_throw != FALSE)
    {
        wp->weapon_vars.egg_throw.is_spin = TRUE;

        wpYoshiEggThrowInitVars(weapon_gobj);
    }
    return FALSE;
}

// 0x8016C2E0
sb32 wpYoshiEggThrowProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if ((wp->weapon_vars.egg_throw.is_spin != FALSE) && (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE))
    {
        efManagerQuakeMakeEffect(2);

        func_800269C0_275C0(nSYAudioFGMYoshiEggShatter1);

        efManagerYoshiEggExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
        efManagerEggBreakMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        wpYoshiEggHitInitVars(weapon_gobj);
    }
    return FALSE;
}

// 0x8016C374
sb32 wpYoshiEggThrowProcHit(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMYoshiEggShatter1);

    efManagerYoshiEggExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
    efManagerEggBreakMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    wpYoshiEggHitInitVars(weapon_gobj);

    return FALSE;
}

// 0x8016C3B4
sb32 wpYoshiEggThrowProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    wpMainVelSetModelPitch(weapon_gobj);

    return FALSE;
}

// 0x8016C404
sb32 wpYoshiEggThrowProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPEGGTHROW_LIFETIME;

    wpMainReflectorSetLR(wp, fp);
    wpMainVelSetModelPitch(weapon_gobj);

    return FALSE;
}

// 0x8016C444
void wpYoshiEggThrowProcDisplay(GObj *weapon_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);

    gDPSetEnvColor(gSYTaskmanDLHeads[1]++, 0x00, 0x00, 0x00, 0x00);

    wpDisplayDLHead1(weapon_gobj);
}

// 0x8016C498
GObj* wpYoshiEggThrowMakeWeapon(GObj *fighter_gobj, Vec3f *pos)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPYoshiEggThrowWeaponDesc, pos, WEAPON_FLAG_PARENT_FIGHTER);
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }

    wp = wpGetStruct(weapon_gobj);

    weapon_gobj->proc_display = wpYoshiEggThrowProcDisplay;

    wp->weapon_vars.egg_throw.is_throw = FALSE;

    wp->proc_dead = wpYoshiEggThrowProcDead;

    wp->lifetime = WPEGGTHROW_LIFETIME;

    wp->attack_coll.attack_state = nGMAttackStateOff;

    wp->is_camera_follow = TRUE;

    wp->physics.vel_air.x = wp->physics.vel_air.y = 0.0F;

    wp->weapon_vars.egg_throw.is_spin = wp->weapon_vars.egg_throw.is_throw;
    wp->weapon_vars.egg_throw.lr = fp->lr;

    return weapon_gobj;
}
