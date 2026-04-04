#ifndef _WPYOSHIEGGTHROW_H_
#define _WPYOSHIEGGTHROW_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpYoshiEggThrowProcDead(GObj *weapon_gobj);
extern sb32 wpYoshiEggExplodeProcUpdate(GObj *weapon_gobj);
extern void wpYoshiEggHitInitVars(GObj *weapon_gobj);
extern void wpYoshiEggExpireInitVars(GObj *weapon_gobj);
extern void wpYoshiEggThrowInitVars(GObj *weapon_gobj);
extern sb32 wpYoshiEggThrowProcUpdate(GObj *weapon_gobj);
extern sb32 wpYoshiEggThrowProcMap(GObj *weapon_gobj);
extern sb32 wpYoshiEggThrowProcHit(GObj *weapon_gobj);
extern sb32 wpYoshiEggThrowProcHop(GObj *weapon_gobj);
extern sb32 wpYoshiEggThrowProcReflector(GObj *weapon_gobj);
extern void wpYoshiEggThrowProcDisplay(GObj *weapon_gobj);
extern GObj* wpYoshiEggThrowMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
