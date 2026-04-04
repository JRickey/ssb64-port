#include <ft/fighter.h>
#include <gr/ground.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/rdp.h>
#include <reloc_data.h>


// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80192BC0 - Red color values of shooting stars on Final Destination
u8 dSC1PGameBossCometEnvColorR[/* */] = { 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x78 };

// 0x80192BC8 - Green color values of shooting stars on Final Destination
u8 dSC1PGameBossCometEnvColorG[/* */] = { 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x78 };

// 0x80192BD0 - Blue color values of shooting stars on Final Destination
u8 dSC1PGameBossCometEnvColorB[/* */] = { 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x78 };

// 0x80192BD8
SC1PGameBossEffect dSC1PGameBossEffects0[/* */] =
{
    // Effect 0
    {
        SC1PGameBossWallpaper0ProcUpdate,       // Proc Update
        SC1PGameBossWallpaper0ProcDisplay,       // Proc Render
        &llGRLastMapEffects0DObjDesc,         // DObjDesc offset
        &llGRLastMapEffects0MObjSub           // MObjSub offset
    }
};

// 0x80192BE8
SC1PGameBossAnim dSC1PGameBossAnims0[/* */] =
{
    // Anim 0
    {
        &llGRLastMapAnims0AnimJoint,          // AnimJoint offset
        &llGRLastMapAnims0MatAnimJoint,       // MatAnimJoint offset
        0.5F                                    // Animation speed
    },

    // Anim 1
    {
        &llGRLastMapAnims0AnimJoint,          // AnimJoint offset
        &llGRLastMapAnims0MatAnimJoint,       // MatAnimJoint offset
        1.0F                                    // Animation speed
    },

    // Anim 2
    {
        &llGRLastMapAnims0AnimJoint,          // AnimJoint offset
        &llGRLastMapAnims0MatAnimJoint,       // MatAnimJoint offset
        0.7F                                    // Animation speed
    },

    // Anim 3
    {
        &llGRLastMapAnims0AnimJoint,          // AnimJoint offset
        &llGRLastMapAnims0MatAnimJoint,       // MatAnimJoint offset
        0.25F                                   // Animation speed
    },
};

// 0x80192C18
SC1PGameBossPlan dSC1PGameBossPlans0[/* */] =
{
    // Plan 0
    {
        10,                                     // ???
        5,                                      // DL Link
        1,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            0.0F,                               // Y-Axis
            -2000.0F                            // Z-Axis
        }
    },

    // Plan 1
    {
        14,                                     // ???
        5,                                      // DL Link
        2,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            0.0F,                               // Y-Axis
            -3000.0F                            // Z-Axis
        }
    }
};

// 0x80192C48
SC1PGameBossEffect dSC1PGameBossEffects1[/* */] =
{
    // Effect 0
    {
        SC1PGameBossWallpaper1ProcUpdate,       // Proc Update
        SC1PGameBossWallpaper1ProcDisplay,       // Proc Render
        &llGRLastMapEffects1DObjDesc,         // DObjDesc offset
        &llGRLastMapEffects1MObjSub           // MObjSub offset
    }
};

// 0x80192C58
SC1PGameBossAnim dSC1PGameBossAnims1[/* */] =
{
    // Anim 0
    {
        &llGRLastMapAnims1AnimJoint,          // AnimJoint offset
        &llGRLastMapAnims1MatAnimJoint,       // MatAnimJoint offset
        0.5F                                    // Animation speed
    }
};

// 0x80192C64
SC1PGameBossPlan dSC1PGameBossPlans1[/* */] =
{
    // Plan 0
    {
        1,                                      // ???
        5,                                      // DL Link
        2,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            4800.0F,                            // Y-Axis
            -10000.0F                           // Z-Axis
        }
    }
};

// 0x80192C7C
SC1PGameBossEffect dSC1PGameBossEffects2[/* */] =
{
    // Effect 0
    {
        SC1PGameBossWallpaper2ProcUpdate0,      // Proc Update
        SC1PGameBossWallpaper2ProcDisplay,       // Proc Render
        &llGRLastMapEffects2_0DObjDesc,        // DObjDesc offset
        &llGRLastMapEffects2_0MObjSub          // MObjSub offset
    },

    // Effect 1
    {
        SC1PGameBossWallpaper2ProcUpdate1,      // Proc Update
        SC1PGameBossWallpaper3ProcDisplay0,       // Proc Render
        &llGRLastMapEffects2_1DObjDesc,        // DObjDesc offset
        &llGRLastMapEffects2_1MObjSub          // MObjSub offset
    }
};

// 0x80192C9C
SC1PGameBossAnim dSC1PGameBossAnims2[/* */] =
{
    // Anim 0
    {
        &llGRLastMapAnims2_0AnimJoint,         // AnimJoint offset
        &llGRLastMapAnims2_0MatAnimJoint,      // MatAnimJoint offset
        1.0F                                    // Animation speed
    },

    // Anim 1
    {
        0x0,                                    // AnimJoint offset
        &llGRLastMapAnims2_1MatAnimJoint,      // MatAnimJoint offset
        1.0F                                    // Animation speed
    }
};

// 0x801292CB4
SC1PGameBossPlan dSC1PGameBossPlans2[/* */] =
{
    // Plan 0
    {
        1,                                      // ???
        5,                                      // DL Link
        2,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            0.0F,                               // Y-Axis
            2000.0F                             // Z-Axis
        }
    },

    // Plan 1
    {
        1,                                      // ???
        5,                                      // DL Link
        2,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            0.0F,                               // Y-Axis
            1000.0F                             // Z-Axis
        }
    }
};

// 0x80192CE4
SC1PGameBossEffect dSC1PGameBossEffects3[/* */] =
{
    // Effect 0
    {
        SC1PGameBossWallpaper3ProcUpdate0,      // Proc Update
        SC1PGameBossWallpaper3ProcDisplay0,      // Proc Render
        &llGRLastMapEffects3_0DObjDesc,        // DObjDesc offset
        &llGRLastMapEffects3_0MObjSub          // MObjSub offset
    },

    // Effect 1
    {
        SC1PGameBossWallpaper3ProcUpdate1,      // Proc Update
        SC1PGameBossWallpaper2ProcDisplay,       // Proc Render
        &llGRLastMapEffects3_1DObjDesc,        // DObjDesc offset
        0x0                                     // MObjSub offset
    }
};

// 0x80192D04
SC1PGameBossAnim dSC1PGameBossAnims3[/* */] =
{
    // Anim 0
    {
        0x0,                                    // AnimJoint offset
        &llGRLastMapAnims3_0MatAnimJoint,      // MatAnimJoint offset
        0.5F                                    // Animation speed
    },

    // Anim 1
    {
        &llGRLastMapAnims3_1AnimJoint,         // AnimJoint offset
        0x0,                                    // MatAnimJoint offset
        1.0F                                    // Animation speed
    }
};

// 0x80192D1C
SC1PGameBossPlan dSC1PGameBossPlans3[/* */] =
{
    // Plan 0
    {
        1,                                      // ???
        5,                                      // DL Link
        2,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            0.0F,                               // Y-Axis
            1000.0F                             // Z-Axis
        }
    },

    // Plan 1
    {
        1,                                      // ???
        5,                                      // DL Link
        1,                                      // ???

        // Translation vector
        {
            0.0F,                               // X-Axis
            0.0F,                               // Y-Axis
            -2000.0F                            // Z-Axis
        }
    }
};

// 0x80192D4C
SC1PGameBossWallpaper dSC1PGameBossWallpapers[/* */] =
{
    // Space background?
    {
        24,                                     // Loop count?
        ARRAY_COUNT(dSC1PGameBossEffects0),     // Total effect count?
        ARRAY_COUNT(dSC1PGameBossAnims0),       // Total animation count?
        ARRAY_COUNT(dSC1PGameBossPlans0),       // Total plan count?
        1,                                      // ???
        1,                                      // Color ID
        -1,                                     // Background change wait default
        90,                                     // Minimum damage for background change
        TRUE,                                   // Use random effects and animations?
        dSC1PGameBossEffects0,                  // Effect descriptor(s)
        dSC1PGameBossAnims0,                    // Animation descriptor(s)
        dSC1PGameBossPlans0                     // Drawing/rendering descriptor(s)
    },

    // ???
    {
        1,                                      // Loop count?
        ARRAY_COUNT(dSC1PGameBossEffects1),     // Total effect count?
        ARRAY_COUNT(dSC1PGameBossAnims1),       // Total animation count?
        ARRAY_COUNT(dSC1PGameBossPlans1),       // Total plan count?
        1,                                      // ???
        1,                                      // Color ID
        -1,                                     // Background change wait default
        180,                                    // Minimum damage for background change
        FALSE,                                  // Use random effects and animations?
        dSC1PGameBossEffects1,                  // Effect descriptor(s)
        dSC1PGameBossAnims1,                    // Animation descriptor(s)
        dSC1PGameBossPlans1                     // Drawing/rendering descriptor(s)
    },

    // ???
    {
        2,                                      // Loop count?
        ARRAY_COUNT(dSC1PGameBossEffects2),     // Total effect count?
        ARRAY_COUNT(dSC1PGameBossAnims2),       // Total animation count?
        ARRAY_COUNT(dSC1PGameBossPlans2),       // Total plan count?
        1,                                      // ???
        3,                                      // Color ID
        -1,                                     // Background change wait default
        -1,                                     // Minimum damage for background change
        FALSE,                                  // Use random effects and animations?
        dSC1PGameBossEffects2,                  // Effect descriptor(s)
        dSC1PGameBossAnims2,                    // Animation descriptor(s)
        dSC1PGameBossPlans2                     // Drawing/rendering descriptor(s)
    },

    // ???
    {
        2,                                      // Loop count?
        ARRAY_COUNT(dSC1PGameBossEffects3),     // Total effect count?
        ARRAY_COUNT(dSC1PGameBossAnims3),       // Total animation count?
        ARRAY_COUNT(dSC1PGameBossPlans3),       // Total plan count?
        5,                                      // ???
        1,                                      // Color ID
        -1,                                     // Background change wait default
        -1,                                     // Minimum damage for background change
        FALSE,                                  // Use random effects and animations?
        dSC1PGameBossEffects3,                  // Effect descriptor(s)
        dSC1PGameBossAnims3,                    // Animation descriptor(s)
        dSC1PGameBossPlans3                     // Drawing/rendering descriptor(s)
    }
};
 
// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801938D0
GObj *sSC1PGameBossWallpaperGObj;

// 0x801938D4 - Unused?
s32 sSC1PGamePad0x801938D4;

// 0x801938D8
SC1PGameBossMain sSC1PGameBossMain;

// 0x801938F0
f32 sSC1PGameBossWallpaperStepRGBA;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801910B0
void func_ovl65_801910B0(void)
{
    GObj *gobj;

    gcResumeGObjProcessAll(sSC1PGameBossWallpaperGObj);

    gobj = gGCCommonLinks[nGCCommonLinkIDWallpaper];

    while (gobj != NULL)
    {
        if (gobj->id == nGCCommonKindBossWallpaper)
        {
            gcResumeGObjProcessAll(gobj);
        }
        gobj = gobj->link_next;
    }
}

// 0x80191114
void sc1PGameBossSetChangeWallpaper(void)
{
    sSC1PGameBossMain.is_skip_wallpaper_change = FALSE;
}

// 0x80191120
void sc1PGameBossMakeCamera(void)
{
    CObj *cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindUnkCamera3, 
            NULL,
            nGCCommonLinkIDCamera,
            GOBJ_PRIORITY_DEFAULT,
            func_80017EC0,
            40,
            COBJ_MASK_DLLINK(5),
            1,
            TRUE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, gGMCameraStruct.viewport_ulx, gGMCameraStruct.viewport_uly, gGMCameraStruct.viewport_lrx, gGMCameraStruct.viewport_lry);

    cobj->projection.persp.aspect = (f32)(gGMCameraStruct.viewport_lrx - gGMCameraStruct.viewport_ulx) / (f32)(gGMCameraStruct.viewport_lry - gGMCameraStruct.viewport_uly);

    cobj->flags |= COBJ_FLAG_DLBUFFERS;

    cobj->vec.at.x = cobj->vec.at.y = cobj->vec.at.z = 0.0F;
    cobj->vec.eye.x = cobj->vec.eye.y = 0.0F;
    cobj->vec.eye.z = 2000.0F;

    cobj = CObjGetStruct
    (
        gcMakeCameraGObj
        (
            nGCCommonKindUnkCamera3,
            NULL,
            nGCCommonLinkIDCamera,
            GOBJ_PRIORITY_DEFAULT,
            func_80017EC0,
            60,
            COBJ_MASK_DLLINK(5),
            2,
            TRUE,
            nGCProcessKindFunc,
            NULL,
            1,
            FALSE
        )
    );
    syRdpSetViewport(&cobj->viewport, gGMCameraStruct.viewport_ulx, gGMCameraStruct.viewport_uly, gGMCameraStruct.viewport_lrx, gGMCameraStruct.viewport_lry);

    cobj->projection.persp.aspect = (f32)(gGMCameraStruct.viewport_lrx - gGMCameraStruct.viewport_ulx) / (f32)(gGMCameraStruct.viewport_lry - gGMCameraStruct.viewport_uly);

    cobj->flags |= COBJ_FLAG_DLBUFFERS;

    cobj->vec.at.x = cobj->vec.at.y = cobj->vec.at.z = 0.0F;
    cobj->vec.eye.x = cobj->vec.eye.y = 0.0F;
    cobj->vec.eye.z = 2000.0F;
}

// 0x80191364
void SC1PGameBossWallpaper0ProcDisplay(GObj *gobj)
{
    s32 color_id = DObjGetStruct(gobj)->child->user_data.s;
    s32 alpha = gobj->user_data.s;

    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
    gDPSetEnvColor(gSYTaskmanDLHeads[1]++, dSC1PGameBossCometEnvColorR[color_id], dSC1PGameBossCometEnvColorG[color_id], dSC1PGameBossCometEnvColorB[color_id], alpha);

    gcDrawDObjTreeDLLinksForGObj(gobj);

    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
}

// 0x80191498
void SC1PGameBossWallpaper1ProcDisplay(GObj *gobj)
{
    s32 alpha = gobj->user_data.s;
    DObj *dobj = DObjGetStruct(gobj);

    dobj = dobj->child;

    while (dobj != NULL)
    {
        dobj->mobj->sub.primcolor.s.a = alpha;
        dobj = dobj->child;
    }
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetCycleType(gSYTaskmanDLHeads[1]++, G_CYC_2CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_PASS, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, 0x00, 0x00, 0x00, alpha);

    gcDrawDObjTreeDLLinksForGObj(gobj);

    gDPSetCycleType(gSYTaskmanDLHeads[1]++, G_CYC_1CYCLE);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
}

// 0x801915B8
void SC1PGameBossWallpaper2ProcDisplay(GObj *gobj)
{
    s32 alpha = gobj->user_data.s;
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        if (dobj->mobj != NULL) // NULL check here but not the function above? WTF?
        {
            dobj->mobj->sub.primcolor.s.a = alpha;
        }
        dobj = dobj->child;
    }
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);

    gcDrawDObjTreeDLLinksForGObj(gobj);

    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
}

// 0x801916A8
void SC1PGameBossWallpaper3ProcDisplay0(GObj *gobj)
{
    s32 alpha = gobj->user_data.s;
    DObj *dobj = DObjGetStruct(gobj);

    while (dobj != NULL)
    {
        if (dobj->mobj != NULL)
        {
            dobj->mobj->sub.primcolor.s.a = alpha;
        }
        dobj = dobj->child;
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);

    gcDrawDObjTreeForGObj(gobj);

    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x80191798
void sc1PGameBossProcDisplayFadeAlpha(GObj *gobj)
{
    s32 alpha;

    sSC1PGameBossWallpaperStepRGBA++;

    if (sSC1PGameBossWallpaperStepRGBA > 255.0F)
    {
        sSC1PGameBossWallpaperStepRGBA = 255.0F;
    }
    alpha = sSC1PGameBossWallpaperStepRGBA;

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x80191908
void sc1PGameBossProcDisplayFadeColor(GObj *gobj)
{
    f32 sub = 2.55F;
    s32 color;

    sSC1PGameBossWallpaperStepRGBA -= sub; // Maybe this is what they did? Doing this only because there's unused stack otherwise.

    if (sSC1PGameBossWallpaperStepRGBA < 0.0F)
    {
        sSC1PGameBossWallpaperStepRGBA = 0.0F;
    }
    color = sSC1PGameBossWallpaperStepRGBA;

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, color, color, color, 0xFF);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, 310, 230);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x80191A94
void sc1PGameBossUpdateWallpaperColorID(void)
{
    GObj *gobj = gGCCommonLinks[nGCCommonLinkIDWallpaper];

    while (gobj != NULL)
    {
        if (gobj->id == nGCCommonKindBossWallpaper)
        {
            s32 color = sSC1PGameBossMain.bosswallpaper->color_id;

            DObjGetStruct(gobj)->user_data.s = color * -1;
        }
        gobj = gobj->link_next;
    }
}

// 0x80191AEC
void SC1PGameBossWallpaper3ProcUpdate0(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);

    gobj->user_data.s += dobj->user_data.s;

    if (gobj->user_data.s < 0)
    {
        gcEjectGObj(gobj);
    }
    else
    {
        if (gobj->user_data.s > 0xFF)
        {
            gobj->user_data.s = 0xFF;
        }
        gcPlayAnimAll(gobj);
    }
}

// 0x80191B44
void func_ovl65_80191B44(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);
    f32 lr;
    f32 bt;
    s32 angle;
    s32 sw;

    sw = 0;
    angle = (syUtilsRandIntRange(2) * 30) + 30;

    lr = ABS(gMPCollisionBounds.current.left - 2000.0F) + ABS(gMPCollisionBounds.current.right + 2000.0F);
    bt = ABS(gMPCollisionGroundData->map_bound_top - 2000.0F) + ABS(gMPCollisionGroundData->map_bound_bottom + 2000.0F);

    dobj->translate.vec.f.x = (syUtilsRandFloat() * lr) + (gMPCollisionBounds.current.left - 2000.0F);
    dobj->translate.vec.f.y = (syUtilsRandFloat() * bt) + (gMPCollisionGroundData->map_bound_bottom + 2000.0F);

    if (dobj->translate.vec.f.x < 0.0F)
    {
        sw = 2;
    }
    if (dobj->translate.vec.f.y < 0.0F)
    {
        sw++;
    }
    switch (sw)
    {
    case 1:
        angle += 180;
        break;

    case 3:
        angle += 90;
        break;

    case 0:
        angle += 270;
        break;
    }
    dobj->rotate.vec.f.z = F_CLC_DTOR32(angle);
}

// 0x80191D60
void SC1PGameBossWallpaper0ProcUpdate(GObj *gobj)
{
    SC1PGameBossWallpaper3ProcUpdate0(gobj);

    if (gobj->anim_frame <= 0.0F)
    {
        func_ovl65_80191B44(gobj);
    }
}

// 0x80191DA4
void SC1PGameBossWallpaper1ProcUpdate(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);

    if (dobj->user_data.s == -1)
    {
        dobj->anim_speed += (-0.0012);

        gcSetAllAnimSpeed(gobj, dobj->anim_speed);
    }
    else if (gobj->user_data.s < 0xFF)
    {
        dobj->translate.vec.f.y += (-18.8F);
    }
    SC1PGameBossWallpaper3ProcUpdate0(gobj);
}

// 0x80191E28
void SC1PGameBossWallpaper2ProcUpdate0(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj);

    if (gobj->user_data.s == 0)
    {
        dobj->scale.vec.f.x = dobj->scale.vec.f.y = dobj->scale.vec.f.z = 0.0F;
    }
    if (gSCManagerBattleState->players[sSC1PGameBossMain.bossplayer].stock_damage_all > 270)
    {
        dobj->anim_speed += 0.02;

        gcSetAllAnimSpeed(gobj, dobj->anim_speed);
    }
    else if (gobj->user_data.s < 0xFF)
    {
        dobj->scale.vec.f.x += 0.004;
        dobj->scale.vec.f.y += 0.004;
        dobj->scale.vec.f.z += 0.004;
    }
    SC1PGameBossWallpaper3ProcUpdate0(gobj);
}

// 0x80191F28
void SC1PGameBossWallpaper2ProcUpdate1(GObj *gobj)
{
    if (gSCManagerBattleState->players[sSC1PGameBossMain.bossplayer].stock_damage_all > 270)
    {
        gobj->flags = GOBJ_FLAG_NONE;

        SC1PGameBossWallpaper3ProcUpdate0(gobj);
    }
    else gobj->flags = GOBJ_FLAG_HIDDEN;
}

// 0x80191F90
void SC1PGameBossWallpaper3ProcUpdate1(GObj *gobj)
{
    DObj *dobj = DObjGetStruct(gobj)->child;

    if (dobj->anim_wait == AOBJ_ANIM_NULL)
    {
        if ((gobj->proc_display != sc1PGameBossProcDisplayFadeAlpha) && (gobj->proc_display != sc1PGameBossProcDisplayFadeColor))
        {
            sSC1PGameBossWallpaperStepRGBA = 230.0F;
            dobj->user_data.s = 0x64;
            gobj->proc_display = sc1PGameBossProcDisplayFadeAlpha;
        }
        else
        {
            dobj->user_data.s--;

            if (dobj->user_data.s == 0)
            {
                if (gobj->proc_display == sc1PGameBossProcDisplayFadeAlpha)
                {
                    sSC1PGameBossWallpaperStepRGBA = 255.0F;
                    dobj->user_data.s = 0x64;
                    gobj->proc_display = sc1PGameBossProcDisplayFadeColor;
                }
                else if (gobj->proc_display == sc1PGameBossProcDisplayFadeColor)
                {
                    ifCommonBattleEndSetBossDefeat();
                    gcFuncGObjAll(ifCommonBattleInterfacePauseGObj, 0);
                }
            }
        }
    }
    else SC1PGameBossWallpaper3ProcUpdate0(gobj);
}

// 0x80192078
void sc1PGameBossSetupBackgroundDObjs(GObj *gobj, DObjDesc *dobjdesc, MObjSub ***p_mobjsubs, u8 transform_kind)
{
    s32 i, id;
    MObjSub **mobjsubs, *mobjsub;
    DObj *array_dobjs[DOBJ_ARRAY_MAX], *dobj;

    for (i = 0; i < ARRAY_COUNT(array_dobjs); i++)
    {
        array_dobjs[i] = NULL;
    }
    id = (dobjdesc->id & 0xFFF);

    while ((id ^ 0) != ARRAY_COUNT(array_dobjs)) // Ewwwww... we meet again, XOR hack.
    {
        if (id != 0)
        {
            dobj = array_dobjs[id] = gcAddChildForDObj(array_dobjs[id - 1], dobjdesc->dl);
        }
        else dobj = array_dobjs[0] = gcAddDObjForGObj(gobj, dobjdesc->dl);
        
        id = dobjdesc->id & 0xF000;

        if (id != 0)
        {
            gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
            gcAddXObjForDObjFixed(dobj, nGCMatrixKind46, 0);
        }
        else gcAddXObjForDObjFixed(dobj, transform_kind, 0);

        dobj->translate.vec.f = dobjdesc->translate;
        dobj->rotate.vec.f = dobjdesc->rotate;
        dobj->scale.vec.f = dobjdesc->scale;

        if (p_mobjsubs != NULL)
        {
            if (*p_mobjsubs != NULL)
            {
                mobjsubs = *p_mobjsubs;

                mobjsub = *mobjsubs;

                while (mobjsub != NULL)
                {
                    gcAddMObjForDObj(dobj, mobjsub);

                    mobjsubs++;
                    
                    mobjsub = *mobjsubs;
                }
            }
            p_mobjsubs++;
        }
        dobjdesc++, id = dobjdesc->id & 0xFFF;
    }
}

// 0x8019223C
void sc1PGameBossSetWallpaperTranslate(GObj *gobj, s32 plan_id)
{
    DObj *dobj = DObjGetStruct(gobj);

    if (sSC1PGameBossMain.bosswallpaper->is_random_wallpaper == TRUE)
    {
        func_ovl65_80191B44(gobj);

        dobj->translate.vec.f.z = sSC1PGameBossMain.bosswallpaper->bossplan[plan_id].pos.z;
    }
    else dobj->translate.vec.f = sSC1PGameBossMain.bosswallpaper->bossplan[plan_id].pos;
}

// 0x801922D4
GObj* sc1PGameBossMakeWallpaperEffect(s32 effect_id, s32 anim_id, s32 plan_id)
{
    GObj *effect_gobj;
    DObj *dobj;
    uintptr_t addr;
    uintptr_t o_mobjsub;
    uintptr_t o_anim_joint;
    uintptr_t o_matanim_joint;
    s32 dobj_color_id = sSC1PGameBossMain.bosswallpaper->dobj_color_id;
    
    o_matanim_joint = 0x0;

    effect_gobj = gcMakeGObjSPAfter(nGCCommonKindBossWallpaper, NULL, nGCCommonLinkIDWallpaperEffect, GOBJ_PRIORITY_DEFAULT);

    if (effect_gobj == NULL)
    {
        return NULL;
    }
    addr = (uintptr_t)sSC1PGameBossMain.file_head;
    o_mobjsub = sSC1PGameBossMain.bosswallpaper->bosseffect[effect_id].o_mobjsub;

    gcAddGObjDisplay
    (
        effect_gobj, 
        sSC1PGameBossMain.bosswallpaper->bosseffect[effect_id].proc_display, 
        sSC1PGameBossMain.bosswallpaper->bossplan[plan_id].dl_link, 
        GOBJ_PRIORITY_DEFAULT, 
        sSC1PGameBossMain.bosswallpaper->bossplan[plan_id].camera_tag
    );
    sc1PGameBossSetupBackgroundDObjs
    (
        effect_gobj, 
        (DObjDesc*)(sSC1PGameBossMain.bosswallpaper->bosseffect[effect_id].o_dobjdesc + addr),
        (o_mobjsub != 0) ? (MObjSub***)(addr + o_mobjsub) : NULL, 
        nGCMatrixKindTraRotRpyRSca
    );
    gcSetAllAnimSpeed(effect_gobj, sSC1PGameBossMain.bosswallpaper->bossanim[anim_id].anim_speed);
    gcAddGObjProcess(effect_gobj, sSC1PGameBossMain.bosswallpaper->bosseffect[effect_id].proc_update, nGCProcessKindFunc, 1);

    dobj = DObjGetStruct(effect_gobj);

    o_anim_joint = sSC1PGameBossMain.bosswallpaper->bossanim[anim_id].o_anim_joint;

    if (o_mobjsub != 0)
    {
        o_matanim_joint = sSC1PGameBossMain.bosswallpaper->bossanim[anim_id].o_matanim_joint;
    }
    if ((o_anim_joint != 0) || (o_matanim_joint != 0))
    {
        lbCommonAddTreeDObjsAnimAll(dobj, (o_anim_joint != 0) ? (void*) (addr + o_anim_joint) : NULL, (o_matanim_joint != 0) ? (void*) (addr + o_matanim_joint) : NULL, 0.0F);
        gcPlayAnimAll(effect_gobj);
    }
    DObjGetStruct(effect_gobj)->child->user_data.s = syUtilsRandIntRange
    (
        (
            ARRAY_COUNT(dSC1PGameBossCometEnvColorR) + 
            ARRAY_COUNT(dSC1PGameBossCometEnvColorG) + 
            ARRAY_COUNT(dSC1PGameBossCometEnvColorR)
        ) / 3
    );

    DObjGetStruct(effect_gobj)->user_data.s = dobj_color_id;
    effect_gobj->user_data.s = 0;

    return effect_gobj;
}

// 0x801924E0
void sc1PGameBossAdvanceWallpaper(void)
{
    GObj *gobj;
    s32 anim_id;
    s32 effect_id;
    s32 plan_id;
    s32 i, j, k;

    for (i = j = k = plan_id = 0; i < sSC1PGameBossMain.bosswallpaper->loop_count; i++, j++)
    {
        if (sSC1PGameBossMain.bosswallpaper->is_random_wallpaper == TRUE)
        {
            effect_id = syUtilsRandIntRange(sSC1PGameBossMain.bosswallpaper->effect_count);
            anim_id   = syUtilsRandIntRange(sSC1PGameBossMain.bosswallpaper->anim_count);

            if (j == sSC1PGameBossMain.bosswallpaper->bossplan[k].unk_sc1pbossplan_0x0)
            {
                plan_id++, k++;
                j = 0;
            }
        }
        else
        {
            effect_id = anim_id = i;

            if (j == sSC1PGameBossMain.bosswallpaper->bossplan[k].unk_sc1pbossplan_0x0)
            {
                plan_id++, k++;
                j = 0;
            }
        }
        gobj = sc1PGameBossMakeWallpaperEffect(effect_id, anim_id, plan_id);

        if (gobj != NULL)
        {
            sc1PGameBossSetWallpaperTranslate(gobj, plan_id);
        }
    }
    sSC1PGameBossMain.is_skip_wallpaper_change = TRUE;
    sSC1PGameBossMain.wallpaper_id++;
    sSC1PGameBossMain.change_wait = sSC1PGameBossMain.bosswallpaper->change_wait_base;
}

// 0x80192620
void sc1PGameBossWallpaperProcUpdate(GObj *gobj)
{
    if (sSC1PGameBossMain.is_skip_wallpaper_change == FALSE)
    {
        sc1PGameBossUpdateWallpaperColorID();
        sSC1PGameBossMain.bosswallpaper = &dSC1PGameBossWallpapers[sSC1PGameBossMain.wallpaper_id];
        sc1PGameBossAdvanceWallpaper();
    }
    if (sSC1PGameBossMain.change_wait != -1)
    {
        sSC1PGameBossMain.change_wait--;
    }
    if (sSC1PGameBossMain.bosswallpaper->change_damage_min != -1)
    {
        if (sSC1PGameBossMain.bosswallpaper->change_damage_min < gSCManagerBattleState->players[sSC1PGameBossMain.bossplayer].stock_damage_all)
        {
            sSC1PGameBossMain.is_skip_wallpaper_change = FALSE;
        }
    }
    else if (sSC1PGameBossMain.change_wait == 0)
    {
        sSC1PGameBossMain.is_skip_wallpaper_change = FALSE;
    }
}

// 0x801926F8
void sc1PGameBossSetBossPlayer(void)
{
    s32 player;

    for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
    {
        if (gSCManagerBattleState->players[player].fkind == nFTKindBoss)
        {
            sSC1PGameBossMain.bossplayer = player;
        }
    }
}

// 0x80192764
void sc1PGameBossInitWallpaper(void)
{
    GObj *gobj;

    sSC1PGameBossWallpaperGObj = gobj = gcMakeGObjSPAfter(nGCCommonKindWallpaper, NULL, nGCCommonLinkIDWallpaper, GOBJ_PRIORITY_DEFAULT);

    if (gobj != NULL)
    {
        gcAddGObjProcess(gobj, sc1PGameBossWallpaperProcUpdate, nGCProcessKindFunc, 3);

        sc1PGameBossMakeCamera();
        sc1PGameBossSetBossPlayer();

        sSC1PGameBossMain.is_skip_wallpaper_change = FALSE;
        sSC1PGameBossMain.wallpaper_id = 0;
        sSC1PGameBossMain.file_head = (void*) ((uintptr_t)gMPCollisionGroundData->gr_desc[1].dobjdesc - (intptr_t)&llGRLastMapFileHead);
        sSC1PGameBossMain.change_wait = 0;
        sSC1PGameBossWallpaperStepRGBA = 0.0F;
    }
}
