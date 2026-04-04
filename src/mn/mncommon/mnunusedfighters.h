#ifndef _MNUNUSEDFIGHTERS_H_
#define _MNUNUSEDFIGHTERS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnUnusedFightersFuncLights(Gfx **dls);
extern void mnUnusedFightersTimeProcUpdate(GObj *gobj);
extern void mnUnusedFightersFuncRun(GObj *gobj);
extern void mnUnusedFightersFuncStart(void);
extern void mnUnusedFightersStartScene(void);

#endif
