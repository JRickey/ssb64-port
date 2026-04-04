#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>


extern dbUnknownLink D_ovl8_8038A170;

// 0x8037C2D0
void* func_ovl8_8037C2D0(void** arg0)
{
	if (arg0 != NULL || (arg0 = func_ovl8_803717A0(0xC)))
		arg0[0x8/4] = &D_ovl8_8038A170;

	return arg0;
}

// 0x8037C30C
void func_ovl8_8037C30C(dbUnknownLink* arg0, s32 arg1)
{
	if (arg0 == NULL)
		return;

	arg0->unk_dbunklink_0x8 = &D_ovl8_8038A170;

	if (arg1 & 1)
		func_ovl8_803717C0(arg0);
}

// 0x8037C344
s32 func_ovl8_8037C344(s32 arg0, s32 arg1, s32 arg2)
{
	return 0;
}

// 0x8037C358
s32 func_ovl8_8037C358(dbUnknown5_2* arg0, s32 arg1, s32 arg2)
{
	dbUnknown5_2 *temp_a3;
	dbFunction *temp_v0;
	dbUnknown5 *temp_v1;

	temp_a3 = arg0->dbUnknown5_2_0x4;
	if (temp_a3 == NULL)
	{
		return 0;
	}

	temp_v1 = temp_a3->dbUnknown5_2_0x4C;
	if (temp_v1 == NULL)
	{
		return 0;
	}

	temp_v0 = temp_v1->db_func;
	return temp_v0[18].unk_dbfunc_0x4(temp_v0[18].unk_dbfunc_0x0 + (uintptr_t)temp_v1, arg1, arg2, temp_a3);
}

// 0x8037C3AC
void func_ovl8_8037C3AC(s32* arg0, s32 arg1)
{
	arg0[1] = arg1;
}