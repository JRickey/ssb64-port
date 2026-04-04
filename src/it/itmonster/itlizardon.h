#ifndef _ITLIZARDON_H_
#define _ITLIZARDON_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itLizardonFallUnusedProcUpdate(GObj *item_gobj);
extern sb32 itLizardonFallUnusedProcMap(GObj *item_gobj);
extern sb32 itLizardonFallUnusedSetStatus(GObj *item_gobj);
extern sb32 itLizardonFallProcUpdate(GObj *item_gobj);
extern sb32 itLizardonFallProcMap(GObj *item_gobj);
extern void itLizardonFallSetStatus(GObj *item_gobj);
extern sb32 itLizardonAttackProcUpdate(GObj *item_gobj);
extern sb32 itLizardonAttackProcMap(GObj *item_gobj);
extern void itLizardonAttackInitVars(GObj *item_gobj);
extern void itLizardonAttackSetStatus(GObj *item_gobj);
extern sb32 itLizardonCommonProcUpdate(GObj *item_gobj);
extern sb32 itLizardonCommonProcMap(GObj *item_gobj);
extern GObj* itLizardonMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itLizardonWeaponFlameProcUpdate(GObj *weapon_gobj);
extern sb32 itLizardonWeaponFlameProcMap(GObj *weapon_gobj);
extern sb32 itLizardonWeaponFlameProcHit(GObj *weapon_gobj);
extern sb32 itLizardonWeaponFlameProcReflector(GObj *weapon_gobj);
extern GObj* itLizardonWeaponFlameMakeWeapon(GObj *item_gobj, Vec3f *pos, Vec3f *vel);
extern void itLizardonAttackMakeFlame(GObj *item_gobj, Vec3f *pos, s32 lr);

#endif
