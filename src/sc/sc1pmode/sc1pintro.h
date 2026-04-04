#ifndef _SC1PSTAGECARD_H_
#define _SC1PSTAGECARD_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>
#include <ft/ftdef.h>

extern void sc1PIntroFuncLights(Gfx **dls);
extern void sc1PIntroMakeSky(void);
extern void sc1PIntroMakeBanners(void);
extern void func_ovl24_80131C58(GObj *gobj);
extern void sc1PIntroMakeVSDecal(void);
extern sb32 sc1PIntroCheckNotBonusStage(s32 stage);
extern void sc1PIntroMakeLabels(s32 stage);
extern void sc1PIntroMakeFigures(s32 stage);
extern void sc1PIntroMakeBonusTasks(s32 stage);
extern void sc1PIntroMakeVSName(s32 stage);
extern s32 sc1PIntroGetAlliesNum(s32 stage);
extern void sc1PIntroMakeName(s32 stage);
extern s32 sc1PIntroGetPlayerNameOffsetX(s32 stage);
extern s32 sc1PIntroGetVSNameOffsetX(s32 stage);
extern s32 sc1PIntroGetTotalNameOffsetX(s32 stage);
extern void sc1PIntroSetNamePositions(s32 stage);
extern void sc1PIntroMakeNameAll(s32 stage);
extern void sc1PIntroMakeStageInfo(s32 stage);
extern f32 sc1PIntroGetFighterVelocityZ(s32 card_anim_frame_id);
extern void sc1PIntroFighterProcUpdate(GObj *fighter_gobj);
extern f32 sc1PIntroGetFighterPositionZ(s32 card_anim_frame_id);
extern void sc1PIntroMakeFighter(FTDemoDesc fighter, s32 card_anim_frame_id, void **figatree);
extern void sc1PIntroInitAllyTextParams(SObj *sobj);
extern void sc1PIntroMakeAllyText(s32 stage);
extern f32 sc1PIntroGetVSFighterVelocityZ(s32 stage, s32 fkind);
extern void sc1PIntroVSFighterProcUpdate(GObj *fighter_gobj);
extern void sc1PIntroSetKirbyTeamModelPartIDs(GObj *fighter_gobj, s32 fkind);
extern f32 sc1PIntroGetVSFighterPositionZ(s32 stage, s32 fkind);
extern void sc1PIntroVSFighterProcDisplay(GObj *fighter_gobj);
extern GObj* sc1PIntroMakeVSFighter(s32 fkind, s32 stage, s32 card_anim_frame_id, void **figatree, u8 dl_link);
extern void sc1PIntroMakeBonusPicture(s32 stage);
extern CObjDesc* sc1PIntroGetStageCObjDesc(CObjDesc *cobj_desc, s32 stage);
extern CObj* sc1PIntroMakeStageCamera(s32 stage, u32 dl_link);
extern sb32 sSC1PIntroCheckCostumeUsed(s32 stage, s32 fkind, s32 color);
extern void sc1PIntroInitVSFighters(s32 stage);
extern CObjDesc* sc1PIntroGetFighterCObjDesc(CObjDesc *cobj_desc, s32 fkind, s32 cobj_id);
extern void sc1PIntroMakeFighterCamera(s32 fkind, s32 cobj_id);
extern void sc1PIntroInitFighters(s32 stage);
extern void sc1PIntroMakeBannersCamera(void);
extern void sc1PIntroMakeDecalsCamera(void);
extern void sc1PIntroMakePicturesCamera(void);
extern s32 sc1PIntroGetFighterAllocsNum(s32 stage);
extern void sc1PIntroUpdateAnnounce(void);
extern void sc1PIntroInitVars(void);
extern void sc1PIntroFuncRun(GObj *gobj);
extern void sc1PIntroSetupFighterFiles(s32 stage);
extern void sc1PIntroFuncStart(void);
extern void sc1PIntroStartScene(void);

#endif
