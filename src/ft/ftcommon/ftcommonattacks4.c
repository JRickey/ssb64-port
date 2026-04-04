#include <ft/fighter.h>
#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014FE40
void ftCommonAttackS4ProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f offset;

    switch (fp->fkind)
    {
    case nFTKindPikachu:
    case nFTKindNPikachu:
        if ((fp->motion_vars.flags.flag1 != 0) || (fp->motion_vars.flags.flag2 != 0))
        {
            fp->status_vars.common.attack4.gfx_id += syUtilsRandIntRange((FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_ID_MAX - 1)) + 1;

            if (fp->status_vars.common.attack4.gfx_id >= FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_ID_MAX)
            {
                fp->status_vars.common.attack4.gfx_id -= FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_ID_MAX;
            }
            if (fp->motion_vars.flags.flag1 != 0)
            {
                fp->motion_vars.flags.flag1 = 0;

                offset.x = -FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_OFF_X;
                offset.z = FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_OFF_Z;
                offset.y = FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_OFF_Y;
            }
            if (fp->motion_vars.flags.flag2 != 0)
            {
                fp->motion_vars.flags.flag2 = 0;

                offset.x = FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_OFF_X;
                offset.z = FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_OFF_Z;
                offset.y = FTCOMMON_ATTACKS4_THUNDERSHOCK_GFX_OFF_Y;

            }
            gmCollisionGetFighterPartsWorldPosition(fp->joints[11], &offset);
            func_ovl2_800EE018(fp->joints[nFTPartsJointTopN], &offset);

            if (efManagerPikachuThunderShockMakeEffect(fighter_gobj, &offset, fp->status_vars.common.attack4.gfx_id) != NULL)
            {
                fp->is_effect_attach = TRUE;
            }
        }
        // Fallthrough, should break here for efficiency
    case nFTKindNess:
    case nFTKindNNess:
        if ((fp->motion_vars.flags.flag1 != 0) && !(fp->is_reflect))
        {
            fp->is_reflect = TRUE;
        }
        if ((fp->motion_vars.flags.flag1 == 0) && (fp->is_reflect))
        {
            fp->is_reflect = FALSE;
        }
        break;
    }
    ftAnimEndSetWait(fighter_gobj);
}

// 0x8014FFE0
void ftCommonAttackS4SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 stick_angle;
    s32 status_id;

    if (fp->data->mainmotion->motion_desc[nFTCommonMotionAttackS4HiS].anim_file_id != 0)
    {
        stick_angle = ftParamGetStickAngleRads(fp);

        status_id = (stick_angle > FTCOMMON_ATTACKS4_5ANGLE_HI_MIN)  ? nFTCommonStatusAttackS4Hi  :
                    (stick_angle > FTCOMMON_ATTACKS4_5ANGLE_HIS_MIN) ? nFTCommonStatusAttackS4HiS :
                    (stick_angle < FTCOMMON_ATTACKS4_5ANGLE_LW_MIN)  ? nFTCommonStatusAttackS4Lw  :
                    (stick_angle < FTCOMMON_ATTACKS4_5ANGLE_LWS_MIN) ? nFTCommonStatusAttackS4LwS :
                                                                       nFTCommonStatusAttackS4;
    }
    else if (fp->data->mainmotion->motion_desc[nFTCommonMotionAttackS4Hi].anim_file_id != 0)
    {
        stick_angle = ftParamGetStickAngleRads(fp);

        status_id = (stick_angle > FTCOMMON_ATTACKS4_3ANGLE_HI_MIN)  ? nFTCommonStatusAttackS4Hi  :
                    (stick_angle < FTCOMMON_ATTACKS4_3ANGLE_LW_MIN)  ? nFTCommonStatusAttackS4Lw  :
                                                                       nFTCommonStatusAttackS4;
    }
    else status_id = nFTCommonStatusAttackS4;

    switch (fp->fkind)
    {
    case nFTKindPikachu:
    case nFTKindNPikachu:
        fp->motion_vars.flags.flag2 = 0;
        fp->motion_vars.flags.flag1 = 0;
        break;

    case nFTKindNess:
    case nFTKindNNess:
        fp->motion_vars.flags.flag1 = 0;
        break;
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    switch (fp->fkind)
    {
    case nFTKindPikachu:
    case nFTKindNPikachu:
        fp->status_vars.common.attack4.gfx_id = 0;

        fp->proc_lagstart = ftParamProcPauseEffect;
        fp->proc_lagend = ftParamProcResumeEffect;
        break;

    case nFTKindNess:
    case nFTKindNNess:
        fp->special_coll = (FTSpecialColl*) ((uintptr_t)gFTNessFileMainMotion + (intptr_t)&llNessMainMotionAttackS4ReflectorFTSpecialColl);
        break;
    }
}

// 0x801501E0
sb32 ftCommonAttackS4CheckInterruptDash(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (((fp->input.pl.stick_range.x * fp->lr) >= FTCOMMON_ATTACKS4_STICK_RANGE_MIN) && (fp->input.pl.button_tap & fp->input.button_mask_a))
    {
        GObj *item_gobj = fp->item_gobj;

        if (item_gobj != NULL)
        {
            ITStruct *ip = itGetStruct(item_gobj);

            if ((fp->input.pl.button_hold & fp->input.button_mask_z) || ((ip->type == nITTypeThrow) || (ip->type == nITTypeShoot) && (itMainCheckShootNoAmmo(item_gobj) != FALSE)))
            {
                ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowF4);

                return TRUE;
            }
            switch (ip->type)
            {
            case nITTypeSwing:
                ftCommonItemSwingSetStatus(fighter_gobj, nFTItemSwingTypeAttack4);
                return TRUE;

            case nITTypeShoot:
                ftCommonItemShootSetStatus(fighter_gobj);
                return TRUE;
            }
        }
        if (attr->is_have_attacks4)
        {
            ftCommonAttackS4SetStatus(fighter_gobj);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x8015030C
sb32 ftCommonAttackS4CheckInterruptTurn(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (((fp->input.pl.stick_range.x * fp->status_vars.common.attack4.lr) >= FTCOMMON_ATTACKS4_STICK_RANGE_MIN) && (fp->input.pl.button_tap & fp->input.button_mask_a))
    {
        GObj *item_gobj = fp->item_gobj;

        if (item_gobj != NULL)
        {
            ITStruct *ip = itGetStruct(item_gobj);

            if ((fp->input.pl.button_hold & fp->input.button_mask_z) || ((ip->type == nITTypeThrow) || (ip->type == nITTypeShoot) && (itMainCheckShootNoAmmo(item_gobj) != FALSE)))
            {
                ftCommonItemThrowSetStatus(fighter_gobj, ((fp->input.pl.stick_range.x * fp->lr) >= 0) ? nFTCommonStatusLightThrowF4 : nFTCommonStatusLightThrowB4);

                return TRUE;
            }
            switch (ip->type)
            {
            case nITTypeSwing:
                ftParamSetStickLR(fp);
                ftCommonItemSwingSetStatus(fighter_gobj, nFTItemSwingTypeAttack4);
                return TRUE;

            case nITTypeShoot:
                ftParamSetStickLR(fp);
                ftCommonItemShootSetStatus(fighter_gobj);
                return TRUE;
            }
        }
        if (attr->is_have_attacks4)
        {
            ftParamSetStickLR(fp);
            ftCommonAttackS4SetStatus(fighter_gobj);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80150470
sb32 ftCommonAttackS4CheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if ((ABS(fp->input.pl.stick_range.x) >= FTCOMMON_ATTACKS4_STICK_RANGE_MIN) && (fp->tap_stick_x < FTCOMMON_ATTACKS4_BUFFER_TICS_MAX) && (fp->input.pl.button_tap & fp->input.button_mask_a))
    {
        GObj *item_gobj = fp->item_gobj;

        if (item_gobj != NULL)
        {
            ITStruct *ip = itGetStruct(item_gobj);

            if ((fp->input.pl.button_hold & fp->input.button_mask_z) || ((ip->type == nITTypeThrow) || (ip->type == nITTypeShoot) && (itMainCheckShootNoAmmo(item_gobj) != FALSE)))
            {
                ftCommonItemThrowSetStatus(fighter_gobj, ((fp->input.pl.stick_range.x * fp->lr) >= 0) ? nFTCommonStatusLightThrowF4 : nFTCommonStatusLightThrowB4);

                return TRUE;
            }
            switch (ip->type)
            {
            case nITTypeSwing:
                ftParamSetStickLR(fp);
                ftCommonItemSwingSetStatus(fighter_gobj, nFTItemSwingTypeAttack4);
                return TRUE;

            case nITTypeShoot:
                ftParamSetStickLR(fp);
                ftCommonItemShootSetStatus(fighter_gobj);
                return TRUE;
            }
        }
        if (attr->is_have_attacks4)
        {
            ftParamSetStickLR(fp);
            ftCommonAttackS4SetStatus(fighter_gobj);

            return TRUE;
        }
    }
    return FALSE;
}
