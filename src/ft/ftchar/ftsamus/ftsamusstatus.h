#ifndef _FTSAMUS_STATUS_H_
#define _FTSAMUS_STATUS_H_

#include <ft/fighter.h>

// 0x8012A41C
FTStatusDesc dFTSamusSpecialStatusDescs[/* */] = 
{
	// Status 220 (0xDC): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionAppearR,					// Script ID
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
		nFTSamusMotionAppearL,					// Script ID
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

	// Status 222 (0xDE): SpecialNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialNStart,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialNStartProcUpdate,		// Proc Update
		ftSamusSpecialNStartProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftSamusSpecialNStartProcMap			// Proc Map
	},

	// Status 223 (0xDF): SpecialNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialNLoop,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialNLoopProcUpdate,		// Proc Update
		ftSamusSpecialNLoopProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftSamusSpecialNLoopProcMap			// Proc Map
	},

	// Status 224 (0xE0): SpecialNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialNEnd,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialNEndProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftSamusSpecialNEndProcMap				// Proc Map
	},

	// Status 225 (0xE1): SpecialAirNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialAirNStart,		// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialNStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftSamusSpecialAirNStartProcMap		// Proc Map
	},

	// Status 226 (0xE2): SpecialAirNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialAirNEnd,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialNEndProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftSamusSpecialAirNEndProcMap			// Proc Map
	},

	// Status 227 (0xE3): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftSamusSpecialHiProcPhysics,			// Proc Physics
		ftSamusSpecialHiProcMap				// Proc Map
	},

	// Status 228 (0xE4): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialAirHi,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftSamusSpecialAirHiProcPhysics,		// Proc Physics
		ftSamusSpecialHiProcMap				// Proc Map
	},

	// Status 229 (0xE5): SpecialLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialLw,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftSamusSpecialLwProcPhysics,			// Proc Physics
		ftSamusSpecialLwProcMap				// Proc Map
	},

	// Status 230 (0xE6): SpecialAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTSamusMotionSpecialAirLw,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftSamusSpecialAirLwProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftSamusSpecialAirLwProcPhysics,		// Proc Physics
		ftSamusSpecialAirLwProcMap			// Proc Map
	}
};

#endif