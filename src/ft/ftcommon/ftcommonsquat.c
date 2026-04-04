#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftCommonSquatCheckInterrupt(fighter_gobj)                       \
(                                                                       \
    (ftCommonSpecialNCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonSpecialHiCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonSpecialLwCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonCatchCheckInterruptCommon(fighter_gobj) != FALSE)      ||  \
    (ftCommonAttackS4CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi4CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw4CheckInterruptSquat(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackS3CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttack1CheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonGuardOnCheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonAppealCheckInterruptCommon(fighter_gobj) != FALSE)     ||  \
    (ftCommonKneeBendCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonPassCheckInterruptSquat(fighter_gobj) != FALSE)        ||  \
    (ftCommonDokanStartCheckInterruptCommon(fighter_gobj) != FALSE)     \
)

#define ftCommonSquatWaitCheckInterrupt(fighter_gobj)                   \
(                                                                       \
    (ftCommonSpecialNCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonSpecialHiCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonSpecialLwCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonCatchCheckInterruptCommon(fighter_gobj) != FALSE)      ||  \
    (ftCommonAttackS4CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi4CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw4CheckInterruptSquat(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackS3CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttack1CheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonGuardOnCheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonAppealCheckInterruptCommon(fighter_gobj) != FALSE)     ||  \
    (ftCommonKneeBendCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonDashCheckInterruptCommon(fighter_gobj) != FALSE)       ||  \
    (ftCommonPassCheckInterruptSquat(fighter_gobj) != FALSE)        ||  \
    (ftCommonDokanStartCheckInterruptCommon(fighter_gobj) != FALSE) ||  \
    (ftCommonSquatRvCheckInterruptSquatWait(fighter_gobj) != FALSE)     \
)

#define ftCommonSquatRvCheckInterrupt(fighter_gobj)                     \
(                                                                       \
    (ftCommonSpecialNCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonSpecialHiCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonSpecialLwCheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonCatchCheckInterruptCommon(fighter_gobj) != FALSE)      ||  \
    (ftCommonAttackS4CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi4CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw4CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackS3CheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonAttackHi3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttackLw3CheckInterruptCommon(fighter_gobj) != FALSE)  ||  \
    (ftCommonAttack1CheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonGuardOnCheckInterruptCommon(fighter_gobj) != FALSE)    ||  \
    (ftCommonAppealCheckInterruptCommon(fighter_gobj) != FALSE)     ||  \
    (ftCommonKneeBendCheckInterruptCommon(fighter_gobj) != FALSE)   ||  \
    (ftCommonDashCheckInterruptCommon(fighter_gobj) != FALSE)       ||  \
    (ftCommonPassCheckInterruptCommon(fighter_gobj) != FALSE)       ||  \
    (ftCommonDokanStartCheckInterruptCommon(fighter_gobj) != FALSE)     \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80142E70
sb32 ftCommonSquatCheckGotoPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.squat.is_allow_pass != FALSE)
    {
        if (fp->status_vars.common.squat.pass_wait != 0)
        {
            fp->status_vars.common.squat.pass_wait--;

            if (fp->status_vars.common.squat.pass_wait == 0)
            {
                ftCommonPassSetStatusParam(fighter_gobj, nFTCommonStatusPass, 1.0F, 0);

                return TRUE;
            }
        }
    }
    if (fp->status_vars.common.squat.unk_0x8 != 0)
    {
        fp->status_vars.common.squat.unk_0x8--;
    }
    return FALSE;
}

// 0x80142ED8
void ftCommonSquatProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonSquatWaitSetStatus);
}

// 0x80142EFC
void ftCommonSquatProcInterrupt(GObj *fighter_gobj)
{
    if (!(ftCommonSquatCheckInterrupt(fighter_gobj)))
    {
        ftCommonSquatCheckGotoPass(fighter_gobj);
    }
}

// 0x80143024
void ftCommonSquatSetStatusNoPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusSquat, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->is_special_interrupt = TRUE;

    fp->status_vars.common.squat.is_allow_pass = FALSE;
    fp->status_vars.common.squat.pass_wait = 0;

    if ((fp->input.pl.stick_range.y <= FTCOMMON_SQUAT_STICK_RANGE_MIN) && (fp->tap_stick_y < FTCOMMON_SQUAT_BUFFER_TICS_MAX))
    {
        fp->status_vars.common.squat.unk_0x8 = 3;
    }
    else fp->status_vars.common.squat.unk_0x8 = 0;
}

// 0x801430A8
void ftCommonSquatSetStatusPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusSquat, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->is_special_interrupt = TRUE;

    fp->status_vars.common.squat.is_allow_pass = TRUE;
    fp->status_vars.common.squat.pass_wait = FTCOMMON_SQUAT_PASS_WAIT;
    fp->status_vars.common.squat.unk_0x8 = 3;
}

// 0x8014310C
sb32 ftCommonSquatCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->input.pl.stick_range.y <= (FTCOMMON_SQUAT_STICK_RANGE_MIN - 2))
    {
        ftCommonSquatSetStatusNoPass(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x8014314C
void ftCommonSquatWaitProcUpdate(GObj *fighter_gobj)
{
    return;
}

// 0x80143154
void ftCommonSquatWaitProcInterrupt(GObj *fighter_gobj)
{
    if (!(ftCommonSquatWaitCheckInterrupt(fighter_gobj)))
    {
        ftCommonSquatCheckGotoPass(fighter_gobj);
    }
}

// 0x8014329C
void ftCommonSquatWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusSquatWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_special_interrupt = TRUE;

    ftParamSetPlayerTagWait(fighter_gobj, 120);
}

// 0x801432F0
void ftCommonSquatWaitSetStatusNoPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusSquatWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_special_interrupt = TRUE;

    ftParamSetPlayerTagWait(fighter_gobj, 120);

    fp->status_vars.common.squat.is_allow_pass = FALSE;
    fp->status_vars.common.squat.unk_0x8 = 0;
    fp->status_vars.common.squat.pass_wait = 0;
}

// 0x80143354
sb32 ftCommonSquatWaitCheckInterruptLanding(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->input.pl.stick_range.y <= (FTCOMMON_SQUAT_STICK_RANGE_MIN - 2))
    {
        ftCommonSquatWaitSetStatusNoPass(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x80143394
void ftCommonSquatRvProcInterrupt(GObj *fighter_gobj)
{
    if (!(ftCommonSquatRvCheckInterrupt(fighter_gobj)))
    {
        ftCommonWalkCheckInterruptCommon(fighter_gobj);
    }
}

// 0x801434CC
void ftCommonSquatRvSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusSquatRv, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->is_special_interrupt = TRUE;
}

// 0x8014351C
sb32 ftCommonSquatRvCheckInterruptSquatWait(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->input.pl.stick_range.y >= (FTCOMMON_SQUAT_STICK_RANGE_MIN + 4))
    {
        ftCommonSquatRvSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
