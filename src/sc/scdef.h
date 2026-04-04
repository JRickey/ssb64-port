#ifndef _SCDEF_H_
#define _SCDEF_H_

#define SCBATTLE_TIMELIMIT_INFINITE 100
#define SCBATTLE_BONUSGAME_TASK_MAX 10

#define SC1PGAME_BONUS_MASK_CREATE(kind)    (1 << ((kind) - (((kind) / NBITS(u32, 1)) * NBITS(u32, 1))))

#define SCBATTLE_GAMERULE_TIME 	            (1 << nSCBattleGameRuleTime)	        // 0x1
#define SCBATTLE_GAMERULE_STOCK	            (1 << nSCBattleGameRuleStock)           // 0x2
#define SCBATTLE_GAMERULE_BONUS             (1 << nSCBattleGameRuleBonus) 	        // 0x4
#define SCBATTLE_GAMERULE_1PGAME            (1 << nSCBattleGameRule1PGame)          // 0x8

// First set of bonuses
#define SC1PGAME_BONUS_MASK0_CHEAPSHOT		(1 << nSC1PGameBonusCheapShot)          // One attack made up >= 35% of all attacks used
#define SC1PGAME_BONUS_MASK0_STARFINISH		(1 << nSC1PGameBonusStarFinish)	        // KO'd last enemy with a Star KO or Screen KO
#define SC1PGAME_BONUS_MASK0_NOITEM			(1 << nSC1PGameBonusNoItem)			    // Did not use any items
#define SC1PGAME_BONUS_MASK0_SHIELDBREAKER	(1 << nSC1PGameBonusShieldBreaker)      // Broke enemy's shield
#define SC1PGAME_BONUS_MASK0_JUDOWARRIOR	(1 << nSC1PGameBonusJudoWarrior)		// Only used throws
#define SC1PGAME_BONUS_MASK0_HAWK			(1 << nSC1PGameBonusHawk)		        // Only used aerials
#define SC1PGAME_BONUS_MASK0_SHOOTER		(1 << nSC1PGameBonusShooter)		    // Only used projectiles
#define SC1PGAME_BONUS_MASK0_HEAVYDAMAGE	(1 << nSC1PGameBonusHeavyDamage)		// Accumulated over 200% damage
#define SC1PGAME_BONUS_MASK0_ALLVARIATIONS	(1 << nSC1PGameBonusAllVariations)	    // Used all jabs, tilts, attacks and aerials
#define SC1PGAME_BONUS_MASK0_ITEMSTRIKE		(1 << nSC1PGameBonusItemStrike)		    // Only used items
#define SC1PGAME_BONUS_MASK0_DOUBLEKO		(1 << nSC1PGameBonusDoubleKO) 		    // Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK0_TRICKSTER		(1 << nSC1PGameBonusTrickster)		    // KO'd every enemy of a team with a Star KO or Screen KO
#define SC1PGAME_BONUS_MASK0_GIANTIMPACT	(1 << nSC1PGameBonusGiantImpact)		// Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK0_SPEEDSTER		(1 << nSC1PGameBonusSpeedster)		    // Cleared stage in less than 30 seconds
#define SC1PGAME_BONUS_MASK0_ITEMTHROW		(1 << nSC1PGameBonusItemThrow)		    // Threw or dropped all items, never used them in any other way
#define SC1PGAME_BONUS_MASK0_TRIPLEKO		(1 << nSC1PGameBonusTripleKO)		    // Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK0_LASTCHANCE		(1 << nSC1PGameBonusLastChance)		    // Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK0_PACIFIST		(1 << nSC1PGameBonusPacifist)		    // Cleared stage without dealing any damage
#define SC1PGAME_BONUS_MASK0_PERFECT		(1 << nSC1PGameBonusPerfect)		    // Cleared Bonus Stage with all targets broken or all platforms boarded
#define SC1PGAME_BONUS_MASK0_NOMISS			(1 << nSC1PGameBonusNoMiss)		        // Cleared stage without falling once throughout 1P Game progress; multiplied each time it is earned, lost after first fall
#define SC1PGAME_BONUS_MASK0_NODAMAGE		(1 << nSC1PGameBonusNoDamage)	        // Cleared stage without falling and taking any damage
#define SC1PGAME_BONUS_MASK0_FULLPOWER		(1 << nSC1PGameBonusFullPower)	        // Cleared stage with 0% damage (does not yield No Damage)
#define SC1PGAME_BONUS_MASK0_GAMECLEAR		(1 << nSC1PGameBonusStageClear)	        // Cleared Final Stage (number of bonus points depends on difficulty?)
#define SC1PGAME_BONUS_MASK0_NOMISSCLEAR	(1 << nSC1PGameBonusNoMissClear)	    // Cleared all stages without falling once (except Bonus Stages?)
#define SC1PGAME_BONUS_MASK0_NODAMAGECLEAR	(1 << nSC1PGameBonusNoDamageClear)	    // Cleared all stages without taking any damage
#define SC1PGAME_BONUS_MASK0_SPEEDKING		(1 << nSC1PGameBonusSpeedKing)	        // Cleared all stages in under 12 minutes (not earned if Speed Demon criteria is met)
#define SC1PGAME_BONUS_MASK0_SPEEDDEMON		(1 << nSC1PGameBonusSpeedDemon)	        // Cleared all stages in under 8 minutes
#define SC1PGAME_BONUS_MASK0_MEWCATCHER		(1 << nSC1PGameBonusMewCatcher)	        // Mew was released from player's Pok� Ball
#define SC1PGAME_BONUS_MASK0_STARCLEAR		(1 << nSC1PGameBonusStarClear)	        // Cleared stage while under the effect of Star Man
#define SC1PGAME_BONUS_MASK0_VEGETARIAN		(1 << nSC1PGameBonusVegetarian)	        // Consumed 3 or more Maxim Tomatoes
#define SC1PGAME_BONUS_MASK0_HEARTTHROB		(1 << nSC1PGameBonusHeartThrob)	        // Consumed 3 or more Hearts
#define SC1PGAME_BONUS_MASK0_THROWDOWN		(1 << nSC1PGameBonusThrowDown)	        // KO'd last enemy with a throw

// Second set of bonuses
#define SC1PGAME_BONUS_MASK1_SMASHMANIA		(1 << (nSC1PGameBonusSmashMania-32))    // Only used Smash Attacks
#define SC1PGAME_BONUS_MASK1_SMASHLESS		(1 << (nSC1PGameBonusSmashless-32))	    // Never used Smash Attacks
#define SC1PGAME_BONUS_MASK1_SPECIALMOVE	(1 << (nSC1PGameBonusSpecialMove-32))	// Only used Special Attacks
#define SC1PGAME_BONUS_MASK1_SINGLEMOVE		(1 << (nSC1PGameBonusSingleMove-32))    // Only used the same Special Attack
#define SC1PGAME_BONUS_MASK1_POKEMONFINISH	(1 << (nSC1PGameBonusPokemonFinish-32))	// KO'd last enemy with a Pok�mon (Saffron City Pok�mon projectiles also count, but not the Pok�mon themselves?)
#define SC1PGAME_BONUS_MASK1_BOOBYTRAP		(1 << (nSC1PGameBonusBoobyTrap-32))		// KO'd last enemy with a Motion Sensor Bomb
#define SC1PGAME_BONUS_MASK1_FIGHTERSTANCE	(1 << (nSC1PGameBonusFighterStance-32)) // Player was taunting as stage ended
#define SC1PGAME_BONUS_MASK1_MYSTIC			(1 << (nSC1PGameBonusMystic-32))		// Player fell as stage ended
#define SC1PGAME_BONUS_MASK1_COMETMYSTIC	(1 << (nSC1PGameBonusCometMystic-32))   // Player was getting Star KO'd as stage ended (Screen KOs do not count)
#define SC1PGAME_BONUS_MASK1_ACIDCLEAR		(1 << (nSC1PGameBonusAcidClear-32))		// Last enemy got KO'd by Acid
#define SC1PGAME_BONUS_MASK1_BUMPERCLEAR	(1 << (nSC1PGameBonusBumperClear-32))	// Last enemy got KO'd by Bumper
#define SC1PGAME_BONUS_MASK1_TORNADOCLEAR	(1 << (nSC1PGameBonusTornadoClear-32))  // Last enemy got KO'd by Tornado
#define SC1PGAME_BONUS_MASK1_ARWINGCLEAR	(1 << (nSC1PGameBonusArwingClear-32))	// Last enemy got KO'd by ARWING
#define SC1PGAME_BONUS_MASK1_COUNTERATTACK	(1 << (nSC1PGameBonusCounterAttack-32)) // Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK1_METEORSMASH	(1 << (nSC1PGameBonusMeteorSmash-32))	// Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK1_AERIAL			(1 << (nSC1PGameBonusAerial-32))		// Unused, translated from Japanese
#define SC1PGAME_BONUS_MASK1_LASTSECOND		(1 << (nSC1PGameBonusLastSecond-32))	// Cleared stage with 1 second left on the timer
#define SC1PGAME_BONUS_MASK1_LUCKY3			(1 << (nSC1PGameBonusLucky3-32))		// Cleared stage with 3:33 left on the timer
#define SC1PGAME_BONUS_MASK1_JACKPOT		(1 << (nSC1PGameBonusJackpot-32))		// Cleared stage with a damage % of two or three identical digits (e.g. 44% or 111%)
#define SC1PGAME_BONUS_MASK1_YOSHIRAINBOW	(1 << (nSC1PGameBonusYoshiRainbow-32))	// KO'd every member of Yoshi Team in the order they appeared
#define SC1PGAME_BONUS_MASK1_KIRBYRANKS		(1 << (nSC1PGameBonusKirbyRanks-32))	// KO'd every member of Kirby Team in the order they appeared
#define SC1PGAME_BONUS_MASK1_BROSCALAMITY	(1 << (nSC1PGameBonusBrosCalamity-32))	// KO'd Luigi before damaging Mario on VS Mario Bros. stage
#define SC1PGAME_BONUS_MASK1_DKDEFENDER		(1 << (nSC1PGameBonusDKDefender-32))	// Allies did not fall on VS. Giant Donkey Kong stage
#define SC1PGAME_BONUS_MASK1_DKPERFECT		(1 << (nSC1PGameBonusDKPerfect-32))	    // Allies did not fall and took no damage on VS. Giant Donkey Kong stage
#define SC1PGAME_BONUS_MASK1_GOODFRIEND		(1 << (nSC1PGameBonusGoodFriend-32))	// Ally did not fall on VS Mario Bros. stage
#define SC1PGAME_BONUS_MASK1_TRUEFRIEND		(1 << (nSC1PGameBonusTrueFriend-32))	// Ally did not fall and took no damage on VS Mario Bros. stage

// "DAMAGE", "COMBO", "ENEMY", "SPEED" text
#define SCTRAINING_STATDISPLAY_TEXT_COUNT 4
#define SCTRAINING_STATDISPLAY_CHARACTER_COUNT 39

#define SCTRAINING_DAMAGEDISPLAY_DIGIT_COUNT 3

#define SCTRAINING_COMBODISPLAY_DIGIT_COUNT 2

// Total menu description text sprite count
#define SCTRAINING_MENULABELS_SPRITE_COUNT 10

// Text describing what each option is for (orange text)
#define SCTRAINING_MENULABELS_TEXT_COUNT 6

// Wait this many frames before magnifying glass is shown again after changing
// back from Close-Up view
#define SCTRAINING_VIEW_MAGNIFY_WAIT 180

#define SCTRAINING_GENERAL_SCROLL_WAIT_NORMAL 30
#define SCTRAINING_GENERAL_SCROLL_WAIT_FAST 5

#define SCTRAINING_ITEMSPAWN_MAX 4
#define SCTRAINING_ITEMSPAWN_WAIT 8
#define SCTRAINING_ITEMSPAWN_VEL_Y 30.0F
#define SCTRAINING_ITEMSPAWN_OFF_Y 200.0F

#define SCTRAINING_INPUT_STICK_RANGE_MIN (I_CONTROLLER_RANGE_MAX / 2)

#define SCMANAGER_OVERLAY_DEFINE(OVL_NUM) 		\
{												\
	(uintptr_t)&ovl##OVL_NUM##_ROM_START, 		\
	(uintptr_t)&ovl##OVL_NUM##_ROM_END,			\
	(uintptr_t)&ovl##OVL_NUM##_VRAM, 			\
	(uintptr_t)&ovl##OVL_NUM##_TEXT_START, 		\
	(uintptr_t)&ovl##OVL_NUM##_TEXT_END, 		\
	(uintptr_t)&ovl##OVL_NUM##_DATA_START,		\
	(uintptr_t)&ovl##OVL_NUM##_RODATA_END,		\
	(uintptr_t)&ovl##OVL_NUM##_BSS_START,		\
	(uintptr_t)&ovl##OVL_NUM##_BSS_END			\
}

/*
#define SCMANAGER_OVERLAY_DEFINE(OVL_NUM) 		\
{												\
	(uintptr_t)&ovl##OVL_NUM##_ROM_START, 		\
	(uintptr_t)&ovl##OVL_NUM##_ROM_END,			\
	(uintptr_t)&ovl##OVL_NUM##_VRAM, 			\
	(uintptr_t)&ovl##OVL_NUM##_TEXT_START, 		\
	(uintptr_t)&ovl##OVL_NUM##_TEXT_END, 		\
	(uintptr_t)&ovl##OVL_NUM##_DATA_START,		\
	(uintptr_t)&ovl##OVL_NUM##_RODATA_END,		\
	(uintptr_t)&ovl##OVL_NUM##_BSS_START,		\
	(uintptr_t)&ovl##OVL_NUM##_BSS_END			\
}
*/

typedef enum SCKind
{
	nSCKindNoController,      	  	// "No Controller" screen
	nSCKindTitle,             	  	// Title screen
	nSCKindDebugMaps,      	  	  	// Debug stage select
	nSCKindDebugCube,      	  		// Debug main menu with Kirby cube
	nSCKindDebugBattle,      	  	// Debug character select
	nSCKindDebugFalls,    	  		// Debug results screen
	nSCKindDebugUnknown,      	  	// Debug joystick test
	nSCKindModeSelect,            	// Main Menu
	nSCKind1PMode,            	  	// 1P mode menu
	nSCKindVSMode,        		  	// VS mode
	nSCKindVSOptions,         	  	// VS mode options
	nSCKindVSItemSwitch,		  	// VS mode item switch
	nSCKindMessage,		  	      	// Unlock message
	nSCKind1PChallenger,   		  	// Challenger approaching
	nSCKind1PIntro,       	  		// 1P game "[player] VS [opponent]" screen
	nSCKindScreenAdjust,      	  	// Screen adjust
	nSCKindPlayersVS,          		// VS mode character select
	nSCKind1PGamePlayers,		  	// 1P game character select
	nSCKindPlayers1PTraining,  	// Training mode character select
	nSCKind1PBonus1Players,      	// Bonus 1 practice character select
	nSCKind1PBonus2Players,      	// Bonus 2 practice character select
	nSCKindMaps,     	  		  	// Stage select
	nSCKindVSBattle,          	  	// VS mode in-game
	nSCKindUnknownMario,	   	  	// Unknown movie sequence with Mario?
	nSCKindVSResults,    	  	  	// VS mode results
	nSCKindVSRecord,          	  	// Records screen
	nSCKindCharacters,        		// Character data screen
#if defined(REGION_US)
	nSCKindStartup,               	// N64 logo
#endif
	nSCKindOpeningRoom,       	  	// Room with Master Hand and figures coming to life
	nSCKindOpeningPortraits,		// Stylized horizontal sliding portraits
	nSCKindOpeningMario,			// Mario opening scene
	nSCKindOpeningDonkey,			// Donkey Kong opening scene
	nSCKindOpeningSamus,			// Samus opening scene
	nSCKindOpeningFox,		  		// Fox opening scene
	nSCKindOpeningLink,				// Donkey Kong opening scene
	nSCKindOpeningYoshi,			// Yoshi opening scene
	nSCKindOpeningPikachu,			// Pikachu opening scene
	nSCKindOpeningKirby,			// Kirby opening scene
	nSCKindOpeningRun,				// Characters running on purple background
	nSCKindOpeningYoster,	  		// Yoshis in nest with eggs on Yoshi's Island
	nSCKindOpeningCliff,	      	// Link on dark hill
	nSCKindOpeningStandoff,			// Dark plains with Mario and Kirby in a standoff
	nSCKindOpeningYamabuki,  	  	// Pikachu rocking on Poké Ball
	nSCKindOpeningClash,          	// Characters clashing with each other, doing Neutral Aerials
	nSCKindOpeningSector,		  	// Great Fox, Arwings and Fox scene
	nSCKindOpeningJungle,	      	// DK VS Samus scene
	nSCKindOpeningNewcomers,      	// Last intro scene with hidden characters
	nSCKindBackupClear,  	      	// Save data clear
	nSCKindEnding,            	  	// Ending movie
	nSCKind1PContinue,        	  	// "Continue?" screen
	nSCKind1PScoreUnk,   	  	  	// Stage clear bonus thing?
	nSCKind1PStageClear, 		  	// Stage clear bonus score
	nSCKind1PGame,            	  	// 1P Game logic loop
	nSCKind1PBonusStage,       	  	// Bonus game in-game
	nSCKind1PTrainingMode,    		// Training mode in-game
#if defined(REGION_US)
	nSCKindCongra,	 		  	  	// Congratulations screen
#endif	
	nSCKindStaffroll,             	// Credits scene
	nSCKindOption,           	  	// Options menu
	nSCKindData,              	  	// Data menu
	nSCKindSoundTest,        	  	// Sound Test
	nSCKindExplain,         	  	// How to Play
	nSCKindAutoDemo               	// Auto-Demo

} SCKind;

typedef enum SCBattlePlayerColor
{
	nSCBattlePlayerColorRed,
	nSCBattlePlayerColorBlue,
	nSCBattlePlayerColorYellow,
	nSCBattlePlayerColorGreen,
	nSCBattlePlayerColorCP

} SCBattlePlayerColor;

typedef enum SCBattleTeamID
{
	nSCBattleTeamIDBattleStart,		// Start of Team Battle Team IDs
    nSCBattleTeamIDRed = nSCBattleTeamIDBattleStart,
    nSCBattleTeamIDBlue,
    nSCBattleTeamIDGreen,
	nSCBattleTeamIDBattleEnd = nSCBattleTeamIDGreen, // End of Team Battle Team IDs
    nSCBattleTeamIDCom,				// Computer Team ID (1P Game / Training Mode / Free for All)
	nSCBattleTeamIDEnumCount

} SCBattleTeamID;

typedef enum SCBattleGameStatus
{
	nSCBattleGameStatusWait,	   	// Match not yet started
	nSCBattleGameStatusGo,		   	// Ongoing match
	nSCBattleGameStatusPause,	   	// Player paused
	nSCBattleGameStatusUnpause,	   	// Player unpaused
	nSCBattleGameStatusEnd = 5,	   	// Normal match end
	nSCBattleGameStatusBossDefeat, 	// Master Hand defeated
	nSCBattleGameStatusSet		   	// Player has input A + B + Z + R / 1P Game Stage End?

} SCBattleGameStatus;

typedef enum SCBattleHandicap
{
	nSCBattleHandicapOff,
	nSCBattleHandicapOn,
	nSCBattleHandicapAuto,
	nSCBattleHandicapEnumCount

} SCBattleHandicap;

typedef enum SCBattleItemSwitch
{
	nSCBattleItemSwitchNone,
	nSCBattleItemSwitchVeryLow,
	nSCBattleItemSwitchLow,
	nSCBattleItemSwitchMiddle,
	nSCBattleItemSwitchHigh,
	nSCBattleItemSwitchVeryHigh,
	nSCBattleItemSwitchEnumCount

} SCBattleItemSwitch;

typedef enum SCBattleGameRules
{
	nSCBattleGameRuleTime,
	nSCBattleGameRuleStock,
	nSCBattleGameRuleBonus,
	nSCBattleGameRule1PGame

} SCBattleGameRules;

typedef enum SCBattleGameType
{
	nSCBattleGameTypeDemo,          // 0x0
	nSCBattleGameTypeRoyal,        	// 0x1
	nSCBattleGameTypeBonus,         // 0x2
	nSCBattleGameTypeExplain,       // 0x3
	nSCBattleGameTypeMovie,       // 0x4
	nSCBattleGameType1PGame,        // 0x5
	nSCBattleGameTypeUnk6,          // 0x6
	nSCBattleGameTypeTraining   	// 0x7

} SCBattleGameType;

typedef enum SC1PGameDifficulty
{
	nSC1PGameDifficultyVeryEasy,
	nSC1PGameDifficultyEasy,
	nSC1PGameDifficultyNormal,
	nSC1PGameDifficultyHard,
	nSC1PGameDifficultyVeryHard,
	nSC1PGameDifficultyEnumCount

} SC1PGameDifficulty;

typedef enum SC1PGameStageKind
{
	nSC1PGameStageCommonStart,
	nSC1PGameStageLink = nSC1PGameStageCommonStart,			// VS Link
	nSC1PGameStageYoshi,									// VS Yoshi Team
	nSC1PGameStageFox,										// VS Fox
	nSC1PGameStageBonus1,									// Break the Targets
	nSC1PGameStageMario,									// VS Mario Bros.
	nSC1PGameStagePikachu, 									// VS Pikachu
	nSC1PGameStageDonkey,									// VS Giant Donkey Kong
	nSC1PGameStageBonus2,									// Board the Platforms
	nSC1PGameStageKirby,									// VS Kirby Team
	nSC1PGameStageSamus,									// VS Samus
	nSC1PGameStageMMario,									// VS Metal Mario
	nSC1PGameStageBonus3,									// Race to the Finish
	nSC1PGameStageZako,										// VS Fighting Polygon Team
	nSC1PGameStageBoss,										// VS Master Hand
	nSC1PGameStageCommonEnd = nSC1PGameStageBoss,

	nSC1PGameStageChallengerStart,						   	// Start of unlockable character stages
	nSC1PGameStageLuigi = nSC1PGameStageChallengerStart, 	// Challenger Approaching: Luigi
	nSC1PGameStageNess,								   		// Challenger Approaching: Ness
	nSC1PGameStagePurin,								   	// Challenger Approaching: Jigglypuff
	nSC1PGameStageCaptain,								   	// Challenger Approaching: Captain Falcon
	nSC1PGameStageChallengerEnd = nSC1PGameStageCaptain  	// End of unlockable character stages

} SC1PGameStageKind;

typedef enum SC1PGameBonus
{
	nSC1PGameBonusCheapShot,
	nSC1PGameBonusStarFinish,
	nSC1PGameBonusNoItem,
	nSC1PGameBonusShieldBreaker,
	nSC1PGameBonusJudoWarrior,
	nSC1PGameBonusHawk,
	nSC1PGameBonusShooter,
	nSC1PGameBonusHeavyDamage,
	nSC1PGameBonusAllVariations,
	nSC1PGameBonusItemStrike,
	nSC1PGameBonusDoubleKO,
	nSC1PGameBonusTrickster,
	nSC1PGameBonusGiantImpact,
	nSC1PGameBonusSpeedster,
	nSC1PGameBonusItemThrow,
	nSC1PGameBonusTripleKO,
	nSC1PGameBonusLastChance,
	nSC1PGameBonusPacifist,
	nSC1PGameBonusPerfect,
	nSC1PGameBonusNoMiss,
	nSC1PGameBonusNoDamage,
	nSC1PGameBonusFullPower,
	nSC1PGameBonusStageClear,
	nSC1PGameBonusNoMissClear,
	nSC1PGameBonusNoDamageClear,
	nSC1PGameBonusSpeedKing,
	nSC1PGameBonusSpeedDemon,
	nSC1PGameBonusMewCatcher,
	nSC1PGameBonusStarClear,
	nSC1PGameBonusVegetarian,
	nSC1PGameBonusHeartThrob,
	nSC1PGameBonusThrowDown,
	nSC1PGameBonusSmashMania,
	nSC1PGameBonusSmashless,
	nSC1PGameBonusSpecialMove,
	nSC1PGameBonusSingleMove,
	nSC1PGameBonusPokemonFinish,
	nSC1PGameBonusBoobyTrap,
	nSC1PGameBonusFighterStance,
	nSC1PGameBonusMystic,
	nSC1PGameBonusCometMystic,
	nSC1PGameBonusAcidClear,
	nSC1PGameBonusBumperClear,
	nSC1PGameBonusTornadoClear,
	nSC1PGameBonusArwingClear,
	nSC1PGameBonusCounterAttack,
	nSC1PGameBonusMeteorSmash,
	nSC1PGameBonusAerial,
	nSC1PGameBonusLastSecond,
	nSC1PGameBonusLucky3,
	nSC1PGameBonusJackpot,
	nSC1PGameBonusYoshiRainbow,
	nSC1PGameBonusKirbyRanks,
	nSC1PGameBonusBrosCalamity,
	nSC1PGameBonusDKDefender,
	nSC1PGameBonusDKPerfect,
	nSC1PGameBonusGoodFriend,
	nSC1PGameBonusTrueFriend,
	nSC1PGameBonusEnumCount

} SC1PGameBonus;

typedef enum MN1PContinueOption
{
    nMN1PContinueOptionYes,
    nMN1PContinueOptionNo,
    nMN1PContinueOptionEnumCount

} MN1PContinueOption;

typedef enum SC1PStageClearKind
{
	nSC1PStageClearKindStage,        // Normal stage clear
	nSC1PStageClearKindGame,         // Final stage clear
	nSC1PStageClearKindResult        // Bonus stage results

} SC1PStageClearKind;

typedef enum SC1PTrainingModeMain
{
	nSC1PTrainingModeMenuMainEnumStart,

	nSC1PTrainingModeMenuMainCP = nSC1PTrainingModeMenuMainEnumStart,

	nSC1PTrainingModeMenuMainScrollStart = nSC1PTrainingModeMenuMainCP,
	nSC1PTrainingModeMenuMainItem,
	nSC1PTrainingModeMenuMainSpeed,
	nSC1PTrainingModeMenuMainView,
	nSC1PTrainingModeMenuMainScrollEnd = nSC1PTrainingModeMenuMainView,

	nSC1PTrainingModeMenuMainReset,
	nSC1PTrainingModeMenuMainExit,
	nSC1PTrainingModeMenuEnumEnd = nSC1PTrainingModeMenuMainExit,

	nSC1PTrainingModeMenuMainEnumCount

} SC1PTrainingModeMain;

typedef enum SC1PTrainingModeCP
{
	nSC1PTrainingModeMenuCPEnumStart,
	nSC1PTrainingModeMenuCPStand = nSC1PTrainingModeMenuCPEnumStart,
	nSC1PTrainingModeMenuCPWalk,
	nSC1PTrainingModeMenuCPEvade,
	nSC1PTrainingModeMenuCPJump,
	nSC1PTrainingModeMenuCPAttack,
	nSC1PTrainingModeMenuCPEnumCount

} SC1PTrainingModeCP;

typedef enum SC1PTrainingModeItem
{
	nSC1PTrainingModeMenuItemEnumStart,
	nSC1PTrainingModeMenuItemNone = nSC1PTrainingModeMenuItemEnumStart,
	nSC1PTrainingModeMenuItemMaximTomato,
	nSC1PTrainingModeMenuItemHeart,
	nSC1PTrainingModeMenuItemStar,
	nSC1PTrainingModeMenuItemBeamSword,
	nSC1PTrainingModeMenuItemHomeRunBat,
	nSC1PTrainingModeMenuItemFan,
	nSC1PTrainingModeMenuItemStarRod,
	nSC1PTrainingModeMenuItemRayGun,
	nSC1PTrainingModeMenuItemFireFlower,
	nSC1PTrainingModeMenuItemHammer,
	nSC1PTrainingModeMenuItemMotionSensorBomb,
	nSC1PTrainingModeMenuItemBobomb,
	nSC1PTrainingModeMenuItemBumper,
	nSC1PTrainingModeMenuItemGreenShell,
	nSC1PTrainingModeMenuItemRedShell,
	nSC1PTrainingModeMenuItemPokeBall,
	nSC1PTrainingModeMenuItemEnumCount

} SC1PTrainingModeItem;

typedef enum SC1PTrainingModeSpeed
{
	nSC1PTrainingModeMenuSpeedEnumStart,
	nSC1PTrainingModeMenuSpeedFull = nSC1PTrainingModeMenuSpeedEnumStart,
	nSC1PTrainingModeMenuSpeed2Thirds,
	nSC1PTrainingModeMenuSpeedHalf,
	nSC1PTrainingModeMenuSpeedQuarter,
	nSC1PTrainingModeMenuSpeedEnumCount

} SC1PTrainingModeSpeed;

typedef enum SC1PTrainingModeView
{
	nSC1PTrainingModeMenuViewEnumStart,
	nSC1PTrainingModeMenuViewCloseUp = nSC1PTrainingModeMenuViewEnumStart,
	nSC1PTrainingModeMenuViewNormal,
	nSC1PTrainingModeMenuViewEnumCount

} SC1PTrainingModeView;

typedef enum SC1PTrainingModeMenuOptionSprites
{
	nSC1PTrainingModeMenuOptionSpriteItemStart,
	nSC1PTrainingModeMenuOptionSpriteItemNone = nSC1PTrainingModeMenuOptionSpriteItemStart,
	nSC1PTrainingModeMenuOptionSpriteItemMaximTomato,
	nSC1PTrainingModeMenuOptionSpriteItemHeart,
	nSC1PTrainingModeMenuOptionSpriteItemStar,
	nSC1PTrainingModeMenuOptionSpriteItemBeamSword,
	nSC1PTrainingModeMenuOptionSpriteItemHomeRunBat,
	nSC1PTrainingModeMenuOptionSpriteItemFan,
	nSC1PTrainingModeMenuOptionSpriteItemStarRod,
	nSC1PTrainingModeMenuOptionSpriteItemRayGun,
	nSC1PTrainingModeMenuOptionSpriteItemFireFlower,
	nSC1PTrainingModeMenuOptionSpriteItemHammer,
	nSC1PTrainingModeMenuOptionSpriteItemMotionSensorBomb,
	nSC1PTrainingModeMenuOptionSpriteItemBobomb,
	nSC1PTrainingModeMenuOptionSpriteItemBumper,
	nSC1PTrainingModeMenuOptionSpriteItemGreenShell,
	nSC1PTrainingModeMenuOptionSpriteItemRedShell,
	nSC1PTrainingModeMenuOptionSpriteItemPokeBall,
	nSC1PTrainingModeMenuOptionSpriteItemEnd = nSC1PTrainingModeMenuOptionSpriteItemPokeBall,

	nSC1PTrainingModeMenuOptionSpriteSpeedStart,
	nSC1PTrainingModeMenuOptionSpriteSpeedFull = nSC1PTrainingModeMenuOptionSpriteSpeedStart,
	nSC1PTrainingModeMenuOptionSpriteSpeed2Thirds,
	nSC1PTrainingModeMenuOptionSpriteSpeedHalf,
	nSC1PTrainingModeMenuOptionSpriteSpeedQuarter,
	nSC1PTrainingModeMenuOptionSpriteSpeedEnd = nSC1PTrainingModeMenuOptionSpriteSpeedQuarter,

	nSC1PTrainingModeMenuOptionSpriteCPStart,
	nSC1PTrainingModeMenuOptionSpriteCPStand = nSC1PTrainingModeMenuOptionSpriteCPStart,
	nSC1PTrainingModeMenuOptionSpriteCPWalk,
	nSC1PTrainingModeMenuOptionSpriteCPEvade,
	nSC1PTrainingModeMenuOptionSpriteCPJump,
	nSC1PTrainingModeMenuOptionSpriteCPAttack,
	nSC1PTrainingModeMenuOptionSpriteCPEnd = nSC1PTrainingModeMenuOptionSpriteCPAttack,

	nSC1PTrainingModeMenuOptionSpriteViewStart,
	nSC1PTrainingModeMenuOptionSpriteViewNormal = nSC1PTrainingModeMenuOptionSpriteViewStart,
	nSC1PTrainingModeMenuOptionSpriteViewCloseUp,
	nSC1PTrainingModeMenuOptionSpriteViewEnd = nSC1PTrainingModeMenuOptionSpriteViewCloseUp,

	nSC1PTrainingModeMenuOptionSpriteIndicatorStart,
	nSC1PTrainingModeMenuOptionSpriteLeftArrow = nSC1PTrainingModeMenuOptionSpriteIndicatorStart,
	nSC1PTrainingModeMenuOptionSpriteRightArrow,
	nSC1PTrainingModeMenuOptionSpriteCursor, 	// Red orb + underline

	nSC1PTrainingModeMenuOptionSpriteEnumCount

} SC1PTrainingModeMenuOptionSprites;

typedef enum SCStaffrollCompany
{
	nSCStaffrollCompanyNull = -1,
	nSCStaffrollCompanyHAL,
	nSCStaffrollCompanyNINTENDO,
	nSCStaffrollCompanyCreatures,
	nSCStaffrollCompanyGAMEFREAK,
	nSCStaffrollCompanyRare,
	nSCStaffrollCompanyMickeys,
	nSCStaffrollCompanyKENProd,
	nSCStaffrollCompanyAONIProd,
	nSCStaffrollCompanyARTSVISION,
	nSCStaffrollCompanyEZAKIProd,
	nSCStaffrollCompanyNOA

} SCStaffrollCompany;

typedef struct SC1PTrainingModeMenu				SC1PTrainingModeMenu;
typedef struct SC1PGameComputer				SC1PGameComputer;
typedef struct SC1PGameStage				SC1PGameStage;
typedef struct SC1PGameFighter				SC1PGameFighter;
typedef struct SC1PGameStats				SC1PGameStats;
typedef struct SC1PGameBossPlan				SC1PGameBossPlan;
typedef struct SC1PGameBossAnim				SC1PGameBossAnim;
typedef struct SC1PGameBossEffect			SC1PGameBossEffect;
typedef struct SC1PGameBossWallpaper		SC1PGameBossWallpaper;
typedef struct SC1PGameBossMain				SC1PGameBossMain;
typedef struct SC1PStageClearStats			SC1PStageClearStats;
typedef struct SC1PStageClearScore			SC1PStageClearScore;
typedef struct SC1PTrainingModeSprites		SC1PTrainingModeSprites;
typedef struct SC1PTrainingModeFiles		SC1PTrainingModeFiles;
typedef struct SCStaffrollMatrix			SCStaffrollMatrix;
typedef struct SCStaffrollText				SCStaffrollText;
typedef struct SCStaffrollSprite			SCStaffrollSprite;
typedef struct SCStaffrollStaff				SCStaffrollStaff;
typedef struct SCStaffrollName				SCStaffrollName;
typedef struct SCStaffrollJob				SCStaffrollJob;
typedef struct SCStaffrollSetup				SCStaffrollSetup;
typedef struct SCStaffrollProjection		SCStaffrollProjection;
typedef struct SCExplainMain				SCExplainMain;
typedef struct SCExplainArgs				SCExplainArgs;
typedef struct SCExplainPhase				SCExplainPhase;
typedef struct SCAutoDemoProc				SCAutoDemoProc;
typedef struct SCBattleResults				SCBattleResults;
typedef struct SCPlayerData					SCPlayerData;
typedef struct SCBattleState				SCBattleState;
typedef struct SCCommonData					SCCommonData;

#endif
