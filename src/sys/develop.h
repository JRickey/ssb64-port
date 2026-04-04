#ifndef _DEVELOP_H_
#define _DEVELOP_H_

#include <ssb_types.h>
#include <PR/ultratypes.h>
#include <sys/obj.h>
#include <db/dbtypes.h>

typedef enum dbObjectDisplayMode
{
	nDBDisplayModeMaster,
	nDBDisplayModeHitCollisionFill,
	nDBDisplayModeHitAttackOutline,
	nDBDisplayModeMapCollision

} dbObjectDisplayMode;

typedef struct dbUnknownLinkStruct dbUnknownLinkStruct;
typedef struct dbUnknownLink dbUnknownLink;
typedef struct dbFunction dbFunction;
typedef struct dbUnknown16 dbUnknown16;
typedef struct dbBytesContainer dbBytesContainer;
typedef struct db4Bytes db4Bytes;
typedef struct db2Shorts db2Shorts;
typedef struct db4Shorts db4Shorts;
typedef struct dbUnAl1 dbUnAl1;
typedef struct dbBytesCopy dbBytesCopy;
typedef struct dbTestMenu dbTestMenu;
typedef struct dbMenuSprite dbMenuSprite;
typedef struct dbUnknown2 dbUnknown2;
typedef struct dbUnknown3 dbUnknown3;
typedef struct dbUnknownS38 dbUnknownS38;
typedef struct dbUnkStruct dbUnkStruct;
typedef struct dbUnknownStructS18 dbUnknownStructS18;
typedef struct dbUnknownStruct2S18 dbUnknownStruct2S18;

struct dbMenuSprite
{
    GObj *unk_dbmenusprite_0x0;
    GObj *unk_dbmenusprite_0x4;
    Sprite *sprite;
    s32 unk_dbmenusprite_0xC;
    dbMenuSprite *unk_dbmenusprite_0x10;
    u8 filler_0xC[0x48 - 0x14];
    GObj *unk_dbmenusprite_0x48;
};

struct db4Bytes
{
    u8 arr[4];
};

struct db2Shorts
{
    s16 arr[2];
};

struct db4Shorts
{
    union
    {
        s16 arr[4];
        u16 uarr[4];
    };
};

struct dbBytesContainer
{
    db4Shorts unk_dbbytescont_0x0;
};

struct dbUnAl1
{
    u16 unk_dbunal1_0x0;
    db4Shorts unk_dbunal1_0x2;
};

struct dbUnknown16
{
    s16 unk_dbunk16_0x0;
    s16 unk_dbunk16_0x2;
    s16 unk_dbunk16_0x4;
};

struct dbFunction
{
    s16 unk_dbfunc_0x0;
    sb32 (*unk_dbfunc_0x4)();
};

struct dbBytesCopy
{
    DBMenuPosition position;
    SYColorRGBA text_color;
    SYColorRGBA bg_color;
    u16 unk_dbbytescopy_0x10;
    s32 unk_dbbytescopy_0x14;
    s32 unk_dbbytescopy_0x18;
};

struct dbUnknownLinkStruct
{
    u16 unk_dbunkstruct_0x0;
    DBMenuPosition position;
    s32 unk_dbunkstruct_0xC;
    s32 id;
    SYColorRGBA text_color;
    SYColorRGBA bg_color;
    union {
        dbUnknownLinkStruct *link;
        s16 s[2];
    } unk_dbunkstruct_0x1C;
    dbUnknownLinkStruct *unk_dbunkstruct_0x20;
    u16 unk_dbunkstruct_0x24;
    sb32 unk_dbunkstruct_0x28;
    u16 unk_dbunkstruct_0x2C;
    s32 unk_dbunkstruct_0x30;
    void** unk_dbunkstruct_0x34;
    dbUnknownLinkStruct *unk_dbunkstruct_0x38;
    s32 unk_dbunkstruct_0x3C;
    s32 unk_dbunkstruct_0x40;
    s32 unk_dbunkstruct_0x44;
    GObj *unk_dbunkstruct_0x48;
    dbUnknownLinkStruct *unk_dbunkstruct_0x4C;
    db4Bytes unk_dbunkstruct_0x50;
    sb32(*unk_dbunkstruct_0x54)();
    dbFunction *db_func;                            // Points to a whole bunch of function pointers?
    DBMenuPosition unk_dbunkstruct_0x5C;
    s32 unk_dbunkstruct_0x64;
};

struct dbTestMenu
{
    GObj *unk_dbtestmenu_0x0;
    GObj *unk_dbtestmenu_0x4;
    db4Shorts *unk_dbtestmenu_0x8;
    dbFunction *db_func;
    dbUnknownLinkStruct *unk_dbtestmenu_0x10;
    dbUnknownLinkStruct unk_dbtestmenu_0x14;
};

struct dbUnknown2
{
    db4Bytes unk_dbmorethan_0x0;
};

struct dbUnknown3
{
    s32 unk_dbunk3_0x0;
    s32 unk_dbunk3_0x4;
    s32 unk_dbunk3_0x8;
    u8 filler_0xC[0xC];
    dbFunction *unk_dbunk3_0x18;
    dbUnknownLink *unk_dbunk3_0x1C;
    dbUnknown3 *unk_dbunk3_0x20;
    dbUnknown3 *unk_dbunk3_0x24;
    dbUnknown3 *unk_dbunk3_0x28;
    void *unk_dbunk3_0x2C;
    u32 unk_dbunk3_0x30;
    void *unk_dbunk3_0x34;
    dbUnknownLinkStruct *unk_dbunk3_0x38;
    u8 filler_0x3C[0x40 - 0x3C];
    char** unk_dbunk3_0x40;
    dbUnknown3 *unk_dbunk3_0x44;
    dbUnknown3 *unk_dbunk3_0x48;
    void *unk_dbunk3_0x4C;
    void *unk_dbunk3_0x50;
    void *unk_dbunk3_0x54;
    void *unk_dbunk3_0x58;
};

struct dbUnknownLink
{
    dbUnknownLink *dbunklink_0x0;
    s32 unk_dbunklink_0x4;
    dbUnknownLink *unk_dbunklink_0x8;
    void *unk_dbunklink_0xC;
    void *unk_dbunklink_0x10;
    dbUnknownLinkStruct unk_dbunklink_0x14;
};

typedef struct dbUnknown8_S28 {
	s32 dbUnknown8_S28_0x0[6];
	s32 str;
	s32 dbUnknown8_S28_0x20[3];
} dbUnknown8_S28;

struct dbUnknownS38
{
    s32 unk_dbunks38_0x0;
    s32 unk_dbunks38_0x4;
    s32 unk_dbunks38_0x8;
    s32 unk_dbunks38_0xC;
    s32 unk_dbunks38_0x10;
    s32 unk_dbunks38_0x14;
    dbFunction *unk_dbunks38_0x18;
    dbUnknownLink *unk_dbunks38_0x1C;
    dbUnknownLinkStruct *unk_dbunks38_0x20;
    void *unk_dbunks38_0x24;
    void *unk_dbunks38_0x28;
    void *unk_dbunks38_0x2C;
    dbFunction *unk_dbunks38_0x30;
    dbUnknownLink* unk_dbunks38_0x34;
    dbUnknownLinkStruct unk_dbunks38_0x38;
    dbUnknownLink unk_dbunks38_0x94;
};

struct dbUnkStruct
{
    s32 unk0;
    s32 unk4;
    s32 unk8;
    dbFunction *db_func;
};

struct dbUnknownStructS18
{
    s32 unk_0x0;
    s32 unk_0x4;
    s32 unk_0x8;
    s32 unk_0xC;
    s32 unk_0x10;
    s32 unk_0x14;
};

struct dbUnknownStruct2S18
{
    s32 unk_0x0;
    s32 unk_0x4;
    s32 unk_0x8;
    s32 unk_0xC;
    s32 unk_0x10;
    u16 unk_0x14;
    u16 unk_0x16;
};

struct dbUnknownStructS1C
{ 
    s32 unk_0x0;
    s32 unk_0x4;
    s32 unk_0x8;
    s32 unk_0xC;
    s32 unk_0x10;
    s32 unk_0x14;
    dbFunction * dbfunc;
};

typedef struct dbUnknown5
{
    f32 unk_dbunk5_0x0;
    f32 unk_dbunk5_0x4;
    f32 unk_dbunk5_0x8;
    union {
        u8 b;
        s32 w;
        char *str;
    } unk_dbunk5_0xC;
    u8 filler_0x10[0x4];
    u32 unk_dbunk5_0x14;
    dbFunction *db_func;
    u8 filler_0x1C[0x2];
    s16 unk_dbunk5_0x1E;
    union {
        f32 f;
        dbUnknownLinkStruct *ls;
    } unk_dbunk5_0x20;
    s32 unk_dbunk5_0x24;
    s32 unk_dbunk5_0x28;
    s32 unk_dbunk5_0x2C;
    dbUnknownLinkStruct *unk_dbunk5_0x30;
    dbUnknownLink *unk_dbunk5_0x34;
    dbUnknownLinkStruct *unk_dbunk5_0x38;
    union {
        u8 *str;
        u16 u_16;
        s16 s_16;
        s32 w;
        dbUnknownLinkStruct *ls;
    } unk_dbunk5_0x3C;
    union {
        u8 *str;
        u16 u_16;
        s16 s_16;
        Vec2h v;
        dbUnknownLinkStruct *ls;
        GObj *gobj;
    } unk_dbunk5_0x40;
    union {
        s16 s_16;
        Vec2h v;
        dbUnknownLinkStruct *ls;
    } unk_dbunk5_0x44;
    dbUnknownLinkStruct *unk_dbunk5_0x48;
    dbUnknownLinkStruct *unk_dbunk5_0x4C;
    s32 unk_dbunk5_0x50;
    s32 unk_dbunk5_0x54;
    dbFunction *unk_dbunk5_0x58;
    dbUnknownLinkStruct *unk_dbunk5_0x5C;
    dbUnknownLinkStruct *unk_dbunk5_0x60;
    dbUnknownLinkStruct *unk_dbunk5_0x64;
    u8 filler_0x68[0x4];
    dbUnknownLinkStruct *unk_dbunk5_0x6C;
    u8 filler_0x70[0x24];
    dbUnknownLinkStruct *unk_dbunk5_0x94;
    u8 unk_dbunk5_0x98[0x4];
    dbUnknownLinkStruct *unk_dbunk5_0x9C;
    u8 filler_0xA0[0x8];
    s32 unk_dbunk5_0xA8;
    dbUnknownS38 *unk_dbunk5_0xAC;
    s32 unk_dbunk5_0xB0;
    dbUnknownLinkStruct *unk_dbunk5_0xB4;
    s32 unk_dbunk5_0xB8[1];
    s32 unk_dbunk5_0xBC;
    dbUnknownLink *unk_dbunk5_0xC0;
    s32 unk_dbunk5_0xC4;
    dbUnknownLink *unk_dbunk5_0xC8;
    u8 filler_0xCC[0x4];
    s32 unk_dbunk5_0xD0;
    s32 unk_dbunk5_0xD4;
    s32 unk_dbunk5_0xD8;
    s32 unk_dbunk5_0xDC;
    s32 unk_dbunk5_0xE0;
    s32 unk_dbunk5_0xE4;
    u8 filler_0xE8[0x24];
    dbUnknownLink *unk_dbunk5_0x10C;
    u8 filler_0x110[0x4];
    dbUnknownLink *unk_dbunk5_0x114;
    u8 filler_0x118[0xC];
    dbUnknownLink *unk_dbunk5_0x124;
    u8 filler_0x128[0xC];
    s32 unk_dbunk5_0x134;
    u8 filler_0x138[0x8];
} dbUnknown5;

typedef struct dbUnknown5_2
{
    f32 dbUnknown5_2_unk_f32_0x0;
    struct dbUnknown5_2 *dbUnknown5_2_0x4;
    u8 dbUnknown5_2_filler_0x8[0x4];
    char dbUnknown5_2_string[0x10];
    u8 dbUnknown5_2_filler_0x1C[0x14];
    dbFunction *dbUnknown5_2_db_func;  
    u8  dbUnknown5_2_filler_0x34[0x08];
    s16 dbUnknown5_2_unk_0x3C;
    s16 dbUnknown5_2_unk_0x3E;
    s32 *dbUnknown5_2_unk_0x40;       
    u8  dbUnknown5_2_filler_0x44[0x02];
    s16  dbUnknown5_2_unk_0x46;
    s32  dbUnknown5_2_unk_0x48;
    dbUnknown5 *dbUnknown5_2_0x4C;
} dbUnknown5_2;

typedef struct dbUnknownStructSC
{ 
    u32 unk_0x0;
    dbUnknown5_2* unk_0x4;   
    void** unk_0x8;       
    s32 unk_0xC;          
} dbUnknownStructSC;

typedef struct dbUnknown7
{
    s32 unk0;
    s32 unk4;
    s32 *unk8;
    s32 *unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    dbUnknownLink *unk1C;
    dbUnknownLinkStruct *unk20;
    s32 unk24;
    void *unk28;
    void *unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C[2];
    s32 unk44;
    s32 unk48[0x60/4];
    s32 unkA8;
    s32 unkAC[5];
    s32 unkC0;
} dbUnknown7;

typedef struct dbBytesCopy2
{
    db4Shorts unk_dbbytescopy_0x0;
    db4Bytes unk_dbbytescopy_0x8;
    db4Bytes unk_dbbytescopy_0xC;
    u16 unk_dbbytescopy_0x10;
    s32 unk_dbbytescopy_0x14;
    s32 unk_dbbytescopy_0x18;
    s16 unk_dbbytescopy_0x1C;
} dbBytesCopy2;

typedef struct dbUnknownS14 {
    s32 dbUnknownS14_0x0;
    db4Bytes dbUnknownS14_0x4;
    db4Bytes dbUnknownS14_0x8;
    s32 dbUnknownS14_0xC;
    s32 dbUnknownS14_0x10;
} dbUnknownS14;

#endif
