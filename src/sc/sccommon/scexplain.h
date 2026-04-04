#ifndef _SCEXPLAIN_H_
#define _SCEXPLAIN_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern void scExplainLoadExplainFiles(void);
extern void scExplainSetBattleState(void);
extern void scExplainStartBattle(void);
extern void scExplainMakeCamera(void);
extern void scExplainWindowProcDisplay(GObj *gobj);
extern void scExplainMakeWindowCamera(void);
extern void scExplainSetPlayerInterfacePositions(void);
extern GObj* scExplainMakeTextCamera(void);
extern GObj* scExplainMakeControlStickCamera(void);
extern void scExplainControlStickProcDisplay(GObj *gobj);
extern void scExplainProcUpdateControlStickSprite(GObj *gobj);
extern GObj* scExplainMakeControlStickInterface(void);
extern void scExplainTapSparkProcDisplay(GObj *gobj);
extern void scExplainUpdateTapSparkEffect(void);
extern void scExplainTapSparkProcUpdate(GObj *gobj);
extern GObj* scExplainMakeTapSpark(void);
extern void scExplainSpecialMoveRGBProcUpdate(void);
extern GObj* scExplainMakeSpecialMoveRGB(void);
extern void scExplainSetInterfaceGObjs(void);
extern SObj* scExplainMakeSObjOffset(intptr_t offset);
extern void scExplainSetPhaseSObjs(void);
extern void scExplainUpdateTextBoxSprite(void);
extern void func_ovl63_8018DDBC(void);
extern void scExplainHideTapSpark(void);
extern void scExplainUpdateArgsSObj(SCExplainArgs *args, SObj *sobj);
extern void scExplainDetectExit(void);
extern void scExplainTryMakeFireFlower(void);
extern void scExplainUpdatePhase(void);
extern void scExplainSceneInterfaceProcUpdate(GObj *gobj);
extern GObj* scExplainMakeSceneInterface(void);
extern void scExplainFuncStart(void);
extern void scExplainFuncLights(Gfx **dls);
extern void scExplainFuncUpdate(void);
extern void scExplainFuncDraw(void);
extern void scExplainStartScene(void);
extern void scExplainSetupFiles(void);

#endif
