#ifndef _MVOPENINGKIRBY_H_
#define _MVOPENINGKIRBY_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningKirbySetupFiles(void);
extern void mvOpeningKirbyInitName(SObj *sobj);
extern void mvOpeningKirbyMakeName(void);
extern void mvOpeningKirbyMotionCameraProcUpdate(GObj *gobj);
extern void mvOpeningKirbyMakeMotionCamera(Vec3f move);
extern void mvOpeningKirbyMakeMotionWindow(void);
extern void mvOpeningKirbyPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningKirbyMakePosedWallpaper(void);
extern void mvOpeningKirbyPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningKirbyMakePosedFighter(void);
extern void mvOpeningKirbyMakeNameCamera(void);
extern void mvOpeningKirbyMakePosedFighterCamera(void);
extern void mvOpeningKirbyMakePosedWallpaperCamera(void);
extern void mvOpeningKirbyFuncRun(GObj *gobj);
extern void mvOpeningKirbyInitVars(void);
extern void mvOpeningKirbyFuncStart(void);
extern void mvOpeningKirbyFuncLights(Gfx **dls);
extern void mvOpeningKirbyStartScene(void);

#endif
