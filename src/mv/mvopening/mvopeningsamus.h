#ifndef _MVOPENINGSAMUS_H_
#define _MVOPENINGSAMUS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningSamusSetupFiles(void);
extern void mvOpeningSamusInitName(SObj *sobj);
extern void mvOpeningSamusMakeName(void);
extern void mvOpeningSamusMotionCameraProcUpdate(GObj *camera_gobj);
extern void mvOpeningSamusMakeMotionCamera(Vec3f vec);
extern void mvOpeningSamusMakeMotionWindow(void);
extern void mvOpeningSamusPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningSamusMakePosedWallpaper(void);
extern void mvOpeningSamusPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningSamusMakePosedFighter(void);
extern void mvOpeningSamusMakeNameCamera(void);
extern void mvOpeningSamusMakePosedFighterCamera(void);
extern void mvOpeningSamusMakePosedWallpaperCamera(void);
extern void mvOpeningSamusFuncRun(GObj *gobj);
extern void mvOpeningSamusInitVars(void);
extern void mvOpeningSamusFuncStart(void);
extern void mvOpeningSamusFuncLights(Gfx **dls);
extern void mvOpeningSamusStartScene(void);

#endif
