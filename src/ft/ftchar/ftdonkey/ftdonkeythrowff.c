#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014DD00
void ftDonkeyThrowFFProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.throwff.is_turn != FALSE)
    {
        fp->status_vars.common.throwff.turn_tics--;

        if (fp->lr == +1)
        {
            DObjGetStruct(fighter_gobj)->rotate.vec.f.y = F_CST_DTOR32(90.0F) - (((f32)fp->status_vars.common.throwff.turn_tics / FTCOMMON_THROWFF_TURN_FRAMES) * F_CST_DTOR32(180.0F));
        }
        else DObjGetStruct(fighter_gobj)->rotate.vec.f.y = F_CST_DTOR32(-90.0F) + (((f32)fp->status_vars.common.throwff.turn_tics / FTCOMMON_THROWFF_TURN_FRAMES) * F_CST_DTOR32(180.0F));
        
        if (fp->status_vars.common.throwff.turn_tics == 0)
        {
            fp->status_vars.common.throwff.is_turn = FALSE;
        }
    }
    if (fp->motion_vars.flags.flag2 != 0)
    {
        ftCommonThrownProcPhysics(fp->catch_gobj);
        ftCommonThrownReleaseThrownUpdateStats(fp->catch_gobj, -fp->lr, 0, TRUE);

        fp->motion_vars.flags.flag2 = 0;
        fp->catch_gobj = NULL;

        ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_NONE);
    }
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        mpCommonSetFighterWaitOrFall(fighter_gobj);
    }
}

// 0x8014DE30
void ftDonkeyThrowAirFFSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusThrowFF, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->status_vars.common.throwff.is_turn = FALSE;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);
}

// 0x8014DE80
void ftDonkeyThrowFFSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusThrowAirFF, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);
}

// 0x8014DECC
void ftDonkeyThrowFFProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftDonkeyThrowFFSwitchStatusAir);
}

// 0x8014DEF0
void ftDonkeyThrowAirFFProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftDonkeyThrowAirFFSwitchStatusGround);
}

// 0x8014DF14
void ftDonkeyThrowFFSetStatus(GObj *fighter_gobj, sb32 is_turn)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;
    u32 flags;

    if (fp->ga == nMPKineticsGround)
    {
        status_id = nFTDonkeyStatusThrowFF;
        flags = FTSTATUS_PRESERVE_NONE;
    }
    else
    {
        status_id = nFTDonkeyStatusThrowAirFF;
        flags = FTSTATUS_PRESERVE_FASTFALL;
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, flags);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);

    fp->motion_vars.flags.flag2 = 0;

    fp->status_vars.common.throwff.is_turn = is_turn;

    if (is_turn != FALSE)
    {
        fp->lr = -fp->lr;

        fp->status_vars.common.throwff.turn_tics = FTCOMMON_THROWFF_TURN_FRAMES;
    }
}

// 0x8014DFA8
sb32 ftDonkeyThrowFFCheckInterruptThrowFCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    sb32 is_turn = FALSE;

    if (fp->input.pl.button_tap & (fp->input.button_mask_a | fp->input.button_mask_b))
    {
        if ((ABS(fp->input.pl.stick_range.x) >= FTCOMMON_THROWFF_TURN_STICK_RANGE_MIN) && ((fp->input.pl.stick_range.x * fp->lr) < 0) && (fp->ga == nMPKineticsAir))
        {
            is_turn = TRUE;
        }
        ftDonkeyThrowFFSetStatus(fighter_gobj, is_turn);

        return TRUE;
    }
    else return FALSE;
}
