#ifndef _GRDEF_H_
#define _GRDEF_H_

// Enums
typedef enum GRKind
{
	nGRKindCommonStart,							// Start of common stages
	
	nGRKindBattleStart = nGRKindCommonStart,	// Start of VS Mode Stages
	nGRkindStarterStart = nGRKindBattleStart,	// Start of starter stages
	nGRKindCastle = nGRkindStarterStart, 		// Peach's Castle
	nGRKindSector,								// Sector Z
	nGRKindJungle,								// Kongo Jungle
	nGRKindZebes,								// Planet Zebes
	nGRKindHyrule,								// Hyrule Castle
	nGRKindYoster,	  							// Yoshi's Island
	nGRKindPupupu,	 							// Dream Land
	nGRKindYamabuki, 							// Saffron City
	nGRKindStarterEnd = nGRKindYamabuki,		// End of starter stages
	nGRKindUnlockStart,							// Start of unlockable stages
	nGRKindInishie = nGRKindUnlockStart,  		// Mushroom Kingdom
	nGRKindUnlockEnd = nGRKindInishie,			// End of unlockable stages
	nGRKindBattleEnd = nGRKindUnlockEnd,		// End of VS Mode Stages

	nGRKindPupupuSmall,							// Beta Dream Land
	nGRKindPupupuNew,							// Test Stage
	nGRKindExplain,  							// How to Play
	nGRKindYosterSmall,							// Small Yoshi's Island (1P Game)
	nGRKindMetal,  								// Meta Crystal
	nGRKindZako,	  							// Duel Zone
	nGRKindBonus3,	  							// Race to the Finish
	nGRKindLast,	  							// Final Destination
	
	nGRKindCommonEnd = nGRKindLast,				// End of common stages


	nGRKindBonusStageStart,						// Start of bonus stages

	nGRKindBonus1Start = nGRKindBonusStageStart, // Start of Target Test stages
	nGRKindBonus1Mario = nGRKindBonus1Start,
	nGRKindBonus1Fox,
	nGRKindBonus1Donkey,
	nGRKindBonus1Samus,
	nGRKindBonus1Luigi,
	nGRKindBonus1Link,
	nGRKindBonus1Yoshi,
	nGRKindBonus1Captain,
	nGRKindBonus1Kirby,
	nGRKindBonus1Pikachu,
	nGRKindBonus1Purin,
	nGRKindBonus1Ness,
	nGRKindBonus1End = nGRKindBonus1Ness,

	nGRKindBonus2Start, 						// Start of Board the Platforms stages
	nGRKindBonus2Mario = nGRKindBonus2Start,
	nGRKindBonus2Fox,
	nGRKindBonus2Donkey,
	nGRKindBonus2Samus,
	nGRKindBonus2Luigi,
	nGRKindBonus2Link,
	nGRKindBonus2Yoshi,
	nGRKindBonus2Captain,
	nGRKindBonus2Kirby,
	nGRKindBonus2Pikachu,
	nGRKindBonus2Purin,
	nGRKindBonus2Ness,
	nGRKindBonus2End = nGRKindBonus2Ness,

	nGRKindBonusStageEnd = nGRKindBonus2End		// End of bonus stages

} GRKind;

// Structs
typedef struct GRDisplayDesc 	GRDisplayDesc;
typedef struct GRAttackColl 	GRAttackColl;
typedef struct GRObstacle 		GRObstacle;
typedef struct GRHazard 		GRHazard;
typedef struct GRFileInfo 		GRFileInfo;
typedef union  GRStruct 		GRStruct;

#endif
