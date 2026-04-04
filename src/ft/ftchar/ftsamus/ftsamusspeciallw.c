#include <ft/fighter.h>
#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015DF00
void ftSamusSpecialLwMakeBomb(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    DObj *joint;

    if (fp->motion_vars.flags.flag0 != FALSE)
    {
        fp->motion_vars.flags.flag0 = FALSE;

        pos.x = pos.z = 0.0F;
        pos.y = FTSAMUS_BOMB_OFF_Y;

        joint = fp->joints[nFTPartsJointTopN]; // Not required but stack frame might be suggesting they wrote it like this

        gmCollisionGetFighterPartsWorldPosition(joint, &pos);
        wpSamusBombMakeWeapon(fighter_gobj, &pos);
    }
}

// 0x8015DF64
void ftSamusSpecialLwProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftSamusSpecialLwMakeBomb(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        ftSamusSpecialLwTransferStatusAir(fighter_gobj);
    }
    else ftAnimEndSetWait(fighter_gobj);
}

// 0x8015DFBC
void ftSamusSpecialAirLwProcUpdate(GObj *fighter_gobj)
{
    ftSamusSpecialLwMakeBomb(fighter_gobj);
    ftAnimEndSetFall(fighter_gobj);
}

// 0x8015DFE4
void ftSamusSpecialLwProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->motion_vars.flags.flag3 != FALSE)
    {
        ftPhysicsSetGroundVelStickRange(fp, attr->walk_speed_mul * FTSAMUS_BOMB_DRIFT, attr->traction);
        ftPhysicsSetGroundVelTransferAir(fighter_gobj);
    }
    else ftPhysicsApplyGroundVelFriction(fighter_gobj);
}

// 0x8015E050
void ftSamusSpecialAirLwProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsApplyGravityDefault(fp, attr);

    if (ftPhysicsCheckClampAirVelXDec(fp, attr->air_speed_max_x * FTSAMUS_BOMB_DRIFT) == FALSE)
    {
        ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel * FTSAMUS_BOMB_DRIFT, attr->air_speed_max_x * FTSAMUS_BOMB_DRIFT);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x8015E0E8
void ftSamusSpecialLwProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftSamusSpecialLwSwitchStatusAir);
}

// 0x8015E10C
void ftSamusSpecialAirLwProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftSamusSpecialAirLwSwitchStatusGround);
}

// 0x8015E130
void ftSamusSpecialAirLwSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag3 = FALSE;

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialLw, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8015E170 - Go to aerial Screw Attack from grounded update process
void ftSamusSpecialLwTransferStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialAirLw, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->physics.vel_air.y = FTSAMUS_BOMB_VEL_Y_BASE;
    fp->jumps_used = attr->jumps_max;
}

// 0x8015E1DC
void ftSamusSpecialLwSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialAirLw, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8015E218
void ftSamusSpecialLwInitStatusVars(FTStruct *fp)
{
    fp->motion_vars.flags.flag0 = FALSE;
}

// 0x8015E220
void ftSamusSpecialLwSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag3 = FALSE;

    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialLw, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftSamusSpecialLwInitStatusVars(fp);

    fp->status_vars.samus.speciallw.unused = FALSE;
}

// 0x8015E274
void ftSamusSpecialAirLwSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftMainSetStatus(fighter_gobj, nFTSamusStatusSpecialAirLw, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftSamusSpecialLwInitStatusVars(fp);

    fp->physics.vel_air.y = FTSAMUS_BOMB_VEL_Y_BASE - FTSAMUS_BOMB_VEL_Y_SUB;

    ftPhysicsClampAirVelX(fp, attr->air_speed_max_x * FTSAMUS_BOMB_DRIFT);

    fp->jumps_used = attr->jumps_max; // Why tho

    fp->status_vars.samus.speciallw.unused = TRUE;
}
