#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITHeartItemDesc =
{
    nITKindHeart,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataHeartItemAttributes,     // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,             // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itHeartFallProcUpdate,                  // Proc Update
    itHeartFallProcMap,                     // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITHeartStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itHeartWaitProcMap,                 // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itHeartFallProcUpdate,              // Proc Update
        itHeartFallProcMap,                 // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 2 (Fighter Drop)
    {
        itHeartFallProcUpdate,              // Proc Update
        itHeartDroppedProcMap,              // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itHeartStatus
{
    nITHeartStatusWait,
    nITHeartStatusFall,
    nITHeartStatusDropped,
    nITHeartStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801746F0
sb32 itHeartFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITHEART_GRAVITY, ITHEART_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80174728
sb32 itHeartWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itHeartFallSetStatus);

    return FALSE;
}

// 0x80174750
sb32 itHeartFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHEART_MAP_REBOUND_COMMON, ITHEART_MAP_REBOUND_GROUND, itHeartWaitSetStatus);
}

// 0x80174780
void itHeartWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITHeartStatusDescs, nITHeartStatusWait);
}

// 0x801747B4
void itHeartFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITHeartStatusDescs, nITHeartStatusFall);
}

// 0x801747F8
sb32 itHeartDroppedProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHEART_MAP_REBOUND_COMMON, ITHEART_MAP_REBOUND_GROUND, itHeartWaitSetStatus);
}

// 0x80174828
void itHeartDroppedSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITHeartStatusDescs, nITHeartStatusDropped);
}

// 0x80174850
GObj* itHeartMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITHeartItemDesc, pos, vel, flags);
    DObj *dobj;
#if defined(REGION_US)
    Vec3f translate;
#endif
    ITStruct *ip;

    if (item_gobj != NULL)
    {
#if defined(REGION_US)
        dobj = DObjGetStruct(item_gobj);
        ip = itGetStruct(item_gobj);
        translate = dobj->translate.vec.f;

        gcAddXObjForDObjFixed(dobj, 0x2E, 0);

        dobj->translate.vec.f = translate;
#else
        ip = itGetStruct(item_gobj);
        dobj = DObjGetStruct(item_gobj);

        gcAddXObjForDObjFixed(dobj, 0x2E, 0);

        dobj->translate.vec.f = *pos;
#endif

        dobj->rotate.vec.f.z = 0.0F;

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}
