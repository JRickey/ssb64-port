#ifndef _EFALLOC_H_
#define _EFALLOC_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ef/efdef.h>

extern GObj *gEFParticleStructsGObj;
extern GObj *gEFParticleGeneratorsGObj;

extern void efParticleInitAll(void);
extern void efParticleGObjSetSkipID(u32 id);
extern void efParticleGObjSetSkipAll(void);
extern void efParticleGObjClearSkipID(u32 id);
extern void efParticleGObjClearSkipAll(void);
extern s32 efParticleGetBankID(uintptr_t scripts_lo);
extern s32 efParticleGetLoadBankID(uintptr_t scripts_lo, uintptr_t scripts_hi, uintptr_t textures_lo, uintptr_t textures_hi);

#endif
