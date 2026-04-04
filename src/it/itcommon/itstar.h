#ifndef _ITSTAR_H_
#define _ITSTAR_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itStarCommonProcUpdate(GObj *item_gobj);
extern sb32 itStarCommonProcMap(GObj *item_gobj);
extern sb32 itStarCommonProcHit(GObj *item_gobj);
extern GObj* itStarMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
