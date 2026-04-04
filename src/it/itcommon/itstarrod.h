#ifndef _ITSTARROD_H_
#define _ITSTARROD_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itStarRodFallProcUpdate(GObj *item_gobj);
extern sb32 itStarRodWaitProcMap(GObj *item_gobj);
extern sb32 itStarRodFallProcMap(GObj *item_gobj);
extern void itStarRodWaitSetStatus(GObj *item_gobj);
extern void itStarRodFallSetStatus(GObj *item_gobj);
extern void itStarRodHoldSetStatus(GObj *item_gobj);
extern sb32 itStarRodThrownProcUpdate(GObj *item_gobj);
extern sb32 itStarRodThrownProcMap(GObj *item_gobj);
extern sb32 itStarRodThrownProcHit(GObj *item_gobj);
extern void itStarRodThrownSetStatus(GObj *item_gobj);
extern sb32 itStarRodDroppedProcMap(GObj *item_gobj);
extern void itStarRodDroppedSetStatus(GObj *item_gobj);
extern GObj* itStarRodMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itStarRodWeaponStarProcUpdate(GObj *weapon_gobj);
extern sb32 itStarRodWeaponStarProcMap(GObj *weapon_gobj);
extern sb32 itStarRodWeaponStarProcHit(GObj *weapon_gobj);
extern sb32 itStarRodWeaponStarProcHop(GObj *weapon_gobj);
extern sb32 itStarRodWeaponStarProcReflector(GObj *weapon_gobj);
extern GObj* itStarRodWeaponStarMakeWeapon(GObj *fighter_gobj, Vec3f *pos, ub8 is_smash);
extern void itStarRodMakeStar(GObj *fighter_gobj, Vec3f *pos, ub8 is_smash);

#endif
