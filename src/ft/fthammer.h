#ifndef _FTHAMMER_H_
#define _FTHAMMER_H_

#include "ftdef.h"
#include <ssb_types.h>
#include <sys/objdef.h>

void ftHammerUpdateStats(GObj* fighter_gobj);
sb32 ftHammerCheckHoldHammer(GObj* fighter_gobj);
sb32 ftHammerCheckStatusHammerAll(GObj* fighter_gobj);
sb32 ftHammerCheckMotionWaitOrWalk(GObj* fighter_gobj);
f32 ftHammerGetAnimFrame(GObj* fighter_gobj);
u32 ftHammerGetStatUpdateFlags(GObj* fighter_gobj);
void ftHammerSetColAnim(GObj* fighter_gobj);
void ftHammerProcInterrupt(GObj* fighter_gobj);
void ftHammerProcMap(GObj* fighter_gobj);
void ftHammerSetStatusHammerWait(GObj* fighter_gobj);
sb32 ftHammerCheckGotoHammerWait(GObj* fighter_gobj);

#endif
