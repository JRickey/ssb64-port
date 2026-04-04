#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define ftCommonAttack100CheckFighterKind(fp) \
(                                             \
    ((fp)->fkind == nFTKindFox)        ||  \
    ((fp)->fkind == nFTKindNFox)    ||  \
    ((fp)->fkind == nFTKindLink)       ||  \
    ((fp)->fkind == nFTKindNLink)   ||  \
    ((fp)->fkind == nFTKindKirby)      ||  \
    ((fp)->fkind == nFTKindNKirby)  ||  \
    ((fp)->fkind == nFTKindPurin)      ||  \
    ((fp)->fkind == nFTKindNPurin)  ||  \
    ((fp)->fkind == nFTKindCaptain)    ||  \
    ((fp)->fkind == nFTKindNCaptain)    \
)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014F0D0
void ftCommonAttack100StartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonAttack100LoopSetStatus);
}

// 0x8014F0F4
void ftCommonAttack100StartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    if (ftCommonGetCheckInterruptCommon(fighter_gobj) == FALSE)
    {
        switch (fp->fkind)
        {
        case nFTKindFox:
        case nFTKindNFox:
            status_id = nFTFoxStatusAttack100Start;
            break;

        case nFTKindLink:
        case nFTKindNLink:
            status_id = nFTLinkStatusAttack100Start;
            break;

        case nFTKindKirby:
        case nFTKindNKirby:
            status_id = nFTKirbyStatusAttack100Start;
            break;

        case nFTKindPurin:
        case nFTKindNPurin:
            status_id = nFTPurinStatusAttack100Start;
            break;

        case nFTKindCaptain:
        case nFTKindNCaptain:
            status_id = nFTCaptainStatusAttack100Start;
            break;
        }
        ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
        ftMainPlayAnimEventsAll(fighter_gobj);

        fp->status_vars.common.attack100.is_anim_end = FALSE;
        fp->status_vars.common.attack100.is_goto_loop = FALSE;

        fp->motion_vars.flags.flag1 = 0;
        fp->motion_vars.flags.flag2 = 0;
    }
}

// 0x8014F1BC
void ftCommonAttack100LoopKirbyUpdateEffect(FTStruct *fp)
{
    Vec3f pos;

    if (fp->fkind == nFTKindKirby)
    {
        if (fp->motion_vars.flags.flag2 != 0)
        {
            ftKirbyAttack100Effect *effect = (ftKirbyAttack100Effect*) ((uintptr_t)gFTDataKirbyMainMotion + (intptr_t)&llKirbyMainMotionftKirbyAttack100Effect);

            pos.x = effect[fp->motion_vars.flags.flag2 - 1].offset.x;
            pos.y = effect[fp->motion_vars.flags.flag2 - 1].offset.y;
            pos.z = effect[fp->motion_vars.flags.flag2 - 1].offset.z;

            gmCollisionGetFighterPartsWorldPosition(fp->joints[nFTPartsJointTopN], &pos);

            efManagerKirbyVulcanJabMakeEffect(&pos, fp->lr, effect[fp->motion_vars.flags.flag2 - 1].rotate, effect[fp->motion_vars.flags.flag2 - 1].vel, effect[fp->motion_vars.flags.flag2 - 1].add);

            fp->motion_vars.flags.flag2 = 0;
        }
    }
}

// 0x8014F2A8
void ftCommonAttack100LoopProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fighter_gobj->anim_frame >= 0.0F) && (fighter_gobj->anim_frame < DObjGetStruct(fighter_gobj)->anim_speed))
    {
        fp->status_vars.common.attack100.is_anim_end = TRUE;

        ftParamSetMotionID(fp, nFTMotionAttackIDAttack100);
        ftParamSetStatUpdate(fp, fp->stat_flags.halfword);
        ftParamUpdate1PGameAttackStats(fp, 0);
    }
    if (fp->motion_vars.flags.flag1 != 0)
    {
        fp->motion_vars.flags.flag1 = 0;

        if ((fp->status_vars.common.attack100.is_anim_end != FALSE) && (fp->status_vars.common.attack100.is_goto_loop == FALSE))
        {
            ftCommonAttack100EndSetStatus(fighter_gobj);

            return;
        }
        else if (ftCommonGetCheckInterruptCommon(fighter_gobj) == FALSE)
        {
            fp->status_vars.common.attack100.is_goto_loop = FALSE;
        }
        else return;
    }
    ftCommonAttack100LoopKirbyUpdateEffect(fp);
}

// 0x8014F388
void ftCommonAttack100LoopProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->input.pl.button_tap & fp->input.button_mask_a) || (fp->input.pl.button_release & fp->input.button_mask_a))
    {
        fp->status_vars.common.attack100.is_goto_loop = TRUE;
    }
}

// 0x8014F3C0
void ftCommonAttack100LoopSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    switch (fp->fkind)
    {
    case nFTKindFox:
    case nFTKindNFox:
        status_id = nFTFoxStatusAttack100Loop;
        break;

    case nFTKindLink:
    case nFTKindNLink:
        status_id = nFTLinkStatusAttack100Loop;
        break;

    case nFTKindKirby:
    case nFTKindNKirby:
        status_id = nFTKirbyStatusAttack100Loop;
        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        status_id = nFTPurinStatusAttack100Loop;
        break;

    case nFTKindCaptain:
    case nFTKindNCaptain:
        status_id = nFTCaptainStatusAttack100Loop;
        break;
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftCommonAttack100LoopKirbyUpdateEffect(fp);
}

// 0x8014F45C
void ftCommonAttack100EndSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;

    switch (fp->fkind)
    {
    case nFTKindFox:
    case nFTKindNFox:
        status_id = nFTFoxStatusAttack100End;
        break;

    case nFTKindLink:
    case nFTKindNLink:
        status_id = nFTLinkStatusAttack100End;
        break;

    case nFTKindKirby:
    case nFTKindNKirby:
        status_id = nFTKirbyStatusAttack100End;
        break;

    case nFTKindPurin:
    case nFTKindNPurin:
        status_id = nFTPurinStatusAttack100End;
        break;

    case nFTKindCaptain:
    case nFTKindNCaptain:
        status_id = nFTCaptainStatusAttack100End;
        break;
    }
    ftMainSetStatus(fighter_gobj, status_id, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8014F4EC
sb32 ftCommonAttack100StartCheckInterruptCommon(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id;
    s32 inputs_min;

    if (!(ftCommonAttack100CheckFighterKind(fp)))
    {
        return FALSE;
    }
    if ((fp->input.pl.button_tap & fp->input.button_mask_a) || (fp->input.pl.button_release & fp->input.button_mask_a))
    {
        /*
         * A frame-perfect A press (tapped on one frame and immediately released on the next)
         * registers twice due to the way button_release works. 
         */ 

        fp->attack1_input_count++;

        switch (fp->fkind)
        {
        case nFTKindFox:
        case nFTKindNFox:
            inputs_min = 4;
            status_id = nFTCommonStatusAttack12;
            break;

        case nFTKindLink:
        case nFTKindNLink:
            inputs_min = 5;
            status_id = nFTCommonStatusAttack12;
            break;

        case nFTKindKirby:
        case nFTKindNKirby:
            inputs_min = 4;
            status_id = nFTCommonStatusAttack12;
            break;

        case nFTKindPurin:
        case nFTKindNPurin:
            inputs_min = 4;
            status_id = nFTCommonStatusAttack12;
            break;

        case nFTKindCaptain:
        case nFTKindNCaptain:
            inputs_min = 6;
            status_id = nFTCaptainStatusAttack13;
            break;
        }
        if (fp->attack1_input_count >= inputs_min)
        {
            if ((status_id == fp->status_id) && (fp->motion_vars.flags.flag1 != 0))
            {
                ftCommonAttack100StartSetStatus(fighter_gobj);

                return TRUE;
            }
            else fp->is_goto_attack100 = TRUE;
        }
    }
    return FALSE;
}
