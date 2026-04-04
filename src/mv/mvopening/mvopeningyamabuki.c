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

// 0x801323A0
u32 dMVOpeningYamabukiFileIDs[/* */] = { &llMVOpeningYamabukiFileID };

// 0x801323A8
Lights1 dMVOpeningYamabukiLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x801323C0
Lights1 dMVOpeningYamabukiLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// 0x801323D8
SYVideoSetup dMVOpeningYamabukiVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x801323F4
SYTaskmanSetup dMVOpeningYamabukiTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                      // Frame draw function
        &ovl48_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        2,                              // Number of contexts?
        sizeof(Gfx) * 2500,             // Display List Buffer 0 Size
        sizeof(Gfx) * 512,              // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x8000,                         // Graphics Heap Size
        2,                              // ???
        0xC000,                         // RDP Output Buffer Size
        mvOpeningYamabukiFuncLights,    // Pre-render function
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
    
    mvOpeningYamabukiFuncStart          // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132480
s32 sMVOpeningYamabukiPad0x80132480[2];

// 0x80132488
void *sMVOpeningYamabukiFigatreeHeap;

// 0x8013248C
s32 sMVOpeningYamabukiTotalTimeTics;

// 0x80132490
s32 sMVOpeningYamabukiPad0x80132490;

// 0x80132494
GObj* sMVOpeningYamabukiFighterGObj;

// 0x80132498
s32 sMVOpeningYamabukiPad0x80132498;

// 0x8013249C
s32 sMVOpeningYamabukiUnused0x8013249C;

// 0x801324A0
LBFileNode sMVOpeningYamabukiStatusBuffer[48];

// 0x80132620
LBFileNode sMVOpeningYamabukiForceStatusBuffer[7];

// 0x80132658
void *sMVOpeningYamabukiFiles[ARRAY_COUNT(dMVOpeningYamabukiFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvOpeningYamabukiFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);
    ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void mvOpeningYamabukiMakeWallpaper(void)
{
    GObj* gobj;
    SObj* sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 20, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, nGCMatrixKindTraRotRpyRSca, GOBJ_PRIORITY_DEFAULT, ~0);

    sobj = lbCommonMakeSObjForGObj
    (
        gobj,
        lbRelocGetFileData
        (
            Sprite*,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiWallpaperSprite
        )
    );
    sobj->pos.x = 0.0F;
    sobj->pos.y = 0.0F;
}

// 0x80131BD4
void mvOpeningYamabukiMakeFighter(void)
{
    GObj* fighter_gobj;
    FTDesc desc = dFTManagerDefaultFighterDesc;

    desc.fkind = nFTKindPikachu;
    desc.costume = ftParamGetCostumeCommonID(nFTKindPikachu, 0);

    desc.pos.x = 0.0F;
    desc.pos.y = 0.0F;
    desc.pos.z = 0.0F;

    desc.figatree_heap = sMVOpeningYamabukiFigatreeHeap;
    sMVOpeningYamabukiFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

    scSubsysFighterSetStatus(fighter_gobj, 0x1000F);

    DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x80131CA4
void mvOpeningYamabukiMakeLegs(void)
{
    GObj* gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcSetupCustomDObjs
    (
        gobj,
        lbRelocGetFileData
        (
            DObjDesc*,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiLegsDObjDesc
        ),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 27, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll
    (
        gobj,
        lbRelocGetFileData
        (
            AObjEvent32**,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiLegsAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x80131D7C
void mvOpeningYamabukiMakeLegsShadow(void)
{
    GObj* gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcSetupCustomDObjs
    (
        gobj,
        lbRelocGetFileData
        (
            void*,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiLegsShadowDObjDesc
        ),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddGObjDisplay(gobj, gcDrawDObjTreeDLLinksForGObj, 27, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll
    (
        gobj,
        lbRelocGetFileData
        (
            AObjEvent32**,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiLegsShadowAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x80131E54
void mvOpeningYamabukiMakeMBall(void)
{
    GObj* gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcSetupCustomDObjs
    (
        gobj,
        lbRelocGetFileData
        (
            DObjDesc*,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiMBallDObjDesc
        ),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddGObjDisplay(gobj, gcDrawDObjTreeDLLinksForGObj, 27, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll
    (
        gobj,
        lbRelocGetFileData
        (
            AObjEvent32**,
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiMBallAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x80131F2C
void mvOpeningYamabukiMakeMainCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        80,
        COBJ_MASK_DLLINK(27) |
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
            sMVOpeningYamabukiFiles[0],
            &llMVOpeningYamabukiCamAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x80132030
void mvOpeningYamabukiMakeWallpaperCamera(void)
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

// 0x801320D0
void mvOpeningYamabukiInitTotalTimeTics(void)
{
    sMVOpeningYamabukiTotalTimeTics = 0;
}

// 0x801320DC
void mvOpeningYamabukiFuncRun(GObj *gobj)
{
    sMVOpeningYamabukiTotalTimeTics++;

    if (sMVOpeningYamabukiTotalTimeTics >= 10)
    {
        if (sMVOpeningYamabukiUnused0x8013249C != 0)
        {
            sMVOpeningYamabukiUnused0x8013249C--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE)
        )
        {
            sMVOpeningYamabukiUnused0x8013249C = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();
        }
        if (sMVOpeningYamabukiTotalTimeTics == 160)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindOpeningJungle;

            syTaskmanSetLoadScene();
        }
    }
}

// 0x801321A8
void mvOpeningYamabukiFuncStart(void)
{
    s32 unused;
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVOpeningYamabukiStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningYamabukiStatusBuffer);
    rl_setup.force_status_buffer = sMVOpeningYamabukiForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningYamabukiForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVOpeningYamabukiFileIDs, sMVOpeningYamabukiFiles);
    gcMakeGObjSPAfter(0, mvOpeningYamabukiFuncRun, 0, GOBJ_PRIORITY_DEFAULT);

    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

    efParticleInitAll();
    mvOpeningYamabukiInitTotalTimeTics();
    efManagerInitEffects();
    ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, 1);
    ftManagerSetupFilesAllKind(nFTKindPikachu);

    sMVOpeningYamabukiFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

    mvOpeningYamabukiMakeMainCamera();
    mvOpeningYamabukiMakeWallpaperCamera();
    mvOpeningYamabukiMakeWallpaper();
    mvOpeningYamabukiMakeFighter();
    mvOpeningYamabukiMakeLegs();
    mvOpeningYamabukiMakeLegsShadow();
    mvOpeningYamabukiMakeMBall();

    scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);

    while (sySchedulerGetTicCount() < 2690)
    {
        continue;
    }
}

// 0x80132344
void mvOpeningYamabukiStartScene(void)
{
    dMVOpeningYamabukiVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMVOpeningYamabukiVideoSetup);

    dMVOpeningYamabukiTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl48_BSS_END);
    syTaskmanStartTask(&dMVOpeningYamabukiTaskmanSetup);
}
