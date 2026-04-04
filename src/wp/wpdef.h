#ifndef _WPDEF_H_
#define _WPDEF_H_

// Macros
#define WEAPON_ALLOC_MAX 32 						// Allocate this many weapon user_data structs at once
#define WEAPON_ALLOC_ALIGN 0x8

#define WEAPON_STALE_DEFAULT 1.0F

#define WEAPON_TEAM_DEFAULT 4
#define WEAPON_PORT_DEFAULT GMCOMMON_PLAYERS_MAX
#define WEAPON_HANDICAP_DEFAULT 9

#define WEAPON_FLAG_DOBJDESC 0x1                   // Weapon has a DObj node tree that needs to be set up
#define WEAPON_FLAG_DOBJLINKS 0x2                   // Use DObjDLLink renderers or single DisplayList stream?
#define WEAPON_FLAG_COLLPROJECT (1 << 31)           // Perform initial collision check when spawning weapon?

#define WEAPON_FLAG_PARENT_FIGHTER 0                // Weapon spawned by fighter
#define WEAPON_FLAG_PARENT_GROUND 1	                // Weapon spawned by stage
#define WEAPON_FLAG_PARENT_WEAPON 2	                // Weapon spawned by another weapon
#define WEAPON_FLAG_PARENT_ITEM 3	                // Weapon spawned by Item

#define WEAPON_MASK_PARENT 0xF                   	// Mask all GObj classes that can spawn weapons?

#define WEAPON_ATKCOLL_NUM_MAX 2
#define WEAPON_REHIT_TIME_DEFAULT 16 // If the weapon is multihit, its hitbox will refresh per victim after this many frames have passed

#define WEAPON_REFLECT_TIME_DEFAULT 100	 // Maximum damage cap for reflected weapons
#if defined(REGION_US)
#define WEAPON_REFLECT_MUL_DEFAULT 1.8F	 // Universal reflect damage multiplier
#else
#define WEAPON_REFLECT_MUL_DEFAULT 1.5F	 // Universal reflect damage multiplier
#endif
#define WEAPON_REFLECT_ADD_DEFAULT 0.99F // Added after multiplying weapon's hitbox damage

#define WEAPON_STALE_ADD_DEFAULT 0.999F // Bonus 1% added after multiplying hitbox damage with staling modifier

#define WEAPON_HOP_ANGLE_DEFAULT F_CLC_DTOR32(135.0F) // 2.3561945F - Determines whether weapon bounces off a shield

// Enums
typedef enum WPKind
{
	nWPKindFireball,		   	// 0x00
	nWPKindBlaster,		   		// 0x01
	nWPKindChargeShot,		   	// 0x02
	nWPKindSamusBomb,		   	// 0x03
	nWPKindCutter,			   	// 0x04
	nWPKindEggThrow,		   	// 0x05
	nWPKindYoshiStar,		   	// 0x06
	nWPKindBoomerang,		   	// 0x07
	nWPKindSpinAttack,		   	// 0x08
	nWPKindThunderJoltAir,	   	// 0x09
	nWPKindThunderJoltGround, 	// 0x0A
	nWPKindThunderHead,	   		// 0x0B
	nWPKindThunderTrail,	   	// 0x0C
	nWPKindPKFire,			   	// 0x0D
	nWPKindPKThunderHead,	   	// 0x0E
	nWPKindPKThunderTrail,	   	// 0x0F
	nWPKindBulletNormal,  	   	// 0x10
	nWPKindBulletHard,	   	   	// 0x11
	nWPKindArwingLaser2D,	   	// 0x12
	nWPKindArwingLaser3D,	   	// 0x13
	nWPKindLGunAmmo,		   	// 0x14
	nWPKindFFlowerFlame,	   	// 0x15
	nWPKindStarRodStar,	   		// 0x16

	nWPKindMonsterStart,						  	// 0x17
	nWPKindIwarkRock = nWPKindMonsterStart,	  		// 0x17
	nWPKindNyarsCoin,							  	// 0x18
	nWPKindLizardonFlame,						  	// 0x19
	nWPKindSpearSwarm,							  	// 0x1A
	nWPKindKamexHydro,							  	// 0x1B
	nWPKindStarmieSwift,						  	// 0x1C
	nWPKindDogasSmog,							  	// 0x1D
	nWPKindHitokageFlame,						  	// 0x1E
	nWPKindFushigibanaRazor,					  	// 0x1F
	nWPKindMonsterEnd = nWPKindFushigibanaRazor 	// 0x1F

} WPKind;

// Structs
typedef struct WPStruct 		WPStruct;
typedef struct WPAttributes 	WPAttributes;
typedef struct WPDesc 	WPDesc;
typedef struct WPAttackPos	WPAttackPos;
typedef struct WPAttackColl 	WPAttackColl;

#endif
