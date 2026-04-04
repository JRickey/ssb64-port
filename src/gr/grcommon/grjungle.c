#include <gr/ground.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012EB50
DObjTransformTypes dGRJungleTaruCannTransformKinds[/* */] =
{
    { 0x28, nGCMatrixKindRotRpyR, 0x00 },
    { nGCMatrixKindTraRotRpyRSca, nGCMatrixKindNull, 0x00 }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grJungleTaruCannStatus
{
    nGRJungleTaruCannStatusMove,
    nGRJungleTaruCannStatusRotate
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80109CB0
void grJungleTaruCannAddAnimOffset(GObj *ground_gobj, intptr_t offset)
{
    DObj *dobj = DObjGetStruct(ground_gobj)->child;

    gcAddDObjAnimJoint(dobj, (AObjEvent32*) ((uintptr_t)gGRCommonStruct.jungle.map_head + (intptr_t)offset), 0.0F);
    gcParseDObjAnimJoint(dobj);
    gcPlayDObjAnimJoint(dobj);
}

// 0x80109CFC
void grJungleTaruCannAddAnimFill(GObj *ground_gobj)
{
    grJungleTaruCannAddAnimOffset(ground_gobj, &llGRJungleMapTaruCannFillAnimJoint);
}

// 0x80109D20
void grJungleTaruCannAddAnimShoot(GObj *ground_gobj)
{
    grJungleTaruCannAddAnimOffset(ground_gobj, &llGRJungleMapTaruCannShootAnimJoint);
}

// 0x80109D44
void grJungleTaruCannUpdateMove(GObj *ground_gobj)
{
    gGRCommonStruct.jungle.tarucann_wait--;

    if (gGRCommonStruct.jungle.tarucann_wait == 0)
    {
        gGRCommonStruct.jungle.tarucann_status = nGRJungleTaruCannStatusRotate;

        gGRCommonStruct.jungle.tarucann_rotate_step = ((syUtilsRandUShort() % 2) != 0) ? 0.07F : -0.07F;

        gGRCommonStruct.jungle.tarucann_wait = 90;
    }
}

// 0x80109DBC
void grJungleTaruCannUpdateRotate(GObj *ground_gobj)
{
    DObj *dobj = DObjGetStruct(ground_gobj);

    gGRCommonStruct.jungle.tarucann_wait--;

    if (gGRCommonStruct.jungle.tarucann_wait == 0)
    {
        gGRCommonStruct.jungle.tarucann_status = nGRJungleTaruCannStatusMove;

        gGRCommonStruct.jungle.tarucann_wait = syUtilsRandIntRange(180) + 180;

        dobj->rotate.vec.f.z = F_CST_DTOR32(0.0F);
    }
    else dobj->rotate.vec.f.z += gGRCommonStruct.jungle.tarucann_rotate_step;
}

// 0x80109E34
void grJungleTaruCannProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.jungle.tarucann_status)
    {
    case nGRJungleTaruCannStatusMove:
        grJungleTaruCannUpdateMove(ground_gobj);
        break;

    case nGRJungleTaruCannStatusRotate:
        grJungleTaruCannUpdateRotate(ground_gobj);
        break;
    }
}

// 0x80109E84
void grJungleMakeTaruCann(void)
{
    void *map_head;
    GObj *tarucann_gobj;

    map_head = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRJungleMapMapHead);
    gGRCommonStruct.jungle.map_head = map_head;

    gGRCommonStruct.jungle.tarucann_gobj = tarucann_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(tarucann_gobj, gcDrawDObjTreeForGObj, 6, GOBJ_PRIORITY_DEFAULT, ~0);

    grModelSetupGroundDObjs(tarucann_gobj, (DObjDesc*) ((intptr_t)&llGRJungleMapMapHead + (uintptr_t)map_head), NULL, dGRJungleTaruCannTransformKinds);
    gcAddGObjProcess(tarucann_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);

    gcAddAnimJointAll(tarucann_gobj, ((uintptr_t)map_head + (intptr_t)&llGRJungleMapTaruCannDefaultAnimJoint), 0.0F);
    gcPlayAnimAll(tarucann_gobj);

    gcAddGObjProcess(tarucann_gobj, grJungleTaruCannProcUpdate, nGCProcessKindFunc, 4);
    ftMainCheckAddGroundObstacle(tarucann_gobj, grJungleTaruCannCheckGetDamageKind);

    gGRCommonStruct.jungle.tarucann_status = nGRJungleTaruCannStatusMove;
    gGRCommonStruct.jungle.tarucann_wait = syUtilsRandIntRange(180) + 180;
    gGRCommonStruct.jungle.tarucann_rotate_step = F_CST_DTOR32(0.0F);
}

// 0x80109FB4
GObj* grJungleMakeGround(void)
{
    grJungleMakeTaruCann();

    return NULL;
}

// 0x80109FD8
sb32 grJungleTaruCannCheckGetDamageKind(GObj *ground_gobj, GObj *fighter_gobj, s32 *kind)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    f32 dist_x;
    f32 dist_y;

    if ((this_fp->tarucann_wait == 0) && (this_fp->status_id != nFTCommonStatusTaruCann) && !(this_fp->capture_immune_mask & FTCATCHKIND_MASK_TARUCANN))
    {
        DObj *gr_dobj = DObjGetStruct(ground_gobj);
        DObj *ft_dobj = DObjGetStruct(fighter_gobj);

        if (gr_dobj->translate.vec.f.x < ft_dobj->translate.vec.f.x)
        {
            dist_x = -(gr_dobj->translate.vec.f.x - ft_dobj->translate.vec.f.x);
        }
        else dist_x = gr_dobj->translate.vec.f.x - ft_dobj->translate.vec.f.x;

        if (gr_dobj->translate.vec.f.y < ft_dobj->translate.vec.f.y)
        {
            dist_y = -(gr_dobj->translate.vec.f.y - ft_dobj->translate.vec.f.y);
        }
        else dist_y = gr_dobj->translate.vec.f.y - ft_dobj->translate.vec.f.y;

        if ((dist_x < 280.0F) && (dist_y < 280.0F))
        {
            GObj *other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

            while (other_gobj != NULL)
            {
                if (other_gobj != fighter_gobj)
                {
                    FTStruct *other_fp = ftGetStruct(other_gobj);

                    if ((other_fp->status_id == nFTCommonStatusTaruCann) && (ground_gobj == other_fp->status_vars.common.tarucann.tarucann_gobj))
                    {
                        return FALSE;
                    }
                }
                other_gobj = other_gobj->link_next;
            }
            *kind = nGMHitEnvironmentTaruCann;

            grJungleTaruCannAddAnimFill(ground_gobj);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x8010A104
void grJungleTaruCannGetPosition(Vec3f *pos)
{
    *pos = DObjGetStruct(gGRCommonStruct.jungle.tarucann_gobj)->translate.vec.f;
}

// 0x8010A12C
f32 grJungleTaruCannGetRotate(void)
{
    return DObjGetStruct(gGRCommonStruct.jungle.tarucann_gobj)->rotate.vec.f.z;
}
