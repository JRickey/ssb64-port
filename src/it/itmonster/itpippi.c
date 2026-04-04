#include <it/item.h>
#include <sys/develop.h>
#include <sys/matrix.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018B370
void (*dITPippiStatusProcList[/* */])(GObj*) =
{
    itIwarkAttackSetStatus, 
    itKabigonJumpSetStatus, 
    itTosakintoAppearSetStatus, 
    itNyarsAttackSetStatus,
    itLizardonFallSetStatus, 
    itSpearFlySetStatus, 
    itKamexAppearSetStatus, 
    itMLuckyAppearSetStatus,
    itStarmieNFollowSetStatus, 
    itSawamuraFallSetStatus, 
    itDogasAttackSetStatus, 
    itMewFlySetStatus
};

// 0x8018B3A0
ITDesc dITPippiItemDesc = 
{
    nITKindPippi,                           // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataPippiItemAttributes,     // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itPippiCommonProcUpdate,                // Proc Update
    itPippiCommonProcMap,                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

#if !defined(DAIRANTOU_OPT0)

// 0x8018B3D4 - why
ITStatusDesc dITPippiStatusDesc = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

#endif

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80183210
void itPippiCommonSelectMonster(GObj *item_gobj)
{
    s32 kind;
    s32 index;
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    index = syUtilsRandIntRange(ARRAY_COUNT(dITPippiStatusProcList));

    kind = index + nITKindMBallMonsterStart;

    if ((kind == nITKindSpear) || (kind == nITKindKamex))
    {
        if (syUtilsRandIntRange(2) == 0)
        {
            dobj->rotate.vec.f.y = F_CST_DTOR32(180.0F);

            ip->lr = +1;
        }
        else ip->lr = -1;
    }
    if ((kind == nITKindPippi) || (kind == nITKindTosakinto) || (kind == nITKindMLucky))
    {
        ip->attack_coll.attack_state = nGMAttackStateOff;
    }
    if (kind == nITKindSawamura)
    {
        ip->multi = ITSAWAMURA_KICK_WAIT;
    }
    if ((kind == nITKindSawamura) || (kind == nITKindStarmie))
    {
        item_gobj->proc_display = itPippiCommonMoveDLProcDisplay;

        gcMoveGObjDLHead(item_gobj, 18, item_gobj->dl_link_priority);
    }
    if (kind == nITKindLizardon)
    {
        ip->multi = ITLIZARDON_LIFETIME;
    }
    dITPippiStatusProcList[index](item_gobj);
}

// 0x80183344
void itPippiCommonProcDisplay(GObj *item_gobj)
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

// 0x801834A0 - Render routine of Hitmonlee / Starmie metronome abilities
void itPippiCommonMoveDLProcDisplay(GObj *item_gobj)
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

// 0x801835FC
sb32 itPippiCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        ip->physics.vel_air.x = ip->physics.vel_air.y = 0.0F;

        itPippiCommonSelectMonster(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x80183650
sb32 itPippiCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x80183690
GObj* itPippiMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITPippiItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        gcAddXObjForDObjFixed(dobj, 0x48, 0);

        dobj->translate.vec.f = *pos;

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataPippiDataStart), 0.0F);
        func_800269C0_275C0(nSYAudioVoiceMBallPippiAppear);

        item_gobj->proc_display = itPippiCommonProcDisplay;
    }
    return item_gobj;
}
