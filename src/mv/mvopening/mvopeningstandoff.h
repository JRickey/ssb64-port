#ifndef _MVOPENINGSTANDOFF_H_
#define _MVOPENINGSTANDOFF_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningStandoffFuncLights(Gfx **dls);
extern void mvOpeningStandoffMakeGround(void);
extern void mvOpeningStandoffMakeFighters(void);
extern void mvOpeningStandoffWallpaperProcUpdate(GObj* wallpaper_gobj);
extern void mvOpeningStandoffMakeWallpaper(void);
extern void mvOpeningStandoffMakeLightning(void);
extern void mvOpeningStandoffLightningFlashProcDisplay(GObj *gobj);
extern void mvOpeningStandoffMakeLightningFlash(void);
extern void mvOpeningStandoffMakeLightningFlashCamera(void);
extern void mvOpeningStandoffMakeMainCamera(void);
extern void mvOpeningStandoffMakeWallpaperCamera(void);
extern void mvOpeningStandoffInitTotalTimeTics(void);
extern void mvOpeningStandoffFuncRun(GObj *gobj);
extern void mvOpeningStandoffFuncStart(void);
extern void mvOpeningStandoffStartScene(void);

#endif
