#ifndef _MNOPTION_H_
#define _MNOPTION_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnOptionFuncLights(Gfx **dls);
extern void mnOptionSetOptionSpriteColors(GObj *gobj, s32 status);
extern void mnOptionMakeOptionTabs(GObj *gobj, f32 pos_x, f32 pos_y, s32 lrs);
extern void mnOptionSetSoundToggleSpriteColors(GObj *gobj, sb32 mono_or_stereo);
extern void mnOptionMakeSoundToggle(void);
extern void mnOptionMakeSound(void);
extern void mnOptionMakeScreenAdjust(void);
extern void mnOptionMakeBackupClear(void);
extern void mnOptionSetSubtitleSpriteColors(SObj *sobj);
extern void mnOptionMakeMenuGObj(void);
extern void mnOptionLabelsProcDisplay(GObj *gobj);
extern void mnOptionMakeLabels(void);
extern void mnOptionMakeDecals(void);
extern void func_ovl60_80132618(GObj *gobj);
extern void func_ovl60_8013275C(void);
extern void func_ovl60_80132794(GObj *gobj);
extern void mnOptionSoundUnderlineProcDisplay(GObj *gobj);
extern void mnOptionMakeSoundUnderline(void);
extern void mnOptionMakeSoundUnderlineCamera(void);
extern void mnOptionMakeLink4Camera(void);
extern void mnOptionMakeOptionsCamera(void);
extern void mnOptionMakeLabelsCamera(void);
extern void mnOptionMakeDecalsCamera(void);
extern void mnOptionInitVars(void);
extern void mnOptionWriteBackup(void);
extern void mnOptionFuncRun(GObj *gobj);
extern void mnOptionFuncStart(void);
extern void mnOptionStartScene(void);

#endif
