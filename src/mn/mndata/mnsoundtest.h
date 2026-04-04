#ifndef _MNSOUNDTEST_H_
#define _MNSOUNDTEST_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <PR/gbi.h>
#include <mn/mndef.h>

extern void mnSoundTestUpdateOptionColors(void);
extern void mnSoundTestUpdateControllerInputs(void);
extern void mnSoundTestUpdateFunctions(void);
extern void mnSoundTestFuncRun(GObj *gobj);
extern void mnSoundTestSetupFiles(void);
extern SObj* mnSoundTestMakeHeaderSObjs(void);
extern void mnSoundTestOptionThreadUpdate(GObj *gobj);
extern void mnSoundTestMusicProcDisplay(GObj *gobj);
extern SObj* mnSoundTestMakeMusicSObjs(void);
extern void mnSoundTestSoundProcDisplay(GObj *gobj);
extern SObj* mnSoundTestMakeSoundSObjs(void);
extern void mnSoundTestVoiceProcDisplay(GObj *gobj);
extern SObj* mnSoundTestMakeVoiceSObjs(void);
extern SObj* mnSoundTestMakeAButtonSObj(GObj *gobj);
extern SObj* mnSoundTestMakeBButtonSObj(GObj *gobj);
extern SObj* mnSoundTestMakeStartButtonSObj(GObj *gobj);
extern SObj* mnSoundTestMakeAFunctionSObj(GObj *gobj);
extern SObj* mnSoundTestMakeStartFunctionSObj(GObj *gobj);
extern SObj* mnSoundTestMakeBFunctionSObj(GObj *gobj);
extern void mnSoundTestMakeButtonSObjs(void);
extern void mnSoundTestMakeNumberSObj(GObj *gobj);
extern void mnSoundTestUpdateNumberPositions(GObj *gobj, f32 width);
extern void mnSoundTestUpdateNumberSprites(GObj *gobj);
extern void mnSoundTestSelectIDThreadUpdate(GObj *gobj);
extern void mnSoundTestMakeSelectIDGObjs(void);
extern void mnSoundTestArrowsThreadUpdate(GObj *gobj);
extern void mnSoundTestMakeArrowSObjs(void);
extern void mnSoundTestMakeAllSObjs(void);
extern void mnSoundTestMakeCameras(void);
extern void mnSoundTestInitVars(void);
extern void mnSoundTestFuncStart(void);
extern void mnSoundTestFuncLights(Gfx **dls);
extern void mnSoundTestStartScene(void);

#endif
