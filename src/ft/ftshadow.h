#ifndef _FTSHADOW_H_
#define _FTSHADOW_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

extern f32 ftShadowGetAltitude(Vec3f *a, Vec3f *b, f32 f);
extern void ftShadowProcDisplay(GObj *shadow_gobj);
extern GObj* ftShadowMakeShadow(GObj *fighter_gobj);

#endif
