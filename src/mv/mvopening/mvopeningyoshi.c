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
CObjDesc dMVOpeningYoshiCObjDescStart = { { 1200.0F, 150.0F, 1000.0F }, { 100.0F, 200.0F, 0.0F }, 0.0F };

// 0x8018E0DC
CObjDesc dMVOpeningYoshiCObjDescEnd = { { 2000.0F, 100.0F, 600.0F }, { 1300.0F, 100.0F,-100.0F }, 0.0F };

// 0x8018E0F8
FTKeyEvent dMVOpeningYoshiKeyEvents[/* */] =
{
	FTKEY_EVENT_STICK(I_CONTROLLER_RANGE_MAX, 0, 20),   // 2014, 5000
	FTKEY_EVENT_BUTTON(Z_TRIG, 1),                      // 1001, 2000
	FTKEY_EVENT_BUTTON(A_BUTTON, 1),                    // 1001, 8000
	FTKEY_EVENT_END()                                   // 0000
};

// 0x8018E108
u32 dMVOpeningYoshiFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E220
s32 sMVOpeningYoshiPad0x8018E220[2];

// 0x8018E228
s32 sMVOpeningYoshiTotalTimeTics;

// 0x8018E22C
GObj *sMVOpeningYoshiNameGObj;

// 0x8018E230
GObj *sMVOpeningYoshiFighterGObj;

// 0x8018E234
s32 sMVOpeningYoshiPad0x8018E234;

// 0x8018E238
GObj *sMVOpeningYoshiStageCameraGObj;

// 0x8018E23C
void *sMVOpeningYoshiFigatreeHeap;

// 0x8018E240
f32 sMVOpeningYoshiPosedFighterSpeed;

// 0x8018E244
s32 sMVOpeningYoshiPad0x8018E244;

// 0x8018E248
CObjDesc sMVOpeningYoshiAdjustedStartCObjDesc;

// 0x8018E268
CObjDesc sMVOpeningYoshiAdjustedEndCObjDesc;

// 0x8018E288
LBFileNode sMVOpeningYoshiStatusBuffer[48];

// 0x8018E408
LBFileNode sMVOpeningYoshiForceStatusBuffer[7];

// 0x8018E440
void *sMVOpeningYoshiFiles[ARRAY_COUNT(dMVOpeningYoshiFileIDs)];

// 0x8018E448
SCBattleState sMVOpeningYoshiBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningYoshiSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningYoshiStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningYoshiStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningYoshiForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningYoshiForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningYoshiFileIDs, sMVOpeningYoshiFiles);
}

// 0x8018D160
void mvOpeningYoshiInitName(SObj *sobj)
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
void mvOpeningYoshiMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterYSprite,
		&llIFCommonAnnounceCommonLetterOSprite,
		&llIFCommonAnnounceCommonLetterSSprite,
		&llIFCommonAnnounceCommonLetterHSprite,
		&llIFCommonAnnounceCommonLetterISprite,
		0x0
	};
	f32 pos_x[/* */] =
	{
		0.0F, 30.0F, 65.0F, 95.0F, 128.0F
	};
	s32 i;

	sMVOpeningYoshiNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningYoshiFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		
		sobj->pos.x = pos_x[i] + 80.0F;
		sobj->pos.y = 100.0F;

		mvOpeningYoshiInitName(sobj);
	}
}

// 0x8018D314
void mvOpeningYoshiMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningYoshiTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningYoshiAdjustedEndCObjDesc.eye.x - sMVOpeningYoshiAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningYoshiAdjustedEndCObjDesc.eye.y - sMVOpeningYoshiAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningYoshiAdjustedEndCObjDesc.eye.z - sMVOpeningYoshiAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningYoshiAdjustedEndCObjDesc.at.x - sMVOpeningYoshiAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningYoshiAdjustedEndCObjDesc.at.y - sMVOpeningYoshiAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningYoshiAdjustedEndCObjDesc.at.z - sMVOpeningYoshiAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningYoshiAdjustedEndCObjDesc.upx - sMVOpeningYoshiAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D40C
void mvOpeningYoshiMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningYoshiAdjustedStartCObjDesc = dMVOpeningYoshiCObjDescStart;
	sMVOpeningYoshiAdjustedEndCObjDesc = dMVOpeningYoshiCObjDescEnd;

	sMVOpeningYoshiStageCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningYoshiStageCameraGObj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 150.0F);

	cobj->projection.persp.aspect = 15.0F / 7.0F;

	gcEndProcessAll(sMVOpeningYoshiStageCameraGObj);
	gcAddGObjProcess(sMVOpeningYoshiStageCameraGObj, mvOpeningYoshiMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningYoshiAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningYoshiAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningYoshiAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningYoshiAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningYoshiAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningYoshiAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningYoshiAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningYoshiAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningYoshiAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningYoshiAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningYoshiAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningYoshiAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningYoshiAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningYoshiAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningYoshiAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningYoshiAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningYoshiAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningYoshiAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningYoshiAdjustedStartCObjDesc.upx;
}

// 0x8018D61C
void mvOpeningYoshiMakeMotionWindow(void)
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

	pos.x += -1000.0F;
	pos.y += 70.0F;

	mvOpeningYoshiMakeMotionCamera(pos);
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

		sMVOpeningYoshiFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningYoshiKeyEvents);
	}
}

// 0x8018D874
void mvOpeningYoshiPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xBE, 0x5A, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 150, 310, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D974
void mvOpeningYoshiMakePosedWallpaper(void)
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
		mvOpeningYoshiPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D9C0
void mvOpeningYoshiPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningYoshiTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningYoshiPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningYoshiPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningYoshiPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningYoshiTotalTimeTics > 15) && (sMVOpeningYoshiTotalTimeTics < 45))
	{
		sMVOpeningYoshiPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningYoshiTotalTimeTics > 45) && (sMVOpeningYoshiTotalTimeTics < 60))
	{
		sMVOpeningYoshiPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.x += sMVOpeningYoshiPosedFighterSpeed;
}

// 0x8018DA90
void mvOpeningYoshiMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindYoshi;
	desc.costume = ftParamGetCostumeCommonID(nFTKindYoshi, 0);
	desc.figatree_heap = sMVOpeningYoshiFigatreeHeap;
	desc.pos.x = -600.0F;
	desc.pos.y = 0.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningYoshiPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB90
void mvOpeningYoshiMakeNameCamera(void)
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

// 0x8018DC30
void mvOpeningYoshiMakePosedFighterCamera(void)
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

	syRdpSetViewport(&cobj->viewport, 10.0F, 150.0F, 310.0F, 230.0F);
	
	cobj->projection.persp.aspect = 26.25F / 7.0F;

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningYoshiFiles[1], &llMVOpeningCommonYoshiCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DD18
void mvOpeningYoshiMakePosedWallpaperCamera(void)
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
	syRdpSetViewport(&cobj->viewport, 10.0F, 150.0F, 310.0F, 230.0F);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
}

// 0x8018DDC0
void mvOpeningYoshiFuncRun(GObj *gobj)
{
	sMVOpeningYoshiTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}
	if (sMVOpeningYoshiTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningYoshiNameGObj);
		mvOpeningYoshiMakeMotionWindow();
		mvOpeningYoshiMakePosedWallpaper();
		mvOpeningYoshiMakePosedFighter();
	}
	if (sMVOpeningYoshiTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningKirby;
		syTaskmanSetLoadScene();
	}
}

// 0x8018DE7C
void mvOpeningYoshiInitVars(void)
{
	sMVOpeningYoshiTotalTimeTics = 0;
}

// 0x8018DE88
void mvOpeningYoshiFuncStart(void)
{
	sMVOpeningYoshiBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningYoshiBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindYoster;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindYoshi;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningYoshiSetupFiles();
	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningYoshiFuncRun, nGCCommonLinkIDMovie, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningYoshiInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindYoshi);

	sMVOpeningYoshiFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mvOpeningYoshiMakeNameCamera();
	mvOpeningYoshiMakePosedWallpaperCamera();
	mvOpeningYoshiMakePosedFighterCamera();
	mvOpeningYoshiMakeName();

	while (sySchedulerGetTicCount() < 1875)
	{
		continue;
	}
}

// 0x8018E010
void mvOpeningYoshiFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E13C
SYVideoSetup dMVOpeningYoshiVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E158
SYTaskmanSetup dMVOpeningYoshiTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl41_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningYoshiFuncLights,    	// Pre-render function
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
    
    mvOpeningYoshiFuncStart          	// Task start function
};

// 0x8018E05C
void mvOpeningYoshiStartScene(void)
{
	dMVOpeningYoshiVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningYoshiVideoSetup);

	dMVOpeningYoshiTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl41_BSS_END);
	syTaskmanStartTask(&dMVOpeningYoshiTaskmanSetup);
}
