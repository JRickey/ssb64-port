#include <sys/develop.h>
#include <db/debug.h>

extern dbUnknownLinkStruct D_ovl8_8038CD90;
extern dbFunction D_ovl8_8038CEE0;
extern dbUnknownLink D_ovl8_8038D038;
extern dbUnknownLinkStruct D_ovl8_8038D060;

dbUnknown5* func_ovl8_803841E0(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, DBMenu* arg3, s32 arg4, s32 arg5);

// 0x803840C0
s32 func_ovl8_803840C0(s32 arg0, dbUnknown3* arg1)
{
	s32 sp24;
	s32 temp_v0 = func_ovl8_803717A0(0xD4);

	if (temp_v0 != 0)
	{
		sp24 = temp_v0;
		func_ovl8_803841E0(temp_v0, 0, 0, arg1, arg0, arg1->unk_dbunk3_0x24);
	}
	else
		sp24 = 0;

	return sp24;
}

// 0x80384124
dbUnknown5* func_ovl8_80384124(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xD4)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x6C;\
            arg2 = &arg0->unk_dbunk5_0xC8;
            #line 34
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 41
        }

        func_ovl8_803833A0(arg0, arg1, arg2);
        func_ovl8_80372430(&arg0->unk_dbunk5_0x58, arg1);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038CD90,
        arg1->db_func = &D_ovl8_8038CEE0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038D038;
        arg0->unk_dbunk5_0x64 = &D_ovl8_8038D060;
    }

    return arg0;
}

// 0x803841E0
dbUnknown5* func_ovl8_803841E0(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, DBMenu* arg3, s32 arg4, s32 arg5)
{
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 pad;
    
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xD4)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x6C;
            arg2 = &arg0->unk_dbunk5_0xC8;
            #line 66
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 73
        }

        func_ovl8_80383450(arg0, arg1, arg2, arg3, arg4);
        func_ovl8_80372430(&arg0->unk_dbunk5_0x58, arg1);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038CD90;
        arg1->db_func = &D_ovl8_8038CEE0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038D038;
        arg0->unk_dbunk5_0x64 = &D_ovl8_8038D060;

        if (arg5 != 0) {
            ((dbFunction*) arg0->unk_dbunk5_0x30)[33].unk_dbfunc_0x4(((dbFunction*) arg0->unk_dbunk5_0x30)[33].unk_dbfunc_0x0 + (uintptr_t)arg0, arg5);
        }
        
        if (arg3->unk_dbmenu_0x10 == 2) {
            sp3C = 2;
        } else if (arg3->unk_dbmenu_0x10 == 5){
            sp3C = 3;
        }
        
        sp38 = (s32) arg3->position.w;
        sp34 = (s32) arg3->position.h;
        func_ovl8_8037B85C(sp3C, &sp38, &sp34);
        arg0->unk_dbunk5_0x60 = func_ovl8_8037ACAC(arg3->position.w, arg3->position.h, sp3C, sp38, sp34);
    }

    return arg0;
}

// 0x80384340
void func_ovl8_80384340(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038CD90,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038CEE0;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038D038;
        arg0->unk_dbunk5_0x64 = &D_ovl8_8038D060;

        if (arg0->unk_dbunk5_0x60 != 0)
        {
            func_ovl8_8037B3E4(arg0->unk_dbunk5_0x60);
        }

        func_ovl8_80372554(&arg0->unk_dbunk5_0x58, 0);
        func_ovl8_80383510(arg0, 0);

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

// 0x8038440C
void func_ovl8_8038440C(s32 arg0)
{
	func_ovl8_803725DC(arg0 + 0x58);
}

// 0x8038442C
void func_ovl8_8038442C(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[2].unk_dbfunc_0x4(temp_v0[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
}
