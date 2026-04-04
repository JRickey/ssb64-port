#ifndef _WPKIRBYCUTTER_H_
#define _WPKIRBYCUTTER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpKirbyCutterProcUpdate(GObj *weapon_gobj);
extern sb32 wpKirbyCutterProcMap(GObj *weapon_gobj);
extern sb32 wpKirbyCutterProcHit(GObj *weapon_gobj);
extern sb32 wpKirbyCutterProcShield(GObj *weapon_gobj);
extern sb32 wpKirbyCutterProcSetOff(GObj *weapon_gobj);
extern sb32 wpKirbyCutterProcReflector(GObj *weapon_gobj);
extern GObj* wpKirbyCutterMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
