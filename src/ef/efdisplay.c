#include <ef/effect.h>


// 0x800FCCC0
void efDisplayCLDProcDisplay(GObj *effect_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gDPSetAlphaCompare(gSYTaskmanDLHeads[1]++, G_AC_THRESHOLD);
    gDPSetBlendColor(gSYTaskmanDLHeads[1]++, 0x00, 0x00, 0x00, 0x08);
    gSPClearGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
}

// 0x800FCD64
void efDisplayXLUProcDisplay(GObj *effect_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[1]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[1]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
    gDPSetAlphaCompare(gSYTaskmanDLHeads[1]++, G_AC_NONE);
    gSPSetGeometryMode(gSYTaskmanDLHeads[1]++, G_ZBUFFER);
}

// 0x800FCDEC
void efDisplayMakeCLD(void)
{
    gcAddGObjDisplay(gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT), efDisplayCLDProcDisplay, 15, 3, ~0);
    gcAddGObjDisplay(gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT), efDisplayCLDProcDisplay, 18, 3, ~0);
}

// 0x800FCE6C
void efDisplayMakeXLU(void)
{
    gcAddGObjDisplay(gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT), efDisplayXLUProcDisplay, 15, 0, ~0);
    gcAddGObjDisplay(gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT), efDisplayXLUProcDisplay, 18, 0, ~0);
}

// 0x800FCEEC
void efDisplayZPerspXLUProcDisplay(GObj *effect_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

    lbParticleDrawTextures(effect_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetTexturePersp(gSYTaskmanDLHeads[0]++, G_TP_PERSP);
    gDPSetDepthSource(gSYTaskmanDLHeads[0]++, G_ZS_PIXEL);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x800FCFCC
void efDisplayZPerspCLDProcDisplay(GObj *effect_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_CLD_SURF, G_RM_CLD_SURF2);

    lbParticleDrawTextures(effect_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetTexturePersp(gSYTaskmanDLHeads[0]++, G_TP_PERSP);
    gDPSetDepthSource(gSYTaskmanDLHeads[0]++, G_ZS_PIXEL);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x800FD0AC
void efDisplayZPerspAAXLUProcDisplay(GObj *effect_gobj)
{
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    lbParticleDrawTextures(effect_gobj);

    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetTexturePersp(gSYTaskmanDLHeads[0]++, G_TP_PERSP);
    gDPSetDepthSource(gSYTaskmanDLHeads[0]++, G_ZS_PIXEL);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

// 0x800FD18C
void efDisplayInitAll(void)
{
    GObj *gobj;

    gobj = gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, efDisplayZPerspCLDProcDisplay, 18, 1, ~0);
    gobj->camera_mask = COBJ_MASK_DLLINK(2) | COBJ_MASK_DLLINK(0);

    gobj = gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, efDisplayZPerspCLDProcDisplay, 15, 1, ~0);
    gobj->camera_mask = COBJ_MASK_DLLINK(1);

    gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, efDisplayZPerspXLUProcDisplay, 25, GOBJ_PRIORITY_DEFAULT, ~0);
    gobj->camera_mask = COBJ_MASK_DLLINK(3);

    gobj = gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT);
    gcAddGObjDisplay(gobj, efDisplayZPerspAAXLUProcDisplay, 10, GOBJ_PRIORITY_DEFAULT, ~0);
    gobj->camera_mask = COBJ_MASK_DLLINK(4);

    gEFManagerParticleBankID = efParticleGetLoadBankID
    (
        &lEFCommonParticleScriptBankLo,
        &lEFCommonParticleScriptBankHi,
        &lEFCommonParticleTextureBankLo,
        &lEFCommonParticleTextureBankHi
    );
}
