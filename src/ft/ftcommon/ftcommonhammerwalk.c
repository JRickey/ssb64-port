#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801479A0
void ftCommonHammerWalkProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonHammerKneeBendCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonHammerFallCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftHammerCheckGotoHammerWait(fighter_gobj);
    }
}

// 0x801479E0
void ftCommonHammerWalkSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusHammerWalk, ftHammerGetAnimFrame(fighter_gobj), 1.0F, ftHammerGetStatUpdateFlags(fighter_gobj));
    ftHammerSetColAnim(fighter_gobj);
}

// 0x80147A2C
sb32 ftCommonHammerWalkCheckInterruptCommon(GObj *fighter_gobj)
{
    if (ftCommonWalkCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftCommonHammerWalkSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
