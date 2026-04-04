#ifndef _GRZEBES_H_
#define _GRZEBES_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern void grZebesAcidSetLevelStep(void);
extern void grZebesAcidSetRandomWait(void);
extern GObj* grZebesMakeAcid(void);
extern void grZebesAcidUpdateWait(void);
extern void grZebesAcidUpdateRumble(void);
extern void grZebesAcidUpdateNormal(void);
extern void grZebesAcidUpdateShake(void);
extern void grZebesAcidUpdateRise(void);
extern void grZebesProcUpdate(GObj *ground_gobj);
extern GObj* grZebesMakeGround(void);
extern sb32 grZebesAcidCheckGetDamageKind(GObj *ground_gobj, GObj *fighter_gobj, GRAttackColl **gr_attack_coll, s32 *kind);
extern void grZebesAcidGetLevelInfo(f32 *current, f32 *step);

#endif
