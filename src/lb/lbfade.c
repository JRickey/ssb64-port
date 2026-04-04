#include <lb/library.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D6460
s32 sLBFadeAlphaMax;

// 0x800D6464
s32 sLBFadeAlphaCurrent;

// 0x800D6468
s32 sLBFadeLength;

// 0x800D646C
SYColorRGBA sLBFadeColor;

// 0x800D6470
sb32* sLBFadeIsProceedScene;

// 0x800D6474
sb32 sLBFadeIsEjectGObj;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800D3E80
void lbFadeProcUpdate(GObj *gobj)
{
    if (sLBFadeAlphaCurrent < sLBFadeAlphaMax)
    {
        sLBFadeAlphaCurrent++;
    }
    if (sLBFadeLength != 0)
    {
        sLBFadeLength--;
        
        if (sLBFadeLength == 0)
        {
            if (sLBFadeIsProceedScene != NULL)
            {
                *sLBFadeIsProceedScene = TRUE;
            }
            if (sLBFadeIsEjectGObj != FALSE)
            {
                gcEjectGObj(gobj);
            }
        }
    }
}

// 0x800D3F08
void lbFadeProcDisplay(GObj *gobj)
{
    s32 alpha = ((f32) sLBFadeAlphaCurrent / (f32) sLBFadeAlphaMax) * 255.0F;
    
    if (sLBFadeColor.a == 0)
    {
        alpha = 0xFF - alpha;
    }
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetCycleType(gSYTaskmanDLHeads[0]++, G_CYC_1CYCLE);
    gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, sLBFadeColor.r, sLBFadeColor.g, sLBFadeColor.b, alpha);
    gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gSYTaskmanDLHeads[0]++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gDPFillRectangle(gSYTaskmanDLHeads[0]++, 10, 10, GS_SCREEN_WIDTH_DEFAULT - 10, GS_SCREEN_HEIGHT_DEFAULT - 10);
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
}

// 0x800D4060
void lbFadeMakeActor(u32 id, s32 link, u32 link_priority, SYColorRGBA *color, s32 fade_length, sb32 is_eject_gobj, sb32 *is_proceed_scene)
{
    GObj *gobj = gcMakeGObjSPAfter(id, NULL, link, GOBJ_PRIORITY_DEFAULT);

    func_80009F74(gobj, lbFadeProcDisplay, link_priority, 0, ~0);
    gcAddGObjProcess(gobj, lbFadeProcUpdate, nGCProcessKindFunc, 0);
    
    sLBFadeColor = *color;
    sLBFadeAlphaMax = fade_length;
    sLBFadeAlphaCurrent = 0;
    sLBFadeLength = fade_length + 2;
    sLBFadeIsEjectGObj = is_eject_gobj;
    sLBFadeIsProceedScene = is_proceed_scene;
}
