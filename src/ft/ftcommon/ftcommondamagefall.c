#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80143560
void ftCommonDamageFallProcInterrupt(GObj *fighter_gobj)
{
    if
    (
        (ftCommonSpecialAirCheckInterruptCommon(fighter_gobj) == FALSE)     && 
        (ftCommonAttackAirCheckInterruptCommon(fighter_gobj) == FALSE)      &&
        (ftCommonJumpAerialCheckInterruptCommon(fighter_gobj) == FALSE)
    )
    {
        ftCommonHammerFallCheckInterruptDamageFall(fighter_gobj);
    }
}

// 0x801435B0
void ftCommonDamageFallProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterCliff(fighter_gobj) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else if ((ftCommonPassiveStandCheckInterruptDamage(fighter_gobj) == FALSE) && (ftCommonPassiveCheckInterruptDamage(fighter_gobj) == FALSE))
        {
            ftCommonDownBounceSetStatus(fighter_gobj);
        }
    }
}

// 0x80143630
void ftCommonDamageFallClampRumble(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftPhysicsClampAirVelXMax(fp);
    ftParamMakeRumble(fp, 3, 0);
}

// 0x80143664
void ftCommonDamageFallSetStatusFromDamage(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTCommonStatusDamageFall, 0.0F, 1.0F, (FTSTATUS_PRESERVE_PLAYERTAG | FTSTATUS_PRESERVE_FASTFALL));
    ftCommonDamageFallClampRumble(fighter_gobj);
}

// 0x801436A0
void ftCommonDamageFallSetStatusFromCliffWait(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusDamageFall, 0.0F, 1.0F, FTSTATUS_PRESERVE_FASTFALL);
    ftCommonDamageFallClampRumble(fighter_gobj);

    fp->tics_since_last_z = FTINPUT_ZTRIGLAST_TICS_MAX;
}

// 0x801436F0
void func_ovl3_801436F0(GObj *fighter_gobj) // Unused
{
    ftMainSetStatus(fighter_gobj, nFTCommonStatusDamageFall, fighter_gobj->anim_frame, 1.0F, (FTSTATUS_PRESERVE_PLAYERTAG | FTSTATUS_PRESERVE_FASTFALL));
    ftCommonDamageFallClampRumble(fighter_gobj);
}
