#ifndef _ITDOGAS_H_
#define _ITDOGAS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itDogasDisappearProcUpdate(GObj *item_gobj);
extern void itDogasDisappearSetStatus(GObj *item_gobj);
extern void itDogasAttackUpdateSmog(GObj *item_gobj);
extern sb32 itDogasAttackProcUpdate(GObj *item_gobj);
extern void itDogasAttackInitVars(GObj *item_gobj);
extern void itDogasAttackSetStatus(GObj *item_gobj);
extern sb32 itDogasCommonProcUpdate(GObj *item_gobj);
extern sb32 itDogasCommonProcMap(GObj *item_gobj);
extern GObj* itDogasMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itDogasWeaponSmogProcUpdate(GObj *weapon_gobj);
extern GObj* itDogasWeaponSmogMakeWeapon(GObj *item_gobj, Vec3f *pos, Vec3f *vel);

#endif
