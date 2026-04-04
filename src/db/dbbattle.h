#ifndef _DBBATTLE_H_
#define _DBBATTLE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <db/dbdef.h>

extern void dbBattleFuncLights(Gfx **dls);
extern void dbBattleStartBattle(void);
extern s32 dbBattleGetSetShade(s32 player);
extern s32 dbBattleGetFighterKindsNum(u16 mask);
extern s32 dbBattleGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random);
extern void dbBattleSetDemoFighterKinds(void);
extern void dbBattleFuncRun(GObj *gobj);
extern void dbBattleActorProcUpdate(GObj *gobj);
extern GObj* dbBattleMakeCamera(void (*thread)(GObj*));
extern void dbBattleFuncStart(void);
extern void dbBattleStartScene(void);

#endif
