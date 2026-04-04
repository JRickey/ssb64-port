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

// 0x8018E120
CObjDesc dMVOpeningSamusCObjDescStart = { { 400.0F, 1100.0F, 0.0F }, { 0.0F,  200.0F, 0.0F }, 0.6F };

// 0x8018E13C
CObjDesc dMVOpeningSamusCObjDescEnd = { { 1600.0F, 230.0F, 200.0F }, { 0.0F, 200.0F, 0.0F }, 0.6F };

// 0x8018E158
FTKeyEvent dMVOpeningSamusKeyEvents[/* */] =
{
	FTKEY_EVENT_BUTTON(Z_TRIG, 1),   // 1001, 0x2000
	FTKEY_EVENT_BUTTON(A_BUTTON, 1), // 1001, 0x8000
	FTKEY_EVENT_END()                // 0000
};

// 0x8018E164
u32 dMVOpeningSamusFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E270
s32 sMVOpeningSamusPad0x8018E270[2];

// 0x8018E278
s32 sMVOpeningSamusTotalTimeTics;

// 0x8018E27C
GObj *sMVOpeningSamusNameGObj;

// 0x8018E280
GObj *sMVOpeningSamusFighterGObj;

// 0x8018E284
s32 sMVOpeningSamusPad0x8018E284;

// 0x8018E288
GObj *sMVOpeningSamusStageCameraGObj;

// 0x8018E28C
void *sMVOpeningSamusFigatreeHeap;

// 0x8018E290
f32 sMVOpeningSamusPosedFighterSpeed;

// 0x8018E294
s32 sMVOpeningSamusPad0x8018E294;

// 0x8018E288
CObjDesc sMVOpeningSamusAdjustedStartCObjDesc;

// 0x8018E2B8
CObjDesc sMVOpeningSamusAdjustedEndCObjDesc;

// 0x8018E2D8
LBFileNode sMVOpeningSamusStatusBuffer[48];

// 0x8018E458
LBFileNode sMVOpeningSamusForceStatusBuffer[7];

// 0x8018E490
void *sMVOpeningSamusFiles[ARRAY_COUNT(dMVOpeningSamusFileIDs)];

// 0x8018E498
SCBattleState sMVOpeningSamusBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningSamusSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningSamusStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningSamusStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningSamusForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningSamusForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningSamusFileIDs, sMVOpeningSamusFiles);
}

// 0x8018D160
void mvOpeningSamusInitName(SObj *sobj)
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
void mvOpeningSamusMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterSSprite,
		&llIFCommonAnnounceCommonLetterASprite,
		&llIFCommonAnnounceCommonLetterMSprite,
		&llIFCommonAnnounceCommonLetterUSprite,
		&llIFCommonAnnounceCommonLetterSSprite,
		0x0
	};
	f32 pos_x[/* */] =
	{
		0.0F, 30.0F, 70.0F, 110.0F, 140.0F
	};
	s32 i;

	sMVOpeningSamusNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningSamusFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		
		sobj->pos.x = pos_x[i] + 80.0F;
		sobj->pos.y = 100.0F;

		mvOpeningSamusInitName(sobj);
	}
}

// 0x8018D314
void mvOpeningSamusMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningSamusTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningSamusAdjustedEndCObjDesc.eye.x - sMVOpeningSamusAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningSamusAdjustedEndCObjDesc.eye.y - sMVOpeningSamusAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningSamusAdjustedEndCObjDesc.eye.z - sMVOpeningSamusAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningSamusAdjustedEndCObjDesc.at.x - sMVOpeningSamusAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningSamusAdjustedEndCObjDesc.at.y - sMVOpeningSamusAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningSamusAdjustedEndCObjDesc.at.z - sMVOpeningSamusAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningSamusAdjustedEndCObjDesc.upx - sMVOpeningSamusAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D40C
void mvOpeningSamusMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningSamusAdjustedStartCObjDesc = dMVOpeningSamusCObjDescStart;
	sMVOpeningSamusAdjustedEndCObjDesc = dMVOpeningSamusCObjDescEnd;

	sMVOpeningSamusStageCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningSamusStageCameraGObj);

	syRdpSetViewport(&cobj->viewport, 110.0F, 10.0F, 310.0F, 230.0F);

	cobj->projection.persp.aspect = 10.0F / 11.0F;

	gcEndProcessAll(sMVOpeningSamusStageCameraGObj);
	gcAddGObjProcess(sMVOpeningSamusStageCameraGObj, mvOpeningSamusMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningSamusAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningSamusAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningSamusAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningSamusAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningSamusAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningSamusAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningSamusAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningSamusAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningSamusAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningSamusAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningSamusAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningSamusAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningSamusAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningSamusAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningSamusAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningSamusAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningSamusAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningSamusAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningSamusAdjustedStartCObjDesc.upx;
}

// 0x8018D61C
void mvOpeningSamusMakeMotionWindow(void)
{
	GObj *fighter_gobj;
	s32 i, j;
	s32 pos_ids[3];
	Vec3f pos;
	DObj *root_dobj, *next_dobj;
	s32 unused[2];

	grWallpaperMakeDecideKind();
	grCommonSetupInitAll();

	next_dobj = root_dobj = DObjGetStruct(gGRCommonLayerGObjs[1]);

	// This fixes the spot light things on Zebes
	while (next_dobj != NULL)
	{
		for (j = 0; j < next_dobj->xobjs_num; j++)
		{
			if (next_dobj->xobjs[j]->kind == 0x30)
			{
				next_dobj->xobjs[j]->kind = 0x25;
			}
		}
		next_dobj = lbCommonGetTreeDObjNextFromRoot(next_dobj, root_dobj);
	}
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
	mvOpeningSamusMakeMotionCamera(pos);
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

		sMVOpeningSamusFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningSamusKeyEvents);
	}
}

// 0x8018D8B0
void mvOpeningSamusPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0x50, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 110, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D9AC
void mvOpeningSamusMakePosedWallpaper(void)
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
		mvOpeningSamusPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D9F8
void mvOpeningSamusPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningSamusTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningSamusPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningSamusPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningSamusPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningSamusTotalTimeTics > 15) && (sMVOpeningSamusTotalTimeTics < 45))
	{
		sMVOpeningSamusPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningSamusTotalTimeTics > 45) && (sMVOpeningSamusTotalTimeTics < 60))
	{
		sMVOpeningSamusPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.y -= sMVOpeningSamusPosedFighterSpeed;
}

// 0x8018DAC8
void mvOpeningSamusMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindSamus;
	desc.costume = ftParamGetCostumeCommonID(nFTKindSamus, 0);
	desc.figatree_heap = sMVOpeningSamusFigatreeHeap;
	desc.pos.x = 0.0F;
	desc.pos.y = 600.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningSamusPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DBC8
void mvOpeningSamusMakeNameCamera(void)
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

// 0x8018DC68
void mvOpeningSamusMakePosedFighterCamera(void)
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

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningSamusFiles[1], &llMVOpeningCommonSamusCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DD58
void mvOpeningSamusMakePosedWallpaperCamera(void)
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

// 0x8018DE08
void mvOpeningSamusFuncRun(GObj *gobj)
{
	sMVOpeningSamusTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}
	if (sMVOpeningSamusTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningSamusNameGObj);
		mvOpeningSamusMakeMotionWindow();
		mvOpeningSamusMakePosedWallpaper();
		mvOpeningSamusMakePosedFighter();
	}
	if (sMVOpeningSamusTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningYoshi;
		syTaskmanSetLoadScene();
	}
}

// 0x8018DEC4
void mvOpeningSamusInitVars(void)
{
	sMVOpeningSamusTotalTimeTics = 0;
}

// 0x8018DED0
void mvOpeningSamusFuncStart(void)
{
	sMVOpeningSamusBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningSamusBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindZebes;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindSamus;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningSamusSetupFiles();
	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningSamusFuncRun, nGCCommonLinkIDMovie, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningSamusInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);
	ftManagerSetupFilesAllKind(nFTKindSamus);

	sMVOpeningSamusFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mvOpeningSamusMakeNameCamera();
	mvOpeningSamusMakePosedWallpaperCamera();
	mvOpeningSamusMakePosedFighterCamera();
	mvOpeningSamusMakeName();

	while (sySchedulerGetTicCount() < 1785)
	{
		continue;
	}
}

// 0x8018E07C
void mvOpeningSamusFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E198
SYVideoSetup dMVOpeningSamusVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E1B4
SYTaskmanSetup dMVOpeningSamusTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl38_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningSamusFuncLights,    	// Pre-render function
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
    
    mvOpeningSamusFuncStart          	// Task start function
};

// 0x8018E0C8
void mvOpeningSamusStartScene(void)
{
	dMVOpeningSamusVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningSamusVideoSetup);

	dMVOpeningSamusTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl38_BSS_END);
	syTaskmanStartTask(&dMVOpeningSamusTaskmanSetup);
}
