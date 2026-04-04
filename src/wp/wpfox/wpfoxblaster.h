#ifndef _WPFOXBLASTER_H_
#define _WPFOXBLASTER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpFoxBlasterProcUpdate(GObj *weapon_gobj);
extern sb32 wpFoxBlasterProcMap(GObj *weapon_gobj);
extern sb32 wpFoxBlasterProcHit(GObj *weapon_gobj);
extern sb32 wpFoxBlasterProcHop(GObj *weapon_gobj);
extern sb32 wpFoxBlasterProcReflector(GObj *weapon_gobj);
extern GObj* wpFoxBlasterMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
