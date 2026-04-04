#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038B9D0;
extern dbFunction D_ovl8_8038BAB0;
extern dbFunction D_ovl8_8038BC08;

dbUnknown5* func_ovl8_80380FC4(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4);

// 0x80380EC0
s32 func_ovl8_80380EC0(s32 arg0, s32 arg1)
{
    s32 sp24;
    s32 temp_v0 = func_ovl8_803717A0(0xB4);

    if (temp_v0 != 0)
    {
        sp24 = temp_v0;
        func_ovl8_80380FC4(temp_v0, 0, 0, arg1, arg0);
    }
    else
        sp24 = 0;

    return sp24;
}

// 0x80380F1C
dbUnknown5* func_ovl8_80380F1C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xB4)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x58;
            arg2 = &arg0->unk_dbunk5_0x4C;
            #line 33
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 40
        }

        func_ovl8_8037F300(arg0, arg1, arg2);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038B9D0;
        arg1->db_func = &D_ovl8_8038BAB0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038BC08;
    }

    return arg0;
}

// 0x80380FC4
dbUnknown5* func_ovl8_80380FC4(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4)
{
    dbFunction *temp_v1;

    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xB4)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x58;
            arg2 = &arg0->unk_dbunk5_0x4C;
            #line 60
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 67
        }

        func_ovl8_8037F3A8(arg0, arg1, arg2, arg3, arg4);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038B9D0,
        arg1->db_func = &D_ovl8_8038BAB0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038BC08;
        
        temp_v1 = arg0->unk_dbunk5_0x30;
        temp_v1[2].unk_dbfunc_0x4(temp_v1[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
    }

    return arg0;
}

// 0x80381090
void func_ovl8_80381090(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038B9D0,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038BAB0;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038BC08;

        func_ovl8_8037F4F4(arg0, 0);

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
