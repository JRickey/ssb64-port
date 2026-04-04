#ifndef _ITSWORD_H_
#define _ITSWORD_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itSwordFallProcUpdate(GObj *item_gobj);
extern sb32 itSwordWaitProcMap(GObj *item_gobj);
extern sb32 itSwordFallProcMap(GObj *item_gobj);
extern void itSwordWaitSetStatus(GObj *item_gobj);
extern void itSwordFallSetStatus(GObj *item_gobj);
extern void itSwordHoldSetStatus(GObj *item_gobj);
extern sb32 itSwordThrownProcMap(GObj *item_gobj);
extern sb32 itSwordThrownProcHit(GObj *item_gobj);
extern void itSwordThrownSetStatus(GObj *item_gobj);
extern sb32 itSwordDroppedProcMap(GObj *item_gobj);
extern void itSwordDroppedSetStatus(GObj *item_gobj);
extern GObj* itSwordMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
