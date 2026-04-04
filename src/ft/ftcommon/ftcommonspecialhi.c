#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188CE0
void (*dFTCommonSpecialHiStatusList[/* */])(GObj*) =
{
    ftMarioSpecialHiSetStatus,
    ftFoxSpecialHiStartSetStatus,
    ftDonkeySpecialHiSetStatus,
    ftSamusSpecialHiSetStatus,
    ftMarioSpecialHiSetStatus,
    ftLinkSpecialHiSetStatus,
    ftYoshiSpecialHiSetStatus,
    ftCaptainSpecialHiSetStatus,
    ftKirbySpecialHiSetStatus,
    ftPikachuSpecialHiStartSetStatus,
    ftPurinSpecialHiSetStatus,
    ftNessSpecialHiStartSetStatus,
    ftMarioSpecialHiSetStatus,
    ftMarioSpecialHiSetStatus,
    ftMarioSpecialHiSetStatus,
    ftFoxSpecialHiStartSetStatus,
    ftDonkeySpecialHiSetStatus,
    ftSamusSpecialHiSetStatus,
    ftMarioSpecialHiSetStatus,
    ftLinkSpecialHiSetStatus,
    ftYoshiSpecialHiSetStatus,
    ftCaptainSpecialHiSetStatus,
    ftKirbySpecialHiSetStatus,
    ftPikachuSpecialHiStartSetStatus,
    ftPurinSpecialHiSetStatus,
    ftNessSpecialHiStartSetStatus,
    ftDonkeySpecialHiSetStatus
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80151160
sb32 ftCommonSpecialHiCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if ((fp->input.pl.button_tap & fp->input.button_mask_b) && (attr->is_have_specialhi) && (fp->input.pl.stick_range.y >= FTCOMMON_SPECIALHI_STICK_RANGE_MIN))
    {
        dFTCommonSpecialHiStatusList[fp->fkind](fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
