#include <ft/fighter.h>
#include <gr/ground.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018E160
s32 D_ovl64_8018E160 = 0;

// 0x8018E164
s32 D_ovl64_8018E164 = 0;

// 0x8018E168
u8 dSCAutoDemoGroundOrder[/* */] = 
{
	nGRKindPupupu,
	nGRKindZebes,
	nGRKindCastle,
	nGRKindJungle,
	nGRKindSector,
	nGRKindYoster,
	nGRKindYamabuki,
	nGRKindHyrule
};

// 0x8018E170
s16 dSCAutoDemoMapObjKindList[/* */] = 
{
	nMPMapObjKindAutoDemoPlayer1,
	nMPMapObjKindAutoDemoPlayer2,
	nMPMapObjKindAutoDemoPlayer3,
	nMPMapObjKindAutoDemoPlayer4,
	nMPMapObjKindAutoDemoPlayer5,
	nMPMapObjKindAutoDemoPlayer6,
	nMPMapObjKindAutoDemoPlayer7,
	nMPMapObjKindAutoDemoPlayer8
};

// 0x8018E180
SCAutoDemoProc dSCAutoDemoFuncList[/* */] =
{
	// Nothing?
	{
		0,                              // Wait frames until focus changes 
		NULL,                           // Function to run on focus change
		NULL                            // Function to run when focusing
	},

	// Pre-focus
	{
		340,                            // Wait frames until focus changes 
		scAutoDemoMakeFade,            	// Function to run on focus change
		NULL                            // Function to run when focusing
	},

	// Change to Player 1 focus
	{
		340,                            // Wait frames until focus changes 
		scAutoDemoSetFocusPlayer1,      // Function to run on focus change
		NULL                            // Function to run when focusing
	},

	// Player 1 focus
	{
		340,                            // Wait frames until focus changes 
		scAutoDemoSetFocusPlayer2,      // Function to run on focus change
		SCAutoDemoProcFocusPlayer1      // Function to run when focusing
	},

	// Player 2 focus
	{
		400,                            // Wait frames until focus changes 
		scAutoDemoResetFocusPlayerAll,  // Function to run on focus change
		SCAutoDemoProcFocusPlayer2      // Function to run when focusing
	},

	// End focus
	{
		60,                             // Wait frames until focus changes 
		scAutoDemoSetMagnifyDisplayOn,  // Function to run on focus change
		NULL                            // Function to run when focusing
	},

	// End demo
	{
		1,                              // Wait frames until focus changes 
		scAutoDemoExit,                 // Function to run on focus change
		NULL                            // Function to run when focusing
	}
};

// 0x8018E1D4
f32 dSCAutoDemoZoomEyeX[/* */] = { -40.0F, -28.0F, -14.0F, 14.0F, 28.0F, 40.0F };

// 0x8018E1EC
f32 dSCAutoDemoZoomEyeY[/* */] = { 2.0F, 0.0F, -6.0F, -9.0F, -30.0F };

// 0x8018E200
intptr_t dSCAutoDemoFighterNameSpriteOffsets[/* */] =
{
	&llCharacterNamesMarioSprite,      // Mario
	&llCharacterNamesFoxSprite,        // Fox
	&llCharacterNamesDonkeySprite,     // Donkey Kong
	&llCharacterNamesSamusSprite,      // Samus
	&llCharacterNamesLuigiSprite,      // Luigi
	&llCharacterNamesLinkSprite,       // Link
	&llCharacterNamesYoshiSprite,      // Yoshi
	&llCharacterNamesCaptainSprite,    // Captain Falcon
	&llCharacterNamesKirbySprite,      // Kirby
	&llCharacterNamesPikachuSprite,    // Pikachu
	&llCharacterNamesPurinSprite,      // Jigglypuff
	&llCharacterNamesNessSprite        // Ness
};

// 0x8018E230
SYColorRGBA dSCAutoDemoFadeColor = { 0x00, 0x00, 0x00, 0x00 };

// 0x8018E234
SYVideoSetup dSCAutoDemoVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018E250
SYTaskmanSetup dSCAutoDemoTaskmanSetup = 
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        scAutoDemoFuncUpdate,       // Update function
        scManagerFuncDraw,          // Frame draw function
        &ovl64_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 3072,         // Display List Buffer 0 Size
        sizeof(Gfx) * 1536,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        scAutoDemoFuncLights,    	// Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    0,                              // Number of thread stacks
    0,                              // ???
    0,                              // Number of GObjProcesses
    0,                              // Number of GObjs
    sizeof(GObj),                   // GObj size
    0,                              // Number of XObjs
    dLBCommonFuncMatrixList,        // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    scAutoDemoFuncStart           	// Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018E2F0
SCBattleState sSCAutoDemoBattleState;

// 0x8018E4E0
s32 sSCAutoDemoFocusChangeWait;

// 0x8018E4E4
u16 sSCAutoDemoFighterMask;

// 0x8018E4E8
GObj *sSCAutoDemoFighterNameGObj;

// 0x8018E4EC
SCAutoDemoProc *sSCAutoDemoFunc;

// 0x8018E4F0
s16 sSCAutoDemoMapObjs[8];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void scAutoDemoFuncUpdate(void)
{
	gcRunAll();
}

// 0x8018D0E0
void scAutoDemoStartBattle(void)
{
	GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

	while (fighter_gobj != NULL)
	{
		ftParamUnlockPlayerControl(fighter_gobj);

		fighter_gobj = fighter_gobj->link_next;
	}
	gSCManagerBattleState->game_status = nSCBattleGameStatusGo;
}

// 0x8018D134
void scAutoDemoDetectExit(void)
{
	s32 player;

	for (player = 0; player < ARRAY_COUNT(gSYControllerDevices); player++)
	{
		u16 button_tap = gSYControllerDevices[player].button_tap;

		if (button_tap & (A_BUTTON | B_BUTTON | START_BUTTON))
		{
			gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
			gSCManagerSceneData.scene_curr = nSCKindTitle;

			syTaskmanSetLoadScene();
			break;
		}
	}
}

// 0x8018D19C
void scAutoDemoMakeFade(void)
{
	SYColorRGBA color = dSCAutoDemoFadeColor;

	lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 30, TRUE, NULL);
}

// 0x8018D1EC
sb32 scAutoDemoCheckStopFocusPlayer(FTStruct *fp)
{
	switch (fp->status_id)
	{
	case nFTCommonStatusDeadDown:
	case nFTCommonStatusDeadLeftRight:
	case nFTCommonStatusDeadUpStar:
		return TRUE;

	default:
		return FALSE;
	}
}

// 0x8018D220
void scAutoDemoSetCameraPlayerZoom(GObj *fighter_gobj)
{
	gmCameraSetStatusPlayerZoom
	(
		fighter_gobj,
		F_CLC_DTOR32(dSCAutoDemoZoomEyeX[syUtilsRandIntRange(ARRAY_COUNT(dSCAutoDemoZoomEyeX))]),
		F_CLC_DTOR32(dSCAutoDemoZoomEyeY[syUtilsRandIntRange(ARRAY_COUNT(dSCAutoDemoZoomEyeY))]),
		ftGetStruct(fighter_gobj)->attr->closeup_camera_zoom,
		0.3F,
		28.0F
	);
}

// 0x8018D2CC
void scAutoDemoSetFocusPlayer1(void)
{
	GObj *fighter_gobj = gSCManagerBattleState->players[0].fighter_gobj;
	FTStruct *fp = ftGetStruct(fighter_gobj);

	if (scAutoDemoCheckStopFocusPlayer(fp) != FALSE)
	{
		sSCAutoDemoFocusChangeWait = 0;
	}
	else
	{
		ftGetStruct(gSCManagerBattleState->players[1].fighter_gobj)->level =
		ftGetStruct(gSCManagerBattleState->players[2].fighter_gobj)->level =
		ftGetStruct(gSCManagerBattleState->players[3].fighter_gobj)->level = 1;

		scAutoDemoSetCameraPlayerZoom(fighter_gobj);
		ftParamSetModelPartDetailAll(fighter_gobj, nFTPartsDetailHigh);

		fp->detail_base = nFTPartsDetailHigh;

		SObjGetStruct(sSCAutoDemoFighterNameGObj)->sprite.attr &= ~SP_HIDDEN;

		gIFCommonPlayerInterface.is_magnify_display = FALSE;
	}
}

// 0x8018D39C
void SCAutoDemoProcFocusPlayer1(void)
{
	if (scAutoDemoCheckStopFocusPlayer(ftGetStruct(gSCManagerBattleState->players[0].fighter_gobj)) != FALSE)
	{
		sSCAutoDemoFocusChangeWait = 0;
	}
}

// 0x8018D3D4
void scAutoDemoSetFocusPlayer2(void)
{
	GObj *p2_gobj = gSCManagerBattleState->players[1].fighter_gobj;
	GObj *p1_gobj = gSCManagerBattleState->players[0].fighter_gobj;
	FTStruct *p2_fp = ftGetStruct(p2_gobj);

	SObjGetStruct(sSCAutoDemoFighterNameGObj)->sprite.attr |= SP_HIDDEN;

	ftParamSetModelPartDetailAll(p1_gobj, nFTPartsDetailLow);
	ftGetStruct(p1_gobj)->detail_base = nFTPartsDetailLow;

	if (scAutoDemoCheckStopFocusPlayer(p2_fp) != FALSE)
	{
		sSCAutoDemoFocusChangeWait = 0;
	}
	else
	{
		ftGetStruct(gSCManagerBattleState->players[1].fighter_gobj)->level = 9;

		ftGetStruct(gSCManagerBattleState->players[0].fighter_gobj)->level =
		ftGetStruct(gSCManagerBattleState->players[2].fighter_gobj)->level =
		ftGetStruct(gSCManagerBattleState->players[3].fighter_gobj)->level = 1;

		scAutoDemoSetCameraPlayerZoom(p2_gobj);
		ftParamSetModelPartDetailAll(p2_gobj, nFTPartsDetailHigh);

		p2_fp->detail_base = nFTPartsDetailHigh;

		SObjGetStruct(sSCAutoDemoFighterNameGObj)->next->sprite.attr &= ~SP_HIDDEN;
	}
}

// 0x8018D4F0
void SCAutoDemoProcFocusPlayer2(void)
{
	if (scAutoDemoCheckStopFocusPlayer(ftGetStruct(gSCManagerBattleState->players[1].fighter_gobj)) != FALSE)
	{
		sSCAutoDemoFocusChangeWait = 0;
	}
}

// 0x8018D528
void scAutoDemoResetFocusPlayerAll(void)
{
	GObj *p2_gobj = gSCManagerBattleState->players[1].fighter_gobj;

	gmCameraSetStatusDefault();

	ftGetStruct(gSCManagerBattleState->players[0].fighter_gobj)->level =
	ftGetStruct(p2_gobj)->level =
	ftGetStruct(gSCManagerBattleState->players[2].fighter_gobj)->level =
	ftGetStruct(gSCManagerBattleState->players[3].fighter_gobj)->level = 9;

	ftParamSetModelPartDetailAll(p2_gobj, nFTPartsDetailLow);

	ftGetStruct(p2_gobj)->detail_base = nFTPartsDetailLow;

	SObjGetStruct(sSCAutoDemoFighterNameGObj)->next->sprite.attr |= SP_HIDDEN;
}

// 0x8018D5E0
void scAutoDemoSetMagnifyDisplayOn(void)
{
	gIFCommonPlayerInterface.is_magnify_display = TRUE;
}

// 0x8018D5F0
void scAutoDemoExit(void)
{
	gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
#if defined(REGION_US)        
    gSCManagerSceneData.scene_curr = nSCKindStartup;
#else
    gSCManagerSceneData.scene_curr = nSCKindOpeningRoom;
#endif

	syTaskmanSetLoadScene();
}

// 0x8018D624
void scAutoDemoChangeFocus(void)
{
	sSCAutoDemoFocusChangeWait = sSCAutoDemoFunc->focus_end_wait;

	if (sSCAutoDemoFunc->func_change != NULL)
	{
		sSCAutoDemoFunc->func_change();
	}
	sSCAutoDemoFunc++;
}

// 0x8018D674
void scAutoDemoUpdateFocus(void)
{
	if (sSCAutoDemoFunc->func_focus != NULL)
	{
		sSCAutoDemoFunc->func_focus();
	}
	while (sSCAutoDemoFocusChangeWait == 0)
	{
		scAutoDemoChangeFocus();
	}
	sSCAutoDemoFocusChangeWait--;
}

// 0x8018D6DC
void scAutoDemoFuncRun(GObj *gobj)
{
	scAutoDemoDetectExit();
	scAutoDemoUpdateFocus();
}

// 0x8018D704
GObj* scAutoDemoMakeFocusInterface(void)
{
	GObj *interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, scAutoDemoFuncRun, nGCCommonLinkIDInterfaceActor, GOBJ_PRIORITY_DEFAULT);

	sSCAutoDemoFunc = dSCAutoDemoFuncList;
	sSCAutoDemoFocusChangeWait = 0;

	scAutoDemoUpdateFocus();

	return interface_gobj;
}

// 0x8018D758
void scAutoDemoGetPlayerStartPosition(s32 mapobj_kind, Vec3f *mapobj_pos)
{
	s32 i, j;
	s32 mapobj_random;
	s32 mapobj_select;
	s32 mapobj;

	mapobj_random = syUtilsRandIntRange(((ARRAY_COUNT(dSCAutoDemoMapObjKindList) + ARRAY_COUNT(sSCAutoDemoMapObjs)) / 2) - mapobj_kind);

	for (i = j = 0; i < (ARRAY_COUNT(dSCAutoDemoMapObjKindList) + ARRAY_COUNT(sSCAutoDemoMapObjs)) / 2; i++)
	{
		mapobj_select = dSCAutoDemoMapObjKindList[i];

		if (sSCAutoDemoMapObjs[i] != -1)
		{
			if (mapobj_random == j)
			{
				sSCAutoDemoMapObjs[i] = -1;

				break;
			}
			else j++;
		}
	}
	if (mpCollisionGetMapObjCountKind(mapobj_select) != 0)
	{
		mpCollisionGetMapObjIDsKind(mapobj_select, &mapobj);
		mpCollisionGetMapObjPositionID(mapobj, mapobj_pos);
	}
}

// 0x8018D7FC
s32 scAutoDemoGetFighterKindsNum(u16 flag)
{
	s32 i, j;

	for (i = 0, j = 0; i < (sizeof(u16) * 8); i++, flag = flag >> 1)
	{
		if (flag & 1)
		{
			j++;
		}
	}
	return j;
}

// 0x8018D874
s32 scAutoDemoGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random)
{
	s32 ret = -1;

	random++;

	do
	{
		ret++;

		if ((this_mask & (1 << ret)) && !(prev_mask & (1 << ret)))
		{
			random--;
		}
	} 
	while (random != 0);

	return ret;
}

// 0x8018D8C0
s32 scAutoDemoGetFighterKind(s32 player)
{
	u16 fighter_mask;
	s32 unused;
	s32 fighter_count2;
	s32 fighter_count1;
	s32 fkind;

	if (player < 2)
	{
		return gSCManagerSceneData.demo_fkind[player];
	}
	fighter_mask = (gSCManagerBackupData.fighter_mask | LBBACKUP_CHARACTER_MASK_STARTER);

	fighter_count1 = scAutoDemoGetFighterKindsNum(fighter_mask), 
	fighter_count2 = scAutoDemoGetFighterKindsNum(sSCAutoDemoFighterMask);

	fkind = scAutoDemoGetShuffledFighterKind(fighter_mask, sSCAutoDemoFighterMask, syUtilsRandIntRange(fighter_count1 - fighter_count2));

	sSCAutoDemoFighterMask |= (1 << fkind);

	return fkind;
}

// 0x8018D954
s32 scAutoDemoGetPlayerDamage(s32 player)
{
	if (player < 2)
	{
		return syUtilsRandIntRange(30);
	}
	else return syUtilsRandIntRange(60) + 40;
}

// 0x8018D990
void scAutoDemoInitDemo(void)
{
	s32 i;

	sSCAutoDemoBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sSCAutoDemoBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeDemo;
	gSCManagerBattleState->gkind = dSCAutoDemoGroundOrder[gSCManagerSceneData.demo_gkind_order];

	gSCManagerSceneData.demo_gkind_order++;

	if (gSCManagerSceneData.demo_gkind_order >= ARRAY_COUNT(dSCAutoDemoGroundOrder))
	{
		gSCManagerSceneData.demo_gkind_order = 0;
	}
	sSCAutoDemoFighterMask = (1 << gSCManagerSceneData.demo_fkind[0]) | (1 << gSCManagerSceneData.demo_fkind[1]);

	for (i = 0; i < ARRAY_COUNT(gSCManagerBattleState->players); i++)
	{
		gSCManagerBattleState->players[i].pkind = nFTPlayerKindCom;
		gSCManagerBattleState->players[i].fkind = scAutoDemoGetFighterKind(i);
		gSCManagerBattleState->players[i].level = 9;

		gSCManagerBattleState->players[i].stock_damage_all = scAutoDemoGetPlayerDamage(i);
	}
	gSCManagerBattleState->pl_count = 0;
	gSCManagerBattleState->cp_count = GMCOMMON_PLAYERS_MAX;

	for (i = 0; i < ARRAY_COUNT(sSCAutoDemoMapObjs); i++)
	{
		sSCAutoDemoMapObjs[i] = 0;
	}
}

// 0x8018DB18
void scAutoDemoInitSObjs(void)
{
	GObj *interface_gobj;
	s32 player;
	void *file;

	file = lbRelocGetExternHeapFile
	(
		(uintptr_t)&llCharacterNamesFileID,
		syTaskmanMalloc
		(
			lbRelocGetFileSize((uintptr_t)&llCharacterNamesFileID),
			0x10
		)
	);
	sSCAutoDemoFighterNameGObj = interface_gobj = gcMakeGObjSPAfter
	(
		nGCCommonKindInterface, 
		NULL, 
		nGCCommonLinkIDInterface, 
		GOBJ_PRIORITY_DEFAULT
	);
	gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);

	for (player = 0; player < ARRAY_COUNT(gSCManagerSceneData.demo_fkind); player++)
	{
		SObj *sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, file, dSCAutoDemoFighterNameSpriteOffsets[gSCManagerBattleState->players[player].fkind]));

		sobj->sprite.red   = 0xFF;
		sobj->sprite.green = 0xFF;
		sobj->sprite.blue  = 0xFF;

		sobj->sprite.attr = SP_TEXSHUF | SP_HIDDEN | SP_TRANSPARENT;

		sobj->pos.x = (s32) (160.0F - (sobj->sprite.width * 0.5F));
		sobj->pos.y = (s32) (50.0F - (sobj->sprite.height * 0.5F));
	}
}

// 0x8018DCC4
void scAutoDemoFuncStart(void)
{
	GObj *fighter_gobj;
	FTDesc desc;
	s32 player;

	scAutoDemoInitDemo();
	scAutoDemoSetupFiles();
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	grWallpaperMakeDecideKind();
	gmCameraMakeBattleCamera();
	itManagerInitItems();
	grCommonSetupInitAll();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION, 4);
	wpManagerAllocWeapons();
	efManagerInitEffects();
	ifScreenFlashMakeInterface(0xFF);
	gmRumbleMakeActor();
	ftPublicMakeActor();

	for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
	{
		desc = dFTManagerDefaultFighterDesc;

		ftManagerSetupFilesAllKind(gSCManagerBattleState->players[player].fkind);

		desc.fkind = gSCManagerBattleState->players[player].fkind;

		scAutoDemoGetPlayerStartPosition(player, &desc.pos);

		desc.lr = (desc.pos.x >= 0.0F) ? -1 : +1;
		desc.team = gSCManagerBattleState->players[player].team;
		desc.player = player;
		desc.detail = ((gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count) < 3) ? nFTPartsDetailHigh : nFTPartsDetailLow;
		desc.costume = gSCManagerBattleState->players[player].costume;
		desc.shade = gSCManagerBattleState->players[player].shade;
		desc.handicap = gSCManagerBattleState->players[player].handicap;
		desc.level = gSCManagerBattleState->players[player].level;
		desc.stock_count = gSCManagerBattleState->stocks;
		desc.damage = gSCManagerBattleState->players[player].stock_damage_all;
		desc.pkind = gSCManagerBattleState->players[player].pkind;
		desc.controller = &gSYControllerDevices[player];
		desc.figatree_heap = ftManagerAllocFigatreeHeapKind(gSCManagerBattleState->players[player].fkind);
		desc.is_skip_entry = TRUE;

		fighter_gobj = ftManagerMakeFighter(&desc);

		gSCManagerBattleState->players[player].color = player;
		gSCManagerBattleState->players[player].tag = player;

		ftParamInitPlayerBattleStats(player, fighter_gobj);
	}
	ftManagerSetupFilesPlayablesAll();
	scAutoDemoStartBattle();
	gmCameraMakePlayerArrowsCamera();
	ifCommonPlayerArrowsInitInterface();
	gmCameraMakePlayerMagnifyCamera();
	ifCommonPlayerMagnifyMakeInterface();

	gIFCommonPlayerInterface.is_magnify_display = TRUE;

	gmCameraScreenFlashMakeCamera();
	gmCameraMakeInterfaceCamera();
	gmCameraMakeEffectCamera();
	ifCommonPlayerTagMakeInterface();
	ifCommonPlayerDamageSetDigitPositions();
	ifCommonPlayerDamageInitInterface();
	ifCommonPlayerDamageSetShowInterface();
	ifCommonPlayerStockInitInterface();
	scAutoDemoInitSObjs();
	mpCollisionSetPlayBGM();
	func_800269C0_275C0(nSYAudioVoicePublicExcited);
	scAutoDemoMakeFocusInterface();
}

// 0x8018DFC8
void scAutoDemoFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);

	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018E014
void scAutoDemoStartScene(void)
{
	dSCAutoDemoVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);

	syVideoInit(&dSCAutoDemoVideoSetup);

	dSCAutoDemoTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl64_BSS_END);
	dSCAutoDemoTaskmanSetup.func_start = scAutoDemoFuncStart;

	scManagerFuncUpdate(&dSCAutoDemoTaskmanSetup);
	syAudioStopBGMAll();

	while (syAudioCheckBGMPlaying(0) != FALSE)
	{
		continue;
	}

	syAudioSetBGMVolume(0, 0x7800);
	func_800266A0_272A0();
	gmRumbleInitPlayers();
}
