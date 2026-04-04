#ifndef _FTCAPTAIN_STATUS_H_

#include <ft/fighter.h>

// 0x8012AD7C
FTStatusDesc dFTCaptainSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack13
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAttack13,				// Script ID
		nFTMotionAttackIDAttack13,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack13,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack13ProcUpdate,				// Proc Update
		ftCommonAttack13ProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 221 (0xDD): Attack100Start
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAttack100Start,			// Script ID
		nFTMotionAttackIDAttack100,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack100,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack100StartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 222 (0xDE): Attack100Loop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAttack100Loop,			// Script ID
		nFTMotionAttackIDAttack100,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack100,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack100LoopProcUpdate,		// Proc Update
		ftCommonAttack100LoopProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 223 (0xDF): Attack100End
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAttack100End,			// Script ID
		nFTMotionAttackIDAttack100,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack100,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,					// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 224 (0xE0): AppearRStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAppearRStart,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,			        // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainAppearStartProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,				// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 225 (0xE1): AppearLStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAppearLStart,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,			        // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainAppearStartProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,				// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 226 (0xE2): AppearREnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAppearREnd,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,			        // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,				// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 227 (0xE3): AppearLEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionAppearLEnd,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,			        // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,				// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 228 (0xE4): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialN,				// Script ID
		nFTMotionAttackIDSpecialN,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,					// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialNProcPhysics,			// Proc Physics
		ftCaptainSpecialNProcMap				// Proc Map
	},

	// Status 229 (0xE5): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialN,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,					// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialAirNProcPhysics,		// Proc Physics
		ftCaptainSpecialAirNProcMap				// Proc Map
	},

	// Status 230 (0xE6): SpecialLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialLw,				// Script ID
		nFTMotionAttackIDSpecialLw,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainSpecialLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialLwProcPhysics,			// Proc Physics
		ftCaptainSpecialLwProcMap				// Proc Map
	},

	// Status 231 (0xE7): SpecialLwAir
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialLwAir,			// Script ID
		nFTMotionAttackIDSpecialLw,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainSpecialLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialLwProcPhysics,			// Proc Physics
		ftCaptainSpecialLwAirProcMap			// Proc Map
	},

	// Status 232 (0xE8): SpecialLwLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialLwLanding,		// Script ID
		nFTMotionAttackIDSpecialLw,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,					// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialLwLandingProcPhysics,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 233 (0xE9): SpecialAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialAirLw,			// Script ID
		nFTMotionAttackIDSpecialLw,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,					// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialAirLwProcPhysics,		// Proc Physics
		ftCaptainSpecialAirLwProcMap			// Proc Map
	},

	// Status 234 (0xEA): SpecialLwBound
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialLwBound,			// Script ID
		nFTMotionAttackIDSpecialLw,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,					// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialLwBoundProcPhysics,		// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 235 (0xEB): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainSpecialHiProcUpdate,			// Proc Update
		ftCaptainSpecialHiProcInterrupt,		// Proc Interrupt
		ftCaptainSpecialHiProcPhysics,			// Proc Physics
		ftCaptainSpecialHiProcMap				// Proc Map
	},

	// Status 236 (0xEC): SpecialHiCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialHiCatch,			// Script ID
		nFTMotionAttackIDSpecialHi,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainSpecialHiCatchProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCaptainSpecialHiCatchProcPhysics,		// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 237 (0xED): SpecialHiThrow
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialHiThrow,			// Script ID
		nFTMotionAttackIDSpecialHi,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,					// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,			// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 238 (0xEE): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCaptainMotionSpecialAirHi,			// Script ID
		nFTMotionAttackIDSpecialHi,			    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCaptainSpecialHiProcUpdate,			// Proc Update
		ftCaptainSpecialHiProcInterrupt,		// Proc Interrupt
		ftCaptainSpecialHiProcPhysics,			// Proc Physics
		ftCaptainSpecialHiProcMap				// Proc Map
	}
};

#endif
