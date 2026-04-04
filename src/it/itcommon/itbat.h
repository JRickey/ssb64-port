#ifndef _ITBAT_H_
#define _ITBAT_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itBatFallProcUpdate(GObj *item_gobj);
extern sb32 itBatWaitProcMap(GObj *item_gobj);
extern sb32 itBatFallProcMap(GObj *item_gobj);
extern void itBatWaitSetStatus(GObj *item_gobj);
extern void itBatFallSetStatus(GObj *item_gobj);
extern void itBatHoldSetStatus(GObj *item_gobj);
extern sb32 itBatThrownProcUpdate(GObj *item_gobj);
extern sb32 itBatThrownProcMap(GObj *item_gobj);
extern sb32 itBatThrownProcHit(GObj *item_gobj);
extern void itBatThrownSetStatus(GObj *item_gobj);
extern sb32 itBatDroppedProcMap(GObj *item_gobj);
extern void itBatDroppedSetStatus(GObj *item_gobj);
extern GObj* itBatMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
