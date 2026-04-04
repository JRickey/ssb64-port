#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013E070
void ftCommonWaitProcInterrupt(GObj *fighter_gobj)
{
    !(ftCommonGroundCheckInterrupt(fighter_gobj));
}

// 0x8013E1C8
void ftCommonWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
    {
        ftHammerSetStatusHammerWait(fighter_gobj);
    }
    else
    {
        if (fp->ga == nMPKineticsAir)
        {
            mpCommonSetFighterGround(fp);
        }
        ftMainSetStatus(fighter_gobj, nFTCommonStatusWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

        fp->is_special_interrupt = TRUE;

        ftParamSetPlayerTagWait(fighter_gobj, 120);
    }
}

// 0x8013E258
sb32 ftCommonWaitCheckInputSuccess(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (((fp->input.pl.stick_range.x * fp->lr) < 0) || (ABS(fp->input.pl.stick_range.x) < 8))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8013E2A0
sb32 ftCommonWaitCheckInterruptCommon(GObj *fighter_gobj)
{
    if (ftCommonWaitCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftCommonWaitSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
