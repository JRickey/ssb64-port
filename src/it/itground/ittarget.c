#include <it/item.h>
#include <sc/scene.h>

// 0x8018F130
ITDesc dITTargetItemDesc =
{
	nITKindTarget,						// Item Kind
	&gSC1PBonusStageItemFile, 		// Pointer to item file data?
	0,									// Offset of item attributes in file?

	// DObj transformation struct
	{
		nGCMatrixKindTraRotRpyRSca, 	// Main matrix transformations
		nGCMatrixKindNull,		   		// Secondary matrix transformations?
		0							   	// ???
	},

	nGMAttackStateOff, 					// Hitbox Update State
	NULL,								// Proc Update
	NULL,								// Proc Map
	NULL,								// Proc Hit
	NULL,								// Proc Shield
	NULL,								// Proc Hop
	NULL,								// Proc Set-Off
	NULL,								// Proc Reflector
	itTargetCommonProcDamage			// Proc Damage
};

// 0x8018EE10
sb32 itTargetCommonProcDamage(GObj* item_gobj)
{
	efManagerShieldBreakMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);
	efManagerFireGrindMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);

	func_800269C0_275C0(nSYAudioFGMBonus1TargetBreak);

	sc1PBonusStageUpdateTargetCount();

	return TRUE;
}

// 0x8018EE5C
GObj* itTargetMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
	GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITTargetItemDesc, pos, vel, flags);

	if (item_gobj != NULL)
	{
		ITStruct *ip = itGetStruct(item_gobj);

		ip->ga = nMPKineticsGround;
		ip->coll_data.floor_line_id = -1;
	}
	return item_gobj;
}
