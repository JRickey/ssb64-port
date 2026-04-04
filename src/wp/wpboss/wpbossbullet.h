#ifndef _WPBOSSBULLET_H_
#define _WPBOSSBULLET_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern sb32 wpBossBulletExplodeProcUpdate(GObj *weapon_gobj);
extern void wpBossBulletExplodeInitVars(GObj *weapon_gobj);
extern sb32 wpBossBulletProcMap(GObj *weapon_gobj);
extern sb32 wpBossBulletProcHit(GObj *weapon_gobj);
extern sb32 wpBossBulletProcHop(GObj *weapon_gobj);
extern sb32 wpBossBulletProcReflector(GObj *weapon_gobj);
extern GObj* wpBossBulletNormalMakeWeapon(GObj *fighter_gobj, Vec3f *pos);
extern GObj* wpBossBulletHardMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
