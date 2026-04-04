#ifndef _FTPURIN_STATUS_H_
#define _FTPURIN_STATUS_H_

#include <ft/fighter.h>

// 0x8012B178
FTStatusDesc dFTPurinSpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack100Start
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionAttack100Start,			// Script ID
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
		nFTPurinMotionAttack100Loop,			// Script ID
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
		nFTPurinMotionAttack100End,			// Script ID
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

	// Status 223 (0xDF): JumpAerialF1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionJumpAerialF1,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftCommonJumpAerialProcPhysics,		// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding						// Proc Map
	},

	// Status 224 (0xE0): JumpAerialF2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionJumpAerialF2,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftCommonJumpAerialProcPhysics,		// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding						// Proc Map
	},
	
	// Status 225 (0xE1): JumpAerialF3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionJumpAerialF3,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftCommonJumpAerialProcPhysics,		// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding						// Proc Map
	},

	// Status 226 (0xE2): JumpAerialF4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionJumpAerialF4,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftCommonJumpAerialProcPhysics,		// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding						// Proc Map
	},

	// Status 227 (0xE3): JumpAerialF5
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionJumpAerialF5,			// Script ID
		nFTMotionAttackIDNone,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftCommonJumpAerialProcPhysics,		// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding						// Proc Map
	},

	// Status 228 (0xE4): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionAppearR,					// Script ID
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

	// Status 229 (0xE5): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionAppearL,					// Script ID
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

	// Status 230 (0xE6): SpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionSpecialN,				// Script ID
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
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		ftPurinSpecialNProcMap				// Proc Map
	},

	// Status 231 (0xE7): SpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionSpecialAirN,				// Script ID
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
		ftPurinSpecialAirNProcPhysics,		// Proc Physics
		ftPurinSpecialAirNProcMap				// Proc Map
	},

	// Status 232 (0xE8): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionSpecialHi,				// Script ID
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPurinSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,	    // Proc Physics
		ftPurinSpecialHiProcMap				// Proc Map
	},

	// Status 233 (0xE9): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionSpecialHi,			// Script ID - no SpecialAirHi ID, reuses SpecialHi
		nFTMotionAttackIDSpecialHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftPurinSpecialHiProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftPurinSpecialAirHiProcMap			// Proc Map
	},

	// Status 234 (0xEA): SpecialLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionSpecialLw,				// Script ID
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
		ftPurinSpecialLwProcMap				// Proc Map
	},

	// Status 235 (0xEB): SpecialAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTPurinMotionSpecialLw,			// Script ID - no SpecialAirLw ID, reuses SpecialLw
		nFTMotionAttackIDSpecialLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSpecialLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftPurinSpecialAirLwProcMap			// Proc Map
	}
};

#endif