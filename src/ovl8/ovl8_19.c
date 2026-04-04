#include "common.h"

#include <sys/develop.h>

extern dbUnknownLinkStruct D_ovl8_8038BD20;
extern dbFunction D_ovl8_8038BDF0;
extern dbUnknownLink D_ovl8_8038BF48;
extern dbUnknownLinkStruct D_ovl8_8038BF70;
extern dbFunction D_ovl8_8038C040;
extern dbUnknownLink D_ovl8_8038C198;

extern void stringCopy(char* target, char* source);

void func_ovl8_803822D8(dbUnknownLinkStruct* arg0, s32 arg1);
dbUnknown5* func_ovl8_8038259C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5);

// 0x803820B0
dbUnknown5* func_ovl8_803820B0(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xA8)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x40;
            arg2 = &arg0->unk_dbunk5_0x9C;
            #line 23
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 30
        }

        func_ovl8_803733AC(arg0, arg1, arg2);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038BD20;
        arg1->db_func = &D_ovl8_8038BDF0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038BF48;
        arg0->unk_dbunk5_0x3C.ls = NULL;
    }

    return arg0;
}

// 0x8038215C
dbUnknown5* func_ovl8_8038215C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xA8)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x40;
            arg2 = &arg0->unk_dbunk5_0x9C;
            #line 49
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 56
        }

        func_ovl8_8037345C(arg0, arg1, arg2, arg3, arg4);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038BD20;
        arg1->db_func = &D_ovl8_8038BDF0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038BF48;
        arg0->unk_dbunk5_0x3C.ls = NULL;

        func_ovl8_803822D8(arg0, arg5);
    }

    return arg0;
}

// 0x80382224
void func_ovl8_80382224(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038BD20,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038BDF0;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038BF48;

        if (arg0->unk_dbunk5_0x3C.ls != 0)
        {
            func_ovl8_80371764(arg0->unk_dbunk5_0x3C.ls);
        }

        func_ovl8_8037354C(arg0, 0);

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

// 0x803822D8
void func_ovl8_803822D8(dbUnknownLinkStruct* arg0, s32 arg1)
{
	s32 temp_a2;
	s32 temp_v0;
	dbFunction *db_func;
	dbUnknownLinkStruct* temp_v1;

	temp_a2 = arg0->unk_dbunkstruct_0x3C;

	if (temp_a2 != 0)
		func_ovl8_80371764(temp_a2);

	temp_v0 = func_ovl8_803716D8(stringLength(arg1) + 1);
	arg0->unk_dbunkstruct_0x3C = temp_v0;
	stringCopy(temp_v0, arg1);

	db_func = arg0->unk_dbunkstruct_0x38->db_func;
	db_func[35].unk_dbfunc_0x4(db_func[35].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunkstruct_0x38, 1);
}

// 0x80382354
void func_ovl8_80382354(dbUnknown5 *arg0) 
{
    s32 sp44;
    s32 sp40;
    DBMenuPosition sp38;
    Vec2h pos;
    s32 sp30;
    dbUnknownLinkStruct* temp_a2_2;
    dbFunction* temp_v1;

    sp40 = func_ovl8_8037E7A8(arg0->unk_dbunk5_0x3C.str);
    
    temp_a2_2 = arg0->unk_dbunk5_0x38;
    temp_v1 = temp_a2_2->db_func;
    sp44 = temp_v1[23].unk_dbfunc_0x4(temp_v1[23].unk_dbfunc_0x0 + (uintptr_t)temp_a2_2);
    
    if (sp44) 
    {
        temp_a2_2 = arg0->unk_dbunk5_0x38;
        temp_v1 = temp_a2_2->db_func;
        temp_v1[21].unk_dbfunc_0x4(temp_v1[21].unk_dbfunc_0x0 + (uintptr_t)temp_a2_2, &sp38, temp_a2_2);
        
        func_ovl8_8037D990(0x10);
        func_ovl8_8037D9D0(&arg0->unk_dbunk5_0x38->text_color);
        
        pos.x = sp38.x + 6,
        pos.y = sp38.y + 6;
        
        func_ovl8_8037A5B8(sp44, &pos, &sp30);
        func_ovl8_8037D9B4(&sp30);
        func_ovl8_8037DFCC((s16) ((sp38.x + ((s32) sp38.w / 2)) - (sp40 / 2)), (s16) ((sp38.y + ((s32) sp38.h / 2)) - (func_ovl8_8037E80C() / 2)));
        func_ovl8_8037DD60(sp44, arg0->unk_dbunk5_0x3C.str);
    }
}

// 0x80382490
s32 func_ovl8_80382490(s32 arg0, s32* arg1)
{
	s32 temp_v0;
	s32 sp24;
	s32 var_v1;

	temp_v0 = func_ovl8_803717A0(0xA8);
	if (temp_v0 != 0)
	{
		sp24 = temp_v0;
		func_ovl8_8038259C(temp_v0, 0, 0, arg1, arg0, arg1[0x24/4]);
		var_v1 = sp24;
	}
	else
		var_v1 = 0;
	return var_v1;
}

// 0x803824F4
dbUnknown5* func_ovl8_803824F4(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xA8)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x4C;
            arg2 = &arg0->unk_dbunk5_0x40;
            #line 182
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 189
        }

        func_ovl8_803820B0(arg0, arg1, arg2);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038BF70;
        arg1->db_func = &D_ovl8_8038C040;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038C198;
    }

    return arg0;
}

// 0x8038259C
dbUnknown5* func_ovl8_8038259C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5)
{
    dbFunction* temp_v1;
    
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xA8)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x4C;
            arg2 = &arg0->unk_dbunk5_0x40;
            #line 209
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 216
        }

        func_ovl8_8038215C(arg0, arg1, arg2, arg3, arg4, arg5);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038BF70;
        arg1->db_func = &D_ovl8_8038C040;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038C198;

        temp_v1 = arg0->unk_dbunk5_0x30;
        temp_v1[2].unk_dbfunc_0x4(temp_v1[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
    }

    return arg0;
}

// 0x80382670
void func_ovl8_80382670(dbUnknownLinkStruct* arg0, s32 arg1)
{
	if (arg0 == NULL)
		return;
	arg0->unk_dbunkstruct_0x30 = &D_ovl8_8038BF70, arg0->unk_dbunkstruct_0x38->db_func = &D_ovl8_8038C040;
	arg0->unk_dbunkstruct_0x34[0x8/4] = &D_ovl8_8038C198;
	func_ovl8_80382224(arg0, 0);
	if (arg1 != 0)
	{
		func_ovl8_8037C30C(arg0->unk_dbunkstruct_0x34, 0);
		func_ovl8_803718C4(arg0->unk_dbunkstruct_0x38, 0);
	}
	if (arg1 & 1)
		func_ovl8_803717C0(arg0);
}