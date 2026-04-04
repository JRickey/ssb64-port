#include <mn/menu.h>
#include <gm/gmsound.h>
#include <sc/scene.h>
#include <lb/library.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <sys/controller.h>
#include <reloc_data.h>

extern void syAudioStopBGMAll();
extern void* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define mnDataCheckGetOptionButtonInput(is_button, mask) \
mnCommonCheckGetOptionButtonInput(sMNDataOptionChangeWait, is_button, mask)

#define mnDataCheckGetOptionStickInputUD(stick_range, min, b) \
mnCommonCheckGetOptionStickInputUD(sMNDataOptionChangeWait, stick_range, min, b)

#define mnDataCheckGetOptionStickInputLR(stick_range, min, b) \
mnCommonCheckGetOptionStickInputLR(sMNDataOptionChangeWait, stick_range, min, b)

#define mnDataSetOptionChangeWaitP(is_button, stick_range, div) \
mnCommonSetOptionChangeWaitP(sMNDataOptionChangeWait, is_button, stick_range, div)

#define mnDataSetOptionChangeWaitN(is_button, stick_range, div) \
mnCommonSetOptionChangeWaitN(sMNDataOptionChangeWait, is_button, stick_range, div)

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132F20
u32 dMNDataFileIDs[/* */] = { &llMNCommonFileID, &llMNDataFileID };

// 0x80132F28
Lights1 dMNDataLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x80132F40
Gfx dMNDataDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dMNDataLights1),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80133060
GObj *sMNDataOptionCharactersGObj;

// 0x80133064
GObj *sMNDataOptionVSRecordGObj;

// 0x80133068
GObj *sMNDataOptionSoundTestGObj;

// 0x80133070
s32 sMNDataPad0x80133070[2];

// 0x80133078
s32 sMNDataOption;

// 0x8013307C
GObj *sMNDataMenuGObj;

// 0x80133080
s32 sMNDataFirstAvailableOption;

// 0x80133084
s32 sMNDataLastAvailableOption;

// 0x80133088
sb32 sMNDataIsHaveSoundTest;

// 0x8013308C
sb32 sMNDataIsProceedScene;

// 0x80133090
s32 sMNDataOptionChangeWait;

// 0x80133094
s32 sMNDataTotalTimeTics;

// 0x80133098
s32 sMNDataReturnTic;

// 0x8013309C - Padding?
// s32 sMNDataPad0x8013309C;

// 0x801330A0
LBFileNode sMNDataStatusBuffer[24];

// 0x80133160
void *sMNDataFiles[ARRAY_COUNT(dMNDataFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnDataFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dMNDataDisplayList);
}

// 0x80131B24
sb32 mnDataCheckSoundTestUnlocked(void)
{
    if (gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_SOUNDTEST)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80131B4C
void mnDataSetOptionSpriteColors(GObj *gobj, s32 status)
{
    // 0x80132F68
    SYColorRGBPair selcolors = { { 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF } };

    // 0x80132F70
    SYColorRGBPair hicolors  = { { 0x82, 0x00, 0x28 }, { 0xFF, 0x00, 0x28 } };

    // 0x80132F78
    SYColorRGBPair notcolors = { { 0x00, 0x00, 0x00 }, { 0x82, 0x82, 0xAA } };
    
    SYColorRGBPair *colors;
    SObj *sobj;
    s32 i;

    switch (status)
    {
    case nMNOptionTabStatusHighlight:
        colors = &hicolors;
        break;

    case nMNOptionTabStatusNot:
        colors = &notcolors;
        break;

    case nMNOptionTabStatusSelected:
        colors = &selcolors;
        break;

    default:
        break; // WARNING: Undefined behavior. This will assign sp 0xC to colors which is uninitialized.
    }
    sobj = SObjGetStruct(gobj);

    for (i = 0; i < 3; i++)
    {
        sobj->envcolor.r = colors->prim.r;
        sobj->envcolor.g = colors->prim.g;
        sobj->envcolor.b = colors->prim.b;

        sobj->sprite.red   = colors->env.r;
        sobj->sprite.green = colors->env.g;
        sobj->sprite.blue  = colors->env.b;

        sobj = sobj->next;
    }
}

// 0x80131C24
void mnDataMakeOptionTab(GObj *gobj, f32 pos_x, f32 pos_y, s32 lrs)
{
    SObj *sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonOptionTabLeftSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x;
    sobj->pos.y = pos_y;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonOptionTabMiddleSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x + 16.0F;
    sobj->pos.y = pos_y;

    sobj->cms = 0;
    sobj->cmt = 0;

    sobj->masks = 4;
    sobj->maskt = 0;

    sobj->lrs = lrs * 8;
    sobj->lrt = 29;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonOptionTabRightSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x + 16.0F + (lrs * 8);
    sobj->pos.y = pos_y;
}

// 0x80131D54
void func_ovl61_80131D54(void)
{
    return;
}

// 0x80131D5C
void mnDataMakeCharacters(void)
{
    GObj *gobj;
    SObj *sobj;
    s32 pos_x;
    s32 pos_y;

    if (sMNDataIsHaveSoundTest != FALSE)
    {
        pos_x = 133;
        pos_y = 42;
    }
    else
    {
        pos_x = 113;
        pos_y = 57;
    }

    sMNDataOptionCharactersGObj = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);

    mnDataMakeOptionTab(gobj, pos_x, pos_y, 16);

    mnDataSetOptionSpriteColors(gobj, sMNDataOption == nMNDataOptionCharacters);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[1], &llMNDataCharactersTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x + 26;
    sobj->pos.y = pos_y + 4;

    sobj->sprite.red   = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue  = 0x00;
}

// 0x80131E90
void mnDataMakeVSRecord(void)
{
    GObj *gobj;
    SObj *sobj;
    s32 pos_x;
    s32 pos_y;

    if (sMNDataIsHaveSoundTest != FALSE)
    {
        pos_x = 101;
        pos_y = 89;
    }
    else
    {
        pos_x = 81;
        pos_y = 126;
    }

    sMNDataOptionVSRecordGObj = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);

    mnDataMakeOptionTab(gobj, pos_x, pos_y, 16);

    mnDataSetOptionSpriteColors(gobj, sMNDataOption == nMNDataOptionVSRecord);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[1], &llMNDataVSRecordTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->pos.x = pos_x + 27;
    sobj->pos.y = pos_y + 4;

    sobj->sprite.red   = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue  = 0x00;
}

// 0x80131FC8
void mnDataMakeSoundTest(void)
{
    GObj *gobj;
    SObj *sobj;

    sMNDataOptionSoundTestGObj = gobj = gcMakeGObjSPAfter(0, NULL, 4, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 2, GOBJ_PRIORITY_DEFAULT, ~0);

    mnDataMakeOptionTab(gobj, 69.0F, 136.0F, 16);

    mnDataSetOptionSpriteColors(gobj, sMNDataOption == nMNDataOptionSoundTest);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[1], &llMNDataSoundTestTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red   = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue  = 0x00;

    sobj->pos.x = 95.0F;
    sobj->pos.y = 140.0F;
}

// 0x801320A0 - Unused?
void mnDataSetSubtitleSpriteColors(SObj *sobj)
{
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->envcolor.r = 0x00;
    sobj->envcolor.g = 0x00;
    sobj->envcolor.b = 0x00;

    sobj->sprite.red   = 0xFF;
    sobj->sprite.green = 0xFF;
    sobj->sprite.blue  = 0xFF;
}

// 0x801320D4
void mnDataMakeMenuGObj(void)
{
    GObj *gobj;
    SObj *sobj;

    // 0x80132F80 - Japanese descriptions
    intptr_t offsets[/* */] = { &llMNDataCharactersTextJapSprite, &llMNDataVSRecordTextJapSprite, &llMNDataSoundTestTextJapSprite };

    // 0x8013F28C
    Vec2f positions[/* */] =
    {
        { 105.0F, 194.0F },
        { 131.0F, 194.0F },
        { 113.0F, 194.0F }
    };

    sMNDataMenuGObj = gobj =  gcMakeGObjSPAfter(0, NULL, 5, GOBJ_PRIORITY_DEFAULT);

#if defined(REGION_JP)
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 3, GOBJ_PRIORITY_DEFAULT, ~0);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonFrameSprite));

    sobj->pos.x = 93.0F;
    sobj->pos.y = 189.0F;
    
    mnDataSetSubtitleSpriteColors(sobj);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[1], offsets[sMNDataOption]));

    sobj->pos.x = positions[sMNDataOption].x;
    sobj->pos.y = positions[sMNDataOption].y;
    
    mnDataSetSubtitleSpriteColors(sobj);
#endif
}

// 0x80132164
void mnDataLabelsProcDisplay(GObj *gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xA0, 0x78, 0x14, 0xE6);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 225, 143, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);

    lbCommonClearExternSpriteParams();
    lbCommonDrawSObjAttr(gobj);
}

// 0x801322A87
void mnDataMakeLabels(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(gobj, mnDataLabelsProcDisplay, 1, GOBJ_PRIORITY_DEFAULT, ~0);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonSmashLogoSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;

    sobj->pos.x = 235.0F;
    sobj->pos.y = 158.0F;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[1], &llMNDataDataTextSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red   = 0x00;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue  = 0x00;

    sobj->pos.x = 206.0F;
    sobj->pos.y = 131.0F;
}

// 0x801323A0
void mnDataMakeDecals(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 2, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonSmashBrosCollageSprite));

    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonDecalPaperSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red   = 0xA0;
    sobj->sprite.green = 0x78;
    sobj->sprite.blue  = 0x14;

    sobj->pos.x = 140.0F;
    sobj->pos.y = 143.0F;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[0], &llMNCommonDecalPaperSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red   = 0xA0;
    sobj->sprite.green = 0x78;
    sobj->sprite.blue  = 0x14;

    sobj->pos.x = 225.0F;
    sobj->pos.y = 56.0F;

    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNDataFiles[1], &llMNDataDataIconDarkSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red   = 0x99;
    sobj->sprite.green = 0x99;
    sobj->sprite.blue  = 0x99;

    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
}

// 0x80132534
void mnDataMakeLink3Camera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            20,
            COBJ_MASK_DLLINK(3),
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

// 0x801325D4
void mnDataMakeOptionsCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            40,
            COBJ_MASK_DLLINK(2),
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

// 0x80132674
void mnDataMakeLabelsCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            60,
            COBJ_MASK_DLLINK(1),
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

// 0x80132714
void mnDataMakeDecalsCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            1,
            NULL,
            1,
            GOBJ_PRIORITY_DEFAULT,
            lbCommonDrawSprite,
            80,
            COBJ_MASK_DLLINK(0),
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

// 0x801327B4
void mnDataInitVars(void)
{
    switch (gSCManagerSceneData.scene_prev)
    {
    case nSCKindVSRecord:
        sMNDataOption = nMNDataOptionVSRecord;
        break;

    case nSCKindSoundTest:
        sMNDataOption = nMNDataOptionSoundTest;
        break;

    default:
        sMNDataOption = nMNDataOptionCharacters;
        break;
    }
    sMNDataFirstAvailableOption = nMNDataOptionCharacters;

    if (mnDataCheckSoundTestUnlocked() != FALSE)
    {
        sMNDataLastAvailableOption = nMNDataOptionSoundTest;
        sMNDataIsHaveSoundTest = TRUE;
    }
    else
    {
        sMNDataLastAvailableOption = nMNDataOptionVSRecord;
        sMNDataIsHaveSoundTest = FALSE;
    }
    sMNDataOptionChangeWait = 0;
    sMNDataTotalTimeTics = 0;
    sMNDataIsProceedScene = FALSE;
    sMNDataReturnTic = sMNDataTotalTimeTics + I_MIN_TO_TICS(5);
}

// 0x80132874
void mnDataFuncRun(GObj *gobj)
{
    GObj *select_gobj;
    s32 stick_range;

    // 0x8013F2A4
    GObj **option_gobjs[/* */] = { &sMNDataOptionCharactersGObj, &sMNDataOptionVSRecordGObj, &sMNDataOptionSoundTestGObj };

    sb32 is_button;

    sMNDataTotalTimeTics++;

    if (sMNDataTotalTimeTics >= 10)
    {
        if (sMNDataTotalTimeTics == sMNDataReturnTic)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();

            return;
        }
        if (scSubsysControllerCheckNoInputAll() == FALSE)
        {
            sMNDataReturnTic = sMNDataTotalTimeTics + I_MIN_TO_TICS(5);
        }
        if (sMNDataIsProceedScene != FALSE)
        {
            syTaskmanSetLoadScene();
#if defined(REGION_US)
            return;
#endif
        }
        if (sMNDataOptionChangeWait != 0)
        {
            sMNDataOptionChangeWait--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-20, 20) != FALSE) &&
            (scSubsysControllerGetPlayerStickInRangeUD(-20, 20) != FALSE) &&
            (scSubsysControllerGetPlayerHoldButtons(U_JPAD | U_CBUTTONS) == FALSE) &&
            (scSubsysControllerGetPlayerHoldButtons(D_JPAD | D_CBUTTONS) == FALSE)
        )
        {
            sMNDataOptionChangeWait = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | START_BUTTON) != FALSE)
        {
            switch (sMNDataOption)
            {
            case nMNDataOptionCharacters:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusSelected);
                syAudioStopBGMAll();

                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindCharacters;
                sMNDataIsProceedScene = TRUE;
                return;

            case nMNDataOptionVSRecord:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusSelected);
                syAudioStopBGMAll();

                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindVSRecord;
                sMNDataIsProceedScene = TRUE;
                return;

            case nMNDataOptionSoundTest:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusSelected);
                syAudioStopBGMAll();

                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindSoundTest;
                sMNDataIsProceedScene = TRUE;
                return;
            }
        }
        if (scSubsysControllerGetPlayerTapButtons(B_BUTTON) != FALSE)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindModeSelect;

            syTaskmanSetLoadScene();
#if defined(REGION_US)
            return;
#endif
        }
        if
        (
            mnDataCheckGetOptionButtonInput(is_button, U_JPAD | U_CBUTTONS) ||
            mnDataCheckGetOptionStickInputUD(stick_range, 20, 1)
        )
        {
            func_800269C0_275C0(nSYAudioFGMMenuScroll2);

            mnDataSetOptionChangeWaitP(is_button, stick_range, 7);

            mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusNot);

            if (sMNDataOption == sMNDataFirstAvailableOption)
            {
                sMNDataOption = sMNDataLastAvailableOption;
            }
            else sMNDataOption--;

            mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusHighlight);

            if (sMNDataOption == sMNDataFirstAvailableOption)
            {
                sMNDataOptionChangeWait += 8;
            }
            gcEjectGObj(sMNDataMenuGObj);

            mnDataMakeMenuGObj();
        }
        if
        (
            mnDataCheckGetOptionButtonInput(is_button, D_JPAD | D_CBUTTONS) ||
            mnDataCheckGetOptionStickInputUD(stick_range, -20, 0)
        )
        {
            func_800269C0_275C0(nSYAudioFGMMenuScroll2);

            mnDataSetOptionChangeWaitN(is_button, stick_range, 7);

            mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusNot);

            if (sMNDataOption == sMNDataLastAvailableOption)
            {
                sMNDataOption = sMNDataFirstAvailableOption;
            }
            else sMNDataOption++;

            mnDataSetOptionSpriteColors(*option_gobjs[sMNDataOption], nMNOptionTabStatusHighlight);

            if (sMNDataOption == sMNDataLastAvailableOption)
            {
                sMNDataOptionChangeWait += 8;
            }
            gcEjectGObj(sMNDataMenuGObj);

            mnDataMakeMenuGObj();
        }
    }
}

// 0x80132D64
void mnDataFuncStart(void)
{
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMNDataStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMNDataStatusBuffer);
    rl_setup.force_status_buffer = NULL;
    rl_setup.force_status_buffer_size = 0;

    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMNDataFileIDs, sMNDataFiles);
    gcMakeGObjSPAfter(0, mnDataFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, 0, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));

    mnDataInitVars();
    mnDataMakeDecalsCamera();
    mnDataMakeLabelsCamera();
    mnDataMakeOptionsCamera();
    mnDataMakeLink3Camera();
    mnDataMakeDecals();
    mnDataMakeLabels();
    mnDataMakeCharacters();
    mnDataMakeVSRecord();

    if (sMNDataIsHaveSoundTest != FALSE)
    {
        mnDataMakeSoundTest();
    }
    mnDataMakeMenuGObj();

    if
    (
        (gSCManagerSceneData.scene_prev == nSCKindVSRecord)  || 
        (gSCManagerSceneData.scene_prev == nSCKindCharacters)||
        (gSCManagerSceneData.scene_prev == nSCKindSoundTest)
    )
    {
        syAudioPlayBGM(0, nSYAudioBGMModeSelect);
    }
}

// 0x80132FB0
SYVideoSetup dMNDataVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132FCC
SYTaskmanSetup dMNDataTaskmanSetup = 
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        gcRunAll,              		// Update function
        gcDrawAll,                  // Frame draw function
        &ovl61_BSS_END,             // Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 7500,         // Display List Buffer 0 Size
        0,                          // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0x8000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        mnDataFuncLights,           // Pre-render function
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
    NULL,                           // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    mnDataFuncStart                 // Task start function
};

// 0x80132EC0
void mnDataStartScene(void)
{
    dMNDataVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMNDataVideoSetup);

    dMNDataTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl61_BSS_END);
    syTaskmanStartTask(&dMNDataTaskmanSetup);
}
