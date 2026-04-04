#ifndef _FTKIRBY_STATUS_H_
#define _FTKIRBY_STATUS_H_

#include <ft/fighter.h>

// 0x8012A700
FTStatusDesc dFTKirbySpecialStatusDescs[/* */] =
{
	// Status 220 (0xDC): Attack100Start
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionAttack100Start,					// Script ID
		nFTMotionAttackIDAttack100,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDAttack100,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack100StartProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak					// Proc Map
	},

	// Status 221 (0xDD): Attack100Loop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionAttack100Loop,					// Script ID
		nFTMotionAttackIDAttack100,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDAttack100,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack100LoopProcUpdate,				// Proc Update
		ftCommonAttack100LoopProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak					// Proc Map
	},

	// Status 222 (0xDE): Attack100End
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionAttack100End,					// Script ID
		nFTMotionAttackIDAttack100,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDAttack100,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak					// Proc Map
	},

	// Status 223 (0xDF): JumpAerialF1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionJumpAerialF1,					// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,					// Proc Update
		ftCommonJumpAerialProcInterrupt,				// Proc Interrupt
		ftCommonJumpAerialProcPhysics,				// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding								// Proc Map
	},

	// Status 224 (0xE0): JumpAerialF2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionJumpAerialF2,					// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,					// Proc Update
		ftCommonJumpAerialProcInterrupt,				// Proc Interrupt
		ftCommonJumpAerialProcPhysics,				// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding								// Proc Map
	},

	// Status 225 (0xE1): JumpAerialF3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionJumpAerialF3,					// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,					// Proc Update
		ftCommonJumpAerialProcInterrupt,				// Proc Interrupt
		ftCommonJumpAerialProcPhysics,				// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding								// Proc Map
	},

	// Status 226 (0xE2): JumpAerialF4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionJumpAerialF4,					// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,					// Proc Update
		ftCommonJumpAerialProcInterrupt,				// Proc Interrupt
		ftCommonJumpAerialProcPhysics,				// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding								// Proc Map
	},

	// Status 227 (0xE3): JumpAerialF5
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionJumpAerialF5,					// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,					// Proc Update
		ftCommonJumpAerialProcInterrupt,				// Proc Interrupt
		ftCommonJumpAerialProcPhysics,				// Proc Physics
		mpCommonProcFighterCliffWaitOrLanding								// Proc Map
	},

	// Status 228 (0xE4): ThrowF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionThrowF,						// Script ID
		nFTMotionAttackIDThrowF,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDThrowF,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyThrowFProcUpdate,					// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,					// Proc Physics
		ftKirbyThrowFProcMap							// Proc Map
	},

	// Status 229 (0xE5): ThrowFFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionThrowFFall,						// Script ID
		nFTMotionAttackIDThrowF,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDThrowF,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,											// Proc Update
		NULL,											// Proc Interrupt
		NULL,											// Proc Physics
		ftKirbyThrowFProcMap							// Proc Map
	},

	// Status 230 (0xE6): ThrowFLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionThrowFLanding,					// Script ID
		nFTMotionAttackIDThrowF,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDThrowF,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrowProcUpdate,						// Proc Update
		NULL,											// Proc Interrupt
		ftKirbyThrowFLandingProcPhysics,				// Proc Physics
		ftKirbyThrowFLandingProcMap					// Proc Map
	},

	// Status 231 (0xE7): CopyMarioSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyMarioSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyMario,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyMario,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyMarioSpecialNProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyMarioSpecialNProcMap				// Proc Map
	},

	// Status 232 (0xE8): CopyMarioSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyMarioSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyMario,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyMario,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyMarioSpecialNProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelDrift,						// Proc Physics
		ftKirbyCopyMarioSpecialAirNProcMap			// Proc Map
	},

	// Status 233 (0xE9): CopyLuigiSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLuigiSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyLuigi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLuigi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyMarioSpecialNProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyMarioSpecialNProcMap				// Proc Map
	},

	// Status 234 (0xEA): CopyLuigiSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLuigiSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyLuigi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLuigi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyMarioSpecialNProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelDrift,						// Proc Physics
		ftKirbyCopyMarioSpecialAirNProcMap			// Proc Map
	},

	// Status 235 (0xEB): CopyFoxSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyFoxSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyFox,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyFox,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyFoxSpecialNProcUpdate,			// Proc Update
		ftKirbyCopyFoxSpecialNProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak					// Proc Map
	},

	// Status 236 (0xEC): CopyFoxSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyFoxSpecialAirN,				// Script ID
		nFTMotionAttackIDSpecialNCopyFox,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyFox,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyFoxSpecialNProcUpdate,			// Proc Update
		ftKirbyCopyFoxSpecialNProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyAirVelDrift,						// Proc Physics
		mpCommonProcFighterWaitOrLanding								// Proc Map
	},

	// Status 237 (0xED): CopySamusSpecialNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopySamusSpecialNStart,			// Script ID
		nFTMotionAttackIDSpecialNCopySamus,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopySamus,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopySamusSpecialNStartProcUpdate,		// Proc Update
		ftKirbyCopySamusSpecialNStartProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopySamusSpecialNStartProcMap			// Proc Map
	},

	// Status 238 (0xEE): CopySamusSpecialNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopySamusSpecialNLoop,			// Script ID
		nFTMotionAttackIDSpecialNCopySamus,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopySamus,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopySamusSpecialNLoopProcUpdate,		// Proc Update
		ftKirbyCopySamusSpecialNLoopProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopySamusSpecialNLoopProcMap			// Proc Map
	},

	// Status 239 (0xEF): CopySamusSpecialNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopySamusSpecialNEnd,			// Script ID
		nFTMotionAttackIDSpecialNCopySamus,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopySamus,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopySamusSpecialNEndProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopySamusSpecialNEndProcMap			// Proc Map
	},

	// Status 240 (0xF0): CopySamusSpecialAirNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopySamusSpecialAirNStart,		// Script ID
		nFTMotionAttackIDSpecialNCopySamus,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopySamus,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopySamusSpecialNStartProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelDrift,						// Proc Physics
		ftKirbyCopySamusSpecialAirNStartProcMap		// Proc Map
	},

	// Status 241 (0xF1): CopySamusSpecialAirNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopySamusSpecialAirNEnd,		// Script ID
		nFTMotionAttackIDSpecialNCopySamus,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopySamus,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopySamusSpecialNEndProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopySamusSpecialAirNEndProcMap		// Proc Map
	},

	// Status 242 (0xF2): CopyDonkeySpecialNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialNStart,		// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNStartProcUpdate,	// Proc Update
		ftKirbyCopyDonkeySpecialNStartProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyDonkeySpecialNStartProcMap		// Proc Map
	},

	// Status 243 (0xF3): CopyDonkeySpecialAirNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialAirNStart,		// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialAirNStartProcUpdate,	// Proc Update
		ftKirbyCopyDonkeySpecialNStartProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyDonkeySpecialAirNStartProcMap		// Proc Map
	},

	// Status 244 (0xF4): CopyDonkeySpecialNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialNLoop,			// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNLoopProcUpdate,		// Proc Update
		ftKirbyCopyDonkeySpecialNLoopProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyDonkeySpecialNLoopProcMap			// Proc Map
	},

	// Status 245 (0xF5): CopyDonkeySpecialAirNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialAirNLoop,		// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNLoopProcUpdate,		// Proc Update
		ftKirbyCopyDonkeySpecialNLoopProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyDonkeySpecialAirNLoopProcMap		// Proc Map
	},

	// Status 246 (0xF6): CopyDonkeySpecialNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialNEnd,			// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak					// Proc Map
	},

	// Status 247 (0xF7): CopyDonkeySpecialAirNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialAirNEnd,		// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyDonkeySpecialAirNEndProcMap		// Proc Map
	},

	// Status 248 (0xF8): CopyDonkeySpecialNFull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialNFull,			// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak					// Proc Map
	},

	// Status 249 (0xF9): CopyDonkeySpecialAirNFull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyDonkeySpecialAirNFull,		// Script ID
		nFTMotionAttackIDSpecialNCopyDonkey,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyDonkey,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyDonkeySpecialNEndProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyDonkeySpecialAirNEndProcMap		// Proc Map
	},

	// Status 250 (0xFA): AppearR
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionAppearR,							// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,						// Proc Update
		NULL,											// Proc Interrupt
		ftCommonAppearProcPhysics,					// Proc Physics
		mpCommonUpdateFighterProjectFloor						// Proc Map
	},

	// Status 251 (0xFB): AppearL
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionAppearL,							// Script ID
		nFTMotionAttackIDNone,						// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDNone,						// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAppearProcUpdate,						// Proc Update
		NULL,											// Proc Interrupt
		ftCommonAppearProcPhysics,					// Proc Physics
		mpCommonUpdateFighterProjectFloor						// Proc Map
	},

	// Status 252 (0xFC): CopyPikachuSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyPikachuSpecialN,			// Script ID
		nFTMotionAttackIDSpecialNCopyPikachu,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyPikachu,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyPikachuSpecialNProcMap			// Proc Map
	},

	// Status 253 (0xFD): CopyPikachuSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyPikachuSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyPikachu,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyPikachu,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyPikachuSpecialAirNProcMap			// Proc Map
	},

	// Status 254 (0xFE): CopyNessSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyNessSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyNess,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyNess,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyNessSpecialNProcMap				// Proc Map
	},

	// Status 255 (0xFF): CopyNessSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyNessSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyNess,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyNess,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyNessSpecialAirNProcMap			// Proc Map
	},

	// Status 256 (0x100): SpecialHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialHi,						// Script ID
		nFTMotionAttackIDSpecialHi,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialHi,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialHiProcUpdate,					// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialHiProcPhysics,					// Proc Physics
		ftKirbySpecialHiProcMap						// Proc Map
	},

	// Status 257 (0x101): SpecialHiLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialHiLanding,				// Script ID
		nFTMotionAttackIDSpecialHi,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialHi,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialHiLandingProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialHiLandingProcPhysics,			// Proc Physics
		mpCommonSetFighterFallOnGroundBreak					// Proc Map
	},

	// Status 258 (0x102): SpecialAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirHi,					// Script ID
		nFTMotionAttackIDSpecialHi,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialHi,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialHiProcUpdate,					// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialAirHiProcPhysics,				// Proc Physics
		ftKirbySpecialHiProcMap						// Proc Map
	},

	// Status 259 (0x103): SpecialAirHiFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirHiFall,				// Script ID
		nFTMotionAttackIDSpecialHi,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialHi,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,											// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialAirHiFallProcPhysics,			// Proc Physics
		ftKirbySpecialAirHiFallProcMap				// Proc Map
	},

	// Status 260 (0x104): SpecialLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialLwStart,					// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialLwStartProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		NULL,											// Proc Physics
		ftKirbySpecialLwStartProcMap					// Proc Map
	},

	// Status 261 (0x105): SpecialLwUnk
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialLwUnk,					// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialLwUnkProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialLwUnkProcMap					// Proc Map
	},

	// Status 262 (0x106): SpecialLwHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialLwHold,					// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialLwHoldProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialLwHoldProcPhysics,				// Proc Physics
		ftKirbySpecialLwHoldProcMap					// Proc Map
	},

	// Status 263 (0x107): SpecialLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialLwEnd,					// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,					// Proc Physics
		mpCommonProcFighterWaitOrLanding								// Proc Map
	},

	// Status 264 (0x108): SpecialAirLwStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirLwStart,				// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirLwStartProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		NULL,											// Proc Physics
		ftKirbySpecialAirLwStartProcMap				// Proc Map
	},

	// Status 265 (0x109): SpecialAirLwHold
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirLwHold,				// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirLwHoldProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		NULL,											// Proc Physics
		ftKirbySpecialAirLwHoldProcMap				// Proc Map
	},

	// Status 266 (0x10A): SpecialAirLwLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirLwLanding,				// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialLwHoldProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialLwHoldProcPhysics,				// Proc Physics
		ftKirbySpecialLwHoldProcMap					// Proc Map
	},

	// Status 267 (0x10B): SpecialAirLwFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirLwFall,				// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirLwFallProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		NULL,											// Proc Physics
		ftKirbySpecialAirLwHoldProcMap				// Proc Map
	},

	// Status 268 (0x10C): SpecialAirLwEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirLwEnd,					// Script ID
		nFTMotionAttackIDSpecialLw,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialLw,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,					// Proc Physics
		mpCommonProcFighterWaitOrLanding								// Proc Map
	},

	// Status 269 (0x10D): SpecialNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNStart,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNStartProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNStartProcMap					// Proc Map
	},

	// Status 270 (0x10E): SpecialNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNLoop,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNLoopProcUpdate,				// Proc Update
		ftKirbySpecialNLoopProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNLoopProcMap					// Proc Map
	},

	// Status 271 (0x10F): SpecialNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNEnd,						// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNEndProcMap						// Proc Map
	},

	// Status 272 (0x110): SpecialNCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,												// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNCatchProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNCatchProcMap					// Proc Map
	},

	// Status 273 (0x111): SpecialNEat
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNEat,						// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNEatProcUpdate,					// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNEatProcMap						// Proc Map
	},

	// Status 274 (0x112): SpecialNThrow
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNThrow,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNThrowProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,					// Proc Physics
		ftKirbySpecialNThrowProcMap					// Proc Map
	},

	// Status 275 (0x113): SpecialNWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNWait,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,											// Proc Update
		ftKirbySpecialNWaitProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNWaitProcMap					// Proc Map
	},

	// Status 276 (0x114): SpecialNTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNTurn,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNTurnProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNTurnProcMap					// Proc Map
	},

	// Status 277 (0x115): SpecialNCopy
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialNCopy,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNCopyProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbySpecialNCopyProcMap					// Proc Map
	},

	// Status 278 (0x116): SpecialAirNStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNStart,				// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirNStartProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbySpecialAirNStartProcMap				// Proc Map
	},

	// Status 279 (0x117): SpecialAirNLoop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNLoop,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNLoopProcUpdate,				// Proc Update
		ftKirbySpecialAirNLoopProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbySpecialAirNLoopProcMap					// Proc Map
	},

	// Status 280 (0x118): SpecialAirNEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNEnd,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbySpecialAirNEndProcMap					// Proc Map
	},

	// Status 281 (0x119): SpecialAirNCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,												// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack? - bruh moment here
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNCatchProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbySpecialAirNCatchProcMap				// Proc Map
	},

	// Status 282 (0x11A): SpecialAirNEat
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNEat,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialNEatProcUpdate,					// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbySpecialAirNEatProcMap					// Proc Map
	},

	// Status 283 (0x11B): SpecialAirNThrow
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNThrow,				// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirNThrowProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelTransNAll,					// Proc Physics
		ftKirbySpecialAirNThrowProcMap				// Proc Map
	},

	// Status 284 (0x11C): SpecialAirNWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNWait,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,											// Proc Update
		ftKirbySpecialAirNWaitProcInterrupt,			// Proc Interrupt
		ftKirbySpecialAirNWaitProcPhysics,			// Proc Physics
		ftKirbySpecialAirNWaitProcMap					// Proc Map
	},

	// Status 285 (0x11D): SpecialAirNTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNTurn,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirNTurnProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftKirbySpecialAirNWaitProcPhysics,			// Proc Physics
		ftKirbySpecialAirNTurnProcMap					// Proc Map
	},

	// Status 286 (0x11E): SpecialAirNCopy
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionSpecialAirNCopy,					// Script ID
		nFTMotionAttackIDSpecialN,					// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialN,					// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbySpecialAirNCopyProcUpdate,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbySpecialAirNCopyProcMap					// Proc Map
	},

	// Status 287 (0x11F): CopyLinkSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLinkSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyLink,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLink,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyLinkSpecialNProcUpdate,			// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyLinkSpecialNProcMap				// Proc Map
	},

	// Status 288 (0x120): CopyLinkSpecialNGet
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLinkSpecialNGet,			// Script ID
		nFTMotionAttackIDSpecialNCopyLink,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLink,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		mpCommonSetFighterFallOnGroundBreak					// Proc Map
	},

	// Status 289 (0x121): CopyLinkSpecialNEmpty
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLinkSpecialNEmpty,			// Script ID
		nFTMotionAttackIDSpecialNCopyLink,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLink,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyLinkSpecialNEmptyProcMap			// Proc Map
	},

	// Status 290 (0x122): CopyLinkSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLinkSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyLink,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLink,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyLinkSpecialAirNProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyLinkSpecialAirNProcMap			// Proc Map
	},

	// Status 291 (0x123): CopyLinkSpecialAirNReturn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLinkSpecialAirNReturn,		// Script ID
		nFTMotionAttackIDSpecialNCopyLink,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLink,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		mpCommonProcFighterWaitOrLanding								// Proc Map
	},

	// Status 292 (0x124): CopyLinkSpecialAirNEmpty
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyLinkSpecialAirNEmpty,		// Script ID
		nFTMotionAttackIDSpecialNCopyLink,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		TRUE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyLink,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyLinkSpecialAirNEmptyProcMap		// Proc Map
	},

	// Status 293 (0x125): CopyPurinSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyPurinSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyPurin,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyPurin,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,					// Proc Physics
		ftKirbyCopyPurinSpecialNProcMap				// Proc Map
	},

	// Status 294 (0x126): CopyPurinSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyPurinSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyPurin,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyPurin,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftKirbyCopyPurinSpecialAirNProcPhysics,		// Proc Physics
		ftKirbyCopyPurinSpecialAirNProcMap			// Proc Map
	},

	// Status 295 (0x127): CopyCaptainSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyCaptainSpecialN,			// Script ID
		nFTMotionAttackIDSpecialNCopyCaptain,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyCaptain,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftKirbyCopyCaptainSpecialNProcPhysics,		// Proc Physics
		ftKirbyCopyCaptainSpecialNProcMap			// Proc Map
	},

	// Status 296 (0x128): CopyCaptainSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyCaptainSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyCaptain,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyCaptain,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftKirbyCopyCaptainSpecialAirNProcPhysics,	// Proc Physics
		ftKirbyCopyCaptainSpecialAirNProcMap			// Proc Map
	},

	// Status 297 (0x129): CopyYoshiSpecialN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyYoshiSpecialN,				// Script ID
		nFTMotionAttackIDSpecialNCopyYoshi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyYoshi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyYoshiSpecialNProcMap				// Proc Map
	},

	// Status 298 (0x12A): CopyYoshiSpecialNCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyYoshiSpecialNCatch,			// Script ID
		nFTMotionAttackIDSpecialNCopyYoshi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyYoshi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyYoshiSpecialNCatchProcUpdate,		// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyYoshiSpecialNCatchProcMap			// Proc Map
	},

	// Status 299 (0x12B): CopyYoshiSpecialNRelease
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyYoshiSpecialNRelease,		// Script ID
		nFTMotionAttackIDSpecialNCopyYoshi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsGround,								// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyYoshi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyYoshiSpecialNReleaseProcUpdate,	// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,				// Proc Physics
		ftKirbyCopyYoshiSpecialNReleaseProcMap		// Proc Map
	},

	// Status 300 (0x12C): CopyYoshiSpecialAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyYoshiSpecialAirN,			// Script ID
		nFTMotionAttackIDSpecialNCopyYoshi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyYoshi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,				// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyYoshiSpecialAirNProcMap			// Proc Map
	},

	// Status 301 (0x12D): CopyYoshiSpecialAirNCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyYoshiSpecialAirNCatch,		// Script ID
		nFTMotionAttackIDSpecialNCopyYoshi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyYoshi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyYoshiSpecialAirNCatchProcUpdate,	// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyYoshiSpecialAirNCatchProcMap		// Proc Map
	},

	// Status 302 (0x12E): CopyYoshiSpecialAirNRelease
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTKirbyMotionCopyYoshiSpecialAirNRelease,	// Script ID
		nFTMotionAttackIDSpecialNCopyYoshi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,												// Unused
		FALSE,											// Is Smash attack?
		nMPKineticsAir,									// Grounded or aerial attack?
		FALSE,											// Is projectile?
		nFTStatusAttackIDSpecialNCopyYoshi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftKirbyCopyYoshiSpecialAirNReleaseProcUpdate,// Proc Update
		NULL,											// Proc Interrupt
		ftPhysicsApplyAirVelFriction,					// Proc Physics
		ftKirbyCopyYoshiSpecialAirNReleaseProcMap	// Proc Map
	}
};

#endif