#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftCommonGuardCheckInterrupt(fighter_gobj)                           \
(                                                                           \
    (ftCommonLightThrowCheckInterruptGuardOn(fighter_gobj) != FALSE)    ||  \
    (ftCommonEscapeCheckInterruptGuard(fighter_gobj) != FALSE)          ||  \
    (ftCommonCatchCheckInterruptGuard(fighter_gobj) != FALSE)           ||  \
    (ftCommonGuardKneeBendCheckInterruptGuard(fighter_gobj) != FALSE)   ||  \
    (ftCommonGuardPassCheckInterruptGuard(fighter_gobj) != FALSE)           \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80148120
void ftCommonGuardCheckScheduleRelease(FTStruct *fp)
{
    if (!(fp->input.pl.button_hold & fp->input.button_mask_z))
    {
        fp->status_vars.common.guard.is_release = TRUE;
    }
}

// 0x80148144
void ftCommonGuardOnSetHitStatusYoshi(GObj *fighter_gobj) // Set all of Yoshi's hurtbox collision states to invincible (GuardOn)
{
    ftParamSetHitStatusPartID(fighter_gobj, 5,  nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 6,  nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 7,  nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 15, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 11, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 16, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 12, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 27, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 22, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 28, nGMHitStatusInvincible);
    ftParamSetHitStatusPartID(fighter_gobj, 23, nGMHitStatusInvincible);
}

// 0x80148214
void ftCommonGuardSetHitStatusYoshi(GObj *fighter_gobj) // Set all of Yoshi's hurtbox collision states to intangible (Guard)
{
    ftParamSetHitStatusPartID(fighter_gobj, 5,  nGMHitStatusNormal);
    ftParamSetHitStatusPartID(fighter_gobj, 6,  nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 7,  nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 15, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 11, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 16, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 12, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 27, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 22, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 28, nGMHitStatusIntangible);
    ftParamSetHitStatusPartID(fighter_gobj, 23, nGMHitStatusIntangible);
}

// 0x801482E4
void ftCommonGuardOffSetHitStatusYoshi(GObj *fighter_gobj)
{
    ftParamSetHitStatusPartAll(fighter_gobj, nGMHitStatusNormal);
}

// 0x80148304
void ftCommonGuardUpdateShieldVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->is_shield)
    {
        if (fp->status_vars.common.guard.shield_decay_wait != 0)
        {
            fp->status_vars.common.guard.shield_decay_wait--;

            if (fp->status_vars.common.guard.shield_decay_wait == 0)
            {
                fp->shield_health--;

                if (fp->shield_health != 0)
                {
                    fp->status_vars.common.guard.shield_decay_wait = FTCOMMON_GUARD_DECAY_INT;

                    goto lag_decrement;
                }
                else goto lag_end;
            }
        }
    lag_decrement:
        if (fp->status_vars.common.guard.release_lag != 0)
        {
            fp->status_vars.common.guard.release_lag--;
        }
        if ((fp->status_vars.common.guard.release_lag == 0) && (fp->status_vars.common.guard.is_release != FALSE))
        {
        lag_end:
            if (fp->fkind == nFTKindYoshi)
            {
                ftParamResetModelPartAll(fighter_gobj);
                ftCommonGuardOffSetHitStatusYoshi(fighter_gobj);

                if (fp->is_effect_attach)
                {
                    // 0x801886C0
                    Vec3f egg_effect_offset = { 0.0F, 0.0F, 0.0F };

                    gmCollisionGetFighterPartsWorldPosition(fp->joints[nFTPartsJointYRotN], &egg_effect_offset);
                    efManagerEggBreakMakeEffect(&egg_effect_offset);
                }
            }
            ftParamProcStopEffect(fighter_gobj);

            fp->is_shield = FALSE;
        }
    }
}

// 0x80148408
void ftCommonGuardUpdateShieldCollision(FTStruct *fp)
{
    Vec3f *scale = &fp->joints[nFTPartsJointYRotN]->scale.vec.f;
    f32 scale_final;
    f32 scale_mul;

    if (fp->fkind == nFTKindYoshi)
    {
        scale_mul = 1.0F;
    }
    else scale_mul = fp->shield_health / FTCOMMON_GUARD_SIZE_HEALTH_DIV;

    scale_final =
    (
        (
            (FTCOMMON_GUARD_SIZE_SCALE_MUL_INIT * scale_mul) +
            FTCOMMON_GUARD_SIZE_SCALE_MUL_ADD
        ) * fp->attr->shield_size
    ) / FTCOMMON_GUARD_SIZE_SCALE_MUL_DIV;

    scale->x = scale->y = scale->z = scale_final;
}

// 0x80148488
void ftCommonGuardUpdateShieldAngle(FTStruct *fp)
{
    f32 angle_r = syUtilsArcTan2(fp->input.pl.stick_range.y, fp->input.pl.stick_range.x * fp->lr);
    f32 angle_d;
    f32 range;

    if (angle_r < 0.0F)
    {
        angle_r += F_CST_DTOR32(360.0F);
    }
    angle_d = F_CLC_RTOD32(angle_r);

    if (angle_d < 0.0F)
    {
        angle_d = 0.0F;
    }
    if (angle_d > FTCOMMON_GUARD_ANGLE_MAX)
    {
        angle_d = FTCOMMON_GUARD_ANGLE_MAX;
    }
    fp->status_vars.common.guard.angle_i = (angle_d / 45.0F);
    fp->status_vars.common.guard.angle_f = (angle_d - (fp->status_vars.common.guard.angle_i * 45.0F));

    range = sqrtf(SQUARE(fp->input.pl.stick_range.x) + SQUARE(fp->input.pl.stick_range.y)) / F_CONTROLLER_RANGE_MAX;

    if (range > 1.0F)
    {
        range = 1.0F;
    }
    fp->status_vars.common.guard.shield_rotate_range = range;
}

// WARNING: Actually DObjDesc*?
void ftCommonGuardGetJointTransform(DObj *joint, DObjDesc *dobjdesc, f32 range)
{
    joint->rotate.vec.f.x = ((joint->rotate.vec.f.x - dobjdesc->rotate.x) * range) + dobjdesc->rotate.x;
    joint->rotate.vec.f.y = ((joint->rotate.vec.f.y - dobjdesc->rotate.y) * range) + dobjdesc->rotate.y;
    joint->rotate.vec.f.z = ((joint->rotate.vec.f.z - dobjdesc->rotate.z) * range) + dobjdesc->rotate.z;

    joint->translate.vec.f.x = ((joint->translate.vec.f.x - dobjdesc->translate.x) * range) + dobjdesc->translate.x;
    joint->translate.vec.f.y = ((joint->translate.vec.f.y - dobjdesc->translate.y) * range) + dobjdesc->translate.y;
    joint->translate.vec.f.z = ((joint->translate.vec.f.z - dobjdesc->translate.z) * range) + dobjdesc->translate.z;
}

// WARNING: Actually DObjDesc*?
void ftCommonGuardGetJointTransformScale(DObj *joint, DObjDesc *dobjdesc, f32 range, Vec3f *scale)
{
    f32 scale_translate;

    joint->rotate.vec.f.x = ((joint->rotate.vec.f.x - dobjdesc->rotate.x) * range) + dobjdesc->rotate.x;
    joint->rotate.vec.f.y = ((joint->rotate.vec.f.y - dobjdesc->rotate.y) * range) + dobjdesc->rotate.y;
    joint->rotate.vec.f.z = ((joint->rotate.vec.f.z - dobjdesc->rotate.z) * range) + dobjdesc->rotate.z;

    // y tho

    scale_translate = dobjdesc->translate.x * scale->x;
    joint->translate.vec.f.x = ((joint->translate.vec.f.x - scale_translate) * range) + scale_translate;

    scale_translate = dobjdesc->translate.y * scale->y;
    joint->translate.vec.f.y = ((joint->translate.vec.f.y - scale_translate) * range) + scale_translate;

    scale_translate = dobjdesc->translate.z * scale->z;
    joint->translate.vec.f.z = ((joint->translate.vec.f.z - scale_translate) * range) + scale_translate;
}

// Return to this when the struct at 0x2D8 of FTAttributes is mapped
void ftCommonGuardUpdateJoints(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 unused2;
    DObj *yrotn_joint = fp->joints[nFTPartsJointYRotN];
    DObj **p_joint = &fp->joints[nFTPartsJointXRotN];
    s32 joint_num;
    s32 i;
    Vec3f *scale = &fp->attr->translate_scales[nFTPartsJointYRotN];

    if (fp->is_shield)
    {
        ftCommonGuardUpdateShieldAngle(fp);

        for (i = nFTPartsJointXRotN, joint_num = 0; i < ARRAY_COUNT(fp->joints); i++, p_joint++)
        {
            if (*p_joint != NULL)
            {
                joint_num++;
            }
        }
        joint_num--;

        gcAddDObjAnimJoint(yrotn_joint, fp->attr->shield_anim_joints[fp->status_vars.common.guard.angle_i][joint_num], fp->status_vars.common.guard.angle_f);
        gcParseDObjAnimJoint(yrotn_joint);

        if (fp->is_have_translate_scale)
        {
            lbCommonPlayTranslateScaledDObjAnim(yrotn_joint, scale);
        }
        else gcPlayDObjAnimJoint(yrotn_joint);

        if (fp->is_have_translate_scale)
        {
            ftCommonGuardGetJointTransformScale(yrotn_joint, &fp->attr->dobj_lookup[joint_num], fp->status_vars.common.guard.shield_rotate_range, scale);
        }
        else ftCommonGuardGetJointTransform(yrotn_joint, &fp->attr->dobj_lookup[joint_num], fp->status_vars.common.guard.shield_rotate_range);

        yrotn_joint->anim_wait = AOBJ_ANIM_NULL;

        ftCommonGuardUpdateShieldCollision(fp);
        ftParamsUpdateFighterPartsTransformAll(fp->joints[nFTPartsJointYRotN]);
    }
}

// Need to revisit this when once I have a better understanding of these structs...
void ftCommonGuardInitJoints(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    DObj **p_joint = &fp->joints[nFTPartsJointCommonStart];
    DObjDesc *dobjdesc = &attr->dobj_lookup[1];
    DObj *joint;
    Vec3f *scale;
    s32 i;

    if (fp->status_id != nFTCommonStatusGuardSetOff)
    {
        ftCommonGuardUpdateShieldAngle(fp);
    }
    fp->anim_desc.flags.is_anim_joint = TRUE;

    lbCommonAddDObjAnimJointAll
    (
        fp->joints[nFTPartsJointXRotN],
        attr->shield_anim_joints[fp->status_vars.common.guard.angle_i],
        fp->status_vars.common.guard.angle_f
    );
    ftMainPlayAnimEventsAll(fighter_gobj);

    if (fp->is_have_translate_scale)
    {
        scale = &fp->attr->translate_scales[nFTPartsJointCommonStart];

        for (i = nFTPartsJointCommonStart; i < ARRAY_COUNT(fp->joints); i++, p_joint++, scale++)
        {
            joint = *p_joint;

            if (joint != NULL)
            {
                if (joint->anim_wait != AOBJ_ANIM_NULL)
                {
                    ftCommonGuardGetJointTransformScale
                    (
                        joint,
                        dobjdesc,
                        fp->status_vars.common.guard.shield_rotate_range,
                        scale
                    );
                    joint->anim_wait = AOBJ_ANIM_NULL;
                }
                dobjdesc++;
            }
        }
        joint = fp->joints[nFTPartsJointYRotN];

        if (joint->anim_wait != AOBJ_ANIM_NULL)
        {
            ftCommonGuardGetJointTransformScale
            (
                joint,
                dobjdesc,
                fp->status_vars.common.guard.shield_rotate_range,
                &fp->attr->translate_scales[nFTPartsJointYRotN]
            );
            joint->anim_wait = AOBJ_ANIM_NULL;
        }
    }
    else
    {
        for (i = nFTPartsJointCommonStart; i < ARRAY_COUNT(fp->joints); i++, p_joint++)
        {
            joint = *p_joint;

            if (joint != NULL)
            {
                if (joint->anim_wait != AOBJ_ANIM_NULL)
                {
                    ftCommonGuardGetJointTransform
                    (
                        joint,
                        dobjdesc,
                        fp->status_vars.common.guard.shield_rotate_range
                    );
                    joint->anim_wait = AOBJ_ANIM_NULL;
                }
                dobjdesc++;
            }
        }
        joint = fp->joints[nFTPartsJointYRotN];

        if (joint->anim_wait != AOBJ_ANIM_NULL)
        {
            ftCommonGuardGetJointTransform
            (
                joint,
                dobjdesc,
                fp->status_vars.common.guard.shield_rotate_range
            );
            joint->anim_wait = AOBJ_ANIM_NULL;
        }
    }
    ftCommonGuardUpdateShieldCollision(fp);
    ftParamsUpdateFighterPartsTransform(fp->joints[nFTPartsJointXRotN]);
}

// 0x80148A88
void ftCommonGuardOnProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonGuardCheckScheduleRelease(fp);
    ftCommonGuardUpdateShieldVars(fighter_gobj);

    if (fp->shield_health == 0)
    {
        ftCommonShieldBreakFlyCommonSetStatus(fighter_gobj);
    }
    else
    {
        if (fighter_gobj->anim_frame <= 0.0F)
        {
            if (fp->status_vars.common.guard.is_release != FALSE)
            {
                if (fp->fkind == nFTKindYoshi)
                {
                    ftCommonGuardOffSetHitStatusYoshi(fighter_gobj);
                }
                ftCommonGuardOffSetStatus(fighter_gobj);
            }
            else
            {
                if (fp->fkind == nFTKindYoshi)
                {
                    fp->status_vars.common.guard.effect_gobj = efManagerYoshiShieldMakeEffect(fighter_gobj);

                    ftParamHideModelPartAll(fighter_gobj);
                    ftCommonGuardSetHitStatusYoshi(fighter_gobj);

                    fp->is_shield = TRUE;

                    ftCommonGuardUpdateJoints(fighter_gobj);
                }
                ftCommonGuardSetStatus(fighter_gobj);
            }
        }
        else ftCommonGuardUpdateJoints(fighter_gobj);
    }
}

// 0x80148B84
void ftCommonGuardCommonProcInterrupt(GObj *fighter_gobj)
{
    if (!(ftCommonGuardCheckInterrupt(fighter_gobj)))
    {
        ftCommonDokanStartCheckInterruptCommon(fighter_gobj);
    }
}

// 0x80148BFC
void ftCommonGuardOnSetStatus(GObj *fighter_gobj, s32 slide_tics)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusGuardOn, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    if (fp->shield_health != 0)
    {
        if (fp->fkind == nFTKindYoshi)
        {
            ftCommonGuardOnSetHitStatusYoshi(fighter_gobj);
        }
        else
        {
            fp->status_vars.common.guard.effect_gobj = efManagerShieldMakeEffect(fighter_gobj);
            fp->is_shield = TRUE;
        }
    }
    ftCommonGuardUpdateJoints(fighter_gobj);

    fp->status_vars.common.guard.release_lag = FTCOMMON_GUARD_RELEASE_LAG;
    fp->status_vars.common.guard.shield_decay_wait = FTCOMMON_GUARD_DECAY_INT;
    fp->status_vars.common.guard.is_release = FALSE;
    fp->status_vars.common.guard.slide_tics = slide_tics;
    fp->status_vars.common.guard.is_setoff = FALSE;

    func_800269C0_275C0(nSYAudioFGMGuardOn);
}

// 0x80148CBC
sb32 ftCommonGuardOnCheckInterruptSuccess(GObj *fighter_gobj, s32 slide_tics)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->input.pl.button_hold & fp->input.button_mask_z) && (fp->shield_health != 0))
    {
        ftCommonGuardOnSetStatus(fighter_gobj, slide_tics);

        return TRUE;
    }
    else return FALSE;
}

// 0x80148D0C
sb32 ftCommonGuardOnCheckInterruptCommon(GObj *fighter_gobj)
{
    return ftCommonGuardOnCheckInterruptSuccess(fighter_gobj, 0);
}

// 0x80148D2C
sb32 ftCommonGuardOnCheckInterruptDashRun(GObj *fighter_gobj, s32 slide_tics)
{
    return ftCommonGuardOnCheckInterruptSuccess(fighter_gobj, slide_tics);
}

// 0x80148D4C
void ftCommonGuardProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftCommonGuardCheckScheduleRelease(fp);
    ftCommonGuardUpdateShieldVars(fighter_gobj);

    if (fp->shield_health == 0)
    {
        ftCommonShieldBreakFlyCommonSetStatus(fighter_gobj);
    }
    else if ((fp->status_vars.common.guard.is_release != FALSE) || !(fp->is_shield))
    {
        ftCommonGuardOffSetStatus(fighter_gobj);
    }
    else ftCommonGuardInitJoints(fighter_gobj);
}

// 0x80148DDC
void ftCommonGuardSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTCommonStatusGuard, 0.0F, 1.0F, (FTSTATUS_PRESERVE_MODELPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT));
    ftCommonGuardInitJoints(fighter_gobj);

    fp->is_shield = TRUE;
}
