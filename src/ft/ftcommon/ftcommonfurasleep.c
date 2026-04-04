#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80149940
void ftCommonFuraSleepProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 breakout_wait;

    fp->breakout_wait--;

    breakout_wait = fp->breakout_wait;

    ftCommonCaptureTrappedUpdateBreakoutVars(fp);

    fp->breakout_wait += (fp->breakout_wait - breakout_wait) * 3;

    if (fp->breakout_wait <= 0)
    {
        ftCommonWaitSetStatus(fighter_gobj);
    }
}

// 0x801499A4
void ftCommonFuraSleepSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 breakout_wait;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusFuraSleep, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    breakout_wait = FTCOMMON_FURASLEEP_BREAKOUT_WAIT_DEFAULT - fp->percent_damage;

    if (breakout_wait <= 0)
    {
        breakout_wait = 0;
    }
    breakout_wait += FTCOMMON_FURASLEEP_BREAKOUT_WAIT_MIN;

    ftCommonCaptureTrappedInitBreakoutVars(fp, breakout_wait);
    ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterFuraSleep, 0);
}
