#ifndef _FTYOSHIFUNCTIONS_H_
#define _FTYOSHIFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// JumpAerial
extern void ftYoshiJumpAerialProcPhysics(GObj* fighter_gobj); // Contemplating whether this should be here or in
														 // ftcommonfunctions.h

// SpecialHi / SpecialAirHi
extern void ftYoshiSpecialHiProcDamage(GObj* fighter_gobj);
extern void ftYoshiSpecialHiGetEggPosition(FTStruct* fp, Vec3f* pos);
extern void ftYoshiSpecialHiUpdateEggVectors(FTStruct* fp);
extern void ftYoshiSpecialHiUpdateEggVars(GObj* fighter_gobj);
extern void ftYoshiSpecialHiUpdateEggThrowForce(GObj* fighter_gobj);
extern void ftYoshiSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialAirHiProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftYoshiSpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftYoshiSpecialAirHiSwitchStatusGround(GObj* fighter_gobj);
extern void ftYoshiSpecialHiSwitchStatusAir(GObj* fighter_gobj);
extern void ftYoshiSpecialHiProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialAirHiProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialHiInitStatusVars(GObj* fighter_gobj);
extern void ftYoshiSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirHiSetStatus(GObj* fighter_gobj);

// SpecialN / SpecialAirN
extern void ftYoshiSpecialNSetCatchParams(FTStruct* fp, void (*proc_catch)(GObj*));
extern void ftYoshiSpecialNCatchUpdateProcStatus(GObj* fighter_gobj, void (*proc_status)(GObj*));
extern void ftYoshiSpecialNCatchProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNCatchProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialNCatchUpdateCaptureVars(FTStruct* fp);
extern void ftYoshiSpecialNReleaseProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNReleaseProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialNProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialNCatchProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNCatchProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialNReleaseProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNReleaseProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialNProcStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNProcStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftYoshiSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNCatchSwitchStatusGround(GObj* fighter_gobj);
extern void ftYoshiSpecialNCatchSwitchStatusAir(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNReleaseSwitchStatusGround(GObj* fighter_gobj);
extern void ftYoshiSpecialNReleaseSwitchStatusAir(GObj* fighter_gobj);
extern void ftYoshiSpecialNSetStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNSetStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialNCatchInitStatusVars(GObj* fighter_gobj);
extern void ftYoshiSpecialNCatchProcCatch(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNCatchProcCatch(GObj* fighter_gobj);
extern void ftYoshiSpecialNReleaseInitStatusVars(GObj* fighter_gobj);
extern void ftYoshiSpecialNReleaseSetStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirNReleaseSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftYoshiSpecialLwStartProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialLwLandingProcUpdate(GObj* fighter_gobj);
extern void ftYoshiSpecialAirLwLoopProcPhysics(GObj* fighter_gobj);
extern void ftYoshiSpecialLwStartProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialAirLwLoopProcMap(GObj* fighter_gobj);
extern void ftYoshiSpecialLwStartProcStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialLwStartSetStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirLwStartSetStatus(GObj* fighter_gobj);
extern void ftYoshiSpecialAirLwLoopSetStatus(GObj* fighter_gobj);

#endif
