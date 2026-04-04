#ifndef _MNNOCONTROLLER_H_
#define _MNNOCONTROLLER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <mn/mndef.h>

extern void *gMNNoControllerFiles[/* */];

extern GObj* mnNoControllerMakeCamera(void);
extern void mnNoControllerMakeImage(void);
extern void mnNoControllerFuncStart(void);
extern void mnNoControllerStartScene(void);
extern void mnNoControllerSetupFiles(void);

#endif
