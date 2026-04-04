#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144530
void ftCommonDownStandProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->motion_vars.flags.flag1 != 0) && (ftCommonKneeBendCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonPassCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonDokanStartCheckInterruptCommon(fighter_gobj);
    }
}

// 0x80144580
void ftCommonDownStandSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if ((fp->status_id == nFTCommonStatusDownBounceD) || (fp->status_id == nFTCommonStatusDownWaitD))
    {
        status_id = nFTCommonStatusDownStandD;
    }
    else status_id = nFTCommonStatusDownStandU;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->motion_vars.flags.flag1 = 0;
}

// 0x801445D8
sb32 ftCommonDownStandCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (((fp->input.pl.stick_range.y >= FTCOMMON_DOWNWAIT_STAND_STICK_RANGE_MIN) && (ftParamGetStickAngleRads(fp) >= F_CST_DTOR32(50.0F)) || (fp->input.pl.button_tap & fp->input.button_mask_z)))
    {
        ftCommonDownStandSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
