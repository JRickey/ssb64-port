#ifndef _MNMESSAGE_H_
#define _MNMESSAGE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnMessageFuncLights(Gfx **dls);
extern void mnMessageMakeWallpaper(void);
extern void mnMessageTintProcDisplay(GObj *gobj);
extern void mnMessageMakeTint(void);
extern void mnMessageMakeExclaim(void);
extern void mnMessageMakeMessage(s32 message);
extern void mnMessageMakeTintCamera(void);
extern void mnMessageMakeMessageCamera(void);
extern void mnMessageMakeWallpaperCamera(void);
extern void mnMessageMakeExclaimCamera(void);
extern void mnMessageInitVars(void);
extern void mnMessageApplyUnlock(void);
extern void mnMessageFuncRun(GObj *gobj);
extern void mnMessageFuncStart(void);
extern void mnMessageStartScene(void);

#endif
