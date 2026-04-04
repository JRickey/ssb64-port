#include <ft/fighter.h>
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

// 0x801328A0
u32 dMVOpeningPortraitsFileIDs[/* */] = { &llMVOpeningPortraitsSet1FileID, &llMVOpeningPortraitsSet2FileID };

// 0x801328A8
Lights1 dMVOpeningPortraitsLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x801328C0
Lights1 dMVOpeningPortraitsLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x00);

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801329E0
s32 sMVOpeningPortraitsPad0x801329E0[2];

// 0x801329E8
s32 sMVOpeningPortraitsTotalTimeTics;

// 0x801329EC
s32 sMVOpeningPortraitsRow;

// 0x801329F0
GObj *sMVOpeningPortraitsGObj;

// 0x801329F4
s32 sMVOpeningPortraitsUnused0x801329F4;

// 0x801329F8
LBFileNode sMVOpeningPortraitsStatusBuffer[48];

// 0x80132B78
LBFileNode sMVOpeningPortraitsForceStatusBuffer[7];

// 0x80132BB0
void *sMVOpeningPortraitsFiles[ARRAY_COUNT(dMVOpeningPortraitsFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mvOpeningPortraitsFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
void mvOpeningPortraitsMakeSet1(void)
{
	GObj *gobj;
	SObj *sobj;
	s32 i;

	intptr_t offsets[/* */] =
	{
		&llMVOpeningPortraitsSet1SamusSprite,
		&llMVOpeningPortraitsSet1MarioSprite,
		&llMVOpeningPortraitsSet1FoxSprite,
		&llMVOpeningPortraitsSet1PikachuSprite
	};
	Vec2f pos[/* */] =
	{
		{ 10.0F,  10.0F },
		{ 10.0F,  65.0F },
		{ 10.0F, 120.0F },
		{ 10.0F, 175.0F }
	};

	sMVOpeningPortraitsGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; i < ARRAY_COUNT(offsets); i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningPortraitsFiles[0], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;

		sobj->pos.x = pos[i].x;
		sobj->pos.y = pos[i].y;
	}
}

// 0x80131CAC
void mvOpeningPortraitsMakeSet2(void)
{
	GObj *gobj;
	SObj *sobj;
	s32 i;

	intptr_t offsets[/* */] =
	{
		&llMVOpeningPortraitsSet2LinkSprite,
		&llMVOpeningPortraitsSet2KirbySprite,
		&llMVOpeningPortraitsSet2DonkeySprite,
		&llMVOpeningPortraitsSet2YoshiSprite
	};
	Vec2f pos[/* */] =
	{
		{ 10.0F,  10.0F },
		{ 10.0F,  65.0F },
		{ 10.0F, 120.0F },
		{ 10.0F, 175.0F }
	};

	sMVOpeningPortraitsGObj = gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; i < ARRAY_COUNT(offsets); i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningPortraitsFiles[1], offsets[i]));

		sobj->sprite.attr &= ~SP_FASTCOPY;

		sobj->pos.x = pos[i].x;
		sobj->pos.y = pos[i].y;
	}
}

// 0x80131E00
void mvOpeningPortraitsBlockRow0(void)
{
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 65);
}

// 0x80131E30
void mvOpeningPortraitsBlockRow1(void)
{
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 65, 310, 120);
}

// 0x80131E60
void mvOpeningPortraitsBlockRow2(void)
{
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 120, 310, 175);
}

// 0x80131E90
void mvOpeningPortraitsBlockRow3(void)
{
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 175, 310, 230);
}

// 0x80131EC0
void mvOpeningPortraitsBlockPartialRow(s32 row, s32 pos_x)
{
	s32 uly = 10 + row * 55;
	s32 lry = 65 + row * 55;

	if (pos_x > 0)
	{
		gDPFillRectangle(gSYTaskmanDLHeads[0]++, 0, uly, pos_x, lry);
	}
	if ((pos_x + 656) < 0)
	{
		gDPFillRectangle(gSYTaskmanDLHeads[0]++, 0, uly, 320, lry);
	}
	if ((pos_x + 656) < 320)
	{
		gDPFillRectangle(gSYTaskmanDLHeads[0]++, pos_x + 656, uly, 320, lry);
	}
}

// 0x80131FC4
void mvOpeningPortraitsCoverProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

	switch (sMVOpeningPortraitsRow)
	{
	case 0:
		mvOpeningPortraitsBlockRow1();
		mvOpeningPortraitsBlockRow2();
		mvOpeningPortraitsBlockRow3();
		mvOpeningPortraitsBlockPartialRow(0, SObjGetStruct(gobj)->pos.x);
		break;
		
	case 1:
		mvOpeningPortraitsBlockRow0();
		mvOpeningPortraitsBlockRow2();
		mvOpeningPortraitsBlockRow3();
		mvOpeningPortraitsBlockPartialRow(1, SObjGetStruct(gobj)->pos.x);
		break;
		
	case 2:
		mvOpeningPortraitsBlockRow0();
		mvOpeningPortraitsBlockRow1();
		mvOpeningPortraitsBlockRow3();
		mvOpeningPortraitsBlockPartialRow(2, SObjGetStruct(gobj)->pos.x);
		break;
		
	case 3:
		mvOpeningPortraitsBlockRow0();
		mvOpeningPortraitsBlockRow1();
		mvOpeningPortraitsBlockRow2();
		mvOpeningPortraitsBlockPartialRow(3, SObjGetStruct(gobj)->pos.x);
		break;
	}
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

	lbCommonClearExternSpriteParams();
	lbCommonDrawSObjAttr(gobj);
}

// 0x801321FC
void mvOpeningPortraitsCoverProcUpdate(GObj *gobj)
{
	SObj *sobj = SObjGetStruct(gobj);

	if (sMVOpeningPortraitsTotalTimeTics == 75)
	{
		sobj->pos.x = -656.0F;
	}
	if (sMVOpeningPortraitsTotalTimeTics < 75)
	{
		if (sobj->pos.x < 656.0F)
		{
			sobj->pos.x += 93.0F;

			if (sobj->pos.x > 656.0F)
			{
				sobj->pos.x = 656.0F;
			}
		}
	}
	else if (sobj->pos.x > -656.0F)
	{
		sobj->pos.x -= 93.0F;

		if (sobj->pos.x < -656.0F)
		{
			sobj->pos.x = -656.0F;
		}
	}
	switch (sMVOpeningPortraitsTotalTimeTics)
	{
	case 15:
		sobj->pos.x = -656.0F;
		sobj->pos.y = 10.0F;
		sMVOpeningPortraitsRow = 0;
		break;

	case 45:
		sobj->pos.x = -656.0F;
		sobj->pos.y = 65.0F;
		sMVOpeningPortraitsRow = 1;
		break;

	case 30:
		sobj->pos.x = -656.0F;
		sobj->pos.y = 120.0F;
		sMVOpeningPortraitsRow = 2;
		break;

	case 60:
		sobj->pos.x = -656.0F;
		sobj->pos.y = 175.0F;
		sMVOpeningPortraitsRow = 3;
		break;

	case 105:
		sobj->pos.x = 656.0F;
		sobj->pos.y = 10.0F;
		sMVOpeningPortraitsRow = 0;
		break;

	case 135:
		sobj->pos.x = 656.0F;
		sobj->pos.y = 65.0F;
		sMVOpeningPortraitsRow = 1;
		break;

	case 90:
		sobj->pos.x = 656.0F;
		sobj->pos.y = 120.0F;
		sMVOpeningPortraitsRow = 2;
		break;

	case 120:
		sobj->pos.x = 656.0F;
		sobj->pos.y = 175.0F;
		sMVOpeningPortraitsRow = 3;
		break;
	}
}

// 0x80132414
void mvOpeningPortraitsMakeCover(void)
{
	GObj *gobj;
	SObj *sobj;

	gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, mvOpeningPortraitsCoverProcDisplay, 28, GOBJ_PRIORITY_DEFAULT, ~0);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMVOpeningPortraitsFiles[0], &llMVOpeningPortraitsSet1CoverSprite));

	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;

	sobj->sprite.red = 0x00;
	sobj->sprite.green = 0x00;
	sobj->sprite.blue = 0x00;

	sobj->pos.x = 656.0F;
	sobj->pos.y = 10.0F;

	gcAddGObjProcess(gobj, mvOpeningPortraitsCoverProcUpdate, nGCProcessKindFunc, 1);
}

// 0x801324D0
void mvOpeningPortraitsMakePortraitsCamera(void)
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

// 0x80132570
void mvOpeningPortraitsMakeCoverCamera(void)
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
			60,
			COBJ_MASK_DLLINK(28),
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

// 0x80132610
void mvOpeningPortraitsInitVars(void)
{
	sMVOpeningPortraitsTotalTimeTics = 0;
	sMVOpeningPortraitsRow = 0;
}

// 0x80132624
void mvOpeningPortraitsFuncRun(GObj *gobj)
{
	sMVOpeningPortraitsTotalTimeTics++;

	if (sMVOpeningPortraitsTotalTimeTics >= 10)
	{
		// sMVOpeningPortraitsUnused0x801329F4 gets set to 0 if any diagonal stick press occurs...
		// This scene must have been interactive at some point
		if (sMVOpeningPortraitsUnused0x801329F4 != 0)
		{
			sMVOpeningPortraitsUnused0x801329F4--;
		}
		if ((scSubsysControllerGetPlayerStickInRangeLR(-15, 15) != FALSE) && (scSubsysControllerGetPlayerStickInRangeUD(-15, 15) != FALSE))
		{
			sMVOpeningPortraitsUnused0x801329F4 = 0;
		}
		if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON))
		{
			gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
			gSCManagerSceneData.scene_curr = nSCKindTitle;

			syTaskmanSetLoadScene();
		}
		if (sMVOpeningPortraitsTotalTimeTics == 75)
		{
			gcEjectGObj(sMVOpeningPortraitsGObj);
			mvOpeningPortraitsMakeSet2();
		}
		if (sMVOpeningPortraitsTotalTimeTics == 150)
		{
			gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
			gSCManagerSceneData.scene_curr = nSCKindOpeningMario;

			syTaskmanSetLoadScene();
		}
	}
}

// 0x80132718
void mvOpeningPortraitsFuncStart(void)
{
	s32 unused;
	LBRelocSetup rl_setup;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMVOpeningPortraitsStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMVOpeningPortraitsStatusBuffer);
	rl_setup.force_status_buffer = sMVOpeningPortraitsForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMVOpeningPortraitsForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMVOpeningPortraitsFileIDs, sMVOpeningPortraitsFiles);
	gcMakeGObjSPAfter(0, mvOpeningPortraitsFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	mvOpeningPortraitsInitVars();
	mvOpeningPortraitsMakePortraitsCamera();
	mvOpeningPortraitsMakeCoverCamera();
	mvOpeningPortraitsMakeSet1();
	mvOpeningPortraitsMakeCover();

	while (sySchedulerGetTicCount() < 1335)
	{
		continue;
	}
}

// 0x80132938
SYVideoSetup dMVOpeningPortraitsVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132954
SYTaskmanSetup dMVOpeningPortraitsTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                       // Update function
        gcDrawAll,                  // Frame draw function
        &ovl35_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        2,                              // Number of contexts?
        sizeof(Gfx) * 5000,             // Display List Buffer 0 Size
        sizeof(Gfx) * 512,              // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x8000,                         // Graphics Heap Size
        2,                              // ???
        0xC000,                         // RDP Output Buffer Size
        mvOpeningPortraitsFuncLights,   // Pre-render function
        syControllerFuncRead,           // Controller I/O function
    },

    8,                                  // Number of GObjThreads
    sizeof(u64) * 192,                  // Thread stack size
    8,                                  // Number of thread stacks
    0,                                  // ???
    128,                                // Number of GObjProcesses
    128,                                // Number of GObjs
    sizeof(GObj),                       // GObj size
    256,                                // Number of XObjs
    dLBCommonFuncMatrixList,            // Matrix function list
    NULL,                               // DObjVec eject function
    512,                                // Number of AObjs
    160,                                // Number of MObjs
    256,                                // Number of DObjs
    sizeof(DObj),                       // DObj size
    128,                                // Number of SObjs
    sizeof(SObj),                       // SObj size
    16,                                 // Number of CObjs
    sizeof(CObj),                       // Camera size
    
    mvOpeningPortraitsFuncStart         // Task start function
};

// 0x8013283C
void mvOpeningPortraitsStartScene(void)
{
	dMVOpeningPortraitsVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMVOpeningPortraitsVideoSetup);

	dMVOpeningPortraitsTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl35_BSS_END);
	syTaskmanStartTask(&dMVOpeningPortraitsTaskmanSetup);
}
