#ifndef _ITSPEAR_H_
#define _ITSPEAR_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itSpearFlyCallSwarmMember(GObj *item_gobj);
extern sb32 itSpearAppearProcUpdate(GObj *item_gobj);
extern void itSpearAppearInitVars(GObj *item_gobj);
extern void itSpearAppearSetStatus(GObj *item_gobj);
extern sb32 itSpearFlyProcUpdate(GObj *item_gobj);
extern void itSpearFlyInitVars(GObj *item_gobj);
extern void itSpearFlySetStatus(GObj *item_gobj);
extern sb32 itSpearCommonProcUpdate(GObj *item_gobj);
extern sb32 itSpearCommonProcMap(GObj *item_gobj);
extern GObj* itSpearMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itSpearWeaponSwarmProcUpdate(GObj *weapon_gobj);
extern void itPippiWeaponSwarmRenderSwarm(GObj *item_gobj);
extern void itPippiWeaponSwarmProcDisplay(GObj *item_gobj);
extern GObj* itSpearWeaponSwarmMakeWeapon(GObj *item_gobj, Vec3f *pos, s32 kind);
extern void itSpearFlyMakeSwarm(GObj *item_gobj, Vec3f *pos, s32 kind);

#endif
