#ifndef _FTCAPTAINFUNCTIONS_H_
#define _FTCAPTAINFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// Appear
extern void ftCaptainAppearStartProcUpdate(GObj* fighter_gobj);
extern void ftCaptainAppearEndSetStatus(GObj* fighter_gobj);

// SpecialN / SpecialAirN
extern void ftCaptainSpecialNUpdateEffect(GObj* fighter_gobj);
extern f32 ftCaptainSpecialNGetAngle(s32 stick_y);
extern void ftCaptainSpecialNProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialAirNProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftCaptainSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftCaptainSpecialNProcMap(GObj* fighter_gobj);
extern void ftCaptainSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftCaptainSpecialNInitStatusVars(GObj* fighter_gobj);
extern void func_ovl3_8015FB54();
extern void ftCaptainSpecialNSetStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialAirNSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftCaptainSpecialLwUpdateEffect(GObj* fighter_gobj);
extern void ftCaptainSpecialLwSetAir(GObj* fighter_gobj);
extern void ftCaptainSpecialLwSetGround(GObj* fighter_gobj);
extern void ftCaptainSpecialLwDecideMapCollide(GObj* fighter_gobj);
extern void ftCaptainSpecialLwDecideSetEndStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialLwProcUpdate(GObj* fighter_gobj);
extern void ftCaptainSpecialLwProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialLwLandingProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialAirLwProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialLwBoundProcPhysics(GObj* fighter_gobj);
extern sb32 ftCaptainSpecialLwBoundCheckGoto(GObj* fighter_gobj);
extern sb32 ftCaptainSpecialLwAirCheckAirGoto(GObj* fighter_gobj);
extern void ftCaptainSpecialLwProcMap(GObj* fighter_gobj);
extern void ftCaptainSpecialLwAirProcMap(GObj* fighter_gobj);
extern void ftCaptainSpecialAirLwProcMap(GObj* fighter_gobj);
extern void ftCaptainSpecialLwProcHit(GObj* fighter_gobj);
extern void ftCaptainSpecialLwProcStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialLwAirSetStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialLwLandingSetStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialLwSetStatus(GObj* fighter_gobj);
extern void jtgt_ovl3_801601A0(GObj* fighter_gobj);
extern void ftCaptainSpecialAirLwSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftCaptainSpecialHiSetCatchParams(FTStruct* fp);
extern void ftCaptainSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftCaptainSpecialHiCatchProcUpdate(GObj* fighter_gobj);
extern void ftCaptainSpecialHiProcInterrupt(GObj* fighter_gobj);
extern void ftCaptainSpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialHiCatchProcPhysics(GObj* fighter_gobj);
extern void ftCaptainSpecialHiProcMap(GObj* fighter_gobj);
extern void ftCaptainSpecialHiProcStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialHiProcCatch(GObj* fighter_gobj);
extern void ftCaptainSpecialHiThrowSetStatus(GObj* fighter_gobj);
extern void ftCaptainSpecialAirHiSetStatus(GObj* fighter_gobj);

#endif
