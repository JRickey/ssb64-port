#ifndef _WPSAMUSCHARGESHOT_H_
#define _WPSAMUSCHARGESHOT_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern void wpSamusChargeShotLaunch(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcDead(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcUpdate(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcMap(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcMap(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcHit(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcHop(GObj *weapon_gobj);
extern sb32 wpSamusChargeShotProcReflector(GObj *weapon_gobj);
extern GObj* wpSamusChargeShotMakeWeapon(GObj *fighter_gobj, Vec3f *pos, s32 charge_level, sb32 is_release);

#endif
