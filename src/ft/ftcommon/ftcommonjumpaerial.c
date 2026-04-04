#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188580
f32 dFTKirbyJumpAerialFVelocities[/* */] = { 60.0F, 52.0F, 47.0F, 40.0F };

// 0x801885A0
f32 dFTPurinJumpAerialFVelocities[/* */] = { 60.0F, 40.0F, 20.0F, 0.0F };

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013FA90
void ftCommonJumpAerialUpdateModelYaw(FTStruct *fp)
{
    if (fp->status_vars.common.jumpaerial.turn_tics != 0)
    {
        fp->status_vars.common.jumpaerial.turn_tics--;

        fp->joints[nFTPartsJointTopN]->rotate.vec.f.y += FTCOMMON_JUMPAERIAL_TURN_ROTATE_STEP;

        ftParamsUpdateFighterPartsTransformAll(fp->joints[nFTPartsJointTopN]);

        if (fp->status_vars.common.jumpaerial.turn_tics == (FTCOMMON_JUMPAERIAL_TURN_FRAMES / 2))
        {
            fp->lr = -fp->lr;
        }
    }
}

// 0x8013FB00
void ftCommonJumpAerialProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonJumpAerialUpdateModelYaw(fp);
    ftAnimEndSetFall(fighter_gobj);
}

// 0x8013FB2C
void ftCommonJumpAerialProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonSpecialAirCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonAttackAirCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonJumpAerialCheckInterruptCommon(fighter_gobj);
    }
}

// 0x8013FB6C
void ftYoshiJumpAerialProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsApplyAirVelTransNYZ(fighter_gobj);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsClampAirVelXStickDefault(fp, attr);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x8013FBC4
void ftNessJumpAerialProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    f32 vel_x;

    ftPhysicsGetAirVelTransN(fp, &fp->status_vars.common.jumpaerial.drift, &fp->physics.vel_air.y, &fp->physics.vel_air.z);

    fp->physics.vel_air.x = fp->status_vars.common.jumpaerial.vel_x;

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsClampAirVelXStickDefault(fp, attr);
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
    fp->status_vars.common.jumpaerial.vel_x = fp->physics.vel_air.x;

    fp->physics.vel_air.x += fp->status_vars.common.jumpaerial.drift;
}

// 0x8013FC4C
void ftCommonJumpAerialProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsCheckSetFastFall(fp);

    (fp->is_fastfall) ? ftPhysicsApplyFastFall(fp, attr) : ftPhysicsApplyGravityDefault(fp, attr);

    switch (fp->fkind)
    {
    case nFTKindKirby:
    case nFTKindNKirby:
        if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
        {
            ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel * FTKIRBY_JUMPAERIAL_VEL_MUL, attr->air_speed_max_x * FTKIRBY_JUMPAERIAL_VEL_MUL);
        }
        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
        {
            ftPhysicsClampAirVelXStickRange(fp, FTPHYSICS_AIRDRIFT_CLAMP_RANGE_MIN, attr->air_accel * FTPURIN_JUMPAERIAL_VEL_MUL, attr->air_speed_max_x * FTPURIN_JUMPAERIAL_VEL_MUL);
        }
        break;
    }
    ftPhysicsApplyAirVelXFriction(fp, attr);
}

// 0x8013FD74
void ftCommonJumpAerialSetStatus(GObj *fighter_gobj, s32 input_source)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    s32 status_id = ((fp->input.pl.stick_range.x * fp->lr) >= FTCOMMON_JUMPAERIAL_F_OR_B_RANGE) ? nFTCommonStatusJumpAerialF : nFTCommonStatusJumpAerialB;
    s32 stick_range_y = I_CONTROLLER_RANGE_MAX;
    s32 stick_range_x;

    // It would seem that the stick range jump mechanic was initially considered for double jumps as well...

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_PLAYERTAG);

    if ((fp->fkind == nFTKindYoshi) || (fp->fkind == nFTKindNYoshi))
    {
        fp->proc_physics = ftYoshiJumpAerialProcPhysics;
        fp->knockback_resist_status = FTYOSHI_JUMPAERIAL_KNOCKBACK_RESIST;
    }
    else if ((fp->fkind == nFTKindNess) || (fp->fkind == nFTKindNNess))
    {
        fp->proc_physics = ftNessJumpAerialProcPhysics;
        fp->status_vars.common.jumpaerial.drift = 0.0F;
    }
    switch (input_source) // Last minute bruh moment from HAL
    {
    case FTCOMMON_JUMPAERIAL_INPUT_TYPE_BUTTON:
        stick_range_x = fp->input.pl.stick_range.x;
        break;

    case FTCOMMON_JUMPAERIAL_INPUT_TYPE_STICK:
    default:
        stick_range_x = fp->input.pl.stick_range.x;
        break;
    } 
    fp->physics.vel_air.y = (((stick_range_y * attr->jump_height_mul) + attr->jump_height_base) * attr->jumpaerial_height);

    if ((fp->fkind == nFTKindNess) || (fp->fkind == nFTKindNNess))
    {
        fp->status_vars.common.jumpaerial.vel_x = stick_range_x * attr->jumpaerial_vel_x;
    }
    else fp->physics.vel_air.x = stick_range_x * attr->jumpaerial_vel_x;

    fp->jumps_used++;

    fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;

    fp->is_special_interrupt = TRUE;

    if (((fp->fkind == nFTKindYoshi) || (fp->fkind == nFTKindNYoshi)) && ((fp->input.pl.stick_range.x * fp->lr) < FTCOMMON_JUMPAERIAL_TURN_STICK_RANGE_MIN))
    {
        fp->status_vars.common.jumpaerial.turn_tics = FTCOMMON_JUMPAERIAL_TURN_FRAMES;
    }
    else fp->status_vars.common.jumpaerial.turn_tics = 0;

    ftCommonJumpAerialUpdateModelYaw(fp);
}

// 0x8013FF38
void ftCommonJumpAerialMultiSetStatus(GObj *fighter_gobj, s32 input_source)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    s32 status_id;
    s32 stick_range_x;
    s32 stick_range_y = I_CONTROLLER_RANGE_MAX;

    switch (fp->fkind)
    {
    case nFTKindKirby:
    case nFTKindNKirby:
        status_id = fp->jumps_used + nFTKirbyStatusJumpAerialF1 - 1;
        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        status_id = fp->jumps_used + nFTPurinStatusJumpAerialF1 - 1;
        break;
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_PLAYERTAG);

    switch (input_source)
    {
    case FTCOMMON_JUMPAERIAL_INPUT_TYPE_BUTTON:
        stick_range_x = fp->input.pl.stick_range.x;
        break;

    case FTCOMMON_JUMPAERIAL_INPUT_TYPE_STICK:
    default:
        stick_range_x = fp->input.pl.stick_range.x;
        break;
    }

    fp->physics.vel_air.x = stick_range_x * attr->jumpaerial_vel_x;

    if (fp->jumps_used == 1)
    {
        fp->physics.vel_air.y = (((stick_range_y * attr->jump_height_mul) + attr->jump_height_base) * attr->jumpaerial_height);

        fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
    }
    else switch (fp->fkind)
    {
    case nFTKindKirby:
    case nFTKindNKirby:
        fp->physics.vel_air.y = dFTKirbyJumpAerialFVelocities[fp->jumps_used - 2] * (stick_range_y / F_CONTROLLER_RANGE_MAX);

        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        fp->physics.vel_air.y = dFTPurinJumpAerialFVelocities[fp->jumps_used - 2] * (stick_range_y / F_CONTROLLER_RANGE_MAX);

        break;
    }
    fp->jumps_used++;

    fp->motion_vars.flags.flag1 = 0;

    fp->is_special_interrupt = TRUE;

    if ((fp->input.pl.stick_range.x * fp->lr) < FTCOMMON_JUMPAERIAL_TURN_STICK_RANGE_MIN)
    {
        fp->status_vars.common.jumpaerial.turn_tics = FTCOMMON_JUMPAERIAL_TURN_FRAMES;
    }
    else fp->status_vars.common.jumpaerial.turn_tics = 0;

    ftCommonJumpAerialUpdateModelYaw(fp);
}

// 0x8014012C
sb32 ftCommonJumpAerialMultiCheckJumpButtonHold(FTStruct *fp)
{
    if (fp->input.pl.button_hold & (R_CBUTTONS | L_CBUTTONS | D_CBUTTONS | U_CBUTTONS))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80140150
s32 ftCommonJumpAerialMultiGetJumpInputType(FTStruct *fp)
{
    if (fp->input.pl.stick_range.y >= FTCOMMON_JUMPAERIAL_STICK_RANGE_MIN)
    {
        return FTCOMMON_JUMPAERIAL_INPUT_TYPE_STICK;
    }
    else if (ftCommonJumpAerialMultiCheckJumpButtonHold(fp) != FALSE)
    {
        return FTCOMMON_JUMPAERIAL_INPUT_TYPE_BUTTON;
    }
    else return FTCOMMON_JUMPAERIAL_INPUT_TYPE_NONE;
}

// 0x8014019C
sb32 ftCommonJumpAerialCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 input_source;

    if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
    {
        return FALSE;
    }
    else if ((fp->fkind == nFTKindKirby) || (fp->fkind == nFTKindNKirby) || (fp->fkind == nFTKindPurin) || (fp->fkind == nFTKindNPurin))
    {
        if (fp->jumps_used < fp->attr->jumps_max)
        {
            if (fp->jumps_used == 1)
            {
                input_source = ftCommonKneeBendGetInputTypeCommon(fp);

                if (input_source != FTCOMMON_JUMPAERIAL_INPUT_TYPE_NONE)
                {
                    ftCommonJumpAerialMultiSetStatus(fighter_gobj, input_source);

                    return TRUE;
                }
            }
            else switch (fp->fkind)
            {
            case nFTKindKirby:
            case nFTKindNKirby:
                if ((fp->status_id < nFTKirbyStatusJumpAerialF1) || (fp->status_id > nFTKirbyStatusJumpAerialF5) || (fp->motion_vars.flags.flag1 != 0))
                {
                    input_source = ftCommonJumpAerialMultiGetJumpInputType(fp);

                    if (input_source != FTCOMMON_JUMPAERIAL_INPUT_TYPE_NONE)
                    {
                        ftCommonJumpAerialMultiSetStatus(fighter_gobj, input_source);

                        return TRUE;
                    }
                }
                break;

            case nFTKindPurin:
            case nFTKindNPurin:
                if ((fp->status_id < nFTPurinStatusJumpAerialF1) || (fp->status_id > nFTPurinStatusJumpAerialF5) || (fp->motion_vars.flags.flag1 != 0))
                {
                    input_source = ftCommonJumpAerialMultiGetJumpInputType(fp);

                    if (input_source != FTCOMMON_JUMPAERIAL_INPUT_TYPE_NONE)
                    {
                        ftCommonJumpAerialMultiSetStatus(fighter_gobj, input_source);

                        return TRUE;
                    }
                }
                break;
            }
        }
    }
    else
    {
        input_source = ftCommonKneeBendGetInputTypeCommon(fp);

        if ((input_source != FTCOMMON_JUMPAERIAL_INPUT_TYPE_NONE) && (fp->jumps_used < fp->attr->jumps_max))
        {
            ftCommonJumpAerialSetStatus(fighter_gobj, input_source);

            return TRUE;
        }
    }
    return FALSE;
}
