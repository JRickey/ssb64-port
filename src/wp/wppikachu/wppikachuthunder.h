#ifndef _WPPIKACHUTHUNDER_H_
#define _WPPIKACHUTHUNDER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern void wpPikachuThunderHeadSetDestroy(GObj *weapon_gobj, sb32 is_destroy);
extern void wpPikachuThunderHeadMakeTrailEffect(GObj *weapon_gobj, s32 arg1);
extern sb32 wpPikachuThunderHeadProcUpdate(GObj *weapon_gobj);
extern sb32 wpPikachuThunderHeadProcMap(GObj *weapon_gobj);
extern sb32 wpPikachuThunderHeadProcDead(GObj *weapon_gobj);
extern GObj* wpPikachuThunderHeadMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel);
extern sb32 wpPikachuThunderTrailProcUpdate(GObj *weapon_gobj);
extern sb32 wpPikachuThunderTrailProcHit(GObj *weapon_gobj);
extern GObj* wpPikachuThunderTrailMakeWeapon(GObj *weapon_gobj, Vec3f *pos);

#endif
