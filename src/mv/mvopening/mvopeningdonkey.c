
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
CObjDesc dMVOpeningDonkeyCObjDescStart = { { -1100.0F, 150.0F, 400.0F }, { 0.0F, 150.0F, 0.0F }, 0.0F };

// 0x8018E08C
CObjDesc dMVOpeningDonkeyCObjDescEnd = { { -900.0F, 500.0F, 1800.0F }, { 0.0F, 500.0F, 0.0F }, 0.0F };

// 0x8018E0A8
FTKeyEvent dMVOpeningDonkeyKeyEvents[/* */] =
{
	FTKEY_EVENT_STICK(0, -I_CONTROLLER_RANGE_MAX, 0), // 0x2000, 0x00B0
	FTKEY_EVENT_BUTTON(B_BUTTON, 1),                  // 0x1001, 0x4000
	FTKEY_EVENT_BUTTON(0, 1),                         // 0x1001, 0x0000
	FTKEY_EVENT_BUTTON(B_BUTTON, 1),                  // 0x1001, 0x4000
	FTKEY_EVENT_END()                                 // 0x0000
};

// 0x8018E0BC
u32 dMVOpeningDonkeyFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E1C0
s32 sMVOpeningDonkeyPad0x8018E1C0[2];

// 0x8018E1C8
s32 sMVOpeningDonkeyTotalTimeTics;

// 0x8018E1CC
GObj *sMVOpeningDonkeyNameGObj;

// 0x8018E1D0
GObj *sMVOpeningDonkeyFighterGObj;

// 0x8018E1D4
s32 sMVOpeningDonkeyPad0x8018E1D4;

// 0x8018E1D8
GObj *sMVOpeningDonkeyStageCameraGObj;

// 0x8018E1DC
void *sMVOpeningDonkeyFigatreeHeap;

// 0x8018E1E0
f32 sMVOpeningDonkeyPosedFighterSpeed;

// 0x8018E1E4
s32 sMVOpeningDonkeyPad0x8018E1E4;

// 0x8018E1E8
CObjDesc sMVOpeningDonkeyAdjustedStartCObjDesc;

// 0x8018E208
CObjDesc sMVOpeningDonkeyAdjustedEndCObjDesc;

// 0x8018E228
LBFileNode sMVOpeningDonkeyStatusBuffer[48];

// 0x8018E3A8
LBFileNode sMVOpeningDonkeyForceStatusBuffer[7];

// 0x8018E3E0
void *sMVOpeningDonkeyFiles[ARRAY_COUNT(dMVOpeningDonkeyFileIDs)];

// 0x8018E3E8
SCBattleState sMVOpeningDonkeyBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningDonkeySetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningDonkeyStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningDonkeyStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningDonkeyForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningDonkeyForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningDonkeyFileIDs, sMVOpeningDonkeyFiles);
}

// 0x8018D160
void mvOpeningDonkeyInitName(SObj *sobj)
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
void mvOpeningDonkeyMakeName(void)
{
	GObj *gobj;
	SObj *sobj;
#if defined (REGION_US)
	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterDSprite,
		&llIFCommonAnnounceCommonLetterKSprite,
		0x0
	};
	Vec2f pos[/* */] =
	{
		{  0.0F, 0.0F },
		{ 40.0F, 0.0F }
	};
#else
	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterDSprite,
		&llIFCommonAnnounceCommonLetterOSprite,
		&llIFCommonAnnounceCommonLetterNSprite,
		&llIFCommonAnnounceCommonLetterKSprite,
		&llIFCommonAnnounceCommonLetterESprite,
		&llIFCommonAnnounceCommonLetterYSprite,
		&llIFCommonAnnounceCommonLetterKSprite,
		&llIFCommonAnnounceCommonLetterOSprite,
		&llIFCommonAnnounceCommonLetterNSprite,
		&llIFCommonAnnounceCommonLetterGSprite,
		0x0
	};

	Vec2f pos[/* */] =
	{
		{   0.0F,  0.0F },
		{  30.0F,  0.0F },
		{  65.0F,  0.0F },
		{  97.0F,  0.0F },
		{ 127.0F,  0.0F },
		{ 152.0F,  0.0F },
		{  90.0F, 50.0F },
		{ 119.0F, 50.0F },
		{ 155.0F, 50.0F },
		{ 186.0F, 50.0F }
	};
#endif
	s32 i;

	sMVOpeningDonkeyNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningDonkeyFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		
#if defined (REGION_US)
		sobj->pos.x = pos[i].x + 120.0F;
		sobj->pos.y = pos[i].y + 100.0F;
#else
		sobj->pos.x = pos[i].x + 60.0F;
		sobj->pos.y = pos[i].y + 80.0F;
#endif

		mvOpeningDonkeyInitName(sobj);
	}
}

// 0x8018D2FC
void mvOpeningDonkeyMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningDonkeyTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningDonkeyAdjustedEndCObjDesc.eye.x - sMVOpeningDonkeyAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningDonkeyAdjustedEndCObjDesc.eye.y - sMVOpeningDonkeyAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningDonkeyAdjustedEndCObjDesc.eye.z - sMVOpeningDonkeyAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningDonkeyAdjustedEndCObjDesc.at.x - sMVOpeningDonkeyAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningDonkeyAdjustedEndCObjDesc.at.y - sMVOpeningDonkeyAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningDonkeyAdjustedEndCObjDesc.at.z - sMVOpeningDonkeyAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningDonkeyAdjustedEndCObjDesc.upx - sMVOpeningDonkeyAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D3F4
void mvOpeningDonkeyMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningDonkeyAdjustedStartCObjDesc = dMVOpeningDonkeyCObjDescStart;
	sMVOpeningDonkeyAdjustedEndCObjDesc = dMVOpeningDonkeyCObjDescEnd;

	sMVOpeningDonkeyStageCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningDonkeyStageCameraGObj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 210.0F, 230.0F);

	cobj->projection.persp.aspect = 10.0F / 11.0F;

	gcEndProcessAll(sMVOpeningDonkeyStageCameraGObj);
	gcAddGObjProcess(sMVOpeningDonkeyStageCameraGObj, mvOpeningDonkeyMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningDonkeyAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningDonkeyAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningDonkeyAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningDonkeyAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningDonkeyAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningDonkeyAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningDonkeyAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningDonkeyAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningDonkeyAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningDonkeyAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningDonkeyAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningDonkeyAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningDonkeyAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningDonkeyAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningDonkeyAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningDonkeyAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningDonkeyAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningDonkeyAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningDonkeyAdjustedStartCObjDesc.upx;
}

// 0x8018D604
void mvOpeningDonkeyMakeMotionWindow(void)
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
	mvOpeningDonkeyMakeMotionCamera(pos);
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

		sMVOpeningDonkeyFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningDonkeyKeyEvents);
	}
}

// 0x8018D834
void mvOpeningDonkeyPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x46, 0x5A, 0x00, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 210, 10, 310, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D934
void mvOpeningDonkeyMakePosedWallpaper(void)
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
		mvOpeningDonkeyPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D980
void mvOpeningDonkeyPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningDonkeyTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningDonkeyPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningDonkeyPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningDonkeyPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningDonkeyTotalTimeTics > 15) && (sMVOpeningDonkeyTotalTimeTics < 45))
	{
		sMVOpeningDonkeyPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningDonkeyTotalTimeTics > 45) && (sMVOpeningDonkeyTotalTimeTics < 60))
	{
		sMVOpeningDonkeyPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.y += sMVOpeningDonkeyPosedFighterSpeed;
}

// 0x8018DA50
void mvOpeningDonkeyMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindDonkey;
	desc.costume = ftParamGetCostumeCommonID(nFTKindDonkey, 0);
	desc.figatree_heap = sMVOpeningDonkeyFigatreeHeap;
	desc.pos.x = 0.0F;
	desc.pos.y = -600.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningDonkeyPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB50
void mvOpeningDonkeyMakeNameCamera(void)
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

// 0x8018DBF0
void mvOpeningDonkeyMakePosedFighterCamera(void)
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

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningDonkeyFiles[1], &llMVOpeningCommonDonkeyCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DCD8
void mvOpeningDonkeyMakePosedWallpaperCamera(void)
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

// 0x8018DD80
void mvOpeningDonkeyFuncRun(GObj *gobj)
{
	sMVOpeningDonkeyTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}
	if (sMVOpeningDonkeyTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningDonkeyNameGObj);
		mvOpeningDonkeyMakeMotionWindow();
		mvOpeningDonkeyMakePosedWallpaper();
		mvOpeningDonkeyMakePosedFighter();
	}
	if (sMVOpeningDonkeyTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningLink;
		syTaskmanSetLoadScene();
	}
}

// 0x8018DE3C
void mvOpeningDonkeyInitVars(void)
{
	sMVOpeningDonkeyTotalTimeTics = 0;
}

// 0x8018DE48
void mvOpeningDonkeyFuncStart(void)
{
	sMVOpeningDonkeyBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningDonkeyBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindJungle;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindDonkey;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningDonkeySetupFiles();
	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningDonkeyFuncRun, nGCCommonLinkIDMovie, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningDonkeyInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindDonkey);

	sMVOpeningDonkeyFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mvOpeningDonkeyMakeNameCamera();
	mvOpeningDonkeyMakePosedWallpaperCamera();
	mvOpeningDonkeyMakePosedFighterCamera();
	mvOpeningDonkeyMakeName();

	while (sySchedulerGetTicCount() < 1605)
	{
		continue;
	}
}

// 0x8018DFCC
void mvOpeningDonkeyFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E0E0
SYVideoSetup dMVOpeningDonkeyVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E0FC
SYTaskmanSetup dMVOpeningDonkeyTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl37_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningDonkeyFuncLights,    	// Pre-render function
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
    
    mvOpeningDonkeyFuncStart          	// Task start function
};

// 0x8018E018
void mvOpeningDonkeyStartScene(void)
{
	dMVOpeningDonkeyVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningDonkeyVideoSetup);

	dMVOpeningDonkeyTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl37_BSS_END);
	syTaskmanStartTask(&dMVOpeningDonkeyTaskmanSetup);
}
