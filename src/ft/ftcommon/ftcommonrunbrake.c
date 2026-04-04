#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013EFB0
void ftCommonRunBrakeProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((ftCommonKneeBendCheckInterruptRun(fighter_gobj) == FALSE) && (fp->motion_vars.flags.flag1 != 0) && (fighter_gobj->anim_frame <= 4.0F))
    {
        ftCommonTurnRunCheckInterruptRun(fighter_gobj);
    }
}

// 0x8013F014
void ftCommonRunBrakeProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsSetGroundVelFriction(fp, attr->traction * 1.25F);
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x8013F05C
void ftCommonRunBrakeSetStatus(GObj *fighter_gobj, u32 flag)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusRunBrake, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->motion_vars.flags.flag1 = flag;
}

// 0x8013F0A0
sb32 ftCommonRunBrakeCheckInterruptRun(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->input.pl.stick_range.x * fp->lr) < FTCOMMON_RUN_STICK_RANGE_MIN)
    {
        ftCommonRunBrakeSetStatus(fighter_gobj, 1);

        return TRUE;
    }
    else return FALSE;
}

// 0x8013F0EC
sb32 ftCommonRunBrakeCheckInterruptTurnRun(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ABS(fp->input.pl.stick_range.x) < FTCOMMON_RUN_STICK_RANGE_MIN)
    {
        ftCommonRunBrakeSetStatus(fighter_gobj, 0);

#if defined(REGION_US)
        // Momentum slide fix
        if (fp->physics.vel_ground.x > fp->attr->run_speed)
        {
            fp->physics.vel_ground.x = fp->attr->run_speed;
        }
#endif
        return TRUE;
    }
    else return FALSE;
}
