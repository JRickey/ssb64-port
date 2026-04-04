#ifndef _SC1PGAME_H_
#define _SC1PGAME_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

#define SC1PGAME_STAGE_MAX_TEAM_COUNT 30
#define SC1PGAME_STAGE_MAX_VARIATIONS_COUNT 12

#define SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT 6
#define SC1PGAME_STAGE_YOSHI_TEAM_COUNT 18

#define SC1PGAME_STAGE_KIRBY_VARIATIONS_COUNT 7
#define SC1PGAME_STAGE_KIRBY_TEAM_COUNT	8
#define SC1PGAME_STAGE_KIRBY_SIM_COUNT 2				// Number of opponents fought at once

#define SC1PGAME_STAGE_MAX_OPPONENT_COUNT 3

// 0x801936A4
extern sb32 gSC1PGameBonusBrosCalamity;

// 0x801936A8
extern u8 gSC1PGameBonusTomatoCount;

// 0x801936A9
extern u8 gSC1PGameBonusHeartCount;

// 0x801936AA
extern u8 gSC1PGameBonusStarCount;

// 0x801936AB
extern ub8 gSC1PGameBonusShieldBreaker;

// 0x801936AC
extern u8 gSC1PGameBonusGiantImpact;

// 0x801936AD
extern ub8 gSC1PGameBonusMewCatcher;

// 0x801936B0
extern u32 gSC1PGameBonusAttackIDCount[/* */];

// 0x80193798
extern u32 gSC1PGameBonusAttackIsSmashCount[/* */];

// 0x801937A0
extern u32 gSC1PGameBonusAttackGroundAirCount[/* */];

// 0x801937A8
extern u32 gSC1PGameBonusAttackIsProjectileCount[/* */];

// 0x801937B0
extern u32 gSC1PGameBonusDefendIDCount[/* */];

// 0x80193898
extern u32 gSC1PGameBonusDefendIsSmashCount[/* */];

// 0x801938A0
extern u32 gSC1PGameBonusDefendGroundAirCount[/* */];

// 0x801938A8
extern u32 gSC1PGameBonusDefendIsProjectileCount[/* */];

extern void sc1PGameSetupFiles(void);
extern void sc1PGameSetGameStart(void);
extern void sc1PGameSetGameEnd(void);
extern void sc1PGameFuncUpdate(void);
extern s32 sc1PGameGetNextFreePlayerPort(s32 player);
extern s32 sc1PGameGetNextFreeCostume(s32 com);
extern void func_ovl65_8018D38C(void);
extern s32 sc1PGameGetFighterKindsNum(u16 shuf, s32 max);
extern void func_ovl65_8018D3D0(void);
extern void sc1PGameGetStartPosition(Vec3f *mapobj_pos, s32 mapobj_kind);
extern void sc1PGameGetRandomStartPosition(Vec3f *mapobj_pos, s32 mapobj_kind);
extern void sc1PGameSetupEnemyPlayer(SC1PGameStage *stagesetup, SC1PGameComputer *comsetup, s32 player, s32 enemy_player_num);
extern void sc1PGameSetupStageAll(void);
extern void sc1PGameSpawnEnemyTeamNext(GObj *player_gobj);
extern void sc1PGameSetPlayerInterfacePositions(void);
extern void sc1PGameWaitStageCommonUpdate(void);
extern void sc1PGameWaitStageTeamUpdate(void);
extern void sc1PGameWaitStageBonus3Update(void);
extern void sc1PGameWaitStageBossUpdate(void);
extern void sc1PGameWaitThreadUpdate(GObj *gobj);
extern void sc1PGameSetGameStatusWait(void);
extern void sc1PGameTeamStockDisplayProcDisplay(GObj *interface_gobj);
extern void sc1PGameInitTeamStockDisplay(void);
extern void sc1PGameSetPlayerDefeatStats(s32 player, s32 team_order);
extern void sc1PGameTryInitPlayerArrows(void);
extern void sc1PGameInitTimeUpMessage(void);
extern s32 sc1PGameGetEnemyStartLR(s32 this_player);
extern void sc1PGameSetCameraZoom(void);
extern void sc1PGameBossSetCameraZoom(FTStruct *fp);
extern void sc1PGameBossHidePlayerTagAll(void);
extern void sc1PGameBossAddBossInterface(GObj *fighter_gobj, u32 unused);
extern void sc1PGameBossLockPlayerControl(GObj *fighter_gobj, u32 unused);
extern void sc1PGameBossSetIgnorePlayerMapBounds(GObj *fighter_gobj, u32 unused);
extern void func_ovl65_8018F5E4(void);
extern void sc1PGameBossDefeatInterfaceProcUpdate(void);
extern void func_ovl65_8018F6DC(void);
extern void sc1PGameBossDefeatInterfaceProcSet(void);
extern void sc1PGameBossDefeatInitInterface(GObj *fighter_gobj);
extern void sc1PGameFuncStart(void);
extern void sc1PGameAppendBonusStats(void);
extern void sc1PGameInitBonusStats(void);
extern void sc1PGameFuncLights(Gfx **dls);
extern void sc1PGameStartScene(void);

#endif
