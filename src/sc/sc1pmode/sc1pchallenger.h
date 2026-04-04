#ifndef _SC1PCHALLENGER_H_
#define _SC1PCHALLENGER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern void sc1PChallengerFuncLights(Gfx **dls);
extern void sc1PChallengerDecalsProcDisplay(GObj *gobj);
extern void sc1PChallengerMakeDecals(void);
extern void sc1PChallengerFighterProcUpdate(GObj *fighter_gobj);
extern void sc1PChallengerMakeFighter(s32 fkind);
extern void sc1PChallengerMakeFighterCamera(void);
extern void sc1PChallengerMakeDecalsCamera(void);
extern void sc1PChallengerInitVars(void);
extern void sc1PChallengerFuncRun(GObj *gobj);
extern void sc1PChallengerFuncStart(void);
extern void sc1PChallengerStartScene(void);

#endif
