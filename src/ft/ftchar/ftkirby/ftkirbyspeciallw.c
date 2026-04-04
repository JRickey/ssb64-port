#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80161360
void ftKirbySpecialLwUnused(GObj *fighter_gobj)
{
    return;
}

// 0x80161368
void ftKirbySpecialLwUpdateColAnim(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->damage_resist < FTKIRBY_STONE_HEALTH_MID) // Apply color overlay based on remaining Stone HP
    {
        if (fp->damage_resist < FTKIRBY_STONE_HEALTH_LOW)
        {
            if (fp->status_vars.kirby.speciallw.colanim_id != nGMColAnimFighterKirbySpeciaLwLow)
            {
                ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterKirbySpeciaLwLow, 0);
                fp->status_vars.kirby.speciallw.colanim_id = nGMColAnimFighterKirbySpeciaLwLow;
            }
        }
        else if (fp->status_vars.kirby.speciallw.colanim_id != nGMColAnimFighterKirbySpeciaLwMid)
        {
            ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterKirbySpeciaLwMid, 0);
            fp->status_vars.kirby.speciallw.colanim_id = nGMColAnimFighterKirbySpeciaLwMid;
        }
    }
    else if (fp->status_vars.kirby.speciallw.colanim_id != nGMColAnimFighterKirbySpeciaLwHigh)
    {
        ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterKirbySpeciaLwHigh, 0);
        fp->status_vars.kirby.speciallw.colanim_id = nGMColAnimFighterKirbySpeciaLwHigh;
    }
}

// 0x8016141C
void ftKirbySpecialLwSetDamageResist(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->is_damage_resist = TRUE;
    fp->damage_resist = FTKIRBY_STONE_HEALTH_MAX;

    fp->status_vars.kirby.speciallw.duration = FTKIRBY_STONE_DURATION_MAX;
    fp->status_vars.kirby.speciallw.colanim_id = nGMColAnimFighterKirbySpeciaLwHigh;

    ftParamCheckSetFighterColAnimID(fighter_gobj, nGMColAnimFighterKirbySpeciaLwHigh, 0);
}

// 0x80161468
void ftKirbySpecialLwSetDropFallVel(FTStruct *fp)
{
    fp->physics.vel_air.y = FTKIRBY_STONE_FALL_VEL;
}

// 0x80161478
f32 ftKirbySpecialLwGetGroundAxisYaw(FTStruct *fp)
{
    f32 rot_z = -syUtilsArcTan2(fp->coll_data.floor_angle.x, fp->coll_data.floor_angle.y);

    fp->joints[nFTPartsJointTopN]->rotate.vec.f.z = rot_z;

    return rot_z;
}

// 0x801614B4
sb32 ftKirbySpecialLwCheckRelease(GObj *fighter_gobj, sb32 is_allow_release)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (is_allow_release == TRUE)
    {
        if (fp->input.pl.button_tap & fp->input.button_mask_b)
        {
            return TRUE;
        }
    }
    else if ((fp->status_vars.kirby.speciallw.duration < (FTKIRBY_STONE_DURATION_MAX - FTKIRBY_STONE_DURATION_MIN)) && (fp->input.pl.button_tap & fp->input.button_mask_b))
    {
        return TRUE;
    }
    if (fp->status_vars.kirby.speciallw.duration > 0)
    {
        fp->status_vars.kirby.speciallw.duration--;

        return FALSE;
    }
    else return TRUE;
}

// 0x80161530
void ftKirbySpecialLwStartProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialLwHold, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
        ftKirbySpecialLwSetDamageResist(fighter_gobj);

        fp->motion_vars.flags.flag1 = 0;
    }
    else ftAnimEndCheckSetStatus(fighter_gobj, ftKirbySpecialLwUnkSetStatus);
}

// 0x80161598
void ftKirbySpecialAirLwStartProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftKirbySpecialLwSetDamageResist(fighter_gobj);

        fp->motion_vars.flags.flag1 = 0;
    }
    ftAnimEndCheckSetStatus(fighter_gobj, ftKirbySpecialAirLwHoldSetStatus);
}

// 0x801615E4
void ftKirbySpecialLwUnkDecideNextStatus(GObj *fighter_gobj, sb32 ga)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->is_damage_resist) && (fp->motion_vars.flags.flag1 != 0))
    {
        if (ga == nMPKineticsGround)
        {
            ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialLwHold, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
        }
        ftKirbySpecialLwSetDamageResist(fighter_gobj);

        fp->motion_vars.flags.flag1 = 0;
        fp->motion_vars.flags.flag2 = 1;
    }
    if (fp->motion_vars.flags.flag2 != 0)
    {
        if (ftKirbySpecialLwCheckRelease(fighter_gobj, FALSE) == TRUE)
        {
            (ga == nMPKineticsGround) ? ftKirbySpecialLwEndSetStatus(fighter_gobj) : ftKirbySpecialAirLwEndSetStatus(fighter_gobj);

            fp->motion_vars.flags.flag2 = 0;
        }
        ftKirbySpecialLwUpdateColAnim(fighter_gobj);
    }
}

// 0x801616B0
void ftKirbySpecialLwUnkProcUpdate(GObj *fighter_gobj)
{
    ftKirbySpecialLwUnkDecideNextStatus(fighter_gobj, nMPKineticsGround);
}

// 0x801616D0
void ftKirbySpecialAirLwHoldProcUpdate(GObj *fighter_gobj)
{
    ftKirbySpecialLwUnkDecideNextStatus(fighter_gobj, nMPKineticsAir);
}

// 0x801616F0
void ftKirbySpecialLwHoldDecideNextStatus(GObj *fighter_gobj, sb32 ga)
{
    if (ftKirbySpecialLwCheckRelease(fighter_gobj, TRUE) == TRUE)
    {
        (ga == nMPKineticsGround) ? ftKirbySpecialLwEndSetStatus(fighter_gobj) : ftKirbySpecialAirLwEndSetStatus(fighter_gobj);
    }
    ftKirbySpecialLwUpdateColAnim(fighter_gobj);
}

// 0x8016174C
void ftKirbySpecialLwHoldProcUpdate(GObj *fighter_gobj)
{
    ftKirbySpecialLwHoldDecideNextStatus(fighter_gobj, nMPKineticsGround);
}

// 0x8016176C
void ftKirbySpecialAirLwFallProcUpdate(GObj *fighter_gobj)
{
    ftKirbySpecialLwHoldDecideNextStatus(fighter_gobj, nMPKineticsAir);
}

// 0x8016178C
void ftKirbySpecialLwHoldProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    f32 temp_slide_angle;
    f32 ground_vel_x;
    f32 slide_angle;

    temp_slide_angle = ftKirbySpecialLwGetGroundAxisYaw(fp);

    slide_angle = temp_slide_angle;

    if (FTKIRBY_STONE_SLIDE_ANGLE < temp_slide_angle)
    {
        slide_angle = FTKIRBY_STONE_SLIDE_ANGLE;
    }

    else if (temp_slide_angle < -FTKIRBY_STONE_SLIDE_ANGLE)
    {
        slide_angle = -FTKIRBY_STONE_SLIDE_ANGLE;
    }
    fp->physics.vel_ground.x -= (__sinf(slide_angle) * FTKIRBY_STONE_SLIDE_VEL_MUL * fp->lr);

    ground_vel_x = fp->physics.vel_ground.x;

    if (ground_vel_x > FTKIRBY_STONE_SLIDE_CLAMP_VEL_X)
    {
        fp->physics.vel_ground.x = FTKIRBY_STONE_SLIDE_CLAMP_VEL_X;
    }
    else if (ground_vel_x < -FTKIRBY_STONE_SLIDE_CLAMP_VEL_X)
    {
        fp->physics.vel_ground.x = -FTKIRBY_STONE_SLIDE_CLAMP_VEL_X;
    }

    ftPhysicsSetGroundVelFriction(fp, (dMPCollisionMaterialFrictions[fp->coll_data.floor_flags & MAP_VERTEX_MAT_MASK] * attr->traction * FTKIRBY_STONE_SLIDE_TRACTION_MUL));
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x801618C4
void ftKirbySpecialLwStartProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        mpCommonSetFighterAir(fp);
        ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwStart, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
        ftKirbySpecialLwSetDropFallVel(fp);
    }
}

// 0x80161920
void ftKirbySpecialLwUnkProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        mpCommonSetFighterAir(fp);
        ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwHold, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
    }
}

// 0x80161974
void ftKirbySpecialLwHoldProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        mpCommonSetFighterAir(fp);
        ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwFall, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));

        fp->is_damage_resist = TRUE;

        ftKirbySpecialLwSetDropFallVel(fp);
    }
}

// 0x801619E0
void ftKirbySpecialAirLwStartProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        mpCommonSetFighterGround(fp);
        ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialLwStart, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
    }
}

// 0x80161A30
void ftKirbySpecialAirLwHoldProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        mpCommonSetFighterGround(fp);
        ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwLanding, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));

        fp->is_damage_resist = TRUE;
    }
}

// 0x80161A94
void ftKirbySpecialLwStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialLwStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->motion_vars.flags.flag2 = 0;
    fp->motion_vars.flags.flag1 = 0;

    fp->physics.vel_air.x = fp->physics.vel_air.y = 0.0F;
}

// 0x80161AEC
void ftKirbySpecialLwUnkSetStatus(GObj *fighter_gobj) // Unused
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialLwUnk, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    ftKirbySpecialLwUnused(fighter_gobj);
}

// 0x80161B2C
void ftKirbySpecialLwEndSetStatus(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80161B70
void ftKirbySpecialAirLwStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    ub32 is_damage_resist = fp->is_damage_resist;

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwStart, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM));
    ftMainPlayAnimEventsAll(fighter_gobj);

    if (fp->is_damage_resist = is_damage_resist & TRUE) // wat
    {
        fp->motion_vars.flags.flag2 = 1;
    }
    else
    {
        fp->motion_vars.flags.flag2 = 0;
        fp->motion_vars.flags.flag1 = 0;
    }
    fp->physics.vel_air.x = fp->physics.vel_air.y = 0.0F;
}

// 0x80161C0C
void ftKirbySpecialAirLwHoldSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwHold, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbySpecialLwUnused(fighter_gobj);
    ftKirbySpecialLwSetDropFallVel(fp);
}

// 0x80161C5C
void ftKirbySpecialAirLwEndSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusSpecialAirLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
