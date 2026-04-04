#ifndef _ITPAKKUN_H_
#define _ITPAKKUN_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itPakkunWaitSetStatus(GObj *item_gobj);
extern void itPakkunAppearSetStatus(GObj *item_gobj);
extern void itPakkunDamagedSetStatus(GObj *item_gobj);
extern void itPakkunCommonSetWaitFighter(GObj *item_gobj);
extern sb32 itPakkunCommonCheckNoFighter(GObj *item_gobj);
extern sb32 itPakkunWaitProcUpdate(GObj *item_gobj);
extern void itPakkunWaitInitVars(GObj *item_gobj);
extern void itPakkunAppearUpdateDamageColl(GObj *item_gobj);
extern sb32 itPakkunAppearProcUpdate(GObj *item_gobj);
extern sb32 itPakkunAppearProcDamage(GObj *item_gobj);
extern sb32 itPakkunDamagedProcUpdate(GObj *item_gobj);
extern sb32 itPakkunDamagedProcDead(GObj *item_gobj);
extern GObj* itPakkunMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
