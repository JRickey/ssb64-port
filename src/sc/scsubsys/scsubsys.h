#ifndef _SCSUBSYS_H_
#define _SCSUBSYS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <sc/scdef.h>

extern f32 dSCSubsysFighterScales[/* */];

extern void scSubsysFighterSetLightParams(f32 light_angle_x, f32 light_angle_y, u8 r, u8 g, u8 b, u8 a);
extern f32 scSubsysFighterGetLightAngleX(void);
extern f32 scSubsysFighterGetLightAngleY(void);
extern u8 scSubsysFighterDrawLightColorGetAlpha(Gfx **dls);
extern void scSubsysFighterProcUpdate(GObj *fighter_gobj);
extern void scSubsysFighterSetStatus(GObj *fighter_gobj, s32 status_id);
extern void scSubsysFighterOpeningProcUpdate(GObj *this_gobj, GObj *other_gobj);
extern void scSubsysFighterApplyVelTransN(GObj *fighter_gobj);

extern sb32 scSubsysControllerCheckConnected(s32 status);
extern s32 scSubsysControllerGetPlayerTapButtons(u32 buttons);
extern s32 scSubsysControllerGetPlayerHoldButtons(u32 buttons);
extern s32 scSubsysControllerGetPlayerStickLR(s8 range, sb32 right_or_left);
extern s32 scSubsysControllerGetPlayerStickUD(s8 range, sb32 up_or_down);
extern sb32 scSubsysControllerGetPlayerStickInRangeLR(s32 range_l_min, s32 range_r_min);
extern sb32 scSubsysControllerGetPlayerStickInRangeUD(s32 range_d_min, s32 range_u_min);
extern sb32 scSubsysControllerCheckNoInputAll(void);

#endif
