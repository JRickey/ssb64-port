#ifndef _FTLINK_STATUS_H_
#define _FTLINK_STATUS_H_

#include <ft/fighter.h>

// 0x8012A5AC
FTStatusDesc dFTLinkSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack13
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionAttack13,					// Script ID
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

	// Status 221 (0xDD): Attack100Start
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionAttack100Start,			// Script ID
		nFTMotionAttackIDAttack100,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack100,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack100StartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 222 (0xDE): Attack100Loop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionAttack100Loop,			// Script ID
		nFTMotionAttackIDAttack100,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack100,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack100LoopProcUpdate,		// Proc Update
		ftCommonAttack100LoopProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 223 (0xDF): Attack100End
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionAttack100End,				// Script ID
		nFTMotionAttackIDAttack100,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack100,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 224 (0xE0): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionAppearR,					// Script ID
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

	// Status 225 (0xE1): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionAppearL,					// Script ID
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

	// Status 226 (0xE2): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftLinkSpecialHiProcPhysics,			// Proc Physics
		ftLinkSpecialHiProcMap				// Proc Map
	},

	// Status 227 (0xE3): SpecialHiEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialHiEnd,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialHiEndProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftLinkSpecialHiEndProcMap				// Proc Map
	},

	// Status 228 (0xE4): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialAirHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialAirHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftLinkSpecialAirHiProcPhysics,		// Proc Physics
		ftLinkSpecialAirHiProcMap				// Proc Map
	},

	// Status 229 (0xE5): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialN,					// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialNProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftLinkSpecialNProcMap					// Proc Map
	},

	// Status 230 (0xE6): SpecialNGet
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialNGet,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,					// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 231 (0xE7): SpecialNEmpty
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialNEmpty,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftLinkSpecialNEmptyProcMap			// Proc Map
	},

	// Status 232 (0xE8): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialAirN,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialAirNProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftLinkSpecialAirNProcMap				// Proc Map
	},

	// Status 233 (0xE9): SpecialAirNReturn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialAirNReturn,		// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	},

	// Status 234 (0xEA): SpecialAirNEmpty
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialAirNEmpty,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftLinkSpecialAirNEmptyProcMap			// Proc Map
	},

	// Status 235 (0xEB): SpecialLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialLw,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftLinkSpecialLwProcMap				// Proc Map
	},

	// Status 236 (0xEC): SpecialAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTLinkMotionSpecialAirLw,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftLinkSpecialAirLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftLinkSpecialAirLwProcMap				// Proc Map
	}
};

#endif