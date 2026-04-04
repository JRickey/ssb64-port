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

// 0x8018E0B0
CObjDesc dMVOpeningKirbyStartCObjDesc = { { 0.0F, 400.0F, 2000.0F }, { 0.0F, 400.0F, 0.0F }, 0.0F };

// 0x8018E0CC
CObjDesc dMVOpeningKirbyEndCObjDesc = { { 1100.0F, 400.0F, 1800.0F }, { 1100.0F, 400.0F, 0.0F }, 0.0F };

// 0x8018E0E8
FTKeyEvent dMVOpeningKirbyKeyEvents[/* */] =
{
	FTKEY_EVENT_STICK(45, I_CONTROLLER_RANGE_MAX, 1),   // 2001, 2D50
	FTKEY_EVENT_BUTTON(A_BUTTON, 1),                    // 1001, 8000
	FTKEY_EVENT_END()                                   // 0000
};

// 0x8018E0F4
u32 dMVOpeningKirbyFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E220
s32 sMVOpeningKirbyPad0x8018E220[2];

// 0x8018E228
s32 sMVOpeningKirbyTotalTimeTics;

// 0x8018E22C
GObj *sMVOpeningKirbyNameGObj;

// 0x8018E230
GObj *sMVOpeningKirbyFighterGObj;

// 0x8018E234
s32 sMVOpeningKirbyPad0x8018E234;

// 0x8018E238
GObj *sMVOpeningKirbyMotionCameraGObj;

// 0x8018E23C
void *sMVOpeningKirbyFigatreeHeap;

// 0x8018E240
f32 sMVOpeningKirbyPosedFighterSpeed;

// 0x8018E244
s32 sMVOpeningKirbyPad0x8018E244;

// 0x8018E248
CObjDesc sMVOpeningKirbyAdjustedStartCObjDesc;

// 0x8018E268
CObjDesc sMVOpeningKirbyAdjustedEndCObjDesc;

// 0x8018E288
LBFileNode sMVOpeningKirbyStatusBuffer[48];

// 0x8018E408
LBFileNode sMVOpeningKirbyForceStatusBuffer[7];

// 0x8018E440
void *sMVOpeningKirbyFiles[ARRAY_COUNT(dMVOpeningKirbyFileIDs)];

// 0x8018E448
SCBattleState sMVOpeningKirbyBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningKirbySetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningKirbyStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningKirbyStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningKirbyForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningKirbyForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningKirbyFileIDs, sMVOpeningKirbyFiles);
}

// 0x8018D160
void mvOpeningKirbyInitName(SObj *sobj)
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
void mvOpeningKirbyMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	// 0x8018E0FC
	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterKSprite,
		&llIFCommonAnnounceCommonLetterISprite,
		&llIFCommonAnnounceCommonLetterRSprite,
		&llIFCommonAnnounceCommonLetterBSprite,
		&llIFCommonAnnounceCommonLetterYSprite,
		0x0
	};

	// 0x8018E114
	Vec2f pos[/* */] =
	{
		{   0.0F, 0.0F },
		{  35.0F, 0.0F },
		{  50.0F, 0.0F },
		{  80.0F, 0.0F },
		{ 110.0F, 0.0F }
	};

	s32 i;

	sMVOpeningKirbyNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningKirbyFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;

		sobj->pos.x = pos[i].x + 90.0F;
		sobj->pos.y = pos[i].y + 100.0F;

		mvOpeningKirbyInitName(sobj);
	}
}

// 0x8018D324
void mvOpeningKirbyMotionCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(gobj);

	if (sMVOpeningKirbyTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningKirbyAdjustedEndCObjDesc.eye.x - sMVOpeningKirbyAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningKirbyAdjustedEndCObjDesc.eye.y - sMVOpeningKirbyAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningKirbyAdjustedEndCObjDesc.eye.z - sMVOpeningKirbyAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningKirbyAdjustedEndCObjDesc.at.x - sMVOpeningKirbyAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningKirbyAdjustedEndCObjDesc.at.y - sMVOpeningKirbyAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningKirbyAdjustedEndCObjDesc.at.z - sMVOpeningKirbyAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningKirbyAdjustedEndCObjDesc.upx - sMVOpeningKirbyAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D41C
void mvOpeningKirbyMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningKirbyAdjustedStartCObjDesc = dMVOpeningKirbyStartCObjDesc;
	sMVOpeningKirbyAdjustedEndCObjDesc = dMVOpeningKirbyEndCObjDesc;

	sMVOpeningKirbyMotionCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningKirbyMotionCameraGObj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 210.0F, 230.0F);

	cobj->projection.persp.aspect = 10.0F / 11.0F;

	gcEndProcessAll(sMVOpeningKirbyMotionCameraGObj);
	gcAddGObjProcess(sMVOpeningKirbyMotionCameraGObj, mvOpeningKirbyMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningKirbyAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningKirbyAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningKirbyAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningKirbyAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningKirbyAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningKirbyAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningKirbyAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningKirbyAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningKirbyAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningKirbyAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningKirbyAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningKirbyAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningKirbyAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningKirbyAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningKirbyAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningKirbyAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningKirbyAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningKirbyAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningKirbyAdjustedStartCObjDesc.upx;
}

// 0x8018D62C
void mvOpeningKirbyMakeMotionWindow(void)
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

	pos.y += 30.0F;
	
	mvOpeningKirbyMakeMotionCamera(pos);
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

		sMVOpeningKirbyFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningKirbyKeyEvents);
	}
}

// 0x8018D870
void mvOpeningKirbyPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x50, 0xAA, 0xFF, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 210, 10, 310, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D970
void mvOpeningKirbyMakePosedWallpaper(void)
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
		mvOpeningKirbyPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D9BC
void mvOpeningKirbyPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningKirbyTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningKirbyPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningKirbyPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningKirbyPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningKirbyTotalTimeTics > 15) && (sMVOpeningKirbyTotalTimeTics < 45))
	{
		sMVOpeningKirbyPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningKirbyTotalTimeTics > 45) && (sMVOpeningKirbyTotalTimeTics < 60))
	{
		sMVOpeningKirbyPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.y -= sMVOpeningKirbyPosedFighterSpeed;
}

// 0x8018DA8C
void mvOpeningKirbyMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindKirby;
	desc.costume = ftParamGetCostumeCommonID(nFTKindKirby, 0);
	desc.figatree_heap = sMVOpeningKirbyFigatreeHeap;

	desc.pos.x = 0.0F;
	desc.pos.y = 600.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningKirbyPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB8C
void mvOpeningKirbyMakeNameCamera(void)
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
            80,
            COBJ_MASK_DLLINK(27),
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

// 0x8018DC2C
void mvOpeningKirbyMakePosedFighterCamera(void)
{
	GObj *camera_gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        10,
        COBJ_MASK_DLLINK(26),
        -1,
        TRUE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
	CObj *cobj = CObjGetStruct(camera_gobj);

	syRdpSetViewport(&cobj->viewport, 210.0F, 10.0F, 310.0F, 230.0F);
	
	cobj->projection.persp.aspect = 5.0F / 11.0F;

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningKirbyFiles[1], &llMVOpeningCommonKirbyCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DD14
void mvOpeningKirbyMakePosedWallpaperCamera(void)
{
	CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindSceneCamera,
            NULL,
            16,
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
        )
    );
    syRdpSetViewport(&cobj->viewport, 210.0F, 10.0F, 310.0F, 230.0F);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
}

// 0x8018DDBC
void mvOpeningKirbyFuncRun(GObj *gobj)
{
	sMVOpeningKirbyTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;

		syTaskmanSetLoadScene();
	}
	if (sMVOpeningKirbyTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningKirbyNameGObj);
		mvOpeningKirbyMakeMotionWindow();
		mvOpeningKirbyMakePosedWallpaper();
		mvOpeningKirbyMakePosedFighter();
	}
	if (sMVOpeningKirbyTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningFox;

		syTaskmanSetLoadScene();
	}
}

// 0x8018DE78
void mvOpeningKirbyInitVars(void)
{
	sMVOpeningKirbyTotalTimeTics = 0;
}

// 0x8018DE84
void mvOpeningKirbyFuncStart(void)
{
	sMVOpeningKirbyBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningKirbyBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindPupupu;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindKirby;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningKirbySetupFiles();

	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningKirbyFuncRun, 13, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

	mvOpeningKirbyInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindKirby);

	sMVOpeningKirbyFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);
	
	mvOpeningKirbyMakeNameCamera();
	mvOpeningKirbyMakePosedWallpaperCamera();
	mvOpeningKirbyMakePosedFighterCamera();
	mvOpeningKirbyMakeName();

	while (sySchedulerGetTicCount() < 1965)
	{
		continue;
	};
}

// 0x8018E00C
void mvOpeningKirbyFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E13C
SYVideoSetup dMVOpeningKirbyVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E158
SYTaskmanSetup dMVOpeningKirbyTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl43_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningKirbyFuncLights,      	// Pre-render function
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
    
    mvOpeningKirbyFuncStart            	// Task start function
};

// 0x8018E058
void mvOpeningKirbyStartScene(void)
{
	dMVOpeningKirbyVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningKirbyVideoSetup);

	dMVOpeningKirbyTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl43_BSS_END);
	syTaskmanStartTask(&dMVOpeningKirbyTaskmanSetup);
}
