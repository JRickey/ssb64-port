#ifndef _ITKAMEX_H_
#define _ITKAMEX_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itKamexAttackUpdateHydro(GObj *item_gobj);
extern sb32 itKamexFallProcUpdate(GObj *item_gobj);
extern sb32 itKamexFallProcMap(GObj *item_gobj);
extern void itKamexFallInitVars(GObj *item_gobj);
extern void itKamexFallSetStatus(GObj *item_gobj);
extern sb32 itKamexAppearProcUpdate(GObj *item_gobj);
extern sb32 itKamexAppearProcMap(GObj *item_gobj);
extern void itKamexAppearSetStatus(GObj *item_gobj);
extern sb32 itKamexAttackProcUpdate(GObj *item_gobj);
extern sb32 itKamexAttackProcMap(GObj *item_gobj);
extern void itKamexAttackInitVars(GObj *item_gobj, sb32 is_ignore_setup);
extern void itKamexAttackSetStatus(GObj *item_gobj);
extern sb32 itKamexCommonProcUpdate(GObj *item_gobj);
extern sb32 itKamexCommonProcMap(GObj *item_gobj);
extern void itKamexCommonFindTargetsSetLR(GObj *item_gobj);
extern GObj* itKamexMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itKamexWeaponHydroProcUpdate(GObj *weapon_gobj);
extern sb32 itKamexWeaponHydroProcHit(GObj *weapon_gobj);
extern sb32 itKamexWeaponHydroProcReflector(GObj *weapon_gobj);
extern GObj* itKamexWeaponHydroMakeWeapon(GObj *item_gobj, Vec3f *pos);
extern void itKamexAttackMakeHydro(GObj *item_gobj, Vec3f *pos);

#endif
