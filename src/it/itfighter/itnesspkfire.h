#ifndef _ITNESSPKFIRE_H_
#define _ITNESSPKFIRE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itNessPKFireCommonProcUpdate(GObj *item_gobj);
extern sb32 itNessPKFireCommonUpdateAllCheckDestroy(GObj *item_gobj);
extern sb32 itNessPKFireWaitProcUpdate(GObj *item_gobj);
extern sb32 itNessPKFireFallProcUpdate(GObj *item_gobj);
extern sb32 itNessPKFireWaitProcMap(GObj *item_gobj);
extern sb32 itNessPKFireFallProcMap(GObj *item_gobj);
extern sb32 itNessPKFireCommonProcDamage(GObj *item_gobj);
extern void itNessPKFireWaitSetStatus(GObj *item_gobj);
extern void itNessPKFireFallSetStatus(GObj *item_gobj);
extern GObj* itNessPKFireMakeItem(GObj *weapon_gobj, Vec3f *pos, Vec3f *vel);

#endif
