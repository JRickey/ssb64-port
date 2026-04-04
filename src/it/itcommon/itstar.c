#include <it/item.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITStarItemDesc =
{
    nITKindStar,                            // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataStarItemAttributes,      // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itStarCommonProcUpdate,                 // Proc Update
    itStarCommonProcMap,                    // Proc Map
    itStarCommonProcHit,                    // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80174930
sb32 itStarCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITSTAR_GRAVITY, ITSTAR_TVEL);

    ip->multi--;

    if (ip->multi == 0)
    {
        itMainRefreshAttackColl(item_gobj);
    }
    itVisualsUpdateSpin(item_gobj);

    return FALSE;
}

// 0x80174990
sb32 itStarCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    s32 unused;
    sb32 is_collide_floor = itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR);

    if (itMapCheckCollideAllRebound(item_gobj, (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL), ITSTAR_MAP_REBOUND_COMMON, NULL) != FALSE)
    {
        itMainSetSpinVelLR(item_gobj);
    }
    if (is_collide_floor != FALSE)
    {
        ip->physics.vel_air.y = ITSTAR_BOUNCE_Y;

        func_800269C0_275C0(nSYAudioFGMStarMapCollide);
    }
    return FALSE;
}

// 0x80174A0C
sb32 itStarCommonProcHit(GObj *item_gobj)
{
    return TRUE;
}

// 0x80174A18
GObj* itStarMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    CObj *cobj = CObjGetStruct(gGMCameraGObj);
    GObj *item_gobj;
    DObj *dobj;
    ITStruct *ip;
    Vec3f vel_real;
#if defined(REGION_US)
    Vec3f translate;
#endif

    vel_real.x = (pos->x < cobj->vec.at.x) ? ITSTAR_VEL_X : -ITSTAR_VEL_X;
    vel_real.y = ITSTAR_BOUNCE_Y;
    vel_real.z = 0.0F;

    item_gobj = itManagerMakeItem(parent_gobj, &dITStarItemDesc, pos, &vel_real, flags);

    if (item_gobj != NULL)
    {
        dobj = DObjGetStruct(item_gobj);

#if defined(REGION_US)
        translate = dobj->translate.vec.f;
#endif

        ip = itGetStruct(item_gobj);

        ip->attack_coll.interact_mask = GMHITCOLLISION_FLAG_FIGHTER; // Star Man can only interact with fighters
        
        ip->multi = ITSTAR_INTERACT_DELAY;

        ip->is_unused_item_bool = TRUE;

        gcAddXObjForDObjFixed(dobj, 0x2E, 0);

        dobj->rotate.vec.f.z = 0.0F;

#if defined(REGION_US)
        dobj->translate.vec.f = translate;
#else
        dobj->translate.vec.f = *pos;
#endif
    }
    return item_gobj;
}
