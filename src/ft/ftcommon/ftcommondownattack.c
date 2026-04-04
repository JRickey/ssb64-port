#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144910
void ftCommonDownAttackSetStatus(GObj *fighter_gobj, s32 status_id)
{
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80144944
sb32 ftCommonDownAttackCheckInterruptDownBounce(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (fp->status_vars.common.downbounce.attack_buffer != 0)
    {
        if (fp->status_id == nFTCommonStatusDownBounceD)
        {
            status_id = nFTCommonStatusDownAttackD;
        }
        else status_id = nFTCommonStatusDownAttackU;

        ftCommonDownAttackSetStatus(fighter_gobj, status_id);

        return TRUE;
    }
    else return FALSE;
}

// 0x8014499C
sb32 ftCommonDownAttackCheckInterruptDownWait(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (fp->input.pl.button_tap & (fp->input.button_mask_a | fp->input.button_mask_b))
    {
        if (fp->status_id == nFTCommonStatusDownWaitD)
        {
            status_id = nFTCommonStatusDownAttackD;
        }
        else status_id = nFTCommonStatusDownAttackU;

        ftCommonDownAttackSetStatus(fighter_gobj, status_id);

        return TRUE;
    }
    else return FALSE;
}
