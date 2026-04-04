#ifndef _MVOPENINGYAMABUKI_H_
#define _MVOPENINGYAMABUKI_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningYamabukiFuncLights(Gfx **dls);
extern void mvOpeningYamabukiMakeWallpaper(void);
extern void mvOpeningYamabukiMakeFighter(void);
extern void mvOpeningYamabukiMakeLegs(void);
extern void mvOpeningYamabukiMakeLegsShadow(void);
extern void mvOpeningYamabukiMakeMBall(void);
extern void mvOpeningYamabukiMakeMainCamera(void);
extern void mvOpeningYamabukiMakeWallpaperCamera(void);
extern void mvOpeningYamabukiInitTotalTimeTics(void);
extern void mvOpeningYamabukiFuncRun(GObj *gobj);
extern void mvOpeningYamabukiFuncStart(void);
extern void mvOpeningYamabukiStartScene(void);

#endif
