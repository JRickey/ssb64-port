#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038C1C0;
extern dbFunction D_ovl8_8038C2A0;
extern dbUnknownLink D_ovl8_8038C3F8;
extern dbUnknownLinkStruct D_ovl8_8038C420;
extern s32 D_ovl8_8038C488;

void func_ovl8_803817C0(void*, f32);
void func_ovl8_80373694(void*, f32);
dbUnknown5* func_ovl8_80382870(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5, s32 arg6);

// 0x80382710
dbUnknown5* func_ovl8_80382710(s32 arg0, DBMenu* arg1) {
    dbUnknown5* temp_v0_2;
    dbUnknown5* sp34;
    s32 sp2C;
    dbUnknown5* var_v1;

    if (arg1->unk_dbmenu_0x10 == 2) {
        sp2C = 2;
    } else if (arg1->unk_dbmenu_0x10 == 5) {
        sp2C = 3;
    }
    temp_v0_2 = func_ovl8_803717A0(0x140);
    
    if (temp_v0_2) {
        sp34 = temp_v0_2;
        func_ovl8_80382870(temp_v0_2, NULL, NULL, arg1, sp2C, arg1->unk_dbmenu_0x24, arg0);
        var_v1 = sp34;
    } else {
        var_v1 = NULL;
    }
    return var_v1;
}


// 0x803827A8
dbUnknown5* func_ovl8_803827A8(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0x140)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0xE4;
            arg2 = &arg0->unk_dbunk5_0xD8;
            #line 45
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 52
        }

        func_ovl8_8037E8C8(arg0, arg1, arg2);
        func_ovl8_80381130(arg0->unk_dbunk5_0xB8);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038C1C0;
        arg1->db_func = &D_ovl8_8038C2A0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038C3F8;
        arg0->unk_dbunk5_0x4C = &D_ovl8_8038C420;\
        arg0->unk_dbunk5_0xD4 = &D_ovl8_8038C488;
    }

    return arg0;
}

// 0x80382870
dbUnknown5* func_ovl8_80382870(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5, s32 arg6)
{
    s32 unused;
    
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0x140)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0xE4;
            arg2 = &arg0->unk_dbunk5_0xD8;
            #line 75
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 82
        }

        func_ovl8_8037E97C(arg0, arg1, arg2, arg3, arg4, 0, arg6);
        func_ovl8_8038116C(arg0->unk_dbunk5_0xB8, arg5, arg0);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038C1C0;
        arg1->db_func = &D_ovl8_8038C2A0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038C3F8;
        arg0->unk_dbunk5_0x4C = &D_ovl8_8038C420;\
        arg0->unk_dbunk5_0xD4 = &D_ovl8_8038C488;
    }

    return arg0;
}

// 0x8038295C
void func_ovl8_8038295C(dbUnknown5* arg0, s32 arg1) 
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038C1C0,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038C2A0;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038C3F8;

        arg0->unk_dbunk5_0x4C = &D_ovl8_8038C420;\
        arg0->unk_dbunk5_0xD4 = &D_ovl8_8038C488;

        #line 105
        func_ovl8_8038120C(&arg0->unk_dbunk5_0xB8, 0);
        #line 112
        func_ovl8_8037EB00(arg0, 0);

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

// 0x80382A20
void func_ovl8_80382A20(dbUnknown5* arg0)
{
    s32 current_value = arg0->unk_dbunk5_0x0;

    if (arg0->unk_dbunk5_0x1E == 0)
    {
        arg0->unk_dbunk5_0x0 = 0.0f;
    }

    func_ovl8_8037EBC8(arg0);

    arg0->unk_dbunk5_0x0 = current_value;
}

// 0x80382A80
void func_ovl8_80382A80(dbUnknown5 *arg0)
{
    func_ovl8_80381308(arg0->unk_dbunk5_0xB8);
}

// 0x80382AA0
void func_ovl8_80382AA0(dbUnknown5* arg0, f32 arg1)
    {
    func_ovl8_803817C0(arg0->unk_dbunk5_0xB8, arg1);
    func_ovl8_80373694(arg0, arg1);
}