#ifndef _MNCHARACTERS_H_
#define _MNCHARACTERS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnCharactersFuncLights(Gfx **dls);
extern s32 mnCharactersGetFighterKind(s32 page);
extern s32 mnCharactersGetPage(s32 fkind);
extern void mnCharactersMakeStory(s32 fkind);
extern void mnCharactersMakeDecals(void);
extern void mnCharactersMakeEmblem(s32 fkind);
extern void mnCharactersMakeName(s32 fkind);
extern void mnCharactersMakeWorksWallpaper(void);
extern void mnCharactersMakeWorks(s32 fkind);
extern void mnCharactersSetFighterScale(GObj *fighter_gobj, s32 fkind);
extern void mnCharactersSetFighterPosition(GObj *fighter_gobj, s32 fkind);
extern MNCharactersMotion* mnCharactersGetMotion(MNCharactersMotion *motion, s32 fkind, s32 motion_kind, s32 unused, s32 track);
extern sb32 mnCharactersCheckFighterAnimEnd(GObj *fighter_gobj);
extern void mnCharactersInitRecentMotionKinds(void);
extern sb32 mnCharactersCheckRecentMotionKind(s32 motion_kind);
extern s32 mnCharactersRandMotionKind(void);
extern MNCharactersMotion* mnCharactersSetMotion(MNCharactersMotion *motion, s32 motion_kind);
extern MNCharactersMotion* mnCharactersAdvanceTrack(MNCharactersMotion *motion, s32 unused);
extern void mnCharactersFighterProcUpdate(GObj *fighter_gobj);
extern void mnCharactersMakeFighter(s32 fkind);
extern s32 mnCharactersGetMotionKind(void);
extern void mnCharactersUpdateMotionName(GObj *gobj);
extern void mnCharactersMakeMotionName(void);
extern void mnCharactersMakeStoryCamera(void);
extern void mnCharactersMakeDecalsCamera(void);
extern void mnCharactersMakeEmblemCamera(void);
extern void mnCharactersMakeNameCamera(void);
extern void mnCharactersMakeWorksWallpaperCamera(void);
extern void mnCharactersMakeWorksCamera(void);
extern void mnCharactersMakeFighterCamera(void);
extern sb32 mnCharactersCheckHaveFighterKind(s32 fkind);
extern void mnCharactersInitVars(void);
extern void mnCharactersChangeFighter(s32 fkind);
extern void mnCharactersMoveFighterCamera(CObj *cobj, f32 angle, s32 unused);
extern void mnCharactersResetFighterCamera(void);
extern void mnCharactersUpdateScene(void);
extern void mnCharactersUpdateSceneDemo(void);
extern void mnCharactersFuncRun(GObj *gobj);
extern void mnCharactersFuncStart(void);
extern void mnCharactersStartScene(void);

#endif
