#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188A50
void (*dFTKirbySpecialAirNStatusList[/* */])(GObj*) = 
{ 
    ftKirbyCopyMarioSpecialAirNSetStatus, // Mario
    ftKirbyCopyFoxSpecialAirNSetStatus, // Fox
    ftKirbyCopyDonkeySpecialAirNStartSetStatus, // Donkey Kong
    ftKirbyCopySamusSpecialAirNStartSetStatus, // Samus
    ftKirbyCopyMarioSpecialAirNSetStatus, // Luigi
    ftKirbyCopyLinkSpecialAirNSetStatus, // Link
    ftKirbyCopyYoshiSpecialAirNSetStatus, // Yoshi
    ftKirbyCopyCaptainSpecialAirNSetStatus, // Captain Faclon
    ftKirbySpecialAirNStartSetStatus, // Kirby
    ftKirbyCopyPikachuSpecialAirNSetStatus, // Pikachu
    ftKirbyCopyPurinSpecialAirNSetStatus, // Jigglypuff
    ftKirbyCopyNessSpecialAirNSetStatus, // Ness
    ftKirbySpecialAirNStartSetStatus, // Master Hand
    ftKirbyCopyMarioSpecialAirNSetStatus, // Metal Mario
    ftKirbySpecialAirNStartSetStatus, // Poly Mario
    ftKirbySpecialAirNStartSetStatus, // Poly Fox
    ftKirbySpecialAirNStartSetStatus, // Poly Donkey Kong
    ftKirbySpecialAirNStartSetStatus, // Poly Samus
    ftKirbySpecialAirNStartSetStatus, // Poly Luigi
    ftKirbySpecialAirNStartSetStatus, // Poly Link
    ftKirbySpecialAirNStartSetStatus, // Poly Yoshi
    ftKirbySpecialAirNStartSetStatus, // Poly Captain Falcon
    ftKirbySpecialAirNStartSetStatus, // Poly Kirby
    ftKirbySpecialAirNStartSetStatus, // Poly Pikachu
    ftKirbySpecialAirNStartSetStatus, // Poly Jigglypuff
    ftKirbySpecialAirNStartSetStatus, // Poly Ness
    ftKirbySpecialAirNStartSetStatus, // Giant Donkey Kong (This is actually inaccessible, Kirby's copy ID for Giant DK is always 2)
};

// 0x80188ABC
void (*dFTCommonSpecialAirNStatusList[/* */])(GObj*) =
{
    ftMarioSpecialAirNSetStatus,
    ftFoxSpecialAirNSetStatus,
    ftDonkeySpecialAirNStartSetStatus,
    ftSamusSpecialAirNStartSetStatus,
    ftMarioSpecialAirNSetStatus,
    ftLinkSpecialAirNSetStatus,
    ftYoshiSpecialAirNSetStatus,
    ftCaptainSpecialAirNSetStatus,
    ftKirbySpecialAirNSetStatusSelect,
    ftPikachuSpecialAirNSetStatus,
    ftPurinSpecialAirNSetStatus,
    ftNessSpecialAirNSetStatus,
    ftMarioSpecialAirNSetStatus,
    ftMarioSpecialAirNSetStatus,
    ftMarioSpecialAirNSetStatus,
    ftFoxSpecialAirNSetStatus,
    ftDonkeySpecialAirNStartSetStatus,
    ftSamusSpecialAirNStartSetStatus,
    ftMarioSpecialAirNSetStatus,
    ftLinkSpecialAirNSetStatus,
    ftMarioSpecialAirNSetStatus,            // more bro momento
    ftCaptainSpecialAirNSetStatus,
    ftKirbySpecialAirNSetStatusSelect,
    ftPikachuSpecialAirNSetStatus,
    ftPurinSpecialAirNSetStatus,
    ftNessSpecialAirNSetStatus,
    ftDonkeySpecialAirNStartSetStatus
};

// 0x80188B28
void (*dFTCommonSpecialAirHiStatusList[/* */])(GObj*) =
{
    ftMarioSpecialAirHiSetStatus,
    ftFoxSpecialAirHiStartSetStatus,
    ftDonkeySpecialAirHiSetStatus,
    ftSamusSpecialAirHiSetStatus,
    ftMarioSpecialAirHiSetStatus,
    ftLinkSpecialAirHiSetStatus,
    ftYoshiSpecialAirHiSetStatus,
    ftCaptainSpecialAirHiSetStatus,
    ftKirbySpecialAirHiSetStatus,
    ftPikachuSpecialAirHiStartSetStatus,
    ftPurinSpecialAirHiSetStatus,
    ftNessSpecialAirHiStartSetStatus,
    ftMarioSpecialAirNSetStatus,
    ftMarioSpecialAirHiSetStatus,
    ftMarioSpecialAirHiSetStatus,
    ftFoxSpecialAirHiStartSetStatus,
    ftDonkeySpecialAirHiSetStatus,
    ftSamusSpecialAirHiSetStatus,
    ftMarioSpecialAirHiSetStatus,
    ftLinkSpecialAirHiSetStatus,
    ftYoshiSpecialAirHiSetStatus,
    ftCaptainSpecialAirHiSetStatus,
    ftKirbySpecialAirHiSetStatus,
    ftPikachuSpecialAirHiStartSetStatus,
    ftPurinSpecialAirHiSetStatus,
    ftNessSpecialAirHiStartSetStatus,
    ftDonkeySpecialAirHiSetStatus
};

// 0x80188B94
void (*dFTCommonSpecialAirLwStatusList[/* */])(GObj*) =
{
    ftMarioSpecialAirLwSetStatus,
    ftFoxSpecialAirLwStartSetStatus,
    NULL,
    ftSamusSpecialAirLwSetStatus,
    ftMarioSpecialAirLwSetStatus,
    ftLinkSpecialAirLwSetStatus,
    ftYoshiSpecialAirLwStartSetStatus,
    ftCaptainSpecialAirLwSetStatus,
    ftKirbySpecialAirLwStartSetStatus,
    ftPikachuSpecialAirLwStartSetStatus,
    ftPurinSpecialAirLwSetStatus,
    ftNessSpecialAirLwStartSetStatus,
    ftMarioSpecialAirLwSetStatus,
    ftMarioSpecialAirLwSetStatus,
    ftMarioSpecialAirLwSetStatus,
    ftFoxSpecialAirLwStartSetStatus,
    NULL,
    ftSamusSpecialAirLwSetStatus,
    ftMarioSpecialAirLwSetStatus,
    ftLinkSpecialAirLwSetStatus,
    ftYoshiSpecialAirLwStartSetStatus,
    ftCaptainSpecialAirLwSetStatus,
    ftKirbySpecialAirLwStartSetStatus,
    ftPikachuSpecialAirLwStartSetStatus,
    ftPurinSpecialAirLwSetStatus,
    ftNessSpecialAirLwStartSetStatus,
    NULL
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80150ED0
void ftKirbySpecialAirNSetStatusSelect(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    dFTKirbySpecialAirNStatusList[fp->passive_vars.kirby.copy_id](fighter_gobj);
}

// 0x80150F08
sb32 ftCommonSpecialAirCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->input.pl.button_tap & fp->input.button_mask_b)
    {
        if (ftHammerCheckHoldHammer(fighter_gobj) == FALSE)
        {
            if (fp->input.pl.stick_range.y >= FTCOMMON_SPECIALHI_STICK_RANGE_MIN)
            {
                if (attr->is_have_specialairhi)
                {
                    dFTCommonSpecialAirHiStatusList[fp->fkind](fighter_gobj);

                    return TRUE;
                }
            }
            else if (fp->input.pl.stick_range.y <= FTCOMMON_SPECIALLW_STICK_RANGE_MIN)
            {
                if (attr->is_have_specialairlw)
                {
                    dFTCommonSpecialAirLwStatusList[fp->fkind](fighter_gobj);

                    return TRUE;
                }
            }
            else if (attr->is_have_specialairn)
            {
                if ((fp->input.pl.stick_range.x * fp->lr) < FTCOMMON_SPECIALN_TURN_STICK_RANGE_MIN)
                {
                    ftParamSetStickLR(fp);
                }
                dFTCommonSpecialAirNStatusList[fp->fkind](fighter_gobj);

                return TRUE;
            }
        }
    }
    return FALSE;
}
