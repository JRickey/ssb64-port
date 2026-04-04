#ifndef _SCMANAGER_H_
#define _SCMANAGER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <sys/taskman.h>
#include <sc/scdef.h>
#include <lb/lbdef.h>

extern char dSCManagerBuildDate[/* */];

extern s32 gSCManagerUnkown0x800A50F0;
extern u32 gSCManagerCIC;

extern SCBattleState
*gSCManagerBattleState,
dSCManagerDefaultBattleState, 
gSCManager1PGameBattleState,
gSCManagerTransferBattleState,
gSCManagerVSBattleState;

extern SCCommonData gSCManagerSceneData, dSCManagerDefaultSceneData;
extern LBBackupData gSCManagerBackupData, dSCManagerDefaultBackupData;

extern void scManagerRunLoop(sb32 arg);
extern void scManagerFuncUpdate(SYTaskmanSetup *arg);
extern void scManagerFuncDraw(void);
extern void scManagerMeterProcDisplay(GObj *gobj);
extern GObj* scManagerMakeMeterCamera(s32 link, u32 link_priority, u32 dl_link_priority);
extern void scManagerMakeDebugCameras(s32 link, u32 link_priority, s32 dl_link_priority);
extern void scManagerInspectGObj(GObj *gobj);
extern void scManagerFuncPrint(void);
extern void scManagerRunPrintGObjStatus(void);

#endif
