#ifndef _ITHARISEN_H_
#define _ITHARISEN_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itHarisenCommonSetScale(GObj *item_gobj, f32 scale);
extern sb32 itHarisenFallProcUpdate(GObj *item_gobj);
extern sb32 itHarisenWaitProcMap(GObj *item_gobj);
extern sb32 itHarisenFallProcMap(GObj *item_gobj);
extern void itHarisenWaitSetStatus(GObj *item_gobj);
extern void itHarisenFallSetStatus(GObj *item_gobj);
extern void itHarisenHoldSetStatus(GObj *item_gobj);
extern sb32 itHarisenThrownProcUpdate(GObj *item_gobj);
extern sb32 itHarisenThrownProcMap(GObj *item_gobj);
extern sb32 itHarisenCommonProcHit(GObj *item_gobj);
extern void itHarisenThrownSetStatus(GObj *item_gobj);
extern sb32 itHarisenDroppedProcMap(GObj *item_gobj);
extern void itHarisenDroppedSetStatus(GObj *item_gobj);
extern void func_ovl3_80175408(GObj *item_gobj, s32 index);
extern GObj* itHarisenMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
