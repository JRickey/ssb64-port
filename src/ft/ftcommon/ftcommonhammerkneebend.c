#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80147EC0
void ftCommonHammerKneeBendProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    fp->status_vars.common.hammer.kneebend_anim_frame++;

    if 
    (
        (fp->status_vars.common.hammer.input_source == FTCOMMON_KNEEBEND_INPUT_TYPE_BUTTON)     &&
        (fp->status_vars.common.hammer.kneebend_anim_frame <= FTCOMMON_KNEEBEND_SHORTHOP_FRAMES)&&
        (fp->input.pl.button_release & (R_CBUTTONS | L_CBUTTONS | D_CBUTTONS | U_CBUTTONS))
    )
    {
        fp->status_vars.common.hammer.is_shorthop = TRUE;
    }
    if (fp->status_vars.common.hammer.kneebend_anim_frame >= attr->kneebend_anim_length)
    {
        ftCommonHammerFallSetStatusJump(fighter_gobj);
    }
}

// 0x80147F54
void ftCommonHammerKneeBendProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.hammer.jump_force < fp->input.pl.stick_range.y)
    {
        fp->status_vars.common.hammer.jump_force = fp->input.pl.stick_range.y;
    }
}

// 0x80147F88
void ftCommonHammerKneeBendSetStatus(GObj *fighter_gobj, s32 input_source)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusHammerKneeBend, ftHammerGetAnimFrame(fighter_gobj), 1.0F, ftHammerGetStatUpdateFlags(fighter_gobj));
    ftHammerSetColAnim(fighter_gobj);

    fp->status_vars.common.hammer.jump_force = fp->input.pl.stick_range.y;
    fp->status_vars.common.hammer.kneebend_anim_frame = 0.0F;
    fp->status_vars.common.hammer.input_source = input_source;
    fp->status_vars.common.hammer.is_shorthop = FALSE;
}

// 0x8014800C
sb32 ftCommonHammerKneeBendCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 input_source = ftCommonKneeBendGetInputTypeCommon(fp);

    if (input_source != FTCOMMON_KNEEBEND_INPUT_TYPE_NONE)
    {
        ftCommonHammerKneeBendSetStatus(fighter_gobj, input_source);

        return TRUE;
    }
    else return FALSE;
}
