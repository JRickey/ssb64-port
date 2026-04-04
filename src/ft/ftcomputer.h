#ifndef _FTCOMPUTER_H_
#define _FTCOMPUTER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>
#include <wp/wpdef.h>

extern WPStruct* ftComputerGetOwnWeapon(FTStruct *fp);
extern Vec3f* ftComputerGetOwnWeaponPositionKind(FTStruct *fp, s32 kind);
extern void ftComputerSetControlPKThunder(FTStruct *fp);
extern void ftComputerUpdateInputs(FTStruct *this_fp);
extern void ftComputerSetCommandWaitShort(FTStruct *fp, s32 index);
extern void ftComputerSetCommandImmediate(FTStruct *fp, s32 index);
extern void ftComputerSetCommandWaitLong(FTStruct *fp, s32 index);
extern sb32 ftComputerCheckFindTarget(FTStruct *this_fp);
extern sb32 ftComputerCheckEvadeDistance(FTStruct *this_fp);
extern FTStruct* ftComputerWaitGetTarget(FTStruct *this_fp);
extern void func_ovl3_80132EC0(void);
extern sb32 ftComputerCheckDetectTarget(FTStruct *this_fp, f32 detect_range_base);
extern sb32 ftComputerCheckSetTargetEdgeRight(FTStruct *fp, sb32 is_find_edge_target);
extern sb32 ftComputerCheckSetTargetEdgeLeft(FTStruct *fp, sb32 is_find_edge_target);
extern void func_ovl3_801346D4(FTStruct *fp);
extern void func_ovl3_80134964(FTStruct *fp);
extern sb32 ftComputerCheckTargetItemOrTwister(FTStruct *fp);
extern void ftComputerFollowObjectiveWalk(FTStruct *fp);
extern sb32 func_ovl3_80135B78(FTStruct *this_fp);
extern void func_ovl3_801361BC(FTStruct *fp);
extern sb32 ftComputerCheckFindItem(FTStruct *fp);
extern sb32 ftComputerCheckTargetItemInRange(FTStruct *fp);
extern sb32 ftComputerCheckSetEvadeTarget(FTStruct *this_fp);
extern sb32 ftComputerCheckTryChargeSpecialN(FTStruct *fp);
extern sb32 ftComputerCheckTryCancelSpecialN(FTStruct *fp);
extern s32 ftComputerGetObjectiveStatus(GObj *this_gobj);
extern s32 ftComputerProcDefault(GObj *fighter_gobj);
extern s32 ftComputerProcStand(GObj *fighter_gobj);
extern s32 ftComputerProcWalk(GObj *fighter_gobj);
extern s32 ftComputerProcEvade(GObj *fighter_gobj);
extern s32 ftComputerProcJump(GObj *fighter_gobj);
extern s32 func_ovl3_80137E70(GObj *fighter_gobj);
extern void ftComputerFollowObjectiveRecover(FTStruct *fp);
extern void ftComputerFollowObjectiveCounterAttack(FTStruct *fp);
extern void ftComputerFollowObjectiveTrackItem(FTStruct *fp);
extern void ftComputerFollowObjectiveUseItem(FTStruct *fp);
extern void ftComputerFollowObjectiveEvade(FTStruct *fp);
extern s32 func_ovl3_8013837C(FTStruct *this_fp);
extern void func_ovl3_8013877C(FTStruct *this_fp);
extern sb32 func_ovl3_80138AA8(FTStruct *this_fp, sb32 is_delay);
extern sb32 func_ovl3_80138EE4(FTStruct *fp);
extern void ftComputerFollowObjectiveAttack(FTStruct *fp);
extern void func_ovl3_801397F4(FTStruct *fp);
extern void ftComputerFollowObjectiveAlly(FTStruct *fp);
extern void ftComputerFollowObjectivePatrol(FTStruct *fp);
extern void ftComputerFollowObjectiveRush(FTStruct *fp);
extern void ftComputerProcessObjective(FTStruct *fp);
extern void ftComputerProcessBehavior(FTStruct *fp);
extern void ftComputerProcessTrait(FTStruct *fp);
extern void ftComputerProcessAll(GObj *fighter_gobj);
extern void ftComputerSetupAll(GObj *fighter_gobj);
extern void ftComputerSetFighterDamageDetectSize(GObj *fighter_gobj);

#endif
