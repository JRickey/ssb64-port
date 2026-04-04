#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80160810
void ftKirbyCopyCaptainSpecialNUpdateEffect(GObj *fighter_gobj)
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

// 0x80160894
f32 ftKirbyCopyCaptainSpecialNGetAngle(s32 stick_y)
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

// 0x8016090C
void ftKirbyCopyCaptainSpecialNProcPhysics(GObj *fighter_gobj)
{
    ftKirbyCopyCaptainSpecialNUpdateEffect(fighter_gobj);
    ftPhysicsApplyGroundVelTransN(fighter_gobj);
}

// 0x80160934
void ftKirbyCopyCaptainSpecialAirNProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 unused;
    f32 boost;

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;
        fp->passive_vars.kirby.copycaptain_falcon_punch_unk++;

        boost = ftKirbyCopyCaptainSpecialNGetAngle(fp->input.pl.stick_range.y);

        fp->physics.vel_air.y = __sinf(boost) * FTKIRBY_COPYCAPTAIN_FALCONPUNCH_VEL_BASE;
        fp->physics.vel_air.x = __cosf(boost) * fp->lr * FTKIRBY_COPYCAPTAIN_FALCONPUNCH_VEL_BASE;
    }
    ftKirbyCopyCaptainSpecialNUpdateEffect(fighter_gobj);

    switch (fp->motion_vars.flags.flag2)
    {
    case 0:
        ftPhysicsApplyAirVelFriction(fighter_gobj);
        break;

    case 1:
        fp->physics.vel_air.y *= FTKIRBY_COPYCAPTAIN_FALCONPUNCH_VEL_MUL; // Interestingly, Kirby's version multiplies by singles, 
        fp->physics.vel_air.x *= FTKIRBY_COPYCAPTAIN_FALCONPUNCH_VEL_MUL; // as opposed to Falcon's multiplying by doubles
        break;

    case 2:
        ftPhysicsApplyAirVelDriftFastFall(fighter_gobj);
        break;
    }
}

// 0x80160A40
void ftKirbyCopyCaptainSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyCaptainSpecialN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM));
}

// 0x80160A80
void ftKirbyCopyCaptainSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyCaptainSpecialAirN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM));
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80160ACC
void ftKirbyCopyCaptainSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftKirbyCopyCaptainSpecialNSwitchStatusAir);
}

// 0x80160AF0
void ftKirbyCopyCaptainSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftKirbyCopyCaptainSpecialAirNSwitchStatusGround);
}

// 0x80160B14
void ftKirbyCopyCaptainSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    
    fp->motion_vars.flags.flag0 = fp->motion_vars.flags.flag1 = fp->motion_vars.flags.flag2 = 0;
}

// 0x80160B28
void func_ovl3_80160B28() // Unused
{
    return;
}

// 0x80160B30
void ftKirbyCopyCaptainSpecialNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyCaptainSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyCaptainSpecialNInitStatusVars(fighter_gobj);
}

// 0x80160B70
void ftKirbyCopyCaptainSpecialAirNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyCaptainSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyCaptainSpecialNInitStatusVars(fighter_gobj);
}
