#ifndef _ITTARUBOMB_H_
#define _ITTARUBOMB_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itTaruBombContainerSmashUpdateEffect(GObj *effect_gobj);
extern void itTaruBombContainerSmashMakeEffect(Vec3f *pos);
extern sb32 itTaruBombFallProcUpdate(GObj *item_gobj);
extern sb32 itTaruBombCommonProcHit(GObj *item_gobj);
extern sb32 itTaruBombCommonProcDamage(GObj *item_gobj);
extern void itTaruBombRollSetStatus(GObj *item_gobj);
extern sb32 itTaruBombFallCheckCollideGround(GObj *item_gobj, f32 common_rebound);
extern sb32 itTaruBombFallProcMap(GObj *item_gobj);
extern void itTaruBombCommonSetMapCollisionBox(GObj *item_gobj);
extern sb32 itTaruBombExplodeProcUpdate(GObj *item_gobj);
extern sb32 itTaruBombRollProcUpdate(GObj *item_gobj);
extern sb32 itTaruBombRollProcMap(GObj *item_gobj);
extern GObj* itTaruBombMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern void itTaruBombExplodeInitVars(GObj *item_gobj);
extern void itTaruBombExplodeSetStatus(GObj *item_gobj);
extern void itTaruBombExplodeMakeEffectGotoSetStatus(GObj *item_gobj);

#endif
