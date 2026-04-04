#include <ft/fighter.h>

// 0x8015F7F0
void ftCaptainSpecialNUpdateEffect(GObj *fighter_gobj) // Falcon Punch
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->is_effect_attach))
    {
        if (fp->motion_vars.flags.flag0 == 1)
        {
            if (efManagerCaptainFalconPunchMakeEffect(fighter_gobj) != FALSE)
            {
                fp->is_effect_attach = TRUE;
            }
            fp->motion_vars.flags.flag0 = 0;
        }
    }

    else if (fp->motion_vars.flags.flag0 == 1)
    {
        ftParamProcStopEffect(fighter_gobj);

        fp->motion_vars.flags.flag0 = 2;
    }
}

// 0x8015F874
f32 ftCaptainSpecialNGetAngle(s32 stick_y)
{
    s32 temp_stick_y = ABS(stick_y);

    if (temp_stick_y > 50)
    {
        temp_stick_y = 50;
    }

    temp_stick_y -= 10;

    if (temp_stick_y < 0)
    {
        temp_stick_y = 0;
    }

    if (stick_y < 0)
    {
        temp_stick_y = -temp_stick_y;
    }

    return F_CLC_DTOR32((temp_stick_y * 30) / 40.0F);
}

// 0x8015F8EC
void ftCaptainSpecialNProcPhysics(GObj *fighter_gobj)
{
    ftCaptainSpecialNUpdateEffect(fighter_gobj);
    ftPhysicsApplyGroundVelTransN(fighter_gobj);
}

// 0x8015F914
void ftCaptainSpecialAirNProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 unused;
    f32 boost;

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;
        fp->passive_vars.captain.falcon_punch_unk++;

        boost = ftCaptainSpecialNGetAngle(fp->input.pl.stick_range.y);

        fp->physics.vel_air.y = (__sinf(boost) * FTCAPTAIN_FALCONPUNCH_VEL_BASE);
        fp->physics.vel_air.x = (__cosf(boost) * fp->lr * FTCAPTAIN_FALCONPUNCH_VEL_BASE);
    }

    ftCaptainSpecialNUpdateEffect(fighter_gobj);

    switch (fp->motion_vars.flags.flag2)
    {
    case 0:
        ftPhysicsApplyAirVelFriction(fighter_gobj);
        break;

    case 1:
        fp->physics.vel_air.y *= FTCAPTAIN_FALCONPUNCH_VEL_MUL;
        fp->physics.vel_air.x *= FTCAPTAIN_FALCONPUNCH_VEL_MUL;
        break;

    case 2:
        ftPhysicsApplyAirVelDriftFastFall(fighter_gobj);
        break;
    }
}

// 0x8015FA2C
void ftCaptainSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM));

    fp->proc_lagstart = ftParamProcPauseEffect;
    fp->proc_lagend = ftParamProcResumeEffect;
}

// 0x8015FA8C
void ftCaptainSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialAirN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM));
    ftPhysicsClampAirVelXMax(fp);

    fp->proc_lagstart = ftParamProcPauseEffect;
    fp->proc_lagend = ftParamProcResumeEffect;
}

// 0x8015FAF8
void ftCaptainSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftCaptainSpecialNSwitchStatusAir);
}

// 0x8015FB1C
void ftCaptainSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftCaptainSpecialAirNSwitchStatusGround);
}

// 0x8015FB40
void ftCaptainSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;
    fp->motion_vars.flags.flag0 = 0;
}

// 0x8015FB54
void func_ovl3_8015FB54() // Unused???
{
    return;
}

// 0x8015FB5C
void ftCaptainSpecialNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftCaptainSpecialNInitStatusVars(fighter_gobj);

    fp->proc_lagstart = ftParamProcPauseEffect;
    fp->proc_lagend = ftParamProcResumeEffect;
}

// 0x8015FBC0
void ftCaptainSpecialAirNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCaptainStatusSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftCaptainSpecialNInitStatusVars(fighter_gobj);

    fp->proc_lagstart = ftParamProcPauseEffect;
    fp->proc_lagend = ftParamProcResumeEffect;
}
