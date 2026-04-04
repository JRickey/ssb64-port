#include <ft/fighter.h>
#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80163B40
void ftLinkSpecialHiDestroyWeapon(FTStruct *fp, WPStruct *wp)
{
    wp->is_hitlag_weapon = FALSE;

    wp->weapon_vars.spin_attack.is_destroy = TRUE;

    wpMainDestroyWeapon(wp->weapon_gobj);

    fp->status_vars.link.specialhi.spin_attack_gobj = NULL;
}

// 0x80163B80
void ftLinkSpecialHiUpdateWeaponPos(GObj *fighter_gobj, WPStruct *wp)
{
    wp->weapon_vars.spin_attack.pos_index++;
    wp->weapon_vars.spin_attack.pos_index %= WPSPINATTACK_EXTEND_POS_COUNT;

    wp->weapon_vars.spin_attack.pos_x[wp->weapon_vars.spin_attack.pos_index] = DObjGetStruct(fighter_gobj)->translate.vec.f.x;
    wp->weapon_vars.spin_attack.pos_y[wp->weapon_vars.spin_attack.pos_index] = DObjGetStruct(fighter_gobj)->translate.vec.f.y;
}

// 0x80163BF0
void ftLinkSpecialHiDecWeaponLifeCheckDestroy(GObj *fighter_gobj, WPStruct *wp)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        ftLinkSpecialHiDestroyWeapon(fp, wp);
    }
}

// 0x80163C2C
void ftLinkSpecialHiUpdateWeaponAttack(GObj *fighter_gobj, WPStruct *wp)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    /*
     * WARNING: UBs everywhere. Passing a fighter_gobj to wpProcessUpdateHitPositions which expects a weapon_gobj. Quite literally one hair away from breaking the game.
     * If either FTStruct or WPStruct got shifted, this inevitably would crash the game. In base SSB, wpProcessUpdateHitPositions grabs 0x150 from the GObj's user_data,
     * which happens to be attack1_followup_frames in FTStruct and attack_count in WPStruct. This is bad enough on its own, because attack1_followup_frames is a float,
     * which gets loaded as an integer. Not only that, this would be used as the loop iterator for weapon hitboxes... So not only does it receive the wrong struct, it also
     * could very well iterate out of bounds until it crashes from that instead. The only saving grace of this whole situation is that attack1_followup_frames is 0 outside of jabs.
     * On top of that, fixing this function yields no changes at all, because weapons are updated after fighters, and wpProcessUpdateHitPositions runs regardless.
     * It is absolutely ridiculous how close HAL were to casually making the game collapse from Link's specials on two occasions.
     */

    switch (fp->motion_vars.flags.flag2) 
    {
    case 0:
        break;

    case 1:
        wp->attack_coll.attack_state = nGMAttackStateNew;
        wp->attack_coll.size = FTLINK_SPINATTACK_FLAG_SIZE_1;

    #if !defined (AVOID_UB)
        wpProcessUpdateHitPositions(fighter_gobj);
    #elif !defined (DAIRANTOU_OPT0)
        wpProcessUpdateHitPositions(wp->weapon_gobj);
    #endif
        break;

    case 2:
        wp->attack_coll.attack_state = nGMAttackStateNew;
        wp->attack_coll.size = FTLINK_SPINATTACK_FLAG_SIZE_2;

    #if !defined (AVOID_UB)
        wpProcessUpdateHitPositions(fighter_gobj);
    #elif !defined (DAIRANTOU_OPT0)
        wpProcessUpdateHitPositions(wp->weapon_gobj);
    #endif
        break;

    case 3:
        wp->attack_coll.attack_state = nGMAttackStateNew;
        wp->attack_coll.size = FTLINK_SPINATTACK_FLAG_SIZE_3;

    #if !defined (AVOID_UB)
        wpProcessUpdateHitPositions(fighter_gobj);
    #elif !defined (DAIRANTOU_OPT0)
        wpProcessUpdateHitPositions(wp->weapon_gobj);
    #endif
        break;

    case 4:
        wp->attack_coll.attack_state = nGMAttackStateNew;
        wp->attack_coll.size = FTLINK_SPINATTACK_FLAG_SIZE_4;

    #if !defined (AVOID_UB)
        wpProcessUpdateHitPositions(fighter_gobj);
    #elif !defined (DAIRANTOU_OPT0)
        wpProcessUpdateHitPositions(wp->weapon_gobj);
    #endif
        break;

    case 13:
        wp->attack_coll.attack_state = nGMAttackStateOff;
        break;

    default:
        wp->attack_coll.attack_state = nGMAttackStateOff;
        break;
    }
    fp->motion_vars.flags.flag2 = 0;
}

// 0x80163D00
void ftLinkSpecialHiProcEffect(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.link.specialhi.spin_attack_gobj != NULL)
    {
        WPStruct *wp = wpGetStruct(fp->status_vars.link.specialhi.spin_attack_gobj);

        if (fp->hitlag_tics != 0)
        {
            wp->is_hitlag_weapon = TRUE;
        }
        else wp->is_hitlag_weapon = FALSE;
    }
}

// 0x80163D44
void ftLinkSpecialHiUpdateWeaponVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.link.specialhi.spin_attack_gobj != NULL)
    {
        WPStruct *wp = wpGetStruct(fp->status_vars.link.specialhi.spin_attack_gobj);

        ftLinkSpecialHiUpdateWeaponAttack(fighter_gobj, wp);
        ftLinkSpecialHiUpdateWeaponPos(fighter_gobj, wp);
        ftLinkSpecialHiDecWeaponLifeCheckDestroy(fighter_gobj, wp);
    }
}

// 0x80163D94
void ftLinkSpecialHiMakeWeapon(GObj *fighter_gobj, sb32 is_skip_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *spin_attack_gobj;
    Vec3f pos;

    if ((fp->motion_vars.flags.flag0 != 0) && (fp->status_vars.link.specialhi.spin_attack_gobj == NULL))
    {
        fp->motion_vars.flags.flag0 = 0;

        if (efManagerLinkSpinAttackMakeEffect(fighter_gobj) != NULL)
        {
            fp->is_effect_attach = TRUE;
        }
        if (is_skip_gobj == FALSE)
        {
            pos.x = pos.y = pos.z = 0.0F;

            gmCollisionGetFighterPartsWorldPosition(fp->joints[FTLINK_SPINATTACK_SPAWN_JOINT], &pos);

            fp->status_vars.link.specialhi.spin_attack_gobj = wpLinkSpinAttackMakeWeapon(fighter_gobj, &pos);

            if (fp->status_vars.link.specialhi.spin_attack_gobj != NULL)
            {
                WPStruct *wp = wpGetStruct(fp->status_vars.link.specialhi.spin_attack_gobj);
                s32 i;

                wp->attack_coll.attack_state = nGMAttackStateOff;

                for (i = 0; i < WPSPINATTACK_EXTEND_POS_COUNT; i++)
                {
                    wp->weapon_vars.spin_attack.pos_x[i] = (s16) DObjGetStruct(fighter_gobj)->translate.vec.f.x;
                    wp->weapon_vars.spin_attack.pos_y[i] = (s16) DObjGetStruct(fighter_gobj)->translate.vec.f.y;
                }
            }
        }
    }
}

// 0x80163EFC
void ftLinkSpecialHiProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.link.specialhi.spin_attack_gobj != NULL)
    {
        ftLinkSpecialHiDestroyWeapon(fp, wpGetStruct(fp->status_vars.link.specialhi.spin_attack_gobj));
    }
}

// 0x80163F30
void ftLinkSpecialHiProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftLinkSpecialHiEndSetStatus);
}

// 0x80163F54
void ftLinkSpecialHiEndProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (fp->status_vars.link.specialhi.spin_attack_gobj != NULL)
        {
            ftLinkSpecialHiDestroyWeapon(fp, wpGetStruct(fp->status_vars.link.specialhi.spin_attack_gobj));
        }
        ftCommonWaitSetStatus(fighter_gobj);
    }
}

// 0x80163FB0
void ftLinkSpecialAirHiProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (fp->status_vars.link.specialhi.spin_attack_gobj != NULL)
        {
            ftLinkSpecialHiDestroyWeapon(fp, wpGetStruct(fp->status_vars.link.specialhi.spin_attack_gobj));
        }
        ftCommonFallSpecialSetStatus(fighter_gobj, FTLINK_SPINATTACK_FALLSPECIAL_DRIFT, FALSE, TRUE, TRUE, FTLINK_SPINATTACK_LANDING_LAG, FALSE);
    }
}

// 0x80164030
void ftLinkSpecialHiProcPhysics(GObj *fighter_gobj)
{
    ftLinkSpecialHiUpdateWeaponVars(fighter_gobj);
    ftLinkSpecialHiMakeWeapon(fighter_gobj, FALSE);
    ftPhysicsApplyGroundVelFriction(fighter_gobj);
}

// 0x80164064
void ftLinkSpecialAirHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 gravity;

    ftLinkSpecialHiUpdateWeaponVars(fighter_gobj);
    ftLinkSpecialHiMakeWeapon(fighter_gobj, TRUE);

    gravity = (fp->motion_vars.flags.flag1 != 0) ? fp->attr->gravity : fp->attr->gravity * FTLINK_SPINATTACK_GRAVITY_MUL;

    ftPhysicsApplyGravityClampTVel(fp, gravity, fp->attr->tvel_base);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, fp->attr) == FALSE)
    {
        FTAttributes *attr = fp->attr;

        ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel * FTLINK_SPINATTACK_AIR_DRIFT_MUL, attr->air_speed_max_x);
        ftPhysicsApplyAirVelXFriction(fp, fp->attr);
    }
}

// 0x80164128
void ftLinkSpecialHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        mpCommonSetFighterAir(fp);
        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirHi, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_HIT));

        fp->proc_damage = ftLinkSpecialHiProcDamage;

        fp->jumps_used = fp->attr->jumps_max;
    }
}

// 0x80164198
void ftLinkSpecialHiEndProcMap(GObj *fighter_gobj)
{
    if (mpCommonProcFighterOnFloor(fighter_gobj, ftCommonFallSetStatus) == FALSE)
    {
        ftLinkSpecialHiProcDamage(fighter_gobj);
    }
}

// 0x801641D0
void ftLinkSpecialAirHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterCeilHeavyCliff(fighter_gobj) != FALSE)
    {
        ftLinkSpecialHiProcDamage(fighter_gobj);

        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else if (fp->coll_data.mask_stat & MAP_FLAG_FLOOR)
        {
            mpCommonSetFighterGround(fp);
            ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

            fp->proc_damage = ftLinkSpecialHiProcDamage;
        }
    }
}

// 0x8016426C
void ftLinkSpecialHiProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag0 = 0;
    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;

    fp->status_vars.link.specialhi.spin_attack_gobj = NULL;
}

// 0x80164284
void ftLinkSpecialHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftLinkSpecialHiProcStatus;

    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->proc_damage = ftLinkSpecialHiProcDamage;
    fp->proc_passive = ftLinkSpecialHiProcEffect;
}

// 0x801642EC
void ftLinkSpecialHiEndSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->proc_damage = ftLinkSpecialHiProcDamage;
    fp->proc_passive = ftLinkSpecialHiProcEffect;
}

// 0x80164348
void ftLinkSpecialAirHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftLinkSpecialHiProcStatus;

    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->physics.vel_air.y = FTLINK_SPINATTACK_AIR_VEL_Y;

    fp->jumps_used = fp->attr->jumps_max;

    fp->proc_damage = ftLinkSpecialHiProcDamage;
    fp->proc_passive = ftLinkSpecialHiProcEffect;
}
