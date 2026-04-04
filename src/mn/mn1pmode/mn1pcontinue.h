#ifndef _MN1PCONTINUE_H_
#define _MN1PCONTINUE_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <PR/sp.h>
#include <mn/mndef.h>

extern void mnPlayers1PGameContinueFuncLights(Gfx **dls);
extern s32 mnPlayers1PGameContinueGetPowerOf(s32 base, s32 exp);
extern void mnPlayers1PGameContinueScoreDigitInitSprite(SObj *sobj);
extern s32 mnPlayers1PGameContinueGetScoreDigitCount(s32 points, s32 digit_count_max);
extern Sprite* mnPlayers1PGameContinueScoreDigitGetSprite(s32 digit);
extern void mnPlayers1PGameContinueMakeScoreDigits
(
    GObj *gobj,
    s32 points,
    f32 x,
    f32 y,
    f32 offset_x,
    s32 sub,
    s32 digit_count,
    sb32 is_fixed_digit_count
);
extern void mnPlayers1PGameContinueMakeScoreDisplay(s32 points);
extern void mnPlayers1PGameContinueSetFighterScale(GObj *gobj, s32 fkind);
extern void mnPlayers1PGameContinueMakeFighter(s32 fkind);
extern void mnPlayers1PGameContinueRoomFadeOutProcDisplay(GObj *gobj);
extern void mnPlayers1PGameContinueMakeRoomFadeOut(void);
extern void mnPlayers1PGameContinueRoomFadeInProcDisplay(GObj *gobj);
extern void mnPlayers1PGameContinueMakeRoomFadeIn(void);
extern void mnPlayers1PGameContinueSpotlightFadeProcDisplay(GObj *gobj);
extern void mnPlayers1PGameContinueMakeSpotlightFade(void);
extern void mnPlayers1PGameContinueMakeRoom(void);
extern void mnPlayers1PGameContinueMakeSpotlight(void);
extern void mnPlayers1PGameContinueMakeContinue(void);
extern void MN1PContinueOptionSetHighlightColors(GObj *gobj, s32 option);
extern void MN1PContinueOptionProcUpdate(GObj *gobj);
extern void mnPlayers1PGameContinueMakeOptions(void);
extern void mnPlayers1PGameContinueCursorSetPosition(GObj *gobj, s32 option);
extern void mnPlayers1PGameContinueCursorProcUpdate(GObj *gobj);
extern void mnPlayers1PGameContinueMakeCursor(void);
extern void mnPlayers1PGameContinueGameOverInitSprites(SObj *sobj);
extern void mnPlayers1PGameContinueGameOverTextStepColors(GObj *gobj);
extern void mnPlayers1PGameContinueMakeGameOverText(void);
extern void mnPlayers1PGameContinueGameOverProcUpdate(GObj *gobj);
extern void mnPlayers1PGameContinueMakeGameOver(void);
extern void mnPlayers1PGameContinueMakeRoomFadeInCamera(void);
extern void mnPlayers1PGameContinueMakeSpotlightFadeCamera(void);
extern void mnPlayers1PGameContinueMakeRoomFadeOutCamera(void);
extern void mnPlayers1PGameContinueSetupCamera(CObj *cobj);
extern void mnPlayers1PGameContinueMakeMainCamera(void);
extern void mnPlayers1PGameContinueMakeRoomCamera(void);
extern void mnPlayers1PGameContinueMakeSpotlightCamera(void);
extern void mnPlayers1PGameContinueMakeTextCamera(void);
extern void mnPlayers1PGameContinueInitVars(void);
extern void mnPlayers1PGameContinueUnused0x80133990(void);
extern void mnPlayers1PGameContinueFuncRun(GObj *gobj);
extern void mnPlayers1PGameContinueFuncStart(void);
extern void mnPlayers1PGameContinueStartScene(void);

#endif
