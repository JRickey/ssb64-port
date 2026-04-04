#ifndef _MVOPENINGCLASH_H_
#define _MVOPENINGCLASH_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningClashFuncLights(Gfx **dls);
extern void mvOpeningClashMakeFighters(void);
extern void mvOpeningClashVoidProcDisplay(GObj *gobj);
extern void mvOpeningClashMakeVoid(void);
extern void mvOpeningClashMakeWallpaper(void);
extern void mvOpeningClashMakeFightersCamera(void);
extern void mvOpeningClashMakeVoidCamera(void);
extern void mvOpeningClashWallpaperProcDisplay(GObj*);
extern void mvOpeningClashMakeWallpaperCamera(void);
extern void mvOpeningClashInitTotalTimeTics(void);
extern void mvOpeningClashFuncRun(GObj *gobj);
extern void mvOpeningClashFuncStart(void);
extern void mvOpeningClashStartScene(void);

#endif
