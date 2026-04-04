#include <ft/fighter.h>
#include <sc/scene.h>
#include <reloc_data.h>

extern s32 gSYTaskmanTaskID;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80188410
Gfx dFTShadowNoPrevLinkDL[/* */] =
{
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetBlendColor(0x00, 0x00, 0x00, 0x0F),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList()
};

// 0x80188458
Gfx dFTShadowNoNextLinkDL[/* */] =
{
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8013AE10
f32 ftShadowGetAltitude(Vec3f *a, Vec3f *b, f32 f)
{
    if (b->x == a->x)
    {
        return a->y;
    }
    else return (((f - a->x) * (b->y - a->y)) / (b->x - a->x)) + a->y;
}

// 0x8013AE60
void ftShadowProcDisplay(GObj *shadow_gobj)
{
    FTShadow *fs;
    FTStruct *fp;
    f32 shadow_center;
    Vtx *shadow_vertex;
    f32 shadow_size;
    Vtx *sv;
    GObj *fighter_gobj;
    s32 i;
    s32 floor_line_id;
    Vec3f vertex_pos0;
    Vec3f vertex_pos1;
    f32 shadow_calc_left;
    f32 shadow_calc_right;
    s32 unused;
    f32 shadow_edge_left;
    f32 shadow_alt_left;
    f32 shadow_alt_right;
    f32 spF0;
    f32 spEC;
    f32 spE8;
    f32 spE4;
    f32 spE0;
    f32 spDC;
    s32 gfx_vertex_num;
    s32 coll_vertex_num;
    f32 shadow_edge_right;
    sb32 edge_left_or_right;

    if (shadow_gobj->dl_link_prev == NULL)
    {
        gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTShadowNoPrevLinkDL);

       
        gDPLoadTextureBlock_4b(gSYTaskmanDLHeads[0]++, ((uintptr_t)gEFManagerFiles[1] + (intptr_t)&llEFCommonEffects2ShadowTextureImage), G_IM_FMT_I, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
    }
    fs = (FTShadow*)shadow_gobj->user_data.p;

    fighter_gobj = gSCManagerBattleState->players[fs->player].fighter_gobj;

    fp = ftGetStruct(fighter_gobj);

    if (!(fp->is_invisible) && !(fp->is_shadow_hide))
    {
        Vec3f ga_last;
        Vec3f pos_project;
        f32 ga_dist;

        if (fp->ga == nMPKineticsGround)
        {
            floor_line_id = fp->coll_data.floor_line_id;

            ga_dist = 0;
        }
        else if (mpCollisionCheckProjectFloor(&DObjGetStruct(fighter_gobj)->translate.vec.f, &floor_line_id, &ga_dist, NULL, NULL) == FALSE)
        {
            floor_line_id = -1;
        }
        if ((floor_line_id != -1) && (fp->coll_data.floor_line_id != -2))
        {
            pos_project.x = DObjGetStruct(fighter_gobj)->translate.vec.f.x;
            pos_project.y = DObjGetStruct(fighter_gobj)->translate.vec.f.y - 65536;

            if 
            (
                (
                    ((mpCollisionCheckLWallLineCollisionSame(&DObjGetStruct(fighter_gobj)->translate.vec.f, &pos_project, &ga_last, NULL, NULL, NULL) == FALSE)) ||
                    !((DObjGetStruct(fighter_gobj)->translate.vec.f.y - ga_last.y) < -ga_dist)
                ) 
                &&
                (
                    (mpCollisionCheckRWallLineCollisionSame(&DObjGetStruct(fighter_gobj)->translate.vec.f, &pos_project, &ga_last, NULL, NULL, NULL) == FALSE) ||
                    !((DObjGetStruct(fighter_gobj)->translate.vec.f.y - ga_last.y) < -ga_dist)
                )
            )
            {
                shadow_center = DObjGetStruct(fighter_gobj)->translate.vec.f.x;
                shadow_size = fp->attr->shadow_size;
                shadow_calc_left = 0.0F, shadow_calc_right = 1984.0F;

                mpCollisionGetFloorEdgeL(floor_line_id, &vertex_pos0);
                mpCollisionGetFloorEdgeR(floor_line_id, &vertex_pos1);

                shadow_edge_left = shadow_center - shadow_size;
                shadow_edge_right = shadow_center + shadow_size;

                if (shadow_edge_left < vertex_pos0.x)
                {
                    shadow_edge_left = vertex_pos0.x;

                    shadow_calc_left = (((shadow_edge_left - shadow_center) + shadow_size) * 992.0F) / shadow_size;

                    if (shadow_calc_left < 0.0F)
                    {
                        shadow_calc_left = 0.0F;
                    }
                    if (shadow_calc_left > 2048.0F)
                    {
                        shadow_calc_left = 2048.0F;
                    }
                }
                if (vertex_pos1.x < shadow_edge_right)
                {
                    shadow_edge_right = vertex_pos1.x;

                    shadow_calc_right = (((shadow_edge_right - shadow_center) * 992.0F) / shadow_size) + 992.0F;

                    if (shadow_calc_right < 0.0F)
                    {
                        shadow_calc_right = 0.0F;
                    }
                    if (shadow_calc_right > 2048.0F)
                    {
                        shadow_calc_right = 2048.0F;
                    }
                }
                gfx_vertex_num = 4;
                coll_vertex_num = mpCollisionGetVertexCountLineID(floor_line_id);

                if (coll_vertex_num >= 3)
                {
                    mpCollisionGetVertexPositionID(floor_line_id, 0, &vertex_pos0);
                    mpCollisionGetVertexPositionID(floor_line_id, 1, &vertex_pos1);

                    if (vertex_pos0.x < vertex_pos1.x)
                    {
                        edge_left_or_right = 0;

                        for (i = 1; i < coll_vertex_num; i++)
                        {
                            mpCollisionGetVertexPositionID(floor_line_id, i, &vertex_pos1);

                            if ((vertex_pos0.x <= shadow_edge_left) && (shadow_edge_left <= vertex_pos1.x))
                            {
                                shadow_alt_left = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_left);

                                if ((vertex_pos0.x <= shadow_edge_right) && (shadow_edge_right <= vertex_pos1.x))
                                {
                                    shadow_alt_right = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_right);
                                }
                                else
                                {
                                    gfx_vertex_num = 6;
                                    vertex_pos0 = vertex_pos1;

                                    mpCollisionGetVertexPositionID(floor_line_id, i + 1, &vertex_pos1);

                                    spE8 = vertex_pos0.x;
                                    spE4 = vertex_pos0.y;
                                    spF0 = (((shadow_calc_right - shadow_calc_left) * (vertex_pos0.x - shadow_edge_left)) / (shadow_edge_right - shadow_edge_left)) + shadow_calc_left;

                                    if (shadow_edge_right <= vertex_pos1.x)
                                    {
                                        shadow_alt_right = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_right);
                                    }
                                    else
                                    {
                                        gfx_vertex_num = 8;
                                        vertex_pos0 = vertex_pos1;

                                        mpCollisionGetVertexPositionID(floor_line_id, i + 2, &vertex_pos1);
                                        shadow_alt_right = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_right);
                                        spE0 = vertex_pos0.x;
                                        spDC = vertex_pos0.y;
                                        spEC = (((shadow_calc_right - shadow_calc_left) * (vertex_pos0.x - shadow_edge_left)) / (shadow_edge_right - shadow_edge_left)) + shadow_calc_left;
                                    }
                                }
                                break;
                            }
                            else vertex_pos0 = vertex_pos1;
                        }
                    }
                    else
                    {
                        edge_left_or_right = 1;
                        vertex_pos1 = vertex_pos0;

                        for (i = 1; i < coll_vertex_num; i++)
                        {
                            mpCollisionGetVertexPositionID(floor_line_id, i, &vertex_pos0);

                            if ((vertex_pos0.x <= shadow_edge_right) && (shadow_edge_right <= vertex_pos1.x))
                            {
                                shadow_alt_right = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_right);

                                if ((vertex_pos0.x <= shadow_edge_left) && (vertex_pos1.x >= shadow_edge_left))
                                {
                                    shadow_alt_left = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_left);
                                }
                                else
                                {
                                    gfx_vertex_num = 6;
                                    vertex_pos1 = vertex_pos0;

                                    mpCollisionGetVertexPositionID(floor_line_id, i + 1, &vertex_pos0);

                                    spE8 = vertex_pos1.x;
                                    spE4 = vertex_pos1.y;
                                    spF0 = (((shadow_calc_right - shadow_calc_left) * (vertex_pos1.x - shadow_edge_left)) / (shadow_edge_right - shadow_edge_left)) + shadow_calc_left;

                                    if (vertex_pos0.x <= shadow_edge_left)
                                    {
                                        shadow_alt_left = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_left);
                                    }
                                    else
                                    {
                                        gfx_vertex_num = 8;
                                        vertex_pos1 = vertex_pos0;

                                        mpCollisionGetVertexPositionID(floor_line_id, i + 2, &vertex_pos0);

                                        shadow_alt_left = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_left);
                                        spE0 = vertex_pos1.x;
                                        spDC = vertex_pos1.y;

                                        spEC = (((shadow_calc_right - shadow_calc_left) * (vertex_pos1.x - shadow_edge_left)) / (shadow_edge_right - shadow_edge_left)) + shadow_calc_left;
                                    }
                                }
                                break;
                            }
                            else vertex_pos1 = vertex_pos0;
                        }
                    }
                }
                else
                {
                    shadow_alt_left = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_left);
                    shadow_alt_right = ftShadowGetAltitude(&vertex_pos0, &vertex_pos1, shadow_edge_right);
                }
                if (gfx_vertex_num != 0)
                {
                    if (gSYTaskmanTaskID != 0)
                    {
                        shadow_vertex = &fs->vtx2[0];
                    }
                    else shadow_vertex = &fs->vtx1[0];

                    gDPPipeSync(gSYTaskmanDLHeads[0]++);

                    if ((gSCManagerBattleState->is_team_battle == TRUE) && !(gSCManagerBattleState->is_not_teamshadows))
                    {
                        gDPSetPrimColor
                        (
                            gSYTaskmanDLHeads[0]++,
                            0,
                            0,
                            dFTCommonDataShadowColorTeams[fp->team].r,
                            dFTCommonDataShadowColorTeams[fp->team].g,
                            dFTCommonDataShadowColorTeams[fp->team].b,
                            dFTCommonDataShadowColorTeams[fp->team].a
                        );
                    }
                    else gDPSetPrimColor
                    (
                        gSYTaskmanDLHeads[0]++,
                        0,
                        0,
                        dFTCommonDataShadowColorDefault.r,
                        dFTCommonDataShadowColorDefault.g,
                        dFTCommonDataShadowColorDefault.b,
                        dFTCommonDataShadowColorDefault.a
                    );
                    sv = shadow_vertex;

                    sv->n.ob[0] = shadow_edge_left;
                    sv->n.ob[1] = shadow_alt_left;
                    sv->n.ob[2] = 200;
                    sv->n.tc[0] = 0;
                    sv->n.tc[1] = shadow_calc_left;

                    sv++;

                    sv->n.ob[0] = shadow_edge_left;
                    sv->n.ob[1] = shadow_alt_left;
                    sv->n.ob[2] = -200;
                    sv->n.tc[0] = 1984;
                    sv->n.tc[1] = shadow_calc_left;

                    sv++;

                    sv->n.ob[0] = shadow_edge_right;
                    sv->n.ob[1] = shadow_alt_right;
                    sv->n.ob[2] = 200;
                    sv->n.tc[0] = 0;
                    sv->n.tc[1] = shadow_calc_right;

                    sv++;

                    sv->n.ob[0] = shadow_edge_right;
                    sv->n.ob[1] = shadow_alt_right;
                    sv->n.ob[2] = -200;
                    sv->n.tc[0] = 1984;
                    sv->n.tc[1] = shadow_calc_right;

                    if (gfx_vertex_num >= 6)
                    {
                        sv++;

                        sv->n.ob[0] = spE8;
                        sv->n.ob[1] = spE4;
                        sv->n.ob[2] = 200;
                        sv->n.tc[0] = 0;

                        if (spF0 < 0.0F)
                        {
                            spF0 = 0.0F;
                        }
                        if (spF0 > 2048.0F)
                        {
                            spF0 = 2048.0F;
                        }
                        sv->n.tc[1] = spF0;

                        sv++;

                        sv->n.ob[0] = spE8;
                        sv->n.ob[1] = spE4;
                        sv->n.ob[2] = -200;
                        sv->n.tc[0] = 1984;
                        sv->n.tc[1] = spF0;

                        if (gfx_vertex_num == 8)
                        {
                            sv++;

                            sv->n.ob[0] = spE0;
                            sv->n.ob[1] = spDC;
                            sv->n.ob[2] = 200;
                            sv->n.tc[0] = 0;

                            if (spEC < 0.0F)
                            {
                                spEC = 0.0F;
                            }
                            if (spEC > 2048.0F)
                            {
                                spEC = 2048.0F;
                            }
                            sv->n.tc[1] = spEC;

                            sv++;

                            sv->n.ob[0] = spE0;
                            sv->n.ob[1] = spDC;
                            sv->n.ob[2] = -200;
                            sv->n.tc[0] = 1984;
                            sv->n.tc[1] = spEC;

                            gSPVertex(gSYTaskmanDLHeads[0]++, shadow_vertex, 8, 0);

                            if (edge_left_or_right != 0)
                            {
                                gSP2Triangles(gSYTaskmanDLHeads[0]++, 1, 0, 7, 0, 0, 6, 7, 0);
                                gSP2Triangles(gSYTaskmanDLHeads[0]++, 7, 6, 5, 0, 6, 4, 5, 0);
                                gSP2Triangles(gSYTaskmanDLHeads[0]++, 5, 4, 3, 0, 4, 2, 3, 0);
                            }
                            else
                            {
                                gSP2Triangles(gSYTaskmanDLHeads[0]++, 1, 0, 5, 0, 0, 4, 5, 0);
                                gSP2Triangles(gSYTaskmanDLHeads[0]++, 5, 4, 7, 0, 4, 6, 7, 0);
                                gSP2Triangles(gSYTaskmanDLHeads[0]++, 7, 6, 3, 0, 6, 2, 3, 0);
                            }
                        }
                        else
                        {
                            gSPVertex(gSYTaskmanDLHeads[0]++, shadow_vertex, 6, 0);
                            gSP2Triangles(gSYTaskmanDLHeads[0]++, 1, 0, 5, 0, 0, 4, 5, 0);
                            gSP2Triangles(gSYTaskmanDLHeads[0]++, 5, 4, 3, 0, 4, 2, 3, 0);
                        }
                    }
                    else
                    {
                        gSPVertex(gSYTaskmanDLHeads[0]++, shadow_vertex, 4, 0);
                        gSP2Triangles(gSYTaskmanDLHeads[0]++, 1, 0, 3, 0, 0, 2, 3, 0);
                    }
                }
            }
        }
    }
    if (shadow_gobj->dl_link_next == NULL)
    {
        gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTShadowNoNextLinkDL);
    }
}

// 0x8013BB88
GObj* ftShadowMakeShadow(GObj *fighter_gobj)
{
    GObj *shadow_gobj = gcMakeGObjSPAfter(nGCCommonKindShadow, NULL, nGCCommonLinkIDShadow, GOBJ_PRIORITY_DEFAULT);
    FTStruct *fp;
    FTShadow *fs = syTaskmanMalloc(sizeof(FTShadow), 0x8);
    s32 i;

    if (fs == NULL) 
    {
        return NULL;
    }
    fp = ftGetStruct(fighter_gobj);

    fs->player = fp->player;

    for (i = 0; i < (ARRAY_COUNT(fs->vtx1) + ARRAY_COUNT(fs->vtx2)) / 2; i++)
    {
        fs->vtx1[i].n.flag = 0;
        fs->vtx1[i].n.n[0] = 0;
        fs->vtx1[i].n.n[1] = 0;
        fs->vtx1[i].n.n[2] = 0;
        fs->vtx1[i].n.a = 128;
        fs->vtx2[i].n.flag = 0;
        fs->vtx2[i].n.n[0] = 0;
        fs->vtx2[i].n.n[1] = 0;
        fs->vtx2[i].n.n[2] = 0;
        fs->vtx2[i].n.a = 128;
    }
    shadow_gobj->user_data.p = fs;

    gcAddGObjDisplay(shadow_gobj, ftShadowProcDisplay, 7, GOBJ_PRIORITY_DEFAULT, ~0);

    return shadow_gobj;
}
