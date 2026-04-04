#include <gr/ground.h>
#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8010B340
void grCastleBumperProcUpdate(GObj *ground_gobj)
{
    Vec3f *ground_pos = &DObjGetStruct(ground_gobj)->translate.vec.f;

    if (gGRCommonStruct.castle.bumper_gobj != NULL)
    {
        Vec3f *bumper_pos = &DObjGetStruct(gGRCommonStruct.castle.bumper_gobj)->translate.vec.f;

        bumper_pos->x = ground_pos->x + gGRCommonStruct.castle.bumper_pos.x;
    }
}

// 0x8010B378
void grCastleInitAll(void)
{
    void *map_head;
    GObj *ground_gobj;
    Vec3f yakumono_pos;
    Vec3f vel;
    s32 pos_id;
    DObj *dobj;

    gGRCommonStruct.castle.map_head = map_head = (void*)((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRCastleMapMapHead);

    ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjProcess(ground_gobj, grCastleBumperProcUpdate, nGCProcessKindFunc, 4);

    dobj = gcAddDObjForGObj(ground_gobj, NULL);
    dobj->translate.vec.f.x = dobj->translate.vec.f.y = dobj->translate.vec.f.z = 0.0F;

    gcAddGObjProcess(ground_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);

    gcAddAnimJointAll(ground_gobj, gMPCollisionGroundData->map_nodes, 0.0F);
    gcPlayAnimAll(ground_gobj);

    mpCollisionGetMapObjIDsKind(nMPMapObjKindBumper, &pos_id);
    mpCollisionGetMapObjPositionID(pos_id, &yakumono_pos);

    gGRCommonStruct.castle.bumper_pos = yakumono_pos;

    vel.x = 0.0F;
    vel.y = 0.0F;
    vel.z = 0.0F;

    gGRCommonStruct.castle.bumper_gobj = itManagerMakeItemSetupCommon(NULL, nITKindGBumper, &yakumono_pos, &vel, ITEM_FLAG_PARENT_GROUND);
}

// 0x8010B4AC
GObj* grCastleMakeGround(void)
{
    grCastleInitAll();

    return NULL;
}
