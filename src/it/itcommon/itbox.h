#ifndef _ITBOX_H_
#define _ITBOX_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itBoxContainerSmashUpdateEffect(GObj *effect_gobj);
extern void itBoxContainerSmashMakeEffect(Vec3f *pos);
extern sb32 itBoxCommonCheckSpawnItems(GObj *item_gobj);
extern sb32 itBoxFallProcUpdate(GObj *item_gobj);
extern sb32 itBoxWaitProcMap(GObj *item_gobj);
extern sb32 itBoxCommonProcHit(GObj *item_gobj);
extern sb32 itBoxCommonProcDamage(GObj *item_gobj);
extern sb32 itBoxFallProcMap(GObj *item_gobj);
extern void itBoxWaitSetStatus(GObj *item_gobj);
extern void itBoxFallSetStatus(GObj *item_gobj);
extern void itBoxHoldSetStatus(GObj *item_gobj);
extern sb32 itBoxThrownProcMap(GObj *item_gobj);
extern void itBoxThrownSetStatus(GObj *item_gobj);
extern sb32 func_ovl3_801798B8(GObj *item_gobj);
extern sb32 itBoxDroppedProcMap(GObj *item_gobj);
extern void itBoxDroppedSetStatus(GObj *item_gobj);
extern sb32 itBoxExplodeProcUpdate(GObj *item_gobj);
extern GObj *itBoxMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern void itBoxExplodeInitVars(GObj *item_gobj);
extern void itBoxExplodeSetStatus(GObj *item_gobj);
extern void itBoxExplodeMakeEffectGotoSetStatus(GObj *item_gobj);

#endif
