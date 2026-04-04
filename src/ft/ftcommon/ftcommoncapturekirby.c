#include <ft/fighter.h>
#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188700
FTThrowReleaseDesc dFTCommonCaptureKirbyKnockbackCatch = { 361, 100, 90, 0 };

// 0x80188710
FTThrowReleaseDesc dFTCommonCaptureKirbyKnockbackCapture = { 80, 100, 60, 0 };

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014B700
void ftCommonCaptureKirbyUpdatePositionsMag(GObj *fighter_gobj, Vec3f *dist)
{
    f32 mag = syVectorMag3D(dist);

    if (mag < FTCOMMON_CAPTUREKIRBY_MAGNITUDE_MAX)
    {
        DObj *joint = DObjGetStruct(fighter_gobj)->child;

        mag /= FTCOMMON_CAPTUREKIRBY_MAGNITUDE_MAX;
        mag *= FTCOMMON_CAPTUREKIRBY_MAGNITUDE_MUL;

        joint->scale.vec.f.x = joint->scale.vec.f.y = joint->scale.vec.f.z = FTCOMMON_CAPTUREKIRBY_MAGNITUDE_ADD + mag;
    }
}

// 0x8014B774
void ftCommonCaptureKirbyUpdatePositionsAll(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    Vec3f dist;
    FTStruct *capture_fp = ftGetStruct(this_fp->capture_gobj);

    dist.x = capture_fp->status_vars.kirby.specialn.dist.x;
    dist.y = capture_fp->status_vars.kirby.specialn.dist.y;
    dist.z = 0;

    ftCommonCaptureKirbyUpdatePositionsMag(fighter_gobj, &dist);

    if (ABSF(dist.x) > FTCOMMON_CAPTUREKIRBY_DIST_X_MIN)
    {
        dist.x = ((dist.x < 0.0F) ? -1 : +1) * FTCOMMON_CAPTUREKIRBY_DIST_X_MIN;
    }
    if (ABSF(dist.y) > FTCOMMON_CAPTUREKIRBY_DIST_Y_MIN)
    {
        dist.y = ((dist.y < 0.0F) ? -1 : +1) * FTCOMMON_CAPTUREKIRBY_DIST_Y_MIN;
    }
    capture_fp->status_vars.kirby.specialn.dist.x -= dist.x;
    capture_fp->status_vars.kirby.specialn.dist.y -= dist.y;

    ftKirbySpecialNAddCaptureDistance(capture_fp, this_fp->capture_gobj, &dist);

    DObjGetStruct(fighter_gobj)->translate.vec.f.x = capture_fp->status_vars.kirby.specialn.dist.x + dist.x;
    DObjGetStruct(fighter_gobj)->translate.vec.f.y = capture_fp->status_vars.kirby.specialn.dist.y + dist.y;
    DObjGetStruct(fighter_gobj)->translate.vec.f.z = DObjGetStruct(this_fp->capture_gobj)->translate.vec.f.z;
}

// 0x8014B914
void ftCommonCaptureKirbyProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonCaptureKirbyUpdatePositionsAll(fighter_gobj);

    if ((fp->status_id == nFTCommonStatusCaptureKirby) && (fp->status_vars.common.capturekirby.is_goto_capturewait != FALSE))
    {
        ftCommonCaptureWaitKirbySetStatus(fighter_gobj);
    }
}

// 0x8014B968
void ftCommonCaptureKirbyProcCapture(GObj *fighter_gobj, GObj *capture_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *capture_fp;

    ftParamStopVoiceRunProcDamage(fighter_gobj);

    if ((this_fp->item_gobj != NULL) && (itGetStruct(this_fp->item_gobj)->weight == nITWeightHeavy))
    {
        ftSetupDropItem(this_fp);
    }
    if (this_fp->catch_gobj != NULL)
    {
        ftCommonThrownSetStatusDamageRelease(this_fp->catch_gobj);

        this_fp->catch_gobj = NULL;
    }
    else if (this_fp->capture_gobj != NULL)
    {
        ftCommonThrownDecideFighterLoseGrip(this_fp->capture_gobj, fighter_gobj);
    }
    this_fp->is_catch_or_capture = FALSE;

    this_fp->capture_gobj = capture_gobj;

    capture_fp = ftGetStruct(capture_gobj);

    this_fp->lr = -capture_fp->lr;

    mpCommonSetFighterAir(this_fp);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCaptureKirby, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftParamMakeRumble(this_fp, 7, 0);

    this_fp->status_vars.common.capturekirby.is_goto_capturewait = FALSE;
    this_fp->status_vars.common.capturekirby.lr = 0;
    this_fp->status_vars.common.capturekirby.is_kirby = FALSE;

    ftParamSetCaptureImmuneMask(this_fp, FTCATCHKIND_MASK_ALL);
    ftPhysicsStopVelAll(fighter_gobj);
    ftCommonCaptureKirbyProcPhysics(fighter_gobj);
    mpCommonUpdateFighterProjectFloor(fighter_gobj);
}

// 0x8014BA98
void ftCommonCaptureWaitKirbyUpdateBreakoutVars(FTStruct *this_fp, FTStruct *capture_fp)
{
    sb32 is_wiggle = FALSE;

    if ((capture_fp->status_id == nFTKirbyStatusSpecialAirNWait) || (capture_fp->status_id == nFTKirbyStatusSpecialNWait))
    {
        if ((this_fp->input.pl.stick_range.y >= FTCOMMON_CAPTUREKIRBY_WIGGLE_STICK_RANGE_MIN) && (this_fp->tap_stick_y < FTCOMMON_CAPTUREKIRBY_WIGGLE_BUFFER_TICS_MAX))
        {
            this_fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
            is_wiggle = TRUE;

            if (capture_fp->ga == nMPKineticsGround)
            {
                ftKirbySpecialNWaitSwitchStatusAir(capture_fp->fighter_gobj);

                is_wiggle = TRUE;

                capture_fp->physics.vel_air.y = FTCOMMON_CAPTUREKIRBY_WIGGLE_VEL;
            }
        }
        else if ((this_fp->input.pl.stick_range.y <= -FTCOMMON_CAPTUREKIRBY_WIGGLE_STICK_RANGE_MIN) && (this_fp->tap_stick_y < FTCOMMON_CAPTUREKIRBY_WIGGLE_BUFFER_TICS_MAX) && (this_fp->coll_data.floor_flags & 0x4000))
        {
            this_fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
            is_wiggle = TRUE;

            if (capture_fp->ga == nMPKineticsGround)
            {
                ftKirbySpecialNWaitSwitchStatusAir(capture_fp->fighter_gobj);

                is_wiggle = TRUE;

                capture_fp->coll_data.ignore_line_id = capture_fp->coll_data.floor_line_id;
                capture_fp->physics.vel_air.y = -FTCOMMON_CAPTUREKIRBY_WIGGLE_VEL;
            }
        }
        if ((ABS(this_fp->input.pl.stick_range.x) >= (FTCOMMON_CAPTUREKIRBY_WIGGLE_STICK_RANGE_MIN + 3)) && (this_fp->tap_stick_x < (FTCOMMON_CAPTUREKIRBY_WIGGLE_BUFFER_TICS_MAX * 2)))
        {
            this_fp->tap_stick_x = FTINPUT_STICKBUFFER_TICS_MAX;
            is_wiggle = TRUE;

            if (capture_fp->ga == nMPKineticsGround)
            {
                capture_fp->physics.vel_ground.x = (((this_fp->input.pl.stick_range.x < 0) ? -1 : 1) * capture_fp->lr) * FTCOMMON_CAPTUREKIRBY_WIGGLE_VEL;
            }
            else capture_fp->physics.vel_air.x = ((this_fp->input.pl.stick_range.x < 0) ? -1 : 1) * FTCOMMON_CAPTUREKIRBY_WIGGLE_VEL;
        }
        if (is_wiggle != FALSE)
        {
            if (capture_fp->figatree != NULL)
            {
                lbCommonAddFighterPartsFigatree(capture_fp->joints[nFTPartsJointTopN]->child, capture_fp->figatree, 0.0F);
            }
        }
    }
}

// 0x8014BC94
void ftCommonCaptureWaitKirbyProcMap(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *capture_gobj = this_fp->capture_gobj;
    FTStruct *capture_fp = ftGetStruct(capture_gobj);
    Vec3f *this_translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;

    *this_translate = DObjGetStruct(capture_gobj)->translate.vec.f;

    if (capture_fp->lr == +1)
    {
        this_translate->x += 10.0F;
    }
    else this_translate->x -= 10.0F;
}

// 0x8014BD04
void ftCommonCaptureWaitKirbyProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *capture_gobj = this_fp->capture_gobj;

    if (capture_gobj != NULL)
    {
        FTStruct *capture_fp = ftGetStruct(capture_gobj);
        s32 breakout_wait;

        ftCommonCaptureWaitKirbyUpdateBreakoutVars(this_fp, capture_fp);

        breakout_wait = this_fp->breakout_wait;

        ftCommonCaptureTrappedUpdateBreakoutVars(this_fp);

        this_fp->breakout_wait -= ((breakout_wait - this_fp->breakout_wait) * 5);

        this_fp->breakout_wait--;

        if (this_fp->breakout_wait <= 0)
        {
            ftCommonCaptureApplyCatchKnockback(capture_gobj, &dFTCommonCaptureKirbyKnockbackCatch); // Deal knockback to player who caught this fighter
            ftCommonCaptureApplyCaptureKnockback(fighter_gobj, &dFTCommonCaptureKirbyKnockbackCapture); // Deal knockback to this fighter

            capture_fp->catch_gobj = NULL;
            this_fp->capture_gobj = NULL;
        }
    }
}

// 0x8014BDB4
void ftCommonCaptureWaitKirbySetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCaptureWaitKirby, 0.0F, 1.0F, (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_MODELPART));
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    fp->is_invisible = TRUE;

    ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusIntangible);
    ftCommonCaptureTrappedInitBreakoutVars(fp, 500);
}

// 0x8014BE24
void ftCommonThrownKirbyEscape(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTCommonStatusFall, 0.0F, 1.0F, FTSTATUS_PRESERVE_DAMAGEPLAYER);
}

// 0x8014BE54
void ftCommonThrownCommonStarProcHit(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusFall, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x8014BE90
void ftCommonThrownKirbyStarMakeEffect(GObj *fighter_gobj, f32 arg1, f32 arg2)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->is_effect_attach))
    {
        if (efManagerCaptureKirbyStarMakeEffect(fighter_gobj) != NULL)
        {
            fp->is_effect_attach = TRUE;
        }
    }
}

// 0x8014BEE0
void ftCommonThrownKirbyStarProcUpdate(GObj *fighter_gobj)
{
    ftCommonThrownKirbyStarMakeEffect(fighter_gobj, 360.0F, 210.0F);
}

// 0x8014BF04
void ftCommonThrownCommonStarUpdatePhysics(GObj *fighter_gobj, f32 decelerate)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->motion_vars.flags.flag1 != 0) && (fp->motion_vars.flags.flag2 != 3))
    {
        f32 sqrt_vel = sqrtf(SQUARE(fp->physics.vel_air.x) + SQUARE(fp->physics.vel_air.y));

        if (decelerate < sqrt_vel)
        {
            fp->physics.vel_air.x = (fp->physics.vel_air.x * (sqrt_vel - decelerate)) / sqrt_vel;
            fp->physics.vel_air.y = (fp->physics.vel_air.y * (sqrt_vel - decelerate)) / sqrt_vel;

            if (fp->physics.vel_air.y < 0)
            {
                fp->lr = -1;
            }
            else fp->lr = +1;
        }
        else fp->physics.vel_air.x = fp->physics.vel_air.y = 0;

        fp->motion_vars.flags.flag1--;

        ftCommonCaptureTrappedUpdateBreakoutVars(fp);

        if (fp->breakout_wait <= 0)
        {
            fp->motion_vars.flags.flag1 = 0;
        }
    }
    else
    {
        if (fp->motion_vars.flags.flag2 == 1)
        {
            if (((fp->fkind == nFTKindKirby) || (fp->fkind == nFTKindNKirby)) && (fp->status_vars.common.capturekirby.is_kirby != FALSE))
            {
                ftKirbySpecialNInitPassiveVars(fp);

                fp->passive_vars.kirby.copy_id = nFTKindKirby;

                ftParamSetModelPartDefaultID(fighter_gobj, 6, 0);
                ftParamResetModelPartAll(fighter_gobj);
            }
            fp->is_invisible = FALSE;

            ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusNormal);
            ftParamProcStopEffect(fighter_gobj);
            ftCommonThrownKirbyEscape(fighter_gobj);

            if (efManagerStarSplashMakeEffect(&DObjGetStruct(fighter_gobj)->translate.vec.f, (-fp->physics.vel_air.x < 0.0F) ? -1 : +1) != NULL)
            {
                fp->is_effect_attach = TRUE;
            }
        }
        else
        {
            if (fp->motion_vars.flags.flag2 == 2)
            {
                fp->motion_vars.flags.flag1 = 4;
                fp->motion_vars.flags.flag2 = 3;

                fp->physics.vel_air.y = FTCOMMON_THROWNKIRBYSTAR_RELEASE_VEL_Y;

                if (fp->status_vars.common.capturekirby.lr != 0)
                {
                    fp->physics.vel_air.x = fp->status_vars.common.capturekirby.lr * FTCOMMON_THROWNKIRBYSTAR_RELEASE_VEL_X;
                }
                else fp->physics.vel_air.x = ((fp->physics.vel_air.x < 0.0F) ? -1 : +1) * FTCOMMON_THROWNKIRBYSTAR_RELEASE_VEL_X;

                ftParamProcStopEffect(fighter_gobj);

                if (efManagerStarSplashMakeEffect(&DObjGetStruct(fighter_gobj)->translate.vec.f, (-fp->physics.vel_air.x < 0.0F) ? -1 : +1) != NULL)
                {
                    fp->is_effect_attach = TRUE;
                }
                fp->is_invisible = FALSE;

                ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusNormal);
            }
            if (fp->motion_vars.flags.flag2 == 3)
            {
                DObj *joint = DObjGetStruct(fighter_gobj)->child;
                f32 scale = (4.0F - fp->motion_vars.flags.flag1) * 0.25F;

                scale *= FTCOMMON_CAPTUREKIRBY_MAGNITUDE_MUL;

                joint->scale.vec.f.x = joint->scale.vec.f.y = joint->scale.vec.f.z = FTCOMMON_CAPTUREKIRBY_MAGNITUDE_ADD + scale;

                if (fp->motion_vars.flags.flag1-- <= 0)
                {
                    ftCommonThrownKirbyEscape(fighter_gobj);
                }
            }
        }
    }
}

// 0x8014C260
void ftCommonThrownKirbyStarProcPhysics(GObj *fighter_gobj)
{
    ftCommonThrownCommonStarUpdatePhysics(fighter_gobj, FTCOMMON_THROWNKIRBYSTAR_DECELERATE);
}

// 0x8014C280
void ftCommonThrownCommonStarProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f *angle;
    Vec3f pos = DObjGetStruct(fighter_gobj)->translate.vec.f;
    Vec3f vel_bak;

    angle = NULL;

    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        angle = &fp->coll_data.floor_angle;
    }
    else if (fp->coll_data.mask_curr & MAP_FLAG_CEIL)
    {
        angle = &fp->coll_data.ceil_angle;

        pos.y += fp->coll_data.map_coll.top;
    }
    else if (fp->coll_data.mask_curr & MAP_FLAG_LWALL)
    {
        angle = &fp->coll_data.lwall_angle;

        pos.x += fp->coll_data.map_coll.width;
        pos.y += fp->coll_data.map_coll.center;
    }
    else if (fp->coll_data.mask_curr & MAP_FLAG_RWALL)
    {
        angle = &fp->coll_data.rwall_angle;

        pos.x -= fp->coll_data.map_coll.width;
        pos.y += fp->coll_data.map_coll.center;
    }
    if (angle != NULL)
    {
        vel_bak = fp->physics.vel_air;

        lbCommonReflect2D(&fp->physics.vel_air, angle);

        if (((fp->physics.vel_air.x * vel_bak.x) + (vel_bak.y * fp->physics.vel_air.y)) < 0.0F)
        {
            fp->status_vars.common.capturekirby.lr = (angle->x < 0) ? -1 : +1;

            fp->motion_vars.flags.flag1 = 0;
        }
        efManagerQuakeMakeEffect(2);
        efManagerImpactWaveMakeEffect(&pos, SYVECTOR_AXIS_Z, syUtilsArcTan2(-angle->x, angle->y));
    }
}

// 0x8014C424
void ftCommonThrownKirbyStarInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *capture_gobj = this_fp->capture_gobj;

    if (capture_gobj != NULL)
    {
        FTStruct *capture_fp = ftGetStruct(capture_gobj);

        ftCommonThrownReleaseFighterLoseGrip(fighter_gobj);
        ftParamSetCaptureImmuneMask(this_fp, FTCATCHKIND_MASK_NONE);
        ftParamSetCaptureImmuneMask(capture_fp, FTCATCHKIND_MASK_NONE);
        ftParamUpdate1PGameDamageStats(this_fp, capture_fp->player, nFTHitLogObjectFighter, capture_fp->fkind, capture_fp->stat_flags.halfword, capture_fp->stat_count);

        this_fp->capture_gobj = NULL;
        this_fp->catch_gobj = NULL;
        capture_fp->catch_gobj = NULL;
        capture_fp->capture_gobj = NULL;

        this_fp->status_vars.common.capturekirby.lr = (this_fp->physics.vel_air.x < 0.0F) ? -1 : +1;
    }
}

// 0x8014C4D8
void ftCommonThrownKirbyStarProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 30;
    fp->motion_vars.flags.flag2 = 2;

    ftCommonThrownKirbyStarInitStatusVars(fighter_gobj);
}

// 0x8014C508
void ftCommonThrownKirbyStarSetStatus(GObj *fighter_gobj)
{
    s32 i;
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTKirbyCopy *copy = lbRelocGetFileData(FTKirbyCopy*, gFTDataKirbyMainMotion, &llKirbyMainMotionSpecialNFTKirbyCopy);

    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterAir(fp);
    }
    fp->proc_status = ftCommonThrownKirbyStarProcStatus;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusThrownKirbyStar, 0.0F, 1.0F, FTSTATUS_PRESERVE_THROWPOINTER);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    fp->proc_hit = ftCommonThrownCommonStarProcHit;

    for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
    {
        FTAttackColl *attack_coll = &fp->attack_colls[i];

        if (attack_coll->attack_state == nGMAttackStateNew)
        {
            attack_coll->damage = copy[fp->fkind].star_damage;
        }
    }
    fp->is_invisible = fp->is_shadow_hide = TRUE;

    ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusIntangible);
    ftParamSetPlayerTagWait(fighter_gobj, 1);
    ftCommonCaptureTrappedInitBreakoutVars(fp, FTCOMMON_THROWNKIRBYSTAR_BREAKOUT_INPUTS_MIN);
}

// 0x8014C634
void ftCommonThrownCopyStarProcUpdate(GObj *fighter_gobj)
{
    ftCommonThrownKirbyStarMakeEffect(fighter_gobj, -400.0F, 280.0F);
}

// 0x8014C658
void ftCommonThrownCopyStarProcPhysics(GObj *fighter_gobj)
{
    ftCommonThrownCommonStarUpdatePhysics(fighter_gobj, FTCOMMON_THROWNCOPYSTAR_DECELERATE);
}

// 0x8014C67C
void ftCommonThrownCopyStarProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 10;
    fp->motion_vars.flags.flag2 = 1;

    ftCommonThrownKirbyStarInitStatusVars(fighter_gobj);
}

// 0x8014C6AC
void ftCommonThrownCopyStarSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterAir(fp);
    }
    fp->proc_status = ftCommonThrownCopyStarProcStatus;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusThrownCopyStar, 0.0F, 1.0F, FTSTATUS_PRESERVE_THROWPOINTER);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    fp->proc_hit = ftCommonThrownCommonStarProcHit;
    fp->is_invisible = fp->is_shadow_hide = TRUE;

    ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusIntangible);
    ftParamSetPlayerTagWait(fighter_gobj, 1);
}
