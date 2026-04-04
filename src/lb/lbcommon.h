#ifndef _LBCOMMON_H_
#define _LBCOMMON_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <PR/sp.h>
#include <ft/ftdef.h>
#include <gm/gmsound.h>

extern sb32 (*dLBCommonFuncMatrixList[/* */])(/* */);
extern Vec3f gLBCommonScale;

extern f32 lbCommonSin(f32 x);
extern f32 lbCommonCos(f32 x);
extern f32 lbCommonTan(f32 angle);
extern f32 lbCommonNormDist2D(Vec3f *p);
extern f32 lbCommonMag2D(Vec3f *p);
extern Vec3f* lbCommonAdd2D(Vec3f *dst, Vec3f *p);
extern Vec3f* lbCommonScale2D(Vec3f *dst, f32 scale);
extern Vec3f* lbCommonReflect2D(Vec3f *dst, Vec3f *p);
extern f32 lbCommonSim3D(Vec3f *a, Vec3f *b);
extern f32 lbCommonSim2D(Vec3f *a, Vec3f *b);
extern sb32 lbCommonCheckAdjustSim2D(Vec3f *a, Vec3f *b, f32 angle);
extern void lbCommonMatrixTraRotScaInv
(
    Mtx *mtx,
    f32 trax,
    f32 tray,
    f32 traz,
    f32 rotx,
    f32 roty,
    f32 rotz,
    f32 scax_inv,
    f32 scay_inv,
    f32 scaz_inv,
    f32 scax,
    f32 scay,
    f32 scaz
);
extern void lbCommonMatrixRotSca(Mtx *mtx, f32 rotx, f32 roty, f32 rotz, f32 scax, f32 scay, f32 scaz);
extern void lbCommonFuncUpdate(void);
extern alSoundEffect* lbCommonMakePositionFGM(u16 fgm, f32 pos);
extern DObj* lbCommonGetTreeDObjNextFromRoot(DObj *a, DObj *b);
extern void lbCommonAddDObjAnimJointAll(DObj *root_dobj, AObjEvent32 **anim_joints, f32 anim_frame);
extern void lbCommonAddFighterPartsFigatree(DObj *root_dobj, void **figatree, f32 anim_frame);
extern void lbCommonAddTreeDObjsAnimAll(DObj *root_dobj, AObjEvent32 **anim_joints, AObjEvent32 ***p_matanim_joints, f32 anim_frame);
extern void lbCommonInitDObj3Transforms(DObj *dobj, u8 tk1, u8 tk2, u8 tk3);
extern void lbCommonInitDObj(DObj *dobj, u8 tk1, u8 tk2, u8 tk3, u8 arg4);
extern void lbCommonSetupTreeDObjs(DObj *root_dobj, DObjDesc *dobjdesc, DObj **dobjs, u8 tk1, u8 tk2, u8 tk3);
extern void lbCommonAddMObjForFighterPartsDObj
(
    DObj *dobj,
    MObjSub **mobjsubs,
    AObjEvent32 **costume_matanim_joints,
    AObjEvent32 **main_matanim_joints,
    f32 anim_frame
);
extern void lbCommonSetupFighterPartsDObjs
(
    DObj *root_dobj,
    FTCommonPartContainer *commonparts_container,
    s32 detail_curr,
    DObj **dobjs,
    u32 *setup_parts,
    u8 tk1,
    u8 tk2,
    u8 tk3,
    f32 anim_frame,
    u8 arg9
);
extern void lbCommonSetupCustomTreeDObjsWithMObj
(
    DObj *root_dobj,
    DObjDesc *dobjdesc,
    MObjSub ***p_mobjsubs,
    DObj **dobjs,
    u8 tk1,
    u8 tk2,
    u8 tk3
);
extern void lbCommonAddMObjForTreeDObjs(DObj *root_dobj, MObjSub ***p_mobjsubs);
extern void lbCommonPlayTreeDObjsAnim(DObj *root_dobj);
extern void lbCommonSetDObjTransformsForTreeDObjs(DObj *root_dobj, DObjDesc *dobjdesc);
extern void lbCommonInsertTreeDObjChild(DObj *root_dobj, void *dvar);
extern void lbCommonEjectTreeDObj(DObj *dobj);
extern void lbCommonPlayTranslateScaledDObjAnim(DObj *dobj, Vec3f *scale);
extern void func_ovl0_800C96DC(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800C96EC(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800C9714(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 lbCommonFighterPartsFuncMatrix(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800C994C(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800C99CC(Mtx *mtx, DObj *dobj, Gfx **dls);
extern void func_ovl0_800C9A38(Mtx44f mtx, DObj *dobj);
extern sb32 func_ovl0_800C9F30(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800C9F70(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800CA024(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 lbCommonRotScaFuncMatrix(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800CA194(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800CA5C8(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800CAB48(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800CB140(Mtx *mtx, DObj *dobj, Gfx **dls);
extern sb32 func_ovl0_800CB2F0(Mtx *mtx, DObj *dobj, Gfx **dls);
extern void lbCommonDrawDObjScaleX(DObj *dobj);
extern void lbCommonDObjScaleXProcDisplay(GObj *gobj);
extern void lbCommonDrawDObjDefault(DObj *dobj);
extern void lbCommonEjectGObjLinkedList(GObj *gobj);
extern u8 lbCommonGetBitmapDecodeNibble(u8 index);
extern void lbCommonDecodeBitmapSiz4b(u8 *bitmap_csr, u8 *bitmap_buf, u8 *bitmap_start);
extern void lbCommonDecodeSpriteBitmapsSiz4b(Sprite *sprite);
extern void lbCommonDrawSObjBitmap
(
    Gfx **dls,
    SObj *sobj,
    Sprite *sprite,
    Bitmap *bitmap,
    s32 x, s32 y,
    s32 xx, s32 yy,
    s32 fs, s32 ft,
    s32 sx, s32 sy
);
extern void lbCommonPrepSObjAttr(Gfx **dls, SObj *sobj);
extern void lbCommonPrepSObjDraw(Gfx **dls, SObj *sobj);
extern void lbCommonClearExternSpriteParams(void);
extern void lbCommonSetExternSpriteParams(Sprite *sprite);
extern void lbCommonDrawSObjAttr(GObj *gobj);
extern void lbCommonDrawSObjNoAttr(GObj *gobj);
extern SObj* lbCommonMakeSObjForGObj(GObj *gobj, Sprite *sprite);
extern GObj* lbCommonMakeSpriteGObj
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
extern void lbCommonStartSprite(Gfx **dls);
extern void lbCommonSetSpriteScissor(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
extern void lbCommonFinishSprite(Gfx **dls);
extern void lbCommonDrawSprite(GObj *camera_gobj);
extern void lbCommonInitCameraOrtho(CObj *cobj, u8 tk, u8 arg2);
extern void lbCommonInitCameraPersp(CObj *cobj, u8 tk, u8 arg2);
extern void lbCommonInitCameraVec(CObj *cobj, u8 tk, u8 arg2);
extern void lbCommonCross3D(Vec3f *a, Vec3f *b, Vec3f *out);

#endif
