#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTPIKACHU_SPECIALHI_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801527B0
void ftPikachuSpecialHiStartProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.anim_frames--;

    if (fp->status_vars.pikachu.specialhi.anim_frames == 0)
    {
        ftPikachuSpecialHiSetStatus(fighter_gobj);
    }
}

// 0x801527E4
void ftPikachuSpecialAirHiStartProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.anim_frames--;

    if (fp->status_vars.pikachu.specialhi.anim_frames == 0)
    {
        ftPikachuSpecialAirHiSetStatus(fighter_gobj);
    }
}

// 0x80152818
void ftPikachuSpecialAirHiStartProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsApplyGravityClampTVel(fp, 0.8F, attr->tvel_base);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x8015286C
void ftPikachuSpecialHiStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftPikachuSpecialHiStartSwitchStatusAir);
}

// 0x80152890
void ftPikachuSpecialAirHiStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftPikachuSpecialAirHiStartSwitchStatusGround);
}

// 0x801528B4
void ftPikachuSpecialAirHiStartSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialHiStart, fighter_gobj->anim_frame, 0.0F, FTPIKACHU_SPECIALHI_STATUS_FLAGS);
}

// 0x801528F4
void ftPikachuSpecialHiStartSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirHiStart, fighter_gobj->anim_frame, 0.0F, FTPIKACHU_SPECIALHI_STATUS_FLAGS);
}

// 0x80152934
void ftPikachuSpecialHiStartInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;

    fp->status_vars.pikachu.specialhi.anim_frames = FTPIKACHU_QUICKATTACK_START_TIME;
    fp->status_vars.pikachu.specialhi.is_subsequent_zip = FALSE;
    fp->status_vars.pikachu.specialhi.pass_timer = 0;
    
    fp->physics.vel_air.x = fp->physics.vel_air.y = fp->physics.vel_ground.x = 0.0F;
}

// 0x80152960 - Set hit collision, color animation and anim rate
void ftPikachuSpecialHiInitMiscVars(GObj *fighter_gobj)
{
    ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusIntangible); // Set hit status
    ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterPikachuSpecialHiStart, 0); // Apply color animation
    gcSetAnimSpeed(fighter_gobj, 0.0F); // Set animation speed (0.0F = freeze)
}

// 0x801529A0
void ftPikachuSpecialHiStartSetStatus(GObj *fighter_gobj)
{
    ftPikachuSpecialHiStartInitStatusVars(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialHiStart, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPikachuSpecialHiInitMiscVars(fighter_gobj);
}

// 0x801529EC
void ftPikachuSpecialAirHiStartSetStatus(GObj *fighter_gobj)
{
    ftPikachuSpecialHiStartInitStatusVars(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirHiStart, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPikachuSpecialHiInitMiscVars(fighter_gobj);
}

// 0x80152A38
void ftPikachuSpecialHiProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.anim_frames--;

    if (fp->status_vars.pikachu.specialhi.anim_frames <= 0) // NOW you're checking if less than too?
    {
        ftPikachuSpecialHiEndSetStatus(fighter_gobj);
    }
}

// 0x80152A6C
void ftPikachuSpecialAirHiProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.anim_frames--;

    if (fp->status_vars.pikachu.specialhi.anim_frames <= 0) // NOW you're checking if less than too?
    {
        ftPikachuSpecialAirHiEndSetStatus(fighter_gobj);
    }
}

// 0x80152AA0
void ftPikachuSpecialHiUpdateModelPitchScale(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->joints[FTPIKACHU_QUICKATTACK_BASE_JOINT]->rotate.vec.f.x = (syUtilsArcTan2(fp->physics.vel_air.x, fp->physics.vel_air.y) * fp->lr) - F_CST_DTOR32(90.0F);

    fp->joints[FTPIKACHU_QUICKATTACK_BASE_JOINT]->scale.vec.f.x = FTPIKACHU_QUICKATTACK_SCALE_X;
    fp->joints[FTPIKACHU_QUICKATTACK_BASE_JOINT]->scale.vec.f.y = FTPIKACHU_QUICKATTACK_SCALE_Y;
    fp->joints[FTPIKACHU_QUICKATTACK_BASE_JOINT]->scale.vec.f.z = FTPIKACHU_QUICKATTACK_SCALE_Z;

    ftParamsUpdateFighterPartsTransformAll(fp->joints[FTPIKACHU_QUICKATTACK_BASE_JOINT]);
}

// 0x80152B24
void ftPikachuSpecialHiProcPhysics(GObj *fighter_gobj)
{
    ftPhysicsApplyGroundVelTransferAir(fighter_gobj);
    ftPikachuSpecialHiUpdateModelPitchScale(fighter_gobj);
}

// 0x80152B4C
void ftPikachuSpecialAirHiProcPhysics(GObj *fighter_gobj)
{
    ftPikachuSpecialHiUpdateModelPitchScale(fighter_gobj);
}

// 0x80152B6C
void ftPikachuSpecialHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        if (fp->coll_data.mask_curr & (MAP_FLAG_RWALL | MAP_FLAG_LWALL))
        {
            mpCommonSetFighterAir(fp);
            ftPikachuSpecialAirHiEndSetStatus(fighter_gobj);
        }
        else ftPikachuSpecialHiSwitchStatusAir(fighter_gobj);
    }
#if defined(REGION_US)
    else if (fp->coll_data.mask_curr & (MAP_FLAG_RWALL | MAP_FLAG_LWALL))
#else
    if (fp->coll_data.mask_curr & (MAP_FLAG_RWALL | MAP_FLAG_LWALL))
#endif
    {
        ftPikachuSpecialHiEndSetStatus(fighter_gobj);
    }
}

// 0x80152BF4
sb32 ftPikachuSpecialHiProcPass(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) || (fp->status_vars.pikachu.specialhi.pass_timer >= FTPIKACHU_QUICKATTACK_PASS_BUFFER_MAX))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80152C2C
void ftPikachuSpecialAirHiProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.pass_timer++;

    if (mpCommonCheckFighterPassCliff(fighter_gobj, ftPikachuSpecialHiProcPass) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);

        }
        else if (syVectorAngleDiff3D(&fp->coll_data.floor_angle, &fp->physics.vel_air) > FTPIKACHU_QUICKATTACK_HALT_ANGLE)
        {
            mpCommonSetFighterGround(fp);
            ftPikachuSpecialHiEndSetStatus(fighter_gobj);
        }
        else ftPikachuSpecialAirHiSwitchStatusGround(fighter_gobj);
    }
    else
    {
        if ((fp->coll_data.mask_curr & MAP_FLAG_CEIL) && (syVectorAngleDiff3D(&fp->coll_data.ceil_angle, &fp->physics.vel_air) > FTPIKACHU_QUICKATTACK_HALT_ANGLE))
        {
            ftPikachuSpecialAirHiEndSetStatus(fighter_gobj);
        }
        if ((fp->coll_data.mask_curr & MAP_FLAG_LWALL) && (syVectorAngleDiff3D(&fp->coll_data.lwall_angle, &fp->physics.vel_air) > FTPIKACHU_QUICKATTACK_HALT_ANGLE))
        {
            ftPikachuSpecialAirHiEndSetStatus(fighter_gobj);
        }
        if ((fp->coll_data.mask_curr & MAP_FLAG_RWALL) && (syVectorAngleDiff3D(&fp->coll_data.rwall_angle, &fp->physics.vel_air) > FTPIKACHU_QUICKATTACK_HALT_ANGLE))
        {
            ftPikachuSpecialAirHiEndSetStatus(fighter_gobj);
        }
    }
}

// 0x80152D98
void ftPikachuSpecialAirHiSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialHi, fighter_gobj->anim_frame, 0.0F, FTPIKACHU_SPECIALHI_STATUS_FLAGS);
}

// 0x80152DD8
void ftPikachuSpecialHiSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirHi, fighter_gobj->anim_frame, 0.0F, FTPIKACHU_SPECIALHI_STATUS_FLAGS);

    fp->jumps_used = fp->attr->jumps_max;
}

// 0x80152E2C
void ftPikachuSpecialHiInitStatusVarsZip(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.anim_frames = FTPIKACHU_QUICKATTACK_ZIP_TIME;
    fp->jumps_used = fp->attr->jumps_max;
}

// 0x80152E48
void ftPikachuSpecialHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 sqrt_stick_range;
    f32 temp_stick_x;
    Vec3f stick_range;
    f32 temp_stick_y;

    temp_stick_x = fp->input.pl.stick_range.x;
    temp_stick_y = fp->input.pl.stick_range.y;

    sqrt_stick_range = sqrtf(SQUARE(temp_stick_x) + SQUARE(temp_stick_y));

    if (sqrt_stick_range > F_CONTROLLER_RANGE_MAX)
    {
        sqrt_stick_range = F_CONTROLLER_RANGE_MAX;
    }
    if (!(sqrt_stick_range < FTPIKACHU_QUICKATTACK_STICK_RANGE_MIN) && !(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS))
    {
        stick_range.x = fp->input.pl.stick_range.x;
        stick_range.y = fp->input.pl.stick_range.y;
        stick_range.z = 0.0F;

        if (syVectorAngleDiff3D(&fp->coll_data.floor_angle, &stick_range) < F_CST_DTOR32(90.0F)) goto block_end; /* HALF_PI32 */

        fp->status_vars.pikachu.specialhi.stick_range.x = stick_range.x;
        fp->status_vars.pikachu.specialhi.stick_range.y = stick_range.y;

        ftParamSetStickLR(fp);
        ftPikachuSpecialHiInitStatusVarsZip(fighter_gobj);

        fp->physics.vel_ground.x = (FTPIKACHU_QUICKATTACK_VEL_BASE * sqrt_stick_range) + FTPIKACHU_QUICKATTACK_VEL_ADD;

        if (fp->status_vars.pikachu.specialhi.is_subsequent_zip != FALSE)
        {
            fp->physics.vel_ground.x *= FTPIKACHU_QUICKATTACK_VEL_MUL;
        }
        ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialHi, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);

        return;
    }
block_end:
    mpCommonSetFighterAir(fp);

    ftPikachuSpecialAirHiSetStatus(fighter_gobj);
}

// 0x80152FEC
void ftPikachuSpecialAirHiSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 angle;
    f32 sqrt_stick_range = sqrtf(SQUARE((f32)fp->input.pl.stick_range.x) + SQUARE((f32)fp->input.pl.stick_range.y));

    if (sqrt_stick_range > F_CONTROLLER_RANGE_MAX)
    {
        sqrt_stick_range = F_CONTROLLER_RANGE_MAX;
    }
    ftParamSetStickLR(fp);

    if (sqrt_stick_range > FTPIKACHU_QUICKATTACK_STICK_RANGE_MIN)
    {
        angle = syUtilsArcTan2(fp->input.pl.stick_range.y, fp->input.pl.stick_range.x * fp->lr);

        fp->status_vars.pikachu.specialhi.stick_range.x = fp->input.pl.stick_range.x;
        fp->status_vars.pikachu.specialhi.stick_range.y = fp->input.pl.stick_range.y;
    }
    else
    {
        angle = F_CST_DTOR32(90.0F);
        sqrt_stick_range = F_CONTROLLER_RANGE_MAX;

        fp->status_vars.pikachu.specialhi.stick_range.x = 0;
        fp->status_vars.pikachu.specialhi.stick_range.y = I_CONTROLLER_RANGE_MAX;
    }
    ftPikachuSpecialHiInitStatusVarsZip(fighter_gobj);

    fp->physics.vel_air.x = __cosf(angle) * ((FTPIKACHU_QUICKATTACK_VEL_BASE * sqrt_stick_range) + FTPIKACHU_QUICKATTACK_VEL_ADD) * fp->lr;
    fp->physics.vel_air.y = __sinf(angle) * ((FTPIKACHU_QUICKATTACK_VEL_BASE * sqrt_stick_range) + FTPIKACHU_QUICKATTACK_VEL_ADD);

    if (fp->status_vars.pikachu.specialhi.is_subsequent_zip != FALSE)
    {
        fp->physics.vel_air.x *= FTPIKACHU_QUICKATTACK_VEL_MUL;
        fp->physics.vel_air.y *= FTPIKACHU_QUICKATTACK_VEL_MUL;
    }
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirHi, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x801531AC
sb32 ftPikachuSpecialHiCheckGotoSubZip(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f current_angle;
    Vec3f previous_angle;
    f32 unused[2];
    f32 temp_stick_x;
    f32 temp_stick_y;

    temp_stick_x = (f32)fp->input.pl.stick_range.x;
    temp_stick_y = (f32)fp->input.pl.stick_range.y;

    if (sqrtf((temp_stick_x * temp_stick_x) + (temp_stick_y * temp_stick_y)) < FTPIKACHU_QUICKATTACK_STICK_RANGE_MIN)
    {
        return FALSE;
    }
    else if (fp->status_vars.pikachu.specialhi.is_subsequent_zip == FALSE)
    {
        current_angle.x = fp->input.pl.stick_range.x;
        current_angle.y = fp->input.pl.stick_range.y;
        current_angle.z = 0.0F;

        previous_angle.x = fp->status_vars.pikachu.specialhi.stick_range.x;
        previous_angle.y = fp->status_vars.pikachu.specialhi.stick_range.y;
        previous_angle.z = 0.0F;

        if (syVectorAngleDiff3D(&previous_angle, &current_angle) > FTPIKACHU_QUICKATTACK_ANGLE_DIFF_MIN)
        {
            return TRUE;
        }
        else return FALSE;
    }
    else return FALSE;
}

// 0x801532B8
void ftPikachuSpecialHiEndProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 == 1)
    {
        if (ftPikachuSpecialHiCheckGotoSubZip(fighter_gobj) != FALSE)
        {
            fp->motion_vars.flags.flag1 = 0;

            fp->status_vars.pikachu.specialhi.is_subsequent_zip = TRUE;

            ftPikachuSpecialHiSetStatus(fighter_gobj);
        }
        else fp->motion_vars.flags.flag1 = 2;
    }
    else if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonWaitSetStatus(fighter_gobj);
    }
}

// 0x80153340
void ftPikachuSpecialAirHiEndProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 == 1)
    {
        if (ftPikachuSpecialHiCheckGotoSubZip(fighter_gobj) != FALSE)
        {
            fp->motion_vars.flags.flag1 = 0;
            fp->status_vars.pikachu.specialhi.is_subsequent_zip = TRUE;

            ftPikachuSpecialAirHiSetStatus(fighter_gobj);
        }
        else fp->motion_vars.flags.flag1 = 2;
    }
    else if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTPIKACHU_QUICKATTACK_FALLSPECIAL_DRIFT, FALSE, TRUE, TRUE, FTPIKACHU_QUICKATTACK_LANDING_LAG, FALSE);
    }
}

// 0x801533E4
void ftPikachuSpecialHiEndProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftPhysicsApplyGroundVelFriction(fighter_gobj);
    }
}

// 0x80153414
void ftPikachuSpecialAirHiEndProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        FTAttributes *attr;

        ftPhysicsApplyGravityDefault(fp, fp->attr);

        attr = fp->attr;

        ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel * FTPIKACHU_QUICKATTACK_AIR_ACCEL_MUL, attr->air_speed_max_x * FTPIKACHU_QUICKATTACK_AIR_SPEED_MUL);
       
        ftPhysicsApplyAirVelXFriction(fp, fp->attr);
    }
    else
    {
        fp->physics.vel_air.y -= (fp->physics.vel_air.y / FTPIKACHU_QUICKATTACK_VEL_Y_DIV);

        ftPhysicsApplyAirVelXFriction(fp, fp->attr);
    }
}

// 0x801534BC
void ftPikachuSpecialHiEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftPikachuSpecialHiEndSwitchStatusAir);
}

// 0x801534E0
void ftPikachuSpecialAirHiEndProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterCliff(fighter_gobj) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else ftCommonLandingFallSpecialSetStatus(fighter_gobj, FALSE, FTPIKACHU_QUICKATTACK_LANDING_LAG);
    }
}

// 0x80153544
void ftPikachuSpecialAirHiEndSwitchStatusGround(GObj *fighter_gobj) // Unused
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialHiEnd, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALHI_STATUS_FLAGS);
}

// 0x80153584
void ftPikachuSpecialHiEndSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirHiEnd, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALHI_STATUS_FLAGS);
}

// 0x801535C4
void ftPikachuSpecialHiEndBackupVel(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.pikachu.specialhi.vel_x_bak = fp->physics.vel_air.x;
    fp->status_vars.pikachu.specialhi.vel_y_bak = fp->physics.vel_air.y;
    fp->status_vars.pikachu.specialhi.vel_ground_bak = fp->physics.vel_ground.x;

    fp->physics.vel_air.x = fp->physics.vel_air.y = 0.0F;
    fp->physics.vel_ground.x = 0.0F;
}

// 0x801535F4
void ftPikachuSpecialHiEndSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPikachuSpecialHiEndBackupVel(fighter_gobj);

    fp->physics.vel_ground.x = fp->status_vars.pikachu.specialhi.vel_ground_bak * FTPIKACHU_QUICKATTACK_VEL_BAK_MUL;

    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80153654
void ftPikachuSpecialAirHiEndSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPikachuSpecialHiEndBackupVel(fighter_gobj);

    fp->physics.vel_air.x = fp->status_vars.pikachu.specialhi.vel_x_bak * FTPIKACHU_QUICKATTACK_VEL_BAK_MUL;
    fp->physics.vel_air.y = fp->status_vars.pikachu.specialhi.vel_y_bak * FTPIKACHU_QUICKATTACK_VEL_BAK_MUL;

    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
