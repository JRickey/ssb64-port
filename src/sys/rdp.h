#ifndef _SYRDP_H_
#define _SYRDP_H_

#include <PR/mbi.h>
#include <PR/ultratypes.h>

extern Vp gSYRdpViewport;

extern void syRdpSetViewport(Vp *vp, f32 ulx, f32 uly, f32 lrx, f32 lry);
extern void syRdpSetDefaultViewport(Vp *vp);
extern void syRdpSetFuncLights(void (*func_lights)(Gfx**));
extern void syRdpResetSettings(Gfx **dl);

#endif
