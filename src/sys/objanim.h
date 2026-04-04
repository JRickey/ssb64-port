#ifndef _OBJANIM_H_
#define _OBJANIM_H_

#include <sys/objtypes.h>

// Get next depth-first DObj node from tree
extern DObj* gcGetTreeDObjNext(DObj *dobj);

// Set animation playback rate for all DObjs on a GObj
extern void gcSetAnimSpeed(GObj *gobj, f32 anim_speed);

// Set animation playback rate for all DObjs and MObjs on a GObj
extern void gcSetAllAnimSpeed(GObj *gobj, f32 anim_speed);

// Add animation script to specific DObj
extern void gcAddDObjAnimJoint(DObj*, AObjEvent32*, f32);

// Add material animation script to specific MObj
extern void gcAddMObjMatAnimJoint(MObj*, AObjEvent32*, f32);

// Add animation script to all DObjs on a GObj
extern void gcAddAnimJointAll(GObj *gobj, AObjEvent32 **anim_joints, f32 anim_frame);

// Add material animation script to all MObjs on a GObj
extern void gcAddMatAnimJointAll(GObj *gobj, AObjEvent32 ***p_matanim_joints, f32 anim_frame);

// Add animation and material animation scripts to all DObjs and MObjs on a GObj
extern void gcAddAnimAll(GObj *gobj, AObjEvent32 **anim_joints, AObjEvent32 ***p_matanim_joints, f32 anim_frame);

// Parse AnimJoint on specific DObj
extern void gcParseDObjAnimJoint(DObj *dobj);

// Interpolation?
extern f32 gcGetInterpValueCubic(f32 length_invert, f32 length, f32 value_base, f32 value_target, f32 rate_base, f32 rate_target);

// Other kind of interpolation?
extern f32 gcGetInterpRateCubic(f32 length_invert, f32 length, f32 value_base, f32 value_target, f32 rate_base, f32 rate_target);

// ???
extern f32 gcGetAObjValue(AObj *aobj);

// ???
extern f32 gcGetAObjRate(AObj *aobj);

// Apply AnimJoint values on current frame of animation to specific DObj
extern void gcPlayDObjAnimJoint(DObj *dobj);

// Apply MatAnimJoint values on current frame of animation to specific MObj
extern void gcPlayMObjMatAnim(MObj *mobj);

// Play all DObj and MObj animations on a GObj
extern void gcPlayAnimAll(GObj *gobj);

// Get AObj with matching track ID
extern AObj* gcGetTrackAObj(AObj *aobj, u8 track);

// Set DObj animation length?
extern void gcSetDObjAnimLength(DObj *dobj, f32 length);

// Get DObj axis value of corresponding track ID
extern f32 gcGetDObjAxisTrack(DObj *dobj, s32 track);

// Get DObjDesc axis value of corresponding track ID
extern f32 gcGetDObjDescAxisTrack(DObjDesc *dobjdesc, s32 track);

// Check if DObj or DObjDesc axis value of corresponding track ID has been retreived
extern sb32 gcCheckGetDObjNoAxisTrack
(
    sb32 is_desc_or_dobj,
    DObj *dobj,
    f32 *axis_value,
    f32 *rate,
    AObj *seek_aobj,
    DObjDesc *dobjdesc,
    s32 track,
    sb32 rate_kind,
    Vec3f *translate,
    sb32 *is_axis_ready
);

// ???
extern void gcGetAObjTrackAnimTimeMax(s32 track, f32 translate, f32 rotate, f32 scale, f32 *anim_time_max, AObj *aobj);

// ???
extern f32 gcGetDObjTempAnimTimeMax
(
    DObj *dobj,
    AObjEvent32 **anim_joints,
    f32 anim_frame,
    DObjDesc *dobjdesc,
    s32 rate_kind,
    f32 length,
    f32 translate,
    f32 rotate,
    f32 scale
);

// ???
extern f32 func_8000EC64_F864
(
    GObj *gobj,
    AObjEvent32 **anim_joints,
    f32 anim_frame,
    DObjDesc *dobjdesc,
    s32 rate_kind,
    f32 length_max,
    f32 length_min,
    f32 translate,
    f32 rotate,
    f32 scale
);

// ???
extern void func_8000EE40_FA40(GObj *gobj, AObjEvent32 **anim_joints, f32 anim_frame, DObjDesc *dobjdesc);

// Add Translate, Rotate and Scale XObjes for specific DObj
extern void gcAddDObjTransformTraRotSca(DObj *dobj);

// Add DObj with Translate, Rotate and Scale XObjes for GObj
extern DObj* gcAddDObjForGObjTraRotSca(GObj *gobj, void *dvar);

// Add sibling with Transform, Rotate and Scale XObjes for specific DObj
extern DObj* gcAddSiblingForDObjTraRotSca(DObj *dobj, void *dvar);

// Add child with Transform, Rotate and Scale XObjes for specific DObj
extern DObj* gcAddChildForDObjTraRotSca(DObj *dobj, void *dvar);

// Set up DObj node tree with common XObj transformations for GObj
extern void gcSetupCommonDObjs(GObj *gobj, DObjDesc *dobjdesc, DObj **dobjs);

// Add three XObjes of transformation kind for specific DObj
extern void gcAddDObj3TransformsKind(DObj *dobj, u8 tk1, u8 tk2, u8 tk3);

// Decide what transformation XObjes to add to specific DObj based on tk1, tk2, tk3 and flags
extern void gcDecideDObj3TransformsKind(DObj *dobj, u8 tk1, u8 tk2, u8 tk3, s32 flags);

// Set up DObj node tree with custom XObj transformations for GObj
extern void gcSetupCustomDObjs(GObj *gobj, DObjDesc *dobjdesc, DObj **dobjs, u8 tk1, u8 tk2, u8 tk3);

// Set up DObj node tree with MObj and custom XObj transformations for GObj
extern void gcSetupCustomDObjsWithMObj
(
    GObj *gobj, 
    DObjDesc *dobjdesc, 
    MObjSub ***p_mobjsubs, 
    DObj **dobjs, 
    u8 tk1, 
    u8 tk2, 
    u8 tk3
);

// Add MObj for all DObjs on a GObj
extern void gcAddMObjAll(GObj *gobj, MObjSub ***p_mobjsubs);

// Set transformation vectors for all DObjs on a GObj
extern void gcSetDObjTransformsForGObj(GObj *gobj, DObjDesc *dobjdesc);

// Add CamAnimJoint to Camera
extern void gcAddCObjCamAnimJoint(CObj *cobj, AObjEvent32 *camanim_joint, f32 anim_frame); 

// Parse CamAnimJoint on Camera
extern void gcParseCObjCamAnimJoint(CObj *cobj);

// Play CamAnimJoint on Camera
extern void gcPlayCObjCamAnim(CObj *cobj);

// Parse and update CamAnimJoint on GObj's Camera
extern void gcPlayCamAnim(GObj *gobj);

// Get total animation length
extern s32 gcGetAnimTotalLength(AObjEvent32 **anim_joints);

#endif
