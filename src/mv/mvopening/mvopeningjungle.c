#include <ft/fighter.h>
#include <gr/ground.h>
#include <if/interface.h>
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

// 0x8018D870
CObjDesc dMVOpeningJungleCObjDescStart =
{
    // Eye
    { 1000.0F, 900.0F, 3600.0F },

    // At
    { 600.0F, 800.0F, 0.0F },

    // Roll (Up X)
    0.0F
};

// 0x8018D88C
CObjDesc dMVOpeningJungleCObjDescEnd =
{
    // Eye
    { -1800.0F, 800.0F, 1500.0F },

    // At
    { 1000.0F, 700.0F, 0.0F },

    // Roll (Up X)
    0.0F
};

// 0x8018D8A8
FTKeyEvent dMVOpeningJungleDonkeyKeyEvents[/* */] =
{
    FTKEY_EVENT_STICK(0, -I_CONTROLLER_RANGE_MAX, 0),   // 0x2000, 0x00B0
    FTKEY_EVENT_BUTTON(B_BUTTON, 1),                    // 0x1001, 0x4000
    FTKEY_EVENT_BUTTON(0, 1),                           // 0x1001, 0x0000
    FTKEY_EVENT_STICK(0, 0, 50),                        // 0x2032, 0x0000
    FTKEY_EVENT_STICK(15, I_CONTROLLER_RANGE_MAX, 30),  // 0x201E, 0x0F50
    FTKEY_EVENT_STICK(I_CONTROLLER_RANGE_MAX, 0, 10),   // 0x200A, 0x5000
    FTKEY_EVENT_BUTTON(A_BUTTON, 1),                    // 0x1001, 0x8000
    FTKEY_EVENT_BUTTON(0, 1),                           // 0x1001, 0x0000
    FTKEY_EVENT_STICK(I_CONTROLLER_RANGE_MAX, 0, 60),   // 0x203C, 0x5000
    FTKEY_EVENT_STICK(-30, 0, 3),                       // 0x2003, 0xE200
    FTKEY_EVENT_STICK(0, 0, 10),                        // 0x200A, 0x0000
    FTKEY_EVENT_STICK(-I_CONTROLLER_RANGE_MAX, 0, 1),   // 0x2001, 0xB000
    FTKEY_EVENT_BUTTON(B_BUTTON, 1),                    // 0x1001, 0x4000
    FTKEY_EVENT_BUTTON(0, 1),                           // 0x1001, 0x0000
    FTKEY_EVENT_BUTTON(B_BUTTON, 1),                    // 0x1001, 0x4000
    FTKEY_EVENT_BUTTON(0, 1),                           // 0x1001, 0x0000
    FTKEY_EVENT_STICK(0, 0, 1),                         // 0x2001, 0x0000
    FTKEY_EVENT_END()                                   // 0x0000
};

// 0x8018D8F0
FTKeyEvent dMVOpeningJungleSamusKeyEvents[/* */] =
{
    FTKEY_EVENT_STICK(-I_CONTROLLER_RANGE_MAX, 0, 20),  // 0x2014, 0xB000
    FTKEY_EVENT_STICK(0, 0, 75),                        // 0x204B, 0x0000
    FTKEY_EVENT_BUTTON(Z_TRIG, 1),                      // 0x1001, 0x2000
    FTKEY_EVENT_STICK(-I_CONTROLLER_RANGE_MAX, 0, 60),  // 0x203C, 0xB000
    FTKEY_EVENT_STICK(0, 0, 1),                         // 0x2001, 0x0000
    FTKEY_EVENT_STICK(-I_CONTROLLER_RANGE_MAX, 0, 5),   // 0x2005, 0xB000
    FTKEY_EVENT_STICK(0, 0, 1),                         // 0x2001, 0x0000
    FTKEY_EVENT_STICK(0, 0, 23),                        // 0x2017, 0x0000
    FTKEY_EVENT_STICK(-I_CONTROLLER_RANGE_MAX, 0, 5),   // 0x2005, 0xB000
    FTKEY_EVENT_BUTTON(0, 1),                           // 0x1001, 0x0000
    FTKEY_EVENT_STICK(0, 0, 40),                        // 0x2028, 0x0000
    FTKEY_EVENT_STICK(30, 0, 3),                        // 0x2003, 0x1E00
    FTKEY_EVENT_STICK(0, 0, 1),                         // 0x2001, 0x0000
    FTKEY_EVENT_BUTTON(B_BUTTON, 1),                    // 0x1001, 0x4000
    FTKEY_EVENT_BUTTON(0, 1),                           // 0x1001, 0x0000
    FTKEY_EVENT_BUTTON(B_BUTTON, 1),                    // 0x1001, 0x4000
    FTKEY_EVENT_END()                                   // 0x0000
};

// 0x8018D934
u32 dMVOpeningJungleFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningJungleFileID };

// 0x8018D93C
SYVideoSetup dMVOpeningJungleVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018D958
SYTaskmanSetup dMVOpeningJungleTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl51_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        2,                              // Number of contexts?
        sizeof(Gfx) * 2048,             // Display List Buffer 0 Size
        sizeof(Gfx) * 1024,             // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x8000,                         // Graphics Heap Size
        2,                              // ???
        0xC000,                         // RDP Output Buffer Size
        mvOpeningJungleFuncLights,      // Pre-render function
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
    
    mvOpeningJungleFuncStart            // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018DA40
s32 sMVOpeningJunglePad0x8018DA40[2];

// 0x8018DA48
s32 sMVOpeningJungleTotalTimeTics;

// 0x8018DA4C
s32 sMVOpeningJunglePad0x8018DA4C;

// 0x8018DA50
GObj* sMVOpeningJungleFighterGObj;

// 0x8018DA54
s32 sMVOpeningJunglePad0x8018DA54;

// 0x8018DA58
GObj* sMVOpeningJungleStageCameraGObj;

// 0x8018DA5C
s32 sMVOpeningJunglePad0x8018DA5C[2];

// 0x8018DA68
CObjDesc sMVOpeningJungleUnusedAdjustedStartCObjDesc;

// 0x8018DA88
CObjDesc sMVOpeningJungleUnusedAdjustedEndCObjDesc;

// 0x8018DAA8
LBFileNode sMVOpeningJungleStatusBuffer[48];

// 0x8018DC28
LBFileNode sMVOpeningJungleForceStatusBuffer[7];

// 0x8018DC60
void *sMVOpeningJungleFiles[ARRAY_COUNT(dMVOpeningJungleFileIDs)];

// 0x8018DC68
SCBattleState sMVOpeningJungleBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningJungleSetupFiles(void)
{
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVOpeningJungleStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningJungleStatusBuffer);
    rl_setup.force_status_buffer = sMVOpeningJungleForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningJungleForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVOpeningJungleFileIDs, sMVOpeningJungleFiles);
}

// 0x8018D160 - Unused?
void func_ovl51_8018D160(void)
{
    return;
}

// 0x8018D168
void mvOpeningJungleMakeGroundViewport(Vec3f unused)
{
    CObj *cobj;

    sMVOpeningJungleUnusedAdjustedStartCObjDesc = dMVOpeningJungleCObjDescStart;
    sMVOpeningJungleUnusedAdjustedEndCObjDesc = dMVOpeningJungleCObjDescEnd;

    sMVOpeningJungleStageCameraGObj = gmCameraMakeMovieCamera(NULL);

    cobj = CObjGetStruct(sMVOpeningJungleStageCameraGObj);

    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.persp.aspect = 15.0F / 11.0F;

    gcEndProcessAll(sMVOpeningJungleStageCameraGObj);

    cobj->projection.persp.near = 50.0F;
    cobj->projection.persp.far = 15000.0F;

    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningJungleFiles[1], &llMVOpeningJungleCamAnimJoint), 0.0F);
    gcAddGObjProcess(sMVOpeningJungleStageCameraGObj, gcPlayCamAnim, nGCProcessKindFunc, 1);

    gcPlayCamAnim(sMVOpeningJungleStageCameraGObj);
}

// 0x8018D2DC
void mvOpeningJungleMakeFighters(void)
{
    GObj* fighter_gobj;
    s32 i;
    s32 unused[2];
    s32 pos_ids[2];
    Vec3f spawn_position[2];
    FTStruct *fp;

    grWallpaperMakeDecideKind();
    grCommonSetupInitAll();

    if (mpCollisionGetMapObjCountKind(nMPMapObjKindMoviePlayer2) != 1)
    {
        while (TRUE)
        {
            syDebugPrintf("wrong number of mapobject\n");
            scManagerRunPrintGObjStatus();
        }
    }
    mpCollisionGetMapObjIDsKind(nMPMapObjKindMoviePlayer2, &pos_ids[1]);
    mpCollisionGetMapObjPositionID(pos_ids[1], &spawn_position[1]);

    if (mpCollisionGetMapObjCountKind(nMPMapObjKindMoviePlayer3) != 1)
    {
        while (TRUE)
        {
            syDebugPrintf("wrong number of mapobject\n");
            scManagerRunPrintGObjStatus();
        }
    }
    mpCollisionGetMapObjIDsKind(nMPMapObjKindMoviePlayer3, &pos_ids[0]);
    mpCollisionGetMapObjPositionID(pos_ids[0], &spawn_position[0]);

    spawn_position[0].x += 1100.0F;

    mvOpeningJungleMakeGroundViewport(spawn_position[1]);

    gmRumbleMakeActor();
    ftPublicMakeActor();

    for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
    {
        FTDesc desc = dFTManagerDefaultFighterDesc;

        if (gSCManagerBattleState->players[i].pkind == nFTPlayerKindNot)
        {
            continue;
        }
        ftManagerSetupFilesAllKind(gSCManagerBattleState->players[i].fkind);

        desc.fkind = gSCManagerBattleState->players[i].fkind;

        if (gSCManagerBattleState->players[i].fkind == nFTKindDonkey)
        {
            desc.pos.x = spawn_position[1].x;
            desc.pos.y = spawn_position[1].y;
            desc.pos.z = spawn_position[1].z;

            desc.lr = +1;

            desc.damage = 200;
        }
        else
        {
            desc.pos.x = spawn_position[0].x;
            desc.pos.y = spawn_position[0].y;
            desc.pos.z = spawn_position[0].z;

            desc.lr = -1;

            desc.damage = 40;
        }
        desc.team = gSCManagerBattleState->players[i].team;
        desc.player = i;
        desc.detail = nFTPartsDetailHigh;
        desc.costume = gSCManagerBattleState->players[i].costume;
        desc.handicap = gSCManagerBattleState->players[i].handicap;
        desc.level = gSCManagerBattleState->players[i].level;
        desc.stock_count = gSCManagerBattleState->stocks;
        desc.pkind = gSCManagerBattleState->players[i].pkind;
        desc.controller = &gSYControllerDevices[i];
        desc.figatree_heap = ftManagerAllocFigatreeHeapKind(gSCManagerBattleState->players[i].fkind);

        sMVOpeningJungleFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

        if (gSCManagerBattleState->players[i].fkind == nFTKindDonkey)
        {
            fp = ftGetStruct(fighter_gobj);

            fp->passive_vars.donkey.charge_level = 9;
        }
        else
        {
            fp = ftGetStruct(fighter_gobj);

            fp->passive_vars.samus.charge_level = 6;
        }
        ftParamInitPlayerBattleStats(i, fighter_gobj);

        if (gSCManagerBattleState->players[i].fkind == nFTKindDonkey)
        {
            ftParamSetKey(fighter_gobj, dMVOpeningJungleDonkeyKeyEvents);
        }
        else ftParamSetKey(fighter_gobj, dMVOpeningJungleSamusKeyEvents);
    }
}

// 0x8018D5E4
void mvOpeningJungleFuncRun(GObj *gobj)
{
    sMVOpeningJungleTotalTimeTics++;

    if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
    {
        gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
        gSCManagerSceneData.scene_curr = nSCKindTitle;

        syTaskmanSetLoadScene();
    }
    if (sMVOpeningJungleTotalTimeTics == 320)
    {
        gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
        gSCManagerSceneData.scene_curr = nSCKindOpeningYoster;

        syTaskmanSetLoadScene();
    }
}

// 0x8018D668 - Unused?
void func_ovl51_8018D668(void)
{
    return;
}

// 0x8018D670
void mvOpeningJungleFuncStart(void)
{
    sMVOpeningJungleBattleState = dSCManagerDefaultBattleState;
    gSCManagerBattleState = &sMVOpeningJungleBattleState;

    gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

    gSCManagerBattleState->gkind = nGRKindJungle;
    gSCManagerBattleState->pl_count = 1;

    gSCManagerBattleState->players[0].fkind = nFTKindDonkey;
    gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;
    gSCManagerBattleState->players[1].fkind = nFTKindSamus;
    gSCManagerBattleState->players[1].pkind = nFTPlayerKindKey;

    mvOpeningJungleSetupFiles();
    gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningJungleFuncRun, 13, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, 0x80000000, 0x64, 3, 0xFF);
    efParticleInitAll();
    ftParamInitGame();
    mpCollisionInitGroundData();
    gmCameraSetViewportDimensions(10, 10, 310, 230);
    gmCameraMakeWallpaperCamera();
    ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION, 2);
    wpManagerAllocWeapons();
    itManagerInitItems();
    efManagerInitEffects();
    ifScreenFlashMakeInterface(0xFF);
    mvOpeningJungleMakeFighters();

    while (sySchedulerGetTicCount() < 2880)
    {
        continue;
    }
}

// 0x8018D7CC
void mvOpeningJungleFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);
    ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018D818
void mvOpeningJungleStartScene(void)
{
    dMVOpeningJungleVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMVOpeningJungleVideoSetup);

    dMVOpeningJungleTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl51_BSS_END);
    syTaskmanStartTask(&dMVOpeningJungleTaskmanSetup);
}
