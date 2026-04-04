#include <ft/fighter.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801505F0
void ftCommonAttackHi4SetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTCommonStatusAttackHi4, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80150628
sb32 ftCommonAttackHi4CheckInputSuccess(FTStruct *fp)
{
    if ((fp->input.pl.stick_range.y >= FTCOMMON_ATTACKHI4_STICK_RANGE_MIN) && (fp->input.pl.button_tap & fp->input.button_mask_a))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80150660
sb32 ftCommonAttackHi4CheckInterruptMain(FTStruct *fp)
{
    FTAttributes *attr = fp->attr;

    if (ftCommonLightThrowCheckItemTypeThrow(fp) != FALSE)
    {
        ftCommonItemThrowSetStatus(fp->fighter_gobj, nFTCommonStatusLightThrowHi4);

        return TRUE;
    }
    if (attr->is_have_attackhi4)
    {
        ftCommonAttackHi4SetStatus(fp->fighter_gobj);

        return TRUE;
    }
    return FALSE;
}

// 0x801506CC
sb32 ftCommonAttackHi4CheckInterruptKneeBend(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftCommonAttackHi4CheckInputSuccess(fp) != FALSE)
    {
        return ftCommonAttackHi4CheckInterruptMain(fp);
    }
    else return FALSE;
}

// 0x8015070C
sb32 ftCommonAttackHi4CheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((ftCommonAttackHi4CheckInputSuccess(fp) != FALSE) && (fp->tap_stick_y < FTCOMMON_ATTACKHI4_BUFFER_TICS_MAX))
    {
        return ftCommonAttackHi4CheckInterruptMain(fp);
    }
    else return FALSE;
}
