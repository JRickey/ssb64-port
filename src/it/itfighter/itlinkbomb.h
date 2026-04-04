#ifndef _ITLINKBOMB_H_
#define _ITLINKBOMB_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itLinkBombExplodeWaitUpdateScale(GObj* item_gobj);
extern void itLinkBombExplodeMakeEffectGotoSetStatus(GObj* item_gobj);
extern void func_ovl3_80185B18(GObj* item_gobj);
extern sb32 itLinkBombCommonProcDamage(GObj* item_gobj);
extern sb32 itLinkBombThrownProcHit(GObj* item_gobj);
extern void itLinkBombCommonSetHitStatusNormal(GObj* item_gobj);
extern void itLinkBombCommonSetHitStatusNone(GObj* item_gobj);
extern sb32 itLinkBombFallProcUpdate(GObj* item_gobj);
extern sb32 itLinkBombWaitProcUpdate(GObj* item_gobj);
extern sb32 itLinkBombWaitProcMap(GObj* item_gobj);
extern sb32 itLinkBombFallProcMap(GObj* item_gobj);
extern void itLinkBombWaitSetStatus(GObj* item_gobj);
extern void itLinkBombFallSetStatus(GObj* item_gobj);
extern sb32 itLinkBombHoldProcUpdate(GObj* item_gobj);
extern void itLinkBombHoldSetStatus(GObj* item_gobj);
extern sb32 itLinkBombThrownProcMap(GObj* item_gobj);
extern void itLinkBombThrownSetStatus(GObj* item_gobj);
extern sb32 itLinkBombDroppedProcUpdate(GObj* item_gobj);
extern sb32 itLinkBombDroppedProcHit(GObj* item_gobj);
extern sb32 itLinkBombDroppedProcDamage(GObj* item_gobj);
extern void itLinkBombDroppedSetStatus(GObj* item_gobj);
extern void itLinkBombExplodeInitVars(GObj* item_gobj);
extern void itLinkBombExplodeUpdateAttackEvent(GObj* item_gobj);
extern sb32 itLinkBombCommonProcShield(GObj* item_gobj);
extern sb32 func_ovl3_801864BC(GObj* item_gobj);
extern void itLinkBombExplodeInitAttackColl(GObj* item_gobj);
extern sb32 itLinkBombExplodeProcUpdate(GObj* item_gobj);
extern void itLinkBombExplodeSetStatus(GObj* item_gobj);
extern GObj* itLinkBombMakeItem(GObj* fighter_gobj, Vec3f* pos, Vec3f* vel);

#endif
