#ifndef _ITMARUMINE_H_
#define _ITMARUMINE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itMarumineExplodeMakeEffectGotoSetStatus(GObj *item_gobj);
extern void itMarumineExplodeUpdateAttackEvent(GObj *item_gobj);
extern sb32 itMarumineCommonProcUpdate(GObj *item_gobj);
extern sb32 itMarumineExplodeProcUpdate(GObj *item_gobj);
extern void itMarumineExplodeSetStatus(GObj *item_gobj);
extern GObj* itMarumineMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
