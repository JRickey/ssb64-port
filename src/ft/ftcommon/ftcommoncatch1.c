#include <ft/fighter.h>
#ifdef PORT
extern void port_log(const char *fmt, ...);
#endif

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80149A10
void ftCommonCatchProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
#ifdef PORT
    DObj *_d = DObjGetStruct(fighter_gobj);
    port_log("SSB64: ftCommonCatchProcUpdate fkind=%d anim_frame=%f dobj_speed=%f dobj_wait=%f dobj_anim_frame=%f pull_begin=%f pull_frames=%f flag1=%u flag2=%u\n",
        (int)fp->fkind,
        fighter_gobj->anim_frame,
        _d ? _d->anim_speed : -999.0F,
        _d ? _d->anim_wait : -999.0F,
        _d ? _d->anim_frame : -999.0F,
        fp->status_vars.common.catchmain.catch_pull_frame_begin,
        fp->status_vars.common.catchmain.catch_pull_anim_frames,
        (unsigned)fp->motion_vars.flags.flag1,
        (unsigned)fp->motion_vars.flags.flag2);
#endif

    if (fp->status_vars.common.catchmain.catch_pull_frame_begin > 0.0F)
    {
        fp->status_vars.common.catchmain.catch_pull_frame_begin -= fp->status_vars.common.catchmain.catch_pull_anim_frames;

        if (fp->status_vars.common.catchmain.catch_pull_frame_begin <= 0.0F)
        {
            fp->status_vars.common.catchmain.catch_pull_frame_begin = 0.0F;
        }
    }
    if (fp->motion_vars.flags.flag2 != 0)
    {
        fp->status_vars.common.catchmain.catch_pull_frame_begin = fp->motion_vars.flags.flag2;

        fp->status_vars.common.catchmain.catch_pull_anim_frames = fp->status_vars.common.catchmain.catch_pull_frame_begin / fp->motion_vars.flags.flag1;

        fp->motion_vars.flags.flag2 = 0;
    }
    ftAnimEndSetWait(fighter_gobj);
}

// 0x80149AC8
void ftCommonCatchCaptureSetStatusRelease(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *catch_fp;
    GObj *catch_gobj;

    ftCommonFallSetStatus(fighter_gobj);

    catch_gobj = this_fp->catch_gobj;

    if (catch_gobj != NULL)
    {
        catch_fp = ftGetStruct(catch_gobj);

        ftCommonThrownReleaseFighterLoseGrip(catch_gobj);

        if (catch_fp->ga == nMPKineticsGround)
        {
            ftCommonWaitSetStatus(catch_gobj);
        }
        else ftCommonFallSetStatus(catch_gobj);

        catch_fp->capture_gobj = NULL;
        this_fp->catch_gobj = NULL;
    }
}

// 0x80149B48
void func_ovl3_80149B48(GObj *fighter_gobj) // Unused
{
    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        ftCommonCatchCaptureSetStatusRelease(fighter_gobj);
    }
}

// 0x80149B78
void ftCommonCatchProcMap(GObj *fighter_gobj)
{
    if (mpCommonCheckFighterOnEdge(fighter_gobj) == FALSE)
    {
        ftCommonCatchCaptureSetStatusRelease(fighter_gobj);
    }
}

// 0x80149BA8
void ftCommonCatchSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCatch, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->motion_vars.flags.flag1 = 1;
    fp->motion_vars.flags.flag2 = 0;

    fp->status_vars.common.catchmain.catch_pull_anim_frames = 0.0F;
    fp->status_vars.common.catchmain.catch_pull_frame_begin = 0.0F;

    ftParamSetCatchParams(fp, FTCATCHKIND_MASK_COMMON, ftCommonCatchPullProcCatch, ftCommonCapturePulledProcCapture);

    fp->is_shield_catch = FALSE;

    if (((fp->fkind == nFTKindSamus) || (fp->fkind == nFTKindNSamus)) && (efManagerSamusGrappleBeamGlowMakeEffect(fighter_gobj) != NULL))
    {
        fp->is_effect_attach = TRUE;
    }
}

// 0x80149C60
sb32 ftCommonCatchCheckInterruptGuard(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    sb32 is_shield_catch = fp->status_vars.common.guard.is_setoff;

    if ((fp->input.pl.button_tap & fp->input.button_mask_a) && (attr->is_have_catch))
    {
        ftCommonCatchSetStatus(fighter_gobj);

        fp->is_shield_catch = is_shield_catch;

        return TRUE;
    }
    else return FALSE;
}

// 0x80149CE0
sb32 ftCommonCatchCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (ftCommonLightThrowCheckItemTypeThrow(fp) != FALSE)
    {
        ftCommonLightThrowDecideSetStatus(fighter_gobj);

        return TRUE;
    }
    else if ((fp->input.pl.button_hold & fp->input.button_mask_z) && (fp->input.pl.button_tap & fp->input.button_mask_a) && (attr->is_have_catch))
    {
        ftCommonCatchSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x80149D80
sb32 ftCommonCatchCheckInterruptDashRun(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (ftCommonLightThrowCheckItemTypeThrow(fp) != FALSE)
    {
        ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowDash);

        return TRUE;
    }
    else if ((fp->input.pl.button_hold & fp->input.button_mask_z) && (fp->input.pl.button_tap & fp->input.button_mask_a) && (attr->is_have_catch))
    {
        ftCommonCatchSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}

// 0x80149E24
sb32 ftCommonCatchCheckInterruptAttack11(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (ftCommonLightThrowCheckItemTypeThrow(fp) != FALSE)
    {
        ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowDash);

        return TRUE;
    }
    else if ((fp->input.pl.button_tap & fp->input.button_mask_z) && (attr->is_have_catch))
    {
        ftCommonCatchSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
