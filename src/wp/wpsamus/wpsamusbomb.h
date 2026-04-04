#ifndef _WPSAMUSBOMB_H_
#define _WPSAMUSBOMB_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpSamusBombExplodeProcUpdate(GObj *weapon_gobj);
extern void wpSamusBombExplodeInitVars(GObj *weapon_gobj);
extern sb32 wpSamusBombProcUpdate(GObj *weapon_gobj);
extern sb32 wpSamusBombProcMap(GObj *weapon_gobj);
extern sb32 wpSamusBombProcHit(GObj *weapon_gobj);
extern sb32 wpSamusBombProcAbsorb(GObj *weapon_gobj);
extern sb32 wpSamusBombProcHop(GObj *weapon_gobj);
extern sb32 wpSamusBombProcReflector(GObj *weapon_gobj);
extern GObj* wpSamusBombMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
