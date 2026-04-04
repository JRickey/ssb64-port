#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITHammerItemDesc =
{
    nITKindHammer,                          // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataHammerItemAttributes,    // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itHammerFallProcUpdate,                 // Proc Update
    itHammerFallProcMap,                    // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITHammerStatusDescs[/* */] =
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itHammerWaitProcMap,                // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itHammerFallProcUpdate,             // Proc Update
        itHammerFallProcMap,                // Proc Map
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
        itHammerThrownProcUpdate,           // Proc Update
        itHammerThrownProcMap,              // Proc Map
        itHammerCommonProcHit,              // Proc Hit
        itHammerCommonProcHit,              // Proc Shield
        NULL,                               // Proc Hop
        itHammerCommonProcHit,              // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itHammerFallProcUpdate,             // Proc Update
        itHammerDroppedProcMap,             // Proc Map
        itHammerCommonProcHit,              // Proc Hit
        itHammerCommonProcHit,              // Proc Shield
        NULL,                               // Proc Hop
        itHammerCommonProcHit,              // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itHammerStatus
{
    nITHammerStatusWait,
    nITHammerStatusFall,
    nITHammerStatusHold,
    nITHammerStatusThrown,
    nITHammerStatusDropped,
    nITHammerStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80176110
void itHammerCommonSetColAnim(GObj *item_gobj)
{
    itMainCheckSetColAnimID(item_gobj, nGMColAnimItemHammerEnd, 0);
}

// 0x80176134
sb32 itHammerFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITHAMMER_GRAVITY, ITHAMMER_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x8017616C
sb32 itHammerWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itHammerFallSetStatus);

    return FALSE;
}

// 0x80176194
sb32 itHammerFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHAMMER_MAP_REBOUND_COMMON, ITHAMMER_MAP_REBOUND_GROUND, itHammerWaitSetStatus);
}

// 0x801761C4
void itHammerWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITHammerStatusDescs, nITHammerStatusWait);
}

// 0x801761F8
void itHammerFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITHammerStatusDescs, nITHammerStatusFall);
}

// 0x8017623C
void itHammerHoldSetStatus(GObj *item_gobj)
{
    DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(0.0F);

    itMainSetStatus(item_gobj, dITHammerStatusDescs, nITHammerStatusHold);
}

// 0x80176270
sb32 itHammerThrownProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITHAMMER_GRAVITY, ITHAMMER_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x801762A8
sb32 itHammerThrownProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHAMMER_MAP_REBOUND_COMMON, ITHAMMER_MAP_REBOUND_GROUND, itHammerWaitSetStatus);
}

// 0x801762D8
sb32 itHammerCommonProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x80176300
void itHammerThrownSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITHammerStatusDescs, nITHammerStatusThrown);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(90.0F);

    ftParamTryUpdateItemMusic();
}

// 0x80176348
sb32 itHammerDroppedProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITHAMMER_MAP_REBOUND_COMMON, ITHAMMER_MAP_REBOUND_GROUND, itHammerWaitSetStatus);
}

// 0x80176378
void itHammerDroppedSetStatus(GObj *item_gobj)
{
    itMainClearColAnim(item_gobj);
    itMainSetStatus(item_gobj, dITHammerStatusDescs, nITHammerStatusDropped);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(90.0F);

    ftParamTryUpdateItemMusic();
}

// 0x8017633C8
GObj* itHammerMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITHammerItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(90.0F);

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}
