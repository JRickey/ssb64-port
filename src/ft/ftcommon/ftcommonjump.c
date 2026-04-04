#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013F660
void ftCommonJumpProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonSpecialAirCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonAttackAirCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonJumpAerialCheckInterruptCommon(fighter_gobj);
    }
}

// 0x8013F6A0
void ftCommonJumpGetJumpForceButton(s32 stick_range_x, s32 *jump_vel_x, s32 *jump_vel_y, sb32 is_shorthop)
{
    f32 sqrt_vel_x;
    f32 vel_y;
    f32 vel_x;

    vel_x = ABS(stick_range_x);

    sqrt_vel_x = sqrtf(1.0F - SQUARE(vel_x / F_CONTROLLER_RANGE_MAX));

    if (is_shorthop == FALSE)
    {
        vel_y = (FTCOMMON_KNEEBEND_BUTTON_LONG_FORCE * sqrt_vel_x) + FTCOMMON_KNEEBEND_BUTTON_LONG_MIN;

        if ((SQUARE(vel_x) + SQUARE(vel_y)) > SQUARE(F_CONTROLLER_RANGE_MAX))
        {
            vel_y = sqrtf(SQUARE(F_CONTROLLER_RANGE_MAX) - SQUARE(vel_x));
        }
        if (vel_y < FTCOMMON_KNEEBEND_BUTTON_LONG_MIN)
        {
            vel_y = FTCOMMON_KNEEBEND_BUTTON_LONG_MIN;
        }
    }
    else
    {
        vel_y = (FTCOMMON_KNEEBEND_BUTTON_SHORT_FORCE * sqrt_vel_x) + FTCOMMON_KNEEBEND_BUTTON_SHORT_MIN;

        if ((SQUARE(vel_x) + SQUARE(vel_y)) > SQUARE(F_CONTROLLER_RANGE_MAX))
        {
            vel_y = sqrtf(SQUARE(F_CONTROLLER_RANGE_MAX) - SQUARE(vel_x));
        }
        if (vel_y < FTCOMMON_KNEEBEND_BUTTON_SHORT_MIN)
        {
            vel_y = FTCOMMON_KNEEBEND_BUTTON_SHORT_MIN;
        }
    }
    if (vel_y > FTCOMMON_KNEEBEND_BUTTON_HEIGHT_CLAMP)
    {
        vel_y = FTCOMMON_KNEEBEND_BUTTON_HEIGHT_CLAMP;
    }
    *jump_vel_x = (stick_range_x >= 0) ? vel_x : -vel_x;

    *jump_vel_y = vel_y;
}

// 0x8013F880
void ftCommonJumpSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    s32 status_id;
    s32 vel_x, vel_y;

    mpCommonSetFighterAir(fp);

    status_id = ((fp->input.pl.stick_range.x * fp->lr) > FTCOMMON_KNEEBEND_JUMP_F_OR_B_RANGE) ? nFTCommonStatusJumpF : nFTCommonStatusJumpB;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    switch (fp->status_vars.common.kneebend.input_source)
    {
    case FTCOMMON_KNEEBEND_INPUT_TYPE_BUTTON:
        ftCommonJumpGetJumpForceButton(fp->input.pl.stick_range.x, &vel_x, &vel_y, fp->status_vars.common.kneebend.is_shorthop);
        break;

    case FTCOMMON_KNEEBEND_INPUT_TYPE_STICK:
    default:
        vel_x = fp->input.pl.stick_range.x;
        vel_y = fp->status_vars.common.kneebend.jump_force;

        if (vel_y < FTCOMMON_KNEEBEND_STICK_RANGE_MIN)
        {
            vel_y = FTCOMMON_KNEEBEND_STICK_RANGE_MIN;
        }
        break;
    }
    fp->physics.vel_air.y = (vel_y * attr->jump_height_mul) + attr->jump_height_base;
    fp->physics.vel_air.x = vel_x * attr->jump_vel_x;

    fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;

    fp->is_special_interrupt = TRUE;
}
