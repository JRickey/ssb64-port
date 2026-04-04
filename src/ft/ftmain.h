#ifndef _FTMAIN_H_
#define _FTMAIN_H_

#include <sys/objdef.h>
#include <ft/ftdef.h>
#include <wp/wpdef.h>
#include <it/itdef.h>
#include <gr/grdef.h>
#include <gm/gmdef.h>

extern void ftMainParseMotionEvent(GObj *fighter_gobj, FTStruct *fp, FTMotionScript *ms, u32 ev_kind);
extern void ftMainUpdateMotionEventsAll(GObj* fighter_gobj);
extern void ftMainUpdateMotionEventsForward(GObj* fighter_gobj);
extern void ftMainUpdateMotionEventsForwardEffect(GObj* fighter_gobj);
extern void ftMainPlayAnimEventsAll(GObj* fighter_gobj);
extern void ftMainPlayAnimEventsForward(GObj* fighter_gobj);
extern sb32 ftMainUpdateColAnim(GMColAnim* colanim, GObj* fighter_gobj, sb32 is_muted, sb32 is_effect_skip);
extern void ftMainRunUpdateColAnim(GObj* fighter_gobj);
extern void ftMainProcUpdateInterrupt(GObj* fighter_gobj);
extern void ftMainClearGroundElementsAll();
extern sb32 ftMainCheckAddGroundObstacle(GObj* ogobj, sb32 (*proc_update)(GObj*, GObj*, s32*));
extern void ftMainClearGroundObstacle(GObj* ogobj);
extern sb32 ftMainCheckAddGroundHazard(GObj* egobj, sb32 (*proc_update)(GObj*, GObj*, GRAttackColl**, s32*));
extern void ftMainClearHazard(GObj* egobj);
extern void ftMainSetHitHazard(GObj* ogobj, GObj* fighter_gobj, FTStruct* fp, s32 kind);
extern void ftMainSearchHitHazard(GObj* fighter_gobj);
extern void ftMainUpdateVelDamageGround(FTStruct* fp, f32 move);
extern void ftMainProcPhysicsMap(GObj* fighter_gobj);
extern void ftMainProcPhysicsMapDefault(GObj* fighter_gobj);
extern void ftMainProcPhysicsMapCapture(GObj* fighter_gobj);
extern void ftMainSetHitInteractStats(FTStruct* fp, u32 attack_group_id, GObj* victim_gobj, s32 attack_type,
									  u32 victim_group_id, sb32 ignore_damage_or_hit);
extern void ftMainSetHitRebound(GObj* attacker_gobj, FTStruct* fp, FTAttackColl* attack_coll, GObj* victim_gobj);
extern void ftMainUpdateAttackStatFighter(FTStruct* other_fp, FTAttackColl* other_hit, FTStruct* this_fp, FTAttackColl* this_hit,
									GObj* other_gobj, GObj* this_gobj);
extern void ftMainUpdateShieldStatFighter(FTStruct* attacker_fp, FTAttackColl* attack_coll, FTStruct* victim_fp,
									GObj* attacker_gobj, GObj* victim_gobj);
extern void ftMainUpdateCatchStatFighter(FTStruct* attacker_fp, FTAttackColl* attack_coll, FTStruct* victim_fp,
								   GObj* attacker_gobj, GObj* victim_gobj);
extern void ftMainPlayHitSFX(FTStruct* fp, FTAttackColl* attack_coll);
extern sb32 ftMainCheckGetUpdateDamage(FTStruct* fp, s32* damage);
extern void ftMainUpdateDamageStatFighter(FTStruct* attacker_fp, FTAttackColl* attack_coll, FTStruct* victim_fp,
									FTDamageColl* damage_coll, GObj* attacker_gobj, GObj* victim_gobj);
extern void ftMainUpdateAttackStatWeapon(WPStruct* ip, WPAttackColl* wp_attack_coll, s32 attack_id, FTStruct* fp, FTAttackColl* ft_attack_coll,
								   GObj* weapon_gobj, GObj* fighter_gobj);
extern void ftMainUpdateShieldStatWeapon(WPStruct* wp, WPAttackColl* wp_attack_coll, s32 attack_id, FTStruct* fp, GObj* weapon_gobj,
								   GObj* fighter_gobj, f32 angle, Vec3f* vec);
extern void ftMainUpdateReflectorStatWeapon(WPStruct* wp, WPAttackColl* wp_attack_coll, FTStruct* fp, GObj* fighter_gobj);
extern void ftMainUpdateAbsorbStatWeapon(WPStruct* ip, WPAttackColl* wp_attack_coll, FTStruct* fp, GObj* fighter_gobj);
extern void ftMainUpdateDamageStatWeapon(WPStruct* wp, WPAttackColl* wp_attack_coll, s32 attack_id, FTStruct* fp, FTDamageColl* damage_coll,
								   GObj* weapon_gobj, GObj* fighter_gobj);
extern void ftMainUpdateAttackStatItem(ITStruct* ip, ITAttackColl* it_attack_coll, s32 attack_id, FTStruct* fp, FTAttackColl* ft_attack_coll,
								 GObj* item_gobj, GObj* fighter_gobj);
extern void ftMainUpdateAttackStatItem(ITStruct* ip, ITAttackColl* it_attack_coll, s32 attack_id, FTStruct* fp, FTAttackColl* ft_attack_coll,
								 GObj* item_gobj, GObj* fighter_gobj);
extern void ftMainUpdateShieldStatItem(ITStruct* ip, ITAttackColl* it_attack_coll, s32 attack_id, FTStruct* fp, GObj* item_gobj,
								 GObj* fighter_gobj, f32 angle, Vec3f* vec);
extern void ftMainUpdateReflectorStatItem(ITStruct* ip, ITAttackColl* it_attack_coll, FTStruct* fp, GObj* fighter_gobj);
extern void ftMainUpdateDamageStatItem(ITStruct* ip, ITAttackColl* it_attack_coll, s32 attack_id, FTStruct* fp, FTDamageColl* damage_coll,
								 GObj* item_gobj, GObj* fighter_gobj);
extern void ftMainUpdateDamageStatGround(GObj* special_gobj, GObj* fighter_gobj, FTStruct* fp, GRAttackColl* gr_attack_coll,
								   s32 target_kind);
extern void ftMainGetBumperDamageAngle(GObj* fighter_gobj, GObj* attacker_gobj);
extern void ftMainProcessHitCollisionStatsMain(GObj* fighter_gobj);
extern void ftMainSearchHitFighter(GObj* this_gobj);
extern void ftMainSearchHitWeapon(GObj* fighter_gobj);
extern void ftMainSearchHitItem(GObj* fighter_gobj);
extern sb32 ftMainGetGroundHitObstacle(FTStruct* fp, GRAttackColl** p_gr_attack_coll);
extern void ftMainSearchGroundHit(GObj* fighter_gobj);
extern void ftMainSearchFighterCatch(GObj* this_gobj);
extern void ftMainProcSearchCatch(GObj* fighter_gobj);
extern void ftMainProcSearchHitAll(GObj* fighter_gobj);
extern void ftMainProcParams(GObj* fighter_gobj);
extern void ftMainUpdateHiddenPartID(FTStruct* fp, s32 index);
extern void func_ovl2_800E6CE0(FTStruct* fp, s32 index);
extern void func_ovl2_800E6E00(FTStruct* fp, s32 index);
extern void ftMainSetStatus(GObj* fighter_gobj, s32 status_id, f32 frame_begin, f32 anim_speed, u32 flags);

#endif
