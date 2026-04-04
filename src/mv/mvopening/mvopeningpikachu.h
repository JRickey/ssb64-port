#ifndef _MVOPENINGPIKACHU_H_
#define _MVOPENINGPIKACHU_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningPikachuSetupFiles(void);
extern void mvOpeningPikachuInitName(SObj *sobj);
extern void mvOpeningPikachuMakeName(void);
extern void mvOpeningPikachuMotionCameraProcUpdate(GObj *gobj);
extern void mvOpeningPikachuMakeMotionCamera(Vec3f move);
extern void mvOpeningPikachuMakeMotionWindow(void);
extern void mvOpeningPikachuPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningPikachuMakePosedWallpaper(void);
extern void mvOpeningPikachuPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningPikachuMakePosedFighter(void);
extern void mvOpeningPikachuMakeNameCamera(void);
extern void mvOpeningPikachuMakePosedFighterCamera(void);
extern void mvOpeningPikachuMakePosedWallpaperCamera(void);
extern void mvOpeningPikachuFuncRun(GObj *gobj);
extern void mvOpeningPikachuInitVars(void);
extern void mvOpeningPikachuFuncStart(void);
extern void mvOpeningPikachuFuncLights(Gfx **dls);
extern void mvOpeningPikachuStartScene(void);

#endif
