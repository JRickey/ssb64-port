#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018ABC0
ITDesc dITTosakintoItemDesc =
{
    nITKindTosakinto,                       // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataTosakintoItemAttributes, // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                   // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0,                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itTosakintoCommonProcUpdate,            // Proc Update
    itTosakintoCommonProcMap,               // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018ABF4 
ITStatusDesc dITTosakintoStatusDescs[/* */] =
{
    // Status 0 (Neutral Appear)
    {
        itTosakintoAppearProcUpdate,        // Proc Update
        itTosakintoAppearProcMap,           // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Neutral Splash)
    {
        itTosakintoBounceProcUpdate,        // Proc Update
        itTosakintoBounceProcMap,           // Proc Map
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

enum itTosakintoStatus
{
    nITTosakintoStatusAppear,
    nITTosakintoStatusBounce,
    nITTosakintoStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017E7A0
sb32 itTosakintoAppearProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITTOSAKINTO_GRAVITY, ITTOSAKINTO_TVEL);

    return FALSE;
}

// 0x8017E7CC
sb32 itTosakintoAppearProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMapTestAllCheckCollEnd(item_gobj);

    if (ip->coll_data.mask_curr & MAP_FLAG_FLOOR)
    {
        ip->physics.vel_air.y = ITTOSAKINTO_FLAP_VEL_Y;

        itTosakintoBounceSetStatus(item_gobj);

        func_800269C0_275C0(nSYAudioFGMTosakintoSplash);
    }
    return FALSE;
}

// 0x8017E828
void itTosakintoAppearSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = ITTOSAKINTO_LIFETIME;

    if (ip->kind == nITKindTosakinto)
    {
        func_800269C0_275C0(nSYAudioVoiceMBallTosakintoAppear);
    }
    itMainSetStatus(item_gobj, dITTosakintoStatusDescs, nITTosakintoStatusAppear);
}

// 0x8017E880
sb32 itTosakintoBounceProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITTOSAKINTO_GRAVITY, ITTOSAKINTO_TVEL);

    if (ip->multi == 0)
    {
        return TRUE;
    }
    ip->multi--;

    return FALSE;
}

// 0x8017E8CC
sb32 itTosakintoBounceProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMapTestAllCheckCollEnd(item_gobj);

    if (ip->coll_data.mask_curr & MAP_FLAG_FLOOR)
    {
        ip->physics.vel_air.y = ITTOSAKINTO_FLAP_VEL_Y;

        if (syUtilsRandIntRange(2) != 0)
        {
            ip->physics.vel_air.x = -ip->physics.vel_air.x;
        }
        func_800269C0_275C0(nSYAudioFGMTosakintoSplash);
    }
    return FALSE;
}

// 0x8017E93C
void itTosakintoBounceInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    void *anim_joint;
    void *matanim_joint;
    s32 unused;

    ip->item_vars.tosakinto.pos = dobj->translate.vec.f;

    ip->physics.vel_air.y = ITTOSAKINTO_FLAP_VEL_Y;
    ip->physics.vel_air.x = ITTOSAKINTO_FLAP_VEL_X;

    if (ip->kind == nITKindTosakinto)
    {
        anim_joint = itGetPData(ip, &llITCommonDataTosakintoDataStart, &llITCommonDataTosakintoAnimJoint);

        gcAddDObjAnimJoint(dobj->child, anim_joint, 0.0F);

        matanim_joint = itGetPData(ip, &llITCommonDataTosakintoDataStart, &llITCommonDataTosakintoMatAnimJoint);

        gcAddMObjMatAnimJoint(dobj->child->mobj, matanim_joint, 0.0F);

        gcPlayAnimAll(item_gobj);
    }
}

// 0x8017EA14
void itTosakintoBounceSetStatus(GObj *item_gobj)
{
    itTosakintoBounceInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITTosakintoStatusDescs, nITTosakintoStatusBounce);
}

// 0x8017EA48
sb32 itTosakintoCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        ip->physics.vel_air.y = 0.0F;

        itTosakintoAppearSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017EA98
sb32 itTosakintoCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x8017EAD8
GObj* itTosakintoMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITTosakintoItemDesc, pos, vel, flags);
    DObj *dobj;
    ITStruct *ip;

    if (item_gobj != NULL)
    {
        itMainClearOwnerStats(item_gobj);

        dobj = DObjGetStruct(item_gobj);

        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
        gcAddXObjForDObjFixed(dobj, 0x48, 0);

        dobj->translate.vec.f = *pos;

        ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataTosakintoDataStart), 0.0F);
    }
    return item_gobj;
}
