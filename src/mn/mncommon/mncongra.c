#include <ft/fighter.h>
#include <mn/menu.h>
#include <sc/scene.h> // includes sys/obj.h
#include <sys/video.h>
#include <sys/controller.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern void* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132100
MNCongraPicture dMNCongraPictures[/* */] =
{
	// Mario
	{ 
		&llMNCongraMarioBottomFileID,         &llMNCongraMarioBottomSprite, 
		&llMNCongraMarioTopFileID,         &llMNCongraMarioTopSprite 
	},

	// Fox
	{ 
		&llMNCongraFoxBottomFileID, &llMNCongraFoxBottomSprite,
		&llMNCongraFoxTopFileID,    &llMNCongraFoxTopSprite
	},

	// Donkey Kong
	{ 
		&llMNCongraDonkeyBottomFileID, &llMNCongraDonkeyBottomSprite,
		&llMNCongraDonkeyTopFileID,    &llMNCongraDonkeyTopSprite
	},

	// Samus
	{ 
		&llMNCongraSamusBottomFileID, &llMNCongraSamusBottomSprite,
		&llMNCongraSamusTopFileID,    &llMNCongraSamusTopSprite
	},

	// Luigi
	{ 
		&llMNCongraLuigiBottomFileID, &llMNCongraLuigiBottomSprite,
		&llMNCongraLuigiTopFileID,    &llMNCongraLuigiTopSprite
	},

	// Link
	{
		&llMNCongraLinkBottomFileID, &llMNCongraLinkBottomSprite,
		&llMNCongraLinkTopFileID,    &llMNCongraLinkTopSprite
	},

	// Yoshi
	{
		&llMNCongraYoshiBottomFileID, &llMNCongraYoshiBottomSprite,
		&llMNCongraYoshiTopFileID,    &llMNCongraYoshiTopSprite
	},

	// Captain Falcon
	{ 
		&llMNCongraCaptainBottomFileID, &llMNCongraCaptainBottomSprite,
		&llMNCongraCaptainTopFileID,    &llMNCongraCaptainTopSprite
	},

	// Kirby
	{
		&llMNCongraKirbyBottomFileID, &llMNCongraKirbyBottomSprite,
		&llMNCongraKirbyTopFileID,    &llMNCongraKirbyTopSprite
	},

	// Pikachu
	{
		&llMNCongraPikachuBottomFileID, &llMNCongraPikachuBottomSprite,
		&llMNCongraPikachuTopFileID,    &llMNCongraPikachuTopSprite
	},

	// Jigglypuff
	{
		&llMNCongraPurinBottomFileID, &llMNCongraPurinBottomSprite,
		&llMNCongraPurinTopFileID,    &llMNCongraPurinTopSprite
	},

	// Ness
	{
		&llMNCongraNessBottomFileID, &llMNCongraNessBottomSprite,
		&llMNCongraNessTopFileID,    &llMNCongraNessTopSprite
	}
};

// 0x801321C0
SYColorRGBA dMNCongraFadeColor = { 0x00, 0x00, 0x00, 0xFF };

// 0x801321C4
SYColorRGBA D_ovl57_801321C4 = { 0x00, 0x00, 0x00, 0x00 };

// 0x801321C8
Lights1 dMNCongraLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x0A, 0x32, 0x32);

// 0x801321E0
Gfx dMNCongraDisplayList[/* */] =
{
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPSetLights1(dMNCongraLights1),
	gsSPEndDisplayList()
};

// 0x80132208
SYVideoSetup dMNCongraVideoSetup =
{
	SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(320, 230, 0, 10, u32, 0),
	SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(320, 230, 0, 10, u32, 1),
	SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(320, 230, 0, 10, u32, 2),
	NULL,
	320,
	240, 
	SYVIDEO_FLAG_DIVOT        | SYVIDEO_FLAG_DITHERFILTER | SYVIDEO_FLAG_GAMMADITHER |
	0x800                     | SYVIDEO_FLAG_NOBLACKOUT   | SYVIDEO_FLAG_NOGAMMA     |
    SYVIDEO_FLAG_COLORDEPTH32 | SYVIDEO_FLAG_NOSERRATE    | 0x1
};

// 0x80132224
SYTaskmanSetup dMNCongraTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        mnCongraFuncDraw,           // Frame draw function
        &ovl57_BSS_END,             // Allocatable memory pool start
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
        mnCongraFuncLights,         // Pre-render function
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
    sizeof(CObj),                 	// Camera size
    
    mnCongraFuncStart               // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801322B0
s32 sMNCongraPad0x801322B0[2];

// 0x801322B8
LBFileNode sMNCongraStatusBuffer[5];

// 0x801322E0
s32 sMNCongraFighterKind;

// 0x801322E4
s32 sMNCongraSkipWait;

// 0x801322E8
s32 sMNCongraSceneChangeWait;

// 0x801322EC
sb32 sMNCongraIsProceed;

// 0x801322F0
sb32 sMNCongraIsProceedScene;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
sb32 mnCongraCheckPlayerControllerConnected(s32 player)
{
	s32 i;

	for (i = 0; i < ARRAY_COUNT(gSYControllerDeviceStatuses); i++)
	{
		if (player == gSYControllerDeviceStatuses[i])
		{
			return TRUE;
		}	
	}
	return FALSE;
}

// 0x80131B6C
s32 mnCongraGetPlayerTapButtons(u32 buttons)
{
	s32 player;

	for (player = 0; player < ARRAY_COUNT(gSYControllerDevices); player++)
	{
		if
		(
			(mnCongraCheckPlayerControllerConnected(player) != FALSE) &&
			(gSYControllerDevices[player].button_tap & buttons)
		)
		{
			return player + 1;
		}
	}
	return 0;
}

// 0x80131C04
void mnCongraActorFuncRun(GObj *gobj)
{
	SYColorRGBA color;

	if (sMNCongraSkipWait != 0)
	{
		sMNCongraSkipWait--;
	}
	if
	(
		(sMNCongraSkipWait == 0) 		&& 
		(sMNCongraIsProceed == FALSE) 	&& 
		(mnCongraGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
	)
	{
		sMNCongraIsProceed = TRUE;
		color = dMNCongraFadeColor;
		lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 90, FALSE, &sMNCongraIsProceedScene);
	}
}

// 0x80131CA4
void mnCongraFuncStart(void)
{
	LBRelocSetup rl_setup;
	CObj *cobj;
	GObj *gobj;
	SObj *sobj;

	sMNCongraSkipWait = 8;
	sMNCongraSceneChangeWait = 0;
	sMNCongraIsProceed = FALSE;
	sMNCongraIsProceedScene = 0;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMNCongraStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMNCongraStatusBuffer);
	rl_setup.force_status_buffer = NULL;
	rl_setup.force_status_buffer_size = 0;

	lbRelocInitSetup(&rl_setup);

	gcMakeGObjSPAfter(0, mnCongraActorFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
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

	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);

	sobj = lbCommonMakeSObjForGObj
	(
		gobj,
		lbRelocGetFileData
		(
			Sprite*,
			lbRelocGetExternHeapFile
			(
				dMNCongraPictures[sMNCongraFighterKind].bottom_file_id,
				syTaskmanMalloc
				(
					lbRelocGetFileSize
					(
						dMNCongraPictures[sMNCongraFighterKind].bottom_file_id
					),
					0x10
				)
			),
			dMNCongraPictures[sMNCongraFighterKind].bottom_offset
		)
	);
	sobj->sprite.attr &= ~SP_FASTCOPY;

	sobj->pos.x = 10.0F;
	sobj->pos.y = 120.0F;

	sobj = lbCommonMakeSObjForGObj
	(
		gobj,
		lbRelocGetFileData
		(
			Sprite*,
			lbRelocGetExternHeapFile
			(
				dMNCongraPictures[sMNCongraFighterKind].top_file_id,
				syTaskmanMalloc
				(
					lbRelocGetFileSize
					(
						dMNCongraPictures[sMNCongraFighterKind].top_file_id
					),
					0x10
				)
			),
			dMNCongraPictures[sMNCongraFighterKind].top_offset
		)
	);
	sobj->sprite.attr &= ~SP_FASTCOPY;

	sobj->pos.x = 10.0F;
	sobj->pos.y = 10.0F;

	func_800269C0_275C0
	(
		(gSCManagerSceneData.spgame_score >= 1000000) ? nSYAudioVoiceAnnounceIncredible : nSYAudioVoiceAnnounceCongra
	);
}

// 0x80131F60
void mnCongraFuncDraw(void)
{
	gcDrawAll();

	if (sMNCongraIsProceedScene != FALSE)
	{
		sMNCongraIsProceedScene = FALSE;

		syVideoSetFlags(SYVIDEO_FLAG_BLACKOUT);

		sMNCongraSceneChangeWait = 5;
	}
	if (sMNCongraSceneChangeWait != 0)
	{
		sMNCongraSceneChangeWait--;

		if (sMNCongraSceneChangeWait == 0)
		{
			gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
			gSCManagerSceneData.scene_curr = nSCKindTitle;

			syTaskmanSetLoadScene();
		}
	}
}

// 0x80131FE8
void mnCongraFuncLights(Gfx **dls)
{
	gSPDisplayList(dls[0]++, dMNCongraDisplayList);
}

// 0x8013200C
void mnCongraStartScene(void)
{
	u16 *fb16;
	u32 *fb32;

	fb32 = (u32*)SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(320, 230, 0, 10, u32, 0);

	while ((uintptr_t)fb32 < 0x80400000) { *fb32++ = GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF); } // WARNING: Newline memes!

	switch (gSCManagerSceneData.scene_prev)
	{
	default:
		sMNCongraFighterKind = nFTKindMario;
		break;

	case nSCKind1PGame:
		sMNCongraFighterKind = gSCManagerSceneData.fkind;
		break;

	case nSCKindDebugBattle:
		sMNCongraFighterKind = gSCManagerTransferBattleState.players[0].fkind;
		break;
	}
	dMNCongraVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMNCongraVideoSetup);

	dMNCongraTaskmanSetup.scene_setup.arena_size = (size_t) (SYVIDEO_DEFINE_FRAMEBUFFER_ADDR(320, 230, 0, 10, u32, 0) - (uintptr_t)&ovl57_BSS_END);
	syTaskmanStartTask(&dMNCongraTaskmanSetup); fb16 = (u16*) gSYFramebufferSets; // WARNING: Newline memes!

	while ((uintptr_t)fb16 < 0x80400000) { *fb16++ = GPACK_RGBA5551(0x00, 0x00, 0x00, 0x01); }
}
