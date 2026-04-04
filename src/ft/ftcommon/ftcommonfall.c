#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013F9A0
void ftCommonFallProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonSpecialAirCheckInterruptCommon(fighter_gobj) == FALSE) && (ftCommonAttackAirCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonJumpAerialCheckInterruptCommon(fighter_gobj);
    }
}

// 0x8013F9E0
void ftCommonFallSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftHammerCheckHoldHammer(fighter_gobj) != FALSE)
    {
        ftCommonHammerFallSetStatus(fighter_gobj);
    }
    else
    {
        if (fp->ga == nMPKineticsGround)
        {
            mpCommonSetFighterAir(fp);
        }
        ftMainSetStatus(fighter_gobj, ((fp->jumps_used >= fp->attr->jumps_max) ? nFTCommonStatusFallAerial : nFTCommonStatusFall), 0.0F, 1.0F, FTSTATUS_PRESERVE_FASTFALL);

        ftPhysicsClampAirVelXMax(fp);

        fp->is_special_interrupt = TRUE;
    }
}
