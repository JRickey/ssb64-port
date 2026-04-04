#ifndef _IFCOMMON_H_
#define _IFCOMMON_H_

#include <ssb_types.h>
#include <PR/gbi.h>
#include <sys/objdef.h>
#include <if/ifdef.h>
#include <ft/ftdef.h>
#include <it/itdef.h>

extern u8 dIFCommonPlayerTeamColorIDs[/* */];
extern intptr_t dIFCommonTimerDigitSpriteOffsets[/* */];
extern u8 dIFCommonPlayerTagPrimColorsR[/* */];
extern u8 dIFCommonPlayerTagPrimColorsG[/* */];
extern u8 dIFCommonPlayerTagPrimColorsB[/* */];
extern u8 dIFCommonPlayerTagEnvColorsR[/* */];
extern u8 dIFCommonPlayerTagEnvColorsG[/* */];
extern u8 dIFCommonPlayerTagEnvColorsB[/* */];

extern void ifCommonPlayerDamageSetShowInterface(void);
extern s32 ifCommonPlayerDamageGetSpecialArrayID(s32 damage, u8 *digits);
extern s32 ifCommonPlayerDamageGetPercentArrayID(s32 damage, u8 *digits);
extern s32 ifCommonPlayerDamageGetHitPointsArrayID(s32 damage, u8 *digits);
extern s32 ifCommonPlayerDamageGetDigitOffset(s32 digit_count, u8 *digit_ids);
extern void ifCommonPlayerDamageUpdateDigits(GObj *interface_gobj);
extern void ifCommonPlayerDamageUpdateAnim(GObj *interface_gobj);
extern void ifCommonPlayerDamageProcUpdate(GObj *interface_gobj);
extern void ifCommonPlayerDamageProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerDamageSetDigitAttr(void);
extern void ifCommonPlayerDamageSetDigitPositions(void);
extern void ifCommonPlayerDamageInitInterface(void);
extern void ifCommonPlayerDamageStartBreakAnim(FTStruct *fp);
extern void ifCommonPlayerDamageStopBreakAnim(FTStruct *fp);
extern void ifCommonPlayerStockMultiProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerStockSetIconAttr(void);
extern void ifCommonPlayerStockMultiMakeInterface(s32 player);
extern void ifCommonPlayerStockSingleProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerStockSetLUT(s32 player, s32 lut_id, FTAttributes *attr);
extern void ifCommonPlayerStockSingleMakeInterface(s32 player);
extern void ifCommonPlayerStockStealProcUpdate(GObj *interface_gobj);
extern void ifCommonPlayerStockStealMakeInterface(s32 thief, s32 stolen);
extern void ifCommonPlayerStockInitInterface(void);
extern void ifCommonPlayerMagnifyGetPosition(f32 player_pos_x, f32 player_pos_y, Vec2f *magnify_pos);
extern void ifCommonPlayerMagnifyUpdateRender(Gfx **dls, s32 color_id, f32 ulx, f32 uly);
extern void ifCommonPlayerMagnifyUpdateViewport(Gfx **dls, FTStruct *fp);
extern void ifCommonPlayerMagnifyProcDisplay(FTStruct *fp);
extern void ifCommonPlayerMagnifyMakeInterface(void);
extern void ifCommonPlayerArrowsLeftProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerArrowsRightProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerArrowsAddAnim(GObj *interface_gobj);
extern void ifCommonPlayerArrowsLeftProcUpdate(GObj *interface_gobj);
extern void ifCommonPlayerArrowsRightProcUpdate(GObj *interface_gobj);
extern GObj* ifCommonPlayerArrowsMakeInterface(void (*proc_display)(GObj*), void (*proc_update)(GObj*));
extern void ifCommonPlayerArrowsFuncRun(GObj *interface_gobj);
extern void ifCommonPlayerArrowsMainProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerArrowsInitInterface(void);
extern void ifCommonPlayerArrowsUpdateFlags(f32 x, f32 y);
extern void ifCommonPlayerTagProcDisplay(GObj *interface_gobj);
extern void ifCommonPlayerTagMakeInterface(void);
extern void ifCommonItemArrowProcDisplay(GObj *interface_gobj);
extern GObj* ifCommonItemArrowMakeInterface(ITStruct *ip);
extern void ifCommonItemArrowSetAttr(void);
extern void ifCommonAnnounceThread(GObj *interface_gobj);
extern void ifCommonAnnounceSetAttr(GObj *interface_gobj, s32 file_id, IFACharacter *character, s32 sprite_count);
extern void ifCommonAnnounceGoMakeInterface(void);
extern void ifCommonAnnounceGoSetStatus(void);
extern SObj* ifCommonTrafficMakeSObj(GObj *interface_gobj, s32 id);
extern void ifCommonCountdownThread(GObj *interface_gobj);
extern SObj* ifCommonCountdownMakeInterface(void);
extern GObj* ifCommonAnnounceTimeUpMakeInterface(void);
extern void ifCommonEntryFocusThread(GObj *interface_gobj);
extern void ifCommonEntryFocusMakeInterface(s32 id);
extern void ifCommonEntryAllThread(GObj *interface_gobj);
extern void ifCommonEntryAllMakeInterface(void);
extern void ifCommonSuddenDeathThread(GObj *interface_gobj);
extern void ifCommonAnnounceSetColors(GObj *interface_gobj, SYColorRGBPair *colors);
extern void ifCommonSuddenDeathMakeInterface(void);
extern void ifCommonTimerProcDisplay(GObj *interface_gobj);
extern void ifCommonTimerSetAttr(void);
extern void ifCommonTimerInitAnnouncedSeconds(void);
extern SObj* ifCommonTimerMakeDigits(void);
extern void ifCommonTimerFuncRun(GObj *interface_gobj);
extern void ifCommonTimerMakeInterface(void (*proc)(void));
extern GObj* ifCommonAnnounceGameSetMakeInterface(void);
extern void ifCommonBattleInitPlacement(void);
extern void ifCommonBattleInterfacePauseGObj(GObj *interface_gobj, u32 unused);
extern void ifCommonBattleInterfaceResumeGObj(GObj *interface_gobj, u32 unused);
extern void ifCommonBattleInterfaceProcUpdate(void);
extern void ifCommonBattleEndInitSoundNum(void);
extern void ifCommonBattleEndPlaySoundQueue(void);
extern void ifCommonBattleEndAddSoundQueueID(u16 sfx_id);
extern void ifCommonBattleEndSetBossDefeat(void);
extern void ifCommonBattleUpdateScoreStocks(FTStruct *fp);
extern void ifCommonBattlePauseProcDisplay(GObj *interface_gobj);
extern void ifCommonBattlePausePlayerNumMakeSObj(GObj *interface_gobj, s32 player);
extern void ifCommonBattlePauseDecalMakeSObjID(GObj *interface_gobj, s32 id);
extern void ifCommonBattlePauseMakeSObjsAll(GObj *interface_gobj);
extern void ifCommonBattlePauseMakeInterface(s32 player);
extern void ifCommonBattlePauseEjectGObjs(void);
extern void ifCommonInterfaceSetGObjFlagsAll(u32 flags);
extern void ifCommonBattlePauseSetGObjFlagsAll(u32 flags);
extern void ifCommonBattlePauseInitInterface(s32 player);
extern void ifCommonBattleGoUpdateInterface(void);
extern void ifCommonBattleInterfaceProcSet(void);
extern void ifCommonBattlePauseUpdateInterface(void);
extern void ifCommonBattlePauseRestoreInterfaceAll(void);
extern void ifCommonBattleEndUpdateInterface(void);
extern void ifCommonBattleBossDefeatUpdateInterface(void);
extern void ifCommonBattleSetUpdateInterface(void);
extern void ifCommonSetMaxNumGObj(void);
extern void ifCommonBattleUpdateInterfaceAll(void);
extern void ifCommonBattleSetGameStatusWait(void);
extern void ifCommonPlayerStockMakeStockSnap(FTStruct *fp);
extern void ifCommonPlayerScoreMakeEffect(FTStruct *fp, s32 score);
extern GObj* ifCommonAnnounceFailureMakeInterface(void);
extern GObj* ifCommonAnnounceCompleteMakeInterface(void);
extern void ifCommonBonusInterfaceProcUpdate(void);
extern void ifCommonBattleSetInterface(void (*proc_update)(void), void (*proc_set)(void), u16 sfx_id, u16 restore_wait);
extern void ifCommonBattleBossDefeatSetGameStatus(void);
extern void ifCommon1PGameInterfaceProcSet(void);
extern void ifCommonAnnounceEndMessage(void);
extern void ifCommonAnnounceCompleteInitInterface(u16 sfx_id);
extern void ifCommonAnnounceTimeUpInitInterface(void);
extern void ifCommonAnnounceFailureInitInterface(void);

#endif
