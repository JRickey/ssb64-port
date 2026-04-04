#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80160280
void ftCaptainSpecialHiSetCatchParams(FTStruct *fp)
{
    ftParamSetCatchParams(fp, FTCATCHKIND_MASK_CAPTAINSPECIALHI, ftCaptainSpecialHiProcCatch, ftCommonCaptureCaptainProcCapture);
}

// 0x801602B0
void ftCaptainSpecialHiProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTCAPTAIN_FALCONDIVE_FALLSPECIAL_DRIFT, FALSE, TRUE, TRUE, FTCAPTAIN_FALCONDIVE_LANDING_LAG, FALSE);
    }
}

// 0x80160304
void ftCaptainSpecialHiCatchProcUpdate(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);

    if ((fighter_gobj->anim_frame <= 0.0F) || (this_fp->motion_vars.flags.flag0 != 0))
    {
        FTStruct *catch_fp = ftGetStruct(this_fp->catch_gobj);

        catch_fp->status_vars.common.capturecaptain.capture_flag |= FTCOMMON_CAPTURECAPTAIN_MASK_THROW;

        efManagerQuakeMakeEffect(1); // Apply screen shake/rumble?
        ftCaptainSpecialHiThrowSetStatus(fighter_gobj);
    }
}

// 0x80160370
void ftCaptainSpecialHiProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        if (ABS(fp->input.pl.stick_range.x) > FTCAPTAIN_FALCONDIVE_TURN_STICK_RANGE_MIN)
        {
            ftParamSetStickLR(fp);

            fp->joints[nFTPartsJointTopN]->rotate.vec.f.y = fp->lr * HALF_PI32;
        }
    }
}

// 0x801603F0
void ftCaptainSpecialHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    fp->physics.vel_air.x = fp->status_vars.captain.specialhi.vel.x;
    fp->physics.vel_air.y = fp->status_vars.captain.specialhi.vel.y;
    fp->physics.vel_air.z = 0.0F;

    if (ftPhysicsCheckClampAirVelXDec(fp, attr->air_speed_max_x * FTCAPTAIN_FALCONDIVE_AIR_SPEED_MAX_MUL) == FALSE)
    {
        ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel * FTCAPTAIN_FALCONDIVE_AIR_ACCEL_MUL, attr->air_speed_max_x * FTCAPTAIN_FALCONDIVE_AIR_SPEED_MAX_MUL);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
    fp->status_vars.captain.specialhi.vel.x = fp->physics.vel_air.x;
    fp->status_vars.captain.specialhi.vel.y = fp->physics.vel_air.y;

    ftPhysicsApplyAirVelTransNAll(fighter_gobj);

    fp->physics.vel_air.x += fp->status_vars.captain.specialhi.vel.x;
    fp->physics.vel_air.y += fp->status_vars.captain.specialhi.vel.y;
}

// 0x801604D8
void ftCaptainSpecialHiCatchProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f vec;

    if (!(fp->status_vars.captain.specialhi.flags & 4))
    {
        ftCommonCaptureCaptainUpdatePositions(fighter_gobj, fp->catch_gobj, &vec);

        if (syVectorMag3D(&vec) > 180.0F)
        {
            syVectorNorm3D(&vec);
            syVectorScale3D(&vec, 180.0F);
        }
        syVectorSub3D(&DObjGetStruct(fighter_gobj)->translate.vec.f, &vec);
    }
}

// 0x80160560
void ftCaptainSpecialHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->physics.vel_air.y < 0.0F)
    {
        if (fp->motion_vars.flags.flag2 == 0)
        {
            mpCommonProcFighterCliffWaitOrLanding(fighter_gobj);
        }
        else
        {
            mpCommonProcFighterProject(fighter_gobj);
            fp->motion_vars.flags.flag2--;
        }
    }
    else if ((mpCommonCheckFighterCeilHeavyCliff(fighter_gobj) != FALSE) && (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK))
    {
        ftCommonCliffCatchSetStatus(fighter_gobj);
    }
}

// 0x801605FC
void ftCaptainSpecialHiProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->jumps_used = fp->attr->jumps_max;

    fp->status_vars.captain.specialhi.flags = 0;

    fp->motion_vars.flags.flag0 = 0;
    fp->motion_vars.flags.flag1 = 0;
    fp->motion_vars.flags.flag2 = FTCAPTAIN_FALCONDIVE_UNK_TIMER;

    fp->status_vars.captain.specialhi.vel.x = 0.0F;
    fp->status_vars.captain.specialhi.vel.y = 0.0F;
}

// 0x80160630
void ftCaptainSpecialHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);

    fp->proc_status = ftCaptainSpecialHiProcStatus;

    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCaptainSpecialHiSetCatchParams(fp);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80160690
void ftCaptainSpecialHiProcCatch(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj), *catch_fp;
    GObj *search_gobj;

    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialHiCatch, 0.0F, 1.0F, FTSTATUS_PRESERVE_EFFECT);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);
    ftPhysicsStopVelAll(fighter_gobj);

    search_gobj = fp->search_gobj;
    fp->catch_gobj = search_gobj;

    catch_fp = ftGetStruct(search_gobj); // No NULL check, might be dangerous

    if (catch_fp->ga == nMPKineticsAir)
    {
        fp->status_vars.common.capturecaptain.capture_flag |= FTCOMMON_CAPTURECAPTAIN_MASK_NOUPDATE;
        fp->is_catch_or_capture = FALSE;
    }
    else fp->is_catch_or_capture = TRUE;
}

// 0x80160730
void ftCaptainSpecialHiThrowSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialHiThrow, 0.0F, 1.0F, FTSTATUS_PRESERVE_EFFECT);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_NONE);

    if ((fp->is_catch_or_capture == TRUE) && (fp->catch_gobj != NULL))
    {
        ftCommonThrownReleaseFighterLoseGrip(fighter_gobj);
    }
    fp->catch_gobj = NULL;
}

// 0x801607B4
void ftCaptainSpecialAirHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftCaptainSpecialHiProcStatus;

    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCaptainSpecialHiSetCatchParams(fp);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
