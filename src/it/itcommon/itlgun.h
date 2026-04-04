#ifndef _ITLGUN_H_
#define _ITLGUN_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itLGunFallProcUpdate(GObj *item_gobj);
extern sb32 itLGunWaitProcMap(GObj *item_gobj);
extern sb32 itLGunFallProcMap(GObj *item_gobj);
extern void itLGunWaitSetStatus(GObj *item_gobj);
extern void itLGunFallSetStatus(GObj *item_gobj);
extern void itLGunHoldSetStatus(GObj *item_gobj);
extern sb32 itLGunThrownProcMap(GObj *item_gobj);
extern sb32 itLGunCommonProcHit(GObj *item_gobj);
extern void itLGunThrownSetStatus(GObj *item_gobj);
extern sb32 itLGunDroppedProcMap(GObj *item_gobj);
extern void itLGunDroppedSetStatus(GObj *item_gobj);
extern GObj* itLGunMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itLGunWeaponAmmoProcUpdate(GObj *weapon_gobj);
extern sb32 itLGunWeaponAmmoProcMap(GObj *weapon_gobj);
extern sb32 itLGunWeaponAmmoProcHit(GObj *weapon_gobj);
extern sb32 itLGunWeaponAmmoProcHop(GObj *weapon_gobj);
extern sb32 itLGunWeaponAmmoProcReflector(GObj *weapon_gobj);
extern GObj* itLGunWeaponAmmoMakeWeapon(GObj *fighter_gobj, Vec3f *pos);
extern void itLGunMakeAmmo(GObj *fighter_gobj, Vec3f *pos);

#endif
