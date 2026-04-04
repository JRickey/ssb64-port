#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015BD00
void ftFoxSpecialHiStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftFoxSpecialHiHoldSetStatus);
}

// 0x8015BD24
void ftFoxSpecialAirHiStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftFoxSpecialAirHiHoldSetStatus);
}

// 0x8015BD48
void ftFoxSpecialAirHiStartProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->status_vars.fox.specialhi.gravity_delay != 0)
    {
        fp->status_vars.fox.specialhi.gravity_delay--;
    }
    else ftPhysicsApplyGravityClampTVel(fp, 0.5F, attr->tvel_base);
    
    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x8015BDC0
void ftFoxSpecialHiStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftFoxSpecialHiStartSwitchStatusAir);
}

// 0x8015BDE4
void ftFoxSpecialAirHiStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftFoxSpecialAirHiStartSwitchStatusGround);
}

// 0x8015BE08
void ftFoxSpecialAirHiStartSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHiStart, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_COLANIM);
}

// 0x8015BE48
void ftFoxSpecialHiStartSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHiStart, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_COLANIM);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x8015BE94
void ftFoxSpecialHiHoldProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.fox.specialhi.launch_delay--;

    if (fp->status_vars.fox.specialhi.launch_delay == 0)
    {
        if (fp->ga == nMPKineticsAir)
        {
            ftFoxSpecialAirHiSetStatusFromGround(fighter_gobj);
        }
        else ftFoxSpecialHiDecideSetStatus(fighter_gobj);
    }
}

// 0x8015BEE8
void ftFoxSpecialHiHoldProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftFoxSpecialHiHoldSwitchStatusAir);
}

// 0x8015BF0C
void ftFoxSpecialAirHiHoldProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftFoxSpecialAirHiHoldSwitchStatusGround);
}

// 0x8015BF30
void ftFoxSpecialAirHiHoldSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHiHold, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_COLANIM));
}

// 0x8015BF70
void ftFoxSpecialHiHoldSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHiHold, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_COLANIM));
    ftPhysicsClampAirVelXMax(fp);
}

// 0x8015BFBC
void ftFoxSpecialHiHoldInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    fp->status_vars.fox.specialhi.launch_delay = FTFOX_FIREFOX_LAUNCH_DELAY;
}

// 0x8015BFCC
void ftFoxSpecialHiHoldSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHiHold, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftFoxSpecialHiHoldInitStatusVars(fighter_gobj);
}

// 0x8015C010
void ftFoxSpecialAirHiHoldSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHiHold, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftFoxSpecialHiHoldInitStatusVars(fighter_gobj);
}

// 0x8015C054
void ftFoxSpecialHiUpdateModelPitch(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->joints[4]->rotate.vec.f.x = (syUtilsArcTan2(fp->physics.vel_air.x, fp->physics.vel_air.y) * fp->lr) - F_CST_DTOR32(90.0F);
    ftParamsUpdateFighterPartsTransformAll(fp->joints[4]);
}

// 0x8015C0B4
void ftFoxSpecialHiProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.fox.specialhi.anim_frames--;

    if (fp->status_vars.fox.specialhi.anim_frames == 0)
    {
        if (fp->ga == nMPKineticsAir)
        {
            ftFoxSpecialAirHiEndSetStatus(fighter_gobj);
        }
        else ftFoxSpecialHiEndSetStatus(fighter_gobj);
    }
}

// 0x8015C108
void ftFoxSpecialHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.fox.specialhi.decelerate_wait++;

    if (fp->status_vars.fox.specialhi.decelerate_wait >= FTFOX_FIREFOX_DECELERATE_DELAY)
    {
        ftPhysicsSetGroundVelFriction(fp, FTFOX_FIREFOX_DECELERATE_VEL);
    }
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x8015C15C
void ftFoxSpecialAirHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.fox.specialhi.decelerate_wait++;

    if (fp->status_vars.fox.specialhi.decelerate_wait >= FTFOX_FIREFOX_DECELERATE_DELAY)
    {
        fp->physics.vel_air.x -= (FTFOX_FIREFOX_DECELERATE_VEL * __cosf(fp->status_vars.fox.specialhi.angle) * fp->lr);
        fp->physics.vel_air.y -= (FTFOX_FIREFOX_DECELERATE_VEL * __sinf(fp->status_vars.fox.specialhi.angle));
    }
    ftFoxSpecialHiUpdateModelPitch(fighter_gobj);
}

// 0x8015C1F4
void ftFoxSpecialHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.fox.specialhi.pass_timer++;

    if (mpCommonProcFighterOnFloor(fighter_gobj, ftFoxSpecialAirHiSetStatus) != FALSE)
    {
        fp->status_vars.fox.specialhi.angle = syUtilsArcTan2(-fp->coll_data.floor_angle.x * fp->lr, fp->coll_data.floor_angle.y);
    }
}

// 0x8015C264
sb32 ftFoxSpecialHiProcPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) || (fp->status_vars.fox.specialhi.pass_timer >= 15))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8015C29C
void ftFoxSpecialAirHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    u16 mask;

    fp->status_vars.fox.specialhi.pass_timer++;

    if (mpCommonCheckFighterPass(fighter_gobj, ftFoxSpecialHiProcPass) != FALSE)
    {
        mask = (fp->coll_data.mask_prev ^ fp->coll_data.mask_curr) & fp->coll_data.mask_curr & MAP_FLAG_FLOOR;

        if (!(mask & MAP_FLAG_FLOOR) || (lbCommonCheckAdjustSim2D(&fp->physics.vel_air, &fp->coll_data.floor_angle, FTFOX_FIREFOX_BOUND_ANGLE) == FALSE))
        {
            if (syVectorAngleDiff3D(&fp->coll_data.floor_angle, &fp->physics.vel_air) > F_CST_DTOR32(110.0F)) // 1.9198622F
            {
                ftFoxSpecialAirHiBoundSetStatus(fighter_gobj);
            }
            else ftFoxSpecialHiEndSetStatus(fighter_gobj);

            return;
        }
        goto coll_end;
    }
    mask = (fp->coll_data.mask_prev ^ fp->coll_data.mask_curr) & fp->coll_data.mask_curr & (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL);

    if (mask & MAP_FLAG_CEIL)
    {
        if (lbCommonCheckAdjustSim2D(&fp->physics.vel_air, &fp->coll_data.ceil_angle, FTFOX_FIREFOX_BOUND_ANGLE) != FALSE)
        {
            goto coll_end;
        }
    }
    else if (mask & MAP_FLAG_LWALL)
    {
        if (lbCommonCheckAdjustSim2D(&fp->physics.vel_air, &fp->coll_data.lwall_angle, FTFOX_FIREFOX_BOUND_ANGLE) != FALSE)
        {
            goto coll_end;
        }
    }
    else if ((mask & MAP_FLAG_RWALL) && (lbCommonCheckAdjustSim2D(&fp->physics.vel_air, &fp->coll_data.rwall_angle, FTFOX_FIREFOX_BOUND_ANGLE) != FALSE))
    {
    coll_end:
        fp->lr = (fp->physics.vel_air.x >= 0.0F) ? +1 : -1;

        fp->joints[nFTPartsJointTopN]->rotate.vec.f.y = fp->lr * F_CST_DTOR32(90.0F);

        fp->status_vars.fox.specialhi.angle = syUtilsArcTan2(fp->physics.vel_air.y, fp->physics.vel_air.x * fp->lr);

        ftFoxSpecialHiUpdateModelPitch(fighter_gobj);
    }
}

// 0x8015C46C
void ftFoxSpecialAirHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHi, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_COLANIM);

    fp->jumps_used = attr->jumps_max;
}

// 0x8015C4C8
void ftFoxSpecialHiInitStatusVars(FTStruct *fp)
{
    fp->status_vars.fox.specialhi.anim_frames = FTFOX_FIREFOX_TRAVEL_TIME;
    fp->status_vars.fox.specialhi.decelerate_wait = 0;
    fp->status_vars.fox.specialhi.pass_timer = 0;
}

// 0x8015C4DC
void ftFoxSpecialHiDecideSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 unused[2];
    Vec3f angle;

    if
    (
        (ABS(fp->input.pl.stick_range.x) + ABS(fp->input.pl.stick_range.y) >= FTFOX_FIREFOX_ANGLE_STICK_THRESHOLD) &&
        !(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS)
    )
    {
        angle.x = fp->input.pl.stick_range.x;
        angle.y = fp->input.pl.stick_range.y;
        angle.z = 0.0F;

        if (syVectorAngleDiff3D(&fp->coll_data.floor_angle, &angle) < F_CST_DTOR32(90.0F))
        {
            goto setair;
        }
        else
        {
            ftParamSetStickLR(fp);
            ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
            ftFoxSpecialHiInitStatusVars(fp);

            fp->physics.vel_ground.x = 115.0F;
            fp->status_vars.fox.specialhi.angle = syUtilsArcTan2(-fp->coll_data.floor_angle.x * fp->lr, fp->coll_data.floor_angle.y);
            return;
        }
    }
setair:
    mpCommonSetFighterAir(fp);
    ftFoxSpecialAirHiSetStatusFromGround(fighter_gobj);
}

// 0x8015C60C
void ftFoxSpecialAirHiSetStatusFromGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (ABS(fp->input.pl.stick_range.x) + ABS(fp->input.pl.stick_range.y) >= FTFOX_FIREFOX_ANGLE_STICK_THRESHOLD)
    {
        if (ABS(fp->input.pl.stick_range.x) >= FTFOX_FIREFOX_MODEL_STICK_THRESHOLD)
        {
            ftParamSetStickLR(fp);
        }
        fp->status_vars.fox.specialhi.angle = syUtilsArcTan2(fp->input.pl.stick_range.y, fp->input.pl.stick_range.x * fp->lr);
    }
    else fp->status_vars.fox.specialhi.angle = F_CST_DTOR32(90.0F);
    
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
    ftFoxSpecialHiInitStatusVars(fp);

    fp->physics.vel_air.x = (__cosf(fp->status_vars.fox.specialhi.angle) * FTFOX_FIREFOX_VEL * fp->lr);
    fp->physics.vel_air.y = (__sinf(fp->status_vars.fox.specialhi.angle) * FTFOX_FIREFOX_VEL);

    ftFoxSpecialHiUpdateModelPitch(fighter_gobj);

    fp->jumps_used = attr->jumps_max;
}

// 0x8015C750
void ftFoxSpecialAirHiEndProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTFOX_FIREFOX_AIR_DRIFT, FALSE, TRUE, FALSE, FTFOX_FIREFOX_LANDING_LAG, TRUE);
    }
}

// 0x8015C7A4
void ftFoxSpecialHiEndProcPhysics(GObj *fighter_gobj)
{
    ftPhysicsSetGroundVelFriction(ftGetStruct(fighter_gobj), FTFOX_FIREFOX_DECELERATE_END);
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x8015C7D4
void ftFoxSpecialAirHiEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftFoxSpecialAirHiEndSwitchStatusGround);
}

// 0x8015C7F8
void ftFoxSpecialAirHiEndSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHiEnd, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_COLANIM);
}

// 0x8015C838
void ftFoxSpecialHiEndSetStatus(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterGround(fp);
    }
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
}

// 0x8015C88C
void ftFoxSpecialAirHiEndSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
}

// 0x8015C8BC
void ftFoxSpecialAirHiBoundProcUpdate(GObj* fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->motion_vars.flags.flag1 != 0) && (fp->ga == nMPKineticsAir))
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTFOX_FIREFOX_AIR_DRIFT, FALSE, TRUE, FALSE, FTFOX_FIREFOX_LANDING_LAG, TRUE);
    }

    else if (fighter_gobj->anim_frame <= 0.0F)
    {
        if (fp->ga == nMPKineticsAir)
        {
            ftCommonFallSpecialSetStatus(fighter_gobj, FTFOX_FIREFOX_AIR_DRIFT, FALSE, TRUE, FALSE, FTFOX_FIREFOX_LANDING_LAG, TRUE);
        }
        else ftCommonWaitSetStatus(fighter_gobj);
    }
}

// 0x8015C97C
void ftFoxSpecialAirHiBoundProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->ga == nMPKineticsAir)
    {
        ftPhysicsApplyAirVelTransNYZ(fighter_gobj);

        if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
        {
            ftPhysicsApplyAirVelXFriction(fp, attr);
        }
    }
    else ftPhysicsApplyGroundVelFriction(fighter_gobj);
}

// 0x8015C9E8
void ftFoxSpecialAirHiBoundProcMap(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        if (mpCommonCheckFighterCliff(fighter_gobj) != FALSE)
        {
            if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
            {
                ftCommonCliffCatchSetStatus(fighter_gobj);
            }
            else mpCommonSetFighterGround(fp);
        }
    }
    else mpCommonSetFighterFallOnGroundBreak(fighter_gobj);
}

// 0x8015CA64
void ftFoxSpecialAirHiBoundSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHiBound, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);

    fp->motion_vars.flags.flag1 = FALSE;
}

// 0x8015CAA4
void ftFoxSpecialHiStartInitGravity(FTStruct *fp)
{
    fp->status_vars.fox.specialhi.gravity_delay = FTFOX_FIREFOX_GRAVITY_DELAY;
}

// 0x8015CAB0
void ftFoxSpecialHiStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialHiStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftFoxSpecialHiStartInitGravity(fp);

    fp->physics.vel_ground.x /= 2;
}

// 0x8015CB10
void ftFoxSpecialAirHiStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirHiStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftFoxSpecialHiStartInitGravity(fp);

    fp->physics.vel_air.y = 0.0F;
    fp->physics.vel_air.x /= 2;
}
