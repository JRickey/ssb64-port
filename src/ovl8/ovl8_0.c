#include <sys/develop.h>

extern dbUnknownLink *D_ovl8_8038EE60;
extern u32 D_ovl8_8038EE64;
extern u32 D_ovl8_8038EE68;
extern dbUnknownLink D_ovl8_8038EE70;

void func_ovl8_80371460(dbUnknownLink* arg0, dbUnknownLink* arg1, dbUnknownLink* arg2)
{
	arg0->unk_dbunklink_0xC = arg1;
	arg1->unk_dbunklink_0xC = arg2;
	arg2->unk_dbunklink_0x10 = arg1;
	arg1->unk_dbunklink_0x10 = arg0;
}

void func_ovl8_80371474(dbUnknownLink* arg0)
{
	func_ovl8_80371460(D_ovl8_8038EE70.unk_dbunklink_0x10, arg0, &D_ovl8_8038EE70);
}

void func_ovl8_803714A0(dbUnknownLink *arg0)
{
	dbUnknownLink* temp_v0;
	dbUnknownLink* temp_v1;
	temp_v0 = arg0->unk_dbunklink_0x10;
	temp_v1 = arg0->unk_dbunklink_0xC;
	temp_v0->unk_dbunklink_0xC = temp_v1;
	temp_v1->unk_dbunklink_0x10 = temp_v0;
}

dbUnknownLink* func_ovl8_803714B4(s32 arg0)
{
	dbUnknownLink* var_v1;
	for (var_v1 = D_ovl8_8038EE70.unk_dbunklink_0xC; var_v1 != &D_ovl8_8038EE70; var_v1 = var_v1->unk_dbunklink_0xC)
	{
		if ((s32)var_v1->unk_dbunklink_0x4 >= arg0)
			return var_v1;
	}
	return NULL;
}

void func_ovl8_803714F8(dbUnknownLink *arg0, s32 arg1)
{
	dbUnknownLink* temp_a2;
	dbUnknownLink* temp_v0;

	temp_a2 = (void*) ((uintptr_t)arg0 + (intptr_t)arg1);
	temp_a2->dbunklink_0x0 = arg0;

	temp_a2->unk_dbunklink_0x4 = arg0->unk_dbunklink_0x4 - arg1;
	temp_a2->unk_dbunklink_0x8 = NULL;

	temp_v0 = (dbUnknownLink*) ((uintptr_t)temp_a2 + (intptr_t)temp_a2->unk_dbunklink_0x4);
	
	if ((u32) temp_v0 < (u32) D_ovl8_8038EE68) 
		temp_v0->dbunklink_0x0 = temp_a2;

	func_ovl8_80371474(temp_a2);

	arg0->unk_dbunklink_0x4 = arg1;
}

void func_ovl8_80371560(dbUnknownLink* arg0)
{
	dbUnknownLink* temp_a0;
	dbUnknownLink* temp_a2;
	dbUnknownLink* temp_v0;

	temp_v0 = arg0->dbunklink_0x0;
	
	temp_a2 = (dbUnknownLink*) ((uintptr_t)arg0 + (intptr_t)arg0->unk_dbunklink_0x4);
	
	if ((temp_v0 != NULL) && (temp_v0->unk_dbunklink_0x8 == NULL)) 
	{
		temp_v0->unk_dbunklink_0x4 += arg0->unk_dbunklink_0x4;
		
		if ((u32) temp_a2 < (u32) D_ovl8_8038EE68) 
		{
			temp_a2->dbunklink_0x0 = temp_v0;
		}
		arg0 = temp_v0;
	}
	else 
	{
		func_ovl8_80371474(arg0);
		
		arg0->unk_dbunklink_0x8 = NULL;
	}
	if (((u32) temp_a2 < D_ovl8_8038EE68) && (temp_a2->unk_dbunklink_0x8 == NULL)) 
	{
		temp_a0 = (dbUnknownLink*) ((uintptr_t)temp_a2 + (intptr_t)temp_a2->unk_dbunklink_0x4);
		
		if ((u32) temp_a0 < D_ovl8_8038EE68)
		{
			temp_a0->dbunklink_0x0 = arg0;
		}
		arg0->unk_dbunklink_0x4 += temp_a2->unk_dbunklink_0x4;
		
		func_ovl8_803714A0(temp_a2);
	}
}

void func_ovl8_80371638(dbUnknownLink *arg0, u32 arg1)
{
	dbUnknownLink *var_a1;
	
	D_ovl8_8038EE64 = arg1;
	D_ovl8_8038EE60 = arg0;
	
	D_ovl8_8038EE68 = ((uintptr_t)D_ovl8_8038EE60 + (uintptr_t)arg1);

	var_a1 = D_ovl8_8038EE60;

	var_a1->dbunklink_0x0 = NULL;
	var_a1->unk_dbunklink_0x4 = arg1;
	var_a1->unk_dbunklink_0x8 = NULL;

	func_ovl8_80371460(&D_ovl8_8038EE70, var_a1, &D_ovl8_8038EE70);
}

s32 func_ovl8_8037169C(s32 arg0)
{
	if (arg0 < 8U)
		arg0 = 8U;

	return func_ovl8_803714B4((arg0 + 0xF) & ~3) != 0;
}

dbUnknownLink** func_ovl8_803716D8(u32 arg0)
{
	dbUnknownLink* temp_v0;
	u32 temp_a1;

	if (arg0 < 8) 
		arg0 = 8;

	temp_a1 = (arg0 + 0xF) & ~3;

	temp_v0 = func_ovl8_803714B4(temp_a1);
	
	if (temp_v0 == NULL) 
		return NULL;

	if ((u32) (temp_v0->unk_dbunklink_0x4 - temp_a1) > 0x14)
		func_ovl8_803714F8(temp_v0, temp_a1);

	func_ovl8_803714A0(temp_v0);
	
	temp_v0->unk_dbunklink_0x8 = (dbUnknownLink*)1;
	
	return &temp_v0->unk_dbunklink_0xC;
}

void func_ovl8_80371764(s32 arg0)
{
	func_ovl8_80371560(arg0 - 0xC);
}