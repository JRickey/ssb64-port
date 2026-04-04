#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144B30
void ftCommonCliffCatchProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonCliffWaitSetStatus);
}

// 0x80144B54
void ftCommonCliffCommonProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *topn_joint = fp->joints[nFTPartsJointTopN], *transn_joint = fp->joints[nFTPartsJointTransN];
    Vec3f vel;

    if (fp->lr == +1)
    {
        mpCollisionGetFloorEdgeL(fp->coll_data.cliff_id, &vel);
    }
    else mpCollisionGetFloorEdgeR(fp->coll_data.cliff_id, &vel);

    topn_joint->translate.vec.f.x = ((transn_joint->translate.vec.f.z * fp->lr * topn_joint->scale.vec.f.x) + vel.x);
    topn_joint->translate.vec.f.y = ((transn_joint->translate.vec.f.y * topn_joint->scale.vec.f.y) + vel.y);
}

// 0x80144C1C
void ftCommonCliffCommonProcMap(GObj *fighter_gobj)
{
    return;
}

// 0x80144C24
void ftCommonCliffCatchSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffCatch, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    mpCommonSetFighterAir(fp);
    ftPhysicsStopVelAll(fighter_gobj);

    fp->coll_data.floor_line_id = -1;

    ftCommonCliffCommonProcPhysics(fighter_gobj);

    fp->is_cliff_hold = TRUE;

    if (fp->lr == +1)
    {
        mpCollisionGetFloorEdgeL(fp->coll_data.cliff_id, &pos);
    }
    else mpCollisionGetFloorEdgeR(fp->coll_data.cliff_id, &pos);

    efManagerFlashMiddleMakeEffect(&pos);

    fp->proc_damage = ftCommonCliffCommonProcDamage;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_TARUCANN);
}

// 0x80144CF8
void ftCommonCliffCommonProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    MPObjectColl *map_coll = &fp->coll_data.map_coll;
    Vec3f pos;

    if (fp->lr == +1)
    {
        mpCollisionGetFloorEdgeL(fp->coll_data.cliff_id, &pos);
    }
    else mpCollisionGetFloorEdgeR(fp->coll_data.cliff_id, &pos);

    pos.x -= ((map_coll->width + 30.0F) * fp->lr);
    pos.y -= map_coll->center;

    mpCommonRunFighterCollisionDefault(fighter_gobj, &pos, &fp->coll_data);
}

// 0x80144DA4
void ftCommonCliffWaitProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonCliffAttackCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonCliffEscapeCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonCliffClimbOrFallCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonCliffWaitCheckFall(fighter_gobj);
    }
}

// 0x80144DF4
void ftCommonCliffWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffWait, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->status_vars.common.cliffwait.is_allow_interrupt = FALSE;

    if (fp->percent_damage < FTCOMMON_CLIFF_DAMAGE_HIGH)
    {
        fp->status_vars.common.cliffwait.fall_wait = FTCOMMON_CLIFF_FALL_WAIT_DAMAGE_LOW;
    }
    else fp->status_vars.common.cliffwait.fall_wait = FTCOMMON_CLIFF_FALL_WAIT_DAMAGE_HIGH;

    fp->is_cliff_hold = TRUE;

    ftParamSetPlayerTagWait(fighter_gobj, 120);

    fp->proc_damage = ftCommonCliffCommonProcDamage;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_TARUCANN);
}

// 0x80144E84
sb32 ftCommonCliffWaitCheckFall(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.common.cliffwait.fall_wait--;

    if (fp->status_vars.common.cliffwait.fall_wait == 0)
    {
        fp->cliffcatch_wait = FTCOMMON_CLIFF_CATCH_WAIT;

        ftCommonCliffCommonProcDamage(fighter_gobj);
        ftCommonDamageFallSetStatusFromCliffWait(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
