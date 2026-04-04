#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015B6D0
void ftDonkeySpecialHiProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndSetWait(fighter_gobj);
}

// 0x8015B6F0
void ftDonkeySpecialAirHiProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTDONKEY_SPINNINGKONG_FALLSPECIAL_DRIFT, FALSE, TRUE, FALSE, FTDONKEY_SPINNINGKONG_LANDING_LAG, TRUE);
    }
}

// 0x8015B744
void ftDonkeySpecialHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPhysicsApplyClampGroundVelStickRange(fp, 0, FTDONKEY_SPINNINGKONG_GROUND_ACCEL, FTDONKEY_SPINNINGKONG_GROUND_VEL_MAX);
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x8015B780
void ftDonkeySpecialAirHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    FTAttributes *attr = fp->attr;

    f32 gravity_mul = (fp->motion_vars.flags.flag1 != 0) ? FTDONKEY_SPINNINGKONG_END_GRAVITY_MUL : FTDONKEY_SPINNINGKONG_START_GRAVITY_MUL;

    ftPhysicsApplyGravityClampTVel(fp, attr->gravity * gravity_mul, attr->tvel_base);

    ftPhysicsClampAirVelXStickRange(fp, 0, FTDONKEY_SPINNINGKONG_AIR_ACCEL, FTDONKEY_SPINNINGKONG_AIR_VEL_MAX);
}

// 0x8015B800
void ftDonkeySpecialHiProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftDonkeySpecialHiSwitchStatusAir);
}

// 0x8015B824
void ftDonkeySpecialAirHiProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftDonkeySpecialAirHiSwitchStatusGround);
}

// 0x8015B848
void ftDonkeySpecialAirHiSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusSpecialHi, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_RUMBLE);
    ftPhysicsClampGroundVel(fp, FTDONKEY_SPINNINGKONG_GROUND_VEL_MAX);
}

// 0x8015B898
void ftDonkeySpecialHiSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusSpecialAirHi, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_RUMBLE);
    ftPhysicsClampAirVelX(fp, FTDONKEY_SPINNINGKONG_AIR_VEL_MAX);
}

// 0x8015B8E8
void ftDonkeySpecialHiSetStatusFlagGA(GObj *fighter_gobj, sb32 ga)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    ftPhysicsClampAirVelX(fp, FTDONKEY_SPINNINGKONG_AIR_VEL_MAX);

    fp->jumps_used = attr->jumps_max;

    fp->motion_vars.flags.flag1 = 0;

    fp->status_vars.donkey.specialhi.unk_0x0 = 9;

    if (ga == nMPKineticsGround)
    {
        fp->stat_flags.ga = nMPKineticsGround;
    }
}

// 0x8015B974
void ftDonkeySpecialHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);

    fp->physics.vel_air.y = 0.0F;

    ftDonkeySpecialHiSetStatusFlagGA(fighter_gobj, nMPKineticsGround);
}

// 0x8015B9B8
void ftDonkeySpecialAirHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->physics.vel_air.y = FTDONKEY_SPINNINGKONG_AIR_VEL_Y;

    ftDonkeySpecialHiSetStatusFlagGA(fighter_gobj, nMPKineticsAir);
}
