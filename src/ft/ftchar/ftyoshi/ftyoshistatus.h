#ifndef _FTYOSHI_STATUS_H_
#define _FTYOSHI_STATUS_H_

#include <ft/fighter.h>

// 0x8012AEF8
FTStatusDesc dFTYoshiSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionAppearR,					// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 221 (0xDD): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionAppearL,					// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 222 (0xDE): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftYoshiSpecialHiProcPhysics,			// Proc Physics
		ftYoshiSpecialHiProcMap				// Proc Map
	},

	// Status 223 (0xDF): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialAirHi,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialAirHiProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftYoshiSpecialAirHiProcPhysics,		// Proc Physics
		ftYoshiSpecialAirHiProcMap			// Proc Map
	},

	// Status 224 (0xE0): SpecialLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialLwStart,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,			// Proc Physics
		ftYoshiSpecialLwStartProcMap			// Proc Map
	},

	// Status 225 (0xE1): SpecialLwLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialLwLanding,		// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialLwLandingProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 226 (0xE2): SpecialAirLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialAirLwStart,		// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,			// Proc Physics
		ftYoshiSpecialLwStartProcMap			// Proc Map
	},

	// Status 227 (0xE3): SpecialAirLwLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftYoshiSpecialAirLwLoopProcPhysics,	// Proc Physics
		ftYoshiSpecialAirLwLoopProcMap		// Proc Map
	},

	// Status 228 (0xE4): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialN,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftYoshiSpecialNProcMap				// Proc Map
	},

	// Status 229 (0xE5): SpecialNCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialNCatch,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialNCatchProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftYoshiSpecialNCatchProcMap			// Proc Map
	},

	// Status 230 (0xE6): SpecialNRelease
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialNRelease,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialNReleaseProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftYoshiSpecialNReleaseProcMap			// Proc Map
	},

	// Status 231 (0xE7): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialAirN,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftYoshiSpecialAirNProcMap				// Proc Map
	},

	// Status 232 (0xE8): SpecialAirNCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialAirNCatch,		// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialAirNCatchProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftYoshiSpecialAirNCatchProcMap		// Proc Map
	},

	// Status 233 (0xE9): SpecialAirNRelease
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTYoshiMotionSpecialAirNRelease,		// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftYoshiSpecialAirNReleaseProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftYoshiSpecialAirNReleaseProcMap		// Proc Map
	}
};

#endif