#include <it/item.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITGBumperItemDesc =
{
    nITKindGBumper,                         // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataGBumperItemAttributes,   // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itGBumperCommonProcUpdate,              // Proc Update
    NULL,                                   // Proc Map
    itGBumperCommonProcHit,                 // Proc Hit
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

// 0x8017D590
sb32 itGBumperCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    if ((ip->item_vars.bumper.hit_anim_length == 0) && (dobj->mobj->palette_id == 1.0F))
    {
        dobj->mobj->palette_id = 0;
    }
    else ip->item_vars.bumper.hit_anim_length--;
    
    if (ip->multi != 0)
    {
        dobj->scale.vec.f.x = dobj->scale.vec.f.y = ( 2.0F - ( (10 - ip->multi) * 0.1F ) );

        ip->multi--;
    }
    else dobj->scale.vec.f.x = dobj->scale.vec.f.y = 1;
    
    return FALSE;
}

// 0x8017D63C
sb32 itGBumperCommonProcHit(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->scale.vec.f.x = 2.0F;
    dobj->scale.vec.f.y = 2.0F;

    ip->item_vars.bumper.hit_anim_length = ITBUMPER_HIT_ANIM_LENGTH;

    dobj->mobj->palette_id = 1.0F;

    ip->multi = ITBUMPER_HIT_SCALE;

    return FALSE;
}

// 0x8017D67C
GObj* itGBumperMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITGBumperItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip;
        DObj *dobj;

        itMainClearOwnerStats(item_gobj);

        ip = itGetStruct(item_gobj);
        dobj = DObjGetStruct(item_gobj);

        ip->multi = 0;

        ip->attack_coll.interact_mask = GMHITCOLLISION_FLAG_FIGHTER;
        ip->attack_coll.can_rehit_shield = TRUE;

        ip->physics.vel_air.x = 0.0F;
        ip->physics.vel_air.y = 0.0F;
        ip->physics.vel_air.z = 0.0F;

        dobj->mobj->palette_id = 0;

        if (gSCManagerBattleState->gkind == nGRKindCastle)
        {
            ip->attack_coll.knockback_weight = ITBUMPER_CASTLE_KNOCKBACK;
            ip->attack_coll.angle = ITBUMPER_CASTLE_ANGLE;
        }
    }
    return item_gobj;
}
