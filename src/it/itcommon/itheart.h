#ifndef _ITHEART_H_
#define _ITHEART_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itHeartFallProcUpdate(GObj *item_gobj);
extern sb32 itHeartWaitProcMap(GObj *item_gobj);
extern sb32 itHeartFallProcMap(GObj *item_gobj);
extern void itHeartWaitSetStatus(GObj *item_gobj);
extern void itHeartFallSetStatus(GObj *item_gobj);
extern sb32 itHeartDroppedProcMap(GObj *item_gobj);
extern void itHeartDroppedSetStatus(GObj *item_gobj);
extern GObj* itHeartMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
