#include <ft/fighter.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/malloc.h>
#include <sys/matrix.h>
#include <sys/develop.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern SYMallocRegion gSYTaskmanGraphicsHeap;

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x801312F0
u8 sFTDisplayMainSkyFogAlpha;

// 0x801312F1
ub8 sFTDisplayMainIsShadeFog;

// 0x801312F4
SYColorRGBA sFTDisplayMainFogColor;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012B930
Vec2f dFTDisplayMainShufflePositions[/* */][4] = 
{
    // Non-electric
    { 
        {   0.0F, -100.0F }, 
        {   0.0F,  -50.0F },
        {   0.0F,  100.0F }, 
        {   0.0F,   50.0F } 
    },

    // Electric
    { 
        {   0.0F,  -20.0F }, 
        {  15.0F,    5.0F },
        { -15.0F,    5.0F }, 
        {   0.0F,    0.0F } 
    }
};

// 0x8012B970
s32 dFTDisplayMainPad0x8012B970[2] = { 0, 0 };

/* 
 * 0x8012B978 - use xxd -i in Linux to convert your collision box texture into an unsigned char[] array;
 * I went out of my way to convert them into unsigned short[] instead, but as long as the bytes match,
 * it doesn't really matter what the datatype is, as the gbi macro doesn't care.
 * Update: might include the u16 array version.
 */
u16 dFTDisplayMainCollisionTexture[/* */] = 
{
    #include <ft/ftcollision.txt>
};

// 0x8012BB78
Vtx_tn dFTDisplayMainCollisionCuboidVtx[/* */] =
{
    { { -15, 15, 15 }, 0, { -16, 512 }, { 0, 0, 127 }, 0xFF },
    { { -15, -15, 15 }, 0, { -16, -16 }, { 0, 0, 127 }, 0xFF },
    { { 15, -15, 15 }, 0, { 512, -16 }, { 0, 0, 127 }, 0xFF },
    { { 15, 15, 15 }, 0, { 512, 512 }, { 0, 0, 127 }, 0xFF },
    { { -15, 15, -15 }, 0, { -16, 512 }, { 0, 127, 0 }, 0xFF },
    { { -15, 15, 15 }, 0, { -16, -16 }, { 0, 127, 0 }, 0xFF },
    { { 15, 15, 15 }, 0, { 512, -16 }, { 0, 127, 0 }, 0xFF },
    { { 15, 15, -15 }, 0, { 512, 512 }, { 0, 127, 0 }, 0xFF },
    { { -15, -15, -15 }, 0, { -16, 512 }, { 0, 0, -127 }, 0xFF },
    { { -15, 15, -15 }, 0, { -16, -16 }, { 0, 0, -127 }, 0xFF },
    { { 15, 15, -15 }, 0, { 512, -16 }, { 0, 0, -127 }, 0xFF },
    { { 15, -15, -15 }, 0, { 512, 512 }, { 0, 0, -127 }, 0xFF },
    { { -15, -15, 15 }, 0, { -16, 512 }, { 0, -127, 0 }, 0xFF },
    { { -15, -15, -15 }, 0, { -16, -16 }, { 0, -127, 0 }, 0xFF },
    { { 15, -15, -15 }, 0, { 512, -16 }, { 0, -127, 0 }, 0xFF },
    { { 15, -15, 15 }, 0, { 512, 512 }, { 0, -127, 0 }, 0xFF },
    { { 15, 15, 15 }, 0, { -16, 512 }, { 127, 0, 0 }, 0xFF },
    { { 15, -15, 15 }, 0, { -16, -16 }, { 127, 0, 0 }, 0xFF },
    { { 15, -15, -15 }, 0, { 512, -16 }, { 127, 0, 0 }, 0xFF },
    { { 15, 15, -15 }, 0, { 512, 512 }, { 127, 0, 0 }, 0xFF },
    { { -15, 15, -15 }, 0, { -16, 512 }, { -128, 0, 0 }, 0xFF },
    { { -15, -15, -15 }, 0, { -16, -16 }, { -128, 0, 0 }, 0xFF },
    { { -15, -15, 15 }, 0, { 512, -16 }, { -128, 0, 0 }, 0xFF },
    { { -15, 15, 15 }, 0, { 512, 512 }, { -128, 0, 0 }, 0xFF }
};

// 0x8012BCF8
Vtx_t dFTDisplayMainCollisionEdgeVtx[/* */] = 
{ 
    { { -15, 15, 15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, 15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, 15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, 15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, -15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, 15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, 15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, -15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, -15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, -15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, -15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, -15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, 15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, -15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, -15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, 15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, 15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, 15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, -15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, -15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, -15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, -15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, 15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, 15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BE78
Vtx_t dFTDisplayMainCollisionCubeCommonVtx0[/* */] = 
{
    { { -15, 15, 15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, 15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, 15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, 15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BEB8
Vtx_t dFTDisplayMainCollisionCubeCommonVtx1[/* */] = 
{ 
    { { -15, 15, -15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, 15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, 15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, -15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BEF8
Vtx_t dFTDisplayMainCollisionCubeCommonVtx2[/* */] =
{ 
    { { -15, -15, -15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, -15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, -15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, -15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BF38
Vtx_t dFTDisplayMainCollisionCubeCommonVtx3[/* */] = 
{
    { { -15, -15, 15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, -15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, -15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, 15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BF78
Vtx_t dFTDisplayMainCollisionCubeCommonVtx4[/* */] = 
{
    { { 15, 15, 15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, 15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, -15, -15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 15, 15, -15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BFB8
Vtx_t dFTDisplayMainCollisionCubeCommonVtx5[/* */] = 
{
    { { -15, 15, -15 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, -15 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, -15, 15 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { -15, 15, 15 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012BFF8
Vtx_t dFTDisplayMainMapCollisionBottomVtx[/* */] = 
{
    { { -30, 30, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 0, 0, 0 }, 0, { 512, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 30, 30, 0 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012C028
Vtx_t dFTDisplayMainMapCollisionTopVtx[/* */] =
{
    { { -30, 0, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 30, 0, 0 }, 0, { 512, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } },
    { { 0, 30, 0 }, 0, { -16, 512 }, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

// 0x8012C058
Gfx dFTDisplayMainHurtCollisionCuboidDL[/* */] =
{
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),
    gsSPLightColor(LIGHT_1, GPACK_RGBA8888(0x40, 0x40, 0x40, 0x00)),
    gsSPLightColor(LIGHT_2, GPACK_RGBA8888(0xC0, 0xC0, 0xC0, 0x00)),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, dFTDisplayMainCollisionTexture),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 255, 512),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 4, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x003C, 0x003C),
    gsSPVertex(dFTDisplayMainCollisionCuboidVtx, ARRAY_COUNT(dFTDisplayMainCollisionCuboidVtx), 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 4, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 8, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 12, 14, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 16, 18, 0),
    gsSP2Triangles(20, 21, 22, 0, 23, 20, 22, 0),
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList()
};

// 0x8012C128
Gfx dFTDisplayMainHitCollisionEdgeDL[/* */] =
{
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK),
    gsSPVertex(dFTDisplayMainCollisionEdgeVtx, ARRAY_COUNT(dFTDisplayMainCollisionEdgeVtx), 0),
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList()
};

// 0x8012C160
Gfx dFTDisplayMainHitCollisionBlendDL[/* */] =
{
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK),
    gsDPSetCombineMode(G_CC_BLENDPEDECALA, G_CC_BLENDPEDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, dFTDisplayMainCollisionTexture),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 255, 512),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 4, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x003C, 0x003C),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 4, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 8, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 12, 14, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 16, 18, 0),
    gsSP2Triangles(20, 21, 22, 0, 23, 20, 22, 0),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx0, ARRAY_COUNT(dFTDisplayMainCollisionCubeCommonVtx0), 24),
    gsSP2Triangles(0, 24, 27, 0, 3, 0, 27, 0),
    gsSP2Triangles(1, 25, 26, 0, 2, 1, 26, 0),
    gsSP2Triangles(27, 24, 0, 0, 27, 0, 3, 0),
    gsSP2Triangles(26, 25, 1, 0, 26, 1, 2, 0),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx1, ARRAY_COUNT(dFTDisplayMainCollisionCubeCommonVtx1), 24),
    gsSP2Triangles(5, 25, 24, 0, 4, 5, 24, 0),
    gsSP2Triangles(6, 26, 27, 0, 7, 6, 27, 0),
    gsSP2Triangles(24, 25, 5, 0, 24, 5, 4, 0),
    gsSP2Triangles(27, 26, 6, 0, 27, 6, 7, 0),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx2, ARRAY_COUNT(dFTDisplayMainCollisionCubeCommonVtx2), 24),
    gsSP2Triangles(8, 24, 27, 0, 11, 8, 27, 0),
    gsSP2Triangles(9, 25, 26, 0, 10, 9, 26, 0),
    gsSP2Triangles(27, 24, 8, 0, 27, 8, 11, 0),
    gsSP2Triangles(26, 25, 9, 0, 26, 9, 10, 0),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx3, ARRAY_COUNT(dFTDisplayMainCollisionCubeCommonVtx3), 24),
    gsSP2Triangles(13, 25, 24, 0, 12, 13, 24, 0),
    gsSP2Triangles(14, 26, 27, 0, 15, 14, 27, 0),
    gsSP2Triangles(24, 25, 13, 0, 24, 13, 12, 0),
    gsSP2Triangles(27, 26, 14, 0, 27, 14, 15, 0),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx4, ARRAY_COUNT(dFTDisplayMainCollisionCubeCommonVtx4), 24),
    gsSP2Triangles(17, 25, 24, 0, 16, 17, 24, 0),
    gsSP2Triangles(18, 26, 27, 0, 19, 18, 27, 0),
    gsSP2Triangles(24, 25, 17, 0, 24, 17, 16, 0),
    gsSP2Triangles(27, 26, 18, 0, 27, 18, 19, 0),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx5, ARRAY_COUNT(dFTDisplayMainCollisionCubeCommonVtx5), 24),
    gsSP2Triangles(21, 25, 24, 0, 20, 21, 24, 0),
    gsSP2Triangles(22, 26, 27, 0, 23, 22, 27, 0),
    gsSP2Triangles(24, 25, 21, 0, 24, 21, 20, 0),
    gsSP2Triangles(27, 26, 22, 0, 27, 22, 23, 0),
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList()
};

// 0x8012C310
Gfx dFTDisplayMainHitCollisionCubeDL[/* */] =
{
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK),
    gsDPSetCombineMode(G_CC_BLENDPEDECALA, G_CC_BLENDPEDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, dFTDisplayMainCollisionTexture),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 255, 512),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 4, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x003C, 0x003C),
    gsSPVertex(dFTDisplayMainCollisionCubeCommonVtx0, 24, 0), // WARNING: Reads 24 vertices in a row, most of which are out of the bounds of this array!
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 4, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 8, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 12, 14, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 16, 18, 0),
    gsSP2Triangles(20, 21, 22, 0, 23, 20, 22, 0),
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList()
};

// 0x8012C3D8
Gfx dFTDisplayMainMapCollisionBottomDL[/* */] =
{
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING | G_SHADING_SMOOTH),
    gsDPSetPrimColor(0, 0, 0xFF, 0xFF, 0xFF, 0xAC),
    gsDPSetEnvColor(0xB0, 0x00, 0xB0, 0xFF),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, PRIMITIVE),
    gsDPSetRenderMode(AA_EN | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_OPA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_OPA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureImage(G_IM_FMT_IA, G_IM_SIZ_16b, 1, dFTDisplayMainCollisionTexture),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 255, 512),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_16b, 4, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x003C, 0x003C),
    gsSPVertex(dFTDisplayMainMapCollisionBottomVtx, ARRAY_COUNT(dFTDisplayMainMapCollisionBottomVtx), 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList()
};

// 0x8012C458
Gfx dFTDisplayMainMapCollisionTopDL[/* */] =
{
    gsDPPipeSync(),
    gsSPVertex(dFTDisplayMainMapCollisionTopVtx, ARRAY_COUNT(dFTDisplayMainMapCollisionTopVtx), 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADING_SMOOTH),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList()
};

// 0x8012C490
Gfx dFTDisplayMainAfterImageVertexDL[/* */] =
{
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPClearGeometryMode(G_CULL_BOTH | G_LIGHTING),
    gsSPEndDisplayList()
};

// 0x8012C4B0
Gfx dFTDisplayMainAfterImageTriangleDL[/* */] =
{
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList()
};

// 0x8012C4C8
SYColorRGBA dFTDisplayMainDefaultAfterImageColor1 = { 0x00, 0xFF, 0xFF, 0x00 };

// 0x8012C4CC
SYColorRGBA dFTDisplayMainDefaultAfterImageColor2 = { 0xFF, 0xFF, 0xFF, 0x00 };

// 0x8012C4D0
SYColorRGBA dFTDisplayMainItemAfterImageColor1 = { 0xFF, 0x40, 0xC0, 0x00 };

// 0x8012C4D4
SYColorRGBA dFTDisplayMainItemAfterImageColor2 = { 0xFF, 0xFF, 0xFF, 0x00 };

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800F1020
void ftDisplayMainDrawAfterImage(FTStruct *fp)
{
    s32 i, j;
    s32 next_index;
    s32 index;
    Vtx *base_p_vtx;
    Vtx *p_vtx;
    s32 vtx_count;
    s32 add_alpha;
    s32 base_alpha;
    f32 var_f20;
    f32 var_f22;
    f32 rotate;
    SYColorRGBA *color1, *color2;
    Gfx *vtx_dl, *tri_dl;
    FTAfterImage *afterimage;
    Vec3f spC8;
    FTAfterImage *next_afterimage;
    s32 alpha;
    f32 scale;
    s32 alphainc;
    Vec3f spAC;

    index = fp->afterimage.desc_id;

    switch (fp->afterimage.is_itemswing)
    {
    case FALSE:
        var_f20 = 50.0F;
        var_f22 = 250.0F;

        add_alpha = 0;
        base_alpha = 0xFF;

        rotate = F_CLC_DTOR32(30.0F); // 0.5235988F

        color1 = &dFTDisplayMainDefaultAfterImageColor1;
        color2 = &dFTDisplayMainDefaultAfterImageColor2;

        vtx_dl = dFTDisplayMainAfterImageVertexDL;
        tri_dl = dFTDisplayMainAfterImageTriangleDL;
        break;

    case TRUE:
        var_f20 = 80.0F;
        var_f22 = 580.0F;

        add_alpha = 0;
        base_alpha = 0xFF;

        rotate = F_CLC_DTOR32(30.0F); // 0.5235988F

        color1 = &dFTDisplayMainItemAfterImageColor1;
        color2 = &dFTDisplayMainItemAfterImageColor2;

        vtx_dl = dFTDisplayMainAfterImageVertexDL;
        tri_dl = dFTDisplayMainAfterImageTriangleDL;
        break;
    }
    base_p_vtx = p_vtx = (Vtx*)gSYTaskmanGraphicsHeap.ptr;

    if (index != 0)
    {
        index = index - 1;
    }
    else index = ARRAY_COUNT(fp->afterimage.desc) - 1;

    for (i = fp->afterimage.drawstatus - 1; i >= 0; index = next_index, i--)
    {
        afterimage = &fp->afterimage.desc[index];

        alpha = (((base_alpha - add_alpha) / (fp->afterimage.drawstatus - 1)) * i) + add_alpha;

        p_vtx->v.ob[0] = (afterimage->translate_x + (afterimage->vec.x * var_f20));
        p_vtx->v.ob[1] = (afterimage->translate_y + (afterimage->vec.y * var_f20));
        p_vtx->v.ob[2] = (afterimage->translate_z + (afterimage->vec.z * var_f20));

        p_vtx->v.flag = 0;

        p_vtx->v.tc[0] = p_vtx->v.tc[1] = 0;

        p_vtx->v.cn[0] = color1->r;
        p_vtx->v.cn[1] = color1->g;
        p_vtx->v.cn[2] = color1->b;
        p_vtx->v.cn[3] = alpha;

        p_vtx = (gSYTaskmanGraphicsHeap.ptr = (Vtx*)gSYTaskmanGraphicsHeap.ptr + 1);

        p_vtx->v.ob[0] = (afterimage->translate_x + (afterimage->vec.x * var_f22));
        p_vtx->v.ob[1] = (afterimage->translate_y + (afterimage->vec.y * var_f22));
        p_vtx->v.ob[2] = (afterimage->translate_z + (afterimage->vec.z * var_f22));

        p_vtx->v.flag = 0;

        p_vtx->v.tc[0] = p_vtx->v.tc[1] = 0;

        p_vtx->v.cn[0] = color2->r;
        p_vtx->v.cn[1] = color2->g;
        p_vtx->v.cn[2] = color2->b;
        p_vtx->v.cn[3] = alpha;

        p_vtx = (gSYTaskmanGraphicsHeap.ptr = (Vtx*)gSYTaskmanGraphicsHeap.ptr + 1);

        if (i != 0)
        {
            if (index != 0)
            {
                next_index = index - 1;
            }
            else next_index = ARRAY_COUNT(fp->afterimage.desc) - 1;

            next_afterimage = &fp->afterimage.desc[next_index];

            if (syVectorNormCross3D(&afterimage->vec, &next_afterimage->vec, &spC8) != NULL)
            {
                f32 f_angle_diff = syVectorAngleDiff3D(&afterimage->vec, &next_afterimage->vec);
                s32 target_angle = f_angle_diff / rotate;

                if (target_angle != 0)
                {
                    s16 n_ai_x;
                    s16 vtx_x;
                    s16 vtx_y;
                    s16 vtx_z;
                    s16 n_ai_y;
                    s16 n_ai_z;

                    target_angle++;

                    scale = 1.0F / (target_angle);

                    n_ai_x = afterimage->translate_x;
                    n_ai_y = afterimage->translate_y;
                    n_ai_z = afterimage->translate_z;

                    f_angle_diff *= scale;

                    vtx_x = ((next_afterimage->translate_x - n_ai_x) * scale);
                    vtx_y = ((next_afterimage->translate_y - n_ai_y) * scale);
                    vtx_z = ((next_afterimage->translate_z - n_ai_z) * scale);

                    spAC = afterimage->vec;

                    alphainc = (((((base_alpha - add_alpha) / (fp->afterimage.drawstatus - 1)) * (i - 1)) + add_alpha) - alpha) * scale;

                    for (j = 0; j < target_angle - 1; j++)
                    {
                        n_ai_x += vtx_x;
                        n_ai_y += vtx_y;
                        n_ai_z += vtx_z;

                        syVectorRotateAbout3D(&spAC, &spC8, f_angle_diff);

                        alpha += alphainc;

                        p_vtx->v.ob[0] = (n_ai_x + (spAC.x * var_f20));
                        p_vtx->v.ob[1] = (n_ai_y + (spAC.y * var_f20));
                        p_vtx->v.ob[2] = (n_ai_z + (spAC.z * var_f20));

                        p_vtx->v.flag = 0;

                        p_vtx->v.tc[0] = p_vtx->v.tc[1] = 0;

                        p_vtx->v.cn[0] = color1->r;
                        p_vtx->v.cn[1] = color1->g;
                        p_vtx->v.cn[2] = color1->b;
                        p_vtx->v.cn[3] = alpha;

                        p_vtx = (gSYTaskmanGraphicsHeap.ptr = (Vtx*)gSYTaskmanGraphicsHeap.ptr + 1);

                        p_vtx->v.ob[0] = (n_ai_x + (spAC.x * var_f22));
                        p_vtx->v.ob[1] = (n_ai_y + (spAC.y * var_f22));
                        p_vtx->v.ob[2] = (n_ai_z + (spAC.z * var_f22));

                        p_vtx->v.flag = 0;

                        p_vtx->v.tc[0] = p_vtx->v.tc[1] = 0;

                        p_vtx->v.cn[0] = color2->r;
                        p_vtx->v.cn[1] = color2->g;
                        p_vtx->v.cn[2] = color2->b;
                        p_vtx->v.cn[3] = alpha;

                        p_vtx = (gSYTaskmanGraphicsHeap.ptr = (Vtx*)gSYTaskmanGraphicsHeap.ptr + 1);
                    }
                }
            }
        }
    }
    vtx_count = ((uintptr_t)p_vtx - (uintptr_t)base_p_vtx) / (sizeof(*p_vtx) | sizeof(*base_p_vtx));

    gSPDisplayList(gSYTaskmanDLHeads[1]++, vtx_dl);
    gSPVertex(gSYTaskmanDLHeads[1]++, base_p_vtx, vtx_count, 0);

    for (i = 0; i < (vtx_count - 2); i += 2)
    {
        gSP2Triangles(gSYTaskmanDLHeads[1]++, i, i + 1, i + 2, FALSE, i + 1, i + 3, i + 2, FALSE);
    }
    gSPDisplayList(gSYTaskmanDLHeads[1]++, tri_dl);
}

// 0x800F17E8
void ftDisplayMainCalcFogColor(FTStruct *fp)
{
    s32 shade;
    s32 temp_color;
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 shade_base;

    if (fp->shade == 0)
    {
        red = fp->colanim.color1.r;
        green = fp->colanim.color1.g;
        blue = fp->colanim.color1.b;
        alpha = fp->colanim.color1.a;
    }
    else
    {
        SYColorRGBA *attr_shade_color = &fp->attr->shade_color[fp->shade - 1];
        GMColKeys *ck = &fp->colanim.color1;

        shade_base = (((0xFF - attr_shade_color->a) * (0xFF - ck->a)) / 0xFF);

        if (shade_base == 0xFF)
        {
            red = fp->colanim.color1.r;
            green = fp->colanim.color1.g;
            blue = fp->colanim.color1.b;
            alpha = fp->colanim.color1.a;
        }
        else
        {
            shade_base = (0xFF - shade_base);

            temp_color = (((ck->r - fp->shade_color.r) * ck->a) / 0xFF) + fp->shade_color.r;

            red = (temp_color * 0xFF) / shade_base;

            if (red != 0)
            {
                alpha = ((temp_color * 0xFF) / red);
            }
            else alpha = ((shade_base - temp_color) * 0xFF) / 0xFF;

            temp_color = (((ck->g - fp->shade_color.g) * ck->a) / 0xFF) + fp->shade_color.g;

            green = (temp_color * 0xFF) / shade_base;

            temp_color = (((ck->b - fp->shade_color.b) * ck->a) / 0xFF) + fp->shade_color.b;

            blue = (temp_color * 0xFF) / shade_base;
        }
    }

    if (fp->attr->fog_color.r != 0xFF)
    {
        alpha = (fp->attr->fog_color.r * alpha) / 0xFF;
    }
    sFTDisplayMainFogColor.r = red;
    sFTDisplayMainFogColor.g = green;
    sFTDisplayMainFogColor.b = blue;
    sFTDisplayMainFogColor.a = alpha;
}

// 0x800F1B24
void ftDisplayMainSetFogColor(FTStruct *fp)
{
    gDPSetFogColor(gSYTaskmanDLHeads[0]++, sFTDisplayMainFogColor.r, sFTDisplayMainFogColor.g, sFTDisplayMainFogColor.b, sFTDisplayMainFogColor.a);
}

// 0x800F1B7C
void ftDisplayMainDecideFogColor(FTStruct *fp)
{
    if (fp->shade == 0)
    {
        gDPSetFogColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, 0x00);
    }
    else
    {
        SYColorRGBA *fog_color = &fp->attr->shade_color[fp->shade - 1];

        gDPSetFogColor(gSYTaskmanDLHeads[0]++, fog_color->r, fog_color->g, fog_color->b, fog_color->a);
    }
}

// 0x800F1C08
void ftDisplayMainDecideFogDraw(u8 flags, FTStruct *fp)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);

    if (flags & FTPARTS_FLAG_NOFOG)
    {
        gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_PASS, G_RM_AA_ZB_OPA_SURF2);
    }
    else
    {
        if (fp->colanim.is_use_color1)
        {
            if (flags & FTPARTS_FLAG_TOGGLEFOG)
            {
                if (sFTDisplayMainIsShadeFog == FALSE)
                {
                    sFTDisplayMainIsShadeFog = TRUE;

                    ftDisplayMainDecideFogColor(fp);
                }
            }
            else if (sFTDisplayMainIsShadeFog != FALSE)
            {
                sFTDisplayMainIsShadeFog = FALSE;

                ftDisplayMainSetFogColor(fp);
            }
        }
        if (sFTDisplayMainSkyFogAlpha == 0xFF)
        {
            gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_FOG_PRIM_A, G_RM_AA_ZB_OPA_SURF2);
        }
        else gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_FOG_PRIM_A, G_RM_AA_ZB_XLU_SURF2);
    }
}

// 0x800F1D44
void ftDisplayMainDrawAccessory(FTStruct *fp, DObj *dobj, FTParts *parts)
{
    DObj *root_dobj = DObjGetStruct(parts->gobj);

    switch (parts->flags & 0xF)
    {
    case 0:
        if ((dobj->dv != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gcDrawMObjForDObj(root_dobj, gSYTaskmanDLHeads);
            ftDisplayMainDecideFogDraw(parts->flags, fp);

            gSPDisplayList(gSYTaskmanDLHeads[0]++, root_dobj->dl);
        }
        break;

    case 1:
        if ((dobj->dls != NULL) && (dobj->dls[1] != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
        {
            gcDrawMObjForDObj(root_dobj, gSYTaskmanDLHeads);
            ftDisplayMainDecideFogDraw(parts->flags, fp);

            gSPDisplayList(gSYTaskmanDLHeads[0]++, root_dobj->dl);
        }
        break;
    }
}

// 0x800F1E60
void ftDisplayMainDrawDefault(DObj *dobj)
{
    FTStruct *fp = ftGetStruct(dobj->parent_gobj);
    s32 sp58;
    s32 unused;
    Vec3f sp48;
    FTParts *parts;
    DObj *sibling_dobj;
    Gfx **dls;

    parts = ftGetParts(dobj);

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        sp48 = gLBCommonScale;

        if (parts != NULL)
        {
            switch (parts->flags & 0xF)
            {
            case 0:
                sp58 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

                if ((parts != NULL) && (parts->gobj != NULL) && (fp->fkind == nFTKindPurin))
                {
                    ftDisplayMainDrawAccessory(fp, dobj, parts);
                }
                if ((dobj->dv != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
                {
                    gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                    ftDisplayMainDecideFogDraw(parts->flags, fp);

                    gSPDisplayList(gSYTaskmanDLHeads[0]++, dobj->dl);
                }
                break;

            case 1:
                dls = dobj->dls;

                if ((dls != NULL) && (dls[0] != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
                {
                    ftDisplayMainDecideFogDraw(parts->flags, fp);

                    gSPDisplayList(gSYTaskmanDLHeads[0]++, dls[0]);
                }
                sp58 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

                if ((dls != NULL) && (dls[1] != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
                {
                    gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                    ftDisplayMainDecideFogDraw(parts->flags, fp);

                    gSPDisplayList(gSYTaskmanDLHeads[0]++, dls[1]);
                }
                break;
            }
        }
        else
        {
            sp58 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

            if ((parts != NULL) && (parts->gobj != NULL) && (fp->fkind == nFTKindPurin))
            {
                ftDisplayMainDrawAccessory(fp, dobj, parts);
            }
        }
        if ((parts != NULL) && (parts->gobj != NULL) && (fp->fkind == nFTKindPikachu))
        {
            ftDisplayMainDrawAccessory(fp, dobj, parts);
        }
        if (dobj->child != NULL)
        {
            ftDisplayMainDrawDefault(dobj->child);
        }
        if ((sp58 != FALSE) && ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL)))
        {
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
        gLBCommonScale = sp48;
    }
    if (dobj->sib_prev == NULL)
    {
        sibling_dobj = dobj->sib_next;

        while (sibling_dobj != NULL)
        {
            ftDisplayMainDrawDefault(sibling_dobj);

            sibling_dobj = sibling_dobj->sib_next;
        }
    }
}

// 0x800F21B4
void ftDisplayMainDrawSkeleton(DObj *dobj)
{
    FTStruct *fp;
    s32 sp60;
    s32 unused;
    Vec3f sp50;
    Gfx **dls;
    FTParts *parts;
    DObj *sibling_dobj;
    FTSkeleton *skeleton;

    fp = ftGetStruct(dobj->parent_gobj);
    parts = ftGetParts(dobj);

    if (!(dobj->flags & DOBJ_FLAG_HIDDEN))
    {
        sp50 = gLBCommonScale;

        if ((parts != NULL) && (parts->joint_id >= nFTPartsJointCommonStart))
        {
            skeleton = &fp->attr->skeleton[fp->colanim.skeleton_id][parts->joint_id - nFTPartsJointCommonStart];

            switch (skeleton->flags & 0xF)
            {
            case 0:
                sp60 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

                if (!(dobj->flags & DOBJ_FLAG_NOTEXTURE) && (skeleton->dl != NULL))
                {
                    gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                    ftDisplayMainDecideFogDraw(skeleton->flags, fp);

                    gSPDisplayList(gSYTaskmanDLHeads[0]++, skeleton->dl);
                }
                break;

            case 1:
                dls = skeleton->dls;

                if ((dls != NULL) && (dls[0] != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
                {
                    ftDisplayMainDecideFogDraw(skeleton->flags, fp);

                    gSPDisplayList(gSYTaskmanDLHeads[0]++, dls[0]);
                }
                sp60 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

                if ((dls != NULL) && (dls[1] != NULL) && !(dobj->flags & DOBJ_FLAG_NOTEXTURE))
                {
                    gcDrawMObjForDObj(dobj, gSYTaskmanDLHeads);
                    ftDisplayMainDecideFogDraw(skeleton->flags, fp);

                    gSPDisplayList(gSYTaskmanDLHeads[0]++, dls[1]);
                }
                break;
            }
        }
        else sp60 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

        if (dobj->child != NULL)
        {
            ftDisplayMainDrawSkeleton(dobj->child);
        }
        if ((sp60 != FALSE) && ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL)))
        {
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
        gLBCommonScale = sp50;
    }
    if (dobj->sib_prev == NULL)
    {
        sibling_dobj = dobj->sib_next;

        while (sibling_dobj != NULL)
        {
            ftDisplayMainDrawSkeleton(sibling_dobj);
            sibling_dobj = sibling_dobj->sib_next;
        }
    }
}

// 0x800F24A0
void ftDisplayMainDrawAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if
    (
        (fp->colanim.skeleton_id)                           &&
        (attr->skeleton != NULL)                            &&
        (attr->skeleton[fp->colanim.skeleton_id] != NULL)   &&
        (fp->joints[(s32)(attr->skeleton[0])] != NULL)      &&  // ???
        (fp->joints[(s32)(attr->skeleton[0])]->dl != NULL)      // Hello???
    )
    {
        ftDisplayMainDrawSkeleton(DObjGetStruct(fighter_gobj));
    }
    else ftDisplayMainDrawDefault(DObjGetStruct(fighter_gobj));

    if (fp->afterimage.drawstatus >= 2)
    {
        switch (fp->afterimage.is_itemswing)
        {
        case FALSE:
            ftDisplayMainDrawAfterImage(fp);
            break;

        case TRUE:
            ftDisplayMainDrawAfterImage(fp);
            break;
        }
    }
}

// 0x800F2584
void ftDisplayMainDrawParts(DObj *dobj)
{
    FTStruct *fp;
    s32 sp90;
    s32 hitstatus;
    DObj *sibling_dobj;
    Mtx *m;
    s32 i;
    Vec3f sp74;

    fp = ftGetStruct(dobj->parent_gobj);

    sp74 = gLBCommonScale;

    sp90 = gcPrepDObjMatrix(gSYTaskmanDLHeads, dobj);

    for (i = 0; i < ARRAY_COUNT(fp->damage_colls); i++)
    {
        FTDamageColl *damage_coll = &fp->damage_colls[i];

        if ((damage_coll->hitstatus != nGMHitStatusNone) && (dobj == damage_coll->joint))
        {
            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixTra(m, damage_coll->offset.x, damage_coll->offset.y, damage_coll->offset.z);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixSca(m, damage_coll->size.x / 15.0F, damage_coll->size.y / 15.0F, damage_coll->size.z / 15.0F);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPPipeSync(gSYTaskmanDLHeads[0]++);

            if
            (
                (fp->special_hitstatus == nGMHitStatusIntangible)  ||
                (fp->star_hitstatus == nGMHitStatusIntangible)     ||
                (fp->hitstatus == nGMHitStatusIntangible)          ||
                (damage_coll->hitstatus == nGMHitStatusIntangible)
            )
            {
                hitstatus = nGMHitStatusIntangible;
            }
            else if
            (
                (fp->special_hitstatus == nGMHitStatusInvincible)  ||
                (fp->star_hitstatus == nGMHitStatusInvincible)     ||
                (fp->hitstatus == nGMHitStatusInvincible)          ||
                (damage_coll->hitstatus == nGMHitStatusInvincible)
            )
            {
                hitstatus = nGMHitStatusInvincible;
            }
            else hitstatus = nGMHitStatusNormal;

            switch (hitstatus)
            {
            case nGMHitStatusNormal:
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xD0, 0xD0, 0x00, 0xFF);
                break;

            case nGMHitStatusInvincible:
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0x00, 0xD0, 0x00, 0xFF);
                break;

            case nGMHitStatusIntangible:
                gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0xD0, 0xFF);
                break;
            }
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHurtCollisionCuboidDL);
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
            break;
        }
    }
    if (dobj->child != NULL)
    {
        ftDisplayMainDrawParts(dobj->child);
    }
    if (sp90 != FALSE)
    {
        if ((dobj->parent == DOBJ_PARENT_NULL) || (dobj->sib_next != NULL))
        {
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
    }
    gLBCommonScale = sp74;

    if (dobj->sib_prev == NULL)
    {
        sibling_dobj = dobj->sib_next;

        while (sibling_dobj != NULL)
        {
            ftDisplayMainDrawParts(sibling_dobj);

            sibling_dobj = sibling_dobj->sib_next;
        }
    }
}

// 0x800F293C - WARNING: Fake match. dist snaps to sp114, cannot make room on stack to align it.
void ftDisplayMainProcDisplay(GObj *fighter_gobj)
{
    FTStruct *fp;
    FTAttributes *attr;
    FTAttackColl *attack_coll;
    Mtx *m;
    s32 i;
    Vec3f ft_pos;
    f32 cam_pos_x;
    f32 cam_pos_y;
    Vec3f dist;

    fp = ftGetStruct(fighter_gobj);
    attr = fp->attr;

    sFTDisplayMainSkyFogAlpha = 0xFF;
    sFTDisplayMainIsShadeFog = FALSE;

    if ((fp->is_invisible) && (fp->display_mode == nDBDisplayModeMaster))
    {
        fp->is_magnify_show = FALSE;

        return;
    }
    if ((fp->pkind == nFTPlayerKindMan) || (fp->pkind == nFTPlayerKindCom) || (fp->pkind == nFTPlayerKindGameKey))
    {
        if (gGCCurrentCamera->id == nGCCommonKindMainCamera)
        {
            switch (fp->status_id)
            {
            case nFTCommonStatusDeadUpStar:
            case nFTCommonStatusDeadUpFall:
                fp->is_magnify_show = FALSE;
                break;

            default:
                ft_pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;
                ft_pos.y += fp->attr->cam_offset_y;

            #if defined(AVOID_UB) || defined(NON_MATCHING)
                syVectorDiff3D(&dist, &CObjGetStruct(gGMCameraGObj)->vec.at, &ft_pos);

                if (fp->attr->cam_offset_y < syVectorMag3D(&dist))
                {
                    syVectorNorm3D(&dist);
                    syVectorScale3D(&dist, fp->attr->cam_offset_y);
                    syVectorAdd3D(&ft_pos, &dist);
                }
            #else
                // SUPER FAKE. I hope I can fix this in the future. ft_pos - 2 should really be dist, but we get stack issues otherwise.
                syVectorDiff3D(&ft_pos - 2, &CObjGetStruct(gGMCameraGObj)->vec.at, &ft_pos);

                if (fp->attr->cam_offset_y < syVectorMag3D(&ft_pos - 2))
                {
                    syVectorNorm3D(&ft_pos - 2);
                    syVectorScale3D(&ft_pos - 2, fp->attr->cam_offset_y);
                    syVectorAdd3D(&ft_pos, &ft_pos - 2);
                }
            #endif

                func_ovl2_800EB924(CObjGetStruct(gGMCameraGObj), gGMCameraMatrix, &ft_pos, &cam_pos_x, &cam_pos_y);

                if (gmCameraCheckTargetInBounds(cam_pos_x, cam_pos_y) == FALSE)
                {
                    ft_pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;
                    ft_pos.y += 300.0F;

                    func_ovl2_800EB924(CObjGetStruct(gGMCameraGObj), gGMCameraMatrix, &ft_pos, &fp->magnify_pos.x, &fp->magnify_pos.y);

                    fp->is_magnify_show = TRUE;

                    if (gIFCommonPlayerInterface.is_magnify_display != FALSE)
                    {
                        if (!(fp->is_magnify_ignore) && !(fp->is_rebirth))
                        {
                            gIFCommonPlayerInterface.magnify_mode = 1;

                            ifCommonPlayerArrowsUpdateFlags(cam_pos_x, cam_pos_y);
                        }
                    }
                    return;
                }
                else fp->is_magnify_show = FALSE;

                break;
            }
        }
        else if (!(fp->is_magnify_ignore) && !(fp->is_rebirth) && (fp->is_magnify_show))
        {
            ifCommonPlayerMagnifyUpdateViewport(gSYTaskmanDLHeads, fp);
        }
        else return;
    }
    gLBCommonScale.x = gLBCommonScale.y = gLBCommonScale.z = 1.0F;

    if ((fp->display_mode == nDBDisplayModeMaster) || (fp->display_mode == nDBDisplayModeMapCollision))
    {
        gDPPipeSync(gSYTaskmanDLHeads[0]++);

        if (fp->colanim.is_use_light)
        {
            if (fp->pkind != nFTPlayerKindDemo)
            {
                ftDisplayLightsDrawReflect(gSYTaskmanDLHeads, fp->lr * fp->colanim.light_angle_x, fp->colanim.light_angle_y);
            }
            else ftDisplayLightsDrawReflect(gSYTaskmanDLHeads, F_CLC_RTOD32(DObjGetStruct(fighter_gobj)->rotate.vec.f.y) + fp->colanim.light_angle_x, fp->colanim.light_angle_y);
        }
        gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_2CYCLE);
        gSPSetGeometryMode(gSYTaskmanDLHeads[0]++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
        gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_FOG_PRIM_A, G_RM_AA_ZB_OPA_SURF2);

        if (fp->colanim.is_use_color2)
        {
            gDPSetEnvColor(gSYTaskmanDLHeads[0]++, fp->colanim.color2.r, fp->colanim.color2.g, fp->colanim.color2.b, fp->colanim.color2.a);

            sFTDisplayMainSkyFogAlpha = fp->colanim.color2.a;
        }
        else if (fp->is_use_fogcolor)
        {
            gDPSetEnvColor(gSYTaskmanDLHeads[0]++, fp->fog_color.r, fp->fog_color.g, fp->fog_color.b, fp->fog_color.a);

            sFTDisplayMainSkyFogAlpha = fp->fog_color.a;
        }
        else if (fp->pkind != nFTPlayerKindDemo)
        {
            sFTDisplayMainSkyFogAlpha = mpCollisionSetLightColorGetAlpha(gSYTaskmanDLHeads);
        }
        else sFTDisplayMainSkyFogAlpha = scSubsysFighterDrawLightColorGetAlpha(gSYTaskmanDLHeads);

        if (fp->colanim.is_use_color1)
        {
            ftDisplayMainCalcFogColor(fp);
            ftDisplayMainSetFogColor(fp);
        }
        else ftDisplayMainDecideFogColor(fp);

        if (fp->shuffle_tics != 0)
        {
            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixTra
            (
                m,
                dFTDisplayMainShufflePositions[fp->is_shuffle_electric][fp->shuffle_frame_index].x,
                dFTDisplayMainShufflePositions[fp->is_shuffle_electric][fp->shuffle_frame_index].y,
                0.0F
            );
            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        }
        if ((fp->pkind == nFTPlayerKindDemo) || (fp->pkind == nFTPlayerKindKey) || (gGCCurrentCamera->id == nGCCommonKindMainCamera))
        {
            ftDisplayMainDrawAll(fighter_gobj);
        }
        else
        {
            fp->joints[nFTPartsJointTopN]->xobjs[0]->kind = nGCMatrixKindRotRpyR;

            ftDisplayMainDrawAll(fighter_gobj);

            fp->joints[nFTPartsJointTopN]->xobjs[0]->kind = 0x4B;
        }
        if (fp->shuffle_tics != 0)
        {
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
        gDPPipeSync(gSYTaskmanDLHeads[0]++);
        gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
        gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

        if (fp->pkind != nFTPlayerKindDemo)
        {
            ftDisplayLightsDrawReflect(gSYTaskmanDLHeads, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
        }
        else ftDisplayLightsDrawReflect(gSYTaskmanDLHeads, scSubsysFighterGetLightAngleX(), scSubsysFighterGetLightAngleY());

        if (fp->display_mode == nDBDisplayModeMapCollision)
        {
            gDPPipeSync(gSYTaskmanDLHeads[0]++);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixTra
            (
                m,
                DObjGetStruct(fighter_gobj)->translate.vec.f.x,
                DObjGetStruct(fighter_gobj)->translate.vec.f.y + attr->map_coll.bottom,
                DObjGetStruct(fighter_gobj)->translate.vec.f.z
            );

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixSca(m, attr->map_coll.width / 30.0F, attr->map_coll.center / 30.0F, 1.0F);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainMapCollisionBottomDL);
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixTra
            (
                m,
                DObjGetStruct(fighter_gobj)->translate.vec.f.x,
                DObjGetStruct(fighter_gobj)->translate.vec.f.y + attr->map_coll.center,
                DObjGetStruct(fighter_gobj)->translate.vec.f.z
            );

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixSca(m, attr->map_coll.width / 30.0F, (attr->map_coll.top - attr->map_coll.center) / 30.0F, 1.0F);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainMapCollisionTopDL);
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            gDPPipeSync(gSYTaskmanDLHeads[0]++);

            syMatrixTra
            (
                m,
                DObjGetStruct(fighter_gobj)->translate.vec.f.x + (fp->coll_data.cliffcatch_coll.x * fp->lr),
                DObjGetStruct(fighter_gobj)->translate.vec.f.y + fp->coll_data.cliffcatch_coll.y,
                DObjGetStruct(fighter_gobj)->translate.vec.f.z
            );

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

            syMatrixSca(m, 3.0F, 3.0F, 3.0F);

            gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
            gDPSetEnvColor(gSYTaskmanDLHeads[0]++, 0xD0, 0x00, 0xD0, 0xFF);
            gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHurtCollisionCuboidDL);
            gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
        }
    }
    else
    {
        if (gGCCurrentCamera->id == nGCCommonKindMainCamera)
        {
            ftDisplayMainDrawParts(DObjGetStruct(fighter_gobj));
        }
        else
        {
            fp->joints[nFTPartsJointTopN]->xobjs[0]->kind = nGCMatrixKindRotRpyR;

            ftDisplayMainDrawParts(DObjGetStruct(fighter_gobj));

            fp->joints[nFTPartsJointTopN]->xobjs[0]->kind = 0x4B;
        }
        for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
        {
            attack_coll = &fp->attack_colls[i];

            if ((attack_coll->attack_state != nGMAttackStateOff) && (attack_coll->attack_state != nGMAttackStateNew))
            {
                gDPPipeSync(gSYTaskmanDLHeads[0]++);

                if (fp->display_mode == nDBDisplayModeHitAttackOutline)
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

                    syMatrixTra(m, attack_coll->pos_prev.x, attack_coll->pos_prev.y, attack_coll->pos_prev.z);

                    gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);

                    syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

                    syMatrixSca(m, attack_coll->size / 15.0F, attack_coll->size / 15.0F, attack_coll->size / 15.0F);

                    gSPMatrix(gSYTaskmanDLHeads[0]++, m, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                    gSPDisplayList(gSYTaskmanDLHeads[0]++, dFTDisplayMainHitCollisionEdgeDL);
                    gSPPopMatrix(gSYTaskmanDLHeads[0]++, G_MTX_MODELVIEW);
                }
                syMatrixAdvanceW(m, gSYTaskmanGraphicsHeap);

                syMatrixTra(m, attack_coll->pos_curr.x, attack_coll->pos_curr.y, attack_coll->pos_curr.z);

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
    if ((fp->pkind == nFTPlayerKindMan) || (fp->pkind == nFTPlayerKindCom) || (fp->pkind == nFTPlayerKindGameKey))
    {
        if (gGCCurrentCamera->id != nGCCommonKindMainCamera)
        {
            if (!(fp->is_magnify_ignore) && !(fp->is_rebirth) && (fp->is_magnify_show))
            {
                ifCommonPlayerMagnifyProcDisplay(fp);
            }
        }
    }
}
