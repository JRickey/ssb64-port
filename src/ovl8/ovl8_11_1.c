#include <common.h>
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038A4A0;
extern dbFunction D_ovl8_8038A578;
extern dbUnknownLink D_ovl8_8038A6D0;

void func_ovl8_8037CE30(s32 arg0);

// 0x8037CA60
s32 func_ovl8_8037CA60(s32 arg0, s32** arg1)
{
	s32 sp34;
	s32* temp_v0;
	s32 var_v1;
	s32 var_t0;

	temp_v0 = arg1[0x24/4];
	var_v1 = 0;
	var_t0 = 0;
	if (temp_v0 != NULL)
	{
		var_v1 = temp_v0[0x0/4];
		var_t0 = temp_v0[0x4/4];
	}
	sp34 = func_ovl8_803717A0(0xB8);
	if (sp34 != 0)
	{
		func_ovl8_8037CBA0(sp34, 0, 0, arg1, var_v1, var_t0, arg0);
		var_v1 = sp34;
	}
	else
		var_v1 = 0;

	return var_v1;
}

// 0x8037CAF0
dbUnknown5* func_ovl8_8037CAF0(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if ((arg0 != NULL) || ((arg0 = func_ovl8_803717A0(0xB8)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x5C;
            arg2 = &arg0->unk_dbunk5_0x50;
            #line 44
            func_ovl8_803717E0(arg1, arg2);
            func_ovl8_8037C2D0(arg2);
            #line 51
        }
        func_ovl8_80373C10(arg0, arg1, arg2);
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038A4A0;
        arg1->db_func = &D_ovl8_8038A578;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038A6D0;
        func_ovl8_8037CE30(arg0);
    }
    return arg0;
}

// 0x8037CBA0
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_11_1/func_ovl8_8037CBA0.s")

// 0x8037CCC8
void func_ovl8_8037CCC8(dbUnknown5* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->unk_dbunk5_0x30 = &D_ovl8_8038A4A0,
		arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038A578;
		arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038A6D0;

		func_ovl8_80373D88(arg0, 0);

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

// 0x8037CD64
void func_ovl8_8037CD64(dbUnknown5* arg0) 
{
    dbFunction* temp_v0;
    dbUnknownLinkStruct *var_v0;
    dbUnknownLinkStruct* temp_v1;
    Vec2h sp28;
    Vec2h sp24;

    var_v0 = var_v0 = arg0->unk_dbunk5_0x0 != 0.0f ? (arg0->unk_dbunk5_0x40.ls != NULL ? arg0->unk_dbunk5_0x40.ls : arg0->unk_dbunk5_0x44.ls) : arg0->unk_dbunk5_0x44.ls;
    
    if (var_v0 != NULL) 
    {
        temp_v1 = arg0->unk_dbunk5_0x38;
        temp_v1->db_func[21].unk_dbfunc_0x4(temp_v1->db_func[21].unk_dbfunc_0x0 + (uintptr_t)temp_v1, &sp28);
        
        sp24.x = sp28.x;
        sp24.y = sp28.y;
        sp28.y = 0;
        sp28.x = 0;
        
        temp_v1 = arg0->unk_dbunk5_0x38;
        if (1);
        temp_v0 = temp_v1->db_func;
        
        func_ovl8_803787C0(var_v0, temp_v0[23].unk_dbfunc_0x4(temp_v0[23].unk_dbfunc_0x0 + (uintptr_t)temp_v1), &sp28, &sp24);
    }
}

// 0x8037CE30
void func_ovl8_8037CE30(s32 arg0) {}