#include <ft/fighter.h>
#ifdef PORT
extern void port_log(const char *fmt, ...);
#endif

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188D50
void (*dFTCommonSpecialLwStatusList[/* */])(GObj*) = 
{
    ftMarioSpecialLwSetStatus,
    ftFoxSpecialLwStartSetStatus,
    ftDonkeySpecialLwStartSetStatus,
    ftSamusSpecialLwSetStatus,
    ftMarioSpecialLwSetStatus,
    ftLinkSpecialLwSetStatus,
    ftYoshiSpecialLwStartSetStatus,
    ftCaptainSpecialLwSetStatus,
    ftKirbySpecialLwStartSetStatus,
    ftPikachuSpecialLwStartSetStatus,
    ftPurinSpecialLwSetStatus,
    ftNessSpecialLwStartSetStatus,
    ftMarioSpecialLwSetStatus,
    ftMarioSpecialLwSetStatus,
    ftMarioSpecialLwSetStatus,
    ftFoxSpecialLwStartSetStatus,
    ftDonkeySpecialLwStartSetStatus,
    ftSamusSpecialLwSetStatus,
    ftMarioSpecialLwSetStatus,
    ftLinkSpecialLwSetStatus,
    ftYoshiSpecialLwStartSetStatus,
    ftCaptainSpecialLwSetStatus,
    ftKirbySpecialLwStartSetStatus,
    ftPikachuSpecialLwStartSetStatus,
    ftPurinSpecialLwSetStatus,
    ftNessSpecialLwStartSetStatus,
    ftDonkeySpecialLwStartSetStatus
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801511E0
sb32 ftCommonSpecialLwCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

#ifdef PORT
    if (fp->input.pl.button_tap & fp->input.button_mask_b)
    {
        port_log("SSB64: SpecialLwCheck fkind=%d B_tap=0x%04X have_lw=%d stick_y=%d (need<=%d)\n",
            fp->fkind, fp->input.pl.button_tap & fp->input.button_mask_b,
            attr->is_have_speciallw, fp->input.pl.stick_range.y,
            FTCOMMON_SPECIALLW_STICK_RANGE_MIN);
    }
#endif
    if ((fp->input.pl.button_tap & fp->input.button_mask_b) && (attr->is_have_speciallw) && (fp->input.pl.stick_range.y <= FTCOMMON_SPECIALLW_STICK_RANGE_MIN))
    {
#ifdef PORT
        port_log("SSB64: SpecialLwCheck -> PASS, calling down-B for fkind=%d\n", fp->fkind);
#endif
        dFTCommonSpecialLwStatusList[fp->fkind](fighter_gobj);

        return TRUE;
    }
    else return FALSE;
}
