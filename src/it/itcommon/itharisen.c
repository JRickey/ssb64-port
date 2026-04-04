#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80189A70
intptr_t dITHarisenAnimJoint[/* */] = 
{ 
    0x2250, 0x2270 
};

ITDesc dITHarisenItemDesc =
{
    nITKindHarisen,                         // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataHarisenItemAttributes,   // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itHarisenFallProcUpdate,                // Proc Update
    itHarisenFallProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITHarisenStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itHarisenWaitProcMap,               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itHarisenFallProcUpdate,            // Proc Update
        itHarisenFallProcMap,               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 2 (Fighter Hold)
    {
        NULL,                               // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 3 (Fighter Throw)
    {
        itHarisenThrownProcUpdate,          // Proc Update
        itHarisenThrownProcMap,             // Proc Map
        itHarisenCommonProcHit,             // Proc Hit
        itHarisenCommonProcHit,             // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itHarisenCommonProcHit,             // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itHarisenFallProcUpdate,            // Proc Update
        itHarisenDroppedProcMap,            // Proc Map
        itHarisenCommonProcHit,             // Proc Hit
        itHarisenCommonProcHit,             // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itHarisenCommonProcHit,             // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itHarisenStatus
{
    nITHarisenStatusWait,
    nITHarisenStatusFall,
    nITHarisenStatusHold,
    nITHarisenStatusThrown,
    nITHarisenStatusDropped,
    nITHarisenStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80175140
void itHarisenCommonSetScale(GObj *item_gobj, f32 scale)
{
    DObjGetStruct(item_gobj)->scale.vec.f.x = scale;
    DObjGetStruct(item_gobj)->scale.vec.f.y = scale;
    DObjGetStruct(item_gobj)->scale.vec.f.z = scale;
}

// 0x80175160
sb32 itHarisenFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITHARISEN_GRAVITY, ITHARISEN_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80175198
sb32 itHarisenWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itHarisenFallSetStatus);

    return FALSE;
}

// 0x801751C0
sb32 itHarisenFallProcMap(GObj *item_gobj)
{
    itMapCheckDestroyDropped(item_gobj, ITHARISEN_MAP_REBOUND_COMMON, ITHARISEN_MAP_REBOUND_GROUND, itHarisenWaitSetStatus);

    return FALSE;
}

// 0x801751F4
void itHarisenWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITHarisenStatusDescs, nITHarisenStatusWait);
}

// 0x80175228
void itHarisenFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITHarisenStatusDescs, nITHarisenStatusFall);
}

// 0x8017526C
void itHarisenHoldSetStatus(GObj *item_gobj)
{
    DObj *dobj = DObjGetStruct(item_gobj);

    gcAddXObjForDObjFixed(dobj, nGCMatrixKindSca, 0);

    dobj->rotate.vec.f.y = 0.0F;

    itMainSetStatus(item_gobj, dITHarisenStatusDescs, nITHarisenStatusHold);
}

// 0x801752C0
sb32 itHarisenThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITHARISEN_GRAVITY, ITHARISEN_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x801752F8
sb32 itHarisenThrownProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHARISEN_MAP_REBOUND_COMMON, ITHARISEN_MAP_REBOUND_GROUND, itHarisenWaitSetStatus);
}

// 0x80175328
sb32 itHarisenCommonProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x80175350
void itHarisenThrownSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITHarisenStatusDescs, nITHarisenStatusThrown);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(-90.0F);
}

// 0x80175394
sb32 itHarisenDroppedProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHARISEN_MAP_REBOUND_COMMON, ITHARISEN_MAP_REBOUND_GROUND, itHarisenWaitSetStatus);
}

// 0x801753C4
void itHarisenDroppedSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITHarisenStatusDescs, nITHarisenStatusDropped);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(-90.0F);
}

// 0x80175408
void func_ovl3_80175408(GObj *item_gobj, s32 index) // Unused
{
    ITStruct *ip = itGetStruct(item_gobj);

    gcAddAnimJointAll(item_gobj, (((uintptr_t)ip->attr->data + dITHarisenAnimJoint[index]) - (intptr_t)&llITCommonDataHarisenDataStart), 0.0F);
    gcPlayAnimAll(item_gobj);
}

// 0x80175460
GObj* itHarisenMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITHarisenItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(-90.0F);

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}
