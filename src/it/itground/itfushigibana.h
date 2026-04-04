#ifndef _ITFUSHIGIBANA_H_
#define _ITFUSHIGIBANA_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itFushigibanaCommonUpdateMonsterEvent(GObj *item_gobj);
extern sb32 itFushigibanaCommonProcUpdate(GObj *item_gobj);
extern GObj* itFushigibanaMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itFushigibanaWeaponRazorProcUpdate(GObj *weapon_gobj);
extern sb32 itFushigibanaWeaponRazorProcHit(GObj *weapon_gobj);
extern sb32 itFushigibanaWeaponRazorProcHop(GObj *weapon_gobj);
extern sb32 itFushigibanaWeaponRazorProcReflector(GObj *weapon_gobj);
extern GObj *itFushigibanaWeaponRazorMakeWeapon(GObj *item_gobj, Vec3f *pos);

#endif
