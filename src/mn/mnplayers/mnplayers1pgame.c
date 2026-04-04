#include <ft/fighter.h>
#include <if/interface.h>
#include <mn/menu.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801385B0
s32 dMNPlayers1PGameUnknown0x801385B0[/* */] =
{
	0x000080C7, 0x914B4001, 0x30017085, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x000031A1, 0x39E52119, 0x2113211D, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00008BC1, 0x9C0341C1, 0x31417B41, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00001AC7, 0x230901C1, 0x09431245, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// 0x80138630
u32 dMNPlayers1PGameFileIDs[/* */] =
{
	&llMNPlayersCommonFileID,
	&llFTEmblemSpritesFileID,
	&llMNSelectCommonFileID,
	&llMNPlayersGameModesFileID, 
	&llMNPlayersPortraitsFileID,
	&llMNPlayers1PModeFileID,
	&llMNPlayersDifficultyFileID,
	&llFTStocksZakoFileID,
	&llMNCommonFontsFileID,
	&llIFCommonDigitsFileID,
	&llMNPlayersSpotlightFileID
};

// 0x80138660
Lights1 dMNPlayers1PGameLights11 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x14, 0x14, 0x14);

// 0x80138678
Lights1 dMNPlayers1PGameLights12 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x00, 0xEC, 0x00);

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80138EE0
s32 sMNPlayers1PGamePad0x80138EE0[2];

// 0x80138EE8
MNPlayersSlot1PGame sMNPlayers1PGameSlot;

// 0x80138F70
GObj *sMNPlayers1PGameTimeGObj;

// 0x80138F74
s32 sMNPlayers1PGameTotalTimeTics;

// 0x80138F78
s32 sMNPlayers1PGameReturnTic;

// 0x80138F7C
s32 sMNPlayers1PGameReadyBlinkWait;

// 0x80138F80
s32 sMNPlayers1PGameTimeSetting;

// 0x80138F80
s32 sMNPlayers1PGamePad0x80138F80[4];

// 0x80138F98
s32 sMNPlayers1PGameStartProceedWait;

// 0x80138F9C
sb32 sMNPlayers1PGameIsStart;

// 0x80138FA0
sb32 sMNPlayers1PGameIsTeamBattle;

// 0x80138FA4
s32 sMNPlayers1PGameRule;

// 0x80138FA8
s32 sMNPlayers1PGameManPlayer;

// 0x80138FAC
GObj *sMNPlayers1PGameHiScoreGObj;

// 0x80138FB0
GObj *sMNPlayers1PGameBonusesGObj;

// 0x80138FB4 level
s32 sMNPlayers1PGameLevelValue;

// 0x80138FB8 stocks
s32 sMNPlayers1PGameStockValue;

// 0x80138FBC
GObj *sMNPlayers1PGameLevelGObj;

// 0x80138FC0
GObj *sMNPlayers1PGameStockGObj;

// 0x80138FC4
void *sMNPlayers1PGameFigatreeHeap;

// 0x80138FC8
u16 sMNPlayers1PGameFighterMask;

// 0x80138FCC
s32 sMNPlayers1PGameFighterKind;

// 0x80138FD0
s32 sMNPlayers1PGameCostume;

// 0x80138FD8
s32 sMNPlayers1PGamePad0x80138FD8[180];

// 0x801392A8
LBFileNode sMNPlayers1PGameForceStatusBuffer[7];

// 0x801392E0
LBFileNode sMNPlayers1PGameStatusBuffer[120];

// 0x801396A0
void *sMNPlayers1PGameFiles[ARRAY_COUNT(dMNPlayers1PGameFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnPlayers1PGameFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);
	ftDisplayLightsDrawReflect(dls, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());
}

// 0x80131B58
s32 mnPlayers1PGameGetPowerOf(s32 base, s32 exp)
{
	s32 raised = base;
	s32 i;

	if (exp == 0)
	{
		return 1;
	}
	i = exp;

	while (i > 1)
	{
		i--;
		raised *= base;
	}
	return raised;
}

// 0x80131BF8
void mnPlayers1PGameSetDigitColors(SObj *sobj, u32 *colors)
{
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->envcolor.r = colors[0];
	sobj->envcolor.g = colors[1];
	sobj->envcolor.b = colors[2];
	sobj->sprite.red = colors[3];
	sobj->sprite.green = colors[4];
	sobj->sprite.blue = colors[5];
}

// 0x80131C40
s32 mnPlayers1PGameGetNumberDigitCount(s32 number, s32 digit_count_max)
{
    s32 digit_count_curr = digit_count_max;

    while (digit_count_curr > 0)
    {
        s32 digit = (mnPlayers1PGameGetPowerOf(10, digit_count_curr - 1) != 0) ? number / mnPlayers1PGameGetPowerOf(10, digit_count_curr - 1) : 0;

        if (digit != 0)
        {
            return digit_count_curr;
        }
        else digit_count_curr--;
    }
    return 0;
}

// 0x80131CEC
void mnPlayers1PGameMakeNumber(GObj *gobj, s32 number, f32 x, f32 y, u32 *colors, s32 digit_count_max, sb32 is_fixed_digit_count)
{
	intptr_t offsets[/* */] =
	{
		&llIFCommonDigits0Sprite, &llIFCommonDigits1Sprite,
		&llIFCommonDigits2Sprite, &llIFCommonDigits3Sprite,
		&llIFCommonDigits4Sprite, &llIFCommonDigits5Sprite,
		&llIFCommonDigits6Sprite, &llIFCommonDigits7Sprite,
		&llIFCommonDigits8Sprite, &llIFCommonDigits9Sprite
	};
	SObj *sobj;
	f32 left_x = x;
	s32 i;
	s32 unused;
	s32 digit;

	if (number < 0)
	{
		number = 0;
	}
	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[9], offsets[number % 10]));
	mnPlayers1PGameSetDigitColors(sobj, colors);
	left_x -= 8;
	sobj->pos.x = left_x;
	sobj->pos.y = y;

	for (i = 1; i < ((is_fixed_digit_count != FALSE) ? digit_count_max : mnPlayers1PGameGetNumberDigitCount(number, digit_count_max)); i++)
	{
		digit = (mnPlayers1PGameGetPowerOf(10, i) != 0) ? number / mnPlayers1PGameGetPowerOf(10, i) : 0;

		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[9], offsets[digit % 10]));
		mnPlayers1PGameSetDigitColors(sobj, colors);
		left_x -= 8;
		sobj->pos.x = left_x;
		sobj->pos.y = y;
	}
}

// 0x80131F5C
s32 mnPlayers1PGameGetCharacterID(const char c)
{
	switch (c)
	{
	case '\'':
		return 0x1A;
	
	case '%':
		return 0x1B;
		
	case '.':
		return 0x1C;
		
	case ' ':
		return 0x1D;
		
	default:
		if ((c < 'A') || (c > 'Z'))
		{
			return 0x1D;
		}
		else return c - 'A';
	}
}

// 0x80131FD4
f32 mnPlayers1PGameGetCharacterSpacing(const char *str, s32 c)
{
	switch (str[c])
	{
	case 'A':
		switch (str[c + 1])
		{
		case 'F':
		case 'P':
		case 'T':
		case 'V':
		case 'Y':
			return 0.0F;

		default:
			return 1.0F;
		}
		break;

	case 'F':
	case 'P':
	case 'V':
	case 'Y':
		switch(str[c + 1])
		{
		case 'A':
		case 'T':
			return 0.0F;

		default:
			return 1.0F;
		}
		break;

	case 'Q':
	case 'T':
		switch(str[c + 1])
		{
		case '\'':
		case '.':
			return 1.0F;

		default:
			return 0.0F;
		}
		break;

	case '\'':
		return 1.0F;

	case '.':
		return 1.0F;

	default:
		switch(str[c + 1])
		{
		case 'T':
			return 0.0F;

		default:
			return 1.0F;
		}
		break;
	}
}

// 0x801320F8
void mnPlayers1PGameMakeString(GObj *gobj, const char *str, f32 x, f32 y, u32 *colors)
{
	intptr_t offsets[/* */] =
	{
		&llMNCommonFontsLetterASprite, &llMNCommonFontsLetterBSprite,
		&llMNCommonFontsLetterCSprite, &llMNCommonFontsLetterDSprite,
		&llMNCommonFontsLetterESprite, &llMNCommonFontsLetterFSprite,
		&llMNCommonFontsLetterGSprite, &llMNCommonFontsLetterHSprite,
		&llMNCommonFontsLetterISprite, &llMNCommonFontsLetterJSprite,
		&llMNCommonFontsLetterKSprite, &llMNCommonFontsLetterLSprite,
		&llMNCommonFontsLetterMSprite, &llMNCommonFontsLetterNSprite,
		&llMNCommonFontsLetterOSprite, &llMNCommonFontsLetterPSprite,
		&llMNCommonFontsLetterQSprite, &llMNCommonFontsLetterRSprite,
		&llMNCommonFontsLetterSSprite, &llMNCommonFontsLetterTSprite,
		&llMNCommonFontsLetterUSprite, &llMNCommonFontsLetterVSprite,
		&llMNCommonFontsLetterWSprite, &llMNCommonFontsLetterXSprite,
		&llMNCommonFontsLetterYSprite, &llMNCommonFontsLetterZSprite,

		&llMNCommonFontsSymbolApostropheSprite,
		&llMNCommonFontsSymbolPercentSprite,
		&llMNCommonFontsSymbolPeriodSprite
	};
	f32 widths[/* */] =
	{
		5.0F, 4.0F, 4.0F, 4.0F, 4.0F, 4.0F, 4.0F, 4.0F, 3.0F, 4.0F, 4.0F, 4.0F, 5.0F, 5.0F, 4.0F,
		4.0F, 5.0F, 4.0F, 4.0F, 5.0F, 4.0F, 5.0F, 5.0F, 5.0F, 5.0F, 4.0F, 2.0F, 7.0F, 3.0F
	};
	SObj *sobj;
	f32 current_x = x;
	s32 i;
	sb32 is_number;

	for (i = 0; str[i] != 0; i++)
	{
		is_number = ((str[i] >= '0') && (str[i] <= '9')) ? TRUE : FALSE;

		if ((is_number != FALSE) || (str[i] == ' '))
		{
			if (str[i] == ' ')
			{
				current_x += 3.0F;
			}
			else current_x += str[i] - '0';
		}
		else
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[8], offsets[mnPlayers1PGameGetCharacterID(str[i])]));
			sobj->pos.x = current_x;

			current_x += widths[mnPlayers1PGameGetCharacterID(str[i])] + mnPlayers1PGameGetCharacterSpacing(str, i);

			switch (str[i])
			{
			case '\'':
				sobj->pos.y = y - 1.0F;
				break;
				
			case '.':
				sobj->pos.y = y + 4.0F;
				break;
				
			default:
				sobj->pos.y = y;
				break;
			}
			sobj->sprite.attr &= ~SP_FASTCOPY;
			sobj->sprite.attr |= SP_TRANSPARENT;
			sobj->sprite.red = colors[0];
			sobj->sprite.green = colors[1];
			sobj->sprite.blue = colors[2];
		}
	}
}

// 0x80132384
void mnPlayers1PGameSelectFighterPuck(s32 player, s32 select_button)
{
	s32 held_player = sMNPlayers1PGameSlot.held_player;
	s32 costume = ftParamGetCostumeCommonID(sMNPlayers1PGameSlot.fkind, select_button);

	ftParamInitAllParts(sMNPlayers1PGameSlot.player, costume, 0);

	sMNPlayers1PGameSlot.costume = costume;
	sMNPlayers1PGameSlot.is_selected = TRUE;
	sMNPlayers1PGameSlot.holder_player = GMCOMMON_PLAYERS_MAX;
	sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusHover;

	mnPlayers1PGameUpdateCursor(sMNPlayers1PGameSlot.cursor, player, sMNPlayers1PGameSlot.cursor_status);

	sMNPlayers1PGameSlot.held_player = -1;
	sMNPlayers1PGameSlot.is_fighter_selected = TRUE;

	mnPlayers1PGameUpdateCursorPlacementPriorities(held_player);
	mnPlayers1PGameAnnounceFighter(player, held_player);
	mnPlayers1PGameMakePortraitFlash(held_player);
	mnPlayers1PGameMakeStock(sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);
}

// 0x8013243C
f32 mnPlayers1PGameGetNextPortraitX(s32 portrait, f32 current_pos_x)
{
	f32 portrait_pos_x[/* */] =
	{
		25.0F, 70.0F, 115.0F, 160.0F, 205.0F, 250.0F,
		25.0F, 70.0F, 115.0F, 160.0F, 205.0F, 250.0F
	};

	f32	portrait_vel[/* */] =
	{
		1.9F, 3.9F, 7.8F, -7.8F, -3.8F, -1.8F,
		1.8F, 3.8F, 7.8F, -7.8F, -3.8F, -1.8F
	};

	if (current_pos_x == portrait_pos_x[portrait])
	{
		return -1.0F;
	}
	else if (portrait_pos_x[portrait] < current_pos_x)
	{
		return ((current_pos_x + portrait_vel[portrait]) <= portrait_pos_x[portrait]) ? 
		portrait_pos_x[portrait] :
		current_pos_x + portrait_vel[portrait];
	}
	else return (current_pos_x + portrait_vel[portrait]) >= portrait_pos_x[portrait] ?
	portrait_pos_x[portrait] :
	current_pos_x + portrait_vel[portrait];
}

// 0x80132550
sb32 mnPlayers1PGameCheckFighterCrossed(s32 fkind)
{
	return FALSE;
}

// 0x8013255C
void mnPlayers1PGamePortraitProcUpdate(GObj *gobj)
{
	f32 new_pos_x = mnPlayers1PGameGetNextPortraitX(gobj->user_data.s, SObjGetStruct(gobj)->pos.x);

	if (new_pos_x != -1.0F)
	{
		SObjGetStruct(gobj)->pos.x = new_pos_x;

		if (SObjGetStruct(gobj)->next != NULL)
		{
			SObjGetStruct(gobj)->next->pos.x = SObjGetStruct(gobj)->pos.x + 4.0F;
		}
	}
}

// 0x801325D8
void mnPlayers1PGameSetPortraitWallpaperPosition(SObj *sobj, s32 portrait)
{
	Vec2f pos[/* */] =
	{
		{ -35.0F, 36.0F }, { -35.0F, 36.0F },
		{ -35.0F, 36.0F }, { 310.0F, 36.0F },
		{ 310.0F, 36.0F }, { 310.0F, 36.0F },
		{ -35.0F, 79.0F }, { -35.0F, 79.0F },
		{ -35.0F, 79.0F }, { 310.0F, 79.0F },
		{ 310.0F, 79.0F }, { 310.0F, 79.0F }
	};

	sobj->pos.x = pos[portrait].x;
	sobj->pos.y = pos[portrait].y;
}

// 0x80132634
void mnPlayers1PGamePortraitAddCross(GObj *gobj, s32 portrait)
{
	SObj *sobj = SObjGetStruct(gobj);
	f32 x = sobj->pos.x;
	f32 y = sobj->pos.y;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], &llMNPlayersPortraitsCrossSprite));

	sobj->pos.x = x + 4.0F;
	sobj->pos.y = y + 12.0F;
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0xFF;
	sobj->sprite.green = 0x00;
	sobj->sprite.blue = 0x00;
}

// 0x801326C8
sb32 mnPlayers1PGameCheckFighterLocked(s32 fkind)
{
	switch (fkind)
	{
	case nFTKindNess:
		return (sMNPlayers1PGameFighterMask & (1 << nFTKindNess)) ? FALSE : TRUE;

	case nFTKindPurin:
		return (sMNPlayers1PGameFighterMask & (1 << nFTKindPurin)) ? FALSE : TRUE;

	case nFTKindCaptain:
		return (sMNPlayers1PGameFighterMask & (1 << nFTKindCaptain)) ? FALSE : TRUE;

	case nFTKindLuigi:
		return (sMNPlayers1PGameFighterMask & (1 << nFTKindLuigi)) ? FALSE : TRUE;

	default:
		return FALSE;
	}
}

// 0x80138860
s32 dMNPlayers1PGameUnknown0x80138860[/* */] =
{
	0xC55252C5,
	0xA6524294,
	0x595252C5,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000
};

// 0x80132794 - Unused?
void func_ovl27_80132794(void)
{
	return;
}

// 0x8013279C
s32 mnPlayers1PGameGetFighterKind(s32 portrait)
{
	s32 fkinds[/* */] =
	{
		nFTKindLuigi, nFTKindMario, nFTKindDonkey, nFTKindLink, nFTKindSamus,   nFTKindCaptain,
		nFTKindNess,  nFTKindYoshi, nFTKindKirby,  nFTKindFox,  nFTKindPikachu, nFTKindPurin
	};

	return fkinds[portrait];
}

// 0x801327EC
s32 mnPlayers1PGameGetPortrait(s32 fkind)
{
	s32 portraits[/* */] =
	{
		1, 9, 2, 4, 0, 3,
		7, 5, 8, 10, 11, 6
	};

	return portraits[fkind];
}

// 0x8013283C
void mnPlayers1PGamePortraitProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x30, 0x30, 0x30, 0xFF);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, NOISE, TEXEL0, PRIMITIVE, TEXEL0, 0, 0, 0, TEXEL0, NOISE, TEXEL0, PRIMITIVE, TEXEL0,  0, 0, 0, TEXEL0);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

	lbCommonDrawSObjNoAttr(gobj);
}

// 0x801328FC
void mnPlayers1PGameMakePortraitShadow(s32 portrait)
{
	GObj *gobj;
	SObj *sobj;
	intptr_t offsets[/* */] =
	{
		0x0, 									0x0,
		0x0, 									0x0,
		&llMNPlayersPortraitsLuigiShadowSprite, 	0x0,
		0x0, 									&llMNPlayersPortraitsCaptainShadowSprite,
		0x0, 									0x0,
		&llMNPlayersPortraitsPurinShadowSprite, 	&llMNPlayersPortraitsNessShadowSprite
	};

	gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGamePortraitProcUpdate, nGCProcessKindFunc, 1);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], &llMNPlayersPortraitsPortraitFireBgSprite));
	sobj->pos.x = (((portrait >= 6) ? portrait - 6 : portrait) * 45) + 25;
	sobj->pos.y = (((portrait >= 6) ? 1 : 0) * 43) + 36;

	mnPlayers1PGameSetPortraitWallpaperPosition(sobj, portrait);
	gobj->user_data.s = portrait;

	gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, mnPlayers1PGamePortraitProcDisplay, 27, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGamePortraitProcUpdate, nGCProcessKindFunc, 1);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], offsets[mnPlayers1PGameGetFighterKind(portrait)]));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;

	gobj->user_data.s = portrait;
	mnPlayers1PGameSetPortraitWallpaperPosition(sobj, portrait);

	gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGamePortraitProcUpdate, nGCProcessKindFunc, 1);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], &llMNPlayersPortraitsPortraitQuestionMarkSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->envcolor.r = 0x5B;
	sobj->envcolor.g = 0x41;
	sobj->envcolor.b = 0x33;
	sobj->sprite.red = 0xC4;
	sobj->sprite.green = 0xB9;
	sobj->sprite.blue = 0xA9;

	gobj->user_data.s = portrait;
	mnPlayers1PGameSetPortraitWallpaperPosition(sobj, portrait);
}

// 0x80132BA4
void mnPlayers1PGameMakePortrait(s32 portrait)
{
	GObj *portrait_gobj, *wallpaper_gobj;
	SObj *sobj;
	intptr_t offsets[/* */] =
	{
		&llMNPlayersPortraitsMarioSprite,	&llMNPlayersPortraitsFoxSprite,
		&llMNPlayersPortraitsDonkeySprite,	&llMNPlayersPortraitsSamusSprite,
		&llMNPlayersPortraitsLuigiSprite,	&llMNPlayersPortraitsLinkSprite,
		&llMNPlayersPortraitsYoshiSprite,	&llMNPlayersPortraitsCaptainSprite,
		&llMNPlayersPortraitsKirbySprite,	&llMNPlayersPortraitsPikachuSprite,
		&llMNPlayersPortraitsPurinSprite,	&llMNPlayersPortraitsNessSprite
	};

	if (mnPlayers1PGameCheckFighterLocked(mnPlayers1PGameGetFighterKind(portrait)) != FALSE)
	{
		mnPlayers1PGameMakePortraitShadow(portrait);
	}
	else
	{
		wallpaper_gobj = gcMakeGObjSPAfter(0, NULL, 25, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(wallpaper_gobj, lbCommonDrawSObjAttr, 32, GOBJ_PRIORITY_DEFAULT, ~0);
		wallpaper_gobj->user_data.s = portrait;
		gcAddGObjProcess(wallpaper_gobj, mnPlayers1PGamePortraitProcUpdate, nGCProcessKindFunc, 1);

		sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], &llMNPlayersPortraitsPortraitFireBgSprite));

		mnPlayers1PGameSetPortraitWallpaperPosition(sobj, portrait);

		portrait_gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(portrait_gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
		gcAddGObjProcess(portrait_gobj, mnPlayers1PGamePortraitProcUpdate, nGCProcessKindFunc, 1);

		sobj = lbCommonMakeSObjForGObj(portrait_gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], offsets[mnPlayers1PGameGetFighterKind(portrait)]));
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		portrait_gobj->user_data.s = portrait;

		if (mnPlayers1PGameCheckFighterCrossed(mnPlayers1PGameGetFighterKind(portrait)) != FALSE)
		{
			mnPlayers1PGamePortraitAddCross(portrait_gobj, portrait);
		}
		mnPlayers1PGameSetPortraitWallpaperPosition(sobj, portrait);
	}
}

// 0x80132D60
void mnPlayers1PGameMakePortraitAll(void)
{
	s32 i;

	for (i = nFTKindPlayableStart; i <= nFTKindPlayableEnd; i++)
	{
		mnPlayers1PGameMakePortrait(i);
	}
}

// 0x80132DA0
void mnPlayers1PGameMakeNameAndEmblem(GObj *gobj, s32 player, s32 fkind)
{
	SObj *sobj;
	Vec2f pos[/* */] =
	{
		{ 13.0F, 28.0F }, {  6.0F, 25.0F },
		{  5.0F, 25.0F }, { 13.0F, 25.0F },
		{ 13.0F, 28.0F }, { 13.0F, 28.0F },
		{ 16.0F, 25.0F }, {  4.0F, 25.0F },
		{ 13.0F, 25.0F }, { 13.0F, 25.0F },
		{ 13.0F, 25.0F }, { 13.0F, 25.0F }
	};
	intptr_t emblem_offsets[/* */] =
	{
		&llFTEmblemSpritesMarioSprite,     &llFTEmblemSpritesFoxSprite,
		&llFTEmblemSpritesDonkeySprite,    &llFTEmblemSpritesMetroidSprite,
		&llFTEmblemSpritesMarioSprite,     &llFTEmblemSpritesZeldaSprite,
		&llFTEmblemSpritesYoshiSprite,     &llFTEmblemSpritesFZeroSprite,
		&llFTEmblemSpritesKirbySprite,     &llFTEmblemSpritesPMonstersSprite,
		&llFTEmblemSpritesPMonstersSprite, &llFTEmblemSpritesMotherSprite
	};
	intptr_t name_offsets[/* */] =
	{
		&llMNPlayersCommonMarioTextSprite,      &llMNPlayersCommonFoxTextSprite,
		&llMNPlayersCommonDKTextSprite,         &llMNPlayersCommonSamusTextSprite,
		&llMNPlayersCommonLuigiTextSprite,      &llMNPlayersCommonLinkTextSprite,
		&llMNPlayersCommonYoshiTextSprite,      &llMNPlayersCommonCaptainFalconTextSprite,
		&llMNPlayersCommonKirbyTextSprite,      &llMNPlayersCommonPikachuTextSprite,
		&llMNPlayersCommonJigglypuffTextSprite, &llMNPlayersCommonNessTextSprite
	};

	if (fkind != nFTKindNull)
	{
		gcRemoveSObjAll(gobj);

		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[1], emblem_offsets[fkind]));
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		sobj->sprite.red = 0x00;
		sobj->sprite.green = 0x00;
		sobj->sprite.blue = 0x00;
		sobj->pos.x = 35.0F;
		sobj->pos.y = 144.0F;

		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], name_offsets[fkind]));
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		sobj->pos.x = 33.0F;
		sobj->pos.y = 202.0F;
	}
}

// 0x80132F1C
void mnPlayers1PGameMakePortraitCamera(void)
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
			40,
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

// 0x80132FBC
void mnPlayers1PGameMakePortraitWallpaperCamera(void)
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
			COBJ_MASK_DLLINK(32),
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

// 0x8013305C
void mnPlayers1PGameMakePortraitFlashCamera(void)
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
			50,
			COBJ_MASK_DLLINK(33),
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

// 0x801330FC
void mnPlayers1PGameMakePlayerKindCamera(void)
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
			30,
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

// 0x8013319C
void mnPlayers1PGameSetGateLUT(GObj *gobj, s32 player)
{
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llMNPlayersCommonGateMan1PLUT, &llMNPlayersCommonGateMan2PLUT,
		&llMNPlayersCommonGateMan3PLUT, &llMNPlayersCommonGateMan4PLUT
	};

	sobj = SObjGetStruct(gobj);
	sobj->sprite.LUT = lbRelocGetFileData(int*, sMNPlayers1PGameFiles[0], offsets[player]);
}

// 0x801331F4
void mnPlayers1PGameMakeGate(s32 player)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llMNPlayersCommon1PTextSprite, &llMNPlayersCommon2PTextSprite,
		&llMNPlayersCommon3PTextSprite, &llMNPlayersCommon4PTextSprite
	};
	f32 pos_x[/* */] = { 8.0F, 5.0F, 5.0F, 5.0F };

	gobj = lbCommonMakeSpriteGObj
	(
		0,
		NULL,
		22,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSObjAttr,
		28,
		GOBJ_PRIORITY_DEFAULT,
		~0,
		lbRelocGetFileData
		(
			Sprite*,
			sMNPlayers1PGameFiles[5],
			&llMNPlayers1PModeRedCardSprite
		),
		nGCProcessKindFunc,
		NULL,
		1
	);
	SObjGetStruct(gobj)->pos.x = 25.0F;
	SObjGetStruct(gobj)->pos.y = 127.0F;
	SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
	SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;

	sMNPlayers1PGameSlot.panel = gobj;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], offsets[player]));
	sobj->pos.x = pos_x[player] + 25.0F;
	sobj->pos.y = 132.0F;
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0x00;
	sobj->sprite.green = 0x00;
	sobj->sprite.blue = 0x00;

	mnPlayers1PGameSetGateLUT(gobj, player);

	gobj = gcMakeGObjSPAfter(0, NULL, 22, GOBJ_PRIORITY_DEFAULT);
	sMNPlayers1PGameSlot.name_emblem_gobj = gobj;
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 28, GOBJ_PRIORITY_DEFAULT, ~0);

	mnPlayers1PGameUpdateNameAndEmblem(player);
}

// 0x801333D4
void mnPlayers1PGameMakeTimeNumber(GObj *gobj, s32 number, f32 x, f32 y, u32 *colors, s32 digit_count_max, sb32 is_fixed_digit_count)
{
	intptr_t offsets[/* */] =
	{
		&llMNPlayersCommon0DarkSprite,
		&llMNPlayersCommon1DarkSprite,
		&llMNPlayersCommon2DarkSprite,
		&llMNPlayersCommon3DarkSprite,
		&llMNPlayersCommon4DarkSprite,
		&llMNPlayersCommon5DarkSprite,
		&llMNPlayersCommon6DarkSprite,
		&llMNPlayersCommon7DarkSprite,
		&llMNPlayersCommon8DarkSprite,
		&llMNPlayersCommon9DarkSprite
	};

	f32 widths[/* */] =
	{
		8.0F, 6.0F, 9.0F, 8.0F, 8.0F,
		9.0F, 8.0F, 8.0F, 8.0F, 9.0F
	};

	SObj *sobj;
	f32 left_x = x;
	s32 i;
	s32 digit;

	if (number < 0)
	{
		number = 0;
	}
	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], offsets[number % 10]));
	mnPlayers1PGameSetDigitColors(sobj, colors);
	left_x -= widths[number % 10];
	sobj->pos.x = left_x;
	sobj->pos.y = y;

	for (i = 1; i < ((is_fixed_digit_count != FALSE) ? digit_count_max : mnPlayers1PGameGetNumberDigitCount(number, digit_count_max)); i++)
	{
		digit = (mnPlayers1PGameGetPowerOf(10, i) != 0) ? number / mnPlayers1PGameGetPowerOf(10, i) : 0;

		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], offsets[digit % 10]));
		mnPlayers1PGameSetDigitColors(sobj, colors);
		left_x -= widths[digit % 10];
		sobj->pos.x = left_x;
		sobj->pos.y = y;
	}
}

// 0x80133680
void mnPlayers1PGameMakeTimeSetting(s32 number)
{
	u32 colors[/* */] = { 0x32, 0x1C, 0x0E, 0xFF, 0xFF, 0xFF };
	SObj *sobj;

	while (SObjGetStruct(sMNPlayers1PGameTimeGObj)->next != NULL)
	{
		gcEjectSObj(SObjGetStruct(sMNPlayers1PGameTimeGObj)->next);
	}
	if (number == 100)
	{
		sobj = lbCommonMakeSObjForGObj(sMNPlayers1PGameTimeGObj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonInfinityDarkSprite));
		sobj->pos.x = 194.0F;
		sobj->pos.y = 24.0F;
		sobj->envcolor.r = colors[0];
		sobj->envcolor.g = colors[1];
		sobj->envcolor.b = colors[2];
		sobj->sprite.red = colors[3];
		sobj->sprite.green = colors[4];
		sobj->sprite.blue = colors[5];
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
	}
	else if (number < 10)
	{
		mnPlayers1PGameMakeTimeNumber(sMNPlayers1PGameTimeGObj, number, 205.0F, 23.0F, colors, 2, FALSE);
	}
	else mnPlayers1PGameMakeTimeNumber(sMNPlayers1PGameTimeGObj, number, 209.0F, 23.0F, colors, 2, FALSE);
}

// 0x80133804
void mnPlayers1PGameMakeTimeSelect(s32 number)
{
	GObj *gobj;

	if (sMNPlayers1PGameTimeGObj != NULL)
	{
		gcEjectGObj(sMNPlayers1PGameTimeGObj);
	}
	gobj = lbCommonMakeSpriteGObj
	(
		0,
		NULL,
		23,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSObjAttr,
		26,
		GOBJ_PRIORITY_DEFAULT,
		~0,
		lbRelocGetFileData
		(
			Sprite*,
			sMNPlayers1PGameFiles[0],
			&llMNPlayersCommonTimeSelectorSprite
		),
		nGCProcessKindFunc,
		NULL,
		1
	);
	sMNPlayers1PGameTimeGObj = gobj;

	SObjGetStruct(gobj)->pos.x = 140.0F;
	SObjGetStruct(gobj)->pos.y = 22.0F;
	SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
	SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;

	mnPlayers1PGameMakeTimeSetting(sMNPlayers1PGameTimeSetting);
}

// 0x801338EC
void mnPlayers1PGameMakeWallpaper(void)
{
	GObj *gobj;
	SObj *sobj;

	gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);
	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[2], &llMNSelectCommonStoneBackgroundSprite));
	sobj->cms = G_TX_WRAP;
	sobj->cmt = G_TX_WRAP;
	sobj->masks = 6;
	sobj->maskt = 5;
	sobj->lrs = 300;
	sobj->lrt = 220;
	sobj->pos.x = 10.0F;
	sobj->pos.y = 10.0F;
}

// 0x80133990
void mnPlayers1PGameMakeWallpaperCamera(void)
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
			COBJ_MASK_DLLINK(26),
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

// 0x80133A30
void mnPlayers1PGameLabelsProcDisplay(GObj *gobj)
{
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
	gDPSetCombineLERP(gSYTaskmanDLHeads[0]++, 0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE,  0, 0, 0, PRIMITIVE);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x57, 0x60, 0x88, 0xFF);
	gDPFillRectangle(gSYTaskmanDLHeads[0]++, 157, 136, 320, 141);
	gDPPipeSync(gSYTaskmanDLHeads[0]++);
	gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
	gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);

	lbCommonClearExternSpriteParams();
	lbCommonDrawSObjAttr(gobj);
}

// 0x80133B74
SObj* mnPlayers1PGameGetArrowSObj(GObj *gobj, s32 direction)
{
	if (SObjGetStruct(gobj) != NULL)
	{
		if (direction == SObjGetStruct(gobj)->user_data.s)
		{
			return SObjGetStruct(gobj);
		}
		if ((SObjGetStruct(gobj)->next != NULL) && (direction == SObjGetStruct(gobj)->next->user_data.s))
		{
			return SObjGetStruct(gobj)->next;
		}
	}
	return NULL;
}

// 0x80133BC0
void mnPlayers1PGameLevelThreadUpdate(GObj *gobj)
{
	SObj *sobj;
	s32 player = gobj->user_data.s;
	s32 blink_wait = 10;

	while (TRUE)
	{
		blink_wait--;

		if (blink_wait == 0)
		{
			blink_wait = 10;
			gobj->flags = (gobj->flags == GOBJ_FLAG_HIDDEN) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;
		}
		if (sMNPlayers1PGameLevelValue == nSC1PGameDifficultyVeryEasy)
		{
			sobj = mnPlayers1PGameGetArrowSObj(gobj, 0);

			if (sobj != NULL)
			{
				gcEjectSObj(sobj);
			}
		}
		else if (mnPlayers1PGameGetArrowSObj(gobj, 0) == NULL)
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonArrowLSprite));
			sobj->pos.x = 194.0F;
			sobj->pos.y = 158.0F;
			sobj->sprite.attr &= ~SP_FASTCOPY;
			sobj->sprite.attr |= SP_TRANSPARENT;
			sobj->user_data.s = 0;
		}
		if (sMNPlayers1PGameLevelValue == nSC1PGameDifficultyVeryHard)
		{
			sobj = mnPlayers1PGameGetArrowSObj(gobj, 1);

			if (sobj != NULL)
			{
				gcEjectSObj(sobj);
			}
		}
		else if (mnPlayers1PGameGetArrowSObj(gobj, 1) == NULL)
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonArrowRSprite));
			sobj->pos.x = 269.0F;
			sobj->pos.y = 158.0F;
			sobj->sprite.attr &= ~SP_FASTCOPY;
			sobj->sprite.attr |= SP_TRANSPARENT;
			sobj->user_data.s = 1;
		}
		gcSleepCurrentGObjThread(1);
	}
}

// 0x80133D9C
void mnPlayers1PGameMakeLevel(s32 level)
{
	GObj *gobj;
	SObj *sobj;

	intptr_t offsets[/* */] =
	{
		&llMNPlayersDifficultyVeryEasyTextSprite,
		&llMNPlayersDifficultyEasyTextSprite,
		&llMNPlayersDifficultyNormalTextSprite,
		&llMNPlayersDifficultyHardTextSprite,
		&llMNPlayersDifficultyVeryHardTextSprite
	};
	Vec2f pos[/* */] =
	{
		{ 204.0F, 159.0F },
		{ 219.0F, 159.0F },
		{ 209.0F, 159.0F },
		{ 219.0F, 159.0F },
		{ 205.0F, 159.0F }
	};
	SYColorRGB colors[/* */] =
	{
		{ 0x41, 0x6F, 0xE4 },
		{ 0x8D, 0xBB, 0x5A },
		{ 0xE4, 0xBE, 0x41 },
		{ 0xE4, 0x78, 0x41 },
		{ 0xE4, 0x41, 0x41 }
	};

	if (sMNPlayers1PGameLevelGObj != NULL)
	{
		gcEjectGObj(sMNPlayers1PGameLevelGObj);
	}
	sMNPlayers1PGameLevelGObj = gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 34, GOBJ_PRIORITY_DEFAULT, ~0);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[6], offsets[level]));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->pos.x = pos[level].x;
	sobj->pos.y = pos[level].y;
	sobj->sprite.red = colors[level].r;
	sobj->sprite.green = colors[level].g;
	sobj->sprite.blue = colors[level].b;
}

// 0x80133F30
void mnPlayers1PGameMakeLevelOption(void)
{
	GObj *gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 34, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGameLevelThreadUpdate, nGCProcessKindThread, 1);
	mnPlayers1PGameMakeLevel(sMNPlayers1PGameLevelValue);
}

// 0x80133FA4
void mnPlayers1PGameStockThreadUpdate(GObj *gobj)
{
	SObj *sobj;
	s32 player = gobj->user_data.s;
	s32 blink_wait = 10;

	while (TRUE)
	{
		blink_wait--;

		if (blink_wait == 0)
		{
			blink_wait = 10;
			gobj->flags = (gobj->flags == GOBJ_FLAG_HIDDEN) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;
		}
		if (sMNPlayers1PGameStockValue == 0)
		{
			sobj = mnPlayers1PGameGetArrowSObj(gobj, 0);

			if (sobj != NULL)
			{
				gcEjectSObj(sobj);
			}
		}
		else if (mnPlayers1PGameGetArrowSObj(gobj, 0) == NULL)
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonArrowLSprite));
			sobj->pos.x = 194.0F;
			sobj->pos.y = 178.0F;
			sobj->sprite.attr &= ~SP_FASTCOPY;
			sobj->sprite.attr |= SP_TRANSPARENT;
			sobj->user_data.s = 0;
		}
		if (sMNPlayers1PGameStockValue == 4)
		{
			sobj = mnPlayers1PGameGetArrowSObj(gobj, 1);

			if (sobj != NULL)
			{
				gcEjectSObj(sobj);
			}
		}
		else if (mnPlayers1PGameGetArrowSObj(gobj, 1) == NULL)
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonArrowRSprite));
			sobj->pos.x = 269.0F;
			sobj->pos.y = 178.0F;
			sobj->sprite.attr &= ~SP_FASTCOPY;
			sobj->sprite.attr |= SP_TRANSPARENT;
			sobj->user_data.s = 1;
		}
		gcSleepCurrentGObjThread(1);
	}
}

// 0x8013419C
void mnPlayers1PGameMakeStock(s32 stock, s32 fkind)
{
	GObj *gobj;

	if (sMNPlayers1PGameStockGObj != NULL)
	{
		gcEjectGObj(sMNPlayers1PGameStockGObj);
	}
	sMNPlayers1PGameStockGObj = gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 34, GOBJ_PRIORITY_DEFAULT, ~0);

	for (stock += 1; stock > 0; stock--)
	{
		SObj *sobj;

		if (fkind == nFTKindNull)
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[7], &llFTStocksZakoSprite));
			sobj->pos.y = 179.0F;
		}
		else
		{
			FTStruct *fp = ftGetStruct(sMNPlayers1PGameSlot.player);
			sobj = lbCommonMakeSObjForGObj(gobj, fp->attr->sprites->stock_sprite);
			sobj->sprite.LUT = fp->attr->sprites->stock_luts[fp->costume];
			sobj->pos.y = 178.0F;
		}
		sobj->pos.x = (stock - 1) * 12 + 207.0F;
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
	}
}

// 0x8013434C
void mnPlayers1PGameMakeStockOption(void)
{
	GObj *gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);

	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 34, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGameStockThreadUpdate, nGCProcessKindThread, 1);
	mnPlayers1PGameMakeStock(sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);
}

// 0x801343C8
void mnPlayers1PGameMakeLabels(void)
{
	GObj *gobj;
	SObj *sobj;

	gobj = lbCommonMakeSpriteGObj
	(
		0,
		NULL,
		23,
		GOBJ_PRIORITY_DEFAULT,
		mnPlayers1PGameLabelsProcDisplay,
		34,
		GOBJ_PRIORITY_DEFAULT,
		~0,
		lbRelocGetFileData
		(
			Sprite*,
			sMNPlayers1PGameFiles[5],
			&llMNPlayers1PMode1PlayerGameTextSprite
		),
		nGCProcessKindFunc,
		NULL,
		1
	);
	SObjGetStruct(gobj)->pos.x = 27.0F;
	SObjGetStruct(gobj)->pos.y = 24.0F;
	SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
	SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
	SObjGetStruct(gobj)->sprite.red = 0xE3;
	SObjGetStruct(gobj)->sprite.green = 0xAC;
	SObjGetStruct(gobj)->sprite.blue = 0x04;

	mnPlayers1PGameMakeTimeSelect(sMNPlayers1PGameTimeSetting);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonBackButtonSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->pos.x = 244.0F;
	sobj->pos.y = 23.0F;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeOptionTextSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->envcolor.r = 0x00;
	sobj->envcolor.g = 0x00;
	sobj->envcolor.b = 0x00;
	sobj->sprite.red = 0xAF;
	sobj->sprite.green = 0xB1;
	sobj->sprite.blue = 0xCC;
	sobj->pos.x = 180.0F;
	sobj->pos.y = 129.0F;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeOptionOutlineSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0x57;
	sobj->sprite.green = 0x60;
	sobj->sprite.blue = 0x88;
	sobj->cms = G_TX_WRAP;
	sobj->cmt = G_TX_MIRROR;
	sobj->masks = 0;
	sobj->maskt = 5;
	sobj->lrs = 184;
	sobj->lrt = 64;
	sobj->pos.x = 128.0F;
	sobj->pos.y = 141.0F;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeLevelColonTextSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0xC5;
	sobj->sprite.green = 0xB6;
	sobj->sprite.blue = 0xA7;
	sobj->pos.x = 145.0F;
	sobj->pos.y = 159.0F;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeStockColonTextSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0xC5;
	sobj->sprite.green = 0xB6;
	sobj->sprite.blue = 0xA7;
	sobj->pos.x = 144.0F;
	sobj->pos.y = 179.0F;
}

// 0x801346B8
void mnPlayers1PGameMakeLabelsCamera(void)
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
			70,
			COBJ_MASK_DLLINK(34),
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

// 0x80134758
u32 mnPlayers1PGameGetHiScore(s32 fkind)
{
	return gSCManagerBackupData.spgame_records[fkind].spgame_hiscore;
}

// 0x8013476C
void mnPlayers1PGameMakeHiScore(void)
{
	GObj *gobj;
	s32 unused[2];
	SObj *sobj;

	u32 text_colors[/* */] = { 0x7E, 0x7C, 0x77 };
	u32 number_colors[/* */] = { 0x00, 0x00, 0x00, 0x7E, 0x7C, 0x77 };
	SYColorRGB difficulty_colors[/* */] =
	{
		{ 0x41, 0x6F, 0xE4 },
		{ 0x8D, 0xBB, 0x5A },
		{ 0xE4, 0xBE, 0x41 },
		{ 0xE4, 0x78, 0x41 },
		{ 0xE4, 0x41, 0x41 }
	};

	s32 best_difficulty;
	s32 fkind = mnPlayers1PGameGetForcePuckFighterKind();

	if (sMNPlayers1PGameHiScoreGObj != NULL)
	{
		gcEjectGObj(sMNPlayers1PGameHiScoreGObj);
		sMNPlayers1PGameHiScoreGObj = NULL;
	}
	if (fkind != nFTKindNull)
	{
		sMNPlayers1PGameHiScoreGObj = gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);

		mnPlayers1PGameMakeString(gobj, "HIGH SCORE", 142.0F, 201.0F, text_colors);
		mnPlayers1PGameMakeNumber(gobj, mnPlayers1PGameGetHiScore(fkind), 256.0F, 198.0F, number_colors, 8, TRUE);

		best_difficulty = gSCManagerBackupData.spgame_records[fkind].spgame_best_difficulty;

		if (best_difficulty != 0)
		{
			sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeSmashLogoSprite));
			sobj->sprite.attr &= ~SP_FASTCOPY;
			sobj->sprite.attr |= SP_TRANSPARENT;
			sobj->pos.x = 126.0F;
			sobj->pos.y = 198.0F;
			sobj->sprite.red = difficulty_colors[best_difficulty - 1].r;
			sobj->sprite.green = difficulty_colors[best_difficulty - 1].g;
			sobj->sprite.blue = difficulty_colors[best_difficulty - 1].b;
		}
	}
}

// 0x80134968
u32 mnPlayers1PGameGetBonusCount(s32 fkind)
{
	return gSCManagerBackupData.spgame_records[fkind].spgame_total_bonuses;
}

// 0x8013497C
void mnPlayers1PGameMakeBonusCount(void)
{
	GObj *gobj;
	s32 unused[2];
	SObj *sobj;
	u32 colors[/* */] = { 0x00, 0x00, 0x00, 0x40, 0x6F, 0xCD };
	s32 fkind = mnPlayers1PGameGetForcePuckFighterKind();

	if (sMNPlayers1PGameBonusesGObj != NULL)
	{
		gcEjectGObj(sMNPlayers1PGameBonusesGObj);
		sMNPlayers1PGameBonusesGObj = NULL;
	}
	if (fkind != nFTKindNull)
	{
		sMNPlayers1PGameBonusesGObj = gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);
		gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);

		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeOpeningParenthesisSprite));
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		sobj->pos.x = 258.0F;
		sobj->pos.y = 199.0F;
		sobj->sprite.red = 0x40;
		sobj->sprite.green = 0x6F;
		sobj->sprite.blue = 0xCD;

		sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeClosingParenthesisSprite));
		sobj->sprite.attr &= ~SP_FASTCOPY;
		sobj->sprite.attr |= SP_TRANSPARENT;
		sobj->pos.x = 286.0F;
		sobj->pos.y = 199.0F;
		sobj->sprite.red = 0x40;
		sobj->sprite.green = 0x6F;
		sobj->sprite.blue = 0xCD;

		mnPlayers1PGameMakeNumber(gobj, mnPlayers1PGameGetBonusCount(fkind), 285.0F, 198.0F, colors, 3, TRUE);
	}
}

// 0x80134B3C
void mnPlayers1PGameMakeFighterRecord(void)
{
	mnPlayers1PGameMakeHiScore();
	mnPlayers1PGameMakeBonusCount();
}

// 0x80134B64
u32 mnPlayers1PGameGetTotalHiScore(void)
{
	s32 i;
	u32 sum = 0;

	for (i = nFTKindPlayableStart; i <= nFTKindPlayableEnd; i++)
	{
		sum += mnPlayers1PGameGetHiScore(i);
	}
	return sum;
}

// 0x80134BB4
void mnPlayers1PGameMakeTotalHiScore(void)
{
	GObj *gobj;
	s32 unused[3];
	u32 text_colors[/* */] = { 0x7E, 0x7C, 0x77 };
	u32 number_colors[/* */] = { 0x00, 0x00, 0x00, 0x7E, 0x7C, 0x77 };

	gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);
	mnPlayers1PGameMakeString(gobj, "TOTAL HIGH SCORE", 109.0F, 211.0F, text_colors);
	mnPlayers1PGameMakeNumber(gobj, mnPlayers1PGameGetTotalHiScore(), 256.0F, 208.0F, number_colors, 9, TRUE);
}

// 0x80134CB8
u32 mnPlayers1PGameGetTotalBonusCount(void)
{
	s32 i;
	u32 sum = 0;

	for (i = nFTKindPlayableStart; i <= nFTKindPlayableEnd; i++)
	{
		sum += mnPlayers1PGameGetBonusCount(i);
	}
	return sum;
}

// 0x80134D08
void mnPlayers1PGameMakeTotalBonusCount(void)
{
	GObj *gobj;
	s32 unused[2];
	SObj *sobj;
	u32 colors[/* */] = { 0x00, 0x00, 0x00, 0x40, 0x6F, 0xCD };

	gobj = gcMakeGObjSPAfter(0, NULL, 23, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeOpeningParenthesisSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->pos.x = 258.0F;
	sobj->pos.y = 209.0F;
	sobj->sprite.red = 0x40;
	sobj->sprite.green = 0x6F;
	sobj->sprite.blue = 0xCD;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[5], &llMNPlayers1PModeClosingParenthesisSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->pos.x = 286.0F;
	sobj->pos.y = 209.0F;
	sobj->sprite.red = 0x40;
	sobj->sprite.green = 0x6F;
	sobj->sprite.blue = 0xCD;

	mnPlayers1PGameMakeNumber(gobj, mnPlayers1PGameGetTotalBonusCount(), 285.0F, 208.0F, colors, 3, TRUE);
}

// 0x80134E88
void mnPlayers1PGameMakeTotalRecord(void)
{
	mnPlayers1PGameMakeTotalHiScore();
	mnPlayers1PGameMakeTotalBonusCount();
}

// 0x80134EB0 - Unused?
void func_ovl27_80134EB0(void)
{
	return;
}

// 0x80134EB8 - Unused?
void func_ovl27_80134EB8(void)
{
	return;
}

// 0x80134EC0
s32 mnPlayers1PGameGetFreeCostume(s32 fkind, s32 select_button)
{
	return ftParamGetCostumeCommonID(fkind, select_button);
}

// 0x80134EE0
s32 mnPlayers1PGameGetStatusSelected(s32 fkind)
{
	switch (fkind)
	{
	case nFTKindFox:
	case nFTKindSamus:	
		return nFTDemoStatusWin4;

	case nFTKindDonkey:
	case nFTKindLuigi:
	case nFTKindLink:
	case nFTKindCaptain:
		return nFTDemoStatusWin1;

	case nFTKindYoshi:
	case nFTKindPurin:
	case nFTKindNess:
		return nFTDemoStatusWin2;
		
	case nFTKindMario:
	case nFTKindKirby:
		return nFTDemoStatusWin3;
		
	default:
		return nFTDemoStatusWin1;
	}
}

// 0x80134F40
void mnPlayers1PGameFighterProcUpdate(GObj *fighter_gobj)
{
	FTStruct *fp = ftGetStruct(fighter_gobj);
	s32 player = fp->player;

	if (sMNPlayers1PGameSlot.is_fighter_selected == TRUE)
	{
		if (DObjGetStruct(fighter_gobj)->rotate.vec.f.y < F_CLC_DTOR32(0.1F))
		{
			if (sMNPlayers1PGameSlot.is_status_selected == FALSE)
			{
				scSubsysFighterSetStatus(sMNPlayers1PGameSlot.player, mnPlayers1PGameGetStatusSelected(sMNPlayers1PGameSlot.fkind));

				sMNPlayers1PGameSlot.is_status_selected = TRUE;
			}
		}
		else
		{
			DObjGetStruct(fighter_gobj)->rotate.vec.f.y += F_CST_DTOR32(20.0F);

			if (DObjGetStruct(fighter_gobj)->rotate.vec.f.y > F_CLC_DTOR32(360.0F))
			{
				DObjGetStruct(fighter_gobj)->rotate.vec.f.y = 0.0F;

				scSubsysFighterSetStatus(sMNPlayers1PGameSlot.player, mnPlayers1PGameGetStatusSelected(sMNPlayers1PGameSlot.fkind));

				sMNPlayers1PGameSlot.is_status_selected = TRUE;
			}
		}
	}
	else
	{
		DObjGetStruct(fighter_gobj)->rotate.vec.f.y += F_CST_DTOR32(2.0F);

		if (DObjGetStruct(fighter_gobj)->rotate.vec.f.y > F_CST_DTOR32(360.0F))
		{
			DObjGetStruct(fighter_gobj)->rotate.vec.f.y -= F_CST_DTOR32(360.0F);
		}
	}
}

// 0x80135060
void mnPlayers1PGameMakeFighter(GObj *fighter_gobj, s32 player, s32 fkind, s32 costume)
{
	f32 rot_y;
	FTDesc desc = dFTManagerDefaultFighterDesc;

	if (fkind != nFTKindNull)
	{
		if (fighter_gobj != NULL)
		{
			rot_y = DObjGetStruct(fighter_gobj)->rotate.vec.f.y;
			ftManagerDestroyFighter(fighter_gobj);
		}
		else rot_y = F_CST_DTOR32(0.0F);

		desc.fkind = fkind;
		sMNPlayers1PGameSlot.costume = desc.costume = costume;
		desc.shade = 0;
		desc.figatree_heap = sMNPlayers1PGameFigatreeHeap;
		desc.player = player;
		sMNPlayers1PGameSlot.player = fighter_gobj = ftManagerMakeFighter(&desc);

		gcAddGObjProcess(fighter_gobj, mnPlayers1PGameFighterProcUpdate, nGCProcessKindFunc, 1);

		DObjGetStruct(fighter_gobj)->translate.vec.f.x = -1100.0F;
		DObjGetStruct(fighter_gobj)->translate.vec.f.y = -850.0F;

		DObjGetStruct(fighter_gobj)->rotate.vec.f.y = rot_y;

		DObjGetStruct(fighter_gobj)->scale.vec.f.x = dSCSubsysFighterScales[fkind];
		DObjGetStruct(fighter_gobj)->scale.vec.f.y = dSCSubsysFighterScales[fkind];
		DObjGetStruct(fighter_gobj)->scale.vec.f.z = dSCSubsysFighterScales[fkind];
	}
}

// 0x801351CC
void mnPlayers1PGameMakeFighterCamera(void)
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
			COBJ_MASK_DLLINK(18) | COBJ_MASK_DLLINK(15) |
			COBJ_MASK_DLLINK(10) | COBJ_MASK_DLLINK(9),
			~0,
			TRUE,
			nGCProcessKindFunc,
			NULL,
			1,
			FALSE
		)
	);
	syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

	cobj->vec.eye.x = 0.0F;
	cobj->vec.eye.y = 0.0F;
	cobj->vec.eye.z = 5000.0F;

	cobj->flags = COBJ_FLAG_DLBUFFERS;

	cobj->vec.at.x = 0.0F;
	cobj->vec.at.y = 0.0F;
	cobj->vec.at.z = 0.0F;

	cobj->vec.up.x = 0.0F;
	cobj->vec.up.z = 0.0F;
	cobj->vec.up.y = 1.0F;
}

// 0x801352BC
void mnPlayers1PGameUpdateCursor(GObj *gobj, s32 player, s32 cursor_status)
{
	SObj *sobj;
	f32 start_pos_x, start_pos_y;

	SYColorRGBPair colors[/* */] =
	{
		{ { 0xE0, 0x15, 0x15 }, { 0x5B, 0x00, 0x00 } },
		{ { 0x00, 0x00, 0xFB }, { 0x00, 0x00, 0x52 } },
		{ { 0xCA, 0x94, 0x08 }, { 0x62, 0x3C, 0x00 } },
		{ { 0x00, 0x91, 0x00 }, { 0x00, 0x4F, 0x00 } }
	};
	intptr_t num_offsets[/* */] =
	{
		&llMNPlayersCommon1PTextGradientSprite,
		&llMNPlayersCommon2PTextGradientSprite,
		&llMNPlayersCommon3PTextGradientSprite,
		&llMNPlayersCommon4PTextGradientSprite
	};
	intptr_t cursor_offsets[/* */] =
	{
		&llMNPlayersCommonCursorHandPointSprite,
		&llMNPlayersCommonCursorHandGrabSprite,
		&llMNPlayersCommonCursorHandHoverSprite
	};
	Vec2i pos[/* */] =
	{
		{ 7, 15 },
		{ 9, 10 },
		{ 9, 15 }
	};

	start_pos_x = SObjGetStruct(gobj)->pos.x;
	start_pos_y = SObjGetStruct(gobj)->pos.y;

	gcRemoveSObjAll(gobj);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], cursor_offsets[cursor_status]));
	sobj->pos.x = start_pos_x;
	sobj->pos.y = start_pos_y;
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], num_offsets[player]));
	sobj->pos.x = sobj->prev->pos.x + pos[cursor_status].x;
	sobj->pos.y = sobj->prev->pos.y + pos[cursor_status].y;

	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;

	sobj->sprite.red = colors[player].prim.r;
	sobj->sprite.green = colors[player].prim.g;
	sobj->sprite.blue = colors[player].prim.b;

	sobj->envcolor.r = colors[player].env.r;
	sobj->envcolor.g = colors[player].env.g;
	sobj->envcolor.b = colors[player].env.b;
}

// 0x801354CC
sb32 mnPlayers1PGameCheckTimeArrowRInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	sb32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_y = sobj->pos.y + 3.0F;

	is_in_range = ((pos_y < 12.0F) || (pos_y > 35.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		return FALSE;
	}
	pos_x = sobj->pos.x + 20.0F;

	is_in_range = ((pos_x >= 210.0F) && (pos_x <= 230.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		return TRUE;
	}
	else return FALSE;
}

// 0x80135594
sb32 mnPlayers1PGameCheckTimeArrowLInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	sb32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_y = sobj->pos.y + 3.0F;

	is_in_range = ((pos_y < 12.0F) || (pos_y > 35.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		return FALSE;
	}
	pos_x = sobj->pos.x + 20.0F;

	is_in_range = ((pos_x >= 140.0F) && (pos_x <= 160.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		return TRUE;
	}
	else return FALSE;
}

// 0x8013565C
sb32 mnPlayers1PGameCheckBackInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	sb32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_y = sobj->pos.y + 3.0F;

	is_in_range = ((pos_y < 13.0F) || (pos_y > 34.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		return FALSE;
	}
	pos_x = sobj->pos.x + 20.0F;

	is_in_range = ((pos_x >= 244.0F) && (pos_x <= 292.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		return TRUE;
	}
	else return FALSE;
}

// 0x80135724
sb32 mnPlayers1PGameCheckPuckInRange(GObj *gobj, s32 cursor_player, s32 player)
{
	f32 pos_x, pos_y;
	sb32 is_in_range;
	SObj *sobj = SObjGetStruct(gobj);
	SObj *puck_sobj = SObjGetStruct(sMNPlayers1PGameSlot.puck);

	pos_x = sobj->pos.x + 25.0F;
	is_in_range = ((pos_x >= puck_sobj->pos.x) && (pos_x <= puck_sobj->pos.x + 26.0F)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		pos_y = sobj->pos.y + 3.0F;
		is_in_range = ((pos_y >= puck_sobj->pos.y) && (pos_y <= puck_sobj->pos.y + 24.0F)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 0x801357FC - Unused?
void func_ovl27_801357FC(void)
{
	return;
}

// 0x80135804
void mnPlayers1PGameUpdateFighter(s32 player)
{
	s32 is_skip_fighter = FALSE;

	if ((sMNPlayers1PGameSlot.fkind == nFTKindNull) && (sMNPlayers1PGameSlot.is_selected == FALSE))
	{
		sMNPlayers1PGameSlot.player->flags = GOBJ_FLAG_HIDDEN;
		mnPlayers1PGameMakeStock(sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);
		mnPlayers1PGameMakeFighterRecord();
		is_skip_fighter = TRUE;
	}
	if (is_skip_fighter == FALSE)
	{
		mnPlayers1PGameMakeFighter(sMNPlayers1PGameSlot.player, player, sMNPlayers1PGameSlot.fkind, mnPlayers1PGameGetFreeCostume(sMNPlayers1PGameSlot.fkind, 0));
		mnPlayers1PGameMakeStock(sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);
		mnPlayers1PGameMakeFighterRecord();
		sMNPlayers1PGameSlot.player->flags = GOBJ_FLAG_NONE;
		sMNPlayers1PGameSlot.is_status_selected = FALSE;
	}
}

// 0x801358BC - Unused?
void func_ovl27_801358BC(void)
{
	return;
}

// 0x801358C4
void mnPlayers1PGameUpdateNameAndEmblem(s32 player)
{
	if ((sMNPlayers1PGameSlot.fkind == nFTKindNull) && (sMNPlayers1PGameSlot.is_selected == FALSE))
	{
		sMNPlayers1PGameSlot.name_emblem_gobj->flags = GOBJ_FLAG_HIDDEN;
	}
	else
	{
		sMNPlayers1PGameSlot.name_emblem_gobj->flags = GOBJ_FLAG_NONE;
		mnPlayers1PGameMakeNameAndEmblem(sMNPlayers1PGameSlot.name_emblem_gobj, player, sMNPlayers1PGameSlot.fkind);
	}
}

// 0x80135924
void mnPlayers1PGameDestroyPortraitFlash(s32 player)
{
	GObj *gobj = sMNPlayers1PGameSlot.flash;

	if (gobj != NULL)
	{
		sMNPlayers1PGameSlot.flash = NULL;
		gcEjectGObj(gobj);
	}
}

// 0x8013595C
void mnPlayers1PGamePortraitFlashThreadUpdate(GObj *gobj)
{
	s32 length = 16;
	s32 wait_tics = 1;

	while (TRUE)
	{
		length--, wait_tics--;

		if (length == 0)
		{
			mnPlayers1PGameDestroyPortraitFlash(gobj->user_data.s);
		}
		if (wait_tics == 0)
		{
			wait_tics = 1;
			gobj->flags = (gobj->flags == GOBJ_FLAG_HIDDEN) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;
		}
		gcSleepCurrentGObjThread(1);
	}
}

// 0x801359FC
void mnPlayers1PGameMakePortraitFlash(s32 player)
{
	GObj *gobj;
	SObj *sobj;
	s32 portrait = mnPlayers1PGameGetPortrait(sMNPlayers1PGameSlot.fkind);

	mnPlayers1PGameDestroyPortraitFlash(player);

	sMNPlayers1PGameSlot.flash = gobj = gcMakeGObjSPAfter(0, NULL, 26, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 33, GOBJ_PRIORITY_DEFAULT, ~0);
	gobj->user_data.s = player;
	gcAddGObjProcess(gobj, mnPlayers1PGamePortraitFlashThreadUpdate, nGCProcessKindThread, 1);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[4], &llMNPlayersPortraitsWhiteSquareSprite));
	sobj->pos.x = (((portrait >= 6) ? portrait - 6 : portrait) * 45) + 26;
	sobj->pos.y = (((portrait >= 6) ? 1 : 0) * 43) + 37;
}

// 0x80135B30
void mnPlayers1PGameAnnounceFighter(s32 player, s32 slot)
{
	u16 announce_names[/* */] =
	{
		nSYAudioVoiceAnnounceMario,
		nSYAudioVoiceAnnounceFox,
		nSYAudioVoiceAnnounceDonkey,
		nSYAudioVoiceAnnounceSamus,
		nSYAudioVoiceAnnounceLuigi,
		nSYAudioVoiceAnnounceLink,
		nSYAudioVoiceAnnounceYoshi,
		nSYAudioVoiceAnnounceCaptain,
		nSYAudioVoiceAnnounceKirby,
		nSYAudioVoiceAnnouncePikachu,
		nSYAudioVoiceAnnouncePurin,
		nSYAudioVoiceAnnounceNess
	};

	if (sMNPlayers1PGameSlot.p_sfx != NULL)
	{
		if ((sMNPlayers1PGameSlot.p_sfx->sfx_id != 0) && (sMNPlayers1PGameSlot.p_sfx->sfx_id == sMNPlayers1PGameSlot.sfx_id))
		{
			func_80026738_27338(sMNPlayers1PGameSlot.p_sfx);
		}
	}
	func_800269C0_275C0(nSYAudioFGMMarioDash);

	sMNPlayers1PGameSlot.p_sfx = func_800269C0_275C0(announce_names[sMNPlayers1PGameSlot.fkind]);

	if (sMNPlayers1PGameSlot.p_sfx != NULL)
	{
		sMNPlayers1PGameSlot.sfx_id = sMNPlayers1PGameSlot.p_sfx->sfx_id;
	}
}

// 0x80135BFC - Unused?
void func_ovl27_80135BFC(void)
{
	return;
}

// 0x80135C04
sb32 mnPlayers1PGameCheckSelectFighter(GObj *gobj, s32 player, s32 unused, s32 select_button)
{
	if (sMNPlayers1PGameSlot.cursor_status != nMNPlayersCursorStatusGrab)
	{
		return FALSE;
	}
	else if (sMNPlayers1PGameSlot.fkind != nFTKindNull)
	{
		mnPlayers1PGameSelectFighterPuck(player, select_button);
		sMNPlayers1PGameSlot.recall_end_tic = sMNPlayers1PGameTotalTimeTics + 30;

		return TRUE;
	}
	else func_800269C0_275C0(nSYAudioFGMMenuDenied);

	return FALSE;
}

// 0x80135C88
void mnPlayers1PGameUpdateCursorGrabPriorities(s32 player, s32 puck)
{
	u32 priorities[/* */] = { 6, 4, 2, 0 };

	gcMoveGObjDL(sMNPlayers1PGameSlot.puck, 30, priorities[player] + 1);
}

// 0x80135CF4
void mnPlayers1PGameUpdateCursorPlacementPriorities(s32 player)
{
	u32 priorities[/* */] = { 3, 2, 1, 0 };

	gcMoveGObjDL(sMNPlayers1PGameSlot.puck, 31, priorities[player]);
}

// 0x80135D58
void mnPlayers1PGameSetCursorPuckOffset(s32 player)
{
	sMNPlayers1PGameSlot.cursor_pickup_x = SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.x - 11.0F;
	sMNPlayers1PGameSlot.cursor_pickup_y = SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.y - -14.0F;
}

// 0x80135D9C
void mnPlayers1PGameSetCursorGrab(s32 player)
{
	sMNPlayers1PGameSlot.holder_player = player;
	sMNPlayers1PGameSlot.is_selected = FALSE;
	sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusGrab;
	sMNPlayers1PGameSlot.held_player = player;
	sMNPlayers1PGameSlot.is_fighter_selected = FALSE;

	mnPlayers1PGameUpdateFighter(player);
	mnPlayers1PGameUpdateCursorGrabPriorities(player, player);
	mnPlayers1PGameSetCursorPuckOffset(player);
	mnPlayers1PGameUpdateCursor(sMNPlayers1PGameSlot.cursor, player, sMNPlayers1PGameSlot.cursor_status);

	sMNPlayers1PGameSlot.is_cursor_adjusting = TRUE;

	func_800269C0_275C0(nSYAudioFGMSamusDash);

	mnPlayers1PGameDestroyPortraitFlash(player);
	mnPlayers1PGameUpdateNameAndEmblem(player);
}

// 0x80135E2C
sb32 mnPlayers1PGameCheckCursorPuckGrab(GObj *gobj, s32 player)
{
	MNPlayersSlot1PGame *pslot = &sMNPlayers1PGameSlot;

	if ((sMNPlayers1PGameTotalTimeTics < sMNPlayers1PGameSlot.recall_end_tic) || (sMNPlayers1PGameSlot.is_recalling != FALSE))
	{
		return FALSE;
	}
	else if (sMNPlayers1PGameSlot.cursor_status != nMNPlayersCursorStatusHover)
	{
		return FALSE;
	}
	else if ((sMNPlayers1PGameSlot.holder_player == GMCOMMON_PLAYERS_MAX) && (mnPlayers1PGameCheckPuckInRange(gobj, player, player) != FALSE))
	{
		sMNPlayers1PGameSlot.holder_player = player;
		sMNPlayers1PGameSlot.is_selected = FALSE;
		sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusGrab;
		pslot->held_player = player;
		sMNPlayers1PGameSlot.is_fighter_selected = FALSE;

		mnPlayers1PGameUpdateFighter(player);
		mnPlayers1PGameUpdateCursorGrabPriorities(player, player);
		mnPlayers1PGameSetCursorPuckOffset(player);
		mnPlayers1PGameUpdateCursor(gobj, player, sMNPlayers1PGameSlot.cursor_status);

		sMNPlayers1PGameSlot.is_cursor_adjusting = TRUE;

		func_800269C0_275C0(nSYAudioFGMSamusDash);

		mnPlayers1PGameDestroyPortraitFlash(player);
		mnPlayers1PGameUpdateNameAndEmblem(player);

		return TRUE;
	}
	else return FALSE;
}

// 0x80135F34
s32 mnPlayers1PGameGetForcePuckFighterKind(void)
{
	SObj *sobj = SObjGetStruct(sMNPlayers1PGameSlot.puck);
	s32 pos_x = (s32) sobj->pos.x + 13;
	s32 pos_y = (s32) sobj->pos.y + 12;
	s32 fkind;
	sb32 is_in_range = ((pos_y > 35) && (pos_y < 79)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		is_in_range = ((pos_x > 24) && (pos_x < 295)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return mnPlayers1PGameGetFighterKind((pos_x - 25) / 45);
		}
	}
	is_in_range = ((pos_y > 78) && (pos_y < 122)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		is_in_range = ((pos_x > 24) && (pos_x < 295)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return mnPlayers1PGameGetFighterKind(((pos_x - 25) / 45) + 6);
		}
	}
	return nFTKindNull;
}

// 0x80136050
s32 mnPlayers1PGameGetPuckFighterKind(s32 player)
{
	SObj *sobj = SObjGetStruct(sMNPlayers1PGameSlot.puck);
	s32 pos_x = (s32) sobj->pos.x + 13;
	s32 pos_y = (s32) sobj->pos.y + 12;
	s32 fkind;
	sb32 is_in_range = ((pos_y > 35) && (pos_y < 79)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		is_in_range = ((pos_x > 24) && (pos_x < 295)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			fkind = mnPlayers1PGameGetFighterKind((pos_x - 25) / 45);

			if ((mnPlayers1PGameCheckFighterCrossed(fkind) != FALSE) || (mnPlayers1PGameCheckFighterLocked(fkind) != FALSE))
			{
				return nFTKindNull;
			}
			else return fkind;
		}
	}
	is_in_range = ((pos_y > 78) && (pos_y < 122)) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		is_in_range = ((pos_x > 24) && (pos_x < 295)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			fkind = mnPlayers1PGameGetFighterKind(((pos_x - 25) / 45) + 6);

			if ((mnPlayers1PGameCheckFighterCrossed(fkind) != FALSE) || (mnPlayers1PGameCheckFighterLocked(fkind) != FALSE))
			{
				return nFTKindNull;
			}
			else return fkind;
		}
	}
	return nFTKindNull;
}

// 0x801361C8
void mnPlayers1PGameAdjustCursor(GObj *gobj, s32 player)
{
	s32 unused;
	Vec2i pos[/* */] =
	{
		{ 7, 15 },
		{ 9, 10 },
		{ 9, 15 }
	};
	f32 delta;
	sb32 is_in_range;

	if (sMNPlayers1PGameSlot.is_cursor_adjusting != FALSE)
	{
		delta = (sMNPlayers1PGameSlot.cursor_pickup_x - SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.x) / 5.0F;
		is_in_range = ((delta >= -1.0F) && (delta <= 1.0F)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.x = sMNPlayers1PGameSlot.cursor_pickup_x;
		}
		else SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.x += delta;

		delta = (sMNPlayers1PGameSlot.cursor_pickup_y - SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.y) / 5.0F;
		is_in_range = ((delta >= -1.0F) && (delta <= 1.0F)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.y = sMNPlayers1PGameSlot.cursor_pickup_y;
		}
		else SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.y += delta;

		if
		(
			(SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.x == sMNPlayers1PGameSlot.cursor_pickup_x) &&
			(SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.y == sMNPlayers1PGameSlot.cursor_pickup_y)
		)
		{
			sMNPlayers1PGameSlot.is_cursor_adjusting = FALSE;
		}
		SObjGetStruct(gobj)->next->pos.x = SObjGetStruct(gobj)->pos.x + pos[sMNPlayers1PGameSlot.cursor_status].x;
		SObjGetStruct(gobj)->next->pos.y = SObjGetStruct(gobj)->pos.y + pos[sMNPlayers1PGameSlot.cursor_status].y;
	}
	else if (sMNPlayers1PGameSlot.is_recalling == FALSE)
	{
		is_in_range = ((gSYControllerDevices[player].stick_range.x < -8) || (gSYControllerDevices[player].stick_range.x > 8)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			delta = (gSYControllerDevices[player].stick_range.x / 20.0F) + SObjGetStruct(gobj)->pos.x;
			is_in_range = ((delta >= 0.0F) && (delta <= 280.0F)) ? TRUE : FALSE;

			if (is_in_range != FALSE)
			{
				SObjGetStruct(gobj)->pos.x = delta;
				SObjGetStruct(gobj)->next->pos.x = SObjGetStruct(gobj)->pos.x + pos[sMNPlayers1PGameSlot.cursor_status].x;
			}
		}
		is_in_range = ((gSYControllerDevices[player].stick_range.y < -8) || (gSYControllerDevices[player].stick_range.y > 8)) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			delta = (gSYControllerDevices[player].stick_range.y / -20.0F) + SObjGetStruct(gobj)->pos.y;
			is_in_range = ((delta >= 10.0F) && (delta <= 205.0F)) ? TRUE : FALSE;

			if (is_in_range != FALSE)
			{
				SObjGetStruct(gobj)->pos.y = delta;
				SObjGetStruct(gobj)->next->pos.y = SObjGetStruct(gobj)->pos.y + pos[sMNPlayers1PGameSlot.cursor_status].y;
			}
		}
	}
}

// 0x80136540
void mnPlayers1PGameUpdateCursorNoRecall(GObj *gobj, s32 player)
{
	s32 i;

	if ((SObjGetStruct(gobj)->pos.y > 124.0F) || (SObjGetStruct(gobj)->pos.y < 38.0F))
	{
		if (sMNPlayers1PGameSlot.cursor_status != nMNPlayersCursorStatusPointer)
		{
			mnPlayers1PGameUpdateCursor(gobj, player, nMNPlayersCursorStatusPointer);
			sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusPointer;
		}
	}
	else if (sMNPlayers1PGameSlot.held_player == -1)
	{
		if (sMNPlayers1PGameSlot.cursor_status != nMNPlayersCursorStatusHover)
		{
			mnPlayers1PGameUpdateCursor(gobj, player, nMNPlayersCursorStatusHover);
			sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusHover;
		}
	}
	else if (sMNPlayers1PGameSlot.cursor_status != nMNPlayersCursorStatusGrab)
	{
		mnPlayers1PGameUpdateCursor(gobj, player, nMNPlayersCursorStatusGrab);
		sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusGrab;
	}
	if ((sMNPlayers1PGameSlot.cursor_status == nMNPlayersCursorStatusPointer) && (sMNPlayers1PGameSlot.is_selected != FALSE))
	{
		for (i = 0; i < GMCOMMON_PLAYERS_MAX; i++)
		{
			if ((sMNPlayers1PGameSlot.is_selected == TRUE) && (mnPlayers1PGameCheckPuckInRange(gobj, player, i) != FALSE))
			{
				mnPlayers1PGameUpdateCursor(gobj, player, nMNPlayersCursorStatusHover);
				sMNPlayers1PGameSlot.cursor_status = nMNPlayersCursorStatusHover;

				break;
			}
		}
	}
}

// 0x801366C4
sb32 mnPlayers1PGameCheckLevelArrowRInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	s32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_x = sobj->pos.x + 20.0F;

	is_in_range = (pos_x >= 258.0F) && (pos_x <= 280.0F) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		pos_y = sobj->pos.y + 3.0F;

		is_in_range = (pos_y >= 155.0F) && (pos_y <= 174.0F) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 0x80136788
sb32 mnPlayers1PGameCheckLevelArrowLInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	s32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_x = sobj->pos.x + 20.0F;

	is_in_range = (pos_x >= 190.0F) && (pos_x <= 212.0F) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		pos_y = sobj->pos.y + 3.0F;

		is_in_range = (pos_y >= 155.0F) && (pos_y <= 174.0F) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 0x8013684C
sb32 mnPlayers1PGameCheckLevelArrowPress(GObj *gobj)
{
	if (mnPlayers1PGameCheckLevelArrowRInRange(gobj) != FALSE)
	{
		if (sMNPlayers1PGameLevelValue < nSC1PGameDifficultyVeryHard)
		{
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);

			mnPlayers1PGameMakeLevel(++sMNPlayers1PGameLevelValue);
		}
		return TRUE;
	}
	else if (mnPlayers1PGameCheckLevelArrowLInRange(gobj) != FALSE)
	{
		if (sMNPlayers1PGameLevelValue > nSC1PGameDifficultyVeryEasy)
		{
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);

			mnPlayers1PGameMakeLevel(--sMNPlayers1PGameLevelValue);
		}
		return TRUE;
	}
	else return FALSE;
}

// 0x801368FC
sb32 mnPlayers1PGameCheckStockArrowRInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	s32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_x = sobj->pos.x + 20.0F;

	is_in_range = (pos_x >= 258.0F) && (pos_x <= 280.0F) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		pos_y = sobj->pos.y + 3.0F;

		is_in_range = (pos_y >= 175.0F) && (pos_y <= 194.0F) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 0x801369C0
sb32 mnPlayers1PGameCheckStockArrowLInRange(GObj *gobj)
{
	f32 pos_x, pos_y;
	s32 is_in_range;
	SObj *sobj;

	sobj = SObjGetStruct(gobj);

	pos_x = sobj->pos.x + 20.0F;

	is_in_range = (pos_x >= 190.0F) && (pos_x <= 212.0F) ? TRUE : FALSE;

	if (is_in_range != FALSE)
	{
		pos_y = sobj->pos.y + 3.0F;

		is_in_range = (pos_y >= 175.0F) && (pos_y <= 194.0F) ? TRUE : FALSE;

		if (is_in_range != FALSE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 0x80136A84
sb32 mnPlayers1PGameCheckStockArrowPress(GObj *gobj)
{
	if (mnPlayers1PGameCheckStockArrowRInRange(gobj))
	{
		if (sMNPlayers1PGameStockValue < 4)
		{
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);

			mnPlayers1PGameMakeStock(++sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);
		}
		return TRUE;
	}
	else if (mnPlayers1PGameCheckStockArrowLInRange(gobj))
	{
		if (sMNPlayers1PGameStockValue > 0)
		{
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);

			mnPlayers1PGameMakeStock(--sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);
		}
		return TRUE;
	}
	else return FALSE;
}

// 0x80136B44
void mnPlayers1PGameUpdateCostume(s32 player, s32 select_button)
{
	s32 costume = ftParamGetCostumeCommonID(sMNPlayers1PGameSlot.fkind, select_button);

	ftParamInitAllParts(sMNPlayers1PGameSlot.player, costume, 0);

	sMNPlayers1PGameSlot.costume = costume;

	mnPlayers1PGameMakeStock(sMNPlayers1PGameStockValue, sMNPlayers1PGameSlot.fkind);

	func_800269C0_275C0(nSYAudioFGMMenuScroll2);
}

// 0x80136BAC
sb32 mnPlayers1PGameCheckManFighterSelected(s32 player)
{
	if (sMNPlayers1PGameSlot.is_selected)
	{
		return TRUE;
	}
	else return FALSE;
}

// 0x80136BD4
void mnPlayers1PGameRecallPuck(s32 player)
{
	sMNPlayers1PGameSlot.is_fighter_selected = FALSE;
	sMNPlayers1PGameSlot.is_selected = FALSE;
	sMNPlayers1PGameSlot.is_recalling = TRUE;
	sMNPlayers1PGameSlot.recall_tics = 0;
	sMNPlayers1PGameSlot.recall_start_x = SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.x;
	sMNPlayers1PGameSlot.recall_start_y = SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.y;

	sMNPlayers1PGameSlot.recall_end_x = SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.x + 20.0F;

	if (sMNPlayers1PGameSlot.recall_end_x > 280.0F)
	{
		sMNPlayers1PGameSlot.recall_end_x = 280.0F;
	}
	sMNPlayers1PGameSlot.recall_end_y = SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.y + -15.0F;

	if (sMNPlayers1PGameSlot.recall_end_y < 10.0F)
	{
		sMNPlayers1PGameSlot.recall_end_y = 10.0F;
	}
	if (sMNPlayers1PGameSlot.recall_end_y < sMNPlayers1PGameSlot.recall_start_y)
	{
		sMNPlayers1PGameSlot.recall_mid_y = sMNPlayers1PGameSlot.recall_end_y - 20.0F;
	}
	else sMNPlayers1PGameSlot.recall_mid_y = sMNPlayers1PGameSlot.recall_start_y - 20.0F;
}

// 0x80136CB8
void mnPlayers1PGameBackTo1PMode(void)
{
	gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
	gSCManagerSceneData.scene_curr = nSCKind1PMode;

	mnPlayers1PGameSetSceneData();
	syAudioStopBGMAll();
	func_800266A0_272A0();
	syTaskmanSetLoadScene();
}

// 0x80136D04
void mnPlayers1PGameDetectBack(s32 player)
{
	if ((sMNPlayers1PGameTotalTimeTics >= 10) && (gSYControllerDevices[player].button_tap & B_BUTTON))
	{
		mnPlayers1PGameBackTo1PMode();
	}
}

// 0x80136D58
void mnPlayers1PGameCursorProcUpdate(GObj *gobj)
{
	s32 unused[5];
	s32 player = gobj->user_data.s;

	mnPlayers1PGameAdjustCursor(gobj, player);

	if
	(
		(gSYControllerDevices[player].button_tap & A_BUTTON) &&
		(mnPlayers1PGameCheckSelectFighter(gobj, player, sMNPlayers1PGameSlot.held_player, 0) == FALSE) &&
		(mnPlayers1PGameCheckCursorPuckGrab(gobj, player) == FALSE)
	)
	{
		if (mnPlayers1PGameCheckTimeArrowRInRange(gobj) != FALSE)
		{
			sMNPlayers1PGameTimeSetting = mnPlayers1PGameGetNextTimeValue(sMNPlayers1PGameTimeSetting);
			mnPlayers1PGameMakeTimeSelect(sMNPlayers1PGameTimeSetting);
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);
		}
		else if (mnPlayers1PGameCheckTimeArrowLInRange(gobj) != FALSE)
		{
			sMNPlayers1PGameTimeSetting = mnPlayers1PGameGetPrevTimeValue(sMNPlayers1PGameTimeSetting);
			mnPlayers1PGameMakeTimeSelect(sMNPlayers1PGameTimeSetting);
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);
		}
		else if (mnPlayers1PGameCheckBackInRange(gobj) != FALSE)
		{
			mnPlayers1PGameBackTo1PMode();
			func_800269C0_275C0(nSYAudioFGMMenuScroll2);
		}
		else if (mnPlayers1PGameCheckLevelArrowPress(gobj) == FALSE)
		{
			mnPlayers1PGameCheckStockArrowPress(gobj);
		}
	}
	if
	(
		(gSYControllerDevices[player].button_tap & U_CBUTTONS) &&
		(mnPlayers1PGameCheckSelectFighter(gobj, player, sMNPlayers1PGameSlot.held_player, 0) == FALSE) &&
		(sMNPlayers1PGameSlot.is_fighter_selected != FALSE)
	)
	{
		mnPlayers1PGameUpdateCostume(player, 0);
	}
	if
	(
		(gSYControllerDevices[player].button_tap & R_CBUTTONS) &&
		(mnPlayers1PGameCheckSelectFighter(gobj, player, sMNPlayers1PGameSlot.held_player, 1) == FALSE) &&
		(sMNPlayers1PGameSlot.is_fighter_selected != FALSE)
	)
	{
		mnPlayers1PGameUpdateCostume(player, 1);
	}
	if
	(
		(gSYControllerDevices[player].button_tap & D_CBUTTONS) &&
		(mnPlayers1PGameCheckSelectFighter(gobj, player, sMNPlayers1PGameSlot.held_player, 2) == FALSE) &&
		(sMNPlayers1PGameSlot.is_fighter_selected != FALSE)
	)
	{
		mnPlayers1PGameUpdateCostume(player, 2);
	}
	if
	(
		(gSYControllerDevices[player].button_tap & L_CBUTTONS) &&
		(mnPlayers1PGameCheckSelectFighter(gobj, player, sMNPlayers1PGameSlot.held_player, 3) == FALSE) &&
		(sMNPlayers1PGameSlot.is_fighter_selected != FALSE)
	)
	{
		mnPlayers1PGameUpdateCostume(player, 3);
	}
	if ((gSYControllerDevices[player].button_tap & B_BUTTON) && (mnPlayers1PGameCheckManFighterSelected(player) != FALSE))
	{
		mnPlayers1PGameRecallPuck(player);
	}
	if (sMNPlayers1PGameSlot.is_recalling == FALSE)
	{
		mnPlayers1PGameDetectBack(player);
	}
	if (sMNPlayers1PGameSlot.is_recalling == FALSE)
	{
		mnPlayers1PGameUpdateCursorNoRecall(gobj, player);
	}
}

// 0x80138C0C
intptr_t dMNPlayers1PGamePuckSpriteOffsets[/* */] =
{
	&llMNPlayersCommon1PPuckSprite,
	&llMNPlayersCommon2PPuckSprite,
	&llMNPlayersCommon3PPuckSprite,
	&llMNPlayersCommon4PPuckSprite,
	&llMNPlayersCommonCPPuckSprite
};

// 0x8013702C - Unused?
void func_ovl27_8013702C(void)
{
	return;
}

// 0x80137034
void mnPlayers1PGameCenterPuckInPortrait(GObj *gobj, s32 fkind)
{
	s32 portrait = mnPlayers1PGameGetPortrait(fkind);

	if (portrait >= 6)
	{
		SObjGetStruct(gobj)->pos.x = (portrait * 45) - (6 * 45) + 36;
		SObjGetStruct(gobj)->pos.y = 89.0F;
	}
	else
	{
		SObjGetStruct(gobj)->pos.x = (portrait * 45) + 36;
		SObjGetStruct(gobj)->pos.y = 46.0F;
	}
}

// 0x801370E4 - Unused?
void func_ovl27_801370E4(void)
{
	return;
}

// 0x801370EC
void mnPlayers1PGameMovePuck(s32 player)
{
	SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.x += sMNPlayers1PGameSlot.puck_vel_x;
	SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.y += sMNPlayers1PGameSlot.puck_vel_y;
}

// 0x8013712C
void mnPlayers1PGamePuckProcUpdate(GObj *gobj)
{
	s32 fkind;
	s32 player = gobj->user_data.s;

	if (sMNPlayers1PGameTotalTimeTics < 30)
	{
		gobj->flags = GOBJ_FLAG_HIDDEN;
	}
	else if
	(
		(sMNPlayers1PGameSlot.cursor_status != nMNPlayersCursorStatusPointer) ||
		(sMNPlayers1PGameSlot.is_selected == TRUE) ||
		(sMNPlayers1PGameSlot.is_recalling == TRUE)
	)
	{
		gobj->flags = GOBJ_FLAG_NONE;
	}
	else gobj->flags = GOBJ_FLAG_HIDDEN;

	if ((sMNPlayers1PGameSlot.is_selected == FALSE) && (sMNPlayers1PGameSlot.holder_player != GMCOMMON_PLAYERS_MAX))
	{
		if (sMNPlayers1PGameSlot.is_cursor_adjusting == FALSE)
		{
			SObjGetStruct(gobj)->pos.x = SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.x + 11.0F;
			SObjGetStruct(gobj)->pos.y = SObjGetStruct(sMNPlayers1PGameSlot.cursor)->pos.y + -14.0F;
		}
	}
	else mnPlayers1PGameMovePuck(player);

	fkind = mnPlayers1PGameGetPuckFighterKind(player);

	if ((sMNPlayers1PGameSlot.is_selected == FALSE) && (fkind != sMNPlayers1PGameSlot.fkind))
	{
		sMNPlayers1PGameSlot.fkind = fkind;

		mnPlayers1PGameUpdateFighter(player);
		mnPlayers1PGameUpdateNameAndEmblem(player);
	}
	mnPlayers1PGameMakeFighterRecord();
}

// 0x80137268
void mnPlayers1PGameMakeCursorCamera(void)
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
			13,
			COBJ_MASK_DLLINK(30),
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

// 0x80137308
void mnPlayers1PGameMakePuckCamera(void)
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
			15,
			COBJ_MASK_DLLINK(31),
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

// 0x801373A8
void mnPlayers1PGameMakeReadyCamera(void)
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
			10,
			COBJ_MASK_DLLINK(35),
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

// 0x80137448
void mnPlayers1PGameMakeCursor(s32 player)
{
	GObj *gobj;
	s32 unused;

	// ???
	intptr_t unused_offsets[/* */] =
	{
		&llMNPlayersCommon1PTextGradientSprite,
		&llMNPlayersCommon2PTextGradientSprite,
		&llMNPlayersCommon3PTextGradientSprite,
		&llMNPlayersCommon4PTextGradientSprite
	};
	u32 priorities[/* */] = { 6, 4, 2, 0 };

	gobj = lbCommonMakeSpriteGObj
	(
		0,
		NULL,
		19,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSObjAttr,
		30,
		priorities[player],
		~0,
		lbRelocGetFileData
		(
			Sprite*,
			sMNPlayers1PGameFiles[0],
			&llMNPlayersCommonCursorHandGrabSprite
		),
		nGCProcessKindFunc,
		mnPlayers1PGameCursorProcUpdate,
		2
	);
	gobj->user_data.s = player;
	sMNPlayers1PGameSlot.cursor = gobj;
	
	SObjGetStruct(gobj)->pos.x = 60.0F;
	SObjGetStruct(gobj)->pos.y = 170.0F;
	SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
	SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;

	mnPlayers1PGameUpdateCursor(gobj, player, nMNPlayersCursorStatusPointer);
}

// 0x80137590
void mnPlayers1PGameMakePuck(s32 player)
{
	GObj *gobj;
	MNPlayersSlot1PGame *pslot;

	intptr_t offsets[/* */] =
	{
		&llMNPlayersCommon1PPuckSprite,
		&llMNPlayersCommon2PPuckSprite,
		&llMNPlayersCommon3PPuckSprite,
		&llMNPlayersCommon4PPuckSprite
	};

	u32 priorities[/* */] = { 3, 2, 1, 0 };
	s32 unused;

	gobj = lbCommonMakeSpriteGObj
	(
		0,
		NULL,
		20,
		GOBJ_PRIORITY_DEFAULT,
		lbCommonDrawSObjAttr,
		31,
		priorities[player],
		~0,
		lbRelocGetFileData
		(
			Sprite*,
			sMNPlayers1PGameFiles[0],
			offsets[player]
		),
		nGCProcessKindFunc,
		mnPlayers1PGamePuckProcUpdate,
		1
	);
	gobj->user_data.s = player;
	sMNPlayers1PGameSlot.puck = gobj;

	if (sMNPlayers1PGameSlot.fkind == nFTKindNull)
	{
		SObjGetStruct(gobj)->pos.x = 51.0F;
		SObjGetStruct(gobj)->pos.y = 161.0F;
	}
	else mnPlayers1PGameCenterPuckInPortrait(gobj, sMNPlayers1PGameSlot.fkind);

	SObjGetStruct(gobj)->sprite.attr &= ~SP_FASTCOPY;
	SObjGetStruct(gobj)->sprite.attr |= SP_TRANSPARENT;
}

// 0x801376F0 - Unused?
void func_ovl27_801376F0(void)
{
	return;
}

// 0x801376F8
void mnPlayers1PGamePuckAdjustPortraitEdge(s32 player)
{
	s32 portrait = mnPlayers1PGameGetPortrait(sMNPlayers1PGameSlot.fkind);
	f32 portrait_edge_x = ((portrait >= 6) ? portrait - 6 : portrait) * 45 + 25;
	f32 portrait_edge_y = ((portrait >= 6) ? 1 : 0) * 43 + 36;
	f32 new_pos_x = SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.x + sMNPlayers1PGameSlot.puck_vel_x + 13.0F;
	f32 new_pos_y = SObjGetStruct(sMNPlayers1PGameSlot.puck)->pos.y + sMNPlayers1PGameSlot.puck_vel_y + 12.0F;

	if (new_pos_x < (portrait_edge_x + 5.0F))
	{
		sMNPlayers1PGameSlot.puck_vel_x = ((portrait_edge_x + 5.0F) - new_pos_x) / 10.0F;
	}
	if (((portrait_edge_x + 45.0F) - 5.0F) < new_pos_x)
	{
		sMNPlayers1PGameSlot.puck_vel_x = ((new_pos_x - ((portrait_edge_x + 45.0F) - 5.0F)) * -1.0F) / 10.0F;
	}
	if (new_pos_y < (portrait_edge_y + 5.0F))
	{
		sMNPlayers1PGameSlot.puck_vel_y = ((portrait_edge_y + 5.0F) - new_pos_y) / 10.0F;
	}
	if (((portrait_edge_y + 43.0F) - 5.0F) < new_pos_y)
	{
		sMNPlayers1PGameSlot.puck_vel_y = ((new_pos_y - ((portrait_edge_y + 43.0F) - 5.0F)) * -1.0F) / 10.0F;
	}
}

// 0x801378A8
void mnPlayers1PGamePuckAdjustPlaced(s32 player)
{
	mnPlayers1PGamePuckAdjustPortraitEdge(player);
}

// 0x801378C8
void mnPlayers1PGamePuckAdjustRecall(s32 player)
{
	f32 vel_y, vel_x;

	sMNPlayers1PGameSlot.recall_tics++;

	if (sMNPlayers1PGameSlot.recall_tics < 11)
	{
		vel_x = (sMNPlayers1PGameSlot.recall_end_x - sMNPlayers1PGameSlot.recall_start_x) / 10.0F;

		if (sMNPlayers1PGameSlot.recall_tics < 6)
		{
			vel_y = (sMNPlayers1PGameSlot.recall_mid_y - sMNPlayers1PGameSlot.recall_start_y) / 5.0F;
		}
		else vel_y = (sMNPlayers1PGameSlot.recall_end_y - sMNPlayers1PGameSlot.recall_mid_y) / 5.0F;
		
		sMNPlayers1PGameSlot.puck_vel_x = vel_x;
		sMNPlayers1PGameSlot.puck_vel_y = vel_y;
	}
	else if (sMNPlayers1PGameSlot.recall_tics == 11)
	{
		mnPlayers1PGameSetCursorGrab(player);

		sMNPlayers1PGameSlot.puck_vel_x = 0.0F;
		sMNPlayers1PGameSlot.puck_vel_y = 0.0F;
	}
	if (sMNPlayers1PGameSlot.recall_tics == 30)
	{
		sMNPlayers1PGameSlot.is_recalling = FALSE;
	}
}

// 0x8013799C
void mnPlayers1PGamePuckAdjustProcUpdate(GObj *gobj)
{
	if (sMNPlayers1PGameSlot.is_recalling != FALSE)
	{
		mnPlayers1PGamePuckAdjustRecall(sMNPlayers1PGameManPlayer);
	}
	if (sMNPlayers1PGameSlot.is_selected != FALSE)
	{
		mnPlayers1PGamePuckAdjustPlaced(0);
	}
}

// 0x801379E8
void mnPlayers1PGameMakePuckAdjust(void)
{
	gcAddGObjProcess(gcMakeGObjSPAfter(0, NULL, 24, GOBJ_PRIORITY_DEFAULT), mnPlayers1PGamePuckAdjustProcUpdate, nGCProcessKindFunc, 1);
}

// 0x80137A2C
void mnPlayers1PGameSpotlightProcUpdate(GObj *gobj)
{
	f32 sizes[/* */] =
	{
		1.5F, 1.5F, 2.0F, 1.5F, 1.5F, 1.5F,
		1.5F, 1.5F, 1.5F, 1.5F, 1.5F, 1.5F
	};

	if ((sMNPlayers1PGameSlot.is_fighter_selected == FALSE) && (sMNPlayers1PGameSlot.fkind != nFTKindNull))
	{
		gobj->flags = (gobj->flags == GOBJ_FLAG_HIDDEN) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;

		DObjGetStruct(gobj)->scale.vec.f.x = sizes[sMNPlayers1PGameSlot.fkind];
		DObjGetStruct(gobj)->scale.vec.f.y = sizes[sMNPlayers1PGameSlot.fkind];
		DObjGetStruct(gobj)->scale.vec.f.y = sizes[sMNPlayers1PGameSlot.fkind];
	}
	else gobj->flags = GOBJ_FLAG_HIDDEN;
}

// 0x80137B04
void mnPlayers1PGameMakeSpotlight(void)
{
	GObj *gobj = gcMakeGObjSPAfter(0, NULL, 21, GOBJ_PRIORITY_DEFAULT);

	gcSetupCommonDObjs(gobj, lbRelocGetFileData(DObjDesc*, sMNPlayers1PGameFiles[10], &llMNPlayersSpotlightDObjDesc), NULL);
	gcAddGObjDisplay(gobj, gcDrawDObjTreeDLLinksForGObj, 9, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddMObjAll(gobj, lbRelocGetFileData(MObjSub***, sMNPlayers1PGameFiles[10], &llMNPlayersSpotlightMObjSub));
	gcAddGObjProcess(gobj, mnPlayers1PGameSpotlightProcUpdate, nGCProcessKindFunc, 1);
	gcPlayAnimAll(gobj);

	DObjGetStruct(gobj)->translate.vec.f.x = -1100.0F;
	DObjGetStruct(gobj)->translate.vec.f.y = -850.0F;
	DObjGetStruct(gobj)->translate.vec.f.z = 0.0F;
}

// 0x80137BE4
void mnPlayers1PGameReadyProcUpdate(GObj *gobj)
{
	if (mnPlayers1PGameCheckReady() != FALSE)
	{
		sMNPlayers1PGameReadyBlinkWait++;

		if (sMNPlayers1PGameReadyBlinkWait == 40)
		{
			sMNPlayers1PGameReadyBlinkWait = 0;
		}
		gobj->flags = (sMNPlayers1PGameReadyBlinkWait < 30) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;
	}
	else
	{
		gobj->flags = GOBJ_FLAG_HIDDEN;
		sMNPlayers1PGameReadyBlinkWait = 0;
	}
}

// 0x80137C64
void mnPlayers1PGameMakeReady(void)
{
	GObj *gobj;
	SObj *sobj;

	gobj = gcMakeGObjSPAfter(0, NULL, 28, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 35, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGameReadyProcUpdate, nGCProcessKindFunc, 1);

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonReadyBannerSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->envcolor.r = 0x00;
	sobj->envcolor.g = 0x00;
	sobj->envcolor.b = 0x00;
	sobj->sprite.red = 0xF4;
	sobj->sprite.green = 0x56;
	sobj->sprite.blue = 0x7F;
	sobj->cms = 0;
	sobj->cmt = 0;
	sobj->masks = 3;
	sobj->maskt = 0;
	sobj->lrs = 320;
	sobj->lrt = 17;
	sobj->pos.x = 0.0F;
	sobj->pos.y = 71.0F;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonReadyToFightTextSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->envcolor.r = 0xFF;
	sobj->envcolor.g = 0xCA;
	sobj->envcolor.b = 0x13;
	sobj->sprite.red = 0xFF;
	sobj->sprite.green = 0xFF;
	sobj->sprite.blue = 0x9D;
	sobj->pos.x = 50.0F;
	sobj->pos.y = 76.0F;

	gobj = gcMakeGObjSPAfter(0, NULL, 22, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 28, GOBJ_PRIORITY_DEFAULT, ~0);
	gcAddGObjProcess(gobj, mnPlayers1PGameReadyProcUpdate, nGCProcessKindFunc, 1);

#if defined(REGION_US)
	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonPressTextSprite));
#else
	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonPushTextSprite));
#endif
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0xD6;
	sobj->sprite.green = 0xDD;
	sobj->sprite.blue = 0xC6;
#if defined(REGION_US)
	sobj->pos.x = 133.0F;
#else
	sobj->pos.x = 120.0F;
#endif
	sobj->pos.y = 219.0F;

	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonStartTextSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0xFF;
	sobj->sprite.green = 0x56;
	sobj->sprite.blue = 0x92;
#if defined(REGION_US)
	sobj->pos.x = 162.0F;
#else
	sobj->pos.x = 143.0F;
#endif
	sobj->pos.y = 219.0F;

#if defined(REGION_JP)
	sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNPlayers1PGameFiles[0], &llMNPlayersCommonButtonTextSprite));
	sobj->sprite.attr &= ~SP_FASTCOPY;
	sobj->sprite.attr |= SP_TRANSPARENT;
	sobj->sprite.red = 0xD6;
	sobj->sprite.green = 0xDD;
	sobj->sprite.blue = 0xC6;
	sobj->pos.x = 171.0F;
	sobj->pos.y = 219.0F;
#endif
}

// 0x80137EE0 - Unused?
void func_ovl27_80137EE0(void)
{
	return;
}

// 0x80137EE8 - Unused?
void func_ovl27_80137EE8(void)
{
	return;
}

// 0x80137EF0
sb32 mnPlayers1PGameCheckReady(void)
{
	sb32 is_ready = TRUE;

	if (!sMNPlayers1PGameSlot.is_fighter_selected)
	{
		is_ready = FALSE;
	}
	return is_ready;
}

// 0x80137F10
void mnPlayers1PGameSetSceneData(void)
{
	gSCManagerSceneData.spgame_time_limit = sMNPlayers1PGameTimeSetting;
	gSCManagerSceneData.player = sMNPlayers1PGameManPlayer;
	gSCManagerBackupData.spgame_difficulty = sMNPlayers1PGameLevelValue;
	gSCManagerSceneData.spgame_stage = 0;
	gSCManagerBackupData.spgame_stock_count = sMNPlayers1PGameStockValue;

	if (sMNPlayers1PGameSlot.is_fighter_selected != FALSE)
	{
		gSCManagerSceneData.fkind = sMNPlayers1PGameSlot.fkind;
	}
	else gSCManagerSceneData.fkind = nFTKindNull;

	gSCManagerSceneData.costume = sMNPlayers1PGameSlot.costume;

	lbBackupWrite();
}

// 0x80137F9C
void mnPlayers1PGamePauseSlotProcesses(void)
{
	if (sMNPlayers1PGameSlot.cursor != NULL)
	{
		gcPauseGObjProcess(sMNPlayers1PGameSlot.cursor->gobjproc_head);
	}
}

// 0x80137FCC
void mnPlayers1PGameFuncRun(GObj *gobj)
{
	sMNPlayers1PGameTotalTimeTics++;

	if (sMNPlayers1PGameTotalTimeTics == sMNPlayers1PGameReturnTic)
	{
		gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
		gSCManagerSceneData.scene_curr = nSCKindTitle;

		mnPlayers1PGameSetSceneData();
		syTaskmanSetLoadScene();
	}
	else
	{
		if (scSubsysControllerCheckNoInputAll() == FALSE)
		{
			sMNPlayers1PGameReturnTic = sMNPlayers1PGameTotalTimeTics + I_MIN_TO_TICS(5);
		}
		if (sMNPlayers1PGameIsStart != FALSE)
		{
			sMNPlayers1PGameStartProceedWait--;

			if (sMNPlayers1PGameStartProceedWait == 0)
			{
				gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
				gSCManagerSceneData.scene_curr = nSCKind1PGame;

				mnPlayers1PGameSetSceneData();
				syTaskmanSetLoadScene();
			}
		}
		else if ((scSubsysControllerGetPlayerTapButtons(START_BUTTON)) && (sMNPlayers1PGameTotalTimeTics > 60))
		{
			if (mnPlayers1PGameCheckReady() != FALSE)
			{
				func_800269C0_275C0(nSYAudioVoicePublicCheer);

				sMNPlayers1PGameStartProceedWait = 30;
				sMNPlayers1PGameIsStart = TRUE;

				mnPlayers1PGamePauseSlotProcesses();
			}
			else func_800269C0_275C0(nSYAudioFGMMenuDenied);
		}
	}
}

// 0x80138118
s32 mnPlayers1PGameGetNextTimeValue(s32 value)
{
	return (value == 5) ? SCBATTLE_TIMELIMIT_INFINITE : 5;
}

// 0x80138134
s32 mnPlayers1PGameGetPrevTimeValue(s32 value)
{
	return (value == 5) ? SCBATTLE_TIMELIMIT_INFINITE : 5;
}

// 0x80138150
void mnPlayers1PGameInitPlayer(s32 player)
{
	sMNPlayers1PGameSlot.flash = NULL;
	sMNPlayers1PGameSlot.p_sfx = NULL;
	sMNPlayers1PGameSlot.sfx_id = 0;
	sMNPlayers1PGameSlot.player = NULL;
	sMNPlayers1PGameSlot.fkind = gSCManagerSceneData.fkind;
	sMNPlayers1PGameSlot.costume = gSCManagerSceneData.costume;

	if (sMNPlayers1PGameSlot.fkind == nFTKindNull)
	{
		sMNPlayers1PGameSlot.holder_player = player;
		sMNPlayers1PGameSlot.held_player = player;
		sMNPlayers1PGameSlot.is_fighter_selected = FALSE;
		sMNPlayers1PGameSlot.is_selected = FALSE;
		sMNPlayers1PGameSlot.is_recalling = FALSE;
		sMNPlayers1PGameSlot.is_cursor_adjusting = FALSE;
	}
	else
	{
		sMNPlayers1PGameSlot.holder_player = GMCOMMON_PLAYERS_MAX;
		sMNPlayers1PGameSlot.held_player = -1;
		sMNPlayers1PGameSlot.is_fighter_selected = TRUE;
		sMNPlayers1PGameSlot.is_selected = TRUE;
		sMNPlayers1PGameSlot.is_recalling = FALSE;
		sMNPlayers1PGameSlot.is_cursor_adjusting = FALSE;
	}
}

// 0x801381D0 - Unused?
void func_ovl27_801381D0(void)
{
	return;
}

// 0x801381D8
void mnPlayers1PGameInitVars(void)
{
	sMNPlayers1PGameTotalTimeTics = 0;
	sMNPlayers1PGameReturnTic = sMNPlayers1PGameTotalTimeTics + I_MIN_TO_TICS(5);
	sMNPlayers1PGameIsStart = FALSE;
	sMNPlayers1PGameTimeSetting = gSCManagerSceneData.spgame_time_limit;
	sMNPlayers1PGameManPlayer = gSCManagerSceneData.player;
	sMNPlayers1PGameLevelValue = gSCManagerBackupData.spgame_difficulty;
	sMNPlayers1PGameStockValue = gSCManagerBackupData.spgame_stock_count;
	sMNPlayers1PGameFighterKind = gSCManagerSceneData.fkind;
	sMNPlayers1PGameCostume = gSCManagerSceneData.costume;
	sMNPlayers1PGameHiScoreGObj = NULL;
	sMNPlayers1PGameBonusesGObj = NULL;
	sMNPlayers1PGameLevelGObj = NULL;
	sMNPlayers1PGameStockGObj = NULL;
	sMNPlayers1PGameIsTeamBattle = gSCManager1PGameBattleState.is_team_battle;
	sMNPlayers1PGameRule = gSCManager1PGameBattleState.game_rules;

	mnPlayers1PGameInitPlayer(sMNPlayers1PGameManPlayer);

	sMNPlayers1PGameSlot.recall_end_tic = 0;
	sMNPlayers1PGameFighterMask = gSCManagerBackupData.fighter_mask;
}

// 0x801382C8
void mnPlayers1PGameInitSlot(s32 player)
{
	mnPlayers1PGameMakeCursor(player);
	mnPlayers1PGameMakePuck(player);
	mnPlayers1PGameMakeGate(player);

	if ((sMNPlayers1PGameSlot.is_selected != FALSE) && (sMNPlayers1PGameSlot.fkind != nFTKindNull))
	{
		mnPlayers1PGameMakeFighter(sMNPlayers1PGameSlot.player, player, sMNPlayers1PGameSlot.fkind, sMNPlayers1PGameSlot.costume);
		mnPlayers1PGameMakeFighterRecord();
	}
}

// 0x80138334
void mnPlayers1PGameFuncStart(void)
{
	s32 unused1[2];
	LBRelocSetup rl_setup;
	s32 unused2;
	s32 i, j;

	rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
	rl_setup.table_files_num = (u32)&llRelocFileCount;
	rl_setup.file_heap = NULL;
	rl_setup.file_heap_size = 0;
	rl_setup.status_buffer = sMNPlayers1PGameStatusBuffer;
	rl_setup.status_buffer_size = ARRAY_COUNT(sMNPlayers1PGameStatusBuffer);
	rl_setup.force_status_buffer = sMNPlayers1PGameForceStatusBuffer;
	rl_setup.force_status_buffer_size = ARRAY_COUNT(sMNPlayers1PGameForceStatusBuffer);

	lbRelocInitSetup(&rl_setup);
	lbRelocLoadFilesListed(dMNPlayers1PGameFileIDs, sMNPlayers1PGameFiles);

	gcMakeGObjSPAfter(nGCCommonKindPlayerSelect, mnPlayers1PGameFuncRun, 15, GOBJ_PRIORITY_DEFAULT);
	gcMakeDefaultCameraGObj(16, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));
	efParticleInitAll();
	efManagerInitEffects();
	ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, 1);

	for (i = nFTKindPlayableStart; i <= nFTKindPlayableEnd; i++)
	{
		ftManagerSetupFilesAllKind(i);
	}
	sMNPlayers1PGameFigatreeHeap = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);

	mnPlayers1PGameInitVars();
	mnPlayers1PGameMakePortraitCamera();
	mnPlayers1PGameMakeCursorCamera();
	mnPlayers1PGameMakePuckCamera();
	mnPlayers1PGameMakePlayerKindCamera();
	mnPlayers1PGameMakeFighterCamera();
	mnPlayers1PGameMakePortraitWallpaperCamera();
	mnPlayers1PGameMakePortraitFlashCamera();
	mnPlayers1PGameMakeWallpaperCamera();
	mnPlayers1PGameMakeLabelsCamera();
	mnPlayers1PGameMakeReadyCamera();
	mnPlayers1PGameMakeWallpaper();
	mnPlayers1PGameMakePortraitAll();
	mnPlayers1PGameInitSlot(sMNPlayers1PGameManPlayer);
	mnPlayers1PGameMakeLabels();
	mnPlayers1PGameMakeTotalRecord();
	mnPlayers1PGameMakeLevelOption();
	mnPlayers1PGameMakeStockOption();
	mnPlayers1PGameMakePuckAdjust();
	mnPlayers1PGameMakeSpotlight();
	mnPlayers1PGameMakeReady();
	scSubsysFighterSetLightParams(45.0F, 45.0F, 0xFF, 0xFF, 0xFF, 0xFF);

	if (gSCManagerSceneData.scene_prev != nSCKindMaps)
	{
		syAudioPlayBGM(0, nSYAudioBGMBattleSelect);
	}
	func_800269C0_275C0(nSYAudioVoiceAnnounceSelectPlayer);
}

// 0x80138C90
SYVideoSetup dMNPlayers1PGameVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80138CAC
SYTaskmanSetup dMNPlayers1PGameTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,          		// Frame draw function
        &ovl27_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 2375,         // Display List Buffer 0 Size
        sizeof(Gfx) * 64,          	// Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0x8000,                     // RDP Output Buffer Size
        mnPlayers1PGameFuncLights,  // Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 32,              	// Thread stack size
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
    
    mnPlayers1PGameFuncStart        // Task start function
};

// 0x80138558
void mnPlayers1PGameStartScene(void)
{
	dMNPlayers1PGameVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dMNPlayers1PGameVideoSetup);

	dMNPlayers1PGameTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl27_BSS_END);
	syTaskmanStartTask(&dMNPlayers1PGameTaskmanSetup);
}
