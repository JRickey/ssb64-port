#ifndef _MN_TITLE_H_
#define _MN_TITLE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern uintptr_t lMNTitleParticleScriptBankLo;  // 0x00B22C30
extern uintptr_t lMNTitleParticleScriptBankHi;  // 0x00B22D40
extern uintptr_t lMNTitleParticleTextureBankLo; // 0x00B22D40
extern uintptr_t lMNTitleParticleTextureBankHi; // 0x00B277B0

extern void *sMNTitleFiles[/* */];

extern s32 mnTitleGetFighterKindsNum(u16 mask);
extern s32 mnTitleGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random);
extern s32 mnTitleSetDemoFighterKinds(void);
extern void mnTitleInitVars(void);
extern void mnTitleSetEndLogoPosition(void);
extern void mnTitleSetEndLayout(void);
extern void mnTitleProceedDemoNext(void);
extern void mnTitleProceedModeSelect(void);
extern void mnTitleFuncRun(GObj *gobj);
extern void mnTitleUpdateFireVars(void);
extern void mnTitleTransitionFromFireLogo(void);
extern void mnTitleShowGObjLinkID(s32 link_id);
extern void mnTitleAdvanceLayout(void);
extern void mnTitleSetAllowProceedWait(void);
extern void mnTitleTransitionsFuncRun(GObj *gobj);
extern void mnTitlePlayAnim(GObj *gobj);
extern void mnTitlePressStartProcUpdate(GObj *gobj);
extern void mnTitleProcUpdate(GObj *gobj);
extern void mnTitleUpdateLabelsPosition(GObj *gobj);
extern void mnTitleSetPosition(DObj *dobj, SObj *sobj, s32 kind);
extern void mnTitleSetColors(SObj *sobj, s32 kind);
extern void mnTitleFireProcDisplay(GObj *gobj);
extern void mnTitleFireFuncRun(GObj *gobj);
extern void mnTitleShowFire(GObj *gobj);
extern void mnTitleUpdateFireSprite(SObj *sobj, sb32 is_next);
extern void mnTitleFireProcUpdate(GObj *gobj);
extern void mnTitleMakeFire(void);
extern void mnTitleLogoProcUpdate(GObj *gobj);
extern void mnTitleLogoProcDisplay(GObj *gobj);
extern void mnTitleFadeOutLogoFuncRun(GObj *gobj);
extern void mnTitleMakeLogoNoOpening(void);
extern void mnTitleMakeLogo(void);
extern void mnTitleMakeSprites(void);
extern void mnTitleMakeLabels(void);
extern void mnTitleMakePressStart(void);
extern void func_ovl10_80133634(void);
extern void mnTitleSlashProcUpdate(GObj *gobj);
extern void mnTitleMakeSlash(void);
extern void mnTitleFireCameraProcUpdate(GObj *gobj);
extern s32 mnTitleMakeCameras(void);
extern void mnTitleLogoFireProcDisplay(GObj *gobj);
extern void mnTitleMakeLogoFire(void);
extern void mnTitleMakeLogoFireParticles(void);
extern void mnTitleMakeActors(void);
extern void mnTitleFuncStart(void);
extern void mnTitleFuncLights(Gfx **dls);
extern void mnTitleFuncUpdate(void);
extern void mnTitleStartScene(void);
extern void mnTitleLoadFiles(void);

#endif