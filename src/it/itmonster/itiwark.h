#ifndef _ITIWARK_H_
#define _ITIWARK_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itIwarkAttackUpdateRock(GObj *iwark_gobj);
extern sb32 itIwarkAttackProcUpdate(GObj *item_gobj);
extern void itIwarkAttackInitVars(GObj *item_gobj);
extern void itIwarkAttackSetStatus(GObj *item_gobj);
extern sb32 itIwarkFlyProcUpdate(GObj *item_gobj);
extern void itIwarkFlySetStatus(GObj *item_gobj);
extern sb32 itIwarkCommonProcUpdate(GObj *item_gobj);
extern sb32 itIwarkCommonProcMap(GObj *item_gobj);
extern GObj* itIwarkMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itIwarkWeaponRockProcDead(GObj *weapon_gobj);
extern sb32 itIwarkWeaponRockProcUpdate(GObj *weapon_gobj);
extern sb32 itIwarkWeaponRockProcMap(GObj *weapon_gobj);
extern sb32 itIwarkWeaponRockProcHop(GObj *weapon_gobj);
extern sb32 itIwarkWeaponRockProcReflector(GObj *weapon_gobj);
extern GObj* itIwarkWeaponRockMakeWeapon(GObj *parent_gobj, Vec3f *pos, u8 random);

#endif
