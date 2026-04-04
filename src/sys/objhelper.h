#ifndef _OBJHELPER_H_
#define _OBJHELPER_H_

#include <sys/objtypes.h>

extern void gcFuncGObjByLink(s32 link, void (*func)(GObj*, u32), u32 param);
extern void gcFuncGObjAll(void (*func)(GObj*, u32), u32 param);
extern GObj* gcFuncGObjByLinkEx(s32 link, GObj* (*func)(GObj*, u32), u32 param, sb32 is_return_immediate);
extern GObj* gcFuncGObjAllEx(GObj* (*func)(GObj*, u32), u32 param, sb32 is_return_immediate);
extern GObj* gcGetGObjByID(GObj *gobj, u32 id);
extern GObj* gcFindGObjByLinkAndID(s32 link, u32 id);
extern GObj* gcFindGObjByID(u32 id);
extern void gcDefaultFuncRun(GObj *gobj);
extern void gcSleepCurrentGObjThread(s32 tics);
extern void gcPauseGObjProcessAll(GObj *gobj);
extern void gcResumeGObjProcessAll(GObj *gobj);
extern void gcPauseGObjProcess(GObjProcess *gobjproc);
extern void gcResumeGObjProcess(GObjProcess *gobjproc);
extern void gcPauseGObjProcessByProc(GObj *gobj, void (*func_id)(GObj*));
extern void gcResumeGObjProcessByProc(GObj *gobj, void (*func_id)(GObj*));
extern void gcEndProcessAll(GObj *gobj);
extern void gcAddDObjMatrixSetsRpyD(DObj* dobj);
extern void gcAddDObjMatrixSetsRpyR(DObj* dobj);
extern void gcAddCameraMatrixSets(CObj *cobj);
extern void gcRemoveTreeMObjAll(GObj *gobj);
extern DObj* gcAddDObjRpyD(GObj *gobj, void *dvar);
extern DObj* gcAddDObjSiblingRpyD(DObj *dobj, void *dvar);
extern DObj* gcAddDObjChildRpyD(DObj *dobj, void *dvar);
extern DObj* gcAddDObjRpyR(GObj *gobj, void *dvar);
extern DObj* gcAddDObjSiblingRpyR(DObj *dobj, void *dvar);
extern DObj* gcAddDObjChildRpyR(DObj *dobj, void *dvar);
extern void gcRemoveDObjAll(GObj *gobj);
extern void gcRemoveSObjAll(GObj *gobj);
extern void gcEjectAll(void);
extern GObj* gcMakeModelGObj
(
    u32 id,
    void (*func_run)(GObj*),
    s32 link,
    u32 link_priority,
    void (*proc_display)(GObj*),
    u8 dl_link,
    u32 dl_link_priority,
    u32 camera_tag,
    void *dvar,
    sb32 is_add_default_xobj,
    u8 gobjproc_kind,
    void (*proc)(GObj*),
    u32 gobjproc_priority
);
extern GObj* gcMakeSpriteGObj
(
    u32 id,
    void (*func_run)(GObj*),
    s32 link,
    u32 link_priority,
    void (*proc_display)(GObj*),
    s32 dl_link,
    u32 dl_link_priority,
    u32 camera_tag,
    Sprite *sprite,
    u8 gobjproc_kind,
    void (*proc)(GObj*),
    u32 gobjproc_priority
);
GObj* gcMakeCameraGObj
(
    u32 id,
    void (*func_run)(GObj*),
    s32 link,
    u32 link_priority,
    void (*proc_display)(GObj*),
    u32 dl_link_priority,
    u64 camera_mask,
    u32 camera_tag,
    sb32 is_add_default_xobj,
    u8 gobjproc_kind,
    void (*proc)(GObj*),
    u32 gobjproc_priority,
    sb32 argD
);
extern GObj* gcMakeDefaultCameraGObj(s32 link, u32 link_priority, u32 dl_link_priority, u32 flags, u32 color);

#endif
