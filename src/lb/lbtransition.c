#include <lb/library.h>
#include <sys/video.h>
#include <reloc_data.h>

extern void *gSYSchedulerCurrentFramebuffer;
extern syRdpSetViewport(void*, f32, f32, f32, f32);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x800D5D60
LBTransitionDesc dLBTransitionDescs[/* */] =
{
    // Paper Plane
    {
        &llLBTransitionAeroplaneFileID,
        &llLBTransitionAeroplaneDObjDesc,
        &llLBTransitionAeroplaneAnimJoint,
        0
    },

    // Checkered Board
    {
        &llLBTransitionCheckFileID,
        &llLBTransitionCheckDObjDesc,
        &llLBTransitionCheckAnimJoint,
        0
    },

    // Falling Board
    {
        &llLBTransitionGakubuthiFileID,
        &llLBTransitionGakubuthiDObjDesc,
        &llLBTransitionGakubuthiAnimJoint,
        0
    },

    // Doors
    {
        &llLBTransitionKannonFileID,
        &llLBTransitionKannonDObjDesc,
        &llLBTransitionKannonAnimJoint,
        0
    },

    // Star
    {
        &llLBTransitionStarFileID,
        &llLBTransitionStarDObjDesc,
        &llLBTransitionStarAnimJoint,
        0
    },

    // Vertical Lines
    {
        &llLBTransitionSudare1FileID,
        &llLBTransitionSudare1DObjDesc,
        &llLBTransitionSudare1AnimJoint,
        0
    },

    // Diagonal Lines
    {
        &llLBTransitionSudare2FileID,
        &llLBTransitionSudare2DObjDesc,
        &llLBTransitionSudare2AnimJoint,
        0
    },

    // Camera Shutter
    {
        &llLBTransitionCameraFileID,
        &llLBTransitionCameraDObjDesc,
        &llLBTransitionCameraAnimJoint,
        0
    },

    // Collapsing Blocks
    {
        &llLBTransitionBlockFileID,
        &llLBTransitionBlockDObjDesc,
        &llLBTransitionBlockAnimJoint,
        0
    },

    // Rotating Frame Zooming Out
    {
        &llLBTransitionRotScaleFileID,
        &llLBTransitionRotScaleDObjDesc,
        &llLBTransitionRotScaleAnimJoint,
        0
    },

    // Curtain
    {
        &llLBTransitionCurtainFileID,
        &llLBTransitionCurtainDObjDesc,
        &llLBTransitionCurtainAnimJoint,
        0
    }
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D6480
s32 sLBTransitionPad0x800D6480;

// 0x800D6484
void *sLBTransitionFileHeap;

// 0x800D6488 - Heap for "photocopy" of last frame drawn to framebuffer
void *sLBTransitionPhotoHeap;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D4130
GObj* lbTransitionMakeCamera(u32 id, s32 link, u32 link_priority, u64 camera_mask)
{
    GObj *gobj;
    CObj *cobj;

    gobj = gcMakeGObjSPAfter(id, NULL, link, GOBJ_PRIORITY_DEFAULT);
    func_80009F74(gobj, func_80017DBC, link_priority, camera_mask, -1);
    
    cobj = gcAddCameraForGObj(gobj);
    gcAddXObjForCamera(cobj, nGCMatrixKindPerspFastF, 1);
    gcAddXObjForCamera(cobj, nGCMatrixKindLookAt, 1);
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);

    cobj->projection.persp.aspect = 15.0F / 11.0F;
    cobj->projection.persp.fovy = 45.0F;
    
    cobj->vec.eye.z = 1100.0F / syUtilsTan(F_CLC_DTOR32(cobj->projection.persp.fovy * 0.5F));

    cobj->flags |= COBJ_FLAG_DLBUFFERS | COBJ_FLAG_ZBUFFER;
    
    return gobj;
}

// 0x800D4248
void lbTransitionProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gSPSegment(gSYTaskmanDLHeads[0]++, 0x1, sLBTransitionPhotoHeap);
    
    gcDrawDObjTreeForGObj(gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x800D42C8
void lbTransitionProcUpdate(GObj *gobj)
{
    gcPlayAnimAll(gobj);
    
    if (gobj->anim_frame <= 0.0F)
    {
        gcEjectGObj(gobj);
    }
}

// 0x800D430C
GObj* lbTransitionMakeTransition(s32 transition_id, u32 id, s32 link, void (*proc_display)(GObj*), u8 dl_link_id, void (*func)(GObj*))
{
    LBTransitionDesc *transition_desc = &dLBTransitionDescs[transition_id];
    GObj *gobj;

    lbRelocGetExternHeapFile(transition_desc->file_id, sLBTransitionFileHeap);
    gobj = gcMakeGObjSPAfter(id, NULL, link, GOBJ_PRIORITY_DEFAULT);
    
    gobj->user_data.s = transition_desc->unk_lbtransition_0xC;
    
    gcAddGObjDisplay(gobj, proc_display, dl_link_id, GOBJ_PRIORITY_DEFAULT, ~0);
    gcSetupCustomDObjs
    (
        gobj, 
        (DObjDesc*) (transition_desc->o_dobjdesc + (uintptr_t)sLBTransitionFileHeap),
        NULL,
        nGCMatrixKindTraRotRpyRSca,
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    if (transition_desc->o_anim_joint != 0)
    {
        gcAddAnimJointAll(gobj, (AObjEvent32**) (transition_desc->o_anim_joint + (uintptr_t)sLBTransitionFileHeap), 0.0F);
        gcPlayAnimAll(gobj);
    }
    gcAddGObjProcess(gobj, func, nGCProcessKindFunc, 1);
    
    return gobj;
}

// 0x800D4404
void lbTransitionSetupTransition(void)
{
    s32 i, j;
    size_t current_size;
    size_t largest_size;
    u32 *heap_pixels, *framebuffer_pixels;

    largest_size = 0;
    
    for (i = 0; i < ARRAY_COUNT(dLBTransitionDescs); i++)
    {
        current_size = lbRelocGetFileSize(dLBTransitionDescs[i].file_id);

        if (largest_size < current_size)
        {
            largest_size = current_size;
        }
    }
    sLBTransitionFileHeap = syTaskmanMalloc(largest_size, 0x10);
    heap_pixels = sLBTransitionPhotoHeap = syTaskmanMalloc(300 * 220 * sizeof(u16), 0x10);

    framebuffer_pixels = (u32*)
	(
		(uintptr_t)gSYSchedulerCurrentFramebuffer + SYVIDEO_BORDER_SIZE(320, 10, u16) + 
        SYVIDEO_BORDER_SIZE(320, 220, u16) + SYVIDEO_BORDER_SIZE(1, 10, u16)
	);
    for (i = 0; i < 220; i++)
    {
        for (j = 0; j < 150; j++)
        {
            *heap_pixels++ = *framebuffer_pixels++;
        }
        framebuffer_pixels -= 310;
    }
}
