#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144EE0
void ftCommonCliffQuickProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fighter_gobj->anim_frame <= 0.0F)
    {
        switch (fp->status_vars.common.cliffmotion.status_id)
        {
        case nFTCommonCliffKindClimbQuick:
            ftCommonCliffClimbQuick1SetStatus(fighter_gobj);
            break;

        case nFTCommonCliffKindAttackQuick:
            ftCommonCliffAttackQuick1SetStatus(fighter_gobj);
            break;

        case nFTCommonCliffKindEscapeQuick:
            ftCommonCliffEscapeQuick1SetStatus(fighter_gobj);
            break;
        }
    }
}

// 0x80144F64
void ftCommonCliffSlowProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fighter_gobj->anim_frame <= 0.0F)
    {
        switch (fp->status_vars.common.cliffmotion.status_id)
        {
        case nFTCommonCliffKindClimbSlow:
            ftCommonCliffClimbSlow1SetStatus(fighter_gobj);
            break;

        case nFTCommonCliffKindAttackSlow:
            ftCommonCliffAttackSlow1SetStatus(fighter_gobj);
            break;

        case nFTCommonCliffKindEscapeSlow:
            ftCommonCliffEscapeSlow1SetStatus(fighter_gobj);
            break;
        }
    }
}

// 0x80144FE8
void ftCommonCliffQuickOrSlowSetStatus(GObj *fighter_gobj, s32 status_input)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;
    s32 status_queue;

    if (fp->percent_damage < FTCOMMON_CLIFF_DAMAGE_HIGH)
    {
        status_id = nFTCommonStatusCliffQuick, status_queue = nFTCommonCliffKindClimbQuick;
    }
    else status_id = nFTCommonStatusCliffSlow, status_queue = nFTCommonCliffKindClimbSlow;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->status_vars.common.cliffmotion.status_id = status_input + status_queue;
    fp->status_vars.common.cliffmotion.cliff_id = fp->coll_data.cliff_id;

    fp->is_cliff_hold = TRUE;

    fp->proc_damage = ftCommonCliffCommonProcDamage;
}

// 0x80145084
sb32 ftCommonCliffClimbOrFallCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((ABS(fp->input.pl.stick_range.x) >= FTCOMMON_CLIFF_MOTION_STICK_RANGE_MIN) || (ABS(fp->input.pl.stick_range.y) >= FTCOMMON_CLIFF_MOTION_STICK_RANGE_MIN))
    {
        f32 angle = ftParamGetStickAngleRads(fp);

        if ((angle > F_CST_DTOR32(50.0F)) || ((angle > F_CST_DTOR32(-50.0F)) && ((fp->input.pl.stick_range.x * fp->lr) >= 0)))
        {
            if (fp->status_vars.common.cliffwait.is_allow_interrupt != FALSE)
            {
                ftCommonCliffQuickOrSlowSetStatus(fighter_gobj, 0);

                return TRUE;
            }
            else return FALSE;
        }
        else if (fp->status_vars.common.cliffwait.is_allow_interrupt != FALSE)
        {
            fp->cliffcatch_wait = FTCOMMON_CLIFF_CATCH_WAIT;

            ftCommonCliffCommonProcDamage(fighter_gobj);
            ftCommonFallSetStatus(fighter_gobj);

            return TRUE;
        }
        else return FALSE;
    }
    else fp->status_vars.common.cliffwait.is_allow_interrupt = TRUE;

    return FALSE;
}

// 0x801451A8
void ftCommonCliffClimbQuick1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonCliffClimbQuick2SetStatus);
}

// 0x801451CC
void ftCommonCliffClimbSlow1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonCliffClimbSlow2SetStatus);
}

// 0x801451F0
void ftCommonCliffClimbQuick1SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffClimbQuick1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_cliff_hold = TRUE;

    fp->proc_damage = ftCommonCliffCommonProcDamage;
}

// 0x80145240
void ftCommonCliffClimbSlow1SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffClimbSlow1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_cliff_hold = TRUE;

    fp->proc_damage = ftCommonCliffCommonProcDamage;
}

// 0x80145290
void ftCommonCliffCommon2ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, mpCommonSetFighterWaitOrFall);
}

// 0x801452B4
void ftCommonCliffCommon2ProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    Vec3f vel;
    Vec3f *translate;
    f32 y;

    if (fp->ga == nMPKineticsGround)
    {
        ftPhysicsApplyGroundVelTransN(fighter_gobj);
    }
    else
    {
        translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;

        pos = *translate;

        ftPhysicsGetAirVelTransN(fp, &vel.x, NULL, &vel.z);

        pos.x += vel.x;
        pos.z += vel.z;

        mpCollisionGetSpeedLineID(fp->status_vars.common.cliffmotion.cliff_id, &vel);

        pos.x += vel.x;

        if (mpCollisionGetFCCommonFloor(fp->status_vars.common.cliffmotion.cliff_id, &pos, &y, NULL, NULL) != FALSE)
        {
            pos.y += y;

            pos.y += fp->joints[nFTPartsJointTransN]->translate.vec.f.y;

            fp->physics.vel_air.x = pos.x - translate->x;
            fp->physics.vel_air.y = pos.y - translate->y;
            fp->physics.vel_air.z = pos.z - translate->z;
        }
        else ftPhysicsApplyAirVelTransNAll(fighter_gobj);
    }
}

// 0x801453F0
void ftCommonCliffClimbCommon2ProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterFallOnGroundBreak(fighter_gobj);
    }
    else if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        mpCommonSetFighterGround(fp);
    }
}

// 0x80145440
void ftCommonCliffAttackEscape2ProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterFallOnEdgeBreak(fighter_gobj);
    }
    else if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        mpCommonSetFighterGround(fp);
    }
}

// 0x80145490
void ftCommonCliffCommon2UpdateCollData(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    MPCollData *coll_data = &fp->coll_data;
    Vec3f *translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;

    if (fp->attr->cliff_status_ga[fp->status_vars.common.cliffmotion.status_id] == nMPKineticsGround)
    {
        mpCommonSetFighterGround(fp);
    }
    if (fp->lr == +1)
    {
        mpCollisionGetFloorEdgeL(coll_data->cliff_id, translate);

        translate->x += 5.0F;
    }
    else
    {
        mpCollisionGetFloorEdgeR(coll_data->cliff_id, translate);

        translate->x -= 5.0F;
    }
    coll_data->floor_line_id = coll_data->cliff_id;

    mpCollisionGetFCCommonFloor(coll_data->floor_line_id, translate, &coll_data->floor_dist, &coll_data->floor_flags, &coll_data->floor_angle);

    translate->y += coll_data->floor_dist;

    coll_data->floor_dist = 0.0F;
}

// 0x8014557C
void ftCommonCliffCommon2InitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        fp->is_jostle_ignore = TRUE;
    }
}

// 0x801455A0
void ftCommonCliffClimbQuick2SetStatus(GObj *fighter_gobj)
{
    ftCommonCliffCommon2UpdateCollData(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffClimbQuick2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCommonCliffCommon2InitStatusVars(fighter_gobj);
}

// 0x801455E0
void ftCommonCliffClimbSlow2SetStatus(GObj *fighter_gobj)
{
    ftCommonCliffCommon2UpdateCollData(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffClimbSlow2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCommonCliffCommon2InitStatusVars(fighter_gobj);
}
