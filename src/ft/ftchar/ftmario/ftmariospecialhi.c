#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801560A0
void ftMarioSpecialHiProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTMARIO_SUPERJUMP_AIR_DRIFT, TRUE, FALSE, TRUE, FTMARIO_SUPERJUMP_LANDING_LAG, FALSE);
    }
}

// 0x801560F4
void ftMarioSpecialHiProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 rot_z;
    f32 stick_rot;
    f32 joint_rot;
    s32 stick_x;

    if (fp->motion_vars.flags.flag1 == 0)
    {
        stick_x = ABS(fp->input.pl.stick_range.x);

        if (stick_x > FTMARIO_SUPERJUMP_TURN_STICK_THRESHOLD)
        {
            stick_x = (fp->input.pl.stick_range.x > 0) ? FTMARIO_SUPERJUMP_TURN_STICK_THRESHOLD : -FTMARIO_SUPERJUMP_TURN_STICK_THRESHOLD;

            rot_z = ((fp->input.pl.stick_range.x - stick_x) * FTMARIO_SUPERJUMP_AIR_DRIFT);

            rot_z = -F_CLC_DTOR32(rot_z);

            stick_rot = ABSF(rot_z);

            joint_rot = ABSF(fp->joints[nFTPartsJointTransN]->rotate.vec.f.z);

            if (joint_rot < stick_rot)
            {
                fp->joints[nFTPartsJointTransN]->rotate.vec.f.z = rot_z;
            }
        }
    }

    if (fp->motion_vars.flags.flag2 != 0)
    {
        fp->motion_vars.flags.flag2 = 0;

        stick_x = ABS(fp->input.pl.stick_range.x);

        if (stick_x > FTMARIO_SUPERJUMP_TURN_MODEL_THRESHOLD)
        {
            ftParamSetStickLR(fp);

            fp->joints[nFTPartsJointTopN]->rotate.vec.f.y = fp->lr * F_CST_DTOR32(90.0F);
        }
    }
}

// 0x80156240
void ftMarioSpecialHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->status_vars.mario.specialhi.is_air_bool == FALSE)
    {
        if (fp->ga == nMPKineticsAir)
        {
            ftPhysicsApplyAirVelTransNAll(fighter_gobj);
        }
        else ftPhysicsApplyGroundVelTransN(fighter_gobj);
    }
    else if (fp->motion_vars.flags.flag1 != 0)
    {
        ftPhysicsApplyAirVelTransNAll(fighter_gobj);

        fp->physics.vel_air.x *= 0.95F;
        fp->physics.vel_air.y *= 0.95F;
        fp->physics.vel_air.z *= 0.95F;
    }
    else
    {
        ftPhysicsApplyGravityClampTVel(fp, 0.5F, attr->tvel_base);

        if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
        {
            ftPhysicsApplyAirVelXFriction(fp, attr);
        }
    }
}

// 0x80156320
sb32 ftMarioSpecialHiProcPass(GObj *fighter_gobj) // TRUE = no platform pass?
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    if (!(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) || (fp->input.pl.stick_range.y >= FTMARIO_SUPERJUMP_STICK_Y_UNK))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80156358
void ftMarioSpecialHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        if ((fp->motion_vars.flags.flag1 == 0) || ((fp->physics.vel_air.y >= 0.0F)))
        {
            mpCommonCheckFighterProject(fighter_gobj);
        }
        else if (mpCommonCheckFighterPassCliff(fighter_gobj, ftMarioSpecialHiProcPass) != FALSE)
        {
            if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
            {
                ftCommonCliffCatchSetStatus(fighter_gobj);
            }
            else ftCommonLandingFallSpecialSetStatus(fighter_gobj, FALSE, FTMARIO_SUPERJUMP_LANDING_LAG);
        }
    }
    else mpCommonSetFighterFallOnEdgeBreak(fighter_gobj);
}

// 0x80156418
void ftMarioSpecialHiInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = fp->motion_vars.flags.flag2 = 0;
}

// 0x80156428
void ftMarioSpecialHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMarioSpecialHiInitStatusVars(fighter_gobj);

    fp->status_vars.mario.specialhi.is_air_bool = FALSE;

    ftMainSetStatus(fighter_gobj, nFTMarioStatusSpecialHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80156478
void ftMarioSpecialAirHiSetStatus(GObj* fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMarioSpecialHiInitStatusVars(fighter_gobj);

    fp->status_vars.mario.specialhi.is_air_bool = TRUE;

    fp->physics.vel_air.y = 0.0F;
    fp->physics.vel_air.x /= 1.5F;

    ftMainSetStatus(fighter_gobj, nFTMarioStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
