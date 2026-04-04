#ifndef _WPPIKACHUTHUNDERJOLT_H_
#define _WPPIKACHUTHUNDERJOLT_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpPikachuThunderJoltAirProcUpdate(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltAirProcMap(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltAirProcHit(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltAirProcHop(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltAirProcReflector(GObj *weapon_gobj);
extern GObj* wpPikachuThunderJoltAirMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel);
extern void wpPikachuThunderJoltGroundAddAnim(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltGroundProcUpdate(GObj *weapon_gobj);
extern s32 wpPikachuThunderJoltGroundGetStatus(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltGroundCheckDestroy(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltGroundProcMap(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltGroundProcHit(GObj *weapon_gobj);
extern sb32 wpPikachuThunderJoltGroundProcReflector(GObj *weapon_gobj);
extern GObj* wpPikachuThunderJoltGroundMakeWeapon(GObj *prev_gobj, Vec3f *pos, s32 coll_type);

#endif
