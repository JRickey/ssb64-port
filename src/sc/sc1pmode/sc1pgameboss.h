#ifndef _SC1PGAMEBOSS_H_
#define _SC1PGAMEBOSS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <sc/scdef.h>

extern void func_ovl65_801910B0(void);
extern void sc1PGameBossSetChangeWallpaper(void);
extern void sc1PGameBossMakeCamera(void);
extern void SC1PGameBossWallpaper0ProcDisplay(GObj *gobj);
extern void SC1PGameBossWallpaper1ProcDisplay(GObj *gobj);
extern void SC1PGameBossWallpaper2ProcDisplay(GObj *gobj);
extern void SC1PGameBossWallpaper3ProcDisplay0(GObj *gobj);
extern void sc1PGameBossProcDisplayFadeAlpha(GObj *gobj);
extern void sc1PGameBossProcDisplayFadeColor(GObj *gobj);
extern void sc1PGameBossUpdateWallpaperColorID(void);
extern void SC1PGameBossWallpaper3ProcUpdate0(GObj *gobj);
extern void func_ovl65_80191B44(GObj *gobj);
extern void SC1PGameBossWallpaper0ProcUpdate(GObj *gobj);
extern void SC1PGameBossWallpaper1ProcUpdate(GObj *gobj);
extern void SC1PGameBossWallpaper2ProcUpdate0(GObj *gobj);
extern void SC1PGameBossWallpaper2ProcUpdate1(GObj *gobj);
extern void SC1PGameBossWallpaper3ProcUpdate1(GObj *gobj);
extern void sc1PGameBossSetupBackgroundDObjs(GObj *gobj, DObjDesc *dobjdesc, MObjSub ***p_mobjsubs, u8 transform_kind);
extern void sc1PGameBossSetWallpaperTranslate(GObj *gobj, s32 plan_id);
extern GObj* sc1PGameBossMakeWallpaperEffect(s32 effect_id, s32 anim_id, s32 plan_id);
extern void sc1PGameBossAdvanceWallpaper(void);
extern void sc1PGameBossWallpaperProcUpdate(GObj *gobj);
extern void sc1PGameBossSetBossPlayer(void);

#endif
