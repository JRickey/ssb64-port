#include "common.h"

#include <sys/develop.h>

extern void func_ovl8_803718FC(void*);
extern void func_ovl8_80371930(void*);
extern void func_ovl8_80371960(void*);
extern void func_ovl8_80371968(void*);
extern void func_ovl8_80371970(void*);
extern void func_ovl8_80371978(void*);
extern void func_ovl8_80371980(void*);
extern void func_ovl8_803719A4(void*);
extern void func_ovl8_803719C8(void*);
extern void func_ovl8_80371A2C(void*);
extern void func_ovl8_80371ABC(void*);
extern void func_ovl8_80371AC4(void*);
extern void func_ovl8_80371ACC(void*);
extern void func_ovl8_80371AD8(void*);
extern void func_ovl8_80371B34(void*);
extern void func_ovl8_80371B98(void*);
extern void func_ovl8_80371BCC(void*);
extern void func_ovl8_80371C1C(void*);
extern void func_ovl8_80371C2C(void*);
extern void func_ovl8_80371C50(void*);
extern void func_ovl8_80371C8C(void*);
extern void func_ovl8_80371CB8(void*);
extern void func_ovl8_80371D00(void*);
extern void func_ovl8_80371D08(void*);
extern void func_ovl8_80371D64(void*);
extern void func_ovl8_80371D74(void*);
extern void func_ovl8_80371DC0(void*);
extern void func_ovl8_80371DD0(void*);
extern void func_ovl8_80371DDC(void*);
extern void func_ovl8_80371DE4(void*);
extern void func_ovl8_80371DEC(void*);
extern void func_ovl8_80371DF4(void*);
extern void func_ovl8_80371DFC(void*);
extern void func_ovl8_80371E04(void*);
extern void func_ovl8_80371E0C(void*);
extern void func_ovl8_80371E14(void*);
extern void func_ovl8_80371E1C(void*);
extern void func_ovl8_80371E24(void*);
extern void func_ovl8_80371E2C(void*);
extern void func_ovl8_80371E58(void*);

void func_ovl8_8037CEE8(dbUnknownLinkStruct* arg0, s32 arg1);
void func_ovl8_8037CF4C(dbUnknownLinkStruct* arg0);

dbFunction D_ovl8_8038A700[] =
{
	0x0000, NULL,
	0x0000, func_ovl8_8037CEE8,
	0x0000, func_ovl8_803718FC,
	0x0000, func_ovl8_80371930,
	0x0000, func_ovl8_80371960,
	0x0000, func_ovl8_80371968,
	0x0000, func_ovl8_80371970,
	0x0000, func_ovl8_80371978,
	0x0000, func_ovl8_80371980,
	0x0000, func_ovl8_803719A4,
	0x0000, func_ovl8_803719C8,
	0x0000, func_ovl8_80371A2C,
	0x0000, func_ovl8_80371ABC,
	0x0000, func_ovl8_80371AC4,
	0x0000, func_ovl8_80371ACC,
	0x0000, func_ovl8_80371AD8,
	0x0000, func_ovl8_80371B34,
	0x0000, func_ovl8_80371B98,
	0x0000, func_ovl8_80371BCC,
	0x0000, func_ovl8_80371C1C,
	0x0000, func_ovl8_80371C2C,
	0x0000, func_ovl8_80371C50,
	0x0000, func_ovl8_80371C8C,
	0x0000, func_ovl8_80371CB8,
	0x0000, func_ovl8_80371D00,
	0x0000, func_ovl8_80371D08,
	0x0000, func_ovl8_80371D64,
	0x0000, func_ovl8_80371D74,
	0x0000, func_ovl8_80371DC0,
	0x0000, func_ovl8_80371DD0,
	0x0000, func_ovl8_80371DDC,
	0x0000, func_ovl8_80371DE4,
	0x0000, func_ovl8_80371DEC,
	0x0000, func_ovl8_80371DF4,
	0x0000, func_ovl8_80371DFC,
	0x0000, func_ovl8_80371E04,
	0x0000, func_ovl8_80371E0C,
	0x0000, func_ovl8_80371E14,
	0x0000, func_ovl8_80371E1C,
	0x0000, func_ovl8_80371E24,
	0x0000, func_ovl8_80371E2C,
	0x0000, func_ovl8_80371E58,
	0x0000, NULL,
	0x0000, NULL
};

// 0x8037CE40
void* func_ovl8_8037CE40(dbUnknownLinkStruct* arg0)
{
	if ((arg0 != NULL) || ((arg0 = func_ovl8_803717A0(0x68)) != NULL))
	{
		func_ovl8_803717E0(arg0);
		arg0->db_func = &D_ovl8_8038A700;
		func_ovl8_8037CF4C(arg0);
	}
	return arg0;
}

// 0x8037CE90
void* func_ovl8_8037CE90(dbUnknownLinkStruct* arg0, dbBytesCopy* arg1)
{
	if ((arg0 != NULL) || ((arg0 = func_ovl8_803717A0(0x68)) != NULL))
	{
		func_ovl8_80371828(arg0, arg1);
		arg0->db_func = &D_ovl8_8038A700;
		func_ovl8_8037CF4C(arg0);
	}
	return arg0;
}

// 0x8037CEE8
void func_ovl8_8037CEE8(dbUnknownLinkStruct* arg0, s32 arg1)
{
	if (arg0 == NULL)
		return;

	arg0->db_func = &D_ovl8_8038A700;
	
	gcEjectGObj(arg0->unk_dbunkstruct_0x48);
	#line 128
	func_ovl8_803718C4(arg0, 0); // weird swapping
	#line 133

	if (arg1 & 1)
		func_ovl8_803717C0(arg0);
}

// 0x8037CF4C
void func_ovl8_8037CF4C(dbUnknownLinkStruct* arg0) {
    arg0->unk_dbunkstruct_0x64 = 0;
    arg0->unk_dbunkstruct_0x5C = arg0->position;
    arg0->unk_dbunkstruct_0x48 = gcMakeGObjSPAfter(-0x1FB, NULL, 0x1F, 0);
}



// 0x8037CFAC
void func_ovl8_8037CFAC(void** arg0)
{
	void (*temp_v0)();

	temp_v0 = arg0[0x64/4];

	if (temp_v0 != NULL)
		temp_v0();
}

// 0x8037CFD8
void func_ovl8_8037CFD8(dbUnknownLinkStruct* arg0, dbUnknownLinkStruct* arg1)
{
	GObj *temp_v0 = arg0->unk_dbunkstruct_0x48;
	temp_v0->user_data.p = arg1;
}
