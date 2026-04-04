#ifndef _EFRENDER_H_
#define _EFRENDER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ef/efdef.h>

extern uintptr_t lEFCommonParticleScriptBankLo;  // 0x00AC7340
extern uintptr_t lEFCommonParticleScriptBankHi;  // 0x00AC9DE0
extern uintptr_t lEFCommonParticleTextureBankLo; // 0x00AC9DE0
extern uintptr_t lEFCommonParticleTextureBankHi; // 0x00B16C80

extern void efDisplayCLDProcDisplay(GObj *effect_gobj);
extern void efDisplayXLUProcDisplay(GObj *effect_gobj);
extern void efDisplayMakeCLD(void);
extern void efDisplayMakeXLU(void);
extern void efDisplayZPerspXLUProcDisplay(GObj *effect_gobj);
extern void efDisplayZPerspCLDProcDisplay(GObj *effect_gobj);
extern void efDisplayZPerspAAXLUProcDisplay(GObj *effect_gobj);
extern void efDisplayInitAll(void);

#endif
