#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013EE50
void ftCommonRunProcInterrupt(GObj *fighter_gobj)
{
    if 
    (
        (ftCommonSpecialNCheckInterruptCommon(fighter_gobj) == FALSE)   &&
        (ftCommonCatchCheckInterruptDashRun(fighter_gobj) == FALSE)     &&
        (ftCommonAttackDashCheckInterruptCommon(fighter_gobj) == FALSE) &&
        (ftCommonGuardOnCheckInterruptDashRun(fighter_gobj, 4) == FALSE)&&
        (ftCommonAppealCheckInterruptCommon(fighter_gobj) == FALSE)     &&
        (ftCommonKneeBendCheckInterruptRun(fighter_gobj) == FALSE)      &&
        (ftCommonTurnRunCheckInterruptRun(fighter_gobj) == FALSE)
    )
    {
        ftCommonRunBrakeCheckInterruptRun(fighter_gobj);
    }
}

// 0x8013EEE8
void ftCommonRunSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusRun, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_ground.x = fp->attr->run_speed;
}

// 0x8013EF2C
sb32 ftCommonRunCheckInterruptDash(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (attr->dash_to_run <= fighter_gobj->anim_frame)
    {
        if (fighter_gobj->anim_frame < (attr->dash_to_run + DObjGetStruct(fighter_gobj)->anim_speed))
        {
            if ((fp->input.pl.stick_range.x * fp->lr) >= FTCOMMON_RUN_STICK_RANGE_MIN)
            {
                ftCommonRunSetStatus(fighter_gobj);

                return TRUE;
            }
        }
    }
    return FALSE;
}
