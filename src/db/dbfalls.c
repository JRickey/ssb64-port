#include <db/debug.h>
#include <ft/fighter.h>
#include <sc/scene.h>
#include <sys/video.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x800D66E0
Lights1 dDBFallsLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x800D66F8
Gfx dDBFallsDisplayList[/* */] =
{
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPSetLights1(dDBFallsLights1),
	gsSPEndDisplayList()
};

// 0x800D6720
sb32 dDBFallsIsExitInterrupt = FALSE;

// 0x800D6724 - UB: Menu expects 27 like in debug-battle.c but here only 5 are defined
char *dDBFallsFighterNames[/* */] =
{
	"Mario",
	"Fox",
	"Donkey",
	"Samus",
	"Luigi"
};

// 0x800D6738
DBMenuOption dDBFallsMenuOptions[/* */] =
{
	{ nDBMenuOptionKindExitLabel,  dbFallsExit, (char*) "Exit",              NULL,                                     			0.0F, 0.0F,     0 },
	{ nDBMenuOptionKindStringByte, NULL,        (char*) dDBFallsFighterNames,&gSCManagerTransferBattleState.players[0].fkind, 	0.0F, 26.0F,    0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Dead : %3d",       &gSCManagerTransferBattleState.players[0].falls,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Finish : %3d",     &gSCManagerTransferBattleState.players[0].score,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindStringByte, NULL,        (char*) dDBFallsFighterNames,&gSCManagerTransferBattleState.players[1].fkind, 	0.0F, 26.0F,    0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Dead : %3d",       &gSCManagerTransferBattleState.players[1].falls,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Finish : %3d",     &gSCManagerTransferBattleState.players[1].score,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindStringByte, NULL,        (char*) dDBFallsFighterNames,&gSCManagerTransferBattleState.players[2].fkind, 	0.0F, 26.0F,    0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Dead : %3d",       &gSCManagerTransferBattleState.players[2].falls,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Finish : %3d",     &gSCManagerTransferBattleState.players[2].score,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindStringByte, NULL,        (char*) dDBFallsFighterNames,&gSCManagerTransferBattleState.players[3].fkind, 	0.0F, 26.0F,    0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Dead : %3d",       &gSCManagerTransferBattleState.players[3].falls,   0.0F, 65536.0F, 0 },
	{ nDBMenuOptionKindNumeric,    NULL,        (char*) " Finish : %3d",     &gSCManagerTransferBattleState.players[3].score,	0.0F, 65536.0F, 0 }
};

// 0x800D68A4
SYVideoSetup dDBFallsVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x800D68C0
SYTaskmanSetup dDBFallsTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,   				// Update function
        gcDrawAll,          		// Frame draw function
        &ovl15_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2048,         // Display List Buffer 0 Size
        0,         					// Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        dbFallsFuncLights,   		// Pre-render function
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
    NULL,        					// Matrix function list
    NULL,                           // DObjVec eject function
    1024,                           // Number of AObjs
    0,                            	// Number of MObjs
    256,                            // Number of DObjs
    sizeof(DObj),                   // DObj size
    7,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    8,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    dbFallsFuncStart         		// Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D6A00
s32 sDBFallsPad0x800D6A00;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D6490
void dbFallsFuncLights(Gfx **dls)
{
	gSPDisplayList(dls[0]++, dDBFallsDisplayList);
}

// 0x800D64B4
void dbFallsExit(void)
{
	dDBFallsIsExitInterrupt = TRUE;
}

// 0x800D64C4
void dbFallsFuncRun(GObj *gobj)
{
	if (gSYControllerMain.button_tap & START_BUTTON)
	{
		dbMenuMakeMenu(0x32, 0x32, 0x64, dDBFallsMenuOptions, ARRAY_COUNT(dDBFallsMenuOptions));
	}
	if (dDBFallsIsExitInterrupt != FALSE)
	{
		dbMenuDestroyMenu();

		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindDebugBattle;

		syTaskmanSetLoadScene();
	}
}

// 0x800D6544
GObj* dbFallsMakeCamera(void (*thread)(GObj*))
{
	GObj *camera_gobj = gcMakeCameraGObj
	(
		0x10000002,
		gcDefaultFuncRun,
		0,
		GOBJ_PRIORITY_DEFAULT,
		func_80017DBC,
		50,
		COBJ_MASK_DLLINK(0),
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

	cobj->vec.at.y = 300.0F;
	cobj->vec.eye.z = 3000.0F;
	cobj->vec.eye.y = 600.0F;

	return camera_gobj;
}

// 0x800D660C
void dbFallsFuncStart(void)
{
	gcMakeGObjSPAfter(0, dbFallsFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	dbFallsMakeCamera(NULL);
	dbMenuInitMenu();
	dbMenuMakeMenu(0x32, 0x32, 0x64, dDBFallsMenuOptions, ARRAY_COUNT(dDBFallsMenuOptions));
}

// 0x800D6688
void dbFallsStartScene(void)
{
	dDBFallsVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dDBFallsVideoSetup);

	dDBFallsTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl15_BSS_END);
	syTaskmanStartTask(&dDBFallsTaskmanSetup);
}
