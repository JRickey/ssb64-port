#include <ft/fighter.h>
#include <it/item.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftCommonAttack13CheckFighterKind(fp)\
(                                           \
    ((fp)->fkind == nFTKindMario)   ||      \
    ((fp)->fkind == nFTKindMMario)  ||      \
    ((fp)->fkind == nFTKindNMario)  ||      \
    ((fp)->fkind == nFTKindLuigi)   ||      \
    ((fp)->fkind == nFTKindNLuigi)  ||      \
    ((fp)->fkind == nFTKindCaptain) ||      \
    ((fp)->fkind == nFTKindNCaptain)||      \
    ((fp)->fkind == nFTKindLink)    ||      \
    ((fp)->fkind == nFTKindNLink)   ||      \
    ((fp)->fkind == nFTKindNess)    ||      \
    ((fp)->fkind == nFTKindNNess)           \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014E7B0
void ftCommonAttack11ProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->motion_vars.flags.flag1 != 0) && (fp->status_vars.common.attack1.is_goto_followup != FALSE))
    {
        if ((fp->fkind == nFTKindPikachu) || (fp->fkind == nFTKindNPikachu))
        {
            ftCommonAttack11SetStatus(fighter_gobj);
        }
        else ftCommonAttack12SetStatus(fighter_gobj);
    }
    else ftAnimEndSetWait(fighter_gobj);
}

// 0x8014E824
void ftCommonAttack12ProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->fkind != nFTKindCaptain) && (fp->fkind != nFTKindNCaptain) && (fp->motion_vars.flags.flag1 != 0) && (fp->is_goto_attack100))
    {
        ftCommonAttack100StartSetStatus(fighter_gobj);
    }
    else if ((fp->motion_vars.flags.flag1 != 0) && (fp->status_vars.common.attack1.is_goto_followup != FALSE))
    {
        ftCommonAttack13SetStatus(fighter_gobj);
    }
    else ftAnimEndSetWait(fighter_gobj);
}

// 0x8014E8B4
void ftCommonAttack13ProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (((fp->fkind == nFTKindCaptain) || (fp->fkind == nFTKindNCaptain)) && (fp->motion_vars.flags.flag1 != 0) && (fp->is_goto_attack100))
    {
        ftCommonAttack100StartSetStatus(fighter_gobj);
    }
    else ftAnimEndSetWait(fighter_gobj);
}

// 0x8014E91C
void ftCommonAttack11ProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.attack1.interrupt_catch_timer < 2)
    {
        fp->status_vars.common.attack1.interrupt_catch_timer++;

        if (ftCommonCatchCheckInterruptAttack11(fighter_gobj) != FALSE)
        {
            return;
        }
    }
    if (ftCommonAttack100StartCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        if ((fp->fkind == nFTKindPikachu) || (fp->fkind == nFTKindNPikachu))
        {
            if (ftCommonAttack11CheckGoto(fighter_gobj) != FALSE)
            {
                return;
            }
        }
        else ftCommonAttack12CheckGoto(fighter_gobj);
    }
}

// 0x8014E9B4
void ftCommonAttack12ProcInterrupt(GObj *fighter_gobj)
{
    if (ftCommonAttack13CheckGoto(fighter_gobj) == FALSE)
    {
        ftCommonAttack100StartCheckInterruptCommon(fighter_gobj);
    }
}

// 0x8014E9E4
void ftCommonAttack13ProcInterrupt(GObj *fighter_gobj)
{
    ftCommonAttack100StartCheckInterruptCommon(fighter_gobj);
}

// 0x8014EA04
void ftCommonAttack11ProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamSetMotionID(fp, nFTMotionAttackIDAttack11);
    ftParamSetStatUpdate(fp, fp->stat_flags.halfword);
    ftParamUpdate1PGameAttackStats(fp, 0);
}

// 0x8014EA44
void ftCommonAttack11SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (ftCommonGetCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        fp->proc_status = ftCommonAttack11ProcStatus;

        ftMainSetStatus(fighter_gobj, nFTCommonStatusAttack11, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);

        fp->motion_vars.flags.flag1 = 0;

        fp->status_vars.common.attack1.is_goto_followup = FALSE;
        fp->status_vars.common.attack1.interrupt_catch_timer = 0;

        fp->attack1_input_count = 0;
        fp->attack1_status_id = fp->status_id;
        fp->is_goto_attack100 = FALSE;
        fp->attack1_followup_frames = attr->attack1_followup_frames;
    }
}

// 0x8014EAD8
void ftCommonAttack12SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftCommonGetCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        ftMainSetStatus(fighter_gobj, nFTCommonStatusAttack12, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);

        fp->motion_vars.flags.flag1 = 0;

        fp->status_vars.common.attack1.is_goto_followup = FALSE;

        fp->attack1_status_id = fp->status_id;

        switch (fp->fkind)
        {
        case nFTKindMario:
        case nFTKindMMario:
        case nFTKindNMario:
            fp->attack1_followup_frames = FTCOMMON_ATTACK1_FOLLOWUP_FRAMES_DEFAULT;
            break;

        case nFTKindLuigi:
        case nFTKindNLuigi:
            fp->attack1_followup_frames = FTCOMMON_ATTACK1_FOLLOWUP_FRAMES_DEFAULT;
            break;

        case nFTKindCaptain:
        case nFTKindNCaptain:
            fp->attack1_followup_frames = FTCOMMON_ATTACK1_FOLLOWUP_FRAMES_DEFAULT;
            break;

        case nFTKindLink:
        case nFTKindNLink:
            fp->attack1_followup_frames = FTCOMMON_ATTACK1_FOLLOWUP_FRAMES_DEFAULT;
            break;

        case nFTKindNess:
        case nFTKindNNess:
            fp->attack1_followup_frames = FTCOMMON_ATTACK1_FOLLOWUP_FRAMES_DEFAULT;
            break;
        }
    }
}

// 0x8014EBB4
void ftCommonAttack13SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (ftCommonGetCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        switch (fp->fkind)
        {
        case nFTKindMario:
        case nFTKindMMario:
        case nFTKindNMario:
            status_id = nFTMarioStatusAttack13;
            break;

        case nFTKindLuigi:
        case nFTKindNLuigi:
            status_id = nFTMarioStatusAttack13;
            break;

        case nFTKindCaptain:
        case nFTKindNCaptain:
            status_id = nFTCaptainStatusAttack13;
            break;

        case nFTKindLink:
        case nFTKindNLink:
            status_id = nFTLinkStatusAttack13;
            break;

        case nFTKindNess:
        case nFTKindNNess:
            status_id = nFTNessStatusAttack13;
            break;
        }
        ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);

        fp->motion_vars.flags.flag1 = 0;
        fp->status_vars.common.attack1.is_goto_followup = FALSE;
        fp->attack1_status_id = fp->status_id;
    }
}

// 0x8014EC78
sb32 ftCommonAttack1CheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->input.pl.button_tap & fp->input.button_mask_a)
    {
        if (fp->item_gobj != NULL)
        {
            if (itGetStruct(fp->item_gobj)->type == nITTypeThrow)
            {
                ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowF);

                return TRUE;
            }
            if (fp->input.pl.button_hold & fp->input.button_mask_z)
            {
                ftCommonItemThrowSetStatus(fighter_gobj, nFTCommonStatusLightThrowDrop);

                return TRUE;
            }
            switch (itGetStruct(fp->item_gobj)->type)
            {
            case nITTypeSwing:
                ftCommonItemSwingSetStatus(fighter_gobj, nFTItemSwingTypeAttack1);
                return TRUE;

            case nITTypeShoot:
                ftCommonItemShootSetStatus(fighter_gobj);
                return TRUE;
            }
        }
        if (fp->attack1_followup_frames != 0.0F)
        {
            switch (fp->attack1_status_id)
            {
            case nFTCommonStatusAttack11:
                if ((fp->fkind == nFTKindPikachu) || (fp->fkind == nFTKindNPikachu))
                {
                    if (attr->is_have_attack11)
                    {
                        ftCommonAttack11SetStatus(fighter_gobj);

                        fp->attack1_input_count++;

                        return TRUE;
                    }
                    break;
                }
                else if (attr->is_have_attack12)
                {
                    ftCommonAttack12SetStatus(fighter_gobj);

                    fp->attack1_input_count++;

                    return TRUE;
                }
                break;

            case nFTCommonStatusAttack12:
                if (ftCommonAttack13CheckFighterKind(fp))
                {
                    ftCommonAttack13SetStatus(fighter_gobj);

                    return TRUE;
                }
                break;
            }
        }
        else if (attr->is_have_attack11)
        {
            ftCommonAttack11SetStatus(fighter_gobj);

            return TRUE;
        }
    }
    if (fp->attack1_followup_frames != 0.0F)
    {
        fp->attack1_followup_frames -= DObjGetStruct(fighter_gobj)->anim_speed;
    }
    return FALSE;
}

// 0x8014EEC0
sb32 ftCommonAttack11CheckGoto(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->attack1_followup_frames != 0.0F)
    {
        fp->attack1_followup_frames -= DObjGetStruct(fighter_gobj)->anim_speed;

        if ((fp->input.pl.button_tap & fp->input.button_mask_a) && (attr->is_have_attack11))
        {
            if (fp->motion_vars.flags.flag1 != 0)
            {
                ftCommonAttack11SetStatus(fighter_gobj);

                return TRUE;
            }
            fp->status_vars.common.attack1.is_goto_followup = TRUE;
        }
    }
    return FALSE;
}

// 0x8014EF50
sb32 ftCommonAttack12CheckGoto(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->attack1_followup_frames != 0.0F)
    {
        fp->attack1_followup_frames -= DObjGetStruct(fighter_gobj)->anim_speed;

        if ((fp->input.pl.button_tap & fp->input.button_mask_a) && (attr->is_have_attack12))
        {
            if (fp->motion_vars.flags.flag1 != 0)
            {
                ftCommonAttack12SetStatus(fighter_gobj);

                return TRUE;
            }
            fp->status_vars.common.attack1.is_goto_followup = TRUE;
        }
    }
    return FALSE;
}

// 0x8014EFE0
sb32 ftCommonAttack13CheckGoto(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(ftCommonAttack13CheckFighterKind(fp)))
    {
        return FALSE;
    }
    else
    {
        if (fp->attack1_followup_frames != 0.0F)
        {
            fp->attack1_followup_frames -= DObjGetStruct(fighter_gobj)->anim_speed;

            if (fp->input.pl.button_tap & fp->input.button_mask_a)
            {
                if (fp->motion_vars.flags.flag1 != 0)
                {
                    ftCommonAttack13SetStatus(fighter_gobj);

                    return TRUE;
                }
                fp->status_vars.common.attack1.is_goto_followup = TRUE;
            }
        }
    }
    return FALSE;
}
