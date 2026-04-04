#ifndef _FTDISPLAYMAIN_H_
#define _FTDISPLAYMAIN_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

extern Vec2f dFTDisplayMainShufflePositions[/* */][4];

extern void ftDisplayMainDrawAfterImage(FTStruct *fp);
extern void ftDisplayMainCalcFogColor(FTStruct *fp);
extern void ftDisplayMainSetFogColor(FTStruct *fp);
extern void ftDisplayMainDecideFogColor(FTStruct *fp);
extern void ftDisplayMainDecideFogDraw(u8 flags, FTStruct *fp);
extern void ftDisplayMainDrawAccessory(FTStruct *fp, DObj *dobj, FTParts *parts);
extern void ftDisplayMainDrawDefault(DObj *dobj);
extern void ftDisplayMainDrawSkeleton(DObj *dobj);
extern void ftDisplayMainDrawAll(GObj *fighter_gobj);
extern void ftDisplayMainDrawParts(DObj *dobj);
extern void ftDisplayMainProcDisplay(GObj *fighter_gobj);

#endif
