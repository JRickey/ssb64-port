#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038DC00;
extern dbFunction D_ovl8_8038DCD8;
extern dbUnknownLink D_ovl8_8038DE30;
extern dbUnknownLinkStruct D_ovl8_8038DE58;

dbUnknown5* func_ovl8_80385758(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, DBMenu *arg3, s32 arg4);

extern void func_ovl8_803724B4(void *, void *, void *, s32, s32);

// 0x80385640
s32 func_ovl8_80385640(s32 arg0, s32 arg1)
{
	s32 temp_v0;
	s32 var_v1;

	temp_v0 = func_ovl8_803717A0(0xC0);
	if (temp_v0 != 0)
	{
		func_ovl8_80385758(temp_v0, 0, 0, arg1, arg0);
		var_v1 = temp_v0;
	}
	else
		var_v1 = 0;

	return var_v1;
}

// 0x8038569C
dbUnknown5* func_ovl8_8038569C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xC0)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x54;\
            arg2 = &arg0->unk_dbunk5_0xB4;
            #line 37
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 44
        }

        func_ovl8_803820B0(arg0, arg1, arg2);
        func_ovl8_80372430(&arg0->unk_dbunk5_0x40, arg1);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038DC00,
        arg1->db_func = &D_ovl8_8038DCD8;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038DE30;
        arg0->unk_dbunk5_0x4C = &D_ovl8_8038DE58;
    }

    return arg0;
}

// 0x80385758
dbUnknown5* func_ovl8_80385758(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, DBMenu *arg3, s32 arg4)
{
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    u16 temp_v1;
    
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xC0)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x54;\
            arg2 = &arg0->unk_dbunk5_0xB4;
            #line 69
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 76
        }

        func_ovl8_8038215C(arg0, arg1, arg2, arg3, arg4, arg3->unk_dbmenu_0x24);
        func_ovl8_803724B4(&arg0->unk_dbunk5_0x40, arg1, arg3, arg4, 0);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038DC00,
        arg1->db_func = &D_ovl8_8038DCD8;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038DE30;
        arg0->unk_dbunk5_0x4C = &D_ovl8_8038DE58;
        temp_v1 = arg3->unk_dbmenu_0x10;
        
        if (temp_v1 == 2) 
        {
            sp3C = 2;
        } 
        else if (temp_v1 == 5)
        {
            sp3C = 3;
        }
        
        sp44 = (s32) arg3->position.w;
        sp40 = (s32) arg3->position.h;
        func_ovl8_8037B85C(sp3C, &sp44, &sp40);
        arg0->unk_dbunk5_0xB0 = func_ovl8_8037ACAC(arg3->position.w, arg3->position.h, sp3C, sp44, sp40);
        arg0->unk_dbunk5_0x48 = arg0->unk_dbunk5_0xB0;
    }

    return arg0;
}

// 0x803858B0
void func_ovl8_803858B0(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038DC00,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038DCD8;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038DE30;
        arg0->unk_dbunk5_0x4C = &D_ovl8_8038DE58;

        if (arg0->unk_dbunk5_0xB0 != 0) {
            func_ovl8_8037B3E4(arg0->unk_dbunk5_0xB0);
        }

        func_ovl8_80372554(&arg0->unk_dbunk5_0x40, 0);
        func_ovl8_80382224(arg0, 0);

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

// 0x8038597C
void func_ovl8_8038597C(s32 arg0)
{
    func_ovl8_803725DC(arg0 + 0x40);
}

// 0x8038599C
void func_ovl8_8038599C(dbFunction **arg0)
{
    s32 sp2C;
    dbFunction *temp_v0;
    s32 sp24;
    dbFunction *temp_v0_3;

    temp_v0 = arg0[19];
    sp2C = temp_v0[9].unk_dbfunc_0x4(temp_v0[9].unk_dbfunc_0x0 + (char*) arg0 + 0x40);

    if (sp2C == 0)
        return;

    temp_v0_3 = arg0[19];
    temp_v0_3[4].unk_dbfunc_0x4(temp_v0_3[4].unk_dbfunc_0x0 + (char*) arg0 + 0x40, &sp24);
    func_ovl8_80377AEC(sp2C, &sp24, arg0[14] + 3, 4);
    func_ovl8_80382354(arg0);
}

// 0x80385A1C
void func_ovl8_80385A1C(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[2].unk_dbfunc_0x4(temp_v0[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
}
