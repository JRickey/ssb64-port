#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTKIRBY_COPYPURIN_SPECIALN_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80151860
void ftKirbyCopyPurinSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    
    fp->motion_vars.flags.flag0 = fp->motion_vars.flags.flag1 = fp->motion_vars.flags.flag2 = 0;
}

// 0x80151874
f32 ftKirbyCopyPurinSpecialNGetAngle(s32 stick_y)
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

    return F_CLC_DTOR32((temp_stick_y * 20) / 40.0F);
}

// 0x801518EC
void ftKirbyCopyPurinSpecialAirNProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 unused;
    f32 boost;

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        fp->passive_vars.kirby.copypurin_unk++;

        boost = ftKirbyCopyPurinSpecialNGetAngle(fp->input.pl.stick_range.y);

        fp->physics.vel_air.y = __sinf(boost) * FTKIRBY_COPYPURIN_POUND_VEL_BASE;
        fp->physics.vel_air.x = __cosf(boost) * fp->lr * FTKIRBY_COPYPURIN_POUND_VEL_BASE;
    }

    switch (fp->motion_vars.flags.flag2)
    {
    case 0:
        ftPhysicsApplyAirVelFriction(fighter_gobj);
        break;

    case 1:
        fp->physics.vel_air.y *= FTKIRBY_COPYPURIN_POUND_VEL_MUL;
        fp->physics.vel_air.x *= FTKIRBY_COPYPURIN_POUND_VEL_MUL;
        break;

    case 2:
        ftPhysicsApplyAirVelDriftFastFall(fighter_gobj);
        break;
    }
}

// 0x801519F0
void ftKirbyCopyPurinSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftKirbyCopyPurinSpecialNSwitchStatusAir);
}

// 0x80151A14
void ftKirbyCopyPurinSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftKirbyCopyPurinSpecialAirNSwitchStatusGround);
}

// 0x80151A38
void ftKirbyCopyPurinSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyPurinSpecialN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
}

// 0x80151A78
void ftKirbyCopyPurinSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyPurinSpecialAirN, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80151AC4
void ftKirbyCopyPurinSpecialNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyPurinSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftKirbyCopyPurinSpecialNInitStatusVars(fighter_gobj);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80151B04
void ftKirbyCopyPurinSpecialAirNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyPurinSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftKirbyCopyPurinSpecialNInitStatusVars(fighter_gobj);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
