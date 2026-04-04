#ifndef _ITBOMBHEI_H_
#define _ITBOMBHEI_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itBombHeiCommonSetExplode(GObj *item_gobj, u8 unused);
extern void itBombHeiCommonSetWalkLR(GObj *item_gobj, ub8 lr);
extern void itBombHeiCommonCheckMakeDustEffect(GObj *item_gobj, u8 override);
extern void itBombHeiCommonSetHitStatusNormal(GObj *item_gobj);
extern void itBombHeiCommonSetHitStatusNone(GObj *item_gobj);
extern sb32 itBombHeiFallProcUpdate(GObj *item_gobj);
extern s32 itBombHeiWalkGetLR(GObj *item_gobj);
extern sb32 itBombHeiWaitProcUpdate(GObj *item_gobj);
extern sb32 itBombHeiWaitProcMap(GObj *item_gobj);
extern sb32 itBombHeiCommonProcHit(GObj *item_gobj);
extern sb32 itBombHeiFallProcMap(GObj *item_gobj);
extern void itBombHeiWaitSetStatus(GObj *item_gobj);
extern void itBombHeiFallSetStatus(GObj *item_gobj);
extern void itBombHeiHoldSetStatus(GObj *item_gobj);
extern sb32 itBombHeiThrownProcUpdate(GObj *item_gobj);
extern sb32 itBombHeiThrownProcMap(GObj *item_gobj);
extern void itBombHeiThrownSetStatus(GObj *item_gobj);
extern sb32 itBombHeiDroppedProcMap(GObj *item_gobj);
extern void itBombHeiDroppedSetStatus(GObj *item_gobj);
extern void itBombHeiWalkUpdateEffect(GObj *item_gobj);
extern sb32 itBombHeiWalkProcUpdate(GObj *item_gobj);
extern sb32 itBombHeiWalkProcMap(GObj *item_gobj);
extern void itBombHeiWalkInitVars(GObj *item_gobj);
extern void itBombHeiWalkSetStatus(GObj *item_gobj);
extern void itBombHeiCommonClearVelSetExplode(GObj *item_gobj, u8 unused);
extern void itBombHeiCommonUpdateAttackEvent(GObj *item_gobj);
extern sb32 itBombHeiExplodeMapProcUpdate(GObj *item_gobj);
extern sb32 itBombHeiExplodeCommonProcHit(GObj *item_gobj);
extern void itBombHeiExplodeMapSetStatus(GObj *item_gobj);
extern void itBombHeiExplodeInitVars(GObj *item_gobj);
extern sb32 itBombHeiExplodeProcUpdate(GObj *item_gobj);
extern void itBombHeiExplodeSetStatus(GObj *item_gobj);
extern sb32 itBombHeiExplodeWaitProcUpdate(GObj *item_gobj);
extern sb32 itBombHeiExplodeWaitProcMap(GObj *item_gobj);
extern void itBombHeiExplodeWaitInitVars(GObj *item_gobj);
extern void itBombHeiExplodeWaitSetStatus(GObj *item_gobj);
extern GObj *itBombHeiMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
