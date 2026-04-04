#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

// Donkey Kong's Cargo Wait
#define ftDonkeyThrowFWaitCheckInterupt(fighter_gobj)                             \
(                                                                                  \
    (ftCommonHeavyThrowCheckInterruptCommon(fighter_gobj) != FALSE) ||           \
    (ftDonkeyThrowFFCheckInterruptThrowFCommon(fighter_gobj) != FALSE) ||        \
    (ftDonkeyThrowFKneeBendCheckInterruptThrowFCommon(fighter_gobj) != FALSE) || \
    (ftDonkeyThrowFFallCheckInterruptPass(fighter_gobj) != FALSE) ||             \
    (ftDonkeyThrowFTurnCheckInterruptThrowFCommon(fighter_gobj) != FALSE)        \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014D400
void ftDonkeyThrowFWaitProcInterrupt(GObj *fighter_gobj)
{
    if (!(ftDonkeyThrowFWaitCheckInterupt(fighter_gobj)))
    {
        ftDonkeyThrowFWalkCheckInterruptThrowFWait(fighter_gobj);
    }
}

// 0x8014D478
void ftDonkeyThrowFCommonProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftDonkeyThrowFFallSetStatus);
}

// 0x8014D49C
void ftDonkeyThrowFWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterGround(fp);
    }
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusThrowFWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8014D4EC
sb32 ftDonkeyThrowFWaitCheckInterruptThrowFWalk(GObj *fighter_gobj)
{
    if (ftCommonWaitCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftDonkeyThrowFWaitSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
