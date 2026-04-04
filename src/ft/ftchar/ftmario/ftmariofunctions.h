#ifndef _FTMARIOFUNCTIONS_H_
#define _FTMARIOFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// SpecialN / SpecialAirN
extern void ftMarioSpecialNProcUpdate(GObj* fighter_gobj);
extern void ftMarioSpecialNProcAccessory(GObj* fighter_gobj);
extern void ftMarioSpecialNProcMap(GObj* fighter_gobj);
extern void ftMarioSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftMarioSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftMarioSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftMarioSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftMarioSpecialNSetStatus(GObj* fighter_gobj);
extern void ftMarioSpecialAirNSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftMarioSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftMarioSpecialHiProcInterrupt(GObj* fighter_gobj);
extern void ftMarioSpecialHiProcPhysics(GObj* fighter_gobj);
extern sb32 ftMarioSpecialHiProcPass(GObj* fighter_gobj);
extern void ftMarioSpecialHiProcMap(GObj* fighter_gobj);
extern void ftMarioSpecialHiInitStatusVars(GObj* fighter_gobj);
extern void ftMarioSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftMarioSpecialAirHiSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftMarioSpecialLwProcUpdate(GObj* fighter_gobj);
extern void ftMarioSpecialAirLwProcUpdate(GObj* fighter_gobj);
extern f32 ftMarioSpecialLwUpdateFriction(FTStruct* fp, f32 vel);
extern void ftMarioSpecialLwProcPhysics(GObj* fighter_gobj);
extern void ftMarioSpecialAirLwProcPhysics(GObj* fighter_gobj);
extern void ftMarioSpecialLwProcMap(GObj* fighter_gobj);
extern void ftMarioSpecialAirLwProcMap(GObj* fighter_gobj);
extern void ftMarioSpecialAirLwSetDisableRise(GObj* fighter_gobj);
extern void ftMarioSpecialAirLwSwitchStatusGround(GObj* fighter_gobj);
extern void ftMarioSpecialLwSwitchStatusAir(GObj* fighter_gobj);
extern void ftMarioSpecialLwInitStatusVars(GObj* fighter_gobj);
extern void ftMarioSpecialLwSetStatus(GObj* fighter_gobj);
extern void ftMarioSpecialAirLwSetStatus(GObj* fighter_gobj);

#endif
