#ifndef _MVOPENINGYOSHI_H_
#define _MVOPENINGYOSHI_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningYoshiSetupFiles(void);
extern void mvOpeningYoshiInitName(SObj *sobj);
extern void mvOpeningYoshiMakeName(void);
extern void mvOpeningYoshiMotionCameraProcUpdate(GObj *gobj);
extern void mvOpeningYoshiMakeMotionCamera(Vec3f move);
extern void mvOpeningYoshiMakeMotionWindow(void);
extern void mvOpeningYoshiPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningYoshiMakePosedWallpaper(void);
extern void mvOpeningYoshiPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningYoshiMakePosedFighter(void);
extern void mvOpeningYoshiMakeNameCamera(void);
extern void mvOpeningYoshiMakePosedFighterCamera(void);
extern void mvOpeningYoshiMakePosedWallpaperCamera(void);
extern void mvOpeningYoshiFuncRun(GObj *gobj);
extern void mvOpeningYoshiInitVars(void);
extern void mvOpeningYoshiFuncStart(void);
extern void mvOpeningYoshiFuncLights(Gfx **dls);
extern void mvOpeningYoshiStartScene(void);

#endif
