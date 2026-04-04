#ifndef _MNMODESELECT_H_
#define _MNMODESELECT_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnModeSelectFuncLights(Gfx **dls);
extern void mnModeSelectMake1PMode(void);
extern void mnModeSelectMakeVSMode(void);
extern void mnModeSelectMakeOption(void);
extern void mnModeSelectMakeData(void);
extern void mnModeSelectMakeLabels(void);
extern void mnModeSelectMakeDecals(void);
extern void mnModeSelectMakeLabelsCamera(void);
extern void mnModeSelectMakeDecalsCamera(void);
extern void mnModeSelectMakeOptions(void);
extern void mnModeSelectEjectOptions(void);
extern void mnModeSelectInitVars(void);
extern void mnModeSelectFuncRun(GObj *gobj);
extern void mnModeSelectFuncStart(void);
extern void mnModeSelectStartScene(void);

#endif
