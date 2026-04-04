#include <sys/objman.h>
#include <sys/develop.h>
#include <db/debug.h>

extern dbFunction D_ovl8_80389C90;
extern dbFunction D_ovl8_80389D48;
extern dbFunction D_ovl8_80389EA0;
extern dbUnknownLinkStruct D_ovl8_80389EC8;

extern void func_ovl8_8037BE34();
extern void func_ovl8_8037BE94();

void func_ovl8_80376AA4(dbUnknown5* arg0, s16 arg1, s16 arg2);
void func_ovl8_80376B60(s32 arg0, u8* arg1, u32* arg2);

// 0x80376530
dbUnknown5* func_ovl8_80376530(dbUnknown5* arg0, dbUnknownLinkStruct* arg1, dbUnknownLink* arg2, DBMenu* arg3, s32 arg4, s32 arg5)
{
    s32 sp4C;
    DBMenuPosition sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;

	if ((arg0 != NULL) || (arg0 = func_ovl8_803717A0(0xB8), (arg0 != NULL)))
	{
		if (arg1 == NULL)
		{
			arg1 = &arg0->unk_dbunk5_0x4C;
			arg2 = &arg0->unk_dbunk5_0xAC;
            #line 29
			func_ovl8_803717E0(arg1);
			func_ovl8_8037C2D0(arg2);
            #line 36
		}
        
        func_ovl8_80372844(arg0, arg1, arg2, arg3);
        func_ovl8_8037203C(&arg0->unk_dbunk5_0x38, arg1, arg3, arg4);\
		arg0->db_func = &D_ovl8_80389C90;
		arg1->db_func = &D_ovl8_80389D48;
		arg2->unk_dbunklink_0x8 = &D_ovl8_80389EA0;
		arg0->unk_dbunk5_0x44.ls = &D_ovl8_80389EC8;

        if (arg3->id == 'BHWN' || arg3->id == 'BFHW') 
        {
            arg0->db_func[4].unk_dbfunc_0x4(arg0->db_func[4].unk_dbfunc_0x0 + (uintptr_t)arg0);
        }


        if (arg3->unk_dbmenu_0x10 == 1) 
        {
            sp4C = 2;
            sp40 = 0x40;
            sp3C = 0x20;
        } 
        else if (arg3->unk_dbmenu_0x10 == 4) 
        {
            sp4C = 3;
            sp40 = 0x20;
            sp3C = 0x20;
        } 
        else if (arg3->unk_dbmenu_0x10 == 6) 
        {
            sp4C = 1;
            sp40 = 0x40;
            sp3C = 0x20;
        }
        
        arg0->unk_dbunk5_0xA8 = 0;
        
        if (arg4 == 0) 
        {
            arg0->unk_dbunk5_0x40.gobj = func_ovl8_8037ACAC(arg3->position.w, arg3->position.h, sp4C, sp40, sp3C);;
            arg0->unk_dbunk5_0xA8 = arg0->unk_dbunk5_0x40.gobj;
            
            if (arg3->unk_dbmenu_0x10 == 6) 
            {
                func_ovl8_80376B60(2, &((dbUnknownLinkStruct*)arg0)->unk_dbunkstruct_0x20->text_color, &sp34);
                arg0->unk_dbunk5_0x40.ls->unk_dbunkstruct_0x20->unk_dbunkstruct_0x0 = sp34;
                func_ovl8_80376B60(2, &((dbUnknownLinkStruct*)arg0)->unk_dbunkstruct_0x20->bg_color, &sp34);
				arg0->unk_dbunk5_0x40.ls->unk_dbunkstruct_0x20[4].unk_dbunkstruct_0x5C.y = sp34; // golly that's weird
            } 
            else 
            {
                *(DBMenuPosition*)&sp44 = arg3->position;
                sp44.x = 0;
                sp44.y = 0;
                func_ovl8_80377AEC(arg0->unk_dbunk5_0x40.gobj, &sp44, &arg3->bg_color, 4);
            }
            func_ovl8_803723AC(&arg0->unk_dbunk5_0x38, arg0, arg0->unk_dbunk5_0x40.gobj, func_ovl8_8037BE34, func_ovl8_8037BE94, arg5);
            ((dbUnknownLinkStruct*)arg0)->unk_dbunkstruct_0x20->position.x = arg3->position.x;
            ((dbUnknownLinkStruct*)arg0)->unk_dbunkstruct_0x20->position.y = arg3->position.y;
            func_ovl8_80376AA4(arg0, arg3->position.x, arg3->position.y);
        } 
        else if (arg4 != -1) 
        {
            arg0->unk_dbunk5_0x40.gobj = arg4;
            func_ovl8_803723AC(&arg0->unk_dbunk5_0x38, arg0, arg4, func_ovl8_8037BE34, func_ovl8_8037BE94, arg5);
            ((dbUnknownLinkStruct*)arg0)->unk_dbunkstruct_0x20->position.x = arg3->position.x;
            ((dbUnknownLinkStruct*)arg0)->unk_dbunkstruct_0x20->position.y = arg3->position.y;
            func_ovl8_80376AA4(arg0, arg3->position.x, arg3->position.y);
        }
        
        
        if (arg3->position.x == -0x2710) 
        {
            func_ovl8_803772AC(arg0);
        }
    }
    
	return arg0;
}

// 0x80376848
s32 func_ovl8_80376848(s32 arg0, dbUnknown5* arg1)
{
	s32 var_v1;
	s32 var_v1_2;

	var_v1_2 = arg1->unk_dbunk5_0x14;
	var_v1_2 = ((var_v1_2 != 'BFHW') && (var_v1_2 != 'BFWN')) ? 0 : 1;
	var_v1 = func_ovl8_803717A0(0xB8);

	if (var_v1 != 0)
	{
		func_ovl8_80376530(var_v1, 0, 0, arg1, arg1->unk_dbunk5_0x24, var_v1_2);
		var_v1_2 = var_v1;
	}
	else
		var_v1_2 = 0;

	return var_v1_2;
}

// 0x803768E0
void func_ovl8_803768E0(dbUnknown7* arg0, s32 arg1)
{
	if (arg0 != NULL)
	{
		arg0->unk18 = &D_ovl8_80389C90,
		arg0->unk20->db_func = &D_ovl8_80389D48;
		arg0->unk1C->unk_dbunklink_0x8 = &D_ovl8_80389EA0;
		arg0->unk44 = &D_ovl8_80389EC8;

		if (arg0->unkA8 != 0)
			func_ovl8_8037B3E4(arg0->unkA8);

		func_ovl8_803720D4(&arg0->unk38, 0);
		func_ovl8_8037295C(arg0, 0);

		if (arg1 != 0)
		{
			func_ovl8_8037C30C(arg0->unk1C, 0);
			func_ovl8_803718C4(arg0->unk20, 0);
		}
		if (arg1 & 1)
		{
			func_ovl8_803717C0(arg0);
		}
	}
}

// 0x803769AC
s32 func_ovl8_803769AC(Vec3i* arg0)
{
	return arg0[4].z;
}

// 0x803769B4
void func_ovl8_803769B4(dbUnknownS38 *arg0, dbUnknownS38 *arg1)
{
	func_ovl8_80372A48(arg0, arg1);
}

// 0x803769D4
s32 func_ovl8_803769D4(dbUnknown5* arg0)
{
	dbFunction* temp_v0;

	temp_v0 = arg0->db_func;
	temp_v0[17].unk_dbfunc_0x4(temp_v0[17].unk_dbfunc_0x0 + (uintptr_t)arg0);

	return 1;
}

// 0x80376A04
void func_ovl8_80376A04(dbUnknown5* arg0)
{
	dbFunction* temp_v0;

	func_ovl8_8037217C(&arg0->unk_dbunk5_0x38);
	func_ovl8_80375354(arg0);

	temp_v0 = arg0->db_func;
	temp_v0[16].unk_dbfunc_0x4(temp_v0[16].unk_dbfunc_0x0 + (uintptr_t)arg0);
}

// 0x80376A48
void func_ovl8_80376A48(dbUnknownLinkStruct *arg0, s16 arg1, s16 arg2)
{
	dbUnknownLinkStruct *var_v0;
	dbUnknownLinkStruct *var_v1;

	var_v0 = arg0;
	if (arg0 != NULL)
		var_v0 = arg0->unk_dbunkstruct_0x20;

	var_v1 = arg0;
	if (arg0 != NULL)
		var_v1 = arg0->unk_dbunkstruct_0x20;

	spMove(arg0->unk_dbunkstruct_0x40, var_v0->position.x + arg1, var_v1->position.y + arg2);
}

// 0x80376AA4
void func_ovl8_80376AA4(dbUnknown5* arg0, s16 arg1, s16 arg2)
{
	spMove(arg0->unk_dbunk5_0x40.gobj, arg1, arg2);
}

// 0x80376ADC
void func_ovl8_80376ADC(dbUnknownS38* arg0)
{
	dbUnknown3* temp_v0;
	dbFunction* temp_v0_2;

	temp_v0 = func_ovl8_80373160(arg0);

	if (temp_v0 != NULL)
	{
		while (temp_v0 != NULL)
		{
			temp_v0_2 = temp_v0->unk_dbunk3_0x38->db_func;

			if (temp_v0_2[36].unk_dbfunc_0x4(temp_v0_2[36].unk_dbfunc_0x0 + (uintptr_t)temp_v0->unk_dbunk3_0x38) != 0)
			{
				temp_v0_2 = temp_v0->unk_dbunk3_0x38->db_func;
				temp_v0_2[11].unk_dbfunc_0x4(temp_v0_2[11].unk_dbfunc_0x0 + (uintptr_t)temp_v0->unk_dbunk3_0x38);
			}
			temp_v0 = temp_v0->unk_dbunk3_0x28;
		}
	}
}