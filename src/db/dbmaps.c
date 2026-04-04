#include <db/debug.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/controller.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x800D6680
sb32 dDBMapsIsExit = FALSE;

// 0x800D6684
s32 dDBMapsGroundKind = nGRKindCastle;

// 0x800D6688
char *dDBMapsGroundNames[/* */] =
{
	"Mario",
	"Fox",
	"Donkey",
	"Samus",
	"Link",
	"Yoshi",
	"Kirby",
	"Pikacyu",
	"OldMario",
	"Small",
	"New",
	"Explain",
	"SYoshi",
	"Metal",
	"Zako",
	"Bonus3",
	"Boss",
	"Bonus1Mario",
	"Bonus1Fox",
	"Bonus1Donkey",
	"Bonus1Samus",
	"Bonus1Luigi",
	"Bonus1Link",
	"Bonus1Yoshi",
	"Bonus1Captain",
	"Bonus1Kirby",
	"Bonus1Pikachu",
	"Bonus1Purin",
	"Bonus1Nes",
	"Bonus2Mario",
	"Bonus2Fox",
	"Bonus2Donkey",
	"Bonus2Samus",
	"Bonus2Luigi",
	"Bonus2Link",
	"Bonus2Yoshi",
	"Bonus2Captain",
	"Bonus2Kirby",
	"Bonus2Pikachu",
	"Bonus2Purin",
	"Bonus2Nes"
};

// 0x800D672C
DBMenuOption dDBMapsMenuOptions[/* */] =
{
	// Exit
	{
		nDBMenuOptionKindExitLabel,
		dbMapsExitFuncMenu,
		"Exit",
		NULL,
		0.0F,
		0.0F,
		0.0F
	},

	// Stage
	{
		nDBMenuOptionKindString,
		NULL,
		(char*)dDBMapsGroundNames,
		&dDBMapsGroundKind,
		0.0F,
		40.0F,
		0.0F
	}
};

// 0x800D6764
SYVideoSetup dDBMapsVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x800D6780
SYTaskmanSetup dDBMapsTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,          			// Update function
        gcDrawAll,                  // Frame draw function
        &ovl12_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        1,                          // Number of contexts?
        sizeof(Gfx) * 1750,         // Display List Buffer 0 Size
        0,         					// Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x1000,                     // Graphics Heap Size
        2,                          // ???
        0x1000,                     // RDP Output Buffer Size
        NULL,         				// Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    8,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    8,                              // Number of thread stacks
    0,                              // ???
    32,                             // Number of GObjProcesses
    64,                             // Number of GObjs
    sizeof(GObj),                   // GObj size
    256,                            // Number of XObjs
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    32,                             // Number of AObjs
    16,                             // Number of MObjs
    64,                             // Number of DObjs
    sizeof(DObj),                   // DObj size
    7,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    8,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    dbMapsFuncStart         		// Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D69E0
s32 sDBMapsPad0x800D69E0;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D6490
void dbMapsExitFuncMenu(void)
{
	dDBMapsIsExit = TRUE;
}

// 0x800D64A0
void dbMapsFuncRun(GObj *gobj)
{
	if (gSYControllerMain.button_tap & START_BUTTON)
	{
		if (gDBMenuIsMenuOpen != FALSE)
		{
			dbMapsExitFuncMenu();
		}
		else dbMenuMakeMenu(0x32, 0x32, 0x64, dDBMapsMenuOptions, ARRAY_COUNT(dDBMapsMenuOptions));
	}
	if (dDBMapsIsExit != FALSE)
	{
		dbMenuDestroyMenu();

		gSCManagerSceneData.gkind = dDBMapsGroundKind;
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;

		if (gSCManagerSceneData.gkind >= nGRKindBonusStageStart)
		{
			gSCManager1PGameBattleState = gSCManagerTransferBattleState;
			gSCManager1PGameBattleState.gkind = gSCManagerSceneData.gkind;
			gSCManagerSceneData.scene_curr = nSCKind1PBonusStage;
		}
		else gSCManagerSceneData.scene_curr = nSCKindVSBattle;

		syTaskmanSetLoadScene();
	}
}

// 0x800D65AC
void dbMapsFuncStart(void)
{
	gcMakeGObjSPAfter(0, dbMapsFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	dbMenuInitMenu();
	dbMenuMakeMenu(0x32, 0x32, 0x64, dDBMapsMenuOptions, ARRAY_COUNT(dDBMapsMenuOptions));
}

// 0x800D6620
void dbMapsStartScene(void)
{
	dDBMapsVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dDBMapsVideoSetup);

	dDBMapsTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl9_VRAM - (uintptr_t)&ovl12_BSS_END);
	syTaskmanStartTask(&dDBMapsTaskmanSetup);
}
