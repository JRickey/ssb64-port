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
CObjDesc dMVOpeningMarioStartCObjDesc = { { 300.0F, 500.0F, 1700.0F }, { 0.0F, 100.0F, 0.0F }, 0.15F };

// 0x8018E0AC
CObjDesc dMVOpeningMarioEndCObjDesc = { { 800.0F, 500.0F, 1300.0F }, { 100.0F, 100.0F, 0.0F }, 0.15F };

// 0x8018E0C8
FTKeyEvent dMVOpeningMarioKeyEvents[/* */] =
{
	FTKEY_EVENT_STICK(0, 0, 0),                         // 0x2000, 0x0000
	FTKEY_EVENT_BUTTON(A_BUTTON, 1),                    // 0x1001, 0x8000
	FTKEY_EVENT_BUTTON(0, 11),                          // 0x100B, 0x0000
	FTKEY_EVENT_BUTTON(A_BUTTON, 1),                    // 0x1001, 0x8000
	FTKEY_EVENT_BUTTON(0, 20),                          // 0x1014, 0x0000
	FTKEY_EVENT_STICK(0, -I_CONTROLLER_RANGE_MAX, 0),   // 0x2000, 0x00B0
	FTKEY_EVENT_BUTTON(A_BUTTON, 1),                    // 0x1001, 0x8000
	FTKEY_EVENT_END()                                   // 0x0000
};

// 0x8018E0E8
u32 dMVOpeningMarioFileIDs[/* */] = { &llIFCommonAnnounceCommonFileID, &llMVOpeningCommonFileID };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E200
s32 sMVOpeningMarioPad0x8018E200[2];

// 0x8018E208
s32 sMVOpeningMarioTotalTimeTics;

// 0x8018E20C
GObj *sMVOpeningMarioNameGObj;

// 0x8018E210
GObj *sMVOpeningMarioFighterGObj;

// 0x8018E214
s32 sMVOpeningMarioPad0x8018E234;

// 0x8018E218
GObj *sMVOpeningMarioMotionCameraGObj;

// 0x8018E21C
void *sMVOpeningMarioFigatreeHeap;

// 0x8018E220
f32 sMVOpeningMarioPosedFighterSpeed;

// 0x8018E224
s32 sMVOpeningMarioPad0x8018E244;

// 0x8018E228
CObjDesc sMVOpeningMarioAdjustedStartCObjDesc;

// 0x8018E248
CObjDesc sMVOpeningMarioAdjustedEndCObjDesc;

// 0x8018E268
LBFileNode sMVOpeningMarioStatusBuffer[48];

// 0x8018E3E8
LBFileNode sMVOpeningMarioForceStatusBuffer[7];

// 0x8018E420
void *sMVOpeningMarioFiles[ARRAY_COUNT(dMVOpeningMarioFileIDs)];

// 0x8018E428
SCBattleState sMVOpeningMarioBattleState;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void mvOpeningMarioSetupFiles(void)
{
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningMarioStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningMarioStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningMarioForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningMarioForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningMarioFileIDs, sMVOpeningMarioFiles);
}

// 0x8018D160
void mvOpeningMarioInitName(SObj *sobj)
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
void mvOpeningMarioMakeName(void)
{
	GObj *gobj;
	SObj *sobj;

	// 0x8018E0F0
	intptr_t offsets[/* */] =
	{
		&llIFCommonAnnounceCommonLetterMSprite,
		&llIFCommonAnnounceCommonLetterASprite,
		&llIFCommonAnnounceCommonLetterRSprite,
		&llIFCommonAnnounceCommonLetterISprite,
		&llIFCommonAnnounceCommonLetterOSprite,
		0x0
	};

	// 0x8018E108
	f32 pos_x[/* */] =
	{
		0.0F, 40.0F, 80.0F, 110.0F, 125.0F
	};

	s32 i;

	sMVOpeningMarioNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; offsets[i] != 0x0; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningMarioFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;

		sobj->pos.x = pos_x[i] + 80.0F;
		sobj->pos.y = 100.0F;

		mvOpeningMarioInitName(sobj);
	}
}

// 0x8018D314
void mvOpeningMarioMotionCameraProcUpdate(GObj *camera_gobj)
{
	CObj *cobj = CObjGetStruct(camera_gobj);

	if (sMVOpeningMarioTotalTimeTics >= 15)
	{
		cobj->vec.eye.x += (((sMVOpeningMarioAdjustedEndCObjDesc.eye.x - sMVOpeningMarioAdjustedStartCObjDesc.eye.x) / 45.0F));
		cobj->vec.eye.y += (((sMVOpeningMarioAdjustedEndCObjDesc.eye.y - sMVOpeningMarioAdjustedStartCObjDesc.eye.y) / 45.0F));
		cobj->vec.eye.z += (((sMVOpeningMarioAdjustedEndCObjDesc.eye.z - sMVOpeningMarioAdjustedStartCObjDesc.eye.z) / 45.0F));
		cobj->vec.at.x += (((sMVOpeningMarioAdjustedEndCObjDesc.at.x - sMVOpeningMarioAdjustedStartCObjDesc.at.x) / 45.0F));
		cobj->vec.at.y += (((sMVOpeningMarioAdjustedEndCObjDesc.at.y - sMVOpeningMarioAdjustedStartCObjDesc.at.y) / 45.0F));
		cobj->vec.at.z += (((sMVOpeningMarioAdjustedEndCObjDesc.at.z - sMVOpeningMarioAdjustedStartCObjDesc.at.z) / 45.0F));
		cobj->vec.up.x += (((sMVOpeningMarioAdjustedEndCObjDesc.upx - sMVOpeningMarioAdjustedStartCObjDesc.upx) / 45.0F));
	}
}

// 0x8018D40C
void mvOpeningMarioMakeMotionCamera(Vec3f move)
{
	CObj *cobj;

	sMVOpeningMarioAdjustedStartCObjDesc = dMVOpeningMarioStartCObjDesc;
	sMVOpeningMarioAdjustedEndCObjDesc = dMVOpeningMarioEndCObjDesc;

	sMVOpeningMarioMotionCameraGObj = gmCameraMakeMovieCamera(NULL);
	cobj = CObjGetStruct(sMVOpeningMarioMotionCameraGObj);

	syRdpSetViewport(&cobj->viewport, 110.0F, 10.0F, 310.0F, 230.0F);

	cobj->projection.persp.aspect = 10.0F / 11.0F;

	gcEndProcessAll(sMVOpeningMarioMotionCameraGObj);
	gcAddGObjProcess(sMVOpeningMarioMotionCameraGObj, mvOpeningMarioMotionCameraProcUpdate, nGCProcessKindFunc, 1);

	sMVOpeningMarioAdjustedStartCObjDesc.eye.x += move.x;
	sMVOpeningMarioAdjustedStartCObjDesc.eye.y += move.y;
	sMVOpeningMarioAdjustedStartCObjDesc.eye.z += move.z;
	sMVOpeningMarioAdjustedStartCObjDesc.at.x += move.x;
	sMVOpeningMarioAdjustedStartCObjDesc.at.y += move.y;
	sMVOpeningMarioAdjustedStartCObjDesc.at.z += move.z;

	sMVOpeningMarioAdjustedEndCObjDesc.eye.x += move.x;
	sMVOpeningMarioAdjustedEndCObjDesc.eye.y += move.y;
	sMVOpeningMarioAdjustedEndCObjDesc.eye.z += move.z;
	sMVOpeningMarioAdjustedEndCObjDesc.at.x += move.x;
	sMVOpeningMarioAdjustedEndCObjDesc.at.y += move.y;
	sMVOpeningMarioAdjustedEndCObjDesc.at.z += move.z;

	cobj->vec.eye.x = sMVOpeningMarioAdjustedStartCObjDesc.eye.x;
	cobj->vec.eye.y = sMVOpeningMarioAdjustedStartCObjDesc.eye.y;
	cobj->vec.eye.z = sMVOpeningMarioAdjustedStartCObjDesc.eye.z;
	cobj->vec.at.x = sMVOpeningMarioAdjustedStartCObjDesc.at.x;
	cobj->vec.at.y = sMVOpeningMarioAdjustedStartCObjDesc.at.y;
	cobj->vec.at.z = sMVOpeningMarioAdjustedStartCObjDesc.at.z;
	cobj->vec.up.x = sMVOpeningMarioAdjustedStartCObjDesc.upx;
}

// 0x8018D614
void mvOpeningMarioMakeMotionWindow(void)
{
	GObj* fighter_gobj;
	s32 i;
	s32 pos_ids[2];
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
	mvOpeningMarioMakeMotionCamera(pos);
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

		sMVOpeningMarioFighterGObj = fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(i, fighter_gobj);
		ftParamSetKey(fighter_gobj, dMVOpeningMarioKeyEvents);
	}
}

// 0x8018D844
void mvOpeningMarioPosedWallpaperProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xA0, 0xAA, 0xFF, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 110, 230);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x8018D944
void mvOpeningMarioMakePosedWallpaper(void)
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
		mvOpeningMarioPosedWallpaperProcDisplay,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0
	);
}

// 0x8018D990
void mvOpeningMarioPosedFighterProcUpdate(GObj *fighter_gobj)
{
	switch (sMVOpeningMarioTotalTimeTics)
	{
	default:
		break;

	case 15:
		sMVOpeningMarioPosedFighterSpeed = 17.0F;
		break;

	case 45:
		sMVOpeningMarioPosedFighterSpeed = 15.0F;
		break;

	case 60:
		sMVOpeningMarioPosedFighterSpeed = 0.0F;
		break;
	}
	if ((sMVOpeningMarioTotalTimeTics > 15) && (sMVOpeningMarioTotalTimeTics < 45))
	{
		sMVOpeningMarioPosedFighterSpeed += -1.0F / 15.0F;
	}
	if ((sMVOpeningMarioTotalTimeTics > 45) && (sMVOpeningMarioTotalTimeTics < 60))
	{
		sMVOpeningMarioPosedFighterSpeed += -1.0F;
	}
	DObjGetStruct(fighter_gobj)->translate.vec.f.y -= sMVOpeningMarioPosedFighterSpeed;
}

// 0x8018DA60
void mvOpeningMarioMakePosedFighter(void)
{
	GObj *fighter_gobj;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	desc.fkind = nFTKindMario;
	desc.costume = ftParamGetCostumeCommonID(nFTKindMario, 0);
	desc.figatree_heap = sMVOpeningMarioFigatreeHeap;

	desc.pos.x = 0.0F;
	desc.pos.y = 600.0F;
	desc.pos.z = 0.0F;

	fighter_gobj = ftManagerMakeFighter(&desc);
	scSubsysFighterSetStatus(fighter_gobj, nFTDemoStatusStance);
	gcMoveGObjDL(fighter_gobj, 26, -1);
	gcAddGObjProcess(fighter_gobj, mvOpeningMarioPosedFighterProcUpdate, nGCProcessKindFunc, 1);

	DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
	DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x8018DB5C
void mvOpeningMarioMakeNameCamera(void)
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

// 0x8018DBFC
void mvOpeningMarioMakePosedFighterCamera(void)
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

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 110.0F, 230.0F);
	
	cobj->projection.persp.aspect = 5.0F / 11.0F;

	gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sMVOpeningMarioFiles[1], &llMVOpeningCommonMarioCamAnimJoint), 0.0F);
	gcAddGObjProcess(camera_gobj, gcPlayCamAnim, nGCProcessKindFunc, 1);
}

// 0x8018DCEC
void mvOpeningMarioMakePosedWallpaperCamera(void)
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
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 110.0F, 230.0F);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
}

// 0x8018DD9C
void mvOpeningMarioFuncRun(GObj *gobj)
{
	sMVOpeningMarioTotalTimeTics++;

	if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;

		syTaskmanSetLoadScene();
	}
	if (sMVOpeningMarioTotalTimeTics == 15)
	{
		gcEjectGObj(sMVOpeningMarioNameGObj);
		mvOpeningMarioMakeMotionWindow();
		mvOpeningMarioMakePosedWallpaper();
		mvOpeningMarioMakePosedFighter();
	}
	if (sMVOpeningMarioTotalTimeTics == 60)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningDonkey;

		syTaskmanSetLoadScene();
	}
}

// 0x8018DE58
void mvOpeningMarioInitVars(void)
{
	sMVOpeningMarioTotalTimeTics = 0;
}

// 0x8018DE64
void mvOpeningMarioFuncStart(void)
{
	sMVOpeningMarioBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sMVOpeningMarioBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeMovie;

	gSCManagerBattleState->gkind = nGRKindCastle;
	gSCManagerBattleState->pl_count = 1;

	gSCManagerBattleState->players[0].fkind = nFTKindMario;
	gSCManagerBattleState->players[0].pkind = nFTPlayerKindKey;

	mvOpeningMarioSetupFiles();

	gcMakeGObjSPAfter(nGCCommonKindMovie, mvOpeningMarioFuncRun, 13, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

	mvOpeningMarioInitVars();
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 2);
	wpManagerAllocWeapons();
	itManagerInitItems();
	efManagerInitEffects();
	ftManagerSetupFilesAllKind(nFTKindMario);

	sMVOpeningMarioFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);
	
	mvOpeningMarioMakeNameCamera();
	mvOpeningMarioMakePosedWallpaperCamera();
	mvOpeningMarioMakePosedFighterCamera();
	mvOpeningMarioMakeName();

	while (sySchedulerGetTicCount() < 1515)
	{
		continue;
	};
}

// 0x8018DFE4
void mvOpeningMarioFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E11C
SYVideoSetup dMVOpeningMarioVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E138
SYTaskmanSetup dMVOpeningMarioTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl36_BSS_END,                 // Allocatable memory pool start
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
        mvOpeningMarioFuncLights,      	// Pre-render function
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
    
    mvOpeningMarioFuncStart            	// Task start function
};

// 0x8018E030
void mvOpeningMarioStartScene(void)
{
	dMVOpeningMarioVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningMarioVideoSetup);

	dMVOpeningMarioTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl36_BSS_END);
	syTaskmanStartTask(&dMVOpeningMarioTaskmanSetup);
}
