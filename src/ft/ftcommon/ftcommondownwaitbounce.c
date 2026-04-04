#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTCOMMON_DOWNBOUNCE_STATUS_FLAGS (FTSTATUS_PRESERVE_PLAYERTAG | FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_SLOPECONTOUR | FTSTATUS_PRESERVE_MODELPART)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80144220
void ftCommonDownWaitProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.common.downwait.stand_wait--;

    if (fp->status_vars.common.downwait.stand_wait == 0)
    {
        ftCommonDownStandSetStatus(fighter_gobj);
    }
}

// 0x80144254
void ftCommonDownWaitProcInterrupt(GObj *fighter_gobj)
{
    if ((ftCommonDownAttackCheckInterruptDownWait(fighter_gobj) == FALSE) && (ftCommonDownForwardOrBackCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonDownStandCheckInterruptCommon(fighter_gobj);
    }
}

// 0x80144294
void ftCommonDownWaitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (fp->status_id == nFTCommonStatusDownBounceD)
    {
        status_id = nFTCommonStatusDownWaitD;
    }
    else status_id = nFTCommonStatusDownWaitU;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTCOMMON_DOWNBOUNCE_STATUS_FLAGS);

    fp->status_vars.common.downwait.stand_wait = FTCOMMON_DOWNWAIT_STAND_WAIT;

    ftParamSetCaptureImmuneMask(fp, (FTCATCHKIND_MASK_CAPTAINSPECIALHI | FTCATCHKIND_MASK_COMMON | FTCATCHKIND_MASK_KIRBYSPECIALN | FTCATCHKIND_MASK_YOSHISPECIALN));

    fp->damage_mul = 0.5F;
}

// 0x80144308
void ftCommonDownBounceProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.downbounce.attack_buffer != 0)
    {
        fp->status_vars.common.downbounce.attack_buffer--;
    }
    if (fp->input.pl.button_tap & (fp->input.button_mask_a | fp->input.button_mask_b))
    {
        fp->status_vars.common.downbounce.attack_buffer = FTCOMMON_DOWNBOUNCE_ATTACK_BUFFER;
    }
    if ((fighter_gobj->anim_frame <= 0.0F) && (ftCommonDownAttackCheckInterruptDownBounce(fighter_gobj) == FALSE) && (ftCommonDownForwardOrBackCheckInterruptCommon(fighter_gobj) == FALSE))
    {
        ftCommonDownWaitSetStatus(fighter_gobj);
    }
}

// 0x80144398
sb32 ftCommonDownBounceCheckUpOrDown(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 rot_x = fp->joints[4]->rotate.vec.f.x;

    rot_x /= F_CST_DTOR32(360.0F);

    rot_x -= (s32)rot_x;

    if ((rot_x < -0.5F) || (rot_x > 0.0F) && (rot_x < 0.5F))
    {
        return 1;
    }
    else return 0;
}

// 0x80144428
void ftCommonDownBounceUpdateEffects(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamMakeEffect(fighter_gobj, nEFKindImpactWave, nFTPartsJointTopN, NULL, NULL, fp->lr, FALSE, FALSE);
    func_800269C0_275C0(dFTCommonDataDownBounceSFX[fp->fkind]);
    ftParamMakeRumble(fp, 4, 0);
}

// 0x80144498
void ftCommonDownBounceSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterGround(fp);
    }
    if (ftCommonDownBounceCheckUpOrDown(fighter_gobj) != 0)
    {
        status_id = nFTCommonStatusDownBounceD;
    }
    else status_id = nFTCommonStatusDownBounceU;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_PLAYERTAG);
    ftCommonDownBounceUpdateEffects(fighter_gobj);

    fp->status_vars.common.downbounce.attack_buffer = 0;
    fp->damage_mul = 0.5F;

    ftParamVelDamageTransferGround(fp);
}
