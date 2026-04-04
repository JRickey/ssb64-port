#ifndef _FTLINKFUNCTIONS_H_
#define _FTLINKFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>
#include <wp/wpdef.h>

// SpecialHi / SpecialAirHi
extern void ftLinkSpecialHiDestroyWeapon(FTStruct* fp, WPStruct* wp);
extern void ftLinkSpecialHiUpdateWeaponPos(GObj* fighter_gobj, WPStruct* wp);
extern void ftLinkSpecialHiDecWeaponLifeCheckDestroy(GObj* fighter_gobj, WPStruct* wp);
extern void ftLinkSpecialHiUpdateWeaponAttack(GObj* fighter_gobj, WPStruct* wp);
extern void ftLinkSpecialHiProcEffect(GObj* fighter_gobj);
extern void ftLinkSpecialHiUpdateWeaponVars(GObj* fighter_gobj);
extern void ftLinkSpecialHiMakeWeapon(GObj* fighter_gobj, sb32 is_skip_gobj);
extern void ftLinkSpecialHiProcDamage(GObj* fighter_gobj);
extern void ftLinkSpecialHiProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialHiEndProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialAirHiProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialHiProcPhysics(GObj* fighter_gobj);
extern void ftLinkSpecialAirHiProcPhysics(GObj* fighter_gobj);
extern void ftLinkSpecialHiProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialHiEndProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialAirHiProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialHiProcStatus(GObj* fighter_gobj);
extern void ftLinkSpecialHiSetStatus(GObj* fighter_gobj);
extern void ftLinkSpecialHiEndSetStatus(GObj* fighter_gobj);
extern void ftLinkSpecialAirHiSetStatus(GObj* fighter_gobj);

// SpecialN / SpecialAirN
extern void ftLinkSpecialNDestroyBoomerang(GObj* fighter_gobj);
extern void ftLinkSpecialNMakeBoomerang(GObj* fighter_gobj);
extern void ftLinkSpecialNProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialAirNProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialNProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialNEmptyProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialAirNProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialAirNEmptyProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialNProcStatus(GObj* fighter_gobj);
extern void ftLinkSpecialNSwitchStatusAir(GObj* fighter_gobj);
extern void ftLinkSpecialAirNSwitchStatusGround(GObj* fighter_gobj);
extern void ftLinkSpecialNEmptySwitchStatusAir(GObj* fighter_gobj);
extern void ftLinkSpecialAirNEmptySwitchStatusGround(GObj* fighter_gobj);
extern void ftLinkSpecialNSetStatus(GObj* fighter_gobj);
extern void ftLinkSpecialAirNSetStatus(GObj* fighter_gobj);
extern void ftLinkSpecialNGetSetStatus(GObj* fighter_gobj);

// SpecialLw / SpecialAirLw
extern void ftLinkSpecialLwMakeBomb(GObj* fighter_gobj);
extern void ftLinkSpecialLwProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialAirLwProcUpdate(GObj* fighter_gobj);
extern void ftLinkSpecialLwProcMap(GObj* fighter_gobj);
extern void ftLinkSpecialAirLwProcMap(GObj* fighter_gobj);
extern sb32 ftLinkSpecialLwCheckGotoItemThrow(GObj* fighter_gobj, sb32 is_ground);
extern void ftLinkSpecialLwSetStatus(GObj* fighter_gobj);
extern void ftLinkSpecialAirLwSetStatus(GObj* fighter_gobj);

#endif
