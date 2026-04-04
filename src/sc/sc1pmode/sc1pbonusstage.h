#ifndef _SC1PBONUSSTAGE_H_
#define _SC1PBONUSSTAGE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern void *gSC1PBonusStageItemFile;

extern void sc1PBonusStageFuncUpdate(void);
extern void sc1PBonusStageInitVars(void);
extern void sc1PBonusStageBonus1LoadFile(void);
extern void sc1PBonusStageMakeTargets(void);
extern void sc1PBonusStageUpdateTargetInterface(void);
extern void sc1PBonusStageUpdateTargetCount(void);
extern void sc1PBonusStageMakeBonus1Ground(void);
extern void sc1PBonusStageBonus2LoadFile(void);
extern s32 sc1PBonusStageGetPlatformKind(s32 line_id);
extern void sc1PBonusStageInitPlatforms(s32 line_id);
extern void sc1PBonusStageMakePlatforms(void);
extern void sc1PBonusStageUpdatePlatformInterface(void);
extern void sc1PBonusStageUpdatePlatformCount(DObj *dobj);
extern void sc1PBonusStageBonus2ProcUpdate(GObj *ground_gobj);
extern void sc1PBonusStageMakeBonus2Ground(void);
extern void sc1PBonusStageMakeBumpers(void);
extern void sc1PBonusStageInitBonus2(void);
extern void sc1PBonusStageInterfaceThreadUpdate(GObj *interface_gobj);
extern void sc1PBonusStageMakeInterface(void);
extern void sc1PBonusStageInitCamera(void);
extern void sc1PBonusStageMakeTargetSprites(void);
extern void sc1PBonusStageMakePlatformSprites(void);
extern void sc1PBonusStageMakeTaskSprites(void);
extern void sc1PBonusStageGetPlayerStartPosition(Vec3f *pos);
extern void sc1PBonusStageTimerProcUpdate(GObj *interface_gobj);
extern void sc1PBonusStageSetTimeUp(void);
extern void sc1PBonusStageTimeUpProcUpdate(GObj *interface_gobj);
extern void sc1PBonusStageMakeTimeUp(void);
extern void sc1PBonusStageMakeTimer(void);
extern void sc1PBonusStageSetPlayerInterfacePositions(void);
extern void sc1PBonusStageFuncStart(void);
extern void sc1PBonusStageSetBonusStats(s32 tasks_remain);
extern void sc1PBonusStageWriteBackup(sb32 is_tasks_fail, s32 fkind);
extern void sc1PBonusStageFuncLights(Gfx **dls);
extern void sc1PBonusStageStartScene(void);

#endif
