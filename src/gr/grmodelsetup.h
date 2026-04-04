#ifndef _GRMODEL_H_
#define _GRMODEL_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <gr/grdef.h>

extern void grModelSetupGroundDObjs(GObj *gobj, DObjDesc *dobjdesc, DObj **p_dobj, DObjTransformTypes *transform_types);

#endif
