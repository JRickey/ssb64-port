#ifndef _GRYOSTER_H_
#define _GRYOSTER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>
#include <ef/efdef.h>

extern intptr_t lGRYosterParticleScriptBankLo;  // 0x00B22980
extern intptr_t lGRYosterParticleScriptBankHi;  // 0x00B22A00
extern intptr_t lGRYosterParticleTextureBankLo; // 0x00B22A00
extern intptr_t lGRYosterParticleTextureBankHi; // 0x00B22C30

extern LBGenerator* grYosterCloudVaporMakeEffect(Vec3f *pos);
extern sb32 grYosterCheckFighterCloudStand(s32 cloud_id);
extern void grYosterUpdateCloudSolid(s32 cloud_id);
extern void grYosterUpdateCloudEvaporate(s32 cloud_id);
extern void grYosterUpdateCloudAnim(s32 cloud_id);
extern void grYosterProcUpdate(GObj *ground_gobj);
extern void grYosterInitAll(void);
extern GObj* grYosterMakeGround(void);

#endif
