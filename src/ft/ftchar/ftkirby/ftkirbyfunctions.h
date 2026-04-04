#ifndef _FTKIRBYFUNCTIONS_H_
#define _FTKIRBYFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// ThrowF
extern void ftKirbyThrowFProcUpdate(GObj *fighter_gobj);
extern void ftKirbyThrowFProcMap(GObj *fighter_gobj);
extern void ftKirbyThrowFFallSetStatus(GObj *fighter_gobj);
extern void ftKirbyThrowFLandingProcPhysics(GObj *fighter_gobj);
extern void ftKirbyThrowFLandingProcMap(GObj *fighter_gobj);
extern void ftKirbyThrowFLandingSetStatus(GObj *fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftKirbySpecialHiUpdateEffect(GObj* fighter_gobj);
extern void ftKirbySpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialHiLandingProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftKirbySpecialHiLandingProcPhysics(GObj* fighter_gobj);
extern void ftKirbySpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftKirbySpecialAirHiFallProcPhysics(GObj* fighter_gobj);
extern void ftKirbySpecialHiProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirHiFallProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialHiProcStatus(GObj* fighter_gobj);
extern void ftKirbySpecialHiSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialHiLandingSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirHiSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirHiFallSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftKirbySpecialLwUnused(GObj* fighter_gobj);
extern void ftKirbySpecialLwUpdateColAnim(GObj* fighter_gobj);
extern void ftKirbySpecialLwSetDamageResist(GObj* fighter_gobj);
extern void ftKirbySpecialLwSetDropFallVel(FTStruct* fp);
extern f32 ftKirbySpecialLwGetGroundAxisYaw(FTStruct* fp);
extern sb32 ftKirbySpecialLwCheckRelease(GObj* fighter_gobj, sb32 is_allow_release);
extern void ftKirbySpecialLwStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialLwUnkDecideNextStatus(GObj* fighter_gobj, sb32 ga);
extern void ftKirbySpecialLwUnkProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwHoldProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialLwHoldDecideNextStatus(GObj* fighter_gobj, sb32 ga);
extern void ftKirbySpecialLwHoldProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwFallProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialLwHoldProcPhysics(GObj* fighter_gobj);
extern void ftKirbySpecialLwStartProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialLwUnkProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialLwHoldProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwStartProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwHoldProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialLwStartSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialLwUnkSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialLwEndSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwStartSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwHoldSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirLwEndSetStatus(GObj* fighter_gobj);

// SpecialN / SpecialAirN
extern void ftKirbySpecialNApplyCaptureDamage(GObj* kirby_gobj, GObj* victim_gobj, s32 damage);
extern f32 ftKirbySpecialNGetCaptureDistance(Vec3f* kirby_pos, Vec3f* victim_pos);
extern void ftKirbySpecialNAddCaptureDistance(FTStruct* fp, GObj* fighter_gobj, Vec3f* pos);
extern void ftKirbySpecialNInitStatusVars(GObj* fighter_gobj, sb32 unused);
extern void ftKirbySpecialNSetCatchParams(FTStruct* fp);
extern void ftKirbySpecialNGotoSetCatchParams(GObj* fighter_gobj);
extern void ftKirbySpecialNInitPassiveVars(FTStruct* fp);
extern void ftKirbySpecialNCopyInitCopyVars(GObj* fighter_gobj);
extern void ftKirbySpecialNStopEffect(GObj* fighter_gobj);
extern void ftKirbySpecialNStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialNLoopProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialNCatchProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirNStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialNTurnProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirNTurnProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialNEatProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialNThrowUpdateCheckThrowStar(GObj* fighter_gobj);
extern void ftKirbySpecialNCopyUpdateCheckCopyStar(GObj* fighter_gobj);
extern void ftKirbySpecialNThrowProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirNThrowProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialNCopyProcUpdate(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCopyProcUpdate(GObj* fighter_gobj);
extern sb32 ftKirbySpecialNLoopCheckContinueLoop(GObj* fighter_gobj);
extern void ftKirbySpecialNLoopProcInterrupt(GObj* fighter_gobj);
extern void ftKirbySpecialAirNLoopProcInterrupt(GObj* fighter_gobj);
extern sb32 ftKirbySpecialNThrowCheckGotoThrow(GObj* fighter_gobj, void (*proc_status)(GObj*));
extern sb32 ftKirbySpecialNCopyCheckGotoCopy(GObj* fighter_gobj, void (*proc_status)(GObj*));
extern sb32 ftKirbySpecialNTurnCheckGotoTurn(GObj* fighter_gobj, void (*proc_status)(GObj*));
extern void ftKirbySpecialNWaitProcInterrupt(GObj* fighter_gobj);
extern void ftKirbySpecialAirNWaitProcInterrupt(GObj* fighter_gobj);
extern void ftKirbySpecialAirNWaitProcPhysics(GObj* fighter_gobj);
extern void ftKirbySpecialNStartProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNLoopProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNEndProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNCatchProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNEatProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNThrowProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNWaitProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNTurnProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialNCopyProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNStartProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNLoopProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNEndProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCatchProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNEatProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNThrowProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNWaitProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNTurnProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCopyProcMap(GObj* fighter_gobj);
extern void ftKirbySpecialAirNStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialAirNLoopSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialAirNEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialNStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialNLoopSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialNEndSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialNEatSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialNCatchSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCatchSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialAirNEatSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialNThrowSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialAirNThrowSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialNWaitSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialAirNWaitSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialNTurnSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialAirNTurnSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialNCopySwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCopySwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbySpecialNWaitUpdateLR(GObj* fighter_gobj);
extern void ftKirbySpecialNWaitSetStatusFromTurn(GObj* fighter_gobj);
extern void ftKirbySpecialAirNWaitSetStatusFromTurn(GObj* fighter_gobj);
extern void ftKirbySpecialNEatSetStatusParam(GObj* fighter_gobj, s32 status_id);
extern void ftKirbySpecialNCatchEatSetStatusParam(GObj* fighter_gobj, s32 status_id);
extern void ftKirbySpecialNStartSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNLoopSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNCatchProcCatch(GObj* fighter_gobj);
extern void ftKirbySpecialNEatSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNThrowSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNWaitSetStatusFromEat(GObj* fighter_gobj);
extern void ftKirbySpecialNTurnSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNCopySetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNEndSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNStartSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNLoopSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCatchProcCatch(GObj* fighter_gobj);
extern void ftKirbySpecialAirNEatSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNThrowSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNWaitSetStatusFromEat(GObj* fighter_gobj);
extern void ftKirbySpecialAirNTurnSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNCopySetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialAirNEndSetStatus(GObj* fighter_gobj);
extern void ftKirbySpecialNLoseCopy(GObj* fighter_gobj);
extern void ftKirbySpecialNDamageCheckLoseCopy(GObj* fighter_gobj);

// CopyMarioSpecialN
extern void ftKirbyCopyMarioSpecialNProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialNProcAccessory(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyMarioSpecialAirNSetStatus(GObj* fighter_gobj);

// CopyFoxSpecialN
extern void ftKirbyCopyFoxSpecialNProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyFoxSpecialNProcInterrupt(GObj* fighter_gobj);
extern void ftKirbyCopyFoxSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyFoxSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyFoxSpecialAirNSetStatus(GObj* fighter_gobj);

// CopySamusSpecialN
extern void ftKirbyCopySamusSpecialNDestroyChargeShot(FTStruct* fp);
extern void ftKirbyCopySamusSpecialNProcDamage(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNGetChargeShotPosition(FTStruct* fp, Vec3f* pos);
extern void ftKirbyCopySamusSpecialNSetChargeShotPosition(FTStruct* fp);
extern void ftKirbyCopySamusSpecialNStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNStartProcInterrupt(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNStartProcMap(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialAirNStartProcMap(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialAirNStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNLoopProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNLoopProcInterrupt(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNLoopProcMap(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNLoopSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNEndProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNEndProcMap(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialAirNEndProcMap(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialAirNEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNEndSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialNEndSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialAirNEndSetStatus(GObj* fighter_gobj);
extern f32 ftKirbyCopySamusSpecialNStartGetAnimSpeed(FTStruct* fp);
extern void ftKirbyCopySamusSpecialNInitStatusVars(FTStruct* fp);
extern void ftKirbyCopySamusSpecialNStartSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopySamusSpecialAirNStartSetStatus(GObj* fighter_gobj);

// CopyDonkeySpecialN
extern void ftKirbyCopyDonkeySpecialNProcDamage(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNStartProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNStartProcInterrupt(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNStartProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNStartProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNLoopProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNLoopProcInterrupt(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNLoopProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNLoopProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNLoopSetProcDamageAnimSpeed(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNLoopSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNLoopSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNLoopSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNLoopSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNEndProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNEndProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNGetStatusChargeLevelReset(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNEndSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNEndSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialNStartSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyDonkeySpecialAirNStartSetStatus(GObj* fighter_gobj);

// CopyPikachuSpecialN
extern void ftKirbyCopyPikachuSpecialNProcAccessory(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyPikachuSpecialAirNSetStatus(GObj* fighter_gobj);

// CopyNessSpecialN
extern void ftKirbyCopyNessSpecialNProcAccessory(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyNessSpecialAirNSetStatus(GObj* fighter_gobj);

// CopyLinkSpecialN
extern void ftKirbyCopyLinkSpecialNDestroyBoomerang(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNMakeBoomerang(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialAirNProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNEmptyProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialAirNEmptyProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNProcStatus(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNEmptySwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialAirNEmptySwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialAirNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyLinkSpecialNGetSetStatus(GObj* fighter_gobj);

// CopyPurinSpecialN
extern void ftKirbyCopyPurinSpecialNInitStatusVars(GObj* fighter_gobj);
extern f32 ftKirbyCopyPurinSpecialNGetAngle(s32 stick_y);
extern void ftKirbyCopyPurinSpecialAirNProcPhysics(GObj* fighter_gobj);
extern void ftKirbyCopyPurinSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyPurinSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyPurinSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyPurinSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyPurinSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyPurinSpecialAirNSetStatus(GObj* fighter_gobj);

// CopyCaptainSpecialN
extern void ftKirbyCopyCaptainSpecialNUpdateEffect(GObj* fighter_gobj);
extern f32 ftKirbyCopyCaptainSpecialNGetAngle(s32 stick_y);
extern void ftKirbyCopyCaptainSpecialNProcPhysics(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialAirNProcPhysics(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialNInitStatusVars(GObj* fighter_gobj);
extern void func_ovl3_80160B28(); // Unused
extern void ftKirbyCopyCaptainSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyCaptainSpecialAirNSetStatus(GObj* fighter_gobj);

// CopyYoshiSpecialN
extern void ftKirbyCopyYoshiSpecialNSetCatchParams(FTStruct* fp, void (*proc_catch)(GObj*));
extern void ftKirbyCopyYoshiSpecialNCatchUpdateProcStatus(GObj* fighter_gobj, void (*proc_status)(GObj*));
extern void ftKirbyCopyYoshiSpecialNCatchProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNCatchProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNCatchUpdateCaptureVars(FTStruct* fp);
extern void ftKirbyCopyYoshiSpecialNReleaseProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNReleaseProcUpdate(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNCatchProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNCatchProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNReleaseProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNReleaseProcMap(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNProcStatus(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNProcStatus(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNCatchSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNCatchSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNReleaseSwitchStatusGround(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNReleaseSwitchStatusAir(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNCatchInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNCatchProcCatch(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNCatchProcCatch(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNReleaseInitStatusVars(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialNReleaseSetStatus(GObj* fighter_gobj);
extern void ftKirbyCopyYoshiSpecialAirNReleaseSetStatus(GObj* fighter_gobj);

#endif
