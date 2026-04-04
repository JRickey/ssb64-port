#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038DEC0;
extern dbFunction D_ovl8_8038DFA0;
extern dbFunction D_ovl8_8038E0F8;
extern dbUnknownLinkStruct D_ovl8_8038E120;
extern dbFunction D_ovl8_8038E188;

void func_ovl8_803817C0(void*, f32);
void func_ovl8_80373694(void*, f32);
dbUnknown5* func_ovl8_80385B74(dbUnknown5 *arg0, dbUnknownLinkStruct *arg1, dbUnknownLink *arg2, dbUnknown3* arg3, s32 arg4);

// 0x80385A50
s32 func_ovl8_80385A50(s32 arg0, s32 arg1)
{
	s32 sp24;
	s32 temp_v0 = func_ovl8_803717A0(0x140);

	if (temp_v0 != 0)
	{
		sp24 = temp_v0;
		func_ovl8_80385B74(temp_v0, 0, 0, arg1, arg0);
	}
	else
		sp24 = 0;

	return sp24;
}

// 0x80385AAC
dbUnknown5* func_ovl8_80385AAC(dbUnknown5 *arg0, dbUnknownLinkStruct *arg1, dbUnknownLink *arg2) 
{
    if ((arg0 != 0) || ((arg0 = (dbUnknown5 *) func_ovl8_803717A0(0x140)) != 0)) 
    {
        if (arg1 == NULL) 
        {
            arg1 = (dbUnknownLinkStruct *)&arg0->unk_dbunk5_0xD8;
            arg2 = (dbUnknownLink *)&arg0->unk_dbunk5_0x134;
            #line 37
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 44
        }
        func_ovl8_8037C710(arg0, arg1, arg2);
        func_ovl8_80381130(arg0->unk_dbunk5_0xB8);
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038DEC0;
        arg1->db_func = &D_ovl8_8038DFA0;
        arg2->unk_dbunklink_0x8 = (dbUnknownLink *)&D_ovl8_8038E0F8;
        arg0->unk_dbunk5_0x4C = (dbUnknownLinkStruct *)&D_ovl8_8038E120,\
        arg0->unk_dbunk5_0xD4 = (s32)&D_ovl8_8038E188;
    }
    return arg0;
}

// 0x80385B74
dbUnknown5* func_ovl8_80385B74(dbUnknown5 *arg0, dbUnknownLinkStruct *arg1, dbUnknownLink *arg2, dbUnknown3* arg3, s32 arg4) 
{
    s32 pad;
    
    if ((arg0 != 0) || ((arg0 = (dbUnknown5 *) func_ovl8_803717A0(0x140)) != 0)) 
    {
        if (arg1 == NULL) 
        {
            arg1 = (dbUnknownLinkStruct *)&arg0->unk_dbunk5_0xD8;
            arg2 = (dbUnknownLink *)&arg0->unk_dbunk5_0x134;
            #line 64
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 71
        }
        
        func_ovl8_8037C7D4(arg0, arg1, arg2, arg3, arg3->unk_dbunk3_0x24->unk_dbunk3_0x0, arg3->unk_dbunk3_0x24->unk_dbunk3_0x4, arg4);
        func_ovl8_8038116C(arg0->unk_dbunk5_0xB8, arg3->unk_dbunk3_0x24->unk_dbunk3_0x8, arg0);
        
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038DEC0;
        arg1->db_func = &D_ovl8_8038DFA0;
        arg2->unk_dbunklink_0x8 = (dbUnknownLink *)&D_ovl8_8038E0F8;
        arg0->unk_dbunk5_0x4C = (dbUnknownLinkStruct *)&D_ovl8_8038E120,\
        arg0->unk_dbunk5_0xD4 = (s32)&D_ovl8_8038E188;
    }
    
    return arg0;
}

// 0x80385C70
void func_ovl8_80385C70(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038DEC0,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038DFA0;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038E0F8;
        
        arg0->unk_dbunk5_0x4C = &D_ovl8_8038E120;\
        arg0->unk_dbunk5_0xD4 = &D_ovl8_8038E188;

        #line 94
        func_ovl8_8038120C(arg0->unk_dbunk5_0xB8, 0);
        #line 101
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

// 0x80385D34
void func_ovl8_80385D34(s32 arg0)
{
    func_ovl8_80381308(arg0 + 0xB8);
}

// 0x80385D54
void func_ovl8_80385D54(s32 arg0, f32 arg1)
{
    func_ovl8_803817C0(arg0 + 0xB8, arg1);
    func_ovl8_80373694(arg0, arg1);
}

// 0x80385D88
void func_ovl8_80385D88(dbUnknown5* arg0)
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
