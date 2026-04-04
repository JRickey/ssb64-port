#include "common.h"
#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038D960;
extern dbFunction D_ovl8_8038DA80;
extern dbUnknownLink D_ovl8_8038DBD8;

void* func_ovl8_803854C4(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, s32 arg3, s32 arg4, s32 arg5);

// 0x80385460
s32 func_ovl8_80385460(s32 arg0, dbUnknown3* arg1)
{
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = func_ovl8_803717A0(0xB4);

    if (temp_v0 != 0)
    {
        func_ovl8_803854C4(temp_v0, 0, 0, arg1, arg0, arg1->unk_dbunk3_0x24);
        var_v1 = temp_v0;
    }
    else
    {
        var_v1 = 0;
    }

    return var_v1;
}

// 0x803854C4
void* func_ovl8_803854C4(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, s32 arg3, s32 arg4, s32 arg5)
{
    dbFunction *db_func;

    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xB4)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x58;
            arg2 = &arg0->unk_dbunk5_0x4C;
            #line 39
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 46
        }

        func_ovl8_803847A0(arg0, arg1, arg2, arg3, arg4, arg5);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D960;
        arg1->db_func = &D_ovl8_8038DA80;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038DBD8;

        db_func = arg0->unk_dbunk5_0x30;
        db_func[2].unk_dbfunc_0x4(db_func[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
    }

    return arg0;
}

// 0x80385598
void func_ovl8_80385598(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D960,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038DA80;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038DBD8;

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
