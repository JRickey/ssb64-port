#ifndef _EFDEF_H_
#define _EFDEF_H_

#define EFFECT_ALLOC_NUM 38
#define EFFECT_ALLOC_ALIGN 0x8

#define EFFECT_FLAG_USERDATA 0x2            // Effect GObj has an EFStruct user_data and update process
#define EFFECT_FLAG_SPECIALLINK 0x8         // Effect GObj has CommonLink array ID of 8 rather than the usual 6

typedef enum efKind
{
    nEFKindDamageNormal,               // Texture displayed when hit by an attack of the normal element
    nEFKindFlameLR = 6,                // Flame whose velocity depends on the user's facing direction
    nEFKindFlameRandom,                // Small flame texture that scatters
    nEFKindFlameStatic,                // Small flame texture that is stationary?
    nEFKindShockSmall = 10,            // Texture displayed when hit by an attack of the electric element; small shockwave texture
    nEFKindDustLight,                  // e.g. Light landing (no fast fall)
    nEFKindDustLightRapid,             // Faster version of DustLight?
    nEFKindDustHeavyDouble,            // Two instances of heavy dust clouds
    nEFKindDustHeavyDoubleRapid,       // Two instances of faster heavy dust clouds?
    nEFKindDustHeavy,                  // Single instance of heavy dust cloud
    nEFKindDustHeavyReverse,           // Single instance of direction-flipped heavy dust cloud
    nEFKindDustExpandLarge,            // Large dust cloud that grows in size
    nEFKindDustExpandSmall,            // Small dust cloud that grows in size
    nEFKindDustDashSmall,              // Small (actually pretty large) dust kickup behind user
    nEFKindDustDashLarge,              // Large dust kickup behind user
    nEFKindDamageFlyOrbs,              // Cyan orbs that fly outward
    nEFKindImpactWave,                 // Green impact shockwave that is created when a character slams into collision
    nEFKindStarRodSpark,               // Star Rod / Star projectile sparkles 
    nEFKindDamageFlySparks,            // Shows sometimes when getting hit
    nEFKindDamageFlySparksReverse,     // LR-inverted version
    nEFKindDamageFlyMDust,             // Metal dust effect, shows sometimes when getting hit
    nEFKindDamageFlyMDustReverse,      // LR-inverted version
    nEFKindSparkleWhite,               // Plays when a flame (Fire Flower, Charizard, Charmander) or when Pikachu's Thunder hits the ground
    nEFKindSparkleWhiteMultiExplode,   // e.g. Kirby's F-Throw explosion
    nEFKindSparkleWhiteMulti,          // I can't really trigger this but it's just SparkleWhiteMultiExplode without the explosion; seems to be exclusive to fighter scripts
    nEFKindSparkleWhiteScale,          // Most common white sparkle, plays during many moves such as Smash attacks
    nEFKindQuakeMag0,                  // Creates an earthquake of magnitude 0
    nEFKindQuakeMag1,                  // Creates an earthquake of magnitude 1
    nEFKindQuakeMag2,                  // Creates an earthquake of magnitude 2
    nEFKindFireSpark = 37,             // Not sure who uses this
    nEFKindFuraSparkle = 40,           // Dizzy sparkles after shield break
    nEFKindPsionic,                    // Ness PSI sparkles
    nEFKindFlashSmall,                 // Expanding green circle, plays when teching
    nEFKindFlashMiddle,                // Other expanding green circle, plays when grabbing ledge
    nEFKindFlashLarge,                 // Large expanding green circle, plays when grabbing Star Man?
    nEFKindBoxSmash = 46,              // Crate / Barrel smash effect
    nEFKindCrashTheGame,               // Crashes the game; this takes an argument which acts as a timer; probably extra specific, during a certain scene
    nEFKindKirbyStar = 54,             // Can be created manually but this plays when Kirby makes impact with stage collision
    nEFKindThunderAmp = 70,            // Pikachu's Thunder self-hit
    nEFKindRipple,                     // Pikachu's Quick-Attack, Ness's PSI Magnet, Mew spawn, etc.
    nEFKindChargeSparkle = 73,         // Fully charged Neutral Special, e.g. DK or Samus
    nEFKindHealSparkles,               // Used for Heart, Maxim Tomato and Star Man
    nEFKindYoshiEggEscape = 87,
    nEFKindMusicNote = 90,
    nEFKindEggBreak

} efKind;

typedef enum LBTransformStatus
{
    nLBTransformStatusDefault,
    nLBTransformStatusReady,
    nLBTransformStatusFinished
    
} LBTransformStatus;

typedef struct EFStruct			EFStruct;
typedef struct EFDesc		EFDesc;

#endif
