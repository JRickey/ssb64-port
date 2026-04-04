#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80143730
void ftCommonFallSpecialProcInterrupt(GObj *fighter_gobj)
{
    ftCommonJumpAerialCheckInterruptCommon(fighter_gobj);
}

// 0x80143750
void ftCommonFallSpecialProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsCheckSetFastFall(fp);

    if (fp->is_fastfall)
    {
        ftPhysicsApplyFastFall(fp, attr);
    }
    else if (fp->status_vars.common.fallspecial.is_fall_accelerate != FALSE) // Accelerate until fighter reaches terminal velocity?
    {
        ftPhysicsApplyGravityDefault(fp, attr);
    }
    else ftPhysicsApplyGravityClampTVel(fp, attr->gravity, attr->tvel_fast);

    if (ftPhysicsCheckClampAirVelXDec(fp, fp->status_vars.common.fallspecial.drift) == FALSE)
    {
        ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel, fp->status_vars.common.fallspecial.drift);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x80143808
sb32 ftCommonFallSpecialProcPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->status_vars.common.fallspecial.is_allow_pass == FALSE) || !(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) || (fp->input.pl.stick_range.y >= FTCOMMON_FALLSPECIAL_PASS_STICK_RANGE_MIN))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8014384C
void ftCommonFallSpecialProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterPassCliff(fighter_gobj, ftCommonFallSpecialProcPass) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else if ((fp->status_vars.common.fallspecial.is_goto_landing != FALSE) || (fp->physics.vel_air.y < FTCOMMON_FALLSPECIAL_SKIPLANDING_VEL_Y_MAX))
        {
            ftCommonLandingFallSpecialSetStatus(fighter_gobj, fp->status_vars.common.fallspecial.is_allow_interrupt, fp->status_vars.common.fallspecial.landing_lag);
        }
        else ftCommonWaitSetStatus(fighter_gobj);
    }
}

// 0x801438F0
void ftCommonFallSpecialSetStatus(GObj *fighter_gobj, f32 drift, sb32 unknown, sb32 is_fall_accelerate, sb32 is_goto_landing, f32 landing_lag, sb32 is_allow_interrupt)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusFallSpecial, 0.0F, 1.0F, FTSTATUS_PRESERVE_FASTFALL);

    fp->status_vars.common.fallspecial.drift = (attr->air_speed_max_x * drift);

    ftPhysicsClampAirVelX(fp, fp->status_vars.common.fallspecial.drift);

    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterAir(fp);
    }
    fp->jumps_used = attr->jumps_max;

    fp->status_vars.common.fallspecial.is_allow_pass = TRUE;
    fp->status_vars.common.fallspecial.is_goto_landing = is_goto_landing;
    fp->status_vars.common.fallspecial.landing_lag = landing_lag;
    fp->status_vars.common.fallspecial.is_allow_interrupt = is_allow_interrupt;
    fp->status_vars.common.fallspecial.is_fall_accelerate = is_fall_accelerate;

    ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterFallSpecial, 0);
    ftPublicTryPlayFallSpecialReact(fighter_gobj);

    fp->is_special_interrupt = TRUE;
}
