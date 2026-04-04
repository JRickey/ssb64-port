#ifndef _ITTARU_H_
#define _ITTARU_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itTaruFallProcUpdate(GObj *item_gobj);
extern sb32 itTaruWaitProcMap(GObj *item_gobj);
extern sb32 itTaruCommonProcHit(GObj *item_gobj);
extern sb32 itTaruCommonProcDamage(GObj *item_gobj);
extern sb32 itTaruFallProcMap(GObj *item_gobj);
extern void itTaruWaitSetStatus(GObj *item_gobj);
extern void itTaruFallSetStatus(GObj *item_gobj);
extern void itTaruHoldSetStatus(GObj *item_gobj);
extern sb32 itTaruThrownCheckMapCollision(GObj *item_gobj, f32 common_rebound);
extern void itTaruRollSetStatus(GObj *item_gobj);
extern sb32 itTaruThrownProcMap(GObj *item_gobj);
extern void itTaruThrownInitVars(GObj *item_gobj);
extern void itTaruThrownSetStatus(GObj *item_gobj);
extern sb32 func_ovl3_80179F50(GObj *item_gobj);
extern void itTaruDroppedSetStatus(GObj *item_gobj);
extern sb32 itTaruExplodeProcUpdate(GObj *item_gobj);
extern sb32 itTaruRollProcUpdate(GObj *item_gobj);
extern sb32 itTaruRollProcMap(GObj *item_gobj);
extern GObj* itTaruMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern void itTaruExplodeInitVars(GObj *item_gobj);
extern void itTaruExplodeSetStatus(GObj *item_gobj);
extern void itTaruExplodeMakeEffectGotoSetStatus(GObj *item_gobj);

#endif
