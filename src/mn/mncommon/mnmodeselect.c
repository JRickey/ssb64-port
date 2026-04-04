#include <mn/menu.h>
#include <sc/scene.h>
#include <gm/gmsound.h>
#include <sys/controller.h>
#include <sys/video.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define mnModeSelectCheckGetOptionButtonInput(is_button, mask) \
mnCommonCheckGetOptionButtonInput(sMNModeSelectOptionChangeWait, is_button, mask)

#define mnModeSelectCheckGetOptionStickInputUD(stick_range, min, b) \
mnCommonCheckGetOptionStickInputUD(sMNModeSelectOptionChangeWait, stick_range, min, b)

#define mnModeSelectCheckGetOptionStickInputLR(stick_range, min, b) \
mnCommonCheckGetOptionStickInputLR(sMNModeSelectOptionChangeWait, stick_range, min, b)

#define mnModeSelectSetOptionChangeWaitP(is_button, stick_range, div) \
mnCommonSetOptionChangeWaitP(sMNModeSelectOptionChangeWait, is_button, stick_range, div)

#define mnModeSelectSetOptionChangeWaitN(is_button, stick_range, div) \
mnCommonSetOptionChangeWaitN(sMNModeSelectOptionChangeWait, is_button, stick_range, div)

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80132B90
u32 dMNModeSelectFileIDs[/* */] = { &llMNCommonFileID, &llMNMainFileID };

// 0x80133088
Lights1 dMNModeSelectLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C);

// 0x801330A0
Gfx dMNModeSelectDisplayList[/* */] =
{
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(dMNModeSelectLights1),
    gsSPEndDisplayList()
};

// 0x80132BD8
SYVideoSetup dMNModeSelectVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x80132BF4
SYTaskmanSetup dMNModeSelectTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                              // ???
        gcRunAll,                 	 	// Update function
        gcDrawAll,                      // Frame draw function
        &ovl17_BSS_END,                 // Allocatable memory pool start
        0,                              // Allocatable memory pool size
        1,                              // ???
        2,                              // Number of contexts?
        sizeof(Gfx) * 3750,             // Display List Buffer 0 Size
        0,                              // Display List Buffer 1 Size
        0,                              // Display List Buffer 2 Size
        0,                              // Display List Buffer 3 Size
        0x8000,                         // Graphics Heap Size
        2,                              // ???
        0xC000,                         // RDP Output Buffer Size
        mnModeSelectFuncLights,    	    // Pre-render function
        syControllerFuncRead,           // Controller I/O function
    },

    0,                                  // Number of GObjThreads
    sizeof(u64) * 192,                  // Thread stack size
    0,                                  // Number of thread stacks
    0,                                  // ???
    0,                                  // Number of GObjProcesses
    0,                                  // Number of GObjs
    sizeof(GObj),                       // GObj size
    0,                                  // Number of XObjs
    NULL,                               // Matrix function list
    NULL,                               // DObjVec eject function
    0,                                  // Number of AObjs
    0,                                  // Number of MObjs
    0,                                  // Number of DObjs
    sizeof(DObj),                       // DObj size
    0,                                  // Number of SObjs
    sizeof(SObj),                       // SObj size
    0,                                  // Number of CObjs
    sizeof(CObj),                       // Camera size
    
    mnModeSelectFuncStart          	    // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80132C80
s32 sMNModeSelectPad0x80132C80[2];

// 0x80132C88
s32 sMNModeSelectOption;

// 0x80132C8C
GObj *sMNModeSelectOption1PModeGObj;

// 0x80132C90
GObj *sMNModeSelectOptionVSModeGObj;

// 0x80132C94
GObj *sMNModeSelectOptionOptionGObj;

// 0x80132C98
GObj *sMNModeSelectOptionDataGObj;

// 0x80132C9C
s32 sMNModeSelectOptionChangeWait;

// 0x80132CA0
s32 sMNModeSelectTotalTimeTics;

// 0x80132CA4
s32 sMNModeSelectReturnTic;

// 0x80132CA8
LBFileNode sMNModeSelectStatusBuffer[24];

// 0x80132D68
void *sMNModeSelectFiles[ARRAY_COUNT(dMNModeSelectFileIDs)];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
void mnModeSelectFuncLights(Gfx **dls)
{
    gSPDisplayList(dls[0]++, dMNModeSelectDisplayList);
}

// 0x80131B24
void mnModeSelectMake1PMode(void)
{
    GObj *gobj;
    SObj *sobj;

    sMNModeSelectOption1PModeGObj = gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT); 
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    if (sMNModeSelectOption == nMNModeSelectOption1PMode)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainControllerIconSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 169.0F;
        sobj->pos.y = 27.0F;

#if defined(REGION_JP)
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMain1PModeTextJapSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 122.0F;
        sobj->pos.y = 195.0F;

        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[0], &llMNCommonFrameSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 93.0F;
        sobj->pos.y = 189.0F;
#endif
    }
    else
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainControllerIconDarkSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0x96;
        sobj->sprite.green = 0x96;
        sobj->sprite.blue = 0x96;
        
        sobj->pos.x = 169.0F;
        sobj->pos.y = 27.0F;
    }
}

// 0x80131C44
void mnModeSelectMakeVSMode(void)
{
    GObj *gobj;
    SObj *sobj;

    sMNModeSelectOptionVSModeGObj = gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT); 
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    if (sMNModeSelectOption == nMNModeSelectOptionVSMode)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainConsoleIconSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 128.0F;
        sobj->pos.y = 64.0F;

#if defined(REGION_JP)
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainVsModeTextJapSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 126.0F;
        sobj->pos.y = 195.0F;

        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[0], &llMNCommonFrameSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 93.0F;
        sobj->pos.y = 189.0F;
#endif
    }
    else
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainConsoleIconDarkSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0x96;
        sobj->sprite.green = 0x96;
        sobj->sprite.blue = 0x96;
        
        sobj->pos.x = 128.0F;
        sobj->pos.y = 64.0F;
    }
}

// 0x80131D68
void mnModeSelectMakeOption(void)
{
    GObj *gobj;
    SObj *sobj;

    sMNModeSelectOptionOptionGObj = gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT); 
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    if (sMNModeSelectOption == nMNModeSelectOptionOption)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainSettingsIconSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 87.0F;
        sobj->pos.y = 101.0F;

#if defined(REGION_JP)
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainOptionTextJapSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 112.0F;
        sobj->pos.y = 195.0F;

        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[0], &llMNCommonFrameSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 93.0F;
        sobj->pos.y = 189.0F;
#endif
    }
    else
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainSettingsIconDarkSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0x96;
        sobj->sprite.green = 0x96;
        sobj->sprite.blue = 0x96;
        
        sobj->pos.x = 87.0F;
        sobj->pos.y = 101.0F;
    }
}

// 0x80131E8C
void mnModeSelectMakeData(void)
{
    GObj *gobj;
    SObj *sobj;

    sMNModeSelectOptionDataGObj = gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT); 
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    if (sMNModeSelectOption == nMNModeSelectOptionData)
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainDataIconSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 46.0F;
        sobj->pos.y = 138.0F;

#if defined(REGION_JP)
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainDataTextJapSprite));

        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;

        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 126.0F;
        sobj->pos.y = 195.0F;

        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[0], &llMNCommonFrameSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0xFF;
        sobj->sprite.green = 0xFF;
        sobj->sprite.blue = 0xFF;
    
        sobj->envcolor.r = 0x00;
        sobj->envcolor.g = 0x00;
        sobj->envcolor.b = 0x00;
        
        sobj->pos.x = 93.0F;
        sobj->pos.y = 189.0F;
#endif
    }
    else
    {
        sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainDataIconDarkSprite));
        
        sobj->sprite.attr &= ~SP_FASTCOPY;
        sobj->sprite.attr |= SP_TRANSPARENT;
        
        sobj->sprite.red = 0x96;
        sobj->sprite.green = 0x96;
        sobj->sprite.blue = 0x96;
        
        sobj->pos.x = 46.0F;
        sobj->pos.y = 138.0F;
    }
}

// 0x80131FB0
void mnModeSelectMakeLabels(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 3, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 1, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMain1PModeTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    sobj->pos.x = 224.0F;
    sobj->pos.y = 52.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainVsModeTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    sobj->pos.x = 183.0F;
    sobj->pos.y = 89.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainOptionTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    sobj->pos.x = 142.0F;
    sobj->pos.y = 126.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainDataTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;

    sobj->sprite.red = 0xFF;
    sobj->sprite.green = 0x00;
    sobj->sprite.blue = 0x00;
    
    sobj->pos.x = 102.0F;
    sobj->pos.y = 163.0F;
}

// 0x80132168
void mnModeSelectMakeDecals(void)
{
    GObj *gobj;
    SObj *sobj;

    gobj = gcMakeGObjSPAfter(0, NULL, 2, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[0], &llMNCommonSmashBrosCollageSprite));
    
    sobj->pos.x = 10.0F;
    sobj->pos.y = 10.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainDecalBarMiddleSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0x08;
    sobj->sprite.green = 0x33;
    sobj->sprite.blue = 0x65;
    
    sobj->cms = 0;
    sobj->cmt = 0;
    
    sobj->masks = 4;
    sobj->maskt = 0;
    
    sobj->lrs = 96;
    sobj->lrt = 38;
    
    sobj->pos.x = 0.0f;
    sobj->pos.y = 37.0f;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainDecalBarEdgeSprite));

    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0x08;
    sobj->sprite.green = 0x33;
    sobj->sprite.blue = 0x65;
    
    sobj->pos.x = 96.0F;
    sobj->pos.y = 37.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainModeSelectTextSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->envcolor.r = 0x00;
    sobj->envcolor.g = 0x00;
    sobj->envcolor.b = 0x00;
    
    sobj->sprite.red = 0x3C;
    sobj->sprite.green = 0x73;
    sobj->sprite.blue = 0xB4;
    
    sobj->pos.x = 28.0F;
    sobj->pos.y = 27.0F;
    
    sobj = lbCommonMakeSObjForGObj(gobj, lbRelocGetFileData(Sprite*, sMNModeSelectFiles[1], &llMNMainSmashLogoSprite));
    
    sobj->sprite.attr &= ~SP_FASTCOPY;
    sobj->sprite.attr |= SP_TRANSPARENT;
    
    sobj->sprite.red = 0x08;
    sobj->sprite.green = 0x33;
    sobj->sprite.blue = 0x65;
    
    sobj->pos.x = 226.0F;
    sobj->pos.y = 137.0F;
}

// 0x80132398
void mnModeSelectMakeLabelsCamera(void)
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

// 0x80132438
void mnModeSelectMakeDecalsCamera(void)
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

// 0x801324D8
void mnModeSelectMakeOptions(void)
{
    mnModeSelectMake1PMode();
    mnModeSelectMakeVSMode();
    mnModeSelectMakeOption();
    mnModeSelectMakeData();
}

// 0x80132510
void mnModeSelectEjectOptions(void)
{
    gcEjectGObj(sMNModeSelectOption1PModeGObj);
    gcEjectGObj(sMNModeSelectOptionVSModeGObj);
    gcEjectGObj(sMNModeSelectOptionOptionGObj);
    gcEjectGObj(sMNModeSelectOptionDataGObj);
}

// 0x80132558
void mnModeSelectInitVars(void)
{
    switch (gSCManagerSceneData.scene_prev)
    {
    default:
        sMNModeSelectOption = nMNModeSelectOption1PMode;
        break;
        
    case nSCKind1PMode:
        sMNModeSelectOption = nMNModeSelectOption1PMode;
        break;
        
    case nSCKindVSMode:
        sMNModeSelectOption = nMNModeSelectOptionVSMode;
        break;
        
    case nSCKindOption:
        sMNModeSelectOption = nMNModeSelectOptionOption;
        break;
        
    case nSCKindData:
        sMNModeSelectOption = nMNModeSelectOptionData;
        break;
    }
    sMNModeSelectOptionChangeWait = 0;
    
    sMNModeSelectTotalTimeTics = 0;
    sMNModeSelectReturnTic = sMNModeSelectTotalTimeTics + I_MIN_TO_TICS(5);
}

// 0x801325E8
void mnModeSelectFuncRun(GObj *gobj)
{
    s32 unused1;
    s32 stick_range;
    s32 unused2;
    sb32 is_button;

    sMNModeSelectTotalTimeTics++;
    
    if (sMNModeSelectTotalTimeTics >= 10)
    {
        if (sMNModeSelectTotalTimeTics == sMNModeSelectReturnTic)
        {
            gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
            gSCManagerSceneData.scene_curr = nSCKindTitle;

            syTaskmanSetLoadScene();
            return;
        }
        if (scSubsysControllerCheckNoInputAll() == FALSE)
        {
            sMNModeSelectReturnTic = sMNModeSelectTotalTimeTics + I_MIN_TO_TICS(5);
        }
        if (sMNModeSelectOptionChangeWait != 0)
        {
            sMNModeSelectOptionChangeWait--;
        }
        if
        (
            (scSubsysControllerGetPlayerStickInRangeLR(-20, 20) != FALSE)                                &&
            (scSubsysControllerGetPlayerStickInRangeUD(-20, 20) != FALSE)                                &&
            (scSubsysControllerGetPlayerHoldButtons(U_JPAD | R_JPAD | U_CBUTTONS | R_CBUTTONS) == FALSE) &&
            (scSubsysControllerGetPlayerHoldButtons(D_JPAD | L_JPAD | D_CBUTTONS | L_CBUTTONS) == FALSE)
        )
        {
            sMNModeSelectOptionChangeWait = 0;
        }
        if (scSubsysControllerGetPlayerTapButtons(A_BUTTON | START_BUTTON) != FALSE)
        {
            switch (sMNModeSelectOption)
            {
            case nMNModeSelectOption1PMode:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                
                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKind1PMode;
                
                syTaskmanSetLoadScene();
                return;
                
            case nMNModeSelectOptionVSMode:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                
                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindVSMode;
                
                syTaskmanSetLoadScene();
                return;
                
            case nMNModeSelectOptionOption:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                
                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindOption;
                
                syTaskmanSetLoadScene();
                return;
                
            case nMNModeSelectOptionData:
                func_800269C0_275C0(nSYAudioFGMMenuSelect);
                
                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindData;
                
                syTaskmanSetLoadScene();
                return;
            }
        }
        else
        {
            if (scSubsysControllerGetPlayerTapButtons(B_BUTTON) != FALSE)
            {
                syAudioStopBGMAll();

                gSCManagerSceneData.scene_prev = gSCManagerSceneData.scene_curr;
                gSCManagerSceneData.scene_curr = nSCKindTitle;
            
                syTaskmanSetLoadScene();
            }
            if
            (
                (
                    (scSubsysControllerGetPlayerStickUD(20, 1) <= 0) ||
                    (scSubsysControllerGetPlayerStickLR(-20, 0) >= 0)
                )
                &&
                (
                    (scSubsysControllerGetPlayerStickUD(-20, 0) >= 0) ||
                    (scSubsysControllerGetPlayerStickLR(20, 1) <= 0)
                )
            )
            {
                if
                (
                    mnModeSelectCheckGetOptionButtonInput(is_button, U_JPAD | R_JPAD | U_CBUTTONS | R_CBUTTONS) ||
                    (sMNModeSelectOptionChangeWait == 0)
                    &&
                    (
                        ((stick_range) = scSubsysControllerGetPlayerStickUD(20, 1), stick_range != 0) ||
                        ((stick_range) = scSubsysControllerGetPlayerStickLR(20, 1), stick_range != 0)
                    )
                )
                {    
                    func_800269C0_275C0(nSYAudioFGMMenuScroll2);
                    
                    mnModeSelectSetOptionChangeWaitP(is_button, stick_range, 7);
                        
                    if (sMNModeSelectOption == nMNModeSelectOptionStart)
                    {
                        sMNModeSelectOption = nMNModeSelectOptionEnd;
                    }
                    else sMNModeSelectOption--;
                    
                    if (sMNModeSelectOption == nMNModeSelectOptionStart)
                    {
                        sMNModeSelectOptionChangeWait += 8;
                    }
                    mnModeSelectEjectOptions();
                    mnModeSelectMakeOptions();
                }
                if
                (
                    mnModeSelectCheckGetOptionButtonInput(is_button, D_JPAD | L_JPAD | D_CBUTTONS | L_CBUTTONS) ||
                    (sMNModeSelectOptionChangeWait == 0)
                    &&
                    (
                        ((stick_range) = scSubsysControllerGetPlayerStickUD(-20, 0), stick_range != 0) ||
                        ((stick_range) = scSubsysControllerGetPlayerStickLR(-20, 0), stick_range != 0)
                    )
                )
                {    
                    func_800269C0_275C0(nSYAudioFGMMenuScroll2);
                    
                    mnModeSelectSetOptionChangeWaitN(is_button, stick_range, 7);
                        
                    if (sMNModeSelectOption == nMNModeSelectOptionEnd)
                    {
                        sMNModeSelectOption = nMNModeSelectOptionStart;
                    }
                    else sMNModeSelectOption++;
                    
                    if (sMNModeSelectOption == nMNModeSelectOptionEnd)
                    {
                        sMNModeSelectOptionChangeWait += 8;
                    }
                    mnModeSelectEjectOptions();
                    mnModeSelectMakeOptions();
                }
            }
        }
    }
}

// 0x80132A0C
void mnModeSelectFuncStart(void)
{
    LBRelocSetup rl_setup;

    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;
    rl_setup.table_files_num = (u32)&llRelocFileCount;
    rl_setup.file_heap = NULL;
    rl_setup.file_heap_size = 0;
    rl_setup.status_buffer = sMNModeSelectStatusBuffer;
    rl_setup.status_buffer_size = ARRAY_COUNT(sMNModeSelectStatusBuffer);
    rl_setup.force_status_buffer = NULL;
    rl_setup.force_status_buffer_size = 0;
    
    lbRelocInitSetup(&rl_setup);
    lbRelocLoadFilesListed(dMNModeSelectFileIDs, sMNModeSelectFiles);
    gcMakeGObjSPAfter(0, mnModeSelectFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
    gcMakeDefaultCameraGObj(0, GOBJ_PRIORITY_DEFAULT, 100, 0, GPACK_RGBA8888(0x00, 0x00, 0x00, 0x00));
    
    mnModeSelectInitVars();
    mnModeSelectMakeDecalsCamera();
    mnModeSelectMakeLabelsCamera();
    mnModeSelectMakeDecals();
    mnModeSelectMakeOptions();
    mnModeSelectMakeLabels();
    
    if
    (
        (gSCManagerSceneData.scene_prev != nSCKind1PMode) &&
        (gSCManagerSceneData.scene_prev != nSCKindVSMode) &&
        (gSCManagerSceneData.scene_prev != nSCKindOption) &&
        (gSCManagerSceneData.scene_prev != nSCKindData)
    )
    {
        syAudioPlayBGM(0, nSYAudioBGMModeSelect);
    }
}

// 0x80132B34
void mnModeSelectStartScene(void)
{
    dMNModeSelectVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
    syVideoInit(&dMNModeSelectVideoSetup);
    
    dMNModeSelectTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&ovl1_VRAM - (uintptr_t)&ovl17_BSS_END);
    syTaskmanStartTask(&dMNModeSelectTaskmanSetup);
}
