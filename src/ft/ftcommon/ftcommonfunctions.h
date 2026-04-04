#ifndef _FTCOMMON_FUNCTIONS_H_
#define _FTCOMMON_FUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// DeadDown / DeadLeft / DeadRight / DeadUpStar / DeadUpFall
void ftCommonDeadAddDeadSFXSoundQueue(u16 sfx_id);
void ftCommonDeadUpdateRumble(FTStruct* this_fp);
void ftCommonDeadUpdateRumble(FTStruct* this_fp);
void ftCommonDeadUpdateScore(FTStruct* this_fp);
void ftCommonDeadCheckRebirth(GObj* fighter_gobj);
void ftCommonDeadResetCommonVars(GObj* fighter_gobj);
void ftCommonDeadResetSpecialStats(GObj* fighter_gobj);
void ftCommonDeadCommonProcUpdate(GObj* fighter_gobj);
void ftCommonDeadInitStatusVars(GObj* fighter_gobj);
void ftCommonDeadDownSetStatus(GObj* fighter_gobj);
void ftCommonDeadRightSetStatus(GObj* fighter_gobj);
void ftCommonDeadLeftSetStatus(GObj* fighter_gobj);
void ftCommonDeadUpStarProcUpdate(GObj* fighter_gobj);
void ftCommonDeadUpStarSetStatus(GObj* fighter_gobj);
void ftCommonDeadUpFallProcUpdate(GObj* fighter_gobj);
void ftCommonDeadUpFallSetStatus(GObj* fighter_gobj);
sb32 ftCommonDeadCheckInterruptCommon(GObj* fighter_gobj);

// Sleep
sb32 ftCommonSleepCheckIgnorePauseMenu(GObj* fighter_gobj);
void ftCommonSleepProcUpdate(GObj* fighter_gobj);
void ftCommonSleepSetStatus(GObj* fighter_gobj);

// Entry / Appear
void ftCommonEntrySetStatus(GObj* fighter_gobj);
void ftCommonEntryNullProcUpdate(GObj* fighter_gobj);
void ftCommonAppearUpdateEffects(GObj* fighter_gobj);
void ftCommonAppearProcUpdate(GObj* fighter_gobj);
void ftCommonAppearProcPhysics(GObj* fighter_gobj);
void ftCommonAppearInitStatusVars(GObj* fighter_gobj);
void ftCommonAppearSetStatus(GObj* fighter_gobj);
void ftCommonAppearSetPosition(GObj* fighter_gobj); // This runs when spawning multi-character team
													  // members in VS mode I suppose?

// RebirthDown / RebirthStand / RebirthWait
void ftCommonRebirthDownSetStatus(GObj* this_gobj);
void ftCommonRebirthCommonUpdateHaloWait(GObj* fighter_gobj);
void ftCommonRebirthDownProcUpdate(GObj* fighter_gobj);
void ftCommonRebirthCommonProcMap(GObj* fighter_gobj);
void ftCommonRebirthStandProcUpdate(GObj* fighter_gobj);
void ftCommonRebirthStandSetStatus(GObj* fighter_gobj);
void ftCommonRebirthWaitProcUpdate(GObj* fighter_gobj);
void ftCommonRebirthWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonRebirthWaitSetStatus(GObj* fighter_gobj);

// Wait
void ftCommonWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonWaitSetStatus(GObj* fighter_gobj);
sb32 ftCommonWaitCheckInputSuccess(GObj* fighter_gobj);
sb32 ftCommonWaitCheckInterruptCommon(GObj* fighter_gobj);

// WalkSlow / WalkMiddle / WalkFast
f32 ftCommonWalkGetWalkAnimLength(FTStruct* fp, s32 status_id);
s32 ftCommonWalkGetWalkStatus(s8 stick_range_x);
void ftCommonWalkProcInterrupt(GObj* fighter_gobj);
void ftCommonWalkProcPhysics(GObj* fighter_gobj);
void ftCommonWalkSetStatusParam(GObj* fighter_gobj, f32 anim_frame_begin);
void ftCommonWalkSetStatusDefault(GObj* fighter_gobj);
sb32 ftCommonWalkCheckInputSuccess(GObj* fighter_gobj);
sb32 ftCommonWalkCheckInterruptCommon(GObj* fighter_gobj);

// Dash
void ftCommonDashProcUpdate(GObj* fighter_gobj);
void ftCommonDashProcInterrupt(GObj* fighter_gobj);
void ftCommonDashProcPhysics(GObj* fighter_gobj);
void ftCommonDashProcMap(GObj* fighter_gobj);
void ftCommonDashSetStatus(GObj* fighter_gobj, u32 flag);
sb32 ftCommonDashCheckInterruptCommon(GObj* fighter_gobj);
sb32 ftCommonDashCheckTurn(GObj* fighter_gobj);

// Run
void ftCommonRunProcInterrupt(GObj* fighter_gobj);
void ftCommonRunSetStatus(GObj* fighter_gobj);
sb32 ftCommonRunCheckInterruptDash(GObj* fighter_gobj);

// RunBrake
void ftCommonRunBrakeProcInterrupt(GObj* fighter_gobj);
void ftCommonRunBrakeProcPhysics(GObj* fighter_gobj);
void ftCommonRunBrakeSetStatus(GObj* fighter_gobj, u32 flag);
sb32 ftCommonRunBrakeCheckInterruptRun(GObj* fighter_gobj);
sb32 ftCommonRunBrakeCheckInterruptTurnRun(GObj* fighter_gobj);

// Turn
void ftCommonTurnProcUpdate(GObj* fighter_gobj);
void ftCommonTurnProcInterrupt(GObj* fighter_gobj);
void ftCommonTurnSetStatus(GObj* fighter_gobj, s32 lr_dash);
void ftCommonTurnSetStatusCenter(GObj* fighter_gobj);
void ftCommonTurnSetStatusInvertLR(GObj* fighter_gobj);
sb32 ftCommonTurnCheckInputSuccess(GObj* fighter_gobj);
sb32 ftCommonTurnCheckInterruptCommon(GObj* fighter_gobj);

// TurnRun
void ftCommonTurnRunProcUpdate(GObj* fighter_gobj);
void ftCommonTurnRunProcInterrupt(GObj* fighter_gobj);
void ftCommonTurnRunSetStatus(GObj* fighter_gobj);
sb32 ftCommonTurnRunCheckInterruptRun(GObj* fighter_gobj);

// KneeBend / GuardKneeBend
void ftCommonKneeBendProcUpdate(GObj* fighter_gobj);
void ftCommonKneeBendProcInterrupt(GObj* fighter_gobj);
void ftCommonKneeBendSetStatusParam(GObj* fighter_gobj, s32 status_id, s32 input_source);
void ftCommonKneeBendSetStatus(GObj* fighter_gobj, s32 input_source);
void ftCommonGuardKneeBendSetStatus(GObj* fighter_gobj, s32 input_source);
sb32 ftCommonKneeBendCheckButtonTap(FTStruct* fp);
s32 ftCommonKneeBendGetInputTypeCommon(FTStruct* fp);
sb32 ftCommonKneeBendCheckInterruptCommon(GObj* fighter_gobj);
s32 ftCommonKneeBendGetInputTypeRun(FTStruct* fp);
sb32 ftCommonKneeBendCheckInterruptRun(GObj* fighter_gobj);
sb32 ftCommonGuardKneeBendCheckInterruptGuard(GObj* fighter_gobj);

// Jump
void ftCommonJumpProcInterrupt(GObj* fighter_gobj);
void ftCommonJumpGetJumpForceButton(s32 stick_range_x, s32* jump_vel_x, s32* jump_vel_y, sb32 is_shorthop);
void ftCommonJumpSetStatus(GObj* fighter_gobj);

// JumpAerial
void ftCommonJumpAerialUpdateModelYaw(FTStruct* fp);
void ftCommonJumpAerialProcUpdate(GObj* fighter_gobj);
void ftCommonJumpAerialProcInterrupt(GObj* fighter_gobj);
void ftCommonJumpAerialProcPhysics(GObj* fighter_gobj);
void ftCommonJumpAerialSetStatus(GObj* fighter_gobj, s32 input_source);
void ftCommonJumpAerialMultiSetStatus(GObj* fighter_gobj, s32 input_source);
sb32 ftCommonJumpAerialMultiCheckJumpButtonHold(FTStruct* fp);
s32 ftCommonJumpAerialMultiGetJumpInputType(FTStruct* fp);
sb32 ftCommonJumpAerialCheckInterruptCommon(GObj* fighter_gobj);

// Fall / FallAerial
void ftCommonFallProcInterrupt(GObj* fighter_gobj);
void ftCommonFallSetStatus(GObj* fighter_gobj);

// Squat / SquatWait / SquatRv
sb32 ftCommonSquatCheckGotoPass(GObj* fighter_gobj);
void ftCommonSquatProcUpdate(GObj* fighter_gobj);
void ftCommonSquatProcInterrupt(GObj* fighter_gobj);
void ftCommonSquatSetStatusNoPass(GObj* fighter_gobj);
void ftCommonSquatSetStatusPass(GObj* fighter_gobj);
sb32 ftCommonSquatCheckInterruptCommon(GObj* fighter_gobj);
void ftCommonSquatWaitProcUpdate(GObj* fighter_gobj);
void ftCommonSquatWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonSquatWaitSetStatus(GObj* fighter_gobj);
void ftCommonSquatWaitSetStatusNoPass(GObj* fighter_gobj);
sb32 ftCommonSquatWaitCheckInterruptLanding(GObj* fighter_gobj);
void ftCommonSquatRvProcInterrupt(GObj* fighter_gobj);
void ftCommonSquatRvSetStatus(GObj* fighter_gobj);
sb32 ftCommonSquatRvCheckInterruptSquatWait(GObj* fighter_gobj);

// LandingLight / LandingHeavy
void ftCommonLandingProcInterrupt(GObj* fighter_gobj);
void ftCommonLandingSetStatusParam(GObj* fighter_gobj, s32 status_id, sb32 is_allow_interrupt, f32 anim_speed);
void ftCommonLandingSetStatus(GObj* fighter_gobj);
void ftCommonLandingAirNullSetStatus(GObj* fighter_gobj, f32 anim_speed);
void ftCommonLandingFallSpecialSetStatus(GObj* fighter_gobj, sb32 is_allow_interrupt, f32 anim_speed);

// Pass
void ftCommonPassProcInterrupt(GObj* fighter_gobj);
void ftCommonPassSetStatusParam(GObj* fighter_gobj, s32 status_id, f32 frame_begin, u32 flags);
void ftCommonPassSetStatusSquat(GObj* fighter_gobj);
void ftCommonGuardPassSetStatus(GObj* fighter_gobj);
sb32 ftCommonPassCheckInputSuccess(FTStruct* fp);
sb32 ftCommonPassCheckInterruptCommon(GObj* fighter_gobj);
sb32 ftCommonPassCheckInterruptSquat(GObj* fighter_gobj);
sb32 ftCommonGuardPassCheckInterruptGuard(GObj* fighter_gobj);

// Ottotto / OttottoWait
void ftCommonOttottoProcUpdate(GObj* fighter_gobj);
void ftCommonOttottoProcInterrupt(GObj* fighter_gobj);
void ftCommonOttottoProcMap(GObj* fighter_gobj);
void ftCommonOttottoWaitSetStatus(GObj* fighter_gobj);

// Damage
void ftCommonDamageSetDustGFXInterval(FTStruct* fp);
void ftCommonDamageUpdateDustEffect(GObj* fighter_gobj);
void ftCommonDamageDecHitStunSetPublic(GObj* fighter_gobj);
void ftCommonDamageCommonProcUpdate(GObj* fighter_gobj);
void ftCommonDamageAirCommonProcUpdate(GObj* fighter_gobj);
void ftCommonDamageCheckSetInvincible(GObj* fighter_gobj);
void ftCommonDamageSetStatus(GObj* fighter_gobj);
void ftCommonDamageCommonProcInterrupt(GObj* fighter_gobj);
void ftCommonDamageAirCommonProcInterrupt(GObj* fighter_gobj);
void ftCommonDamageFlyRollUpdateModelPitch(GObj* fighter_gobj);
void ftCommonDamageCommonProcPhysics(GObj* fighter_gobj);
void ftCommonDamageCommonProcLagUpdate(GObj* fighter_gobj);
void func_ovl3_80140934();
void ftCommonDamageAirCommonProcMap(GObj* fighter_gobj);
f32 ftCommonDamageGetKnockbackAngle(s32 angle_i, s32 ga, f32 knockback);
s32 ftCommonDamageGetDamageLevel(f32 hitstun);
void ftCommonDamageSetPublic(FTStruct* this_fp, f32 knockback, f32 angle);
sb32 ftCommonDamageCheckElementSetColAnim(GObj* fighter_gobj, s32 element, s32 damage_level);
void ftCommonDamageCheckMakeScreenFlash(f32 knockback, s32 element);
sb32 ftCommonDamageCheckCatchResist(FTStruct* fp);
void ftCommonDamageUpdateCatchResist(GObj* fighter_gobj);
sb32 ftCommonDamageCheckCaptureKeepHold(FTStruct* fp);
void ftCommonDamageInitDamageVars(GObj* this_gobj, s32 status_id_replace, s32 damage, f32 knockback, s32 angle_start,
									s32 damage_lr, s32 damage_index, s32 element, s32 damage_player_num, s32 arg9,
									sb32 unk_bool, sb32 is_public);
void ftCommonDamageGotoDamageStatus(GObj* fighter_gobj);
void ftCommonDamageUpdateDamageColAnim(GObj* fighter_gobj, f32 knockback, s32 element);
void ftCommonDamageSetDamageColAnim(GObj* fighter_gobj);
void ftCommonDamageUpdateMain(GObj* fighter_gobj);
void ftCommonWallDamageProcUpdate(GObj* fighter_gobj);
void ftCommonWallDamageSetStatus(GObj* fighter_gobj, Vec3f* angle, Vec3f* pos);
sb32 ftCommonWallDamageCheckGoto(GObj* fighter_gobj);

// DamageFall
void ftCommonDamageFallProcInterrupt(GObj* fighter_gobj);
void ftCommonDamageFallProcMap(GObj* fighter_gobj);
void ftCommonDamageFallClampRumble(GObj* fighter_gobj);
void ftCommonDamageFallSetStatusFromDamage(GObj* fighter_gobj);
void ftCommonDamageFallSetStatusFromCliffWait(GObj* fighter_gobj);
void func_ovl3_801436F0(GObj* fighter_gobj);

// FallSpecial
void ftCommonFallSpecialProcInterrupt(GObj* fighter_gobj);
void ftCommonFallSpecialProcPhysics(GObj* fighter_gobj);
sb32 ftCommonFallSpecialProcPass(GObj* fighter_gobj);
void ftCommonFallSpecialProcMap(GObj* fighter_gobj);
void ftCommonFallSpecialSetStatus(GObj* fighter_gobj, f32 drift, sb32 unknown, sb32 is_fall_accelerate,
									sb32 is_goto_landing, f32 landing_lag, sb32 is_allow_interrupt);

// Twister
void ftCommonTwisterProcUpdate(GObj* fighter_gobj);
void ftCommonTwisterProcPhysics(GObj* fighter_gobj);
void ftCommonTwisterSetStatus(GObj* fighter_gobj, GObj* tornado_gobj);
void ftCommonTwisterShootFighter(GObj* fighter_gobj);

// TaruCann
void ftCommonTaruCannProcUpdate(GObj* fighter_gobj);
void ftCommonTaruCannProcInterrupt(GObj* fighter_gobj);
void ftCommonTaruCannProcPhysics(GObj* fighter_gobj);
void ftCommonTaruCannSetStatus(GObj* fighter_gobj, GObj* tarucann_gobj);
void ftCommonTaruCannShootFighter(GObj* fighter_gobj);

// Dokan
void ftCommonDokanStartUpdateModelYaw(FTStruct* fp);
void ftCommonDokanStartProcUpdate(GObj* fighter_gobj);
void ftCommonDokanStartProcPhysics(GObj* fighter_gobj);
void ftCommonDokanStartSetStatus(GObj* fighter_gobj, s32 floor_line_id);
sb32 ftCommonDokanStartCheckInterruptCommon(GObj* fighter_gobj);
void ftCommonDokanWaitProcUpdate(GObj* fighter_gobj);
void ftCommonDokanWaitProcMap(GObj* fighter_gobj);
void ftCommonDokanWaitSetStatus(GObj* fighter_gobj);
void ftCommonDokanEndUpdateModelYaw(GObj* fighter_gobj);
void ftCommonDokanEndUpdatePlayerTag(GObj* fighter_gobj);
void ftCommonDokanEndProcUpdate(GObj* fighter_gobj);
void ftCommonDokanEndSetStatus(GObj* fighter_gobj);
void ftCommonDokanWalkSetStatus(GObj* fighter_gobj);

// DownWait / DownBounce
void ftCommonDownWaitProcUpdate(GObj* fighter_gobj);
void ftCommonDownWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonDownWaitSetStatus(GObj* fighter_gobj);
void ftCommonDownBounceProcUpdate(GObj* fighter_gobj);
// Up = 0, Down = 1
sb32 ftCommonDownBounceCheckUpOrDown(GObj* fighter_gobj);
void ftCommonDownBounceUpdateEffects(GObj* fighter_gobj);
void ftCommonDownBounceSetStatus(GObj* fighter_gobj);

// DownStand
void ftCommonDownStandProcInterrupt(GObj* fighter_gobj);
void ftCommonDownStandSetStatus(GObj* fighter_gobj);
sb32 ftCommonDownStandCheckInterruptCommon(GObj* fighter_gobj);

// PassiveStand
void ftCommonPassiveStandSetStatus(GObj* fighter_gobj, s32 status_id);
sb32 ftCommonPassiveStandCheckInterruptDamage(GObj* fighter_gobj);

// DownForward / DownBack
void ftCommonDownForwardOrBackSetStatus(GObj* fighter_gobj, s32 status_id);
sb32 ftCommonDownForwardOrBackCheckInterruptCommon(GObj* fighter_gobj);

// DownAttack
void ftCommonDownAttackSetStatus(GObj* fighter_gobj, s32 status_id);
sb32 ftCommonDownAttackCheckInterruptDownBounce(GObj* fighter_gobj);
sb32 ftCommonDownAttackCheckInterruptDownWait(GObj* fighter_gobj);

// Passive
void ftCommonPassiveSetStatus(GObj* fighter_gobj);
sb32 ftCommonPassiveCheckInterruptDamage(GObj* fighter_gobj);

// Rebound / ReboundWait
void ftCommonReboundProcUpdate(GObj* fighter_gobj);
void ftCommonReboundSetStatus(GObj* fighter_gobj);
void ftCommonReboundWaitProcUpdate(GObj* fighter_gobj);
void ftCommonReboundWaitSetStatus(GObj* fighter_gobj);

// CliffCatch / CliffWait
void ftCommonCliffCatchProcUpdate(GObj* fighter_gobj);
void ftCommonCliffCommonProcPhysics(GObj* fighter_gobj);
void ftCommonCliffCommonProcMap(GObj* fighter_gobj);
void ftCommonCliffCatchSetStatus(GObj* fighter_gobj);
void ftCommonCliffCommonProcDamage(GObj* fighter_gobj);
void ftCommonCliffWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonCliffWaitSetStatus(GObj* fighter_gobj);
sb32 ftCommonCliffWaitCheckFall(GObj* fighter_gobj);

// CliffQuick / CliffSlow / CliffClimb
void ftCommonCliffQuickProcUpdate(GObj* fighter_gobj);
void ftCommonCliffSlowProcUpdate(GObj* fighter_gobj);
void ftCommonCliffQuickOrSlowSetStatus(GObj* fighter_gobj, s32 status_input);
sb32 ftCommonCliffClimbOrFallCheckInterruptCommon(GObj* fighter_gobj);
void ftCommonCliffClimbQuick1ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffClimbSlow1ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffClimbQuick1SetStatus(GObj* fighter_gobj);
void ftCommonCliffClimbSlow1SetStatus(GObj* fighter_gobj);
void ftCommonCliffCommon2ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffCommon2ProcPhysics(GObj* fighter_gobj);
void ftCommonCliffClimbCommon2ProcMap(GObj* fighter_gobj);
void ftCommonCliffAttackEscape2ProcMap(GObj* fighter_gobj);
void ftCommonCliffCommon2UpdateCollData(GObj* fighter_gobj);
void ftCommonCliffCommon2InitStatusVars(GObj* fighter_gobj);
void ftCommonCliffClimbQuick2SetStatus(GObj* fighter_gobj);
void ftCommonCliffClimbSlow2SetStatus(GObj* fighter_gobj);

// CliffAttack
sb32 ftCommonCliffAttackCheckInterruptCommon(GObj* fighter_gobj);
void ftCommonCliffAttackQuick1ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffAttackSlow1ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffAttackQuick1SetStatus(GObj* fighter_gobj);
void ftCommonCliffAttackSlow1SetStatus(GObj* fighter_gobj);
void ftCommonCliffAttackQuick2SetStatus(GObj* fighter_gobj);
void ftCommonCliffAttackSlow2SetStatus(GObj* fighter_gobj);

// CliffEscape
sb32 ftCommonCliffEscapeCheckInterruptCommon(GObj* fighter_gobj);
void ftCommonCliffEscapeQuick1ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffEscapeSlow1ProcUpdate(GObj* fighter_gobj);
void ftCommonCliffEscapeQuick1SetStatus(GObj* fighter_gobj);
void ftCommonCliffEscapeSlow1SetStatus(GObj* fighter_gobj);
void ftCommonCliffEscapeQuick2SetStatus(GObj* fighter_gobj);
void ftCommonCliffEscapeSlow2SetStatus(GObj* fighter_gobj);

// LightGet / HeavyGet / LiftWait / LiftTurn
GObj* ftCommonGetFindItem(GObj* fighter_gobj, u8 pickup_mask);
void ftCommonLightGetProcDamage(GObj* fighter_gobj);
void ftCommonHeavyGetProcDamage(GObj* fighter_gobj);
void ftCommonGetProcUpdate(GObj* fighter_gobj);
void ftCommonLightGetProcMap(GObj* fighter_gobj);
void ftCommonHeavyGetProcMap(GObj* fighter_gobj);
void ftCommonHeavyThrowProcMap(GObj* fighter_gobj);
void ftCommonGetSetStatus(GObj* fighter_gobj, GObj* item_gobj);
sb32 ftCommonGetCheckInterruptCommon(GObj* fighter_gobj);
void ftCommonLiftWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonLiftWaitSetStatus(GObj* fighter_gobj);
void ftCommonLiftTurnUpdateModelYaw(FTStruct* fp);
void ftCommonLiftTurnProcUpdate(GObj* fighter_gobj);
void ftCommonLiftTurnProcInterrupt(GObj* fighter_gobj);
void ftCommonLiftTurnSetStatus(GObj* fighter_gobj);
sb32 ftCommonLiftTurnCheckInterruptLiftWait(GObj* fighter_gobj);

// ItemThrow
void ftCommonItemThrowUpdateModelYaw(GObj* fighter_gobj);
void ftCommonItemThrowProcUpdate(GObj* fighter_gobj);
void ftCommonItemThrowProcPhysics(GObj* fighter_gobj);
void ftCommonItemThrowInitCommandVars(FTStruct* fp);
void ftCommonItemThrowInitStatusVars(FTStruct* fp);
void ftCommonItemThrowSetStatus(GObj* fighter_gobj, s32 status_id);
void ftCommonLightThrowDecideSetStatus(GObj* fighter_gobj);
void ftCommonHeavyThrowDecideSetStatus(GObj* fighter_gobj);
sb32 ftCommonLightThrowCheckItemTypeThrow(FTStruct* fp);
sb32 ftCommonLightThrowCheckInterruptGuardOnOn(GObj* fighter_gobj);
sb32 ftCommonLightThrowCheckInterruptEscape(GObj* fighter_gobj);
sb32 ftCommonHeavyThrowCheckInterruptCommon(GObj* fighter_gobj);

// ItemSwing
void ftCommonHarisenSwingProcHit(GObj* fighter_gobj);
void ftCommonHarisenSwingProcUpdate(GObj* fighter_gobj);
void ftCommonStarRodSwingProcUpdate(GObj* fighter_gobj);
void ftCommonItemSwingSetStatus(GObj* fighter_gobj, s32 swing_type);

// ItemShoot
void ftCommonLGunShootProcUpdate(GObj* fighter_gobj);
void ftCommonLGunShootProcAccessory(GObj* fighter_gobj);
void ftCommonLGunShootProcMap(GObj* fighter_gobj);
void ftCommonLGunShootAirProcMap(GObj* fighter_gobj);
void ftCommonLGunShootAirSwitchStatusGround(GObj* fighter_gobj);
void ftCommonLGunShootSwitchStatusAir(GObj* fighter_gobj);
void ftCommonFireFlowerShootProcUpdate(GObj* fighter_gobj);
void ftCommonFireFlowerShootUpdateAmmoStats(FTStruct* fp, s32 ammo_sub);
void ftCommonFireFlowerShootProcAccessory(GObj* fighter_gobj);
void ftCommonFireFlowerShootProcMap(GObj* fighter_gobj);
void ftCommonFireFlowerShootAirProcMap(GObj* fighter_gobj);
void ftCommonFireFlowerShootAirSwitchStatusGround(GObj* fighter_gobj);
void ftCommonFireFlowerShootSwitchStatusAir(GObj* fighter_gobj);
void ftCommonFireFlowerShootInitStatusVars(FTStruct* fp);
void ftCommonItemShootSetStatus(GObj* fighter_gobj);
void ftCommonItemShootAirSetStatus(GObj* fighter_gobj);

// HammerWalk
void ftCommonHammerWalkProcInterrupt(GObj* fighter_gobj);
void ftCommonHammerWalkSetStatus(GObj* fighter_gobj);
sb32 ftCommonHammerWalkCheckInterruptCommon(GObj* fighter_gobj);

// HammerTurn
void ftCommonHammerTurnUpdateModelYaw(GObj* fighter_gobj);
void ftCommonHammerTurnProcUpdate(GObj* fighter_gobj);
void ftCommonHammerTurnProcInterrupt(GObj* fighter_gobj);
void ftCommonHammerTurnSetStatus(GObj* fighter_gobj);
sb32 ftCommonHammerTurnCheckInterruptCommon(GObj* fighter_gobj);

// HammerKneeBend
void ftCommonHammerKneeBendProcUpdate(GObj* fighter_gobj);
void ftCommonHammerKneeBendProcInterrupt(GObj* fighter_gobj);
void ftCommonHammerKneeBendSetStatus(GObj* fighter_gobj, s32 input_source);
sb32 ftCommonHammerKneeBendCheckInterruptCommon(GObj* fighter_gobj);

// HammerFall
void ftCommonHammerFallProcInterrupt(GObj* fighter_gobj);
void ftCommonHammerFallProcMap(GObj* fighter_gobj);
void ftCommonHammerFallSetStatus(GObj* fighter_gobj);
sb32 ftCommonHammerFallCheckInterruptDamageFall(GObj* fighter_gobj);
void ftCommonHammerFallSetStatusJump(GObj* fighter_gobj);
void ftCommonHammerFallSetStatusPass(GObj* fighter_gobj);
sb32 ftCommonHammerFallCheckInterruptCommon(GObj*);

// HammerLanding
void ftCommonHammerLandingProcUpdate(GObj* fighter_gobj);
void ftCommonHammerLandingSetStatus(GObj* fighter_gobj);

// GuardOn / Guard
void ftCommonGuardCheckScheduleRelease(FTStruct* fp);
void ftCommonGuardOnSetHitStatusYoshi(GObj* fighter_gobj);
void ftCommonGuardSetHitStatusYoshi(GObj* fighter_gobj);
void ftCommonGuardOffSetHitStatusYoshi(GObj* fighter_gobj);
void ftCommonGuardUpdateShieldVars(GObj* fighter_gobj);
void ftCommonGuardUpdateShieldCollision(FTStruct* fp);
void ftCommonGuardUpdateShieldAngle(FTStruct* fp);
void ftCommonGuardGetJointTransform(DObj* joint, DObjDesc* dobjdesc, f32 range);
void ftCommonGuardGetJointTransformScale(DObj* joint, DObjDesc* dobjdesc, f32 range, Vec3f* scale);
void ftCommonGuardUpdateJoints(GObj* fighter_gobj);
void ftCommonGuardInitJoints(GObj* fighter_gobj);
void ftCommonGuardOnProcUpdate(GObj* fighter_gobj);
void ftCommonGuardCommonProcInterrupt(GObj* fighter_gobj);
void ftCommonGuardOnSetStatus(GObj* fighter_gobj, s32 slide_tics);
sb32 ftCommonGuardOnCheckInterruptSuccess(GObj* fighter_gobj, s32 slide_tics);
sb32 ftCommonGuardOnCheckInterruptCommon(GObj* fighter_gobj);
sb32 ftCommonGuardOnCheckInterruptDashRun(GObj* fighter_gobj, s32 slide_tics);
void ftCommonGuardProcUpdate(GObj* fighter_gobj);
void ftCommonGuardSetStatus(GObj* fighter_gobj);

// GuardOff / GuardSetOff
void ftCommonGuardSetStatusFromEscape(GObj* fighter_gobj);
sb32 ftCommonGuardCheckInterruptEscape(GObj* fighter_gobj);
void ftCommonGuardOffProcUpdate(GObj* fighter_gobj);
void ftCommonGuardOffSetStatus(GObj* fighter_gobj);
void ftCommonGuardSetOffProcUpdate(GObj* fighter_gobj);
void ftCommonGuardSetOffSetStatus(GObj* fighter_gobj);

// EscapeF / EscapeB
void ftCommonEscapeProcUpdate(GObj* fighter_gobj);
void ftCommonEscapeProcInterrupt(GObj* fighter_gobj);
void ftCommonEscapeProcStatus(GObj* fighter_gobj);

// Samus erroneously calls this without specifying itemthrow_buffer_tics; same story as Link's Bomb?
// void ftCommonEscapeSetStatus(GObj* fighter_gobj, s32 status_id, s32 itemthrow_buffer_tics); 
s32 ftCommonEscapeGetStatus(FTStruct* fp);
sb32 ftCommonEscapeCheckInterruptSpecialNDonkey(GObj* fighter_gobj);
sb32 ftCommonEscapeCheckInterruptDash(GObj* fighter_gobj);
sb32 ftCommonEscapeCheckInterruptGuard(GObj* fighter_gobj);

// ShieldBreakFly
void ftCommonShieldBreakFlyProcUpdate(GObj* fighter_gobj);
void ftCommonShieldBreakFlyProcMap(GObj* fighter_gobj);
void ftCommonShieldBreakFlySetStatus(GObj* fighter_gobj);
void ftCommonShieldBreakFlyCommonSetStatus(GObj* fighter_gobj);
void ftCommonShieldBreakFlyReflectorSetStatus(GObj* fighter_gobj);

// ShieldBreakFall
void ftCommonShieldBreakFallProcMap(GObj* fighter_gobj);
void ftCommonShieldBreakFallSetStatus(GObj* fighter_gobj);

// ShieldBreakDown
void ftCommonShieldBreakDownProcUpdate(GObj* fighter_gobj);
void ftCommonShieldBreakDownSetStatus(GObj* fighter_gobj);

// ShieldBreakStand
void ftCommonShieldBreakStandProcUpdate(GObj* fighter_gobj);
void ftCommonShieldBreakStandSetStatus(GObj* fighter_gobj);

// FuraFura
void ftCommonFuraFuraProcUpdate(GObj* fighter_gobj);
void ftCommonFuraFuraSetStatus(GObj* fighter_gobj);

// FuraSleep
void ftCommonFuraSleepProcUpdate(GObj* fighter_gobj);
void ftCommonFuraSleepSetStatus(GObj* fighter_gobj);

// Catch
void ftCommonCatchProcUpdate(GObj* fighter_gobj);
void ftCommonCatchCaptureSetStatusRelease(GObj* fighter_gobj);
void func_ovl3_80149B48(GObj* fighter_gobj); // Unused?
void ftCommonCatchProcMap(GObj* fighter_gobj);
void ftCommonCatchSetStatus(GObj* fighter_gobj);
sb32 ftCommonCatchCheckInterruptGuard(GObj* fighter_gobj);
sb32 ftCommonCatchCheckInterruptCommon(GObj* fighter_gobj);
sb32 ftCommonCatchCheckInterruptDashRun(GObj* fighter_gobj);
sb32 ftCommonCatchCheckInterruptAttack11(GObj* fighter_gobj);

// CatchPull / CatchWait
void ftCommonCatchPullProcUpdate(GObj* fighter_gobj);
void ftCommonCatchPullProcCatch(GObj* fighter_gobj);
void ftCommonCatchWaitProcInterrupt(GObj* fighter_gobj);
void ftCommonCatchWaitSetStatus(GObj* fighter_gobj);

// ThrowF / ThrowB
void ftCommonThrowProcUpdate(GObj* fighter_gobj);
void ftCommonThrowSetStatus(GObj* fighter_gobj, sb32 is_throwf);
sb32 ftCommonThrowCheckInterruptCatchWait(GObj* fighter_gobj);

// CapturePulled
void ftCommonCapturePulledRotateScale(GObj* fighter_gobj, Vec3f* this_pos, Vec3f* rotate);
void ftCommonCapturePulledProcPhysics(GObj* fighter_gobj);
void ftCommonCapturePulledProcMap(GObj* fighter_gobj);
void ftCommonCapturePulledProcCapture(GObj* fighter_gobj, GObj* capture_gobj);

// CaptureWait
void ftCommonCaptureWaitProcMap(GObj* fighter_gobj);
void ftCommonCaptureWaitSetStatus(GObj* fighter_gobj);

// CatchCut / CaptureCut / Shouldered
void ftCommonCaptureApplyCatchKnockback(GObj* fighter_gobj, FTThrowReleaseDesc* throw_release);
void ftCommonCaptureApplyCaptureKnockback(GObj* fighter_gobj, FTThrowReleaseDesc* throw_release);
void ftCommonCaptureTrappedInitBreakoutVars(FTStruct* fp, s32 breakout_wait);
sb32 ftCommonCaptureTrappedUpdateBreakoutVars(FTStruct* fp);
void ftCommonCaptureShoulderedProcInterrupt(GObj* fighter_gobj);
void ftCommonCaptureShoulderedSetStatus(GObj* fighter_gobj);

// CaptureKirby
void ftCommonCaptureKirbyUpdatePositionsMag(GObj* fighter_gobj, Vec3f* dist);
void ftCommonCaptureKirbyUpdatePositionsAll(GObj* fighter_gobj);
void ftCommonCaptureKirbyProcPhysics(GObj* fighter_gobj);
void ftCommonCaptureKirbyProcCapture(GObj* fighter_gobj, GObj* capture_gobj);
void ftCommonCaptureWaitKirbyUpdateBreakoutVars(FTStruct* this_fp, FTStruct* capture_fp);
void ftCommonCaptureWaitKirbyProcMap(GObj* fighter_gobj);
void ftCommonCaptureWaitKirbyProcInterrupt(GObj* fighter_gobj);
void ftCommonCaptureWaitKirbySetStatus(GObj* fighter_gobj);
void ftCommonThrownKirbyEscape(GObj* fighter_gobj);
void ftCommonThrownCommonStarProcHit(GObj* fighter_gobj);
void ftCommonThrownKirbyStarMakeEffect(GObj* fighter_gobj, f32 arg1, f32 arg2);
void ftCommonThrownKirbyStarProcUpdate(GObj* fighter_gobj);
void ftCommonThrownCommonStarUpdatePhysics(GObj* fighter_gobj, f32 decelerate);
void ftCommonThrownKirbyStarProcPhysics(GObj* fighter_gobj);
void ftCommonThrownCommonStarProcMap(GObj* fighter_gobj);
void ftCommonThrownKirbyStarInitStatusVars(GObj* fighter_gobj);
void ftCommonThrownKirbyStarProcStatus(GObj* fighter_gobj);
void ftCommonThrownKirbyStarSetStatus(GObj* fighter_gobj);
void ftCommonThrownCopyStarProcUpdate(GObj* fighter_gobj);
void ftCommonThrownCopyStarProcPhysics(GObj* fighter_gobj);
void ftCommonThrownCopyStarProcStatus(GObj* fighter_gobj);
void ftCommonThrownCopyStarSetStatus(GObj* fighter_gobj);

// CaptureYoshi / YoshiEgg
void func_ovl3_8014C770(); // Unused
void ftCommonCaptureYoshiProcPhysics(GObj* fighter_gobj);
void ftCommonCaptureYoshiProcCapture(GObj* fighter_gobj, GObj* capture_gobj);
void ftCommonYoshiEggMakeEffect(GObj* fighter_gobj);
void ftCommonYoshiEggProcUpdate(GObj* fighter_gobj);
void ftCommonYoshiEggProcInterrupt(GObj* fighter_gobj);
void ftCommonYoshiEggProcPhysics(GObj* fighter_gobj);
void ftCommonYoshiEggProcMap(GObj* fighter_gobj);
void ftCommonYoshiEggProcTrap(GObj* fighter_gobj);
void ftCommonYoshiEggSetDamageCollCollisions(GObj* fighter_gobj);
void ftCommonYoshiEggProcStatus(GObj* fighter_gobj);
void ftCommonYoshiEggSetStatus(GObj* fighter_gobj);

// CaptureCaptain
void ftCommonCaptureCaptainUpdatePositions(GObj* fighter_gobj, GObj* capture_gobj, Vec3f* pos);
void ftCommonCaptureCaptainProcPhysics(GObj* fighter_gobj);
void ftCommonCaptureCaptainProcCapture(GObj* fighter_gobj, GObj* capture_gobj);
void ftCommonCaptureCaptainRelease(GObj* fighter_gobj);

// Thrown
void ftCommonThrownProcUpdate(GObj* fighter_gobj);
void ftCommonThrownProcPhysics(GObj* fighter_gobj);
void ftCommonThrownProcMap(GObj* fighter_gobj);
void ftCommonThrownSetStatusQueue(GObj* fighter_gobj, s32 status_id_new, s32 status_id_queue);
void ftCommonThrownSetStatusImmediate(GObj* fighter_gobj, s32 status_id);

// Thrown2
void ftCommonThrownReleaseFighterLoseGrip(GObj* fighter_gobj);
void ftCommonThrownDecideFighterLoseGrip(GObj* fighter_gobj, GObj* interact_gobj);
void ftCommonThrownDecideDeadResult(GObj* fighter_gobj);
void ftCommonThrownProcStatus(GObj* fighter_gobj);
void ftCommonThrownReleaseThrownUpdateStats(GObj* fighter_gobj, s32 lr, s32 script_id, sb32 is_proc_status);
void ftCommonThrownUpdateDamageStats(FTStruct* this_fp);
void ftCommonThrownSetStatusDamageRelease(GObj* fighter_gobj);
void ftCommonThrownSetStatusNoDamageRelease(GObj* fighter_gobj);

// Appeal
void ftCommonAppealProcInterrupt(GObj* fighter_gobj);
void ftCommonAppealSetStatus(GObj* fighter_gobj);
sb32 ftCommonAppealCheckInterruptCommon(GObj* fighter_gobj);

// Attack11 / Attack12 / Attack13
void ftCommonAttack11ProcUpdate(GObj* fighter_gobj);
void ftCommonAttack12ProcUpdate(GObj* fighter_gobj);
void ftCommonAttack13ProcUpdate(GObj* fighter_gobj);
void ftCommonAttack11ProcInterrupt(GObj* fighter_gobj);
void ftCommonAttack12ProcInterrupt(GObj* fighter_gobj);
void ftCommonAttack13ProcInterrupt(GObj* fighter_gobj);
void ftCommonAttack11ProcStatus(GObj* fighter_gobj);
void ftCommonAttack11SetStatus(GObj* fighter_gobj);
void ftCommonAttack12SetStatus(GObj* fighter_gobj);
void ftCommonAttack13SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttack1CheckInterruptCommon(GObj* fighter_gobj);
sb32 ftCommonAttack11CheckGoto(GObj* fighter_gobj);
sb32 ftCommonAttack12CheckGoto(GObj* fighter_gobj);
sb32 ftCommonAttack13CheckGoto(GObj* fighter_gobj);

// Attack100Start / Attack100Loop / Attack100End
void ftCommonAttack100StartProcUpdate(GObj* fighter_gobj);
void ftCommonAttack100StartSetStatus(GObj* fighter_gobj);
void ftCommonAttack100LoopKirbyUpdateEffect(FTStruct* fp);
void ftCommonAttack100LoopProcUpdate(GObj* fighter_gobj);
void ftCommonAttack100LoopProcInterrupt(GObj* fighter_gobj);
void ftCommonAttack100LoopSetStatus(GObj* fighter_gobj);
void ftCommonAttack100EndSetStatus(GObj* fighter_gobj);
sb32 ftCommonAttack100StartCheckInterruptCommon(GObj* fighter_gobj);

// AttackDash
void ftCommonAttackDashSetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackDashCheckInterruptCommon(GObj* fighter_gobj);

// AttackS3
void ftCommonAttackS3SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackS3CheckInterruptCommon(GObj* fighter_gobj);

// AttackHi3
void ftCommonAttackHi3SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackHi3CheckInterruptCommon(GObj* fighter_gobj);

// AttackLw3
void ftCommonAttackLw3ProcUpdate(GObj* fighter_gobj);
void ftCommonAttackLw3ProcInterrupt(GObj* fighter_gobj);
sb32 ftCommonAttackLw3CheckInterruptSelf(GObj* fighter_gobj);
void ftCommonAttackLw3InitStatusVars(GObj* fighter_gobj);
void ftCommonAttackLw3SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackLw3CheckInterruptCommon(GObj* fighter_gobj);

// AttackS4
void ftCommonAttackS4ProcUpdate(GObj* fighter_gobj);
void ftCommonAttackS4SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackS4CheckInterruptDash(GObj* fighter_gobj);
sb32 ftCommonAttackS4CheckInterruptTurn(GObj* fighter_gobj);
sb32 ftCommonAttackS4CheckInterruptCommon(GObj* fighter_gobj);

// AttackHi4
void ftCommonAttackHi4SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackHi4CheckInputSuccess(FTStruct* fp);
sb32 ftCommonAttackHi4CheckInterruptMain(FTStruct* fp);
sb32 ftCommonAttackHi4CheckInterruptKneeBend(GObj* fighter_gobj);
sb32 ftCommonAttackHi4CheckInterruptCommon(GObj* fighter_gobj);

// AttackLw4
void ftCommonAttackLw4SetStatus(GObj* fighter_gobj);
sb32 ftCommonAttackLw4CheckInputSuccess(FTStruct* fp);
sb32 ftCommonAttackLw4CheckInterruptMain(FTStruct* fp);
sb32 ftCommonAttackLw4CheckInterruptSquat(GObj* fighter_gobj);
sb32 ftCommonAttackLw4CheckInterruptCommon(GObj* fighter_gobj);

// AttackAirN / AttackAirF / AttackAirB / AttackAirHi / AttackAirLw
void ftCommonAttackAirLwProcHit(GObj* fighter_gobj);
void ftCommonAttackAirLwProcUpdate(GObj* fighter_gobj);
void ftCommonAttackAirProcMap(GObj* fighter_gobj);
sb32 ftCommonAttackAirCheckInterruptCommon(GObj* fighter_gobj);

// LandingAirN / LandingAirF / LandingAirB / LandingAirHi / LandingAirLw
void ftCommonLandingAirSetStatus(GObj* fighter_gobj);

// SpecialN
extern void ftKirbySpecialNSetStatusSelect(GObj* fighter_gobj);
extern sb32 ftCommonSpecialNCheckInterruptCommon(GObj* fighter_gobj);

// SpecialHi
extern sb32 ftCommonSpecialHiCheckInterruptCommon(GObj* fighter_gobj);

// SpecialLw
extern sb32 ftCommonSpecialLwCheckInterruptCommon(GObj* fighter_gobj);

// SpecialAirN / SpecialAirHi / SpecialAirLw
extern void ftKirbySpecialAirNSetStatusSelect(GObj* fighter_gobj);
extern sb32 ftCommonSpecialAirCheckInterruptCommon(GObj* fighter_gobj);

#endif
