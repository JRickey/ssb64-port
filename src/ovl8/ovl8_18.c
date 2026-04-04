#include "common.h"
#include <sys/develop.h>
#include <db/debug.h>

typedef s32 (*dbUiNodeHandler)(s32, DBMenu*);
typedef struct {
    u32 id;
    dbUiNodeHandler handler;
} dbUiNodeTypeEntry;

// 0x8038FBC0
extern dbUiNodeTypeEntry gDBUiNodeTypeTable[256];

// 0x8038BD10
u16 gDBUiNodeTypeCount = 0;

extern s32 func_ovl8_80376164(s32, DBMenu*);
extern s32 func_ovl8_80376848(s32, DBMenu*);
extern s32 func_ovl8_8037C660(s32, DBMenu*);
extern s32 func_ovl8_8037CA60(s32, DBMenu*);
extern s32 func_ovl8_8037E830(s32, DBMenu*);
extern s32 func_ovl8_8037EE00(s32, DBMenu*);
extern s32 func_ovl8_8037FBC0(s32, DBMenu*);
extern s32 func_ovl8_80380EC0(s32, DBMenu*);
extern s32 func_ovl8_80382490(s32, DBMenu*);
extern s32 func_ovl8_80382710(s32, DBMenu*);
extern s32 func_ovl8_80382AE0(s32, DBMenu*);
extern s32 func_ovl8_80382D90(s32, DBMenu*);
extern s32 func_ovl8_803840C0(s32, DBMenu*);
extern s32 func_ovl8_80384460(s32, DBMenu*);
extern s32 func_ovl8_80385180(s32, DBMenu*);
extern s32 func_ovl8_80385460(s32, DBMenu*);
extern s32 func_ovl8_80385640(s32, DBMenu*);
extern s32 func_ovl8_80385A50(s32, DBMenu*);
extern s32 func_ovl8_803879A0(s32, DBMenu*);

u16 dbUiNodeTypeFindIndex(u32);

// 0x80381B70
void dbUiNodeTypeRegisterHandler(u32 id, dbUiNodeHandler handler) 
{
    u16 index;
    u16 i;

    i = index = dbUiNodeTypeFindIndex(id);

    if (index == 0) 
    {
        gDBUiNodeTypeCount++;
        i = gDBUiNodeTypeCount;
    }

    gDBUiNodeTypeTable[i-1].id = id;
    gDBUiNodeTypeTable[i-1].handler = handler;
}

// 0x80381BD0
s32 dbUiNodeDispatch(DBMenu *arg0, s32 arg1)
{
    u16 index = dbUiNodeTypeFindIndex(arg0->id);
    
    if (index != 0)
    {
        return gDBUiNodeTypeTable[index-1].handler(arg1, arg0);
    }
    
    return 0;
}

// 0x80381C2C
u16 dbUiNodeTypeFindIndex(u32 id) {
    u16 i;
    u16 index = 0;

    for (i = 0; i < gDBUiNodeTypeCount; i++) 
    {
        if (id == gDBUiNodeTypeTable[i].id) 
        {
            index = i + 1;
        }
    }
    
    return index + 0;
}

// 0x80381C80
dbUnknownLinkStruct* func_ovl8_80381C80(DBMenu* arg0)
{
    dbUnknownLinkStruct *root;
    dbUnknownLinkStruct *elem;
    DBMenu *local = arg0 + 1;

    root = dbUiNodeDispatch(arg0, NULL);

    while (TRUE)
    {
        
        if (local->unk_dbmenu_0x10 != 0)
        {
            DBMenu *local2 = local++;
            dbUiNodeDispatch(local2, root);
        }
        else break;
    }
    
    if (root) 
    {
        dbUnknownLinkStruct* v0;
        dbUnknownLinkStruct* v0_2;
        dbFunction* a2;
        
        elem = func_ovl8_80373160(root);
        
        v0 = elem->unk_dbunkstruct_0x38;
        if (v0->db_func && v0->db_func);
        
        while (elem) 
        {
            v0_2 = v0 = elem->unk_dbunkstruct_0x38;
            a2 = v0_2->db_func;
            v0_2->db_func[13].unk_dbfunc_0x4(a2[13].unk_dbfunc_0x0 + (uintptr_t)elem->unk_dbunkstruct_0x38);
            
            if (1);
            a2 = elem->unk_dbunkstruct_0x30;
            elem = ((dbFunction*)elem->unk_dbunkstruct_0x30)[15].unk_dbfunc_0x4(a2[15].unk_dbfunc_0x0 + (uintptr_t)elem);
        }

        v0 = root->unk_dbunkstruct_0x20;
        a2 = v0->db_func;
        if (1);
        v0->db_func[13].unk_dbfunc_0x4(a2[13].unk_dbfunc_0x0 + (uintptr_t)root->unk_dbunkstruct_0x20);
    }
    
    return root;
}

// 0x80381D40
void dbUiNodeListDispatch(s32 arg0, DBMenu *arg1)
{
    while (arg1->unk_dbmenu_0x10 != 0)
    {
        if (arg1->unk_dbmenu_0x10); // why
        
        switch (arg1->unk_dbmenu_0x10)
        {
            case 1:
            case 4:
            case 6:
                break;
            default:
                dbUiNodeDispatch(arg1, arg0);
                break;
        }
        arg1++;
    }
}

// 0x80381DC8
void dbUiNodeTypeRegisterHandlers(void)
{
    dbUiNodeTypeRegisterHandler('FWIN', func_ovl8_80376164);
    dbUiNodeTypeRegisterHandler('FHWN', func_ovl8_80376164);
    dbUiNodeTypeRegisterHandler('FFWN', func_ovl8_80376164);
    dbUiNodeTypeRegisterHandler('FFHW', func_ovl8_80376164);
    dbUiNodeTypeRegisterHandler('BWIN', func_ovl8_80376848);
    dbUiNodeTypeRegisterHandler('BHWN', func_ovl8_80376848);
    dbUiNodeTypeRegisterHandler('BFWN', func_ovl8_80376848);
    dbUiNodeTypeRegisterHandler('BFHW', func_ovl8_80376848);
    dbUiNodeTypeRegisterHandler('SBTN', func_ovl8_8037C660);
    dbUiNodeTypeRegisterHandler('STGB', func_ovl8_8037C660);
    dbUiNodeTypeRegisterHandler('SOPB', func_ovl8_8037C660);
    dbUiNodeTypeRegisterHandler('STCB', func_ovl8_8037C660);
    dbUiNodeTypeRegisterHandler('STBN', func_ovl8_8037E830);
    dbUiNodeTypeRegisterHandler('STTB', func_ovl8_8037E830);
    dbUiNodeTypeRegisterHandler('STOB', func_ovl8_8037E830);
    dbUiNodeTypeRegisterHandler('STTC', func_ovl8_8037E830);
    dbUiNodeTypeRegisterHandler('BBTN', func_ovl8_8037CA60);
    dbUiNodeTypeRegisterHandler('BTGB', func_ovl8_8037CA60);
    dbUiNodeTypeRegisterHandler('BOPB', func_ovl8_8037CA60);
    dbUiNodeTypeRegisterHandler('BOPT', func_ovl8_8037CA60);
    dbUiNodeTypeRegisterHandler('BTXB', func_ovl8_8037EE00);
    dbUiNodeTypeRegisterHandler('BTTB', func_ovl8_8037EE00);
    dbUiNodeTypeRegisterHandler('BTOB', func_ovl8_8037EE00);
    dbUiNodeTypeRegisterHandler('BTTC', func_ovl8_8037EE00);
    dbUiNodeTypeRegisterHandler('POPM', func_ovl8_80376848);
    dbUiNodeTypeRegisterHandler('SPOP', func_ovl8_80382710);
    dbUiNodeTypeRegisterHandler('BPOP', func_ovl8_80382AE0);
    dbUiNodeTypeRegisterHandler('SVLM', func_ovl8_8037FBC0);
    dbUiNodeTypeRegisterHandler('BVLM', func_ovl8_80380EC0);
    dbUiNodeTypeRegisterHandler('BCAP', func_ovl8_80382490);
    dbUiNodeTypeRegisterHandler('SCAP', func_ovl8_80385640);
    dbUiNodeTypeRegisterHandler('CAM ', func_ovl8_80382D90);
    dbUiNodeTypeRegisterHandler('SLED', func_ovl8_803840C0);
    dbUiNodeTypeRegisterHandler('BLED', func_ovl8_80384460);
    dbUiNodeTypeRegisterHandler('SLST', func_ovl8_80385180);
    dbUiNodeTypeRegisterHandler('BLST', func_ovl8_80385460);
    dbUiNodeTypeRegisterHandler('SBPP', func_ovl8_80385A50);
    dbUiNodeTypeRegisterHandler('SBIP', func_ovl8_803879A0);
}
