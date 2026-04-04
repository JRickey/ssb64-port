#ifndef _GRTYPES_H_
#define _GRTYPES_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <mp/map.h>
#include <gm/generic.h>

#include <gr/grdef.h>
#include <gr/grvars.h>

struct GRDisplayDesc
{
	void *pri_proc_display;
	void *sec_proc_display;
	u8 dl_link;
	void *proc_update;
};

struct GRAttackColl
{
	s32 kind;
	s32 damage;
	s32 angle;
	s32 knockback_scale;
	s32 knockback_weight;
	s32 knockback_base;
	s32 element;
};

struct GRObstacle
{
	GObj *gobj;
	sb32 (*proc_update)(GObj*, GObj*, s32*);
};

struct GRHazard
{
	GObj *gobj;
	sb32 (*proc_update)(GObj*, GObj*, GRAttackColl**, s32*);
};

struct GRFileInfo
{
	intptr_t file_id, offset;
};

union GRStruct
{
	// Common stages
	GRCommonGroundVarsPupupu pupupu;
	GRCommonGroundVarsSector sector;
	GRCommonGroundVarsZebes zebes;
	GRCommonGroundVarsYoster yoster;
	GRCommonGroundVarsInishie inishie;
	GRCommonGroundVarsJungle jungle;
	GRCommonGroundVarsHyrule hyrule;
	GRCommonGroundVarsYamabuki yamabuki;
	GRCommonGroundVarsCastle castle;

	// Bonus stages
	GRBonusGroundVarsBonus1 bonus1;
	GRBonusGroundVarsBonus2 bonus2;
	GRBonusGroundVarsBonus3 bonus3;
};

#endif
