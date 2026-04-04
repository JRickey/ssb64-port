#ifndef _ITMBALL_H_
#define _ITMBALL_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itMBallOpenAddAnim(GObj *item_gobj);
extern void itMBallOpenClearAnim(GObj *item_gobj);
extern sb32 itMBallFallProcUpdate(GObj *item_gobj);
extern sb32 itMBallWaitProcMap(GObj *item_gobj);
extern sb32 itMBallFallProcMap(GObj *item_gobj);
extern void itMBallWaitSetStatus(GObj *item_gobj);
extern void itMBallFallSetStatus(GObj *item_gobj);
extern void itMBallHoldSetStatus(GObj *item_gobj);
extern sb32 itMBallThrownProcUpdate(GObj *item_gobj);
extern sb32 itMBallThrownProcMap(GObj *item_gobj);
extern sb32 itMBallCommonProcHit(GObj *item_gobj);
extern sb32 itMBallCommonProcReflector(GObj *item_gobj);
extern void itMBallThrownSetStatus(GObj *item_gobj);
extern void itMBallDroppedSetStatus(GObj *item_gobj);
extern sb32 itMBallOpenProcUpdate(GObj *mball_gobj);
extern sb32 itMBallOpenProcMap(GObj *item_gobj);
extern void itMBallOpenInitVars(GObj *item_gobj);
extern void itMBallOpenSetStatus(GObj *item_gobj);
extern sb32 itMBallOpenAirProcUpdate(GObj *mball_gobj);
extern sb32 itMBallOpenAirProcMap(GObj *item_gobj);
extern void itMBallOpenAirSetStatus(GObj *item_gobj);
extern GObj* itMBallMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
