#ifndef _MVENDING_H_
#define _MVENDING_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvEndingFuncLights(Gfx **dls);
extern void mvEndingMakeRoomBackground(void);
extern void mvEndingMakeRoomDesk(void);
extern void mvEndingMakeRoomBooks(void);
extern void mvEndingMakeRoomPencils(void);
extern void mvEndingMakeRoomLamp(void);
extern void mvEndingMakeRoomTissues(void);
extern void mvEndingMakeFighter(s32 fkind);
extern void mvEndingRoomFadeInProcDisplay(GObj *gobj);
extern void mvEndingMakeRoomFadeIn(void);
extern void mvEndingMakeRoomFadeInCamera(void);
extern void mvEndingRoomLightProcDisplay(GObj *gobj);
extern void mvEndingMakeRoomLight(void);
extern void mvEndingMakeRoomLightCamera(void);
extern void mvEndingEjectRoomGObjs(void);
extern void mvEndingSetupOperatorCamera(GObj *gobj);
extern void mvEndingMakeMainCameras(void);
extern void mvEndingInitVars(void);
extern void mvEndingFuncRun(GObj *gobj);
extern void mvEndingFuncStart(void);
extern void mvEndingStartScene(void);

#endif
