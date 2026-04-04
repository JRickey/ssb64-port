#include <gr/ground.h>
#include <ft/fighter.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012EA60
GRZebesAcid dGRZebesAcidAttributes[/* */] =
{
    { 1200, 60, 70, -3600.0F },
    {  180, 60, 70, -1000.0F },
    {   60, 60, 70,  -200.0F },
    {   60, 60, 70,  1800.0F },
    {   60, 60, 70, -3600.0F },
    {  120,  0,  0,  2600.0F },
    {   30,  0,  0, -1000.0F },
    { 1200, 60, 70,  -500.0F },
    {  600, 60, 70,  -400.0F },
    {  100, 60, 70,   800.0F },
    { 1200, 60, 70,  1200.0F },
    {   60,  0,  0, -1000.0F },
    {   60, 60, 70,     0.0F },
    {   60, 60, 70, -1000.0F },
    {  120, 60, 70,   500.0F },
    {  200, 60, 70, -3000.0F }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grZebesStatus
{
    nGRZebesAcidStatusWait,
    nGRZebesAcidStatusNormal,
    nGRZebesAcidStatusShake,
    nGRZebesAcidStatusRise
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80108020
void grZebesAcidSetLevelStep(void)
{
    gGRCommonStruct.zebes.acid_level_step = 
    ((dGRZebesAcidAttributes[gGRCommonStruct.zebes.acid_attr_id].acid_level + (syUtilsRandFloat() * 250.0F)) - gGRCommonStruct.zebes.acid_level_curr) / 240.0F;
}

// 0x80108088
void grZebesAcidSetRandomWait(void)
{
    s32 index = gGRCommonStruct.zebes.acid_attr_id;

    gGRCommonStruct.zebes.acid_level_wait = dGRZebesAcidAttributes[index].acid_wait_base +
                                          dGRZebesAcidAttributes[index].acid_random_min +
                                          syUtilsRandIntRange(dGRZebesAcidAttributes[index].acid_random_max - dGRZebesAcidAttributes[index].acid_random_min);
}

// 0x801080EC
GObj* grZebesMakeAcid(void)
{
    // Many linker things here
    GObj *map_gobj;
    void *map_head;

    map_head = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRZebesMapAcidDObjDesc);
    gGRCommonStruct.zebes.map_head = map_head;

    map_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);
    gGRCommonStruct.zebes.map_gobj = map_gobj;

    gcAddGObjDisplay(map_gobj, gcDrawDObjTreeDLLinksForGObj, 12, GOBJ_PRIORITY_DEFAULT, ~0);
    gcSetupCustomDObjs
    (
        map_gobj, 
        (DObjDesc*) ((intptr_t)&llGRZebesMapAcidDObjDesc + (uintptr_t)map_head), 
        NULL, 
        nGCMatrixKindTra, 
        nGCMatrixKindNull,
        nGCMatrixKindNull
    );
    gcAddMObjAll(map_gobj, lbRelocGetFileData(MObjSub***, map_head, &llGRZebesMapAcidMObjSub));
    gcAddGObjProcess(map_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);
    gcAddAnimAll
    (
        map_gobj,
        lbRelocGetFileData(AObjEvent32**, map_head, &llGRZebesMapAcidAnimJoint),
        lbRelocGetFileData(AObjEvent32***, map_head, &llGRZebesMapAcidMatAnimJoint),
        0.0F
    );
    gcPlayAnimAll(map_gobj);

    gGRCommonStruct.zebes.acid_status = nGRZebesAcidStatusWait;
    gGRCommonStruct.zebes.acid_level_curr = dGRZebesAcidAttributes[ARRAY_COUNT(dGRZebesAcidAttributes) - 1].acid_level;
    gGRCommonStruct.zebes.acid_attr_id = 0;

    gGRCommonStruct.zebes.attack_coll = lbRelocGetFileData(GRAttackColl*, ((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRZebesMapMapHeader), &llGRZebesMapAcidGRAttackColl);

    grZebesAcidSetRandomWait();

    DObjGetStruct(map_gobj)->translate.vec.f.y = gGRCommonStruct.zebes.acid_level_curr;

    return map_gobj;
}

// 0x80108240
void grZebesAcidUpdateWait(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        gGRCommonStruct.zebes.acid_status = nGRZebesAcidStatusNormal;
    }
}

// 0x80108268
void grZebesAcidUpdateRumble(void)
{
    if (gGRCommonStruct.zebes.rumble_wait == 0)
    {
        efManagerQuakeMakeEffect(0);

        gGRCommonStruct.zebes.rumble_wait = 18;
    }
    gGRCommonStruct.zebes.rumble_wait--;
}

// 0x801082B4
void grZebesAcidUpdateNormal(void)
{
    gGRCommonStruct.zebes.acid_level_wait--;

    if (gGRCommonStruct.zebes.acid_level_wait == 0)
    {
        gGRCommonStruct.zebes.acid_status = nGRZebesAcidStatusShake;
        gGRCommonStruct.zebes.acid_level_wait = 18;
        gGRCommonStruct.zebes.rumble_wait = 0;
    }
}

// 0x801082EC
void grZebesAcidUpdateShake(void)
{
    gGRCommonStruct.zebes.acid_level_wait--;

    if (gGRCommonStruct.zebes.acid_level_wait == 0)
    {
        gGRCommonStruct.zebes.acid_status = nGRZebesAcidStatusRise;
        gGRCommonStruct.zebes.acid_level_wait = 240;

        grZebesAcidSetLevelStep();
    }
    grZebesAcidUpdateRumble();
}

// 0x8010833C
void grZebesAcidUpdateRise(void)
{
    gGRCommonStruct.zebes.acid_level_curr += gGRCommonStruct.zebes.acid_level_step;

    DObjGetStruct(gGRCommonStruct.zebes.map_gobj)->translate.vec.f.y = gGRCommonStruct.zebes.acid_level_curr;

    gGRCommonStruct.zebes.acid_level_wait--;

    if (gGRCommonStruct.zebes.acid_level_wait == 0)
    {
        gGRCommonStruct.zebes.acid_status = nGRZebesAcidStatusNormal;
        gGRCommonStruct.zebes.acid_attr_id++;

        if (gGRCommonStruct.zebes.acid_attr_id >= ARRAY_COUNT(dGRZebesAcidAttributes))
        {
            gGRCommonStruct.zebes.acid_attr_id = 0;
        }
        grZebesAcidSetRandomWait();
    }
    grZebesAcidUpdateRumble();
}

// 0x801083C4
void grZebesProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.zebes.acid_status)
    {
    case nGRZebesAcidStatusWait:
        grZebesAcidUpdateWait();
        break;

    case nGRZebesAcidStatusNormal:
        grZebesAcidUpdateNormal();
        break;

    case nGRZebesAcidStatusShake:
        grZebesAcidUpdateShake();
        break;

    case nGRZebesAcidStatusRise:
        grZebesAcidUpdateRise();
        break;
    }
}

// 0x80108448
GObj* grZebesMakeGround(void)
{
    GObj *ground_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);
    GObj *acid_gobj = grZebesMakeAcid();

    gcAddGObjProcess(ground_gobj, grZebesProcUpdate, nGCProcessKindFunc, 4);
    ftMainCheckAddGroundHazard(acid_gobj, grZebesAcidCheckGetDamageKind);

    return ground_gobj;
}

// 0x801084AC
sb32 grZebesAcidCheckGetDamageKind(GObj *ground_gobj, GObj *fighter_gobj, GRAttackColl **gr_attack_coll, s32 *kind)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->acid_wait == 0)
    {
        DObj *dobj = DObjGetStruct(ground_gobj);

        if (DObjGetStruct(fighter_gobj)->translate.vec.f.y < (dobj->translate.vec.f.y + dobj->child->translate.vec.f.y))
        {
            *gr_attack_coll = gGRCommonStruct.zebes.attack_coll;
            *kind = nGMHitEnvironmentAcid;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x8010850C
void grZebesAcidGetLevelInfo(f32 *current, f32 *step)
{
    *current = gGRCommonStruct.zebes.acid_level_curr;

    *step = (gGRCommonStruct.zebes.acid_status == nGRZebesAcidStatusRise) ? gGRCommonStruct.zebes.acid_level_step : 0.0F;
}
