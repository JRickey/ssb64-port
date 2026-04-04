#ifndef _ITNBUMPER_H_
#define _ITNBUMPER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itNBumperFallProcUpdate(GObj *item_gobj);
extern sb32 itNBumperWaitProcMap(GObj *item_gobj);
extern sb32 itNBumperFallProcMap(GObj *item_gobj);
extern sb32 itNBumperThrownProcHit(GObj *item_gobj);
extern void itNBumperWaitSetStatus(GObj *item_gobj);
extern void itNBumperFallSetStatus(GObj *item_gobj);
extern void itNBumperHoldSetStatus(GObj *item_gobj);
extern sb32 itNBumperThrownProcUpdate(GObj *item_gobj);
extern sb32 itNBumperThrownProcMap(GObj *item_gobj);
extern sb32 itNBumperThrownProcShield(GObj *item_gobj);
extern sb32 itNBumperThrownProcReflector(GObj *item_gobj);
extern void itNBumperThrownSetStatus(GObj *item_gobj);
extern void itNBumperDroppedSetStatus(GObj *item_gobj);
extern void itNBumperAttachedSetModelPitch(GObj *item_gobj);
extern void itNBumperAttachedInitVars(GObj *item_gobj);
extern sb32 itNBumperAttachedProcHit(GObj *item_gobj);
extern sb32 itNBumperAttachedProcUpdate(GObj *item_gobj);
extern sb32 itNBumperAttachedProcMap(GObj *item_gobj);
extern sb32 itNBumperAttachedProcReflector(GObj *item_gobj);
extern void itNBumperAttachedSetStatus(GObj *item_gobj);
extern sb32 itNBumperHitAirProcUpdate(GObj *item_gobj);
extern void itNBumperHitAirSetStatus(GObj *item_gobj);
extern sb32 itNBumperGDisappearProcUpdate(GObj *item_gobj);
extern void itNBumperGDisappearSetStatus(GObj *item_gobj);
extern GObj* itNBumperMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
