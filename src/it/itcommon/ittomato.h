#ifndef _ITTOMATO_H_
#define _ITTOMATO_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itTomatoFallProcUpdate(GObj *item_gobj);
extern sb32 itTomatoWaitProcMap(GObj *item_gobj);
extern sb32 itTomatoFallProcMap(GObj *item_gobj);
extern void itTomatoWaitSetStatus(GObj *item_gobj);
extern void itTomatoFallSetStatus(GObj *item_gobj);
extern sb32 itTomatoDroppedProcMap(GObj *item_gobj);
extern void itTomatoDroppedSetStatus(GObj *item_gobj);
extern GObj* itTomatoMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
