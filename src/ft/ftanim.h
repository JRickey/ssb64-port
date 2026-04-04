#ifndef _FTANIM_H_
#define _FTANIM_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

extern f32 ftAnimGetTargetValue(s16 arg0, s32 arg1, s32 arg2);
extern void ftAnimParseDObjFigatree(DObj *root_dobj);
extern void func_ovl2_800ECCA4(GObj *gobj);

#endif
