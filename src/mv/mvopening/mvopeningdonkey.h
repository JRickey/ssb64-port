#ifndef _MVOPENINGDONKEY_H_
#define _MVOPENINGDONKEY_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningDonkeySetupFiles(void);
extern void mvOpeningDonkeyInitName(SObj *sobj);
extern void mvOpeningDonkeyMakeName(void);
extern void mvOpeningDonkeyMotionCameraProcUpdate(GObj *gobj);
extern void mvOpeningDonkeyMakeMotionCamera(Vec3f move);
extern void mvOpeningDonkeyMakeMotionWindow(void);
extern void mvOpeningDonkeyPosedWallpaperProcDisplay(GObj *gobj);
extern void mvOpeningDonkeyMakePosedWallpaper(void);
extern void mvOpeningDonkeyPosedFighterProcUpdate(GObj *fighter_gobj);
extern void mvOpeningDonkeyMakePosedFighter(void);
extern void mvOpeningDonkeyMakeNameCamera(void);
extern void mvOpeningDonkeyMakePosedFighterCamera(void);
extern void mvOpeningDonkeyMakePosedWallpaperCamera(void);
extern void mvOpeningDonkeyFuncRun(GObj *gobj);
extern void mvOpeningDonkeyInitVars(void);
extern void mvOpeningDonkeyFuncStart(void);
extern void mvOpeningDonkeyFuncLights(Gfx **dls);
extern void mvOpeningDonkeyStartScene(void);

#endif
