#include <db/debug.h>
#include <ft/fighter.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132690
sb32 dDBBattleIsExitInterrupt = FALSE;

// 0x80132694
s8 dDBBattleMenuValueScene = 0; // scene

// 0x80132698
s32 dDBBattleCurrentFighterStatus = 0x0002000A;

// 0x8013269C
char *dDBBattleFighterNames[/* */] =
{
	" Mario",
	" Fox",
	" Donkey",
	" Samus",
	" Luigi",
	" Link",
	" Yoshi",
	" Captain",
	" Kirby",
	" Pikacyu",
	" Purin",
	" Nes",
	" Boss",
	" Metal",
	" NMario",
	" NFox",
	" NDonkey",
	" NSamus",
	" NLuigi",
	" NLink",
	" NYoshi",
	" NCaptain",
	" NKirby",
	" NPikacyu",
	" NPurin",
	" NNes",
	" GDonkey"
};

// 0x80132708
char *dDBBattlePlayerKindNames[/* */] =
{
	"Man",
	"Com",
	"Not",
	"Demo",
	"Key",
	"GameKey"
};

// 0x80132720
char *dDBBattleSceneNames[/* */] =
{
	"VS mode",
	"1P mode",
	"Staffroll",
	"Explain",
	"AutoDemo",
#if defined(REGION_US)
	"Congra"
#endif
};

// 0x80132738
char *dDBBattleStageNames[/* */] =
{
	"S:Link",
	"S:Yoshi",
	"S:Fox",
	"S:Bonus1",
	"S:MarioBros",
	"S:Pikacyu",
	"S:GDonkey",
	"S:Bonus2",
	"S:Kirby",
	"S:Samus",
	"S:Metal",
	"S:Bonus3",
	"S:Zako",
	"S:Boss",
	"S:Luigi",
	"S:Nes",
	"S:Purin",
	"S:Captain"
};

// 0x80132780
DBMenuOption dDBBattleMenuOptions[/* */] =
{
#if defined(REGION_US)
	{ nDBMenuOptionKindStringByte,  dbBattleStartBattle,(char*)	dDBBattleSceneNames,      	&dDBBattleMenuValueScene,           				0.0F, 5.0F,  0 },
#else
	{ nDBMenuOptionKindStringByte,  dbBattleStartBattle,(char*)	dDBBattleSceneNames,      	&dDBBattleMenuValueScene,           				0.0F, 4.0F,  0 },
#endif
	{ nDBMenuOptionKindNumericByte, NULL,               (char*) "BattleTime %3d",           &gSCManagerTransferBattleState.time_limit,        	1.0F, 60.0F, 0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattleStageNames,       	&gSCManagerSceneData.spgame_stage,              	0.0F, 17.0F, 0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattlePlayerKindNames,  	&gSCManagerTransferBattleState.players[0].pkind, 	0.0F, 5.0F,  0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattleFighterNames, 		&gSCManagerTransferBattleState.players[0].fkind, 	0.0F, 26.0F, 0 },
	{ nDBMenuOptionKindNumericByte, NULL,               (char*) "  %1d",                   	&gSCManagerTransferBattleState.players[0].costume,	0.0F, 10.0F, 0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattlePlayerKindNames,  	&gSCManagerTransferBattleState.players[1].pkind, 	0.0F, 5.0F,  0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattleFighterNames, 		&gSCManagerTransferBattleState.players[1].fkind, 	0.0F, 26.0F, 0 },
	{ nDBMenuOptionKindNumericByte, NULL,               (char*) "  %1d",                    &gSCManagerTransferBattleState.players[1].costume,	0.0F, 10.0F, 0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattlePlayerKindNames,  	&gSCManagerTransferBattleState.players[2].pkind, 	0.0F, 5.0F,  0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattleFighterNames, 		&gSCManagerTransferBattleState.players[2].fkind, 	0.0F, 26.0F, 0 },
	{ nDBMenuOptionKindNumericByte, NULL,               (char*) "  %1d",                    &gSCManagerTransferBattleState.players[2].costume,	0.0F, 10.0F, 0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattlePlayerKindNames,  	&gSCManagerTransferBattleState.players[3].pkind, 	0.0F, 5.0F,  0 },
	{ nDBMenuOptionKindStringByte,  NULL,               (char*) dDBBattleFighterNames, 		&gSCManagerTransferBattleState.players[3].fkind, 	0.0F, 26.0F, 0 },
	{ nDBMenuOptionKindNumericByte, NULL,               (char*) "  %1d",                    &gSCManagerTransferBattleState.players[3].costume,	0.0F, 10.0F, 0 }
};

// 0x80132924
s32 dDBBattleStartFighterKinds[/* */] = { nFTKindMario, nFTKindDonkey, nFTKindSamus, nFTKindFox };

// 0x80132934
SYVideoSetup dDBBattleVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132950
SYTaskmanSetup dDBBattleTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,   				// Update function
        gcDrawAll,          		// Frame draw function
        &ovl14_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2048,         // Display List Buffer 0 Size
        sizeof(Gfx) * 1024,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        dbBattleFuncLights,   		// Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    8,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    8,                              // Number of thread stacks
    0,                              // ???
    32,                             // Number of GObjProcesses
    32,                             // Number of GObjs
    sizeof(GObj),                   // GObj size
    256,                            // Number of XObjs
    dLBCommonFuncMatrixList,        // Matrix function list
    NULL,                           // DObjVec eject function
    1024,                           // Number of AObjs
    160,                            // Number of MObjs
    256,                            // Number of DObjs
    sizeof(DObj),                   // DObj size
    7,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    8,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    dbBattleFuncStart         		// Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132C20
s32 sDBBattlePad0x80132C20[2];

// 0x80132C28
LBFileNode sDBBattleStatusBuffer[150];

// 0x801330D8
LBFileNode sDBBattleForceStatusBuffer[7];

// 0x80133110
DBFighter sDBBattleFighters[GMCOMMON_PLAYERS_MAX];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void dbBattleFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void dbBattleStartBattle(void)
{
	s32 i;

	dDBBattleIsExitInterrupt = TRUE;
	gSCManagerTransferBattleState.pl_count = gSCManagerTransferBattleState.cp_count = 0;

	for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
	{
		switch (gSCManagerTransferBattleState.players[i].pkind)
		{
		case nFTPlayerKindMan:
			gSCManagerTransferBattleState.pl_count++;
			break;

		case nFTPlayerKindCom:
			gSCManagerTransferBattleState.cp_count++;
			break;
		}
		gSCManagerTransferBattleState.players[i].team = i;
		gSCManagerTransferBattleState.players[i].color = i;
		gSCManagerTransferBattleState.players[i].tag = i;
	}
	gSCManagerTransferBattleState.is_team_battle = FALSE;
}

// 0x80131CD8
s32 dbBattleGetSetShade(s32 player)
{
	s32 i, shade;

	for (i = shade = 0; i != ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
	{
	loop:
		if (i == player)
		{
			continue;
		}
		if
		(
			(gSCManagerTransferBattleState.players[i].fkind == gSCManagerTransferBattleState.players[player].fkind) 	&&
			(gSCManagerTransferBattleState.players[i].costume == gSCManagerTransferBattleState.players[player].costume) &&
			(gSCManagerTransferBattleState.players[i].shade == shade)
		)
		{
			shade++, i = 0;
			goto loop;
		}
	}
	gSCManagerTransferBattleState.players[player].shade = shade;

	return shade;
}

// 0x80131D74
s32 dbBattleGetFighterKindsNum(u16 mask)
{
	s32 i, j;

	for (i = 0, j = 0; i < sizeof(u16) * 8; i++, mask = mask >> 1)
	{
		if (mask & 1)
		{
			j++;
		}
	}
	return j;
}

// 0x80131DEC
s32 dbBattleGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random)
{
	s32 fkind = -1;

	random++;

	do
	{
		fkind++;

		if ((this_mask & LBBACKUP_MASK_FIGHTER(fkind)) && !(prev_mask & LBBACKUP_MASK_FIGHTER(fkind)))
		{
			random--;
		}
	}
	while (random != 0);

	return fkind;
}

// 0x80131E38
void dbBattleSetDemoFighterKinds(void)
{
	s32 non_recently_demoed_count;
	u16 unlocked_mask = (gSCManagerBackupData.fighter_mask | LBBACKUP_CHARACTER_MASK_STARTER);

	if (unlocked_mask == gSCManagerSceneData.demo_mask_prev)
	{
		gSCManagerSceneData.demo_mask_prev = 0;
	}
	non_recently_demoed_count = dbBattleGetFighterKindsNum(unlocked_mask) - dbBattleGetFighterKindsNum(gSCManagerSceneData.demo_mask_prev);

	gSCManagerSceneData.demo_fkind[0] = dbBattleGetShuffledFighterKind(unlocked_mask, gSCManagerSceneData.demo_mask_prev, syUtilsRandIntRange(non_recently_demoed_count));

	if (gSCManagerSceneData.demo_mask_prev == 0)
	{
		gSCManagerSceneData.demo_first_fkind = gSCManagerSceneData.demo_fkind[0];
	}
	gSCManagerSceneData.demo_mask_prev |= LBBACKUP_MASK_FIGHTER(gSCManagerSceneData.demo_fkind[0]);

	non_recently_demoed_count = dbBattleGetFighterKindsNum(unlocked_mask) - dbBattleGetFighterKindsNum(gSCManagerSceneData.demo_mask_prev);

	if (non_recently_demoed_count == 0)
	{
		gSCManagerSceneData.demo_fkind[1] = gSCManagerSceneData.demo_first_fkind;
	}
	else
	{
		gSCManagerSceneData.demo_fkind[1] = dbBattleGetShuffledFighterKind(unlocked_mask, gSCManagerSceneData.demo_mask_prev, syUtilsRandIntRange(non_recently_demoed_count));
		gSCManagerSceneData.demo_mask_prev |= LBBACKUP_MASK_FIGHTER(gSCManagerSceneData.demo_fkind[1]);
	}
}

// 0x80131F3C
void dbBattleFuncRun(GObj *gobj)
{
	void **figatree_heap;
	s32 i;
	GObj *fighter_gobj;
	FTStruct *fp;

	if (gSYControllerMain.button_tap & START_BUTTON)
	{
		if (gDBMenuIsMenuOpen != FALSE)
		{
			dbBattleStartBattle();
		}
		else dbMenuMakeMenu(0x1E, 0x14, 0x55, dDBBattleMenuOptions, ARRAY_COUNT(dDBBattleMenuOptions));
	}
	for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
	{
		if (gSCManagerTransferBattleState.players[i].fkind != sDBBattleFighters[i].fkind)
		{
			FTDesc desc = dFTManagerDefaultFighterDesc;

			fighter_gobj = sDBBattleFighters[i].fighter_gobj;
			fp = ftGetStruct(fighter_gobj);

			figatree_heap = fp->figatree_heap;

			ftManagerDestroyFighter(fighter_gobj);

			desc.fkind = gSCManagerTransferBattleState.players[i].fkind;
			desc.pos.x = (i * 400.0F) - 600.0F;

			if (ftParamGetCostumeDebug(desc.fkind) < gSCManagerTransferBattleState.players[i].costume)
			{
				gSCManagerTransferBattleState.players[i].costume = ftParamGetCostumeDebug(gSCManagerTransferBattleState.players[i].fkind);
			}
			desc.costume = gSCManagerTransferBattleState.players[i].costume;
			desc.shade = dbBattleGetSetShade(i);
			desc.figatree_heap = figatree_heap;

			sDBBattleFighters[i].fighter_gobj = ftManagerMakeFighter(&desc);
			sDBBattleFighters[i].fkind = gSCManagerTransferBattleState.players[i].fkind;
			sDBBattleFighters[i].costume = gSCManagerTransferBattleState.players[i].costume;
		}
		if (gSCManagerTransferBattleState.players[i].costume != sDBBattleFighters[i].costume)
		{
			dbBattleGetSetShade(i);
			ftParamInitAllParts(sDBBattleFighters[i].fighter_gobj, gSCManagerTransferBattleState.players[i].costume, gSCManagerTransferBattleState.players[i].shade);
			sDBBattleFighters[i].costume = gSCManagerTransferBattleState.players[i].costume;
		}
	}
	if (gSYControllerMain.button_tap & U_CBUTTONS)
	{
		scSubsysFighterSetStatus(sDBBattleFighters[1].fighter_gobj, dDBBattleCurrentFighterStatus++);
	}
	if (gSYControllerMain.button_tap & D_CBUTTONS)
	{
		scSubsysFighterSetStatus(sDBBattleFighters[1].fighter_gobj, dDBBattleCurrentFighterStatus--);
	}
	if (dDBBattleIsExitInterrupt != FALSE)
	{
		dbMenuDestroyMenu();

		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;

		switch (dDBBattleMenuValueScene)
		{
		case nDBBattleSceneVSMode:
			gSCManagerSceneData.scene_curr = nSCKindDebugMaps;
			break;

		case nDBBattleScene1PMode:
			gSCManagerSceneData.scene_curr = nSCKind1PGame;
			gSCManagerSceneData.player = 0;
			gSCManagerSceneData.fkind = gSCManagerTransferBattleState.players[0].fkind;
			gSCManagerSceneData.costume = gSCManagerTransferBattleState.players[0].costume;
			break;

		case nDBBattleSceneStaffroll:
			gSCManagerSceneData.scene_curr = nSCKindStaffroll;
			break;
		
		case nDBBattleSceneExplain:
			gSCManagerSceneData.scene_curr = nSCKindExplain;
			gSCManagerTransferBattleState.is_team_battle = FALSE;
			break;
		
		case nDBBattleSceneAutoDemo:
			dbBattleSetDemoFighterKinds();
			gSCManagerSceneData.scene_curr = nSCKindAutoDemo;
			gSCManagerTransferBattleState.is_team_battle = FALSE;
			break;
			
#if defined(REGION_US)
		case nDBBattleSceneCongra:
			gSCManagerSceneData.scene_curr = nSCKindCongra;
			break;
#endif
		}
		syTaskmanSetLoadScene();
	}
}

// 0x80132238
void dbBattleActorProcUpdate(GObj *gobj)
{
	s32 i;

	for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
	{
		if (gSCManagerTransferBattleState.players[i].costume != sDBBattleFighters[i].costume)
		{
			if (gSCManagerTransferBattleState.players[i].costume == 10)
			{
				gSCManagerTransferBattleState.players[i].costume = ftParamGetCostumeDebug(gSCManagerTransferBattleState.players[i].fkind);
			}
			else if (ftParamGetCostumeDebug(gSCManagerTransferBattleState.players[i].fkind) < gSCManagerTransferBattleState.players[i].costume)
			{
				gSCManagerTransferBattleState.players[i].costume = 0;
			}
		}
	}
}

// 0x801322DC
GObj* dbBattleMakeCamera(void (*thread)(GObj*))
{
	GObj *camera_gobj = gcMakeCameraGObj
	(
		nGCCommonKindPlayerSelect,
		gcDefaultFuncRun,
		15,
		GOBJ_PRIORITY_DEFAULT,
		func_80017DBC,
		50,
		COBJ_MASK_DLLINK(18) | COBJ_MASK_DLLINK(15) |
		COBJ_MASK_DLLINK(10) | COBJ_MASK_DLLINK(9),
		~0,
		TRUE,
		nGCProcessKindThread,
		thread,
		1,
		FALSE
	);
	CObj *cobj;

	if (camera_gobj == NULL)
	{
		return NULL;
	}
	cobj = CObjGetStruct(camera_gobj);

	cobj->flags = COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;

	cobj->vec.at.y = 600.0F;
	cobj->vec.eye.z = 3000.0F;
	cobj->vec.eye.y = 600.0F;

	return camera_gobj;
}

// 0x8013239C
void dbBattleFuncStart(void)
{
	s32 i;
	LBRelocSetup rl_setup;
	GObj *gobj;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sDBBattleStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sDBBattleStatusBuffer);
	rl_setup.force_status_buffer = sDBBattleForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sDBBattleForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);

	gobj = gcMakeGObjSPAfter(0, dbBattleFuncRun, 15, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjProcess(gobj, dbBattleActorProcUpdate, nGCProcessKindFunc, 0);

	gcMakeDefaultCameraGObj(15, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	efParticleInitAll();
	dbBattleMakeCamera(NULL);
	efManagerInitEffects();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION | FTDATA_FLAG_SUBMOTION, 4);

	for (i = nFTKindMario; i < nFTKindEnumCount; i++)
	{
		ftManagerSetupFilesAllKind(i);
	}
	for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
	{
		gSCManagerTransferBattleState.players[i].team = i;
		gSCManagerTransferBattleState.players[i].pkind = nFTPlayerKindMan;
		gSCManagerTransferBattleState.players[i].shade = 0;
		gSCManagerTransferBattleState.players[i].fkind = dDBBattleStartFighterKinds[i];
	}
	for (i = 0; i < ARRAY_COUNT(gSCManagerTransferBattleState.players); i++)
	{
		FTDesc desc = dFTManagerDefaultFighterDesc;

		desc.fkind = gSCManagerTransferBattleState.players[i].fkind;
		desc.costume = gSCManagerTransferBattleState.players[i].costume;
		desc.pos.x = (i * 400.0F) - 600.0F;
		desc.figatree_heap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

		sDBBattleFighters[i].fighter_gobj = ftManagerMakeFighter(&desc);
		sDBBattleFighters[i].fkind = gSCManagerTransferBattleState.players[i].fkind;
		sDBBattleFighters[i].costume = gSCManagerTransferBattleState.players[i].costume;
	}
	dbMenuInitMenu();
	dbMenuMakeMenu(0x1E, 0x14, 0x55, dDBBattleMenuOptions, ARRAY_COUNT(dDBBattleMenuOptions));
	scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);
}

// 0x80132638
void dbBattleStartScene(void)
{
	dDBBattleVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dDBBattleVideoSetup);

	dDBBattleTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl9_VRAM - (uintptr_t)&ovl14_BSS_END);
	syTaskmanStartTask(&dDBBattleTaskmanSetup);
}
