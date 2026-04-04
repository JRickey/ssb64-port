#ifndef _IFSCREENFLASH_H_
#define _IFSCREENFLASH_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <if/ifdef.h>

extern void ifScreenFlashSetColAnimID(s32 colanim_id, s32 colanim_duration);
extern void ifScreenFlashProcDisplay(GObj *gobj);
extern void ifScreenFlashProcUpdate(GObj *fighter_gobj);
extern void ifScreenFlashMakeInterface(u8 alpha);

#endif
