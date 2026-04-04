#ifndef _WPLINKBOOMERANG_H_
#define _WPLINKBOOMERANG_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <wp/wpdef.h>
#include <ft/ftdef.h>

extern void wpLinkBoomerangClampAngle360(f32 *angle);
extern sb32 wpLinkBoomerangCheckOffCamera(GObj *weapon_gobj);
extern void wpLinkBoomerangSetReturnVars(GObj *weapon_gobj, sb32 angle_max_or_min);
extern f32 wpLinkBoomerangAddVelSqrt(WPStruct *wp, f32 vel_add);
extern f32 wpLinkBoomerangSubVelSqrt(WPStruct *wp, f32 vel_sub);
extern void wpLinkBoomerangUpdateVelLR(WPStruct *wp, f32 vel_mul);
extern void wpLinkBoomerangClampAngleForward(f32 *angle);
extern f32 wpLinkBoomerangGetDistUpdateAngle(GObj *weapon_gobj);
extern void wpLinkBoomerangClearGObjs(WPStruct *wp);
extern void wpLinkBoomerangCheckOwnerCatch(GObj *weapon_gobj, f32 distance);
extern sb32 wpLinkBoomerangCheckBound(WPStruct *wp, Vec3f *coll_angle);
extern sb32 wpLinkBoomerangProcDead(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcUpdate(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcMap(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcHit(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcSetOff(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcShield(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcHop(GObj *weapon_gobj);
extern sb32 wpLinkBoomerangProcReflector(GObj *weapon_gobj);
extern f32 wpLinkBoomerangGetAngleSetVel(Vec3f *vel, FTStruct *fp, s32 lr, f32 vel_mul);
extern GObj* wpLinkBoomerangMakeWeapon(GObj *fighter_gobj, Vec3f *pos);

#endif
