#include <it/item.h>
#include <sys/develop.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018AB40
ITDesc dITKabigonItemDesc = 
{
    nITKindKabigon,                         // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataKabigonItemAttributes,   // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itKabigonCommonProcUpdate,              // Proc Update
    NULL,                                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018AB74
ITStatusDesc dITKabigonStatusDescs[/* */] = 
{
    // Status 0 (Neutral Jump)
    {
        itKabigonJumpProcUpdate,            // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Neutral Fall)
    {
        itKabigonFallProcUpdate,            // Proc Update
        NULL,                               // Proc Map
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

enum itKabigonStatus
{
    nITKabigonStatusJump,
    nITKabigonStatusFall,
    nITKabigonStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017E070
sb32 itKabigonFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    if (ip->item_vars.kabigon.rumble_wait == 0)
    {
        efManagerQuakeMakeEffect(0);

        ip->item_vars.kabigon.rumble_wait = ITKABIGON_RUMBLE_WAIT;
    }
    ip->item_vars.kabigon.rumble_wait--;

    if (dobj->translate.vec.f.y < (gMPCollisionGroundData->map_bound_bottom + ITKABIGON_MAP_OFF_Y))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8017E100
void itKabigonFallProcDisplay(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);

    if (itDisplayCheckItemVisible(ip) != FALSE)
    {
        if ((ip->display_mode == nDBDisplayModeMaster) || (ip->is_hold))
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

            gcDrawDObjTreeForGObj(item_gobj);
        }
        else if (ip->display_mode == nDBDisplayModeMapCollision)
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

            gcDrawDObjTreeForGObj(item_gobj);
            itDisplayMapCollisions(item_gobj);
        }
        else if ((ip->damage_coll.hitstatus == nGMHitStatusNone) && (ip->attack_coll.attack_state == nGMAttackStateOff))
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

            gcDrawDObjTreeForGObj(item_gobj);
        }
        else itDisplayHitCollisions(item_gobj);
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x8017E25C
void itKabigonFallInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    ip->physics.vel_air.y = ITKABIGON_DROP_VEL_Y;

    dobj->translate.vec.f.x += ((ITKABIGON_DROP_OFF_X_MUL * syUtilsRandFloat()) + ITKABIGON_DROP_OFF_X_ADD);

    itMainRefreshAttackColl(item_gobj);

    ip->item_vars.kabigon.rumble_wait = 0;

    func_800269C0_275C0(nSYAudioFGMKabigonFall);

    if (ip->kind == nITKindKabigon)
    {
        func_800269C0_275C0(nSYAudioVoiceMBallKabigonFall);

        dobj->scale.vec.f.x = dobj->scale.vec.f.y = ITKABIGON_DROP_SIZE_KABIGON;

        ip->attack_coll.size *= ITKABIGON_DROP_SIZE_KABIGON;
    }
    else
    {
        dobj->scale.vec.f.x = dobj->scale.vec.f.y = ITKABIGON_DROP_SIZE_OTHER;

        ip->attack_coll.size *= ITKABIGON_DROP_SIZE_OTHER;
    }
    item_gobj->proc_display = itKabigonFallProcDisplay;

    gcMoveGObjDLHead(item_gobj, 18, item_gobj->dl_link_priority);
}

// 0x8017E350
void itKabigonFallSetStatus(GObj *item_gobj)
{
    itKabigonFallInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITKabigonStatusDescs, nITKabigonStatusFall);
}

// 0x8017E384
sb32 itKabigonJumpProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    if (dobj->translate.vec.f.y >= (gMPCollisionGroundData->map_bound_top - ITKABIGON_MAP_OFF_Y))
    {
        ip->multi--;

        ip->physics.vel_air.y = 0.0F;

        if (ip->multi == 0)
        {
            itKabigonFallSetStatus(item_gobj);
        }
    }
    if (ip->item_vars.kabigon.dust_effect_int == 0)
    {
        Vec3f pos = dobj->translate.vec.f;

        pos.x += (syUtilsRandFloat() * ITKABIGON_JUMP_GFX_MUL_OFF) - ITKABIGON_JUMP_GFX_SUB_OFF;
        pos.y += (syUtilsRandFloat() * ITKABIGON_JUMP_GFX_MUL_OFF) - ITKABIGON_JUMP_GFX_SUB_OFF;

        efManagerDustExpandLargeMakeEffect(&pos);

        ip->item_vars.kabigon.dust_effect_int = ITKABIGON_EFFECT_SPAWN_INT;
    }
    ip->item_vars.kabigon.dust_effect_int--;

    return FALSE;
}

// 0x8017E4A4
void itKabigonCommonProcDisplay(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);

    if (itDisplayCheckItemVisible(ip) != FALSE)
    {
        if ((ip->display_mode == nDBDisplayModeMaster) || (ip->is_hold))
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

            gcDrawDObjTreeForGObj(item_gobj);
        }
        else if (ip->display_mode == nDBDisplayModeMapCollision)
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

            gcDrawDObjTreeForGObj(item_gobj);
            itDisplayMapCollisions(item_gobj);
        }
        else if ((ip->damage_coll.hitstatus == nGMHitStatusNone) && (ip->attack_coll.attack_state == nGMAttackStateOff))
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

            gcDrawDObjTreeForGObj(item_gobj);
        }
        else itDisplayHitCollisions(item_gobj);
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x8017E600
void itKabigonJumpInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    func_800269C0_275C0(nSYAudioFGMKabigonJump);

    ip->multi = ITKABIGON_DROP_WAIT;

    ip->item_vars.kabigon.dust_effect_int = ITKABIGON_EFFECT_SPAWN_INT;

    ip->physics.vel_air.y = ITKABIGON_JUMP_VEL_Y;
}

// 0x8017E648
void itKabigonJumpSetStatus(GObj *item_gobj)
{
    itKabigonJumpInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITKabigonStatusDescs, nITKabigonStatusJump);
}

// 0x8017E67C
sb32 itKabigonCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        itKabigonJumpSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017E6C0
GObj* itKabigonMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITKabigonItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->attack_coll.interact_mask = GMHITCOLLISION_FLAG_FIGHTER;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataKabigonAnimJoint), 0.0F);

        if (ip->kind == nITKindKabigon)
        {
            func_800269C0_275C0(nSYAudioVoiceMBallKabigonAppear);
        }
        item_gobj->proc_display = itKabigonCommonProcDisplay;
    }
    return item_gobj;
}
