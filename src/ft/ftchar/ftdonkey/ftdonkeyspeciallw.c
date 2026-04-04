#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015B9F0
void ftDonkeySpecialLwStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftDonkeySpecialLwLoopSetStatus);
}

// 0x8015BA14
void ftDonkeySpecialLwLoopProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fighter_gobj->anim_frame >= 0.0F) && (fighter_gobj->anim_frame < DObjGetStruct(fighter_gobj)->anim_speed))
    {
        if (fp->status_vars.donkey.speciallw.is_loop != FALSE)
        {
            fp->status_vars.donkey.speciallw.is_loop = FALSE;
        }
        else ftDonkeySpecialLwEndSetStatus(fighter_gobj);
    }
}

// 0x8015BA7C
void ftDonkeySpecialLwLoopProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->input.pl.button_tap & fp->input.button_mask_b)
    {
        fp->status_vars.donkey.speciallw.is_loop = TRUE;
    }
}

// 0x8015BAA4
void ftDonkeySpecialLwLoopSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusSpecialLwLoop, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8015BAD0
void ftDonkeySpecialLwEndSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusSpecialLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8015BAFC
void ftDonkeySpecialLwStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTDonkeyStatusSpecialLwStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->status_vars.donkey.speciallw.is_loop = FALSE;
}
