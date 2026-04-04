#include <mn/menu.h>
#include <sc/scene.h>
#include <gm/gmsound.h>
#include <sys/controller.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132830
u32 dMNScreenAdjustFileIDs[/* */] = { &llMNScreenAdjustFileID };

// 0x80132838
Lights1 dMNScreenAdjustLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x80132850
Gfx dMNScreenAdjustDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dMNScreenAdjustLights1),
    gsSPEndDisplayList()
};

// 0x80132878
SYVideoSetup dMNScreenAdjustVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132894
SYTaskmanSetup dMNScreenAdjustTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,                  // Frame draw function
        &ovl25_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2500,         // Display List Buffer 0 Size
        sizeof(Gfx) * 128,          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        mnScreenAdjustFuncLights,   // Pre-render function
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
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    mnScreenAdjustFuncStart         // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132920
s32 sMNScreenAdjustPad0x80132920[2];

// 0x80132928
f32 sMNScreenAdjustOffsetH;

// 0x8013292C
f32 sMNScreenAdjustOffsetV;

// 0x80132930
s32 sMNScreenAdjustButtonHoldWait;

// 0x80132934
s32 sMNScreenAdjustTotalTimeTics;

// 0x80132938
s32 sMNScreenAdjustReturnTic;

// 0x80132940
LBFileNode sMNScreenAdjustForceStatusBuffer[7];

// 0x80132978
LBFileNode sMNScreenAdjustStatusBuffer[24];

// 0x80132A38
void *sMNScreenAdjustFiles[ARRAY_COUNT(dMNScreenAdjustFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnScreenAdjustFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dMNScreenAdjustDisplayList);
}

// 0x80131B24
void mnScreenAdjustFrameProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xBF, 0xA4, 0x47, 0xFF);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 159, 0, 161, 254);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 0, 119, 334, 121);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x8B, 0x8B, 0x8B, 0xFF);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 44, 44, 276, 45);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 44, 196, 276, 197);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 44, 44, 45, 196);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 276, 44, 277, 196);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80131D00
void mnScreenAdjustMakeFrame(void)
{
    GObj *gobj = gcMakeGObjSPAfter(0, NULL, 2, GOBJ_PRIORITY_DEFAULT);
    
    gcAddGObjDisplay(gobj, mnScreenAdjustFrameProcDisplay, 0, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x80131D4C
void mnScreenAdjustMakeGuide(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNScreenAdjustFiles[0], &llMNScreenAdjustGuideSprite));
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
}

// 0x80131DCC
void mnScreenAdjustMakeInstruction(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNScreenAdjustFiles[0], &llMNScreenAdjustInstructionSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
    
#if defined(REGION_US)
    sobj->pos.x = 16.0F;
#else
    sobj->pos.x = 41.0F;
#endif
    sobj->pos.y = 198.0F;
}

// 0x80131E74
void mnScreenAdjustMakeFrameCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            40,
            COBJ_MASK_DLLINK(0),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x80131F14
void mnScreenAdjustMakeSpriteCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            70,
            COBJ_MASK_DLLINK(1),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x80131FB4
void mnScreenAdjustApplyCenterOffsets(s16 h, s16 v)
{
    syVideoSetCenterOffsets(h, h, v, v);
}

// 0x80131FF8
void mnScreenAdjustInitVars(void)
{
    sMNScreenAdjustOffsetH = gSYVideoOffsetLeft;
    sMNScreenAdjustOffsetV = gSYVideoOffsetTop;
    
    sMNScreenAdjustButtonHoldWait = 0;
    sMNScreenAdjustTotalTimeTics = 0;
    
    sMNScreenAdjustReturnTic = sMNScreenAdjustTotalTimeTics + I_MIN_TO_TICS(5);
}

// 0x8013204C
void mnScreenAdjustBackupOffsets(void)
{
    gSCManagerBackupData.screen_adjust_h = sMNScreenAdjustOffsetH;
    gSCManagerBackupData.screen_adjust_v = sMNScreenAdjustOffsetV;
    
    lbBackupWrite();
}

// 0x8013209C
void mnScreenAdjustFuncRun(GObj *gobj)
{
    s32 stick_range;

    sMNScreenAdjustTotalTimeTics++;
    
    if (sMNScreenAdjustTotalTimeTics >= 10)
    {
        if (sMNScreenAdjustButtonHoldWait != 0)
        {
            sMNScreenAdjustButtonHoldWait--;
        }
        if ((scSubsysControllerGetPlayerStickInRangeLR(-30, 30) != FALSE) && (scSubsysControllerGetPlayerStickInRangeUD(-30, 30) != FALSE))
        {
            sMNScreenAdjustButtonHoldWait = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            func_800269C0_275C0(nSYAudioFGMMenuSelect);
            
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindOption;

            mnScreenAdjustBackupOffsets();
            syTaskmanSetLoadScene();
        }
        if (scSubsysControllerGetPlayerTapButtons(U_JPAD | U_CBUTTONS) != FALSE)
        {
            if (sMNScreenAdjustOffsetV > -14.0F)
            {
                sMNScreenAdjustOffsetV--;
            
                if (sMNScreenAdjustOffsetV < -14.0F)
                {
                    sMNScreenAdjustOffsetV = -14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (sMNScreenAdjustButtonHoldWait == 0)
        {
            stick_range = scSubsysControllerGetPlayerStickUD(30, 1);
            
            if ((stick_range != 0) && (sMNScreenAdjustOffsetV > -14.0F))
            {
                sMNScreenAdjustOffsetV -= stick_range / 50.0F;
                
                if (sMNScreenAdjustOffsetV < -14.0F)
                {
                    sMNScreenAdjustOffsetV = -14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (scSubsysControllerGetPlayerTapButtons(D_JPAD | D_CBUTTONS) != FALSE)
        {
            if (sMNScreenAdjustOffsetV < 14.0F)
            {
                sMNScreenAdjustOffsetV++;
            
                if (sMNScreenAdjustOffsetV > 14.0F)
                {
                    sMNScreenAdjustOffsetV = 14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (sMNScreenAdjustButtonHoldWait == 0)
        {
            stick_range = scSubsysControllerGetPlayerStickUD(-30, 0);
            
            if ((stick_range != 0) && (sMNScreenAdjustOffsetV < 14.0F))
            {
                sMNScreenAdjustOffsetV -= stick_range / 50.0F;
                
                if (sMNScreenAdjustOffsetV > 14.0F)
                {
                    sMNScreenAdjustOffsetV = 14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (scSubsysControllerGetPlayerTapButtons(L_JPAD | L_TRIG | L_CBUTTONS) != FALSE)
        {
            if (sMNScreenAdjustOffsetH > -14.0F)
            {
                sMNScreenAdjustOffsetH--;
            
                if (sMNScreenAdjustOffsetH < -14.0F)
                {
                    sMNScreenAdjustOffsetH = -14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (sMNScreenAdjustButtonHoldWait == 0)
        {
            stick_range = scSubsysControllerGetPlayerStickLR(-30, 0);
            
            if ((stick_range != 0) && (sMNScreenAdjustOffsetH > -14.0F))
            {
                sMNScreenAdjustOffsetH += stick_range / 50.0F;
                
                if (sMNScreenAdjustOffsetH < -14.0F)
                {
                    sMNScreenAdjustOffsetH = -14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (scSubsysControllerGetPlayerTapButtons(R_JPAD | R_TRIG | R_CBUTTONS) != FALSE)
        {
            if (sMNScreenAdjustOffsetH < 14.0F)
            {
                sMNScreenAdjustOffsetH++;
            
                if (sMNScreenAdjustOffsetH > 14.0F)
                {
                    sMNScreenAdjustOffsetH = 14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        if (sMNScreenAdjustButtonHoldWait == 0)
        {
            stick_range = scSubsysControllerGetPlayerStickLR(30, 1);
            
            if ((stick_range != 0) && (sMNScreenAdjustOffsetH < 14.0F))
            {
                sMNScreenAdjustOffsetH += stick_range / 50.0F;
                
                if (sMNScreenAdjustOffsetH > 14.0F)
                {
                    sMNScreenAdjustOffsetH = 14.0F;
                }
                mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
            }
        }
        sMNScreenAdjustButtonHoldWait = 0;
        
        if (scSubsysControllerGetPlayerTapButtons(Z_TRIG) != FALSE)
        {
            sMNScreenAdjustOffsetH = 0.0F;
            sMNScreenAdjustOffsetV = 0.0F;
            
            mnScreenAdjustApplyCenterOffsets(sMNScreenAdjustOffsetH, sMNScreenAdjustOffsetV);
        }
    }
}

// 0x801326CC
void mnScreenAdjustFuncStart(void)
{
    LBRelocSetup rl_setup;
    s32 unused[2];

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMNScreenAdjustStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMNScreenAdjustStatusBuffer);
    rl_setup.force_status_buffer = sMNScreenAdjustForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMNScreenAdjustForceStatusBuffer);
    
    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMNScreenAdjustFileIDs, sMNScreenAdjustFiles);
    gcMakeGObjSPAfter(0, mnScreenAdjustFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
    mnScreenAdjustInitVars();
    mnScreenAdjustMakeFrameCamera();
    mnScreenAdjustMakeSpriteCamera();
    mnScreenAdjustMakeGuide();
    mnScreenAdjustMakeFrame();
    mnScreenAdjustMakeInstruction();
    syAudioStopBGMAll();
}

// 0x801327D8
void mnScreenAdjustStartScene(void)
{
    dMNScreenAdjustVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMNScreenAdjustVideoSetup);
    
    dMNScreenAdjustTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl25_BSS_END);
    syTaskmanStartTask(&dMNScreenAdjustTaskmanSetup);
}
