#ifndef _SC1PMANAGER_H_
#define _SC1PMANAGER_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <sc/scdef.h>

// 0x80130D60
extern u8 sSC1PManagerScenePrev;

// 0x80130D64 - Total time (in frames) taken to complete 1P Game
extern u32 gSC1PManagerTotalTimeTics;

// 0x80130D68 - Total times fallen in 1P Game
extern s32 gSC1PManagerTotalFalls;

// 0x80130D6C - Total damage taken in 1P Game
extern s32 gSC1PManagerTotalDamage;

// 0x80130D70 - Starts at 0, increments by 1 each time a continue is used; lowers CP difficulty (on current stage?)
extern s32 gSC1PManagerLevelDrop;

// 0x80130D74 - Starts at 2, each time this reaches 0 it increments gSC1PManagerLevelDrop
extern u8 sSC1PManagerLevelGuard;

// 0x80130D75 - Copy ability of final Kirby on Kirby Team in 1P Game
extern u8 gSC1PManagerKirbyTeamFinalCopy;

// 0x80130D76
extern u8 gSC1PManagerKirbyTeamModelPartID;

extern s32 sc1PManagerGetFighterKindsNum(u16 flag);
extern s32 sc1PManagerGetShuffledFighterKind(u16 this_mask, u16 prev_mask, s32 random);
extern s32 sc1PManagerGetShuffledKirbyCopy(u16 flags, s32 random);
extern void sc1PManagerTrySetChallengers(void);
extern sb32 sc1PManagerCheckUnlockSoundTest(void);
extern void sc1PManagerTrySaveBackup(sb32 is_complete_spgame);
extern void sc1PManagerUpdateScene(void);

#endif
