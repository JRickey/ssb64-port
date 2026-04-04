#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>

extern db4Bytes D_ovl8_80389F4C;
extern db4Bytes D_ovl8_80389F50;
extern dbUnknownLinkStruct D_ovl8_8038AB70;
extern dbFunction D_ovl8_8038AC48;
extern dbUnknownLink D_ovl8_8038ADA0;

void func_ovl8_8037DFCC(s16, s16);

// 0x8037EE00
s32 func_ovl8_8037EE00(s32 arg0, dbUnknown3* arg1)
{
	s32 sp24;
	s32 temp_v0 = func_ovl8_803717A0(0xA8);

	if (temp_v0 != 0)
	{
		sp24 = temp_v0;
		func_ovl8_8037EF0C(temp_v0, 0, 0, arg1, arg1->unk_dbunk3_0x24, arg0);
	}
	else
		sp24 = 0;

	return sp24;
}

// 0x8037EE64
dbUnknown5* func_ovl8_8037EE64(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2) 
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xA8)) != NULL)) 
    {
        if (arg1 == NULL) 
        {
            arg1 = &arg0->unk_dbunk5_0x4C;
            arg2 = &arg0->unk_dbunk5_0x40;
            #line 36
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 43
        }
        
        func_ovl8_80373C10(arg0, arg1, arg2);
        
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038AB70;
        arg1->db_func = &D_ovl8_8038AC48;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038ADA0;
        
    } 
    
    return arg0;
}

// 0x8037EF0C
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_14/func_ovl8_8037EF0C.s")

// 0x8037F030
void func_ovl8_8037F030(dbUnknown5* arg0) 
{
    s32 sp5C;
    s32 sp58;
    DBMenuPosition sp50;
    Vec2h sp4C;
    db4Bytes sp48;
    s32 unused;
    s32 unused2;
    s32 a0;
    s32 a1;
    dbUnknownS14* sp34;
    dbFunction* temp_v1;
    f32 unused3;

    sp58 = func_ovl8_8037E7A8(&arg0->unk_dbunk5_0xC.str);
    
    func_ovl8_8037D95C(&sp34);

    temp_v1 = arg0->unk_dbunk5_0x38->db_func;
    sp5C = temp_v1[23].unk_dbfunc_0x4(temp_v1[23].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
    
    temp_v1 = arg0->unk_dbunk5_0x38->db_func;
    temp_v1[21].unk_dbfunc_0x4(temp_v1[21].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38, &sp50);
    
    func_ovl8_8037D990(0x10);

    if (arg0->unk_dbunk5_0x0 != 0.0F) 
    {
        func_ovl8_8037B434(sp5C, &sp50, 1, &arg0->unk_dbunk5_0x38->bg_color);
        func_ovl8_8037D9D0(&D_ovl8_80389F50);
        
        sp4C.x = sp50.x + 6,
        sp4C.y = sp50.y + 6;
        func_ovl8_8037A5B8(sp5C, &sp4C, &sp48);
        func_ovl8_8037D9B4(&sp48);

        a0 = ((sp50.x + sp50.w / 2) - (sp58 / 2)) + 1;
        a1 = sp50.y + 3;
        func_ovl8_8037DFCC(a0, a1);
    } 
    else 
    {
        func_ovl8_8037B434(sp5C, &sp50, 0, &arg0->unk_dbunk5_0x38->bg_color);
        func_ovl8_8037D9D0(&D_ovl8_80389F4C);
        
        sp4C.x = sp50.x + 6,
        sp4C.y = sp50.y + 6;
        func_ovl8_8037A5B8(sp5C, &sp4C, &sp48);
        func_ovl8_8037D9B4(&sp48);

        a1 = (sp50.y + (sp50.h / 2)) - (func_ovl8_8037E80C() / 2);
        a0 = ((sp50.x + sp50.w / 2) - (sp58 / 2));
        func_ovl8_8037DFCC(a0, a1);
    }
    
    func_ovl8_8037DD60(sp5C, &arg0->unk_dbunk5_0xC.str);
    func_ovl8_8037D908(&sp34);
}

// 0x8037F260
void func_ovl8_8037F260(dbUnknown5* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->unk_dbunk5_0x30 = &D_ovl8_8038AB70;\
		arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038AC48;
		arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038ADA0;

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