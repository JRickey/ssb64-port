#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015DC40
void ftSamusSpecialHiProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTSAMUS_SCREWATTACK_FALLSPECIAL_DRIFT, TRUE, TRUE, TRUE, FTSAMUS_SCREWATTACK_LANDING_LAG, FALSE);
    }
}

// 0x8015DC94
void ftSamusSpecialHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != FALSE)
    {
        fp->motion_vars.flags.flag1 = FALSE;
        fp->physics.vel_air.x = fp->lr * FTSAMUS_SCREWATTACK_VEL_X_BASE;
    }
    if (fp->ga == nMPKineticsAir)
    {
        ftPhysicsApplyAirVelTransNYZ(fighter_gobj);
        ftPhysicsClampAirVelXStickRange(fp, 0, FTSAMUS_SCREWATTACK_DRIFT_MUL, FTSAMUS_SCREWATTACK_DRIFT_CLAMP);
    }
    else ftPhysicsApplyGroundVelFriction(fighter_gobj);
}

// 0x8015DD20
sb32 ftSamusSpecialHiProcPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) || (fp->input.pl.stick_range.y >= FTSAMUS_SCREWATTACK_PASS_STICK_RANGE_MIN))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8015DD58
void ftSamusSpecialHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        if (fp->physics.vel_air.y >= 0.0F)
        {
            mpCommonCheckFighterProject(fighter_gobj);
        }
        else if (mpCommonCheckFighterPassCliff(fighter_gobj, ftSamusSpecialHiProcPass) != FALSE)
        {
            if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
            {
                ftCommonCliffCatchSetStatus(fighter_gobj);
            }
            else ftCommonLandingFallSpecialSetStatus(fighter_gobj, FALSE, FTSAMUS_SCREWATTACK_LANDING_LAG);
        }
    }
    else mpCommonSetFighterFallOnEdgeBreak(fighter_gobj);
}

// 0x8015DE0C
void ftSamusSpecialHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015DE54
void ftSamusSpecialAirHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPhysicsApplyGravityDefault(fp, fp->attr);
    ftPhysicsClampAirVelXStickRange(fp, 0, FTSAMUS_SCREWATTACK_DRIFT_MUL, FTSAMUS_SCREWATTACK_DRIFT_CLAMP);
}

// 0x8015DE90
void ftSamusSpecialAirHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->jumps_used = fp->attr->jumps_max;
    fp->physics.vel_air.y = FTSAMUS_SCREWATTACK_VEL_Y_BASE;

    ftPhysicsClampAirVelX(fp, FTSAMUS_SCREWATTACK_DRIFT_CLAMP);
}
