#ifndef _FTBOSSFUNCTIONS_H_
#define _FTBOSSFUNCTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

// Common
extern void ftBossCommonInvertLR(GObj* fighter_gobj);
extern void ftBossCommonCheckEdgeInvertLR(GObj* fighter_gobj);
extern void ftBossCommonCheckPlayerInvertLR(GObj* fighter_gobj);
extern void ftBossCommonRandEdgeLR(s32 line_id, Vec3f* pos);
extern void ftBossCommonGotoTargetEdge(GObj* fighter_gobj, Vec3f* pos);
extern void ftBossCommonSetPosOffsetY(GObj* fighter_gobj, Vec3f* pos, f32 off_y);
extern void ftBossCommonSetPosAddVelPlayer(GObj* fighter_gobj, Vec3f* pos, f32 vel_x, f32 vel_y);
extern void ftBossCommonSetPosAddVelAuto(GObj* fighter_gobj, Vec3f* pos, f32 vel_x, f32 vel_y);
extern void ftBossCommonGetPositionCenter(s32 var, Vec3f* pos_input);
extern void ftBossCommonSetNextAttackWait(GObj* fighter_gobj);
extern void ftBossCommonUpdateFogColor(GObj* fighter_gobj);
extern void ftBossCommonSetUseFogColor(GObj* fighter_gobj);
extern void ftBossCommonSetDisableFogColor(GObj* fighter_gobj);
extern void ftBossCommonSetDefaultLineID(GObj* fighter_gobj);
extern void ftBossCommonUpdateDamageStats(GObj* fighter_gobj);

// Unk
extern void ftBossDefaultProcInterrupt(GObj* fighter_gobj);
extern void ftBossDefaultSetStatus(GObj* fighter_gobj); // Unused

// Wait
extern void ftBossWaitSetVelStickRange(GObj* fighter_gobj);
extern void ftBossWaitDecideStatusPlayer(GObj* fighter_gobj);
extern void ftBossWaitDecideStatusComputer(GObj* fighter_gobj);
extern void ftBossWaitProcInterrupt(GObj* fighter_gobj);
extern void ftBossWaitProcPhysics(GObj* fighter_gobj);
extern void ftBossWaitSetStatus(GObj* fighter_gobj);

// Move
extern void ftBossMoveProcPhysics(GObj* fighter_gobj);
extern void ftBossMoveProcMap(GObj* fighter_gobj);
extern void ftBossMoveSetStatus(GObj* fighter_gobj, void (*proc_setstatus)(GObj*), Vec3f* vel);

// Hippataku
extern void ftBossHippatakuProcUpdate(GObj* fighter_gobj);
extern void ftBossHippatakuSetStatus(GObj* fighter_gobj);

// Harau
extern void ftBossHarauResetStatus(GObj* fighter_gobj);
extern void ftBossHarauProcUpdate(GObj* fighter_gobj);
extern void ftBossHarauProcPhysics(GObj* fighter_gobj);
extern void ftBossHarauSetStatus(GObj* fighter_gobj);

// Okuhikouki1
extern void ftBossOkuhikouki1ProcUpdate(GObj* fighter_gobj);
extern void ftBossOkuhikouki1SetStatus(GObj* fighter_gobj);

// Okuhikouki2
extern void ftBossOkuhikouki2ProcUpdate(GObj* fighter_gobj);
extern void ftBossOkuhikouki2ProcPhysics(GObj* fighter_gobj);
extern void ftBossOkuhikouki2SetStatus(GObj* fighter_gobj);

// Okuhikouki3
extern void ftBossOkuhikouki3ProcUpdate(GObj* fighter_gobj);
extern void ftBossOkuhikouki3SetStatus(GObj* fighter_gobj);

// Walk
extern void ftBossWalkProcUpdate(GObj* fighter_gobj);
extern void ftBossWalkSetStatus(GObj* fighter_gobj);

// WalkLoop
extern sb32 ftBossWalkLoopCheckPlayerInRange(GObj* fighter_gobj);
extern void ftBossWalkLoopProcPhysics(GObj* fighter_gobj);
extern void ftBossWalkLoopProcMap(GObj* fighter_gobj);
extern void ftBossWalkLoopSetStatus(GObj* fighter_gobj);

// WalkWait
extern void ftBossWalkWaitProcUpdate(GObj* fighter_gobj);
extern void ftBossWalkWaitSetStatus(GObj* fighter_gobj);

// WalkShoot
extern void ftBossWalkShootProcUpdate(GObj* fighter_gobj);
extern void ftBossWalkShootSetStatus(GObj* fighter_gobj);

// GootsubusuUp
extern void ftBossGootsubusuUpProcPhysics(GObj* fighter_gobj);
extern void ftBossGootsubusuUpProcMap(GObj* fighter_gobj);
extern void ftBossGootsubusuUpSetStatus(GObj* fighter_gobj);

// GootsubusuWait
extern void ftBossGootsubusuWaitProcPhysics(GObj* fighter_gobj);
extern void ftBossGootsubusuWaitProcMap(GObj* fighter_gobj);
extern void ftBossGootsubusuWaitSetStatus(GObj* fighter_gobj);

// GootsubusuEnd
extern void ftBossGootsubusuEndProcUpdate(GObj* fighter_gobj);
extern void ftBossGootsubusuEndSetStatus(GObj* fighter_gobj);

// GootsubusuDown
extern void ftBossGootsubusuDownProcMap(GObj* fighter_gobj);
extern void ftBossGootsubusuDownSetStatus(GObj* fighter_gobj);

// Tsutsuku1
extern void ftBossTsutsuku1ProcUpdate(GObj* fighter_gobj);
extern void ftBossTsutsuku1SetStatus(GObj* fighter_gobj);

// Tsutsuku2
extern void ftBossTsutsuku2ProcPhysics(GObj* fighter_gobj);
extern void ftBossTsutsuku2SetStatus(GObj* fighter_gobj);

// Tsutsuku3
extern void ftBossTsutsuku3ProcUpdate(GObj* fighter_gobj);
extern void ftBossTsutsuku3SetStatus(GObj* fighter_gobj);

// Drill
extern void ftBossDrillProcUpdate(GObj* fighter_gobj);
extern void ftBossDrillProcPhysics(GObj* fighter_gobj);
extern void ftBossDrillProcPhysicsFollow(GObj* fighter_gobj);
extern void ftBossDrillProcMap(GObj* fighter_gobj);
extern void ftBossDrillSetStatus(GObj* fighter_gobj);

// Okukouki
extern void ftBossOkukoukiProcUpdate(GObj* fighter_gobj);
extern void ftBossOkukoukiSetStatus(GObj* fighter_gobj);

// Yubideppou1
extern void ftBossYubideppou1ProcUpdate(GObj* fighter_gobj);
extern void ftBossYubideppou1SetStatus(GObj* fighter_gobj);

// Yubideppou2
extern void ftBossYubideppou2UpdatePosition(GObj* fighter_gobj);
extern void ftBossYubideppou2ProcPhysics(GObj* fighter_gobj);
extern void ftBossYubideppou2SetStatus(GObj* fighter_gobj);

// Yubideppou3
extern void ftBossYubideppou3ProcUpdate(GObj* fighter_gobj);
extern void ftBossYubideppou3ProcPhysics(GObj* fighter_gobj);
extern void ftBossYubideppou3SetStatus(GObj* fighter_gobj);

// Okupunch1
extern void ftBossOkupunch1ProcUpdate(GObj* fighter_gobj);
extern void ftBossOkupunch1SetStatus(GObj* fighter_gobj);

// Okupunch2
extern void ftBossOkupunch2ProcUpdate(GObj* fighter_gobj);
extern void ftBossOkupunch2ProcPhysics(GObj* fighter_gobj);
extern void ftBossOkupunch2SetStatus(GObj* fighter_gobj);

// Okupunch3
extern void ftBossOkupunch3ProcUpdate(GObj* fighter_gobj);
extern void ftBossOkupunch3SetStatus(GObj* fighter_gobj);

// Okutsubushi
extern void ftBossOkutsubushiProcUpdate(GObj* fighter_gobj);
extern void ftBossOkutsubushiProcPhysics(GObj* fighter_gobj);
extern void ftBossOkutsubushiSetStatus(GObj* fighter_gobj);

// OkutsubushiStart
extern void ftBossOkutsubushiStartProcUpdate(GObj* fighter_gobj);
extern void ftBossOkutsubushiStartSetStatus(GObj* fighter_gobj);

// DeadLeft
extern void ftBossDeadLeftProcUpdate(GObj* fighter_gobj);
extern void ftBossDeadLeftSetStatus(GObj* fighter_gobj);

// DeadCenter?
extern void ftBossDeadCenterProcPhysics(GObj* fighter_gobj);
extern void ftBossDeadCenterSetStatus(GObj* fighter_gobj);

// DeadRight?
extern void ftBossDeadRightSetStatus(GObj* fighter_gobj);

// Appear
extern void ftBossAppearProcPhysics(GObj* fighter_gobj);

#endif
