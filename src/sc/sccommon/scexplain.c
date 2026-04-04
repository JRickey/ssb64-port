#include <ft/fighter.h>
#include <it/item.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018E6E0
s32 dSCExplainInterfacePositions[/* */] = {  55, 125, 195, 265 };

// 0x8018E6F0
s32 dSCExplainPad0x8018E6F0 = 0;

// 0x8018E6F4
intptr_t dSCExplainStickMatAnimJoints[/* */] = 
{
    0x0, 
    &llSCExplainGraphicsStickNeutralMatAnimJoint,
    &llSCExplainGraphicsStickNeutralMatAnimJoint,
    &llSCExplainGraphicsStickHoldUpMatAnimJoint,
    &llSCExplainGraphicsStickTapUpMatAnimJoint,
    &llSCExplainGraphicsStickHoldForwardMatAnimJoint,
    &llSCExplainGraphicsStickTapForwardMatAnimJoint
};

// 0x8018E710
intptr_t dSCExplainKeyKeyEventss[/* */] = 
{
    &llSCExplainMain0KeyEvent,
    &llSCExplainMain1KeyEvent,
    &llSCExplainMain2KeyEvent,
    &llSCExplainMain3KeyEvent
};

// 0x8018E720
SYColorRGBA dSCExplainFadeColor = { 0x00, 0x00, 0x00, 0x00 };

// 0x8018E724
s32 dSCExplainRandomSeed1 = 0x00000001;

// 0x8018E728
s32 dSCExplainRandomSeed2 = 0x00000001;

// 0x8018E72C
SYVideoSetup dSCExplainVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E748
SYTaskmanSetup dSCExplainTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        scExplainFuncUpdate,        // Update function
        scExplainFuncDraw,          // Frame draw function
        &ovl63_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 3072,         // Display List Buffer 0 Size
        sizeof(Gfx) * 1536,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        scExplainFuncLights,    	// Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    0,                              // Number of thread stacks
    0,                              // ???
    0,                              // Number of GObjProcesses
    0,                              // Number of GObjs
    sizeof(GObj),                   // GObj size
    0,                              // Number of XObjs
    dLBCommonFuncMatrixList,        // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    scExplainFuncStart           	// Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E7F0
SCBattleState sSCExplainBattleState;

// 0x8018E9E0
void *sSCExplainGraphicsFileHead;

// 0x8018E9E4
void *sSCExplainMainFileHead;

// 0x8018E9E8
SCExplainPhase *sSCExplainPhase;

// 0x8018E9F0
SCExplainMain sSCExplainStruct;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void scExplainLoadExplainFiles(void)
{
    sSCExplainGraphicsFileHead = lbRelocGetExternHeapFile
    (
        (u32)&llSCExplainGraphicsFileID,
        syTaskmanMalloc
        (
            lbRelocGetFileSize((u32)&llSCExplainGraphicsFileID),
            0x10
        )
    );
    sSCExplainMainFileHead = lbRelocGetExternHeapFile
    (
        (u32)&llSCExplainMainFileID,
        syTaskmanMalloc
        (
            lbRelocGetFileSize((u32)&llSCExplainMainFileID),
            0x10
        )
    );
    sSCExplainPhase = lbRelocGetFileData(SCExplainPhase*, sSCExplainMainFileHead, &llSCExplainMainExplainPhase);
}

// 0x8018D14C
void scExplainSetBattleState(void)
{
    sSCExplainBattleState = dSCManagerDefaultBattleState;

    gSCManagerBattleState = &sSCExplainBattleState;

    gSCManagerBattleState->game_type = nSCBattleGameTypeExplain;

    gSCManagerBattleState->gkind = nGRKindExplain;

    gSCManagerBattleState->pl_count = 2;
    gSCManagerBattleState->cp_count = 0;

    gSCManagerBattleState->players[0].fkind = nFTKindMario;
    gSCManagerBattleState->players[1].fkind = nFTKindLuigi;

    gSCManagerBattleState->players[0].pkind = nFTPlayerKindGameKey;
    gSCManagerBattleState->players[1].pkind = nFTPlayerKindGameKey;
}

// 0x8018D1D4
void scExplainStartBattle(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        ftCommonAppearSetStatus(fighter_gobj);
        ftParamUnlockPlayerControl(fighter_gobj);

        fp->camera_mode = nFTCameraModeExplain;

        fighter_gobj = fighter_gobj->link_next;
    }
    gSCManagerBattleState->game_status = nSCBattleGameStatusGo;
}

// 0x8018D248
void scExplainMakeCamera(void)
{
    CObj *cobj = CObjGetStruct(gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF)));

    syRdpSetViewport
    (
        &cobj->viewport,
        gGMCameraStruct.viewport_ulx,
        gGMCameraStruct.viewport_uly,
        gGMCameraStruct.viewport_lrx,
        gGMCameraStruct.viewport_lry
    );
}

// 0x8018D2D0
void scExplainWindowProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetScissor(gSYTaskmanDLHeads[0]++, G_SC_NON_INTERLACE, 10, 160, 310, 230);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_FILL);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetFillColor(gSYTaskmanDLHeads[0]++, syVideoGetFillColor(GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF)));
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 160, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x8018D3D8
void scExplainMakeWindowCamera(void)
{
    gcMakeCameraGObj
    (
        nGCCommonKindScissorCamera,
        NULL,
        nGCCommonLinkIDCamera,
        GOBJ_PRIORITY_DEFAULT,
        scExplainWindowProcDisplay,
        15,
        0,
        0,
        FALSE,
        nGCProcessKindThread,
        NULL,
        0,
        FALSE
    );
}

// 0x8018D440
void scExplainSetPlayerInterfacePositions(void)
{
    gIFCommonPlayerInterface.player_pos_x = dSCExplainInterfacePositions;
    gIFCommonPlayerInterface.player_pos_y = 150;
}

// 0x8018D460
GObj* scExplainMakeTextCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindScissorCamera,
        NULL,
        9,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSprite,
        15,
        COBJ_MASK_DLLINK(26), 
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    syRdpSetViewport(&cobj->viewport, 10.0F, 160.0F, 310.0F, 230.0F);

    return camera_gobj;
}

// 0x8018D500
GObj* scExplainMakeControlStickCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindScissorCamera,
        NULL,
        nGCCommonLinkIDCamera,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        15,
        COBJ_MASK_DLLINK(27),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    gcAddXObjForCamera(cobj, nGCMatrixKindOrtho, 1);
    gcAddXObjForCamera(cobj, 6, 1);

    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.ortho.l = -150.0F;
    cobj->projection.ortho.r = 150.0F;
    cobj->projection.ortho.b = -110.0F;
    cobj->projection.ortho.t = 110.0F;

    return camera_gobj;
}

// 0x8018D5FC
void scExplainControlStickProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gSPClearGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

    gcDrawDObjTreeDLLinksForGObj(gobj);

    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gSPSetGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
}

// 0x8018D6DC
void scExplainProcUpdateControlStickSprite(GObj *gobj)
{
    gcPlayAnimAll(gobj);

    if (DObjGetStruct(gobj)->mobj->anim_frame == 15.0F)
    {
        if ((sSCExplainStruct.stick_status == 4) || (sSCExplainStruct.stick_status == 6))
        {
            scExplainUpdateTapSparkEffect();
        }
    }
}

// 0x8018D748
GObj* scExplainMakeControlStickInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter
    (
        nGCCommonKindInterface,
        NULL,
        nGCCommonLinkIDInterface,
        GOBJ_PRIORITY_DEFAULT
    );
    gcAddGObjDisplay(interface_gobj, scExplainControlStickProcDisplay, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    gcSetupCustomDObjs
    (
        interface_gobj, 
        ((uintptr_t)sSCExplainGraphicsFileHead + (intptr_t)&llSCExplainGraphicsStickDObjDesc),
        NULL,
        nGCMatrixKindTra,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddMObjAll
    (
        interface_gobj,
        lbRelocGetFileData
        (
            MObjSub***, 
            sSCExplainGraphicsFileHead, 
            &llSCExplainGraphicsStickMObjSub
        )
    );
    gcAddGObjProcess(interface_gobj, scExplainProcUpdateControlStickSprite, nGCProcessKindFunc, 5);

    interface_gobj->flags = GOBJ_FLAG_HIDDEN;

    return interface_gobj;
}

// 0x8018D808
void scExplainTapSparkProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gSPClearGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

    gcDrawDObjDLHead1(gobj);

    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gSPSetGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
}

// 0x8018D8E8
void scExplainUpdateTapSparkEffect(void)
{
    GObj *gobj = sSCExplainStruct.spark_gobj;
    DObj *dobj = DObjGetStruct(gobj);
    Vec3f *pos = &DObjGetStruct(sSCExplainStruct.stick_gobj)->translate.vec.f;

    if (sSCExplainStruct.stick_status == 4)
    {
        dobj->translate.vec.f.x = pos->x + 5.0F;
        dobj->translate.vec.f.y = pos->y + 15.0F;
    }
    else
    {
        dobj->translate.vec.f.x = pos->x + 15.0F;
        dobj->translate.vec.f.y = pos->y + 5.0F;
    }
    gcAddAnimAll(gobj, NULL, lbRelocGetFileData(AObjEvent32***, sSCExplainGraphicsFileHead, &llSCExplainGraphicsTapSparkMatAnimJoint), 0.0F);
    gcPlayAnimAll(gobj);

    gobj->flags = GOBJ_FLAG_NONE;
}

// 0x8018D9B4
void scExplainTapSparkProcUpdate(GObj *gobj)
{
    gcPlayAnimAll(gobj);

    if (DObjGetStruct(gobj)->mobj->anim_wait == AOBJ_ANIM_NULL)
    {
        gobj->flags = GOBJ_FLAG_HIDDEN;
    }
}

// 0x8018DA04
GObj* scExplainMakeTapSpark(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(interface_gobj, scExplainTapSparkProcDisplay, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddDObjForGObj(interface_gobj, (void*) ((uintptr_t)sSCExplainGraphicsFileHead + (intptr_t)&llSCExplainGraphicsTapSparkDisplayList));
    gcAddXObjForDObjFixed(DObjGetStruct(interface_gobj), nGCMatrixKindTra, 0);
    gcAddMObjAll(interface_gobj, lbRelocGetFileData(MObjSub***, sSCExplainGraphicsFileHead, &llSCExplainGraphicsTapSparkMObjSub));
    gcAddGObjProcess(interface_gobj, scExplainTapSparkProcUpdate, nGCProcessKindFunc, 5);

    interface_gobj->flags = GOBJ_FLAG_HIDDEN;

    sSCExplainStruct.stick_status = 0;

    return interface_gobj;
}

// 0x8018DACC
void scExplainSpecialMoveRGBProcUpdate(void)
{
    s32 sw = sSCExplainPhase->rgb_overlay_args.sprite_status;
    GObj *gobj = sSCExplainStruct.rgb_gobj;

    // Check to apply trarnsparent RGB color overlay over control stick sprite when explaining B-button moves

    if (sw == 1)
    {
        DObj *dobj = DObjGetStruct(gobj);

        dobj->translate.vec.f.x = (sSCExplainPhase->control_stick_args.sprite_pos_x - 0x96);
        dobj->translate.vec.f.y = (-0x28 - sSCExplainPhase->control_stick_args.sprite_pos_y);

        gobj->flags = GOBJ_FLAG_NONE;
    }
    else gobj->flags = GOBJ_FLAG_HIDDEN;
}

// 0x8018DB44
GObj* scExplainMakeSpecialMoveRGB(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter
    (
        nGCCommonKindInterface, 
        NULL,
        nGCCommonLinkIDInterface,
        GOBJ_PRIORITY_DEFAULT
    );
    gcAddGObjDisplay(interface_gobj, scExplainTapSparkProcDisplay, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddDObjForGObj(interface_gobj, lbRelocGetFileData(void*, sSCExplainGraphicsFileHead, &llSCExplainGraphicsSpecialMoveRGBDisplayList));
    gcAddXObjForDObjFixed(DObjGetStruct(interface_gobj), nGCMatrixKindTra, 0);

    interface_gobj->flags = GOBJ_FLAG_HIDDEN;

    return interface_gobj;
}

// 0x8018DBD0
void scExplainSetInterfaceGObjs(void)
{
    sSCExplainStruct.stick_gobj = scExplainMakeControlStickInterface();
    sSCExplainStruct.spark_gobj = scExplainMakeTapSpark();
    sSCExplainStruct.rgb_gobj   = scExplainMakeSpecialMoveRGB();
}

// 0x8018DC0C
SObj* scExplainMakeSObjOffset(intptr_t offset)
{
    GObj *interface_gobj;
    SObj *sobj;

    interface_gobj = gcMakeGObjSPAfter
    (
        nGCCommonKindInterface,
        NULL,
        nGCCommonLinkIDInterface,
        GOBJ_PRIORITY_DEFAULT
    );
    gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);

    sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, sSCExplainGraphicsFileHead, offset));

    sobj->sprite.attr |= SP_HIDDEN;

    return sobj;
}

// 0x8018DC84
void scExplainSetPhaseSObjs(void)
{
    sSCExplainStruct.textbox_sobj = scExplainMakeSObjOffset(&llSCExplainGraphicsTapTheStickSprite);
    sSCExplainStruct.phase_sobj0 = scExplainMakeSObjOffset(&llSCExplainGraphicsAButtonSprite);
    sSCExplainStruct.phase_sobj1 = scExplainMakeSObjOffset(&llSCExplainGraphicsBButtonSprite);
    sSCExplainStruct.phase_sobj2 = scExplainMakeSObjOffset(&llSCExplainGraphicsZButtonSprite);
    sSCExplainStruct.phase_sobj3 = scExplainMakeSObjOffset(&llSCExplainGraphicsHereTextSprite);
    sSCExplainStruct.phase_sobj4 = scExplainMakeSObjOffset(&llSCExplainGraphicsPlusSymbolSprite);
    sSCExplainStruct.phase_sobj5 = scExplainMakeSObjOffset(&llSCExplainGraphicsPlusSymbolSprite);
}

// 0x8018DD18
void scExplainUpdateTextBoxSprite(void)
{
    sSCExplainStruct.textbox_sobj->sprite = *sSCExplainPhase->sprite;

    sSCExplainStruct.textbox_sobj->pos.x = sSCExplainPhase->textbox_pos_x + 10;
    sSCExplainStruct.textbox_sobj->pos.y = sSCExplainPhase->textbox_pos_y + 160;

    sSCExplainStruct.textbox_sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;
}

// 0x8018DDBC
void func_ovl63_8018DDBC(void)
{
    s32 sw = sSCExplainPhase->control_stick_args.sprite_status;
    GObj *stick_gobj = sSCExplainStruct.stick_gobj;

    if (sw == 0)
    {
        stick_gobj->flags = GOBJ_FLAG_HIDDEN;
    }
    else
    {
        DObj *dobj = DObjGetStruct(stick_gobj);

        dobj->translate.vec.f.x = sSCExplainPhase->control_stick_args.sprite_pos_x - 150;
        dobj->translate.vec.f.y = -40 - sSCExplainPhase->control_stick_args.sprite_pos_y;

        DObjGetStruct(stick_gobj)->child->flags = (sw == 2) ? DOBJ_FLAG_NONE : DOBJ_FLAG_HIDDEN;

        gcAddAnimAll
        (
            stick_gobj,
            NULL,
            lbRelocGetFileData(AObjEvent32***, sSCExplainGraphicsFileHead, dSCExplainStickMatAnimJoints[sw]), 
            0.0F
        );
        gcPlayAnimAll(stick_gobj);

        stick_gobj->flags = GOBJ_FLAG_NONE;
    }
    sSCExplainStruct.stick_status = sw;
}

// 0x8018DEA0
void scExplainHideTapSpark(void)
{
    sSCExplainStruct.spark_gobj->flags = GOBJ_FLAG_HIDDEN;
}

// 0x8018DEB4
void scExplainUpdateArgsSObj(SCExplainArgs *args, SObj *sobj)
{
    if (args->sprite_status == 1)
    {
        sobj->sprite.attr &= ~SP_HIDDEN;

        sobj->pos.x = args->sprite_pos_x + 10;
        sobj->pos.y = args->sprite_pos_y + 160;
    }
    else sobj->sprite.attr |= SP_HIDDEN;
}

// 0x8018DF18
void scExplainDetectExit(void)
{
    s32 player;

    for (player = 0; player < ARRAY_COUNT(gSYControllerDevices); player++)
    {
        u16 button_tap = gSYControllerDevices[player].button_tap;

        if (button_tap & (A_BUTTON | B_BUTTON | START_BUTTON))
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();

            break;
        }
    }
}

// 0x8018DF80
void scExplainTryMakeFireFlower(void)
{
    Vec3f pos;
    Vec3f vel;

    if (sSCExplainStruct.phase == 16)
    {
        pos.x = -1400.0F;
        pos.y = 1500.0F;
        pos.z = 0.0F;

        vel.x = vel.z = 0.0F;
        vel.y = 10.0F;

        itManagerMakeItemSetupCommon(NULL, nITKindFFlower, &pos, &vel, ITEM_FLAG_PARENT_DEFAULT);
    }
}

// 0x8018DFF8
void scExplainUpdatePhase(void)
{
    if (sSCExplainStruct.phase_advance_wait == 0)
    {
        scExplainTryMakeFireFlower();

        sSCExplainStruct.phase++;

        if (sSCExplainStruct.phase > 22)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindCharacters;

            syTaskmanSetLoadScene();

            return;
        }
        else
        {
            scExplainUpdateTextBoxSprite();
            func_ovl63_8018DDBC();
            scExplainHideTapSpark();
            scExplainSpecialMoveRGBProcUpdate();
            scExplainUpdateArgsSObj(&sSCExplainPhase->phase_args0, sSCExplainStruct.phase_sobj0);
            scExplainUpdateArgsSObj(&sSCExplainPhase->phase_args1, sSCExplainStruct.phase_sobj1);
            scExplainUpdateArgsSObj(&sSCExplainPhase->phase_args2, sSCExplainStruct.phase_sobj2);
            scExplainUpdateArgsSObj(&sSCExplainPhase->phase_args3, sSCExplainStruct.phase_sobj3);
            scExplainUpdateArgsSObj(&sSCExplainPhase->phase_args4, sSCExplainStruct.phase_sobj4);
            scExplainUpdateArgsSObj(&sSCExplainPhase->phase_args5, sSCExplainStruct.phase_sobj5);

            sSCExplainStruct.phase_advance_wait = sSCExplainPhase->phase_time;

            sSCExplainPhase++;
        }
    }
    sSCExplainStruct.phase_advance_wait--;
}

// 0x8018E114
void scExplainSceneInterfaceProcUpdate(GObj *gobj)
{
    scExplainDetectExit();
    scExplainUpdatePhase();
}

// 0x8018E13C
GObj* scExplainMakeSceneInterface(void)
{
    GObj *interface_gobj = gcMakeGObjSPAfter
    (
        nGCCommonKindInterface, 
        NULL, 
        nGCCommonLinkIDInterface, 
        GOBJ_PRIORITY_DEFAULT
    );
    gcAddGObjProcess(interface_gobj, scExplainSceneInterfaceProcUpdate, nGCProcessKindFunc, 5);

    sSCExplainStruct.phase_advance_wait = 0;
    sSCExplainStruct.phase = 0;

    scExplainSceneInterfaceProcUpdate(interface_gobj);

    return interface_gobj;
}

// 0x8018E1A0
void scExplainFuncStart(void)
{
    s32 unused[3];
    GObj *fighter_gobj;
    s32 player;
    FTDesc desc;
    SYColorRGBA color;

    scExplainSetBattleState();
    scExplainSetupFiles();
    scExplainLoadExplainFiles();
    gmCameraSetViewportDimensions(10, 10, 310, 160);
    scExplainMakeCamera();
    efParticleInitAll();
    ftParamInitGame();
    mpCollisionInitGroundData();
    gmCameraMakeWallpaperCamera();
    grWallpaperMakeDecideKind();
    gmCameraMakeBattleCamera();
    itManagerInitItems();
    grCommonSetupInitAll();
    ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION, 2);
    wpManagerAllocWeapons();
    efManagerInitEffects();
    ifScreenFlashMakeInterface(0xFF);
    gmRumbleMakeActor();
    ftPublicMakeActor();

    for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
    {
        desc = dFTManagerDefaultFighterDesc;

        if (gSCManagerBattleState->players[player].pkind == nFTPlayerKindNot)
        {
            continue;
        }
        ftManagerSetupFilesAllKind(gSCManagerBattleState->players[player].fkind);

        desc.fkind = gSCManagerBattleState->players[player].fkind;

        mpCollisionGetPlayerMapObjPosition(player, &desc.pos);

        desc.lr = (desc.pos.x >= 0.0F) ? -1 : +1;

        desc.team = gSCManagerBattleState->players[player].team;

        desc.player = player;

        desc.detail = ((gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count) < 3) ? nFTPartsDetailHigh : nFTPartsDetailLow;

        desc.costume = gSCManagerBattleState->players[player].costume;

        desc.handicap = gSCManagerBattleState->players[player].handicap;

        desc.level = gSCManagerBattleState->players[player].level;

        desc.stock_count = gSCManagerBattleState->stocks;

        desc.damage = 0;

        desc.pkind = gSCManagerBattleState->players[player].pkind;

        desc.controller = &gSYControllerDevices[player];

        desc.figatree_heap = ftManagerAllocFigatreeHeapKind(gSCManagerBattleState->players[player].fkind);

        fighter_gobj = ftManagerMakeFighter(&desc);

        gSCManagerBattleState->players[player].color = player;
        gSCManagerBattleState->players[player].tag = player;

        ftParamInitPlayerBattleStats(player, fighter_gobj);

        ftParamSetKey
        (
            fighter_gobj,
            lbRelocGetFileData
            (
                FTKeyEvent*,
                sSCExplainMainFileHead,
                dSCExplainKeyKeyEventss[player]
            )
        );
    }
    scExplainStartBattle();
    gmCameraMakePlayerArrowsCamera();
    ifCommonPlayerArrowsInitInterface();
    gmCameraMakePlayerMagnifyCamera();
    ifCommonPlayerMagnifyMakeInterface();

    gIFCommonPlayerInterface.is_magnify_display = TRUE;

    gmCameraScreenFlashMakeCamera();
    gmCameraMakeInterfaceCamera();
    gmCameraMakeEffectCamera();
    ifCommonPlayerTagMakeInterface();
    scExplainSetPlayerInterfacePositions();
    ifCommonPlayerDamageInitInterface();
    ifCommonPlayerDamageSetShowInterface();
    ifCommonPlayerStockInitInterface();
    scExplainMakeWindowCamera();
    scExplainMakeTextCamera();
    scExplainMakeControlStickCamera();
    scExplainSetInterfaceGObjs();
    scExplainSetPhaseSObjs();
    scExplainMakeSceneInterface();
    mpCollisionSetPlayBGM();
    func_800269C0_275C0(nSYAudioVoiceAnnounceHowToPlay);

    color = dSCExplainFadeColor;

    lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 12, TRUE, NULL);
}

// 0x8018E51C
void scExplainFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);

    ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E568
void scExplainFuncUpdate(void)
{
    syUtilsSetRandomSeedPtr(&dSCExplainRandomSeed1);
    gcRunAll();
}

// 0x8018E594
void scExplainFuncDraw(void)
{
    syUtilsSetRandomSeedPtr(&dSCExplainRandomSeed2);
    scManagerFuncDraw();
}

// 0x8018E5C0
void scExplainStartScene(void)
{
    dSCExplainVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);

    syVideoInit(&dSCExplainVideoSetup);

    syUtilsSetRandomSeedPtr(&dSCExplainRandomSeed1);

    dSCExplainTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl63_BSS_END);
    dSCExplainTaskmanSetup.func_start = scExplainFuncStart;

    scManagerFuncUpdate(&dSCExplainTaskmanSetup);
    gmRumbleInitPlayers();
    syUtilsSetRandomSeedPtr(NULL);
}
