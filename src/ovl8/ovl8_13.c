#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>


typedef struct menuCursorStruct {
    u32 button_tap;
    u32 button_update;
    Vec2h cursor_pos;
} menuCursorStruct;

typedef struct dbUnknown8_13
{
	u16 unk_dbunknown8_13_0x0;
	u16 width;
	u16 height;
	DBFontPadding padding;
	u8 bits_per_pixel;
	u8 *glyphs;
} dbUnknown8_13;

// direct references to gSYControllerMain?
extern u16 gSYControllerMainButtonTap;
extern u16 gSYControllerMainButtonUpdate;

extern db4Bytes D_ovl8_80389F4C;
extern db4Bytes D_ovl8_80389F50;

// Forward declarations for all functions referenced in dispatch tables
extern sb32 func_ovl8_8037EB00();
extern sb32 func_ovl8_8037EBC8();
extern sb32 func_ovl8_8037C9E4();
extern sb32 func_ovl8_80373650();
extern sb32 func_ovl8_8037367C();
extern sb32 func_ovl8_80373684();
extern sb32 func_ovl8_8037368C();
extern sb32 func_ovl8_80373694();
extern sb32 func_ovl8_80373750();
extern sb32 func_ovl8_80373790();
extern sb32 func_ovl8_803737DC();
extern sb32 func_ovl8_80373E24();
extern sb32 func_ovl8_80373EA8();
extern sb32 func_ovl8_80373930();
extern sb32 func_ovl8_80373980();
extern sb32 func_ovl8_80373988();
extern sb32 func_ovl8_80373990();
extern sb32 func_ovl8_80373A28();
extern sb32 func_ovl8_80373ABC();
extern sb32 func_ovl8_80373AC4();
extern sb32 func_ovl8_80373B28();
extern sb32 func_ovl8_80373B84();
extern sb32 func_ovl8_803743D8();
extern sb32 func_ovl8_80373BD4();
extern sb32 func_ovl8_803743C0();
extern sb32 func_ovl8_8037CA48();
extern sb32 func_ovl8_803718FC();
extern sb32 func_ovl8_80371930();
extern sb32 func_ovl8_80371960();
extern sb32 func_ovl8_80371968();
extern sb32 func_ovl8_80371970();
extern sb32 func_ovl8_80371978();
extern sb32 func_ovl8_80371980();
extern sb32 func_ovl8_803719A4();
extern sb32 func_ovl8_803719C8();
extern sb32 func_ovl8_80371A2C();
extern sb32 func_ovl8_80371AC4();
extern sb32 func_ovl8_80371ACC();
extern sb32 func_ovl8_80371AD8();
extern sb32 func_ovl8_80371B34();
extern sb32 func_ovl8_80371B98();
extern sb32 func_ovl8_80371BCC();
extern sb32 func_ovl8_80371C2C();
extern sb32 func_ovl8_80372224();
extern sb32 func_ovl8_80371C8C();
extern sb32 func_ovl8_80372358();
extern sb32 func_ovl8_80371D08();
extern sb32 func_ovl8_80371D64();
extern sb32 func_ovl8_80371D74();
extern sb32 func_ovl8_80371DC0();
extern sb32 func_ovl8_80371DD0();
extern sb32 func_ovl8_80371DDC();
extern sb32 func_ovl8_80371DE4();
extern sb32 func_ovl8_803726CC();
extern sb32 func_ovl8_80371DF4();
extern sb32 func_ovl8_80371DFC();
extern sb32 func_ovl8_80371E04();
extern sb32 func_ovl8_80371E0C();
extern sb32 func_ovl8_80371E14();
extern sb32 func_ovl8_80371E1C();
extern sb32 func_ovl8_80371E24();
extern sb32 func_ovl8_80371E2C();
extern sb32 func_ovl8_80371E58();
extern sb32 func_ovl8_8037C344();
extern sb32 func_ovl8_8037C358();

GObj* D_ovl8_8038A860 = NULL;
s16 D_ovl8_8038A864 = 0x0010;

// Auto-repeat timing variables (64-bit timestamps in milliseconds)
u64 D_ovl8_8038A868 = 0;  // Last button press time
u64 D_ovl8_8038A870 = 0;  // Repeat start time
u64 D_ovl8_8038A878 = 250000;  // Initial repeat delay (250ms)
u64 D_ovl8_8038A880 = 500000;  // Repeat interval (500ms)

s32 D_ovl8_8038A888 = 0;
s32 D_ovl8_8038A88C = 0;
db4Bytes D_ovl8_8038A890[] = {
    {-1, -1, -1, -1},  // 0xFFFFFFFF
    { 0,  0,  0, -1}   // 0x000000FF
};
s32 D_ovl8_8038A898 = 0;
s32 D_ovl8_8038A89C = 0;

// Function dispatch table - array of dbFunction structs
// offset (s16) + function pointer, organized as vtable
dbFunction D_ovl8_8038A8A0[] = {
    {0, NULL}, 
    {0, func_ovl8_8037EB00},
    {0, func_ovl8_8037EBC8},
    {0, func_ovl8_8037C9E4},
    {0, func_ovl8_80373650},
    {0, func_ovl8_8037367C},
    {0, func_ovl8_80373684},
    {0, func_ovl8_8037368C},
    {0, func_ovl8_80373694},
    {0, func_ovl8_80373750},
    {0, func_ovl8_80373790},
    {0, func_ovl8_803737DC},
    {0, func_ovl8_80373E24},
    {0, func_ovl8_80373EA8},
    {0, func_ovl8_80373930},
    {0, func_ovl8_80373980},
    {0, func_ovl8_80373988},
    {0, func_ovl8_80373990},
    {0, func_ovl8_80373A28},
    {0, func_ovl8_80373ABC},
    {0, func_ovl8_80373AC4},
    {0, func_ovl8_80373B28},
    {0, func_ovl8_80373B84},
    {0, func_ovl8_803743D8},
    {0, func_ovl8_80373BD4},
    {0, func_ovl8_803743C0},
    {0, func_ovl8_8037CA48},
    {0, NULL}
};

// Another function dispatch table with sentinel markers
dbFunction D_ovl8_8038A980[] = {
    {0, NULL},
    {(s16)0xFF48, func_ovl8_8037EB00},
    {0, func_ovl8_803718FC},
    {0, func_ovl8_80371930},
    {0, func_ovl8_80371960},
    {0, func_ovl8_80371968},
    {0, func_ovl8_80371970},
    {0, func_ovl8_80371978},
    {0, func_ovl8_80371980},
    {0, func_ovl8_803719A4},
    {0, func_ovl8_803719C8},
    {0, func_ovl8_80371A2C},
    {(s16)0xFF48, func_ovl8_8037C9E4},
    {0, func_ovl8_80371AC4},
    {0, func_ovl8_80371ACC},
    {0, func_ovl8_80371AD8},
    {0, func_ovl8_80371B34},
    {0, func_ovl8_80371B98},
    {0, func_ovl8_80371BCC},
    {(s16)0xFF48, func_ovl8_80373650},
    {0, func_ovl8_80371C2C},
    {(s16)0xFF88, func_ovl8_80372224},
    {0, func_ovl8_80371C8C},
    {(s16)0xFF88, func_ovl8_80372358},
    {(s16)0xFF48, func_ovl8_8037EBC8},
    {0, func_ovl8_80371D08},
    {0, func_ovl8_80371D64},
    {0, func_ovl8_80371D74},
    {0, func_ovl8_80371DC0},
    {0, func_ovl8_80371DD0},
    {0, func_ovl8_80371DDC},
    {0, func_ovl8_80371DE4},
    {(s16)0xFF88, func_ovl8_803726CC},
    {0, func_ovl8_80371DF4},
    {0, func_ovl8_80371DFC},
    {0, func_ovl8_80371E04},
    {0, func_ovl8_80371E0C},
    {0, func_ovl8_80371E14},
    {0, func_ovl8_80371E1C},
    {0, func_ovl8_80371E24},
    {0, func_ovl8_80371E2C},
    {0, func_ovl8_80371E58},
    {0, NULL}
};

// Third function dispatch table
dbFunction D_ovl8_8038AAD8[] = {
    {0, NULL}, 
    {(s16)0xFEEC, func_ovl8_8037EB00},
    {0, func_ovl8_8037C344},
    {0, func_ovl8_8037C358},
    {0, NULL}
};

// extern dbUnknownLinkStruct D_ovl8_8038AB00;

extern menuCursorStruct D_8038EFE0_1AB830;
extern s32 D_8038EFEC_1AB83C;
extern menuCursorStruct D_8038EFF0_1AB840;
extern s16 D_8038EFFC_1AB84C;
extern db4Shorts D_8038F000_1AB850;
extern DBFont D_8038F008_1AB858;
extern dbUnknown8_13 D_8038F020_1AB870;
extern u16 D_8038F030_1AB880;
extern u16 D_8038F032_1AB882;
extern f32 D_8038F034_1AB884;
extern f32 D_8038F038_1AB888;
extern void (*D_8038F03C_1AB88C)(u16, u16, u8*, u8*);
extern void (*D_8038F040_1AB890)(u16, u16, u8*, u8*);
extern u16 D_8038F044_1AB894;
extern u16 D_8038F046_1AB896;
extern u16 D_8038F048_1AB898;
extern u8* D_8038F050_1AB8A0;
extern s32* D_8038F290_1ABAE0;
extern char D_8038FBA8_1AC3F8[16];

extern dbUnknownS14 D_8038FB90_1AC3E0;
extern db4Bytes D_8038FB98_1AC3E8;
extern void func_ovl8_803798A0(Sprite*, s32*, u32, u32, db4Shorts*, s32);
void func_ovl8_8037D000(s32);
s32 func_ovl8_8037D28C();
menuCursorStruct* func_ovl8_8037D3DC(menuCursorStruct *arg0);
void func_ovl8_8037D45C();
void func_ovl8_8037D470(u16, u16, u8*, u8*);
void func_ovl8_8037D518(u16, u16, u8*, u8*);
void func_ovl8_8037D5AC(u16, u16, u8*, u8*);
void func_ovl8_8037D990(s32);
void func_ovl8_8037D99C(s32);
void func_ovl8_8037D9A8(s32);
void func_ovl8_8037D9B4(db4Bytes*);
void func_ovl8_8037D9D0(db4Bytes*);
void func_ovl8_8037D9EC(u16, u16);
extern void func_ovl8_8037DFF8(Sprite*, u16, u16, u8, s32, s32*, s32*, f32);

// 0x8037D000
void func_ovl8_8037D000(s32 arg0) 
{
    menuCursorStruct cs;
    s32 should_fire_repeat;
    s32 directional_input;
    u64 current_time;
    u64 time_since_press;
    
    // Get controller input
    cs.button_tap = gSYControllerMainButtonTap;
    cs.button_update = gSYControllerMainButtonUpdate;
    
    // Get cursor position
    func_ovl8_8037726C(&cs.cursor_pos);
    
    should_fire_repeat = 0;
    
    // Check if a button is being held down
    if (D_ovl8_8038A868 != 0) 
    {
        // Get current time in milliseconds
        // osGetTime returns 64-bit counter at 62.5MHz
        // Multiply by 0x40 (64) and divide by 0xBB8 (3000) to get milliseconds
        current_time = osGetTime() * 64ULL / 3000ULL;
        
        // Check if we should start auto-repeat
        // Must exceed initial delay
        if ((current_time - D_ovl8_8038A868) > D_ovl8_8038A878) 
        {
            should_fire_repeat = 1;
        }
    }
    
    // Mask to 16-bit button values
    cs.button_tap &= 0xFFFF;
    cs.button_update &= 0xFFFF;
    
    // Get directional input from D-pad/stick
    directional_input = func_ovl8_8037D28C();
    
    // Handle auto-repeat logic
    if ((should_fire_repeat != 0) != 0) 
    {
        current_time = osGetTime() * 64ULL / 3000ULL;
        
        // Check if this is the first repeat or if repeat interval has elapsed
        if ((D_ovl8_8038A868 == D_ovl8_8038A870) || ((current_time - D_ovl8_8038A870) > D_ovl8_8038A880)) 
        {
            
            // Fire repeat event
            cs.button_update |= directional_input;
            
            // Update repeat start time
            D_ovl8_8038A870 = current_time;
        }
    } 
    // No button held, so treat any directional input as new press
    else if (D_ovl8_8038A868 == 0) 
    {
        cs.button_tap |= directional_input;
        cs.button_update |= directional_input;
    }
    
    // Handle button press timing
    if ((should_fire_repeat != 0) == 0 && ((cs.button_tap >> 0x10) != 0)) 
    {
        // New button press - record timestamp
        current_time = osGetTime() * 64ULL / 3000ULL;

        D_ovl8_8038A870 = current_time;
        D_ovl8_8038A868 = D_ovl8_8038A870;
    } 
    // No directional input - clear timestamps
    else if (directional_input == 0) 
    {
        D_ovl8_8038A868 = 0;
    }
    
    // Send processed input to handler
    func_ovl8_8037D3DC(&cs);
}

// 0x8037D28C
s32 func_ovl8_8037D28C() 
{
	SYController *controller = &gSYControllerMain;
    s32 mask = 0;
    s16 threshold = D_ovl8_8038A864;\
    s8 range = controller->stick_range.x;
    
    if (range > threshold) 
    {
        mask = 0x40000;
    }
    if (range < -threshold) 
    {
        mask |= 0x80000;
    }

    range = controller->stick_range.y;
    
    if (range > threshold) 
    {
        mask |= 0x10000;
    }
    if (range < -threshold) 
    {
        mask |= 0x20000;
    }
    
    return mask;
}

// 0x8037D2F0
s32 func_ovl8_8037D2F0()
{
    D_ovl8_8038A860 = gcMakeGObjSPAfter(-0x1FA, func_ovl8_8037D000, 0, -1);

    if (D_ovl8_8038A860 == 0)
    {
        return -1;
    }

    func_ovl8_8037D45C();

    D_8038EFFC_1AB84C = 0;

    return 0;
}

// 0x8037D34C
void func_ovl8_8037D34C()
{
	if (D_ovl8_8038A860 != NULL)
	{
		gcEjectGObj(D_ovl8_8038A860);
		D_ovl8_8038A860 = NULL;
	}
}

// 0x8037D384
sb32 func_ovl8_8037D384()
{
    sb32 var_v1 = D_8038EFE0_1AB830.button_tap != 0;

    if (var_v1 == FALSE)
    {
        var_v1 = D_8038EFE0_1AB830.button_update != 0;
    }

    return var_v1;
}

// 0x8037D3AC
menuCursorStruct* func_ovl8_8037D3AC(menuCursorStruct* arg0)
{
    *arg0 = D_8038EFE0_1AB830;
    D_8038EFE0_1AB830.button_tap = 0;
    D_8038EFE0_1AB830.button_update = 0;

    return arg0;
}

// 0x8037D3DC
menuCursorStruct* func_ovl8_8037D3DC(menuCursorStruct *arg0)
{
    D_8038EFE0_1AB830 = *arg0;
    return arg0;
}

// 0x8037D404
menuCursorStruct* func_ovl8_8037D404(menuCursorStruct* arg0, s32 arg1)
{
    D_8038EFEC_1AB83C = arg1;
    D_8038EFF0_1AB840 = *arg0;

    return arg0;
}

// 0x8037D434
menuCursorStruct* func_ovl8_8037D434(menuCursorStruct* arg0)
{
    D_8038EFE0_1AB830 = *arg0;

    return arg0;
}

// 0x8037D45C
void func_ovl8_8037D45C()
{
    D_8038EFE0_1AB830.button_update = 0;
    D_8038EFE0_1AB830.button_tap = 0;
}

// 0x8037D470
void func_ovl8_8037D470(u16 totalBits, u16 rowBits, u8* src, u8* dest) 
{
    u32 bitInByte;
    u32 bitInRow;
    
    for(bitInRow = bitInByte = 0; totalBits != 0; totalBits--)
    {
        u32 b, nibble;
        u8* temp_dest = dest++; 
        
        nibble = ((bitInByte & 1) ? (*src & 0xF) : ((*src >> 4) & 0xF)); 
        *temp_dest = nibble;
        
        if (FALSE);
        b = bitInByte++;

        if ((b & 1) != 0) 
        {
            bitInByte = 0;
            src++;
        }

        bitInRow++;

        if (bitInRow == rowBits) 
        {
            if (bitInByte != 0) 
            {
                src++;
            }

            bitInByte = 0;
            bitInRow = 0;
        }
    }
}

// 0x8037D518
void func_ovl8_8037D518(u16 totalPixels, u16 rowPixels, u8* src, u8* dest) 
{
    u32 pixelInByte;
    u32 bitInRow;
    
    for(bitInRow = pixelInByte = 0; totalPixels != 0; totalPixels--)
    {
        u32 b = pixelInByte;
        u8* temp_dest = dest++; *temp_dest = (((*src >> (3 - (b & 3) << 1))) & 3);
        
        if (FALSE);
        b = pixelInByte++;

        if ((b & 3) == 3) 
        {
            pixelInByte = 0;
            src++;
        }

        bitInRow++;

        if (bitInRow == rowPixels) 
        {
            if (pixelInByte != 0) 
            {
                src++;
            }

            pixelInByte = 0;
            bitInRow = 0;
        }
    }
}

// 0x8037D5AC
void func_ovl8_8037D5AC(u16 totalPixels, u16 rowPixels, u8* src, u8* dest) 
{
    u32 pixelInByte;
    u32 bitInRow;
    
    for(bitInRow = pixelInByte = 0; totalPixels != 0; totalPixels--)
    {
        u32 b = pixelInByte;
        u8* temp_dest = dest++; *temp_dest = ((*src >> (7 - (b & 7))) & 1);
        
        if (FALSE);
        b = pixelInByte++;

        if ((b & 7) == 7) 
        {
            pixelInByte = 0;
            src++;
        }

        bitInRow++;

        if (bitInRow == rowPixels) 
        {
            if (pixelInByte != 0) 
            {
                src++;
            }

            pixelInByte = 0;
            bitInRow = 0;
        }
    }
}

// 0x8037D63C
void func_ovl8_8037D63C() 
{
    db4Bytes thing[2];

    thing[1] = D_ovl8_8038A890[0];
    thing[0] = D_ovl8_8038A890[1];
    D_8038F000_1AB850.arr[0] = D_8038F000_1AB850.arr[1] = 0;
    D_8038F048_1AB898 = 0;
    
    func_ovl8_8037D990(0x10);
    func_ovl8_8037D9D0(&thing[1]);
    func_ovl8_8037D9B4(&thing[0]);
    func_ovl8_8037D9EC(0, 0);
    func_ovl8_8037D99C(0);
    func_ovl8_8037D9A8(0);
}

// 0x8037D6D4
void func_ovl8_8037D6D4(DBFont* dbFont)
{
	u16 temp_a1;
	s32 var_v0;

	D_8038F008_1AB858 = *dbFont;
	temp_a1 = (D_8038F008_1AB858.width * D_8038F008_1AB858.bits_per_pixel) & 0xFFFF;
	var_v0 = (temp_a1 & 7 ? 1 : 0);

	D_8038F030_1AB880 = ((temp_a1 / 8) + var_v0) * D_8038F008_1AB858.height;
	D_8038F034_1AB884 = D_8038F008_1AB858.bits_per_pixel == 1 ? 1.0f : (f32) (1 << D_8038F008_1AB858.bits_per_pixel);

	if (D_8038F008_1AB858.bits_per_pixel == 1)
	{
		D_8038F03C_1AB88C = func_ovl8_8037D5AC;
		return;
	}
	if (D_8038F008_1AB858.bits_per_pixel == 2)
	{
		D_8038F03C_1AB88C = func_ovl8_8037D518;
		return;
	}
	D_8038F03C_1AB88C = func_ovl8_8037D470;
}

// 0x8037D7D4
void func_ovl8_8037D7D4(dbUnknown8_13 *arg0) {
    u16 temp_a1 = 0;
    s32 var_v0 = 0;

    D_8038F020_1AB870 = *arg0;
    temp_a1 = D_8038F020_1AB870.width * D_8038F020_1AB870.bits_per_pixel;
    var_v0 = (temp_a1 & 7 ? 1 : 0);
    
    D_8038F032_1AB882 = ((temp_a1 / 8) + var_v0) * D_8038F020_1AB870.height;
    D_8038F038_1AB888 = D_8038F020_1AB870.bits_per_pixel == 1 ? 1.0f : (f32) (1 << D_8038F020_1AB870.bits_per_pixel);

    if (D_8038F020_1AB870.bits_per_pixel == 1)
    {
        D_8038F040_1AB890 = &func_ovl8_8037D5AC;
        return;
    }
    if (D_8038F020_1AB870.bits_per_pixel == 2)
    {
        D_8038F040_1AB890 = &func_ovl8_8037D518;
        return;
    }
    D_8038F040_1AB890 = &func_ovl8_8037D470;
}

// 0x8037D8CC
void func_ovl8_8037D8CC(char* source) 
{
	char *buffer = D_8038FBA8_1AC3F8;
    char *targetCurrent = NULL;
    char *sourceCurrent;
    s32 count = 15;
    
	do
	{
        targetCurrent = buffer++;
        sourceCurrent = source++;
        *targetCurrent = *sourceCurrent;
	}
    while (count--);
}

// 0x8037D908
void func_ovl8_8037D908(dbUnknownS14* arg0)
{
    D_8038FB90_1AC3E0 = *arg0;

    func_ovl8_8037D9B4(&D_8038FB98_1AC3E8);
}

// 0x8037D95C
void func_ovl8_8037D95C(dbUnknownS14* arg0)
{
    *arg0 = D_8038FB90_1AC3E0;
}

// 0x8037D990
void func_ovl8_8037D990(s32 arg0)
{
    D_8038FB90_1AC3E0.dbUnknownS14_0x0 = arg0;
}

// 0x8037D99C
void func_ovl8_8037D99C(s32 arg0)
{
    D_8038FB90_1AC3E0.dbUnknownS14_0xC = arg0;
}

// 0x8037D9A8
void func_ovl8_8037D9A8(s32 arg0)
{
    D_8038FB90_1AC3E0.dbUnknownS14_0x10 = arg0;
}

// 0x8037D9B4
void func_ovl8_8037D9B4(db4Bytes* arg0)
{
    D_8038FB90_1AC3E0.dbUnknownS14_0x8 = *arg0;
}

// 0x8037D9D0
void func_ovl8_8037D9D0(db4Bytes* arg0)
{
    D_8038FB90_1AC3E0.dbUnknownS14_0x4 = *arg0;
}

// 0x8037D9EC
void func_ovl8_8037D9EC(u16 arg0, u16 arg1)
{
    D_8038F044_1AB894 = arg0;
    D_8038F046_1AB896 = arg1;
}

// 0x8037DA08
u32 func_ovl8_8037DA08(u8 arg0, u8 arg1) 
{
    arg0 = arg0 & 0x7F;
    arg1 = arg1 & 0x7F;
    
    arg1 = (arg0 & 1) ? arg1 + ' ' - 1 : arg1 + '~' - 1;
    arg0 = ((arg0 - '!') >> 1) + 0x81;
    
    if (arg1 > '~') 
    {
        arg1 += 1;
    }
    
    if (arg0 >= 0xA0) 
    {
        arg0 += 'A' - 1;
    }
    
    return (((arg0 * 0xC0) + arg1) - 0x6100) * D_8038F032_1AB882;
}

void func_ovl8_8037DAA0(Sprite *arg0, u8 arg1)
{
    u8 *glyph;
    u32 id;
    u32 index;
    u32 pad;
    
    func_ovl8_80377134(&D_8038FB98_1AC3E8, arg0->bmsiz);
    
    if (D_8038F048_1AB898 & 0xFF00)
    {
        D_8038F048_1AB898 += arg1;
        id = func_ovl8_8037DA08(D_8038F048_1AB898 >> 8, D_8038F048_1AB898);
        glyph = &D_8038F020_1AB870.glyphs[id];
        
        D_8038F040_1AB890(D_8038F020_1AB870.width * D_8038F020_1AB870.height, D_8038F020_1AB870.width, glyph, &D_8038F050_1AB8A0);
        func_ovl8_8037DFF8(arg0, D_8038F020_1AB870.width, D_8038F020_1AB870.height, 0, 0, &D_8038F050_1AB8A0, &D_8038F290_1ABAE0, D_8038F038_1AB888);
        func_ovl8_803798A0(arg0, &D_8038F290_1ABAE0, D_8038F020_1AB870.width, D_8038F020_1AB870.height, &D_8038F000_1AB850, D_8038FB90_1AC3E0.dbUnknownS14_0x0);
        
        D_8038F000_1AB850.arr[0] += D_8038F020_1AB870.width + D_8038F044_1AB894;
        D_8038F048_1AB898 = 0;
    }
    else if (arg1 & 0x80)
    {
        D_8038F048_1AB898 = (arg1 << 4) << 4;
    }
    else 
    {
        if (((arg1 >= ' ') && (arg1 <= '~')) || ((arg1 >= 0xA0) && (arg1 < 0xE0)))
        {
            id = (arg1 >= 0xA1) ? (arg1 - 'A') : (arg1 - ' ');
            glyph = (D_8038F008_1AB858.glyphs, D_8038F008_1AB858.glyphs) + (D_8038F030_1AB880 * id);
            index = id;
            
            D_8038F03C_1AB88C(D_8038F008_1AB858.width * D_8038F008_1AB858.height, D_8038F008_1AB858.width, glyph, &D_8038F050_1AB8A0);
            func_ovl8_8037DFF8(arg0, D_8038F008_1AB858.width, D_8038F008_1AB858.height, D_8038F008_1AB858.padding[index].left_padding, D_8038F008_1AB858.padding[index].right_padding, &D_8038F050_1AB8A0, &D_8038F290_1ABAE0, D_8038F034_1AB884);
            func_ovl8_803798A0(arg0, &D_8038F290_1ABAE0, D_8038F008_1AB858.width - D_8038F008_1AB858.padding[index].left_padding - D_8038F008_1AB858.padding[index].right_padding, D_8038F008_1AB858.height, &D_8038F000_1AB850, D_8038FB90_1AC3E0.dbUnknownS14_0x0);
            
            D_8038F000_1AB850.arr[0] += ((D_8038F008_1AB858.width - D_8038F008_1AB858.padding[index].left_padding) - D_8038F008_1AB858.padding[index].right_padding) + D_8038F044_1AB894;
        }
    }
}