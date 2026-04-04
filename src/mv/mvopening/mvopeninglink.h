#ifndef _MVOPENINGLINK_H_
#define _MVOPENINGLINK_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningLinkSetupFiles(void);
extern void mvOpeningLinkInitName(SObj *sobj);
extern void mvOpeningLinkMakeName(void);
extern void mvOpeningLinkMotionCameraProcUpdate(GObj *gobj);
extern void mvOpeningLinkMakeMotionCamera(Vec3f move);
extern void mvOpeningLinkMakeMotionWindow(void);
extern void mvOpeningLinkPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningLinkMakePosedWallpaper(void);
extern void mvOpeningLinkPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningLinkMakePosedFighter(void);
extern void mvOpeningLinkMakeNameCamera(void);
extern void mvOpeningLinkMakePosedFighterCamera(void);
extern void mvOpeningLinkMakePosedWallpaperCamera(void);
extern void mvOpeningLinkFuncRun(GObj *gobj);
extern void mvOpeningLinkInitVars(void);
extern void mvOpeningLinkFuncStart(void);
extern void mvOpeningLinkFuncLights(Gfx **dls);
extern void mvOpeningLinkStartScene(void);

#endif
