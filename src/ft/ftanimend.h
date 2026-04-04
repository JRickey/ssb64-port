#ifndef _FTANIMEND_H_
#define _FTANIMEND_H_

#include "ftdef.h"
#include <sys/obj.h>

sb32 ftAnimEndCheckSetStatus(GObj* fighter_gobj, void (*proc_status)(GObj*));
void ftAnimEndSetWait(GObj* fighter_gobj);
void ftAnimEndSetFall(GObj* fighter_gobj);

#endif
