#ifndef _WPNESSPKTHUNDER_H_
#define _WPNESSPKTHUNDER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern void wpNessPKThunderHeadSetDestroyTrails(GObj *weapon_gobj, sb32 is_destroy);
extern void wpNessPKThunderTrailUpdatePositions(GObj *weapon_gobj);
extern void wpNessPKThunderHeadMakeTrail(GObj *weapon_gobj, s32 trail_id);
extern sb32 wpNessPKThunderHeadProcUpdate(GObj *weapon_gobj);
extern sb32 wpNessPKThunderHeadProcMap(GObj *weapon_gobj);
extern sb32 wpNessPKThunderHeadProcHit(GObj *weapon_gobj);
extern sb32 wpNessPKThunderHeadProcReflector(GObj *weapon_gobj);
extern sb32 wpNessPKThunderHeadProcDead(GObj *weapon_gobj);
extern GObj* wpNessPKThunderHeadMakeWeapon(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel);
extern sb32 wpNessPKThunderTrailProcUpdate(GObj *weapon_gobj);
extern sb32 wpNessPKThunderTrailProcHit(GObj *weapon_gobj);
extern GObj* wpNessPKThunderTrailMakeWeapon(GObj *head_gobj, Vec3f *pos, s32 trail_id);
extern void wpNessPKReflectHeadMakeTrail(GObj *weapon_gobj, s32 trail_id);
extern void wpNessPKReflectHeadSetDestroyTrails(GObj *weapon_gobj, s32 unused);
extern sb32 wpNessPKReflectHeadProcUpdate(GObj *weapon_gobj);
extern sb32 wpNessPKReflectHeadProcMap(GObj *weapon_gobj);
extern sb32 wpNessPKReflectHeadProcHit(GObj *weapon_gobj);
extern sb32 wpNessPKReflectHeadProcReflector(GObj *weapon_gobj);
extern sb32 wpNessPKReflectHeadProcDead(GObj *weapon_gobj);
extern GObj* wpNessPKReflectHeadMakeWeapon(GObj *old_gobj, Vec3f *pos, f32 angle);
extern sb32 wpNessPKReflectTrailProcUpdate(GObj *weapon_gobj);
extern sb32 wpNessPKReflectTrailProcHit(GObj *weapon_gobj);
extern GObj* wpNessPKReflectTrailMakeWeapon(GObj *old_gobj, Vec3f *pos, s32 trail_id);

#endif
