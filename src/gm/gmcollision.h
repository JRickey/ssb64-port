#ifndef _GMCOLLISION_H_
#define _GMCOLLISION_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gm/gmdef.h>
#include <ft/ftdef.h>
#include <wp/wpdef.h>
#include <it/itdef.h>

extern void gmCollisionCopyMatrix(Mtx44f dst, Mtx44f src);
extern void gmCollisionTransformMatrixAll(DObj *dobj, FTParts *parts, Mtx44f mtx);
extern void gmCollisionSetMatrixNcs(DObj *dobj, FTParts *parts, Mtx44f mtx, Vec3f *scale_mul);
extern void gmCollisionGetWorldPosition(Mtx44f mtx, Vec3f *vec);
extern void func_ovl2_800ED490(Mtx44f dst, Mtx44f lhs, Mtx44f rhs);
extern void gmCollisionSetInvertMatrix(Mtx44f dst, Mtx44f src);
extern void func_ovl2_800EDA0C(Mtx44f mtx, Vec3f *rotate);
extern void func_ovl2_800EDB88(Mtx44f mtx, Vec3f *vec);
extern void func_ovl2_800EDBA4(DObj *main_dobj);
extern void func_ovl2_800EDE00(DObj *main_dobj);
extern void func_ovl2_800EDE5C(DObj *main_dobj);
extern void gmCollisionGetFighterPartsWorldPosition(DObj *main_dobj, Vec3f *vec);
extern void func_ovl2_800EE018(DObj *main_dobj, Vec3f *vec);
extern void func_ovl2_800EE050(s32 arg0, Vec3f *arg1, Vec3f *arg2, sb32 *arg3, Mtx44f mtx, f32 *p_scale);
extern u32 func_ovl2_800EE24C(Vec3f *lhs, Vec3f *rhs);
extern u32 func_ovl2_800EE2C0(Vec3f *lhs, Vec3f *rhs);
extern sb32 gmCollisionTestRectangle(Vec3f *pos_curr, Vec3f *pos_prev, f32 radius, s32 opkind, Mtx44f mtx, Vec3f *offset, Vec3f *size, Vec3f *scale);
extern sb32 gmCollisionTestSphere(Vec3f *pos_curr, Vec3f *pos_prev, f32 hitsize, s32 attack_state, Mtx44f mtx, Vec3f *sphere_offset, Vec3f *sphere_size, Vec3f *arg7, s32 sphit_kind, f32 *p_angle, Vec3f *argA);
extern sb32 func_ovl2_800EEEAC(Vec3f *arg0, Vec3f *arg1, f32 arg2, s32 opkind, Mtx44f mtx, s32 arg5, Vec3f *arg6, f32 arg7, s32 arg8, f32 arg9);
extern sb32 gmCollisionCheckAttackInFighterRange(Vec3f *attack_position, Vec3f *obj_position, Vec3f *range, f32 size);
extern sb32 gmCollisionCheckFighterInFighterRange(FTAttackColl *attack_coll, GObj *fighter_gobj);
extern sb32 gmCollisionCheckWeaponInFighterRange(WPAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj);
extern sb32 gmCollisionCheckItemInFighterRange(ITAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj);
extern sb32 func_ovl2_800EF5D4(Vec3f *arg0, Vec3f *arg1, f32 arg2, s32 arg3, Vec3f *arg4, Vec3f *arg5, f32 arg6, s32 arg7);
extern sb32 gmCollisionCheckFighterAttacksCollide(FTAttackColl *attack_coll1, FTAttackColl *attack_coll2);
extern sb32 gmCollisionCheckFighterAttackDamageCollide(FTAttackColl *attack_coll, FTDamageColl *damage_coll);
extern sb32 gmCollisionCheckFighterAttackItemDamageCollide(FTAttackColl *attack_coll, ITDamageColl *damage_coll, GObj *item_gobj);
extern sb32 gmCollisionCheckFighterAttackShieldCollide(FTAttackColl *attack_coll, GObj *fighter_gobj, DObj *dobj, f32 *p_angle);
extern sb32 gmCollisionCheckWeaponAttackFighterAttackCollide(WPAttackColl *wp_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll);
extern sb32 gmCollisionCheckWeaponAttackFighterDamageCollide(WPAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll);
extern sb32 gmCollisionCheckWeaponAttackShieldCollide(WPAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj, DObj *dobj, f32 *p_angle, Vec3f *vec);
extern sb32 gmCollisionCheckWeaponAttackSpecialCollide(WPAttackColl *attack_coll, s32 attack_id, FTStruct *fp, FTSpecialColl *special_coll);
extern sb32 gmCollisionCheckWeaponAttacksCollide(WPAttackColl *attack_coll1, s32 atk1_id, WPAttackColl *attack_coll2, s32 atk2_id);
extern sb32 gmCollisionCheckWeaponAttackItemAttackCollide(WPAttackColl *wp_attack_coll, s32 wp_attack_id, ITAttackColl *it_attack_coll, s32 it_attack_id);
extern sb32 gmCollisionCheckItemAttackFighterAttackCollide(ITAttackColl *it_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll);
extern sb32 gmCollisionCheckItemAttackFighterDamageCollide(ITAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll);
extern sb32 gmCollisionCheckItemAttackShieldCollide(ITAttackColl *attack_coll, s32 attack_id, GObj *fighter_gobj, DObj *dobj, f32 *p_angle, Vec3f *vec);
extern sb32 gmCollisionCheckItemAttackSpecialCollide(ITAttackColl *attack_coll, s32 attack_id, FTStruct *fp, FTSpecialColl *special_coll);
extern sb32 gmCollisionCheckItemAttacksCollide(ITAttackColl *attack_coll1, s32 atk1_id, ITAttackColl *attack_coll2, s32 atk2_id);
extern sb32 gmCollisionCheckItemAttackDamageCollide(ITAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj);
extern sb32 gmCollisionCheckWeaponAttackItemDamageCollide(WPAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj);
extern void gmCollisionGetFighterAttackPosition(Vec3f *dst, FTAttackColl *ft_attack_coll);
extern void gmCollisionGetWeaponAttackPosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id);
extern void gmCollisionGetItemAttackPosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id);
extern void gmCollisionGetShieldPosition(Vec3f *dst, GObj *gobj, DObj *dobj);
extern void gmCollisionGetCommonImpactPosition(Vec3f *dst, Vec3f *pos, Vec3f *offset);
extern void gmCollisionGetFighterAttackDamagePosition(Vec3f *dst, FTAttackColl *ft_attack_coll, FTDamageColl *damage_coll);
extern void gmCollisionGetFighterAttackItemDamagePosition(Vec3f *dst, FTAttackColl *ft_attack_coll, ITDamageColl *damage_coll, GObj *item_gobj);
extern void gmCollisionGetFighterAttackShieldPosition(Vec3f *dst, FTAttackColl *ft_attack_coll, GObj *gobj, DObj *dobj);
extern void gmCollisionGetFighterAttacksPosition(Vec3f *dst, FTAttackColl *ft_attack_coll1, FTAttackColl *ft_attack_coll2);
extern void gmCollisionGetWeaponAttackFighterAttackPosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll);
extern void gmCollisionGetWeaponAttackShieldPosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, GObj *gobj, DObj *dobj);
extern void gmCollisionGetWeaponAttacksPosition(Vec3f *dst, WPAttackColl *attack_coll1, s32 atk1_id, WPAttackColl *attack_coll2, s32 atk2_id);
extern void gmCollisionGetWeaponAttackItemAttackPosition(Vec3f *dst, WPAttackColl *wp_attack_coll, s32 wp_attack_id, ITAttackColl *it_attack_coll, s32 it_attack_id);
extern void gmCollisionGetWeaponAttackFighterDamagePosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll);
extern void gmCollisionGetWeaponAttackItemDamagePosition(Vec3f *dst, WPAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj);
extern void gmCollisionGetItemAttackFighterDamagePosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id, FTDamageColl *damage_coll);
extern void gmCollisionGetItemAttackFighterAttackPosition(Vec3f *dst, ITAttackColl *it_attack_coll, s32 attack_id, FTAttackColl *ft_attack_coll);
extern void gmCollisionGetItemAttackShieldPosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id, GObj *gobj, DObj *dobj);
extern void gmCollisionGetItemAttackItemAttackPosition(Vec3f *dst, ITAttackColl *attack_coll1, s32 atk1_id, ITAttackColl *attack_coll2, s32 atk2_id);
extern void gmCollisionGetItemAttackItemDamagePosition(Vec3f *dst, ITAttackColl *attack_coll, s32 attack_id, ITDamageColl *damage_coll, GObj *item_gobj);
extern f32 gmCollisionGetDamageSlashRotation(FTStruct *fp, FTAttackColl *attack_coll);

#endif
