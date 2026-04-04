#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>

extern void* D_ovl8_80388980[]; // function pointers it seems
extern void* func_ovl8_803716D8(s32);
extern void func_ovl8_80371764(s32 arg0);

void func_ovl8_80371E58();
void func_ovl8_80371EA0(dbUnknownLinkStruct* arg0);
void func_ovl8_80371ECC(dbUnknownLinkStruct* arg0, DBMenu* arg1);


void* func_ovl8_803717A0(s32 arg0)
{
	return func_ovl8_803716D8(arg0);
}

void func_ovl8_803717C0(dbUnknownLinkStruct* arg0)
{
	func_ovl8_80371764(arg0);
}

void* func_ovl8_803717E0(dbUnknownLinkStruct* arg0)
{
	if ((arg0 != NULL) || ((arg0 = func_ovl8_803717A0(0x5C)) != NULL))
	{
		arg0->db_func = &D_ovl8_80388980;
		func_ovl8_80371EA0(arg0);
	}
	return arg0;
}

void* func_ovl8_80371828(dbUnknownLinkStruct* arg0, DBMenu* arg1)
{
	if ((arg0 != NULL) || ((arg0 = func_ovl8_803717A0(0x5C)) != NULL))
	{
		arg0->db_func = &D_ovl8_80388980;
		func_ovl8_80371ECC(arg0, arg1);
	}
	return arg0;
}

s32 func_ovl8_80371878(s32 arg0, s32 arg1)
{
	s32 temp_v0;
	s32 var_v1;

	temp_v0 = func_ovl8_803717A0(0x5C);
	if (temp_v0 != 0)
	{
		func_ovl8_80371828(temp_v0, arg1);
		var_v1 = temp_v0;
	}
	else
		var_v1 = 0;
	return var_v1;
}

void func_ovl8_803718C4(dbUnknownLinkStruct* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->db_func = &D_ovl8_80388980;
		if (arg1 & 1)
			func_ovl8_803717C0(arg0);
	}
}

// 0x803718FC
void func_ovl8_803718FC(dbUnknownLinkStruct* arg0)
{
	dbFunction *temp_v0 = arg0->db_func;
	arg0->unk_dbunkstruct_0x20 = 1;
	temp_v0[4].unk_dbfunc_0x4(temp_v0[4].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

void func_ovl8_80371930(dbUnknownLinkStruct* arg0)
{
	dbFunction *temp_v0 = arg0->db_func;
	arg0->unk_dbunkstruct_0x20 = 0;
	temp_v0[5].unk_dbfunc_0x4(temp_v0[5].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

void func_ovl8_80371960(s32 arg0) {}

void func_ovl8_80371968(s32 arg0) {}

void func_ovl8_80371970(s32 arg0) {}

void func_ovl8_80371978(s32 arg0) {}

void func_ovl8_80371980(dbUnknown16* arg0, dbUnknown16* arg1)
{
	arg1->unk_dbunk16_0x0 -= arg0->unk_dbunk16_0x2;
	arg1->unk_dbunk16_0x2 -= arg0->unk_dbunk16_0x4;
}

void func_ovl8_803719A4(dbUnknown16* arg0, dbUnknown16* arg1)
{
	arg1->unk_dbunk16_0x0 += arg0->unk_dbunk16_0x2;
	arg1->unk_dbunk16_0x2 += arg0->unk_dbunk16_0x4;
}

void func_ovl8_803719C8(dbUnknownLinkStruct* arg0, s32 arg1)
{
	dbFunction *temp_v0 = arg0->db_func;

	temp_v0[15].unk_dbfunc_0x4(temp_v0[15].unk_dbfunc_0x0 + (u32)arg0, arg1);

	if (arg0->unk_dbunkstruct_0x4C != 0)
	{
		temp_v0 = arg0->db_func;
		temp_v0[8].unk_dbfunc_0x4(temp_v0[8].unk_dbfunc_0x0 + (u32)arg0, arg1);
	}
}

void func_ovl8_80371A2C(dbUnknownLinkStruct* arg0)
{
	dbFunction *temp_v0;

	func_ovl8_80371E58();

	temp_v0 = arg0->db_func;

	if (temp_v0[36].unk_dbfunc_0x4(temp_v0[36].unk_dbfunc_0x0 + (u32)arg0))
	{
		temp_v0 = arg0->db_func;
		temp_v0[24].unk_dbfunc_0x4(temp_v0[24].unk_dbfunc_0x0 + (u32)arg0);
	}
	if (arg0->unk_dbunkstruct_0x1C.link)
	{
		temp_v0 = arg0->db_func;
		temp_v0[12].unk_dbfunc_0x4(temp_v0[12].unk_dbfunc_0x0 + (u32)arg0);
	}

	temp_v0 = arg0->db_func;
	temp_v0[35].unk_dbfunc_0x4(temp_v0[35].unk_dbfunc_0x0 + (u32)arg0, 0);
}

void func_ovl8_80371ABC(s32 arg0) {}

void func_ovl8_80371AC4(s32 arg0) {}

void func_ovl8_80371ACC(s32 arg0, s32 arg1) {}

void func_ovl8_80371AD8(dbUnknownLinkStruct* arg0, s32 arg1)
{
	dbFunction *temp_v0;

	if (arg0->unk_dbunkstruct_0x4C != NULL)
	{
		temp_v0 = arg0->unk_dbunkstruct_0x4C->unk_dbunkstruct_0x20->db_func;
		temp_v0[15].unk_dbfunc_0x4(temp_v0[15].unk_dbfunc_0x0 + (u32)arg0->unk_dbunkstruct_0x4C->unk_dbunkstruct_0x20, arg1);
	}
	else
	{
		temp_v0 = arg0->db_func;
		temp_v0[8].unk_dbfunc_0x4(temp_v0[8].unk_dbfunc_0x0 + (u32)arg0, arg1);
	}
}

void func_ovl8_80371B34(dbUnknownLinkStruct* arg0, s32 arg1)
{
	if (arg0->unk_dbunkstruct_0x4C != NULL)
		arg0->db_func[15].unk_dbfunc_0x4(arg0->db_func[15].unk_dbfunc_0x0 + (u32)arg0, arg1);

	arg0->db_func[17].unk_dbfunc_0x4(arg0->db_func[17].unk_dbfunc_0x0 + (u32)arg0, arg1);
}

void func_ovl8_80371B98(void *arg0, db4Bytes *arg1)
{
	func_ovl8_80377168(*arg1, (u32)arg0 + 2);
}

sb32 func_ovl8_80371BCC(dbUnknownLinkStruct* arg0, s32 arg1)
{
	if ((arg0->unk_dbunkstruct_0x1C.link == NULL) || (arg0->unk_dbunkstruct_0x20 == NULL))
		return FALSE;

	return arg0->db_func[19].unk_dbfunc_0x4(arg0->db_func[19].unk_dbfunc_0x0 + (u32)arg0, arg1, arg0);
}

sb32 func_ovl8_80371C1C(s32 arg0, s32 arg1)
{
	return FALSE;
}

void func_ovl8_80371C2C(dbUnknownLinkStruct* arg0, DBMenuPosition *arg1)
{
	*arg1 = arg0->position;
}

void func_ovl8_80371C50(dbUnknownLinkStruct* arg0, DBMenuPosition *arg1)
{
	*arg1 = arg0->position;
    
    if (arg0->unk_dbunkstruct_0x4C == NULL)
    {
        arg1->y = 0;
        arg1->x = 0;
    }
}

void func_ovl8_80371C8C(dbUnknownLinkStruct *arg0, DBMenuPosition *arg1)
{
	*arg1 = arg0->position;
	arg1->x = 0;
	arg1->y = 0;
}

sb32 func_ovl8_80371CB8(dbUnknownLinkStruct* arg0)
{
	if (arg0->unk_dbunkstruct_0x4C != NULL)
	{
		dbFunction *db_func = arg0->unk_dbunkstruct_0x4C->unk_dbunkstruct_0x20->db_func;
		dbUnknownLinkStruct *temp_v1 = arg0->unk_dbunkstruct_0x4C->unk_dbunkstruct_0x20;

		return db_func[23].unk_dbfunc_0x4(db_func[23].unk_dbfunc_0x0 + (u32)temp_v1);
	}
	else
		return FALSE;
}

void func_ovl8_80371D00(s32 arg0) {}

void func_ovl8_80371D08(dbUnknownLinkStruct* arg0, s16 arg1, s16 arg2)
{
	// Whitespace regswap memes
	arg0->position.x += arg1, arg0->position.y += arg2;
	arg0->db_func[26].unk_dbfunc_0x4(arg0->db_func[26].unk_dbfunc_0x0 + (u32)arg0);
}

void func_ovl8_80371D64(s32 arg0, s32 arg1, s32 arg2) {}

void func_ovl8_80371D74(dbUnknownLinkStruct* arg0, s16 arg1, s16 arg2)
{
	arg0->position.y = arg2;
	arg0->position.x = arg1;
	arg0->db_func[28].unk_dbfunc_0x4(arg0->db_func[28].unk_dbfunc_0x0 + (u32)arg0);
}

void func_ovl8_80371DC0(s32 arg0, s32 arg1, s32 arg2) {}

void func_ovl8_80371DD0(s32* arg0)
{
	arg0[0x1C/4] = 1;
}

void func_ovl8_80371DDC(s32* arg0)
{
	arg0[0x1C/4] = 0;
}

s32 func_ovl8_80371DE4(s32* arg0)
{
	return arg0[0xC/4];
}

void func_ovl8_80371DEC(s32 arg0) {}

void func_ovl8_80371DF4(s32 arg0) {}

void func_ovl8_80371DFC(s32 arg0) {}

void func_ovl8_80371E04(s32* arg0, s32 arg1)
{
	arg0[0x28/4] = arg1;
}

s32 func_ovl8_80371E0C(s32* arg0)
{
	return arg0[0x28/4];
}

void func_ovl8_80371E14(s32 arg0) {}

void func_ovl8_80371E1C(s32 arg0) {}

void func_ovl8_80371E24(s32* arg0, s32 arg1)
{
	arg0[0x54/4] = arg1;
}

void func_ovl8_80371E2C(dbUnknownLinkStruct* arg0)
{
	arg0->db_func[37].unk_dbfunc_0x4(arg0->db_func[37].unk_dbfunc_0x0 + (u32)arg0);
}

void func_ovl8_80371E58(dbUnknownLinkStruct* arg0)
{
	if (arg0->unk_dbunkstruct_0x54 != NULL)
		arg0->unk_dbunkstruct_0x54();

	arg0->db_func[38].unk_dbfunc_0x4(arg0->db_func[38].unk_dbfunc_0x0 + (u32)arg0, arg0);
}

void func_ovl8_80371EA0(dbUnknownLinkStruct* arg0)
{
	arg0->unk_dbunkstruct_0x48 = arg0->unk_dbunkstruct_0x4C = NULL;
	arg0->unk_dbunkstruct_0xC = 'PANE';
	arg0->unk_dbunkstruct_0x1C.link = (void*)1;
	arg0->unk_dbunkstruct_0x20 = (void*)1;
	arg0->unk_dbunkstruct_0x28 = 0;
	arg0->unk_dbunkstruct_0x54 = NULL;
}

void func_ovl8_80371ECC(dbUnknownLinkStruct* arg0, DBMenu* arg1)
{
	func_ovl8_80371EA0(arg0);
	arg0->position = arg1->position;
	arg0->text_color = arg1->text_color;
    arg0->bg_color = arg1->bg_color;
    arg0->unk_dbunkstruct_0x50.arr[0] = arg1->bg_color.r;
    arg0->unk_dbunkstruct_0x50.arr[1] = arg1->bg_color.g;
    arg0->unk_dbunkstruct_0x50.arr[2] = arg1->bg_color.b;
    arg0->unk_dbunkstruct_0x50.arr[3] = arg1->bg_color.a;
	arg0->unk_dbunkstruct_0xC = arg1->unk_dbmenu_0x18;
	arg0->id = arg1->id;
	arg0->unk_dbunkstruct_0x24 = arg1->unk_dbmenu_0x10;
	arg0->unk_dbunkstruct_0x2C = 0;
	arg0->unk_dbunkstruct_0x30 = 0x8000;
	arg0->unk_dbunkstruct_0x34 = 0x4000;
	arg0->unk_dbunkstruct_0x38 = 0x800;
	arg0->unk_dbunkstruct_0x3C = 0x400;
	arg0->unk_dbunkstruct_0x40 = 0x100;
	arg0->unk_dbunkstruct_0x44 = 0x200;
}