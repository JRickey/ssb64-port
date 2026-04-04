#include <ft/fighter.h>
#include <mn/menu.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132500
u32 dMNMessageFileIDs[/* */] = { &llMNCommonFileID, &llMNMessageFileID };

// 0x80132508
Lights1 dMNMessageLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x80132520
Gfx dMNMessageDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dMNMessageLights1),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132650
s32 sMNMessagePadd0x80132650[2];

// 0x80132658
s32 sMNMessageUnlockID;

// 0x8013265C
s32 sMNMessageQueueID;

// 0x80132660
s32 sMNMessageUnk0x80132660;

// 0x80132664
s32 sMNMessageTotalTimeTics;

// 0x80132668
LBFileNode sMNMessageStatusBuffer[100];

// 0x80132988
void *sMNMessageFiles[ARRAY_COUNT(dMNMessageFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnMessageFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dMNMessageDisplayList);
}

// 0x80131B24
void mnMessageMakeWallpaper(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 2, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNMessageFiles[0], &llMNCommonSmashBrosCollageSprite));
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
}

// 0x80131BA4
void mnMessageTintProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0xFF, 0x3F);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    
    lbCommonClearExternSpriteParams();
}

// 0x80131CB8
void mnMessageMakeTint(void)
{
    gcAddGObjDisplay(gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT), mnMessageTintProcDisplay, 2, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x80131D04
void mnMessageMakeExclaim(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 5, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 3, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNMessageFiles[1], &llMNMessageDecalExclaimSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 140.0F;
    sobj->pos.y = 62.0F;
}

// 0x80131D9C
void mnMessageMakeMessage(s32 message)
{
    GObj *gobj;
    SObj *sobj;
    
    // 0x80132548
    intptr_t message_offsets[/* */] =
    {
        &llMNMessageUnlockLuigiSprite,
        &llMNMessageUnlockNessSprite,
        &llMNMessageUnlockCaptainSprite,
        &llMNMessageUnlockPurinSprite,
        &llMNMessageUnlockInishieSprite,
        &llMNMessageUnlockSoundTestSprite,
        &llMNMessageUnlockItemSwitchSprite
    };

    // 0x80132564
    Vec2i message_pos[/* */] =
    {
#if defined(REGION_US)
        { 85, 114 },
        { 35, 123 },
        { 58, 114 },
        { 44, 114 },
        { 48, 123 },
        { 56, 114 },
        { 54, 114 }
#else
        { 28, 121 },
        { 46, 121 },
        { 44, 114 },
        { 24, 121 },
        { 68, 121 },
        { 49, 114 },
        { 43, 114 }
#endif
    };

    gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNMessageFiles[1], message_offsets[message]));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = message_pos[message].x;
    sobj->pos.y = message_pos[message].y;
}

// 0x80131EE8
void mnMessageMakeTintCamera(void)
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
            COBJ_MASK_DLLINK(2),
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

// 0x80131F88
void mnMessageMakeMessageCamera(void)
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

// 0x80132028
void mnMessageMakeWallpaperCamera(void)
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
            80,
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

// 0x801320C8
void mnMessageMakeExclaimCamera(void)
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
            60,
            COBJ_MASK_DLLINK(3),
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

// 0x80132168
void mnMessageInitVars(void)
{
    sMNMessageUnlockID = gSCManagerSceneData.unlock_messages[sMNMessageQueueID];
    gSCManagerSceneData.unlock_messages[sMNMessageQueueID] = nLBBackupUnlockEnumCount;
    
    sMNMessageUnk0x80132660 = 0;
    sMNMessageTotalTimeTics = 0;
}

// 0x801321A4
void mnMessageApplyUnlock(void)
{
    // 0x8013259C
    u8 fkinds[/* */] = { nFTKindLuigi, nFTKindNess, nFTKindCaptain, nFTKindPurin };
    
    gSCManagerBackupData.unlock_mask |= (1 << sMNMessageUnlockID);
    
    switch (sMNMessageUnlockID)
    {
    case nLBBackupUnlockLuigi:
    case nLBBackupUnlockNess:
    case nLBBackupUnlockCaptain:
    case nLBBackupUnlockPurin:
        gSCManagerBackupData.fighter_mask |= (1 << fkinds[sMNMessageUnlockID]);
        gSCManagerBackupData.characters_fkind = fkinds[sMNMessageUnlockID];
        break;
    }
    lbBackupWrite();
}

// 0x8013223C
void mnMessageFuncRun(GObj *gobj)
{
    sMNMessageTotalTimeTics++;
    
    if (sMNMessageTotalTimeTics >= 120)
    {
        if (sMNMessageUnk0x80132660 != 0)
        {
            sMNMessageUnk0x80132660--;
        }
        if ((scSubsysControllerGetPlayerStickInRangeLR(-30, 30) != FALSE) && (scSubsysControllerGetPlayerStickInRangeUD(-30, 30) != FALSE))
        {
            sMNMessageUnk0x80132660 = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            mnMessageApplyUnlock();
            syTaskmanSetLoadScene();
        }
    }
}

// 0x801322D4
void mnMessageFuncStart(void)
{
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMNMessageStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMNMessageStatusBuffer);
    rl_setup.force_status_buffer = NULL;
    rl_setup.force_status_buffer_size = 0;
    
    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMNMessageFileIDs, sMNMessageFiles);
    gcMakeGObjSPAfter(0, mnMessageFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, 0, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));

    mnMessageInitVars();
    mnMessageMakeWallpaperCamera();
    mnMessageMakeMessageCamera();
    mnMessageMakeTintCamera();
    mnMessageMakeExclaimCamera();
    mnMessageMakeWallpaper();
    mnMessageMakeTint();
    mnMessageMakeExclaim();
    mnMessageMakeMessage(sMNMessageUnlockID);

    syAudioPlayBGM(0, nSYAudioBGMMessage);
    func_800269C0_275C0(nSYAudioFGMDeadUpStar);
}

// 0x801325A0
SYVideoSetup dMNMessageVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x801325BC
SYTaskmanSetup dMNMessageTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,                  // Frame draw function
        &ovl22_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2500,         // Display List Buffer 0 Size
        0,                          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        mnMessageFuncLights,        // Pre-render function
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
    sizeof(CObj),                 	// Camera size
    
    mnMessageFuncStart              // Task start function
};

// 0x801323F8
void mnMessageStartScene(void)
{
    s32 i;

    dMNMessageVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    
    syVideoInit(&dMNMessageVideoSetup);

    dMNMessageTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl22_BSS_END);

    for
    (
        sMNMessageQueueID = 0;
        sMNMessageQueueID < nLBBackupUnlockEnumCount && gSCManagerSceneData.unlock_messages[sMNMessageQueueID] != nLBBackupUnlockEnumCount;
        sMNMessageQueueID++
    )
    {
        syTaskmanStartTask(&dMNMessageTaskmanSetup);

#if defined(REGION_JP)
        if (sc1PManagerCheckUnlockSoundTest() != FALSE)
        {
            for
            (
                i = sMNMessageQueueID + 1;
                i < nLBBackupUnlockEnumCount;
                i++
            )
            {
                if (gSCManagerSceneData.unlock_messages[i] == nLBBackupUnlockEnumCount)
                {
                    gSCManagerSceneData.unlock_messages[i] = nLBBackupUnlockSoundTest;
                    break;
                }
            }
        }
#endif
    }
    
    if (gSCManagerSceneData.scene_prev == nSCKindVSResults)
    {
        gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
        gSCManagerSceneData.scene_curr = nSCKindPlayersVS;
    }
    else
    {
        gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
#if defined(REGION_US)        
        gSCManagerSceneData.scene_curr = nSCKindStartup;
#else
        gSCManagerSceneData.scene_curr = nSCKindOpeningRoom;
#endif
    }
}
