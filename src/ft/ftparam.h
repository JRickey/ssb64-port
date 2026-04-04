#ifndef _FTPARAM_H_
#define _FTPARAM_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>
#include <gm/gmdef.h>

extern s32 ftParamGetItemMusicLength(u32 bgm_id);
extern void ftParamTryPlayItemMusic(u32 bgm_id);
extern void ftParamTryUpdateItemMusic(void);
extern void ftParamInitPlayerBattleStats(s32 player, GObj *fighter_gobj);
extern GObj* ftParamGetPlayerNumGObj(s32 player_num);
extern void ftParamLockPlayerControl(GObj *fighter_gobj);
extern void ftParamUnlockPlayerControl(GObj *fighter_gobj);
extern void ftParamSetPlayerTagWait(GObj *fighter_gobj, s32 playertag_wait);
extern void ftParamSetDamageShuffle(FTStruct *fp, sb32 is_electric, s32 damage, s32 status_id, f32 hitlag_mul);
extern f32 ftParamGetStickAngleRads(FTStruct *fp);
extern void ftParamSetStickLR(FTStruct *fp);
extern void ftParamMakeRumble(FTStruct *fp, s32 rumble_id, s32 length);
extern void ftParamSetCaptureImmuneMask(FTStruct *fp, u8 capture_immune_mask);
extern void ftParamSetCatchParams(FTStruct *fp, u8 catch_mask, void (*proc_catch)(GObj*), void (*proc_capture)(GObj*, GObj*));
extern void ftParamSetThrowParams(FTStruct *this_fp, GObj *throw_gobj);
extern void ftParamPlayVoice(FTStruct *fp, u16 voice_id);
extern void ftParamStopVoice(FTStruct *fp);
extern void ftParamPlayLoopSFX(FTStruct *fp, u16 sfx_id);
extern void ftParamStopLoopSFX(FTStruct *fp);
extern void ftParamStopVoiceRunProcDamage(GObj *fighter_gobj);
extern void ftParamMoveDLLink(GObj *fighter_gobj, u8 dl_link);
extern void ftParamUpdateAnimKeys(GObj *fighter_gobj);
extern void ftParamClearAttackCollAll(GObj *fighter_gobj);
extern void ftParamClearAttackRecordID(FTStruct *fp, s32 attack_id);
extern void ftParamRefreshAttackCollID(GObj *fighter_gobj, s32 attack_id);
extern void ftParamSetVelPush(GObj *fighter_gobj, Vec3f *vel_push);
extern s32 ftParamGetJointID(FTStruct *fp, s32 joint_id);
extern void ftParamLinkResetShieldModelParts(GObj *fighter_gobj);
extern void ftParamSetHammerParams(GObj *fighter_gobj);
extern void ftParamSetHitStatusColAnim(GObj *fighter_gobj, s32 hitstatus);
extern void ftParamSetHitStatusPartAll(GObj *fighter_gobj, s32 hitstatus);
extern void ftParamSetHitStatusPartID(GObj *fighter_gobj, s32 joint_id, s32 hitstatus);
extern void ftParamSetHitStatusAll(GObj *fighter_gobj, s32 hitstatus);
extern s32 ftParamGetBestHitStatusPart(GObj *fighter_gobj);
extern s32 ftParamGetBestHitStatusAll(GObj *fighter_gobj);
extern void ftParamResetFighterDamageCollsAll(GObj *fighter_gobj);
extern void ftParamModifyDamageCollID(GObj *fighter_gobj, s32 joint_id, Vec3f *offset, Vec3f *size);
extern void ftParamSetModelPartID(GObj *fighter_gobj, s32 joint_id, s32 modelpart_id);
extern void ftParamSetModelPartDefaultID(GObj *fighter_gobj, s32 joint_id, s32 modelpart_id);
extern void ftParamResetModelPartAll(GObj *fighter_gobj);
extern void ftParamHideModelPartAll(GObj *fighter_gobj);
extern void ftParamSetModelPartDetailAll(GObj *fighter_gobj, u8 detail);
extern void ftParamInitAllParts(GObj *fighter_gobj, s32 costume, s32 shade);
extern void ftParamInitTexturePartAll(GObj *fighter_gobj);
extern void ftParamSetTexturePartID(GObj *fighter_gobj, s32 texturepart_id, s32 texture_id);
extern void ftParamResetTexturePartAll(GObj *fighter_gobj);
extern sb32 ftParamCheckSetColAnimID(GMColAnim *colanim, s32 colanim_id, s32 length);
extern sb32 ftParamCheckSetFighterColAnimID(GObj *fighter_gobj, s32 colanim_id, s32 length);
extern void ftParamResetColAnim(GMColAnim *colanim);
extern void ftParamResetFighterColAnim(GObj *fighter_gobj);
extern void ftParamResetStatUpdateColAnim(GObj *fighter_gobj);
extern sb32 ftParamCheckSetSkeletonColAnimID(GObj *fighter_gobj, s32 damage_level);
extern void ftParamSetKey(GObj *fighter_gobj, FTKeyEvent *script);
extern sb32 ftParamCheckHaveKey(GObj *fighter_gobj);
extern void ftParamRunProcEffect(GObj *fighter_gobj, void (*proc)(GObj*, EFStruct*));
extern void ftParamStopEffect(GObj *effect_gobj, EFStruct *ep);
extern void ftParamProcStopEffect(GObj *fighter_gobj);
extern void ftParamPauseEffect(GObj *effect_gobj, EFStruct *ep);
extern void ftParamProcPauseEffect(GObj *effect_gobj);
extern void ftParamResumeEffect(GObj *effect_gobj, EFStruct *ep);
extern void ftParamProcResumeEffect(GObj *fighter_gobj);
extern void ftParamVelDamageTransferGround(FTStruct *fp);
extern f32 ftParamGetCommonKnockback(s32 percent_damage, s32 recent_damage, s32 hit_damage, s32 knockback_weight, s32 knockback_scale, s32 knockback_base, f32 weight, s32 attack_handicap, s32 defend_handicap);
extern f32 ftParamGetGroundHazardKnockback(s32 percent_damage, s32 recent_damage, s32 hit_damage, s32 knockback_weight, s32 knockback_scale, s32 knockback_base, f32 weight, s32 attack_handicap, s32 defend_handicap);
extern f32 ftParamGetHitStun(f32 knockback);
extern s32 ftParamGetHitLag(s32 damage, s32 status_id, f32 hitlag_mul);
extern void ftParamUpdateDamage(FTStruct *fp, s32 damage);
extern void ftParamSetHealDamage(FTStruct *fp, s32 heal);
extern s32 ftParamGetCapturedDamage(FTStruct *fp, s32 damage);
extern f32 ftParamGetStale(s32 player, s32 attack_id, u16 motion_count);
extern s32 ftParamGetStaledDamage(s32 player, s32 damage, s32 attack_id, u16 motion_count);
extern u16 ftParamGetMotionCount(void);
extern void ftParamSetMotionID(FTStruct *fp, s32 attack_id);
extern void ftParamUpdateStaleQueue(s32 attack_player, s32 defend_player, s32 attack_id, u16 motion_count);
extern u16 ftParamGetStatUpdateCount(void);
extern void ftParamSetStatUpdate(FTStruct *fp, u16 flags);
extern void ftParamUpdate1PGameAttackStats(FTStruct *fp, u16 flags);
extern void ftParamSetStarHitStatusInvincible(FTStruct *fp, s32 star_invincible_tics);
extern void ftParamSetTimedHitStatusInvincible(FTStruct *fp, s32 invincible_tics);
extern void ftParamSetTimedHitStatusIntangible(FTStruct *fp, s32 intangible_tics);
extern void ftParamUpdatePlayerBattleStats(s32 attack_player, s32 defend_player, s32 attack_damage);
extern void ftParamUpdate1PGameDamageStats(FTStruct *fp, s32 damage_player, s32 damage_object_class, s32 damage_object_kind, u16 flags, u16 damage_stat_count);
extern void ftParamGetEffectJointPosition(FTStruct *fp, Vec3f *pos);
extern void* ftParamMakeEffect(GObj *fighter_gobj, s32 effect_id, s32 joint_id, Vec3f *effect_pos, Vec3f *effect_scatter, s32 lr, sb32 is_scale_pos, u32 arg7);
extern void ftParamKirbyTryMakeMapStarEffect(GObj *fighter_gobj);
extern void ftParamsUpdateFighterPartsTransformAll(DObj *arg0);
extern void ftParamsUpdateFighterPartsTransform(DObj *arg0);
extern void ftParamSetAnimLocks(FTStruct *fp);
extern void ftParamClearAnimLocks(FTStruct *fp);
extern void func_ovl2_800EB924(CObj *cobj, Mtx44f mtx, Vec3f *vec, f32 *rx, f32 *ry);
extern f32 func_ovl2_800EBA6C(Vec3f *arg0, Vec3f *arg1);
extern f32 func_ovl2_800EBB3C(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2);
extern void func_ovl2_800EBC0C(s32 arg0, Vec3f *arg1, f32 *arg2, f32 arg3, DObj *dobj);
extern void func_ovl2_800EBD08(DObj *root_dobj, f32 arg1, Vec3f *vec, f32 arg3);
extern s32 ftParamGetCostumeCommonID(s32 fkind, s32 color);
extern s32 ftParamGetCostumeTeamID(s32 fkind, s32 color);
extern s32 ftParamGetCostumeDebug(s32 fkind);
extern void ftParamInitGame(void);

#endif
