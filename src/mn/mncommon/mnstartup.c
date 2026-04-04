
#include <mn/menu.h>
#include <sc/scene.h>
#include <sys/controller.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80131F50
SYColorRGBA dMNStartupEndFadeColor = { 0x00, 0x00, 0x00, 0xFF };

// 0x80131F54
SYColorRGBA dMNStartupStartFadeColor = { 0x00, 0x00, 0x00, 0x00 };

// 0x80131F58
Lights1 dMNStartupLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x0A, 0x32, 0x32);

// 0x80131F70
Gfx dMNStartupDisplayList[/* */] =
{
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPSetLights1(dMNStartupLights1),
	gsSPEndDisplayList()
};

// 0x80131F98
SYVideoSetup dMNStartupVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80131FB4
SYTaskmanSetup dMNStartupTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,                  // Frame draw function
        &ovl58_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 1280,         // Display List Buffer 0 Size
        sizeof(Gfx) * 1280,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x2800,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        mnStartupFuncLights,        // Pre-render function
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
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    mnStartupFuncStart              // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132040
s32 sMNStartupPad0x80132040[2];

// 0x80132048
LBFileNode sMNStartupStatusBuffer[5];

// 0x80132070 - Delay frames before N64 logo can be skipped
s32 sMNStartupSkipAllowWait;

// 0x80132074 - if TRUE, proceed to the opening movie
sb32 sMNStartupIsProceedOpening;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnStartupLogoThreadUpdate(GObj *gobj)
{
	f32 step;
	s32 i;
	SObj *sobj;
	SYColorRGBA color;

	sobj = SObjGetStruct(gobj);

	i = 0;

	while (i < 16)
	{
		step = 16 - i;
		sobj->pos.y = 65.0F - ((-(38.75F / 64.0F) * step) * step);

		gcSleepCurrentGObjThread(1);
		
		i++;
	}
	sobj->pos.y = 65.0F;

	i = 0;

	while (i < 24)
	{
		gcSleepCurrentGObjThread(1);
		i++;
	}
	color = dMNStartupEndFadeColor;

	lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 10, FALSE, NULL);

	i = 0;

	while (i < 13)
	{
		gcSleepCurrentGObjThread(1);
		i++;
	}
	sMNStartupIsProceedOpening = TRUE;

	while (TRUE)
	{
		gcSleepCurrentGObjThread(1);
	}
}

// 0x80131C20
void mnStartupActorFuncRun(GObj *gobj)
{
	if (sMNStartupSkipAllowWait != 0)
	{
		sMNStartupSkipAllowWait--;
	}
	if ((sMNStartupSkipAllowWait == 0) && (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE))
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;
		
		syTaskmanSetLoadScene();
	}
	else if (sMNStartupIsProceedOpening != FALSE)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindOpeningRoom;
		syTaskmanSetLoadScene();
	}
}

// 0x80131CB8
void mnStartupFuncStart(void)
{
	LBRelocSetup rl_setup;
	CObj *cobj;
	GObj *gobj;
	SObj *sobj;
	Sprite *sprite;
	SYColorRGBA color;

	sMNStartupSkipAllowWait = 8;
	sMNStartupIsProceedOpening = FALSE;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMNStartupStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMNStartupStatusBuffer);
	rl_setup.force_status_buffer = NULL;
	rl_setup.force_status_buffer_size = 0;

	lbRelocInitSetup(&rl_setup);

	gcMakeGObjSPAfter(0, mnStartupActorFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));

	cobj = CObjGetStruct
	(
		gcMakeCameraGObj
		(
			nGCCommonKindWallpaperCamera,
			NULL,
			nGCCommonLinkIDCamera,
			GOBJ_PRIORITY_DEFAULT,
			lbCommonDrawSprite,
			80,
			COBJ_MASK_DLLINK(0),
			~0,
			FALSE,
			nGCProcessKindFunc,
			NULL,
			1,
			FALSE
		)
	);
	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

	gobj = gcMakeGObjSPAfter(nGCCommonKindWallpaper, NULL, nGCCommonLinkIDWallpaper, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjProcess(gobj, mnStartupLogoThreadUpdate, nGCProcessKindThread, 1);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);

	sprite = lbRelocGetFileData
	(
		Sprite*,
		lbRelocGetExternHeapFile
		(
			&llN64LogoFileID,
			syTaskmanMalloc
			(
				lbRelocGetFileSize
				(
					&llN64LogoFileID
				),
				0x10
			)
		),
		&llN64LogoSprite
	);
	sobj = lbCommonMakeSObjForGObj(gobj, sprite);

	sobj->sprite.attr &= ~SP_FASTCOPY;

	sobj->pos.x = 96.0F;
	sobj->pos.y = 220.0F;

	color = dMNStartupStartFadeColor;

	lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 16, TRUE, NULL);
}

// 0x80131ECC
void mnStartupFuncLights(Gfx **dls)
{
	gSPDisplayList(dls[0]++, dMNStartupDisplayList);
}

// 0x80131EF0
void mnStartupStartScene(void)
{
	syAudioStopBGMAll();
	
	dMNStartupVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMNStartupVideoSetup);

	dMNStartupTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl58_BSS_END);
	syTaskmanStartTask(&dMNStartupTaskmanSetup);
}
