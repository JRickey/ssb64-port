#include <gr/ground.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801313D0
ub8 sGRWallpaperIsPausePerspUpdate;

// 0x801313D4
s32 sGRWallpaperPad801313D4;

// 0x801313D8
GObj *sGRWallpaperGObj;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012E7C0
Gfx dGRWallpaperDisplayList[/* */] =
{
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
    gsDPSetFillColor(GPACK_FILL16(GPACK_RGBA5551(0x00, 0x00, 0x00, 0x01))),
    gsDPFillRectangle(10, 10, 310, 230), // 10 less than width and height?
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80104620 - Calculate perspective of stage background image?
void grWallpaperCalcPersp(SObj *wallpaper_sobj)
{
    f32 mag;
    CObj *cobj;
    Vec2f angle;
    Vec3f dist;
    f32 bak_pos_x;
    f32 bak_pos_y;
    f32 pos_x;
    f32 pos_y;
    f32 neg;
    f32 width;
    f32 height;
    f32 scale;

    cobj = CObjGetStruct(gGMCameraGObj);

    syVectorDiff3D(&dist, &cobj->vec.eye, &cobj->vec.at);

    mag = syVectorMag3D(&dist);

    if (dist.z < 0.0F)
    {
        angle.x = 0.0F;
        angle.y = 0.0F;
    }
    else
    {
        angle.y = syUtilsArcTan2(dist.x, dist.z);
        angle.x = syUtilsArcTan2(dist.y, dist.z);
    }
    scale = 20000.0F / (mag + 8000.0F);

    if (scale < 1.004F)
    {
        scale = 1.004F;
    }
    else if (scale > 2.0F)
    {
        scale = 2.0F;
    }
    wallpaper_sobj->sprite.scalex = wallpaper_sobj->sprite.scaley = scale;

    width = 300.0F * scale;     // Background image width  is 300px 
    height = 220.0F * scale;    // Background image height is 220px

    // Remember to make a macro for default resolution if one does not yet exist
    pos_x = bak_pos_x = ((angle.y / F_CST_DTOR32(180.0F)) * width) - ((width - 320.0F) * 0.5F);      // Ultra64 default width  is 320px
    pos_y = bak_pos_y = ((-angle.x / F_CST_DTOR32(180.0F)) * height) - ((height - 240.0F) * 0.5F);   // Ultra64 default height is 240px

    if (bak_pos_x > 10.0F)
    {
        pos_x = 10.0F;
    }
    else
    {
        neg = (-width - 10.0F) + 320.0F;

        if (bak_pos_x < neg)
        {
            pos_x = neg;
        }
    }
    if (bak_pos_y > 10.0F)
    {
        pos_y = 10.0F;
    }
    else
    {
        neg = (-height - 10.0F) + 240.0F;

        if (bak_pos_y < neg)
        {
            pos_y = neg;
        }
    }
    wallpaper_sobj->pos.x = pos_x;
    wallpaper_sobj->pos.y = pos_y;
}

// 0x80104830
void grWallpaperCommonProcUpdate(GObj *wallpaper_gobj)
{
    grWallpaperCalcPersp(SObjGetStruct(wallpaper_gobj));
}

// 0x80104850
void grWallpaperMakeCommon(void)
{
    GObj *wallpaper_gobj;
    SObj *wallpaper_sobj;

    sGRWallpaperGObj = wallpaper_gobj = lbCommonMakeSpriteGObj
    (
        nGCCommonKindWallpaper,
        NULL,
        nGCCommonLinkIDWallpaper,
        GOBJ_PRIORITY_DEFAULT,
        lbCommonDrawSObjAttr,
        0,
        GOBJ_PRIORITY_DEFAULT,
        ~0,
        gMPCollisionGroundData->wallpaper,
        nGCProcessKindFunc,
        grWallpaperCommonProcUpdate,
        3
    );
    wallpaper_sobj = SObjGetStruct(wallpaper_gobj);

    wallpaper_sobj->pos.x = 10.0F;
    wallpaper_sobj->pos.y = 10.0F;

    wallpaper_sobj->sprite.attr = SP_TEXSHUF;
    wallpaper_sobj->sprite.scalex = wallpaper_sobj->sprite.scaley = 1.004F;
}

// 0x801048F8
void grWallpaperMakeStatic(void)
{
    GObj *wallpaper_gobj;
    SObj *wallpaper_sobj;

    sGRWallpaperGObj = wallpaper_gobj = lbCommonMakeSpriteGObj
    (
        nGCCommonKindWallpaper, 
        NULL, 
        nGCCommonLinkIDWallpaper,
        GOBJ_PRIORITY_DEFAULT, 
        lbCommonDrawSObjAttr,
        0, 
        GOBJ_PRIORITY_DEFAULT, 
        -1, 
        gMPCollisionGroundData->wallpaper, 
        nGCProcessKindFunc, 
        NULL, 
        3
    );
    wallpaper_sobj = SObjGetStruct(wallpaper_gobj);

    wallpaper_sobj->pos.x = 10.0F;
    wallpaper_sobj->pos.y = 10.0F;

    wallpaper_sobj->sprite.attr = SP_TEXSHUF | SP_FASTCOPY;
    wallpaper_sobj->sprite.scalex = wallpaper_sobj->sprite.scaley = 1.0F;
}

// 0x80104998
void grWallpaperSectorProcUpdate(GObj *wallpaper_gobj)
{
    CObj *cobj;
    SObj *wallpaper_sobj;
    f32 sqrt;
    Vec3f dist;
    f32 temp;
    f32 scale;

    cobj = gGMCameraGObj->obj;

    syVectorDiff3D(&dist, &cobj->vec.eye, &cobj->vec.at);

    sqrt = sqrtf(SQUARE(dist.x) + SQUARE(dist.y) + SQUARE(dist.z));

    if (sqrt > 0.0F)
    {
        temp = 20000.0F / (sqrt + 10000.0F);

        if (temp < 1.004F)
        {
            temp = 1.004F;
        }
        else if (temp > 2.0F)
        {
            temp = 2.0F;
        }
        scale = (temp - 1.0F) * 0.5F;

        wallpaper_sobj = SObjGetStruct(wallpaper_gobj);

        wallpaper_sobj->sprite.scalex = wallpaper_sobj->sprite.scaley = temp;

        wallpaper_sobj->pos.x = 10.0F - (300.0F * scale);
        wallpaper_sobj->pos.y = 10.0F - (220.0F * scale);
    }
}

// 0x80104ABC
void grWallpaperMakeSector(void)
{
    GObj *wallpaper_gobj;
    SObj *wallpaper_sobj;

    sGRWallpaperGObj = wallpaper_gobj = gcMakeGObjSPAfter(nGCCommonKindWallpaper, NULL, nGCCommonLinkIDWallpaper, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(wallpaper_gobj, lbCommonDrawSObjAttr, 0, GOBJ_PRIORITY_DEFAULT, ~0);

    wallpaper_sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, gMPCollisionGroundData->wallpaper);

    wallpaper_sobj->pos.x = 10.0F;
    wallpaper_sobj->pos.y = 10.0F;

    wallpaper_sobj->sprite.attr = SP_TEXSHUF;

    gcAddGObjProcess(wallpaper_gobj, grWallpaperSectorProcUpdate, nGCProcessKindFunc, 3);
}

// 0x80104B58
void grWallpaperBonus3ProcDisplay(GObj *wallpaper_gobj)
{
    gSPDisplayList(gSYTaskmanDLHeads[0]++, dGRWallpaperDisplayList);
}

// 0x80104B88
void grWallpaperMakeBonus3(void)
{
    GObj *wallpaper_gobj;

    sGRWallpaperGObj = wallpaper_gobj = gcMakeGObjSPAfter(nGCCommonKindWallpaper, NULL, nGCCommonLinkIDWallpaper, GOBJ_PRIORITY_DEFAULT);

    gcAddGObjDisplay(wallpaper_gobj, grWallpaperBonus3ProcDisplay, 0, GOBJ_PRIORITY_DEFAULT, ~0);
}

// 0x80104BDC
void grWallpaperMakeDecideKind(void)
{
    if (gSCManagerSceneData.scene_curr == nSCKind1PTrainingMode)
    {
        sc1PTrainingModeLoadWallpaper();
        grWallpaperMakeStatic();
    }
    else if (gSCManagerBattleState->gkind >= nGRKindBonusStageStart)
    {
        grWallpaperMakeStatic();
    }
    else switch (gSCManagerBattleState->gkind)
    {
    case nGRKindYoster:
    case nGRKindYosterSmall:
        grWallpaperMakeStatic();
        break;

    case nGRKindSector:
        grWallpaperMakeSector();
        break;

    case nGRKindBonus3:
        grWallpaperMakeBonus3();
        break;

    case nGRKindLast:
        sc1PGameBossInitWallpaper();
        grWallpaperMakeCommon();
        break;

    default:
        grWallpaperMakeCommon();
        break;
    }
}

// 0x80104CB4
void grWallpaperPausePerspUpdate(void)
{
    sGRWallpaperIsPausePerspUpdate = TRUE; // ??? Runs when the game is paused
}

// 0x80104CC4
void grWallpaperResumePerspUpdate(void)
{
    sGRWallpaperIsPausePerspUpdate = FALSE; // Similarly, this runs when the game is unpaused
}

// 0x80104CD0 - New file?
void grWallpaperRunProcessAll(void)
{
    GObjProcess *gobjproc = sGRWallpaperGObj->gobjproc_head;

    while (gobjproc != NULL)
    {
        if (gobjproc->is_paused == FALSE)
        {
            if (gobjproc->exec.func != NULL)
            {
                gobjproc->exec.func(gobjproc->parent_gobj);
            }
        }
        gobjproc = gobjproc->link_next;
    }
}

// 0x80104D30
void grWallpaperResumeProcessAll(void)
{
    GObj *gobj = gGCCommonLinks[nGCCommonLinkIDWallpaper];

    while (gobj != NULL)
    {
        if (gobj->id == nGCCommonKindWallpaper)
        {
            gcResumeGObjProcessAll(gobj);
        }
        gobj = gobj->link_next;
    }
}
