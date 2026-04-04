#include <ft/fighter.h>
#include <it/item.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801508E0
void ftCommonAttackAirLwProcHit(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->fkind == nFTKindLink) || (fp->fkind == nFTKindNLink))
    {
        ftParamClearAttackCollAll(fighter_gobj);

        fp->is_fastfall = FALSE;

        fp->physics.vel_air.y = FTCOMMON_ATTACKAIRLW_LINK_REHIT_BOUNCE_VEL_Y;

        if (fighter_gobj->anim_frame > FTCOMMON_ATTACKAIRLW_LINK_REHIT_FRAME_BEGIN)
        {
            ftMainSetStatus(fighter_gobj, nFTCommonStatusAttackAirLw, FTCOMMON_ATTACKAIRLW_LINK_REHIT_FRAME_BEGIN, 1.0F, FTSTATUS_PRESERVE_NONE);
        }
        fp->status_vars.common.attackair.rehit_timer = FTCOMMON_ATTACKAIRLW_LINK_REHIT_TIMER;
    }
}

// 0x80150980
void ftCommonAttackAirLwProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->fkind == nFTKindLink) || (fp->fkind == nFTKindNLink))
    {
        if (fp->status_vars.common.attackair.rehit_timer != 0)
        {
            fp->status_vars.common.attackair.rehit_timer--;

            if ((fp->status_vars.common.attackair.rehit_timer == 0) && (fighter_gobj->anim_frame < FTCOMMON_ATTACKAIRLW_LINK_REHIT_FRAME_END))
            {
                ftParamRefreshAttackCollID(fighter_gobj, 0);
                ftParamRefreshAttackCollID(fighter_gobj, 1);
            }
        }
    }
    ftAnimEndSetFall(fighter_gobj);
}

// 0x80150A08
void ftCommonAttackAirProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterLanding(fighter_gobj) != FALSE)
    {
        if ((fp->motion_vars.flags.flag1 != 0) && (fp->tics_since_last_z > FTCOMMON_ATTACKAIR_SMOOTHLANDING_TICS_MAX))
        {
            s32 landing_motion_id = nFTCommonMotionLandingAirStart - nFTCommonMotionAttackAirStart;

            if (fp->data->mainmotion->motion_desc[fp->motion_id + landing_motion_id].anim_file_id != 0)
            {
                ftCommonLandingAirSetStatus(fighter_gobj);
            }
            else ftCommonLandingAirNullSetStatus(fighter_gobj, F_PCT_TO_DEC(fp->motion_vars.flags.flag1));
        }
        else if (fp->physics.vel_air.y > FTCOMMON_ATTACKAIR_SKIPLANDING_VEL_Y_MAX)
        {
            ftCommonWaitSetStatus(fighter_gobj);
        }
        else ftCommonLandingSetStatus(fighter_gobj);
    }
}

// 0x80150B00 - Also checks LightThrowAir and ItemShoot
sb32 ftCommonAttackAirCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;
    s32 status_id;
    ub32 is_have_attackair;
    f32 angle;
    sb32 is_goto_shoot = FALSE;

    if (fp->input.pl.button_tap & fp->input.button_mask_a)
    {
        if (ftHammerCheckHoldHammer(fighter_gobj) == FALSE)
        {
            if (ftCommonLightThrowCheckItemTypeThrow(fp) != FALSE)
            {
                if ((ABS(fp->input.pl.stick_range.x) < FTCOMMON_ATTACKAIR_DIRECTION_STICK_RANGE_MIN) && (ABS(fp->input.pl.stick_range.y) < FTCOMMON_ATTACKAIR_DIRECTION_STICK_RANGE_MIN))
                {
                    if (itGetStruct(fp->item_gobj)->type == nITTypeThrow)
                    {
                        ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowAirF);

                        return TRUE;
                    }
                    else ftSetupDropItem(fp);

                    return FALSE;
                }
                else
                {
                    angle = ftParamGetStickAngleRads(fp);

                    if (angle > F_CST_DTOR32(50.0F)) // 0.87266463F
                    {
                        if (fp->hold_stick_y < FTCOMMON_LIGHTTHROWAIR4_BUFFER_TICS_MAX)
                        {
                            status_id = nFTCommonStatusLightThrowAirHi4;
                        }
                        else status_id = nFTCommonStatusLightThrowAirHi;
                    }
                    else if (angle < F_CST_DTOR32(-50.0F)) // -0.87266463F
                    {
                        if (fp->hold_stick_y < FTCOMMON_LIGHTTHROWAIR4_BUFFER_TICS_MAX)
                        {
                            status_id = nFTCommonStatusLightThrowAirLw4;
                        }
                        else status_id = nFTCommonStatusLightThrowAirLw;
                    }
                    else if ((fp->input.pl.stick_range.x * fp->lr) >= 0)
                    {
                        if (fp->hold_stick_x < FTCOMMON_LIGHTTHROWAIR4_BUFFER_TICS_MAX)
                        {
                            status_id = nFTCommonStatusLightThrowAirF4;
                        }
                        else status_id = nFTCommonStatusLightThrowAirF;
                    }
                    else if (fp->hold_stick_x < FTCOMMON_LIGHTTHROWAIR4_BUFFER_TICS_MAX)
                    {
                        status_id = nFTCommonStatusLightThrowAirB4;
                    }
                    else status_id = nFTCommonStatusLightThrowAirB;

                    ftCommonItemThrowSetStatus(fighter_gobj, status_id);

                    return TRUE;
                }
            }
            else
            {
                if ((ABS(fp->input.pl.stick_range.x) < FTCOMMON_ATTACKAIR_DIRECTION_STICK_RANGE_MIN) && (ABS(fp->input.pl.stick_range.y) < FTCOMMON_ATTACKAIR_DIRECTION_STICK_RANGE_MIN))
                {
                    status_id = nFTCommonStatusAttackAirN;
                    is_have_attackair = attr->is_have_attackairn;
                    is_goto_shoot = TRUE;
                }
                else
                {
                    angle = ftParamGetStickAngleRads(fp);

                    if (angle > F_CST_DTOR32(50.0F)) // 0.87266463F
                    {
                        status_id = nFTCommonStatusAttackAirHi;
                        is_have_attackair = attr->is_have_attackairhi;
                    }
                    else if (angle < F_CST_DTOR32(-50.0F)) // -0.87266463F
                    {
                        status_id = nFTCommonStatusAttackAirLw;
                        is_have_attackair = attr->is_have_attackairlw;
                    }
                    else if ((fp->input.pl.stick_range.x * fp->lr) >= 0)
                    {
                        status_id = nFTCommonStatusAttackAirF;
                        is_have_attackair = attr->is_have_attackairf;
                        is_goto_shoot = TRUE;
                    }
                    else
                    {
                        status_id = nFTCommonStatusAttackAirB;
                        is_have_attackair = attr->is_have_attackairb;
                    }
                }
            }
            if (is_goto_shoot != FALSE)
            {
                if ((fp->item_gobj != NULL) && (itGetStruct(fp->item_gobj)->type == nITTypeShoot))
                {
                    ftCommonItemShootAirSetStatus(fighter_gobj);

                    return TRUE;
                }
            }
            if (is_have_attackair)
            {
                fp->motion_vars.flags.flag1 = 0;

                ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_FASTFALL);

                if (status_id == nFTCommonStatusAttackAirLw)
                {
                    fp->proc_hit = ftCommonAttackAirLwProcHit;

                    fp->status_vars.common.attackair.rehit_timer = 0;
                }
                ftMainPlayAnimEventsAll(fighter_gobj);

                fp->tics_since_last_z = FTINPUT_ZTRIGLAST_TICS_MAX;

                return TRUE;
            }
        }
    }
    return FALSE;
}
