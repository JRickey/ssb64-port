#ifndef _MNVSOPTIONS_H_
#define _MNVSOPTIONS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnVSOptionsFuncLights(Gfx **dls);
extern s32 mnVSOptionsGetPowerOf(s32 base, s32 exp);
extern void mnVSOptionsSetDamageDigitSpriteColors(SObj *sobj, u32 *colors);
extern s32 mnVSOptionsGetDamageDigitCount(s32 damage, s32 digit_count_max);
extern void mnVSOptionsMakeDamageDigitSObjs(GObj *gobj, s32 damage, f32 pos_x, f32 pos_y, u32 *colors, s32 digit_count_max, sb32 is_fixed_digit_count);
extern void mnVSOptionsSetOptionSpriteColors(GObj *gobj, s32 status);
extern void mnVSOptionsSetToggleSpriteColors(GObj *gobj, s32 status);
extern void mnVSOptionsMakeOnOffToggle(GObj *gobj, f32 pos_x, f32 pos_y);
extern void mnVSOptionsSetHandicapSpriteColors(GObj *gobj, s32 setting);
extern void mnVSOptionsMakeHandicapToggle(GObj *gobj, f32 pos_x, f32 pos_y);
extern void mnVSOptionsMakeDamageDigits(void);
extern void mnVSOptionsMakeDamageOption(void);
extern void mnVSOptionsMakeItemSwitchOption(void);
extern void mnVSOptionsMakeStageSelectOption(void);
extern void mnVSOptionsMakeTeamAttackOption(void);
extern void mnVSOptionsMakeHandicapOption(void);
extern void mnVSOptionsLabelProcDisplay(GObj *gobj);
extern void mnVSOptionsMakeLabel(void);
extern void mnVSOptionsSetSubtitleSpriteColors(SObj *sobj);
extern void mnVSOptionsMakeSubtitle(void);
extern void mnVSOptionsMakeWallpaper(void);
extern void mnVSOptionsUnderlineProcDisplay(GObj *gobj);
extern void mnVSOptionsMakeUnderline(void);
extern void mnVSOptionsTintProcDisplay(GObj *gobj);
extern void mnVSOptionsMakeTint(void);
extern void mnVSOptionsMakeDecal(void);
extern void mnVSOptionsMakeTintCamera(void);
extern void mnVSOptionsMakeOptionCamera(void);
extern void mnVSOptionsMakeLabelUnderlineCamera(void);
extern void mnVSOptionsMakeWallpaperCamera(void);
extern void mnVSOptionsMakeDecalCamera(void);
extern void mnVSOptionsInitVars(void);
extern void mnVSOptionsSetAllSettings(void);
extern void mnVSOptionsSetHandicapSettings(void);
extern void mnVSOptionsFuncRun(GObj *gobj);
extern void mnVSOptionsFuncStart(void);
extern void mnVSOptionsStartScene(void);

#endif
