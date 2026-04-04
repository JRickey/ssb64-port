#ifndef _ITRSHELL_H_
#define _ITRSHELL_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itRShellSpinUpdateFollowPlayer(GObj *item_gobj, GObj *fighter_gobj);
extern void itRShellSpinSearchFollowPlayer(GObj *item_gobj);
extern void itRShellSpinUpdateGFX(GObj *item_gobj);
extern void itRShellSpinAddAnim(GObj *item_gobj);
extern void itRShellCommonClearAnim(GObj *item_gobj);
extern sb32 itRShellFallProcUpdate(GObj *item_gobj);
extern sb32 itRShellWaitProcMap(GObj *item_gobj);
extern sb32 itRShellFallProcMap(GObj *item_gobj);
extern void itRShellCommonSetStatusWaitOrSpin(GObj *item_gobj);
extern void itRShellCommonProcStatusWaitOrSpin(GObj *item_gobj);
extern void itRShellFallSetStatus(GObj *item_gobj);
extern sb32 itRShellCommonProcDamage(GObj *item_gobj);
extern void itRShellHoldSetStatus(GObj *item_gobj);
extern void itRShellThrownSetStatus(GObj *item_gobj);
extern void itRShellDroppedSetStatus(GObj *item_gobj);
extern sb32 itRShellThrownProcMap(GObj *item_gobj);
extern void itRShellSpinEdgeInvertVelLR(GObj *item_gobj, ub8 lr);
extern void itRShellSpinCheckCollisionEdge(GObj *item_gobj);
extern sb32 itRShellSpinProcUpdate(GObj *item_gobj);
extern sb32 itRShellSpinProcMap(GObj *item_gobj);
extern sb32 itRShellCommonProcHit(GObj *item_gobj);
extern sb32 itRShellSpinProcDamage(GObj *item_gobj);
extern void itRShellSpinInitVars(GObj *item_gobj);
extern void itRShellSpinSetStatus(GObj *item_gobj);
extern void itRShellSpinAirInitVars(GObj *item_gobj);
extern void itRShellSpinAirSetStatus(GObj *item_gobj);
extern GObj* itRShellMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itRShellCommonProcShield(GObj *item_gobj);
extern sb32 itRShellCommonProcReflector(GObj *item_gobj);

#endif
