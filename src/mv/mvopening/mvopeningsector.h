#ifndef _MVOPENINGSECTOR_H_
#define _MVOPENINGSECTOR_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningSectorFuncLights(Gfx **dls);
extern void mvOpeningSectorWallpaperProcUpdate(GObj* wallpaper_gobj);
extern void mvOpeningSectorMakeWallpaper(void);
extern void mvOpeningSectorMakeGreatFox(void);
extern void mvOpeningSectorCockpitProcDisplay(GObj *cockpit_gobj);
extern void mvOpeningSectorCockpitProcUpdate(GObj* cockpit_gobj);
extern void mvOpeningSectorMakeCockpit(void);
extern void mvOpeningSectorMakeArwings(void);
extern void mvOpeningSectorCameraProcUpdate(GObj* camera_gobj);
extern void mvOpeningSectorMakeMainCamera(void);
extern void mvOpeningSectorMakeWallpaperCamera(void);
extern void mvOpeningSectorMakeCockpitCamera(void);
extern void mvOpeningSectorInitTotalTimeTics(void);
extern void mvOpeningSectorFuncRun(GObj *gobj);
extern void mvOpeningSectorFuncStart(void);
extern void mvOpeningSectorStartScene(void);

#endif
