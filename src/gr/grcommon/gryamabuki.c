#include <gr/ground.h>
#include <ft/fighter.h>
#include <it/item.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012EB60
s32 dGRYamabukiMonsterAttackKind = GRYAMABUKI_MONSTER_WEAPON_MAX;

// 0x8012EB64
u16 dGRYamabukiMonsterMapObjKinds[/* */] =
{
    nMPMapObjKindMonster,
    nMPMapObjKindMonsterUnused2,
    nMPMapObjKindMonsterUnused3,
    nMPMapObjKindMonsterUnused4,
    nMPMapObjKindMonsterUnused1
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grYamabukiGateStatus
{
    nGRYamabukiGateStatusSleep,
    nGRYamabukiGateStatusWait,   // The gates open + lights on state is still considered Gate_Wait
    nGRYamabukiGateStatusOpen    // Fully open when a Pokémon appears
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8010ACD0
void grYamabukiGateUpdateSleep(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        gGRCommonStruct.yamabuki.gate_status = nGRYamabukiGateStatusWait;
        gGRCommonStruct.yamabuki.monster_wait = syUtilsRandIntRange(1000) + 1000;
    }
}

// 0x8010AD18
sb32 grYamabukiGateCheckPlayersNear(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if ((fp->ga == nMPKineticsGround) && ((fp->coll_data.floor_flags & MAP_VERTEX_MAT_MASK) == nMPMaterialDetect))
        {
            return TRUE;
        }
        else fighter_gobj = fighter_gobj->link_next;
    }
    return FALSE;
}

// 0x8010AD70
void grYamabukiGateMakeMonster(void)
{
    Vec3f pos;
    Vec3f vel;
    s32 mapobj;
    s32 item_id;

    gGRCommonStruct.yamabuki.gate_status = nGRYamabukiGateStatusOpen;
    gGRCommonStruct.yamabuki.gate_noentry = FALSE;

    mpCollisionGetMapObjIDsKind(dGRYamabukiMonsterMapObjKinds[0], &mapobj);
    mpCollisionGetMapObjPositionID(mapobj, &pos);

    vel.x = vel.y = vel.z = 0.0F;

    if ((dITManagerForceMonsterKind == 0) || (dITManagerForceMonsterKind > (nITKindGroundMonsterEnd - nITKindGroundMonsterStart + 1)))
    {
        item_id = syUtilsRandIntRange(nITKindGroundMonsterEnd - nITKindGroundMonsterStart + 1);

        if (item_id == gGRCommonStruct.yamabuki.monster_id_prev)
        {
            item_id = (item_id == (nITKindGroundMonsterEnd - nITKindGroundMonsterStart)) ? 0 : item_id + 1;
        }
        gGRCommonStruct.yamabuki.monster_id_prev = item_id;
    }
    else item_id = dITManagerForceMonsterKind - 1;

    gGRCommonStruct.yamabuki.monster_gobj = itManagerMakeItemSetupCommon(NULL, item_id + nITKindGroundMonsterStart, &pos, &vel, ITEM_FLAG_PARENT_GROUND);
}

// 0x8010AE3C
void grYamabukiGateSetPositionFar(void)
{
    gGRCommonStruct.yamabuki.gate_pos.x = 1600.0F;
    gGRCommonStruct.yamabuki.gate_pos.y = gMPCollisionYakumonoDObjs->dobjs[3]->translate.vec.f.y;
}

// 0x8010AE68
void grYamabukiGateSetPositionNear(void)
{
    gGRCommonStruct.yamabuki.gate_pos.x = 960.0F;
    gGRCommonStruct.yamabuki.gate_pos.y = gMPCollisionYakumonoDObjs->dobjs[3]->translate.vec.f.y;
}

// 0x8010AE94
void grYamabukiGateAddAnimOffset(intptr_t offset)
{
    gcAddAnimJointAll(gGRCommonStruct.yamabuki.gate_gobj, (uintptr_t)gGRCommonStruct.yamabuki.map_head + (intptr_t)offset, 0.0F);
    gcPlayAnimAll(gGRCommonStruct.yamabuki.gate_gobj);
}

// 0x8010AED8
void grYamabukiGateAddAnimOpen(void)
{
    grYamabukiGateAddAnimOffset((intptr_t)&llGRYamabukiMapGateOpenAnimJoint);
}

// 0x8010AEFC
void grYamabukiGateAddAnimClose(void)
{
    grYamabukiGateAddAnimOffset((intptr_t)&llGRYamabukiMapGateCloseAnimJoint);
}

// 0x8010AF20 - Allow entry inside Pokémon spawn hub?
void grYamabukiGateAddAnimOpenEntry(void)
{
    grYamabukiGateAddAnimOpen();
    grYamabukiGateSetPositionFar();
}

// 0x8010AF48
void grYamabukiGateUpdateWait(void)
{
    if (gGRCommonStruct.yamabuki.gate_wait == 0)
    {
        if (grYamabukiGateCheckPlayersNear() != FALSE)
        {
            grYamabukiGateMakeMonster();

            return;
        }
    }
    else if (--gGRCommonStruct.yamabuki.gate_wait == 0)
    {
        grYamabukiGateAddAnimOpenEntry();
        func_800269C0_275C0(nSYAudioFGMYamabukiGate);
    }
    gGRCommonStruct.yamabuki.monster_wait--;

    if (gGRCommonStruct.yamabuki.monster_wait == 0)
    {
        if (gGRCommonStruct.yamabuki.gate_wait != 0)
        {
            grYamabukiGateAddAnimOpen();
            func_800269C0_275C0(nSYAudioFGMYamabukiGate);
        }
        grYamabukiGateMakeMonster();
    }
}

// 0x8010AFF4
void grYamabukiGateUpdateOpen(void)
{
    if (gGRCommonStruct.yamabuki.monster_gobj == NULL)
    {
        grYamabukiGateSetClosedWait();
    }
    else if (gGRCommonStruct.yamabuki.gate_noentry == FALSE)
    {
        ITStruct *ip = itGetStruct(gGRCommonStruct.yamabuki.monster_gobj);

        gGRCommonStruct.yamabuki.gate_pos.x = DObjGetStruct(gGRCommonStruct.yamabuki.monster_gobj)->translate.vec.f.x - ip->coll_data.map_coll.width;
        gGRCommonStruct.yamabuki.gate_pos.y = gMPCollisionYakumonoDObjs->dobjs[3]->translate.vec.f.y;

        if (gGRCommonStruct.yamabuki.gate_pos.x < 960.0F)
        {
            gGRCommonStruct.yamabuki.gate_pos.x = 960.0F;

            gGRCommonStruct.yamabuki.gate_noentry = TRUE;
        }
        else if (gGRCommonStruct.yamabuki.gate_pos.x > 1600.0F)
        {
            gGRCommonStruct.yamabuki.gate_pos.x = 1600.0F;
        }
    }
}

// 0x8010B0AC
void grYamabukiGateClearMonsterGObj(void)
{
    gGRCommonStruct.yamabuki.monster_gobj = NULL;
}

// 0x8010B0B8
void grYamabukiGateSetClosedWait(void)
{
    gGRCommonStruct.yamabuki.gate_status = nGRYamabukiGateStatusWait;
    gGRCommonStruct.yamabuki.gate_wait = 1000;

    gGRCommonStruct.yamabuki.monster_wait = syUtilsRandIntRange(1000) + 1000;

    grYamabukiGateSetPositionNear();
    grYamabukiGateAddAnimClose();
}

// 0x8010B108
void grYamabukiGateUpdateYakumonoPos(void)
{
    mpCollisionSetYakumonoPosID(3, &gGRCommonStruct.yamabuki.gate_pos);
}

// 0x8010B130
void grYamabukiGateProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.yamabuki.gate_status)
    {
    case nGRYamabukiGateStatusSleep:
        grYamabukiGateUpdateSleep();
        break;

    case nGRYamabukiGateStatusWait:
        grYamabukiGateUpdateWait();
        grYamabukiGateUpdateYakumonoPos();
        break;

    case nGRYamabukiGateStatusOpen:
        grYamabukiGateUpdateOpen();
        grYamabukiGateUpdateYakumonoPos();
        break;
    }
}

void grYamabukiMakeGate(void)
{
    GObj *gate_gobj;

    gGRCommonStruct.yamabuki.gate_gobj = gate_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gate_gobj, gcDrawDObjTreeDLLinksForGObj, 6, GOBJ_PRIORITY_DEFAULT, ~0);
    gcSetupCustomDObjs
    (
        gate_gobj, 
        (DObjDesc*) 
        ((uintptr_t)gGRCommonStruct.yamabuki.map_head + (intptr_t)&llGRYamabukiMapMapHead), 
        NULL, 
        nGCMatrixKindTraRotRpyR, 
        nGCMatrixKindNull, 
        nGCMatrixKindNull
    );
    gcAddGObjProcess(gate_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);
    grYamabukiGateAddAnimClose();
}

// 0x8010B250
void grYamabukiInitGroundVars(void)
{
    gGRCommonStruct.yamabuki.map_head = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRYamabukiMapMapHead);

    mpCollisionSetYakumonoOnID(3);

    gGRCommonStruct.yamabuki.gate_wait = 1;
    gGRCommonStruct.yamabuki.item_head = (void*) ((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRYamabukiMapItemHead);

    dGRYamabukiMonsterAttackKind = GRYAMABUKI_MONSTER_WEAPON_MAX;

    gGRCommonStruct.yamabuki.monster_id_prev = (nITKindGroundMonsterEnd - nITKindGroundMonsterStart) + 1;
    gGRCommonStruct.yamabuki.gate_pos.z = 0.0F;

    grYamabukiGateSetPositionNear();
    grYamabukiMakeGate();
    grYamabukiGateUpdateYakumonoPos();

    gGRCommonStruct.yamabuki.gate_status = 0;
}

// 0x8010B2EC
GObj* grYamabukiMakeGround(void)
{
    GObj *ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjProcess(ground_gobj, grYamabukiGateProcUpdate, nGCProcessKindFunc, 4);
    grYamabukiInitGroundVars();

    return ground_gobj;
}
