#ifndef _ITDEF_H_
#define _ITDEF_H_

// Macros

// Allocate this many item user_data structs at once
#define ITEM_ALLOC_MAX 16

// Perform initial collision check when spawning item?
#define ITEM_FLAG_COLLPROJECT (1 << 31)

// Item spawned by fighter
#define ITEM_FLAG_PARENT_FIGHTER 0

// Item spawned by stage
#define ITEM_FLAG_PARENT_GROUND 1

// Item spawned by weapon
#define ITEM_FLAG_PARENT_WEAPON 2

// Item spawned by another item
#define ITEM_FLAG_PARENT_ITEM 3

#define ITEM_FLAG_PARENT_DEFAULT 4

// Mask all GObj classes that can spawn items?
#define ITEM_MASK_PARENT 0xF

// Item is teamless; deals damage to any eligible target
#define ITEM_TEAM_DEFAULT 4
#define ITEM_PORT_DEFAULT GMCOMMON_PLAYERS_MAX
#define ITEM_HANDICAP_DEFAULT 9
#define ITEM_STALE_DEFAULT 1.0F
#define ITEM_THROW_DEFAULT 1.0F

// "Lifetime" while item is not being carried
#define ITEM_PICKUP_WAIT_DEFAULT 1400

// Maximum damage cap for reflected items
#define ITEM_REFLECT_MAX_DEFAULT 100

// Universal reflect damage multiplier
#if defined(REGION_US)
#define ITEM_REFLECT_MUL_DEFAULT 1.8F
#else
#define ITEM_REFLECT_MUL_DEFAULT 1.5F
#endif

// Added after multiplying item's hitbox damage
#define ITEM_REFLECT_ADD_DEFAULT 0.99F

// Item starts flashing rapidly once its pickup duration drops below this value
#define ITEM_DESPAWN_FLASH_BEGIN_DEFAULT 180U

// Red arrow pointing downward at item "blinks" at this frequency (45 frames visible, 45 frames invisible)
#define ITEM_ARROW_FLASH_INT_DEFAULT 45

#define ITEM_REHIT_TIME_DEFAULT 16
#define ITEM_ATKCOLL_NUM_MAX 2

#define ITEM_HOP_ANGLE_DEFAULT F_CST_DTOR32(135.0F)

#define ITEM_SPIN_SPEED_COMMON F_CST_DTOR32(18.0F)
#define ITEM_SPIN_SPEED_APPEAR_SLOW F_CLC_DTOR32(10.0F)
#define ITEM_SPIN_SPEED_APPEAR_FAST F_CST_DTOR32(16.0F)

#define ITEM_SPIN_SPEED_SMASH_THROW F_CST_DTOR32(-21.0F)
#define ITEM_SPIN_SPEED_NORMAL_THROW F_CLC_DTOR32(-10.0F)

// Also multiplies spin speed
#define ITEM_SPIN_SPEED_FRACTION_DEFAULT 0.01F

#define ITEM_TOGGLE_MASK_KIND(kind) (1 << (kind))

/* 
 * Maximum number of times item can be thrown/dropped before it is guaranteed to despawn
 * Default is 4 and caps at 7 due to being 3 bits wide
 */
#define ITEM_THROW_NUM_MAX 4

/*
 * Random chance for item to despawn when landing after being thrown/dropped
 * Guaranteed despawn beyond ITEM_THROW_NUM_MAX
 */
#define ITEM_THROW_DESPAWN_RANDOM 4

#define ITEM_LANDING_DESPAWN_CHECK 1
#define ITEM_LANDING_NUM_MAX 2

// Enums
typedef enum ITKind
{
	// Common items
	nITKindCommonStart, // Start of common item IDs

	nITKindContainerStart = nITKindCommonStart, 		// Start of container items
	nITKindBox = nITKindContainerStart,		  			// Crate
	nITKindTaru,										// Barrel
	nITKindCapsule,							  			// Capsule
	nITKindEgg,								  			// Egg
	nITKindContainerEnd = nITKindEgg,					// End of container items

	nITKindUtilityStart, 								// Start of utility items (or items that can be spawned in training mode / toggled)
	nITKindTomato = nITKindUtilityStart, 				// Maxim Tomato
	nITKindHeart,						   				// Heart
	nITKindStar,						   				// Star Man
	nITKindSword,						   				// Beam Sword
	nITKindBat,						   					// Home-Run Bat
	nITKindHarisen,					   					// Fan
	nITKindStarRod,					   					// Star Rod
	nITKindLGun,						   				// Ray Gun
	nITKindFFlower,					   					// Fire Flower
	nITKindHammer,						  	 			// Hammer
	nITKindMSBomb,						   				// Motion-Sensor Bomb
	nITKindBombHei,					   					// Bob-Omb
	nITKindNBumper,					   					// Bumper (Item)
	nITKindGShell,						   				// Green Shell
	nITKindRShell,						   				// Red Shell
	nITKindMBall,						   				// Poké Ball
	nITKindUtilityEnd = nITKindMBall,	   				// End of utility items

	nITKindCommonEnd = nITKindUtilityEnd, 				// End of common item IDs

	// Fighter items
	nITKindFighterStart,					   			// Start of fighter item IDs
	nITKindNessPKFire = nITKindFighterStart, 			// Ness PK Fire pillar
	nITKindLinkBomb,						   			// Link's Bomb
	nITKindFighterEnd = nITKindLinkBomb,	   			// End of fighter item IDs

	// Stage items
	nITKindGroundoundStart, 							// Start of stage hazard IDs

	nITKindPowerBlock = nITKindGroundoundStart, 		// POW block
	nITKindGBumper,						  				// Bumper (Stage Hazard)
	nITKindPakkun,							  			// Pirahna Plant
	nITKindTarget,							  			// Target
	nITKindTaruBomb,							  		// Race to the Finish bomb

	nITKindGroundMonsterStart,					 		// Start of stage Pokémon
	nITKindGLucky = nITKindGroundMonsterStart, 			// Chansey (Saffron City)
	nITKindMarumine,						 			// Electrode
	nITKindHitokage,						 			// Charmander
	nITKindFushigibana,					 				// Venusaur
	nITKindPorygon,						 				// Porygon
	nITKindGroundMonsterEnd = nITKindPorygon,	 		// End of stage Pokémon IDs

	nITKindGroundoundEnd = nITKindGroundMonsterEnd, 	// End of stage hazard IDs

	// Pokémon
	nITKindMBallMonsterStart,							// Start of Pokémon item IDs
	nITKindMBallCommonStart = nITKindMBallMonsterStart, // Start of Pokémon with a common appearance rate
	nITKindIwark = nITKindMBallMonsterStart, 			// Onix
	nITKindKabigon,										// Snorlax
	nITKindTosakinto,									// Goldeen
	nITKindNyars,										// Meowth
	nITKindLizardon,									// Charizard
	nITKindSpear,										// Beedrill
	nITKindKamex,										// Blastoise
	nITKindMLucky,										// Chansey (Poké Ball)
	nITKindStarmie,										// Starmie
	nITKindSawamura,									// Hitmonlee
	nITKindDogas,										// Koffing
	nITKindPippi,										// Clefairy
	nITKindMBallCommonEnd = nITKindPippi,				// End of Pokémon with a common appearance rate
	nITKindMew,											// Mew
	nITKindMBallMonsterEnd = nITKindMew,				// End of Pokémon item IDs

	nITKindEnumCount 										// End of all item IDs

} ITKind;

typedef enum ITType
{
	nITTypeDamage,	// Item has a hurtbox that can be damaged (?)
	nITTypeSwing,	// Item can be thrown and swung
	nITTypeShoot,	// Item can be fired
	nITTypeThrow,	// Item can only be thrown
	nITTypeTouch,	// Item has special properties on hitbox interaction
	nITTypeConsume, // Hammer, Heart and Maxim Tomato?
	nITTypeFighter	// Item spawned by fighter's weapon?

} ITType;

typedef enum ITWeight
{
	nITWeightHeavy, 	// Crate and barrel
	nITWeightLight	 	// Everything else lol

} ITWeight;

// Structs
typedef struct ITStruct 		ITStruct; 		// Main item struct
typedef struct ITAttributes 	ITAttributes;
typedef struct ITMonsterData 	ITMonsterData; 	// Info specific to summoning Pokémon
typedef struct ITDesc 	ITDesc;
typedef struct ITStatusDesc 	ITStatusDesc;
typedef struct ITAppearActor 	ITAppearActor;
typedef struct ITRandomWeights 	ITRandomWeights;
typedef struct ITAttackPos 		ITAttackPos;
typedef struct ITAttackColl 	ITAttackColl;
typedef struct ITAttackEvent 		ITAttackEvent;
typedef struct ITMonsterEvent 	ITMonsterEvent;
typedef struct ITDamageColl 	ITDamageColl;

#endif
