#ifndef _FTDONKEYFUNCTIONS_H_
#define _FTDONKEYFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// SpecialN / SpecialAirN
extern void ftDonkeySpecialNProcDamage(GObj* fighter_gobj);
extern void ftDonkeySpecialNStartProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNStartProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialNStartProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeySpecialNStartProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNStartProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftDonkeySpecialNStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftDonkeySpecialNLoopProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialNLoopProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeySpecialNLoopProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNLoopProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialNLoopSetProcDamageAnimSpeed(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNLoopSwitchStatusGround(GObj* fighter_gobj);
extern void ftDonkeySpecialNLoopSwitchStatusAir(GObj* fighter_gobj);
extern void ftDonkeySpecialNLoopSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNLoopSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialNEndProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNEndProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftDonkeySpecialNGetStatusChargeLevelReset(GObj* fighter_gobj);
extern void ftDonkeySpecialNEndSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNEndSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftDonkeySpecialNStartSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialAirNStartSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftDonkeySpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialAirHiProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftDonkeySpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftDonkeySpecialHiProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialAirHiProcMap(GObj* fighter_gobj);
extern void ftDonkeySpecialAirHiSwitchStatusGround(GObj* fighter_gobj);
extern void ftDonkeySpecialHiSwitchStatusAir(GObj* fighter_gobj);
extern void ftDonkeySpecialHiSetStatusFlagGA(GObj* fighter_gobj, sb32 ga);
extern void ftDonkeySpecialHiSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialAirHiSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftDonkeySpecialLwStartProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialLwLoopProcUpdate(GObj* fighter_gobj);
extern void ftDonkeySpecialLwLoopProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeySpecialLwLoopSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialLwEndSetStatus(GObj* fighter_gobj);
extern void ftDonkeySpecialLwStartSetStatus(GObj* fighter_gobj);

/* The ThrowF functions are grouped with the Common action states, but I felt
 * like it would be cleaner if I declared them here instead.
 * After all, only DK uses them.
 */

// ThrowFWait
extern void ftDonkeyThrowFWaitProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeyThrowFCommonProcMap(GObj* fighter_gobj);
extern void ftDonkeyThrowFWaitSetStatus(GObj* fighter_gobj);
extern sb32 ftDonkeyThrowFWaitCheckInterruptThrowFWalk(GObj* fighter_gobj);

// ThrowFWalk
extern f32 ftDonkeyThrowFWalkGetWalkAnimLength(FTStruct* fp, s32 status_id);
extern void ftDonkeyThrowFWalkProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeyThrowFWalkSetStatusParam(GObj* fighter_gobj, f32 frame_begin);
extern void ftDonkeyThrowFWalkSetStatusDefault(GObj* fighter_gobj);
extern sb32 ftDonkeyThrowFWalkCheckInterruptThrowFWait(GObj* fighter_gobj);

// ThrowFTurn
extern void ftDonkeyThrowFTurnProcUpdate(GObj* fighter_gobj);
extern void ftDonkeyThrowFTurnProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeyThrowFTurnSetStatus(GObj* fighter_gobj);
extern sb32 ftDonkeyThrowFTurnCheckInterruptThrowFCommon(GObj* fighter_gobj);

// ThrowFKneeBend
extern void ftDonkeyThrowFKneeBendProcUpdate(GObj* fighter_gobj);
extern void ftDonkeyThrowFKneeBendProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeyThrowFKneeBendSetStatus(GObj* fighter_gobj, s32 input_source);
extern sb32 ftDonkeyThrowFKneeBendCheckInterruptThrowFCommon(GObj* fighter_gobj);

// ThrowFFall
extern void ftDonkeyThrowFFallProcInterrupt(GObj* fighter_gobj);
extern void ftDonkeyThrowFFallProcMap(GObj* fighter_gobj);
extern void ftDonkeyThrowFFallSetStatus(GObj* fighter_gobj);
extern void ftDonkeyThrowFJumpSetStatus(GObj* fighter_gobj);
extern void ftDonkeyThrowFFallSetStatusPass(GObj* fighter_gobj);
extern sb32 ftDonkeyThrowFFallCheckInterruptPass(GObj* fighter_gobj);

// ThrowFLanding
extern void ftDonkeyThrowFLandingProcUpdate(GObj* fighter_gobj);
extern void ftDonkeyThrowFLandingSetStatus(GObj* fighter_gobj);

// ThrowFDamage
extern void ftDonkeyThrowFDamageProcUpdate(GObj* fighter_gobj);
extern void ftDonkeyThrowFDamageSetStatus(GObj* fighter_gobj);

// ThrowFF / ThrowAirFF
extern void ftDonkeyThrowFFProcUpdate(GObj* fighter_gobj);
extern void ftDonkeyThrowAirFFSwitchStatusGround(GObj* fighter_gobj);
extern void ftDonkeyThrowFFSwitchStatusAir(GObj* fighter_gobj);
extern void ftDonkeyThrowFFProcMap(GObj* fighter_gobj);
extern void ftDonkeyThrowAirFFProcMap(GObj* fighter_gobj);
extern void ftDonkeyThrowFFSetStatus(GObj* fighter_gobj, sb32 is_turn);
extern sb32 ftDonkeyThrowFFCheckInterruptThrowFCommon(GObj* fighter_gobj);

#endif
