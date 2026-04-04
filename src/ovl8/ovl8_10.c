#include "common.h"

#include <sys/objtypes.h>

typedef struct unkdata1 {
	s32 unk0[0x21];
	s32 unk84;
} unkdata1;

typedef struct unkdata0 {
	s32 unk0;
	s32 unk4;
	unkdata1* unk8;
} unkdata0;

void func_ovl8_8037C440(s32* arg0, s32 arg1);

// DATA
s32 D_ovl8_8038A1A0[] =
{
	0x00000000, 0x00000000, 0x00000000, func_ovl8_8037C440,
	0x00000000, 0x00000000
};

s32 D_ovl8_8038A1B8 = 0;

s32 D_ovl8_8038A1BC = 0;

s32 D_ovl8_8038A1C0 = 0;

GObj* D_ovl8_8038A1C4 = NULL;

GObj* D_ovl8_8038A1C8 = NULL;

extern u8 D_8038EFD0_1AB820;
extern s32 D_8038EFD4_1AB824;

void func_ovl8_803717C0(void*);

void* func_ovl8_8037C3C0(s32* arg0)
{
	if (arg0 || (arg0 = func_ovl8_803717A0(0x10)))
	{
		arg0[0xC/4] = &D_ovl8_8038A1A0;
		arg0[0x4/4] = 0;
		D_ovl8_8038A1BC = 0;
		D_ovl8_8038A1C0 = 0;
		D_ovl8_8038A1C4 = NULL;
		D_ovl8_8038A1C8 = NULL;
		arg0[0x8/4] = gcMakeGObjSPAfter(-0x200, 0, 0x1F, 3);
	}
	return arg0;
}

void func_ovl8_8037C440(s32* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0[0xC/4] = &D_ovl8_8038A1A0;
		gcEjectGObj(arg0[0x8/4]);
		if (arg1 & 1)
			func_ovl8_803717C0(arg0);
	}
}


void func_ovl8_8037C498(unkdata0* arg0, s32 arg1)
{
	arg0->unk8->unk84 = arg1;
}

void func_ovl8_8037C4A4(void** arg0, s32 unused)
{
	void (*temp_v0)();

	temp_v0 = arg0[1];
	if (temp_v0 != NULL)
		temp_v0();

	if (D_ovl8_8038A1C0 != 0)
	{
		func_80009CC8(D_ovl8_8038A1C0, D_8038EFD0_1AB820, D_8038EFD4_1AB824);
		D_ovl8_8038A1C0 = 0;
		D_ovl8_8038A1B8 = 1;
	}
	D_ovl8_8038A1BC = 0;
}

s32 func_ovl8_8037C510(s32* arg)
{
	return arg[2];
}

void func_ovl8_8037C518(s32 arg0, u8 arg1, s32 arg2)
{
	D_ovl8_8038A1C4 = gcFindGObjByLinkAndID(0x1F, -0x1FD);
	D_ovl8_8038A1C8 = gcFindGObjByLinkAndID(0x1F, -0x1FC);
	D_ovl8_8038A1C0 = arg0;
	D_8038EFD0_1AB820 = arg1;
	D_8038EFD4_1AB824 = arg2;
}

GObj* func_ovl8_8037C584(s32 arg0)
{
	GObj* var_v1;

	D_ovl8_8038A1C4 = gcFindGObjByLinkAndID(0x1F, -0x1FD);
	D_ovl8_8038A1C8 = gcFindGObjByLinkAndID(0x1F, -0x1FC);

	if (arg0 == 0)
		var_v1 = D_ovl8_8038A1C4;
	else
		var_v1 = D_ovl8_8038A1C8;

	return var_v1;
}

s32 func_ovl8_8037C5E8(s32 arg0)
{
	GObj* var_v1;

	D_ovl8_8038A1C4 = gcFindGObjByLinkAndID(0x1F, -0x1FD);
	D_ovl8_8038A1C8 = gcFindGObjByLinkAndID(0x1F, -0x1FC);
	if (arg0 == 0)
		var_v1 = D_ovl8_8038A1C4;
	else
		var_v1 = D_ovl8_8038A1C8;

	if (var_v1 == NULL)
		return 0;

	return var_v1->user_data.s;
}