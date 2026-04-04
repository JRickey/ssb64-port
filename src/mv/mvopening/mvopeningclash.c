#include <ft/fighter.h>
#include <gr/ground.h>
#include <mv/movie.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <lb/library.h>
#include <reloc_data.h>

extern void syTaskmanSetLoadScene();
extern u32 sySchedulerGetTicCount();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801328D0
u32 dMVOpeningClashFileIDs[/* */] = { &llMVOpeningClashFightersFileID, &llMVOpeningClashWallpaperFileID };

// 0x801328D8
Lights1 dMVOpeningClashLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x801328F0
Lights1 dMVOpeningClashLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801329D0
s32 sMVOpeningClashPad0x801329D0[2];

// 0x801329D8
void *sMVOpeningClashFigatreeHeaps[8];

// 0x801329F8
s32 sMVOpeningClashPad0x801329F8[4];

// 0x80132A08
s32 sMVOpeningClashTotalTimeTics;

// 0x80132A0C
s32 sMVOpeningClashVoidAlpha;

// 0x80132A10
s32 sMVOpeningClashUnused0x80132A10;

// 0x80132A18
LBFileNode sMVOpeningClashStatusBuffer[100];

// 0x80132D38
LBFileNode sMVOpeningClashForceStatusBuffer[7];

// 0x80132D70
void *sMVOpeningClashFiles[ARRAY_COUNT(dMVOpeningClashFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvOpeningClashFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);
    ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58 - Unused?
void func_ovl49_80131B58()
{
    return;
}

// 0x80131B60
void mvOpeningClashMakeFighters(void)
{
    GObj* fighter_gobj;

    // 0x80132908
    s32 fkinds[/* */] =
    {
        nFTKindMario,
        nFTKindKirby,
        nFTKindLink,
        nFTKindYoshi,
        nFTKindFox,
        nFTKindDonkey,
        nFTKindSamus,
        nFTKindPikachu
    };

    s32 i;
    FTDesc desc = dFTManagerDefaultFighterDesc;

    for (i = 0; i < (ARRAY_COUNT(fkinds) + ARRAY_COUNT(sMVOpeningClashFigatreeHeaps)) / 2; i++)
    {
        desc.fkind = fkinds[i];
        desc.costume = ftParamGetCostumeCommonID(fkinds[i], 0);

        desc.pos.x = 0.0F;
        desc.pos.y = 0.0F;
        desc.pos.z = 0.0F;

        desc.figatree_heap = sMVOpeningClashFigatreeHeaps[i];
        fighter_gobj = ftManagerMakeFighter(&desc);

        scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusClash);

        DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
        DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
        DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
    }
}

// 0x80131CCC
void mvOpeningClashVoidProcDisplay(GObj *gobj)
{
    if (sMVOpeningClashVoidAlpha < 0xFF)
    {
        sMVOpeningClashVoidAlpha += 0x1E;

        if (sMVOpeningClashVoidAlpha > 0xFF)
        {
            sMVOpeningClashVoidAlpha = 0xFF;
        }
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, sMVOpeningClashVoidAlpha);
    gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80131E08
void mvOpeningClashMakeVoid(void)
{
    sMVOpeningClashVoidAlpha = 0;
    gcAddGObjDisplay
    (
        gcMakeGObjSPAfter
        (
            0,
            NULL,
            18,
            GOBJ_PRIORITY_DEFAULT
        ),
        mvOpeningClashVoidProcDisplay,
        26,
        GOBJ_PRIORITY_DEFAULT,
        ~0
    );
}

// 0x80131E5C
void mvOpeningClashMakeWallpaper(void)
{
    // lower left quadrant
    GObj* gobj = gcMakeGObjSPAfter(0, NULL, 20, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, gcDrawDObjDLHead0, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddXObjForDObjFixed(gcAddDObjForGObj(gobj, lbRelocGetFileData(void*, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLLDisplayList)), 0x1C, 0);
    gcAddMObjAll(gobj, lbRelocGetFileData(MObjSub***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLLMObjSub));
    gcAddMatAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLLMatAnimJoint), 0.0F);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLLAnimJoint), 0.0F);
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);

    // lower right quadrant
    gobj = gcMakeGObjSPAfter(0, NULL, 20, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, gcDrawDObjDLHead0, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddXObjForDObjFixed(gcAddDObjForGObj(gobj, lbRelocGetFileData(void*, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLRDisplayList)), 0x1C, 0);
    gcAddMObjAll(gobj, lbRelocGetFileData(MObjSub***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLRMObjSub));
    gcAddMatAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLRMatAnimJoint), 0.0F);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperLRAnimJoint), 0.0F);
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);

    // upper left quadrant
    gobj = gcMakeGObjSPAfter(0, NULL, 20, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, gcDrawDObjDLHead0, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddXObjForDObjFixed(gcAddDObjForGObj(gobj, lbRelocGetFileData(void*, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperULDisplayList)), 0x1C, 0);
    gcAddMObjAll(gobj, lbRelocGetFileData(MObjSub***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperULMObjSub));
    gcAddMatAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperULMatAnimJoint), 0.0F);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperULAnimJoint), 0.0F);
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);

    // upper right quadrant
    gobj = gcMakeGObjSPAfter(0, NULL, 20, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, gcDrawDObjDLHead0, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddXObjForDObjFixed(gcAddDObjForGObj(gobj, lbRelocGetFileData(void*, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperURDisplayList)), 0x1C, 0);
    gcAddMObjAll(gobj, lbRelocGetFileData(MObjSub***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperURMObjSub));
    gcAddMatAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32***, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperURMatAnimJoint), 0.0F);

    DObjGetStruct(gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperURAnimJoint), 0.0F);
    gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x80132204
void mvOpeningClashMakeFightersCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        60,
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

    cobj->flags |= COBJ_FLAG_ZBUFFER;

    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;

    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningClashFiles[0], &llMVOpeningClashFightersCamAnimJoint), 0.0F);
    gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x80132314
void mvOpeningClashMakeVoidCamera(void)
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
            40,
            COBJ_MASK_DLLINK(26),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
    
    cobj->flags |= COBJ_FLAG_ZBUFFER;
}

// 0x801323C8
void mvOpeningClashWallpaperProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);

    func_80017DBC(gobj);
    
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8013246C
void mvOpeningClashMakeWallpaperCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        mvOpeningClashWallpaperProcDisplay,
        90,
        COBJ_MASK_DLLINK(29),
        -1,
        TRUE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    CObj *cobj = CObjGetStruct(camera_gobj);

    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningClashFiles[1], &llMVOpeningClashWallpaperCamAnimJoint), 0.0F);

    gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x80132550
void mvOpeningClashInitTotalTimeTics(void)
{
    sMVOpeningClashTotalTimeTics = 0;
}

// 0x8013255C
void mvOpeningClashFuncRun(GObj *gobj)
{
    sMVOpeningClashTotalTimeTics++;

    if (sMVOpeningClashTotalTimeTics >= 10)
    {
        if (sMVOpeningClashUnused0x80132A10 != 0)
        {
            sMVOpeningClashUnused0x80132A10--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE)
        )
        {
            sMVOpeningClashUnused0x80132A10 = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;
            
            syTaskmanSetLoadScene();
        }

        if (sMVOpeningClashTotalTimeTics == 144)
        {
            mvOpeningClashMakeVoid();
        }
        if (sMVOpeningClashTotalTimeTics == 160)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindOpeningNewcomers;

            syTaskmanSetLoadScene();
        }
        if
        (
            (sMVOpeningClashTotalTimeTics == 15) ||
            (sMVOpeningClashTotalTimeTics == 75) ||
            (sMVOpeningClashTotalTimeTics == 90) ||
            (sMVOpeningClashTotalTimeTics == 105)
        )
        {
            func_800269C0_275C0(nSYAudioFGMOpeningClash);
        }
    }
}

// 0x8013267C
void mvOpeningClashFuncStart(void)
{
    s32 i;
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVOpeningClashStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningClashStatusBuffer);
    rl_setup.force_status_buffer = sMVOpeningClashForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningClashForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVOpeningClashFileIDs, sMVOpeningClashFiles);
    gcMakeGObjSPAfter(0, mvOpeningClashFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

    efParticleInitAll();
    mvOpeningClashInitTotalTimeTics();
    efManagerInitEffects();

    // Not quite correct to use the length of sMVOpeningClashFigatreeHeaps here, but also probably not worth a #define
    ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, ARRAY_COUNT(sMVOpeningClashFigatreeHeaps));

    ftManagerSetupFilesAllKind(nFTKindMario);
    ftManagerSetupFilesAllKind(nFTKindFox);
    ftManagerSetupFilesAllKind(nFTKindDonkey);
    ftManagerSetupFilesAllKind(nFTKindSamus);
    ftManagerSetupFilesAllKind(nFTKindLink);
    ftManagerSetupFilesAllKind(nFTKindYoshi);
    ftManagerSetupFilesAllKind(nFTKindKirby);
    ftManagerSetupFilesAllKind(nFTKindPikachu);

    for (i = 0; i < ARRAY_COUNT(sMVOpeningClashFigatreeHeaps); i++)
    {
        sMVOpeningClashFigatreeHeaps[i] = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);
    }
    mvOpeningClashMakeFightersCamera();
    mvOpeningClashMakeVoidCamera();
    mvOpeningClashMakeWallpaperCamera();
    mvOpeningClashMakeFighters();
    mvOpeningClashMakeWallpaper();

    scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);

    while (sySchedulerGetTicCount() < 3975)
    {
        continue;
    }
}

// 0x80132928
SYVideoSetup dMVOpeningClashVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132944
SYTaskmanSetup dMVOpeningClashTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                  // Frame draw function
        &ovl49_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningClashFuncLights,       // Pre-render function
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
    
    mvOpeningClashFuncStart             // Task start function
};

// 0x80132874
void mvOpeningClashStartScene(void)
{
    dMVOpeningClashVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMVOpeningClashVideoSetup);

    dMVOpeningClashTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl49_BSS_END);
    syTaskmanStartTask(&dMVOpeningClashTaskmanSetup);
}
