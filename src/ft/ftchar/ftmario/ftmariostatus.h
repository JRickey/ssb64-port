#ifndef _FTMARIO_STATUS_H_
#define _FTMARIO_STATUS_H_

#include <ft/fighter.h>

// 0x80129F08
FTStatusDesc dFTMarioSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack13
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionAttack13,				// Script ID
		nFTMotionAttackIDAttack13,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack13,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 221 (0xDD): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionAppearR,					// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTMotionAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 222 (0xDE): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionAppearL,					// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTMotionAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 223 (0xDF): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionSpecialN,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTMotionAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftMarioSpecialNProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftMarioSpecialNProcMap				// Proc Map
	},

	// Status 224 (0xE0): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionSpecialAirN,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTMotionAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftMarioSpecialNProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftMarioSpecialAirNProcMap				// Proc Map
	},

	// Status 225 (0xE1): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTMotionAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftMarioSpecialHiProcUpdate,			// Proc Update
		ftMarioSpecialHiProcInterrupt,		// Proc Interrupt
		ftMarioSpecialHiProcPhysics,			// Proc Physics
		ftMarioSpecialHiProcMap				// Proc Map
	},

	// Status 226 (0xE2): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionSpecialAirHi,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTMotionAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftMarioSpecialHiProcUpdate,			// Proc Update
		ftMarioSpecialHiProcInterrupt,		// Proc Interrupt
		ftMarioSpecialHiProcPhysics,			// Proc Physics
		ftMarioSpecialHiProcMap				// Proc Map
	},

	// Status 227 (0xE3): SpecialLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionSpecialLw,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTMotionAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftMarioSpecialLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftMarioSpecialLwProcPhysics,			// Proc Physics
		ftMarioSpecialLwProcMap				// Proc Map
	},

	// Status 228 (0xE4): SpecialAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTMarioMotionSpecialAirLw,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTMotionAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftMarioSpecialAirLwProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftMarioSpecialAirLwProcPhysics,		// Proc Physics
		ftMarioSpecialAirLwProcMap			// Proc Map
	}
};

#endif