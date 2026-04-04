#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>

extern dbUnknownLink D_ovl8_80388AE0;
extern dbFunction D_ovl8_80388B40[];
extern db4Bytes D_ovl8_80388C98;
extern void *D_ovl8_80388CA0[];
extern dbFunction D_ovl8_80388D08[];

void func_ovl8_80372420();
void func_ovl8_8037277C(s32 arg0);
void func_ovl8_80377AEC(char* arg0, s32 arg1, s32 arg2, s32 arg3);

dbUnknownLink* func_ovl8_80371FB0(dbUnknownLink *arg0, dbUnknownLinkStruct *arg1)
{
	if (arg0 != NULL || (arg0 = func_ovl8_803717A0(0x70)))
	{
		if (arg1 == NULL)
		{
			arg1 = &arg0->unk_dbunklink_0x14;
			func_ovl8_803717E0(&arg0->unk_dbunklink_0x14);
		}
		arg0->unk_dbunklink_0x10 = arg1,
			arg0->unk_dbunklink_0xC = &D_ovl8_80388AE0,
			arg1->db_func = D_ovl8_80388B40;
		func_ovl8_80372420(arg0, 0);
	}
	return arg0;
}

void* func_ovl8_8037203C(dbUnknownLink *arg0, dbUnknownLinkStruct *arg1, void *arg2, void *arg3)
{
	if (arg0 != NULL || (arg0 = func_ovl8_803717A0(0x70)))
	{
		if (arg1 == NULL)
		{
			arg1 = &arg0->unk_dbunklink_0x14;
			func_ovl8_803717E0(&arg0->unk_dbunklink_0x14);
		}
		arg0->unk_dbunklink_0x10 = (void*)arg1,
			arg0->unk_dbunklink_0xC = &D_ovl8_80388AE0,
			arg1->db_func = D_ovl8_80388B40;

		func_ovl8_80371ECC(arg0->unk_dbunklink_0x10, arg2);
		func_ovl8_80372420(arg0, arg3);
	}
	return arg0;
}

void func_ovl8_803720D4(dbTestMenu *arg0, u32 arg1)
{
	if (arg0 != NULL)
	{
		// Whitespace memes
		arg0->db_func = &D_ovl8_80388AE0,
		arg0->unk_dbtestmenu_0x10->db_func = D_ovl8_80388B40;

		if (arg0->unk_dbtestmenu_0x0 != NULL)
			gcEjectGObj(arg0->unk_dbtestmenu_0x0);

		if (arg0->unk_dbtestmenu_0x4 != NULL)
			gcEjectGObj(arg0->unk_dbtestmenu_0x4);

		if (arg1 != 0)
			func_ovl8_803718C4(arg0->unk_dbtestmenu_0x10, 0);

		if (arg1 & 1)
			func_ovl8_803717C0(arg0);
	}
}

void func_ovl8_8037217C(dbUnknownLink *arg0)
{
	dbMenuSprite *next;
	SObj *sobj;
	GObj *gobj;

	next = arg0->unk_dbunklink_0x10;
	gobj = next->unk_dbmenusprite_0x48;
	sobj = SObjGetStruct(gobj);

	sobj->sprite.rsp_dl_next = gSYTaskmanDLHeads[0];

	spDraw(&sobj->sprite);

	gSYTaskmanDLHeads[0] = sobj->sprite.rsp_dl_next - 1;

	gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

void func_ovl8_803721E8(dbTestMenu *arg0, dbUnknownLinkStruct *arg1)
{
	arg1->position.x = 0;
	arg1->unk_dbunkstruct_0x0 = 0;

	if (arg0->unk_dbtestmenu_0x8 != NULL)
	{
		arg1->position.y = arg0->unk_dbtestmenu_0x8->arr[2];
		arg1->position.w = arg0->unk_dbtestmenu_0x8->arr[3];
	}
	else
	{
		arg1->position.w = 0;
		arg1->position.y = 0;
	}
}

void func_ovl8_80372224(dbTestMenu *arg0)
{
	arg0->db_func[3].unk_dbfunc_0x4(arg0->db_func[3].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

void func_ovl8_80372250(dbTestMenu* arg0, s32 arg1, s32 arg2, s32 arg3)
{
	func_ovl8_80377AEC(arg0->db_func[9].unk_dbfunc_0x4(arg0->db_func[9].unk_dbfunc_0x0 + (uintptr_t)arg0), arg1, arg2, arg3);
}

void func_ovl8_803722A0(dbTestMenu *arg0, s32 arg1, s32 arg2, s32 arg3)
{
	func_ovl8_803787C0(arg1, arg0->db_func[9].unk_dbfunc_0x4(arg0->db_func[9].unk_dbfunc_0x0 + (uintptr_t)arg0), arg2, arg3);
}

void func_ovl8_803722F0(dbTestMenu *arg0, s32 arg1)
{
	dbUnknown2 sp1C;
	db4Bytes *fbytes = &D_ovl8_80388C98;

	sp1C.unk_dbmorethan_0x0 = *fbytes;

	arg0->db_func[5].unk_dbfunc_0x4(arg0->db_func[5].unk_dbfunc_0x0 + (uintptr_t)arg0, arg1, &sp1C, 1);
}

void func_ovl8_80372348(s32 arg0, s32 arg1, s32 arg2) {}

dbUnknownLink* func_ovl8_80372358(dbUnknownLink *arg0)
{
	return arg0->unk_dbunklink_0x8;
}

void func_ovl8_80372360(dbMenuSprite *mnsprite, Sprite *arg1)
{
	SObj *sobj;
	mnsprite->sprite = arg1;
	sobj = SObjGetStruct(mnsprite->unk_dbmenusprite_0x4);
	sobj->sprite = *arg1;
}

void* func_ovl8_803723AC(dbMenuSprite *arg0, s32 user_data, Sprite* sprite, void (*proc)(GObj*), void (*proc_display)(GObj*), sb32 low_priority)
{
	dbMenuSprite *mnsprite;
	SObj *sobj;
	void *thing = func_ovl8_8037BF68(user_data, sprite, proc, proc_display, &arg0->unk_dbmenusprite_0x0, &arg0->unk_dbmenusprite_0x4, low_priority);

	mnsprite = arg0->unk_dbmenusprite_0x10;
	mnsprite->unk_dbmenusprite_0x48 = arg0->unk_dbmenusprite_0x4;
	sobj = SObjGetStruct(arg0->unk_dbmenusprite_0x4);
	arg0->sprite = &sobj->sprite;
	return thing;
}

void func_ovl8_80372420(dbMenuSprite *mnsprite, Sprite *sprite)
{
	mnsprite->unk_dbmenusprite_0x0 = mnsprite->unk_dbmenusprite_0x4 = NULL;
	mnsprite->sprite = sprite;
}

void* func_ovl8_80372430(dbUnknownLink *arg0, dbUnknownLinkStruct *arg1)
{
	if (arg0 != NULL || (arg0 = func_ovl8_803717A0(0x70)))
	{
		if (arg1 == NULL)
		{
			arg1 = &arg0->unk_dbunklink_0x14;
			func_ovl8_803717E0(&arg0->unk_dbunklink_0x14);
		}
		func_ovl8_80371FB0(arg0, arg1);
		arg0->unk_dbunklink_0xC = D_ovl8_80388CA0,
			arg1->db_func = D_ovl8_80388D08;
		func_ovl8_8037277C(arg0);
	}
	return arg0;
}

void* func_ovl8_803724B4(dbUnknownLink *arg0, dbUnknownLinkStruct *arg1, void *arg2, void *arg3, void *arg4)
{
	dbUnknownLinkStruct *temp_v1;

	if (arg0 != NULL || (arg0 = func_ovl8_803717A0(0x70)))
	{
		if (arg1 == NULL)
		{
			arg1 = &arg0->unk_dbunklink_0x14;
			func_ovl8_803717E0(&arg0->unk_dbunklink_0x14);
		}
		func_ovl8_8037203C(arg0, arg1, arg2, arg4);
		arg0->unk_dbunklink_0xC = D_ovl8_80388CA0;
		arg1->db_func = D_ovl8_80388D08;
		temp_v1 = (dbUnknownLinkStruct*)arg0->unk_dbunklink_0x10,
			temp_v1->unk_dbunkstruct_0x4C = arg3;
		func_ovl8_8037277C(arg0);
	}
	return arg0;
}

void func_ovl8_80372554(dbUnknownLink *arg0, u32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->unk_dbunklink_0xC = D_ovl8_80388CA0, // Needs to be joined with following statement or else scheduler mismatch
		((dbUnknownLinkStruct*)arg0->unk_dbunklink_0x10)->db_func = D_ovl8_80388D08;

		func_ovl8_803720D4(arg0, 0);

		if (arg1 != 0)
			func_ovl8_803718C4(arg0->unk_dbunklink_0x10, 0);

		if (arg1 & 1)
			func_ovl8_803717C0(arg0);
	}
}

// 0x803725DC
void func_ovl8_803725DC(dbTestMenu *arg0)
{
    dbUnknownLinkStruct *a3;
    dbUnknownLinkStruct *v1;
    dbUnknownLinkStruct *a2;
    DBMenuPosition sp3C;
    DBMenuPosition sp34;
    Sprite *sprite;
    dbFunction *v0;

    v1 = arg0->db_func;
    sprite = (Sprite*) ((dbFunction *)v1)[9].unk_dbfunc_0x4(((dbFunction *)v1)[9].unk_dbfunc_0x0 + (uintptr_t)arg0);
    if (a2);

    if (sprite != NULL) 
    {
        a3 = arg0->unk_dbtestmenu_0x10;
        if (a3);
        v1 = a3->unk_dbunkstruct_0x4C;
        a2 = v1->unk_dbunkstruct_0x20;
        a2->db_func[20].unk_dbfunc_0x4(a2->db_func[20].unk_dbfunc_0x0 + (uintptr_t)a2, &sp3C);
        if (1);
        
        v0 = arg0->unk_dbtestmenu_0x10;
        ((dbFunction *)((dbUnknownLinkStruct *)v0)->db_func)[20].unk_dbfunc_0x4(((dbFunction *)((dbUnknownLinkStruct *)v0)->db_func)[20].unk_dbfunc_0x0 + (uintptr_t)v0, &sp34);

        spMove(sprite, sp3C.x + sp34.x, sp3C.y + sp34.y);
        
        sprite->rsp_dl_next = gSYTaskmanDLHeads[0];
        
        spDraw(sprite);
        
        gSYTaskmanDLHeads[0] = sprite->rsp_dl_next - 1;
        
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
    }
}

void func_ovl8_803726CC(dbTestMenu *arg0)
{
	dbFunction *db_func;
	db4Shorts sp2C;
	db4Shorts sp24;
	dbUnknownLinkStruct *db_unk10;
	dbUnknownLinkStruct *db_unk4C;
	dbUnknownLinkStruct *db_unk20;

	db_unk10 = arg0->unk_dbtestmenu_0x10;

	db_unk4C = db_unk10->unk_dbunkstruct_0x4C;

	if (db_unk4C != NULL)
	{
		db_unk20 = db_unk4C->unk_dbunkstruct_0x20;

		db_func = db_unk20->db_func;

		db_func[20].unk_dbfunc_0x4(db_func[20].unk_dbfunc_0x0 + (uintptr_t)db_unk20, &sp2C);

		db_unk20 = arg0->unk_dbtestmenu_0x10;

		db_func = db_unk20->db_func;

		db_func[20].unk_dbfunc_0x4(db_func[20].unk_dbfunc_0x0 + (uintptr_t)db_unk20, &sp24);

		db_func = arg0->db_func;

		db_func[8].unk_dbfunc_0x4(db_func[8].unk_dbfunc_0x0 + (uintptr_t)arg0, (s16) (sp24.arr[0] + sp2C.arr[0]), (s16) (sp24.arr[1] + sp2C.arr[1]));
	}
}

void func_ovl8_8037277C(s32 arg0) {}