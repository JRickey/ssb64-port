#include "common.h"
#include <sys/develop.h>
#include <db/debug.h>

extern DBMenuPosition D_ovl8_80389F48;
extern db4Bytes D_ovl8_80389F4C;
extern DBMenu D_ovl8_8038E1E0[3];
extern DBMenu D_ovl8_8038E2D4;
extern DBMenu D_ovl8_8038E564;
extern s32 D_ovl8_8038E58C;
extern s32* D_ovl8_8038E590;
extern s32* D_ovl8_8038E594;
extern s32 D_803903C0_1ACC10;
extern s32 D_803903C4_1ACC14;
extern s32 D_803903C8_1ACC18;

extern void func_ovl8_8037481C(Vec3i*, s32);
extern void func_ovl8_8037DFCC(s16, s16);
extern void gcSleepCurrentGObjThread(s32);
void func_ovl8_80385F44(s32, s32);
void func_ovl8_803863A4(s32, s32, const char*);
s32 func_ovl8_80386488(s32, s32);
void func_ovl8_8038649C();
void func_ovl8_803864CC(s32, DBMenu*);

// 0x80385E10
void func_ovl8_80385E10(s32 arg0, ...)
{
    func_ovl8_803863A4(0x30, 1, &arg0);

    D_ovl8_8038E1E0[1] = D_ovl8_8038E564;

    D_803903C0_1ACC10 = func_ovl8_80381C80(&D_ovl8_8038E1E0);
}

// 0x80385E98
void func_ovl8_80385E98(s32 arg0)
{
    s32 temp_v0;

    func_ovl8_803863A4(arg0, 0, 0);
    D_803903C0_1ACC10 = func_ovl8_80381C80(&D_ovl8_8038E1E0);
    func_ovl8_8037481C(D_803903C0_1ACC10, func_ovl8_80386488);
}

// 0x80385EE4
void func_ovl8_80385EE4(void)
{
    func_ovl8_8038649C();
}

// 0x80385F04
void func_ovl8_80385F04(const char *str, ...)
{
    s32 unused;
    char sp18[250];

    func_ovl8_80386BE0(sp18, &str);
    func_ovl8_80385F44(1, sp18);
}

// 0x80385F44
void func_ovl8_80385F44(s32 arg0, s32 arg1) {
    dbBytesCopy sp2C;
    s32 temp_s0;
    s32 temp_v0;

    temp_s0 = D_ovl8_8038E1E0[0].unk_dbmenu_0x24;
    
    sp2C.position = D_ovl8_8038E1E0[0].position;
    sp2C.position.x = 0;
    sp2C.position.y = 0;
    
    func_ovl8_80377AEC(temp_s0, &sp2C.position, &D_ovl8_80389F48, 4);
    func_ovl8_80378064(temp_s0, &sp2C.position, &D_ovl8_80389F4C, 4);
    
    sp2C.position.x = sp2C.position.y = 2;
    sp2C.position.w -= 4;
    sp2C.position.h -= 4;
    func_ovl8_80378064(temp_s0, &sp2C.position, &D_ovl8_80389F4C, 4);
    
    if (arg1 != 0) 
    {
        func_ovl8_8037D95C(&sp2C.text_color);
        func_ovl8_8037D9B4(&D_ovl8_8038E1E0[0].bg_color);
        temp_v0 = func_ovl8_8037E7A8(arg1);
        
        if (arg0 == 1) 
        {
            func_ovl8_8037DFCC(((sp2C.position.x + (sp2C.position.w / 2)) - (temp_v0 / 2)), ((sp2C.position.y + (sp2C.position.h / 2)) - (func_ovl8_8037E80C() / 2)));
        } else {
            func_ovl8_8037DFCC(4, ((sp2C.position.y + (sp2C.position.h / 2)) - (func_ovl8_8037E80C() / 2)));
        }
        
        func_ovl8_8037DD60(temp_s0, arg1);
        func_ovl8_8037D908(&sp2C.text_color);
    }
}

// 0x8038612C
void func_ovl8_8038612C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) 
{
    s32 pad[2];
    
    func_ovl8_803863A4(0x50, 0, &pad[2]);
    
    D_ovl8_8038E2D4.unk_dbmenu_0x24 = &D_ovl8_8038E58C;
    D_ovl8_8038E1E0[1] = D_ovl8_8038E2D4;
    
    func_ovl8_803864CC(1, &D_ovl8_8038E1E0[1]);
    
    D_ovl8_8038E1E0[1].unk_dbmenu_0x18 = 1;
    D_803903C0_1ACC10 = func_ovl8_80381C80(D_ovl8_8038E1E0);
    
    func_ovl8_8037481C(D_803903C0_1ACC10, func_ovl8_80386488);
    
    while (1 != D_803903C8_1ACC18) 
    {
        gcSleepCurrentGObjThread(1);
    }
    
    func_ovl8_8038649C();
}

// 0x80386228
s32 func_ovl8_80386228(s32 arg0, ...) 
{
    func_ovl8_803863A4(0x50, 0, &arg0);
    
    D_ovl8_8038E2D4.unk_dbmenu_0x24 = &D_ovl8_8038E590;
    D_ovl8_8038E1E0[1] = D_ovl8_8038E2D4;
    
    func_ovl8_803864CC(1, &D_ovl8_8038E1E0[1]);
    
    D_ovl8_8038E1E0[1].unk_dbmenu_0x18 = 1;
    D_ovl8_8038E2D4.unk_dbmenu_0x24 = &D_ovl8_8038E594;
    
    D_ovl8_8038E1E0[2] = D_ovl8_8038E2D4;
    
    func_ovl8_803864CC(2, &D_ovl8_8038E1E0[2]);
    
    D_ovl8_8038E1E0[2].unk_dbmenu_0x18 = 2;
    
    D_803903C0_1ACC10 = func_ovl8_80381C80(&D_ovl8_8038E1E0);
    
    func_ovl8_8037481C(D_803903C0_1ACC10, &func_ovl8_80386488);
    
    while ((1 != D_803903C8_1ACC18) && (2 != D_803903C8_1ACC18)) 
    {
        gcSleepCurrentGObjThread(1);
    }
    
    func_ovl8_8038649C();
    
    return D_803903C8_1ACC18;
}

// 0x803863A4
void func_ovl8_803863A4(s32 arg0, s32 arg1, const char *str) 
{
    s32 var_v0;
    s32 width;
    s32 sp12C; 
    s32 sp128;
    s32 sp134;
    DBMenu* menu = &D_ovl8_8038E1E0[0];
    char buffer[250];

    if (str) 
    {
        func_ovl8_80386BE0(buffer, str);
        width = func_ovl8_8037E7A8(buffer);
    } 
    else 
    {
        width = 0;
    }
    
    var_v0 = sp12C = (width + arg0);
    if (var_v0 >= 0x119) 
    {
        var_v0 = 0x118;
    }
    
    sp128 = 0x18;
    sp134 = var_v0;
    func_ovl8_8037B85C(2, &sp12C, &sp128);
    
    D_ovl8_8038E1E0[0].unk_dbmenu_0x24 = D_803903C4_1ACC14 = func_ovl8_8037ACAC(var_v0, 0x18, 2, sp12C, sp128);
    D_ovl8_8038E1E0[0].position.w = var_v0;
    menu->position.h = 0x18;
    
    func_ovl8_80385F44(arg1, buffer);
    func_ovl8_80377108(&D_ovl8_8038E1E0[1], 0, 0xC8);
    
    D_803903C8_1ACC18 = 0;
}

// 0x80386488
s32 func_ovl8_80386488(s32 arg0, s32 arg1)
{
	D_803903C8_1ACC18 = arg0;
	return 0;
}

// 0x8038649C
void func_ovl8_8038649C()
{
	func_ovl8_8037B3E4(D_803903C4_1ACC14);
	func_ovl8_8037488C(D_803903C0_1ACC10);
}

// 0x803864CC
void func_ovl8_803864CC(s32 arg0, DBMenu* arg1)
{
	s32 temp_v1;
	s32 temp_lo;

	temp_v1 = func_ovl8_8037E7A8(arg1->unk_dbmenu_0x24) + 4;
	temp_lo = arg0 * temp_v1;
	arg1->position.x = ((D_ovl8_8038E1E0[0].position.w - temp_lo) - (arg0 * 2)) - 4;
	arg1->position.y = 4;
	arg1->position.w = temp_v1;
	arg1->position.h = 0x10;
}