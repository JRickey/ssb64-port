#include <mn/menu.h>
#include <ft/fighter.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern void dbMenuUpdateMenuInputs();

extern u32 sySchedulerGetTicCount();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801341E0
intptr_t dMNTitleLogoAnimSprites[/* */] =
{
	&llMNTitleLogoAnimCutoutSprite,  // Full circle cutout
	&llMNTitleLogoAnimStrikeVSprite, // Vertical rectangle
	&llMNTitleLogoAnimStrikeHSprite, // Horizontal rectangle
	&llMNTitleLogoAnimFullSprite     // Full sprite? Unused?
};

// 0x801341F0
intptr_t dMNTitleFireSpriteOffsets[/* */] =
{
	&llMNTitleFireAnimFrame1Sprite,
	&llMNTitleFireAnimFrame2Sprite,
	&llMNTitleFireAnimFrame3Sprite,
	&llMNTitleFireAnimFrame4Sprite,
	&llMNTitleFireAnimFrame5Sprite,
	&llMNTitleFireAnimFrame6Sprite,
	&llMNTitleFireAnimFrame7Sprite,
	&llMNTitleFireAnimFrame8Sprite,
	&llMNTitleFireAnimFrame9Sprite,
	&llMNTitleFireAnimFrame10Sprite,
	&llMNTitleFireAnimFrame11Sprite,
	&llMNTitleFireAnimFrame12Sprite,
	&llMNTitleFireAnimFrame13Sprite,
	&llMNTitleFireAnimFrame14Sprite,
	&llMNTitleFireAnimFrame15Sprite,
	&llMNTitleFireAnimFrame16Sprite,
	&llMNTitleFireAnimFrame17Sprite,
	&llMNTitleFireAnimFrame18Sprite,
	&llMNTitleFireAnimFrame19Sprite,
	&llMNTitleFireAnimFrame20Sprite,
	&llMNTitleFireAnimFrame21Sprite,
	&llMNTitleFireAnimFrame22Sprite,
	&llMNTitleFireAnimFrame23Sprite,
	&llMNTitleFireAnimFrame24Sprite,
	&llMNTitleFireAnimFrame25Sprite,
	&llMNTitleFireAnimFrame26Sprite,
	&llMNTitleFireAnimFrame27Sprite,
	&llMNTitleFireAnimFrame28Sprite,
	&llMNTitleFireAnimFrame29Sprite,
	&llMNTitleFireAnimFrame30Sprite
};

// 0x80134268
MNTitleSpriteDesc dMNTitleCommonSpriteDescs[/* */] =
{
#if defined(REGION_US)
	{ { 157,  94 }, &llMNTitleCutoutSprite },
	{ { 161,  88 }, &llMNTitleSmashSprite },
	{ {  55,  96 }, &llMNTitleSuperSprite },
	{ { 268,  96 }, &llMNTitleBrosSprite },
	{ { 270, 132 }, &llMNTitleTMUnkSprite },
	{ { 160, 208 }, &llMNTitleCopyrightSprite },
#else
	{ { 136,  68 }, 0x10580 },
	{ { 172,  71 }, 0x10CA8 },
	{ { 202,  73 }, 0x113C8 },
	{ {  83, 127 }, 0x12780 },
	{ { 112, 135 }, 0x12EA8 },
	{ { 130, 150 }, 0x13178 },
	{ { 144, 121 }, 0x137C8 },
	{ { 158, 136 }, 0x139A8 },
	{ { 175, 127 }, 0x14308 },
	{ { 191, 145 }, 0x14728 },
	{ { 210, 128 }, 0x14F08 },
	{ { 224, 132 }, 0x15008 },
	{ { 243, 120 }, 0x16468 },
	{ { 160, 208 }, 0x19CE0 },
#endif
	{ { 160,  15 }, &llMNTitleBorderUpperSprite },
	{ { 162, 177 }, &llMNTitlePressStartSprite },
	{ { 260,  60 }, &llMNTitleLogoAnimFullSprite },
	{ { 277, 157 }, &llMNTitleTMSprite }
};

// 0x801342E0
#if defined(REGION_US)
char *dMNTitleUnknown0x801342E0[/* */] = { NULL, NULL, "English", NULL, NULL, NULL, NULL, NULL, NULL, dSCManagerBuildDate, NULL, NULL, NULL, NULL };
#else
char *dMNTitleUnknown0x801342E0[/* */] = { NULL, NULL, dSCManagerBuildDate, NULL, NULL, NULL, NULL };
#endif

// 0x80134318
u8 dMNTitleFireColorsR[/* */] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xE6, 0xFF, 0xFF };

// 0x80134320
u8 dMNTitleFireColorsG[/* */] = { 0xFF, 0xF0, 0xFF, 0xD1, 0xFF, 0xE2, 0xD2 };

// 0x80134328
u8 dMNTitleFireColorsB[/* */] = { 0xFF, 0x9B, 0x64, 0xD1, 0xE6, 0xB8, 0x94 };

// 0x80134330
Lights1 dMNTitleLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x0A, 0x32, 0x32);

// 0x80134348
Gfx dMNTitleDisplayList[/* */] =
{
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPSetLights1(dMNTitleLights1),
	gsSPEndDisplayList()
};

// 0x80134370
SYVideoSetup dMNTitleVideoSetup =
{
	&gSYFramebufferSets[0],
	&gSYFramebufferSets[1],
	&gSYFramebufferSets[2],
	NULL,
	320,
	240,
	SYVIDEO_FLAG_DIVOT        | SYVIDEO_FLAG_DITHERFILTER | SYVIDEO_FLAG_GAMMADITHER |
	0x800                     | SYVIDEO_FLAG_NOBLACKOUT   | SYVIDEO_FLAG_NOGAMMA     |
    SYVIDEO_FLAG_COLORDEPTH16 | SYVIDEO_FLAG_NOSERRATE    | 0x1
};

// 0x8013438C
SYTaskmanSetup dMNTitleTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        mnTitleFuncUpdate,          // Update function
        gcDrawAll,                  // Frame draw function
        &ovl10_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 3584,         // Display List Buffer 0 Size
        sizeof(Gfx) * 1280,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x1000,                     // Graphics Heap Size
        2,                          // ???
        0x1000,                     // RDP Output Buffer Size
        mnTitleFuncLights,         	// Pre-render function
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
    
    mnTitleFuncStart               	// Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80134440
s32 D_ovl10_80134440[2];

// 0x80134448
GObj *sMNTitleFireCameraGObj;

// 0x8013444C
s32 sMNTitleParticleBankID;

// 0x80134450
s32 sMNTitleLayout;

// 0x80134454
GObj *sMNTitleTransitionsGObj;

// 0x80134458
GObj *sMNTitleMainGObj;

// 0x8013445C
s32 sMNTitleTransitionTotalTimeTics;

// 0x80134460
sb32 sMNTitleIsStartActorProcess;

// 0x80134464
s32 sMNTitleFireAlpha;

// 0x80134468
s32 sMNTitleFireAlphaUnused;

// 0x8013446C
s32 sMNTitleLogoAlpha;

// 0x80134470
sb32 sMNTitleIsProceedScene;

// 0x80134474
s32 sMNTitleProceedSceneWait;

// 0x80134478
s32 sMNTitleFireTimer;

// 0x8013447C
f32 sMNTitleFireColorR;

// 0x80134480
f32 sMNTitleFireColorG;

// 0x80134484
f32 sMNTitleFireColorB;

// 0x80134488
f32 sMNTitleFireColorDeltaR;

// 0x8013448C
f32 sMNTitleFireColorDeltaG;

// 0x80134490
f32 sMNTitleFireColorDeltaB;

// 0x80134494
s32 sMNTitleFireColorID;

// 0x80134498
u32 sMNTitleAllowProceedWait;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
s32 mnTitleGetFighterKindsNum(u16 mask)
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

// 0x80131B78
s32 mnTitleGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random)
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

// 0x80131BC4
s32 mnTitleSetDemoFighterKinds(void)
{
	u16 unlocked_mask;
	s32 unlocked_count;
	s32 non_recently_demoed_count;

	unlocked_mask = gSCManagerBackupData.fighter_mask | LBBACKUP_CHARACTER_MASK_STARTER;

	if (~unlocked_mask & gSCManagerSceneData.demo_mask_prev)
	{
		gSCManagerSceneData.demo_mask_prev = 0;
	}
	unlocked_count = mnTitleGetFighterKindsNum(unlocked_mask);

	if (unlocked_count <= mnTitleGetFighterKindsNum(gSCManagerSceneData.demo_mask_prev))
	{
		gSCManagerSceneData.demo_mask_prev = 0;
	}
	unlocked_count = mnTitleGetFighterKindsNum(unlocked_mask);

	gSCManagerSceneData.demo_fkind[0] = mnTitleGetShuffledFighterKind(unlocked_mask, gSCManagerSceneData.demo_mask_prev, syUtilsRandIntRange(unlocked_count - mnTitleGetFighterKindsNum(gSCManagerSceneData.demo_mask_prev)));

	if (!(gSCManagerSceneData.demo_mask_prev))
	{
		gSCManagerSceneData.demo_first_fkind = gSCManagerSceneData.demo_fkind[0];
	}
	gSCManagerSceneData.demo_mask_prev |= LBBACKUP_MASK_FIGHTER(gSCManagerSceneData.demo_fkind[0]);

	unlocked_count = mnTitleGetFighterKindsNum(unlocked_mask);

	non_recently_demoed_count = unlocked_count - mnTitleGetFighterKindsNum(gSCManagerSceneData.demo_mask_prev);

	if (non_recently_demoed_count == 0)
	{
		gSCManagerSceneData.demo_fkind[1] = gSCManagerSceneData.demo_first_fkind;
	}
	else
	{
		gSCManagerSceneData.demo_fkind[1] = mnTitleGetShuffledFighterKind(unlocked_mask, gSCManagerSceneData.demo_mask_prev, syUtilsRandIntRange(non_recently_demoed_count));
		gSCManagerSceneData.demo_mask_prev |= LBBACKUP_MASK_FIGHTER(gSCManagerSceneData.demo_fkind[1]);
	}
}

// 0x80131CF4
void mnTitleInitVars(void)
{
	s32 color_id;

	if (gSCManagerSceneData.scene_prev == nSCKindOpeningNewcomers)
	{
		sMNTitleLayout = nMNTitleLayoutOpening;
		sMNTitleTransitionTotalTimeTics = 0;
		sMNTitleFireColorR = sMNTitleFireColorG = sMNTitleFireColorB = 0.0F;
	}
	else
	{
		syAudioStopBGMAll();
		func_800266A0_272A0();

		sMNTitleLayout = nMNTitleLayoutAnimate;
		sMNTitleTransitionTotalTimeTics = 169;

		color_id = syUtilsRandTimeUCharRange(7);
		sMNTitleFireColorID = color_id;
		sMNTitleFireColorR = dMNTitleFireColorsR[color_id];
		sMNTitleFireColorG = dMNTitleFireColorsG[color_id];
		sMNTitleFireColorB = dMNTitleFireColorsB[color_id];
	}
	sMNTitleFireTimer = 0;
	sMNTitleIsProceedScene = FALSE;
	sMNTitleProceedSceneWait = 3;
	sMNTitleIsStartActorProcess = FALSE;
	sMNTitleFireColorDeltaR = 0.0F;
	sMNTitleFireColorDeltaG = 0.0F;
	sMNTitleFireColorDeltaB = 0.0F;
}

// 0x80131E68
void mnTitleSetEndLogoPosition(void)
{
	GObj *smash_logo_gobj;
	SObj *smash_logo_sobj;

	smash_logo_gobj = gGCCommonLinks[10];

	if (gSCManagerSceneData.scene_prev == nSCKindOpeningNewcomers)
	{
		gcEndProcessAll(smash_logo_gobj);
	}
	smash_logo_sobj = SObjGetStruct(smash_logo_gobj);

	mnTitleSetPosition(NULL, smash_logo_sobj, nMNTitleSpriteKindLogo);

	smash_logo_sobj->user_data.s = 0xFF;
	sMNTitleLogoAlpha = 0x4C;

	smash_logo_sobj->sprite.scalex = 1.0F;
	smash_logo_sobj->sprite.scaley = 1.0F;
}

// 0x80131EE4
void mnTitleSetEndLayout(void)
{
	s32 i;
	GObj *texture_gobj;
	GObj *gobj;
	SObj *sobj;

	gobj = gGCCommonLinks[6];

#ifdef AVOID_UB
	texture_gobj = NULL;
#endif

	while (gobj != NULL)
	{
		if (gobj->id == 5)
		{
			mnTitleShowFire(gobj);
		}
		gobj = gobj->link_next;
	}
	gobj = gGCCommonLinks[8];

	while (gobj != NULL)
	{
		if (gobj->id == 8)
		{
			texture_gobj = gobj;
		}
		gobj = gobj->link_next;
	}

#ifdef AVOID_UB
	if (texture_gobj != NULL)
	{
#endif
		// UB: texture_gobj may not be defined!
		gcEndProcessAll(texture_gobj);

		sobj = SObjGetStruct(texture_gobj);
		texture_gobj->flags = GOBJ_FLAG_NONE;

		for (i = 0; sobj != NULL; i++, sobj = sobj->next)
		{
			mnTitleSetPosition(NULL, sobj, i);
			mnTitleSetColors(sobj, i);

			sobj->sprite.scalex = sobj->sprite.scaley = 1.0F;
		}
#ifdef AVOID_UB
	}
#endif
}

// 0x80131FD0
void mnTitleProceedDemoNext(void)
{
	u8 scene_prev = gSCManagerSceneData.scene_prev;

	gcMakeDefaultCameraGObj(2, GOBJ_PRIORITY_DEFAULT, 0, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
#if defined(REGION_JP)
	gcEjectGObj(gGCCommonLinks[13]);
#endif
	mnTitleSetDemoFighterKinds();
	func_800266A0_272A0();

	gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;

	switch (scene_prev)
	{
	case nSCKindExplain:
		gSCManagerSceneData.scene_curr = nSCKindCharacters;
		syAudioPlayBGM(0, nSYAudioBGMExplain);
		break;

	case nSCKindModeSelect:
	case nSCKindAutoDemo:
#if defined(REGION_US)        
        gSCManagerSceneData.scene_curr = nSCKindStartup;
#else
        gSCManagerSceneData.scene_curr = nSCKindOpeningRoom;
#endif
		break;

	default:
		gSCManagerSceneData.scene_curr = nSCKindExplain;
		break;
	}
	gSCManagerSceneData.is_extend_demo_wait = TRUE;
	sMNTitleIsProceedScene = TRUE;
}

// 0x80132090
void mnTitleProceedModeSelect(void)
{
	gcMakeDefaultCameraGObj(2, GOBJ_PRIORITY_DEFAULT, 0, COBJ_FLAG_FILLCOLOR, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
#if defined(REGION_JP)
	gcEjectGObj(gGCCommonLinks[13]);
#endif

	gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
	gSCManagerSceneData.scene_curr = nSCKindModeSelect;

	func_800266A0_272A0();
	func_800269C0_275C0(nSYAudioFGMTitlePressStart);

	sMNTitleIsProceedScene = TRUE;
}

// 0x801320F0
void mnTitleFuncRun(GObj *gobj)
{
	s32 i;
	u16 buttons;

	if (sMNTitleIsStartActorProcess == FALSE)
	{
		sMNTitleIsStartActorProcess++;
	}
	else if (sMNTitleIsProceedScene != FALSE)
	{
		sMNTitleProceedSceneWait--;

		if (sMNTitleProceedSceneWait == 0)
		{
			syTaskmanSetLoadScene();
		}
	}
	else for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
	{
		buttons = gSYControllerDevices[i].button_tap;

		if (gSYControllerDevices[i].button_tap & (A_BUTTON | B_BUTTON | START_BUTTON))
		{
			if (sMNTitleLayout != 0)
			{
				if ((gSCManagerSceneData.is_title_anim_viewed) || (osResetType != 0))
				{
					if (!(buttons & B_BUTTON))
					{
						mnTitleProceedModeSelect();
						break;
					}
				}
			}
			else
			{
				sMNTitleTransitionTotalTimeTics = 169;
				sMNTitleLayout = nMNTitleLayoutAnimate;

				mnTitleTransitionFromFireLogo();
				syAudioStopBGMAll();
				func_800266A0_272A0();
				break;
			}
		}
	}
}

// 0x80132210
void mnTitleUpdateFireVars(void)
{
	s32 kind = syUtilsRandTimeUCharRange(7);

	sMNTitleFireColorID = kind;

	sMNTitleFireColorR = sMNTitleFireColorG = sMNTitleFireColorB = 0.0F;

	sMNTitleFireColorDeltaR = (dMNTitleFireColorsR[kind] - sMNTitleFireColorR) / 80.0F;
	sMNTitleFireColorDeltaG = (dMNTitleFireColorsG[kind] - sMNTitleFireColorG) / 80.0F;
	sMNTitleFireColorDeltaB = (dMNTitleFireColorsB[kind] - sMNTitleFireColorB) / 80.0F;
}

// 0x80132320
void mnTitleTransitionFromFireLogo(void)
{
	GObj *current_gobj;
	GObj *next_gobj;

	current_gobj = gGCCommonLinks[7];

	while (current_gobj != NULL)
	{
		next_gobj = current_gobj->link_next;

		if (current_gobj->id == 6)
		{
			current_gobj->flags = GOBJ_FLAG_HIDDEN;
		}
		current_gobj = next_gobj;
	}
	if (gGCCommonLinks[4] != NULL)
	{
		gcEjectGObj(gGCCommonLinks[4]);
	}
	if (gGCCommonLinks[14] != NULL)
	{
		gcEjectGObj(gGCCommonLinks[14]);
	}
	mnTitleShowGObjLinkID(6);
	mnTitleUpdateFireVars();
}

// 0x801323AC
void mnTitleShowGObjLinkID(s32 link_id)
{
	GObj *gobj = gGCCommonLinks[link_id];

	while (gobj != NULL)
	{
		gobj->flags = GOBJ_FLAG_NONE;

		gobj = gobj->link_next;
	}
}

// 0x801323DC
void mnTitleAdvanceLayout(void)
{
	if ((sMNTitleLayout == nMNTitleLayoutOpening) && (gSCManagerSceneData.scene_prev == nSCKindOpeningNewcomers))
	{
		gSCManagerSceneData.is_extend_demo_wait = FALSE;
	}
	sMNTitleLayout++;
}

// 0x80132414
void mnTitleSetAllowProceedWait(void)
{
	if (sMNTitleLayout == nMNTitleLayoutFinal)
	{
#if defined(REGION_US)
		sMNTitleAllowProceedWait = 280;
	}
	else sMNTitleAllowProceedWait = 364;
#else
		sMNTitleAllowProceedWait = 230;
	}
	else sMNTitleAllowProceedWait = 294;
#endif
}

// 0x80132448
void mnTitleTransitionsFuncRun(GObj *gobj)
{
	sMNTitleTransitionTotalTimeTics++;

	if (sMNTitleTransitionTotalTimeTics == sMNTitleAllowProceedWait)
	{
		gSCManagerSceneData.is_title_anim_viewed = TRUE;
	}
	switch (sMNTitleTransitionTotalTimeTics)
	{
	case 111:
		mnTitleTransitionFromFireLogo();
		break;

	case 170:
		mnTitleSetEndLogoPosition();
		mnTitleShowGObjLinkID(8);

#if defined(REGION_JP)
		if (sMNTitleLayout != nMNTitleLayoutOpening || gSCManagerSceneData.scene_prev != nSCKindOpeningNewcomers)
		{
			mnTitleShowGObjLinkID(13);
		}
#endif
		mnTitleAdvanceLayout();
		mnTitleSetAllowProceedWait();
		break;

#if defined(REGION_US)
	case 220:
#else
	case 230:
#endif
#if defined(REGION_JP)
		mnTitleShowGObjLinkID(13);
#endif
		mnTitleSetEndLayout();
		break;

	case 214:
#if defined(REGION_JP)
		if (sMNTitleLayout == nMNTitleLayoutFinal)
			mnTitleShowGObjLinkID(11);
			break;
#endif
	case 240:
#if defined(REGION_JP)
		if (sMNTitleLayout == nMNTitleLayoutAnimate)
			mnTitleShowGObjLinkID(11);
#endif
		break;

#if defined(REGION_US)
	case 280:
#else
	case 270:
#endif
		mnTitleShowGObjLinkID(9);
		break;

	case 35:
	case 65:
		func_800269C0_275C0(nSYAudioFGMOpeningBatM);
		break;

	case 200:
		if (sMNTitleLayout == nMNTitleLayoutAnimate)
		{
			func_800269C0_275C0(nSYAudioFGMPublicPrologue);
		}
		break;

	case 650:
		if (gSCManagerSceneData.is_extend_demo_wait == FALSE)
		{
			mnTitleProceedDemoNext();
		}
		break;

	case 1190:
		if (gSCManagerSceneData.is_extend_demo_wait != FALSE)
		{
			mnTitleProceedDemoNext();
		}
		break;
	}
}

// 0x801325D4
void mnTitlePlayAnim(GObj *gobj)
{
	GObj *effect_gobj = gobj->user_data.p;
	SObj *sobj = SObjGetStruct(gobj);
	DObj *dobj = DObjGetStruct(effect_gobj);

	gcPlayAnimAll(effect_gobj);

	dobj = dobj->child;

	while (sobj != NULL)
	{
		sobj->sprite.scalex = dobj->scale.vec.f.x;
		sobj->sprite.scaley = dobj->scale.vec.f.y;
		sobj->pos.x = ((dobj->translate.vec.f.x + 160.0F) - (sobj->sprite.width * sobj->sprite.scalex * 0.5F));
		sobj->pos.y = ((120.0F - dobj->translate.vec.f.y) - (sobj->sprite.height * sobj->sprite.scaley * 0.5F));

		sobj = sobj->next;
		dobj = dobj->sib_next;
	}
}

// 0x801326A4
void mnTitlePressStartProcUpdate(GObj *gobj)
{
	if (gobj->flags != GOBJ_FLAG_HIDDEN)
	{
		mnTitlePlayAnim(gobj);
	}
}

#if defined(REGION_US)
// 0x801326D4
void mnTitleProcUpdate(GObj *gobj)
{
	if (gobj->flags != GOBJ_FLAG_HIDDEN)
	{
		mnTitlePlayAnim(gobj);
	}
}
#endif

// 0x80132704
void mnTitleUpdateLabelsPosition(GObj *gobj)
{
	SObj *sobj = SObjGetStruct(gobj);

	if ((sMNTitleLayout != nMNTitleLayoutOpening) || (gSCManagerSceneData.scene_prev != nSCKindOpeningNewcomers))
	{
		mnTitleSetPosition(NULL, sobj, nMNTitleSpriteKindFooter);
		mnTitleSetPosition(NULL, sobj->next, nMNTitleSpriteKindHeader);
	}
}

// 0x80132764
void mnTitleSetPosition(DObj *dobj, SObj *sobj, s32 kind)
{
	MNTitleSpriteDesc *desc;

	if (dobj != NULL)
	{
		desc = &dMNTitleCommonSpriteDescs[kind];
		dobj->translate.vec.f.x = desc->pos.x - 160.0F;
		dobj->translate.vec.f.y = -(desc->pos.y - 120.0F);
	}
	desc = &dMNTitleCommonSpriteDescs[kind];
	sobj->pos.x = (desc->pos.x - (sobj->sprite.width * 0.5F));
	sobj->pos.y = (desc->pos.y - (sobj->sprite.height * 0.5F));
}

// 0x8013282C
void mnTitleSetColors(SObj *sobj, s32 kind)
{
	if (kind < nMNTitleSpriteKindFooter)
	{
#if defined(REGION_US)
		if ((kind == nMNTitleSpriteKindDropShadow) || (kind == nMNTitleSpriteKindTM))
		{
			sobj->sprite.red = 0x00;
			sobj->sprite.green = 0x00;
			sobj->sprite.blue = 0x00;
		}
		else
		{
			sobj->sprite.red = 0xFF;
			sobj->sprite.green = 0xFE;
			sobj->sprite.blue = 0x2A;
			sobj->envcolor.r = 0x00;
			sobj->envcolor.g = 0x00;
			sobj->envcolor.b = 0x00;
		}
#else
		sobj->sprite.red = 0x15;
		sobj->sprite.green = 0x13;
		sobj->sprite.blue = 0x06;
		sobj->envcolor.r = 0x36;
		sobj->envcolor.g = 0x33;
		sobj->envcolor.b = 0x21;
#endif
	}
	else switch (kind)
	{
	case nMNTitleSpriteKindFooter:
		sobj->sprite.red = 0xB7;
		sobj->sprite.green = 0xAE;
		sobj->sprite.blue = 0x7C;
		sobj->envcolor.r = 0x14;
		sobj->envcolor.g = 0x12;
		sobj->envcolor.b = 0x06;
		break;
			
	case nMNTitleSpriteKindHeader:
		sobj->sprite.red = 0x14;
		sobj->sprite.green = 0x12;
		sobj->sprite.blue = 0x06;
		break;

	case nMNTitleSpriteKindPressStart:
		sobj->sprite.red = 0xFF;
		sobj->sprite.green = 0xFF;
		sobj->sprite.blue = 0xFF;
		sobj->envcolor.r = 0x17;
		sobj->envcolor.g = 0x10;
		sobj->envcolor.b = 0xA4;
		break;

	case nMNTitleSpriteKindTM2:
		sobj->sprite.red = 0x15;
		sobj->sprite.green = 0x13;
		sobj->sprite.blue = 0x06;
		break;
	}
}

// 0x80132940
void mnTitleFireProcDisplay(GObj *fire_gobj)
{
	s32 i;
	SObj *fire_sobj = SObjGetStruct(fire_gobj);

	for (i = 0; i < 2; i++)
	{
		lbCommonPrepSObjAttr(gSYTaskmanDLHeads, fire_sobj);

		gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0x00, sMNTitleFireAlpha);
		gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, TEXEL0,  TEXEL0, 0, PRIMITIVE, 0,  0, 0, 0, TEXEL0,  TEXEL0, 0, PRIMITIVE, 0);

		lbCommonPrepSObjDraw(gSYTaskmanDLHeads, fire_sobj);
		lbCommonClearExternSpriteParams();

		fire_sobj = fire_sobj->next;
	}
}

// 0x80132A20
void mnTitleFireFuncRun(GObj *gobj)
{
	if (gobj->flags != GOBJ_FLAG_HIDDEN)
	{
		sMNTitleFireAlpha += 0x0D;

		if (sMNTitleFireAlpha > 0xFF)
		{
			sMNTitleFireAlpha = 0xFF;
		}
	}
}

// 0x80132A58
void mnTitleShowFire(GObj *gobj)
{
	sMNTitleFireAlpha = 0xFF;

	gobj->flags = GOBJ_FLAG_NONE;
}

// 0x80132A6C
void mnTitleUpdateFireSprite(SObj *sobj, sb32 is_next)
{
	Sprite *sprite = lbRelocGetFileData(Sprite*, sMNTitleFiles[1], dMNTitleFireSpriteOffsets[sobj->user_data.s]);

	sobj->sprite = *sprite;
	sobj->sprite.attr = SP_TRANSPARENT;

	sobj->sprite.scalex = (is_next != FALSE) ? 9.5F : 12.0F;
	sobj->sprite.scaley = (is_next != FALSE) ? 7.0F :  8.5F;

	sobj->user_data.s++;

	if (sobj->user_data.s >= 30)
	{
		sobj->user_data.s = 0;
	}
}

// 0x80132B38
void mnTitleFireProcUpdate(GObj *gobj)
{
	SObj *base_sobj = SObjGetStruct(gobj), *next_sobj = base_sobj->next;

	mnTitleUpdateFireSprite(base_sobj, FALSE);
	mnTitleUpdateFireSprite(next_sobj, TRUE);
}

// 0x80132B70
void mnTitleMakeFire(void)
{
	s32 i;
	s32 target_texture;
	GObj *fire_gobj;
	SObj *fire_sobj;

	fire_gobj = gcMakeGObjSPAfter(5, mnTitleFireFuncRun, 6, GOBJ_PRIORITY_DEFAULT);

	if (fire_gobj != NULL)
	{
		gcAddGObjDisplay(fire_gobj, mnTitleFireProcDisplay, 0, GOBJ_PRIORITY_DEFAULT, ~0);
		gcAddGObjProcess(fire_gobj, mnTitleFireProcUpdate, nGCProcessKindFunc, 1);

		for (i = 0; i < 2; i++)
		{
			if (i != 0)
			{
				target_texture = 0;
			}
			else target_texture = 12;

			fire_sobj = lbCommonMakeSObjForGObj(fire_gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[1], dMNTitleFireSpriteOffsets[target_texture]));

			fire_sobj->sprite.attr = SP_TRANSPARENT;

			if (i != 0)
			{
				fire_sobj->pos.x = 8.0F;
			}
			else fire_sobj->pos.x = -32.0F;

			if (i != 0)
			{
				fire_sobj->pos.y = 8.0F;
			}
			else fire_sobj->pos.y = -16.0F;

			if (i != 0)
			{
				fire_sobj->sprite.scalex = 9.5F;
			}
			else fire_sobj->sprite.scalex = 12.0F;

			if (i != 0)
			{
				fire_sobj->sprite.scaley = 7.0F;
			}
			else fire_sobj->sprite.scaley = 8.5F;

			fire_sobj->user_data.s = target_texture;
		}
		sMNTitleFireAlpha = sMNTitleFireAlphaUnused = 0;

		fire_gobj->flags = GOBJ_FLAG_HIDDEN;

		if (gSCManagerSceneData.scene_prev != nSCKindOpeningNewcomers)
		{
			mnTitleShowFire(fire_gobj);
		}
	}
}

// 0x80132D5C
void mnTitleLogoProcUpdate(GObj *gobj)
{
	GObj *effect_gobj = gobj->user_data.p;
	SObj *logo_sobj = SObjGetStruct(gobj);
	DObj *logo_dobj = DObjGetStruct(effect_gobj)->child->sib_next->sib_next->sib_next;

	logo_sobj->sprite.scalex = logo_dobj->scale.vec.f.x;
	logo_sobj->sprite.scaley = logo_dobj->scale.vec.f.y;
	logo_sobj->pos.x = ((logo_dobj->translate.vec.f.x + 160.0F) - (logo_sobj->sprite.width * logo_sobj->sprite.scalex * 0.5F));
	logo_sobj->pos.y = ((120.0F - logo_dobj->translate.vec.f.y) - (logo_sobj->sprite.height * logo_sobj->sprite.scaley * 0.5F));
}

// 0x80132DFC
void mnTitleLogoProcDisplay(GObj *gobj)
{
	SObj *sobj = SObjGetStruct(gobj);

	if ((sobj->sprite.scalex < 0.0001F) || (sobj->sprite.scaley < 0.0001F))
	{
		return;
	}
	else
	{
		lbCommonPrepSObjAttr(gSYTaskmanDLHeads, sobj);

		gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, sobj->sprite.red, sobj->sprite.green, sobj->sprite.blue, sMNTitleLogoAlpha);
		gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  TEXEL0, 0, PRIMITIVE, 0,  0, 0, 0, PRIMITIVE,  TEXEL0, 0, PRIMITIVE, 0);

		lbCommonDrawSObjNoAttr(gobj);
	}
}

// 0x80132EDC
void mnTitleFadeOutLogoFuncRun(GObj *gobj)
{
	SObj *sobj = SObjGetStruct(gobj);

	if ((sobj->sprite.scalex < 0.0001F) || (sobj->sprite.scaley < 0.0001F))
	{
		return;
	}
	else
	{
		sMNTitleLogoAlpha -= 0x04;

		if (sMNTitleLogoAlpha <= 0x4C)
		{
			sMNTitleLogoAlpha = 0x4C;
		}
	}
}

// 0x80132F3C
void mnTitleMakeLogoNoOpening(void)
{
	GObj *gobj = lbCommonMakeSpriteGObj
	(
		11,
		NULL,
		10,
		GOBJ_PRIORITY_DEFAULT,
		mnTitleLogoProcDisplay,
		0,
		GOBJ_PRIORITY_DEFAULT,
		-1,
		lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleCommonSpriteDescs[nMNTitleSpriteKindLogo].offset),
		nGCProcessKindFunc,
		NULL,
		1
	);
	SObj *sobj = SObjGetStruct(gobj);

	sobj->sprite.attr = SP_TRANSPARENT;

	sobj->sprite.red = 0xFF;
	sobj->sprite.green = 0x00;
	sobj->sprite.blue = 0x00;

	mnTitleSetPosition(NULL, sobj, nMNTitleSpriteKindLogo);
}

// 0x80132FD8
void mnTitleMakeLogo(void)
{
	s32 i;
	GObj *animated_logo_gobj;
	GObj *fire_logo_gobj;
	SObj *fire_logo_sobj;
	GObj *logo_gobj;
	SObj *logo_sobj;
	DObj *fire_logo_dobj;

	if (gSCManagerSceneData.scene_prev != nSCKindOpeningNewcomers)
	{
		mnTitleMakeLogoNoOpening();
	}
	else
	{
		animated_logo_gobj = gcMakeGObjSPAfter(7, NULL, 7, GOBJ_PRIORITY_DEFAULT);
		gcSetupCommonDObjs(animated_logo_gobj, lbRelocGetFileData(DObjDesc*, sMNTitleFiles[0], &llMNTitleLogoDObjDesc), 0);
		gcAddAnimJointAll(animated_logo_gobj, lbRelocGetFileData(AObjEvent32**, sMNTitleFiles[0], &llMNTitleLogoAnimJoint), 0.0F);
		gcPlayAnimAll(animated_logo_gobj);

		fire_logo_gobj = gcMakeGObjSPAfter(6, NULL, 7, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(fire_logo_gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);
		gcAddGObjProcess(fire_logo_gobj, mnTitlePlayAnim, nGCProcessKindFunc, 1);

		fire_logo_gobj->user_data.p = animated_logo_gobj;
		fire_logo_dobj = DObjGetStruct(animated_logo_gobj)->child;

		for (i = 0; i < ARRAY_COUNT(dMNTitleLogoAnimSprites) - 1; i++)
		{
			fire_logo_sobj = lbCommonMakeSObjForGObj(fire_logo_gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleLogoAnimSprites[i]));

			fire_logo_sobj->sprite.attr = SP_TRANSPARENT;

			fire_logo_sobj->pos.x = 0;
			fire_logo_sobj->pos.y = 0;

			fire_logo_sobj->sprite.red = 0x00;
			fire_logo_sobj->sprite.green = 0x00;
			fire_logo_sobj->sprite.blue = 0x00;

			fire_logo_dobj->translate.vec.f.x = 0.0F;
			fire_logo_dobj->translate.vec.f.y = 0.0F;

			fire_logo_dobj = fire_logo_dobj->sib_next;
		};

		logo_gobj = lbCommonMakeSpriteGObj
		(
			11,
			mnTitleFadeOutLogoFuncRun,
			10,
			GOBJ_PRIORITY_DEFAULT,
			mnTitleLogoProcDisplay,
			0,
			GOBJ_PRIORITY_DEFAULT,
			-1,
			lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleLogoAnimSprites[3]),
			nGCProcessKindFunc,
			mnTitleLogoProcUpdate,
			1
		);
		logo_sobj = SObjGetStruct(logo_gobj);

		logo_sobj->sprite.attr = SP_TRANSPARENT;

		logo_sobj->sprite.red = 0xFF;
		logo_sobj->sprite.green = 0x00;
		logo_sobj->sprite.blue = 0x00;

		fire_logo_dobj->translate.vec.f.x = 0.0F;
		fire_logo_dobj->translate.vec.f.y = 0.0F;

		logo_gobj->user_data.p = animated_logo_gobj;

		sMNTitleLogoAlpha = 0xFF;
	}
}

// 0x801331FC - Unused?
void mnTitleMakeSprites(void)
{
	GObj *gobj;
	SObj *sobj;
	s32 i;

	gobj = gcMakeGObjSPAfter(8, NULL, 8, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; i < nMNTitleSpriteKindPressStart; i++)
	{
		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleCommonSpriteDescs[i].offset));
		sobj->sprite.attr = SP_TRANSPARENT;

		mnTitleSetPosition(NULL, sobj, i);
		mnTitleSetColors(sobj, i);
	}
}

// 0x801332E4
void mnTitleMakeLabels(void)
{
	s32 i;
	GObj *animation_gobj;
	GObj *gobj;
	SObj *texture_sobj;
	DObj *animation_dobj;

	animation_gobj = gcMakeGObjSPAfter(10, NULL, 8, GOBJ_PRIORITY_DEFAULT);
	gcSetupCommonDObjs(animation_gobj, lbRelocGetFileData(DObjDesc*, sMNTitleFiles[0], &llMNTitleLabelsDObjDesc), 0);
	gcAddAnimJointAll(animation_gobj, lbRelocGetFileData(AObjEvent32**, sMNTitleFiles[0], &llMNTitleLabelsAnimJoint), 0.0F);
	gcPlayAnimAll(animation_gobj);

	gobj = gcMakeGObjSPAfter(8, NULL, 8, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
#if defined(REGION_US)
	gcAddGObjProcess(gobj, mnTitleProcUpdate, nGCProcessKindFunc, 1);
#else
	gcAddGObjProcess(gobj, mnTitlePressStartProcUpdate, nGCProcessKindFunc, 1);
#endif
	gobj->user_data.p = animation_gobj;
	animation_dobj = DObjGetStruct(animation_gobj)->child;

	for (i = 0; i < nMNTitleSpriteKindFooter; i++)
	{
		texture_sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleCommonSpriteDescs[i].offset));
		texture_sobj->sprite.attr = SP_TRANSPARENT;

		mnTitleSetPosition(animation_dobj, texture_sobj, i);
		mnTitleSetColors(texture_sobj, i);

		animation_dobj = animation_dobj->sib_next;
	}
	gobj->flags = GOBJ_FLAG_HIDDEN;

	gobj = gcMakeGObjSPAfter(9, NULL, 8, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnTitleUpdateLabelsPosition, nGCProcessKindFunc, 1);

	gobj->user_data.p = animation_gobj;

	for (i = nMNTitleSpriteKindFooter; i < nMNTitleSpriteKindPressStart; i++)
	{
		texture_sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleCommonSpriteDescs[i].offset));
		texture_sobj->sprite.attr = SP_TRANSPARENT;

		mnTitleSetPosition(animation_dobj, texture_sobj, i);
		mnTitleSetColors(texture_sobj, i);

		animation_dobj = animation_dobj->sib_next;
	}
	gobj->flags = GOBJ_FLAG_HIDDEN;
}

// 0x80133504
void mnTitleMakePressStart(void)
{
	GObj *press_start_anim_gobj;
	GObj *press_start_gobj;
	DObj *press_start_anim_dobj;
	SObj *press_start_sobj;

	press_start_anim_gobj = gcMakeGObjSPAfter(10, NULL, 8, GOBJ_PRIORITY_DEFAULT);
	gcSetupCommonDObjs(press_start_anim_gobj, lbRelocGetFileData(DObjDesc*, sMNTitleFiles[0], &llMNTitlePressStartDObjDesc), 0);
	gcAddAnimJointAll(press_start_anim_gobj, lbRelocGetFileData(AObjEvent32**, sMNTitleFiles[0], &llMNTitlePressStartAnimJoint), 0.0F);
	gcPlayAnimAll(press_start_anim_gobj);

	press_start_anim_dobj = DObjGetStruct(press_start_anim_gobj)->child;

	press_start_gobj = gcMakeGObjSPAfter(8, NULL, 9, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(press_start_gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(press_start_gobj, mnTitlePressStartProcUpdate, nGCProcessKindFunc, 1);

	press_start_gobj->user_data.p = press_start_anim_gobj;

	press_start_sobj = lbCommonMakeSObjForGObj(press_start_gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleCommonSpriteDescs[nMNTitleSpriteKindPressStart].offset));
	press_start_sobj->sprite.attr = SP_TRANSPARENT;

	mnTitleSetPosition(press_start_anim_dobj, press_start_sobj, nMNTitleSpriteKindPressStart);
	mnTitleSetColors(press_start_sobj, nMNTitleSpriteKindPressStart);

	press_start_gobj->flags = GOBJ_FLAG_HIDDEN;
}

// 0x80133634 - discarded language selection?
void func_ovl10_80133634(void)
{
#if defined(REGION_US)
	return;
#else
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(8, NULL, 11, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNTitleFiles[0], dMNTitleCommonSpriteDescs[17].offset));
    sobj->sprite.attr = SP_TRANSPARENT;
    mnTitleSetPosition(NULL, sobj, 17);
    mnTitleSetColors(sobj, 17);
    gobj->flags = GOBJ_FLAG_HIDDEN;
#endif
}

// 0x8013363C - Unused?
void mnTitleSlashProcUpdate(GObj *gobj)
{
	if (gobj->flags != GOBJ_FLAG_HIDDEN)
	{
		gcPlayAnimAll(gobj);
	}
}

// 0x8013366C
void mnTitleMakeSlash(void)
{
	GObj *gobj;

	if (gSCManagerSceneData.scene_prev == nSCKindOpeningNewcomers)
	{
		gobj = gcMakeGObjSPAfter(12, NULL, 14, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(gobj, gcDrawDObjTreeDLLinksForGObj, 2, GOBJ_PRIORITY_DEFAULT, ~0);
		gcSetupCustomDObjsWithMObj
		(
			gobj,
			lbRelocGetFileData(DObjDesc*, sMNTitleFiles[0], &llMNTitleSlashDObjDesc),
			lbRelocGetFileData(MObjSub***, sMNTitleFiles[0], &llMNTitleSlashMObjSub),
			NULL,
			nGCMatrixKindTraRotRpyRSca,
			nGCMatrixKindNull,
			nGCMatrixKindNull
		);
		gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMNTitleFiles[0], &llMNTitleSlashAnimJoint), 0.0F);
		gcAddMatAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32***, sMNTitleFiles[0], &llMNTitleSlashMatAnimJoint), 0.0F);
		gcPlayAnimAll(gobj);
		gcAddGObjProcess(gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);
	}

#if defined(REGION_JP)
	gobj = gcMakeGObjSPAfter(12, NULL, 13, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, gcDrawDObjTreeDLLinksForGObj, 2, GOBJ_PRIORITY_DEFAULT, ~0);
	gcSetupCustomDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMNTitleFiles[0], &llMNTitleUnknownDObjDesc), NULL, nGCMatrixKindTraRotRpyRSca, nGCMatrixKindNull, nGCMatrixKindNull);
	gcAddAnimJointAll(gobj, lbRelocGetFileData(AObjEvent32**, sMNTitleFiles[0], &llMNTitleUnknownAnimJoint), 0.0F);
	gcPlayAnimAll(gobj);
	gcAddGObjProcess(gobj, mnTitleSlashProcUpdate, nGCProcessKindFunc, 1);
	gobj->flags = GOBJ_FLAG_HIDDEN;
#endif
}

// 0x80133770
void mnTitleFireCameraProcUpdate(GObj *gobj)
{
	CObj *cobj = CObjGetStruct(sMNTitleFireCameraGObj);

	if (sMNTitleTransitionTotalTimeTics >= 40)
	{
		if (sMNTitleTransitionTotalTimeTics <= 110)
		{
			sMNTitleFireColorR += 4.0F;

			TAKE_MIN(sMNTitleFireColorR, 255);
		}
		else
		{
			if (sMNTitleFireTimer == 0)
			{
				s32 color_id;

				sMNTitleFireTimer = 260;

				color_id = syUtilsRandTimeUCharRange(7);

				if (color_id == sMNTitleFireColorID)
				{
					color_id++;

					if (color_id >= 7)
					{
						color_id = 0;
					}
				}
				sMNTitleFireColorID = color_id;

				sMNTitleFireColorDeltaR = (dMNTitleFireColorsR[color_id] - sMNTitleFireColorR) / 80.0F;
				sMNTitleFireColorDeltaG = (dMNTitleFireColorsG[color_id] - sMNTitleFireColorG) / 80.0F;
				sMNTitleFireColorDeltaB = (dMNTitleFireColorsB[color_id] - sMNTitleFireColorB) / 80.0F;
			}
			if (sMNTitleFireTimer >= 80)
			{
				sMNTitleFireColorR += sMNTitleFireColorDeltaR;
				sMNTitleFireColorG += sMNTitleFireColorDeltaG;
				sMNTitleFireColorB += sMNTitleFireColorDeltaB;
			}
			sMNTitleFireTimer--;
		}
		TAKE_MIN(sMNTitleFireColorR, 255.0F);
		TAKE_MIN(sMNTitleFireColorG, 255.0F);
		TAKE_MIN(sMNTitleFireColorB, 255.0F);

		TAKE_MAX(sMNTitleFireColorR, 0.0F);
		TAKE_MAX(sMNTitleFireColorG, 0.0F);
		TAKE_MAX(sMNTitleFireColorB, 0.0F);

		cobj->color = GPACK_RGBA8888((s32) sMNTitleFireColorR, (s32) sMNTitleFireColorG, (s32) sMNTitleFireColorB, 0xFF);
	}
}

// 0x80133A94
s32 mnTitleMakeCameras(void)
{
	GObj *camera_gobj;
	s32 unused;
	CObj *cobj;

	sMNTitleFireCameraGObj = gcMakeDefaultCameraGObj(2, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	gcAddGObjProcess(sMNTitleFireCameraGObj, mnTitleFireCameraProcUpdate, nGCProcessKindFunc, 1);

	camera_gobj = gcMakeCameraGObj
	(
		2,
		NULL,
		3,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSprite,
		60,
		COBJ_MASK_DLLINK(1) | COBJ_MASK_DLLINK(0),
		~0,
		FALSE,
		nGCProcessKindFunc,
		NULL,
		1,
		FALSE
	);
	cobj = CObjGetStruct(camera_gobj);
	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

	camera_gobj = gcMakeCameraGObj(3, NULL, 3, GOBJ_PRIORITY_DEFAULT, func_80017EC0, 40, COBJ_MASK_DLLINK(2), -1, FALSE, nGCProcessKindFunc, NULL, 1, FALSE);
	cobj = CObjGetStruct(camera_gobj);

	gcAddXObjForCamera(cobj, nGCMatrixKindOrtho, 0);
	gcAddXObjForCamera(cobj, nGCMatrixKindLookAt, 0);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
	
	cobj->vec.at.x = cobj->vec.at.y = cobj->vec.at.z = 0.0F;
	
	cobj->vec.eye.x = cobj->vec.eye.y = 0.0F;
	cobj->vec.eye.z = 2000.0F;

	camera_gobj = gcMakeCameraGObj(3, NULL, 3, GOBJ_PRIORITY_DEFAULT, func_80017EC0, 80, COBJ_MASK_DLLINK(3), -1, TRUE, nGCProcessKindFunc, NULL, 1, FALSE);
	cobj = CObjGetStruct(camera_gobj);

	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
	
	cobj->vec.at.x = cobj->vec.at.y = cobj->vec.at.z = 0.0F;
	
	cobj->vec.eye.x = cobj->vec.eye.y = 0.0F;
	cobj->vec.eye.z = 1000.0F;

	cobj->projection.persp.fovy = 30.0F;

#ifdef AVOID_UB
	return 0;
#else
	return unused;
#endif
}

// 0x80133CFC
void mnTitleLogoFireProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

	lbParticleDrawTextures(gobj);

	gDPSetTexturePersp(gSYTaskmanDLHeads[0]++, G_TP_PERSP);
	gDPSetDepthSource(gSYTaskmanDLHeads[0]++, G_ZS_PIXEL);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80133DDC
void mnTitleMakeLogoFire(void)
{
	GObj *gobj = gcMakeGObjSPAfter(15, NULL, 4, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, mnTitleLogoFireProcDisplay, 3, GOBJ_PRIORITY_DEFAULT, ~0);

	gobj->camera_mask = COBJ_MASK_DLLINK(0);

	sMNTitleParticleBankID = efParticleGetLoadBankID(&lMNTitleParticleScriptBankLo, &lMNTitleParticleScriptBankHi, &lMNTitleParticleTextureBankLo, &lMNTitleParticleTextureBankHi);
}

// 0x80133E68
void mnTitleMakeLogoFireParticles(void)
{
	GObj *logo_fire_effect_gobj;
	LBGenerator *gn;

	if (gSCManagerSceneData.scene_prev == nSCKindOpeningNewcomers)
	{
		logo_fire_effect_gobj = gcMakeGObjSPAfter(14, NULL, 5, GOBJ_PRIORITY_DEFAULT);

		gcSetupCommonDObjs(logo_fire_effect_gobj, lbRelocGetFileData(DObjDesc*, sMNTitleFiles[0], &llMNTitleFireDObjDesc), NULL);
		gcAddAnimJointAll(logo_fire_effect_gobj, lbRelocGetFileData(AObjEvent32**, sMNTitleFiles[0], &llMNTitleFireAnimJoint), 0.0F);
		gcPlayAnimAll(logo_fire_effect_gobj);
		gcAddGObjProcess(logo_fire_effect_gobj, gcPlayAnimAll, nGCProcessKindFunc, 1);

		gn = lbParticleMakeGenerator(sMNTitleParticleBankID, 0);

		if (gn != NULL)
		{
			gn->dobj = DObjGetStruct(logo_fire_effect_gobj)->child->sib_next->child;
		}
	}
}

// 0x80133F3C
void mnTitleMakeActors(void)
{
	sMNTitleMainGObj = gcMakeGObjSPAfter(0, mnTitleFuncRun, 1, GOBJ_PRIORITY_DEFAULT);
	sMNTitleTransitionsGObj = gcMakeGObjSPAfter(0, mnTitleTransitionsFuncRun, 15, GOBJ_PRIORITY_DEFAULT);
}

// 0x80133F90
void mnTitleFuncStart(void)
{
	s32 i;

	for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
	{
		syControllerStopRumble(i);
	}
	mnTitleLoadFiles();
	mnTitleMakeActors();
	efParticleInitAll();
	mnTitleMakeLogoFire();
	mnTitleMakeCameras();
	mnTitleInitVars();
	mnTitleMakeFire();
	mnTitleMakeLogo();
	mnTitleMakeLabels();
	func_ovl10_80133634();
	mnTitleMakePressStart();
	mnTitleMakeSlash();
	mnTitleMakeLogoFireParticles();

	if (gSCManagerSceneData.scene_prev == nSCKindOpeningNewcomers)
	{
		while (sySchedulerGetTicCount() < 4215)
		{
			continue;
		};
	}
}

// 0x80134074
void mnTitleFuncLights(Gfx **dls)
{
	gSPDisplayList(dls[0]++, dMNTitleDisplayList);
}

// 0x80134098
void mnTitleFuncUpdate(void)
{
	gcRunAll();
}

// 0x801340B8
void mnTitleStartScene(void)
{
	dMNTitleVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMNTitleVideoSetup);

	if ((!gSCManagerSceneData.is_title_anim_viewed) && (gSCManagerBackupData.boot <= U8_MAX))
	{
		gSCManagerBackupData.boot++;
		lbBackupWrite();
	}
	dMNTitleTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl9_VRAM - (uintptr_t)&ovl10_BSS_END);
	syTaskmanStartTask(&dMNTitleTaskmanSetup);
}
