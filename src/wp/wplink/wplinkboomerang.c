#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPLinkBoomerangWeaponDesc =
{
    0x01,                                     // Render flags?
    nWPKindBoomerang,                         // Weapon Kind
    &gFTDataLinkSpecial1,                     // Pointer to character's loaded files?
    &llLinkSpecial1BoomerangWeaponAttributes, // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,          // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0                                    // ???
    },

    wpLinkBoomerangProcUpdate,              // Proc Update
    wpLinkBoomerangProcMap,                 // Proc Map
    wpLinkBoomerangProcHit,                 // Proc Hit
    wpLinkBoomerangProcShield,              // Proc Shield
    wpLinkBoomerangProcHop,                 // Proc Hop
    wpLinkBoomerangProcHit,                 // Proc Set-Off
    wpLinkBoomerangProcReflector,           // Proc Reflector
    wpLinkBoomerangProcHit                  // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

typedef enum wpLinkBoomerangFlags
{
    nWPLinkBoomerangFlagReturn,
    nWPLinkBoomerangFlagDestroy,
    nWPLinkBoomerangFlagForward,
    nWPLinkBoomerangFlagUnk2,
    nWPLinkBoomerangFlagUnk3,
    nWPLinkBoomerangFlagReflect

} wpLinkBoomerangFlags;
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define WPLINK_BOOMERANG_FLAG_RETURN        (1 << nWPLinkBoomerangFlagReturn)
#define WPLINK_BOOMERANG_FLAG_DESTROY       (1 << nWPLinkBoomerangFlagDestroy)
#define WPLINK_BOOMERANG_FLAG_FORWARD       (1 << nWPLinkBoomerangFlagForward)
#define WPLINK_BOOMERANG_FLAG_UNK2          (1 << nWPLinkBoomerangFlagUnk2)
#define WPLINK_BOOMERANG_FLAG_UNK3          (1 << nWPLinkBoomerangFlagUnk3)
#define WPLINK_BOOMERANG_FLAG_REFLECT       (1 << nWPLinkBoomerangFlagReflect)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016CC50
void wpLinkBoomerangClampAngle360(f32 *angle)
{
    if (*angle > F_CST_DTOR32(360.0F))
    {
        *angle -= F_CST_DTOR32(360.0F);
    }
    else if (*angle < F_CST_DTOR32(-360.0F))
    {
        *angle += F_CST_DTOR32(360.0F);
    }
} 

// 0x8016CCA0
sb32 wpLinkBoomerangCheckOffCamera(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    f32 pos_x;
    f32 pos_y;
    f32 camera_bound_x;
    f32 camera_bound_y;

    if (wp->weapon_vars.boomerang.homing_delay > 0)
    {
        wp->weapon_vars.boomerang.homing_delay--;
    }
    else
    {
        wp->weapon_vars.boomerang.adjust_angle_delay++;

        if (wp->weapon_vars.boomerang.adjust_angle_delay > 8)
        {
            wp->weapon_vars.boomerang.adjust_angle_delay = 0;

            func_ovl2_800EB924(CObjGetStruct(gGMCameraGObj), gGMCameraMatrix, &DObjGetStruct(weapon_gobj)->translate.vec.f, &pos_x, &pos_y);

            camera_bound_x = (gGMCameraStruct.viewport_width / 2) + 40.0F;
            camera_bound_y = (gGMCameraStruct.viewport_height / 2) + 40.0F;

            if ((pos_x < -camera_bound_x) || (pos_x > camera_bound_x) || (pos_y < -camera_bound_y) || (pos_y > camera_bound_y))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// 0x8016CDC8
void wpLinkBoomerangSetReturnVars(GObj *weapon_gobj, sb32 angle_max_or_min)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->weapon_vars.boomerang.flags |= WPLINK_BOOMERANG_FLAG_RETURN;

    wp->attack_coll.damage = WPBOOMERANG_RETURN_DAMAGE;

    wp->weapon_vars.boomerang.default_angle -= F_CST_DTOR32(180.0F);

    if (wp->weapon_vars.boomerang.default_angle < 0.0F)
    {
        wp->weapon_vars.boomerang.default_angle += F_CST_DTOR32(360.0F);
    }
    wp->lr = -wp->lr;

    wp->weapon_vars.boomerang.flyforward_timer = 140;

    wp->weapon_vars.boomerang.homing_angle = (angle_max_or_min == 1) ? WPBOOMERANG_HOMING_ANGLE_MAX : WPBOOMERANG_HOMING_ANGLE_MIN;

    DObjGetStruct(weapon_gobj)->child->child->flags = DOBJ_FLAG_NOTEXTURE;

    wpMainPlayFGM(wp, nSYAudioFGMLinkSpecialNReturn);
}

// 0x8016CE90
f32 wpLinkBoomerangAddVelSqrt(WPStruct *wp, f32 vel_add)
{
    f32 sqrt_vel = sqrtf(SQUARE(wp->physics.vel_air.x) + SQUARE(wp->physics.vel_air.y)) + vel_add;

    if (sqrt_vel > 90.0F)
    {
        sqrt_vel = 90.0F;
    }
    return sqrt_vel;
}

// 0x8016CEEC
f32 wpLinkBoomerangSubVelSqrt(WPStruct *wp, f32 vel_sub)
{
    f32 sqrt_vel = sqrtf(SQUARE(wp->physics.vel_air.x) + SQUARE(wp->physics.vel_air.y)) - vel_sub;

    if (sqrt_vel < 10.0F)
    {
        sqrt_vel = 10.0F;
    }
    return sqrt_vel;
}

// 0x8016CF48
void wpLinkBoomerangUpdateVelLR(WPStruct *wp, f32 vel_mul)
{
    // The following floating point operations and sqrtf subroutine call are unused and waste CPU cycles

#if !defined(DAIRANTOU_OPT0)
    sqrtf(SQUARE(wp->physics.vel_air.x) + SQUARE(wp->physics.vel_air.y)); // Can we just talk about how this doesn't do anything
#endif

    wp->physics.vel_air.x = __cosf(wp->weapon_vars.boomerang.default_angle) * vel_mul;
    wp->physics.vel_air.y = __sinf(wp->weapon_vars.boomerang.default_angle) * vel_mul;

    wp->lr = ((wp->weapon_vars.boomerang.default_angle > F_CST_DTOR32(90.0F)) && (wp->weapon_vars.boomerang.default_angle < F_CST_DTOR32(270.0F))) ? -1 : +1;
}

// 0x8016CFFC
void wpLinkBoomerangClampAngleForward(f32 *angle)
{
    if ((*angle > F_CST_DTOR32(30.0F)) && (*angle <= F_CST_DTOR32(90.0F)))
    {
        *angle = F_CST_DTOR32(30.0F);
    }
    else if ((*angle < F_CST_DTOR32(150.0F)) && (*angle >= F_CST_DTOR32(90.0F)))
    {
        *angle = F_CST_DTOR32(150.0F);
    }
    else if ((*angle > F_CLC_DTOR32(210.0F)) && (*angle <= F_CST_DTOR32(270.0F)))
    {
        *angle = F_CLC_DTOR32(210.0F);
    }
    else if ((*angle < F_CLC_DTOR32(330.0F)) && (*angle >= F_CST_DTOR32(270.0F)))
    {
        *angle = F_CLC_DTOR32(330.0F);
    }
}

// 0x8016D0E4
f32 wpLinkBoomerangGetDistUpdateAngle(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    f32 unused;
    f32 dist_x;
    f32 dist_y;
    f32 angle;
    f32 sqrt_dist = 0.0F;

    if (wp->weapon_vars.boomerang.parent_gobj != NULL)
    {
        dist_x = DObjGetStruct(wp->weapon_vars.boomerang.parent_gobj)->translate.vec.f.x - DObjGetStruct(weapon_gobj)->translate.vec.f.x;
        dist_y = (DObjGetStruct(wp->weapon_vars.boomerang.parent_gobj)->translate.vec.f.y - DObjGetStruct(weapon_gobj)->translate.vec.f.y) + 290.0F;

        sqrt_dist = sqrtf(SQUARE(dist_x) + SQUARE(dist_y));

        if (wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_FORWARD)
        {
            if (wp->weapon_vars.boomerang.flyforward_timer > 0)
            {
                wp->weapon_vars.boomerang.flyforward_timer--;
            }
            else if (wp->weapon_vars.boomerang.flyforward_timer == 0)
            {
                return sqrt_dist;
            }
        }
        if ((wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_UNK3) && (((dist_x < 0.0F) && (wp->lr == +1)) || ((dist_x > 0.0F) && (wp->lr == -1))))
        {
            return sqrt_dist;
        }
        else
        {
            angle = syUtilsArcTan2(dist_y, dist_x);

            if (angle < F_CST_DTOR32(-180.0F))
            {
                angle += F_CST_DTOR32(360.0F);
            }
            else if (angle > F_CST_DTOR32(180.0F))
            {
                angle -= F_CST_DTOR32(360.0F);
            }
            angle -= wp->weapon_vars.boomerang.default_angle;

            if (angle < F_CST_DTOR32(-180.0F))
            {
                angle += F_CST_DTOR32(360.0F);
            }
            else if (angle > F_CST_DTOR32(180.0F))
            {
                angle -= F_CST_DTOR32(360.0F);
            }
            if (angle > wp->weapon_vars.boomerang.homing_angle)
            {
                angle = wp->weapon_vars.boomerang.homing_angle;
            }
            else if (angle < -wp->weapon_vars.boomerang.homing_angle)
            {
                angle = -wp->weapon_vars.boomerang.homing_angle;
            }
            wp->weapon_vars.boomerang.default_angle += angle;

            wpLinkBoomerangClampAngle360(&wp->weapon_vars.boomerang.default_angle);

            if (wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_UNK2)
            {
                wpLinkBoomerangClampAngleForward(&wp->weapon_vars.boomerang.default_angle);
            }
        }
    }
    return sqrt_dist;
}

// 0x8016D31C
void wpLinkBoomerangClearGObjs(WPStruct *wp)
{
    if (wp->weapon_vars.boomerang.parent_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(wp->weapon_vars.boomerang.parent_gobj);

        if ((fp->fkind == nFTKindKirby) || (fp->fkind == nFTKindNKirby))
        {
            fp->passive_vars.kirby.copylink_boomerang_gobj = NULL;
        }
        else fp->passive_vars.link.boomerang_gobj = NULL;
        
        wp->weapon_vars.boomerang.parent_gobj = NULL;
    }
}

// 0x8016D35C
void wpLinkBoomerangCheckOwnerCatch(GObj *weapon_gobj, f32 distance)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if ((wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_RETURN) && (distance < 180.0F))
    {
        if (wp->weapon_vars.boomerang.parent_gobj != NULL)
        {
            FTStruct *fp = ftGetStruct(wp->weapon_vars.boomerang.parent_gobj);

            if (fp->is_special_interrupt)
            {
                if ((fp->fkind == nFTKindKirby) || (fp->fkind == nFTKindNKirby))
                {
                    ftKirbyCopyLinkSpecialNGetSetStatus(wp->weapon_vars.boomerang.parent_gobj);
                }
                else ftLinkSpecialNGetSetStatus(wp->weapon_vars.boomerang.parent_gobj);          
            }
        }
        wpLinkBoomerangClearGObjs(wp);
        wpMainDestroyWeapon(weapon_gobj);
    }
}

// 0x8016D40C
sb32 wpLinkBoomerangCheckBound(WPStruct *wp, Vec3f *coll_angle)
{
    f32 angle = lbCommonSim2D(&wp->physics.vel_air, coll_angle);

    if (angle < 0.0F)
    {
        if (angle > -__sinf(F_CST_DTOR32(30.0F))) // 0.5235988F
        {
            lbCommonReflect2D(&wp->physics.vel_air, coll_angle);

            wp->weapon_vars.boomerang.default_angle = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x);

            wpLinkBoomerangClampAngle360(&wp->weapon_vars.boomerang.default_angle);
        }
        else return TRUE;
    }
    return FALSE;
}

// 0x8016D4B8
sb32 wpLinkBoomerangProcDead(GObj *weapon_gobj)
{
    wpLinkBoomerangClearGObjs(wpGetStruct(weapon_gobj));

    return TRUE;
}

// 0x8016D4DC
sb32 wpLinkBoomerangProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if ((wpMainDecLifeCheckExpire(wp) != FALSE) || (wpLinkBoomerangCheckOffCamera(weapon_gobj) == TRUE))
    {
        wpLinkBoomerangClearGObjs(wp);

        return TRUE;
    }
    if (wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_DESTROY)
    {
        wpLinkBoomerangClearGObjs(wp);

        return TRUE;
    }
    if (wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_REFLECT)
    {
        return FALSE;
    }
    else if (wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_RETURN)
    {
        wpLinkBoomerangUpdateVelLR(wp, wpLinkBoomerangAddVelSqrt(wp, 1.0F));

        wpLinkBoomerangCheckOwnerCatch(weapon_gobj, wpLinkBoomerangGetDistUpdateAngle(weapon_gobj));
    }
    else
    {
        f32 vel = wpLinkBoomerangSubVelSqrt(wp, 1.4F);

        wpLinkBoomerangUpdateVelLR(wp, vel);

        if (vel == 10.0F)
        {
            wpLinkBoomerangSetReturnVars(weapon_gobj, 0);
        }
    }
    return FALSE;
}

// 0x8016D5EC
sb32 wpLinkBoomerangProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    sb32 is_collide = FALSE;
    u16 coll_flags;

    if (!(wp->weapon_vars.boomerang.flags & (WPLINK_BOOMERANG_FLAG_REFLECT | WPLINK_BOOMERANG_FLAG_RETURN)))
    {
        wpMapTestAll(weapon_gobj);

        coll_flags = (wp->coll_data.mask_prev ^ wp->coll_data.mask_curr) & wp->coll_data.mask_curr & MAP_FLAG_MAIN_MASK;

        if (coll_flags)
        {
            efManagerDustCollideMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

            if (coll_flags & MAP_FLAG_LWALL)
            {
                is_collide |= wpLinkBoomerangCheckBound(wp, &wp->coll_data.lwall_angle);
            }
            if (coll_flags & MAP_FLAG_RWALL)
            {
                is_collide |= wpLinkBoomerangCheckBound(wp, &wp->coll_data.rwall_angle);
            }
            if (coll_flags & MAP_FLAG_CEIL)
            {
                is_collide |= wpLinkBoomerangCheckBound(wp, &wp->coll_data.ceil_angle);
            }
            if (coll_flags & MAP_FLAG_FLOOR)
            {
                is_collide |= wpLinkBoomerangCheckBound(wp, &wp->coll_data.floor_angle);
            }
            if (is_collide == TRUE)
            {
                wpLinkBoomerangSetReturnVars(weapon_gobj, 1);
            }
        }
    }
    return FALSE;
}

// 0x8016D714
sb32 wpLinkBoomerangProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (!(wp->weapon_vars.boomerang.flags & (WPLINK_BOOMERANG_FLAG_REFLECT | WPLINK_BOOMERANG_FLAG_RETURN)) && (wp->hit_normal_damage != 0))
    {
        wpLinkBoomerangUpdateVelLR(wp, wpLinkBoomerangSubVelSqrt(wp, 5.0F));
        wpLinkBoomerangSetReturnVars(weapon_gobj, 1);
    }
    return FALSE;
}

// 0x8016D77C
sb32 wpLinkBoomerangProcSetOff(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (!(wp->weapon_vars.boomerang.flags & (WPLINK_BOOMERANG_FLAG_REFLECT | WPLINK_BOOMERANG_FLAG_RETURN)))
    {
        wpLinkBoomerangSetReturnVars(weapon_gobj, 1);
    }
    return FALSE;
}

// 0x8016D7B4
sb32 wpLinkBoomerangProcShield(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (!(wp->weapon_vars.boomerang.flags & (WPLINK_BOOMERANG_FLAG_REFLECT | WPLINK_BOOMERANG_FLAG_RETURN)))
    {
        wpLinkBoomerangSetReturnVars(weapon_gobj, 1);
    }
    return FALSE;
}

// 0x8016D7EC
sb32 wpLinkBoomerangProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->shield_collide_dir.z > 0.0F)
    {
        wp->weapon_vars.boomerang.default_angle += (wp->shield_collide_angle * 2);
    }
    else wp->weapon_vars.boomerang.default_angle -= (wp->shield_collide_angle * 2);
    
    wpLinkBoomerangClampAngle360(&wp->weapon_vars.boomerang.default_angle);

    return FALSE;
}

// 0x8016D868
sb32 wpLinkBoomerangProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    f32 dist_x, dist_y;

    if (!(wp->weapon_vars.boomerang.flags & WPLINK_BOOMERANG_FLAG_REFLECT))
    {
        wp->weapon_vars.boomerang.flags = WPLINK_BOOMERANG_FLAG_REFLECT;
        wp->lifetime = WPBOOMERANG_LIFETIME_REFLECT;
    }
    dist_x = DObjGetStruct(weapon_gobj)->translate.vec.f.x - DObjGetStruct(wp->owner_gobj)->translate.vec.f.x;
    dist_y = DObjGetStruct(weapon_gobj)->translate.vec.f.y - (DObjGetStruct(wp->owner_gobj)->translate.vec.f.y + 250.0F);

    wp->weapon_vars.boomerang.default_angle = syUtilsArcTan2(dist_y, dist_x);

    wpLinkBoomerangClampAngle360(&wp->weapon_vars.boomerang.default_angle);
    wpLinkBoomerangUpdateVelLR(wp, sqrtf(SQUARE(wp->physics.vel_air.x) + SQUARE(wp->physics.vel_air.y)));

    return FALSE;
}

// 0x8016D914
f32 wpLinkBoomerangGetAngleSetVel(Vec3f *vel, FTStruct *fp, s32 lr, f32 vel_mul)
{
    f32 angle;

    if (ABS(fp->input.pl.stick_range.y) > WPBOOMERANG_ANGLE_STICK_THRESHOLD)
    {
        angle = syUtilsArcTan2(fp->input.pl.stick_range.y, ABS(fp->input.pl.stick_range.x));

        if (angle > F_CST_DTOR32(30.0F)) // 0.5235988F
        {
            angle = F_CST_DTOR32(30.0F);

        }
        else if (angle < F_CST_DTOR32(-30.0F)) // -0.5235988F
        {
            angle = F_CST_DTOR32(-30.0F);
        }
    }
    else angle = 0.0F;
    
    vel->x = (__cosf(angle) * vel_mul * lr);
    vel->y = (__sinf(angle) * vel_mul * lr);
    vel->z = 0.0F;

    if (lr == -1)
    {
        if (angle < 0.0F)
        {
            angle = F_CST_DTOR32(-180.0F) - angle;
        }
        else angle = F_CST_DTOR32(180.0F) - angle;
    }
    if (angle < 0.0F)
    {
        angle += F_CST_DTOR32(360.0F);
    }
    return angle;
}

// 0x8016DA78
GObj* wpLinkBoomerangMakeWeapon(GObj *fighter_gobj, Vec3f *pos)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *weapon_gobj;
    WPStruct *wp;
    Vec3f offset;
    s32 unused;

    offset = *pos;

    offset.y += WPBOOMERANG_OFF_Y;

    offset.x = (fp->lr == +1) ? offset.x + WPBOOMERANG_OFF_X : offset.x - WPBOOMERANG_OFF_X;

    weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPLinkBoomerangWeaponDesc, &offset, WEAPON_FLAG_PARENT_FIGHTER);

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lr = fp->lr;

    if (fp->status_vars.link.specialn.is_smash == TRUE)
    {
        wp->lifetime = WPBOOMERANG_LIFETIME_SMASH;
        wp->weapon_vars.boomerang.default_angle = wpLinkBoomerangGetAngleSetVel(&wp->physics.vel_air, fp, wp->lr, WPBOOMERANG_VEL_SMASH);
    }
    else
    {
        wp->lifetime = WPBOOMERANG_LIFETIME_TILT;
        wp->weapon_vars.boomerang.default_angle = wpLinkBoomerangGetAngleSetVel(&wp->physics.vel_air, fp, wp->lr, WPBOOMERANG_VEL_TILT);
    }
    wp->proc_setoff = wpLinkBoomerangProcSetOff;
    wp->proc_dead = wpLinkBoomerangProcDead;

    wp->is_camera_follow = TRUE;

    wpMainPlayFGM(wp, nSYAudioFGMLinkSpecialNShoot);

    wp->weapon_vars.boomerang.parent_gobj = fighter_gobj;
    wp->weapon_vars.boomerang.flags = WPLINK_BOOMERANG_FLAG_FORWARD;
    wp->weapon_vars.boomerang.homing_delay = 130;
    wp->weapon_vars.boomerang.adjust_angle_delay = 0;

    wpMainVelSetModelPitch(weapon_gobj);

    wp->is_hitlag_victim = TRUE;

    return weapon_gobj;
}
