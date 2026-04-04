#ifndef _SC1PTRAINING_H_
#define _SC1PTRAINING_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern intptr_t lSC1PTrainingModeDisplayLabelSprites;			// 0x00000000
extern intptr_t lSC1PTrainingModeDisplayOptionSprites;			// 0x00000020
extern intptr_t lSC1PTrainingModeMenuLabelSprites;				// 0x000000BC
extern intptr_t D_NF_0000010C;								    // 0x0000010C
extern intptr_t lSC1PTrainingModeMenuOptionSprites;				// 0x0000013C
extern intptr_t D_NF_000001B8;								    // 0x000001B8

extern void sc1PTrainingModeSetMenuGObjFlags(u32 flags);
extern void sc1PTrainingModeCheckEnterMenu(void);
extern void sc1PTrainingModeCheckLeaveMenu(void);
extern void sc1PTrainingModeUpdateMenuInputs(void);
extern void sc1PTrainingModeUpdateScroll(void);
extern sb32 sc1PTrainingModeCheckUpdateOptionID(s32 *option, s32 option_min, s32 option_max);
extern sb32 sc1PTrainingModeUpdateCPOption(void);
extern s32 sc1PTrainingModeGetItemCount(void);
extern sb32 sc1PTrainingModeUpdateItemOption(void);
extern sb32 sc1PTrainingModeUpdateSpeedOption(void);
extern sb32 sc1PTrainingModeUpdateViewOption(void);
extern sb32 sc1PTrainingModeUpdateResetOption(void);
extern sb32 sc1PTrainingModeUpdateExitOption(void);
extern void sc1PTrainingModeUpdateMainOption(void);
extern void sc1PTrainingModeUpdateMenu(void);
extern sb32 sc1PTrainingModeCheckLagTic(void);
extern void sc1PTrainingModeUpdateAll(void);
extern void sc1PTrainingModeFuncUpdate(void);
extern void sc1PTrainingModeInitVars(void);
extern void sc1PTrainingModeLoadSprites(void);
extern void sc1PTrainingModeLoadWallpaper(void);
extern void sc1PTrainingModeInitDisplayVars(void);
extern SObj* sc1PTrainingModeMakeStatDisplay(GObj *interface_gobj, SC1PTrainingModeSprites *ts);
extern void sc1PTrainingModeMakeStatDisplayText(void);
extern void sc1PTrainingModeUpdateDamageDisplay(GObj *interface_gobj, s32 damage);
extern void sc1PTrainingModeDamageDisplayProcDisplay(GObj *interface_gobj);
extern void sc1PTrainingModeDamageDisplayProcUpdate(GObj *interface_gobj);
extern void sc1PTrainingModeInitStatDisplayCharacterSprites(void);
extern void sc1PTrainingModeInitSpriteEnvColors(SObj *sobj);
extern void sc1PTrainingModeMakeDamageDisplay(void);
extern void sc1PTrainingModeUpdateComboDisplay(GObj *interface_gobj, s32 combo);
extern void sc1PTrainingModeComboDisplayProcUpdate(GObj *interface_gobj);
extern void sc1PTrainingModeComboDisplayProcDisplay(GObj *interface_gobj);
extern void sc1PTrainingModeMakeComboDisplay(void);
extern void sc1PTrainingModeUpdateSpeedDisplaySprite(void);
extern void sc1PTrainingModeMakeSpeedDisplay(void);
extern void sc1PTrainingModeUpdateCPDisplaySprite(void);
extern void sc1PTrainingModeMakeCPDisplay(void);
extern void sc1PTrainingModeUpdateItemDisplaySprite(void);
extern void sc1PTrainingModeItemDisplayProcDisplay(GObj *interface_gobj);
extern void sc1PTrainingModeMakeItemDisplay(void);
extern void sc1PTrainingModeMakeStatDisplayAll(void);
extern void sc1PTrainingModeMakeMenuLabels(void);
extern void sc1PTrainingModeInitMenuOptionSpriteAttrs(void);
extern void sc1PTrainingModeMenuProcDisplay(GObj *interface_gobj);
extern void sc1PTrainingModeMakeMenu(void);
extern void sc1PTrainingModeInitCPOptionSpriteColors(void);
extern void sc1PTrainingModeUpdateCPOptionSprite(void);
extern void sc1PTrainingModeMakeCPOption(void);
extern void sc1PTrainingModeUpdateItemOptionSprite(void);
extern void sc1PTrainingModeInitItemOptionSpriteColors(void);
extern void sc1PTrainingModeMakeItemOption(void);
extern void sc1PTrainingModeInitSpeedOptionSpriteColors(void);
extern void sc1PTrainingModeUpdateSpeedOptionSprite(void);
extern void sc1PTrainingModeMakeSpeedOption(void);
extern void sc1PTrainingModeUpdateViewOptionSprite(void);
extern void sc1PTrainingModeViewOptionProcUpdate(GObj *interface_gobj);
extern void sc1PTrainingModeMakeViewOption(void);
extern void sc1PTrainingModeSetHScrollOptionSObjs(void);
extern void sc1PTrainingModeInitOptionArrowSpriteColors(SObj *sobj);
extern void sc1PTrainingModeUpdateOptionArrows(void);
extern void sc1PTrainingModeMakeOptionArrows(void);
extern SObj* func_ovl7_8018F7C8(GObj *interface_gobj, SC1PTrainingModeSprites *ts);
extern void func_ovl7_8018F804(void);
extern void func_ovl7_8018F874(void);
extern void func_ovl7_8018F8FC(void);
extern void func_ovl7_8018F984(void);
extern s32 sc1PTrainingModeGetOptionSpriteID(void);
extern void func_ovl7_8018FA54(void);
extern void func_ovl7_8018FB40(void);
extern void sc1PTrainingModeUpdateCursorPosition(void);
extern void sc1PTrainingModeMakeCursor(void);
extern void sc1PTrainingModeSetVScrollOptionSObjs(void);
extern void sc1PTrainingModeUnderlineProcDisplay(GObj *interface_gobj);
extern void sc1PTrainingModeUpdateUnderline(void);
extern void sc1PTrainingModeMakeUnderline(void);
extern void sc1PTrainingModeMakeMenuAll(void);
extern void sc1PTrainingModeSetPlayDefaultBGM(void);
extern void sc1PTrainingModeSetGameStatusGo(void);
extern void sc1PTrainingModeUpdateDummyBehavior(void);
extern void sc1PTrainingModeFuncStart(void);
extern void sc1PTrainingModeFuncLights(Gfx **dls);
extern void sc1PTrainingModeStartScene(void);
extern void sc1PTrainingModeSetupFiles(void);

#endif
