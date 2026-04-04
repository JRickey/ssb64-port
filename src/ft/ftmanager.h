#ifndef _FTMANAGER_H_
#define _FTMANAGER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

extern u32 gFTManagerPlayersNum;
extern u16 gFTManagerMotionCount;
extern u16 gFTManagerStatUpdateCount;
extern void *gFTManagerCommonFile;
extern size_t gFTManagerFigatreeHeapSize;

extern FTDesc dFTManagerDefaultFighterDesc;
extern FTData *dFTManagerDataFiles[/* */];

extern void ftManagerSetupFileSize();
extern void ftManagerAllocFighter(u32 data_flags, s32 allocs_num);
extern FTStruct* ftManagerGetNextStructAlloc();
extern void ftManagerSetPrevStructAlloc(FTStruct* fp);
extern FTParts* ftManagerGetNextPartsAlloc();
extern void ftManagerSetPrevPartsAlloc(FTParts* parts);
extern void ftManagerSetupFilesMainKind(s32 fkind);
extern void func_ovl2_800D7710(s32 fkind);
extern void ftManagerSetupFilesPlayablesAll();
extern void ftManagerSetupFilesAllKind(s32 fkind);
extern void* ftManagerAllocFigatreeHeapKind(s32 fkind);
extern void ftManagerDestroyFighter(GObj* fighter_gobj);
extern void ftManagerDestroyFighterWeapons(GObj *fighter_gobj);
extern void func_ovl2_800D79F0(GObj *fighter_gobj, FTDesc *desc);
extern GObj* ftManagerMakeFighter(FTDesc *desc);

#endif
