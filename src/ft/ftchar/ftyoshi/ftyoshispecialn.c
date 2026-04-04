#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015E310
void ftYoshiSpecialNSetCatchParams(FTStruct *fp, void (*proc_catch)(GObj*))
{
    ftParamSetCatchParams(fp, FTCATCHKIND_MASK_YOSHISPECIALN, proc_catch, ftCommonCaptureYoshiProcCapture);
}

// 0x8015E33C
void ftYoshiSpecialNCatchUpdateProcStatus(GObj *fighter_gobj, void (*proc_status)(GObj*))
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (((fp->motion_vars.flags.flag1 != 0) && (fp->catch_gobj != NULL)) || (fighter_gobj->anim_frame <= 0.0F))
    {
        fp->motion_vars.flags.flag1 = 0;

        proc_status(fighter_gobj);
    }
}

// 0x8015E390
void ftYoshiSpecialNCatchProcUpdate(GObj *fighter_gobj)
{
    ftYoshiSpecialNCatchUpdateProcStatus(fighter_gobj, ftYoshiSpecialNReleaseSetStatus);
}

// 0x8015E3B4
void ftYoshiSpecialAirNCatchProcUpdate(GObj *fighter_gobj)
{
    ftYoshiSpecialNCatchUpdateProcStatus(fighter_gobj, ftYoshiSpecialAirNReleaseSetStatus);
}

// 0x8015E3D8
void ftYoshiSpecialNCatchUpdateCaptureVars(FTStruct *fp)
{
    if (fp->motion_vars.flags.flag2 != 0)
    {
        if (fp->catch_gobj != NULL)
        {
            FTStruct *catch_fp = ftGetStruct(fp->catch_gobj);

            catch_fp->status_vars.common.captureyoshi.stage = 1;

            fp->motion_vars.flags.flag2 = 0;
        }
    }
    if (fp->motion_vars.flags.flag1 != 0)
    {
        if (fp->catch_gobj != NULL)
        {
            FTStruct *catch_fp = ftGetStruct(fp->catch_gobj);

            catch_fp->status_vars.common.captureyoshi.stage = 3;

            fp->catch_gobj = NULL;

            fp->motion_vars.flags.flag1 = 0;

            ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_NONE);
        }
    }
}

// 0x8015E44C
void ftYoshiSpecialNReleaseProcUpdate(GObj *fighter_gobj)
{
    ftYoshiSpecialNCatchUpdateCaptureVars(ftGetStruct(fighter_gobj));
    ftAnimEndSetWait(fighter_gobj);
}

// 0x8015E478
void ftYoshiSpecialAirNReleaseProcUpdate(GObj *fighter_gobj)
{
    ftYoshiSpecialNCatchUpdateCaptureVars(ftGetStruct(fighter_gobj));
    ftAnimEndSetFall(fighter_gobj);
}

// 0x8015E4A4
void ftYoshiSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftYoshiSpecialNSwitchStatusAir);
}

// 0x8015E4C8
void ftYoshiSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftYoshiSpecialAirNSwitchStatusGround);
}

// 0x8015E4EC
void ftYoshiSpecialNCatchProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftYoshiSpecialNCatchSwitchStatusAir);
}

// 0x8015E510
void ftYoshiSpecialAirNCatchProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftYoshiSpecialAirNCatchSwitchStatusGround);
}

// 0x8015E534
void ftYoshiSpecialNReleaseProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftYoshiSpecialNReleaseSwitchStatusAir);
}

// 0x8015E558
void ftYoshiSpecialAirNReleaseProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftYoshiSpecialAirNReleaseSwitchStatusGround);
}

// 0x8015E57C
void ftYoshiSpecialNProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015E588
void ftYoshiSpecialAirNProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015E598
void ftYoshiSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_HIT));
    ftYoshiSpecialNSetCatchParams(fp, ftYoshiSpecialNCatchProcCatch);
}

// 0x8015E5EC
void ftYoshiSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialAirN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_HIT));
    ftYoshiSpecialNSetCatchParams(fp, ftYoshiSpecialAirNCatchProcCatch);
}

// 0x8015E640
void ftYoshiSpecialAirNCatchSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015E680
void ftYoshiSpecialNCatchSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialAirNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015E6C0
void ftYoshiSpecialAirNReleaseSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialNRelease, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015E700
void ftYoshiSpecialNReleaseSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialAirNRelease, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015E740
void ftYoshiSpecialNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftYoshiSpecialNProcStatus;

    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftYoshiSpecialNSetCatchParams(fp, ftYoshiSpecialNCatchProcCatch);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8015E
void ftYoshiSpecialAirNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftYoshiSpecialAirNProcStatus;

    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftYoshiSpecialNSetCatchParams(fp, ftYoshiSpecialAirNCatchProcCatch);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8015E7F8
void ftYoshiSpecialNCatchInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL); // Store grab vulnerability flags?
    ftPhysicsStopVelAll(fighter_gobj);

    fp->catch_gobj = fp->search_gobj;
}

// 0x8015E83C
void ftYoshiSpecialNCatchProcCatch(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftYoshiSpecialNCatchInitStatusVars(fighter_gobj);
}

// 0x8015E880
void ftYoshiSpecialAirNCatchProcCatch(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialAirNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftYoshiSpecialNCatchInitStatusVars(fighter_gobj);
}

// 0x8015E8C4
void ftYoshiSpecialNReleaseInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015E8F8
void ftYoshiSpecialNReleaseSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialNRelease, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftYoshiSpecialNReleaseInitStatusVars(fighter_gobj);
}

// 0x8015E93C
void ftYoshiSpecialAirNReleaseSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTYoshiStatusSpecialAirNRelease, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftYoshiSpecialNReleaseInitStatusVars(fighter_gobj);
}
