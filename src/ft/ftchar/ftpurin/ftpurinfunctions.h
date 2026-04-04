#ifndef _FTPURINFUNCTIONS_H_
#define _FTPURINFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// SpecialN / SpecialAirN
extern void ftPurinSpecialNInitStatusVars(GObj* fighter_gobj);
extern f32 ftPurinSpecialNGetAngle(s32 stick_y);
extern void ftPurinSpecialAirNProcPhysics(GObj* fighter_gobj);
extern void ftPurinSpecialNProcMap(GObj* fighter_gobj);
extern void ftPurinSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftPurinSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftPurinSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftPurinSpecialNSetStatus(GObj* fighter_gobj);
extern void ftPurinSpecialAirNSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftPurinSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftPurinSpecialAirHiProcMap(GObj* fighter_gobj);
extern void ftPurinSpecialHiProcMap(GObj* fighter_gobj);
extern void ftPurinSpecialAirHiSwitchStatusGround(GObj* fighter_gobj);
extern void ftPurinSpecialHiSwitchStatusAir(GObj* fighter_gobj);
extern void ftPurinSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftPurinSpecialAirHiSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftPurinSpecialLwProcMap(GObj* fighter_gobj);
extern void ftPurinSpecialAirLwProcMap(GObj* fighter_gobj);
extern void ftPurinSpecialAirLwSwitchStatusGround(GObj* fighter_gobj);
extern void ftPurinSpecialLwSwitchStatusAir(GObj* fighter_gobj);
extern void ftPurinSpecialLwSetStatus(GObj* fighter_gobj);
extern void ftPurinSpecialAirLwSetStatus(GObj* fighter_gobj);

#endif
