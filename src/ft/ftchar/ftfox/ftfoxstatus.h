#ifndef _FTFOX_STATUS_H_
#define _FTFOX_STATUS_H_

#include <ft/fighter.h>

// 0x8012A214
FTStatusDesc dFTFoxSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack100Start
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionAttack100Start,			// Script ID
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

	// Status 221 (0xDD): Attack100Loop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionAttack100Loop,				// Script ID
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

	// Status 222 (0xDE): Attack100End
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionAttack100End,				// Script ID
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

	// Status 223 (0xDF): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionAppearR,					// Script ID
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

	// Status 224 (0xE0): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionAppearL,					// Script ID
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

	// Status 225 (0xE1): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialN,					// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialNProcUpdate,				// Proc Update
		ftFoxSpecialNProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 226 (0xE2): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirN,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialNProcUpdate,				// Proc Update
		ftFoxSpecialNProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	},

	// Status 227 (0xE3): SpecialHiStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialHiStart,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialHiStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftFoxSpecialHiStartProcMap			// Proc Map
	},

	// Status 228 (0xE4): SpecialAirHiStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirHiStart,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialAirHiStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirHiStartProcPhysics,	// Proc Physics
		ftFoxSpecialAirHiStartProcMap			// Proc Map
	},

	// Status 229 (0xE5): SpecialHiHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialHiHold,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialHiHoldProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftFoxSpecialHiHoldProcMap				// Proc Map
	},

	// Status 230 (0xE6): SpecialAirHiHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirHiHold,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialHiHoldProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirHiStartProcPhysics,	// Proc Physics
		ftFoxSpecialAirHiHoldProcMap			// Proc Map
	},

	// Status 231 (0xE7): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialHi,					// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialHiProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialHiProcPhysics,			// Proc Physics
		ftFoxSpecialHiProcMap					// Proc Map
	},

	// Status 232 (0xE8): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialHiProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirHiProcPhysics,			// Proc Physics
		ftFoxSpecialAirHiProcMap				// Proc Map
	},

	// Status 233 (0xE9): SpecialHiEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialHiEnd,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialHiEndProcPhysics,			// Proc Physics
		mpCommonSetFighterFallOnGroundBreak				// Proc Map
	},

	// Status 234 (0xEA): SpecialAirHiEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirHiEnd,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialAirHiEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftFoxSpecialAirHiEndProcMap			// Proc Map
	},

	// Status 235 (0xEB): SpecialAirHiBound
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirHiBound,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialAirHiBoundProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirHiBoundProcPhysics,	// Proc Physics
		ftFoxSpecialAirHiBoundProcMap			// Proc Map
	},

	// Status 236 (0xEC): SpecialLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialLwStart,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 237 (0xED): SpecialLwHit
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialLwHit,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwHitProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 238 (0xEE): SpecialLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialLwEnd,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwEndProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 239 (0xEF): SpecialLwLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialLwLoop,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwLoopProcUpdate,			// Proc Update
		ftFoxSpecialLwLoopProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 240 (0xF0): SpecialLwTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialLwLoop,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwTurnProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 241 (0xF1): SpecialAirLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirLwStart,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirLwCommonProcPhysics,	// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	},

	// Status 242 (0xF2): SpecialAirLwHit
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirLwHit,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwHitProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirLwCommonProcPhysics,	// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	},

	// Status 243 (0xF3): SpecialAirLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirLwEnd,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwEndProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirLwCommonProcPhysics,	// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	},

	// Status 244 (0xF4): SpecialAirLwLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirLwLoop,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwLoopProcUpdate,			// Proc Update
		ftFoxSpecialLwLoopProcInterrupt,		// Proc Interrupt
		ftFoxSpecialAirLwCommonProcPhysics,	// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	},

	// Status 245 (0xF5): SpecialAirLwTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTFoxMotionSpecialAirLwLoop,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftFoxSpecialLwTurnProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftFoxSpecialAirLwCommonProcPhysics,	// Proc Physics
		mpCommonProcFighterWaitOrLanding						// Proc Map
	}
};

#endif