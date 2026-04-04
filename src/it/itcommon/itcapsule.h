#ifndef _ITCAPSULE_H_
#define _ITCAPSULE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itCapsuleFallProcUpdate(GObj *item_gobj);
extern sb32 itCapsuleWaitProcMap(GObj *item_gobj);
extern sb32 itCapsuleCommonProcHit(GObj *item_gobj);
extern sb32 itCapsuleFallProcMap(GObj *item_gobj);
extern void itCapsuleWaitSetStatus(GObj *item_gobj);
extern void itCapsuleFallSetStatus(GObj *item_gobj);
extern void itCapsuleHoldSetStatus(GObj *item_gobj);
extern sb32 itCapsuleThrownProcUpdate(GObj *item_gobj);
extern sb32 itCapsuleThrownProcMap(GObj *item_gobj);
extern void itCapsuleThrownSetStatus(GObj *item_gobj);
extern sb32 func_ovl3_801741F0(GObj *item_gobj);
extern sb32 itCapsuleDroppedProcMap(GObj *item_gobj);
extern void itCapsuleDroppedSetStatus(GObj *item_gobj);
extern sb32 itCapsuleExplodeProcUpdate(GObj *item_gobj);
extern GObj* itCapsuleMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern void itCapsuleExplodeInitVars(GObj *item_gobj);
extern void itCapsuleExplodeSetStatus(GObj *item_gobj);
extern void itCapsuleExplodeMakeEffectGotoSetStatus(GObj *item_gobj);

#endif
