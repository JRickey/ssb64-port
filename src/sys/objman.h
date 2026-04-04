#ifndef _OBJMANAGER_H_
#define _OBJMANAGER_H_

#include <sys/objtypes.h>

extern s32 dGCCurrentStatus;
extern GCPersp dGCPerspDefault;
extern GCOrtho dGCOrthoDefault;
extern CObjVec dCObjVecDefault;
extern GCTranslate dGCTranslateDefault;
extern GCRotate dGCRotateDefaultAXYZ;
extern GCRotate dGCRotateDefaultRpy;
extern GCScale dGCScaleDefault;

extern GObj *gGCCommonLinks[GC_COMMON_MAX_LINKS];
extern GObj *gGCCommonDLLinks[GC_COMMON_MAX_DLLINKS];
extern GObj *gGCCurrentCommon; // Something to do with an initial object to be passed to a new GObjProcess
extern GObj *gGCCurrentCamera; // Is this exclusively a camera GObj?
extern GObj *gGCCurrentDisplay;
extern GObjProcess *gGCCurrentProcess;
extern OSMesgQueue gGCMesgQueue;
extern GCGfxLink gGCFrameQueueGfxLinks[64];

// 0x8003B878
extern GCPersp dGCPerspDefault;

// 0x8003B984
extern GCOrtho dGCOrthoDefault;

// 0x8003B8B4
extern CObjVec dGCCObjVecDefault;

// 0x8003B8DC
extern GCTranslate dGCTranslateDefault;

// 0x8003B8EC
extern GCRotate dGCRotateDefaultAXYZ;

// 0x8003B900
extern GCRotate dGCRotateDefaultRpy;

// 0x8003B914
extern GCScale dGCScaleDefault;

extern s32 gcGetGObjsActiveNum(void);
extern GObjThread* gcGetGObjThread(void);
extern void gcSetGObjThreadPrevAlloc(GObjThread *gobjthread);
extern GObjStack* gcGetGObjStackOfSize(size_t size);
extern GObjStack* gcGetDefaultGObjStack(void);
extern void gcEjectGObjStack(GObjStack *node);
extern GObjProcess* gcGetGObjProcess(void);
extern void gcLinkGObjProcess(GObjProcess *gobjproc);
extern void gcSetGObjProcessPrevAlloc(GObjProcess *gobjproc);
extern void func_80007784(GObjProcess *gobjproc);
extern void func_800077D0(GObjProcess *gobjproc);
extern GObjProcess *unref_80007840(void);
extern u64* gcGetGObjProcessThreadStack(GObjProcess *gobjproc);
extern u32 unref_80007884(GObjProcess *gobjproc);
extern void unref_800078BC(void (*proc)(GObjProcess *));
extern s32 gcGetGObjsActiveNum(void);
extern GObj* gcGetGObjSetNextAlloc(void);
extern void gcSetGObjPrevAlloc(GObj *gobj);
extern void gcLinkGObjAfter(GObj *this_gobj, GObj *link_gobj);
extern void gcLinkGObjSPAfter(GObj *this_gobj);
extern void gcLinkGObjSPBefore(GObj *this_gobj);
extern void gcRemoveGObjFromLinkedList(GObj *this_gobj);
extern void gcAppendGObjToDLLinkedList(GObj *this_gobj, GObj *dl_link_gobj);
extern void gcDLLinkGObjTail(GObj *this_gobj);
extern void gcDLLinkGObjHead(GObj *this_gobj);
extern void gcRemoveGObjFromDLLinkedList(GObj *this_gobj);
extern XObj* gcGetXObjSetNextAlloc(void);
extern void gcSetXObjPrevAlloc(XObj *xobj);
extern AObj* gcGetAObjSetNextAlloc(void);
extern void gcAppendAObjToDObj(DObj *dobj, AObj *aobj);
extern void gcAppendAObjToMObj(MObj *mobj, AObj *aobj);
extern void gcAppendAObjToCamera(CObj *cobj, AObj *aobj);
extern void gcSetAObjPrevAlloc(AObj *aobj);
extern MObj* gcGetMObjSetNextAlloc(void);
extern void gcSetMObjPrevAlloc(MObj *mobj);
extern DObj* gcGetDObjSetNextAlloc(void);
extern void gcSetDObjPrevAlloc(DObj *dobj);
extern SObj* gcGetSObjSetNextAlloc(void);
extern void gcSetSObjPrevAlloc(SObj *sobj);
extern CObj* gcGetCObjSetNextAlloc(void);
extern void gcSetCObjPrevAlloc(CObj *cobj);
extern GObjProcess* gcAddGObjProcess(GObj *gobj, void (*proc)(GObj*), u8 kind, u32 pri);
extern GObjProcess *unref_80008304(GObj *gobj, void (*proc)(GObj*), u32 pri, s32 thread_id, u32 stack_size);
extern void gcEndGObjProcess(GObjProcess *gobjproc);
extern XObj* gcAddXObjForDObjVar(DObj *dobj, u8 kind, u8 arg2, s32 xobj_id);
extern XObj* gcAddXObjForDObjFixed(DObj *dobj, u8 kind, u8 arg2);
extern XObj* gcAddXObjForCamera(CObj *cobj, u8 kind, u8 arg2);
extern AObj* gcAddAObjForDObj(DObj *dobj, u8 track);
extern void gcRemoveAObjFromDObj(DObj *dobj);
extern AObj* gcAddAObjForMObj(MObj *mobj, u8 track);
extern void gcRemoveAObjFromMObj(MObj *mobj);
extern AObj* gcAddAObjForCamera(CObj *cobj, u8 track);
extern void gcRemoveAObjFromCamera(CObj *cobj);
extern MObj* gcAddMObjForDObj(DObj *dobj, MObjSub *mobjsub);
extern void gcRemoveMObjAll(DObj *dobj);
extern void gcInitDObj(DObj *dobj);
extern DObj* gcAddDObjForGObj(GObj *gobj, void *dvar);
extern DObj* gcAddSiblingForDObj(DObj *dobj, void *dvar);
extern DObj* gcAddChildForDObj(DObj *dobj, void *dvar);
extern void gcEjectDObj(DObj *dobj);
extern SObj* gcAddSObjForGObj(GObj *gobj, Sprite *sprite);
extern void gcEjectSObj(SObj *sobj);
extern CObj* gcAddCameraForGObj(GObj *gobj);
extern void gcEjectCamera(CObj *cobj);
extern GObj* gcInitGObjCommon(u32 id, void (*func_run)(GObj*), u8 link, u32 priority);
extern GObj* gcMakeGObjSPAfter(u32 id, void (*func_run)(GObj*), u8 link, u32 priority);
extern GObj* gcMakeGObjSPBefore(u32 id, void (*func_run)(GObj*), u8 link, u32 priority);
extern GObj* gcMakeGObjAfter(u32 id, void (*func_run)(GObj*), GObj *link_gobj);
extern GObj* gcMakeGObjBefore(u32 id, void (*func_run)(GObj*), GObj *link_gobj);
extern void gcEjectGObj(GObj *gobj);
extern void gcMoveGObjCommon(s32 sw, GObj *this_gobj, u8 link, u32 priority, GObj *other_gobj);
extern void func_80009C90(GObj *gobj, u8 link, u32 priority);
extern void func_80009CC8(GObj *gobj, u8 link, u32 priority);
extern void unref_80009D00(GObj *this_gobj, GObj *other_gobj);
extern void unref_80009D3C(GObj *this_gobj, GObj *other_gobj);
extern void gcLinkGObjDLCommon(GObj *gobj, void (*proc_display)(GObj*), u8 dl_link, u32 dl_order, u32 camera_tag);
extern void gcAddGObjDisplay(GObj *gobj, void (*proc_display)(GObj*), u8 dl_link, u32 priority, u32 camera_tag);
extern void unref_80009E38(GObj *gobj, void (*proc_display)(GObj*), u8 dl_link, u32 priority, u32 camera_tag);
extern void unref_80009E7C(GObj *this_gobj, void (*proc_display)(GObj*), s32 arg2, GObj *other_gobj);
extern void unref_80009ED0(GObj *this_gobj, void (*proc_display)(GObj*), s32 arg2, GObj *other_gobj);
extern void func_80009F28(GObj *gobj, void (*proc_display)(GObj*), u32 priority, u64 arg3, u32 camera_tag);
extern void func_80009F74(GObj *gobj, void (*proc_display)(GObj*), u32 priority, u64 arg3, u32 camera_tag);
extern void unref_80009FC0(GObj *gobj, void (*proc_display)(GObj*), u32 priority, u64 arg3, u32 camera_tag);
extern void unref_8000A00C(GObj *this_gobj, void (*proc_display)(GObj*), u64 arg2, s32 arg3, GObj *other_gobj);
extern void unref_8000A06C(GObj *this_gobj, void (*proc_display)(GObj*), u64 arg2, s32 arg3, GObj *other_gobj);
extern void gcMoveGObjDL(GObj *gobj, u8 dl_link, u32 priority);
extern void gcMoveGObjDLHead(GObj *gobj, u8 dl_link, u32 priority);
extern void unref_8000A1C8(GObj *this_gobj, GObj *other_gobj);
extern void unref_8000A208(GObj *this_gobj, GObj *other_gobj);
extern void func_8000A24C(GObj *gobj, u32 priority);
extern void unref_8000A280(GObj *gobj, u32 priority);
extern void func_8000A2B4(GObj *this_gobj, GObj *other_gobj);
extern void unref_8000A2EC(GObj *this_gobj, GObj *other_gobj);
extern void gcSetMaxNumGObj(s32 num);
extern s16 gcGetMaxNumGObj(void);
extern void gcDrawAll(void);
extern GObj *gcRunGObj(GObj *gobj);
extern GObjProcess *gcRunGObjProcess(GObjProcess *gobjproc);
extern void gcRunAll(void);
extern void gcSetupObjman(GCSetup *setup);

#endif
