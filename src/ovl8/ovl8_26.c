#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038D650;
extern dbFunction D_ovl8_8038D778;
extern dbUnknownLink D_ovl8_8038D8D0;
extern dbFunction D_ovl8_8038D8F8;

extern void func_ovl8_803724B4(void *, void *, void *, s32, s32);

void *func_ovl8_803851E4(dbUnknown5 *arg0, dbUnknownLinkStruct *arg1, dbUnknownLink *arg2, dbBytesCopy *arg3, s32 arg4, s32 arg5);

// 0x80385180
s32 func_ovl8_80385180(s32 arg0, s32* arg1)
{
	s32 temp_v0;
	s32 var_v1;

	temp_v0 = func_ovl8_803717A0(0xCC);
	if (temp_v0 != 0)
	{
		func_ovl8_803851E4(temp_v0, 0, 0, arg1, arg0, arg1[0x24/4]);
		var_v1 = temp_v0;
	}
	else
		var_v1 = 0;

	return var_v1;
}

// 0x803851E4
void *func_ovl8_803851E4(dbUnknown5 *arg0, dbUnknownLinkStruct *arg1, dbUnknownLink *arg2, dbBytesCopy *arg3, s32 arg4, s32 arg5) 
{
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    u16 temp_v1;
    
    if ((arg0 != NULL) || ((arg0 = func_ovl8_803717A0(0xCC)) != NULL)) 
    {
        if (arg1 == NULL) 
        {
            arg1 = &arg0->unk_dbunk5_0x60;\
            arg2 = &arg0->unk_dbunk5_0xC0;
            #line 42
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 49
        }
        
        func_ovl8_803847A0(arg0, arg1, arg2, arg3, arg4, arg5);
        func_ovl8_803724B4((char*) arg0 + 0x4C, arg1, arg3, arg4, 0);
        
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D650;\
        arg1->db_func = &D_ovl8_8038D778;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038D8D0;
        arg0->unk_dbunk5_0x58 = &D_ovl8_8038D8F8;
        temp_v1 = arg3->unk_dbbytescopy_0x10;
        
        if (temp_v1 == 2) {
            sp44 = 2;
        } else if (temp_v1 == 5){
            sp44 = 3;
        }
        
        sp40 = (s32) arg3->position.w;
        sp3C = (s32) arg3->position.h;
        func_ovl8_8037B85C(sp44, &sp40, &sp3C);
        arg0->unk_dbunk5_0xBC = func_ovl8_8037ACAC(arg3->position.w, arg3->position.h, sp44, sp40, sp3C);
        arg0->unk_dbunk5_0x54 = arg0->unk_dbunk5_0xBC;
    }
    return arg0;
}

// 0x8038533C
void func_ovl8_8038533C(dbUnknown5* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->unk_dbunk5_0x30 = &D_ovl8_8038D650,
		arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038D778;
		arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038D8D0;
		arg0->unk_dbunk5_0x58 = &D_ovl8_8038D8F8;

		if (arg0->unk_dbunk5_0xBC != 0) {
			func_ovl8_8037B3E4(arg0->unk_dbunk5_0xBC);
		}

		func_ovl8_80372554(&arg0->unk_dbunk5_0x4C, 0);
		func_ovl8_803848CC(arg0, 0);

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

// 0x80385408
void func_ovl8_80385408(s32 arg0)
{
	func_ovl8_803725DC(arg0 + 0x4C);
}

// 0x80385428
void func_ovl8_80385428(dbUnknownS38* arg0)
{
	dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
	temp_v0[2].unk_dbfunc_0x4(temp_v0[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
}
