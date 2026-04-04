#include <ft/fighter.h>

extern void ftParamSetCaptureImmuneMask(FTStruct*, u8);

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014A0C0
void ftCommonThrowProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        fp->lr = -fp->lr;

        fp->physics.vel_ground.x = -fp->physics.vel_ground.x;
    }
    if (fp->motion_vars.flags.flag2 != 0)
    {
        ftCommonThrownProcPhysics(fp->catch_gobj);
        ftCommonThrownReleaseThrownUpdateStats(fp->catch_gobj, (fp->motion_vars.flags.flag2 == 1) ? -fp->lr : fp->lr, (fp->status_id == nFTCommonStatusThrowB) ? 1 : 0, TRUE);

        fp->motion_vars.flags.flag2 = 0;

        fp->catch_gobj = NULL;

        ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_NONE);
    }
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        if ((fp->fkind == nFTKindDonkey) || (fp->fkind == nFTKindNDonkey) || (fp->fkind == nFTKindGDonkey))
        {
            if (fp->status_id == nFTCommonStatusThrowF)
            {
                ftCommonCaptureShoulderedSetStatus(fp->catch_gobj);
                ftDonkeyThrowFWaitSetStatus(fighter_gobj);

                return;
            }
        }
        mpCommonSetFighterWaitOrFall(fighter_gobj);
    }
}

// 0x8014A1E8
void ftCommonThrowSetStatus(GObj *fighter_gobj, sb32 is_throwf)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    s32 status_id;
    GObj *catch_gobj;
    FTStruct *catch_fp;
    FTThrownStatus *thrown_status;

    catch_gobj = this_fp->catch_gobj;
    catch_fp = ftGetStruct(catch_gobj);

    if ((is_throwf != FALSE) || ((this_fp->input.pl.stick_range.x * this_fp->lr) >= 0))
    {
        if ((this_fp->fkind == nFTKindKirby) || (this_fp->fkind == nFTKindNKirby))
        {
            status_id = nFTKirbyStatusThrowF;

            mpCommonSetFighterAir(this_fp);
        }
        else status_id = nFTCommonStatusThrowF;
        thrown_status = &this_fp->attr->thrown_status[catch_fp->fkind].ft_thrown[0];
    }
    else
    {
        status_id = nFTCommonStatusThrowB;
        thrown_status = &this_fp->attr->thrown_status[catch_fp->fkind].ft_thrown[1];
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftParamSetCaptureImmuneMask(this_fp, FTCATCHKIND_MASK_ALL);

    this_fp->motion_vars.flags.flag2 = 0;
    this_fp->motion_vars.flags.flag1 = 0;

    if ((this_fp->fkind == nFTKindSamus) || (this_fp->fkind == nFTKindNSamus))
    {
        if (efManagerSamusGrappleBeamGlowMakeEffect(fighter_gobj) != NULL)
        {
            this_fp->is_effect_attach = TRUE;
        }
    }
    if (thrown_status->status1 != -1)
    {
        ftCommonThrownSetStatusQueue(catch_gobj, thrown_status->status1, thrown_status->status2);
    }
    else ftCommonThrownSetStatusImmediate(catch_gobj, thrown_status->status2);

    if ((this_fp->fkind == nFTKindKirby) || (this_fp->fkind == nFTKindNKirby))
    {
        if (status_id == nFTKirbyStatusThrowF)
        {
            this_fp->is_ignore_dead = TRUE;
            catch_fp->is_ignore_dead = TRUE;
        }
    }
}

// 0x8014A394
sb32 ftCommonThrowCheckInterruptCatchWait(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    sb32 is_throwf = FALSE;

    if ((fp->status_vars.common.catchwait.throw_wait == 0) || (fp->input.pl.button_tap & (fp->input.button_mask_a | fp->input.button_mask_b)))
    {
        is_throwf = TRUE;
    }
    else if ((fp->input.pl.stick_prev.x >= FTCOMMON_CATCH_THROW_STICK_RANGE_MIN) || (fp->input.pl.stick_range.x < FTCOMMON_CATCH_THROW_STICK_RANGE_MIN))
    {
        if ((fp->input.pl.stick_prev.x <= -FTCOMMON_CATCH_THROW_STICK_RANGE_MIN) || (fp->input.pl.stick_range.x > -FTCOMMON_CATCH_THROW_STICK_RANGE_MIN))
        {
            return FALSE;
        }
    }
    ftCommonThrowSetStatus(fighter_gobj, is_throwf);

    return TRUE;
}
