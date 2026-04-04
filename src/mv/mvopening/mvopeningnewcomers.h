#ifndef _MVOPENINGNEWCOMERS_H_
#define _MVOPENINGNEWCOMERS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningNewcomersFuncLights(Gfx **dls);
extern sb32 mvOpeningNewcomersCheckLocked(s32 fkind);
extern void mvOpeningNewcomersMakePurin(void);
extern void mvOpeningNewcomersMakeCaptain(void);
extern void mvOpeningNewcomersMakeLuigi(void);
extern void mvOpeningNewcomersMakeNess(void);
extern void mvOpeningNewcomersMakeAll(void);
extern void mvOpeningNewcomersHideProcDisplay(GObj *gobj);
extern void mvOpeningNewcomersMakeHide(void);
extern void mvOpeningNewcomersMakeNewcomersCamera(void);
extern void mvOpeningNewcomersMakeHideCamera(void);
extern void mvOpeningNewcomersInitVars(void);
extern void mvOpeningNewcomersFuncRun(GObj *gobj);
extern void mvOpeningNewcomersFuncStart(void);
extern void mvOpeningNewcomersStartScene(void);

#endif
