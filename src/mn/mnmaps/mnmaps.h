#ifndef _MNMAPS_H_
#define _MNMAPS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mp/mpdef.h>
#include <mn/mndef.h>

extern void mnMapsAllocModelHeaps(void);
extern void mnMapsFuncLights(Gfx **dls);
extern sb32 mnMapsCheckLocked(s32 gkind);
extern s32 mnMapsGetCharacterID(const char c);
extern f32 mnMapsGetCharacterSpacing(const char *str, s32 c);
extern void mnMapsMakeString(GObj *gobj, const char *str, f32 x, f32 y, u32 *color);
extern void mnMapsMakeWallpaper(void);
extern void mnMapsMakePlaque(void);
extern void mnMapsLabelsProcDisplay(GObj *gobj);
extern void mnMapsMakeLabels(void);
extern s32 mnMapsGetGroundKind(s32 slot);
extern s32 mnMapsGetSlot(s32 gkind);
extern void mnMapsMakeIcons(void);
extern void mnMapsSetNamePosition(SObj *sobj, s32 gkind);
extern void mnMapsMakeName(GObj *gobj, s32 gkind);
extern void mnMapsSetLogoPosition(GObj *gobj, s32 gkind);
extern void mnMapsMakeEmblem(GObj *gobj, s32 gkind);
extern void mnMapsMakeNameAndEmblem(s32 slot);
extern void mnMapsSetCursorPosition(GObj *gobj, s32 slot);
extern void mnMapsMakeCursor(void);
extern void mnMapsLoadMapFile(s32 gkind, void *heap);
extern void mnMapsPreviewWallpaperProcDisplay(GObj *gobj);
extern GObj* mnMapsMakePreviewWallpaper(s32 gkind);
extern void mnMapsModelPriProcDisplay(GObj *gobj);
extern void mnMapsModelSecProcDisplay(GObj *gobj);
extern GObj* mnMapsMakeLayer(s32 gkind, MPGroundData *ground_data, MPGroundDesc *ground_desc, s32 id);
extern void mnMapsMakeModel(s32 gkind, MPGroundData *ground_data, s32 heap_id);
extern void mnMapsDestroyPreview(s32 heap_id);
extern void mnMapsMakePreview(s32 gkind);
extern void mnMapsMakeWallpaperCamera(void);
extern void mnMapsMakePlaqueCamera(void);
extern void mnMapsMakePreviewWallpaperCamera(void);
extern void mnMapsMakeLabelsViewport(void);
extern void mnMapsMakeIconsCamera(void);
extern void mnMapsMakeNameAndEmblemCamera(void);
extern void mnMapsMakeCursorCamera(void);
extern void mnMapsSetPreviewCameraPosition(CObj *cobj, s32 gkind);
extern void mnMapsPreviewCameraThreadUpdate(GObj *gobj);
extern void mnMapsMakePreviewCamera(void);
extern void mnMapsSaveSceneData(void);
extern void mnMapsInitVars(void);
extern void mnMapsSaveSceneData2(void);
extern void mnMapsFuncRun(GObj *gobj);
extern void mnMapsFuncStart(void);
extern void mnMapsStartScene(void);

#endif
