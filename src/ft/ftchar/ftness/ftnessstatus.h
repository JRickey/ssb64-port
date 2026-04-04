#ifndef _FTNESS_STATUS_H_
#define _FTNESS_STATUS_H_

#include <ft/fighter.h>

// 0x8012B2B8
FTStatusDesc dFTNessSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack13
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionAttack13,					// Script ID
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

	// Status 221 (0xDD): AppearRStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionAppearRStart,				// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessAppearStartProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 222 (0xDE): AppearLStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionAppearLStart,				// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessAppearStartProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 223 (0xDF): AppearWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionAppearWait,				// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessAppearWaitProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonAppearProcPhysics,			// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 224 (0xE0): AppearREnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionAppearREnd,				// Script ID
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

	// Status 225 (0xE1): AppearLEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionAppearLEnd,				// Script ID
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

	// Status 226 (0xE2): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialN,					// Script ID
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
		ftNessSpecialNProcMap					// Proc Map
	},

	// Status 227 (0xE3): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirN,				// Script ID
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
		ftNessSpecialAirNProcMap				// Proc Map
	},

	// Status 228 (0xE4): SpecialHiStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialHiStart,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialHiStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialHiProcPhysics,			// Proc Physics
		ftNessSpecialHiStartProcMap			// Proc Map
	},

	// Status 229 (0xE5): SpecialHiHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialHiHold,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialHiHoldProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftNessSpecialHiHoldProcMap			// Proc Map
	},

	// Status 230 (0xE6): SpecialHiEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialHiEnd,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftNessSpecialHiEndProcMap				// Proc Map
	},

	// Status 231 (0xE7): SpecialHi (PK Thunder Self-Hit / PK Jibaku)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialHiJibaku,			// Script ID
		nFTMotionAttackIDSpecialHi,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialHiJibakuProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialHiJibakuProcPhysics,				// Proc Physics
		ftNessSpecialHiJibakuProcMap					// Proc Map
	},

	// Status 232 (0xE8): SpecialAirHiStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirHiStart,		// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirHiStartProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirHiProcPhysics,		// Proc Physics
		ftNessSpecialAirHiStartProcMap		// Proc Map
	},

	// Status 233 (0xE9): SpecialAirHiHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirHiHold,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirHiHoldProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirHiProcPhysics,		// Proc Physics
		ftNessSpecialAirHiHoldProcMap			// Proc Map
	},

	// Status 234 (0xEA): SpecialAirHiEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirHiEnd,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirHiEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirHiProcPhysics,		// Proc Physics
		ftNessSpecialAirHiEndProcMap			// Proc Map
	},

	// Status 235 (0xEB): SpecialAirHiBound (PK Thunder Wall-Hit / PK Jibaku Wall-Hit)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirHiBound,		// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirHiJibakuBoundProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftNessSpecialAirHiJibakuBoundProcMap			// Proc Map
	},

	// Status 236 (0xEC): SpecialAirHi (PK Thunder Self-Hit / PK Jibaku)
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirHiJibaku,	// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirHiJibakuProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirHiJibakuProcPhysics,			// Proc Physics
		ftNessSpecialAirHiJibakuProcMap				// Proc Map
	},

	// Status 237 (0xED): SpecialLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialLwStart,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialLwProcPhysics,			// Proc Physics
		ftNessSpecialLwStartProcMap			// Proc Map
	},

	// Status 238 (0xEE): SpecialLwHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialLwHold,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialLwHoldProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftNessSpecialLwHoldProcMap			// Proc Map
	},

	// Status 239 (0xEF): SpecialLwHit
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialLwHit,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialLwHitProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftNessSpecialLwHitProcMap				// Proc Map
	},

	// Status 240 (0xF0): SpecialLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialLwEnd,				// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftNessSpecialLwEndProcMap				// Proc Map
	},

	// Status 241 (0xF1): SpecialAirLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirLwStart,		// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirLwStartProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirLwProcPhysics,		// Proc Physics
		ftNessSpecialAirLwStartProcMap		// Proc Map
	},

	// Status 242 (0xF2): SpecialAirLwHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirLwHold,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirLwHoldProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirLwProcPhysics,		// Proc Physics
		ftNessSpecialAirLwHoldProcMap			// Proc Map
	},

	// Status 243 (0xF3): SpecialAirLwHit
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirLwHit,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirLwHitProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirLwProcPhysics,		// Proc Physics
		ftNessSpecialAirLwHitProcMap			// Proc Map
	},

	// Status 244 (0xF4): SpecialAirLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTNessMotionSpecialAirLwEnd,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftNessSpecialAirLwEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftNessSpecialAirLwProcPhysics,		// Proc Physics
		ftNessSpecialAirLwEndProcMap			// Proc Map
	}
};

#endif