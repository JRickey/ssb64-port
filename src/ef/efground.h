#ifndef _EFGROUND_H_
#define _EFGROUND_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ef/efdef.h>

extern sb32 efGroundCheckEffectInBounds(GObj *effect_gobj);
extern void efGroundCommonProcUpdate(GObj *effect_gobj);
extern void efGroundUpdateEffectYaw(GObj *effect_gobj);
extern void efGroundUpdateStepPositions(GObj *effect_gobj);
extern void efGroundSetStepPositions(GObj *effect_gobj);
extern void efGroundUpdatePhysics(GObj *effect_gobj, s32 effect_id);
extern void efGroundSetupEffectDObjs(GObj *effect_gobj, DObjDesc *dobjdesc, DObj **dobjs, u8 tk1, u8 tk2, u8 arg5, s32 lr);
extern GObj* efGroundMakeEffect(EFDesc *effect_desc, s32 lr);
extern void efGroundMakeEffectID(s32 effect_id);
extern void EFGroundActorProcUpdate(GObj *gobj);
extern void efGroundSetupRandomWeights(void);
extern void efGroundMakeAppearActor(void);

#endif
