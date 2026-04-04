#ifndef _MVOPENINGRUN_H_
#define _MVOPENINGRUN_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningRunFuncLights(Gfx **dls);
extern void mvOpeningRunFighterProcUpdate(GObj *fighter_proxy_gobj);
extern void mvOpeningRunMakeFighters(void);
extern void mvOpeningRunWallpaperProcUpdate(GObj *gobj);
extern void mvOpeningRunMakeWallpaper(void);
extern void mvOpeningRunMakeCrash(void);
extern void mvOpeningRunInitMainCamera(GObj *camera_gobj);
extern void mvOpeningRunMakeMainCamera(void);
extern void mvOpeningRunMakeWallpaperCamera(void);
extern void mvOpeningRunInitVars(void);
extern void mvOpeningRunFuncRun(GObj *gobj);
extern void mvOpeningRunFuncStart(void);
extern void mvOpeningRunStartScene(void);

#endif
