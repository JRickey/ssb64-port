#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144660
void ftCommonPassiveSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterGround(fp);
    }
    ftMainSetStatus(fighter_gobj, nFTCommonStatusPassive, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftParamVelDamageTransferGround(fp);
}

// 0x801446BC
sb32 ftCommonPassiveCheckInterruptDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->tics_since_last_z < FTCOMMON_PASSIVE_BUFFER_TICS_MAX)
    {
        ftCommonPassiveSetStatus(fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
