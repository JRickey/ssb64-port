#ifndef _DBFALLS_H_
#define _DBFALLS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <db/dbdef.h>

extern void dbFallsFuncLights(Gfx **dls);
extern void dbFallsExit(void);
extern void dbFallsFuncRun(GObj *gobj);
extern GObj* dbFallsMakeCamera(void (*thread)(GObj*));
extern void dbFallsFuncStart(void);
extern void dbFallsStartScene(void);

#endif
