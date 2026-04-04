#ifndef _GRSECTOR_H_
#define _GRSECTOR_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern void func_ovl2_80106730(DObj *arg0, Vec3f *vec1, Vec3f *vec2, Vec3f *vec3);
extern sb32 grSectorArwingLaser3DFuncMatrix(Mtx *mtx, DObj *dobj, Gfx **dls);
extern void grSectorArwingAddAnim(DObj *dobj, AObjEvent32 *anim_joint, f32 unused);
extern void grSectorArwingUpdateSleep(void);
extern void grSectorArwingUpdateWait(void);
extern void func_ovl2_80106C88(void);
extern void func_ovl2_80106CC4(void);
extern void func_ovl2_80106D00(void);
extern void func_ovl2_80106DD8(void);
extern s32 grSectorArwingPrepareLaserCount(void);
extern sb32 grSectorArwingWeaponLaser2DProcMap(GObj *weapon_gobj);
extern sb32 grSectorArwingWeaponLaser2DProcHit(GObj *weapon_gobj);
extern void func_ovl2_801070A4(Vec3f *rotate, Vec3f *direction, Vec3f *vec3, Vec3f *vec4);
extern void func_ovl2_8010719C(Vec3f *vel, Vec3f *rotate);
extern sb32 grSectorArwingWeaponLaser2DProcHop(GObj *weapon_gobj);
extern sb32 grSectorArwingWeaponLaser2DProcReflector(GObj *weapon_gobj);
extern void grSectorArwingWeaponLaser2DMakeWeapon(void);
extern sb32 grSectorArwingWeaponLaserExplodeProcUpdate(GObj *weapon_gobj);
extern void grSectorArwingWeaponLaserExplodeInitVars(GObj *weapon_gobj);
extern sb32 grSectorArwingWeaponLaser3DProcMap(GObj *weapon_gobj);
extern sb32 grSectorArwingWeaponLaser3DProcHit(GObj *weapon_gobj);
extern sb32 grSectorArwingWeaponLaser3DProcAbsorb(GObj *weapon_gobj);
extern void grSectorArwingWeaponLaser3DMakeWeapon(void);
extern void func_ovl2_80107910(void);
extern void func_ovl2_80107958(void);
extern void func_ovl2_80107B30(void);
extern void grSectorArwingUpdateCollisions(void);
extern void grSectorArwingUpdatePatrol(void);
extern void func_ovl2_80107D50(void);
extern void grSectorProcUpdate(GObj *ground_gobj);
extern void grSectorInitAll(void);
extern GObj* grSectorMakeGround(void);

#endif
