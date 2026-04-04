#ifndef _ITHAMMER_H_
#define _ITHAMMER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itHammerCommonSetColAnim(GObj *item_gobj);
extern sb32 itHammerFallProcUpdate(GObj *item_gobj);
extern sb32 itHammerWaitProcMap(GObj *item_gobj);
extern sb32 itHammerFallProcMap(GObj *item_gobj);
extern void itHammerWaitSetStatus(GObj *item_gobj);
extern void itHammerFallSetStatus(GObj *item_gobj);
extern void itHammerHoldSetStatus(GObj *item_gobj);
extern sb32 itHammerThrownProcUpdate(GObj *item_gobj);
extern sb32 itHammerThrownProcMap(GObj *item_gobj);
extern sb32 itHammerCommonProcHit(GObj *item_gobj);
extern void itHammerThrownSetStatus(GObj *item_gobj);
extern sb32 itHammerDroppedProcMap(GObj *item_gobj);
extern void itHammerDroppedSetStatus(GObj *item_gobj);
extern GObj* itHammerMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
