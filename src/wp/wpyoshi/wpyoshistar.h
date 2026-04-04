#ifndef _WPYOSHISTAR_H_
#define _WPYOSHISTAR_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>

extern f32 wpYoshiStarGetScale(WPStruct *wp);
extern sb32 wpYoshiStarProcUpdate(GObj *weapon_gobj);
extern sb32 wpYoshiStarProcMap(GObj *weapon_gobj);
extern sb32 wpYoshiStarProcHit(GObj *weapon_gobj);
extern sb32 wpYoshiStarProcShield(GObj *weapon_gobj);
extern sb32 wpYoshiStarProcHop(GObj *weapon_gobj);
extern sb32 wpYoshiStarProcReflector(GObj *weapon_gobj);
extern GObj* wpYoshiStarMakeWeapon(GObj *fighter_gobj, Vec3f *pos, s32 lr);
extern GObj* wpYoshiStarMakeStars(GObj *fighter_gobj, Vec3f *pos);

#endif
