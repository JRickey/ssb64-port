#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013E690
void ftCommonTurnProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        fp->status_vars.common.turn.is_allow_turn_direction = TRUE;
        fp->status_vars.common.turn.is_disable_sa_interrupts = TRUE;

        fp->lr = -fp->lr;
        fp->physics.vel_ground.x = -fp->physics.vel_ground.x;
    }
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonWaitSetStatus(fighter_gobj);
    }
}

// Dawg what
void ftCommonTurnProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    sb32 is_interrupt_attacks4;

    if (fp->status_vars.common.turn.is_allow_turn_direction != FALSE)
    {
        fp->input.pl.button_tap |= fp->status_vars.common.turn.button_mask;
    }

    if (fp->status_vars.common.turn.is_disable_sa_interrupts == FALSE)
    {
        goto skip_interrupt_specials;
    } 
    if (ftCommonSpecialNCheckInterruptCommon(fighter_gobj) != FALSE) return;
    if (ftCommonSpecialHiCheckInterruptCommon(fighter_gobj) != FALSE) return;
    if (ftCommonSpecialLwCheckInterruptCommon(fighter_gobj) != FALSE) return;

skip_interrupt_specials:
    if (ftCommonCatchCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        if (fp->status_vars.common.turn.attacks4_buffer < 256)
        {
            fp->status_vars.common.turn.attacks4_buffer++;
        } 
        is_interrupt_attacks4 = (fp->status_vars.common.turn.attacks4_buffer < 6) ? ftCommonAttackS4CheckInterruptTurn(fighter_gobj) : ftCommonAttackS4CheckInterruptCommon(fighter_gobj);

        if (is_interrupt_attacks4 == FALSE)
        {
            if (fp->status_vars.common.turn.is_disable_sa_interrupts == FALSE) 
            {
                goto skip_interrupt_attacks;
            }
            if (ftCommonAttackHi4CheckInterruptCommon(fighter_gobj) != FALSE) return;
            if (ftCommonAttackLw4CheckInterruptCommon(fighter_gobj) != FALSE) return;
            if (ftCommonAttackS3CheckInterruptCommon(fighter_gobj) != FALSE) return;
            if (ftCommonAttackHi3CheckInterruptCommon(fighter_gobj) != FALSE) return;
            if (ftCommonAttackLw3CheckInterruptCommon(fighter_gobj) != FALSE) return;
            if (ftCommonAttack1CheckInterruptCommon(fighter_gobj) != FALSE) return;
            
        skip_interrupt_attacks:
            if 
            (
                (ftCommonGuardOnCheckInterruptCommon(fighter_gobj) != FALSE) ||
                (ftCommonAppealCheckInterruptCommon(fighter_gobj) != FALSE)  ||
                (ftCommonKneeBendCheckInterruptCommon(fighter_gobj) != FALSE)
            )
            {
                return;
            }
            ftCommonDashCheckTurn(fighter_gobj);

            if (fp->status_vars.common.turn.is_allow_turn_direction != FALSE)
            {
                if (fp->status_vars.common.turn.lr_dash != 0)
                {
                    if ((fp->input.pl.stick_range.x * fp->status_vars.common.turn.lr_turn) >= FTCOMMON_DASH_STICK_RANGE_MIN)
                    {
                        ftCommonDashSetStatus(fighter_gobj, 0);
                    }
                }
            }
            if (fp->input.pl.button_tap & fp->input.button_mask_a)
            {
                fp->status_vars.common.turn.button_mask |= fp->input.button_mask_a;
            }
            if (fp->input.pl.button_tap & fp->input.button_mask_b)
            {
                fp->status_vars.common.turn.button_mask |= fp->input.button_mask_b;
            }
            if (fp->status_vars.common.turn.is_allow_turn_direction != FALSE)
            {
                fp->status_vars.common.turn.is_allow_turn_direction = FALSE;
            }
        }
    }
}

// 0x8013E908
void ftCommonTurnSetStatus(GObj *fighter_gobj, s32 lr_dash)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;

    ftMainSetStatus(fighter_gobj, nFTCommonStatusTurn, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->status_vars.common.turn.is_allow_turn_direction = FALSE;
    fp->status_vars.common.turn.is_disable_sa_interrupts = FALSE;
    fp->status_vars.common.turn.button_mask = 0;
    fp->status_vars.common.turn.lr_dash = lr_dash;
    fp->status_vars.common.turn.attacks4_buffer = (lr_dash != 0) ? 0 : 256;
    fp->status_vars.common.turn.lr_turn = -fp->lr;
}

// 0x8013E988
void ftCommonTurnSetStatusCenter(GObj *fighter_gobj)
{
    ftCommonTurnSetStatus(fighter_gobj, 0);
}

// 0x8013E9A8
void ftCommonTurnSetStatusInvertLR(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonTurnSetStatus(fighter_gobj, -fp->lr);
}

// 0x8013ED90
sb32 ftCommonTurnCheckInputSuccess(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->input.pl.stick_range.x * fp->lr) <= FTCOMMON_TURN_STICK_RANGE_MIN)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8013EA04
sb32 ftCommonTurnCheckInterruptCommon(GObj *fighter_gobj)
{
    if (ftCommonTurnCheckInputSuccess(fighter_gobj) != FALSE)
    {
        ftCommonTurnSetStatusCenter(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
