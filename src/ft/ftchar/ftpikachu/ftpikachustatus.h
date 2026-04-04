#ifndef _FTPIKACHU_STATUS_H_
#define _FTPIKACHU_STATUS_H_

#include <ft/fighter.h>

// NOTE: Scuffed attack IDs, SpecialHi and SpecialLw are swapped

// 0x8012B010
FTStatusDesc dFTPikachuSpecialStatusDescs[/* */] = 
{
	// Status 220 (0xDC): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionAppearR,				// Script ID
		nFTMotionAttackIDNone,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,				// Proc Physics
		mpCommonUpdateFighterProjectFloor		// Proc Map
	},

	// Status 221 (0xDD): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionAppearL,				// Script ID
		nFTMotionAttackIDNone,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,				// Proc Physics
		mpCommonUpdateFighterProjectFloor		// Proc Map
	},

	// Status 222 (0xDE): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialN,				// Script ID
		nFTMotionAttackIDSpecialN,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,						// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPikachuSpecialNProcMap				// Proc Map
	},

	// Status 223 (0xDF): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialN,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,						// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftPikachuSpecialAirNProcMap				// Proc Map
	},

	// Status 224 (0xE0): SpecialLwStart (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialLwStart,			// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPikachuSpecialLwStartProcMap			// Proc Map
	},

	// Status 225 (0xE1): SpecialLwLoop (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialLwLoop,			// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialLwLoopProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPikachuSpecialLwLoopProcMap			// Proc Map
	},

	// Status 226 (0xE2): SpecialLwHit (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialLwHit,			// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialLwHitProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPikachuSpecialLwHitProcMap			// Proc Map
	},

	// Status 227 (0xE3): SpecialLwHit (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialLwEnd,			// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPikachuSpecialLwEndProcMap			// Proc Map
	},

	// Status 228 (0xE4): SpecialAirLwStart (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirLwStart,		// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirLwStartProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftPikachuSpecialAirLwStartProcMap		// Proc Map
	},

	// Status 229 (0xE5): SpecialAirLwLoop (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirLwLoop,		// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirLwLoopProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftPikachuSpecialAirLwLoopProcMap		// Proc Map
	},

	// Status 230 (0xE6): SpecialAirLwHit (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirLwHit,		// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirLwHitProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPikachuSpecialAirLwHitProcPhysics,	// Proc Physics
		ftPikachuSpecialAirLwHitProcMap			// Proc Map
	},

	// Status 231 (0xE7): SpecialAirLwEnd (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirLwEnd,		// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirLwEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftPikachuSpecialAirLwEndProcMap			// Proc Map
	},

	// Status 232 (0xE8): SpecialHiStart (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDSpecialLw,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialHiStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPikachuSpecialHiStartProcMap			// Proc Map
	},

	// Status 233 (0xE9): SpecialHi (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialLw,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPikachuSpecialHiProcPhysics,	    	// Proc Physics
		ftPikachuSpecialHiProcMap				// Proc Map
	},

	// Status 234 (0xEA): SpecialHiEnd (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialHiEnd,			// Script ID
		nFTMotionAttackIDSpecialLw,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialHiEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPikachuSpecialHiEndProcPhysics,	    // Proc Physics
		ftPikachuSpecialHiEndProcMap			// Proc Map
	},

	// Status 235 (0xEB): SpecialAirHiStart (wrong attack IDs, not marked as airborne)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDSpecialLw,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirHiStartProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftPikachuSpecialAirHiStartProcPhysics,	// Proc Physics
		ftPikachuSpecialAirHiStartProcMap		// Proc Map
	},

	// Status 236 (0xEC): SpecialAirHi (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirHi,			// Script ID
		nFTMotionAttackIDSpecialLw,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirHiProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPikachuSpecialAirHiProcPhysics,		// Proc Physics
		ftPikachuSpecialAirHiProcMap			// Proc Map
	},

	// Status 237 (0xED): SpecialAirHiEnd (wrong attack IDs)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPikachuMotionSpecialAirHiEnd,		// Script ID
		nFTMotionAttackIDSpecialLw,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPikachuSpecialAirHiEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPikachuSpecialAirHiEndProcPhysics,	// Proc Physics
		ftPikachuSpecialAirHiEndProcMap			// Proc Map
	}
};

#endif