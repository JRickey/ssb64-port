#include <sys/obj.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8000AEF0
void gcFuncGObjByLink(s32 link, void (*func)(GObj*, u32), u32 param)
{
    GObj *current_gobj = gGCCommonLinks[link];

    while (current_gobj != NULL)
    {
        GObj *next_gobj = current_gobj->link_next;

        func(current_gobj, param);

        current_gobj = next_gobj;
    }
}

// 0x8000AF58
void gcFuncGObjAll(void (*func)(GObj*, u32), u32 param)
{
    s32 link;

    for (link = 0; link < ARRAY_COUNT(gGCCommonLinks); link++)
    {
        GObj *current_gobj = gGCCommonLinks[link];

        while (current_gobj != NULL)
        {
            GObj *next_gobj = current_gobj->link_next;

            func(current_gobj, param);

            current_gobj = next_gobj;
        }
    }
}

// 0x8000AFE4
GObj* gcFuncGObjByLinkEx(s32 link, GObj* (*func)(GObj*, u32), u32 param, sb32 is_return_immediate)
{
    GObj *current_gobj;
    GObj *next_gobj;
    GObj *return_gobj = NULL;

    current_gobj = gGCCommonLinks[link];

    while (current_gobj != NULL)
    {
        GObj *find_gobj;

        next_gobj = current_gobj->link_next;
        find_gobj = func(current_gobj, param);

        if (find_gobj != NULL)
        {
            return_gobj = find_gobj;

            if (is_return_immediate == TRUE)
            {
                return return_gobj;
            }
        }
        current_gobj = next_gobj;
    }
    return return_gobj;
}

// 0x8000B08C
GObj* gcFuncGObjAllEx(GObj* (*func)(GObj*, u32), u32 param, sb32 is_return_immediate)
{
    GObj *current_gobj;
    GObj *next_gobj;
    s32 link;
    GObj *return_gobj = NULL;

    for (link = 0; link < ARRAY_COUNT(gGCCommonLinks); link++)
    {
        current_gobj = gGCCommonLinks[link];

        while (current_gobj != NULL)
        {
            GObj *find_gobj;

            next_gobj = current_gobj->link_next;
            find_gobj = func(current_gobj, param);

            if (find_gobj != NULL)
            {
                return_gobj = find_gobj;

                if (is_return_immediate == TRUE)
                {
                    return return_gobj;
                }
            }
            current_gobj = next_gobj;
        }
    }
    return return_gobj;
}

// 0x8000B14C
GObj* gcGetGObjByID(GObj *gobj, u32 id)
{
    return (gobj->id == id) ? gobj : NULL;
}

// 0x8000B16C
GObj* gcFindGObjByLinkAndID(s32 link, u32 id)
{
    return gcFuncGObjByLinkEx(link, gcGetGObjByID, id, TRUE);
}

// 0x8000B198
GObj* gcFindGObjByID(u32 id)
{
    return gcFuncGObjAllEx(gcGetGObjByID, id, TRUE);
}

// 0x8000B1C4
void gcDefaultFuncRun(GObj *gobj)
{
    gcParseGObjScript(NULL);
}

// 0x8000B1E8
void gcSleepCurrentGObjThread(s32 tics)
{
    if (gGCCurrentProcess->exec.gobjthread->stack[7] != 0xFEDCBA98)
    {
        syDebugPrintf("gobjthread stack over  gobjid = %d\n", gGCCurrentProcess->parent_gobj->id);
    }
    while (tics != 0)
    {
        osSendMesg(&gGCMesgQueue, (OSMesg)1, OS_MESG_NOBLOCK);
        osStopThread(NULL);
        tics--;
    }
}

// 0x8000B284
void gcPauseGObjProcessAll(GObj *gobj)
{
    GObjProcess *gobjproc;

    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    gobjproc = gobj->gobjproc_head;

    while (gobjproc != NULL)
    {
        gobjproc->is_paused = TRUE;
        gobjproc = gobjproc->link_next;
    }
}

// 0x8000B2B8
void gcResumeGObjProcessAll(GObj *gobj)
{
    GObjProcess *gobjproc;

    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    gobjproc = gobj->gobjproc_head;

    while (gobjproc != NULL)
    {
        gobjproc->is_paused = FALSE;
        gobjproc = gobjproc->link_next;
    }
}

// 0x8000B2EC
void gcPauseGObjProcess(GObjProcess *gobjproc)
{
    if (gobjproc == NULL)
    {
        gobjproc = gGCCurrentProcess;
    }
    gobjproc->is_paused = TRUE;
}

// 0x8000B304
void gcResumeGObjProcess(GObjProcess *gobjproc)
{
    if (gobjproc == NULL)
    {
        gobjproc = gGCCurrentProcess;
    }
    gobjproc->is_paused = FALSE;
}

// 0x8000B31C
void gcPauseGObjProcessByProc(GObj *gobj, void (*func_id)(GObj*))
{
    GObjProcess *gobjproc;

    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    gobjproc = gobj->gobjproc_head;

    while (gobjproc != NULL)
    {
        if (gobjproc->func_id == func_id)
        {
            gobjproc->is_paused = TRUE;
        }
        gobjproc = gobjproc->link_next;
    }
}

// 0x8000B35C
void gcResumeGObjProcessByProc(GObj *gobj, void (*func_id)(GObj*))
{
    GObjProcess *gobjproc;

    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    gobjproc = gobj->gobjproc_head;

    while (gobjproc != NULL)
    {
        if (gobjproc->func_id == func_id)
        {
            gobjproc->is_paused = FALSE;
        }
        gobjproc = gobjproc->link_next;
    }
}

// 0x8000B39C
void gcEndProcessAll(GObj *gobj)
{
    GObjProcess *current_gobjproc, *next_gobjproc;

    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    current_gobjproc = gobj->gobjproc_head;

    while (current_gobjproc != NULL)
    {
        next_gobjproc = current_gobjproc->link_next;

        gcEndGObjProcess(current_gobjproc);

        current_gobjproc = next_gobjproc;
    }
}

// 0x8000B3EC
void gcAddDObjMatrixSetsRpyD(DObj* dobj)
{
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindTra, 0);
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindRotRpyD, 0);
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindSca, 0);
}

// 0x8000B434
void gcAddDObjMatrixSetsRpyR(DObj *dobj)
{
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindTra, 0);
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindRotRpyR, 0);
    gcAddXObjForDObjFixed(dobj, nGCMatrixKindSca, 0);
}

// 0x8000B47C
void gcAddCameraMatrixSets(CObj *cobj)
{
    gcAddXObjForCamera(cobj, nGCMatrixKindPerspFastF, 0);
    gcAddXObjForCamera(cobj, nGCMatrixKindLookAt, 0);
}

// 0x8000B4B4
void gcRemoveTreeMObjAll(GObj *gobj)
{
    DObj *current_dobj = DObjGetStruct(gobj);

    while (current_dobj != NULL)
    {
        gcRemoveMObjAll(current_dobj);

        current_dobj = gcGetTreeDObjNext(current_dobj);
    }
}

// 0x8000B4F8
DObj* gcAddDObjRpyD(GObj *gobj, void *dvar)
{
    DObj *dobj = gcAddDObjForGObj(gobj, dvar);
    gcAddDObjMatrixSetsRpyD(dobj);

    return dobj;
}

// 0x8000B528
DObj* gcAddDObjSiblingRpyD(DObj *dobj, void *dvar)
{
    DObj *sibling_dobj = gcAddSiblingForDObj(dobj, dvar);
    gcAddDObjMatrixSetsRpyD(sibling_dobj);

    return sibling_dobj;
}

// 0x8000B558
DObj* gcAddDObjChildRpyD(DObj *dobj, void *dvar)
{
    DObj *child_dobj = gcAddChildForDObj(dobj, dvar);
    gcAddDObjMatrixSetsRpyD(child_dobj);

    return child_dobj;
}

// 0x8000B588
DObj* gcAddDObjRpyR(GObj *gobj, void *dvar)
{
    DObj *dobj = gcAddDObjForGObj(gobj, dvar);
    gcAddDObjMatrixSetsRpyR(dobj);

    return dobj;
}

// 0x8000B5B8
DObj* gcAddDObjSiblingRpyR(DObj *dobj, void *dvar)
{
    DObj *sibling_dobj = gcAddSiblingForDObj(dobj, dvar);
    gcAddDObjMatrixSetsRpyR(sibling_dobj);

    return sibling_dobj;
}

// 0x8000B5E8
DObj* gcAddDObjChildRpyR(DObj *dobj, void *dvar)
{
    DObj *child_dobj = gcAddChildForDObj(dobj, dvar);
    gcAddDObjMatrixSetsRpyR(child_dobj);
    
    return child_dobj;
}

// 0x8000B618
void unref_8000B618(GObj *gobj, DObjTraDesc *dobjtra, DObj **dobjs)
{
    s32 i;
    DObj *dobj, *array_dobjs[18];
    
    for (i = 0; i < ARRAY_COUNT(array_dobjs); i++)
    {
        array_dobjs[i] = NULL;
    }
    while (dobjtra->id != 18)
    {
        if (dobjtra->id != 0)
        {
            dobj = array_dobjs[dobjtra->id] = gcAddDObjChildRpyD
            (
                array_dobjs[dobjtra->id - 1],
                dobjtra->dl
            );
        }
        else dobj = array_dobjs[0] = gcAddDObjRpyD(gobj, dobjtra->dl);
        
        dobj->translate.vec.f = dobjtra->translate;

        if (dobjs != NULL)
        {
            *dobjs++ = dobj;
        }
        dobjtra++;
    }
}

// 0x8000B70C
void gcRemoveDObjAll(GObj *gobj)
{
    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    while (DObjGetStruct(gobj) != NULL)
    {
        gcEjectDObj(DObjGetStruct(gobj));
    }
}

// 0x8000B760
void gcRemoveSObjAll(GObj *gobj)
{
    if (gobj == NULL)
    {
        gobj = gGCCurrentCommon;
    }
    while (SObjGetStruct(gobj) != NULL)
    {
        gcEjectSObj(SObjGetStruct(gobj));
    }
}

// 0x8000B7B4
void gcEjectAll(void)
{
    GObj *current_gobj;
    GObj *next_gobj;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gGCCommonLinks); i++)
    {
        current_gobj = gGCCommonLinks[i];

        while (current_gobj != NULL)
        {
            next_gobj = current_gobj->link_next;

            gcEjectGObj(current_gobj);

            current_gobj = next_gobj;
        }
    }
}

// 0x8000B824
GObj* gcMakeModelGObj
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
)
{
    GObj *gobj;
    DObj *dobj;

    gobj = gcMakeGObjSPAfter(id, func_run, link, link_priority);

    if (gobj == NULL)
    {
        return NULL;
    }
    gcAddGObjDisplay(gobj, proc_display, dl_link, dl_link_priority, camera_tag);

    dobj = gcAddDObjForGObj(gobj, dvar);

    if (is_add_default_xobj != FALSE)
    {
        gcAddDObjMatrixSetsRpyD(dobj);
    }
    if (proc != NULL)
    {
        gcAddGObjProcess(gobj, proc, gobjproc_kind, gobjproc_priority);
    }
    return gobj;
}

// 0x8000B8BC
GObj* gcMakeSpriteGObj
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
)
{
    GObj *gobj = gcMakeGObjSPAfter(id, func_run, link, link_priority);
        
    if (gobj == NULL)
    {
        return NULL;
    }
    gcAddGObjDisplay(gobj, proc_display, dl_link, dl_link_priority, camera_tag);
    
    gcAddSObjForGObj(gobj, sprite);
        
    if (proc != NULL)
    {
        gcAddGObjProcess(gobj, proc, gobjproc_kind, gobjproc_priority);
    }
    return gobj;
}

// 0x8000B93C
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
)
{
    GObj *gobj;
    CObj *cobj;

    gobj = gcMakeGObjSPAfter(id, func_run, link, link_priority);
    
    if (gobj == NULL)
    {
        return NULL;
    }
    func_80009F74(gobj, proc_display, dl_link_priority, camera_mask, camera_tag);
    
    cobj = gcAddCameraForGObj(gobj);

    if (is_add_default_xobj != FALSE)
    {
        gcAddCameraMatrixSets(cobj);
    }
    if (proc != NULL)
    {
        gcAddGObjProcess(gobj, proc, gobjproc_kind, gobjproc_priority);
    }
    if (argD != 0)
    {
        cobj->flags = 0x4 | COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER;
        cobj->color = GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF);
    }
    return gobj;
}

// 0x8000B9FC
GObj* gcMakeDefaultCameraGObj(s32 link, u32 link_priority, u32 dl_link_priority, u32 flags, u32 color)
{
    GObj *gobj = gcMakeCameraGObj
    (
        -1,
        gcDefaultFuncRun,
        link,
        link_priority,
        func_80017DBC,
        dl_link_priority,
        0,
        0,
        FALSE,
        nGCProcessKindThread,
        NULL,
        0,
        FALSE
    );
    CObj *cobj;
    
    if (gobj == NULL)
    {
        return NULL;
    }
    cobj = CObjGetStruct(gobj);
    
    cobj->flags = flags;
    cobj->color = color;
    
    return gobj;
}
