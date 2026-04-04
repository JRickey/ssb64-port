#ifndef _GRPUPUPU_H_
#define _GRPUPUPU_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern intptr_t lGRPupupuParticleScriptBankLo;                  // 0x00B1E640       
extern intptr_t lGRPupupuParticleScriptBankHi;                  // 0x00B1E7E0
extern intptr_t lGRPupupuParticleTextureBankLo;                 // 0x00B1E7E0
extern intptr_t lGRPupupuParticleTextureBankHi;                 // 0x00B1F960

extern s32 grPupupuWhispyGetLR(GObj *ground_gobj);
extern void grPupupuWhispySetWindPush(void);
extern void grPupupuWhispyUpdateSleep(void);
extern void grPupupuWhispyLeavesMakeEffect(void);
extern void grPupupuWhispyUpdateWait(void);
extern void grPupupuWhispyUpdateTurn(void);
extern void grPupupuWhispyUpdateOpen(void);
extern void grPupupuWhispyUpdateWindRumble(void);
extern void grPupupuWhispyUpdateBlow(void);
extern void grPupupuWhispyUpdateStop(void);
extern void grPupupuWhispyUpdateBlink(void);
extern void grPupupuUpdateWhispyStatus(void);
extern void grPupupuFlowersBackWindStart(void);
extern void grPupupuFlowersBackLoopStart(void);
extern void grPupupuFlowersBackLoopEnd(void);
extern void grPupupuFlowersBackUpdateAll(void);
extern void grPupupuFlowersFrontWindStart(void);
extern void grPupupuWhispyDustMakeEffect(void);
extern void grPupupuFlowersFrontLoopStart(void);
extern void grPupupuFlowersFrontLoopEnd(void);
extern void grPupupuFlowersFrontUpdateAll(void);
extern void grPupupuUpdateGObjAnims(void);
extern void grPupupuProcUpdate(GObj *ground_gobj);
extern GObj* grPupupuMakeMapGObj(intptr_t o_dobjdesc, intptr_t o_mobjsub, void (*proc_display)(GObj*), u8 dl_link);
extern void grPupupuInitAll(void);
extern GObj* grPupupuMakeGround(void);

#endif
