#include <ft/fttypes.h>
#include <mv/mvopening/mvopeningroom.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_80390BE0[2] = { 0, 0 };

FTOpeningDesc D_ovl1_80390BE8[15] =
{
	0x00010000, NULL,
	0x00010001, NULL,
	0x00010002, NULL,
	0x00010003, NULL,
	0x00010004, NULL,
	0x00010005, NULL,
	0x00010006, NULL,
	0x00010007, NULL,
	0x00010008, mvOpeningFighterProcUpdate,
	0x00010009, scSubsysFighterApplyVelTransN,
	0x0001000A, scSubsysFighterApplyVelTransN,
	0x0001000B, NULL,
	0x0001000C, NULL,
	0x0001000D, NULL,
	0x0001000E, NULL
};

FTOpeningDesc D_ovl1_80390C60[1] =
{
	0xFFFFFFFF, NULL
};

FTOpeningDesc D_ovl1_80390C68[4] =
{
	0x0001000F, NULL,
	0x00010010, NULL,
	0x00010011, NULL,
	0x00010012, NULL
};

FTOpeningDesc D_ovl1_80390C88[9] =
{
	0x0001000F, NULL,
	0x00010010, NULL,
	0x00010011, NULL,
	0x00010012, NULL,
	0x00010013, NULL,
	0x00010014, NULL,
	0x00010015, NULL,
	0x00010016, NULL,
	0x00010017, NULL
};

FTOpeningDesc D_ovl1_80390CD0[1] =
{
	0x0001000F, NULL
};

FTOpeningDesc D_ovl1_80390CD8[1] =
{
	0x0001000F, NULL
};

FTOpeningDesc D_ovl1_80390CE0[4] =
{
	0x0001000F, NULL,
	0x00010010, NULL,
	0x00010011, NULL,
	0x00010012, NULL
};

FTOpeningDesc D_ovl1_80390D00[1] =
{
	0x0001000F, NULL
};

FTOpeningDesc D_ovl1_80390D08[3] =
{
	0x0001000F, NULL,
	0x00010010, NULL,
	0x00010011, NULL
};

FTOpeningDesc* D_ovl1_80390D20[] =
{
	D_ovl1_80390C68,
	D_ovl1_80390C88,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390CD8,
	D_ovl1_80390CE0,
	D_ovl1_80390C60,
	D_ovl1_80390CD0,
	D_ovl1_80390D00,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390D08,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	D_ovl1_80390C60,
	NULL,
};

f32 dSCSubsysFighterScales[] =
{
	1.25,
	1.15,
	1.00,
	1.03,
	1.21,
	1.33,
	1.05,
	1.07,
	1.22,
	1.20,
	1.26,
	1.30
};
