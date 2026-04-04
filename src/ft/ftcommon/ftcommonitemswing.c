#include <ft/fighter.h>
#include <it/item.h>

extern alSoundEffect* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80146C40
void ftCommonHarisenSwingProcHit(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(fp->item_gobj);

        if (ip->kind == nITKindHarisen)
        {
            itHarisenCommonSetScale(fp->item_gobj, FTCOMMON_HARISENSWING_SCALE_HIT);

            fp->status_vars.common.itemswing.harisen_scale_reset_wait = FTCOMMON_HARISENSWING_SCALE_RESET_WAIT;
        }
    }
}

// 0x80146C98
void ftCommonHarisenSwingProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->item_gobj != NULL)
    {
        if (fp->status_vars.common.itemswing.harisen_scale_reset_wait != 0)
        {
            fp->status_vars.common.itemswing.harisen_scale_reset_wait--;

            if (fp->status_vars.common.itemswing.harisen_scale_reset_wait == 0)
            {
                itHarisenCommonSetScale(fp->item_gobj, 1.0F);
            }
        }
    }
    ftAnimEndSetWait(fighter_gobj);
}

// 0x80146CF4
void ftCommonStarRodSwingProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    ITStruct *ip;
    f32 scale_mul;
    s32 unused;

    if (fp->item_gobj != NULL)
    {
        if (fp->motion_vars.flags.flag1 != 0)
        {
            ip = itGetStruct(fp->item_gobj);

            if (ip->multi != 0)
            {
                func_800269C0_275C0(fp->motion_vars.flags.flag1);
            }
            else func_800269C0_275C0(nSYAudioFGMStarRodEmpty);

            fp->motion_vars.flags.flag1 = 0;
        }
    }
    if ((fp->item_gobj != NULL) && (fp->motion_vars.flags.flag0 != 0))
    {
        ip = itGetStruct(fp->item_gobj);

        if (ip->multi != 0)
        {
            // 0x80188600
            Vec3f make_star_offset = { 0.0F, 200.0F, 0.0F };

            scale_mul = 1.0F / fp->attr->size;

            make_star_offset.x *= scale_mul;
            make_star_offset.y *= scale_mul;
            make_star_offset.z *= scale_mul;

            gmCollisionGetFighterPartsWorldPosition(fp->joints[fp->attr->joint_itemlight_id], &make_star_offset);

            itStarRodMakeStar(fighter_gobj, &make_star_offset, (fp->motion_vars.flags.flag0 == 1) ? FALSE : TRUE);
        }
        else
        {
            // 0x8018860C
            Vec3f effect_offset = { 0.0F, 200.0F, 0.0F };

            ftParamMakeEffect(fighter_gobj, nEFKindDustLight, fp->attr->joint_itemlight_id, &effect_offset, NULL, -fp->lr, TRUE, FALSE);
        }
        fp->motion_vars.flags.flag0 = 0;
    }
    ftAnimEndSetWait(fighter_gobj);
}

// 0x80188618
s32 dFTCommonItemSwingStatusIDs[/* */][nFTItemSwingTypeEnumCount] =
{
    { nFTCommonStatusSwordSwing1,   nFTCommonStatusSwordSwing3,   nFTCommonStatusSwordSwing4,   nFTCommonStatusSwordSwingDash   },
    { nFTCommonStatusBatSwing1,     nFTCommonStatusBatSwing3,     nFTCommonStatusBatSwing4,     nFTCommonStatusBatSwingDash     },
    { nFTCommonStatusHarisenSwing1, nFTCommonStatusHarisenSwing3, nFTCommonStatusHarisenSwing4, nFTCommonStatusHarisenSwingDash },
    { nFTCommonStatusStarRodSwing1, nFTCommonStatusStarRodSwing3, nFTCommonStatusStarRodSwing4, nFTCommonStatusStarRodSwingDash }
};

// 0x80146E94
void ftCommonItemSwingSetStatus(GObj *fighter_gobj, s32 swing_type)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    ITStruct *ip = itGetStruct(fp->item_gobj);
    s32 swing_item;
    s32 status_id;
    f32 anim_speed;

    switch (ip->kind)
    {
    case nITKindSword:
        swing_item = nFTItemSwingKindSword;
        break;

    case nITKindBat:
        swing_item = nFTItemSwingKindBat;
        break;

    case nITKindHarisen:
        swing_item = nFTItemSwingKindHarisen;
        break;

    case nITKindStarRod:
        swing_item = nFTItemSwingKindStarRod;
        break;
    }
    status_id = dFTCommonItemSwingStatusIDs[swing_item][swing_type];
    anim_speed = F_PCT_TO_DEC(dFTCommonDataItemSwingAnimSpeeds[status_id - nFTCommonStatusItemSwingStart].anim_speed);

    fp->motion_vars.flags.flag0 = 0;

    ftMainSetStatus(fighter_gobj, status_id, 0.0F, anim_speed, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    fp->proc_hit = ftCommonHarisenSwingProcHit;

    fp->status_vars.common.itemswing.harisen_scale_reset_wait = 0;

    fp->motion_vars.flags.flag1 = 0;
}
