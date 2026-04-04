#ifndef _ITEM_H_
#define _ITEM_H_

#include "ittypes.h"
#include "itfunctions.h"

// Global variables declared here as extern for easy access

// 0x8018D040
extern void *gITManagerCommonData;

// 0x8018D044
extern s32 gITManagerParticleBankID;

// 0x8018D048
extern ITRandomWeights gITManagerRandomWeights;

// 0x8018D060
extern ITMonsterData gITManagerMonsterData;

// 0x8018D090
extern s32 gITManagerDisplayMode;

// 0x8018D094
extern ITStruct* gITManagerStructsAllocFree;

// 0x8018D098
extern ITAppearActor gITManagerAppearActor;

// Global data

// 0x80189450
extern s32 dITManagerForceMonsterKind;

// Linker variable, points to base of animation bank in item file? 0x00013624
extern intptr_t lITMonsterAnimBankStart;

#define itGetStruct(item_gobj) ((ITStruct*)(item_gobj)->user_data.p)

// Points to all sorts of data
#define itGetPData(ip, off1, off2)                                                                                     \
	((void*)(((uintptr_t)(ip)->attr->data - (intptr_t) (off1)) + (intptr_t) (off2)))

#define itGetMonsterAnimNode(ip, off)                                                                                  \
	((void*)(((uintptr_t)(ip)->attr->data - (intptr_t) (off)) + (intptr_t)&llITCommonDataMonsterAnimBankStart))

#define itGetAttackEvent(it_desc, off) ((ITAttackEvent*)((uintptr_t) * (it_desc).p_file + (intptr_t) (off)))

#define itGetMonsterEvent(it_desc, off) ((ITMonsterEvent*)((uintptr_t) * (it_desc).p_file + (intptr_t) (off)))

#endif
