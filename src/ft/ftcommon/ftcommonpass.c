#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80141D60
void ftCommonPassProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonSpecialAirCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonAttackAirCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonJumpAerialCheckInterruptCommon(fighter_gobj);
    }
}

// 0x80141DA0
void ftCommonPassSetStatusParam(GObj *fighter_gobj, s32 status_id, f32 frame_begin, u32 flags)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, status_id, frame_begin, 1.0F, flags);
    ftPhysicsClampAirVelXMax(fp);

    fp->physics.vel_air.y = 0.0F;

    fp->coll_data.ignore_line_id = fp->coll_data.floor_line_id;

    fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
}

// 0x80141E18
void ftCommonPassSetStatusSquat(GObj *fighter_gobj)
{
    ftCommonSquatSetStatusPass(fighter_gobj);
}

// 0x80141E38
void ftCommonGuardPassSetStatus(GObj *fighter_gobj)
{
    ftCommonPassSetStatusParam(fighter_gobj, nFTCommonStatusGuardPass, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x80141E60
sb32 ftCommonPassCheckInputSuccess(FTStruct *fp)
{
    if ((fp->input.pl.stick_range.y <= FTCOMMON_PASS_STICK_RANGE_MIN) && (fp->tap_stick_y < FTCOMMON_PASS_BUFFER_TICS_MAX) && (fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80141EA4
sb32 ftCommonPassCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
    {
        return ftCommonHammerFallCheckInterruptCommon(fighter_gobj);
    }
    else if (ftCommonPassCheckInputSuccess(fp) != FALSE)
    {
        ftCommonPassSetStatusSquat(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x80141F0C
sb32 ftCommonPassCheckInterruptSquat(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
    {
        return ftCommonHammerFallCheckInterruptCommon(fighter_gobj);
    }
    if (fp->status_vars.common.squat.is_allow_pass == FALSE)
    {
        if (ftCommonPassCheckInputSuccess(fp) != FALSE)
        {
            fp->status_vars.common.squat.is_allow_pass = TRUE;
            fp->status_vars.common.squat.pass_wait = FTCOMMON_SQUAT_PASS_WAIT;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80141F8C
sb32 ftCommonGuardPassCheckInterruptGuard(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((ftCommonPassCheckInputSuccess(fp) != FALSE) && (fp->input.pl.button_hold & fp->input.button_mask_z))
    {
        ftCommonGuardPassSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
