#ifndef _MVOPENINGYOSTER_H_
#define _MVOPENINGYOSTER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningYosterFuncLights(Gfx **dls);
extern void mvOpeningYosterMakeNest(void);
extern void mvOpeningYosterMakeFighters(void);
extern void mvOpeningYosterMakeWallpaper(void);
extern void mvOpeningYosterMakeGround(void);
extern void mvOpeningYosterMakeMainCamera(void);
extern void mvOpeningYosterMakeWallpaperCamera(void);
extern void mvOpeningYosterInitTotalTimeTics(void);
extern void mvOpeningYosterMainProc(GObj *gobj);
extern void mvOpeningYosterFuncStart(void);
extern void mvOpeningYosterStartScene(void);

#endif
