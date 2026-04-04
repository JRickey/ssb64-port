#ifndef _FTFOXFUNCTIONS_H_
#define _FTFOXFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// SpecialN / SpecialAirN
extern void ftFoxSpecialNProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialNProcInterrupt(GObj* fighter_gobj);
extern void ftFoxSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftFoxSpecialNSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirNSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftFoxSpecialHiStartProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiStartProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiStartProcPhysics(GObj* fighter_gobj);
extern void ftFoxSpecialHiStartProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiStartProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftFoxSpecialHiStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftFoxSpecialHiHoldProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialHiHoldProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiHoldProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiHoldSwitchStatusGround(GObj* fighter_gobj);
extern void ftFoxSpecialHiHoldSwitchStatusAir(GObj* fighter_gobj);
extern void ftFoxSpecialHiHoldInitStatusVars(GObj* fighter_gobj);
extern void ftFoxSpecialHiHoldSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiHoldSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialHiUpdateModelPitch(GObj* fighter_gobj);
extern void ftFoxSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftFoxSpecialHiProcMap(GObj* fighter_gobj);
extern sb32 ftFoxSpecialHiProcPass(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialHiInitStatusVars(FTStruct* fp);
extern void ftFoxSpecialHiDecideSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiSetStatusFromGround(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiEndProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialHiEndProcPhysics(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiEndProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftFoxSpecialHiEndSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiEndSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiBoundProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiBoundProcPhysics(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiBoundProcMap(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiBoundSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialHiStartInitGravity(FTStruct* fp);
extern void ftFoxSpecialHiStartSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirHiStartSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftFoxSpecialLwCheckSetRelease(FTStruct* fp);
extern void ftFoxSpecialLwUpdateEffect(FTStruct* fp);
extern void ftFoxSpecialLwDecReleaseLag(FTStruct* fp);
extern void ftFoxSpecialLwStartProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialAirLwCommonProcPhysics(GObj* fighter_gobj);
extern void ftFoxSpecialLwLoopProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialLwLoopProcInterrupt(GObj* fighter_gobj);
extern void ftFoxSpecialLwLoopSetReflectFlag(GObj* fighter_gobj);
extern void ftFoxSpecialLwLoopSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirLwLoopSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialLwHitDecideSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialLwHitProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialLwHitSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialLwTurnDecTurnFrames(GObj* fighter_gobj);
extern void ftFoxSpecialLwTurnProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialLwTurnInitStatusVars(GObj* fighter_gobj);
extern void ftFoxSpecialLwTurnSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirLwTurnSetStatus(GObj* fighter_gobj);
extern sb32 ftFoxSpecialLwTurnCheckInterruptLoop(GObj* fighter_gobj);
extern void ftFoxSpecialLwEndProcUpdate(GObj* fighter_gobj);
extern void ftFoxSpecialLwEndSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirLwEndSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialLwStartInitStatusVars(GObj* fighter_gobj);
extern void ftFoxSpecialLwStartSetStatus(GObj* fighter_gobj);
extern void ftFoxSpecialAirLwStartSetStatus(GObj* fighter_gobj);

#endif
