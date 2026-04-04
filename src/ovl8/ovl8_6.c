#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>

extern dbFunction* D_ovl8_80389670;
extern dbFunction* D_ovl8_803896A8;
extern dbUnknownLink* D_ovl8_80389800;

typedef struct dbUnknownStruct_80375C54
{
	GObj* unk0;
	GObj* unk4;
} dbUnknownStruct_80375C54;

typedef struct dbUnknown6 {
    s32 unk_dbunk6_0x0;
    s32 unk_dbunk6_0x4;
    s32 unk_dbunk6_0x8;
    s32 unk_dbunk6_0xC;
    s32 unk_dbunk6_0x10;
    s32 unk_dbunk6_0x14;
    s32 unk_dbunk6_0x18;
} dbUnknown6;

typedef struct dbUnknown6_2 dbUnknown6_2;
struct dbUnknown6_2 {
    u8 unk_dbunk6_0x0[0x20];
    dbUnknown6_2* unk_dbunk6_0x20;
    s32 unk_dbunk6_0x24;
    s32 unk_dbunk6_0x28;
    dbUnknown6 unk_dbunk6_0x2C;
};

typedef struct dbUnknown6_3 dbUnknown6_3;
struct dbUnknown6_3 {
    u8 unk_dbunk6_0x0[0x20];
    dbUnknown6_2* unk_dbunk6_0x20;
    u8 unk_dbunk6_0x24[0x28 - 0x24];
    dbUnknown6_3* unk_dbunk6_0x28;
    u8 unk_dbunk6_0x2C[0x38 - 0x2C];
    dbUnknown6_2* unk_dbunk6_0x38;
};

typedef struct dbUnknownStructS24
{
    s32 unk_0x0;
    s32 unk_0x4;
    Sprite* unk_0x8;
    Sprite* unk_0xC;
    Sprite* unk_0x10;
    Sprite* unk_0x14;
    s32 unk_0x18;
    s32 unk_0x1C;
    dbUnknownLinkStruct* unk_0x20;
} dbUnknownStructS24;

extern void* D_ovl8_80389580;
extern void* D_ovl8_80389598;
extern void* D_ovl8_803895D4;
extern void* D_ovl8_803895F8;
extern void* D_ovl8_8038961C;
extern void* D_ovl8_80389634;

extern s16 D_ovl8_8038A864;
extern u64 D_ovl8_8038A878;
extern u64 D_ovl8_8038A880;

extern DBMenu D_ovl8_80389550;

extern dbTestMenu* D_8038EFC4_1AB814;
extern dbUnknownLinkStruct* D_8038EFC8_1AB818;

void func_ovl8_8037588C(dbUnknown6* arg0);

// 0x80374460
s32 func_ovl8_80374460(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0xC4);
    if (temp_v0 != NULL)
    {
        func_ovl8_8037C7D4(temp_v0, 0, 0, arg1, arg2, arg3, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x803744D4
s32 func_ovl8_803744D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0xB8);
    if (temp_v0 != NULL)
    {
        func_ovl8_8037CBA0(temp_v0, 0, 0, arg1, arg2, arg3, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x80374548
s32 func_ovl8_80374548(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0x120);
    if (temp_v0 != NULL)
    {
        func_ovl8_8037E97C(temp_v0, 0, 0, arg1, arg2, arg3, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x803745BC
s32 func_ovl8_803745BC(s32 arg0, s32 arg1, s32 arg2)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0xA8);
    if (temp_v0 != NULL)
    {
        func_ovl8_8037EF0C(temp_v0, 0, 0, arg1, arg2, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x80374624
s32 func_ovl8_80374624(s32 arg0, s32 arg1)
{
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = func_ovl8_803717A0(0xCC);

    if (temp_v0 != 0)
    {
        func_ovl8_8037FCD8(temp_v0, 0, 0, arg1, arg0);
        var_v1 = temp_v0;
    }
    else
    {
        var_v1 = 0;
    }

    return var_v1;
}

// 0x80374680
s32 func_ovl8_80374680(s32 arg0, s32 arg1)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0xB4);
    if (temp_v0 != NULL)
    {
        func_ovl8_80380FC4(temp_v0, 0, 0, arg1, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x803746DC
s32 func_ovl8_803746DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0x140);
    if (temp_v0 != NULL)
    {
        func_ovl8_80382870(temp_v0, 0, 0, arg1, arg2, arg3, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x80374750
s32 func_ovl8_80374750(s32 arg0, s32 arg1, s32 arg2)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0xC8);
    if (temp_v0 != NULL)
    {
        func_ovl8_80382B44(temp_v0, 0, 0, arg1, arg2, arg0);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x803747B8
s32 func_ovl8_803747B8(s32 arg0, dbUnknown5 *arg1)
{
    dbUnknownLinkStruct *temp_v0;
    dbUnknownLinkStruct *var_v1;

    temp_v0 = func_ovl8_803717A0(0xA8);
    if (temp_v0 != NULL)
    {
        func_ovl8_8038259C(temp_v0, 0, 0, arg1, arg0, arg1->unk_dbunk5_0x24);
        var_v1 = temp_v0;
    }
    else
        var_v1 = 0;

    return var_v1;
}

// 0x8037481C
void func_ovl8_8037481C(Vec3i* arg0, s32 arg1)
{
    func_ovl8_80373158(arg0, arg1);
}

// 0x8037483
void func_ovl8_8037483C(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x20->db_func;
    temp_v0[39].unk_dbfunc_0x4(temp_v0[39].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunks38_0x20);
}

// 0x8037486C
void func_ovl8_8037486C(dbUnknown3 *arg0)
{
    func_ovl8_80372C00(arg0);
}

// 0x8037488C
void func_ovl8_8037488C(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    if (arg0 != NULL)
    {
        temp_v0 = arg0->unk_dbunks38_0x18;
        temp_v0[1].unk_dbfunc_0x4(temp_v0[1].unk_dbfunc_0x0 + (uintptr_t)arg0, 3);
    }
}

// 0x803748C0
void func_ovl8_803748C0(dbUnknownS38* arg0, s32 arg1)
{
    dbFunction* temp_v0;
    dbFunction* temp_v0_2;

    if (arg1 != 0)
    {
        temp_v0 = arg0->unk_dbunks38_0x18;
        temp_v0[19].unk_dbfunc_0x4(temp_v0[19].unk_dbfunc_0x0 + (uintptr_t)arg0);
    }
    else {
        temp_v0 = arg0->unk_dbunks38_0x18;
        temp_v0[20].unk_dbfunc_0x4(temp_v0[20].unk_dbfunc_0x0 + (uintptr_t)arg0);
    }
}

// 0x80374910
void func_ovl8_80374910(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x18;
    temp_v0[6].unk_dbfunc_0x4(temp_v0[6].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x8037493C
void func_ovl8_8037493C(dbUnknownS38 *arg0, s32 arg1)
{
    func_ovl8_8037319C(arg0, arg1);
}

// 0x8037495C
void func_ovl8_8037495C(dbUnknown3 *arg0)
{
    func_ovl8_80372E98(arg0);
}

// 0x8037497C
void func_ovl8_8037497C(dbUnknown3 *arg0)
{
    func_ovl8_80372F04(arg0);
}

// 0x8037499C
void func_ovl8_8037499C(s32 arg0)
{
    func_ovl8_8037C5E8(arg0);
}

// 0x803749BC
void func_ovl8_803749BC(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x20->db_func;
    temp_v0[23].unk_dbfunc_0x4(temp_v0[23].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunks38_0x20);
}

// 0x803749EC
void func_ovl8_803749EC(dbUnknownS38* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x18;
	// WHAT IN TARNATION?!?
    temp_v0[5].unk_dbfunc_0x4(temp_v0[5].unk_dbfunc_0x0 + (uintptr_t) (u64) (s32) (u64) (s32) (u64) arg0, arg1, arg2, arg3, arg4);
}

// 0x80374A28
void func_ovl8_80374A28(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x18;
    temp_v0[3].unk_dbfunc_0x4(temp_v0[3].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374A54
void func_ovl8_80374A54(dbUnknownLinkStruct* arg0, DBMenu* arg1) 
{
    dbUnknownLinkStruct* var_v0;

    var_v0 = arg0;
    if (arg0 != NULL) 
    {
        var_v0 = arg0->unk_dbunkstruct_0x20;
    }
    arg1->position = var_v0->position;
    
    var_v0 = arg0;
    if (arg0 != NULL) 
    {
        var_v0 = arg0->unk_dbunkstruct_0x20;
    }
    arg1->text_color = var_v0->text_color;
    
    var_v0 = arg0;
    if (arg0 != NULL) 
    {
        var_v0 = arg0->unk_dbunkstruct_0x20;
    }
    arg1->bg_color = var_v0->bg_color;
    
    var_v0 = arg0;
    if (arg0 != NULL) 
    {
        var_v0 = arg0->unk_dbunkstruct_0x20;
    }
    arg1->unk_dbmenu_0x18 = var_v0->unk_dbunkstruct_0xC;
    
    var_v0 = arg0;
    if (arg0 != NULL) 
    {
        var_v0 = arg0->unk_dbunkstruct_0x20;
    }
    arg1->id = var_v0->id;
}

// 0x80374AE4
void func_ovl8_80374AE4(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[5].unk_dbfunc_0x4(temp_v0[5].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374B10
void func_ovl8_80374B10(dbUnknownS38* arg0, f32 arg1)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    ((void (*)(void*, f32))temp_v0[8].unk_dbfunc_0x4)(temp_v0[8].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1);
}

// 0x80374B44
void func_ovl8_80374B44(dbUnknownS38* arg0, f32 arg1)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    ((void (*)(void*, f32))temp_v0[11].unk_dbfunc_0x4)(temp_v0[11].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1);
}

// 0x80374B78
void func_ovl8_80374B78(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[6].unk_dbfunc_0x4(temp_v0[6].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374BA4
void func_ovl8_80374BA4(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[7].unk_dbfunc_0x4(temp_v0[7].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374BD0
void func_ovl8_80374BD0(dbUnknownS38* arg0, f32 arg1)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    ((void (*)(void*, f32))temp_v0[10].unk_dbfunc_0x4)(temp_v0[10].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1);
}

// 0x80374C04
void func_ovl8_80374C04(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[14].unk_dbfunc_0x4(temp_v0[14].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374C30
void func_ovl8_80374C30(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    if (arg0 != NULL)
    {
        temp_v0 = arg0->unk_dbunks38_0x30;
        temp_v0[1].unk_dbfunc_0x4(temp_v0[1].unk_dbfunc_0x0 + (uintptr_t)arg0, 3);
    }
}

// 0x80374C64
void func_ovl8_80374C64(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[16].unk_dbfunc_0x4(temp_v0[16].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374C90
void func_ovl8_80374C90(dbUnknown5* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunk5_0x38->db_func;
    temp_v0[39].unk_dbfunc_0x4(temp_v0[39].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
}

// 0x80374CC0
s32 func_ovl8_80374CC0(dbUnknown5* arg0)
{
    dbUnknownLinkStruct* temp_v1 = arg0->unk_dbunk5_0x38;
    return temp_v1->unk_dbunkstruct_0x4C;
}

// 0x80374CCC
void func_ovl8_80374CCC(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[26].unk_dbfunc_0x4(temp_v0[26].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374CF8
void* func_ovl8_80374CF8(dbUnknown5* arg0)
{
    return arg0->unk_dbunk5_0x40.ls;
}

// 0x80374D00
void func_ovl8_80374D00(dbUnknownS38* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[17].unk_dbfunc_0x4(temp_v0[17].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80374D2C
void func_ovl8_80374D2C(dbUnknown5* arg0)
{
    dbFunction* temp_v0 = arg0->unk_dbunk5_0x38->db_func;
    temp_v0[23].unk_dbfunc_0x4(temp_v0[23].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
}

// 0x80374D5C
void func_ovl8_80374D5C(dbUnknown5* arg0, s32 arg1)
{
    dbFunction* temp_v0;

    if (arg1 != 0)
    {
        temp_v0 = arg0->unk_dbunk5_0x38->db_func;
        temp_v0[29].unk_dbfunc_0x4(temp_v0[29].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
    }
    else {
        temp_v0 = arg0->unk_dbunk5_0x38->db_func;
        temp_v0[30].unk_dbfunc_0x4(temp_v0[30].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
    }
}

// 0x80374DB4
void func_ovl8_80374DB4(dbUnknown5* arg0, s32 arg1)
{
    dbFunction* temp_v0;

    if (arg1 != 0)
    {
        temp_v0 = arg0->unk_dbunk5_0x38->db_func;
        temp_v0[2].unk_dbfunc_0x4(temp_v0[2].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
    }
    else {
        temp_v0 = arg0->unk_dbunk5_0x38->db_func;
        temp_v0[3].unk_dbfunc_0x4(temp_v0[3].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38);
    }
}

// 0x80374E0C
void func_ovl8_80374E0C(GObj* arg0)
{
    D_8038EFC4_1AB814->unk_dbtestmenu_0x4 = arg0;
}

// 0x80374E1C
void func_ovl8_80374E1C(s32 arg0)
{
    D_8038EFC8_1AB818->unk_dbunkstruct_0x64 = arg0;
}

// 0x80374E2C
s32 func_ovl8_80374E2C(s32 arg0, Vec2h* arg1) 
{
    dbUnknown5* sp54;
    dbUnknown5* sp50;
    dbUnknownS38 *sp4C;
    DBMenu sp24;
    dbUnknown5* sp20;
    
    sp24 = D_ovl8_80389550;
    sp24.position.x = arg1->x;
    sp24.position.y = arg1->y;
    
    sp54 = func_ovl8_803717A0(0x130);
    
    if (sp54 != NULL) 
    {
        func_ovl8_80380048(sp54, 0, 0, &sp24, arg0);
        sp20 = sp54;
    } 
    else 
    {
        sp20 = NULL;
    }

    sp50 = sp20;

    sp4C = func_ovl8_80380414(sp20);

    if (sp20 != NULL) 
    {
        sp20 = sp50->db_func;
        ((dbFunction*)(sp20))[1].unk_dbfunc_0x4(((dbFunction*)(sp20))[1].unk_dbfunc_0x0 + (uintptr_t)sp50, 3);
    }
    
    return sp4C;
}

// 0x80374F18
void func_ovl8_80374F18(s16 arg0, s16 arg1, u16 arg2, u16 arg3)
{
    dbUnknownLinkStruct* temp_v0;

    temp_v0 = func_ovl8_8037C0CC();
    temp_v0->position.x = arg0;
    temp_v0->position.y = arg1;
    temp_v0->position.w = arg2;
    temp_v0->position.h = arg3;
    temp_v0->unk_dbunkstruct_0x5C = temp_v0->position;
}

// 0x80374F84
void func_ovl8_80374F84(s16 arg0, s16 arg1, u16 arg2, u16 arg3)
{
    dbUnknownLinkStruct* temp_v0;

    temp_v0 = func_ovl8_8037C0CC();
    temp_v0->unk_dbunkstruct_0x5C.x = arg0;
    temp_v0->unk_dbunkstruct_0x5C.y = arg1;
    temp_v0->unk_dbunkstruct_0x5C.w = arg2;
    temp_v0->unk_dbunkstruct_0x5C.h = arg3;
}

// 0x80374FD0
void func_ovl8_80374FD0(dbUnknown6_3* arg0, dbUnknown6* arg1)
{
    arg0->unk_dbunk6_0x20->unk_dbunk6_0x2C = *arg1;
}

// 0x80375010
void func_ovl8_80375010(dbUnknown6_3* arg0, dbUnknown6* arg1)
{
    arg0->unk_dbunk6_0x38->unk_dbunk6_0x2C = *arg1;
}

// 0x80375050
void func_ovl8_80375050(dbUnknown6_3* arg0, dbUnknown6* arg1) 
{
    dbUnknown6_3 *var_v1 = func_ovl8_80373160(arg0);

    if (var_v1 == NULL)
        return;
    
    while (var_v1 != NULL)
    {
        var_v1->unk_dbunk6_0x38->unk_dbunk6_0x2C = *arg1;
        var_v1 = var_v1->unk_dbunk6_0x28;
    }
}

// 0x803750CC
void func_ovl8_803750CC(s16 arg0)
{
    D_ovl8_8038A864 = arg0;
}

// 0x803750E4
void func_ovl8_803750E4(u64 arg0, u64 arg1) 
{
    D_ovl8_8038A878 = arg0;
    D_ovl8_8038A880 = arg1;
}

// 0x80375118 Fills info into MenuPositions and passes them to db_funcs
void func_ovl8_80375118(dbUnknownLinkStruct* targetLinkStruct) {
    void* gptr;
    s16 MenuPositions[6];
    dbUnknownLinkStruct* linkedStruct;
    
    func_ovl8_80377108((char*)MenuPositions, 0, sizeof(MenuPositions));
    linkedStruct = targetLinkStruct->unk_dbunkstruct_0x38->unk_dbunkstruct_0x4C;
    
    gptr = targetLinkStruct;
    if (gptr){
        gptr = targetLinkStruct->unk_dbunkstruct_0x38;
    }  
    MenuPositions[4] = ((dbUnknownLinkStruct*)(gptr))->position.x;
    
    gptr = targetLinkStruct;
    if (gptr){
        gptr = targetLinkStruct->unk_dbunkstruct_0x38;
    }
    MenuPositions[5] = ((dbUnknownLinkStruct*)(gptr))->position.y;

    gptr = linkedStruct->unk_dbunkstruct_0x20->db_func;
    ((dbFunction*)(gptr))[9].unk_dbfunc_0x4(((dbFunction*)(gptr))[9].unk_dbfunc_0x0 + (uintptr_t) linkedStruct->unk_dbunkstruct_0x20, &MenuPositions[4]);

    gptr = targetLinkStruct->unk_dbunkstruct_0x38->db_func;
    ((dbFunction*)(gptr))[18].unk_dbfunc_0x4(((dbFunction*)(gptr))[18].unk_dbfunc_0x0 + (uintptr_t) targetLinkStruct->unk_dbunkstruct_0x38, MenuPositions);
}


// 0x803751C0
dbUnknown7* func_ovl8_803751C0(dbUnknown7* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0x8C)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk30;
            arg2 = &arg0->unk24;
            #line 679
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 686
        }

        arg0->unk20 = arg1;
        arg0->unk1C = arg2;
        arg0->unk18 = &D_ovl8_80389670;
        arg1->db_func = &D_ovl8_803896A8;
        arg2->unk_dbunklink_0x8 = &D_ovl8_80389800;

        func_ovl8_8037588C(arg0);
    }

    return arg0;
}

// 0x80375270
void func_ovl8_80375270(dbUnknownS38* arg0, s32 arg1) {
    if (arg0 != NULL) 
    {
        arg0->unk_dbunks38_0x18 = &D_ovl8_80389670,
		arg0->unk_dbunks38_0x20->db_func = &D_ovl8_803896A8;
		arg0->unk_dbunks38_0x1C->unk_dbunklink_0x8 = &D_ovl8_80389800;
        
        if (arg0->unk_dbunks38_0x8 != 0) {
            func_ovl8_8037B3E4(arg0->unk_dbunks38_0x8);
        }
        
        if (arg0->unk_dbunks38_0xC != 0) {
            func_ovl8_8037B3E4(arg0->unk_dbunks38_0xC);
        }
        
        if (arg0->unk_dbunks38_0x10 != 0) {
            func_ovl8_8037B3E4(arg0->unk_dbunks38_0x10);
        }
        
        if (arg0->unk_dbunks38_0x14 != 0) {
            func_ovl8_8037B3E4(arg0->unk_dbunks38_0x14);
        }
        
        if (arg1 != 0) 
        {
            func_ovl8_8037C30C(arg0->unk_dbunks38_0x1C, 0);
            func_ovl8_803718C4(arg0->unk_dbunks38_0x20, 0);
        }
        if (arg1 & 1) 
        {
            func_ovl8_803717C0(arg0);
        }
    }
}

// 0x80375354
void func_ovl8_80375354(dbUnknownStructS24* arg0) 
{
    s32 unused;
    DBMenuPosition sp44;
    dbFunction* temp_s0;
    dbUnknownLinkStruct* temp_v0;

    if ((arg0->unk_0x8 != NULL) && (arg0->unk_0xC != NULL) && (arg0->unk_0x10 != NULL) && (arg0->unk_0x14 != NULL)) 
    {
        temp_v0 = arg0->unk_0x20;
        temp_s0 = temp_v0->db_func;
        temp_s0[20].unk_dbfunc_0x4(temp_s0[20].unk_dbfunc_0x0 + (uintptr_t)temp_v0, &sp44);
        if (temp_s0);
        
        spMove(arg0->unk_0x8, sp44.x - arg0->unk_0x0, sp44.y - arg0->unk_0x4);
        spMove(arg0->unk_0xC, sp44.x - arg0->unk_0x0, sp44.y);
        spMove(arg0->unk_0x10, sp44.x + sp44.w, sp44.y);
        spMove(arg0->unk_0x14, sp44.x - arg0->unk_0x0, sp44.y + sp44.h);

        if (gSYTaskmanDLHeads);

        arg0->unk_0x8->rsp_dl_next = gSYTaskmanDLHeads[0];
        spDraw(arg0->unk_0x8);
        gSYTaskmanDLHeads[0] = arg0->unk_0x8->rsp_dl_next - 1;
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
        
        arg0->unk_0xC->rsp_dl_next = gSYTaskmanDLHeads[0];
        spDraw(arg0->unk_0xC);
        gSYTaskmanDLHeads[0] = arg0->unk_0xC->rsp_dl_next - 1;
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
        
        arg0->unk_0x10->rsp_dl_next = gSYTaskmanDLHeads[0];
        spDraw(arg0->unk_0x10);
        gSYTaskmanDLHeads[0] = arg0->unk_0x10->rsp_dl_next - 1;
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
        
        arg0->unk_0x14->rsp_dl_next = gSYTaskmanDLHeads[0];
        spDraw(arg0->unk_0x14);
        gSYTaskmanDLHeads[0] = arg0->unk_0x14->rsp_dl_next - 1;
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
    }
}

// 0x80375528
void func_ovl8_80375528(struct dbUnknownStructS1C* arg0, s32 arg1, s32 arg2) {
    s32 unk8;
    arg0->unk_0x0 = arg1;
    arg0->unk_0x4 = arg2;
    
    unk8 = arg0->unk_0x8;

    if (unk8) {
        func_ovl8_8037B3E4(arg0->unk_0x8);
    }

    if (arg0->unk_0xC) {
        func_ovl8_8037B3E4(arg0->unk_0xC);
    }

    if (arg0->unk_0x10) {
        func_ovl8_8037B3E4(arg0->unk_0x10);
    }
    
    if (arg0->unk_0x14) {
        func_ovl8_8037B3E4(arg0->unk_0x14);
    }
    
    arg0->dbfunc[4].unk_dbfunc_0x4(arg0->dbfunc[4].unk_dbfunc_0x0 + (uintptr_t)arg0);
}


// 0x803755C4
void func_ovl8_803755C4(dbUnknownS38* arg0) 
{
    DBMenuPosition sp48;
    DBMenuPosition sp40;
    DBMenuPosition sp38;
    s32 var_s1;
    dbUnknownLinkStruct* var_v0;

    var_v0 = arg0;
    if (arg0 != NULL) 
    {
        var_v0 = arg0->unk_dbunks38_0x20;
    }
    if (var_v0->unk_dbunkstruct_0x24);
    var_s1 = (var_v0->unk_dbunkstruct_0x24 != 4) ? 2 : 3;
    
    arg0->unk_dbunks38_0x20->db_func[20].unk_dbfunc_0x4(arg0->unk_dbunks38_0x20->db_func[20].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunks38_0x20, &sp48);
    
    if ((sp48.w) && (sp48.h)) 
    {
        sp40.x = sp40.y = 0;
        sp40.w = sp48.w + (arg0->unk_dbunks38_0x0 * 2);
        sp40.h = arg0->unk_dbunks38_0x4;
        arg0->unk_dbunks38_0x8 = func_ovl8_8037ACAC(sp40.w, sp40.h, var_s1, 0x18, 0x18);
        func_ovl8_8037B46C(arg0->unk_dbunks38_0x8, &sp40, &D_ovl8_80389580, &arg0->unk_dbunks38_0x20->text_color);
        
        *(DBMenuPosition*)&sp38 = *(DBMenuPosition*)&sp40;
        sp38.x = (sp38.x + arg0->unk_dbunks38_0x0) - 2;
        sp38.w = (sp38.w - (arg0->unk_dbunks38_0x0 * 2)) + 4;
        func_ovl8_8037B46C(arg0->unk_dbunks38_0x8, &sp38, &D_ovl8_80389598, &arg0->unk_dbunks38_0x20->text_color);
        
        sp40.x = sp40.y = 0;
        sp40.w = arg0->unk_dbunks38_0x0;
        sp40.h = sp48.h;
        arg0->unk_dbunks38_0xC = func_ovl8_8037ACAC(sp40.w, sp48.h, var_s1, 0x18, 0x18);
        func_ovl8_8037B46C(arg0->unk_dbunks38_0xC, &sp40, &D_ovl8_803895D4, &arg0->unk_dbunks38_0x20->text_color);
        
        sp40.w = arg0->unk_dbunks38_0x0;
        sp40.h = sp48.h;
        arg0->unk_dbunks38_0x10 = func_ovl8_8037ACAC(sp40.w, sp48.h, var_s1, 0x18, 0x18);
        func_ovl8_8037B46C(arg0->unk_dbunks38_0x10, &sp40, &D_ovl8_803895F8, &arg0->unk_dbunks38_0x20->text_color);
        
        sp40.w = sp48.w + (arg0->unk_dbunks38_0x0 * 2);
        sp40.h = arg0->unk_dbunks38_0x4;
        arg0->unk_dbunks38_0x14 = func_ovl8_8037ACAC(sp40.w, sp40.h, var_s1, 0x18, 0x18);
        func_ovl8_8037B46C(arg0->unk_dbunks38_0x14, &sp40, &D_ovl8_8038961C, &arg0->unk_dbunks38_0x20->text_color);
        
        *(DBMenuPosition*)&sp38 = *(DBMenuPosition*)&sp40;
        sp38.x = (sp38.x + arg0->unk_dbunks38_0x0) - 2;
        sp38.w = (sp38.w - (arg0->unk_dbunks38_0x0 * 2)) + 4;
        func_ovl8_8037B46C(arg0->unk_dbunks38_0x14, &sp38, &D_ovl8_80389634, &arg0->unk_dbunks38_0x20->text_color);
    }
}

// 0x80375864
void func_ovl8_80375864(dbUnknown6* arg0, s32* arg1, s32* arg2, s32* arg3, s32* arg4)
{
    *arg1 = arg0->unk_dbunk6_0x8;
    *arg2 = arg0->unk_dbunk6_0x14;
    *arg3 = arg0->unk_dbunk6_0x10;
    *arg4 = arg0->unk_dbunk6_0xC;
}

// 0x8037588C
void func_ovl8_8037588C(dbUnknown6* arg0)
{
    arg0->unk_dbunk6_0x14 = 0;
    arg0->unk_dbunk6_0x10 = 0;
    arg0->unk_dbunk6_0xC = 0;
    arg0->unk_dbunk6_0x8 = 0;
    arg0->unk_dbunk6_0x0 = 4;
    arg0->unk_dbunk6_0x4 = 4;
}