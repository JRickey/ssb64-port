
#include <mn/menu.h>
#include <sc/scene.h>
#include <sys/controller.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x800D7010
u32 dMNUnusedFightersFileIDs[/* */] = { &llMNPlayersCommonFileID };

// 0x800D7018
Lights1 dMNUnusedFightersLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x14);

// 0x800D7030
Gfx dMNUnusedFightersDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dMNUnusedFightersLights1),
    gsSPEndDisplayList()
};

// 0x800D7058
SYVideoSetup dMNUnusedFightersVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x800D7074
SYTaskmanSetup dMNUnusedFightersTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                      // Frame draw function
        &ovl16_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        1,                              // Number of contexts?
        sizeof(Gfx) * 750,              // Display List Buffer 0 Size
        sizeof(Gfx) * 64,               // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x400,                          // Graphics Heap Size
        2,                              // ???
        0x1000,                         // RDP Output Buffer Size
        mnUnusedFightersFuncLights,     // Pre-render function
        syControllerFuncRead,           // Controller I/O function
    },

    32,                                 // Number of GObjThreads
    sizeof(u64) * 32,                   // Thread stack size
    32,                                 // Number of thread stacks
    0,                                  // ???
    32,                                 // Number of GObjProcesses
    32,                                 // Number of GObjs
    sizeof(GObj),                       // GObj size
    256,                                // Number of XObjs
    NULL,                               // Matrix function list
    NULL,                               // DObjVec eject function
    128,                                // Number of AObjs
    8,                                  // Number of MObjs
    64,                                 // Number of DObjs
    sizeof(DObj),                       // DObj size
    32,                                 // Number of SObjs
    sizeof(SObj),                       // SObj size
    8,                                  // Number of CObjs
    sizeof(CObj),                       // Camera size
    
    mnUnusedFightersFuncStart           // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D7110
s32 sMNUnusedFightersPad0x800D7110[2];

// 0x800D7118
LBFileNode sMNUnusedFightersStatusBuffer[10];

// 0x800D7168
void *sMNUnusedFightersFiles[ARRAY_COUNT(dMNUnusedFightersFileIDs)];

// 0x800D716C
f32 sMNUnusedFightersTimePositionX;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D6490
void mnUnusedFightersFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dMNUnusedFightersDisplayList);
}

// 0x800D64B4
void func_ovl16_800D64B4(void)
{
    return;
}

// 0x800D64BC
void func_ovl16_800D64BC(void)
{
    return;
}

// 0x800D64C4
void mnUnusedFightersTimeProcUpdate(GObj *gobj)
{
    if (gSYControllerMain.button_hold & L_JPAD)
    {
        sMNUnusedFightersTimePositionX -= 0.1F;
    }
    if (gSYControllerMain.button_hold & R_JPAD)
    {
        sMNUnusedFightersTimePositionX += 0.1F;
    }
    if (gSYControllerMain.button_hold & A_BUTTON)
    {
        SObjGetStruct(gobj)->pos.x = (s32) sMNUnusedFightersTimePositionX;
    }
    else SObjGetStruct(gobj)->pos.x = sMNUnusedFightersTimePositionX;
}

// 0x800D655C
void mnUnusedFightersFuncRun(GObj *gobj)
{
    if (gSYControllerMain.button_tap & START_BUTTON)
    {
        gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
        gSCManagerSceneData.scene_curr = nSCKindTitle;

        syTaskmanSetLoadScene();
    }
    if (gSYControllerMain.button_tap & Z_TRIG)
    {
        syDebugMakeMeterCamera(31, GOBJ_PRIORITY_DEFAULT, 0);
    }
}

// 0x800D65C4
void mnUnusedFightersFuncStart(void)
{
    s32 unused[2];
    LBRelocSetup rl_setup;
    GObj *gobj;
    CObj *cobj;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMNUnusedFightersStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMNUnusedFightersStatusBuffer);
    rl_setup.force_status_buffer = NULL;
    rl_setup.force_status_buffer_size = 0;
    
    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMNUnusedFightersFileIDs, sMNUnusedFightersFiles);
    gcMakeGObjSPAfter(0, mnUnusedFightersFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(1, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
    
    cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            50,
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
    
    cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            func_80017DBC,
            30,
            COBJ_MASK_DLLINK(2),
            -1,
            TRUE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
        
    cobj->vec.eye.x = 0.0F;
    cobj->vec.eye.y = 0.0F;
    cobj->vec.eye.z = 3000.0F;
    
    cobj->vec.at.x = 0.0F;
    cobj->vec.at.y = 0.0F;
    cobj->vec.at.z = 0.0F;
    
    cobj->vec.up.x = 0.0F;
    cobj->vec.up.y = 1.0F;
    cobj->vec.up.z = 0.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsMarioSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );

    SObjGetStruct(gobj)->pos.x = 25.0F;
    SObjGetStruct(gobj)->pos.y = 36.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsLuigiSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 70.0F;
    SObjGetStruct(gobj)->pos.y = 36.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsDonkeySprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 115.0F;
    SObjGetStruct(gobj)->pos.y = 36.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsSamusSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 160.0F;
    SObjGetStruct(gobj)->pos.y = 36.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsFoxSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 205.0F;
    SObjGetStruct(gobj)->pos.y = 36.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsKirbySprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 250.0F;
    SObjGetStruct(gobj)->pos.y = 36.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsLinkSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 25.0F;
    SObjGetStruct(gobj)->pos.y = 79.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsYoshiSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 70.0F;
    SObjGetStruct(gobj)->pos.y = 79.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersPortraitsPikachuSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 115.0F;
    SObjGetStruct(gobj)->pos.y = 79.0F;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersCommonMarioTextSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 27.0F;
    SObjGetStruct(gobj)->pos.y = 216.0F;

    SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
    SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersCommonSamusTextSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );

    SObjGetStruct(gobj)->pos.x = 95.0F;
    SObjGetStruct(gobj)->pos.y = 216.0F;

    SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
    SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersCommonPikachuTextSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 163.0F;
    SObjGetStruct(gobj)->pos.y = 216.0F;

    SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
    SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersCommonLuigiTextSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );

    SObjGetStruct(gobj)->pos.x = 231.0F;
    SObjGetStruct(gobj)->pos.y = 216.0F;

    SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
    SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersCommonTimeSelectorSprite),
        nGCProcessKindFunc,
        mnUnusedFightersTimeProcUpdate,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = sMNUnusedFightersTimePositionX = 145.0F;
    SObjGetStruct(gobj)->pos.y = 20.0F;
    
    SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
    
    gobj = gcMakeSpriteGObj
    (
        0,
        NULL,
        0,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        1,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        lbRelocGetFileData(Sprite*, sMNUnusedFightersFiles[0], &llMNPlayersCommonInfinityDarkSprite),
        nGCProcessKindFunc,
        NULL,
        1
    );
    
    SObjGetStruct(gobj)->pos.x = 182.0F;
    SObjGetStruct(gobj)->pos.y = 23.0F;
    
    SObjGetStruct(gobj)->envcolor.r = 0x32;
    SObjGetStruct(gobj)->envcolor.g = 0x1C;
    SObjGetStruct(gobj)->envcolor.b = 0x0E;
    
    SObjGetStruct(gobj)->sprite.red = 0xFF;
    SObjGetStruct(gobj)->sprite.green = 0xFF;
    SObjGetStruct(gobj)->sprite.blue = 0xFF;
    
    SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
    SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
}

// 0x800D6FB0
void mnUnusedFightersStartScene(void)
{
    dMNUnusedFightersVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMNUnusedFightersVideoSetup);

    dMNUnusedFightersTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl16_BSS_END);
    syTaskmanStartTask(&dMNUnusedFightersTaskmanSetup);
}
