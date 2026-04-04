#ifndef _GMSOUND_H_
#define _GMSOUND_H_

#include <ssb_types.h>

typedef struct alSoundEffect
{
	void* unk_0x0;
	void* unk_0x4;
	void* unk_0x8;
	void* unk_0xC;
	u16 unk_0x10;
	u16 unk_0x12;
	u16 unk_0x14;
	u16 unk_0x16;
	u16 unk_0x18;
	u16 unk_0x1A;
	u16 unk_0x1C;
	u16 unk_0x1E;
	u16 unk_0x20;
	u16 unk_0x22;
	u16 unk_0x24;
	u16 sfx_id;
	u16 sfx_max; // End of SFX index list
	u8 filler_0x2A[0x2F - 0x2A];
	u8 balance;

} alSoundEffect;

typedef enum gmMusicID
{
	nSYAudioBGMPupupu,						// Dream Land
	nSYAudioBGMZebes,						// Planet Zebes
	nSYAudioBGMInishie,						// Mushroom Kingdom
	nSYAudioBGMInishieHurry,				// Mushroom Kingdom 20-second countdown warning
	nSYAudioBGMSector,						// Sector Z
	nSYAudioBGMJungle,						// Kongo Jungle
	nSYAudioBGMCastle,						// Peach's Castle
	nSYAudioBGMYamabuki,					// Saffron City
	nSYAudioBGMYoster,						// Yoshi's Island
	nSYAudioBGMHyrule,						// Hyrule Castle
	nSYAudioBGMBattleSelect,				// Character / Stage Select
	nSYAudioBGMWinDefault,					// Default victory fanfare? Used in default switch case for results screen winner theme
	nSYAudioBGMWinMario,					// Mario Victory
	nSYAudioBGMWinMetroid,					// Samus Victory
	nSYAudioBGMWinDonkey,					// Donkey Kong Victory
	nSYAudioBGMWinKirby,					// Kirby Victory
	nSYAudioBGMWinFox,						// Fox Victory
	nSYAudioBGMWinMother,					// Ness Victory
	nSYAudioBGMWinYoshi,					// Yoshi Victory
	nSYAudioBGMWinFZero,					// Captain Falcon Victory
	nSYAudioBGMWinPMonsters,				// Pokémon Victory
	nSYAudioBGMWinZelda,					// Link Victory
	nSYAudioBGMResults,						// VS Mode Results Screen
	nSYAudioBGMBossStage,					// Final Stage Card (VS. Master Hand)
	nSYAudioBGMBossEntry,					// Master Hand Appears
	nSYAudioBGMLast,						// Final Destination
	nSYAudioBGM1PBonusStage,				// Bonus Stage
	nSYAudioBGM1PStageClear,				// 1P Game Stage Cleared
	nSYAudioBGM1PBonusStageClear,			// Bonus Stage Cleared
	nSYAudioBGM1PGameClear,					// Final Stage Cleared
	nSYAudioBGM1PBonusStageFailure,			// Bonus Stage Failed
	nSYAudioBGM1PGameEndChoice,				// Continue or Game Over?
	nSYAudioBGM1PGameOver,					// Game Over
	nSYAudioBGMOpening,						// Opening Movie
	nSYAudioBGMExplain,						// How to Play
	nSYAudioBGM1PIntro,						// 1P Game Stage Cards
	nSYAudioBGMZako,						// Battlefield
	nSYAudioBGMMetal,						// Meta Crystal
	nSYAudioBGMEnding,						// Ending Movie
	nSYAudioBGMStaffroll,					// Credits
	nSYAudioBGMMessage,						// Secret Found
	nSYAudioBGM1PChallenger,					// Challenger Approaching
	nSYAudioBGMTrainingMode,				// Training Mode
	nSYAudioBGMData,						// Character Data / VS Records
	nSYAudioBGMModeSelect,					// Main Menu
	nSYAudioBGMHammer,            			// Hammer
    nSYAudioBGMStar,                   		// Star
	nSYAudioBGMEnd

} gmMusicID;

typedef enum gmFGMVoiceID
{
	nSYAudioFGMExplodeS,
	nSYAudioFGMExplodeL,
	nSYAudioFGMFireShoot1,
	nSYAudioFGMUnkShoot1,
	nSYAudioFGMUnkDial1,
	nSYAudioFGMChargeShotAll,
	nSYAudioFGMUnkSmallPing1,
	nSYAudioFGMFoxBlaster,
	nSYAudioFGMSamusJump1,
	nSYAudioFGMDonkeySlap1,
	nSYAudioFGMDonkeySlap2,
	nSYAudioFGMEscape,		// Roll
	nSYAudioFGMDeadUpStar, // Star KO spark
	nSYAudioFGMGuardOn,	// Put up shield
	nSYAudioFGMGuardOff,
	nSYAudioFGMShieldBreak,
	nSYAudioFGMShieldBreakPitchLw,
	nSYAudioFGMHeavySwing1,
	nSYAudioFGMLightSwingLw1,
	nSYAudioFGMCatch, // Grab
	nSYAudioFGMDoorClose,
	nSYAudioFGMTrainingSel,
	nSYAudioFGMShockL,
	nSYAudioFGMShockM,
	nSYAudioFGMShockS,
	nSYAudioFGMBurnL,
	nSYAudioFGMBurnE, // Burn empty (empty Fire Flower)
	nSYAudioFGMBurnM,
	nSYAudioFGMBurnS,
	nSYAudioFGMShockML,
#if defined(REGION_US)
	nSYAudioFGMFirefox,
#endif
	nSYAudioFGMKickL,
	nSYAudioFGMKickM,
#if defined(REGION_US)
	nSYAudioFGMDeadFall, // Plays when dying in a blast zone?
#endif
	nSYAudioFGMKickS,
#if defined(REGION_US)
	nSYAudioFGMUnkTap1,
#endif
	nSYAudioFGMFireShoot2,
	nSYAudioFGMPunchL,
	nSYAudioFGMPunchM,
#if defined(REGION_US)
	nSYAudioFGMUnkSlam1,
#endif
	nSYAudioFGMPunchS,
	nSYAudioFGMLightSwingL,
	nSYAudioFGMLightSwingM,
	nSYAudioFGMLightSwingS,
	nSYAudioFGMMSBombAttach,
	nSYAudioFGMBombHeiFuse,
	nSYAudioFGMItemMapCollide, // Item falls on ground
	nSYAudioFGMBumperHit,
	nSYAudioFGMFireFlowerBurn,
	nSYAudioFGMItemGet,
	nSYAudioFGMHammerSwing,
	nSYAudioFGMHarisenHit,
	nSYAudioFGMBatHit,
	nSYAudioFGMStarMapCollide,
	nSYAudioFGMStarGet,
	nSYAudioFGMBombHeiWalkStart,
	nSYAudioFGMShellHit,
	nSYAudioFGMItemThrow,
	nSYAudioFGMItemSpawn1,
	nSYAudioFGMContainerSmash,
	nSYAudioFGMFireFlowerShoot,
	nSYAudioFGMLGunShoot,
	nSYAudioFGMLGunEmpty,
	nSYAudioFGMStarRodSwing4,
	nSYAudioFGMStarRodSwing3,
	nSYAudioFGMStarRodSwing1,
	nSYAudioFGMStarRodEmpty,
	nSYAudioFGMSwordSwing4,
	nSYAudioFGMSwordSwing3,
	nSYAudioFGMSwordSwing1,
	nSYAudioFGMTaruBombHit, // Hit entity?
	nSYAudioFGMTaruBombMap, // Collide with wall?
	nSYAudioFGMDonkeyLanding,
	nSYAudioFGMCaptainLanding,
	nSYAudioFGMFoxLanding,
	nSYAudioFGMKirbyLanding,
	nSYAudioFGMLinkLanding,
	nSYAudioFGMMarioLanding,
	nSYAudioFGMNessLanding,
	nSYAudioFGMPikachuLanding,
	nSYAudioFGMPurinLanding,
	nSYAudioFGMSamusLanding,
	nSYAudioFGMYoshiLanding,
	nSYAudioFGMUnkGrind2,
	nSYAudioFGMUnkGrind3,
	nSYAudioFGMUnkGrind4,
	nSYAudioFGMInflateJump1,
	nSYAudioFGMUnkGrind5,
	nSYAudioFGMSamusJump2,
	nSYAudioFGMUnkNessPSI,
	nSYAudioFGMInflateJump2,
	nSYAudioFGMInflateJump3,
	nSYAudioFGMSamusJumpAerial,
	nSYAudioFGMInflateJump4,
	nSYAudioFGMKirbyPurinJump,
	nSYAudioFGMGroundGrind1,
	nSYAudioFGMGroundGrind2,
	nSYAudioFGMInflateJump5,
	nSYAudioFGMGroundGrind3,
	nSYAudioFGMInflateJump6,
	nSYAudioFGMNessJump,
	nSYAudioFGMInflateJump7,
	nSYAudioFGMInflateJump8,
	nSYAudioFGMGroundGrind4,
	nSYAudioFGMInflateJump9,
	nSYAudioFGMDonkeyFoot, // Might be footsteps? They are very quiet
	nSYAudioFGMCaptainFoot,
	nSYAudioFGMFoxFoot,
	nSYAudioFGMKirbyFoot,
	nSYAudioFGMLinkFoot,
	nSYAudioFGMMarioFoot,				// Also Luigi
	nSYAudioFGMNessFoot,
	nSYAudioFGMPikachuFoot,
	nSYAudioFGMPurinFoot,
	nSYAudioFGMSamusFoot,
	nSYAudioFGMYoshiFoot,
	nSYAudioFGMDonkeyDash,
	nSYAudioFGMCaptainDash,
	nSYAudioFGMFoxDash,
	nSYAudioFGMKirbyDash,
	nSYAudioFGMLinkDash,
	nSYAudioFGMMarioDash,				// Also Luigi
	nSYAudioFGMMMarioFoot,				// Metal Mario footstep
	nSYAudioFGMMMarioDash, 				// Unconfirmed, Metal Mario uses the above ID 122 for all foot sounds
	nSYAudioFGMNessDash,
	nSYAudioFGMPikachuDash,
	nSYAudioFGMPurinDash,
	nSYAudioFGMSamusDash,
	nSYAudioFGMGroundBrakeGrind,
	nSYAudioFGMUnkEchoHit1,
	nSYAudioFGMYoshiDash,
	nSYAudioFGMDogasSmog,
	nSYAudioFGMIwarkRockMake,
	nSYAudioFGMKabigonFall,
	nSYAudioFGMKabigonJump, // ???
	nSYAudioFGMKamexHydro,
	nSYAudioFGMLizardonFlame,
	nSYAudioFGMMewFly,
	nSYAudioFGMNyarsCoin,
	nSYAudioFGMMBallOpen,
	nSYAudioFGMMonsterShoot, // Venusaur and Starmie firing sound effect
	nSYAudioFGMTosakintoSplash,
#if defined(REGION_US)
	nSYAudioFGMDairantouKickL,
	nSYAudioFGMDairantouKickM,
	nSYAudioFGMDairantouKickS,
	nSYAudioFGMDairantouPunchL,
	nSYAudioFGMDairantouPunchM,
	nSYAudioFGMDairantouPunchS,
#endif
	nSYAudioFGMOpeningSectorAmbient,
	nSYAudioFGMOpeningNewcomersClash,
	nSYAudioFGMPublicPrologue,		// Cheer on Meta Crystal and Duel Zone before the music starts playing
	nSYAudioFGMOpeningClash,			
	nSYAudioFGMOpeningBatM,				// Plays on characters clashing scene in opening?
	nSYAudioFGMAltitudeWarn,			// Bottom map_bound warning sound?
	nSYAudioFGMDeadExplodeL, 			// One of many sounds that play when entering the blast zones
	nSYAudioFGMDeadExplodeS,
	nSYAudioFGMUnkMechanical1, 			// Menu sound?
	nSYAudioFGMTitlePressStart,
	nSYAudioFGMMenuSelect,
	nSYAudioFGMStageSelect,
	nSYAudioFGM1PGameContinue,
	nSYAudioFGM1PGameContinueMulti,
	nSYAudioFGMTrainingSel2,
	nSYAudioFGMMenuScroll1, 		// Scroll vertically?
	nSYAudioFGMMenuScroll2, 		// Scroll horizontally / vertically in Training Mode
	nSYAudioFGMMenuDenied,
	nSYAudioFGMPlayerSlotClose,  	// Plays when player's character portrait is closed on the CSS?
	nSYAudioFGMPlayerSlotWhoosh, 	// Might be unrelated
	nSYAudioFGMScoreDisplayBonus,
	nSYAudioFGMStageClearScoreRegister,
	nSYAudioFGMStageClearScoreDisplay,
	nSYAudioFGMUnkSwoosh1,
	nSYAudioFGMBossDefeatL,
	nSYAudioFGMUnkGate1,
	nSYAudioFGMBossBullet,
	nSYAudioFGMBossSlam,
	nSYAudioFGMBossUnk1,
	nSYAudioFGMBossUnk2,
	nSYAudioFGMDonkeySpin,	  		// Spinning kong swing sound effect
	nSYAudioFGMDonkeyCharge, 		// These two might be swapped
	nSYAudioFGMCaptainAppearCar1,
	nSYAudioFGMCaptainAppearCar2,
	nSYAudioFGMCaptainSpecialHi,
	nSYAudioFGMCaptainSpecialNStart,
	nSYAudioFGMCaptainSpecialNPunch,
	nSYAudioFGMFoxSpecialN, 		// Blaster again???
	nSYAudioFGMFoxSpecialHiStart,
	nSYAudioFGMFoxSpecialHiFly,
	nSYAudioFGMFoxSpecialLwHit,
	nSYAudioFGMFoxSpecialLwStart,
	nSYAudioFGMFoxAttackAirLw,
	nSYAudioFGMFoxAppearArwing,
	nSYAudioFGMUnkMechanical4,
	nSYAudioFGMUnkLongWind,
	nSYAudioFGMKirbySpecialLwLanding,
	nSYAudioFGMKirbyAttackAirHi,
	nSYAudioFGMKirbySpecialNThrow,
	nSYAudioFGMKirbySpecialNCopyEat,
	nSYAudioFGMKirbySpecialNCopyThrow,
	nSYAudioFGMKirbySpecialNCopyUnk,
	nSYAudioFGMKirbyStarPing1,
	nSYAudioFGMKirbyStarPing2,
	nSYAudioFGMKirbySpecialLwStart,
	nSYAudioFGMKirbySpecialNStart,
	nSYAudioFGMKirbySpecialNLoseCopy,
	nSYAudioFGMLinkSpecialLwGet,
	nSYAudioFGMLinkSpecialNReturn,
	nSYAudioFGMLinkSpecialNShoot,
	nSYAudioFGMLinkSpecialNGet,
	nSYAudioFGMLinkSpecialHi,
	nSYAudioFGMLinkCatchHookshot,
	nSYAudioFGMLinkAppear,
	nSYAudioFGMMarioAppealGrow,
	nSYAudioFGMMarioAppealShrink,
	nSYAudioFGMMarioDokan,
	nSYAudioFGMMarioSpecialN,
	nSYAudioFGMMarioSpecialHiCoin,
	nSYAudioFGMMarioSpecialHiJump,
	nSYAudioFGMMarioUnkSwing1, 		// Down air or down special?
	nSYAudioFGMMarioUnkSwing2,
	nSYAudioFGMCharacterUnk1,
	nSYAudioFGMNessPKThunderLoop,
	nSYAudioFGMNessPSI,
	nSYAudioFGMNessPSIAgain, 		// Huh
	nSYAudioFGMNessSpecialLwStart,
	nSYAudioFGMPikachuElectric1, // Might be Pikachu's Thunder Jolt
	nSYAudioFGMPikachuElectric2,
	nSYAudioFGMPikachuElectric3,
	nSYAudioFGMPikachuElectric4,
	nSYAudioFGMPikachuElectric5,
	nSYAudioFGMPikachuElectricLoop,
	nSYAudioFGMPikachuSpecialHiStart,
	nSYAudioFGMPikachuSpecialLwThunder,
	nSYAudioFGMCharacterUnk2,
	nSYAudioFGMCharacterUnk3,
	nSYAudioFGMSamusSpecialNShootF,
	nSYAudioFGMSamusSpecialNShootL,
	nSYAudioFGMSamusSpecialNShootM,
	nSYAudioFGMSamusSpecialNShootS,
	nSYAudioFGMSamusSpecialNCharge0,
	nSYAudioFGMSamusSpecialNCharge1,
	nSYAudioFGMSamusSpecialNCharge2,
	nSYAudioFGMSamusSpecialNCharge3,
	nSYAudioFGMSamusSpecialNCharge4,
	nSYAudioFGMSamusSpecialNCharge5,
	nSYAudioFGMSamusSpecialNCharge6,
	nSYAudioFGMSamusSpecialNCharge7,
	nSYAudioFGMSamusSpecialLw,
	nSYAudioFGMSamusCatchGrappleBeam,
	nSYAudioFGMSamusSpecialHi,
	nSYAudioFGMSamusUnkSwing,
	nSYAudioFGMSamusUnkCharge,
	nSYAudioFGMYoshiEggShatter1,
	nSYAudioFGMYoshiEggShatter2,
	nSYAudioFGMYoshiSpecialNTongue,
	nSYAudioFGMYoshiEggShatter3,
	nSYAudioFGMYoshiSpecialHiThrow,
	nSYAudioFGMYoshiEggLayShatter,
	nSYAudioFGMBladeSwing4,
	nSYAudioFGMBladeSwing3,
	nSYAudioFGMBladeSwing1,
	nSYAudioFGMSlashL,
	nSYAudioFGMSlashM,
	nSYAudioFGMSlashS,
	nSYAudioFGMBladeDraw,
	nSYAudioFGMSectorArwingLaser,
	nSYAudioFGMSectorAmbient1,
	nSYAudioFGMSectorAmbient2,
	nSYAudioFGMSectorAmbient3,
	nSYAudioFGMOptionBackupClear,
	nSYAudioFGMYamabukiGate, 				// Empty?
	nSYAudioFGMMagnify,
	nSYAudioFGMShinePingUnk,
	nSYAudioFGMBonusComplete,
	nSYAudioFGMPlayerHeal, 					// High-pitched short click, sped up Mario Fireball?
	nSYAudioFGMYosterCloudVapor,
	nSYAudioFGMStockSteal,
	nSYAudioFGMBonus2PlatformLanding, 		// Land on Bonus 2 platform
	nSYAudioFGMGamePause,
	nSYAudioFGMInishiePowerBlock, 			// POW Block
	nSYAudioFGMBonus1TargetBreak, 			// Break Bonus 1 target
    nSYAudioFGMJungleTaruCannShoot,
    nSYAudioFGMJungleTaruCannEnter,
	nSYAudioFGMHyruleTwisterAppear,
	nSYAudioFGMHyruleTwisterTrapped,
	nSYAudioFGMPupupuWhispyWind, 			// Despawn?
	nSYAudioFGMFloorDamageFire,
	nSYAudioFGMDonkeyDeadSlam,		  		// 0x11F
	nSYAudioFGMCaptainDeadSlam,		  		// 0x120
	nSYAudioFGMFoxDeadSlam,			  		// 0x121
	nSYAudioFGMKirbyDeadSlam,		  		// 0x122
	nSYAudioFGMLinkDeadSlam,			  	// 0x123
	nSYAudioFGMMarioDeadSlam,		  		// 0x124
	nSYAudioFGMNessDeadSlam,			  	// 0x125
	nSYAudioFGMPikachuDeadSlam,		  		// 0x126
	nSYAudioFGMPurinDeadSlam,		  		// 0x127
	nSYAudioFGMSamusDeadSlam,		  		// 0x128
	nSYAudioFGMYoshiDeadSlam,		  		// 0x129
	nSYAudioFGMDonkeyDownBounce,			// 0x12A
	nSYAudioFGMCaptainDownBounce,		  	// 0x12B
	nSYAudioFGMFoxDownBounce,			  	// 0x12C
	nSYAudioFGMKirbyDownBounce,		  		// 0x12D
	nSYAudioFGMLinkDownBounce,			  	// 0x12E
	nSYAudioFGMMarioDownBounce,		  		// 0x12F
	nSYAudioFGMNessDownBounce,			  	// 0x130
	nSYAudioFGMPikachuDownBounce,		  	// 0x131
	nSYAudioFGMPurinDownBounce,		  		// 0x132
	nSYAudioFGMSamusDownBounce,		  		// 0x133
	nSYAudioFGMYoshiDownBounce,		  		// 0x134
	nSYAudioVoiceMBallDogasAppear,
	nSYAudioVoiceMBallIwarkAppear,
	nSYAudioVoiceMBallKabigonFall,
	nSYAudioVoiceMBallKabigonAppear,
	nSYAudioVoiceMBallKamexAppear,
	nSYAudioVoiceMBallLuckyAppear,
	nSYAudioVoiceMBallMewAppear,
	nSYAudioVoiceMBallPippiAppear,
	nSYAudioVoiceMBallLizardonAppear,
	nSYAudioVoiceMBallSawamuraAppear,
	nSYAudioVoiceMBallSawamuraKick,
	nSYAudioVoiceMBallSpearAppear,
	nSYAudioVoiceMBallSpearSwarm,
	nSYAudioVoiceMBallStarmieAppear,
	nSYAudioVoiceMBallTosakintoAppear,
	nSYAudioVoiceDonkeyFuraSleep,
	nSYAudioVoiceDonkeyAppeal,
	nSYAudioVoiceDonkeySmash1,
	nSYAudioVoiceDonkeySmash2,
	nSYAudioVoiceDonkeySmash3,
	nSYAudioVoiceDonkeySpecialN,
	nSYAudioVoiceDonkeyDeadUp,
	nSYAudioVoiceDonkeyFuraFura,
	nSYAudioVoiceDonkeyDamage,
	nSYAudioVoiceDonkeyDead1,
	nSYAudioVoiceDonkeyHeavyGet,
	nSYAudioVoiceDonkeyHeavyUnk,
	nSYAudioVoiceDonkeyDead2,
	nSYAudioVoiceCaptainAppeal,
	nSYAudioVoiceCaptainSpecialHi,
	nSYAudioVoiceCaptainSmash1,
	nSYAudioVoiceCaptainSmash2,
	nSYAudioVoiceCaptainSmash3,
	nSYAudioVoiceCaptainSmash4,
	nSYAudioVoiceCaptainFinalComeOn,
	nSYAudioVoiceCaptainSmash5, 		// Same as Smash 3 or 4?
	nSYAudioVoiceCaptainAttackS4,
	nSYAudioVoiceCaptainSpecialLw,
	nSYAudioVoiceCaptainSpecialNPunch,
	nSYAudioVoiceCaptainSpecialNFalcon,
	nSYAudioVoiceCaptainDeadUp,
	nSYAudioVoiceCaptainFuraFura,
	nSYAudioVoiceCaptainDamage,
	nSYAudioVoiceCaptainUnkPing1,
	nSYAudioVoiceCaptainJumpAerial, 	// ??? Same as Captain Falcon's airdodge in Melee
	nSYAudioVoiceCaptainHeavyGet,
	nSYAudioVoiceCaptainDead,
	nSYAudioVoiceCaptainFuraSleep,
	nSYAudioVoiceCaptainUnkQuick,
	nSYAudioVoiceCaptainUnkPing2,
	nSYAudioVoiceCaptainUnkPing3,
	nSYAudioVoiceFoxDeadUp,
	nSYAudioVoiceFoxAppeal,
	nSYAudioVoiceFoxSpecialHi,
	nSYAudioVoiceFoxJumpAerial,
	nSYAudioVoiceFoxEscape,
	nSYAudioVoiceFoxSelected,
	nSYAudioVoiceFoxFinalIke,
	nSYAudioVoiceFoxHeavyGet,			// HeavyGet?
	nSYAudioVoiceFoxWin, 				// Same as Appeal and Selected
	nSYAudioVoiceFoxOttotto,			// Hardly any different from JumpAerial
	nSYAudioVoiceFoxDead,
	nSYAudioVoiceFoxFuraSleep,
	nSYAudioVoiceFoxSmash1,
	nSYAudioVoiceFoxSmash2,
	nSYAudioVoiceFoxSmash3,
	nSYAudioVoiceFoxDamage,
	nSYAudioVoiceFoxFuraFura,
	nSYAudioVoiceKirbyAppeal,
	nSYAudioVoiceKirbySmash1,
	nSYAudioVoiceKirbySmash2,
	nSYAudioVoiceKirbySmash3,
	nSYAudioVoiceKirbyCopyLinkSpecialN,
	nSYAudioVoiceKirbyCopyPikachuSpecialN,
	nSYAudioVoiceKirbySpecialHi,
	nSYAudioVoiceKirbyCopyCaptainSpecialNFalcon,
	nSYAudioVoiceKirbyCopyCaptainSpecialNPunch,
	nSYAudioVoiceKirbyCopyDonkeySpecialN,
	nSYAudioVoiceKirbyCopyPurinSpecialN,
	nSYAudioVoiceKirbyDeadUp,
	nSYAudioVoiceKirbyFuraFura,
	nSYAudioVoiceKirbyDamage,
	nSYAudioVoiceKirbyUnkGrunt1,
	nSYAudioVoiceKirbyUnkGrunt2,
	nSYAudioVoiceKirbyHeavyGet,
	nSYAudioVoiceKirbyOttotto,
	nSYAudioVoiceKirbyCopyNessSpecialN,
	nSYAudioVoiceKirbyDead,
	nSYAudioVoiceKirbyFuraSleep,
	nSYAudioVoiceKirbySpecialLw,
	nSYAudioVoiceKirbyUnkPing,				// Is this even Kirby???
	nSYAudioVoiceLinkGrunt1,				// ???
	nSYAudioVoiceLinkSmash1,				// Also Selected?
	nSYAudioVoiceLinkSmash2,
	nSYAudioVoiceLinkSmash3,
	nSYAudioVoiceLinkSpecialHi,
	nSYAudioVoiceLinkDeadUp,
	nSYAudioVoiceLinkFuraFura,
	nSYAudioVoiceLinkDamage,
	nSYAudioVoiceLinkJump,
	nSYAudioVoiceLinkJumpAerial,
	nSYAudioVoiceLinkNull,					// No sound???
	nSYAudioVoiceLinkHeavyGet,
	nSYAudioVoiceLinkOttotto,
	nSYAudioVoiceLinkDead,
	nSYAudioVoiceLinkFuraSleep,
	nSYAudioVoiceLinkGrunt2,				// ???
	nSYAudioVoiceLuigiSmash1,
	nSYAudioVoiceLuigiSmash2,
	nSYAudioVoiceLuigiSmash3,
	nSYAudioVoiceLuigiSpecialLw,
	nSYAudioVoiceLuigiDeadUp,
	nSYAudioVoiceLuigiFuraFura,
	nSYAudioVoiceLuigiDamage,
	nSYAudioVoiceLuigiJump,
	nSYAudioVoiceLuigiJumpAerial,
	nSYAudioVoiceLuigiLets,					// Let's-a-go, unused
	nSYAudioVoiceLuigiHeavyGet,
	nSYAudioVoiceLuigiDead,					// Pretty quiet and obscured by the echo and slam sound effects
	nSYAudioVoiceLuigiHereWe,				// Here we go, unused
	nSYAudioVoiceMarioSmash1,
	nSYAudioVoiceMarioSmash2,
	nSYAudioVoiceMarioSmash3,
	nSYAudioVoiceMarioSpecialLw,
	nSYAudioVoiceMarioDeadUp,
	nSYAudioVoiceMarioFuraFura,
	nSYAudioVoiceMarioJump,
	nSYAudioVoiceMarioJumpAerial,
	nSYAudioVoiceMarioLets,					// Let's-a-go, unused
	nSYAudioVoiceMarioHeavyGet,
	nSYAudioVoiceMarioDead,
	nSYAudioVoiceMarioDamage,
	nSYAudioVoiceMarioHereWe,
	nSYAudioVoiceNessAppeal,				// O K E Y
	nSYAudioVoiceNessSmash1,
	nSYAudioVoiceNessSmash2,
	nSYAudioVoiceNessSmash3,
	nSYAudioVoiceNessUnkGrunt,				// Unused grunt
	nSYAudioVoiceNessDeadUp,
	nSYAudioVoiceNessFuraFura,
	nSYAudioVoiceNessDamage,
	nSYAudioVoiceNessUnkPing,				// ???
	nSYAudioVoiceNessHeavyGet,
	nSYAudioVoiceNessOttotto,
	nSYAudioVoiceNessSpecialN,
	nSYAudioVoiceNessFinalPK,				// Unused
	nSYAudioVoiceNessFinalStarstorm,		// Unused
	nSYAudioVoiceNessSpecialHi,
	nSYAudioVoiceNessDead,
	nSYAudioVoiceNessFuraSleep,
	nSYAudioVoiceAnnounceComplete,
	nSYAudioVoiceAnnounceFailure,
	nSYAudioVoiceAnnouncePurinUnused,		// ??? Says Jigglypuff, Purin is only for consistency with FTKind names
	nSYAudioVoiceAnnounceMMario,
	nSYAudioVoiceAnnounceTitleWait,			// ...Super... SMAAAAAAAAAASH Brotheeeeeeeeers!! (long wait at beginning?)
	nSYAudioVoiceAnnounceNewRecord,
#if defined(REGION_US)
	nSYAudioVoiceAnnounceCongra,			// Congratulations
	nSYAudioVoiceAnnounceIncredible,
#endif
	nSYAudioVoiceAnnounceOne,
	nSYAudioVoiceAnnounceDenied1,			// Trying to start a game with not enough players?
	nSYAudioVoiceAnnounceTwo,
	nSYAudioVoiceAnnounceThree,
	nSYAudioVoiceAnnounceFour,
	nSYAudioVoiceAnnounceFive,
	nSYAudioVoiceAnnounceDenied2,			// Again?
	nSYAudioVoiceAnnounceDenied3,			// Seriously?
	nSYAudioVoiceAnnounceBlueTeam,
	nSYAudioVoiceAnnounceBoardThePlatforms,
	nSYAudioVoiceAnnounceBonusStage,
	nSYAudioVoiceAnnounceBreakTheTargets,
	nSYAudioVoiceAnnounceSelectPlayer,		// Choose your character
	nSYAudioVoiceAnnounceComputerPlayer,
	nSYAudioVoiceAnnounceContinue,
	nSYAudioVoiceAnnounceZako,				// Fighting Polygon Team
	nSYAudioVoiceAnnounceDonkey,
	nSYAudioVoiceAnnounceDrawGame,			// Wat
	nSYAudioVoiceAnnounceCaptain,
	nSYAudioVoiceAnnounceFox,
	nSYAudioVoiceAnnounceGameOver,
	nSYAudioVoiceAnnounceGameSet,
	nSYAudioVoiceAnnounceGDonkey,			// Giant Donkey Kong
	nSYAudioVoiceAnnounceGo,
	nSYAudioVoiceAnnounceGreenTeam,
	nSYAudioVoiceBossDead,					// Master Hand defeated
	nSYAudioVoiceBossAppear,				// Master Hand appears
	nSYAudioVoiceAnnounceHowToPlay,
	nSYAudioVoiceAnnounceRaceToTheFinish,
	nSYAudioVoiceAnnounceKirby,
	nSYAudioVoiceAnnounceLink,
	nSYAudioVoiceAnnounceLuigi,
	nSYAudioVoiceAnnounceMario,
	nSYAudioVoiceAnnounceMarioBros,
	nSYAudioVoiceAnnounceNess,
	nSYAudioVoiceAnnounceNoContest,
	nSYAudioVoiceAnnouncePlayer1,
	nSYAudioVoiceAnnouncePlayer2,
	nSYAudioVoiceAnnouncePlayer3,
	nSYAudioVoiceAnnouncePlayer4,
	nSYAudioVoiceAnnouncePikachu,
	nSYAudioVoiceAnnouncePurin,				// Actual Jigglypuff announcer voice
	nSYAudioVoiceAnnounceAreYouReady,		// Unused
	nSYAudioVoiceAnnounceRedTeam,
	nSYAudioVoiceAnnounceDefeated,
	nSYAudioVoiceAnnounceFreeForAll,
	nSYAudioVoiceAnnounceSamus,
	nSYAudioVoiceAnnounceSuddenDeath,
	nSYAudioVoiceUnkRoboEcho1,				// All of these are unused
	nSYAudioVoiceUnkRoboEcho2,
	nSYAudioVoiceUnkRoboEcho3,
	nSYAudioVoiceUnkRoboEcho4,
	nSYAudioVoiceUnkRoboEcho5,
	nSYAudioVoiceUnkRoboEcho6,
	nSYAudioVoiceUnkRoboEcho7,
	nSYAudioVoiceUnkRoboEcho8,
	nSYAudioVoiceUnkRoboEcho9,
	nSYAudioVoiceUnkRoboEcho10,
	nSYAudioVoiceAnnounceFinalStage,		// Unused
	nSYAudioVoiceAnnounceTeamBattle,
	nSYAudioVoiceAnnounceTimeUp,
	nSYAudioVoiceAnnounceTitle,				// Title announcement that's actually used?
	nSYAudioVoiceAnnounceKirbyTeam,
	nSYAudioVoiceAnnounceTrainingMode,
	nSYAudioVoiceAnnounceYoshiTeam,
	nSYAudioVoiceAnnounceVersus,
	nSYAudioVoiceAnnounceWins,
	nSYAudioVoiceAnnounceWinnerIs,		// This game's winner is...
	nSYAudioVoiceAnnounceYoshi,
	nSYAudioVoicePikachuAppeal,
	nSYAudioVoicePikachuSmash1,
	nSYAudioVoicePikachuSmash2,
	nSYAudioVoicePikachuSmash3,
	nSYAudioVoicePikachuSpecialN,
	nSYAudioVoicePikachuSpecialLw,
	nSYAudioVoicePikachuDeadUp,
	nSYAudioVoicePikachuFuraFura,
	nSYAudioVoicePikachuDamage,
	nSYAudioVoicePikachuFinalPika,
	nSYAudioVoicePikachuFinalChu,
	nSYAudioVoicePikachuSpecialHi,
	nSYAudioVoicePikachuHeavyGet,
	nSYAudioVoicePikachuOttotto,
	nSYAudioVoicePikachuDead,
	nSYAudioVoicePikachuFuraSleep,
	nSYAudioVoiceYamabukiFushigibana,
	nSYAudioVoiceYamabukiHitokage,
	nSYAudioVoiceYamabukiLucky,
	nSYAudioVoiceYamabukiMarumine,
	nSYAudioVoiceYamabukiPorygon,
	nSYAudioVoicePurinAppeal,
	nSYAudioVoicePurinSmash1,
	nSYAudioVoicePurinSmash2,
	nSYAudioVoicePurinSmash3,
	nSYAudioVoicePurinSpecialN,
	nSYAudioVoicePurinDeadUp,
	nSYAudioVoicePurinFuraFura,
	nSYAudioVoicePurinDamage,
	nSYAudioVoicePurinUnkGrunt1,			// "Jiggly!"
	nSYAudioVoicePurinUnkGrunt2,			// "Jiggly!" again?
	nSYAudioVoicePurinUnkGrunt3,			// "Blah!"
	nSYAudioVoicePurinUnkGrunt4,			// "Jigglyyyy!"
	nSYAudioVoicePurinFuraSleep,
	nSYAudioVoicePurinSpecialLwSleep,		// "Hmm... hmm..."
	nSYAudioVoicePurinSpecialLwWake,		// "Jigglypuff!"
	nSYAudioVoicePurinSpecialHi,
	nSYAudioVoiceSamusSmash1,
	nSYAudioVoiceSamusSmash2,
	nSYAudioVoiceSamusSmash3,
	nSYAudioVoiceSamusDeadUp,
	nSYAudioVoiceSamusFura,					// Both FuraFura and FuraSleep
	nSYAudioVoiceSamusAttackHi4,			// Beep
	nSYAudioVoiceSamusUnkSlash,				// ???
	nSYAudioVoiceSamusAppeal,
	nSYAudioVoiceSamusDamage,
	nSYAudioVoiceSamusDead,
	nSYAudioVoiceYoshiAppeal,
	nSYAudioVoiceYoshiSmash1,
	nSYAudioVoiceYoshiSmash2,
	nSYAudioVoiceYoshiSmash3,
	nSYAudioVoiceYoshiCatch,				// "Yum!"
	nSYAudioVoiceYoshiDeadUp,
	nSYAudioVoiceYoshiFuraFura,
	nSYAudioVoiceYoshiDamage,
	nSYAudioVoiceYoshiJump,
	nSYAudioVoiceYoshiJumpAerial,
	nSYAudioVoiceYoshiHeavyGet,				// Same as JumpAerial?
	nSYAudioVoiceYoshiOttotto,
	nSYAudioVoiceYoshiDead,
	nSYAudioVoiceYoshiFuraSleep,
	nSYAudioVoiceYoshiUnkGrunt1,			// "Wah!"
	nSYAudioVoiceYoshiSpecialLwJump,		// "Prrng..."
	nSYAudioVoiceYoshiSpecialLwFall,		// "Hah!"
	nSYAudioVoiceYoshiUnkGrunt2,			// "Yoshi!"
	nSYAudioVoiceYoshiThrow,				// "Hum!"
	nSYAudioVoiceYoshiUnkVocalize,			// "Yoshiiiiii!"
	nSYAudioVoicePublicDonkey,			// 0x25B
	nSYAudioVoicePublicCaptain,		  	// 0x25C
	nSYAudioVoicePublicFox,			  	// 0x25D
	nSYAudioVoicePublicKirby,		  	// 0x25E
	nSYAudioVoicePublicLink,		  		// 0x25F
	nSYAudioVoicePublicLuigi,			// 0x260
	nSYAudioVoicePublicMario,		  	// 0x261
	nSYAudioVoicePublicNess,				// 0x262
	nSYAudioVoicePublicPikachu,		  	// 0x263
	nSYAudioVoicePublicPurin,		 	// 0x264
	nSYAudioVoicePublicSamus,		 	// 0x265
	nSYAudioVoicePublicYoshi,		  	// 0x266
	nSYAudioVoicePublicGaspL,			// Loud audience gasp when fighter recovers successfully
	nSYAudioVoicePublicGaspM,			// Medium audience gasp when a fighter recovers successfully
	nSYAudioVoicePublicGaspS,			// Slight audience gasp when a fighter recovers successfully
	nSYAudioVoicePublicCheer,			// Audience cheer (>= 160 knockback) + pressed start on CSS
	nSYAudioVoicePublicAmazed,			// Amazed at player performance (< 160 knockback)
	nSYAudioVoicePublicGaspClap,			// Audience gasps, then claps
	nSYAudioVoicePublicWin,				// "Static" reaction pre-winner announcement
	nSYAudioVoicePublicDamageL,			// React to fighter receiving heavy knockback
	nSYAudioVoicePublicDamageM,			// React to fighter receiving medium knockback
	nSYAudioVoicePublicNoContest,		// No contest?
	nSYAudioVoicePublicDamageS,			// React to fighter receiving light knockback
	nSYAudioVoicePublicExcited,			// React to winner announcement after game set / match begin
	nSYAudioVoicePublicAbsorb,			// React to fighter absorbing projectiles (Ness)
	nSYAudioVoicePublicClapL,			// Louder audience clap
	nSYAudioVoicePublicClapS,			// Slight audience clap
	nSYAudioFGMCharacterUnkZip1,			// ??? goes repeats for 10 more instances, character-based?
	nSYAudioFGMCharacterUnkZip2,
	nSYAudioFGMCharacterUnkZip3,
	nSYAudioFGMCharacterUnkZip4,
	nSYAudioFGMCharacterUnkZip5,
	nSYAudioFGMCharacterUnkZip6,
	nSYAudioFGMCharacterUnkZip7,
	nSYAudioFGMCharacterUnkZip8,
	nSYAudioFGMCharacterUnkZip9,
	nSYAudioFGMCharacterUnkZip10,
	nSYAudioFGMCharacterUnkZip11,
	nSYAudioVoiceUnkExplode,				// ???
	nSYAudioVoiceUnkSpin,					// ???
	nSYAudioVoiceUnkBurn,					// ???
	nSYAudioVoiceUnkSlashM,					// ???
	nSYAudioVoiceUnkSlashL,					// ???
	nSYAudioVoiceUnkSlashS1,				// ??? lower pitched than the above two
	nSYAudioVoiceUnkSlashS2,				// Similar to the above
	nSYAudioVoiceUnkShockL,					// ???
	nSYAudioVoiceUnkShockM,					// ???
	nSYAudioVoicePublicUnkGasp,			// ???
	// Dood I'm not gonna bother with these unless they're really used somehow
#if defined(REGION_US)
	nSYAudioFGMVoiceEnd = 0x2B7,
#else
	nSYAudioFGMVoiceEnd = 0x29D,
#endif
	nSYAudioFGMVoiceEnumCount = nSYAudioFGMVoiceEnd

} gmFGMVoiceID;

#endif
