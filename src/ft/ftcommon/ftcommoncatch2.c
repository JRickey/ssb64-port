#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801886D0
Vec3f dFTCommonCatchPullEffectOffset = { 0.0F, 0.0F, 0.0F };

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80149EC0
void ftCommonCatchPullProcUpdate(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);

    if (ftAnimEndCheckSetStatus(fighter_gobj, ftCommonCatchWaitSetStatus) != FALSE)
    {
        FTStruct *catch_fp = ftGetStruct(this_fp->catch_gobj);

        catch_fp->status_vars.common.capture.is_goto_pulled_wait = TRUE;
    }
}

// 0x80149F04
void ftCommonCatchPullProcCatch(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCatchPull, fp->status_vars.common.catchmain.catch_pull_frame_begin, 1.0F, (FTSTATUS_PRESERVE_SLOPECONTOUR | FTSTATUS_PRESERVE_EFFECT));

    fp->catch_gobj = fp->search_gobj;

    fp->is_catch_or_capture = FALSE;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    if (fp->proc_slope != NULL)
    {
        fp->proc_slope(fighter_gobj);
    }
    pos = dFTCommonCatchPullEffectOffset;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[fp->attr->joint_itemheavy_id], &pos);
    efManagerCatchSwirlMakeEffect(&pos);
    ftParamMakeRumble(fp, 9, 0);
}

// 0x80149FCC
void ftCommonCatchWaitProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.catchwait.throw_wait != 0)
    {
        fp->status_vars.common.catchwait.throw_wait--;
    }
    ftCommonThrowCheckInterruptCatchWait(fighter_gobj);
}

// 0x8014A000
void ftCommonCatchWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCatchWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_SLOPECONTOUR);

    fp->status_vars.common.catchwait.throw_wait = FTCOMMON_CATCH_THROW_WAIT;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    if ((fp->fkind == nFTKindLink) || (fp->fkind == nFTKindNLink))
    {
        ftParamSetModelPartID(fighter_gobj, 21, 0);
        ftParamSetModelPartID(fighter_gobj, 19, -1);
    }
    else if ((fp->fkind == nFTKindYoshi) || (fp->fkind == nFTKindNYoshi))
    {
        ftParamSetModelPartID(fighter_gobj, 7, 1);
    }
}
