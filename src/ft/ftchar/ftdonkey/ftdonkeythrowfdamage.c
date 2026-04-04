#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// DK enters this state when hit during Cargo Throw

// 0x8014E050
void ftDonkeyThrowFDamageProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.common.throwfdamage.hitstun_tics -= 1.0F;

    if (fp->status_vars.common.throwfdamage.hitstun_tics <= 0.0F)
    {
        if (fp->ga == nMPKineticsAir)
        {
            fp->public_knockback = fp->status_vars.common.throwfdamage.public_knockback;

            ftDonkeyThrowFFallSetStatus(fighter_gobj);
        }
        else ftDonkeyThrowFWaitSetStatus(fighter_gobj);
    }
}

// 0x8014E0E0
void ftDonkeyThrowFDamageSetStatus(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    f32 knockback_resist = (this_fp->knockback_resist_status < this_fp->knockback_resist_passive) ? this_fp->knockback_resist_passive : this_fp->knockback_resist_status;

    this_fp->damage_knockback -= knockback_resist;

    if (this_fp->damage_knockback <= 0.0F)
    {
        this_fp->damage_knockback = 0.0F;
    }
    ftCommonDamageInitDamageVars(fighter_gobj, nFTDonkeyStatusThrowFDamage, this_fp->damage_queue, this_fp->damage_knockback, this_fp->damage_angle, this_fp->damage_lr, this_fp->damage_index, this_fp->damage_element, this_fp->damage_player_num, FALSE, FALSE, TRUE);

    if (this_fp->catch_gobj != NULL)
    {
        GObj *catch_gobj = this_fp->catch_gobj;
        FTStruct *capture_fp = ftGetStruct(catch_gobj);

        ftCommonThrownProcPhysics(catch_gobj);
        ftParamsUpdateFighterPartsTransformAll(capture_fp->joints[nFTPartsJointTopN]);
    }
}
