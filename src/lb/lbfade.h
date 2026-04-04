#ifndef _LBFADE_H_
#define _LBFADE_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <lb/lbdef.h>

extern void lbFadeProcUpdate(GObj *gobj);
extern void lbFadeProcDisplay(GObj *gobj);
extern void lbFadeMakeActor(u32 id, s32 link, u32 link_priority, SYColorRGBA *color, s32 fade_length, sb32 is_eject_gobj, sb32 *is_proceed_scene);

#endif
