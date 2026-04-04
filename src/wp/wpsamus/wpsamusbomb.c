#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPSamusBombWeaponDesc =
{
    0x00,                                   // Render flags?
    nWPKindSamusBomb,                       // Weapon Kind
    &gFTDataSamusMain,                      // Pointer to character's loaded files?
    &llSamusMainBombWeaponAttributes,       // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTra,                   // Main matrix transformations
        0x46,                               // Secondary matrix transformations?
        0                                   // ???
    },

    wpSamusBombProcUpdate,                  // Proc Update
    wpSamusBombProcMap,                     // Proc Map
    wpSamusBombProcHit,                     // Proc Hit
    wpSamusBombProcHit,                     // Proc Shield
    wpSamusBombProcHop,                     // Proc Hop
    wpSamusBombProcHit,                     // Proc Set-Off
    wpSamusBombProcReflector,               // Proc Reflector
    wpSamusBombProcAbsorb                   // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80168F00
sb32 wpSamusBombExplodeProcUpdate(GObj *weapon_gobj)
{
    if (wpMainDecLifeCheckExpire(wpGetStruct(weapon_gobj)) != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80168F2C
void wpSamusBombExplodeInitVars(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPSAMUSBOMB_EXPLODE_LIFETIME;

    wp->attack_coll.can_rehit_item = TRUE;
    wp->attack_coll.can_hop = FALSE;

    wp->physics.vel_air.x = wp->physics.vel_air.y = wp->physics.vel_air.z = 0.0F;

    wp->attack_coll.size = WPSAMUSBOMB_EXPLODE_SIZE;

    DObjGetStruct(weapon_gobj)->dl = NULL;

    wp->proc_update = wpSamusBombExplodeProcUpdate;
    wp->proc_map = NULL;
    wp->proc_hit = NULL;
    wp->proc_shield = NULL;
    wp->proc_hop = NULL;
    wp->proc_setoff = NULL;
    wp->proc_reflector = NULL;
}

// 0x80168F98
sb32 wpSamusBombProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerSparkleWhiteMultiExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
        wpSamusBombExplodeInitVars(weapon_gobj);
        func_800269C0_275C0(nSYAudioFGMExplodeS);

        return FALSE;
    }
    else
    {
        if (wp->ga == nMPKineticsAir)
        {
            wpMainApplyGravityClampTVel(wp, WPSAMUSBOMB_WAIT_GRAVITY, WPSAMUSBOMB_WAIT_TVEL);
            DObjGetStruct(weapon_gobj)->rotate.vec.f.z -= (WPSAMUSBOMB_WAIT_ROTATE_SPEED_AIR * wp->lr);
        }
        else
        {
            wpMainVelGroundTransferAir(weapon_gobj);
            DObjGetStruct(weapon_gobj)->rotate.vec.f.z -= (WPSAMUSBOMB_WAIT_ROTATE_SPEED_GROUND * wp->lr);
        }
        wp->weapon_vars.samus_bomb.bomb_blink_timer--;

        if (wp->weapon_vars.samus_bomb.bomb_blink_timer == 0)
        {
            MObj *mobj = DObjGetStruct(weapon_gobj)->mobj;

            mobj->palette_id = (mobj->palette_id != 0) ? 0.0F : 1.0F;

            if (wp->lifetime > WPSAMUSBOMB_WAIT_BLINK_SLOW)
            {
                wp->weapon_vars.samus_bomb.bomb_blink_timer = WPSAMUSBOMB_WAIT_BLINK_TIMER_SLOW;
            }
            else if (wp->lifetime > WPSAMUSBOMB_WAIT_BLINK_MID)
            {
                wp->weapon_vars.samus_bomb.bomb_blink_timer = WPSAMUSBOMB_WAIT_BLINK_TIMER_MID;
            }
            else wp->weapon_vars.samus_bomb.bomb_blink_timer = WPSAMUSBOMB_WAIT_BLINK_TIMER_FAST;
        }
    }
    return FALSE;
}

// 0x80169108
sb32 wpSamusBombProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f *vel;
    sb32 is_collide;

    if (wp->ga == nMPKineticsAir)
    {
        is_collide = wpMapTestAllCheckFloor(weapon_gobj);

        if (wpMapCheckAllRebound(weapon_gobj, (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL), WPSAMUSBOMB_WAIT_COLLIDE_MOD_VEL, NULL) != FALSE)
        {
            wpMainVelSetLR(weapon_gobj);
        }
        if (is_collide != FALSE)
        {
            vel = &wp->physics.vel_air;

            lbCommonReflect2D(vel, &wp->coll_data.floor_angle);
            lbCommonScale2D(vel, 0.6F);
            wpMainVelSetLR(weapon_gobj);

            if (lbCommonMag2D(vel) < 8.0F)
            {
                wpMapSetGround(wp);
            }
        }
    }
    else if (wpMapTestLRWallCheckFloor(weapon_gobj) == FALSE)
    {
        wpMapSetAir(wp);
    }
    return FALSE;
}

// 0x801691FC
sb32 wpSamusBombProcHit(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMultiExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
    wpSamusBombExplodeInitVars(weapon_gobj);

    return FALSE;
}

// 0x8016923C
sb32 wpSamusBombProcAbsorb(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMultiExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x80169274
sb32 wpSamusBombProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);
    wpMainVelSetLR(weapon_gobj);

    return FALSE;
}

// 0x801692C4
sb32 wpSamusBombProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPSAMUSBOMB_WAIT_LIFETIME;

    if (wp->ga == nMPKineticsAir)
    {
        wpMainReflectorSetLR(wp, fp);
        wpMainVelSetLR(weapon_gobj);
    }
    else wp->lr = fp->lr;
    
    return FALSE;
}

// 0x80169328
GObj* wpSamusBombMakeWeapon(GObj *fighter_gobj, Vec3f *pos)
{
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPSamusBombWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPSAMUSBOMB_WAIT_LIFETIME;

    wp->weapon_vars.samus_bomb.bomb_blink_timer = WPSAMUSBOMB_WAIT_BLINK_TIMER_SLOW;

    wp->physics.vel_air.y = WPSAMUSBOMB_WAIT_VEL_Y;

    return weapon_gobj;
}
