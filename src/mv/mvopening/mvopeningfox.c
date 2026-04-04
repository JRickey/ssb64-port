

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

// 0x8018E090
CObjDesc dMVOpeningFoxCObjDescStart = { { -400.0F, 320.0F, 100.0F }, { 0.0F, 320.0F, 0.0F }, 0.0F };

// 0x8018E0AC
CObjDesc dMVOpeningFoxCObjDescEnd = { { -3000.0F, 300.0F, 250.0F }, { 0.0F, 300.0F,-200.0F }, 0.7F };

// 0x8018E0C8
FTKeyEvent dMVOpeningFoxKeyEvents[/* */] =
{
	FTKEY_EVENT_STICK(-50, 0, 1),     // 2001, CE00
	FTKEY_EVENT_BUTTON(B_BUTTON, 1),  // 1001, 4000
	FTKEY_EVENT_BUTTON(0, 12),        // 100C, 0000
	FTKEY_EVENT_BUTTON(B_BUTTON, 1),  // 1001, 4000
	FTKEY_EVENT_BUTTON(0, 12),        // 100C, 0000
	FTKEY_EVENT_END()                 // 0000
};

// 0x8018E0E0
u32 dMVOpeningFoxFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E1F0
s32 sMVOpeningFoxPad0x8018E1F0[2];

// 0x8018E1F8
s32 sMVOpeningFoxTotalTimeTics;

// 0x8018E1FC
GObj *sMVOpeningFoxNameGObj;

// 0x8018E200
GObj *sMVOpeningFoxFighterGObj;

// 0x8018E204
s32 sMVOpeningFoxPad0x8018E204;

// 0x8018E208
GObj *sMVOpeningFoxStageCameraGObj;

// 0x8018E20C
void *sMVOpeningFoxFigatreeHeap;

// 0x8018E210
f32 sMVOpeningFoxPosedFighterSpeed;

// 0x8018E214
s32 sMVOpeningFoxPad0x8018E214;

// 0x8018E218
CObjDesc sMVOpeningFoxAdjustedStartCObjDesc;

// 0x8018E238
CObjDesc sMVOpeningFoxAdjustedEndCObjDesc;

// 0x8018E258
LBFileNode sMVOpeningFoxStatusBuffer[48];

// 0x8018E3D8
LBFileNode sMVOpeningFoxForceStatusBuffer[7];

// 0x8018E410
void *sMVOpeningFoxFiles[ARRAY_COUNT(dMVOpeningFoxFileIDs)];

// 0x8018E418
SCBattleState sMVOpeningFoxBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningFoxSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningFoxStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningFoxStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningFoxForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningFoxForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningFoxFileIDs, sMVOpeningFoxFiles);
}

// 0x8018D160
void mvOpeningFoxInitName(SObj *sobj)
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
void mvOpeningFoxMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterFSprite,
		&llIFCommonAnnounceCommonLetterOSprite,
		&llIFCommonAnnounceCommonLetterXSprite,
		0x0
	};
	Vec2f pos[/* */] =
	{
		{  0.0F, 0.0F },
		{ 30.0F, 0.0F },
		{ 75.0F, 0.0F }
	};
	s32 i;

	sMVOpeningFoxNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningFoxFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		
		sobj->pos.x = pos[i].x + 110.0F;
		sobj->pos.y = pos[i].y + 100.0F;

		mvOpeningFoxInitName(sobj);
	}
}

// 0x8018D314
void mvOpeningFoxMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningFoxTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningFoxAdjustedEndCObjDesc.eye.x - sMVOpeningFoxAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningFoxAdjustedEndCObjDesc.eye.y - sMVOpeningFoxAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningFoxAdjustedEndCObjDesc.eye.z - sMVOpeningFoxAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningFoxAdjustedEndCObjDesc.at.x - sMVOpeningFoxAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningFoxAdjustedEndCObjDesc.at.y - sMVOpeningFoxAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningFoxAdjustedEndCObjDesc.at.z - sMVOpeningFoxAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningFoxAdjustedEndCObjDesc.upx - sMVOpeningFoxAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D40C
void mvOpeningFoxMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningFoxAdjustedStartCObjDesc = dMVOpeningFoxCObjDescStart;
	sMVOpeningFoxAdjustedEndCObjDesc = dMVOpeningFoxCObjDescEnd;

	sMVOpeningFoxStageCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningFoxStageCameraGObj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 210.0F, 230.0F);

	cobj->projection.persp.aspect = 10.0F / 11.0F;

	gcEndProcessAll(sMVOpeningFoxStageCameraGObj);
	gcAddGObjProcess(sMVOpeningFoxStageCameraGObj, mvOpeningFoxMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningFoxAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningFoxAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningFoxAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningFoxAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningFoxAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningFoxAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningFoxAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningFoxAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningFoxAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningFoxAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningFoxAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningFoxAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningFoxAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningFoxAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningFoxAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningFoxAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningFoxAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningFoxAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningFoxAdjustedStartCObjDesc.upx;
}

// 0x8018D61C
void mvOpeningFoxMakeMotionWindow(void)
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
	mvOpeningFoxMakeMotionCamera(pos);
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
		desc.lr = -1;
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

		sMVOpeningFoxFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningFoxKeyEvents);
	}
}

// 0x8018D84C
void mvOpeningFoxPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x3C, 0x28, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 210, 10, 310, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D94C
void mvOpeningFoxMakePosedWallpaper(void)
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
		mvOpeningFoxPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D998
void mvOpeningFoxPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningFoxTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningFoxPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningFoxPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningFoxPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningFoxTotalTimeTics > 15) && (sMVOpeningFoxTotalTimeTics < 45))
	{
		sMVOpeningFoxPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningFoxTotalTimeTics > 45) && (sMVOpeningFoxTotalTimeTics < 60))
	{
		sMVOpeningFoxPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.y -= sMVOpeningFoxPosedFighterSpeed;
}

// 0x8018DA68
void mvOpeningFoxMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindFox;
	desc.costume = ftParamGetCostumeCommonID(nFTKindFox, 0);
	desc.figatree_heap = sMVOpeningFoxFigatreeHeap;
	desc.pos.x = 0.0F;
	desc.pos.y = 600.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningFoxPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB68
void mvOpeningFoxMakeNameCamera(void)
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

// 0x8018DC08
void mvOpeningFoxMakePosedFighterCamera(void)
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

	syRdpSetViewport(&cobj->viewport, 210.0F, 10.0F, 310.0F, 230.0F);
	
	cobj->projection.persp.aspect = 5.0F / 11.0F;

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningFoxFiles[1], &llMVOpeningCommonFoxCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DCF0
void mvOpeningFoxMakePosedWallpaperCamera(void)
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
	syRdpSetViewport(&cobj->viewport, 210.0F, 10.0F, 310.0F, 230.0F);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
}

// 0x8018DD98
void mvOpeningFoxFuncRun(GObj *gobj)
{
	sMVOpeningFoxTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}
	if (sMVOpeningFoxTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningFoxNameGObj);
		mvOpeningFoxMakeMotionWindow();
		mvOpeningFoxMakePosedWallpaper();
		mvOpeningFoxMakePosedFighter();
	}
	if (sMVOpeningFoxTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningPikachu;
		syTaskmanSetLoadScene();
	}
}

// 0x8018DE54
void mvOpeningFoxInitVars(void)
{
	sMVOpeningFoxTotalTimeTics = 0;
}

// 0x8018DE60
void mvOpeningFoxFuncStart(void)
{
	sMVOpeningFoxBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningFoxBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindSector;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindFox;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningFoxSetupFiles();
	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningFoxFuncRun, nGCCommonLinkIDMovie, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningFoxInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindFox);

	sMVOpeningFoxFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mvOpeningFoxMakeNameCamera();
	mvOpeningFoxMakePosedWallpaperCamera();
	mvOpeningFoxMakePosedFighterCamera();
	mvOpeningFoxMakeName();

	while (sySchedulerGetTicCount() < 2055)
	{
		continue;
	}
}

// 0x8018DFE0
void mvOpeningFoxFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E110
SYVideoSetup dMVOpeningFoxVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E12C
SYTaskmanSetup dMVOpeningFoxTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl39_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningFoxFuncLights,    		// Pre-render function
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
    
    mvOpeningFoxFuncStart          		// Task start function
};

// 0x8018E02C
void mvOpeningFoxStartScene(void)
{
	dMVOpeningFoxVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningFoxVideoSetup);

	dMVOpeningFoxTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl39_BSS_END);
	syTaskmanStartTask(&dMVOpeningFoxTaskmanSetup);
}
