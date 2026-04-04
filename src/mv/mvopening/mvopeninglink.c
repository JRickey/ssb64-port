#include <ft/fighter.h>
#include <if/interface.h>
#include <gr/ground.h>
#include <mv/movie.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern u32 sySchedulerGetTicCount();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018E070
CObjDesc dMVOpeningLinkCObjDescStart = { { -800.0F, 180.0F, 800.0F }, { 0.0F, 180.0F, 0.0F }, 0.0F };

// 0x8018E08C
CObjDesc dMVOpeningLinkCObjDescEnd = { { 200.0F, 0.0F, 400.0F }, { 0.0F, 240.0F, 0.0F }, 0.4F };

// 0x8018E0A8
FTKeyEvent dMVOpeningLinkKeyEvents[/* */] =
{
    FTKEY_EVENT_BUTTON(L_TRIG, 1),  // 0x1001, 0x0020
    FTKEY_EVENT_END()               // 0x0000
};

// 0x8018E0B0
u32 dMVOpeningLinkFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E1C0
s32 sMVOpeningLinkPad0x8018E1C0[2];

// 0x8018E1C8
s32 sMVOpeningLinkTotalTimeTics;

// 0x8018E1CC
GObj *sMVOpeningLinkNameGObj;

// 0x8018E1D0
GObj *sMVOpeningLinkFighterGObj;

// 0x8018E1D4
s32 sMVOpeningLinkPad0x8018E1D4;

// 0x8018E1D8
GObj *sMVOpeningLinkStageCameraGObj;

// 0x8018E1DC
void *sMVOpeningLinkFigatreeHeap;

// 0x8018E1E0
f32 sMVOpeningLinkPosedFighterSpeed;

// 0x8018E1E4
s32 sMVOpeningLinkPad0x8018E1E4;

// 0x8018E1E8
CObjDesc sMVOpeningLinkAdjustedStartCObjDesc;

// 0x8018E208
CObjDesc sMVOpeningLinkAdjustedEndCObjDesc;

// 0x8018E228
LBFileNode sMVOpeningLinkStatusBuffer[48];

// 0x8018E3A8
LBFileNode sMVOpeningLinkForceStatusBuffer[7];

// 0x8018E3E0
void *sMVOpeningLinkFiles[ARRAY_COUNT(dMVOpeningLinkFileIDs)];

// 0x8018E3E8
SCBattleState sMVOpeningLinkBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningLinkSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningLinkStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningLinkStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningLinkForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningLinkForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningLinkFileIDs, sMVOpeningLinkFiles);
}

// 0x8018D160
void mvOpeningLinkInitName(SObj *sobj)
{
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;

	sobj->envcolor.r = 0xFF;
	sobj->envcolor.g = 0xFF;
	sobj->envcolor.b = 0xFF;

	sobj->sprite.red = 0xFF;
	sobj->sprite.green = 0xFF;
	sobj->sprite.blue = 0xFF;
}

// 0x8018D194
void mvOpeningLinkMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterLSprite,
		&llIFCommonAnnounceCommonLetterISprite,
		&llIFCommonAnnounceCommonLetterNSprite,
		&llIFCommonAnnounceCommonLetterKSprite,
		0x0
	};
	f32 pos_x[/* */] =
	{
		0.0F, 30.0F, 45.0F, 80.0F
	};
	s32 i;

	sMVOpeningLinkNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningLinkFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		
		sobj->pos.x = pos_x[i] + 100.0F;
		sobj->pos.y = 100.0F;

		mvOpeningLinkInitName(sobj);
	}
}

// 0x8018D2F4
void mvOpeningLinkMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningLinkTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningLinkAdjustedEndCObjDesc.eye.x - sMVOpeningLinkAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningLinkAdjustedEndCObjDesc.eye.y - sMVOpeningLinkAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningLinkAdjustedEndCObjDesc.eye.z - sMVOpeningLinkAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningLinkAdjustedEndCObjDesc.at.x - sMVOpeningLinkAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningLinkAdjustedEndCObjDesc.at.y - sMVOpeningLinkAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningLinkAdjustedEndCObjDesc.at.z - sMVOpeningLinkAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningLinkAdjustedEndCObjDesc.upx - sMVOpeningLinkAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D3EC
void mvOpeningLinkMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningLinkAdjustedStartCObjDesc = dMVOpeningLinkCObjDescStart;
	sMVOpeningLinkAdjustedEndCObjDesc = dMVOpeningLinkCObjDescEnd;

	sMVOpeningLinkStageCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningLinkStageCameraGObj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 90.0F, 310.0F, 230.0F);

	cobj->projection.persp.aspect = 15.0F / 7.0F;

	gcEndProcessAll(sMVOpeningLinkStageCameraGObj);
	gcAddGObjProcess(sMVOpeningLinkStageCameraGObj, mvOpeningLinkMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningLinkAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningLinkAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningLinkAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningLinkAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningLinkAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningLinkAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningLinkAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningLinkAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningLinkAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningLinkAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningLinkAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningLinkAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningLinkAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningLinkAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningLinkAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningLinkAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningLinkAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningLinkAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningLinkAdjustedStartCObjDesc.upx;
}

// 0x8018D5FC
void mvOpeningLinkMakeMotionWindow(void)
{
	GObj *fighter_gobj;
	s32 i;
	s32 pos_ids[3];
	Vec3f pos;

	grWallpaperMakeDecideKind();
	grCommonSetupInitAll();

	if (mpCollisionGetMapObjCountKind(nMPMapObjKindMoviePlayer1) != 1)
	{
		while (TRUE)
		{
			syDebugPrintf("wrong number of mapobject\n");
			scManagerRunPrintGObjStatus();
		}
	}
	mpCollisionGetMapObjIDsKind(nMPMapObjKindMoviePlayer1, pos_ids);
	mpCollisionGetMapObjPositionID(pos_ids[0], &pos);
	mvOpeningLinkMakeMotionCamera(pos);
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
		desc.pos.x = pos.x;
		desc.pos.y = pos.y;
		desc.pos.z = pos.z;
		desc.lr = +1;
		desc.team = gSCManagerBattleState->players[i].team;
		desc.player = i;
		desc.detail = nFTPartsDetailHigh;
		desc.costume = gSCManagerBattleState->players[i].costume;
		desc.handicap = gSCManagerBattleState->players[i].handicap;
		desc.level = gSCManagerBattleState->players[i].level;
		desc.stock_count = gSCManagerBattleState->stocks;
		desc.damage = 0;
		desc.pkind = gSCManagerBattleState->players[i].pkind;
		desc.controller = &gSYControllerDevices[i];
		desc.figatree_heap = ftManagerAllocFigatreeHeapKind(gSCManagerBattleState->players[i].fkind);

		sMVOpeningLinkFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningLinkKeyEvents);
	}
}

// 0x8018D824
void mvOpeningLinkPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x96, 0x78, 0xB4, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 90);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D924
void mvOpeningLinkMakePosedWallpaper(void)
{
	gcAddGObjDisplay
	(
		gcMakeGObjSPAfter
		(
			0,
			NULL,
			19,
			GOBJ_PRIORITY_DEFAULT
		),
		mvOpeningLinkPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D970
void mvOpeningLinkPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningLinkTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningLinkPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningLinkPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningLinkPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningLinkTotalTimeTics > 15) && (sMVOpeningLinkTotalTimeTics < 45))
	{
		sMVOpeningLinkPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningLinkTotalTimeTics > 45) && (sMVOpeningLinkTotalTimeTics < 60))
	{
		sMVOpeningLinkPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.x -= sMVOpeningLinkPosedFighterSpeed;
}

// 0x8018DA40
void mvOpeningLinkMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindLink;
	desc.costume = ftParamGetCostumeCommonID(nFTKindLink, 0);
	desc.figatree_heap = sMVOpeningLinkFigatreeHeap;
	desc.pos.x = 600.0F;
	desc.pos.y = 0.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningLinkPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB40
void mvOpeningLinkMakeNameCamera(void)
{
	GObj *camera_gobj = gcMakeCameraGObj
	(
		nGCCommonKindSceneCamera,
		NULL,
		nGCCommonLinkIDSceneCamera,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSprite,
		80,
		COBJ_MASK_DLLINK(27),
		~0,
		FALSE,
		nGCProcessKindFunc,
		NULL,
		1,
		FALSE
	);
	CObj *cobj = CObjGetStruct(camera_gobj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x8018DBE0
void mvOpeningLinkMakePosedFighterCamera(void)
{
	GObj *camera_gobj = gcMakeCameraGObj
	(
		nGCCommonKindSceneCamera,
		NULL,
		nGCCommonLinkIDSceneCamera,
		GOBJ_PRIORITY_DEFAULT,
		func_80017EC0,
		10,
		COBJ_MASK_DLLINK(26),
		~0,
		TRUE,
		nGCProcessKindFunc,
		NULL,
		1,
		FALSE
	);
	CObj *cobj = CObjGetStruct(camera_gobj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 90.0F);
	
	cobj->projection.persp.aspect = 26.25F / 7.0F;

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningLinkFiles[1], &llMVOpeningCommonLinkCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DCD0
void mvOpeningLinkMakePosedWallpaperCamera(void)
{
	CObj *cobj;
	GObj *camera_gobj = gcMakeCameraGObj
	(
		nGCCommonKindSceneCamera,
		NULL,
		nGCCommonLinkIDSceneCamera,
		GOBJ_PRIORITY_DEFAULT,
		func_80017EC0,
		20,
		COBJ_MASK_DLLINK(28),
		~0,
		FALSE,
		nGCProcessKindFunc,
		NULL,
		1,
		FALSE
	);
	cobj = CObjGetStruct(camera_gobj);
	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 90.0F);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
}

// 0x8018DD80
void mvOpeningLinkFuncRun(GObj *gobj)
{
	sMVOpeningLinkTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}

	if (sMVOpeningLinkTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningLinkNameGObj);
		mvOpeningLinkMakeMotionWindow();
		mvOpeningLinkMakePosedWallpaper();
		mvOpeningLinkMakePosedFighter();
	}
	if (sMVOpeningLinkTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningSamus;
		syTaskmanSetLoadScene();
	}
}

// 0x8018DE3C
void mvOpeningLinkInitVars(void)
{
	sMVOpeningLinkTotalTimeTics = 0;
}

// 0x8018DE48
void mvOpeningLinkFuncStart(void)
{
	sMVOpeningLinkBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningLinkBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindHyrule;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindLink;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningLinkSetupFiles();
	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningLinkFuncRun, nGCCommonLinkIDMovie, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningLinkInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindLink);

	sMVOpeningLinkFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mvOpeningLinkMakeNameCamera();
	mvOpeningLinkMakePosedWallpaperCamera();
	mvOpeningLinkMakePosedFighterCamera();
	mvOpeningLinkMakeName();

	while (sySchedulerGetTicCount() < 1695)
	{
		continue;
	}
}

// 0x8018DFCC
void mvOpeningLinkFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E0DC
SYVideoSetup dMVOpeningLinkVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E0F8
SYTaskmanSetup dMVOpeningLinkTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl40_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningLinkFuncLights,    	// Pre-render function
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
    
    mvOpeningLinkFuncStart          	// Task start function
};

// 0x8018E018
void mvOpeningLinkStartScene(void)
{
	dMVOpeningLinkVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningLinkVideoSetup);

	dMVOpeningLinkTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl40_BSS_END);
	syTaskmanStartTask(&dMVOpeningLinkTaskmanSetup);
}
