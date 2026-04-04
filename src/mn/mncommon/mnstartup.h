#ifndef _MNStartup_H_
#define _MNStartup_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnStartupLogoThreadUpdate(GObj *gobj);
extern void mnStartupActorFuncRun(GObj *gobj);
extern void mnStartupFuncStart(void);
extern void mnStartupFuncLights(Gfx **dls);
extern void mnStartupStartScene(void);

#endif
