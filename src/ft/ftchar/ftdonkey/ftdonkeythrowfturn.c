#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftDonkeyThrowFTurnCheckInterrupt(fighter_gobj)                            \
(                                                                           \
    (ftCommonHeavyThrowCheckInterruptCommon(fighter_gobj) != FALSE) ||    \
    (ftDonkeyThrowFFCheckInterruptThrowFCommon(fighter_gobj) != FALSE)    \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014D740
void ftDonkeyThrowFTurnProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        fp->lr = -fp->lr;

        fp->physics.vel_ground.x = -fp->physics.vel_ground.x;
    }
    ftAnimEndCheckSetStatus(fighter_gobj, ftDonkeyThrowFWaitSetStatus);
}

// 0x8014D790
void ftDonkeyThrowFTurnProcInterrupt(GObj *fighter_gobj)
{
    if (!(ftDonkeyThrowFTurnCheckInterrupt(fighter_gobj)))
    {
        ftDonkeyThrowFKneeBendCheckInterruptThrowFCommon(fighter_gobj);
    }
}

// 0x8014D7D0
void ftDonkeyThrowFTurnSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;

    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusThrowFTurn, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8014D810
sb32 ftDonkeyThrowFTurnCheckInterruptThrowFCommon(GObj *fighter_gobj)
{
    if (ftCommonTurnCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftDonkeyThrowFTurnSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
