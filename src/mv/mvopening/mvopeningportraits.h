#ifndef _MVOPENINGPORTRAITS_H_
#define _MVOPENINGPORTRAITS_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>

extern void mvOpeningPortraitsFuncLights(Gfx **dls);
extern void mvOpeningPortraitsMakeSet1(void);
extern void mvOpeningPortraitsMakeSet2(void);
extern void mvOpeningPortraitsBlockRow0(void);
extern void mvOpeningPortraitsBlockRow1(void);
extern void mvOpeningPortraitsBlockRow2(void);
extern void mvOpeningPortraitsBlockRow3(void);
extern void mvOpeningPortraitsBlockPartialRow(s32 row, s32 pos_x);
extern void mvOpeningPortraitsCoverProcDisplay(GObj *gobj);
extern void mvOpeningPortraitsCoverProcUpdate(GObj *gobj);
extern void mvOpeningPortraitsMakeCover(void);
extern void mvOpeningPortraitsMakePortraitsCamera(void);
extern void mvOpeningPortraitsMakeCoverCamera(void);
extern void mvOpeningPortraitsInitVars(void);
extern void mvOpeningPortraitsFuncStart(void);
extern void mvOpeningPortraitsStartScene(void);

#endif
