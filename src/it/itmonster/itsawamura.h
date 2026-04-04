#ifndef _ITSAWAMURA_H_
#define _ITSAWAMURA_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itSawamuraFallProcUpdate(GObj *item_gobj);
extern sb32 itSawamuraFallProcMap(GObj *item_gobj);
extern void itSawamuraFallSetStatus(GObj *item_gobj);
extern sb32 itSawamuraWaitProcUpdate(GObj *item_gobj);
extern sb32 itSawamuraWaitProcMap(GObj *item_gobj);
extern void itSawamuraWaitSetStatus(GObj *item_gobj);
extern sb32 itSawamuraAttackProcUpdate(GObj *item_gobj);
extern void itSawamuraAttackSetFollowPlayerLR(GObj *item_gobj, GObj *fighter_gobj);
extern void itSawamuraAttackInitVars(GObj *item_gobj);
extern void itSawamuraAttackSetStatus(GObj *item_gobj);
extern sb32 itSawamuraCommonProcUpdate(GObj *item_gobj);
extern sb32 itSawamuraCommonProcMap(GObj *item_gobj);
extern GObj* itSawamuraMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
