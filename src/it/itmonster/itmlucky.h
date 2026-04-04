#ifndef _ITMLUCKY_H_
#define _ITMLUCKY_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern void itMLuckyMakeEggInitVars(GObj *item_gobj);
extern sb32 itMLuckyFallProcUpdate(GObj *item_gobj);
extern sb32 itMLuckyFallProcMap(GObj *item_gobj);
extern void itMLuckyFallSetStatus(GObj *item_gobj);
extern sb32 itMLuckyAppearProcUpdate(GObj *item_gobj);
extern sb32 itMLuckyAppearProcMap(GObj *item_gobj);
extern void itMLuckyAppearSetStatus(GObj *item_gobj);
extern sb32 itMLuckyMakeEggProcUpdate(GObj *lucky_gobj);
extern sb32 itMLuckyMakeEggProcMap(GObj *item_gobj);
extern sb32 itMLuckyMakeEggProcDamage(GObj *item_gobj);
extern void itMLuckyMakeEggSetStatus(GObj *item_gobj);
extern sb32 itMLuckyDisappearProcUpdate(GObj *item_gobj);
extern void itMLuckyDisappearSetStatus(GObj *item_gobj);
extern sb32 itMLuckyCommonProcUpdate(GObj *item_gobj);
extern sb32 itMLuckyCommonProcMap(GObj *item_gobj);
extern GObj* itMLuckyMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
