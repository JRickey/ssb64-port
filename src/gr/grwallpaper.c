#include <gr/ground.h>
#include <sc/scene.h>
#include <sc/sc1pmode/sc1pgameboss.h>

#ifdef PORT
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
extern void port_log(const char *fmt, ...);
extern bool portRelocDescribePointer(const void *ptr, uintptr_t *out_base, size_t *out_size, u32 *out_file_id, const char **out_path);
extern void portDiagArmImportCapture(int n);
extern char *getenv(const char *name);
extern int atoi(const char *nptr);
#endif

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
#ifdef PORT
    {
        Sprite *sp = (Sprite*)PORT_RESOLVE(gMPCollisionGroundData->wallpaper);
        port_log("[wallpaper] MakeCommon scene=%d gkind=%d gd=%p wp_tok=0x%x wp_resolved=%p",
            gSCManagerSceneData.scene_curr,
            gSCManagerBattleState ? gSCManagerBattleState->gkind : -1,
            (void*)gMPCollisionGroundData,
            (unsigned)(uintptr_t)gMPCollisionGroundData->wallpaper,
            (void*)sp);
        if (sp != NULL) {
            void *bm = PORT_RESOLVE(sp->bitmap);
            port_log("[wallpaper] sprite w=%d h=%d bmH=%d bmHreal=%d fmt=%d siz=%d nbm=%d bm_tok=0x%x bm_resolved=%p",
                sp->width, sp->height, sp->bmheight, sp->bmHreal,
                sp->bmfmt, sp->bmsiz, sp->nbitmaps,
                (unsigned)(uintptr_t)sp->bitmap, bm);
            if (bm != NULL) {
                Bitmap *b0 = (Bitmap*)bm;
                void *buf0 = PORT_RESOLVE(b0->buf);
                port_log("[wallpaper] bm[0] w=%d wimg=%d s=%d t=%d buf_tok=0x%x buf_resolved=%p",
                    b0->width, b0->width_img, b0->s, b0->t,
                    (unsigned)(uintptr_t)b0->buf, buf0);
                if (buf0 != NULL) {
                    const unsigned char *p = (const unsigned char*)buf0;
                    uintptr_t base = 0;
                    size_t size = 0;
                    u32 file_id = 0;
                    const char *path = NULL;
                    bool has_reloc = portRelocDescribePointer(buf0, &base, &size, &file_id, &path);
                    port_log("[wallpaper] bm[0] reloc has=%d file_id=0x%x off=0x%lx size=0x%lx path=%s",
                        (int)has_reloc, (unsigned)file_id,
                        has_reloc ? (unsigned long)((uintptr_t)buf0 - base) : 0UL,
                        (unsigned long)size, path != NULL ? path : "(null)");
                    port_log("[wallpaper] bm[0] bytes %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
                        p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],
                        p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15]);
                }
            }
        }
        {
            const char *arm_env = getenv("SSB64_DIAG_ARM_WALLPAPER_SCENES");
            if (arm_env != NULL && arm_env[0] != '\0') {
                /* Comma-separated scene list; arm capture when scene_curr matches.
                   Empty -> never. SSB64_DIAG_ARM_WALLPAPER_N controls slot count
                   (default 4000). */
                const char *cursor = arm_env;
                int matched = 0;
                while (*cursor != '\0') {
                    char *end = NULL;
                    long parsed = (long)atoi(cursor);
                    /* atoi doesn't return end pointer; advance past digits/sign manually. */
                    if (*cursor == '-' || *cursor == '+') cursor++;
                    while (*cursor >= '0' && *cursor <= '9') cursor++;
                    if ((s32)parsed == gSCManagerSceneData.scene_curr) {
                        matched = 1;
                        break;
                    }
                    while (*cursor == ',' || *cursor == ' ' || *cursor == ';') cursor++;
                    if (*cursor == '\0') break;
                    (void)end;
                }
                if (matched) {
                    const char *n_env = getenv("SSB64_DIAG_ARM_WALLPAPER_N");
                    int n = (n_env != NULL && n_env[0] != '\0') ? atoi(n_env) : 4000;
                    if (n <= 0) n = 4000;
                    portDiagArmImportCapture(n);
                    port_log("[wallpaper] DIAG armed: capturing next %d ImportTexture calls (scene=%d)",
                        n, (int)gSCManagerSceneData.scene_curr);
                }
            }
        }
    }
#endif

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
        (Sprite*)PORT_RESOLVE(gMPCollisionGroundData->wallpaper),
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
        (Sprite*)PORT_RESOLVE(gMPCollisionGroundData->wallpaper),
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

    wallpaper_sobj = lbCommonMakeSObjForGObj(wallpaper_gobj, (Sprite*)PORT_RESOLVE(gMPCollisionGroundData->wallpaper));

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
