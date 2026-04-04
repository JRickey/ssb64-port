#include <ft/fighter.h>
#include <mv/movie.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132BC0
s32 sMVEndingPad0x80132BC0[2];

// 0x80132BC8
void *sMVEndingFigatreeHeap;

// 0x80132BCC
s32 sMVEndingTotalTimeTics;

// 0x80132BD0
GObj *sMVEndingRoomCameraGObj;

// 0x80132BD4
GObj *sMVEndingFighterCameraGObj;

// 0x80132BD8
s32 sMVEndingPad0x80132BD8;

// 0x80132BDC
GObj *sMVEndingFighterGObj;

// 0x80132BE0
GObj *sMVEndingRoomBackgroundGObj;

// 0x80132BE4
GObj *sMVEndingRoomDeskGObj;

// 0x80132BE8
GObj *sMVEndingRoomBooksGObj;

// 0x80132BEC
GObj *sMVEndingRoomPencilsGObj;

// 0x80132BF0
GObj *sMVEndingRoomLampGObj;

// 0x80132BF4
GObj *sMVEndingRoomTissuesGObj;

// 0x80132BF8
s32 sMVEndingRoomFadeInAlpha;

// 0x80132BFC
f32 sMVEndingRoomLightAlpha;

// 0x80132C00
GObj *sMVEndingRoomFadeInGObj;

// 0x80132C04
GObj *sMVEndingRoomLightGObj;

// 0x80132C08
FTDemoDesc sMVEndingFighterDemoDesc;

// 0x80132C14
s32 sMVEndingUnused0x80132C14;

// 0x80132C18
LBFileNode sMVEndingStatusBuffer[100];

// 0x80132F38
LBFileNode sMVEndingForceStatusBuffer[7];

// 0x80132F70
void *sMVEndingFiles[2];

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132AD0
u32 dMVEndingFileIDs[/* */] = { &llMVCommonFileID, &llMVEndingFileID };

// 0x80132AD8
Lights1 dMVEndingLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x80132AF0
Lights1 dMVEndingLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// 0x80132B08
SYVideoSetup dMVEndingVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132B24
SYTaskmanSetup dMVEndingTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,                  // Frame draw function
        &ovl54_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2500,         // Display List Buffer 0 Size
        sizeof(Gfx) * 512,          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        mvEndingFuncLights,         // Pre-render function
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
    
    mvEndingFuncStart               // Task start function
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvEndingFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);

    ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void mvEndingMakeRoomBackground(void)
{
    GObj *gobj;

    sMVEndingRoomBackgroundGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);

    gcSetupCommonDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMVEndingFiles[0], &llMVCommonRoomBackgroundDObjDesc), NULL);
    gcAddGObjDisplay(gobj, gcDrawDObjTreeDLLinksForGObj, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddMObjAll(gobj, lbRelocGetFileData(MObjSub***, sMVEndingFiles[0], &llMVCommonRoomBackgroundMObjSub));
    gcAddMatAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32***, sMVEndingFiles[0], &llMVCommonRoomBackgroundMatAnimJoint), 0.0F);
    gcPlayAnimAll(gobj);
}

// 0x80131C1C
void mvEndingMakeRoomDesk(void)
{
    GObj *gobj;

    sMVEndingRoomDeskGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);

    gcSetupCommonDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMVEndingFiles[0], &llMVCommonRoomDeskDObjDesc), NULL);
    gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 29, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x80131C94
void mvEndingMakeRoomBooks(void)
{
    GObj *gobj;

    sMVEndingRoomBooksGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);

    gcSetupCommonDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMVEndingFiles[0], &llMVCommonRoomBooksDObjDesc), NULL);
    gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVEndingFiles[0], &llMVCommonRoomBooksAnimJoint), 300.0F);
    gcPlayAnimAll(gobj);
}

// 0x80131D34
void mvEndingMakeRoomPencils(void)
{
    GObj *gobj;

    sMVEndingRoomPencilsGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);

    gcSetupCommonDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMVEndingFiles[0], &llMVCommonRoomPencilsDObjDesc), NULL);
    gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVEndingFiles[0], &llMVCommonRoomPencilsAnimJoint), 300.0F);
    gcPlayAnimAll(gobj);
}

// 0x80131DD4
void mvEndingMakeRoomLamp(void)
{
    GObj *gobj;

    sMVEndingRoomLampGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);

    gcSetupCommonDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMVEndingFiles[0], &llMVCommonRoomLampDObjDesc), NULL);
    gcAddGObjDisplay(gobj, gcDrawDObjTreeForGObj, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMVEndingFiles[0], &llMVCommonRoomLampAnimJoint), 300.0F);
    gcPlayAnimAll(gobj);
}

// 0x80131E74
void mvEndingMakeRoomTissues(void)
{
    GObj *gobj;
    DObj *dobj;

    sMVEndingRoomTissuesGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);

    dobj = gcAddDObjForGObj(gobj, lbRelocGetFileData(void*, sMVEndingFiles[0], &llMVCommonRoomTissuesDisplayList));

    gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyRSca, 0);
    gcAddGObjDisplay(gobj, gcDrawDObjDLHead0, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddDObjAnimJoint(dobj, lbRelocGetFileData(AObjEvent32*, sMVEndingFiles[0], &llMVCommonRoomTissuesAnimJoint), 300.0F);
    gcPlayAnimAll(gobj);
}

// 0x80131F24
void mvEndingMakeFighter(s32 fkind)
{
    GObj *fighter_gobj;
    FTDesc desc = dFTManagerDefaultFighterDesc;

    desc.fkind = fkind;

    desc.pos.x = -1077.804F;
    desc.pos.y = 4038.864F;

    desc.costume = sMVEndingFighterDemoDesc.costume;
    desc.shade = sMVEndingFighterDemoDesc.shade;

    desc.figatree_heap = sMVEndingFigatreeHeap;

    desc.pos.z = -3688.5298F;

    sMVEndingFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

    scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusFigureDropped);
}

// 0x80131FE8
void mvEndingRoomFadeInProcDisplay(GObj *gobj)
{
    if (sMVEndingTotalTimeTics >= 540)
    {
        sMVEndingRoomFadeInAlpha = 0xFF;
    }
    if ((sMVEndingTotalTimeTics >= 70) && (sMVEndingTotalTimeTics < 540))
    {
        if (sMVEndingRoomFadeInAlpha > 0x00)
        {
            sMVEndingRoomFadeInAlpha -= 0x07;

            if (sMVEndingRoomFadeInAlpha < 0x00)
            {
                sMVEndingRoomFadeInAlpha = 0x00;
            }
        }
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0x00, sMVEndingRoomFadeInAlpha);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80132144
void mvEndingMakeRoomFadeIn(void)
{
    GObj *gobj;

    sMVEndingRoomFadeInAlpha = 0xFF;

    sMVEndingRoomFadeInGObj = gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, mvEndingRoomFadeInProcDisplay, 26, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x801321A4
void mvEndingMakeRoomFadeInCamera(void)
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
            60,
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
}

// 0x80132244
void mvEndingRoomLightProcDisplay(GObj *gobj)
{
    if ((sMVEndingTotalTimeTics >= 340) && (sMVEndingRoomLightAlpha < 220.0F))
    {
        sMVEndingRoomLightAlpha += 1.1F;

        if (sMVEndingRoomLightAlpha > 220.0F)
        {
            sMVEndingRoomLightAlpha = 220.0F;
        }
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, (u8)sMVEndingRoomLightAlpha);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8013243C
void mvEndingMakeRoomLight(void)
{
    GObj *gobj;

    sMVEndingRoomLightAlpha = 0.0F;

    sMVEndingRoomLightGObj = gobj = gcMakeGObjSPAfter(0, NULL, 21, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, mvEndingRoomLightProcDisplay, 30, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x8013249C
void mvEndingMakeRoomLightCamera(void)
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
            30,
            COBJ_MASK_DLLINK(30),
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

// 0x8013253C
void mvEndingEjectRoomGObjs(void)
{
    gcEjectGObj(sMVEndingRoomBackgroundGObj);
    gcEjectGObj(sMVEndingRoomBooksGObj);
    gcEjectGObj(sMVEndingRoomPencilsGObj);
    gcEjectGObj(sMVEndingRoomLampGObj);
    gcEjectGObj(sMVEndingRoomTissuesGObj);
}

// 0x80132590
void mvEndingSetupOperatorCamera(GObj *gobj)
{
    CObj *cobj = CObjGetStruct(gobj);

    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;

    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVEndingFiles[1], &llMVEndingOperatorCamAnimJoint), 0.0F);
    gcAddGObjProcess(gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x80132630
void mvEndingMakeMainCameras(void)
{
    GObj *gobj;

    sMVEndingRoomCameraGObj = gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        80,
        COBJ_MASK_DLLINK(29),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    gcAddXObjForCamera(CObjGetStruct(gobj), nGCMatrixKindPerspFastF, 0);
    gcAddXObjForCamera(CObjGetStruct(gobj), 8, 0);
    mvEndingSetupOperatorCamera(gobj);

    CObjGetStruct(gobj)->flags |= 4;

    sMVEndingFighterCameraGObj = gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        40,
        COBJ_MASK_DLLINK(9),
        -1,
        TRUE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    mvEndingSetupOperatorCamera(gobj);

    CObjGetStruct(gobj)->flags |= 4;
}

// 0x80132774
void mvEndingInitVars(void)
{
    sMVEndingTotalTimeTics = 0;

    sMVEndingFighterDemoDesc.fkind = gSCManager1PGameBattleState.players[gSCManagerSceneData.player].fkind;
    sMVEndingFighterDemoDesc.costume = gSCManager1PGameBattleState.players[gSCManagerSceneData.player].costume;
    sMVEndingFighterDemoDesc.shade   = gSCManager1PGameBattleState.players[gSCManagerSceneData.player].shade;
}

// 0x801327C8
void mvEndingFuncRun(GObj *gobj)
{
    sMVEndingTotalTimeTics++;

    if (sMVEndingTotalTimeTics >= 10)
    {
        if (sMVEndingUnused0x80132C14 != 0)
        {
            sMVEndingUnused0x80132C14--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE)
        )
        {
            sMVEndingUnused0x80132C14 = 0;
        }
        if (sMVEndingTotalTimeTics == 340)
        {
            mvEndingMakeRoomLight();
        }
        if (sMVEndingTotalTimeTics == 540)
        {
            mvEndingEjectRoomGObjs();
            ftManagerDestroyFighter(sMVEndingFighterGObj);
            gcEjectGObj(sMVEndingRoomLightGObj);
            func_800269C0_275C0(nSYAudioFGMDoorClose);
        }
        if (sMVEndingTotalTimeTics == 660)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindStaffroll;

            syTaskmanSetLoadScene();
        }
    }
}

// 0x801328D0
void mvEndingFuncStart(void)
{
    s32 unused;
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVEndingStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVEndingStatusBuffer);
    rl_setup.force_status_buffer = sMVEndingForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVEndingForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVEndingFileIDs, sMVEndingFiles);
    gcMakeGObjSPAfter(0, mvEndingFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0xFF, 0xFF, 0xFF, 0xFF));
    efParticleInitAll();
    mvEndingInitVars();
    efManagerInitEffects();
    ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, 1);
    ftManagerSetupFilesAllKind(sMVEndingFighterDemoDesc.fkind);

    sMVEndingFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

    mvEndingMakeMainCameras();
    mvEndingMakeRoomFadeInCamera();
    mvEndingMakeRoomLightCamera();
    mvEndingMakeRoomBackground();
    mvEndingMakeRoomDesk();
    mvEndingMakeRoomBooks();
    mvEndingMakeRoomLamp();
    mvEndingMakeRoomPencils();
    mvEndingMakeRoomTissues();
    mvEndingMakeFighter(sMVEndingFighterDemoDesc.fkind);
    mvEndingMakeRoomFadeIn();

    scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);
    syAudioPlayBGM(0, nSYAudioBGMEnding);
}

// 0x80132A78
void mvEndingStartScene(void)
{
    dMVEndingVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);

    syVideoInit(&dMVEndingVideoSetup);

    dMVEndingTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl54_BSS_END);

    syTaskmanStartTask(&dMVEndingTaskmanSetup);
}
