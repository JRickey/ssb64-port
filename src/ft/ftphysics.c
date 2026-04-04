#include <ft/fighter.h>

// 0x800D87D0
void ftPhysicsSetGroundVelTransferAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->physics.vel_air.x = ((fp->lr * fp->coll_data.floor_angle.y * fp->physics.vel_ground.x) + fp->physics.vel_jostle_x);
    fp->physics.vel_air.y = (fp->lr * -fp->coll_data.floor_angle.x * fp->physics.vel_ground.x);
    fp->physics.vel_air.z = fp->physics.vel_jostle_z;

    if 
    (
        (fp->physics.vel_jostle_z > 0.0F)                                                 &&
        (DObjGetStruct(fighter_gobj)->translate.vec.f.z < 0.0F)                           &&
        ((DObjGetStruct(fighter_gobj)->translate.vec.f.z + fp->physics.vel_air.z) > 0.0F) ||
        (fp->physics.vel_air.z < 0.0F)                                                    &&
        (DObjGetStruct(fighter_gobj)->translate.vec.f.z > 0.0F)                           &&
        ((DObjGetStruct(fighter_gobj)->translate.vec.f.z + fp->physics.vel_air.z) < 0.0F)
    )
    {
        fp->physics.vel_air.z = -DObjGetStruct(fighter_gobj)->translate.vec.f.z;
    }
    if ((fp->physics.vel_air.z > 0.0F) && ((DObjGetStruct(fighter_gobj)->translate.vec.f.z + fp->physics.vel_air.z) > 60.0F))
    {
        fp->physics.vel_air.z = (60.0F - DObjGetStruct(fighter_gobj)->translate.vec.f.z);
    }
    else if (fp->physics.vel_air.z < 0.0F)
    {
        if ((DObjGetStruct(fighter_gobj)->translate.vec.f.z + fp->physics.vel_air.z) < -60.0F)
        {
            fp->physics.vel_air.z = (-60.0F - DObjGetStruct(fighter_gobj)->translate.vec.f.z);
        }
    }
    fp->physics.vel_air.z += fp->physics.vel_ground.z;
}

// 0x800D8938
void ftPhysicsClampGroundVel(FTStruct *fp, f32 clamp)
{
    if (fp->physics.vel_ground.x < -clamp)
    {
        fp->physics.vel_ground.x = -clamp;
    }
    else if (fp->physics.vel_ground.x > clamp)
    {
        fp->physics.vel_ground.x = clamp;
    }
}

// 0x800D8978
void ftPhysicsSetGroundVelFriction(FTStruct *fp, f32 friction)
{
    if (fp->physics.vel_ground.x < 0.0F)
    {
        fp->physics.vel_ground.x += friction;

        if (fp->physics.vel_ground.x > 0.0F)
        {
            fp->physics.vel_ground.x = 0.0F;
        }
    }
    else
    {
        fp->physics.vel_ground.x -= friction;

        if (fp->physics.vel_ground.x < 0.0F)
        {
            fp->physics.vel_ground.x = 0.0F;
        }
    }
}

// 0x800D89E0
void ftPhysicsApplyClampGroundVelStickRange(FTStruct *fp, s32 stick_x_min, f32 vel, f32 clamp)
{
    if (ABS(fp->input.pl.stick_range.x) >= stick_x_min)
    {
        fp->physics.vel_ground.x += (fp->input.pl.stick_range.x * vel * fp->lr);

        if (fp->physics.vel_ground.x < -clamp)
        {
            fp->physics.vel_ground.x = -clamp;
        }
        else if (fp->physics.vel_ground.x > clamp)
        {
            fp->physics.vel_ground.x = clamp;
        }
    }
}

// 0x800D8A70
void ftPhysicsSetGroundVelAbsStickRange(FTStruct *fp, f32 vel, f32 friction)
{
    f32 v = ABS(fp->input.pl.stick_range.x) * vel;

    if (fp->physics.vel_ground.x < v)
    {
        fp->physics.vel_ground.x = v;
    }
    else
    {
        fp->physics.vel_ground.x -= friction;

        if (fp->physics.vel_ground.x < v)
        {
            fp->physics.vel_ground.x = v;
        }
    }
}

// 0x800D8ADC
void ftPhysicsSetGroundVelStickRange(FTStruct *fp, f32 vel, f32 friction)
{
    f32 v = fp->input.pl.stick_range.x * vel * fp->lr;

    if (fp->physics.vel_ground.x >= 0.0F)
    {
        if (fp->physics.vel_ground.x < v)
        {
            fp->physics.vel_ground.x = v;
        }
        else
        {
            fp->physics.vel_ground.x -= friction;

            if (fp->physics.vel_ground.x < v)
            {
                fp->physics.vel_ground.x = v;
            }
        }
    }
    else
    {
        if (fp->physics.vel_ground.x > v)
        {
            fp->physics.vel_ground.x = v;
        }
        else
        {
            fp->physics.vel_ground.x += friction;

            if (v < fp->physics.vel_ground.x)
            {
                fp->physics.vel_ground.x = v;
            }
        }
    }
}

// 0x800D8B94
void ftPhysicsApplyGroundVelTransferAir(GObj *fighter_gobj)
{
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x800D8BB4
void ftPhysicsApplyGroundVelFriction(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsSetGroundVelFriction(fp, dMPCollisionMaterialFrictions[fp->coll_data.floor_flags & MAP_VERTEX_MAT_MASK] * attr->traction);
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x800D8C14
void ftPhysicsApplyGroundVelTransN(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->physics.vel_ground.x = ((fp->joints[nFTPartsJointTransN]->translate.vec.f.z - fp->anim_vel.z) * DObjGetStruct(fighter_gobj)->scale.vec.f.z);
    fp->physics.vel_ground.z = ((fp->joints[nFTPartsJointTransN]->translate.vec.f.x - fp->anim_vel.x) * -fp->lr * DObjGetStruct(fighter_gobj)->scale.vec.f.x);

    if ((fp->lr * DObjGetStruct(fighter_gobj)->rotate.vec.f.y) < 0.0F)
    {
        fp->physics.vel_ground.x = -fp->physics.vel_ground.x;
        fp->physics.vel_ground.z = -fp->physics.vel_ground.z;
    }
    ftPhysicsSetGroundVelTransferAir(fighter_gobj);
}

// 0x800D8CCC
void ftPhysicsApplyGroundFrictionOrTransN(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->anim_desc.flags.is_use_transn_joint)
    {
        ftPhysicsApplyGroundVelTransN(fighter_gobj);
    }
    else ftPhysicsApplyGroundVelFriction(fighter_gobj);
}

// 0x800D8D10
void ftPhysicsClampAirVelY(FTStruct *fp, f32 clamp)
{
    if (fp->physics.vel_air.y > clamp)
    {
        fp->physics.vel_air.y = clamp;
    }
}

// 0x800D8D34
void ftPhysicsAddClampAirVelY(FTStruct *fp, f32 vel, f32 clamp)
{
    fp->physics.vel_air.y += vel;

    if (fp->physics.vel_air.y > clamp)
    {
        fp->physics.vel_air.y = clamp;
    }
}

// 0x800D8D68
void ftPhysicsApplyGravityClampTVel(FTStruct *fp, f32 gravity, f32 tvel)
{
    fp->physics.vel_air.y -= gravity;

    if (fp->physics.vel_air.y < -tvel)
    {
        fp->physics.vel_air.y = -tvel;
    }
}

// 0x800D8DA0
void ftPhysicsApplyFastFall(FTStruct *fp, FTAttributes *attr)
{
    fp->physics.vel_air.y = -attr->tvel_fast;
}

// 0x800D8DB0
void ftPhysicsCheckSetFastFall(FTStruct *fp)
{
    if (!(fp->is_fastfall) && (fp->physics.vel_air.y < 0.0F) && (fp->input.pl.stick_range.y <= FTCOMMON_FASTFALL_STICK_RANGE_MIN) && (fp->tap_stick_y < FTCOMMON_FASTFALL_BUFFER_TICS_MAX))
    {
        fp->is_fastfall = TRUE;

        fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;

        if (ftParamCheckSetFighterColAnimID(fp->fighter_gobj, nGMColAnimFighterFastFall, 0) != FALSE)
        {
            ftMainRunUpdateColAnim(fp->fighter_gobj);
        }
    }
}

// 0x800D8E50
void ftPhysicsApplyGravityDefault(FTStruct *fp, FTAttributes *attr)
{
    ftPhysicsApplyGravityClampTVel(fp, attr->gravity, attr->tvel_base);
}

// 0x800D8E78
void ftPhysicsClampAirVelX(FTStruct *fp, f32 clamp)
{
    if (fp->physics.vel_air.x < -clamp)
    {
        fp->physics.vel_air.x = -clamp;
    }
    else if (fp->physics.vel_air.x > clamp)
    {
        fp->physics.vel_air.x = clamp;
    }
}

// 0x800D8EB8
void ftPhysicsClampAirVelXMax(FTStruct *fp)
{
    ftPhysicsClampAirVelX(fp, fp->attr->air_speed_max_x);
}

// 0x800D8EDC
sb32 ftPhysicsCheckClampAirVelXDec(FTStruct *fp, f32 clamp)
{
    if (ABSF(fp->physics.vel_air.x) > clamp)
    {
        fp->physics.vel_air.x += ((fp->physics.vel_air.x >= 0.0F) ? -1.0F : 1.0F);

        if (ABSF(fp->physics.vel_air.x) < clamp)
        {
            if (fp->physics.vel_air.x >= 0.0F)
            {
                fp->physics.vel_air.x = clamp;
            }
            else fp->physics.vel_air.x = -clamp;
        }
        return TRUE;
    }
    else return FALSE;
}

// 0x800D8FA8
sb32 ftPhysicsCheckClampAirVelXDecMax(FTStruct *fp, FTAttributes *attr)
{
    return ftPhysicsCheckClampAirVelXDec(fp, attr->air_speed_max_x);
}

// 0x800D8FC8
void ftPhysicsClampAirVelXStickRange(FTStruct *fp, s32 stick_range_min, f32 vel, f32 clamp)
{
    if (ABS(fp->input.pl.stick_range.x) >= stick_range_min)
    {
        fp->physics.vel_air.x += (fp->input.pl.stick_range.x * vel);

        if (fp->physics.vel_air.x < -clamp)
        {
            fp->physics.vel_air.x = -clamp;
        }
        else if (fp->physics.vel_air.x > clamp)
        {
            fp->physics.vel_air.x = clamp;
        }
    }
}

// 0x800D9044
void ftPhysicsClampAirVelXStickDefault(FTStruct *fp, FTAttributes *attr)
{
    ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel, attr->air_speed_max_x);
}

// 0x800D9074
void ftPhysicsApplyAirVelXFriction(FTStruct *fp, FTAttributes *attr)
{
    if (fp->physics.vel_air.x < 0.0F)
    {
        fp->physics.vel_air.x += attr->air_friction;

        if (fp->physics.vel_air.x >= 0.0F)
        {
            fp->physics.vel_air.x = 0.0F;
        }
    }
    else
    {
        fp->physics.vel_air.x -= attr->air_friction;

        if (fp->physics.vel_air.x <= 0.0F)
        {
            fp->physics.vel_air.x = 0.0F;
        }
    }
}

// 0x800D90E0
void ftPhysicsApplyAirVelDrift(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    (fp->is_fastfall) ? ftPhysicsApplyFastFall(fp, attr) : ftPhysicsApplyGravityDefault(fp, attr);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsClampAirVelXStickDefault(fp, attr);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x800D9160
void ftPhysicsApplyAirVelDriftFastFall(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsCheckSetFastFall(fp);

    (fp->is_fastfall) ? ftPhysicsApplyFastFall(fp, attr) : ftPhysicsApplyGravityDefault(fp, attr);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsClampAirVelXStickDefault(fp, attr);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x800D91EC
void ftPhysicsApplyAirVelFriction(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    (fp->is_fastfall) ? ftPhysicsApplyFastFall(fp, attr) : ftPhysicsApplyGravityDefault(fp, attr);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x800D9260
void ftPhysicsGetAirVelTransN(FTStruct *fp, f32 *z, f32 *y, f32 *x) // Ness / Yoshi double jump physics
{
    DObj *topn_joint = fp->joints[nFTPartsJointTopN];
    DObj *transn_joint = fp->joints[nFTPartsJointTransN];
    f32 anim_vel_z = (transn_joint->translate.vec.f.z - fp->anim_vel.z) * fp->lr * topn_joint->scale.vec.f.z;
    f32 anim_vel_y = (transn_joint->translate.vec.f.y - fp->anim_vel.y) * topn_joint->scale.vec.f.y;
    f32 cos = cosf(transn_joint->rotate.vec.f.z);
    f32 sin = __sinf(transn_joint->rotate.vec.f.z);

    if (z != NULL)
    {
        *z = (anim_vel_z * cos) - (anim_vel_y * sin);
    }
    if (y != NULL)
    {
        *y = (anim_vel_z * sin) + (anim_vel_y * cos);
    }
    if (x != NULL)
    {
        *x = (transn_joint->translate.vec.f.x - fp->anim_vel.x) * -fp->lr * topn_joint->scale.vec.f.x;
    }
}

// 0x800D938C
void ftPhysicsSetAirVelTransN(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *topn_joint = fp->joints[nFTPartsJointTopN];
    DObj *transn_joint = fp->joints[nFTPartsJointTransN];

    fp->physics.vel_air.x = (transn_joint->translate.vec.f.x - fp->anim_vel.x) * topn_joint->scale.vec.f.x;
    fp->physics.vel_air.y = (transn_joint->translate.vec.f.y - fp->anim_vel.y) * topn_joint->scale.vec.f.y;
    fp->physics.vel_air.z = (transn_joint->translate.vec.f.z - fp->anim_vel.z) * topn_joint->scale.vec.f.z;
}

// 0x800D93E4
void ftPhysicsApplyAirVelTransNAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPhysicsGetAirVelTransN(fp, &fp->physics.vel_air.x, &fp->physics.vel_air.y, &fp->physics.vel_air.z);
}

// 0x800D9414
void ftPhysicsApplyAirVelTransNYZ(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPhysicsGetAirVelTransN(fp, NULL, &fp->physics.vel_air.y, &fp->physics.vel_air.z);
}

// 0x800D9444
void ftPhysicsStopVelAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->physics.vel_air.x = fp->physics.vel_air.y = fp->physics.vel_air.z = 0.0F;

    fp->physics.vel_ground.x = fp->physics.vel_ground.y = fp->physics.vel_ground.z = 0.0F;

    fp->physics.vel_damage_air.x = fp->physics.vel_damage_air.y = fp->physics.vel_damage_air.z = 0.0F;

    fp->physics.vel_damage_ground = 0.0F;
}