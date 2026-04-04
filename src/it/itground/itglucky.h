#ifndef _ITGLUCKY_H_
#define _ITGLUCKY_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itGLuckyDamagedSetStatus(GObj *item_gobj);
extern void itGLuckyCommonUpdateEggSpawn(GObj *lucky_gobj);
extern sb32 itGLuckyCommonProcUpdate(GObj *item_gobj);
extern sb32 itGLuckyCommonProcHit(GObj *item_gobj);
extern sb32 itGLuckyDamagedProcUpdate(GObj *item_gobj);
extern sb32 itGLuckyDamagedProcDead(GObj *item_gobj);
extern sb32 itGLuckyCommonProcDamage(GObj *item_gobj);
extern GObj* itGLuckyMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
