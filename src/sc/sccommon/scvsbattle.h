#ifndef _SCVSBATTLE_H_
#define _SCVSBATTLE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern void scVSBattleFuncUpdate(void);
extern s32 scVSBattleGetStartPlayerLR(s32 this_player);
extern void scVSBattleStartBattle(void);
extern sb32 scVSBattleSetScoreCheckSuddenDeath(void);
extern void scVSBattleStartSuddenDeath(void);
extern void scVSBattleFuncLights(Gfx **dls);
extern void scVSBattleStartScene(void);
extern void scVSBattleSetupFiles(void);

#endif
