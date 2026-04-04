#include <ft/fighter.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80150760
void ftCommonAttackLw4SetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTCommonStatusAttackLw4, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80150798
sb32 ftCommonAttackLw4CheckInputSuccess(FTStruct *fp)
{
    if ((fp->input.pl.stick_range.y <= FTCOMMON_ATTACKLW4_STICK_RANGE_MIN) && (fp->input.pl.button_tap & fp->input.button_mask_a))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x801507D0
sb32 ftCommonAttackLw4CheckInterruptMain(FTStruct *fp)
{
    FTAttributes *attr = fp->attr;

    if (ftCommonLightThrowCheckItemTypeThrow(fp) != FALSE)
    {
        ftCommonItemThrowSetStatus(fp->fighter_gobj, nFTCommonStatusLightThrowLw4);

        return TRUE;
    }
    if (attr->is_have_attacklw4)
    {
        ftCommonAttackLw4SetStatus(fp->fighter_gobj);

        return TRUE;
    }
    return FALSE;
}

// 0x80150838
sb32 ftCommonAttackLw4CheckInterruptSquat(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((ftCommonAttackLw4CheckInputSuccess(fp) != FALSE) && (fp->status_vars.common.attack4.is_goto_attacklw4 != FALSE))
    {
        return ftCommonAttackLw4CheckInterruptMain(fp);
    }
    else return FALSE;
}

// 0x80150884
sb32 ftCommonAttackLw4CheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((ftCommonAttackLw4CheckInputSuccess(fp) != FALSE) && (fp->tap_stick_y < FTCOMMON_ATTACKLW4_BUFFER_TICS_MAX))
    {
        return ftCommonAttackLw4CheckInterruptMain(fp);
    }
    else return FALSE;
}
