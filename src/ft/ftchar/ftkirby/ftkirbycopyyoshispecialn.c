#include <ft/fighter.h>

// 0x8015F180
void ftKirbyCopyYoshiSpecialNSetCatchParams(FTStruct *fp, void (*proc_catch)(GObj*)) // Kirby's Yoshi Copy
{
    ftParamSetCatchParams(fp, FTCATCHKIND_MASK_YOSHISPECIALN, proc_catch, ftCommonCaptureYoshiProcCapture);
}

// 0x8015F1AC
void ftKirbyCopyYoshiSpecialNCatchUpdateProcStatus(GObj *fighter_gobj, void (*proc_status)(GObj*))
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (((fp->motion_vars.flags.flag1 != 0) && (fp->catch_gobj != NULL)) || (fighter_gobj->anim_frame <= 0.0F))
    {
        fp->motion_vars.flags.flag1 = 0;

        proc_status(fighter_gobj);
    }
}

// 0x8015F200
void ftKirbyCopyYoshiSpecialNCatchProcUpdate(GObj *fighter_gobj)
{
    ftKirbyCopyYoshiSpecialNCatchUpdateProcStatus(fighter_gobj, ftKirbyCopyYoshiSpecialNReleaseSetStatus);
}

// 0x8015F224
void ftKirbyCopyYoshiSpecialAirNCatchProcUpdate(GObj *fighter_gobj)
{
    ftKirbyCopyYoshiSpecialNCatchUpdateProcStatus(fighter_gobj, ftKirbyCopyYoshiSpecialAirNReleaseSetStatus);
}

// 0x8015F248
void ftKirbyCopyYoshiSpecialAirNCatchUpdateCaptureVars(FTStruct *fp)
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

// 0x8015F2BC
void ftKirbyCopyYoshiSpecialNReleaseProcUpdate(GObj *fighter_gobj)
{
    ftKirbyCopyYoshiSpecialAirNCatchUpdateCaptureVars(ftGetStruct(fighter_gobj));
    ftAnimEndSetWait(fighter_gobj);
}

// 0x8015F2BC
void ftKirbyCopyYoshiSpecialAirNReleaseProcUpdate(GObj *fighter_gobj)
{
    ftKirbyCopyYoshiSpecialAirNCatchUpdateCaptureVars(ftGetStruct(fighter_gobj));
    ftAnimEndSetFall(fighter_gobj);
}

// 0x8015F314
void ftKirbyCopyYoshiSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftKirbyCopyYoshiSpecialNSwitchStatusAir);
}

// 0x8015F338
void ftKirbyCopyYoshiSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftKirbyCopyYoshiSpecialAirNSwitchStatusGround);
}

// 0x8015F35C
void ftKirbyCopyYoshiSpecialNCatchProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftKirbyCopyYoshiSpecialNCatchSwitchStatusAir);
}

// 0x8015F380
void ftKirbyCopyYoshiSpecialAirNCatchProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftKirbyCopyYoshiSpecialAirNCatchSwitchStatusGround);
}

// 0x8015F3A4
void ftKirbyCopyYoshiSpecialNReleaseProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftKirbyCopyYoshiSpecialNReleaseSwitchStatusAir);
}

// 0x8015F3C8
void ftKirbyCopyYoshiSpecialAirNReleaseProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftKirbyCopyYoshiSpecialAirNReleaseSwitchStatusGround);
}

// 0x8015F3EC
void ftKirbyCopyYoshiSpecialNProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015F3F8
void ftKirbyCopyYoshiSpecialAirNProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015F408
void ftKirbyCopyYoshiSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_HIT));
    ftKirbyCopyYoshiSpecialNSetCatchParams(fp, ftKirbyCopyYoshiSpecialNCatchProcCatch);
}

// 0x8015F45C
void ftKirbyCopyYoshiSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialAirN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_HIT));
    ftKirbyCopyYoshiSpecialNSetCatchParams(fp, ftKirbyCopyYoshiSpecialAirNCatchProcCatch);
}

// 0x8015F4B0
void ftKirbyCopyYoshiSpecialAirNCatchSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015F4F0
void ftKirbyCopyYoshiSpecialNCatchSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialAirNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015F530
void ftKirbyCopyYoshiSpecialAirNReleaseSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialNRelease, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015F570
void ftKirbyCopyYoshiSpecialNReleaseSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialAirNRelease, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
}

// 0x8015F5B0
void ftKirbyCopyYoshiSpecialNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftKirbyCopyYoshiSpecialNProcStatus;

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftKirbyCopyYoshiSpecialNSetCatchParams(fp, ftKirbyCopyYoshiSpecialNCatchProcCatch);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8015F60C
void ftKirbyCopyYoshiSpecialAirNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftKirbyCopyYoshiSpecialAirNProcStatus;

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftKirbyCopyYoshiSpecialNSetCatchParams(fp, ftKirbyCopyYoshiSpecialAirNCatchProcCatch);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8015F668
void ftKirbyCopyYoshiSpecialNCatchInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);
    ftPhysicsStopVelAll(fighter_gobj);

    fp->catch_gobj = fp->search_gobj;
}

// 0x8015F6AC
void ftKirbyCopyYoshiSpecialNCatchProcCatch(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyYoshiSpecialNCatchInitStatusVars(fighter_gobj);
}

// 0x8015F6F0
void ftKirbyCopyYoshiSpecialAirNCatchProcCatch(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialAirNCatch, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyYoshiSpecialNCatchInitStatusVars(fighter_gobj);
}

// 0x8015F734
void ftKirbyCopyYoshiSpecialNReleaseInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015F768
void ftKirbyCopyYoshiSpecialNReleaseSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialNRelease, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyYoshiSpecialNReleaseInitStatusVars(fighter_gobj);
}

// 0x8015F7AC
void ftKirbyCopyYoshiSpecialAirNReleaseSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyYoshiSpecialAirNRelease, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyYoshiSpecialNReleaseInitStatusVars(fighter_gobj);
}
