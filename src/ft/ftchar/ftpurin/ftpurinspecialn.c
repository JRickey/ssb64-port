#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTPURIN_SPECIALN_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80151260
void ftPurinSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;
    fp->motion_vars.flags.flag0 = 0;
}

// 0x80151274
f32 ftPurinSpecialNGetAngle(s32 stick_y)
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

// 0x801512EC
void ftPurinSpecialAirNProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 unused;
    f32 angle;

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        fp->passive_vars.purin.unk_0x0++;

        angle = ftPurinSpecialNGetAngle(fp->input.pl.stick_range.y);

        fp->physics.vel_air.y = __sinf(angle) * FTPURIN_POUND_VEL_BASE;
        fp->physics.vel_air.x = __cosf(angle) * fp->lr * FTPURIN_POUND_VEL_BASE;
    }

    switch (fp->motion_vars.flags.flag2)
    {
    case 0:
        ftPhysicsApplyAirVelFriction(fighter_gobj);
        break;

    case 1:
        fp->physics.vel_air.y *= FTPURIN_POUND_VEL_MUL;
        fp->physics.vel_air.x *= FTPURIN_POUND_VEL_MUL;
        break;

    case 2:
        ftPhysicsApplyAirVelDriftFastFall(fighter_gobj);
        break;
    }
}

// 0x801513F0
void ftPurinSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftPurinSpecialNSwitchStatusAir);
}

// 0x80151414
void ftPurinSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPurinSpecialAirNSwitchStatusGround);
}

// 0x80151438
void ftPurinSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialN, fighter_gobj->anim_frame, 1.0F, FTPURIN_SPECIALN_STATUS_FLAGS);
}

// 0x80151478
void ftPurinSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialAirN, fighter_gobj->anim_frame, 1.0F, FTPURIN_SPECIALN_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x801514C4
void ftPurinSpecialNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftPurinSpecialNInitStatusVars(fighter_gobj);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80151504
void ftPurinSpecialAirNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftPurinSpecialNInitStatusVars(fighter_gobj);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
