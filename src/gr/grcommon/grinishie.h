#ifndef _GRINISHIE_H_
#define _GRINISHIE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern void grInishieScaleUpdateFighterStatsGA(void);
extern f32 grInishieScaleGetPressure(s32 line_id);
extern void grInishieScaleUpdateWait(void);
extern void grInishieScaleUpdateFall(void);
extern void grInishieScaleUpdateStep(void);
extern void grInishieScaleUpdateRetract(void);
extern void grInishieScaleProcUpdate(GObj *ground_gobj);
extern void grInishieMakeScale(void);
extern void grInishiePakkunSetWaitFighter(void);
extern void grInishieMakePakkun(void);
extern void grInishiePowerBlockUpdateWait(void);
extern void grInishiePowerBlockSetWait(void);
extern void grInishiePowerBlockUpdateMake(void);
extern void grInishiePowerBlockUpdateDamage(void);
extern void grInishiePowerBlockProcUpdate(GObj *ground_gobj);
extern void grInishieMakePowerBlock(void);
extern void grInishiePowerBlockSetDamage(void);
extern sb32 grInishiePowerBlockCheckGetDamageKind(GObj *item_gobj, GObj *fighter_gobj, GRAttackColl **gr_attack_coll, s32 *kind);
extern void grInishieInitHeaders(void);
extern GObj* grInishieMakeGround(void);
extern void grInishieScaleGetPlatformInfo(f32 *alt, f32 *accel);

#endif
