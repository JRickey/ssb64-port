#include <sys/develop.h>
#include <db/debug.h>

typedef struct dbUnknownThing {
    u8 dbUnknownThing_0x0[0x3C - 0x0];
    s16 dbUnknownThing_0x3C;
    s16 dbUnknownThing_0x3E;
    u8 dbUnknownThing_0x40[0x44 - 0x40];
    s16 dbUnknownThing_0x44;
    s16 dbUnknownThing_0x46;
} dbUnknownThing;

typedef struct DbThingAtArg0_unk34 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ dbFunction *fn;  
    /* 0x0C */ s32 unkC;
    // ...
} DbThingAtArg0_unk34;

extern dbUnknownLinkStruct D_ovl8_8038D0D0;
extern dbFunction D_ovl8_8038D218;
extern dbUnknownLink D_ovl8_8038D370;
extern dbUnknownLinkStruct D_ovl8_8038D3A0;
extern dbFunction D_ovl8_8038D4C0;
extern dbUnknownLink D_ovl8_8038D618;
extern SYColorRGBA D_ovl8_8038D640; // = { 0xFF, 0x00, 0x00, 0xFF };

void func_ovl8_8037726C(db2Shorts *arg0);
extern void func_ovl8_8037DFCC(s16, s16);
dbUnknown5* func_ovl8_8038456C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5);
void func_ovl8_80384AE8(dbUnknown5_2* arg0, s32* arg1);

// 0x80384460
s32 func_ovl8_80384460(s32 arg0, dbUnknown3* arg1)
{
	s32 sp24;
	s32 temp_v0 = func_ovl8_803717A0(0xC0);

	if (temp_v0 != 0)
	{
		sp24 = temp_v0;
		func_ovl8_8038456C(temp_v0, 0, 0, arg1, arg0, arg1->unk_dbunk3_0x24);
	}
	else
		sp24 = 0;

	return sp24;
}

// 0x803844C4
dbUnknown5* func_ovl8_803844C4(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2)
{
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xC0)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x64;
            arg2 = &arg0->unk_dbunk5_0x58;
            #line 56
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 63
        }

        func_ovl8_803833A0(arg0, arg1, arg2);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D0D0;
        arg1->db_func = &D_ovl8_8038D218;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038D370;
    }

    return arg0;
}

// 0x8038456C
dbUnknown5* func_ovl8_8038456C(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, dbUnknown3* arg3, s32 arg4, s32 arg5)
{
    dbFunction *db_func;
    
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xC0)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x64;
            arg2 = &arg0->unk_dbunk5_0x58;
            #line 83
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 90
        }

        func_ovl8_80383450(arg0, arg1, arg2, arg3, arg4);

        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D0D0;
        arg1->db_func = &D_ovl8_8038D218;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038D370;

        if (arg5 != 0) {
            db_func = arg0->unk_dbunk5_0x30;
            db_func[33].unk_dbfunc_0x4(db_func[33].unk_dbfunc_0x0 + (uintptr_t)arg0, arg5);
        }
        
        db_func = arg0->unk_dbunk5_0x30;
        db_func[2].unk_dbfunc_0x4(db_func[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
    }

    return arg0;
}

// 0x80384654
void func_ovl8_80384654(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D0D0,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038D218;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038D370;

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

// 0x803846F0
s16 func_ovl8_803846F0(dbUnknownS38* arg0)
{
    return arg0->unk_dbunks38_0x38.position.w;
}

// 0x803846F8
void func_ovl8_803846F8(dbUnknownS38* arg0, s32 arg1)
{
    arg0->unk_dbunks38_0x30[26].unk_dbfunc_0x4(arg0->unk_dbunks38_0x30[26].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1);
    arg0->unk_dbunks38_0x30[2].unk_dbfunc_0x4(arg0->unk_dbunks38_0x30[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80384748
void func_ovl8_80384748(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[29].unk_dbfunc_0x4(temp_v0[29].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80384774
void func_ovl8_80384774(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[30].unk_dbfunc_0x4(temp_v0[30].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x803847A0
dbUnknown5* func_ovl8_803847A0(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, DBMenu* arg3, s32 arg4, s32 arg5)
{
    dbUnknownLinkStruct* ls;
    
    if (arg0 != NULL || ((arg0 = func_ovl8_803717A0(0xB4)) != NULL))
    {
        if (arg1 == NULL)
        {
            arg1 = &arg0->unk_dbunk5_0x4C;
            arg2 = &arg0->unk_dbunk5_0xA8;
            #line 173
            func_ovl8_803717E0(arg1);
            func_ovl8_8037C2D0(arg2);
            #line 180
        }

        func_ovl8_8037345C(arg0, arg1, arg2, arg3, arg4);
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D3A0;
        arg1->db_func = &D_ovl8_8038D4C0;
        arg2->unk_dbunklink_0x8 = &D_ovl8_8038D618;
        arg0->unk_dbunk5_0x3C.s_16 = 1;
        arg0->unk_dbunk5_0x0 = 1.0F;
        arg0->unk_dbunk5_0x44.v.x = func_ovl8_8037E80C();

        ls = arg0;
        if (arg0 != NULL) {
            ls = arg0->unk_dbunk5_0x38;
        }
        
        arg0->unk_dbunk5_0x44.v.y = ls->position.h / arg0->unk_dbunk5_0x44.v.x;
        arg0->unk_dbunk5_0x48 = 0;
        func_ovl8_80384AE8(arg0, arg5);
    }

    return arg0;
}

// 0x803848CC
void func_ovl8_803848CC(dbUnknown5* arg0, s32 arg1)
{
    if (arg0 != NULL)
    {
        arg0->unk_dbunk5_0x30 = &D_ovl8_8038D3A0,
        arg0->unk_dbunk5_0x38->db_func = &D_ovl8_8038D4C0;
        arg0->unk_dbunk5_0x34->unk_dbunklink_0x8 = &D_ovl8_8038D618;

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

// 0x80384968
void func_ovl8_80384968(dbUnknown5* arg0) 
{
    s32 temp_v0_2;
    DBMenuPosition sp3C;
    DBMenuPosition sp34;
    void* temp_v0;
    s32 count;

    if (arg0->unk_dbunk5_0x40.str) 
    {
        temp_v0 = arg0->unk_dbunk5_0x38->db_func;
        temp_v0_2 = ((dbFunction*)temp_v0)[23].unk_dbfunc_0x4(((dbFunction*)temp_v0)[23].unk_dbfunc_0x0 + (uintptr_t) arg0->unk_dbunk5_0x38);
        
        if (temp_v0_2) 
        {
            temp_v0 = arg0->unk_dbunk5_0x38->db_func;
            ((dbFunction*)temp_v0)[21].unk_dbfunc_0x4(((dbFunction*)temp_v0)[21].unk_dbfunc_0x0 + (uintptr_t) arg0->unk_dbunk5_0x38, &sp34);
        
            func_ovl8_80377AEC(temp_v0_2, &sp34, &arg0->unk_dbunk5_0x38->bg_color, 4);
            
            for (count = 0; ((s16*)(&arg0->unk_dbunk5_0x44))[1] > count; count++) 
            {
                if (((s16*)(&arg0->unk_dbunk5_0x3C))[1] < (count + arg0->unk_dbunk5_0x3C.s_16)) 
                {
                    return;
                }
                
                sp3C.x  = sp34.x;
                sp3C.y = sp34.y + (count * arg0->unk_dbunk5_0x44.s_16);
                
                temp_v0 = arg0;
                if (arg0) 
                {
                    temp_v0 = arg0->unk_dbunk5_0x38;
                }
                
                sp3C.w = ((dbUnknownLinkStruct*)temp_v0)->position.w;
                sp3C.h  = arg0->unk_dbunk5_0x44.s_16;
                
                temp_v0 = arg0->unk_dbunk5_0x30;
                ((dbFunction*)temp_v0)[33].unk_dbfunc_0x4(((dbFunction*)temp_v0)[33].unk_dbfunc_0x0 + (uintptr_t) arg0, temp_v0_2, &sp3C, (s32) count + arg0->unk_dbunk5_0x3C.s_16);
                
                if (((s32)count + arg0->unk_dbunk5_0x3C.s_16)  == arg0->unk_dbunk5_0x0) 
                {
                    temp_v0 = arg0->unk_dbunk5_0x30;
                    ((dbFunction*)temp_v0)[34].unk_dbfunc_0x4(((dbFunction*)temp_v0)[34].unk_dbfunc_0x0 + (uintptr_t) arg0, temp_v0_2, &sp3C, count + arg0->unk_dbunk5_0x3C.s_16);
                }
            }
        }
    }
}

// 0x80384AE8
void func_ovl8_80384AE8(dbUnknown5_2* arg0, s32* arg1) {
    s32* var_v0;
    dbFunction* temp_v0;
    dbFunction* temp_v0_2;

    arg0->dbUnknown5_2_unk_0x3E = 0;
    var_v0 = arg1;
    
    for (var_v0 = arg1; var_v0 != NULL && *var_v0 != 0; var_v0++) {
        arg0->dbUnknown5_2_unk_0x3E++;
    }

    temp_v0 = arg0->dbUnknown5_2_db_func;
    arg0->dbUnknown5_2_unk_0x40 = arg1;
    
    ((sb32 (*)(uintptr_t, f32))(temp_v0[10].unk_dbfunc_0x4))(temp_v0[10].unk_dbfunc_0x0 + (uintptr_t) arg0, 1.0F);
    
    temp_v0_2 = arg0->dbUnknown5_2_db_func;
    ((sb32 (*)(uintptr_t, f32))(temp_v0_2[11].unk_dbfunc_0x4))(temp_v0_2[11].unk_dbfunc_0x0 + (uintptr_t)arg0, arg0->dbUnknown5_2_unk_0x3E);
}


// 0x80384B90
s32 func_ovl8_80384B90(dbUnknownThing* arg0, s32 arg1)
{
    if (arg1 <= 0)
    {
        return 0;
    }
    if (((arg0->dbUnknownThing_0x3E - arg0->dbUnknownThing_0x46) + 1) < arg1)
    {
        return 0;
    }
    arg0->dbUnknownThing_0x3C = arg1;
    return 1;
}

// 0x80384BD4
void func_ovl8_80384BD4(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[26].unk_dbfunc_0x4(temp_v0[26].unk_dbfunc_0x0 + (uintptr_t)arg0, arg0->unk_dbunks38_0x38.position.y + 1);
}

// 0x80384C0C
void func_ovl8_80384C0C(dbUnknownS38* arg0)
{
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunks38_0x30;
    temp_v0[26].unk_dbfunc_0x4(temp_v0[26].unk_dbfunc_0x0 + (uintptr_t)arg0, arg0->unk_dbunks38_0x38.position.y - 1);
}

// 0x80384C44
s16 func_ovl8_80384C44(s16* arg0)
{
	return arg0[0x3c/2];
}

// 0x80384C4C
void func_ovl8_80384C4C(dbUnknown5_2* arg0, s32 arg1) 
{
    dbFunction* db_func;

    if ((arg1 > 0) && (arg0->dbUnknown5_2_unk_0x3E >= arg1)) 
    {
        if (arg1 < arg0->dbUnknown5_2_unk_0x3C) 
        {
            db_func = arg0->dbUnknown5_2_db_func;
            db_func[26].unk_dbfunc_0x4(db_func[26].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1);
        } 
        else 
        {
            if (arg1 >= (arg0->dbUnknown5_2_unk_0x3C + arg0->dbUnknown5_2_unk_0x46)) 
            {
                db_func = arg0->dbUnknown5_2_db_func;
                db_func[26].unk_dbfunc_0x4(db_func[26].unk_dbfunc_0x0 + (uintptr_t)arg0, (arg1 - arg0->dbUnknown5_2_unk_0x46) + 1);
            }
        }
        if (arg0->dbUnknown5_2_unk_0x48 == 0) 
        {
            db_func = arg0->dbUnknown5_2_db_func;
            ((void (*)(uintptr_t, f32))db_func[8].unk_dbfunc_0x4)(db_func[8].unk_dbfunc_0x0 + (uintptr_t)arg0, (f32) arg1);
        }
        else
        {
            arg0->dbUnknown5_2_unk_f32_0x0 = (f32) arg1;
            db_func = arg0->dbUnknown5_2_db_func;
            db_func[2].unk_dbfunc_0x4(db_func[2].unk_dbfunc_0x0 + (uintptr_t)arg0);
        }
    }
}

// 0x80384D58
void func_ovl8_80384D58(dbUnknownS38 *arg0, s32 arg1) 
{
    ((void (*)(void*, s32))arg0->unk_dbunks38_0x30[30].unk_dbfunc_0x4)(
        arg0->unk_dbunks38_0x30[30].unk_dbfunc_0x0 + (uintptr_t)arg0, 
        ((f32 (*)(void*, s32))arg0->unk_dbunks38_0x30[5].unk_dbfunc_0x4)(arg0->unk_dbunks38_0x30[5].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1) - 1.0
    );
}

// 0x80384DC4
void func_ovl8_80384DC4(dbUnknownS38 *arg0, s32 arg1) {
    ((void (*)(void*, s32))arg0->unk_dbunks38_0x30[30].unk_dbfunc_0x4)(
        arg0->unk_dbunks38_0x30[30].unk_dbfunc_0x0 + (uintptr_t)arg0, 
        ((f32 (*)(void*, s32))arg0->unk_dbunks38_0x30[5].unk_dbfunc_0x4)(arg0->unk_dbunks38_0x30[5].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1) + 1.0
    );
}

// 0x80384E30
void func_ovl8_80384E30(dbUnknown3* arg0, DBMenuPosition* arg1, DBMenuPosition* arg2, s32 arg3) 
{
    s32 x, y;
    s32 unused1;
    s32 unused2;
    SYColorRGBA sp3C;
    s32 unused3;
    s32 unused4;
    DBMenuPosition sp2C;
    u32 sp28;
    

    func_ovl8_8037D95C(&sp3C);
    
    sp2C.x = arg2->x + 6;
    sp2C.y = arg2->y + 6;
    func_ovl8_8037A5B8(arg1, &sp2C, &sp28);
    func_ovl8_8037D9B4(&sp28);
    func_ovl8_8037D990(0x10);
    func_ovl8_8037D9D0(&arg0->unk_dbunk3_0x38->text_color);

    if (1);
    x = (arg2->x + (arg2->w / 2)) - func_ovl8_8037E7A8((arg0->unk_dbunk3_0x40)[arg3-1]) / 2;
    y = arg2->y;
    func_ovl8_8037DFCC(x, y);
    func_ovl8_8037DD60(arg1, arg0->unk_dbunk3_0x40[arg3-1]);
    func_ovl8_8037D908(&sp3C);
}

// 0x80384F3C
void func_ovl8_80384F3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    SYColorRGBA colors;

    *(SYColorRGBA*)&colors = *(SYColorRGBA*)&D_ovl8_8038D640;

    func_ovl8_80378064(arg1, arg2, &colors, 4);
}

// 0x80384F90
s32 func_ovl8_80384F90(dbUnknown5* arg0, s16 arg1)
{
    s32 var_v1;
    db4Shorts sp24;
    dbFunction* temp_v0;

    temp_v0 = arg0->unk_dbunk5_0x38->db_func;
    temp_v0[22].unk_dbfunc_0x4(temp_v0[22].unk_dbfunc_0x0 + (uintptr_t)arg0->unk_dbunk5_0x38, &sp24, arg0);

    var_v1 = (arg1 / arg0->unk_dbunk5_0x44.s_16) + arg0->unk_dbunk5_0x3C.s_16;

    if (arg1 < sp24.arr[1])
    {
        var_v1 -= 1;
    }
    return var_v1;
}

// 0x80385024
s32 func_ovl8_80385024(dbUnknown5* arg0, dbUnknown5* arg1) {
    db2Shorts sp3C;
    s32 temp_v0_2;
    dbUnknownLinkStruct* var_s0;
    dbUnknownLinkStruct* var_s0_2; 
    dbFunction* temp_v0;
    s32 var_s3 = 1;

    var_s0 = (dbUnknownLinkStruct*) arg0;
    if (arg0 != NULL) {
        var_s0 = arg0->unk_dbunk5_0x38;
    }
    while (func_ovl8_80377244() & var_s0->unk_dbunkstruct_0x30) {
        func_ovl8_8037726C(&sp3C);
        ((dbFunction*)(var_s0)) = arg0->unk_dbunk5_0x38->db_func;
        ((dbFunction*)(var_s0))[10].unk_dbfunc_0x4(((dbFunction*)(var_s0))[10].unk_dbfunc_0x0 + (uintptr_t) arg0->unk_dbunk5_0x38, &sp3C);
        ((dbFunction*)(var_s0)) = (dbFunction*) arg0->unk_dbunk5_0x30;
        temp_v0_2 = ((dbFunction*)(var_s0))[20].unk_dbfunc_0x4(((dbFunction*)(var_s0))[20].unk_dbfunc_0x0 + (uintptr_t) arg0, sp3C);
        if (arg0->unk_dbunk5_0x0 != temp_v0_2) {
            ((dbFunction*)(var_s0)) = (dbFunction*) arg0->unk_dbunk5_0x30;
            arg0->unk_dbunk5_0x48 = var_s3;
            ((dbFunction*)(var_s0))[30].unk_dbfunc_0x4(((dbFunction*)(var_s0))[30].unk_dbfunc_0x0 + (uintptr_t) arg0, temp_v0_2);
            arg0->unk_dbunk5_0x48 = 0;
        }
        gcSleepCurrentGObjThread(var_s3);
        var_s0 = (dbUnknownLinkStruct*) arg0;
        if (arg0 != NULL) {
            var_s0 = arg0->unk_dbunk5_0x38;
        }
    }
    var_s0 = (dbUnknownLinkStruct*) arg0;
    if (arg0 != NULL) {
        var_s0 = arg0->unk_dbunk5_0x38;
    }
    ((dbFunction*)(temp_v0)) = ((DbThingAtArg0_unk34*)arg0->unk_dbunk5_0x34)->fn;
    ((dbFunction*)(temp_v0))[3].unk_dbfunc_0x4(((dbFunction*)(temp_v0))[3].unk_dbfunc_0x0 + (uintptr_t) ((DbThingAtArg0_unk34*)arg0->unk_dbunk5_0x34), var_s0->unk_dbunkstruct_0xC, arg0);
    return 1;
}
