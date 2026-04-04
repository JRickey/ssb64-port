#include <ft/fighter.h>
#include <it/item.h>

extern alSoundEffect* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801885A0
s32 dFTCommonDamageStatusGroundIDs[/* */][3] =
{
    { nFTCommonStatusDamageLw1,   nFTCommonStatusDamageN1,    nFTCommonStatusDamageHi1   },
    { nFTCommonStatusDamageLw2,   nFTCommonStatusDamageN2,    nFTCommonStatusDamageHi2   },
    { nFTCommonStatusDamageLw3,   nFTCommonStatusDamageN3,    nFTCommonStatusDamageHi3   },
    { nFTCommonStatusDamageFlyLw, nFTCommonStatusDamageFlyN,  nFTCommonStatusDamageFlyHi }
};

// 0x801885D0
s32 dFTCommonDamageStatusAirIDs[/* */][3] =
{
    { nFTCommonStatusDamageAir1,  nFTCommonStatusDamageAir1,  nFTCommonStatusDamageAir1  },
    { nFTCommonStatusDamageAir2,  nFTCommonStatusDamageAir2,  nFTCommonStatusDamageAir2  },
    { nFTCommonStatusDamageAir3,  nFTCommonStatusDamageAir3,  nFTCommonStatusDamageAir3  },
    { nFTCommonStatusDamageFlyLw, nFTCommonStatusDamageFlyN,  nFTCommonStatusDamageFlyHi }
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80140340
void ftCommonDamageSetDustEffectInterval(FTStruct *fp)
{
    f32 vel = (fp->ga == nMPKineticsAir) ? syVectorMag3D(&fp->physics.vel_damage_air) : ABSF(fp->physics.vel_damage_ground);
    s32 make_effect_wait;

    if (vel < FTCOMMON_DAMAGE_EFFECT_KNOCKBACK_LOW)
    {
        make_effect_wait = FTCOMMON_DAMAGE_EFFECT_WAIT_LOW;
    }
    else if (vel < FTCOMMON_DAMAGE_EFFECT_KNOCKBACK_MID_LOW)
    {
        make_effect_wait = FTCOMMON_DAMAGE_EFFECT_WAIT_MID_LOW;
    }
    else if (vel < FTCOMMON_DAMAGE_EFFECT_KNOCKBACK_MID)
    {
        make_effect_wait = FTCOMMON_DAMAGE_EFFECT_WAIT_MID;
    }
    else if (vel < FTCOMMON_DAMAGE_EFFECT_KNOCKBACK_MID_HIGH)
    {
        make_effect_wait = FTCOMMON_DAMAGE_EFFECT_WAIT_MID_HIGH;
    }
    else if (vel < FTCOMMON_DAMAGE_EFFECT_KNOCKBACK_HIGH)
    {
        make_effect_wait = FTCOMMON_DAMAGE_EFFECT_WAIT_HIGH;
    }
    else make_effect_wait = FTCOMMON_DAMAGE_EFFECT_WAIT_DEFAULT;
    
    fp->status_vars.common.damage.dust_effect_int = make_effect_wait;
}

// 0x80140454
void ftCommonDamageUpdateDustEffect(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.damage.dust_effect_int != 0)
    {
        fp->status_vars.common.damage.dust_effect_int--;

        if (fp->status_vars.common.damage.dust_effect_int == 0)
        {
            ftParamMakeEffect(fighter_gobj, nEFKindDustExpandLarge, 4, NULL, NULL, fp->lr, FALSE, FALSE);
            ftCommonDamageSetDustEffectInterval(fp);
        }
    }
}

// 0x801404B8
void ftCommonDamageDecHitStunSetPublic(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.damage.hitstun_tics != 0)
    {
        fp->status_vars.common.damage.hitstun_tics--;

        if (fp->status_vars.common.damage.hitstun_tics == 0)
        {
            fp->public_knockback = fp->status_vars.common.damage.public_knockback;
        }
    }
}

// 0x801404E0
void ftCommonDamageCommonProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonDamageDecHitStunSetPublic(fighter_gobj);

    if ((fighter_gobj->anim_frame <= 0.0F) && (fp->status_vars.common.damage.hitstun_tics == 0))
    {
        mpCommonSetFighterWaitOrFall(fighter_gobj);
    }
}

// 0x8014053C
void ftCommonDamageAirCommonProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonDamageUpdateDustEffect(fighter_gobj);
    ftCommonDamageDecHitStunSetPublic(fighter_gobj);

    if ((fighter_gobj->anim_frame <= 0.0F) && (fp->status_vars.common.damage.hitstun_tics == 0))
    {
        ftCommonDamageFallSetStatusFromDamage(fighter_gobj);
    }
}

// 0x801405A0
void ftCommonDamageCheckSetInvincible(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->hitlag_tics <= 0) && (fp->status_vars.common.damage.is_knockback_over != FALSE))
    {
        fp->status_vars.common.damage.is_knockback_over = FALSE;

        ftParamSetTimedHitStatusInvincible(fp, 1);
    }
}

// 0x801405E4
void ftCommonDamageSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->hitlag_tics <= 0)
    {
        ftMainSetStatus(fighter_gobj, fp->status_vars.common.damage.status_id, 0.0F, 1.0F, (FTSTATUS_PRESERVE_DAMAGEPLAYER | FTSTATUS_PRESERVE_SHUFFLETIME));
        ftMainPlayAnimEventsAll(fighter_gobj);

        if (fp->status_id == nFTCommonStatusDamageFlyRoll)
        {
            ftCommonDamageFlyRollUpdateModelPitch(fighter_gobj);
        }
        fp->is_hitstun = TRUE;

        if (fp->status_vars.common.damage.is_knockback_over != FALSE)
        {
            fp->status_vars.common.damage.is_knockback_over = FALSE;

            ftParamSetTimedHitStatusInvincible(fp, 1);
        }
    }
}

// 0x80140674
void ftCommonDamageCommonProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.damage.hitstun_tics == 0)
    {
        fp->is_hitstun = FALSE;

        if (fp->ga == nMPKineticsAir)
        {
            if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
            {
                ftCommonHammerFallProcInterrupt(fighter_gobj);
            }
            else ftCommonFallProcInterrupt(fighter_gobj);
        }
        else if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
        {
            ftHammerProcInterrupt(fighter_gobj);
        }
        else ftCommonWaitProcInterrupt(fighter_gobj);
    }
}

// 0x8014070C
void ftCommonDamageAirCommonProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.damage.hitstun_tics == 0)
    {
        fp->is_hitstun = FALSE;

        ftCommonDamageFallProcInterrupt(fighter_gobj);
    }
}

// 0x80140744
void ftCommonDamageFlyRollUpdateModelPitch(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->joints[4]->rotate.vec.f.x = syUtilsArcTan2(fp->physics.vel_air.x + fp->physics.vel_damage_air.x, fp->physics.vel_air.y + fp->physics.vel_damage_air.y) * fp->lr;

    ftParamsUpdateFighterPartsTransformAll(fp->joints[4]);
}

// 0x801407A8
void ftCommonDamageCommonProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        if (fp->status_vars.common.damage.hitstun_tics == 0)
        {
            ftPhysicsApplyAirVelDriftFastFall(fighter_gobj);
        }
        else ftPhysicsApplyAirVelFriction(fighter_gobj);
    }
    else ftPhysicsApplyGroundVelFriction(fighter_gobj);
    
    if (fp->status_id == nFTCommonStatusDamageFlyRoll)
    {
        ftCommonDamageFlyRollUpdateModelPitch(fighter_gobj);
    }
    if ((fp->throw_gobj != NULL) && (syVectorMag3D(&fp->physics.vel_damage_air) < 70.0F))
    {
        ftParamClearAttackCollAll(fighter_gobj);
    }
}

// 0x80140878
void ftCommonDamageCommonProcLagUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->hitlag_tics != 0)
    {
        if ((SQUARE(fp->input.pl.stick_range.x) + SQUARE(fp->input.pl.stick_range.y)) >= SQUARE(FTCOMMON_DAMAGE_SMASH_DI_RANGE_MIN))
        {
            if ((fp->tap_stick_x < FTCOMMON_DAMAGE_SMASH_DI_BUFFER_TICS_MAX) || (fp->tap_stick_y < FTCOMMON_DAMAGE_SMASH_DI_BUFFER_TICS_MAX))
            {
                Vec3f *translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;

                translate->x += fp->input.pl.stick_range.x * FTCOMMON_DAMAGE_SMASH_DI_RANGE_MUL;
                translate->y += fp->input.pl.stick_range.y * FTCOMMON_DAMAGE_SMASH_DI_RANGE_MUL;

                fp->tap_stick_x = fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
            }
        }
    }
}

// 0x80140934
void func_ovl3_80140934(void) // Unused?
{
    return;
}

// 0x8014093C
void ftCommonDamageAirCommonProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if 
    (
        (mpCommonCheckFighterDamageCollision(fighter_gobj) != FALSE)        &&
        (ftCommonWallDamageCheckGoto(fighter_gobj) == FALSE)                &&
        (fp->status_vars.common.damage.coll_mask_curr & MAP_FLAG_FLOOR) &&
        (ftCommonPassiveStandCheckInterruptDamage(fighter_gobj) == FALSE)   &&
        (ftCommonPassiveCheckInterruptDamage(fighter_gobj) == FALSE)
    )
    {
        ftCommonDownBounceSetStatus(fighter_gobj);
    }
}

// 0x801409BC
f32 ftCommonDamageGetKnockbackAngle(s32 angle_i, sb32 ga, f32 knockback)
{
    f32 angle_f;

    if (angle_i != 361)
    {
        angle_f = F_CLC_DTOR32(angle_i);
    }
    else if (ga == nMPKineticsAir)
    {
        angle_f = FTCOMMON_DAMAGE_SAKURAI_ANGLE_DEFAULT_AR;
    }
    else if (knockback < FTCOMMON_DAMAGE_SAKURAI_KNOCKBACK_LOW)
    {
        angle_f = FTCOMMON_DAMAGE_SAKURAI_ANGLE_LOW_GR;
    }
    else
    {
        angle_f = F_CLC_DTOR32((((knockback - FTCOMMON_DAMAGE_SAKURAI_KNOCKBACK_LOW) / 0.099998474F) * FTCOMMON_DAMAGE_SAKURAI_ANGLE_HIGH_GD) + 1.0F);

        if (angle_f > FTCOMMON_DAMAGE_SAKURAI_ANGLE_HIGH_GR)
        {
            angle_f = FTCOMMON_DAMAGE_SAKURAI_ANGLE_HIGH_GR;
        }
    }
    return angle_f;
}

// 0x80140A94
s32 ftCommonDamageGetDamageLevel(f32 hitstun)
{
    s32 damage_level;

    if (hitstun < FTCOMMON_DAMAGE_LEVEL_HITSTUN_LOW)
    {
        damage_level = 0; // DamageX1
    }
    else if (hitstun < FTCOMMON_DAMAGE_LEVEL_HITSTUN_MID)
    {
        damage_level = 1; // DamageX2
    }
    else if (hitstun < FTCOMMON_DAMAGE_LEVEL_HITSTUN_HIGH)
    {
        damage_level = 2; // DamageX3
    }
    else damage_level = 3; // Fighter becomes airborne

    return damage_level;
}

// 0x80140B00
void ftCommonDamageSetPublic(FTStruct *this_fp, f32 knockback, f32 angle)
{
    GObj *attacker_gobj = ftParamGetPlayerNumGObj(this_fp->damage_player_num);
    sb32 is_force_curr_knockback;

    this_fp->status_vars.common.damage.public_knockback = knockback;
    this_fp->public_knockback = 0.0F;

    // By default, if knockback angle is between 75 and 115 degrees, the crowd is 20% less likely to gasp
    if ((angle > FTCOMMON_DAMAGE_PUBLIC_REACT_GASP_ANGLE_LOW) && (angle < FTCOMMON_DAMAGE_PUBLIC_REACT_GASP_ANGLE_HIGH))
    {
        this_fp->status_vars.common.damage.public_knockback *= FTCOMMON_DAMAGE_PUBLIC_REACT_GASP_KNOCKBACK_MUL;
    }
    if ((attacker_gobj != NULL) && (ftGetStruct(attacker_gobj)->public_knockback >= FTCOMMON_DAMAGE_KNOCKBACK_VERYHIGH))
    {
        is_force_curr_knockback = TRUE;
    }
    else is_force_curr_knockback = FALSE;

    ftPublicCommonCheck(this_fp->fighter_gobj, this_fp->status_vars.common.damage.public_knockback, is_force_curr_knockback);
}

// 0x80140BCC
sb32 ftCommonDamageCheckElementSetColAnim(GObj *fighter_gobj, s32 element, s32 damage_level)
{
    sb32 is_set_colanim;

    switch (element)
    {
    case nGMHitElementFire:
        is_set_colanim = ftParamCheckSetFighterColAnimID(fighter_gobj, damage_level + nGMColAnimFighterDamageFireStart, 0);
        break;

    case nGMHitElementElectric:
        is_set_colanim = ftParamCheckSetSkeletonColAnimID(fighter_gobj, damage_level);
        break;

    case nGMHitElementFreezing:
        is_set_colanim = ftParamCheckSetFighterColAnimID(fighter_gobj, damage_level + nGMColAnimFighterDamageIceStart, 0);
        break;

    default:
        is_set_colanim = ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterDamageCommon, 0);
        break;
    }
    return is_set_colanim;
}

// 0x80140C50
void ftCommonDamageCheckMakeScreenFlash(f32 knockback, s32 element)
{
    switch (element)
    {
    case nGMHitElementFire:
        if (knockback > FTCOMMON_DAMAGE_KNOCKBACK_VERYHIGH) 
        {
            ifScreenFlashSetColAnimID(nGMColAnimScreenFlashDamageFire, 0);
        }
        break;

    case nGMHitElementElectric:
        if (knockback > FTCOMMON_DAMAGE_KNOCKBACK_VERYHIGH)
        {
            ifScreenFlashSetColAnimID(nGMColAnimScreenFlashDamageElectric, 0);
        }
        break;

    case nGMHitElementFreezing:
        if (knockback > FTCOMMON_DAMAGE_KNOCKBACK_VERYHIGH)
        {
            ifScreenFlashSetColAnimID(nGMColAnimScreenFlashDamageIce, 0);
        }
        break;

    default:
        if (knockback > FTCOMMON_DAMAGE_KNOCKBACK_VERYHIGH)
        {
            ifScreenFlashSetColAnimID(nGMColAnimScreenFlashDamageNormal, 0);
        }
        break;
    }
}

// 0x80140D30
sb32 ftCommonDamageCheckCatchResist(FTStruct *fp)
{
    // Something to do with a fighter in Catch(Wait) being hit?

    if (fp->damage_element == nGMHitElementSleep)
    {
        return FALSE;
    }
    if (fp->damage_knockback == 0.0F)
    {
        return TRUE;
    }
    if ((fp->hitlag_tics > 0) && (fp->is_knockback_paused) && (fp->damage_knockback < (fp->damage_knockback_stack + 30.0F)))
    {
        return TRUE;
    }
    if ((fp->fkind == nFTKindDonkey) || (fp->fkind == nFTKindNDonkey) || (fp->fkind == nFTKindGDonkey))
    {
        if ((fp->status_id >= nFTDonkeyStatusThrowFStart) && (fp->status_id <= nFTDonkeyStatusThrowFEnd) && (ftCommonDamageGetDamageLevel(ftParamGetHitStun(fp->damage_knockback)) < 3))
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 0x80140E2C
void ftCommonDamageUpdateCatchResist(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->damage_knockback == 0.0F) || ((fp->hitlag_tics > 0) && (fp->is_knockback_paused) && (fp->damage_knockback < (fp->damage_knockback_stack + 30.0F))))
    {
        ftCommonDamageSetDamageColAnim(fighter_gobj);
    }
    else
    {
        ftParamStopVoiceRunProcDamage(fighter_gobj);
        ftDonkeyThrowFDamageSetStatus(fighter_gobj);
    }
}

// 0x80140EC0
sb32 ftCommonDamageCheckCaptureKeepHold(FTStruct *fp)
{
    if (fp->damage_queue < FTCOMMON_DAMAGE_CATCH_RELEASE_THRESHOLD)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80140EE4
void ftCommonDamageInitDamageVars(GObj *this_gobj, s32 status_id_replace, s32 damage, f32 knockback, s32 angle_start, s32 damage_lr,
s32 damage_index, s32 element, s32 damage_player_num, sb32 is_rumble, sb32 is_force_damage_sfx, sb32 is_allow_losecopy)
{
    FTStruct *this_fp = ftGetStruct(this_gobj);
    GObj *attacker_gobj;
    f32 angle_end = ftCommonDamageGetKnockbackAngle(angle_start, this_fp->ga, knockback);
    f32 vel_x = __cosf(angle_end) * knockback;
    s32 unused2;
    f32 vel_y = __sinf(angle_end) * knockback;
    f32 hitstun_tics = ftParamGetHitStun(knockback);
    s32 unused1;
    s32 damage_level;
    s32 status_id_set;
    s32 status_id_var;
    Vec3f vel_damage;
    f32 angle_diff;
    s32 unused3;

    this_fp->status_vars.common.damage.hitstun_tics = hitstun_tics;

    if (this_fp->status_vars.common.damage.hitstun_tics == 0)
    {
        this_fp->status_vars.common.damage.hitstun_tics = 1;
    }
    damage_level = ftCommonDamageGetDamageLevel(hitstun_tics);

    if (status_id_replace != -1)
    {
        damage_level = 3;
    }
    this_fp->lr = damage_lr;

    if (this_fp->ga == nMPKineticsAir)
    {
        status_id_var = status_id_set = dFTCommonDamageStatusAirIDs[damage_level][damage_index];

        this_fp->physics.vel_damage_air.x = -vel_x * this_fp->lr;
        this_fp->physics.vel_damage_air.y = vel_y;
        this_fp->physics.vel_damage_ground = 0.0F;
    }
    else
    {
        vel_damage.x = -vel_x * this_fp->lr;
        vel_damage.y = vel_y;
        vel_damage.z = 0.0F;

        angle_diff = syVectorAngleDiff3D(&this_fp->coll_data.floor_angle, &vel_damage);

        if (angle_diff < F_CST_DTOR32(90.0F))
        {
            status_id_var = status_id_set = dFTCommonDamageStatusGroundIDs[damage_level][damage_index];

            mpCommonSetFighterAir(this_fp);

            this_fp->physics.vel_damage_air.x = vel_damage.x;
            this_fp->physics.vel_damage_air.y = vel_damage.y;
            this_fp->physics.vel_damage_ground = 0.0F;
        }
        else if (damage_level == 3)
        {
            mpCommonSetFighterAir(this_fp);

            status_id_var = status_id_set = dFTCommonDamageStatusGroundIDs[damage_level][damage_index];

            if (angle_diff > F_CST_DTOR32(100.0F)) // 1.7453293F
            {
                this_fp->physics.vel_damage_air.x = vel_damage.x;
                this_fp->physics.vel_damage_air.y = -vel_damage.y * 0.8F;
                this_fp->physics.vel_damage_ground = 0.0F;

                ftParamMakeEffect(this_gobj, nEFKindImpactWave, nFTPartsJointTopN, NULL, NULL, this_fp->lr, 0, 0);
                ftParamMakeEffect(this_gobj, nEFKindQuakeMag0, nFTPartsJointTopN, NULL, NULL, this_fp->lr, 0, 0);
            }
            else
            {
                this_fp->physics.vel_damage_air.x = vel_damage.x;
                this_fp->physics.vel_damage_air.y = vel_damage.y;
                this_fp->physics.vel_damage_ground = 0.0F;
            }
        }
        else
        {
            status_id_var = status_id_set = dFTCommonDamageStatusGroundIDs[damage_level][damage_index];

            this_fp->physics.vel_damage_ground = (-vel_x * this_fp->lr);
            this_fp->physics.vel_damage_air.x = this_fp->coll_data.floor_angle.y * (-vel_x * this_fp->lr);
            this_fp->physics.vel_damage_air.y = -this_fp->coll_data.floor_angle.x * (-vel_x * this_fp->lr);
        }
    }
    this_fp->physics.vel_air.x = this_fp->physics.vel_air.y = this_fp->physics.vel_air.z = this_fp->physics.vel_ground.x = 0.0F;

    if ((damage_level == 3) && (this_fp->ga == nMPKineticsAir))
    {
        if ((angle_end > FTCOMMON_DAMAGE_FIGHTER_FLYTOP_ANGLE_LOW) && (angle_end < FTCOMMON_DAMAGE_FIGHTER_FLYTOP_ANGLE_HIGH))
        {
            status_id_var = status_id_set = nFTCommonStatusDamageFlyTop;
        }
        else if ((this_fp->percent_damage >= FTCOMMON_DAMAGE_FIGHTER_FLYROLL_DAMAGE_MIN) && (syUtilsRandFloat() < FTCOMMON_DAMAGE_FIGHTER_FLYROLL_RANDOM_CHANCE))
        {
            status_id_var = status_id_set = nFTCommonStatusDamageFlyRoll;           
        }
    }
    if (status_id_replace != -1)
    {
        status_id_set = status_id_replace;
    }

    if (((element == nGMHitElementElectric) && (status_id_set >= nFTCommonStatusDamageStart)) && (status_id_set <= nFTCommonStatusDamageEnd))
    {
        status_id_var = status_id_set;

        status_id_set = (damage_level == 3) ? nFTCommonStatusDamageE2 : nFTCommonStatusDamageE1;
    }
    this_fp->damage_player_num = damage_player_num;

    ftCommonDamageSetPublic(this_fp, knockback, angle_end);

    if (damage != 0)
    {
        ftCommonDamageCheckElementSetColAnim(this_gobj, element, damage_level);
    }
    ftCommonDamageCheckMakeScreenFlash(knockback, element);

    if ((damage_level == 3) && (is_allow_losecopy != FALSE))
    {
        ftKirbySpecialNDamageCheckLoseCopy(this_gobj);
    }
    ftMainSetStatus(this_gobj, status_id_set, 0.0F, 1.0F, FTSTATUS_PRESERVE_DAMAGEPLAYER);
    ftMainPlayAnimEventsAll(this_gobj);

    if (knockback >= 65000.0F)
    {
        this_fp->status_vars.common.damage.is_knockback_over = TRUE;
    }
    else this_fp->status_vars.common.damage.is_knockback_over = FALSE;
    
    if ((this_fp->status_id == nFTCommonStatusDamageE1) || (this_fp->status_id == nFTCommonStatusDamageE2))
    {
        this_fp->proc_passive = ftCommonDamageSetStatus;
        this_fp->status_vars.common.damage.status_id = status_id_var;
    }
    else this_fp->proc_passive = ftCommonDamageCheckSetInvincible;
    
    this_fp->proc_lagupdate = ftCommonDamageCommonProcLagUpdate;

    this_fp->tap_stick_x = this_fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;

    this_fp->damage_knockback_stack = knockback;

    if ((damage_level == 3) || (is_rumble != FALSE))
    {
        ftParamMakeRumble(this_fp, 2, 0);
    }
    if (this_fp->status_id == nFTCommonStatusDamageFlyRoll)
    {
        ftCommonDamageFlyRollUpdateModelPitch(this_gobj);
    }
    ftCommonDamageSetDustEffectInterval(this_fp);

    if (this_fp->status_vars.common.damage.dust_effect_int != 0)
    {
        this_fp->status_vars.common.damage.dust_effect_int = 1;
    }
    if ((hitstun_tics >= FTCOMMON_DAMAGE_FIGHTER_DAMAGEVOICE_MIN) && (this_fp->attr->damage_sfx != nSYAudioFGMVoiceEnd) || (is_force_damage_sfx != FALSE))
    {
        func_800269C0_275C0(this_fp->attr->damage_sfx);
    }

    this_fp->is_hitstun = TRUE;
    this_fp->tics_since_last_z = FTINPUT_ZTRIGLAST_TICS_MAX;

    if ((damage_level == 3) && (knockback >= FTCOMMON_DAMAGE_FIGHTER_PLAYERTAG_KNOCKBACK_MIN))
    {
        ftParamSetPlayerTagWait(this_gobj, FTCOMMON_DAMAGE_FIGHTER_PLAYERTAG_HIDE_FRAMES);
    }
    this_fp->status_vars.common.damage.coll_mask_curr = 0;

    attacker_gobj = ftParamGetPlayerNumGObj(damage_player_num);

    if (attacker_gobj != NULL)
    {
        FTStruct *attacker_fp = ftGetStruct(attacker_gobj);

        attacker_fp->attack_count++;
        attacker_fp->attack_knockback = knockback;
    }
}

// 0x80141560 - Enter sleep or common damage state
void ftCommonDamageGotoDamageStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->is_cliff_hold)
    {
        fp->cliffcatch_wait = FTCOMMON_CLIFF_CATCH_WAIT;
    }
    if (fp->damage_element == nGMHitElementSleep)
    {
        ftCommonFuraSleepSetStatus(fighter_gobj);
    }
    else ftCommonDamageInitDamageVars(fighter_gobj, -1, fp->damage_queue, fp->damage_knockback, fp->damage_angle, fp->damage_lr, fp->damage_index, fp->damage_element, fp->damage_player_num, FALSE, FALSE, TRUE);
}

// 0x801415F8
void ftCommonDamageUpdateDamageColAnim(GObj *fighter_gobj, f32 knockback, s32 element)
{
    if (ftCommonDamageCheckElementSetColAnim(fighter_gobj, element, ftCommonDamageGetDamageLevel(ftParamGetHitStun(knockback))) != 0)
    {
        ftMainRunUpdateColAnim(fighter_gobj);
    }
}

// 0x80141648
void ftCommonDamageSetDamageColAnim(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonDamageUpdateDamageColAnim(fighter_gobj, fp->damage_knockback, fp->damage_element);
}

// 0x80141670
void ftCommonDamageUpdateMain(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *grab_gobj = this_fp->catch_gobj;
    FTStruct *grab_fp;

    if (grab_gobj != NULL)
    {
        grab_fp = ftGetStruct(grab_gobj);

        if (ftCommonDamageCheckCatchResist(this_fp) != FALSE)
        {
            if (grab_fp->damage_knockback != 0)
            {
                if (ftCommonDamageCheckCaptureKeepHold(grab_fp) != FALSE)
                {
                    grab_fp->damage_lag = this_fp->damage_lag;
                    grab_fp->hitlag_mul = this_fp->hitlag_mul;

                    ftCommonDamageUpdateCatchResist(fighter_gobj);

                    grab_fp->damage_kind = nFTDamageKindColAnim;
                }
                else
                {
                    ftCommonThrownDecideFighterLoseGrip(fighter_gobj, grab_gobj);
                    ftParamStopVoiceRunProcDamage(fighter_gobj);
                    ftCommonDamageGotoDamageStatus(fighter_gobj);

                    grab_fp->damage_kind = nFTDamageKindStatus;
                }
            }
            else ftCommonDamageUpdateCatchResist(fighter_gobj);
        }
        else if (grab_fp->damage_knockback != 0)
        {
            if (ftCommonDamageCheckCaptureKeepHold(grab_fp) != FALSE)
            {
                ftCommonThrownUpdateDamageStats(grab_fp);
            }
            ftCommonThrownDecideFighterLoseGrip(fighter_gobj, grab_gobj);
            ftParamStopVoiceRunProcDamage(fighter_gobj);
            ftCommonDamageGotoDamageStatus(fighter_gobj);

            grab_fp->damage_kind = nFTDamageKindStatus;
        }
        else
        {
            ftParamStopVoiceRunProcDamage(grab_gobj);
            ftCommonThrownSetStatusDamageRelease(grab_gobj);

            if (this_fp->is_catch_or_capture)
            {
                ftCommonThrownReleaseFighterLoseGrip(fighter_gobj);
            }
            this_fp->catch_gobj = NULL;

            ftParamStopVoiceRunProcDamage(fighter_gobj);
            ftCommonDamageGotoDamageStatus(fighter_gobj);
        }
        return;
    }
    else grab_gobj = this_fp->capture_gobj;

    if (grab_gobj != NULL)
    {
        grab_fp = ftGetStruct(grab_gobj);

        if (ftCommonDamageCheckCaptureKeepHold(this_fp) != FALSE)
        {
            if (grab_fp->damage_knockback != 0)
            {
                if (ftCommonDamageCheckCatchResist(grab_fp) != FALSE)
                {
                    this_fp->damage_lag = grab_fp->damage_lag;
                    this_fp->hitlag_mul = grab_fp->hitlag_mul;
                    grab_fp->damage_kind = nFTDamageKindCatch;

                    ftCommonDamageSetDamageColAnim(fighter_gobj);
                }
                else
                {
                    ftCommonThrownUpdateDamageStats(this_fp);
                    ftCommonThrownDecideFighterLoseGrip(grab_gobj, fighter_gobj);
                    ftParamStopVoiceRunProcDamage(fighter_gobj);
                    ftCommonDamageGotoDamageStatus(fighter_gobj);

                    grab_fp->damage_kind = nFTDamageKindStatus;
                }
            }
            else
            {
                grab_fp->hitlag_tics = ftParamGetHitLag(this_fp->damage_lag, grab_fp->status_id, grab_fp->hitlag_mul);

                this_fp->input.pl.button_tap = this_fp->input.pl.button_release = 0;

                if (this_fp->proc_lagstart != NULL)
                {
                    this_fp->proc_lagstart(fighter_gobj);
                }
                ftCommonDamageSetDamageColAnim(fighter_gobj);
            }
        }
        else if (grab_fp->damage_knockback != 0)
        {
            ftCommonThrownDecideFighterLoseGrip(grab_gobj, fighter_gobj);
            ftParamStopVoiceRunProcDamage(fighter_gobj);
            ftCommonDamageGotoDamageStatus(fighter_gobj);

            grab_fp->damage_kind = nFTDamageKindStatus;
        }
        else
        {
            ftCommonThrownDecideFighterLoseGrip(grab_gobj, fighter_gobj);
            ftParamStopVoiceRunProcDamage(fighter_gobj);
            ftCommonDamageGotoDamageStatus(fighter_gobj);
            ftParamStopVoiceRunProcDamage(grab_gobj);
            ftCommonThrownSetStatusNoDamageRelease(grab_gobj);
        }
        return;
    }
    if (this_fp->item_gobj != NULL)
    {
        if ((itGetStruct(this_fp->item_gobj)->weight == nITWeightHeavy) && ((this_fp->fkind == nFTKindDonkey) || (this_fp->fkind == nFTKindNDonkey) || (this_fp->fkind == nFTKindGDonkey)))
        {
            if (ftCommonDamageCheckCatchResist(this_fp) != FALSE)
            {
                ftCommonDamageUpdateCatchResist(fighter_gobj);
            }
            else
            {
                ftSetupDropItem(this_fp);
                ftParamStopVoiceRunProcDamage(fighter_gobj);
                ftCommonDamageGotoDamageStatus(fighter_gobj);
            }
            return;
        }
    }
    if ((this_fp->damage_element != nGMHitElementSleep) && ((this_fp->damage_knockback == 0.0F) || ((this_fp->hitlag_tics > 0) && (this_fp->is_knockback_paused) && (this_fp->damage_knockback < (this_fp->damage_knockback_stack + 30.0F)))))
    {
        ftCommonDamageSetDamageColAnim(fighter_gobj);
    }
    else
    {
        ftParamStopVoiceRunProcDamage(fighter_gobj);
        ftCommonDamageGotoDamageStatus(fighter_gobj);
    }
}
