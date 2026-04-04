#ifndef _WPNESSPKFIRE_H_
#define _WPNESSPKFIRE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpNessPKFireProcUpdate(GObj *weapon_gobj);
extern sb32 wpNessPKFireProcMap(GObj *weapon_gobj);
extern sb32 wpNessPKFireProcHit(GObj *weapon_gobj);
extern sb32 wpNessPKFireProcHop(GObj *weapon_gobj);
extern sb32 wpNessPKFireProcReflector(GObj *weapon_gobj);
extern sb32 wpNessPKFireProcAbsorb(GObj *weapon_gobj);
extern GObj* wpNessPKFireMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel, f32 angle);

#endif
