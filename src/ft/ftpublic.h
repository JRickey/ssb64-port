#ifndef _FTPUBLICITY_H_
#define _FTPUBLICITY_H_

#include "fttypes.h"

extern sb32 ftPublicTryStartCall(GObj *gobj, f32 knockback, s32 player_num);
extern void ftPublicTryInterruptCall(void);
extern void ftPublicPlayCommon(u16 new_sfx);
extern void ftPublicCommonStop(void);
extern void ftPublicDecideCall(GObj *gobj, s32 player_num, f32 knockback);
extern void ftPublicDecideCommon(GObj *fighter_gobj, s32 player_num, f32 knockback, sb32 is_force_curr_knockback);
extern void ftPublicCommonCheck(GObj *fighter_gobj, f32 knockback, sb32 is_force_curr_knockback);
extern void ftPublicPlayCliffReact(GObj *fighter_gobj, f32 knockback);
extern void ftPublicTryPlayFallSpecialReact(GObj *fighter_gobj);
extern void ftPublicDefeatedAddID(u16 sfx_id);
extern void ftPublicProcUpdate(GObj *public_gobj);
extern void ftPublicMakeActor(void);

#endif
