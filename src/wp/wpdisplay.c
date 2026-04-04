#include <wp/weapon.h>
#include <ft/fighter.h>
#include <sys/develop.h>
#include <sys/matrix.h>
#include <sys/malloc.h>

extern SYMallocRegion gSYTaskmanGraphicsHeap;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188E10
SYColorRGB dWPDisplayPKThunderPrimColors[/* */] = { { 0x5E, 0xA3, 0xFF }, { 0x98, 0xBD, 0xFF }, { 0xC2, 0xD9, 0xFF }, { 0xB3, 0xF1, 0xFF } };

// 0x80188E1C
SYColorRGB dWPDisplayPKThunderEnvColors[/* */] = { { 0x3A, 0x00, 0x83 }, { 0x5B, 0x00, 0xB2 }, { 0x86, 0x33, 0xD9 }, { 0xA7, 0x74, 0xF8 } };

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80166E80
void wpDisplayHitCollisions(GObj *weapon_gobj) // Render weapon hitboxes
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    WPAttackColl *attack_coll = &wp->attack_coll;
    Mtx *m;
    s32 i;

    for (i = 0; i < attack_coll->attack_count; i++)
    {
        if ((attack_coll->attack_state != nGMAttackStateOff) && (attack_coll->attack_state != nGMAttackStateNew))
        {
            gDPPipeSync(gSYTaskmanDLHeads[0]++);

            if (wp->display_mode == nDBDisplayModeHitAttackOutline)
            {
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xB0, 0x00, 0x00, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xB0, 0x00, 0x00, 0xFF);
                gDPSetBlendColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0xE0);
            }
            else
            {
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xB0, 0x00, 0x00, 0xFF);
                gDPSetBlendColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0x00);
            }
            if (attack_coll->attack_state == nGMAttackStateInterpolate)
            {
                syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

                syMatrixTra(m, attack_coll->attack_pos[i].pos_prev.x, attack_coll->attack_pos[i].pos_prev.y, attack_coll->attack_pos[i].pos_prev.z);

                gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

                syMatrixSca(m, attack_coll->size / 15.0F, attack_coll->size / 15.0F, attack_coll->size / 15.0F);

                gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionEdgeDL);
                gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            }
            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixTra(m, attack_coll->attack_pos[i].pos_curr.x, attack_coll->attack_pos[i].pos_curr.y, attack_coll->attack_pos[i].pos_curr.z);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixSca(m, attack_coll->size / 15.0F, attack_coll->size / 15.0F, attack_coll->size / 15.0F);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            if (attack_coll->attack_state == nGMAttackStateInterpolate)
            {
                gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionBlendDL);
            }
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionCubeDL);
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
    }
}

// 0x801671F0
void wpDisplayMapCollisions(GObj *weapon_gobj) // Render weapon ECB?
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f *translate = &DObjGetStruct(weapon_gobj)->translate.vec.f;
    MPObjectColl *map_coll = &wp->coll_data.map_coll;
    Mtx *m;

    gDPPipeSync(gSYTaskmanDLHeads[1]++);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixTra(m, translate->x, translate->y + map_coll->bottom, translate->z);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixSca(m, map_coll->width / 30.0F, (map_coll->center - map_coll->bottom) / 30.0F, 1.0F);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(gSYTaskmanDLHeads[1]++, dFTDisplayMainMapCollisionBottomDL);
    gSPPopMatrix(gSYTaskmanDLHeads[1]++, G_MTX_MODELVIEW);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixTra(m, translate->x, translate->y + map_coll->center, translate->z);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

    syMatrixSca(m, map_coll->width / 30.0F, (map_coll->top - map_coll->center) / 30.0F, 1.0F);

    gSPMatrix(gSYTaskmanDLHeads[1]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(gSYTaskmanDLHeads[1]++, dFTDisplayMainMapCollisionTopDL);
    gSPPopMatrix(gSYTaskmanDLHeads[1]++, G_MTX_MODELVIEW);
}

// 0x80167454
void wpDisplayDrawNormal(void)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gSPClearGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
}

// 0x801674B8
void wpDisplayDrawZBuffer(void)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gSPSetGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
}

// 0x80167520
void wpDisplayMain(GObj *weapon_gobj, void (*proc_display)(GObj*))
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wp->display_mode == nDBDisplayModeMapCollision)
    {
        wpDisplayDrawNormal();

        proc_display(weapon_gobj);

        wpDisplayDrawZBuffer();

        wpDisplayMapCollisions(weapon_gobj);
    }
    else if ((wp->display_mode == nDBDisplayModeMaster) || (wp->attack_coll.attack_state == nGMAttackStateOff))
    {
        wpDisplayDrawNormal();

        proc_display(weapon_gobj);

        wpDisplayDrawZBuffer();
    }
    else wpDisplayHitCollisions(weapon_gobj);
}

// 0x801675D0
void wpDisplayDLHead1(GObj *weapon_gobj)
{
    wpDisplayMain(weapon_gobj, gcDrawDObjDLHead1);
}

// 0x801675F4
void wpDisplayDObjDLLinks(GObj *weapon_gobj)
{
    wpDisplayMain(weapon_gobj, gcDrawDObjDLLinksForGObj);
}

// 0x80167618
void func_ovl3_80167618(GObj *weapon_gobj)
{
    wpDisplayMain(weapon_gobj, lbCommonDObjScaleXProcDisplay); // Unused?
}

// 0x8016763C
void wpDisplayDObjTreeDLLinks(GObj *weapon_gobj)
{
    wpDisplayMain(weapon_gobj, gcDrawDObjTreeDLLinksForGObj);
}

// 0x80167660
void wpDisplayPKThunderProcDisplay(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    s32 index = wp->weapon_vars.pkthunder_trail.trail_id;

    if (wp->display_mode == nDBDisplayModeMapCollision)
    {
        wpDisplayDrawNormal();

        gDPPipeSync(gSYTaskmanDLHeads[1]++);
        gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, dWPDisplayPKThunderPrimColors[index].r, dWPDisplayPKThunderPrimColors[index].g, dWPDisplayPKThunderPrimColors[index].b, 0xFF);
        gDPSetEnvColor(gSYTaskmanDLHeads[1]++, dWPDisplayPKThunderEnvColors[index].r, dWPDisplayPKThunderEnvColors[index].g, dWPDisplayPKThunderEnvColors[index].b, 0xFF);

        gcDrawDObjDLLinksForGObj(weapon_gobj);

        wpDisplayDrawZBuffer();

        wpDisplayMapCollisions(weapon_gobj);
    }
    else if ((wp->display_mode == nDBDisplayModeMaster) || (wp->attack_coll.attack_state == nGMAttackStateOff))
    {
        wpDisplayDrawNormal();

        gDPPipeSync(gSYTaskmanDLHeads[1]++);
        gDPSetPrimColor(gSYTaskmanDLHeads[1]++, 0, 0, dWPDisplayPKThunderPrimColors[index].r, dWPDisplayPKThunderPrimColors[index].g, dWPDisplayPKThunderPrimColors[index].b, 0xFF);
        gDPSetEnvColor(gSYTaskmanDLHeads[1]++, dWPDisplayPKThunderEnvColors[index].r, dWPDisplayPKThunderEnvColors[index].g, dWPDisplayPKThunderEnvColors[index].b, 0xFF);

        gcDrawDObjDLLinksForGObj(weapon_gobj);

        wpDisplayDrawZBuffer();
    }
    else wpDisplayHitCollisions(weapon_gobj);
}
