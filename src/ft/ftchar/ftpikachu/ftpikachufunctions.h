#ifndef _FTPIKACHUFUNCTIONS_H_
#define _FTPIKACHUFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>
#include <mp/mpdef.h>

// SpecialN / SpecialAirN
extern void ftPikachuSpecialNProcAccessory(GObj* fighter_gobj);
extern void ftPikachuSpecialNProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialNInitStatusVars(GObj* fighter_gobj);
extern void ftPikachuSpecialNSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirNSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftPikachuSpecialLwMakeThunder(GObj* fighter_gobj);
extern void ftPikachuSpecialLwStartUpdateThunder(GObj* fighter_gobj);
extern void ftPikachuSpecialLwStartProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwStartProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialLwStartProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwStartProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialLwStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialLwStartInitStatusVars(GObj* fighter_gobj);
extern void ftPikachuSpecialLwStartSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwStartSetStatus(GObj* fighter_gobj);
extern sb32 ftPikachuSpecialLwCheckCollideThunder(GObj* fighter_gobj);
extern void ftPikachuSpecialLwLoopProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwLoopProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialLwLoopProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwLoopProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialLwProcDamage(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwLoopSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialLwLoopSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialLwLoopUpdateThunder(GObj* fighter_gobj);
extern void ftPikachuSpecialLwLoopSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwLoopSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialLwHitProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwHitProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwHitProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialLwHitProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwHitProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwHitSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialLwHitSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialLwHitInitStatusVars(GObj* fighter_gobj);
extern void ftPikachuSpecialLwHitSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwHitSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwEndProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialLwEndProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwEndProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwEndSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialLwEndSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialLwClearProcDamage(GObj* fighter_gobj);
extern void ftPikachuSpecialLwEndSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirLwEndSetStatus(GObj* fighter_gobj);

// SpecialHi / SpecialAirHi
extern void ftPikachuSpecialHiStartProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiStartProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiStartProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialHiStartProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiStartProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiStartSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialHiStartSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialHiStartInitStatusVars(GObj* fighter_gobj);
extern void ftPikachuSpecialHiInitMiscVars(GObj* fighter_gobj);
extern void ftPikachuSpecialHiStartSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiStartSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialHiUpdateModelPitchScale(GObj* fighter_gobj);
extern void ftPikachuSpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialHiProcMap(GObj* fighter_gobj);
extern sb32 ftPikachuSpecialHiProcPass(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiSwitchStatusGround(GObj* fighter_gobj);
extern void ftPikachuSpecialHiSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialHiInitStatusVarsZip(GObj* fighter_gobj);
extern void ftPikachuSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiSetStatus(GObj* fighter_gobj);
extern sb32 ftPikachuSpecialHiCheckGotoSubZip(GObj* fighter_gobj);
extern void ftPikachuSpecialHiEndProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiEndProcUpdate(GObj* fighter_gobj);
extern void ftPikachuSpecialHiEndProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiEndProcPhysics(GObj* fighter_gobj);
extern void ftPikachuSpecialHiEndProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiEndProcMap(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiEndSwitchStatusGround(GObj* fighter_gobj); // Unused;
extern void ftPikachuSpecialHiEndSwitchStatusAir(GObj* fighter_gobj);
extern void ftPikachuSpecialHiEndBackupVel(GObj* fighter_gobj);
extern void ftPikachuSpecialHiEndSetStatus(GObj* fighter_gobj);
extern void ftPikachuSpecialAirHiEndSetStatus(GObj* fighter_gobj);

#endif
