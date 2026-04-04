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

// 0x8018E0C0
CObjDesc dMVOpeningPikachuStartCObjDesc = { { 0.0F, 0.0F, 20000.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F };

// 0x8018E0DC
CObjDesc dMVOpeningPikachuEndCObjDesc = { { 50.0F, -1640.0F, 1000.0F }, { 50.0F, -1640.0F, 0.0F }, 0.0F };

// 0x8018E0F8 - Bruh?
FTKeyEvent dMVOpeningPikachuKeyEvents[/* */] =
{
	FTKEY_EVENT_END()
};

// 0x8018E0FC
u32 dMVOpeningPikachuFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E220
s32 sMVOpeningPikachuPad0x8018E220[2];

// 0x8018E228
s32 sMVOpeningPikachuTotalTimeTics;

// 0x8018E22C
GObj *sMVOpeningPikachuNameGObj;

// 0x8018E230
GObj *sMVOpeningPikachuFighterGObj;

// 0x8018E234
s32 sMVOpeningPikachuPad0x8018E234;

// 0x8018E238
GObj *sMVOpeningPikachuMotionCameraGObj;

// 0x8018E23C
void *sMVOpeningPikachuFigatreeHeap;

// 0x8018E240
f32 sMVOpeningPikachuPosedFighterSpeed;

// 0x8018E244
s32 sMVOpeningPikachuPad0x8018E244;

// 0x8018E248
CObjDesc sMVOpeningPikachuAdjustedStartCObjDesc;

// 0x8018E268
CObjDesc sMVOpeningPikachuAdjustedEndCObjDesc;

// 0x8018E288
LBFileNode sMVOpeningPikachuStatusBuffer[48];

// 0x8018E408
LBFileNode sMVOpeningPikachuForceStatusBuffer[7];

// 0x8018E440
void *sMVOpeningPikachuFiles[ARRAY_COUNT(dMVOpeningPikachuFileIDs)];

// 0x8018E448
SCBattleState sMVOpeningPikachuBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningPikachuSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningPikachuStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningPikachuStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningPikachuForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningPikachuForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningPikachuFileIDs, sMVOpeningPikachuFiles);
}

// 0x8018D160
void mvOpeningPikachuInitName(SObj *sobj)
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
void mvOpeningPikachuMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterPSprite,
		&llIFCommonAnnounceCommonLetterISprite,
		&llIFCommonAnnounceCommonLetterKSprite,
		&llIFCommonAnnounceCommonLetterASprite,
		&llIFCommonAnnounceCommonLetterCSprite,
		&llIFCommonAnnounceCommonLetterHSprite,
		&llIFCommonAnnounceCommonLetterUSprite,
		0x0
	};
	f32 pos_x[/* */] =
	{
		0.0F, 30.0F, 45.0F, 75.0F, 110.0F, 140.0F, 170.0F
	};
	s32 i;

	sMVOpeningPikachuNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningPikachuFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		sobj->pos.x = pos_x[i] + 65.0F;
		sobj->pos.y = 100.0F;

		mvOpeningPikachuInitName(sobj);
	}
}

// 0x8018D334
void mvOpeningPikachuMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningPikachuTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningPikachuAdjustedEndCObjDesc.eye.x - sMVOpeningPikachuAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningPikachuAdjustedEndCObjDesc.eye.y - sMVOpeningPikachuAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningPikachuAdjustedEndCObjDesc.eye.z - sMVOpeningPikachuAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningPikachuAdjustedEndCObjDesc.at.x - sMVOpeningPikachuAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningPikachuAdjustedEndCObjDesc.at.y - sMVOpeningPikachuAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningPikachuAdjustedEndCObjDesc.at.z - sMVOpeningPikachuAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningPikachuAdjustedEndCObjDesc.upx - sMVOpeningPikachuAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D42C
void mvOpeningPikachuMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningPikachuAdjustedStartCObjDesc = dMVOpeningPikachuStartCObjDesc;
	sMVOpeningPikachuAdjustedEndCObjDesc = dMVOpeningPikachuEndCObjDesc;

	sMVOpeningPikachuMotionCameraGObj = gmCameraMakeMovieCamera(NULL);

	cobj = CObjGetStruct(sMVOpeningPikachuMotionCameraGObj);

	syRdpSetViewport(&cobj->viewport, 110.0F, 10.0F, 310.0F, 230.0F);

	cobj->projection.persp.aspect = 10.0F / 11.0F;

	gcEndProcessAll(sMVOpeningPikachuMotionCameraGObj);
	gcAddGObjProcess(sMVOpeningPikachuMotionCameraGObj, mvOpeningPikachuMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningPikachuAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningPikachuAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningPikachuAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningPikachuAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningPikachuAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningPikachuAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningPikachuAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningPikachuAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningPikachuAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningPikachuAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningPikachuAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningPikachuAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningPikachuAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningPikachuAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningPikachuAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningPikachuAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningPikachuAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningPikachuAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningPikachuAdjustedStartCObjDesc.upx;
}

// 0x8018D634
void mvOpeningPikachuMakeMotionWindow(void)
{
	GObj* fighter_gobj;
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
	mvOpeningPikachuMakeMotionCamera(pos);
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

		sMVOpeningPikachuFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningPikachuKeyEvents);
	}
}

// 0x8018D864
void mvOpeningPikachuPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x6E, 0xAA, 0x6E, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 110, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D964
void mvOpeningPikachuMakePosedWallpaper(void)
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
		mvOpeningPikachuPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D9B0
void mvOpeningPikachuPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningPikachuTotalTimeTics)
	{
	default:
		break;
		
	case 15:
		sMVOpeningPikachuPosedFighterSpeed = 17.0F;
		break;
	
	case 45:
		sMVOpeningPikachuPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningPikachuPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningPikachuTotalTimeTics > 15) && (sMVOpeningPikachuTotalTimeTics < 45))
	{
		sMVOpeningPikachuPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningPikachuTotalTimeTics > 45) && (sMVOpeningPikachuTotalTimeTics < 60))
	{
		sMVOpeningPikachuPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.y += sMVOpeningPikachuPosedFighterSpeed;
}

// 0x8018DA80
void mvOpeningPikachuMakePosedFighter(void)
{
	GObj* fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindPikachu;
	desc.costume = ftParamGetCostumeCommonID(nFTKindPikachu, 0);
	desc.figatree_heap = sMVOpeningPikachuFigatreeHeap;
	desc.pos.x = 0.0F;
	desc.pos.y = -600.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);

	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningPikachuPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB80
void mvOpeningPikachuMakeNameCamera(void)
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

// 0x8018DC20
void mvOpeningPikachuMakePosedFighterCamera(void)
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

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 110.0F, 230.0F);

	cobj->projection.persp.aspect = 5.0F / 11.0F;

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningPikachuFiles[1], &llMVOpeningCommonPikachuCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DD10
void mvOpeningPikachuMakePosedWallpaperCamera(void)
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
	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 110.0F, 230.0F);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
}

// 0x8018DDC0
void mvOpeningPikachuFuncRun(GObj *gobj)
{
	sMVOpeningPikachuTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}
	if (sMVOpeningPikachuTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningPikachuNameGObj);
		mvOpeningPikachuMakeMotionWindow();
		mvOpeningPikachuMakePosedWallpaper();
		mvOpeningPikachuMakePosedFighter();
	}
	if (sMVOpeningPikachuTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningRun;
		syTaskmanSetLoadScene();
	}
}

// 0x8018DE7C
void mvOpeningPikachuInitVars(void)
{
	sMVOpeningPikachuTotalTimeTics = 0;
}

// 0x8018DE88
void mvOpeningPikachuFuncStart(void)
{
	sMVOpeningPikachuBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningPikachuBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindYamabuki;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindPikachu;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningPikachuSetupFiles();
	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningPikachuFuncRun, nGCCommonLinkIDMovie, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningPikachuInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindPikachu);

	sMVOpeningPikachuFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mvOpeningPikachuMakeNameCamera();
	mvOpeningPikachuMakePosedWallpaperCamera();
	mvOpeningPikachuMakePosedFighterCamera();
	mvOpeningPikachuMakeName();

	while (sySchedulerGetTicCount() < 2145)
	{
		continue;
	}
}

// 0x8018E010
void mvOpeningPikachuFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E140
SYVideoSetup dMVOpeningPikachuVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E15C
SYTaskmanSetup dMVOpeningPikachuTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl42_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningPikachuFuncLights,    	// Pre-render function
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
    
    mvOpeningPikachuFuncStart          	// Task start function
};

// 0x8018E05C
void mvOpeningPikachuStartScene(void)
{
	dMVOpeningPikachuVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningPikachuVideoSetup);

	dMVOpeningPikachuTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl42_BSS_END);
	syTaskmanStartTask(&dMVOpeningPikachuTaskmanSetup);
}
