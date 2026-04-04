#ifndef _ITEGG_H_
#define _ITEGG_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itEggFallProcUpdate(GObj *item_gobj);
extern sb32 itEggWaitProcMap(GObj *item_gobj);
extern sb32 itEggCommonProcHit(GObj *item_gobj);
extern sb32 itEggFallProcMap(GObj *item_gobj);
extern void itEggWaitSetModelVars(GObj *item_gobj);
extern void itEggWaitSetStatus(GObj *item_gobj);
extern void itEggFallSetStatus(GObj *item_gobj);
extern void itEggHoldSetStatus(GObj *item_gobj);
extern sb32 itEggThrownProcUpdate(GObj *item_gobj);
extern sb32 itEggThrownProcMap(GObj *item_gobj);
extern void itEggThrownSetStatus(GObj *item_gobj);
extern sb32 func_ovl3_80181894(GObj *item_gobj);
extern sb32 itEggDroppedProcMap(GObj *item_gobj);
extern void itEggDroppedSetStatus(GObj *item_gobj);
extern sb32 itEggExplodeProcUpdate(GObj *item_gobj);
extern GObj* itEggMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern void itEggExplodeInitVars(GObj *item_gobj);
extern void itEggExplodeSetStatus(GObj *item_gobj);
extern void itEggExplodeMakeEffectGotoSetStatus(GObj *item_gobj);

#endif
