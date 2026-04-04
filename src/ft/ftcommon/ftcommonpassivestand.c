#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144700
void ftCommonPassiveStandSetStatus(GObj *fighter_gobj, s32 status_id)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterGround(fp);
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftParamVelDamageTransferGround(fp);
}

// 0x80144760
sb32 ftCommonPassiveStandCheckInterruptDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (fp->tics_since_last_z < FTCOMMON_PASSIVE_BUFFER_TICS_MAX)
    {
        if (ABS(fp->input.pl.stick_range.x) >= FTCOMMON_PASSIVE_F_OR_B_RANGE)
        {
            if ((fp->input.pl.stick_range.x * fp->lr) >= 0)
            {
                status_id = nFTCommonStatusPassiveStandF;
            }
            else status_id = nFTCommonStatusPassiveStandB;

            ftCommonPassiveStandSetStatus(fighter_gobj, status_id);

            return TRUE;
        }
    }
    return FALSE;
}