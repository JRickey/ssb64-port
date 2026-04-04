#include "rdp.h"

#include "config.h"
#include <sys/video.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

void (*dSYRdpFuncLights)(Gfx**) = NULL;

Mtx dSYRdpIdentityMatrix = 
{
    {
        /* Integer Portion */
        {0x00010000, 0x00000000, 0x00000001, 0x00000000},
        {0x00000000, 0x00010000, 0x00000000, 0x00000001},
        /* Fractional Portion */
        {0x00000000, 0x00000000, 0x00000000, 0x00000000},
        {0x00000000, 0x00000000, 0x00000000, 0x00000000}
    }
};

Gfx sSYRdpResetDisplayList[/* */] = 
{
    gsDPPipeSync(),
    gsSPViewport(&gSYRdpViewport),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPClipRatio(FRUSTRATIO_1),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsSPMatrix(&dSYRdpIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION),
    gsSPMatrix(&dSYRdpIdentityMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetColorDither(G_CD_MAGICSQ),
    gsDPPipeSync(),
    gsSPEndDisplayList()
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

Vp gSYRdpViewport;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80007080
void syRdpSetViewport(Vp *viewport, f32 ulx, f32 uly, f32 lrx, f32 lry)
{
    f32 h = (ulx + lrx) / 2.0F;
    f32 v = (uly + lry) / 2.0F;

    viewport->vp.vscale[0] = ((s32) ((lrx - h) * 4.0F)) & 0xFFFF;
    viewport->vp.vscale[1] = ((s32) ((lry - v) * 4.0F)) & 0xFFFF;
    viewport->vp.vtrans[0] = ((s32) (h * 4.0F)) & 0xFFFF;
    viewport->vp.vtrans[1] = ((s32) (v * 4.0F)) & 0xFFFF;

    viewport->vp.vscale[2] = viewport->vp.vtrans[2] = G_MAXZ / 2;
}

void syRdpSetDefaultViewport(Vp *vp)
{
    vp->vp.vscale[0] = vp->vp.vtrans[0] = gSYVideoResWidth * 2;
    vp->vp.vscale[1] = vp->vp.vtrans[1] = gSYVideoResHeight * 2;
    vp->vp.vscale[2] = vp->vp.vtrans[2] = G_MAXZ / 2;
}

void syRdpSetFuncLights(void (*func_lights)(Gfx**)) 
{
    dSYRdpFuncLights = func_lights; // Actually processes lights?
}

void syRdpResetSettings(Gfx **dls)
{
    Gfx *dl = dls[0];

    gSPSegment(dl++, G_MWO_SEGMENT_0, 0x00000000);
    syTaskmanInitSegmentF(&dl);
    gDPSetDepthImage(dl++, gSYVideoZBuffer);
    syRdpSetDefaultViewport(&gSYRdpViewport);
    gSPDisplayList(dl++, sSYRdpResetDisplayList);

    gDPSetScissor
    (
        dl++,
        G_SC_NON_INTERLACE,
        10 * (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT),
        10 * (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT),
        gSYVideoResWidth - 10 * (gSYVideoResWidth / GS_SCREEN_WIDTH_DEFAULT),
        gSYVideoResHeight - 10 * (gSYVideoResHeight / GS_SCREEN_HEIGHT_DEFAULT)
    );
    if (dSYRdpFuncLights != NULL)
    { 
        dSYRdpFuncLights(&dl);
    }
    dls[0] = dl;
}
