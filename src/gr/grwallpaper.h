#ifndef _GRWALLPAPER_H_
#define _GRWALLPAPER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern void grWallpaperCalcPersp(SObj *wallpaper_sobj);
extern void grWallpaperCommonProcUpdate(GObj *wallpaper_gobj);
extern void grWallpaperMakeCommon(void);
extern void grWallpaperMakeStatic(void);
extern void grWallpaperSectorProcUpdate(GObj *wallpaper_gobj);
extern void grWallpaperMakeSector(void);
extern void grWallpaperBonus3ProcDisplay(GObj *wallpaper_gobj);
extern void grWallpaperMakeBonus3(void);
extern void grWallpaperMakeDecideKind(void);
extern void grWallpaperPausePerspUpdate(void);
extern void grWallpaperResumePerspUpdate(void);
extern void grWallpaperRunProcessAll(void);
extern void grWallpaperResumeProcessAll(void);

#endif
