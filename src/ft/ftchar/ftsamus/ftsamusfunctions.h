#ifndef _FTSAMUSFUNCTIONS_H_
#define _FTSAMUSFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// SpecialN / SpecialAirN
extern void ftSamusSpecialNDestroyChargeShot(FTStruct* fp);
extern void ftSamusSpecialNProcDamage(GObj* fighter_gobj);
extern void ftSamusSpecialNGetChargeShotPosition(FTStruct* fp, Vec3f* pos);
extern void ftSamusSpecialNSetChargeShotPosition(FTStruct* fp);
extern void ftSamusSpecialNStartProcUpdate(GObj* fighter_gobj);
extern void ftSamusSpecialNStartProcInterrupt(GObj* fighter_gobj);
extern void ftSamusSpecialNStartProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialAirNStartProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialAirNStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftSamusSpecialNStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftSamusSpecialNLoopProcUpdate(GObj* fighter_gobj);
extern void ftSamusSpecialNLoopProcInterrupt(GObj* fighter_gobj);
extern void ftSamusSpecialNLoopProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialNLoopSetStatus(GObj* fighter_gobj);
extern void ftSamusSpecialNEndProcUpdate(GObj* fighter_gobj);
extern void ftSamusSpecialNEndProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialAirNEndProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialAirNEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftSamusSpecialNEndSwitchStatusAir(GObj* fighter_gobj);
extern void ftSamusSpecialNEndSetStatus(GObj* fighter_gobj);
extern void ftSamusSpecialAirNEndSetStatus(GObj* fighter_gobj);
extern f32 ftSamusSpecialNStartGetAnimSpeed(FTStruct* fp);
extern void ftSamusSpecialNStartInitStatusVars(FTStruct* fp);
extern void ftSamusSpecialNStartSetStatus(GObj* fighter_gobj);
extern void ftSamusSpecialAirNStartSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftSamusSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftSamusSpecialHiProcPhysics(GObj* fighter_gobj);
extern sb32 ftSamusSpecialHiProcPass(GObj* fighter_gobj);
extern void ftSamusSpecialHiProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftSamusSpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftSamusSpecialAirHiSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftSamusSpecialLwMakeBomb(GObj* fighter_gobj);
extern void ftSamusSpecialLwProcUpdate(GObj* fighter_gobj);
extern void ftSamusSpecialAirLwProcUpdate(GObj* fighter_gobj);
extern void ftSamusSpecialLwProcPhysics(GObj* fighter_gobj);
extern void ftSamusSpecialAirLwProcPhysics(GObj* fighter_gobj);
extern void ftSamusSpecialLwProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialAirLwProcMap(GObj* fighter_gobj);
extern void ftSamusSpecialAirLwSwitchStatusGround(GObj* fighter_gobj);
extern void ftSamusSpecialLwTransferStatusAir(GObj* fighter_gobj);
extern void ftSamusSpecialLwSwitchStatusAir(GObj* fighter_gobj);
extern void ftSamusSpecialLwInitStatusVars(FTStruct* fp);
extern void ftSamusSpecialLwSetStatus(GObj* fighter_gobj);
extern void ftSamusSpecialAirLwSetStatus(GObj* fighter_gobj);

#endif
