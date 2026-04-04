#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80147A70
void ftCommonHammerTurnUpdateModelYaw(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1--;

        if (fp->motion_vars.flags.flag1 == (FTCOMMON_HAMMER_TURN_FRAMES / 2))
        {
            fp->lr = -fp->lr;
        }
        fp->joints[nFTPartsJointTopN]->rotate.vec.f.y += FTCOMMON_HAMMER_TURN_ROTATE_STEP;

        ftParamsUpdateFighterPartsTransformAll(fp->joints[nFTPartsJointTopN]);
    }
}

// 0x80147AD0
void ftCommonHammerTurnProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonHammerTurnUpdateModelYaw(fighter_gobj);

    if (fp->motion_vars.flags.flag1 == 0)
    {
        ftHammerSetStatusHammerWait(fighter_gobj);
    }
}

// 0x80147B14
void ftCommonHammerTurnProcInterrupt(GObj *fighter_gobj)
{
    ftCommonHammerKneeBendCheckInterruptCommon(fighter_gobj);
}

// 0x80147B34
void ftCommonHammerTurnSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = FTCOMMON_HAMMER_TURN_FRAMES;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusHammerTurn, ftHammerGetAnimFrame(fighter_gobj), 1.0F, ftHammerGetStatUpdateFlags(fighter_gobj));
    ftHammerSetColAnim(fighter_gobj);
    ftCommonHammerTurnUpdateModelYaw(fighter_gobj);
}

// 0x80147B9C
sb32 ftCommonHammerTurnCheckInterruptCommon(GObj *fighter_gobj)
{
    if (ftCommonTurnCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftCommonHammerTurnSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
