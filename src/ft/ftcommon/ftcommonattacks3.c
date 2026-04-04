#include <ft/fighter.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014F760
void ftCommonAttackS3SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 stick_angle;
    s32 status_id;

    if (ftCommonGetCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        if (fp->data->mainmotion->motion_desc[nFTCommonMotionAttackS3HiS].anim_file_id != 0)
        {
            stick_angle = ftParamGetStickAngleRads(fp);

            status_id = (stick_angle > FTCOMMON_ATTACKS3_5ANGLE_HI_MIN)  ? nFTCommonStatusAttackS3Hi  : // High-Angled Forward Tilt
                        (stick_angle > FTCOMMON_ATTACKS3_5ANGLE_HIS_MIN) ? nFTCommonStatusAttackS3HiS : // Middle High-Angled Forward Tilt
                        (stick_angle < FTCOMMON_ATTACKS3_5ANGLE_LW_MIN)  ? nFTCommonStatusAttackS3Lw  : // Low-Angled Forward Tilt
                        (stick_angle < FTCOMMON_ATTACKS3_5ANGLE_LWS_MIN) ? nFTCommonStatusAttackS3LwS : // Middle Low-Angled Forward Tilt
                                                                           nFTCommonStatusAttackS3;     // Default Forward Tilt
        }
        else if (fp->data->mainmotion->motion_desc[nFTCommonMotionAttackS3Hi].anim_file_id != 0)
        {
            stick_angle = ftParamGetStickAngleRads(fp);

            status_id = (stick_angle > FTCOMMON_ATTACKS3_3ANGLE_HI_MIN)  ? nFTCommonStatusAttackS3Hi  : // High-Angled Forward Tilt
                        (stick_angle < FTCOMMON_ATTACKS3_3ANGLE_LW_MIN)  ? nFTCommonStatusAttackS3Lw  : // Low-Angled Forward Tilt
                                                                           nFTCommonStatusAttackS3;     // Default Forward Tilt
        }
        else status_id = nFTCommonStatusAttackS3;

        ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);
    }
}

// 0x8014F8C0
sb32 ftCommonAttackS3CheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if ((fp->input.pl.button_tap & fp->input.button_mask_a) && ((fp->input.pl.stick_range.x * fp->lr) >= FTCOMMON_ATTACKS3_STICK_RANGE_MIN))
    {
        if (((ftParamGetStickAngleRads(fp) < 0.0F) ? -ftParamGetStickAngleRads(fp) : ftParamGetStickAngleRads(fp)) <= F_CST_DTOR32(50.0F)) // 0.87266463F
        {
            if (fp->item_gobj != NULL)
            {
                if ((fp->input.pl.button_hold & fp->input.button_mask_z) || (itGetStruct(fp->item_gobj)->type == nITTypeThrow))
                {
                    ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowF);

                    return TRUE;
                }
                switch (itGetStruct(fp->item_gobj)->type)
                {
                case nITTypeSwing:
                    ftCommonItemSwingSetStatus(fighter_gobj, nFTItemSwingTypeAttack3);
                    return TRUE;

                case nITTypeShoot:
                    ftCommonItemShootSetStatus(fighter_gobj);
                    return TRUE;
                }
            }
            if (attr->is_have_attacks3)
            {
                ftCommonAttackS3SetStatus(fighter_gobj);

                return TRUE;
            }
        }
    }
    return FALSE;
}
