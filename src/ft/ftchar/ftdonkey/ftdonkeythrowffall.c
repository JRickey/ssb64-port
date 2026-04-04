#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014DA00
void ftDonkeyThrowFFallProcInterrupt(GObj *fighter_gobj)
{
    if (ftCommonHeavyThrowCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        ftDonkeyThrowFFCheckInterruptThrowFCommon(fighter_gobj);
    }
}

// 0x8014DA30
void ftDonkeyThrowFFallProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        if (fp->physics.vel_air.y > FTCOMMON_THROWFFALL_SKIPLANDING_VEL_Y_MAX)
        {
            ftDonkeyThrowFWaitSetStatus(fighter_gobj);
        }
        else ftDonkeyThrowFLandingSetStatus(fighter_gobj);
    }
}

// 0x8014DA98
void ftDonkeyThrowFFallSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterAir(fp);
    }
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusThrowFFall, 0.0F, 0.0F, FTSTATUS_PRESERVE_FASTFALL);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x8014DAF8
void ftDonkeyThrowFJumpSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    s32 vel_x;
    s32 vel_y;

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusThrowFFall, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);

    switch (fp->status_vars.common.throwf.input_source)
    {
    case FTCOMMON_KNEEBEND_INPUT_TYPE_BUTTON:
        ftCommonJumpGetJumpForceButton(fp->input.pl.stick_range.x, &vel_x, &vel_y, fp->status_vars.common.throwf.is_shorthop);
        break;

    case FTCOMMON_KNEEBEND_INPUT_TYPE_STICK:
    default:
        vel_x = fp->input.pl.stick_range.x;
        vel_y = fp->status_vars.common.throwf.jump_force;
    }
    fp->physics.vel_air.y = (vel_y * attr->jump_height_mul) + attr->jump_height_base;
    fp->physics.vel_air.x = vel_x * attr->jump_vel_x;

    fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
}

// 0x8014DBE0
void ftDonkeyThrowFFallSetStatusPass(GObj *fighter_gobj)
{
    ftCommonPassSetStatusParam(fighter_gobj, nFTDonkeyStatusThrowFFall, 1.0F, 0);
}

// 0x8014DC08
sb32 ftDonkeyThrowFFallCheckInterruptPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftCommonPassCheckInputSuccess(fp) != FALSE)
    {
        ftDonkeyThrowFFallSetStatusPass(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
