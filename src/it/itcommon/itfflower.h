#ifndef _ITFFLOWER_H_
#define _ITFFLOWER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itFFlowerFallProcUpdate(GObj *item_gobj);
extern sb32 itFFlowerWaitProcMap(GObj *item_gobj);
extern sb32 itFFlowerFallProcMap(GObj *item_gobj);
extern void itFFlowerWaitSetStatus(GObj *item_gobj);
extern void itFFlowerFallSetStatus(GObj *item_gobj);
extern void itFFlowerHoldSetStatus(GObj *item_gobj);
extern sb32 itFFlowerThrownProcMap(GObj *item_gobj);
extern sb32 itFFlowerCommonProcHit(GObj *item_gobj);
extern void itFFlowerThrownSetStatus(GObj *item_gobj);
extern sb32 itFFlowerDroppedProcMap(GObj *item_gobj);
extern void itFFlowerDroppedSetStatus(GObj *item_gobj);
extern GObj* itFFlowerMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itFFlowerWeaponFlameProcUpdate(GObj *weapon_gobj);
extern sb32 itFFlowerWeaponFlameProcMap(GObj *weapon_gobj);
extern sb32 itFFlowerWeaponFlameProcHit(GObj *weapon_gobj);
extern sb32 itFFlowerWeaponFlameProcReflector(GObj *weapon_gobj);
extern GObj* itFFlowerWeaponFlameMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel);
extern void itFFlowerShootFlame(GObj *fighter_gobj, Vec3f *pos, s32 index, s32 ammo_sub);

#endif
