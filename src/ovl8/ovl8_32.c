#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038EAE0;
extern dbFunction D_ovl8_8038EBC0;
extern dbUnknownLink D_ovl8_8038ED18;
extern dbUnknownLinkStruct D_ovl8_8038ED40;
extern s32 D_ovl8_8038EDA8;
extern s32 D_ovl8_8038BC8C;

void* func_ovl8_803879FC(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4);

// 0x803879A0
s32 func_ovl8_803879A0(s32 arg0, s32 arg1)
{
	s32 temp_v0;
	s32 var_v1;

	temp_v0 = func_ovl8_803717A0(0x140);

	if (temp_v0 != 0)
	{
		func_ovl8_803879FC(temp_v0, 0, 0, arg1, arg0);
		var_v1 = temp_v0;
	}
	else
		var_v1 = 0;

	return var_v1;
}

// 0x803879FC
void* func_ovl8_803879FC(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4)
{
	dbUnknown3 *temp_v0;

	if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0x140)) != NULL))
	{
		if (arg1 == NULL)
		{
			arg1 = &arg0->unk_dbunk5_0xD8;
			arg2 = &arg0->unk_dbunk5_0x134;
			#line 40
			func_ovl8_803717E0(arg1);
			func_ovl8_8037C2D0(arg2);
			#line 47
		}

		func_ovl8_8037C7D4(arg0, arg1, arg2, arg3, arg3->unk_dbunk3_0x24->unk_dbunk3_0x0, arg3->unk_dbunk3_0x24->unk_dbunk3_0x4, arg4);
		func_ovl8_80381934(&arg0->unk_dbunk5_0xB8, &arg3->unk_dbunk3_0x24->unk_dbunk3_0x8, arg0);

		arg0->unk_dbunk5_0x30 = &D_ovl8_8038EAE0,
		arg1->db_func = &D_ovl8_8038EBC0;
		arg2->unk_dbunklink_0x8 = &D_ovl8_8038ED18;
		arg0->unk_dbunk5_0x4C = &D_ovl8_8038ED40,
		arg0->unk_dbunk5_0xD4 = &D_ovl8_8038EDA8;
	}

	return arg0;
}

// 0x80387AF8
void func_ovl8_80387AF8(dbUnknown5* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->unk_dbunk5_0x30 = &D_ovl8_8038EAE0,
		arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038EBC0;
		arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038ED18,
		arg0->unk_dbunk5_0x4C = &D_ovl8_8038ED40,
		arg0->unk_dbunk5_0xD4 = &D_ovl8_8038EDA8;

		#line 69
		if (&arg0->unk_dbunk5_0xB8 != 0)
		{
			arg0->unk_dbunk5_0xD4 = &D_ovl8_8038BC8C;
			func_ovl8_8038120C(&arg0->unk_dbunk5_0xB8, 0);
		}
		#line 80

		func_ovl8_8037C92C(arg0, 0);

		if (arg1 != 0)
		{
			func_ovl8_8037C30C(arg0->unk_dbunk5_0x34, 0);
			func_ovl8_803718C4(arg0->unk_dbunk5_0x38, 0);
		}
		if (arg1 & 1)
		{
			func_ovl8_803717C0(arg0);
		}
	}
}

// 0x80387BD0
void func_ovl8_80387BD0(s32 arg0)
{
	func_ovl8_80381308(arg0 + 0xB8);
}

// 0x80387BF0
void func_ovl8_80387BF0(s32 arg0, s32 arg1)
{
	func_ovl8_80381B04(arg0 + 0xB8);
	func_ovl8_80373694(arg0, arg1);
}

// 0x80387C24
void func_ovl8_80387C24(dbUnknown5* arg0)
{
	dbUnknownLinkStruct *var_v0;
	dbUnknownLinkStruct *var_v1;

	if ((arg0->unk_dbunk5_0x0 != 0.0f) && ((arg0->unk_dbunk5_0xD0 != 0) || (arg0->unk_dbunk5_0x1E != 0)))
	{
		var_v0 = arg0->unk_dbunk5_0xB0;
		var_v1 = (var_v0 != 0) ? var_v0 : arg0->unk_dbunk5_0xB4;
		var_v0 = var_v1;
	}
	else
	{
		var_v0 = arg0->unk_dbunk5_0xB4;
	}

	arg0->unk_dbunk5_0x48 = var_v0;
	func_ovl8_803725DC(&arg0->unk_dbunk5_0x40);
}