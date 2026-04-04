#include <ft/fighter.h>
#include <mv/movie.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern void syTaskmanSetLoadScene();
extern u32 sySchedulerGetTicCount();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801328D0
u32 dMVOpeningStandoffFileIDs[/* */] = { &llMVOpeningStandoffFileID, &llMVOpeningStandoffWallpaperFileID };

// 0x801328D8
Lights1 dMVOpeningStandoffLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x801328F0
Lights1 dMVOpeningStandoffLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// 0x80132908
SYVideoSetup dMVOpeningStandoffVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132924
SYTaskmanSetup dMVOpeningStandoffTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                      // Frame draw function
        &ovl47_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        2,                              // Number of contexts?
        sizeof(Gfx) * 1250,             // Display List Buffer 0 Size
        sizeof(Gfx) * 512,              // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x8000,                         // Graphics Heap Size
        2,                              // ???
        0xC000,                         // RDP Output Buffer Size
        mvOpeningStandoffFuncLights,    // Pre-render function
        syControllerFuncRead,           // Controller I/O function
    },

    8,                                  // Number of GObjThreads
    sizeof(u64) * 192,                  // Thread stack size
    8,                                  // Number of thread stacks
    0,                                  // ???
    128,                                // Number of GObjProcesses
    128,                                // Number of GObjs
    sizeof(GObj),                       // GObj size
    512,                                // Number of XObjs
    dLBCommonFuncMatrixList,            // Matrix function list
    NULL,                               // DObjVec eject function
    512,                                // Number of AObjs
    160,                                // Number of MObjs
    512,                                // Number of DObjs
    sizeof(DObj),                       // DObj size
    128,                                // Number of SObjs
    sizeof(SObj),                       // SObj size
    16,                                 // Number of CObjs
    sizeof(CObj),                       // Camera size
    
    mvOpeningStandoffFuncStart          // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801329C0
s32 sMVOpeningStandoffPad0x801329C0[2];

// 0x801329C8
void *sMVOpeningStandoffMarioFigatreeHeap;

// 0x801329CC
void *sMVOpeningStandoffKirbyFigatreeHeap;

// 0x801329D0
s32 sMVOpeningStandoffTotalTimeTics;

// 0x801329D4
f32 sMVOpeningStandoffWallpaperScrollSpeed;

// 0x801329D8
s32 sMVOpeningStandoffPad0x801329D8;

// 0x801329DC
s32 sMVOpeningStandoffUnused0x801329DC;

// 0x801329E0
LBFileNode sMVOpeningStandoffStatusBuffer[48];

// 0x80132B60
LBFileNode sMVOpeningStandoffForceStatusBuffer[7];

// 0x80132B98
void *sMVOpeningStandoffFiles[ARRAY_COUNT(dMVOpeningStandoffFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvOpeningStandoffFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);
    ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void mvOpeningStandoffMakeGround(void)
{
    GObj *gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcAddDObjForGObj(gobj, lbRelocGetFileData(void*, sMVOpeningStandoffFiles[0], &llMVOpeningStandoffGroundDisplayList));
    gcAddXObjForDObjFixed(DObjGetStruct(gobj), nGCMatrixKindTraRotRpyRSca, 0);
    gcAddGObjDisplay(gobj, gcDrawDObjDLHead0, 26, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;
}

// 0x80131BF8 - Unused?
void func_ovl47_80131BF8()
{
    return;
}

// 0x80131C00
void mvOpeningStandoffMakeFighters(void)
{
    GObj* fighter_gobj;
    FTDesc desc = dFTManagerDefaultFighterDesc;

    desc.fkind = nFTKindMario;
    desc.costume = ftParamGetCostumeCommonID(nFTKindMario, 0);
    desc.pos.x = 0.0F;
    desc.pos.y = 0.0F;
    desc.pos.z = 0.0F;
    desc.figatree_heap = sMVOpeningStandoffMarioFigatreeHeap;

    fighter_gobj = ftManagerMakeFighter(&desc);

    scSubsysFighterSetStatus(fighter_gobj, 0x1000F);

    DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;

    desc.fkind = nFTKindKirby;
    desc.costume = ftParamGetCostumeCommonID(nFTKindKirby, 0);
    desc.pos.x = 0.0F;
    desc.pos.y = 0.0F;
    desc.pos.z = 0.0F;
    desc.figatree_heap = sMVOpeningStandoffKirbyFigatreeHeap;

    fighter_gobj = ftManagerMakeFighter(&desc);

    scSubsysFighterSetStatus(fighter_gobj, 0x1000F);

    DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x80131D38
void mvOpeningStandoffWallpaperProcUpdate(GObj* wallpaper_gobj)
{
    SObj* wallpaper_sobj = SObjGetStruct(wallpaper_gobj);

    switch (sMVOpeningStandoffTotalTimeTics)
    {
    case 1:
        sMVOpeningStandoffWallpaperScrollSpeed = 2.0F;
        break;

    case 90:
        sMVOpeningStandoffWallpaperScrollSpeed = 8.0F;
        break;

    case 105:
        sMVOpeningStandoffWallpaperScrollSpeed = 2.0F;
        break;

    case 180:
        sMVOpeningStandoffWallpaperScrollSpeed = 8.0F;
        break;

    case 195:
        sMVOpeningStandoffWallpaperScrollSpeed = 2.0F;
        break;

    case 280:
        sMVOpeningStandoffWallpaperScrollSpeed = 2.0F;
        break;

    case 300:
        sMVOpeningStandoffWallpaperScrollSpeed = 7.0F;
        break;
    }
    if ((sMVOpeningStandoffTotalTimeTics >= 281) && (sMVOpeningStandoffTotalTimeTics < 300))
    {
        sMVOpeningStandoffWallpaperScrollSpeed += -0.05F;
    }

    if ((sMVOpeningStandoffTotalTimeTics >= 301) && (sMVOpeningStandoffTotalTimeTics < 320))
    {
        sMVOpeningStandoffWallpaperScrollSpeed += 0.4F;
    }

    if (sMVOpeningStandoffTotalTimeTics >= 301)
    {
        wallpaper_sobj->pos.y += sMVOpeningStandoffWallpaperScrollSpeed;
    }
    else
    {
        wallpaper_sobj->pos.x += sMVOpeningStandoffWallpaperScrollSpeed;

        if (wallpaper_sobj->pos.x > 320.0F)
        {
            wallpaper_sobj->pos.x -= 320.0F;
        }
    }
    if
    (
        ((sMVOpeningStandoffTotalTimeTics >  90) && (sMVOpeningStandoffTotalTimeTics < 105)) ||
        ((sMVOpeningStandoffTotalTimeTics > 180) && (sMVOpeningStandoffTotalTimeTics < 195))
    )
    {
        wallpaper_sobj->next->pos.x = wallpaper_sobj->pos.x - 640.0F;
        wallpaper_sobj->next->pos.y = wallpaper_sobj->pos.y;

        wallpaper_sobj->sprite.scalex = 4.0F;
        wallpaper_sobj->sprite.scaley = 4.0F;

        wallpaper_sobj->next->sprite.scalex = 4.0F;
        wallpaper_sobj->next->sprite.scaley = 4.0F;

        wallpaper_sobj->pos.y = -240.0F;
    }
    else
    {
        wallpaper_sobj->next->pos.x = wallpaper_sobj->pos.x - 320.0F;
        wallpaper_sobj->next->pos.y = wallpaper_sobj->pos.y;

        wallpaper_sobj->sprite.scalex = 2.0F;
        wallpaper_sobj->sprite.scaley = 2.0F;

        wallpaper_sobj->next->sprite.scalex = 2.0F;
        wallpaper_sobj->next->sprite.scaley = 2.0F;

        if (sMVOpeningStandoffTotalTimeTics < 300)
        {
            wallpaper_sobj->pos.y = 0.0F;
        }
    }
}

// 0x80131FC8
void mvOpeningStandoffMakeWallpaper(void)
{
    GObj* wallpaper_gobj;
    SObj* wallpaper_sobj;

    wallpaper_gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(wallpaper_gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(wallpaper_gobj, mvOpeningStandoffWallpaperProcUpdate, nGCProcessKindFunc, 1);

    wallpaper_sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, lbRelocGetFileData(Sprite*, sMVOpeningStandoffFiles[1], &llMVOpeningStandoffWallpaperSprite));
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;

    wallpaper_sobj->sprite.scalex = 2.0F;
    wallpaper_sobj->sprite.scaley = 2.0F;

    wallpaper_sobj->pos.x = 0.0F;
    wallpaper_sobj->pos.y = 0.0F;

    wallpaper_sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, lbRelocGetFileData(Sprite*, sMVOpeningStandoffFiles[1], &llMVOpeningStandoffWallpaperSprite));
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;

    wallpaper_sobj->sprite.scalex = 2.0F;
    wallpaper_sobj->sprite.scaley = 2.0F;

    wallpaper_sobj->pos.x = 320.0F;
    wallpaper_sobj->pos.y = 0.0F;
}

// 0x801320C0
void mvOpeningStandoffMakeLightning(void)
{
    GObj* lightning_gobj;

    lightning_gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcSetupCustomDObjs
    (
        lightning_gobj,
        lbRelocGetFileData
        (
            DObjDesc*,
            sMVOpeningStandoffFiles[0],
            &llMVOpeningStandoffLightningDObjDesc
        ),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddMObjAll
    (
        lightning_gobj,
        lbRelocGetFileData
        (
            MObjSub***,
            sMVOpeningStandoffFiles[0],
            &llMVOpeningStandoffLightningMObjSub
        )
    );
    gcAddMatAnimJointAll
    (
        lightning_gobj,
        lbRelocGetFileData
        (
            AObjEvent32***,
            sMVOpeningStandoffFiles[0],
            &llMVOpeningStandoffLightningMatAnimJoint
        ),
        0.0F
    );
    gcAddGObjDisplay(lightning_gobj, gcDrawDObjTreeDLLinksForGObj, 26, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(lightning_gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(lightning_gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(lightning_gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll
    (
        lightning_gobj,
        lbRelocGetFileData
        (
            AObjEvent32**,
            sMVOpeningStandoffFiles[0],
            &llMVOpeningStandoffLightningAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(lightning_gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x801321D8
void mvOpeningStandoffLightningFlashProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetCycleType(gSYTaskmanDLHeads[1]++, G_CYC_1CYCLE);

    if
    (
        ((sMVOpeningStandoffTotalTimeTics >  19) && (sMVOpeningStandoffTotalTimeTics <  23)) ||
        ((sMVOpeningStandoffTotalTimeTics > 149) && (sMVOpeningStandoffTotalTimeTics < 153)) ||
        ((sMVOpeningStandoffTotalTimeTics > 260) && (sMVOpeningStandoffTotalTimeTics < 264))
    )
    {
        gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, 0xFF, 0xFF, 0xFF, 0x40);
    }
    else gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, 0xFF, 0xFF, 0xFF, 0x00);

    gDPSetCombineLERP(gSYTaskmanDLHeads[1]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[1]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80132330 - Unused?
void func_ovl47_80132330()
{
    return;
}

// 0x80132338
void mvOpeningStandoffMakeLightningFlash(void)
{
    gcAddGObjDisplay
    (
        gcMakeGObjSPAfter
        (
            0,
            NULL,
            18,
            GOBJ_PRIORITY_DEFAULT
        ),
        mvOpeningStandoffLightningFlashProcDisplay,
        28,
        GOBJ_PRIORITY_DEFAULT,
        ~0
    );
}

// 0x80132384
void mvOpeningStandoffMakeLightningFlashCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindSceneCamera,
            NULL,
            16,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            20,
            COBJ_MASK_DLLINK(28),
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

// 0x80132424 - Unused?
void func_ovl147_80132424()
{
    return;
}

// 0x8013242C
void mvOpeningStandoffMakeMainCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        80,
        COBJ_MASK_DLLINK(26) |
        COBJ_MASK_DLLINK(9),
        -1,
        TRUE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;

    gcAddCObjCamAnimJoint
    (
        cobj,
        lbRelocGetFileData
        (
            AObjEvent32*,
            sMVOpeningStandoffFiles[0],
            &llMVOpeningStandoffCamAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x80132530
void mvOpeningStandoffMakeWallpaperCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindSceneCamera,
            NULL,
            16,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            90,
            COBJ_MASK_DLLINK(27),
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

// 0x801325D0
void mvOpeningStandoffInitTotalTimeTics(void)
{
    sMVOpeningStandoffTotalTimeTics = 0;
}

// 0x801325DC
void mvOpeningStandoffFuncRun(GObj *gobj)
{
    sMVOpeningStandoffTotalTimeTics++;

    if (sMVOpeningStandoffTotalTimeTics >= 10)
    {
        if (sMVOpeningStandoffUnused0x801329DC != 0)
        {
            sMVOpeningStandoffUnused0x801329DC--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE)
        )
        {
            sMVOpeningStandoffUnused0x801329DC = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();
        }
        if (sMVOpeningStandoffTotalTimeTics == 320)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindOpeningClash;

            syTaskmanSetLoadScene();
        }
    }
}

// 0x801326A8
void mvOpeningStandoffFuncStart(void)
{
    s32 unused;
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVOpeningStandoffStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningStandoffStatusBuffer);
    rl_setup.force_status_buffer = sMVOpeningStandoffForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningStandoffForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVOpeningStandoffFileIDs, sMVOpeningStandoffFiles);
    gcMakeGObjSPAfter(0, mvOpeningStandoffFuncRun, 0, GOBJ_PRIORITY_DEFAULT);

    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

    efParticleInitAll();
    mvOpeningStandoffInitTotalTimeTics();
    efManagerInitEffects();
    ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, 2);
    ftManagerSetupFilesAllKind(nFTKindMario);
    ftManagerSetupFilesAllKind(nFTKindKirby);

    sMVOpeningStandoffMarioFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);
    sMVOpeningStandoffKirbyFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

    mvOpeningStandoffMakeMainCamera();
    mvOpeningStandoffMakeWallpaperCamera();
    mvOpeningStandoffMakeLightningFlashCamera();
    mvOpeningStandoffMakeWallpaper();
    mvOpeningStandoffMakeFighters();
    mvOpeningStandoffMakeGround();
    mvOpeningStandoffMakeLightning();
    mvOpeningStandoffMakeLightningFlash();

    scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);

    while (sySchedulerGetTicCount() < 3610)
    {
        continue;
    }
}

// 0x8013286C
void mvOpeningStandoffStartScene(void)
{
    dMVOpeningStandoffVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMVOpeningStandoffVideoSetup);

    dMVOpeningStandoffTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl47_BSS_END);
    syTaskmanStartTask(&dMVOpeningStandoffTaskmanSetup);
}
