#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITSwordItemDesc =
{
    nITKindSword,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataSwordItemAttributes,     // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itSwordFallProcUpdate,                  // Proc Update
    itSwordFallProcMap,                     // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITSwordStatusDescs[/* */] = 
{
    // Status 0 (Ground Wait)
    {
        NULL,                               // Proc Update
        itSwordWaitProcMap,                 // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Wait Fall)
    {
        itSwordFallProcUpdate,              // Proc Update
        itSwordFallProcMap,                 // Proc Map
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
        itSwordFallProcUpdate,              // Proc Update
        itSwordThrownProcMap,               // Proc Map
        itSwordThrownProcHit,               // Proc Hit
        itSwordThrownProcHit,               // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itSwordThrownProcHit,               // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 4 (Fighter Drop)
    {
        itSwordFallProcUpdate,              // Proc Update
        itSwordDroppedProcMap,              // Proc Map
        itSwordThrownProcHit,               // Proc Hit
        itSwordThrownProcHit,               // Proc Shield
        itMainCommonProcHop,                // Proc Hop
        itSwordThrownProcHit,               // Proc Set-Off
        itMainCommonProcReflector,          // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itSwordStatus
{
    nITSwordStatusWait,
    nITSwordStatusFall,
    nITSwordStatusHold,
    nITSwordStatusThrown,
    nITSwordStatusDropped,
    nITSwordStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80174B50
sb32 itSwordFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITSWORD_GRAVITY, ITSWORD_TVEL);
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80174B8C
sb32 itSwordWaitProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itSwordFallSetStatus);

    return FALSE;
}

// 0x80174BB4
sb32 itSwordFallProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITSWORD_MAP_REBOUND_COMMON, ITSWORD_MAP_REBOUND_GROUND, itSwordWaitSetStatus);
}

// 0x80174BE4
void itSwordWaitSetStatus(GObj *item_gobj)
{
    itMainSetGroundAllowPickup(item_gobj);
    itMainSetStatus(item_gobj, dITSwordStatusDescs, nITSwordStatusWait);
}

// 0x80174C18
void itSwordFallSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITSwordStatusDescs, nITSwordStatusFall);
}

// 0x80174C5C
void itSwordHoldSetStatus(GObj *item_gobj)
{
    DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(0.0F);

    itMainSetStatus(item_gobj, dITSwordStatusDescs, nITSwordStatusHold);
}

// 0x80174C90
sb32 itSwordThrownProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITSWORD_MAP_REBOUND_COMMON, ITSWORD_MAP_REBOUND_GROUND, itSwordWaitSetStatus);
}

// 0x80174CC0
sb32 itSwordThrownProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    itMainVelSetRebound(item_gobj);

    return FALSE;
}

// 0x80174CE8
void itSwordThrownSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITSwordStatusDescs, nITSwordStatusThrown);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(90.0F);
}

// 0x80174D2C
sb32 itSwordDroppedProcMap(GObj *item_gobj)
{
    return itMapCheckDestroyDropped(item_gobj, ITSWORD_MAP_REBOUND_COMMON, ITSWORD_MAP_REBOUND_GROUND, itSwordWaitSetStatus);
}

// 0x80174D5C
void itSwordDroppedSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITSwordStatusDescs, nITSwordStatusDropped);

    DObjGetStruct(item_gobj)->child->rotate.vec.f.y = F_CST_DTOR32(90.0F);
}

// 0x80174DA0
GObj* itSwordMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITSwordItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        DObjGetStruct(item_gobj)->rotate.vec.f.y = F_CST_DTOR32(90.0F);

        ip->is_unused_item_bool = TRUE;

        ip->arrow_gobj = ifCommonItemArrowMakeInterface(ip);
    }
    return item_gobj;
}
