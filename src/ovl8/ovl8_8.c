#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>

s32 func_ovl8_8037A6D4(db4Shorts *rect1, db4Shorts *rect2, db4Shorts *intersection);
void func_ovl8_8037AA88(s32 arg0, s32 arg1, s32 arg2, s32 arg3, dbUnknown7* arg4);

typedef struct dbUnknown8_SC {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s32 dbUnknown8_SC_0x8;
} dbUnknown8_SC;

typedef struct UiLineStepper {
    s32 startX;                 // dbUnknown8_8_0x0
    s32 startY;                 // dbUnknown8_8_0x4

    s32* errorSub;              // dbUnknown8_8_0x8   (usually dx or dy)
    s32* errorAdd;              // dbUnknown8_8_0xC   (usually 2*minor)

    s32 endX;                   // dbUnknown8_8_0x10
    s32 endY;                   // dbUnknown8_8_0x14

    s32* stepPrimary;           // dbUnknown8_8_0x18  (dx or dy)
    s32* stepSecondary;         // dbUnknown8_8_0x1C  (other axis)

    s32 currentX;               // dbUnknown8_8_0x20
    s32 currentY;               // dbUnknown8_8_0x24

    s32* posPrimary;            // dbUnknown8_8_0x28  (&currentX or &currentY)
    s32* posSecondary;          // dbUnknown8_8_0x2C  (&currentY or &currentX)

    s32 stepsRemaining;         // dbUnknown8_8_0x30
    s32 totalSteps;             // dbUnknown8_8_0x34

    s32 error;                  // dbUnknown8_8_0x38
} UiLineStepper;

void func_ovl8_80377B40(Sprite*, s16*);
void func_ovl8_80377F50(Sprite*, db4Shorts*);
void func_ovl8_80377FE4(Sprite*, db4Shorts*, db4Shorts*);
void func_ovl8_803780B8(Sprite*, DBMenuPosition*);
s32 func_ovl8_8037A67C(s16*, s16*, s16*);
void func_ovl8_8037A904(db4Shorts*, db4Shorts*);
void func_ovl8_8037A9C0(db4Shorts*, s32, s32);
void func_ovl8_8037A9F4(DBMenuPosition*, DBMenuPosition*);
void func_ovl8_8037AA28(db4Shorts*, db4Shorts*);
s32 func_ovl8_8037AA5C(DBMenuPosition*);
s32 func_ovl8_8037ABDC(Vec2h*, UiLineStepper*);
void func_ovl8_8037B46C(Sprite*, DBMenuPosition*, dbUnknown8_SC*, s32); 
void func_ovl8_8037BD44();
void func_ovl8_8037BEC8();
void func_ovl8_8037BF34();
void* func_ovl8_8037BDF4(GObj*);
void* func_ovl8_8037BD94(GObj*);

// BSS
extern s32 D_ovl8_8038EE60;
extern s32 D_ovl8_8038EE64;
extern s32 D_ovl8_8038EE68[];
extern s32 D_ovl8_8038EE70[];
extern Gfx D_8038EE90_1AB6E0[];

// DATA
s32 D_ovl8_80387CA0[] = { 0xDF000000, 0x00000000 };
s32 D_ovl8_80387CA8[]  =
{
	0xFC00FC00, 0xFC00FFFF, 0xFFFFFC40, 0xFC00FC00,
	0xFC00FC00, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0xFC00FC00, 0x0001FFFF,
	0xFC40FC00, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FFFF, 0x00010001, 0xFFFFFC40,
	0xFC00FC00, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0x0001FFFF, 0x00010001,
	0xFC00FC00, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FFFF, 0x00010001, 0x00010001,
	0xFFFFFC40, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0x00010001, 0x00010001,
	0xFC00FC00, 0x0001FFFF, 0xFC40FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FFFF, 0x00010001, 0x00010001,
	0x00010001, 0xFFFFFC40, 0xFC00FC00, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0x00010001, 0x00010001,
	0x0001FFFF, 0x00010001, 0xFC00FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FFFF, 0x00010001, 0x00010001,
	0x00010001, 0x00010001, 0xFFFFFC40, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0x00010001, 0x00010001,
	0xFFFFFFFF, 0x0001FFFF, 0xFC40FC00, 0xFFFFFFFF,
	0xFC00FC00, 0xFC00FFFF, 0x00010001, 0xFFFF0001,
	0x0001FFFF, 0xFC40FC00, 0xFC00FC00, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0xFC00FFFF, 0x0001FFFF,
	0xFFFFFC40, 0x00010001, 0xFC00FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FFFF, 0xFFFFFC40, 0xFC00FFFF,
	0x00010001, 0xFFFFFC40, 0xFC00FC00, 0xFC00FC00,
	0xFC00FFFF, 0xFC40FC00, 0xFC00FC00, 0xFC00FC00,
	0x0001FFFF, 0xFFFF0001, 0xFC40FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFFFF0001, 0x0001FFFF, 0xFC40FC00, 0xFC00FC00,
	0xFC00FC00, 0xFC00FC00, 0xFC00FC00, 0xFC00FC00,
	0xFFFFFFFF, 0xFC40FFFF, 0xFC40FC00, 0xFC00FC00
};

Bitmap D_ovl8_80387EA8 =
{
	16, 16,
	0, 0,
	D_ovl8_80387CA8,
	16, 0
};

Sprite D_ovl8_80387EB8 =
{
	0, 0,
	16, 16,
	1.0, 1.0,
	0, 0,
	0x200,
	0x1234,
	0xff, 0xff, 0xff, 0xff,
	0, 0,
	NULL,
	0, 1,
	1, 0x24,
	16, 16,
	G_IM_FMT_RGBA,
	G_IM_SIZ_16b,
	&D_ovl8_80387EA8,
	D_8038EE90_1AB6E0,
	NULL,
	0, 0
};

s32 D_ovl8_80387EFC = 0;

char D_ovl8_80387F00[] = "1";
char D_ovl8_80387F04[] = "2";
char D_ovl8_80387F08[] = "3";
char D_ovl8_80387F0C[] = "4";
char D_ovl8_80387F10[] = "5";
char D_ovl8_80387F14[] = "6";
char D_ovl8_80387F18[] = "7";
char D_ovl8_80387F1C[] = "8";
char D_ovl8_80387F20[] = "9";
char D_ovl8_80387F24[] = "0";
char D_ovl8_80387F28[] = ".";
char D_ovl8_80387F2C[] = "OK";
char D_ovl8_80387F30[] = "No";
char D_ovl8_80387F34[] = "B";
char D_ovl8_80387F38[] = "Caption";

extern s32 D_ovl8_80389F5C;
extern s32 D_ovl8_80389F60;
extern s32 D_ovl8_80389F64;
extern dbUnknown8_SC* D_ovl8_80389FB4;
extern dbUnknown8_SC* D_ovl8_8038A068;
extern DBMenu D_ovl8_8038A11C;
extern GObj *D_8038EFC0_1AB810;
extern dbUnkStruct *D_8038EFC4_1AB814;
extern dbUnknownLinkStruct* D_8038EFC8_1AB818;
extern dbTestMenu* D_8038EFCC_1AB81C;
extern dbUnknown8_S28 D_ovl8_8038A144;
extern s16 D_ovl8_80389F30;
extern s16 D_ovl8_80389F34;
extern s16 D_ovl8_80389F38;
extern s16 D_ovl8_80389F3C;
extern s16 D_ovl8_80389F40;
extern s16 D_ovl8_80389F44;

extern DBMenu D_ovl8_80389F68;

// 0x80376B60
void func_ovl8_80376B60(s32 arg0, u8* arg1, u32* arg2)
{
	switch (arg0)
	{
	case 1:
		*arg2 = (u32) arg1;
		return;
	case 2:
		*arg2 = ((u32)arg1[0] << 8) & 0xF800;
		*arg2 |= (((u32)arg1[1] * 8) & 0x7C0);
		*arg2 |= (((u32)arg1[2] >> 2) & 0x3E);
		*arg2 |= arg1[3] ? 1 : 0;
		return;
	case 3:
		*arg2 = ((u32)arg1[0] << 24) & 0xFF000000;
		*arg2 |= (((u32)arg1[1] << 16) & 0xFF0000);
		*arg2 |= (((u32)arg1[2] << 8) & 0xFF00);
		*arg2 |= (((u32)arg1[3] << 0) & 0xFF);
		return;
	}
}

// 0x80376C40
void func_ovl8_80376C40(s32 arg0, u32 arg1, u8* arg2)
{
	u32 var_v0;
	switch (arg0)
	{
	case 2:
		arg2[0] = (u32) (((f32) ((arg1 >> 0xB) & 0x1F) / 31.0) * 255.0);
		arg2[1] = (u32) (((f32) ((arg1 >> 6) & 0x1F) / 31.0) * 255.0);
		arg2[2] = (u32) (((f32) ((arg1 >> 1) & 0x1F) / 31.0) * 255.0);
		if (arg1 & 1)
			var_v0 = 0x255;
		else
			var_v0 = 0;
		arg2[3] = var_v0;
		return;
	case 3:
		arg2[0] = (arg1 >> 24) & 0xFF;
		arg2[1] = (arg1 >> 16) & 0xFF;
		arg2[2] = (arg1 >> 8) & 0xFF;
		arg2[3] = (arg1 >> 0) & 0xFF;
		return;
	}
}

// 0x80376EE0
void func_ovl8_80376EE0(u8* arg0, u8* arg1, s32 arg2)
{
	f64 temp_f0;

	temp_f0 = (f64) arg2;
	arg1[0] = (s32) (((f32) arg0[0] / 100.0) * temp_f0);
	arg1[1] = (s32) (((f32) arg0[1] / 100.0) * temp_f0);
	arg1[2] = (s32) (((f32) arg0[2] / 100.0) * temp_f0);
	arg1[3] = arg0[3];
}

// 0x80376FB4
s32 func_ovl8_80376FB4()
{
	return 2;
}

// 0x80376FBC
s32 stringLength(char* string)
{
	s32 length;

	if (string == NULL)
		return 0;

	for (length = 0; *string != '\0'; string++, length++);
	return length;
}

// 0x80376FFC
void stringCopy(char* target, char* source)
{
	char* sourceCurrent;
	char* targetCurrent;

	if (target == NULL || source == NULL)
		return;

	while (*source != '\0')
	{
		targetCurrent = target;
		sourceCurrent = source;
		target += 1;
		source += 1;
		*targetCurrent = *sourceCurrent;
	}

	*target = '\0';
}

// 0x80377044
void stringCopyCount(char* target, char* source, s32 count)
{
	char* sourceCurrent;
	char* targetCurrent;

	if (target == NULL || source == NULL)
		return;

	while (count != 0 && *source != '\0')
	{
		targetCurrent = target;
		sourceCurrent = source;
		count -= 1;
		target += 1;
		source += 1;
		*targetCurrent = *sourceCurrent;
	}

	*target = '\0';
}

// 0x8037709C
void stringConcat(char* target, char* source)
{
	char* sourceCurrent;
	char* targetCurrent;

	if (target == NULL || source == NULL)
		return;

	target += stringLength(target);

	while (*source != '\0')
	{
		targetCurrent = target;
		sourceCurrent = source;
		target += 1;
		source += 1;
		*targetCurrent = *sourceCurrent;
	}

	*target = '\0';
}

// 0x80377108 Some sort of fill function
void func_ovl8_80377108(char *dest, int value, int count)
{ 
    char * pos; 
    pos = count--;
    if(pos != 0){
        pos--;
        do {
            pos = dest;
            dest++; *pos = value;
        } while(count--); 
    } 
}


// 0x80377134
void func_ovl8_80377134(u8* arg0, s32 arg1)
{
	func_ovl8_80376B60(arg1, arg0, &D_ovl8_80389F64);
}

// 0x80377168
s32 func_ovl8_80377168(db2Shorts arg0, DBMenuPosition* arg1)
{
	if ((arg0.arr[0] >= arg1->x) && (arg0.arr[0] < (arg1->x + arg1->w)))
	{
		if ((arg0.arr[1] >= arg1->y) && (arg0.arr[1] < (arg1->y + arg1->h)))
		{
			return 1;
		}
	}

	return 0;
}

// 0x803771CC
void func_ovl8_803771CC(DBMenuPosition* arg0, DBMenuPosition* arg1)
{
	arg1->x = arg0->x;
	arg1->y = arg0->y;
	arg1->w = arg1->x + arg0->w - 1;
	arg1->h = arg1->y + arg0->h - 1;
}

// 0x80377208
void func_ovl8_80377208(DBMenuPosition* arg0, DBMenuPosition* arg1)
{
	arg1->x = arg0->x;
	arg1->y = arg0->y;
	arg1->w = (s16)arg0->w - arg0->x + 1;
	arg1->h = (s16)arg0->h - arg0->y + 1;
}

// 0x80377244
u32 func_ovl8_80377244()
{
	return func_ovl8_8037D28C() | gSYControllerMain.button_hold;
}

// 0x8037726C
void func_ovl8_8037726C(db2Shorts *arg0)
{
	func_ovl8_80375BB4(arg0);
}

// 0x8037728C
void func_ovl8_8037728C(Vec2h* arg0)
{
	func_ovl8_80375BD0(arg0);
}

// 0x803772AC
void func_ovl8_803772AC(dbUnknown5* arg0)
{
	DBMenu sp20;
	s16 arg1, arg2;
	dbFunction* db_func;

	func_ovl8_80374A54(arg0, &sp20);

	db_func = arg0->db_func;
	arg1 = 160 - (sp20.position.w / 2);
	arg2 = 120 - (sp20.position.h / 2);
	db_func[10].unk_dbfunc_0x4(db_func[10].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1, arg2);
}

// 0x80377330
void func_ovl8_80377330(dbUnknown5* arg0, s16 arg1, s16 arg2)
{
	dbFunction* db_func;

	db_func = arg0->db_func;
	db_func[10].unk_dbfunc_0x4(db_func[10].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1, arg2);
}

// 0x80377374
void func_ovl8_80377374(DBMenuPosition *arg0) 
{
    if (arg0 != NULL) 
    {
        D_ovl8_80389F68.position = *arg0;
    }
    else
    {
        D_ovl8_80389F68.position.x = D_ovl8_80389F68.position.y = 0;
        D_ovl8_80389F68.position.w = D_ovl8_80389F68.position.h = 32767;
    }
}

// 0x803773CC
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_803773CC.s")

// 0x80377AEC
void func_ovl8_80377AEC(Sprite* arg0, db4Shorts* arg1, u8* arg2, s32 arg3)
{
	func_ovl8_80376B60(arg0->bmsiz, arg2, &D_ovl8_80389F5C);
	D_ovl8_80389F60 = arg3;
	func_ovl8_80377B40(arg0, arg1);
}

// 0x80377B40
void func_ovl8_80377B40(Sprite *arg0, s16 *arg1) 
{
    DBMenuPosition sp90;
    db4Shorts sp88;
    db4Shorts sp80;
    s16 actualWidth;
    s32 i;
    s32 sp74;
    s32 sp70;
    s32 sp6C;
    s32 x;
    s32 y;
    s32 cols;
    s32 rows;
    s32 actualHeight;
    s32 j;
    s32 var_v1;

    sp80.arr[0] = sp80.arr[1] = 0;
    sp80.arr[2]  = arg0->width;
    sp80.arr[3]  = arg0->height;
    func_ovl8_8037A67C(arg1, &sp80, &sp90);
    
    if (func_ovl8_8037AA5C(&sp90) == 0) 
    {
        actualWidth = arg0->bitmap->width;
        actualHeight = arg0->bitmap->actualHeight;
        cols = sp90.x / actualWidth;
        rows = sp90.y / actualHeight;
        
        var_v1 = ((arg0->width % actualWidth) != 0) ? 1 : 0;
        sp6C = (arg0->width / actualWidth) + var_v1;
        
        var_v1 = ((sp90.w % actualWidth) != 0) ? 1 : 0;
        sp74 = (sp90.w / actualWidth) + var_v1 + 1;
        
        var_v1 = ((sp90.h % actualHeight) != 0) ? 1 : 0;
        sp70 = (sp90.h / actualHeight) + var_v1 + 1;
        
        for (i = 0; i < sp70; i++) 
        {
            for (j = 0; j < sp74; j++) 
            {
                x = cols + j;
                x *= actualWidth;
                
                y = rows + i;
                y *= actualHeight;

                sp88.arr[0] = x;
                sp88.arr[1] = y;
                sp88.arr[2] = actualWidth;
                sp88.arr[3] = actualHeight;
                if (func_ovl8_8037A67C(&sp88, &sp90, &sp80) != 0) 
                {
                    sp80.arr[0] %= actualWidth;
                    sp80.arr[1] %= actualHeight;
                    func_ovl8_803773CC(arg0->bitmap + ((rows + i) * sp6C) + j + cols, arg0->bmsiz, &sp80);
                }
            }
        }
    }
}

// 0x80377EFC
void func_ovl8_80377EFC(Sprite* arg0, db4Shorts* arg1, u8* arg2, s32 arg3)
{
	func_ovl8_80376B60(arg0->bmsiz, arg2, &D_ovl8_80389F5C);
	D_ovl8_80389F60 = arg3;
	func_ovl8_80377F50(arg0, arg1);
}

// 0x80377F50
void func_ovl8_80377F50(Sprite* arg0, db4Shorts* arg1)
{
	db4Shorts sp18;

	sp18.arr[0] = arg1->arr[0];
	sp18.arr[1] = arg1->arr[1];
	sp18.arr[2] = sp18.arr[3] = 1;
	func_ovl8_80377B40(arg0, &sp18);
}

// 0x80377F90
void func_ovl8_80377F90(char* arg0, db4Shorts* arg1, s32 arg2, u8* arg3, s32 arg4)
{
	func_ovl8_80376B60(arg0[0x31], arg3, &D_ovl8_80389F5C);
	D_ovl8_80389F60 = arg4;
	func_ovl8_80377FE4(arg0, arg1, arg2);
}

// 0x80377FE4
void func_ovl8_80377FE4(Sprite* arg0, db4Shorts* arg1, db4Shorts* arg2)
{
	s32 temp_s0;
	s32 unused[12];
	db4Shorts sp3C;
	db4Shorts sp34;

	func_ovl8_8037AA88(arg1->arr[0], arg1->arr[1], arg2->arr[0], arg2->arr[1], &sp3C);

	do
	{
		temp_s0 = func_ovl8_8037ABDC(&sp34, &sp3C);
		func_ovl8_80377F50(arg0, &sp34);
	}
	while (temp_s0 > 0);
}

// 0x80378064
void func_ovl8_80378064(Sprite* arg0, db4Shorts* arg1, u8* arg2, s32 arg3)
{
	func_ovl8_80376B60(arg0->bmsiz, arg2, &D_ovl8_80389F5C);
	D_ovl8_80389F60 = arg3;
	func_ovl8_803780B8(arg0, arg1);
}

// 0x803780B8
void func_ovl8_803780B8(Sprite* arg0, DBMenuPosition* arg1)
{
	DBMenuPosition sp20;

	sp20.x = arg1->x;
	sp20.y = arg1->y;
	sp20.w = arg1->w;
	sp20.h = 1;
	func_ovl8_80377B40(arg0, &sp20);

	sp20.x = arg1->x;
	sp20.y = arg1->y + arg1->h - 1;
	sp20.w = arg1->w;
	sp20.h = 1;
	func_ovl8_80377B40(arg0, &sp20);

	sp20.x = arg1->x;
	sp20.y = arg1->y;
	sp20.w = 1;
	sp20.h = arg1->h;
	func_ovl8_80377B40(arg0, &sp20);

	sp20.x = arg1->x + arg1->w - 1;
	sp20.y = arg1->y;
	sp20.w = 1;
	sp20.h = arg1->h;
	func_ovl8_80377B40(arg0, &sp20);
}

// 0x803781A4
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_803781A4.s")

// 0x803787C0
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_803787C0.s")

// 0x80379070
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_80379070.s")

// 0x803798A0
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_803798A0.s")

// 0x80379D74
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_80379D74.s")

// 0x8037A0FC
void func_ovl8_8037A0FC(Sprite* sprite, DBMenuPosition* dst_rect, u8* dst_buffer) 
{
    DBMenuPosition spB0; // src_rect
    DBMenuPosition spA8; // intersection
    DBMenuPosition spA0; // tile_rect
    DBMenuPosition sp98; // tile_clip
    s32 s0; // tile_x
    s32 sp90; // tile_y
    s32 sp8C; // num_tiles_x
    s32 sp88; // num_tiles_y
    s32 sp84; // tiles_per_row
    s32 s3; // bitmap_width
    s32 s6; // bitmap_height
    s32 sp78; // tile_x_start
    s32 sp74; // tile_y_start
    s32 sp70; // dst_stride
    s32 sp6C; // bytes_per_pixel
    u8* s1; // dst_row_ptr
    s32 s7;
    Bitmap* bitmap;
    u8* sp5C; // dst_buffer_offset
    
    // Copy destination rect
    *(DBMenuPosition*)&spB0 = *dst_rect;
    
    // Determine bytes per pixel
    sp6C = sp6C = (sprite->bmsiz == 1) ? 1 : (sprite->bmsiz == 2) ? 2 : 4;

    // Set up sprite bounds
    sp98.x = sp98.y = 0;
    sp98.w = sprite->width;
    sp98.h = sprite->height;

    // Clip to sprite bounds
    func_ovl8_8037A67C(&spB0, &sp98, &spA8);

    if (func_ovl8_8037AA5C(&spA8) == 0) 
    {
        // Calculate starting buffer offset
        sp5C = dst_buffer;
        s7 = (spA8.x - spB0.x);
        sp5C += (((spA8.y - spB0.y) * dst_rect->w + s7) * sp6C);
        
        s3 = sprite->bitmap->width;
        s6 = sprite->bitmap->actualHeight;
        
        // Calculate starting tile indices
        sp78 = spA8.x / s3;
        sp74 = spA8.y / s6;
        
        // Calculate tiles per row
        sp84 = (sprite->width / s3) + (sprite->width % s3 ? 1 : 0) + (0, 0);
        
        // Calculate number of tiles to cover in X
        sp8C = (spA8.w / s3) + (spA8.w % s3 ? 1 : 0) + (0, 1);
        
        // Calculate number of tiles to cover in Y
        sp88 = (spA8.h / s6) + (spA8.h % s6 ? 1 : 0) + (0, 1);
        
        // Loop through tiles vertically
        for (sp90 = 0; sp90 < sp88; sp90++)
        {
            s1 = sp5C;
            // sp70 = s7;
            
            // Loop through tiles horizontally
            for (s0 = 0; s0 < sp8C; s0++) 
            {
                // Set up tile rectangle in sprite space
                spA0.x = (sp78 + s0) * s3;
                spA0.y = (sp74 + sp90) * s6;
                spA0.w = s3;
                spA0.h = s6;
                
                // Check if tile intersects with our read region
                if (func_ovl8_8037A67C(&spA0, &spA8, &sp98) != 0) 
                {
                    // Calculate offset within tile
                    sp98.x %= s3;
                    sp98.y %= s6;
                    
                    // Read from this tile's bitmap
                    bitmap = sprite->bitmap + ((sp74 + sp90) * sp84) + (s0) + (sp78);
                    
                    func_ovl8_80379D74(
                        s1, 
                        dst_rect->w, 
                        bitmap, 
                        sprite->bmsiz, 
                        &sp98
                    );

                    sp70 = sp98.h;
                }
                
                s1 += sp98.w * sp6C;
            }
            
            sp5C += sp70 * dst_rect->w * sp6C;
        }
    }
}

// 0x8037A5B8
void func_ovl8_8037A5B8(Sprite* arg0, DBMenuPosition* arg1, s32 arg2)
{
	DBMenuPosition sp28;
	u32 var_a1;
	u32 sp20;

	sp28.x = arg1->x;
	sp28.y = arg1->y;
	sp28.w = sp28.h = 1;
	func_ovl8_8037A0FC(arg0, &sp28, &sp20);

	var_a1 = sp20;
	if (arg0->bmsiz == 2)
	{
		var_a1 = sp20 >> 0x10;
	}
	func_ovl8_80376C40(arg0->bmsiz, var_a1, arg2);
}

// 0x8037A62C
void func_ovl8_8037A62C(db4Shorts* arg0, s32 arg1)
{
	DBMenuPosition sp20;
	db2Shorts sp1C;

	sp20.x = 0;
	sp20.y = arg1;
	sp20.w = arg0->arr[2];
	sp20.h = arg0->arr[3] - arg1;
	sp1C.arr[0] = sp1C.arr[1] = 0;
	func_ovl8_803787C0(arg0, arg0, &sp20, &sp1C);
}

// 0x8037A67C
s32 func_ovl8_8037A67C(s16* arg0, s16* arg1, s16* arg2)
{
	s32 sp30[2];
	s32 sp28[2];
	s32 sp20[2];
	s32 sp1C;

	func_ovl8_8037A9F4(arg0, &sp30);
	func_ovl8_8037A9F4(arg1, &sp28);
	sp1C = func_ovl8_8037A6D4(&sp30, &sp28, &sp20);
	func_ovl8_8037AA28(&sp20, arg2);
	return sp1C;
}

// 0x8037A6D4
s32 func_ovl8_8037A6D4(db4Shorts *rect1, db4Shorts *rect2, db4Shorts *intersection) {
    s16 rect1_bottom;
    s16 rect1_right;
    s16 rect2_right;
    s16 rect2_left;
    s16 result_value;
    s32 overlapX;
    s32 overlapY;
    
    // Check X-axis overlap
    overlapX = 0;
    rect2_left = rect2->arr[0], rect1_right = rect1->arr[2];
    
    // Case 1: rect2 left edge inside rect1, rect2 extends past rect1 right
    if ((rect2_left < rect1_right) && (rect2->arr[2] >= rect1_right)) {
        overlapX = 1;
    }
    // Case 2: rect1 left edge inside rect2
    else if ((rect1->arr[0] >= rect2_left) && (rect1->arr[0] < rect2->arr[2])) {
        overlapX = 1;
    }
    // Case 3: rect1 completely contains rect2 horizontally
    else if ((rect1->arr[0] < rect2_left) && (rect1_right >= rect2->arr[2])) {
        overlapX = 1;
    }
    
    if (overlapX != 0) {
        // Compute intersection left edge: max(rect1_left, rect2_left)
        result_value = rect2_left < rect1->arr[0] ? rect1->arr[0] : rect2_left;

        intersection->arr[0] = result_value;
        
        // Compute intersection right edge: min(rect1_right, rect2_right)
        rect2_right = rect2->arr[2];
        rect1_right = rect1->arr[2];
        result_value = rect1_right < rect2_right ? rect1_right : rect2_right;
        intersection->arr[2] = result_value;
        
        // Check Y-axis overlap
        rect1_bottom = rect1->arr[3];
        rect2_left = rect2->arr[1];  // reused as rect2_top
        overlapY = 0;
        rect1_right = rect1->arr[1]; // reused as rect1_top
        
        // Case 1: rect2 top edge inside rect1, rect2 extends past rect1 bottom
        if ((rect2_left < rect1_bottom) && (rect2->arr[3] >= rect1_bottom)) {
            overlapY = 1;
        }
        // Case 2: rect1 top edge inside rect2
        else if ((rect1->arr[1] >= rect2_left) && (rect1->arr[1] < rect2->arr[3])) {
            overlapY = 1;
        }
        // Case 3: rect1 completely contains rect2 vertically
        else if ((rect1->arr[1] < rect2_left) && (rect1_bottom >= rect2->arr[3])) {
            overlapY = 1;
        }
        
        if (overlapY != 0) {
            // Compute intersection top edge: max(rect1_top, rect2_top)
            rect1_right = rect1->arr[1]; // reused as rect1_top
            result_value = rect2_left < rect1_right ?  rect1_right : rect2_left;    // rect2_top
            intersection->arr[1] = result_value;
            
            // Compute intersection bottom edge: min(rect1_bottom, rect2_bottom)
            rect2_right = rect2->arr[3]; // reused as rect2_bottom
            rect1_bottom = rect1->arr[3];
            result_value = rect1_bottom < rect2_right ? rect1_bottom : rect2_right;
            intersection->arr[3] = result_value;
            return 1; // Rectangles intersect
        }
    }
    intersection->arr[2] = 0;
    intersection->arr[3] = 0;
    intersection->arr[1] = 0;
    intersection->arr[0] = 0;
    return 0;
}

// 0x8037A8BC
void func_ovl8_8037A8BC(void* arg0, void* arg1)
{
	s32 sp20[2];
	s32 sp18[2];

	func_ovl8_8037A9F4(arg0, &sp20);
	func_ovl8_8037A9F4(arg1, &sp18);
	func_ovl8_8037A904(&sp20, &sp18);
	func_ovl8_8037AA28(&sp18, arg1);
}

// 0x8037A904
void func_ovl8_8037A904(db4Shorts* arg0, db4Shorts* arg1)
{
	s32 temp_a3;

	temp_a3 = arg0->arr[0] - arg1->arr[0];
	if (temp_a3 > 0)
	{
		func_ovl8_8037A9C0(arg1, temp_a3, 0);
	}

	temp_a3 = arg0->arr[2] - arg1->arr[2];
	if (temp_a3 < 0)
	{
		func_ovl8_8037A9C0(arg1, temp_a3, 0);
	}

	temp_a3 = arg0->arr[1] - arg1->arr[1];
	if (temp_a3 > 0)
	{
		func_ovl8_8037A9C0(arg1, 0, temp_a3);
	}

	temp_a3 = arg0->arr[3] - arg1->arr[3];
	if (temp_a3 < 0)
	{
		func_ovl8_8037A9C0(arg1, 0, temp_a3);
	}
}

// 0x8037A9C0
void func_ovl8_8037A9C0(db4Shorts* arg0, s32 arg1, s32 arg2)
{
	arg0->arr[0] = arg0->arr[0] + arg1;\
	arg0->arr[2] = arg0->arr[2] + arg1;
	arg0->arr[1] = arg0->arr[1] + arg2;\
	arg0->arr[3] = arg0->arr[3] + arg2;
}

// 0x8037A9F4
void func_ovl8_8037A9F4(DBMenuPosition* arg0, DBMenuPosition* arg1)
{
	arg1->x = arg0->x;
	arg1->y = arg0->y;
	arg1->w = arg0->x + arg0->w;
	arg1->h = arg0->y + arg0->h;
}

// 0x8037AA28
void func_ovl8_8037AA28(db4Shorts* arg0, db4Shorts* arg1)
{
	arg1->arr[0] = arg0->arr[0];
	arg1->arr[1] = arg0->arr[1];
	arg1->arr[2] = arg0->arr[2] - arg0->arr[0];
	arg1->arr[3] = arg0->arr[3] - arg0->arr[1];
}

// 0x8037AA5C
s32 func_ovl8_8037AA5C(DBMenuPosition* arg0)
{
	if (arg0->w <= 0 || arg0->h <= 0)
	{
		return 1;
	}
	return 0;
}

// 0x8037AA88
void func_ovl8_8037AA88(s32 arg0, s32 arg1, s32 arg2, s32 arg3, dbUnknown7* arg4) {
    s32 var_v0;
    
    arg4->unk0 = arg2 - arg0;
    arg4->unk4 = arg3 - arg1;
    arg4->unk20 = arg0;
    arg4->unk24 = arg1;
    var_v0 = arg4->unk0 == 0 ? 0 : (arg4->unk0 > 0 ? 1 : -1);
    
    arg4->unk10 = var_v0;
    var_v0 = arg4->unk4 == 0 ? 0 : (arg4->unk4 > 0 ? 1 : -1);
    
    arg4->unk14 = var_v0;
    var_v0 = arg4->unk0 < 0 ? -arg4->unk0 : arg4->unk0;

    arg4->unk0 = var_v0;
    var_v0 = arg4->unk4 < 0 ? -arg4->unk4 : arg4->unk4;
    arg4->unk4 = var_v0;
    
    if (var_v0 < arg4->unk0) {
        arg4->unk30 = arg4->unk0;
        arg4->unk28 = ((u8*)arg4 + 0x20);
        arg4->unk2C = ((u8*)arg4 + 0x24);
        arg4->unk8 =  (s32*) arg4;
        arg4->unkC = (s32*) ((u8*)arg4 + 4);
        arg4->unk18 = ((u8*)arg4 + 0x10);
        arg4->unk1C = (void* ) ((u8*)arg4 + 0x14);
    } else {
        arg4->unk30 = arg4->unk4;
        arg4->unk28 = ((u8*)arg4 + 0x24);
        arg4->unk2C = ((u8*)arg4 + 0x20);
        arg4->unk8 = (s32*) ((u8*)arg4 + 4);
        arg4->unkC = (s32) arg4;
        arg4->unk18 = ((u8*)arg4 + 0x14);
        arg4->unk1C = (void* ) ((u8*)arg4 + 0x10);
    }
    arg4->unk34 = (*arg4->unkC / 2);
    arg4->unk38 = arg4->unk30;
}


// 0x8037ABDC
s32 func_ovl8_8037ABDC(Vec2h* arg0, UiLineStepper* arg1) 
{
    if (arg1->error == arg1->stepsRemaining) 
    {
        arg0->x = arg1->currentX;
        arg0->y = arg1->currentY;
    } 
    else 
    {
        if (arg1->totalSteps < 0) 
        {
            *arg1->posPrimary += *arg1->stepPrimary;
            arg1->totalSteps += *arg1->errorAdd;
        } 
        else 
        {
            *arg1->posPrimary += *arg1->stepPrimary;
            
            if (arg1->currentX && arg1->currentX);
            
            *arg1->posSecondary += *arg1->stepSecondary;
            
            if (arg1->currentX && arg1->currentX);
            
            arg1->totalSteps += *arg1->errorAdd - *arg1->errorSub;
        }
        
        arg0->x = arg1->currentX;
        arg0->y = arg1->currentY;
    }
    
    return arg1->stepsRemaining--;
}

// 0x8037ACAC
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_8037ACAC.s")

// 0x8037B3E4
void func_ovl8_8037B3E4(dbUnknown3* arg0)
{
	dbUnknown3 *temp_a0;

	func_ovl8_80371764(arg0->unk_dbunk3_0x44);
	func_ovl8_80371764(arg0->unk_dbunk3_0x34);

	temp_a0 = arg0->unk_dbunk3_0x20;
	if (temp_a0 != NULL)
	{
		func_ovl8_80371764(temp_a0);
	}

	func_ovl8_80371764(arg0);
}

// 0x8037B434
void func_ovl8_8037B434(Sprite* arg0, s32 arg1, s32 arg2, SYColorRGBA* arg3)
{
	dbUnknown8_SC* var_a2;

	if (arg2 != 0)
	{
		var_a2 = &D_ovl8_8038A068;
	}
	else
	{
		var_a2 = &D_ovl8_80389FB4;
	}

	func_ovl8_8037B46C(arg0, arg1, var_a2, arg3);
}

// 0x8037B46C
void func_ovl8_8037B46C(Sprite* arg0, DBMenuPosition* arg1, dbUnknown8_SC* arg2, s32 arg3) 
{
    s32 var_v0;
    DBMenuPosition abs;
	s32 unused;
	db4Bytes sp44;
    s16 temp1, temp2;
    dbUnknown8_SC* var_s1;

    for (var_s1 = arg2; var_s1->x0 != (0x7FFF, 0x7FFF); var_s1++) 
    {
        var_v0 = (var_s1->x0 & 0x8000) ? (arg1->w - (var_s1->x0 & 0x7FFF)) - 1 : var_s1->x0;
        abs.x = arg1->x + var_v0;
        
        var_v0 = (var_s1->y0 & 0x8000) ? (arg1->h - (var_s1->y0 & 0x7FFF)) - 1 : var_s1->y0;
        abs.y = arg1->y + var_v0;
        
        var_v0 = (var_s1->x1 & 0x8000) ? (arg1->w - (var_s1->x1 & 0x7FFF)) - 1 : var_s1->x1;
        temp1 = arg1->x + var_v0;
        
        var_v0 = (var_s1->y1 & 0x8000) ? (arg1->h - (var_s1->y1 & 0x7FFF)) - 1 : var_s1->y1;
        temp2 = arg1->y + var_v0;

        abs.w = (temp1 - abs.x) + 1;
        abs.h = (temp2 - abs.y) + 1;

        func_ovl8_80376EE0(arg3, &sp44, var_s1->dbUnknown8_SC_0x8);
        func_ovl8_80377AEC(arg0, &abs, &sp44, 4);
    }
}

// 0x8037B5F8
s32 stringToNumberSigned(char* str)
{
	s32 num = 0;
	s32 sign = (str[0] == '-') ? (str++, -1) : 1;

	while (str[0] != '\0')
	{
		num *= 10;
		num += str++[0] - '0';
	}

	return sign * num;
}

// 0x8037B654
s32 func_ovl8_8037B654(s32 val, char* str) 
{
    s32 digit;
    s32 found_first_digit;
    s32 index;
    s32 place;
    s32 remaining;
    s32 abs = (val < 0) ? -val : val;
    s32 ret;
    
    remaining = abs;
    index = 0;
    found_first_digit = FALSE;
    
    if (val < 0)
    {
        abs = index++,
        str[abs] = '-';  // '-' character
    }

    place = 1000000000;
    found_first_digit = FALSE;
    
    while (place != 0) 
    {
        digit = remaining / place;
        
        if ((digit != 0) || (found_first_digit != FALSE)) 
        {   
            abs = index--;
            str[abs] = digit + '0';  // Convert digit to ASCII
            index += 2;
        }
        if (digit != 0)
        {
            found_first_digit = TRUE;
            remaining -= place * digit;
        }
        place /= 10;
    }
    if (index != 0) 
    {
        str[index] = '\0';  // Null terminator
    }
    else
    {
        str[0] = '0';  // '0' character
        str[1] = '\0';     // Null terminator
    }
}

// 0x8037B760
#pragma GLOBAL_ASM("asm/nonmatchings/ovl8/ovl8_8/func_ovl8_8037B760.s")

// 0x8037B7F0
s32 func_ovl8_8037B7F0(u8 *str, s32 index)
{
	s32 pos = 0;
	u8 c = str[pos];

	while (str[pos] != 0)
	{
		if (index < pos)
			return 0;

		if (c & 0x80) // high-bit set => multi-byte char?
		{
			if (index == pos)
				return 1;
			pos++;
			if (index == pos)
				return 1;
		}
		pos++;
		c = str[pos];
	}
	return 0;
}

// 0x8037B85C
void func_ovl8_8037B85C(s32 arg0, s32* arg1, s32* arg2) 
{
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 temp_f6;

    var_a2 = (arg0 == 1) ? 0x800 : (arg0 == 2) ? 0x800 : 0x400;
    
    temp_f6 = sqrtf(var_a2);
    
    temp_v0 = *arg1;
    temp_v1 = *arg2;
    
    if (var_a2 < (temp_v0 * temp_v1)) 
    {
        if (temp_v0 >= temp_v1) 
        {
            var_a0 = temp_v1;
            var_a1 = var_a2 / temp_v1;
        } 
        else 
        {
            var_a0 = var_a2 / temp_v0;
            var_a1 = temp_v0;
        }
        
        if (temp_f6 < var_a1) 
        {
            var_a1 = temp_f6;
        }
        
        if (temp_f6 < var_a0) 
        {
            var_a0 = temp_f6;
        }

        var_a1 &= ~3;
        var_a0 &= ~3;
        
        *arg1 = var_a1;
        *arg2 = var_a0;
    }
}

// 0x8037B98C
void func_ovl8_8037B98C(dbMenuSprite *arg0)
{
    dbUnkStruct *temp_v0;
    dbUnknownLinkStruct *temp_v0_2;
    DBMenu sp28;
    dbMenuSprite *var_v1;

    sp28 = D_ovl8_8038A11C;
    
    D_8038EFC0_1AB810 = arg0->unk_dbmenusprite_0x0;
    
    func_ovl8_80371638(D_8038EFC0_1AB810, arg0->unk_dbmenusprite_0x4);
    func_ovl8_8037D2F0();
    
    temp_v0 = func_ovl8_803717A0(0x10);
    
    if (temp_v0 != NULL) 
    {
        func_ovl8_8037C3C0(temp_v0);
        var_v1 = temp_v0;
    } 
    else var_v1 = NULL;
    
    D_8038EFC4_1AB814 = var_v1;
    
    func_ovl8_8037C498(D_8038EFC4_1AB814, D_8038EFC4_1AB814);
    gcAddGObjProcess(func_ovl8_8037C510(D_8038EFC4_1AB814), func_ovl8_8037BD94, 0, 1);
    
    temp_v0_2 = func_ovl8_803717A0(0x68);
    
    if (temp_v0_2 != NULL) 
    {
        func_ovl8_8037CE90(temp_v0_2, &sp28);
        var_v1 = temp_v0_2;
    } 
    else var_v1 = NULL;
    
    D_8038EFC8_1AB818 = var_v1;
    
    func_ovl8_8037CFD8(D_8038EFC8_1AB818, D_8038EFC8_1AB818);
    var_v1 = D_8038EFC8_1AB818;
    gcAddGObjProcess(var_v1->unk_dbmenusprite_0x48, func_ovl8_8037BDF4, 0, 1);
    func_ovl8_8037D63C();
    
    D_8038EFCC_1AB81C = NULL;
    
    dbUiNodeTypeRegisterHandlers();
}

// 0x8037BAD8
void func_ovl8_8037BAD8()
{
	dbFunction* temp_v0;

	if (D_8038EFC4_1AB814 != NULL)
	{
		if (D_8038EFC4_1AB814 != NULL)
		{
			temp_v0 = D_8038EFC4_1AB814->db_func;
			temp_v0[1].unk_dbfunc_0x4(temp_v0[1].unk_dbfunc_0x0 + (uintptr_t) D_8038EFC4_1AB814, 3);
		}

		D_8038EFC4_1AB814 = NULL;
	}
}

// 0x8037BB28
void func_ovl8_8037BB28()
{
	dbFunction* temp_v0;

	if (D_8038EFC8_1AB818 != NULL)
	{
		if (D_8038EFC8_1AB818 != NULL)
		{
			temp_v0 = D_8038EFC8_1AB818->db_func;
			temp_v0[1].unk_dbfunc_0x4(temp_v0[1].unk_dbfunc_0x0 + (uintptr_t) D_8038EFC8_1AB818, 3);
		}

		D_8038EFC8_1AB818 = NULL;
	}
}

// 0x8037BB78
void func_ovl8_8037BB78()
{
	s32 temp_v0;
	s32 temp_v0_2;

	while(TRUE)
	{
		temp_v0 = func_ovl8_8037499C(1);

		if (temp_v0 != 0)
		{
			func_ovl8_8037488C(temp_v0);
			continue;
		}

		break;
	}

	while (TRUE)
	{
		temp_v0_2 = func_ovl8_8037499C(0);

		if (temp_v0_2 != 0)
		{
			func_ovl8_8037488C(temp_v0_2);
			continue;
		}

		break;
	}

	func_ovl8_8037BD44(temp_v0_2);
	func_ovl8_8037BAD8();
	func_ovl8_8037BB28();
	func_ovl8_8037D34C();
}

// 0x8037BBF0
s32 func_ovl8_8037BBF0(dbUnknown8_S28* arg0, Sprite* sprite, u16 arg2, u16 arg3)
{
	dbTestMenu *temp_v0;
	s32 sp20;
	dbTestMenu *var_v1;

	temp_v0 = func_ovl8_803717A0(0x7C);

	if (temp_v0 != NULL)
	{
		func_ovl8_803759F0(temp_v0, 0, arg0, sprite);
		var_v1 = temp_v0;
	}
	else
		var_v1 = NULL;

	D_8038EFCC_1AB81C = var_v1;
	sp20 = func_ovl8_803723AC(D_8038EFCC_1AB81C, D_8038EFCC_1AB81C, sprite, func_ovl8_8037BEC8, func_ovl8_8037BF34, FALSE);
	func_ovl8_80375B8C(D_8038EFCC_1AB81C, arg2, arg3);

	if (sp20 != 0)
	{
		return sp20;
	}

	func_ovl8_80375C54(D_8038EFCC_1AB81C);

	return 0;
}

// 0x8037BCB8
s32 func_ovl8_8037BCB8()
{
	dbUnknown8_S28 sp18;

	if (D_8038EFCC_1AB81C != NULL)
		return -1;

	sp18 = D_ovl8_8038A144;
	sp18.str = 'Arow'; // 0x41726F77

	return func_ovl8_8037BBF0(&sp18, &D_ovl8_80387EB8, 4, 1);
}

// 0x8037BD44
void func_ovl8_8037BD44()
{
	dbFunction* temp_v0;

	if (D_8038EFCC_1AB81C != NULL)
	{
		if (D_8038EFCC_1AB81C != NULL)
		{
			temp_v0 = D_8038EFCC_1AB81C->db_func;
			temp_v0[1].unk_dbfunc_0x4(temp_v0[1].unk_dbfunc_0x0 + (uintptr_t) D_8038EFCC_1AB81C, 3);
		}

		D_8038EFCC_1AB81C = NULL;
	}
}

// 0x8037BD94
void* func_ovl8_8037BD94(GObj* arg0)
{
	void *temp_s0 = arg0->user_data.p;

	while (TRUE)
	{
		func_ovl8_8037C4A4(temp_s0, arg0);
		gcSleepCurrentGObjThread(1);
	}

	return temp_s0;
}

// 0x8037BDF4
void* func_ovl8_8037BDF4(GObj* arg0)
{
	void *temp_s0 = arg0->user_data.p;

	while (TRUE)
	{
		func_ovl8_8037CFAC(temp_s0);
		gcSleepCurrentGObjThread(1);
	}

	return temp_s0;
}

// 0x8037BE34
void func_ovl8_8037BE34(GObj* arg0)
{
	dbUnknown5 *temp_s0;
	dbFunction *temp_v0;

	temp_s0 = arg0->user_data.p;

	while (TRUE)
	{
		temp_v0 = temp_s0->db_func;
		temp_v0[8].unk_dbfunc_0x4(temp_v0[8].unk_dbfunc_0x0 + (uintptr_t)temp_s0, arg0);
		gcSleepCurrentGObjThread(1);
	}
}

// 0x8037BE94
void func_ovl8_8037BE94(GObj* arg0)
{
	dbUnknownS38* temp_a1;
	dbFunction* temp_v0;

	temp_a1 = arg0->user_data.p;
	temp_v0 = ((dbUnknownLinkStruct*) temp_a1->unk_dbunks38_0x20)->db_func;
	temp_v0[11].unk_dbfunc_0x4(temp_v0[11].unk_dbfunc_0x0 + (uintptr_t)temp_a1->unk_dbunks38_0x20, temp_a1);
}

// 0x8037BEC8
void func_ovl8_8037BEC8(GObj* arg0)
{
	dbTestMenu* temp_s0;
	dbFunction* temp_v0;

	temp_s0 = arg0->user_data.p;

	while (TRUE)
	{
		temp_v0 = ((dbUnknownLinkStruct*)temp_s0->unk_dbtestmenu_0x10)->db_func;
		temp_v0[14].unk_dbfunc_0x4(temp_v0[14].unk_dbfunc_0x0 + (uintptr_t)temp_s0->unk_dbtestmenu_0x10, arg0);
		gcSleepCurrentGObjThread(1);
	}
}

// 0x8037BF34
void func_ovl8_8037BF34(GObj* arg0)
{
	dbTestMenu* temp_a1;
	dbFunction* temp_v0;

	temp_a1 = arg0->user_data.p;
	temp_v0 = ((dbUnknownLinkStruct*) temp_a1->unk_dbtestmenu_0x10)->db_func;
	temp_v0[11].unk_dbfunc_0x4(temp_v0[11].unk_dbfunc_0x0 + (uintptr_t)temp_a1->unk_dbtestmenu_0x10, temp_a1);
}

// 0x8037BF68
s32 func_ovl8_8037BF68(s32 user_data, Sprite* sprite, void (*proc)(GObj*), void (*proc_display)(GObj*), GObj **camera_out, GObj **sprite_out, sb32 low_priority)
{
	GObj *camera_gobj;
	GObj *sprite_gobj;

	camera_gobj = gcMakeCameraGObj(-0x1FF, gcDefaultFuncRun, 0, 0x80000000, func_80018300, !low_priority ? 2 : 1, 0x8000000000000000ULL, 0, 1, 0, NULL, 1, 0);
	if (camera_gobj == NULL)
		return -1;

	((CObj*)camera_gobj->obj)->flags = 8;
	camera_gobj->camera_tag = camera_gobj;
	if (!low_priority)
	{
		sprite_gobj = gcMakeGObjSPBefore(-0x1FD, NULL, 0x1F, 1);
		if (sprite_gobj == NULL)
			return -1;
	}
	else
	{
		sprite_gobj = gcMakeGObjSPBefore(-0x1FC, NULL, 0x1F, 2);
		if (sprite_gobj == NULL)
			return -1;
	}
	gcAddGObjDisplay(sprite_gobj, proc_display, 0x3F, 1, camera_gobj);
	gcAddSObjForGObj(sprite_gobj, sprite);
	gcAddGObjProcess(sprite_gobj, proc, 0, 1);
	sprite_gobj->user_data.p = user_data;
	*camera_out = camera_gobj;
	*sprite_out = sprite_gobj;
	return 0;
}

// 0x8037C0CC
dbUnknownLinkStruct* func_ovl8_8037C0CC()
{
	return D_8038EFC8_1AB818;
}

// 0x8037C0D8
void func_ovl8_8037C0D8(u16 arg0, u16 arg1, u16 arg2, u16 arg3, s16 arg4, s16 arg5)
{
	if (D_8038EFCC_1AB81C != NULL)
	{
		D_ovl8_80389F30 = arg0;
		D_ovl8_80389F34 = arg1;
		D_ovl8_80389F3C = arg2;
		D_ovl8_80389F38 = arg3;
		D_ovl8_80389F40 = arg4;
		D_ovl8_80389F44 = arg5;
	}
}

// 0x8037C144
void func_ovl8_8037C144(s32 arg0)
{
	if (D_8038EFCC_1AB81C != NULL)
	{
		func_ovl8_80375D44(D_8038EFCC_1AB81C, arg0);
	}
}

// 0x8037C174
s32 func_ovl8_8037C174()
{
	if (D_8038EFCC_1AB81C == NULL)
	{
		return 0;
	}
	return func_ovl8_80375D4C(D_8038EFCC_1AB81C);
}

// 0x8037C1AC
void func_ovl8_8037C1AC(db4Shorts *arg0)
{
	func_ovl8_80375D54(D_8038EFCC_1AB81C, arg0);
}

// 0x8037C1D4
void func_ovl8_8037C1D4(u8** src, u8** dest, s32 count) 
{
    s32 var_a3;
    u8* var_v0;
    u8* var_v1;

    var_v0 = *src;
    var_v1 = *dest;
    var_a3 = count;
    
    while (var_a3 > 0) 
    {
        u8 curr = *var_v0;
        u32 next = *var_v0 + 1;
        
        if (curr & 0x80) 
        {
            s16 run_length = (u8)(-curr++) + 1;
            s16 temp_t1;
            var_v0++;

            temp_t1 = run_length--;
            
            while (temp_t1) 
            {
                u8 *a2 = var_v1++;\
                temp_t1 = run_length--;
                *a2 = *var_v0;
                var_a3--;
            }
            
            var_v0++;
        } 
        else 
        {
            s16 run_length = curr++ + 1;
            s16 temp_t1;
            var_v0++;

            temp_t1 = run_length--;
            
            while (temp_t1) 
            {
                u8 *a2 = var_v1++, *t2 = var_v0++;\
                temp_t1 = run_length--;
                *a2 = *t2;
                var_a3--;
            }
        }
    }
    
    *src = var_v0;
    *dest = var_v1;
}
