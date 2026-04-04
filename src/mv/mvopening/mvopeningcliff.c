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

// 0x801326D0
u32 dMVOpeningCliffFileIDs[/* */] = { &llMVOpeningCliffFileID, &llMVOpeningStandoffWallpaperFileID };

// 0x801326D8
Lights1 dMVOpeningCliffLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x801326F0
Lights1 dMVOpeningCliffLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// 0x80132708
SYVideoSetup dMVOpeningCliffVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132724
SYTaskmanSetup dMVOpeningCliffTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                  // Frame draw function
        &ovl46_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        2,                              // Number of contexts?
        sizeof(Gfx) * 5000,             // Display List Buffer 0 Size
        sizeof(Gfx) * 512,              // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x8000,                         // Graphics Heap Size
        2,                              // ???
        0xC000,                         // RDP Output Buffer Size
        mvOpeningCliffFuncLights,       // Pre-render function
        syControllerFuncRead,           // Controller I/O function
    },

    0,                                  // Number of GObjThreads
    sizeof(u64) * 192,                  // Thread stack size
    0,                                  // Number of thread stacks
    0,                                  // ???
    0,                                  // Number of GObjProcesses
    0,                                  // Number of GObjs
    sizeof(GObj),                       // GObj size
    0,                                  // Number of XObjs
    dLBCommonFuncMatrixList,            // Matrix function list
    NULL,                               // DObjVec eject function
    0,                                  // Number of AObjs
    0,                                  // Number of MObjs
    0,                                  // Number of DObjs
    sizeof(DObj),                       // DObj size
    0,                                  // Number of SObjs
    sizeof(SObj),                       // SObj size
    0,                                  // Number of CObjs
    sizeof(CObj),                       // Camera size
    
    mvOpeningCliffFuncStart             // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801327C0
s32 sMVOpeningCliffPad0x801327C0[2];

// 0x801327C8
void *sMVOpeningCliffFigatreeHeap;

// 0x801327CC
s32 sMVOpeningCliffTotalTimeTics;

// 0x801327D0
GObj* sMVOpeningCliffOcarinaGObj;

// 0x801327D4
GObj* sMVOpeningCliffFighterGObj;

// 0x801327D8
f32 sMVOpeningCliffWallpaperScrollSpeed;

// 0x801327DC
s32 sMVOpeningCliffUnused0x801327DC;

// 0x801327E0
LBFileNode sMVOpeningCliffStatusBuffer[48];

// 0x80132960
LBFileNode sMVOpeningCliffForceStatusBuffer[7];

// 0x80132998
void *sMVOpeningCliffFiles[ARRAY_COUNT(dMVOpeningCliffFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvOpeningCliffFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);
    ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void mvOpeningCliffHillsProcDisplay(GObj *hills_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);

    gcDrawDObjTreeForGObj(hills_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80131C34
void mvOpeningCliffMakeHills(void)
{
    GObj* hills_gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcSetupCommonDObjs
    (
        hills_gobj,
        lbRelocGetFileData
        (
            DObjDesc*,
            sMVOpeningCliffFiles[0],
            &llMVOpeningCliffHillsDObjDesc
        ),
        NULL
    );
    gcAddGObjDisplay(hills_gobj, mvOpeningCliffHillsProcDisplay, 26, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x80131CA4 - Unused?
void func_ovl46_80131CA4()
{
    return;
}

// 0x80131CAC
void mvOpeningCliffMakeFighter(void)
{
    GObj* fighter_gobj;
    FTDesc desc = dFTManagerDefaultFighterDesc;

    desc.fkind = nFTKindLink;
    desc.costume = ftParamGetCostumeCommonID(nFTKindLink, 0);
    desc.pos.x = 0.0F;
    desc.pos.y = 0.0F;
    desc.pos.z = 0.0F;
    desc.figatree_heap = sMVOpeningCliffFigatreeHeap;

    sMVOpeningCliffFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

    scSubsysFighterSetStatus(fighter_gobj, 0x1000F);

    gcMoveGObjDL(fighter_gobj, 28, -1);

    DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x80131D8C
void mvOpeningCliffWallpaperProcDisplay(GObj *gobj)
{
    SObj *sobj = SObjGetStruct(gobj);

    switch (sMVOpeningCliffTotalTimeTics)
    {
    case 1:
        sMVOpeningCliffWallpaperScrollSpeed = 15.0F;
        break;

    case 80:
        sMVOpeningCliffWallpaperScrollSpeed = 10.0F;
        break;

    case 90:
        sMVOpeningCliffWallpaperScrollSpeed = 6.0F;
        break;

    case 120:
        sMVOpeningCliffWallpaperScrollSpeed = 2.0F;
        break;

    case 180:
        sMVOpeningCliffWallpaperScrollSpeed = 0.0F;
        break;
    }
    if ((sMVOpeningCliffTotalTimeTics > 1) && (sMVOpeningCliffTotalTimeTics < 80))
    {
        sMVOpeningCliffWallpaperScrollSpeed += (-5.0F / 79.0F);
    }
    if ((sMVOpeningCliffTotalTimeTics > 80) && (sMVOpeningCliffTotalTimeTics < 90))
    {
        sMVOpeningCliffWallpaperScrollSpeed += (-2.0F / 5.0F);
    }
    if ((sMVOpeningCliffTotalTimeTics > 90) && (sMVOpeningCliffTotalTimeTics < 120))
    {
        sMVOpeningCliffWallpaperScrollSpeed += (-2.0F / 15.0F);
    }
    if ((sMVOpeningCliffTotalTimeTics > 120) && (sMVOpeningCliffTotalTimeTics < 180))
    {
        sMVOpeningCliffWallpaperScrollSpeed += (-1.0F / 30.0F);
    }
    sobj->pos.x -= sMVOpeningCliffWallpaperScrollSpeed;

    if (sobj->pos.x < -320.0F)
    {
        sobj->pos.x += 320.0F;
    }
    sobj->next->pos.x = sobj->pos.x + 320.0F;
}

// 0x80131F2C
void mvOpeningCliffMakeWallpaper(void)
{
    GObj *wallpaper_gobj;
    SObj *wallpaper_sobj;

    wallpaper_gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(wallpaper_gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(wallpaper_gobj, mvOpeningCliffWallpaperProcDisplay, nGCProcessKindFunc, 1);

    wallpaper_sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, lbRelocGetFileData(Sprite*, sMVOpeningCliffFiles[1], &llMVOpeningStandoffWallpaperSprite));
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;

    wallpaper_sobj->sprite.scalex = 2.0F;
    wallpaper_sobj->sprite.scaley = 2.0F;

    wallpaper_sobj->pos.x = 0.0F;
    wallpaper_sobj->pos.y = 0.0F;

    wallpaper_sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, lbRelocGetFileData(Sprite*, sMVOpeningCliffFiles[1], &llMVOpeningStandoffWallpaperSprite));
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;

    wallpaper_sobj->sprite.scalex = 2.0F;
    wallpaper_sobj->sprite.scaley = 2.0F;

    wallpaper_sobj->pos.x = 320.0F;
    wallpaper_sobj->pos.y = 0.0F;
}

// 0x80132024
void mvOpeningCliffMakeOcarina(void)
{
    GObj *ocarina_gobj;

    sMVOpeningCliffOcarinaGObj = ocarina_gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcSetupCustomDObjs
    (
        ocarina_gobj,
        lbRelocGetFileData
        (
            DObjDesc*,
            sMVOpeningCliffFiles[0],
            &llMVOpeningCliffOcarinaDObjDesc
        ),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddGObjDisplay(ocarina_gobj, gcDrawDObjTreeForGObj, 26, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(ocarina_gobj)->scale.vec.f.x = DObjGetStruct(sMVOpeningCliffFighterGObj)->scale.vec.f.x;
    DObjGetStruct(ocarina_gobj)->scale.vec.f.y = DObjGetStruct(sMVOpeningCliffFighterGObj)->scale.vec.f.y;
    DObjGetStruct(ocarina_gobj)->scale.vec.f.z = DObjGetStruct(sMVOpeningCliffFighterGObj)->scale.vec.f.z;

    gcAddAnimJointAll
    (
        ocarina_gobj,
        lbRelocGetFileData
        (
            AObjEvent32**,
            sMVOpeningCliffFiles[0],
            &llMVOpeningCliffOcarinaAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(ocarina_gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x8013212C
void mvOpeningCliffCameraProcUpdate(GObj *gobj)
{
    gcPlayCamAnim(gobj);
}

// 0x8013214C
void mvOpeningCliffMakeMainCamera(void)
{
    GObj* camera_gobj;
    CObj* cobj;

    camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        80,
        COBJ_MASK_DLLINK(26),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    gcAddXObjForCamera(CObjGetStruct(camera_gobj), nGCMatrixKindPerspF, 0);
    gcAddXObjForCamera(CObjGetStruct(camera_gobj), 6, 0);

    cobj = CObjGetStruct(camera_gobj);
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;

    gcAddCObjCamAnimJoint
    (
        cobj,
        lbRelocGetFileData
        (
            AObjEvent32*,
            sMVOpeningCliffFiles[0],
            &llMVOpeningCliffCamAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(camera_gobj, mvOpeningCliffCameraProcUpdate, nGCProcessKindFunc, 1);

    camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        70,
        COBJ_MASK_DLLINK(28),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    gcAddXObjForCamera(CObjGetStruct(camera_gobj), nGCMatrixKindPerspF, 0);
    gcAddXObjForCamera(CObjGetStruct(camera_gobj), 6, 0);

    cobj = CObjGetStruct(camera_gobj);
    
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;

    gcAddCObjCamAnimJoint
    (
        cobj,
        lbRelocGetFileData
        (
            AObjEvent32*,
            sMVOpeningCliffFiles[0],
            &llMVOpeningCliffCamAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(camera_gobj, mvOpeningCliffCameraProcUpdate, nGCProcessKindFunc, 1);
}

// 0x80132368
void mvOpeningCliffMakeWallpaperCamera(void)
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

// 0x80132408
void mvOpeningCliffInitTotalTimeTics(void)
{
    sMVOpeningCliffTotalTimeTics = 0;
}

// 0x80132414
void mvOpeningCliffFuncRun(GObj *gobj)
{
    sMVOpeningCliffTotalTimeTics++;

    if (sMVOpeningCliffTotalTimeTics >= 10)
    {
        if (sMVOpeningCliffUnused0x801327DC != 0)
        {
            sMVOpeningCliffUnused0x801327DC--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE)
        )
        {
            sMVOpeningCliffUnused0x801327DC = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();
        }
        if (sMVOpeningCliffTotalTimeTics == 160)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindOpeningYamabuki;

            syTaskmanSetLoadScene();
        }
    }
}

// 0x801324E0
void mvOpeningCliffFuncStart(void)
{
    s32 unused;
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVOpeningCliffStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningCliffStatusBuffer);
    rl_setup.force_status_buffer = sMVOpeningCliffForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningCliffForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVOpeningCliffFileIDs, sMVOpeningCliffFiles);
    gcMakeGObjSPAfter(0, mvOpeningCliffFuncRun, 0, GOBJ_PRIORITY_DEFAULT);

    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));

    efParticleInitAll();
    mvOpeningCliffInitTotalTimeTics();
    efManagerInitEffects();
    ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, 1);
    ftManagerSetupFilesAllKind(nFTKindLink);

    sMVOpeningCliffFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

    mvOpeningCliffMakeMainCamera();
    mvOpeningCliffMakeWallpaperCamera();
    mvOpeningCliffMakeWallpaper();
    mvOpeningCliffMakeHills();
    mvOpeningCliffMakeFighter();
    mvOpeningCliffMakeOcarina();

    scSubsysFighterSetLightParams(-45.0F, 25.0F, 0xFF, 0xFF, 0xFF, 0xFF);

    while (sySchedulerGetTicCount() < 2500)
    {
        continue;
    }
}

// 0x80132674
void mvOpeningCliffStartScene(void)
{
    dMVOpeningCliffVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMVOpeningCliffVideoSetup);

    dMVOpeningCliffTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl46_BSS_END);
    syTaskmanStartTask(&dMVOpeningCliffTaskmanSetup);
}
