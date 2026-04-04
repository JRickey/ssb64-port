#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

WPDesc dWPPikachuThunderJoltAirWeaponDesc =
{
    0x00,                                               // Render flags?
    nWPKindThunderJoltAir,                             // Weapon Kind
    &gFTDataPikachuSpecial1,                              // Pointer to character's loaded files?
    &llPikachuSpecial1ThunderJoltAirWeaponAttributes,          // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTra,                            // Main matrix transformations
        0x2E,                                           // Secondary matrix transformations?
        0                                               // ???
    },

    wpPikachuThunderJoltAirProcUpdate,                  // Proc Update
    wpPikachuThunderJoltAirProcMap,                     // Proc Map
    wpPikachuThunderJoltAirProcHit,                     // Proc Hit
    wpPikachuThunderJoltAirProcHit,                     // Proc Shield
    wpPikachuThunderJoltAirProcHop,                     // Proc Hop
    wpPikachuThunderJoltAirProcHit,                     // Proc Set-Off
    wpPikachuThunderJoltAirProcReflector,               // Proc Reflector
    wpPikachuThunderJoltAirProcHit                      // Proc Absorb
};

WPDesc dWPPikachuThunderJoltGroundWeaponDesc =
{
    0x03,                                               // Render flags?
    nWPKindThunderJoltGround,                          // Weapon Kind
    &gFTDataPikachuSpecial1,                              // Pointer to character's loaded files?
    &llPikachuSpecial1ThunderJoltGroundWeaponAttributes,       // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,                  // Main matrix transformations
        nGCMatrixKindNull,                           // Secondary matrix transformations?
        0                                               // ???
    },

    wpPikachuThunderJoltGroundProcUpdate,               // Proc Update
    wpPikachuThunderJoltGroundProcMap,                  // Proc Map
    wpPikachuThunderJoltGroundProcHit,                  // Proc Hit
    wpPikachuThunderJoltGroundProcHit,                  // Proc Shield
    NULL,                                               // Proc Hop
    wpPikachuThunderJoltGroundProcHit,                  // Proc Set-Off
    wpPikachuThunderJoltGroundProcReflector,            // Proc Reflector
    wpPikachuThunderJoltGroundProcHit                   // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80169390
sb32 wpPikachuThunderJoltAirProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else wpMainApplyGravityClampTVel(wp, WPPIKACHUJOLT_GRAVITY, WPPIKACHUJOLT_TVEL);

    return FALSE;
}

// 0x801693EC
sb32 wpPikachuThunderJoltAirProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f pos;
    u32 unused[2];

    if (wpMapTestAllCheckFloor(weapon_gobj) != FALSE)
    {
        wpPikachuThunderJoltGroundMakeWeapon(weapon_gobj, &DObjGetStruct(weapon_gobj)->translate.vec.f, nMPLineKindFloor);

        return TRUE;
    }

    else if (wp->coll_data.mask_curr & MAP_FLAG_LWALL)
    {
        mpCollisionGetLWallEdgeU(wp->coll_data.lwall_line_id, &pos);

        if (DObjGetStruct(weapon_gobj)->translate.vec.f.y > pos.y)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.y = pos.y;
        }
        mpCollisionGetLWallEdgeD(wp->coll_data.lwall_line_id, &pos);

        if (DObjGetStruct(weapon_gobj)->translate.vec.f.y < pos.y)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.y = pos.y;
        }
        wpPikachuThunderJoltGroundMakeWeapon(weapon_gobj, &DObjGetStruct(weapon_gobj)->translate.vec.f, nMPLineKindLWall);

        return TRUE;
    }

    else if (wp->coll_data.mask_curr & MAP_FLAG_RWALL)
    {
        mpCollisionGetRWallEdgeU(wp->coll_data.rwall_line_id, &pos);

        if (DObjGetStruct(weapon_gobj)->translate.vec.f.y > pos.y)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.y = pos.y;
        }
        mpCollisionGetRWallEdgeD(wp->coll_data.rwall_line_id, &pos);

        if (DObjGetStruct(weapon_gobj)->translate.vec.f.y < pos.y)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.y = pos.y;
        }
        wpPikachuThunderJoltGroundMakeWeapon(weapon_gobj, &DObjGetStruct(weapon_gobj)->translate.vec.f, nMPLineKindRWall);

        return TRUE;
    }
    return FALSE;
}

// 0x8016953C
sb32 wpPikachuThunderJoltAirProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerImpactShockMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->attack_coll.damage);

    return TRUE;
}

// 0x8016956C
sb32 wpPikachuThunderJoltAirProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    return FALSE;
}

// 0x801695B0
sb32 wpPikachuThunderJoltAirProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPPIKACHUJOLT_LIFETIME;

    wpMainReflectorSetLR(wp, fp);

    return FALSE;
}

// 0x801695E4
GObj* wpPikachuThunderJoltAirMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel)
{
    GObj *weapon_gobj = wpManagerMakeWeapon(fighter_gobj, &dWPPikachuThunderJoltAirWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_FIGHTER));
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lifetime = WPPIKACHUJOLT_LIFETIME;

    wp->physics.vel_air = *vel;

    return weapon_gobj;
}

// 0x80169654
void wpPikachuThunderJoltGroundAddAnim(GObj *weapon_gobj)
{
    gcAddAnimAll
    (
        weapon_gobj, 
        lbRelocGetFileData(AObjEvent32**, gFTDataPikachuSpecial3, &llPikachuSpecial3ThunderJoltBAnimJoint), 
        lbRelocGetFileData(AObjEvent32***, gFTDataPikachuSpecial3, &llPikachuSpecial3ThunderJoltBMatAnimJoint), 
        0.0F
    );
    gcPlayAnimAll(weapon_gobj);
}

// 0x801696A0
sb32 wpPikachuThunderJoltGroundProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (weapon_gobj->anim_frame == WPPIKACHUJOLT_ANIM_PUSH_FRAME)
    {
        efManagerPikachuThunderJoltMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, DObjGetStruct(weapon_gobj)->rotate.vec.f.z);
        wpPikachuThunderJoltGroundAddAnim(weapon_gobj);
    }
    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);
        return TRUE;
    }
    wp->physics.vel_air.x = cosf(DObjGetStruct(weapon_gobj)->rotate.vec.f.z) * WPPIKACHUJOLT_VEL;
    wp->physics.vel_air.y = __sinf(DObjGetStruct(weapon_gobj)->rotate.vec.f.z) * WPPIKACHUJOLT_VEL;

    switch (wp->weapon_vars.thunder_jolt.line_type)
    {
    case nMPLineKindFloor:
        wp->physics.vel_air.x *= wp->lr;
        break;

    case nMPLineKindRWall:
    case nMPLineKindLWall:
        if (wp->lr == 2)
        {
            if (wp->physics.vel_air.y < 0.0F)
            {
                wp->physics.vel_air.y *= -1.0F;
            }
        }
        if (wp->lr == 3)
        {
            if (wp->physics.vel_air.y > 0.0F)
            {
                wp->physics.vel_air.y *= -1.0F;
            }
        }
        break;
    }
    return FALSE;
}

// 0x8016981C
s32 wpPikachuThunderJoltGroundGetStatus(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f pos;
    Vec3f rotate;
    s32 line_id;

    switch (wp->weapon_vars.thunder_jolt.line_type)
    {
    case nMPLineKindFloor:
        if (wp->lr == +1)
        {
            if 
            (
                (mpCollisionCheckLWallLineCollisionDiff(&wp->coll_data.pos_prev, wp->coll_data.p_translate, &pos, &line_id, NULL, NULL) != FALSE) && 
                (mpCollisionGetEdgeUnderLLineID(wp->coll_data.floor_line_id) != line_id)
            )
            {
                if (mpCollisionGetLRCommonLWall(line_id, &pos, NULL, NULL, &rotate) != FALSE)
                {
                    wp->weapon_vars.thunder_jolt.line_type = nMPLineKindLWall;
                    wp->coll_data.lwall_line_id = line_id;
                    wp->lr = 2;

                    DObjGetStruct(weapon_gobj)->translate.vec.f = pos;

                    wpPikachuThunderJoltGroundAddAnim(weapon_gobj);

                    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(-rotate.x, rotate.y);

                    return nWPPikachuThunderJoltStatusActive;
                }
                return nWPPikachuThunderJoltStatusDestroy;
            }
            break;
        }
        else if 
        (
            (mpCollisionCheckRWallLineCollisionDiff(&wp->coll_data.pos_prev, wp->coll_data.p_translate, &pos, &line_id, NULL, NULL) != FALSE) && 
            (mpCollisionGetEdgeUnderRLineID(wp->coll_data.floor_line_id) != line_id)
        )
        {
            if (mpCollisionGetLRCommonRWall(line_id, &pos, NULL, NULL, &rotate) != FALSE)
            {
                wp->weapon_vars.thunder_jolt.line_type = nMPLineKindRWall;
                wp->coll_data.rwall_line_id = line_id;
                wp->lr = 2;

                DObjGetStruct(weapon_gobj)->translate.vec.f = pos;

                wpPikachuThunderJoltGroundAddAnim(weapon_gobj);

                DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(-rotate.x, rotate.y);

                return nWPPikachuThunderJoltStatusActive;
            }
            return nWPPikachuThunderJoltStatusDestroy;
        }
        break;

    case nMPLineKindLWall:
        if (wp->lr == 3)
        {
            if 
            (
                (mpCollisionCheckFloorLineCollisionDiff(&wp->coll_data.pos_prev, wp->coll_data.p_translate, &pos, &line_id, NULL, NULL) != FALSE) && 
                (mpCollisionGetEdgeRightULineID(wp->coll_data.lwall_line_id) != line_id)
            )
            {
                if (mpCollisionGetFCCommonFloor(line_id, &pos, NULL, NULL, &rotate) != FALSE)
                {
                    wp->weapon_vars.thunder_jolt.line_type = nMPLineKindFloor;
                    wp->coll_data.floor_line_id = line_id;
                    wp->lr = -1;

                    DObjGetStruct(weapon_gobj)->translate.vec.f = pos;

                    wpPikachuThunderJoltGroundAddAnim(weapon_gobj);

                    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(-rotate.x, rotate.y);

                    return nWPPikachuThunderJoltStatusActive;
                }
                return nWPPikachuThunderJoltStatusDestroy;
            }
            break;
        }
        else if (mpCollisionCheckCeilLineCollisionDiff(&wp->coll_data.pos_prev, wp->coll_data.p_translate, &pos, &line_id, NULL, NULL) != FALSE)
        {
            return nWPPikachuThunderJoltStatusDestroy;
        }
        break;

    case nMPLineKindRWall:
        if (wp->lr == 3)
        {
            if 
            (
                (mpCollisionCheckFloorLineCollisionDiff(&wp->coll_data.pos_prev, wp->coll_data.p_translate, &pos, &line_id, NULL, NULL) != FALSE) &&
                (mpCollisionGetEdgeLeftULineID(wp->coll_data.rwall_line_id) != line_id)
            )
            {
                if (mpCollisionGetFCCommonFloor(line_id, &pos, NULL, NULL, &rotate) != FALSE)
                {
                    wp->weapon_vars.thunder_jolt.line_type = nMPLineKindFloor;
                    wp->coll_data.floor_line_id = line_id;
                    wp->lr = +1;

                    DObjGetStruct(weapon_gobj)->translate.vec.f = pos;

                    wpPikachuThunderJoltGroundAddAnim(weapon_gobj);

                    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(-rotate.x, rotate.y);

                    return nWPPikachuThunderJoltStatusActive;
                }
                return nWPPikachuThunderJoltStatusDestroy;
            }
        }
        else if (mpCollisionCheckCeilLineCollisionDiff(&wp->coll_data.pos_prev, wp->coll_data.p_translate, &pos, &line_id, NULL, NULL) != FALSE)
        {
            return nWPPikachuThunderJoltStatusDestroy;
        }
        break;

    default: 
        break;
    }
    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(-wp->weapon_vars.thunder_jolt.rotate.x, wp->weapon_vars.thunder_jolt.rotate.y);

    return nWPPikachuThunderJoltStatusNull;
}

// 0x80169BF0
sb32 wpPikachuThunderJoltGroundCheckDestroy(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f angle;

    wpPikachuThunderJoltGroundAddAnim(weapon_gobj);

    switch (wp->weapon_vars.thunder_jolt.line_type)
    {
    case nMPLineKindFloor:
        mpCollisionGetFCCommonFloor(wp->coll_data.floor_line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f, NULL, NULL, &angle);
        break;

    case nMPLineKindLWall:
        mpCollisionGetLRCommonLWall(wp->coll_data.lwall_line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f, NULL, NULL, &angle);
        break;

    case nMPLineKindRWall:
        mpCollisionGetLRCommonRWall(wp->coll_data.rwall_line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f, NULL, NULL, &angle);
        break;
    }
    if (syVectorAngleDiff3D(&angle, &wp->weapon_vars.thunder_jolt.rotate) > WPPIKACHUJOLT_ROTATE_ANGLE_MAX)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(-angle.x, angle.y);

    return FALSE;
}

// 0x80169D08
sb32 wpPikachuThunderJoltGroundProcMap(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    s32 coll_type;
    f32 unused2[2];
    f32 dist;
    s32 line_id;
    s32 unused;

    switch (wp->weapon_vars.thunder_jolt.line_type)
    {
    case nMPLineKindFloor:
        if (mpCollisionCheckExistLineID(wp->coll_data.floor_line_id) == FALSE)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);
            
            return TRUE;
        }
        if (mpCollisionGetFCCommonFloor(wp->coll_data.floor_line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f, &dist, NULL, &wp->weapon_vars.thunder_jolt.rotate) != FALSE)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.y += dist;

            if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) == nWPPikachuThunderJoltStatusDestroy)
            {
                efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

                return TRUE;
            }
            return FALSE;
        }
        if (wp->lr == +1)
        {
            line_id = mpCollisionGetEdgeUnderRLineID(wp->coll_data.floor_line_id);
        }
        else if (wp->lr == -1)
        {
            line_id = mpCollisionGetEdgeUnderLLineID(wp->coll_data.floor_line_id);
        }
        else line_id = -1;
        
        if ((line_id == -1) || (line_id == -2))
        {
            if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) != nWPPikachuThunderJoltStatusActive)
            {
                efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

                return TRUE;
            }
            return FALSE;
        }
        coll_type = mpCollisionGetLineTypeID(line_id);

        if (wp->lr == +1)
        {
            if (coll_type == nMPLineKindLWall)
            {
                wp->coll_data.lwall_line_id = line_id;
                wp->weapon_vars.thunder_jolt.line_type = coll_type;
                wp->lr = 2;

                mpCollisionGetLWallEdgeD(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

                return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
            }
            if (coll_type == nMPLineKindRWall)
            {
                wp->coll_data.rwall_line_id = line_id;
                wp->weapon_vars.thunder_jolt.line_type = coll_type;
                wp->lr = 3;

                mpCollisionGetRWallEdgeU(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

                return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
            }
        }
        else if (coll_type == nMPLineKindLWall)
        {
            wp->coll_data.lwall_line_id = line_id;
            wp->weapon_vars.thunder_jolt.line_type = coll_type;
            wp->lr = 3;

            mpCollisionGetLWallEdgeU(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

            return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
        }
        else if (coll_type == nMPLineKindRWall)
        {
            wp->coll_data.rwall_line_id = line_id;
            wp->weapon_vars.thunder_jolt.line_type = coll_type;
            wp->lr = 2;

            mpCollisionGetRWallEdgeD(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

            return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
        }
        if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) != nWPPikachuThunderJoltStatusActive)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

            return TRUE;
        }
        return FALSE;

        break;

    case nMPLineKindLWall:
        if (mpCollisionCheckExistLineID(wp->coll_data.lwall_line_id) == FALSE)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

            return TRUE;
        }
        if (mpCollisionGetLRCommonLWall(wp->coll_data.lwall_line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f, &dist, NULL, &wp->weapon_vars.thunder_jolt.rotate) != FALSE)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.x += dist;

            if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) == nWPPikachuThunderJoltStatusDestroy)
            {
                efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

                return TRUE;
            }
            return FALSE;
        }
        if (wp->lr == 2)
        {
            line_id = mpCollisionGetEdgeRightULineID(wp->coll_data.lwall_line_id);
        }
        else line_id = mpCollisionGetEdgeRightDLineID(wp->coll_data.lwall_line_id);
        
        if ((line_id == -1) || (line_id == -2))
        {
            if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) != nWPPikachuThunderJoltStatusActive)
            {
                efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

                return TRUE;
            }
            return FALSE;
        }
        coll_type = mpCollisionGetLineTypeID(line_id);

        if (wp->lr == 2)
        {
            if (coll_type == nMPLineKindFloor)
            {
                wp->coll_data.floor_line_id = line_id;
                wp->weapon_vars.thunder_jolt.line_type = coll_type;
                wp->lr = +1;

                mpCollisionGetFloorEdgeL(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

                return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
            }
        }
        else if (coll_type == nMPLineKindFloor)
        {
            wp->coll_data.floor_line_id = line_id;
            wp->weapon_vars.thunder_jolt.line_type = coll_type;
            wp->lr = -1;

            mpCollisionGetFloorEdgeR(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

            return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
        }
        if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) != nWPPikachuThunderJoltStatusActive)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

            return TRUE;
        }
        return FALSE;

        break;

    case nMPLineKindRWall:
        if (mpCollisionCheckExistLineID(wp->coll_data.rwall_line_id) == FALSE)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

            return TRUE;
        }
        if (mpCollisionGetLRCommonRWall(wp->coll_data.rwall_line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f, &dist, NULL, &wp->weapon_vars.thunder_jolt.rotate) != FALSE)
        {
            DObjGetStruct(weapon_gobj)->translate.vec.f.x += dist;

            if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) == nWPPikachuThunderJoltStatusDestroy)
            {
                efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

                return TRUE;
            }
            return FALSE;
        }
        if (wp->lr == 2)
        {
            line_id = mpCollisionGetEdgeLeftULineID(wp->coll_data.rwall_line_id);
        }
        else line_id = mpCollisionGetEdgeLeftDLineID(wp->coll_data.rwall_line_id);
        
        if ((line_id == -1) || (line_id == -2))
        {
            if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) != nWPPikachuThunderJoltStatusActive)
            {
                efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

                return TRUE;
            }
            return FALSE;
        }
        coll_type = mpCollisionGetLineTypeID(line_id);

        if (wp->lr == 2)
        {
            if (coll_type == nMPLineKindFloor)
            {
                wp->coll_data.floor_line_id = line_id;
                wp->weapon_vars.thunder_jolt.line_type = coll_type;
                wp->lr = -1;

                mpCollisionGetFloorEdgeR(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

                return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
            }
        }
        else if (coll_type == nMPLineKindFloor)
        {
            wp->coll_data.floor_line_id = line_id;
            wp->weapon_vars.thunder_jolt.line_type = coll_type;
            wp->lr = +1;

            mpCollisionGetFloorEdgeL(line_id, &DObjGetStruct(weapon_gobj)->translate.vec.f);

            return wpPikachuThunderJoltGroundCheckDestroy(weapon_gobj);
        }
        if (wpPikachuThunderJoltGroundGetStatus(weapon_gobj) != nWPPikachuThunderJoltStatusActive)
        {
            efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

            return TRUE;
        }
        return FALSE;

    default:
        return FALSE;
    }
}

// 0x8016A374
sb32 wpPikachuThunderJoltGroundProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerImpactShockMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->attack_coll.damage);

    return TRUE;
}

// 0x8016A3A4
sb32 wpPikachuThunderJoltGroundProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wp->lifetime = WPPIKACHUJOLT_LIFETIME;

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.y = (wp->physics.vel_air.x >= 0.0F) ? F_CST_DTOR32(180.0F) : F_CST_DTOR32(0.0F);

    wpMainVelSetLR(weapon_gobj);

    return FALSE;
}

// 0x8016A42C
GObj* wpPikachuThunderJoltGroundMakeWeapon(GObj *prev_gobj, Vec3f *pos, s32 coll_type)
{
    s32 unused[2];
    WPStruct *prev_wp = wpGetStruct(prev_gobj);
    WPStruct *new_wp;
    GObj *new_gobj = wpManagerMakeWeapon(prev_gobj, &dWPPikachuThunderJoltGroundWeaponDesc, pos, (WEAPON_FLAG_COLLPROJECT | WEAPON_FLAG_PARENT_WEAPON));

    if (new_gobj == NULL)
    {
        return NULL;
    }
    new_wp = wpGetStruct(new_gobj);

    new_wp->weapon_vars.thunder_jolt.line_type = coll_type;

    new_wp->lifetime = prev_wp->lifetime;

    new_wp->coll_data.floor_line_id = prev_wp->coll_data.floor_line_id;
    new_wp->coll_data.lwall_line_id = prev_wp->coll_data.lwall_line_id;
    new_wp->coll_data.rwall_line_id = prev_wp->coll_data.rwall_line_id;

    gcSetAllAnimSpeed(new_gobj, 0.5F);
    wpPikachuThunderJoltGroundProcMap(new_gobj);

    switch (new_wp->weapon_vars.thunder_jolt.line_type)
    {
    case nMPLineKindFloor:
        new_wp->lr = (prev_wp->physics.vel_air.x >= 0.0F) ? +1 : -1;

        DObjGetStruct(new_gobj)->rotate.vec.f.y = (new_wp->lr >= 0.0F) ? F_CST_DTOR32(180.0F) : F_CST_DTOR32(0.0F);
        break;

    case nMPLineKindLWall:
        if (syUtilsArcTan2(new_wp->weapon_vars.thunder_jolt.rotate.y, new_wp->weapon_vars.thunder_jolt.rotate.x) > F_CST_DTOR32(135.0F)) // 2.3561945F
        {
            DObjGetStruct(new_gobj)->rotate.vec.f.y = F_CST_DTOR32(0.0F);
            new_wp->lr = 3;
        }
        else
        {
            DObjGetStruct(new_gobj)->rotate.vec.f.y = F_CST_DTOR32(180.0F);
            new_wp->lr = 2;
        }
        break;

    case nMPLineKindRWall:
        if (syUtilsArcTan2(new_wp->weapon_vars.thunder_jolt.rotate.y, new_wp->weapon_vars.thunder_jolt.rotate.x) > F_CST_DTOR32(45.0F))
        {
            DObjGetStruct(new_gobj)->rotate.vec.f.y = F_CST_DTOR32(0.0F);
            new_wp->lr = 2;
        }
        else
        {
            DObjGetStruct(new_gobj)->rotate.vec.f.y = F_CST_DTOR32(180.0F);
            new_wp->lr = 3;
        }
        break;
    }
    efManagerPikachuThunderJoltMakeEffect(&DObjGetStruct(new_gobj)->translate.vec.f, DObjGetStruct(new_gobj)->rotate.vec.f.z);
    wpMainPlayFGM(new_wp, nSYAudioFGMPikachuElectricLoop); // Bruh

    return new_gobj;
}
