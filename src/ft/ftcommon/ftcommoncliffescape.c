#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801457E0
sb32 ftCommonCliffEscapeCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->input.pl.button_tap & fp->input.button_mask_z)
    {
        ftCommonCliffQuickOrSlowSetStatus(fighter_gobj, 2);

        return TRUE;
    }
    else return FALSE;
}

// 0x80145824
void ftCommonCliffEscapeQuick1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonCliffEscapeQuick2SetStatus);
}

// 0x80145848
void ftCommonCliffEscapeSlow1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonCliffEscapeSlow2SetStatus);
}

// 0x8014586C
void ftCommonCliffEscapeQuick1SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffEscapeQuick1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_cliff_hold = TRUE;

    fp->proc_damage = ftCommonCliffCommonProcDamage;
}

// 0x801458BC
void ftCommonCliffEscapeSlow1SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffEscapeSlow1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp->is_cliff_hold = TRUE;

    fp->proc_damage = ftCommonCliffCommonProcDamage;
}

// 0x8014590C
void ftCommonCliffEscapeQuick2SetStatus(GObj *fighter_gobj)
{
    ftCommonCliffCommon2UpdateCollData(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffEscapeQuick2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCommonCliffCommon2InitStatusVars(fighter_gobj);
}

// 0x8014594C
void ftCommonCliffEscapeSlow2SetStatus(GObj *fighter_gobj)
{
    ftCommonCliffCommon2UpdateCollData(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTCommonStatusCliffEscapeSlow2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCommonCliffCommon2InitStatusVars(fighter_gobj);
}
