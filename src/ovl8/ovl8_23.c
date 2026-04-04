#include "common.h"

#include <sys/develop.h>
#include <db/debug.h>

typedef struct {
	s32 unk0[0x118/4];
	s16 unk118;
	u16 unk11A;
	s32 (*unk11C)(void*, s32, void*);
} unkStructA;

typedef struct {
	u8 unk0[0x30];
	dbFunction* unk30;
	s32 unk34;
	dbUnknownLinkStruct* unk38;
	u8* unk3C;
	s16 unk40;
	s16 unk42;
	s16 unk44;
	s32 unk48;
	s32 unk4C;
	s32 unk50;
	s32 unk54;
} unkStructB;

extern dbUnknownLinkStruct D_ovl8_8038CA10;
extern dbFunction D_ovl8_8038CB58;
extern dbUnknownLink D_ovl8_8038CCB0;
extern s32 D_ovl8_8038CCD8;
extern char D_ovl8_8038CD8C[4];

void func_ovl8_80383B58(unkStructB* arg0);

// 0x803833A0
 dbUnknown5* func_ovl8_803833A0(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
  {
      if ((arg0 != NULL) || (arg0 = func_ovl8_803717A0(0xC0)) != NULL)
      {
          if (arg1 == NULL)
          {
              arg1 = &arg0->unk_dbunk5_0x58;
              arg2 = &arg0->unk_dbunk5_0xB4;
              #line 42
              func_ovl8_803717E0(arg1);
              func_ovl8_8037C2D0(arg2);
              #line 49
          }
          func_ovl8_803733AC(arg0, arg1, arg2);
          arg0->unk_dbunk5_0x30 = &D_ovl8_8038CA10;
          arg1->db_func = &D_ovl8_8038CB58;
          arg2->unk_dbunklink_0x8 = &D_ovl8_8038CCB0;
          func_ovl8_80383B58(arg0);
      }
      return arg0;
  }

// 0x80383450
dbUnknown5* func_ovl8_80383450(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, s32 arg3, s32 arg4)
{
    if ((arg0 != NULL) || (arg0 = func_ovl8_803717A0(0xC0)) != NULL)
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x58;
            arg2 = &arg0->unk_dbunk5_0xB4;
            #line 57
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 64
        }
        func_ovl8_8037345C(arg0, arg1, arg2, arg3, arg4);
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038CA10;
        arg1->db_func = &D_ovl8_8038CB58;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038CCB0;
        func_ovl8_80383B58(arg0);
    }
    return arg0;
}

// 0x80383510
void func_ovl8_80383510(dbUnknown5* arg0, s32 arg1)
{
	dbUnknownLinkStruct* temp_a0;

	if (arg0 == NULL)
		return;

	arg0->unk_dbunk5_0x30 = &D_ovl8_8038CA10, arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038CB58;
	arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038CCB0;
	temp_a0 = arg0->unk_dbunk5_0x3C.ls;

	if (temp_a0 != NULL)
		func_ovl8_80371764(temp_a0);

	func_ovl8_8037354C(arg0, 0);
	if (arg1 != 0)
	{
		func_ovl8_8037C30C(arg0->unk_dbunk5_0x34, 0);
		func_ovl8_803718C4(arg0->unk_dbunk5_0x38, 0);
	}

	if (arg1 & 1)
		func_ovl8_803717C0(arg0);
}

// 0x803835C4
s32 func_ovl8_803835C4(s32* arg0, s32 arg1)
{
	s32 temp_a2;
	dbUnknownLink**  temp_v0;

	temp_a2 = arg0[0x3C/4];
	if (temp_a2 != 0)
		func_ovl8_80371764(temp_a2);

	temp_v0 = func_ovl8_803716D8(arg1 + 1);
	arg0[0x3C/4] = temp_v0;

	if (temp_v0 != 0)
		return 1;

	return 0;
}

// 0x80383618
void func_ovl8_80383618(unkStructB* arg0, u8 arg1)
{
	dbUnknownLinkStruct* temp_v1;

	if (arg1 == 0)
		return;

	if (arg0->unk44 < arg0->unk40)
	{
		if (arg0->unk44 - arg0->unk42 > 0)
		{
			func_ovl8_8037B760(arg0->unk3C + arg0->unk42, arg0->unk3C + arg0->unk42 + 1, arg0->unk44 - arg0->unk42);
		}
		arg0->unk3C[arg0->unk42] = arg1;
		arg0->unk44 = (s16)(arg0->unk44 + 1);
		arg0->unk3C[arg0->unk44] = 0;
		temp_v1 = (void*)arg0->unk38;
		arg0->unk4C = 1;
		temp_v1 = (void*)temp_v1->db_func;
		((dbFunction*)temp_v1)[35].unk_dbfunc_0x4(((dbFunction*)temp_v1)[35].unk_dbfunc_0x0 + (uintptr_t)arg0->unk38, 1);
	}
}

// 0x803836D4
void func_ovl8_803836D4(dbUnknownS38* arg0, u16 arg1)
{
	s32 temp_v1 = arg1;

	arg0->unk_dbunks38_0x30[26].unk_dbfunc_0x4(arg0->unk_dbunks38_0x30[26].unk_dbfunc_0x0 + (uintptr_t)arg0, (u8)temp_v1 & 0xFF);

	if (temp_v1 & 0xFF00)
	{
		arg0->unk_dbunks38_0x30[26].unk_dbfunc_0x4(arg0->unk_dbunks38_0x30[26].unk_dbfunc_0x0 + (uintptr_t)arg0, (u8)(temp_v1 >> 8));
	}
}

// 0x8038374C
void func_ovl8_8038374C(unkStructB *arg0) {
	dbFunction* temp_v0;
	dbFunction* temp_v0_2;
	dbUnknownLinkStruct* temp_v1;

	if (arg0->unk42 < arg0->unk44) {
		temp_v0 = (dbFunction*)arg0->unk30;
		((void (*)())temp_v0[39].unk_dbfunc_0x4)(temp_v0[39].unk_dbfunc_0x0 + (uintptr_t)arg0);

		temp_v0_2 = (dbFunction*)arg0->unk30;
		if (temp_v0_2[31].unk_dbfunc_0x4(temp_v0_2[31].unk_dbfunc_0x0 + (uintptr_t)arg0) != 0) {
			arg0->unk42 = (s16)(arg0->unk42 + 2);
		} else {
			arg0->unk42 = (s16)(arg0->unk42 + 1);
		}

		temp_v1 = (dbUnknownLinkStruct*)arg0->unk38;
		temp_v0_2 = temp_v1->db_func;
		temp_v0_2[35].unk_dbfunc_0x4(temp_v0_2[35].unk_dbfunc_0x0 + (uintptr_t)temp_v1, 1);
	}
}

// 0x803837F0
void func_ovl8_803837F0(unkStructB* arg0) {
    dbFunction* temp_v0;
    dbUnknownLinkStruct* temp_v1;

    if (arg0->unk42 != NULL) {
        temp_v0 = arg0->unk30;
        ((void (*)())temp_v0[39].unk_dbfunc_0x4)(temp_v0[39].unk_dbfunc_0x0 + (uintptr_t)arg0);

        if (arg0->unk30[30].unk_dbfunc_0x4(arg0->unk30[30].unk_dbfunc_0x0 + (uintptr_t)arg0) != 0) {
            arg0->unk42 = (s16)(arg0->unk42 - 2);
        } else {
            arg0->unk42 = (s16)(arg0->unk42 - 1);
        }

        temp_v1 = arg0->unk38;
        temp_v0 = temp_v1->db_func;
        temp_v0[35].unk_dbfunc_0x4(temp_v0[35].unk_dbfunc_0x0 + (uintptr_t)temp_v1, 1);
    }
}

// 0x8038388C
void func_ovl8_8038388C(unkStructB* arg0)
{
	func_ovl8_8037B7F0(arg0->unk3C, arg0->unk42 - 1, arg0);
}

// 0x803838B8
void func_ovl8_803838B8(unkStructB* arg0)
{
	func_ovl8_8037B7F0(arg0->unk3C, arg0->unk42, arg0);
}

// 0x803838E0
void func_ovl8_803838E0(unkStructB* arg0) {
    s32 var_t0;
    s32 temp_t7;
    s32 var_t1;
    dbFunction* temp_v1;
    s16 var_a2;
    dbUnknownLinkStruct* temp_v0_3;
    dbFunction* temp_v1_3;

    if (arg0->unk42 != 0) {
        temp_v1 = arg0->unk30;
        var_t0 = arg0->unk42 - 1;
        var_t1 = 1;

        if (temp_v1[30].unk_dbfunc_0x4(temp_v1[30].unk_dbfunc_0x0 + (uintptr_t)arg0) != 0) {
            var_t0 -= 1;
            var_t1 = 2;
        }

        var_a2 = arg0->unk44;
        temp_t7 = var_a2 - arg0->unk42;

        if (temp_t7 > 0) {
            func_ovl8_8037B760(arg0->unk3C + arg0->unk42, arg0->unk3C + var_t0, temp_t7);
            var_a2 = arg0->unk44;
        }

        arg0->unk44 = (s16)(var_a2 - var_t1);
        arg0->unk3C[arg0->unk44] = 0;
        temp_v0_3 = arg0->unk38;
        arg0->unk42 = (s16)(arg0->unk42 - var_t1);
        arg0->unk4C = 1;
        temp_v1_3 = temp_v0_3->db_func;
        temp_v1_3[35].unk_dbfunc_0x4(temp_v1_3[35].unk_dbfunc_0x0 + (uintptr_t)temp_v0_3, 1);
    }
}

// 0x803839D0
void func_ovl8_803839D0(unkStructB *arg0, u8 *arg1) {
    arg0->unk48 = 1;
    arg0->unk42 = 0;
    arg0->unk44 = 0;

    while (*arg1 != 0) {
        u8 *v1 = arg1;
        arg1++; arg0->unk30[26].unk_dbfunc_0x4(arg0->unk30[26].unk_dbfunc_0x0 + (uintptr_t)arg0, *v1); arg0->unk42++;
    }

    arg0->unk48 = 0;
    arg0->unk4C = 1;
    arg0->unk38->db_func[35].unk_dbfunc_0x4(arg0->unk38->db_func[35].unk_dbfunc_0x0 + (uintptr_t)arg0->unk38, 1);
}

// 0x80383A78
s32 func_ovl8_80383A78(s32* arg0)
{
	return arg0[0x3c/4];
}

// 0x80383A80
s32 func_ovl8_80383A80(unkStructB *arg0, s32 arg1) {
    s32 var_s0;
    s32 var_s1;
    s32 temp_s3;
    s32 var_v0;
    u8 var_a0;
    u8 *new_var;
    s32 half;

    var_s0 = 0;
    var_s1 = 0;

    if (*arg0->unk3C != 0) {
        new_var = arg0->unk3C;
        var_a0 = *new_var;
        do {
            temp_s3 = var_s0;
            var_v0 = func_ovl8_8037E6F4(var_a0);
            if (arg0->unk3C[var_s0] & 0x80) {
                var_s0 += 1;
                var_v0 = func_ovl8_8037E6F4(arg0->unk3C[var_s0]);
            }
            var_s0 += 1;
            half = var_v0 / 2;
            if (var_s1 + half >= arg1) {
                return temp_s3;
            }
            var_s1 += var_v0;
            var_a0 = arg0->unk3C[var_s0];
        } while (var_a0 != 0);
    }

    return arg0->unk44;
}

// 0x80383B58
void func_ovl8_80383B58(unkStructB* arg0)
{
	dbFunction* db_func;
	db_func = arg0->unk30;
	arg0->unk3C = NULL;
	db_func[25].unk_dbfunc_0x4(db_func[25].unk_dbfunc_0x0 + (uintptr_t)arg0, 0x20, arg0);
	arg0->unk40 = 0x20;
	arg0->unk44 = 0;
	arg0->unk42 = 0;
	*arg0->unk3C = 0;
	arg0->unk50 = 0;
	arg0->unk54 = 0;
	arg0->unk48 = 0;
	arg0->unk4C = 0;
}

// 0x80383BC4
void func_ovl8_80383BC4(dbUnknown5 *arg0) {
    DBMenuPosition* sp5C;
    s32 unused;
    DBMenuPosition sp50;
    DBMenuPosition sp48;
    Vec2h sp44;
    db4Bytes sp40;
    dbFunction* temp_v1;
    s32 sp2C[4];
    s32 result;

    if (arg0->unk_dbunk5_0x48 != NULL) {
        return;
    }

    temp_v1 = arg0->unk_dbunk5_0x38->db_func;
    sp5C = temp_v1[23].unk_dbfunc_0x4(temp_v1[23].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);

    if (sp5C == NULL) {
        return;
    }

    if (arg0->unk_dbunk5_0x4C != NULL) {
        arg0->unk_dbunk5_0x4C = NULL;
        func_ovl8_8037D95C(sp2C);
        func_ovl8_8037E7A8(arg0->unk_dbunk5_0x3C.str);


        temp_v1 = arg0->unk_dbunk5_0x38->db_func;
        temp_v1[21].unk_dbfunc_0x4(temp_v1[21].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38, &sp50);

        func_ovl8_8037B46C(sp5C, &sp50.x, &D_ovl8_8038CCD8, &arg0->unk_dbunk5_0x38->bg_color);
        func_ovl8_8037D990(0x10);
        func_ovl8_8037D9D0(&arg0->unk_dbunk5_0x38->text_color);

        sp44.x = sp50.x + 6;\
        sp44.y = sp50.y + 6;
        func_ovl8_8037A5B8(sp5C, &sp44, &sp40);
        func_ovl8_8037D9B4(&sp40);

        temp_v1 = (dbFunction*)arg0->unk_dbunk5_0x30;
        temp_v1[37].unk_dbfunc_0x4(temp_v1[37].unk_dbfunc_0x0 + (uintptr_t)arg0, &sp48, 0);

        result = func_ovl8_8037E80C();
        func_ovl8_8037DFCC((s16)(sp50.x + sp48.x), (s16)(sp50.y + (sp50.h / 2) - (result / 2)));

        func_ovl8_8037DD60(sp5C, arg0->unk_dbunk5_0x3C.str);
        func_ovl8_8037D908(sp2C);
    }

    temp_v1 = (dbFunction*)arg0->unk_dbunk5_0x30;
    temp_v1[38].unk_dbfunc_0x4(temp_v1[38].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80383D4C
void func_ovl8_80383D4C(dbUnknown5* arg0)
{
	u64 time;
	dbFunction* db_func;

	time = ((u32)osGetTime() - arg0->unk_dbunk5_0x50) * 64ULL / 3000ULL;

	if (time >= 1000000)
	{
		db_func = arg0->unk_dbunk5_0x38->db_func;
		db_func[35].unk_dbfunc_0x4(db_func[35].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38, 1);
	}
}

// 0x80383DD4
void func_ovl8_80383DD4(dbUnknown5* arg0, DBMenuPosition* arg1, s32 arg2)
{
	u8 sp2F;
	DBMenuPosition sp24;
	dbFunction* temp_v0;

	temp_v0 = arg0->unk_dbunk5_0x38->db_func;
	temp_v0[21].unk_dbfunc_0x4(temp_v0[21].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38, &sp24);

	arg1->x = 2;

	if (arg2 > 0)
	{
		sp2F = arg0->unk_dbunk5_0x3C.str[arg2];
		arg0->unk_dbunk5_0x3C.str[arg2] = 0;
		arg1->x += func_ovl8_8037E7A8(arg0->unk_dbunk5_0x3C.str);
		arg0->unk_dbunk5_0x3C.str[arg2] = sp2F;
	}
	arg1->y = 2;
	arg1->w = 2;
	arg1->h = (s16) (sp24.h - 4);
}

// 0x80383E98
void func_ovl8_80383E98(dbUnknown5 *arg0) {
    void *sp3C;
    dbUnknownLinkStruct *temp_v1;
    Vec2h sp34;
    dbFunction *temp_v0;
    Vec2h sp2C;
    union { db4Bytes b; } sp28;
    s32 pad;

    sp28.b = *(db4Bytes*)D_ovl8_8038CD8C;
    temp_v1 = arg0->unk_dbunk5_0x38;
    temp_v0 = temp_v1->db_func;
    sp3C = (void*)temp_v0[23].unk_dbfunc_0x4(temp_v0[23].unk_dbfunc_0x0 + (uintptr_t)temp_v1);

    if (sp3C != NULL) {
        temp_v1 = arg0->unk_dbunk5_0x38;
        temp_v0 = temp_v1->db_func;
        temp_v0[21].unk_dbfunc_0x4(temp_v0[21].unk_dbfunc_0x0 + (uintptr_t)temp_v1, &sp34);

        if (1) {}
        temp_v0 = (dbFunction*)arg0->unk_dbunk5_0x30;
        temp_v0[37].unk_dbfunc_0x4(temp_v0[37].unk_dbfunc_0x0 + (uintptr_t)arg0, &sp2C, arg0->unk_dbunk5_0x40.v.y);

        sp2C.x += sp34.x;
        sp2C.y += sp34.y;

        func_ovl8_80377AEC(sp3C, &sp2C, &sp28.b, 1);

        arg0->unk_dbunk5_0x50 = osGetTime();
        arg0->unk_dbunk5_0x54 = 1;
    }
}

// 0x80383F74
void func_ovl8_80383F74(dbUnknownLinkStruct* arg0)
{
	dbFunction *db_func;
	if (arg0->unk_dbunkstruct_0x54 != NULL)
	{
		db_func = arg0->unk_dbunkstruct_0x38->db_func;
		db_func[35].unk_dbfunc_0x4(db_func[35].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunkstruct_0x38, 1);
		arg0->unk_dbunkstruct_0x54 = NULL;
	}
}

// 0x80383FC8
s32 func_ovl8_80383FC8(dbUnknownS38* arg0, db2Shorts arg1)
{
	dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
	return temp_v0[35].unk_dbfunc_0x4(temp_v0[35].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1.arr[0]) + 1;
}

// 0x80384000
typedef struct dbInputData {
    u8 unk0[0x8];
    db2Shorts unk8;
} dbInputData;

s32 func_ovl8_80384000(unkStructB* arg0, dbInputData* arg1) {
    db2Shorts sp24;
    dbUnknownLinkStruct* temp_a2;
    dbFunction* db_func;

    *(db2Shorts*)&sp24 = arg1->unk8;

    temp_a2 = (dbUnknownLinkStruct*)arg0->unk38;
    db_func = temp_a2->db_func;
    db_func[10].unk_dbfunc_0x4(db_func[10].unk_dbfunc_0x0 + (uintptr_t)temp_a2, &sp24);

    db_func = (dbFunction*)arg0->unk30;
    arg0->unk42 = db_func[20].unk_dbfunc_0x4(db_func[20].unk_dbfunc_0x0 + (uintptr_t)arg0, sp24) - 1;

    temp_a2 = (dbUnknownLinkStruct*)arg0->unk38;
    db_func = temp_a2->db_func;
    db_func[35].unk_dbfunc_0x4(db_func[35].unk_dbfunc_0x0 + (uintptr_t)temp_a2, 1, temp_a2);

    return 1;
}

