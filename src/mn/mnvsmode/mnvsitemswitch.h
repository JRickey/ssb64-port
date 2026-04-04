#ifndef _MNVSITEMSWITCH_H_
#define _MNVSITEMSWITCH_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnVSItemSwitchFuncLights(Gfx **dls);
extern void mnVSItemSwitchSetToggleSpriteColors(GObj *gobj, s32 status);
extern void mnVSItemSwitchMakeToggle(GObj *gobj, f32 pos_x, f32 pos_y);
extern void mnVSItemSwitchLabelsProcDisplay(GObj *gobj);
extern void mnVSItemSwitchMakeLabels(void);
extern void mnItemSwitchSetSubtitleSpriteColors(SObj *sobj);
extern void mnItemSwitchMakeSubtitle(s32 option_id);
extern void mnVSItemSwitchMakeDecal(void);
extern void mnVSItemSwitchMakeItemList(void);
extern void mnVSItemSwitchSetCursorPosition(GObj *gobj, s32 option_id);
extern void mnVSItemSwitchMakeCursor(s32 off_y);
extern void mnVSItemSwitchMakeAppearance(s32 rate);
extern void mnVSItemSwitchInitToggles(void);
extern void mnVSItemSwitchMakeCursorCamera(void);
extern void mnVSItemSwitchMakeLabelsCamera(void);
extern void mnVSItemSwitchMakeDecalCamera(void);
extern void mnVSItemSwitchGetItemSettings(void);
extern void mnVSItemSwitchSetItemSettings(void);
extern void mnVSItemSwitchInitVars(void);
extern sb32 mnVSItemSwitchCheckAllTogglesOff(void);
extern void mnVSItemSwitchSetItemToggles(void);
extern void mnVSItemSwitchUpdateOption(s32 option_id, s32 rate);
extern void mnVSItemSwitchFuncRun(GObj *gobj);
extern void mnVSItemSwitchFuncStart(void);
extern void mnVSItemSwitchStartScene(void);

#endif
