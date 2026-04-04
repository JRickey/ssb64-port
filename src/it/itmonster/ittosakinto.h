#ifndef _ITTOSAKINTO_H_
#define _ITTOSAKINTO_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <it/itdef.h>

extern sb32 itTosakintoAppearProcUpdate(GObj *item_gobj);
extern sb32 itTosakintoAppearProcMap(GObj *item_gobj);
extern void itTosakintoAppearSetStatus(GObj *item_gobj);
extern sb32 itTosakintoBounceProcUpdate(GObj *item_gobj);
extern sb32 itTosakintoBounceProcMap(GObj *item_gobj);
extern void itTosakintoBounceInitVars(GObj *item_gobj);
extern void itTosakintoBounceSetStatus(GObj *item_gobj);
extern sb32 itTosakintoCommonProcUpdate(GObj *item_gobj);
extern sb32 itTosakintoCommonProcMap(GObj *item_gobj);
extern GObj* itTosakintoMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags);

#endif
