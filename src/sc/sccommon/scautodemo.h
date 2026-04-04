#ifndef _SCAUTODEMO_H_
#define _SCAUTODEMO_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern void scAutoDemoFuncUpdate(void);
extern void scAutoDemoStartBattle(void);
extern void scAutoDemoDetectExit(void);
extern void scAutoDemoMakeFade(void);
extern sb32 scAutoDemoCheckStopFocusPlayer(FTStruct *fp);
extern void scAutoDemoSetFocusPlayer1(void);
extern void SCAutoDemoProcFocusPlayer1(void);
extern void scAutoDemoSetFocusPlayer2(void);
extern void SCAutoDemoProcFocusPlayer2(void);
extern void scAutoDemoResetFocusPlayerAll(void);
extern void scAutoDemoSetMagnifyDisplayOn(void);
extern void scAutoDemoExit(void);
extern void scAutoDemoChangeFocus(void);
extern void scAutoDemoUpdateFocus(void);
extern void scAutoDemoFuncRun(GObj *gobj);
extern GObj* scAutoDemoMakeFocusInterface(void);
extern void scAutoDemoGetPlayerStartPosition(s32 mapobj_kind, Vec3f *mapobj_pos);
extern s32 scAutoDemoGetFighterKindsNum(u16 flag);
extern s32 scAutoDemoGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random);
extern s32 scAutoDemoGetPlayerDamage(s32 player);
extern void scAutoDemoInitDemo(void);
extern void scAutoDemoInitSObjs(void);
extern void scAutoDemoFuncStart(void);
extern void scAutoDemoFuncLights(Gfx **dls);
extern void scAutoDemoStartScene(void);

// Other file but it's not worth making another header for this one function
extern void scAutoDemoSetupFiles(void);

#endif
