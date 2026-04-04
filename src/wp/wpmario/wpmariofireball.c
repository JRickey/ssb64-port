#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188E30
wpMarioFireballAttributes dWPMarioFireballWeaponAttributes[/* */] = 
{
    // Mario
    {
        140,                                      // Lifetime
        55.0F,                                    // Terminal velocity
        30.0F,                                    // Map collision damage
        1.2F,                                     // Gravity
        0.85F,                                    // Map collision rebound velocity?
        F_CLC_DTOR32(20.0F),                      // Rotate speed (0.349065870047F)
        F_CLC_DTOR32(-5.0F),                      // Grounded launch angle (-0.0872664675117F)
        F_CLC_DTOR32(-5.0F),                      // Aerial launch angle (-0.0872664675117F)
        50.0F,                                    // Base velocity
        &gFTMarioFileSpecial1,                    // Pointer to ???
        &llMarioSpecial1FireballWeaponAttributes, // Offset of hitbox/attributes?
        0.0F                                      // Animation starting frame?
    },

    // Luigi
    {
#if defined(REGION_US)
        80,                                       // Lifetime
#else
        90,                                       // Lifetime
#endif
        55.0F,                                    // Terminal velocity
        30.0F,                                    // Map collision damage
        0.0F,                                     // Gravity
        0.85F,                                    // Map collision rebound velocity?
        F_CLC_DTOR32(25.0F),                      // Rotate speed (0.436332315207F)
        F_CLC_DTOR32(0.0F),                       // Grounded launch angle (0.0F)
        F_CLC_DTOR32(0.0F),                       // Aerial launch angle (0.0F)
        36.0F,                                    // Base velocity
        &gFTDataLuigiSpecial1,                    // Pointer to ???
        &llLuigiSpecial1FireballWeaponAttributes, // Offset of hitbox/attributes?
        1.0F                                      // Animation starting frame?
    }
};

// 0x80188E90
WPDesc dWPMarioFireballWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindFireball,                        // Weapon Kind
    &gFTMarioFileMain,                      // Pointer to character's loaded files?
    &llMarioMainFireballWeaponAttributes,   // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTra,                   // Main matrix transformations
        0x47,                               // Secondary matrix transformations?
        0                                   // ???
    },

    wpMarioFireballProcUpdate,              // Proc Update
    wpMarioFireballProcMap,                 // Proc Map
    wpMarioFireballProcHit,                 // Proc Hit
    wpMarioFireballProcHit,                 // Proc Shield
    wpMarioFireballProcHop,                 // Proc Hop
    wpMarioFireballProcHit,                 // Proc Set-Off
    wpMarioFireballProcReflector,           // Proc Reflector
    wpMarioFireballProcHit                  // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80168540
sb32 wpMarioFireballProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    DObj *dobj;

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    wpMainApplyGravityClampTVel(wp, dWPMarioFireballWeaponAttributes[wp->weapon_vars.fireball.index].gravity, dWPMarioFireballWeaponAttributes[wp->weapon_vars.fireball.index].vel_terminal);

    dobj = DObjGetStruct(weapon_gobj);

    dobj->rotate.vec.f.x += dWPMarioFireballWeaponAttributes[wp->weapon_vars.fireball.index].rotate_speed;

    return FALSE;
}

// 0x801685F0
sb32 wpMarioFireballProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f pos;

    wpMapTestAll(weapon_gobj);

    if (wpMapCheckAllRebound(weapon_gobj, MAP_FLAG_MAIN_MASK, dWPMarioFireballWeaponAttributes[wp->weapon_vars.fireball.index].collide_rebound, &pos) != FALSE)
    {
        if (lbCommonMag2D(&wp->physics.vel_air) < dWPMarioFireballWeaponAttributes[wp->weapon_vars.fireball.index].vel_min)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);
            return TRUE;
        }
        wpMainVelSetModelPitch(weapon_gobj);

        efManagerFireGrindMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
    }
    return FALSE;
}

// 0x801686C0
sb32 wpMarioFireballProcHit(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x801686F8
sb32 wpMarioFireballProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);
    wpMainVelSetModelPitch(weapon_gobj);

    return FALSE;
}

// 0x80168748
sb32 wpMarioFireballProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = dWPMarioFireballWeaponAttributes[wp->weapon_vars.fireball.index].lifetime;

    wpMainReflectorSetLR(wp, fp);
    wpMainVelSetModelPitch(weapon_gobj);

    return FALSE;
}

// 0x801687A0
GObj* wpMarioFireballMakeWeapon(GObj *fighter_gobj, Vec3f *pos, s32 index)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *weapon_gobj;
    WPStruct *wp;
    f32 angle;

    dWPMarioFireballWeaponDesc.p_weapon = dWPMarioFireballWeaponAttributes[index].p_weapon;
    dWPMarioFireballWeaponDesc.o_attributes = dWPMarioFireballWeaponAttributes[index].offset;

    weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPMarioFireballWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->weapon_vars.fireball.index = index;

    wp->lifetime = dWPMarioFireballWeaponAttributes[index].lifetime;

    angle = (fp->ga == nMPKineticsAir) ? dWPMarioFireballWeaponAttributes[index].angle_air : dWPMarioFireballWeaponAttributes[index].angle_ground;

    wp->physics.vel_air.z = 0.0F;

    wp->physics.vel_air.x = dWPMarioFireballWeaponAttributes[index].vel_base * __cosf(angle) * fp->lr;
    wp->physics.vel_air.y = dWPMarioFireballWeaponAttributes[index].vel_base * __sinf(angle);

    DObjGetStruct(weapon_gobj)->mobj->palette_id = dWPMarioFireballWeaponAttributes[index].anim_frame;

    wpMainVelSetModelPitch(weapon_gobj);

    return weapon_gobj;
}
