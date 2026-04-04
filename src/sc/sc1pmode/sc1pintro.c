#include <ft/fighter.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>

extern void func_800266A0_272A0();
extern void* func_800269C0_275C0(u16);
extern u32 sySchedulerGetTicCount();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80134DF0
u32 dSC1PIntroFileIDs[/* */] = { &llSC1PIntroFileID, &llCharacterNamesFileID, &llBonusPictureFileID, &llBonusPicturePlatformFileID };

// 0x80134E00
Lights1 dSC1PIntroLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x80134E18
Gfx dSC1PIntroDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dSC1PIntroLights1),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80135C20
s32 sSC1PIntroPad0x80135C20[2];

// 0x80135C28
s32 sSC1PIntroStage;

// 0x80135C2C
GObj *sSC1PIntroVSNameGObj;

// 0x80135C30
GObj *sSC1PIntroNameGObj;

// 0x80135C34
s32 sSC1PIntroPad0x80135C34;

// 0x80135C38
void *sSC1PIntroFigatreeHeaps[32];

// 0x80135CB8
sb32 sSC1PIntroIsAnnouncedFighterName;

// 0x80135CBC
sb32 sSC1PIntroIsAnnouncedVersus;

// 0x80135CC0
sb32 sSC1PIntroIsAnnouncedVSFighterName;

// 0x80135CC4
s32 sSC1PIntroPad0x80135CC4;

// 0x80135CC8
FTDemoDesc sSC1PIntroPlayerFighterDemoDesc;

// 0x80135CD4
s32 sSC1PIntroPad0x80135CD4;

// 0x80135CD8
FTDemoDesc sSC1PIntroAlly1FighterDemoDesc;

// 0x80135CE4
s32 sSC1PIntroPad0x80135CE4;

// 0x80135CE8
FTDemoDesc sSC1PIntroAlly2FighterDemoDesc;

// 0x80135CF4
s32 sSC1PIntroUnk0x80135CF4;

// 0x80135CF8
s32 sc1PIntroTotalTimeTics;

// 0x80135D00
LBFileNode sSC1PIntroForceStatusBuffer[7];

// 0x80135D38
LBFileNode sSC1PIntroStatusBuffer[100];

// 0x80136058
void *sSC1PIntroFiles[ARRAY_COUNT(dSC1PIntroFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void sc1PIntroFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dSC1PIntroDisplayList);
}

// 0x80131B24
void sc1PIntroMakeSky(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroSkySprite));
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 59.0F;
}

// 0x80131BA8
void sc1PIntroMakeBanners(void)
{
    GObj *gobj;
    SObj *sobj;
    
    gobj = gcMakeGObjSPAfter(0, NULL, 18, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 28, GOBJ_PRIORITY_DEFAULT, ~0);
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroBannerTopSprite));
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroBannerBottomSprite));
    sobj->pos.x = 10.0F;
    sobj->pos.y = 182.0F;
}

// 0x80131C58 - unused?
void func_ovl24_80131C58(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 311, 59);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 182, 311, 231);
    
    if (sSC1PIntroStage < 4)
    {
        gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
    }
    else gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x7F, 0x7F, 0x7F, 0xFF);
    
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 148, 31, 156, 39);
    
    if (sSC1PIntroStage < 8)
    {
        gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
    }
    else gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x7F, 0x7F, 0x7F, 0xFF);
    
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 202, 31, 210, 39);
    
    if (sSC1PIntroStage < 12)
    {
        gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
    }
    else gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0x7F, 0x7F, 0x7F, 0xFF);
    
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 256, 31, 264, 39);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    lbCommonClearExternSpriteParams();
}

// 0x80131ECC - unused?
void func_ovl24_80131ECC(void)
{
    return;
}

// 0x80131ED4
void sc1PIntroMakeVSDecal(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroVSDecalSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->pos.x = 135.0F;
    sobj->pos.y = 104.0F;
}

// 0x80131F6C
sb32 sc1PIntroCheckNotBonusStage(s32 stage)
{
    switch (stage)
    {
    case nSC1PGameStageBonus1:
    case nSC1PGameStageBonus2:
    case nSC1PGameStageBonus3:
        return FALSE;

    default:
        return TRUE;
    }
}

// 0x80131F98
void sc1PIntroMakeLabels(s32 stage)
{
    GObj *gobj;
    SObj *sobj;

    // 0x80134E40
    intptr_t number_offsets[/* */] =    
    {
        &llSC1PIntroNumber1Sprite,
        &llSC1PIntroNumber2Sprite,
        &llSC1PIntroNumber3Sprite,
        &llSC1PIntroNumber1Sprite,
        &llSC1PIntroNumber4Sprite,
        &llSC1PIntroNumber5Sprite,
        &llSC1PIntroNumber6Sprite,
        &llSC1PIntroNumber2Sprite,
        &llSC1PIntroNumber7Sprite,
        &llSC1PIntroNumber8Sprite,
        &llSC1PIntroNumber9Sprite,
        &llSC1PIntroNumber3Sprite,
        &llSC1PIntroNumber10Sprite,
        0x0
    };
    
    gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    
    if (sc1PIntroCheckNotBonusStage(stage) == FALSE)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroBonusTextSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
        
        sobj->pos.x = 15.0F;
        sobj->pos.y = 18.0F;
    }
    if (stage == nSC1PGameStageBoss)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroFinalTextSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
        
        sobj->pos.x = 15.0F;
        sobj->pos.y = 17.0F;
    }
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroStageTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
        
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
    
    sobj->pos.x = 23.0F;
    sobj->pos.y = 33.0F;
    
    if (stage != nSC1PGameStageBoss)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], number_offsets[stage]));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
        
        sobj->pos.x = 80.0F;
        sobj->pos.y = 33.0F;
    }
}

// 0x801321C0
void sc1PIntroMakeFigures(s32 stage)
{
    GObj *gobj;
    SObj *sobj;
    s32 i;

    // 0x80134E78
    intptr_t figure_offsets[/* */] =
    {
        &llSC1PIntroLinkMarkerSprite,
        &llSC1PIntroYoshiMarkerSprite,
        &llSC1PIntroFoxMarkerSprite,
        &llSC1PIntroBonusMarkerSprite,
        &llSC1PIntroMarioBrosMarkerSprite,
        &llSC1PIntroPikachuMarkerSprite,
        &llSC1PIntroDKMarkerSprite,
        &llSC1PIntroBonusMarkerSprite,
        &llSC1PIntroKirbyMarkerSprite,
        &llSC1PIntroSamusMarkerSprite,
        &llSC1PIntroMarioMarkerSprite,
        &llSC1PIntroBonusMarkerSprite,
        &llSC1PIntroExclamationMarkSprite,
        &llSC1PIntroBossMarkerSprite,
        0x0
    };

    // 0x80134EB4
    Vec2i figure_pos[/* */] =
    {
        // Link, Yoshi Team 
        {  99, 18 }, { 115, 26 },

        // Fox, Bonus Stage 1
        { 131, 18 }, { 148, 33 },

        // Mario Bros., Pikachu
        { 156, 26 }, { 172, 18 },

        // Giant DK, Bonus Stage 2
        { 189, 26 }, { 205, 33 },

        // Kirby Team, Samus
        { 212, 18 }, { 230, 26 },

        // Metal Mario, Bonus Stage 3
        { 246, 18 }, { 261, 33 },

        // Fighting Polygon Team, Final Stage
        { 270, 34 }, { 280, 23 }
    };

    gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);

    for (i = nSC1PGameStageCommonStart; i <= nSC1PGameStageCommonEnd; i++)
    {
        if (i >= stage)
        {
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], figure_offsets[i]));

            sobj->sprite.attr &= ~SP_FASTCOPY;
            sobj->sprite.attr |= SP_TRANSPARENT;
            
            sobj->pos.x = figure_pos[i].x;
            sobj->pos.y = figure_pos[i].y;
        }
    }
}

// 0x80132354
void sc1PIntroMakeBonusTasks(s32 stage)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    
    switch (stage)
    {
    case nSC1PGameStageBonus1:
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroBreakTheTargetsTextSprite));

#if defined(REGION_US)
        sobj->pos.x = 160 - (sobj->sprite.width / 2);
#else
        sobj->pos.x = 83.0F;
#endif
        sobj->pos.y = 197.0F;
        break;
        
    case nSC1PGameStageBonus2:
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroBoardThePlatformsTextSprite));
        
#if defined(REGION_US)
        sobj->pos.x = 160 - (sobj->sprite.width / 2);
#else
        sobj->pos.x = 91.0F;
#endif
        sobj->pos.y = 197.0F;
        break;
        
    case nSC1PGameStageBonus3:
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroRaceToTheFinishTextSprite));
        
#if defined(REGION_US)
        sobj->pos.x = 160 - (sobj->sprite.width / 2);
#else
        sobj->pos.x = 59.0F;
#endif
        sobj->pos.y = 197.0F;
        break;

#ifdef AVOID_UB
    // WARNING: sobj is uninitialized in the default case, but still accessed outside of the switch.
    default:
        sobj = NULL;
        break;
#endif
    }

#ifdef AVOID_UB
    // Make sure sobj is a valid pointer...
    if (sobj != NULL)
    {
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
    
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    }
#else
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
#endif
}

// 0x801324EC
void sc1PIntroMakeVSName(s32 stage)
{
    GObj *gobj;
    SObj *sobj;

    // 0x80134F24
    intptr_t opponent_offsets[/* */] =
    {
        &llCharacterNamesLinkSprite,
        &llSC1PIntroYoshiTeamVS18TextSprite,
        &llSC1PIntroFoxMcCloudTextSprite,
        0x0,
        &llSC1PIntroMarioBrosTextSprite,
        &llCharacterNamesPikachuSprite,
        &llSC1PIntroGiantDKTextSprite,
        0x0,
        &llSC1PIntroKirbyTeamVS8TextSprite,
        &llSC1PIntroSamusAranTextSprite,
        &llSC1PIntroMetalMarioTextSprite,
        0x0,
        &llSC1PIntroFightingPolygonTeamVS30TextSprite,
        &llSC1PIntroMasterHandTextSprite,
        0x0
    };

    sSC1PIntroVSNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroVSTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
    
    if ((stage == nSC1PGameStageLink) || (stage == nSC1PGameStagePikachu))
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[1], opponent_offsets[stage]));
    }
    else sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], opponent_offsets[stage]));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
}

// 0x80132634
s32 sc1PIntroGetAlliesNum(s32 stage)
{
    switch (stage)
    {
    case nSC1PGameStageMario:
        return 1;
        
    case nSC1PGameStageDonkey:
        return 2;
        
    default:
        return 0;
    }
}

// 0x80132668
void sc1PIntroMakeName(s32 stage)
{
    GObj *gobj;
    SObj *sobj;
    
    // 0x80134F60
    intptr_t name_offsets[/* */] =
    {
        &llCharacterNamesMarioSprite,
        &llCharacterNamesFoxSprite,
        &llCharacterNamesDonkeySprite,
        &llCharacterNamesSamusSprite,
        &llCharacterNamesLuigiSprite,
        &llCharacterNamesLinkSprite,
        &llCharacterNamesYoshiSprite,
        &llCharacterNamesCaptainSprite,
        &llCharacterNamesKirbySprite,
        &llCharacterNamesPikachuSprite,
        &llCharacterNamesPurinSprite,
        &llCharacterNamesNessSprite
    };

    sSC1PIntroNameGObj = gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[1], name_offsets[sSC1PIntroPlayerFighterDemoDesc.fkind]));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue = 0xFF;
    
    if (sc1PIntroGetAlliesNum(stage) != 0)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroDashSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
    
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
        
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[1], name_offsets[sSC1PIntroAlly1FighterDemoDesc.fkind]));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
    
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
        
        if (sc1PIntroGetAlliesNum(stage) == 2)
        {
            sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[1], name_offsets[sSC1PIntroAlly2FighterDemoDesc.fkind]));
            
            sobj->sprite.attr &= ~SP_FASTCOPY;
            sobj->sprite.attr |= SP_TRANSPARENT;
    
            sobj->sprite.red = 0xFF;
            sobj->sprite.green = 0xFF;
            sobj->sprite.blue = 0xFF;
        }
    }
}

// 0x8013283C
s32 sc1PIntroGetPlayerNameOffsetX(s32 stage)
{
    s32 allies_num = sc1PIntroGetAlliesNum(stage);
    s32 ally1_name_width;
    s32 max_name_width;
    SObj *sobj;
    
    if (allies_num != 0)
    {
        if (allies_num == 1)
        {
            sobj = SObjGetStruct(sSC1PIntroNameGObj);

            return sobj->sprite.width + sobj->next->sprite.width + sobj->next->next->sprite.width + 10;
        }
        else
        {
            sobj = SObjGetStruct(sSC1PIntroNameGObj);

            ally1_name_width = sobj->next->next->sprite.width;

            if (ally1_name_width > sobj->next->next->next->sprite.width)
            {
                max_name_width = sobj->next->next->sprite.width;
            }
            else max_name_width = sobj->next->next->next->sprite.width;
            
            return sobj->sprite.width + sobj->next->sprite.width + max_name_width + 10;
        }
    }
    else return SObjGetStruct(sSC1PIntroNameGObj)->sprite.width + 10;
}

// 0x801328F4
s32 sc1PIntroGetVSNameOffsetX(s32 stage)
{
    SObj *sobj = SObjGetStruct(sSC1PIntroVSNameGObj);
    
    return sobj->sprite.width + sobj->next->sprite.width + 10;
}

// 0x8013291C
s32 sc1PIntroGetTotalNameOffsetX(s32 stage)
{
    return sc1PIntroGetPlayerNameOffsetX(stage) + sc1PIntroGetVSNameOffsetX(stage);
}

// 0x8013294C
void sc1PIntroSetNamePositions(s32 stage)
{
    s32 total_offset;
    s32 allies_num;
    SObj *sobj, *next_sobj;
    
    total_offset = 160 - (sc1PIntroGetTotalNameOffsetX(stage) / 2);

    sobj = SObjGetStruct(sSC1PIntroNameGObj);

    sobj->pos.x = total_offset;
    sobj->pos.y = 196.0F;

    allies_num = sc1PIntroGetAlliesNum(stage);
    
    if (allies_num != 0)
    {
        sobj->next->pos.x = sobj->pos.x + sobj->sprite.width;
        sobj->next->pos.y = 196.0F;
        
        if (allies_num == 1)
        {
            next_sobj = sobj->next;
            
            next_sobj->next->pos.x = next_sobj->pos.x + next_sobj->sprite.width;
            next_sobj->next->pos.y = 196.0F;
        }
        else
        {
            next_sobj = sobj->next;
            
            next_sobj->next->pos.x = next_sobj->pos.x + next_sobj->sprite.width;
            next_sobj->next->pos.y = 188.0F;
            
            next_sobj->next->next->pos.x = next_sobj->pos.x + next_sobj->sprite.width;
            next_sobj->next->next->pos.y = 204.0F;
        }
    }
    sobj = SObjGetStruct(sSC1PIntroVSNameGObj);

    sobj->pos.x = sc1PIntroGetPlayerNameOffsetX(stage) + total_offset;
    sobj->pos.y = 196.0F;

    sobj->next->pos.x = sobj->pos.x + sobj->sprite.width + 10.0F;
#if defined(REGION_US)
    sobj->next->pos.y = (stage == nSC1PGameStageZako) ? 190.0F : 196.0F;
#else
    sobj->next->pos.y = (stage == nSC1PGameStageDonkey) ? 188.0F : 196.0F;
#endif
}

// 0x80132B10
void sc1PIntroMakeNameAll(s32 stage)
{
    sc1PIntroMakeName(stage);
    sc1PIntroMakeVSName(stage);
    sc1PIntroSetNamePositions(stage);
}

// 0x80132B40
void sc1PIntroMakeStageInfo(s32 stage)
{
    if (sc1PIntroCheckNotBonusStage(stage) != FALSE)
    {
        sc1PIntroMakeNameAll(stage);
    }
    else sc1PIntroMakeBonusTasks(stage);
}

// 0x80132B80
f32 sc1PIntroGetFighterVelocityZ(s32 card_anim_frame_id)
{
    // 0x80134F90
    f32 vel_z[/* */] =
    {
        30.0F,
        40.0F,
        40.0F,
        50.0F,
        50.0F,
        50.0F
    };
    
    return vel_z[card_anim_frame_id];
}

// 0x80132BD4
void sc1PIntroFighterProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *dobj = DObjGetStruct(fighter_gobj);
    s32 id = fp->card_anim_frame_id;
    
    if (dobj->translate.vec.f.z < 0.0F)
    {
        dobj->translate.vec.f.z += sc1PIntroGetFighterVelocityZ(id);
        
        if (dobj->translate.vec.f.z > 0.0F)
        {
            dobj->translate.vec.f.z = 0.0F;
        }
    }
}

// 0x80132C44
f32 sc1PIntroGetFighterPositionZ(s32 card_anim_frame_id)
{
    // 0x80134FA8
    f32 pos_z[/* */] =
    {
         -500.0F,
         -600.0F,
         -800.0F,
         -700.0F,
         -900.0F,
        -1100.0F
    };
    return pos_z[card_anim_frame_id];
}

// 0x80132C98
void sc1PIntroMakeFighter(FTDemoDesc fighter, s32 card_anim_frame_id, void **figatree)
{
    FTStruct *fp;
    GObj *fighter_gobj;
    FTDesc desc = dFTManagerDefaultFighterDesc;
    
    // 0x80134FC0
    s32 dl_links[/* */] =
    {
        31,
        31,
        30,
        31,
        30,
        29
    };
    
    desc.fkind = fighter.fkind;
    desc.costume = fighter.costume;
    desc.shade = fighter.shade;
    
    desc.figatree_heap = *figatree;
    
    desc.player = 0;
    
    fighter_gobj = ftManagerMakeFighter(&desc);
    
    scSubsysFighterSetStatus(fighter_gobj, 0x1000D);
    
    fp = ftGetStruct(fighter_gobj);
    
    fp->card_anim_frame_id = card_anim_frame_id;
    
    gcMoveGObjDL(fighter_gobj, dl_links[card_anim_frame_id], GOBJ_PRIORITY_DEFAULT);
    gcAddGObjProcess(fighter_gobj, sc1PIntroFighterProcUpdate, nGCProcessKindFunc, 1);
    
    DObjGetStruct(fighter_gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(fighter_gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(fighter_gobj)->translate.vec.f.z = sc1PIntroGetFighterPositionZ(card_anim_frame_id);
    
    DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
}

// 0x80132E04
void sc1PIntroInitAllyTextParams(SObj *sobj)
{
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
}

// 0x80132E2C
void sc1PIntroMakeAllyText(s32 stage)
{
    GObj *gobj = gcMakeGObjSPAfter(0, NULL, 19, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 27, GOBJ_PRIORITY_DEFAULT, ~0);
    
    if (stage == nSC1PGameStageMario)
    {
        SObj *sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroAllyTextSprite));
        
        sobj->pos.x = 80.0F;
        sobj->pos.y = 80.0F;
        
        sc1PIntroInitAllyTextParams(sobj);
    }
    else
    {
        SObj *sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroAllyText2Sprite));
        
        sobj->pos.x = 80.0F;
        sobj->pos.y = 70.0F;
        
        sc1PIntroInitAllyTextParams(sobj);
        
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[0], &llSC1PIntroAllyTextSprite));
        
        sobj->pos.x = 90.0F;
        sobj->pos.y = 100.0F;
        
        sc1PIntroInitAllyTextParams(sobj);
    }
}

// 0x80132F40
f32 sc1PIntroGetVSFighterVelocityZ(s32 stage, s32 fkind)
{
    if (stage == nSC1PGameStageMario)
    {
        if (fkind == nFTKindMario)
        {
            return 40.0F;
        }
        else return 40.0F;
    }
    else return 30.0F;
}

// 0x80132F84
void sc1PIntroVSFighterProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *dobj = DObjGetStruct(fighter_gobj);
    s32 id = fp->card_anim_frame_id;
    
    switch (sSC1PIntroStage)
    {
    case nSC1PGameStageYoshi:
        fighter_gobj->flags = ((id * 3) < sc1PIntroTotalTimeTics) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;
        break;
        
    case nSC1PGameStageKirby:
        fighter_gobj->flags = ((id * 10) < sc1PIntroTotalTimeTics) ? GOBJ_FLAG_NONE : GOBJ_FLAG_HIDDEN;
        break;

    case nSC1PGameStageZako:
        break;

    default:
        if (dobj->translate.vec.f.z < 0.0F)
        {
            dobj->translate.vec.f.z += sc1PIntroGetVSFighterVelocityZ(sSC1PIntroStage, fp->fkind);
            
            if (dobj->translate.vec.f.z > 0.0F)
            {
                dobj->translate.vec.f.z = 0.0F;
            }
        }
        break;
    }
}

// 0x80133080
void sc1PIntroSetKirbyTeamModelPartIDs(GObj *fighter_gobj, s32 fkind)
{
    // 0x80134FD8
    s32 modelpart_ids[/* */] =
    {
        13,
        5,
        10,
        4,
        7,
        12,
        6,
        8
    };

    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 id = fp->card_anim_frame_id;
    
    if (fkind == nFTKindKirby)
    {
        s32 modelpart_id = modelpart_ids[id];
        
        if (modelpart_id == 13)
        {
            ftParamSetModelPartID(fighter_gobj, 6, gSC1PManagerKirbyTeamModelPartID);
        }
        else ftParamSetModelPartID(fighter_gobj, 6, modelpart_id);
    }
}

// 0x8013312C
f32 sc1PIntroGetVSFighterPositionZ(s32 stage, s32 fkind)
{
    if (stage == nSC1PGameStageMario)
    {
        if (fkind == nFTKindMario)
        {
            return -600.0F;
        }
        else return -800.0F;
    }
    else return -500.0F;
}

// 0x80133170
void sc1PIntroVSFighterProcDisplay(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_2CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_PASS, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xFF, 0xFF, 0xFF, 0x00);
    
    if (sSC1PIntroStage == nSC1PGameStageZako)
    {
        if (((fp->fkind * 2) - 28) < sc1PIntroTotalTimeTics)
        {
            ftMainSetStatus(fighter_gobj, 0x1000E, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
            gcPlayAnimAll(fighter_gobj);
            gcEndProcessAll(fighter_gobj);
            ftDisplayMainProcDisplay(fighter_gobj);
        }
        if (((fp->fkind * 2) - 8) < sc1PIntroTotalTimeTics)
        {
            ftMainSetStatus(fighter_gobj, 0x1000E, 1.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
            gcPlayAnimAll(fighter_gobj);
            gcEndProcessAll(fighter_gobj);
            ftDisplayMainProcDisplay(fighter_gobj);
        }
        if (((fp->fkind * 2) + 12) < sc1PIntroTotalTimeTics)
        {
            ftMainSetStatus(fighter_gobj, 0x1000E, 2.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
            gcPlayAnimAll(fighter_gobj);
            gcEndProcessAll(fighter_gobj);
            ftDisplayMainProcDisplay(fighter_gobj);
        }
    }
    else ftDisplayMainProcDisplay(fighter_gobj);
    
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x80133398
GObj* sc1PIntroMakeVSFighter(s32 fkind, s32 stage, s32 card_anim_frame_id, void **figatree, u8 dl_link)
{
    GObj *fighter_gobj;
    FTStruct *fp;
    FTDesc desc = dFTManagerDefaultFighterDesc;

    desc.fkind = fkind;
    desc.costume = ftParamGetCostumeCommonID(fkind, 0);
    desc.figatree_heap = *figatree;
    desc.proc_display = sc1PIntroVSFighterProcDisplay;
    desc.player = 0;
    desc.unk_rebirth_0x1C = 4;
    desc.unk_rebirth_0x1D = 4;
    
    if ((stage == nSC1PGameStageYoshi) || (stage == nSC1PGameStageZako))
    {
        desc.detail = nFTPartsDetailLow;
    }
    fighter_gobj = ftManagerMakeFighter(&desc);
    ftMainSetStatus(fighter_gobj, 0x1000E, card_anim_frame_id, 0.0F, FTSTATUS_PRESERVE_NONE);
    gcPlayAnimAll(fighter_gobj);
    gcEndProcessAll(fighter_gobj);
    
    fp = ftGetStruct(fighter_gobj);
    fp->card_anim_frame_id = card_anim_frame_id;
    
    gcMoveGObjDL(fighter_gobj, dl_link, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjProcess(fighter_gobj, sc1PIntroVSFighterProcUpdate, nGCProcessKindFunc, 1);
    
    DObjGetStruct(fighter_gobj)->translate.vec.f.x = 0.0F;
    DObjGetStruct(fighter_gobj)->translate.vec.f.y = 0.0F;
    DObjGetStruct(fighter_gobj)->translate.vec.f.z = 0.0F;

    DObjGetStruct(fighter_gobj)->scale.vec.f.x = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.y = 1.0F;
    DObjGetStruct(fighter_gobj)->scale.vec.f.z = 1.0F;
    
    if ((stage != nSC1PGameStageKirby) && (stage != nSC1PGameStageYoshi) && (stage != nSC1PGameStageZako))
    {
        DObjGetStruct(fighter_gobj)->translate.vec.f.z = sc1PIntroGetVSFighterPositionZ(stage, fkind);
    }
    if (stage == nSC1PGameStageBoss)
    {
        fp->colanim.is_use_light = TRUE;
        
        fp->colanim.light_angle_x = -20.0F;
        fp->colanim.light_angle_y = -30.0F;
    }
    return fighter_gobj;
}

// 0x8013357C
void sc1PIntroMakeBonusPicture(s32 stage)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 17, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 26, GOBJ_PRIORITY_DEFAULT, ~0);
    
    switch (stage)
    {
    case nSC1PGameStageBonus1:
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[2], &llBonusPictureTargetSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->pos.x = 103.0F;
        sobj->pos.y = 63.0F;
        break;
        
    case nSC1PGameStageBonus2:
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[3], &llBonusPicturePlatformSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->pos.x = 45.0F;
        sobj->pos.y = 55.0F;
        break;
        
    case nSC1PGameStageBonus3:
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sSC1PIntroFiles[2], &llBonusPictureRaceSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->pos.x = 10.0F;
        sobj->pos.y = 55.0F;
        break;
    }
}

// 0x801336CC
CObjDesc* sc1PIntroGetStageCObjDesc(CObjDesc *cobj_desc, s32 stage)
{
    // 0x80134FF8
    CObjDesc cobj_descs[/* */] =
    {
        // VS Link
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Yoshi Team
        { { 2069.77F, 487.31F, 843.2F }, { -138.97F, 345.57F, 842.82F }, 0.0F },

        // VS Fox
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // Break the Targets
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Mario Bros.
        { { 1053.95F, 404.97F, 364.82F }, { -311.21F, 309.63F, 332.14F }, 0.0F },

        // VS Pikachu
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Giant DK
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // Board the Platforms
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Kirby Team
        { { 3940.63F, 1402.99F, 44.52F }, { -403.2F, 486.08F, 45.02F }, 0.0F },

        // VS Samus
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Metal Mario
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // Race to the Finish
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Fighting Polygon Team
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F },

        // VS Master Hand
        { { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 0.0F }, 0.0F }
    };

    *cobj_desc = cobj_descs[stage];
    
    return cobj_desc;
}

// 0x8013376C
CObj* sc1PIntroMakeStageCamera(s32 stage, u32 dl_link)
{
    GObj *gobj;
    CObj *cobj;
    CObjDesc cobj_desc;

    // 0x80135180
    intptr_t camanim_joints[/* */] =
    {
        &llSC1PIntroStageLinkCamAnimJoint,
        &llSC1PIntroStageYoshiCamAnimJoint,
        &llSC1PIntroStageFoxCamAnimJoint,
        0x0,
        &llSC1PIntroStageMarioCamAnimJoint,
        &llSC1PIntroStagePikachuCamAnimJoint,
        &llSC1PIntroStageDonkeyCamAnimJoint,
        0x0,
        &llSC1PIntroStageKirbyCamAnimJoint,
        &llSC1PIntroStageSamusCamAnimJoint,
        &llSC1PIntroStageMMarioCamAnimJoint,
        0x0,
        &llSC1PIntroStageZakoCamAnimJoint,
        &llSC1PIntroStageBossCamAnimJoint
    };
    
    gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        40,
        COBJ_MASK_DLLINK(dl_link),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    gcAddXObjForCamera(CObjGetStruct(gobj), nGCMatrixKindPerspFastF, 0);
    gcAddXObjForCamera(CObjGetStruct(gobj), 15, 0);
    
    cobj = CObjGetStruct(gobj);
    
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sSC1PIntroFiles[0], camanim_joints[stage]), 0.0F);
    gcPlayCamAnim(gobj);
    
    switch (stage)
    {
    case nSC1PGameStageMario:
        sc1PIntroGetStageCObjDesc(&cobj_desc, stage);
        
        cobj->vec.eye.x = cobj_desc.eye.x;
        cobj->vec.eye.y = cobj_desc.eye.y;
        cobj->vec.eye.z = cobj_desc.eye.z;
        cobj->vec.at.x = cobj_desc.at.x;
        cobj->vec.at.y = cobj_desc.at.y;
        cobj->vec.at.z = cobj_desc.at.z;
        break;
    }
    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;
    
    return cobj;
}

// 0x80133930
sb32 sSC1PIntroCheckCostumeUsed(s32 stage, s32 fkind, s32 color)
{
    switch (stage)
    {
    case nSC1PGameStageMario:
        if ((fkind == sSC1PIntroPlayerFighterDemoDesc.fkind) && (ftParamGetCostumeCommonID(fkind, color) == sSC1PIntroPlayerFighterDemoDesc.costume))
        {
            return TRUE;
        }
        else if ((fkind == sSC1PIntroAlly1FighterDemoDesc.fkind) && (ftParamGetCostumeCommonID(fkind, color) == sSC1PIntroAlly1FighterDemoDesc.costume))
        {
            return TRUE;
        }
        else return FALSE;
        
    case nSC1PGameStageDonkey:
        if ((fkind == sSC1PIntroPlayerFighterDemoDesc.fkind) && (ftParamGetCostumeCommonID(fkind, color) == sSC1PIntroPlayerFighterDemoDesc.costume))
        {
            return TRUE;
        }
        else if ((fkind == sSC1PIntroAlly1FighterDemoDesc.fkind) && (ftParamGetCostumeCommonID(fkind, color) == sSC1PIntroAlly1FighterDemoDesc.costume))
        {
            return TRUE;
        }
        else if ((fkind == sSC1PIntroAlly2FighterDemoDesc.fkind) && (ftParamGetCostumeCommonID(fkind, color) == sSC1PIntroAlly2FighterDemoDesc.costume))
        {
            return TRUE;
        }
        else return FALSE;
        
    default:
        if ((fkind == sSC1PIntroPlayerFighterDemoDesc.fkind) && (ftParamGetCostumeCommonID(fkind, color) == sSC1PIntroPlayerFighterDemoDesc.costume))
        {
            return TRUE;
        }
        else return FALSE;
    }
}

// 0x80133AC8
void sc1PIntroInitVSFighters(s32 stage)
{
    s32 i;
    s32 costume;
    GObj *fighter_gobj;
    CObj *cobj;

    // 0x801351B8
    s32 fkinds[/* */] =
    {
        nFTKindLink,
        nFTKindYoshi,
        nFTKindFox,
        0,
        nFTKindMario,
        nFTKindPikachu,
        nFTKindDonkey,
        0,
        nFTKindKirby,
        nFTKindSamus,
        nFTKindMMario,
        0,
        0,
        nFTKindBoss
    };
    
    switch (stage)
    {
    case nSC1PGameStageYoshi:
        sc1PIntroMakeStageCamera(stage, 32);

        for (i = 0; i < SC1PGAME_STAGE_YOSHI_TEAM_COUNT; i++)
        {
            fighter_gobj = sc1PIntroMakeVSFighter(nFTKindYoshi, stage, i, &sSC1PIntroFigatreeHeaps[i + 1], 32);
            
            if ((sSC1PIntroPlayerFighterDemoDesc.costume == i % SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT) && (sSC1PIntroPlayerFighterDemoDesc.fkind == nFTKindYoshi))
            {
                ftParamInitAllParts(fighter_gobj, i % SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT, 1);
            }
            else ftParamInitAllParts(fighter_gobj, i % SC1PGAME_STAGE_YOSHI_VARIATIONS_COUNT, 0);
        }
        break;
        
    case nSC1PGameStageKirby:
        sc1PIntroMakeStageCamera(stage, 32);
        
        for (i = 0; i < SC1PGAME_STAGE_KIRBY_TEAM_COUNT; i++)
        {
            fighter_gobj = sc1PIntroMakeVSFighter(nFTKindKirby, stage, i, &sSC1PIntroFigatreeHeaps[i + 1], 32);
            sc1PIntroSetKirbyTeamModelPartIDs(fighter_gobj, stage);
            
            if (sSC1PIntroCheckCostumeUsed(stage, nFTKindKirby, 0) != FALSE)
            {
                ftParamInitAllParts(fighter_gobj, ftParamGetCostumeCommonID(nFTKindMario, 1), 0);
            }
        }
        break;
        
    case nSC1PGameStageZako:
        sc1PIntroMakeStageCamera(stage, 32);

        for (i = nFTKindNStart; i <= nFTKindNEnd; i++)
        {
            if ((i != nFTKindNLuigi) && (i != nFTKindNPurin))
            {
                sc1PIntroMakeVSFighter(i, stage, 0, &sSC1PIntroFigatreeHeaps[i - nFTKindNStart + 1], 32);
            }
        }
        break;
        
    case nSC1PGameStageLink:
    case nSC1PGameStageFox:
    case nSC1PGameStagePikachu:
    case nSC1PGameStageSamus:
    case nSC1PGameStageMMario:
    case nSC1PGameStageBoss:
        sc1PIntroMakeStageCamera(stage, 32);
        fighter_gobj = sc1PIntroMakeVSFighter(fkinds[stage], stage, 0, &sSC1PIntroFigatreeHeaps[1], 32);
        
        if (sSC1PIntroPlayerFighterDemoDesc.fkind == fkinds[stage])
        {
            if (ftParamGetCostumeCommonID(sSC1PIntroPlayerFighterDemoDesc.fkind, 0) == sSC1PIntroPlayerFighterDemoDesc.costume)
            {
                ftParamInitAllParts(fighter_gobj, ftParamGetCostumeCommonID(sSC1PIntroPlayerFighterDemoDesc.fkind, 1), 0);
            }
            else ftParamInitAllParts(fighter_gobj, ftParamGetCostumeCommonID(sSC1PIntroPlayerFighterDemoDesc.fkind, 0), 0);
        }
        break;
        
    case nSC1PGameStageDonkey:
        sc1PIntroMakeStageCamera(stage, 32);
        sc1PIntroMakeVSFighter(nFTKindDonkey, stage, 0, &sSC1PIntroFigatreeHeaps[3], 32);
        break;
        
    case nSC1PGameStageMario:
        sc1PIntroMakeStageCamera(stage, 32);
        fighter_gobj = sc1PIntroMakeVSFighter(nFTKindMario, stage, 0, &sSC1PIntroFigatreeHeaps[2], 32);
        
        if (sSC1PIntroCheckCostumeUsed(stage, nFTKindMario, 0) != FALSE)
        {
            ftParamInitAllParts(fighter_gobj, ftParamGetCostumeCommonID(nFTKindMario, 1), 0);
        }
        cobj = sc1PIntroMakeStageCamera(stage, 33);

        cobj->flags |= COBJ_FLAG_ZBUFFER;
        
        fighter_gobj = sc1PIntroMakeVSFighter(nFTKindLuigi, stage, 0, &sSC1PIntroFigatreeHeaps[3], 33);
        
        if (sSC1PIntroCheckCostumeUsed(stage, nFTKindLuigi, 0) != FALSE)
        {
            ftParamInitAllParts(fighter_gobj, ftParamGetCostumeCommonID(nFTKindLuigi, 1), 0);
        }
        break;
    }
}

// 0x80133EE0
CObjDesc* sc1PIntroGetFighterCObjDesc(CObjDesc *cobj_desc, s32 fkind, s32 cobj_id)
{
    
    // 0x801351F0
    CObjDesc cobj_descs[/* */][6] =
    {
        // Mario
        {
            { {  -729.13F,  349.20F,  270.28F }, {  -28.13F,  259.63F,  270.95F }, 0.0F },
            { {  -827.69F,  449.49F,  337.69F }, {  -17.17F,  345.93F,  338.46F }, 0.0F },
            { { -1075.95F,  302.25F,  470.96F }, {  -39.79F,  169.86F,  471.94F }, 0.0F },
            { { -1276.62F,  569.83F,  543.27F }, {   -9.44F,  407.92F,  544.47F }, 0.0F },
            { { -1877.44F,  454.02F,  814.20F }, {  -33.91F,  218.46F,  815.94F }, 0.0F },
            { { -2326.52F,  229.09F, 1052.27F }, {  -69.62F,  -59.28F, 1054.41F }, 0.0F }
        },

        // Fox
        {
            { { -2012.70F,  664.39F,  272.85F }, {  -80.70F,  355.99F,  272.06F }, 0.0F },
            { { -2158.35F,  797.01F,  304.41F }, {  -63.66F,  462.64F,  303.55F }, 0.0F },
            { { -2642.66F,  703.19F,  461.11F }, {  -90.24F,  295.75F,  460.06F }, 0.0F },
            { { -2606.32F,  932.33F,  458.59F }, {  -53.67F,  524.85F,  457.55F }, 0.0F },
            { { -3580.93F,  919.81F,  644.49F }, {  -79.76F,  360.93F,  643.06F }, 0.0F },
            { { -5104.57F,  926.59F,  931.62F }, { -116.45F,  100.35F,  929.59F }, 0.0F }
        },

        // Donkey Kong
        {
            { { -1264.73F,  -49.59F,  460.69F }, {   32.21F,   76.68F,  463.59F }, 0.0F },
            { { -1429.57F,  104.68F,  555.82F }, {   15.58F,  245.38F,  559.05F }, 0.0F },
            { { -2023.22F, -260.50F,  859.49F }, {   44.55F,  -59.18F,  864.11F }, 0.0F },
            { { -2397.12F,   96.45F,  908.66F }, {    6.50F,  330.48F,  914.03F }, 0.0F },
            { { -3077.75F, -292.25F, 1292.64F }, {   36.74F,   10.99F, 1299.60F }, 0.0F },
            { { -3804.93F, -776.41F, 1675.02F }, {   75.76F, -398.57F, 1683.69F }, 0.0F }
        },

        // Samus
        {
            { { -1069.84F,  471.28F,  220.65F }, {   45.20F,  393.09F,  220.33F }, 0.0F },
            { { -1071.18F,  585.65F,  236.15F }, {   53.18F,  506.81F,  235.83F }, 0.0F },
            { { -1513.12F,  424.67F,  416.28F }, {   39.84F,  315.78F,  415.83F }, 0.0F },
            { { -1822.46F,  690.46F,  517.87F }, {   56.90F,  558.69F,  517.32F }, 0.0F },
            { { -2428.29F,  552.51F,  755.05F }, {   44.38F,  379.13F,  754.33F }, 0.0F },
            { { -2998.73F,  317.84F,  977.22F }, {   25.28F,  105.80F,  976.33F }, 0.0F }
        },

        // Luigi
        {
            { { -4979.88F,  884.23F,  252.07F }, {  -17.97F,  317.93F,  251.95F }, 0.0F },
            { { -6177.62F, 1103.97F,  269.10F }, {   -8.62F,  399.90F,  268.95F }, 0.0F },
            { { -8189.23F, 1127.74F,  464.65F }, {  -31.80F,  196.71F,  464.45F }, 0.0F },
            { {-10816.48F, 1705.40F,  581.71F }, {    -0.5F,  470.93F,  581.45F }, 0.0F },
            { {-15142.78F, 1962.65F,  921.32F }, {  -27.14F,  237.45F,  920.95F }, 0.0F },
            { {-16234.35F, 1823.03F, 1060.85F }, {  -56.91F,  -23.36F, 1060.45F }, 0.0F }
        },
        
        // Link
        {
            { {  -968.97F,  165.73F,  181.09F }, {   42.61F,  301.45F,  187.51F }, 0.0F },
            { { -1126.84F,  226.32F,  228.08F }, {   31.54F,  381.74F,  235.44F }, 0.0F },
            { { -1700.41F,  -38.62F,  435.95F }, {   54.99F,  196.89F,  447.09F }, 0.0F },
            { { -1625.61F,  212.50F,  372.92F }, {   23.62F,  433.77F,  383.39F }, 0.0F },
            { { -2207.49F,   12.20F,  572.23F }, {   41.68F,  289.56F,  586.51F }, 0.0F },
            { { -2697.16F, -294.17F,  744.12F }, {   69.09F,   76.97F,  761.69F }, 0.0F }
        },
        
        // Yoshi
        {
            { { -1096.39F,  442.92F,  312.58F }, { -130.07F,  331.90F,  312.28F }, 0.0F },
            { { -1134.16F,  544.90F,  339.59F }, { -118.99F,  428.26F,  339.27F }, 0.0F },
            { { -1822.34F,  392.97F,  627.30F }, { -145.09F,  200.26F,  626.78F }, 0.0F },
            { { -1917.37F,  709.39F,  674.83F }, { -110.43F,  501.78F,  674.27F }, 0.0F },
            { { -2571.02F,  558.52F,  960.53F }, { -135.97F,  278.75F,  959.78F }, 0.0F },
            { { -3304.35F,  301.56F, 1270.76F }, { -174.57F,  -58.04F, 1269.79F }, 0.0F }    
        },

        // Captain Falcon
        {
            { { -2980.81F,  431.00F,  270.55F }, {   20.49F,  312.77F,  273.81F }, 0.0F },
            { { -3537.67F,  536.99F,  304.95F }, {   23.76F,  396.71F,  308.82F }, 0.0F },
            { { -4625.33F,  412.20F,  453.26F }, {   17.00F,  229.34F,  458.31F }, 0.0F },
            { { -5064.61F,  648.18F,  470.29F }, {   25.59F,  447.67F,  475.82F }, 0.0F },
            { { -7368.79F,  565.30F,  743.78F }, {   18.46F,  274.30F,  751.81F }, 0.0F },
            { { -8198.14F,  384.29F,  881.89F }, {    9.90F,   60.97F,  890.80F }, 0.0F }
        },
        
        // Kirby
        {
            { { -1238.45F, -482.45F,  275.18F }, {   94.10F,  219.98F,  274.88F }, 0.0F },
            { { -1619.20F, -583.12F,  340.26F }, {   52.84F,  298.27F,  339.89F }, 0.0F },
            { { -2031.12F,-1077.24F,  552.85F }, {  167.14F,   81.54F,  552.37F }, 0.0F },
            { { -2358.44F, -902.75F,  576.43F }, {   23.98F,  353.11F,  575.90F }, 0.0F },
            { { -3283.15F,-1635.53F,  904.63F }, {  125.25F,  161.15F,  903.87F }, 0.0F },
            { { -4012.13F,-2413.80F, 1200.30F }, {  287.82F, -147.14F, 1199.34F }, 0.0F }
        },

        // Pikachu
        {
            { { -3417.38F,  735.40F,  240.90F }, {   95.04F,  223.38F,  249.07F }, 0.0F },
            { { -3723.29F,  879.00F,  279.28F }, {  109.08F,  320.36F,  288.10F }, 0.0F },
            { { -5957.12F,  963.02F,  526.64F }, {   74.03F,   83.86F,  540.52F }, 0.0F },
            { { -5691.34F, 1214.33F,  486.75F }, {  115.52F,  367.86F,  500.12F }, 0.0F },
            { { -7985.64F, 1355.66F,  755.47F }, {   87.34F,  178.85F,  774.05F }, 0.0F },
            { { -9876.19F, 1353.75F, 1021.62F }, {   47.12F,  -92.78F, 1044.46F }, 0.0F }
        },
    
        // Jigglypuff
        {
            { { -2121.60F,  198.00F,  268.03F }, { -256.38F,  168.53F,  266.69F }, 0.0F },
            { { -2329.80F,  292.30F,  314.68F }, { -254.91F,  259.52F,  313.18F }, 0.0F },
            { { -3731.67F,  106.92F,  554.69F }, { -258.02F,   52.05F,  552.19F }, 0.0F },
            { { -3735.01F,  407.01F,  494.20F }, { -253.32F,  352.01F,  491.68F }, 0.0F },
            { { -5382.05F,  219.51F,  792.89F }, { -256.48F,  138.54F,  789.19F }, 0.0F },
            { { -7459.81F,  -97.71F, 1159.38F }, { -261.75F, -211.42F, 1154.19F }, 0.0F }
        },
    
        // Ness
        {
            { {  -847.72F,  177.76F,  328.15F }, {  -92.79F,  213.04F,  327.90F }, 0.0F },
            { {  -953.41F,  250.40F,  368.18F }, {  -96.39F,  290.45F,  367.90F }, 0.0F },
            { { -1368.12F,   54.33F,  565.32F }, {  -88.09F,  114.14F,  564.90F }, 0.0F },
            { { -1444.96F,  329.54F,  562.34F }, { -101.09F,  392.34F,  561.90F }, 0.0F },
            { { -2103.41F,   84.53F,  848.56F }, {  -91.01F,  178.57F,  847.90F }, 0.0F },
            { { -2636.95F, -229.72F, 1121.23F }, {  -77.43F, -110.11F, 1120.39F }, 0.0F }
        }
    };

    *cobj_desc = cobj_descs[fkind][cobj_id];
    
    return cobj_desc;
}

// 0x80133F88 - unused?
void func_ovl24_80133F88(void)
{
    return;
}

// 0x80133F90
void sc1PIntroMakeFighterCamera(s32 fkind, s32 cobj_id)
{
    GObj* gobj;
    CObj *cobj;
    CObjDesc cobj_desc;

    // 0x801359D0
    intptr_t camanim_joints[/* */] =
    {
        &llSC1PIntroFighterMarioCamAnimJoint,
        &llSC1PIntroFighterFoxCamAnimJoint,
        &llSC1PIntroFighterDonkeyCamAnimJoint,
        &llSC1PIntroFighterSamusCamAnimJoint,
        &llSC1PIntroFighterLuigiCamAnimJoint,
        &llSC1PIntroFighterLinkCamAnimJoint,
        &llSC1PIntroFighterYoshiCamAnimJoint,
        &llSC1PIntroFighterCaptainCamAnimJoint,
        &llSC1PIntroFighterKirbyCamAnimJoint,
        &llSC1PIntroFighterPikachuCamAnimJoint,
        &llSC1PIntroFighterPurinCamAnimJoint,
        &llSC1PIntroFighterNessCamAnimJoint
    };

    // 0x80135A00 - col 0 is dl_link, col 1 is dl_link_priority
    s32 dl_links[/* */][2] =
    {
        { 31, 50 },
        { 31, 50 },
        { 30, 60 },
        { 31, 50 },
        { 30, 60 },
        { 29, 70 }
    };
    s32 unused;

    gobj = gcMakeCameraGObj
    (
        nGCCommonKindSceneCamera,
        NULL,
        16,
        GOBJ_PRIORITY_DEFAULT,
        func_80017EC0,
        dl_links[cobj_id][1],
        COBJ_MASK_DLLINK(dl_links[cobj_id][0]) | COBJ_MASK_DLLINK(15) |
        COBJ_MASK_DLLINK(10)                   | COBJ_MASK_DLLINK(18),
        -1,
        FALSE,
        nGCProcessKindFunc,
        NULL,
        1,
        FALSE
    );
    gcAddXObjForCamera(CObjGetStruct(gobj), nGCMatrixKindPerspFastF, 0);
    gcAddXObjForCamera(CObjGetStruct(gobj), 7, 0);

    cobj = CObjGetStruct(gobj);

    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
    
    sc1PIntroGetFighterCObjDesc(&cobj_desc, fkind, cobj_id);
    
    gcAddCObjCamAnimJoint(cobj, lbRelocGetFileData(AObjEvent32*, sSC1PIntroFiles[0], camanim_joints[fkind]), 0.0F);
    gcPlayCamAnim(gobj);
    
    cobj->vec.eye.x = cobj_desc.eye.x;
    cobj->vec.eye.y = cobj_desc.eye.y;
    cobj->vec.eye.z = cobj_desc.eye.z;

    cobj->vec.at.x = cobj_desc.at.x;
    cobj->vec.at.y = cobj_desc.at.y;
    cobj->vec.at.z = cobj_desc.at.z;

    cobj->projection.persp.near = 128.0F;
    cobj->projection.persp.far = 16384.0F;

    cobj->flags |= COBJ_FLAG_ZBUFFER;
}

// 0x80134190
void sc1PIntroInitFighters(s32 stage)
{
    switch (stage)
    {
    case nSC1PGameStageDonkey:
        sc1PIntroMakeFighterCamera(sSC1PIntroAlly2FighterDemoDesc.fkind, 5);
        sc1PIntroMakeFighter(sSC1PIntroAlly2FighterDemoDesc, 5, &sSC1PIntroFigatreeHeaps[2]);
        sc1PIntroMakeFighterCamera(sSC1PIntroAlly1FighterDemoDesc.fkind, 4);
        sc1PIntroMakeFighter(sSC1PIntroAlly1FighterDemoDesc, 4, &sSC1PIntroFigatreeHeaps[1]);
        sc1PIntroMakeFighterCamera(sSC1PIntroPlayerFighterDemoDesc.fkind, 3);
        sc1PIntroMakeFighter(sSC1PIntroPlayerFighterDemoDesc, 3, &sSC1PIntroFigatreeHeaps[0]);
        sc1PIntroMakeAllyText(stage);
        break;

    case nSC1PGameStageMario:
        sc1PIntroMakeFighterCamera(sSC1PIntroAlly1FighterDemoDesc.fkind, 2);
        sc1PIntroMakeFighter(sSC1PIntroAlly1FighterDemoDesc, 2, &sSC1PIntroFigatreeHeaps[1]);
        sc1PIntroMakeFighterCamera(sSC1PIntroPlayerFighterDemoDesc.fkind, 1);
        sc1PIntroMakeFighter(sSC1PIntroPlayerFighterDemoDesc, 1, &sSC1PIntroFigatreeHeaps[0]);
        sc1PIntroMakeAllyText(stage);
        break;

    default:
        sc1PIntroMakeFighterCamera(sSC1PIntroPlayerFighterDemoDesc.fkind, 0);
        sc1PIntroMakeFighter(sSC1PIntroPlayerFighterDemoDesc, 0, &sSC1PIntroFigatreeHeaps[0]);
        break;
    }
}

// 0x8013438C
void sc1PIntroMakeBannersCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindSceneCamera,
            NULL,
            16,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            30,
            COBJ_MASK_DLLINK(28),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x8013442C
void sc1PIntroMakeDecalsCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindSceneCamera,
            NULL,
            16,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            20,
            COBJ_MASK_DLLINK(27),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x801344CC
void sc1PIntroMakePicturesCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindSceneCamera,
            NULL,
            16,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            80,
            COBJ_MASK_DLLINK(26),
            ~0,
            FALSE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, 10.0F, 10.0F, 310.0F, 230.0F);
}

// 0x8013456C
s32 sc1PIntroGetFighterAllocsNum(s32 stage)
{
    // 0x80135A30
    s32 allocs_num[/* */] =
    {
        2,
        21,
        2,
        0,
        4,
        2,
        4,
        0,
        9,
        2,
        2,
        0,
        13,
        2
    };
    return allocs_num[stage];
}

// 0x801345CC
void sc1PIntroUpdateAnnounce(void)
{
    // 0x80135A68
    u32 fighter_voices[/* */] =
    {
        nSYAudioVoiceAnnounceMario,
        nSYAudioVoiceAnnounceFox,
        nSYAudioVoiceAnnounceDonkey,
        nSYAudioVoiceAnnounceSamus,
        nSYAudioVoiceAnnounceLuigi,
        nSYAudioVoiceAnnounceLink,
        nSYAudioVoiceAnnounceYoshi,
        nSYAudioVoiceAnnounceCaptain,
        nSYAudioVoiceAnnounceKirby,
        nSYAudioVoiceAnnouncePikachu,
        nSYAudioVoiceAnnouncePurin,
        nSYAudioVoiceAnnounceNess
    };

    // 0x80135A98
    u32 vs_fighter_voices[/* */] =
    {
        nSYAudioVoiceAnnounceLink,
        nSYAudioVoiceAnnounceYoshiTeam,
        nSYAudioVoiceAnnounceFox,
        0,
        nSYAudioVoiceAnnounceMarioBros,
        nSYAudioVoiceAnnouncePikachu,
        nSYAudioVoiceAnnounceGDonkey,
        0,
        nSYAudioVoiceAnnounceKirbyTeam,
        nSYAudioVoiceAnnounceSamus,
        nSYAudioVoiceAnnounceMMario,
        0,
        nSYAudioVoiceAnnounceZako,
        nSYAudioVoiceAnnounceZako
    };

    // 0x80135AD0
    u32 announce_wait_tics[/* */] =
    {
        50,
        50,
        70,
        50,
        50,
        50,
        50,
        70,
        50,
        50,
        50,
        50
    };
    u32 tic = announce_wait_tics[sSC1PIntroPlayerFighterDemoDesc.fkind] + 1;
    
    if ((sc1PIntroCheckNotBonusStage(sSC1PIntroStage) != FALSE) && (sSC1PIntroStage != nSC1PGameStageBoss))
    {
        if ((sySchedulerGetTicCount() >= 2) && (sSC1PIntroIsAnnouncedFighterName == FALSE))
        {
            func_800269C0_275C0(fighter_voices[sSC1PIntroPlayerFighterDemoDesc.fkind]);

            sSC1PIntroIsAnnouncedFighterName = TRUE;
        }
        if ((tic < sySchedulerGetTicCount()) && (sSC1PIntroIsAnnouncedVersus == FALSE))
        {
            func_800269C0_275C0(nSYAudioVoiceAnnounceVersus);

            sSC1PIntroIsAnnouncedVersus = TRUE;
        }
        if (((tic + 60) < sySchedulerGetTicCount()) && (sSC1PIntroIsAnnouncedVSFighterName == FALSE))
        {
            func_800269C0_275C0(vs_fighter_voices[sSC1PIntroStage]);

            sSC1PIntroIsAnnouncedVSFighterName = TRUE;
        }
    }
    else if (sc1PIntroTotalTimeTics == 1)
    {
        switch (sSC1PIntroStage)
        {
        case nSC1PGameStageBonus1:
            func_800269C0_275C0(nSYAudioVoiceAnnounceBreakTheTargets);
            break;
            
        case nSC1PGameStageBonus2:
            func_800269C0_275C0(nSYAudioVoiceAnnounceBoardThePlatforms);
            break;
            
        case nSC1PGameStageBonus3:
            func_800269C0_275C0(nSYAudioVoiceAnnounceRaceToTheFinish);
            break;
        }
    }
}

// 0x80134810
void sc1PIntroInitVars(void)
{
    sSC1PIntroStage = gSCManagerSceneData.spgame_stage;

    sSC1PIntroPlayerFighterDemoDesc.fkind = gSCManager1PGameBattleState.players[gSCManagerSceneData.player].fkind;
    sSC1PIntroPlayerFighterDemoDesc.costume = gSCManager1PGameBattleState.players[gSCManagerSceneData.player].costume;
    sSC1PIntroPlayerFighterDemoDesc.shade = gSCManager1PGameBattleState.players[gSCManagerSceneData.player].shade;
    
    sSC1PIntroAlly1FighterDemoDesc.fkind = gSCManager1PGameBattleState.players[gSCManagerSceneData.ally_players[0]].fkind;
    sSC1PIntroAlly1FighterDemoDesc.costume = gSCManager1PGameBattleState.players[gSCManagerSceneData.ally_players[0]].costume;
    sSC1PIntroAlly1FighterDemoDesc.shade = gSCManager1PGameBattleState.players[gSCManagerSceneData.ally_players[0]].shade;
    
    sSC1PIntroAlly2FighterDemoDesc.fkind = gSCManager1PGameBattleState.players[gSCManagerSceneData.ally_players[1]].fkind;
    sSC1PIntroAlly2FighterDemoDesc.costume = gSCManager1PGameBattleState.players[gSCManagerSceneData.ally_players[1]].costume;
    sSC1PIntroAlly2FighterDemoDesc.shade = gSCManager1PGameBattleState.players[gSCManagerSceneData.ally_players[1]].shade;
    
    sSC1PIntroIsAnnouncedFighterName = FALSE;
    sSC1PIntroIsAnnouncedVersus = FALSE;
    sSC1PIntroIsAnnouncedVSFighterName = FALSE;
    
    sSC1PIntroUnk0x80135CF4 = 0;
    sc1PIntroTotalTimeTics = 0;
}

// 0x801348EC - unused?
void func_ovl24_801348EC(void)
{
    return;
}

// 0x801348F4
void sc1PIntroFuncRun(GObj *gobj)
{
    sc1PIntroTotalTimeTics++;
    
    sc1PIntroUpdateAnnounce();
    
    if (sySchedulerGetTicCount() >= 60)
    {
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | B_BUTTON | START_BUTTON) != FALSE)
        {
            func_800266A0_272A0();
            
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;
            
            func_ovl24_801348EC();
            syTaskmanSetLoadScene();
        }
        if (sSC1PIntroUnk0x80135CF4 != 0)
        {
            sSC1PIntroUnk0x80135CF4--;
        }
        if ((scSubsysControllerGetPlayerStickInRangeLR(-30, 30) != FALSE) && (scSubsysControllerGetPlayerStickInRangeUD(-30, 30) != FALSE))
        {
            sSC1PIntroUnk0x80135CF4 = 0;
        }
        if (sySchedulerGetTicCount() > 360)
        {
            func_800266A0_272A0();
            
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;
            
            func_ovl24_801348EC();
            syTaskmanSetLoadScene();
        }
    }
}

// 0x801349F8
void sc1PIntroSetupFighterFiles(s32 stage)
{    
    // 0x80135B00
    s32 fkinds[/* */] =
    {
        nFTKindLink,
        nFTKindYoshi,
        nFTKindFox,
        0,
        nFTKindMario,
        nFTKindPikachu,
        nFTKindDonkey,
        0,
        nFTKindKirby,
        nFTKindSamus,
        nFTKindMMario,
        0,
        0,
        nFTKindBoss
    };
    s32 i;
    
    ftManagerSetupFilesAllKind(sSC1PIntroPlayerFighterDemoDesc.fkind);
    
    switch (stage)
    {
    case nSC1PGameStageDonkey:
        ftManagerSetupFilesAllKind(sSC1PIntroAlly1FighterDemoDesc.fkind);
        ftManagerSetupFilesAllKind(sSC1PIntroAlly2FighterDemoDesc.fkind);
        ftManagerSetupFilesAllKind(nFTKindDonkey);
        break;
        
    case nSC1PGameStageMario:
        ftManagerSetupFilesAllKind(sSC1PIntroAlly1FighterDemoDesc.fkind);
        ftManagerSetupFilesAllKind(nFTKindMario);
        ftManagerSetupFilesAllKind(nFTKindLuigi);
        break;
        
    case nSC1PGameStageZako:
        for (i = nFTKindNStart; i <= nFTKindNEnd; i++)
        {
            if ((i != nFTKindNLuigi) && (i != nFTKindNPurin))
            {
                ftManagerSetupFilesAllKind(i);
            }
        }
        break;
        
    default:
        ftManagerSetupFilesAllKind(fkinds[stage]);
        break;
    }
}

// 0x80134B38
void sc1PIntroFuncStart(void)
{
    LBRelocSetup rl_setup;
    s32 i;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sSC1PIntroStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sSC1PIntroStatusBuffer);
    rl_setup.force_status_buffer = sSC1PIntroForceStatusBuffer;
    rl_setup.force_status_buffer_size = ARRAY_COUNT(sSC1PIntroForceStatusBuffer);
    
    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dSC1PIntroFileIDs, sSC1PIntroFiles);
    gcMakeGObjSPAfter(0, sc1PIntroFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_FILLCOLOR | COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
    sc1PIntroInitVars();
    efParticleInitAll();
    efManagerInitEffects();
    ftManagerAllocFighter(FTDATA_FLAG_SUBMOTION, sc1PIntroGetFighterAllocsNum(sSC1PIntroStage));
    sc1PIntroSetupFighterFiles(sSC1PIntroStage);

    for (i = 0; i < sc1PIntroGetFighterAllocsNum(sSC1PIntroStage); i++)
    {
        sSC1PIntroFigatreeHeaps[i] = syTaskmanMalloc(gFTManagerFigatreeHeapSize, 0x10);
    }
    sc1PIntroMakePicturesCamera();
    sc1PIntroMakeDecalsCamera();
    sc1PIntroMakeBannersCamera();
    sc1PIntroMakeSky();
    sc1PIntroMakeBanners();
    
    if (sc1PIntroCheckNotBonusStage(sSC1PIntroStage) != FALSE)
    {
        sc1PIntroMakeVSDecal();
    }
    else sc1PIntroMakeBonusPicture(sSC1PIntroStage);
    
    sc1PIntroMakeLabels(sSC1PIntroStage);
    sc1PIntroMakeFigures(sSC1PIntroStage);
    sc1PIntroMakeStageInfo(sSC1PIntroStage);
    
    if (sc1PIntroCheckNotBonusStage(sSC1PIntroStage) != FALSE)
    {
        sc1PIntroInitFighters(sSC1PIntroStage);
        sc1PIntroInitVSFighters(sSC1PIntroStage);
    }
    scSubsysFighterSetLightParams(-20.0F, 30.0F, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if (sSC1PIntroStage == nSC1PGameStageBoss)
    {
        syAudioPlayBGM(0, nSYAudioBGMBossStage);
    }
    else syAudioPlayBGM(0, nSYAudioBGM1PIntro);
    
    sySchedulerSetTicCount(0);
}

// 0x80135B38
SYVideoSetup dSC1PIntroVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80135B54
SYTaskmanSetup dSC1PIntroTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        scManagerFuncDraw,              // Frame draw function
        &ovl24_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 6250,         // Display List Buffer 0 Size
        sizeof(Gfx) * 128,          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x10000,                    // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        sc1PIntroFuncLights,    // Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    0,                              // Number of thread stacks
    0,                              // ???
    0,                              // Number of GObjProcesses
    0,                              // Number of GObjs
    sizeof(GObj),                   // GObj size
    0,                              // Number of XObjs
    dLBCommonFuncMatrixList,        // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    sc1PIntroFuncStart          // Task start function
};

// 0x80134D98
void sc1PIntroStartScene(void)
{
    dSC1PIntroVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dSC1PIntroVideoSetup);
    
    dSC1PIntroTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl24_BSS_END);
    scManagerFuncUpdate(&dSC1PIntroTaskmanSetup);
}
