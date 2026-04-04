#ifndef _DBCUBE_H_
#define _DBCUBE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <db/dbdef.h>

extern void dbCubeFuncLights(Gfx **dls);
extern void dbCubeModelThreadUpdate(GObj *gobj);
extern void dbCubeExit(void);
extern void dbCubeStartRumble(void);
extern void dbCubeStopRumble(void);
extern void dbCubePlayFGM(void);
extern void dbCubeStopFGM(void);
extern void dbCubePlayCall(void);
extern void dbCubeStopBGM(void);
extern void dbCubePlayBGM(void);
extern void dbCubePlayTransition(void);
extern void dbCubeAudioThreadUpdate(GObj *gobj);
extern void dbCubeFuncRun(GObj *gobj);
extern GObj* dbCubeMakeGObj(void (*thread)(GObj*), void *dvar);
extern GObj* dbCubeMakeCamera(void (*thread)(GObj*));
extern void dbCubeFuncStart(void);
extern void dbCubeStartScene(void);

#endif
