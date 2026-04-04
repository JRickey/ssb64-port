#ifndef _LBTRANSITION_H_
#define _LBTRANSITION_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <lb/lbdef.h>

extern LBTransitionDesc dLBTransitionDescs[11];

extern GObj* lbTransitionMakeCamera(u32 id, s32 link, u32 link_priority, u64 camera_mask);
extern void lbTransitionProcDisplay(GObj *gobj);
extern void lbTransitionProcUpdate(GObj *gobj);
extern GObj* lbTransitionMakeTransition(s32 transition_id, u32 id, s32 link, void (*proc_display)(GObj*), u8 dl_link_id, void (*func)(GObj*));
extern void lbTransitionSetupTransition(void);

#endif
