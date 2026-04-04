#ifndef _ITGSHELL_H_
#define _ITGSHELL_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itGShellSpinUpdateEffect(GObj *item_gobj);
extern void itGShellSpinAddAnim(GObj *item_gobj);
extern void itGShellCommonClearAnim(GObj *item_gobj);
extern sb32 itGShellFallProcUpdate(GObj *item_gobj);
extern sb32 itGShellWaitProcMap(GObj *item_gobj);
extern sb32 itGShellFallProcMap(GObj *item_gobj);
extern void itGShellWaitInitVars(GObj *item_gobj);
extern void itGShellWaitSetStatus(GObj *item_gobj);
extern void itGShellFallSetStatus(GObj *item_gobj);
extern sb32 itGShellCommonProcDamage(GObj *item_gobj);
extern void itGShellHoldSetStatus(GObj *item_gobj);
extern sb32 itGShellThrownProcMap(GObj *item_gobj);
extern sb32 itGShellThrownProcUpdate(GObj *item_gobj);
extern void itGShellThrownSetStatus(GObj *item_gobj);
extern void itGShellDroppedSetStatus(GObj *item_gobj);
extern sb32 itGShellSpinProcUpdate(GObj *item_gobj);
extern sb32 itGShellSpinProcMap(GObj *item_gobj);
extern sb32 itGShellCommonProcHit(GObj *item_gobj);
extern sb32 itGShellSpinProcDamage(GObj *item_gobj);
extern void itGShellSpinInitVars(GObj *item_gobj);
extern void itGShellSpinSetStatus(GObj *item_gobj);
extern void itGShellSpinAirInitVars(GObj *item_gobj);
extern void itGShellSpinAirSetStatus(GObj *item_gobj);
extern GObj* itGShellMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);
extern sb32 itGShellCommonProcShield(GObj *item_gobj);

#endif
