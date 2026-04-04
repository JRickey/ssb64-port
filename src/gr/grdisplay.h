#ifndef _GRGEOMETRY_H_
#define _GRGEOMETRY_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern void grDisplayLayer0PriProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer0SecProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer1PriProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer1SecProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer2PriProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer2SecProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer3PriProcDisplay(GObj *ground_gobj);
extern void grDisplayLayer3SecProcDisplay(GObj *ground_gobj);
extern void grDisplayDObjSetNoAnimXObj(GObj *ground_gobj, DObjDesc *dobjdesc);
extern GObj* grDisplayMakeGeometryLayer(MPGroundDesc *gr_desc, s32 gr_desc_id, DObj **dobjs);

#endif
