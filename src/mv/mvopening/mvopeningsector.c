#include <ft/fighter.h>
#include <sc/scene.h>
#include <mv/movie.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern u32 sySchedulerGetTicCount();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801328F0
u32 dMVOpeningSectorFileIDs[/* */] = { &llMVOpeningSectorFileID, &llFoxSpecial3FileID, &llMVOpeningSectorWallpaperFileID };

// 0x80132900
Lights1 dMVOpeningSectorLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x80132918
Lights1 dMVOpeningSectorLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132A10
s32 sMVOpeningSectorPad0x80132A10[2];

// 0x80132A18
s32 sMVOpeningSectorTotalTimeTics;

// 0x80132A1C
GObj* sMVOpeningSectorGreatFoxGObj;

// 0x80132A20
GObj* sMVOpeningSectorWallpaperGObj;

// 0x80132A24
s32 sMVOpeningSectorPad0x80132A242;

// 0x80132A28
GObj* sMVOpeningSectorArwingGObjs[3];

// 0x80132A34
f32 sMVOpeningSectorWallpaperScrollSpeedX;

// 0x80132A38
f32 sMVOpeningSectorWallpaperScrollSpeedY;

// 0x80132A3C
s32 sMVOpeningSectorCockpitAlpha;

// 0x80132A40
s32 sMVOpeningSectorUnused0x80132A40;

// 0x80132A48
LBFileNode sMVOpeningSectorStatusBuffer[48];

// 0x80132BC8
LBFileNode sMVOpeningSectorForceStatusBuffer[7];

// 0x80132C00
void *sMVOpeningSectorFiles[ARRAY_COUNT(dMVOpeningSectorFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvOpeningSectorFuncLights(Gfx **dls)
{
    gSPSetGeometryMode(dls[0]++, G_LIGHTING);
    ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void mvOpeningSectorWallpaperProcUpdate(GObj* wallpaper_gobj)
{
    SObj* wallpaper_sobj = SObjGetStruct(wallpaper_gobj);

    switch (sMVOpeningSectorTotalTimeTics)
    {
    case 1:
        sMVOpeningSectorWallpaperScrollSpeedX = 6.0F;
        break;

    case 120:
        sMVOpeningSectorWallpaperScrollSpeedX = 6.0F;
        break;

    case 140:
        sMVOpeningSectorWallpaperScrollSpeedX = 2.0F;
        break;

    case 160:
        sMVOpeningSectorWallpaperScrollSpeedX = 1.0F;
        break;
    }
    switch (sMVOpeningSectorTotalTimeTics)
    {
    case 1:
        sMVOpeningSectorWallpaperScrollSpeedY = 1.0F;
        break;

    case 120:
        sMVOpeningSectorWallpaperScrollSpeedY = 1; // yes, 1, not 1.0F
        break;

    case 160:
        sMVOpeningSectorWallpaperScrollSpeedY = 3.0F;
        break;
    }
    if ((sMVOpeningSectorTotalTimeTics > 1) && (sMVOpeningSectorTotalTimeTics < 120))
    {
        sMVOpeningSectorWallpaperScrollSpeedX += 0.0F;
    }
    if ((sMVOpeningSectorTotalTimeTics > 120) && (sMVOpeningSectorTotalTimeTics < 140))
    {
        sMVOpeningSectorWallpaperScrollSpeedX += -0.2F;
    }
    if ((sMVOpeningSectorTotalTimeTics > 140) && (sMVOpeningSectorTotalTimeTics < 160))
    {
        sMVOpeningSectorWallpaperScrollSpeedX += -0.05F;
    }
    if ((sMVOpeningSectorTotalTimeTics > 1) && (sMVOpeningSectorTotalTimeTics < 120))
    {
        sMVOpeningSectorWallpaperScrollSpeedY += 0.0F;
    }
    if ((sMVOpeningSectorTotalTimeTics > 120) && (sMVOpeningSectorTotalTimeTics < 160))
    {
        sMVOpeningSectorWallpaperScrollSpeedY += 0.05F;
    }
    wallpaper_sobj->pos.x += sMVOpeningSectorWallpaperScrollSpeedX;

    if (wallpaper_sobj->pos.x > 10.0F)
    {
        wallpaper_sobj->pos.x -= 300.0F;
    }
    if (wallpaper_sobj->pos.y < -220.0F)
    {
        wallpaper_sobj->pos.y += 220.0F;
    }
    wallpaper_sobj->next->pos.x = wallpaper_sobj->pos.x + 300.0F;
    wallpaper_sobj->next->pos.y = wallpaper_sobj->pos.y;

    wallpaper_sobj->next->next->pos.x = wallpaper_sobj->pos.x;
    wallpaper_sobj->next->next->pos.y = wallpaper_sobj->pos.y + 220.0F;

    wallpaper_sobj->next->next->next->pos.x = wallpaper_sobj->pos.x + 300.0F;
    wallpaper_sobj->next->next->next->pos.y = wallpaper_sobj->pos.y + 220.0F;
}

// 0x80131DEC
void mvOpeningSectorMakeWallpaper(void)
{
    GObj* wallpaper_gobj;
    SObj* wallpaper_sobj;

    sMVOpeningSectorWallpaperGObj = wallpaper_gobj = gcMakeGObjSPAfter(0, NULL, 20, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(wallpaper_gobj, lbCommonDrawSObjAttr, 28, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(wallpaper_gobj, mvOpeningSectorWallpaperProcUpdate, nGCProcessKindFunc, 1);

    wallpaper_sobj = lbCommonMakeSObjForGObj
    (
        wallpaper_gobj,
        lbRelocGetFileData
        (
            Sprite*,
            sMVOpeningSectorFiles[2],
            &llMVOpeningSectorWallpaperSprite
        )
    );
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;
    
    wallpaper_sobj->pos.x = 10.0F;
    wallpaper_sobj->pos.y = 10.0F;

    wallpaper_sobj = lbCommonMakeSObjForGObj
    (
        wallpaper_gobj,
        lbRelocGetFileData
        (
            Sprite*,
            sMVOpeningSectorFiles[2],
            &llMVOpeningSectorWallpaperSprite
        )
    );
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;
    
    wallpaper_sobj->pos.x = 310.0F;
    wallpaper_sobj->pos.y = 10.0F;

    wallpaper_sobj = lbCommonMakeSObjForGObj
    (
        wallpaper_gobj,
        lbRelocGetFileData
        (
            Sprite*,
            sMVOpeningSectorFiles[2],
            &llMVOpeningSectorWallpaperSprite
        )
    );
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;

    wallpaper_sobj->pos.x = 10.0F;
    wallpaper_sobj->pos.y = 230.0F;

    wallpaper_sobj = lbCommonMakeSObjForGObj
    (
        wallpaper_gobj,
        lbRelocGetFileData
        (
            Sprite*,
            sMVOpeningSectorFiles[2],
            &llMVOpeningSectorWallpaperSprite
        )
    );
    wallpaper_sobj->sprite.attr &= ~SP_FASTCOPY;

    wallpaper_sobj->pos.x = 310.0F;
    wallpaper_sobj->pos.y = 230.0F;
}

// 0x80131F4C
void mvOpeningSectorMakeGreatFox(void)
{
    GObj* great_fox_gobj;

    sMVOpeningSectorGreatFoxGObj = great_fox_gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);

    gcSetupCustomDObjs
    (
        great_fox_gobj,
        lbRelocGetFileData
        (
            DObjDesc*,
            sMVOpeningSectorFiles[0],
            &llMVOpeningSectorGreatFoxDObjDesc
        ),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddGObjDisplay(great_fox_gobj, gcDrawDObjTreeDLLinksForGObj, 27, GOBJ_PRIORITY_DEFAULT, ~0);

    DObjGetStruct(great_fox_gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(great_fox_gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(great_fox_gobj)->translate.vec.f.z = 0.0F;

    gcAddAnimJointAll
    (
        great_fox_gobj,
        lbRelocGetFileData
        (
            AObjEvent32**,
            sMVOpeningSectorFiles[0],
            &llMVOpeningSectorGreatFoxAnimJoint
        ),
        0.0F
    );
    gcAddGObjProcess(great_fox_gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
}

// 0x8013202C
void mvOpeningSectorCockpitProcDisplay(GObj *cockpit_gobj)
{
    SObj* cockpit_sobj = SObjGetStruct(cockpit_gobj);

    if (sMVOpeningSectorCockpitAlpha < 0xFF)
    {
        sMVOpeningSectorCockpitAlpha += 0x09;

        if (sMVOpeningSectorCockpitAlpha > 0xFF)
        {
            sMVOpeningSectorCockpitAlpha = 0xFF;
        }
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0x00, sMVOpeningSectorCockpitAlpha);
    gDPSetEnvColor(gSYTaskmanDLHeads[0]++, cockpit_sobj->envcolor.r, cockpit_sobj->envcolor.g, cockpit_sobj->envcolor.b, cockpit_sobj->envcolor.a);
    gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, TEXEL0,  0, 0, 0, PRIMITIVE,  0, 0, 0, TEXEL0,  0, 0, 0, PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

    lbCommonDrawSObjNoAttr(cockpit_gobj);
}

// 0x8013215C
void mvOpeningSectorCockpitProcUpdate(GObj* cockpit_gobj)
{
    SObj* cockpit_sobj = SObjGetStruct(cockpit_gobj);
    f32 scale = cockpit_sobj->sprite.scalex;

    scale += 0.025F;

    if (scale > 1.0F)
    {
        scale = 1.0F;
    }
    cockpit_sobj->sprite.scalex = scale;
    cockpit_sobj->sprite.scaley = scale;

    cockpit_sobj->pos.x = 160.0F - ((320.0F * scale) / 2);
    cockpit_sobj->pos.y = 120.0F - ((240.0F * scale) / 2);
}

// 0x801321E0
void mvOpeningSectorMakeCockpit(void)
{
    GObj* cockpit_gobj;
    SObj* cockpit_sobj;

    sMVOpeningSectorCockpitAlpha = 0;

    cockpit_gobj = gcMakeGObjSPAfter(0, NULL, 21, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(cockpit_gobj, mvOpeningSectorCockpitProcDisplay, 29, GOBJ_PRIORITY_DEFAULT, ~0);
    gcAddGObjProcess(cockpit_gobj, mvOpeningSectorCockpitProcUpdate, nGCProcessKindFunc, 1);

    cockpit_sobj = lbCommonMakeSObjForGObj
    (
        cockpit_gobj,
        lbRelocGetFileData
        (
            Sprite*,
            sMVOpeningSectorFiles[0],
            &llMVOpeningSectorCockpitSprite
        )
    );
    cockpit_sobj->sprite.attr &= ~SP_FASTCOPY;
    cockpit_sobj->sprite.attr |= SP_TRANSPARENT;

    cockpit_sobj->sprite.scalex = 0.25F;
    cockpit_sobj->sprite.scaley = 0.25F;
}

// 0x80132290
void mvOpeningSectorMakeArwings(void)
{
    GObj* arwing_gobj;

    // 0x80132930
    intptr_t anim_joints[/* */] =
    {
        &llMVOpeningSectorArwing0AnimJoint,
        &llMVOpeningSectorArwing1AnimJoint,
        &llMVOpeningSectorArwing2AnimJoint
    };

    s32 i;

    for (i = 0; i < (ARRAY_COUNT(sMVOpeningSectorArwingGObjs) + ARRAY_COUNT(anim_joints)) / 2; i++)
    {
        sMVOpeningSectorArwingGObjs[i] = arwing_gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
        gcSetupCustomDObjs
        (
            arwing_gobj,
            lbRelocGetFileData
            (
                DObjDesc*,
                sMVOpeningSectorFiles[1],
                &llFoxSpecial3EntryArwingDObjDesc
            ),
            NULL,
            nGCMatrixKindTraRotRpyRSca,
            nGCMatrixKindNull,
            nGCMatrixKindNull
        );
        gcAddGObjDisplay(arwing_gobj, gcDrawDObjTreeDLLinksForGObj, 27, GOBJ_PRIORITY_DEFAULT, ~0);
        gcAddAnimJointAll(arwing_gobj, lbRelocGetFileData(AObjEvent32**, sMVOpeningSectorFiles[0], anim_joints[i]), 0.0F);
        gcAddGObjProcess(arwing_gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
    }
}

// 0x801323E0
void mvOpeningSectorCameraProcUpdate(GObj* camera_gobj)
{
    gcPlayCamAnim(camera_gobj);
}

// 0x80132400
void mvOpeningSectorMakeMainCamera(void)
{
    GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        40,
        COBJ_MASK_DLLINK(27),
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
    cobj->projection.persp.far = 30000.0F;

    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningSectorFiles[0], &llMVOpeningSectorCamAnimJoint), 0.0F);
    gcAddGObjProcess(camera_gobj, mvOpeningSectorCameraProcUpdate, nGCProcessKindFunc, 1);
}

// 0x80132500
void mvOpeningSectorMakeWallpaperCamera(void)
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

// 0x801325A0
void mvOpeningSectorMakeCockpitCamera(void)
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
            COBJ_MASK_DLLINK(29),
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

// 0x80132640
void mvOpeningSectorInitTotalTimeTics(void)
{
    sMVOpeningSectorTotalTimeTics = 0;
}

// 0x8013264C
void mvOpeningSectorFuncRun(GObj *gobj)
{
    sMVOpeningSectorTotalTimeTics++;

    if (sMVOpeningSectorTotalTimeTics >= 10)
    {
        if (sMVOpeningSectorUnused0x80132A40 != 0)
        {
            sMVOpeningSectorUnused0x80132A40--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE)
        )
        {
            sMVOpeningSectorUnused0x80132A40 = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();
        }
        if (sMVOpeningSectorTotalTimeTics == 120)
        {
            mvOpeningSectorMakeCockpit();
        }
        if (sMVOpeningSectorTotalTimeTics == 160)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindOpeningStandoff;

            syTaskmanSetLoadScene();
        }
    }
}

// 0x80132738
void mvOpeningSectorFuncStart(void)
{
    s32 i;
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMVOpeningSectorStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningSectorStatusBuffer);
    rl_setup.force_status_buffer = sMVOpeningSectorForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningSectorForceStatusBuffer);

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMVOpeningSectorFileIDs, sMVOpeningSectorFiles);
    gcMakeGObjSPAfter(0, mvOpeningSectorFuncRun, 0, GOBJ_PRIORITY_DEFAULT);

    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));

    mvOpeningSectorInitTotalTimeTics();
    mvOpeningSectorMakeMainCamera();
    mvOpeningSectorMakeWallpaperCamera();
    mvOpeningSectorMakeCockpitCamera();
    mvOpeningSectorMakeWallpaper();
    mvOpeningSectorMakeGreatFox();
    mvOpeningSectorMakeArwings();

    scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);

    while (sySchedulerGetTicCount() < 3420)
    {
        continue;
    }
    func_800269C0_275C0(nSYAudioFGMOpeningSectorAmbient);
}

// 0x8013293C
SYVideoSetup dMVOpeningSectorVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132958
SYTaskmanSetup mvOpeningSectorTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                  // Frame draw function
        &ovl50_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningSectorFuncLights,      // Pre-render function
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
    
    mvOpeningSectorFuncStart            // Task start function
};

// 0x80132898
void mvOpeningSectorStartScene(void)
{
    dMVOpeningSectorVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMVOpeningSectorVideoSetup);

    mvOpeningSectorTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl50_BSS_END);
    syTaskmanStartTask(&mvOpeningSectorTaskmanSetup);
}
