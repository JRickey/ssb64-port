#ifndef _MVOPENINGMARIO_H_
#define _MVOPENINGMARIO_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningMarioSetupFiles(void);
extern void mvOpeningMarioInitName(SObj *sobj);
extern void mvOpeningMarioMakeName(void);
extern void mvOpeningMarioMotionCameraProcUpdate(GObj *gobj);
extern void mvOpeningMarioMakeMotionCamera(Vec3f move);
extern void mvOpeningMarioMakeMotionWindow(void);
extern void mvOpeningMarioPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningMarioMakePosedWallpaper(void);
extern void mvOpeningMarioPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningMarioMakePosedFighter(void);
extern void mvOpeningMarioMakeNameCamera(void);
extern void mvOpeningMarioMakePosedFighterCamera(void);
extern void mvOpeningMarioMakePosedWallpaperCamera(void);
extern void mvOpeningMarioFuncRun(GObj *gobj);
extern void mvOpeningMarioInitVars(void);
extern void mvOpeningMarioFuncStart(void);
extern void mvOpeningMarioFuncLights(Gfx **dls);
extern void mvOpeningMarioStartScene(void);

#endif
