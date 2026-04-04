#ifndef _FTDONKEY_STATUS_H_
#define _FTDONKEY_STATUS_H_

#include <ft/fighter.h>

// 0x80129FBC
FTStatusDesc dFTDonkeySpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionAppearR,					// Script ID
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
		nFTDonkeyMotionAppearL,					// Script ID
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
		nFTDonkeyMotionSpecialNStart,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNStartProcUpdate,		// Proc Update
		ftDonkeySpecialNStartProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeySpecialNStartProcMap			// Proc Map
	},

	// Status 223 (0xDF): SpecialAirNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialAirNStart,		// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialAirNStartProcUpdate,		// Proc Update
		ftDonkeySpecialNStartProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftDonkeySpecialAirNStartProcMap			// Proc Map
	},

	// Status 224 (0xE0): SpecialNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialNLoop,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNLoopProcUpdate,			// Proc Update
		ftDonkeySpecialNLoopProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeySpecialNLoopProcMap				// Proc Map
	},

	// Status 225 (0xE1): SpecialAirNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialAirNLoop,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNLoopProcUpdate,			// Proc Update
		ftDonkeySpecialNLoopProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftDonkeySpecialAirNLoopProcMap			// Proc Map
	},

	// Status 226 (0xE2): SpecialNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialNEnd,				// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNEndProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 227 (0xE3): SpecialAirNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialAirNEnd,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftDonkeySpecialAirNEndProcMap			// Proc Map
	},

	// Status 228 (0xE4): SpecialNFull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialNFull,			// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 229 (0xE5): SpecialAirNFull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialAirNFull,		// Script ID
		nFTMotionAttackIDSpecialN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftDonkeySpecialAirNEndProcMap			// Proc Map
	},

	// Status 230 (0xE6): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftDonkeySpecialHiProcPhysics,			// Proc Physics
		ftDonkeySpecialHiProcMap				// Proc Map
	},

	// Status 231 (0xE7): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialAirHi,			// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialAirHiProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftDonkeySpecialAirHiProcPhysics,		// Proc Physics
		ftDonkeySpecialAirHiProcMap			// Proc Map
	},

	// Status 232 (0xE8): SpecialLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialLwStart,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialLwStartProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 233 (0xE9): SpecialLwLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialLwLoop,			// Script ID
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeySpecialLwLoopProcUpdate,		// Proc Update
		ftDonkeySpecialLwLoopProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 234 (0xEA): SpecialLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionSpecialLwEnd,			// Script ID
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
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 235 (0xEB): ThrowFWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFWait,				// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftDonkeyThrowFWaitProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 236 (0xEC): ThrowFWalkSlow
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFWalkSlow,			// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftDonkeyThrowFWalkProcInterrupt,		// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 237 (0xED): ThrowFWalkMiddle
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFWalkMiddle,		// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftDonkeyThrowFWalkProcInterrupt,		// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 238 (0xEE): ThrowFWalkFast
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFWalkFast,			// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftDonkeyThrowFWalkProcInterrupt,		// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 239 (0xEF): ThrowFTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFTurn,				// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeyThrowFTurnProcUpdate,			// Proc Update
		ftDonkeyThrowFTurnProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 240 (0xF0): ThrowFKneeBend
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFKneeBend,			// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeyThrowFKneeBendProcUpdate,		// Proc Update
		ftDonkeyThrowFKneeBendProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 241 (0xF1): ThrowFFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFFall,				// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftDonkeyThrowFFallProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		ftDonkeyThrowFFallProcMap				// Proc Map
	},

	// Status 242 (0xF2): ThrowFLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFLanding,			// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeyThrowFLandingProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeyThrowFCommonProcMap			// Proc Map
	},

	// Status 243 (0xF3): ThrowFDamage
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFDamage,			// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeyThrowFDamageProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 244 (0xF4): ThrowFF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowFF,				// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeyThrowFFProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftDonkeyThrowFFProcMap				// Proc Map
	},

	// Status 245 (0xF5): ThrowAirFF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTDonkeyMotionThrowAirFF,				// Script ID
		nFTMotionAttackIDThrowF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftDonkeyThrowFFProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftDonkeyThrowAirFFProcMap				// Proc Map
	},

	// Status 246 (0xF6): HeavyThrowF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowF,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonItemThrowProcPhysics,			// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 247 (0xF7): HeavyThrowB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowB,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonItemThrowProcPhysics,			// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 248 (0xF8): HeavyThrowF4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowF4,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonItemThrowProcPhysics,			// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 249 (0xF9): HeavyThrowB4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowB4,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonItemThrowProcPhysics,			// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	}
};

#endif