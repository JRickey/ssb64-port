#ifndef _IFDEF_H_
#define _IFDEF_H_

// Macros
#define IFCOMMON_PLAYERARROWS_MASK_LEFT  	(1 << 0)
#define IFCOMMON_PLAYERARROWS_MASK_RIGHT 	(1 << 1)

// Enums
typedef enum IFPauseKind
{
	nIFPauseKindPlayerNA, // Player not available (not within pause zoom range)
	nIFPauseKindDefault,  // Default
	nIFPauseKindBonus	  // Bonus Stages

} IFPauseKind;

typedef enum IFPlayerTagKind
{
	nIFPlayerTagKind1P,
	nIFPlayerTagKind2P,
	nIFPlayerTagKind3P,
	nIFPlayerTagKind4P,
	nIFPlayerTagKindCP,
	nIFPlayerTagKindHeart

} IFPlayerTagKind;

// Structs
typedef struct IFPlayerCommon	IFPlayerCommon;
typedef struct IFDCharacter 	IFDCharacter;
typedef struct IFACharacter 	IFACharacter;
typedef struct IFPlayerDamage 	IFPlayerDamage;
typedef struct IFPlayerSteal 	IFPlayerSteal;
typedef struct IFPlayerMagnify 	IFPlayerMagnify;
typedef struct IFTraffic 		IFTraffic;
typedef struct IFPauseDecal 	IFPauseDecal;

#endif
