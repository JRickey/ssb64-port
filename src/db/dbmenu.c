#include <db/debug.h>

// ovl8
extern DBMenuPosition* func_ovl8_803749BC(s32);
extern void func_ovl8_80374A54(s32, DBMenu*);
extern void func_ovl8_80377AEC(DBMenuPosition*, DBMenuPosition*, u32, s32);
extern void func_ovl8_8037DD60(DBMenuPosition*, char*);
extern void func_ovl8_80386BE0(char*, const char**);
extern void func_ovl8_8037DFCC(s16, s16);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80369F60
u32 dDBMenuUnknown0x80369F60 = GPACK_RGBA8888(0xFF, 0xFF, 0xFF, 0xFF);

// 0x80369F64
u32 dDBMenuBGColor = GPACK_RGBA8888(0x00, 0x00, 0xFF, 0xFF);

// 0x80369F68
u32 dDBMenuBorderColor = GPACK_RGBA8888(0x80, 0xFF, 0xFF, 0xFF);

// 0x80369F6C
u32 dDBMenuTextColor = GPACK_RGBA8888(0xFF, 0xFF, 0xFF, 0xFF);

// 0x80369F70
u32 dDBMenuTextBGColor[/* */] = { GPACK_RGBA8888(0x00, 0x00, 0xFF, 0xFF), GPACK_RGBA8888(0x00, 0x00, 0xFF, 0xFF), GPACK_RGBA8888(0xFF, 0xFF, 0xFF, 0xFF) };

// 0x80369F7C
DBMenu dDBMenuArray[/* */] = { 
	{ 
		{ 0, 0, 0, 0 }, 
		{ 0xFF, 0xFF, 0xFF, 0xFF }, 
		{ 0x00, 0x00, 0xFF, 0xFF }, 
		1,
		0,
		'BWIN',
		0,
		0,
		0,
		0
	}, 
	NULL
};

// 0x80369FCC
u16 dDBMenuPrevInputs = 0;

// 0x80369FD0
u16 dDBMenuNewInputs = 0;

// 0x80369FD4
u16 dDBMenuStickInputs = 0;

// 0x80369FD8
s32 dDBMenuRapidScrollWait = 30;

// 0x80369FDC
s32 dDBMenuUnknown0x80369FDC = 0;

// 0x80369FE0
DBFontPadding dDBMenuFontPaddings[/* */] =
{
	{ 0x00, 0x04 },
	{ 0x00, 0x06 },
	{ 0x00, 0x04 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x00 },
	{ 0x00, 0x00 },
	{ 0x00, 0x06 },
	{ 0x00, 0x04 },
	{ 0x00, 0x04 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x05 },
	{ 0x00, 0x03 },
	{ 0x00, 0x06 },
	{ 0x00, 0x03 },
	{ 0x00, 0x02 },
	{ 0x00, 0x05 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x06 },
	{ 0x00, 0x05 },
	{ 0x00, 0x04 },
	{ 0x00, 0x02 },
	{ 0x00, 0x04 },
	{ 0x00, 0x03 },
	{ 0x00, 0x00 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x06 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x03 },
	{ 0x00, 0x00 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x00 },
	{ 0x00, 0x02 },
	{ 0x00, 0x02 },
	{ 0x00, 0x03 },
	{ 0x00, 0x05 },
	{ 0x00, 0x03 },
	{ 0x00, 0x05 },
	{ 0x00, 0x05 },
	{ 0x00, 0x01 },
	{ 0x00, 0x04 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x05 },
	{ 0x00, 0x04 },
	{ 0x00, 0x03 },
	{ 0x00, 0x05 },
	{ 0x00, 0x00 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x04 },
	{ 0x00, 0x03 },
	{ 0x00, 0x02 },
	{ 0x00, 0x00 },
	{ 0x00, 0x02 },
	{ 0x00, 0x03 },
	{ 0x00, 0x03 },
	{ 0x00, 0x04 },
	{ 0x00, 0x06 },
	{ 0x00, 0x04 },
	{ 0x00, 0x02 },
	{ 0x00, 0x00 }
};

// 0x8036A0A0
u32 dDBMenuGlyphs[/* */] =
{
	0x00000000, 0x00000000, 0x80808080,
	0x80008000, 0xA0A00000, 0x00000000,
	0x0050F850, 0xF8500000, 0x2078A0F0,
	0x28A87020, 0x6192946E, 0x19294600,
	0x30485020, 0x54889462, 0x80800000,
	0x00000000, 0x20408080, 0x80804020,
	0x80402020, 0x20204080, 0x005020F8,
	0x20500000, 0x002020F8, 0x20200000,
	0x00000000, 0x00404080, 0x000000F0,
	0x00000000, 0x00000000, 0x00008000,
	0x10102020, 0x40408080, 0x708898A8,
	0xC8887000, 0x40C04040, 0x40404000,
	0x70880810, 0x2040F800, 0xF8102070,
	0x08887000, 0x10305090, 0xF8101000,
	0xF880F008, 0x08887000, 0x304080F0,
	0x88887000, 0xF8081010, 0x20202000,
	0x70888870, 0x88887000, 0x70888878,
	0x08106000, 0x00008000, 0x00800000,
	0x00004000, 0x00404080, 0x00204080,
	0x40200000, 0x0000F800, 0xF8000000,
	0x00804020, 0x40800000, 0x60901020,
	0x40004000, 0x38449AAA, 0xAA9C4038,
	0x20205050, 0xF8888800, 0xF08888F0,
	0x8888F000, 0x70888080, 0x80887000,
	0xE0908888, 0x8890E000, 0xF08080E0,
	0x8080F000, 0xF08080E0, 0x80808000,
	0x70888098, 0x88887000, 0x888888F8,
	0x88888800, 0x80808080, 0x80808000,
	0x08080808, 0x88887000, 0x8890A0C0,
	0xA0908800, 0x80808080, 0x8080F000,
	0x82C6AA92, 0x82828200, 0xC8C8A8A8,
	0x98988800, 0x70888888, 0x88887000,
	0xF08888F0, 0x80808000, 0x70888888,
	0x88A87010, 0xF08888F0, 0xA0908800,
	0x70888070, 0x08887000, 0xF8202020,
	0x20202000, 0x88888888, 0x88887000,
	0x88888850, 0x50202000, 0x82825454,
	0x28282800, 0x88885020, 0x50888800,
	0x88885020, 0x20202000, 0xF0102040,
	0x8080F000, 0xC0808080, 0x808080C0,
	0x80804040, 0x20201010, 0xC0404040,
	0x404040C0, 0x80400000, 0x00000000,
	0x00000000, 0x0000FC00, 0x00000000,
	0x00000000, 0x00006010, 0x70907000,
	0x8080E090, 0x9090E000, 0x00006090,
	0x80906000, 0x10107090, 0x90907000,
	0x00006090, 0xF0806000, 0x3040E040,
	0x40404000, 0x00709090, 0x90701060,
	0x8080E090, 0x90909000, 0x4000C040,
	0x40404000, 0x20006020, 0x202020C0,
	0x808090A0, 0xC0A09000, 0xC0404040,
	0x40404000, 0x0000EC92, 0x92929200,
	0x0000E090, 0x90909000, 0x00006090,
	0x90906000, 0x00E09090, 0x90E08080,
	0x00709090, 0x90701010, 0x0000B0C0,
	0x80808000, 0x00007080, 0x6010E000,
	0x004040E0, 0x40404020, 0x00009090,
	0x90907000, 0x00008850, 0x50202000,
	0x00008254, 0x54282800, 0x00008850,
	0x20508800, 0x00909090, 0x90701060,
	0x0000F020, 0x4080F000, 0x20404080,
	0x40404020, 0x80808080, 0x80808080,
	0x80404020, 0x40404080, 0x68B00000,
	0x00000000
};

// 0x8036A398
DBFont dDBMenuDefaultFont =
{
	0,							// ???
	8,							// ???
	8,							// ???
	dDBMenuFontPaddings,		// Dimensions?
	1,							// ???
	dDBMenuGlyphs				// Glyphs?
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8036A400 - or string buffer?
char sDBMenuFontBuffer[0x7000];

// 0x80371400
s32 sDBMenuCursorExitID;

// 0x80371404
s32 D_ovl9_80371404; // TODO - some struct, pretty important!

// 0x80371408
GObj* sDBMenuActorGObj;

// 0x8037140C
DBMenuOption *sDBMenuOptions;

// 0x80371410
s32 sDBMenuOptionsNum;

// 0x80371414
s32 sDBMenuCursorID;

// 0x80371418
void (*sDBMenuActorProcDisplay)(GObj*);

// 0x8037141C
s32 sDBMenuIsRedrawInterrupt;

// 0x80371420
sb32 gDBMenuIsMenuOpen;

// 0x80371424
s32 sDBMenuTaskCountPrev;

// 0x80371428
char sDBMenuStringBuffer[0x38];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80369240
void dbMenuUpdateMenuInputs(void)
{
	u16 inputs = 0;
	SYController *controller = &gSYControllerMain;

	if (gSYControllerMain.stick_range.x > 40)
	{
		inputs |= R_JPAD;
	}
	if (gSYControllerMain.stick_range.x < -40)
	{
		inputs |= L_JPAD;
	}
	if (controller->stick_range.y > 40)
	{
		inputs |= U_JPAD;
	}
	if (controller->stick_range.y < -40)
	{
		inputs |= D_JPAD;
	}
	dDBMenuNewInputs = (inputs ^ dDBMenuPrevInputs) & inputs;

	if ((inputs ^ dDBMenuPrevInputs) != 0)
	{
		dDBMenuStickInputs = dDBMenuNewInputs;
		dDBMenuRapidScrollWait = 30;
	}
	else
	{
		dDBMenuRapidScrollWait--;

		if (dDBMenuRapidScrollWait > 0)
		{
			dDBMenuStickInputs = 0;
		}
		else
		{
			dDBMenuStickInputs = inputs;
			dDBMenuRapidScrollWait = 5;
		}
	}
	dDBMenuPrevInputs = inputs;
}

// 0x80369310
void dbMenuDrawString(DBMenuPosition *menu_pos, const char *str, ...)
{
	func_ovl8_80386BE0(sDBMenuStringBuffer, &str);
	func_ovl8_8037DD60(menu_pos, sDBMenuStringBuffer);
}

// 0x80369358
void dbMenuDrawBorder(s32 arg0, u32 color)
{
	DBMenuPosition *temp_s0;
	DBMenuPosition menu_position;

	temp_s0 = func_ovl8_803749BC(arg0);

	func_ovl8_80374A54(arg0, &dDBMenuArray[0]);

	// draw top border
	menu_position.x = 0;
	menu_position.y = 0;
	menu_position.w = 1;
	menu_position.h = dDBMenuArray[0].position.h;
	func_ovl8_80377AEC(temp_s0, &menu_position, color, 4);

	// draw left border
	menu_position.w = dDBMenuArray[0].position.w;
	menu_position.h = 1;
	func_ovl8_80377AEC(temp_s0, &menu_position, color, 4);

	// draw bottom border
	menu_position.y = dDBMenuArray[0].position.h - 1;
	menu_position.h = 1;
	func_ovl8_80377AEC(temp_s0, &menu_position, color, 4);

	// draw right border
	menu_position.x = dDBMenuArray[0].position.w - 1;
	menu_position.y = 0;
	menu_position.w = 1;
	menu_position.h = dDBMenuArray[0].position.h;
	func_ovl8_80377AEC(temp_s0, &menu_position, color, 4);
}

// 0x8036944C
void dbMenuDrawBackground(s32 arg0, u32 color)
{
	DBMenuPosition *sp24;
	DBMenuPosition sp1C;

	sp24 = func_ovl8_803749BC(arg0);

	func_ovl8_80374A54(arg0, &dDBMenuArray[0]);

	// draw bg
	sp1C.x = 1;
	sp1C.y = 1;
	sp1C.w = dDBMenuArray[0].position.w - 2;
	sp1C.h = dDBMenuArray[0].position.h - 2;

	func_ovl8_80377AEC(sp24, &sp1C, color, 4);
}

// 0x803694C8
void dbMenuDrawMenuOptionLabel(void *arg0, DBMenuOption *menu_option)
{
	switch (menu_option->type)
	{
	case nDBMenuOptionKindExitLabel:
	case nDBMenuOptionKindLabel:
		dbMenuDrawString(func_ovl8_803749BC(arg0), menu_option->label);
		break;
		
	case nDBMenuOptionKindNumeric:
		dbMenuDrawString(func_ovl8_803749BC(arg0), menu_option->label, *menu_option->value.s);
		break;
		
	case nDBMenuOptionKindNumericByte:
		dbMenuDrawString(func_ovl8_803749BC(arg0), menu_option->label, *menu_option->value.b);
		break;
		
	case nDBMenuOptionKindDouble:
		dbMenuDrawString(func_ovl8_803749BC(arg0), menu_option->label, *menu_option->value.f);
		break;
		
	case nDBMenuOptionKindString:
		dbMenuDrawString(func_ovl8_803749BC(arg0), ((uintptr_t*) menu_option->label)[*menu_option->value.s]);
		break;
		
	case nDBMenuOptionKindStringByte:
		dbMenuDrawString(func_ovl8_803749BC(arg0), ((uintptr_t*) menu_option->label)[*menu_option->value.b]);
		break;
	}
}

// 0x80369600
void dbMenuDrawMenuOptions(void *arg0, DBMenuOption *menu_option, s32 menu_options_num)
{
	s32 i;

	for (i = 0; i < menu_options_num; i++, menu_option++)
	{
		func_ovl8_8037DFCC(12, 2 + 9 * i);
		dbMenuDrawMenuOptionLabel(arg0, menu_option);
	}
}

// 0x80369680
void dbMenuDrawCursor(void* arg0, s32 cursor_id)
{
	func_ovl8_8037DFCC(3, (cursor_id * 9) + 2);
	dbMenuDrawString(func_ovl8_803749BC(arg0), ">");
}

// 0x803696D4
void dbMenuCommonProcDisplay(GObj *gobj)
{
	if (sDBMenuIsRedrawInterrupt != FALSE)
	{
		sDBMenuIsRedrawInterrupt = FALSE;

		dbMenuDrawBackground(D_ovl9_80371404, &dDBMenuBGColor);
		dbMenuDrawMenuOptions(D_ovl9_80371404, sDBMenuOptions, sDBMenuOptionsNum);
		dbMenuDrawCursor(D_ovl9_80371404, sDBMenuCursorID);
	}
	sDBMenuActorProcDisplay(gobj);
}

// 0x8036975C
void dbMenuActorProcUpdate(GObj *gobj)
{
	SYController *controller = &gSYControllerMain;
	s32 stick_x;
	f32 temp;

	dbMenuUpdateMenuInputs();

	if ((controller->button_update & U_JPAD) || (dDBMenuStickInputs & U_JPAD))
	{
		sDBMenuCursorID = (sDBMenuCursorID > 0) ? sDBMenuCursorID - 1 : sDBMenuOptionsNum - 1;

		sDBMenuIsRedrawInterrupt = TRUE;
	}
	if ((controller->button_update & D_JPAD) || (dDBMenuStickInputs & D_JPAD))
	{
		sDBMenuCursorID = (sDBMenuCursorID < (sDBMenuOptionsNum - 1)) ? sDBMenuCursorID + 1 : 0;

		sDBMenuIsRedrawInterrupt = TRUE;
	}
	if ((controller->button_update & L_JPAD) || (dDBMenuStickInputs & L_JPAD))
	{
		switch (sDBMenuOptions[sDBMenuCursorID].type)
		{
		case nDBMenuOptionKindNumeric:
		case nDBMenuOptionKindString:
			if (sDBMenuOptions[sDBMenuCursorID].min < *sDBMenuOptions[sDBMenuCursorID].value.s)
			{
				*sDBMenuOptions[sDBMenuCursorID].value.s -= 1;
			}
			else *sDBMenuOptions[sDBMenuCursorID].value.s = sDBMenuOptions[sDBMenuCursorID].max;
			
			sDBMenuIsRedrawInterrupt = TRUE;
			break;

		case nDBMenuOptionKindNumericByte:
		case nDBMenuOptionKindStringByte:
			if (sDBMenuOptions[sDBMenuCursorID].min < *sDBMenuOptions[sDBMenuCursorID].value.b)
			{
				*sDBMenuOptions[sDBMenuCursorID].value.b -= 1;
			}
			else *sDBMenuOptions[sDBMenuCursorID].value.b = sDBMenuOptions[sDBMenuCursorID].max;
			
			sDBMenuIsRedrawInterrupt = TRUE;
			break;

		case nDBMenuOptionKindDouble:
			if (controller->button_update & L_JPAD)
			{
				if (sDBMenuOptions[sDBMenuCursorID].min < *sDBMenuOptions[sDBMenuCursorID].value.f + -1.0F)
				{
					*sDBMenuOptions[sDBMenuCursorID].value.f += -1.0F;
				}
				else *sDBMenuOptions[sDBMenuCursorID].value.f = sDBMenuOptions[sDBMenuCursorID].min;

				sDBMenuIsRedrawInterrupt = TRUE;
			}
			break;

		default:
			break;
		}
	}
	if ((controller->button_update & R_JPAD) || (dDBMenuStickInputs & R_JPAD))
	{
		switch (sDBMenuOptions[sDBMenuCursorID].type)
		{
		case nDBMenuOptionKindNumeric:
		case nDBMenuOptionKindString:
			if (*sDBMenuOptions[sDBMenuCursorID].value.s < sDBMenuOptions[sDBMenuCursorID].max)
			{
				*sDBMenuOptions[sDBMenuCursorID].value.s += 1;
			}
			else *sDBMenuOptions[sDBMenuCursorID].value.s = sDBMenuOptions[sDBMenuCursorID].min;
				
			sDBMenuIsRedrawInterrupt = TRUE;
			break;

		case nDBMenuOptionKindNumericByte:
		case nDBMenuOptionKindStringByte:
			if (*sDBMenuOptions[sDBMenuCursorID].value.b < sDBMenuOptions[sDBMenuCursorID].max)
			{
				*sDBMenuOptions[sDBMenuCursorID].value.b += 1;
			}
			else *sDBMenuOptions[sDBMenuCursorID].value.b = sDBMenuOptions[sDBMenuCursorID].min;
			
			sDBMenuIsRedrawInterrupt = TRUE;
			break;

		case nDBMenuOptionKindDouble:
			if (controller->button_update & R_JPAD)
			{
				if (sDBMenuOptions[sDBMenuCursorID].max > *sDBMenuOptions[sDBMenuCursorID].value.f + 1.0F)
				{
					*sDBMenuOptions[sDBMenuCursorID].value.f += 1.0F;
				}
				else *sDBMenuOptions[sDBMenuCursorID].value.f = sDBMenuOptions[sDBMenuCursorID].max;
					
				sDBMenuIsRedrawInterrupt = TRUE;
			}
			break;

		default:
			break;
		}
	}
	stick_x = controller->stick_range.x;

	if (ABS(stick_x) > 20)
	{
		if (sDBMenuOptions[sDBMenuCursorID].type == nDBMenuOptionKindDouble)
		{
			temp = (sDBMenuOptions[sDBMenuCursorID].unknown18 * stick_x);

			if (stick_x > 0)
			{
				if (sDBMenuOptions[sDBMenuCursorID].max > *sDBMenuOptions[sDBMenuCursorID].value.f + temp)
				{
					*sDBMenuOptions[sDBMenuCursorID].value.f += temp;
				}
				else *sDBMenuOptions[sDBMenuCursorID].value.f = sDBMenuOptions[sDBMenuCursorID].max;
			}
			else if (sDBMenuOptions[sDBMenuCursorID].min < *sDBMenuOptions[sDBMenuCursorID].value.f + temp)
			{
				*sDBMenuOptions[sDBMenuCursorID].value.f += temp;
			}
			else *sDBMenuOptions[sDBMenuCursorID].value.f = sDBMenuOptions[sDBMenuCursorID].min;

			sDBMenuIsRedrawInterrupt = TRUE;
		}
	}
	if (controller->button_tap & A_BUTTON)
	{
		if (sDBMenuOptions[sDBMenuCursorID].type == nDBMenuOptionKindExitLabel)
		{
			sDBMenuCursorExitID = sDBMenuCursorID;
			func_ovl8_8037488C(D_ovl9_80371404);

			gDBMenuIsMenuOpen = FALSE;

			gSYTaskmanTaskCount = sDBMenuTaskCountPrev;
		}
		if (sDBMenuOptions[sDBMenuCursorID].proc_a != NULL)
		{
			sDBMenuOptions[sDBMenuCursorID].proc_a();

			sDBMenuIsRedrawInterrupt = TRUE;
		}
	}
	if (controller->button_tap & B_BUTTON)
	{
		func_ovl8_8037488C(D_ovl9_80371404);

		gDBMenuIsMenuOpen = FALSE;

		gSYTaskmanTaskCount = sDBMenuTaskCountPrev;
	}
}

// 0x80369D78
void dbMenuMakeMenu(s32 x, s32 y, s32 w, DBMenuOption *menu_options, s32 menu_options_num)
{
	if (gDBMenuIsMenuOpen == FALSE)
	{
		gDBMenuIsMenuOpen = TRUE;

		sDBMenuOptions = menu_options;
		sDBMenuOptionsNum = menu_options_num;

		sDBMenuCursorID = sDBMenuIsRedrawInterrupt = 0;
		sDBMenuCursorExitID = -1;

		dDBMenuArray[0].position.x = x;
		dDBMenuArray[0].position.y = y;
		dDBMenuArray[0].position.w = w;
		dDBMenuArray[0].position.h = (menu_options_num * 9) + 3;

		D_ovl9_80371404 = func_ovl8_80381C80(dDBMenuArray);
		sDBMenuActorGObj = func_ovl8_80374910(D_ovl9_80371404);

		dbMenuDrawBorder(D_ovl9_80371404, &dDBMenuBorderColor);
		dbMenuDrawMenuOptions(D_ovl9_80371404, menu_options, menu_options_num);
		dbMenuDrawCursor(D_ovl9_80371404, sDBMenuCursorID);

		sDBMenuActorProcDisplay = sDBMenuActorGObj->proc_display;
		sDBMenuActorGObj->proc_display = dbMenuCommonProcDisplay;

		gcAddGObjProcess(sDBMenuActorGObj, dbMenuActorProcUpdate, nGCProcessKindFunc, 1);

		sDBMenuTaskCountPrev = gSYTaskmanTaskCount;
		gSYTaskmanTaskCount = 1;
	}
}

// 0x80369EC0
void dbMenuDestroyMenu(void)
{
	func_ovl8_8037BB78();
}

// 0x80369EE0
void dbMenuInitMenu(void)
{
	DBTextBuffer tb;
	s32 sp18[5];

	tb.font_buffer = sDBMenuFontBuffer;
	tb.font_buffer_size = ARRAY_COUNT(sDBMenuFontBuffer);
	tb.unk_dbtextbuf_0x8 = 0;

	func_ovl8_8037B98C(&tb);

	func_ovl8_8037D6D4(&dDBMenuDefaultFont);
	func_ovl8_8037D9D0(&dDBMenuTextColor);
	func_ovl8_8037D9B4(dDBMenuTextBGColor);

	func_ovl8_8037D95C(sp18);

	sp18[0] = 4;

	func_ovl8_8037D908(sp18);

	gDBMenuIsMenuOpen = FALSE;
}
