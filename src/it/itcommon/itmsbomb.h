#ifndef _ITMSBOMB_H_
#define _ITMSBOMB_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itMSBombFallProcUpdate(GObj *item_gobj);
extern sb32 itMSBombWaitProcMap(GObj *item_gobj);
extern sb32 itMSBombFallProcMap(GObj *item_gobj);
extern void itMSBombWaitSetStatus(GObj *item_gobj);
extern void itMSBombFallSetStatus(GObj *item_gobj);
extern void itMSBombHoldSetStatus(GObj *item_gobj);
extern sb32 itMSBombThrownProcUpdate(GObj *item_gobj);
extern sb32 itMSBombThrownProcMap(GObj *item_gobj);
extern sb32 itMSBombCommonProcHit(GObj *item_gobj);
extern void itMSBombThrownSetStatus(GObj *item_gobj);
extern sb32 itMSBombDroppedProcMap(GObj *item_gobj);
extern void itMSBombDroppedSetStatus(GObj *item_gobj);
extern void itMSBombAttachedUpdateSurface(GObj *item_gobj);
extern void itMSBombAttachedInitVars(GObj *item_gobj);
extern void itMSBombExplodeMakeEffect(GObj *item_gobj);
extern void itMSBombExplodeInitStatusVars(GObj *item_gobj, sb32 is_make_effect);
extern sb32 itMSBombCommonProcDamage(GObj *item_gobj);
extern sb32 itMSBombAttachedProcUpdate(GObj *item_gobj);
extern void itMSBombAttachedSetStatus(GObj *item_gobj);
extern sb32 itMSBombAttachedProcMap(GObj *item_gobj);
extern void itMSBombExplodeUpdateAttackEvent(GObj *item_gobj);
extern void itMSBombDetachedInitVars(GObj *item_gobj);
extern sb32 itMSBombDetachedProcUpdate(GObj *item_gobj);
extern void itMSBombDetachedSetStatus(GObj *item_gobj);
extern void itMSBombExplodeInitVars(GObj *item_gobj);
extern sb32 itMSBombExplodeProcUpdate(GObj *item_gobj);
extern void itMSBombExplodeSetStatus(GObj *item_gobj);
extern GObj* itMSBombMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
