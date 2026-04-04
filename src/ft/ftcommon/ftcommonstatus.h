#ifndef _FTCOMMON_STATUS_H_
#define _FTCOMMON_STATUS_H_

#include <ft/fighter.h>

// 0x80128DD8
FTStatusDesc dFTCommonNullStatusDescs[/* */] =
{
	// Status 0 (0x0): DeadDown
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDeadCommonProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 1 (0x1): DeadLeft / DeadRight
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDeadCommonProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 2 (0x2): DeadUpStar
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFall,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDeadUpStarProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 3 (0x3): DeadUpFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFall,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDeadUpFallProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 4 (0x4): Sleep
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonSleepProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 5 (0x5): Entry
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWait,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	}
};

// 0x80128E50
FTStatusDesc dFTCommonActionStatusDescs[/* */] =
{
	// Status 6 (0x6): EntryNull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWait,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonEntryNullProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 7 (0x7): RebirthDown
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionRebirthDown,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonRebirthDownProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonRebirthCommonProcMap			// Proc Map
	},

	// Status 8 (0x8): RebirthStand
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionRebirthStand,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonRebirthStandProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonRebirthCommonProcMap			// Proc Map
	},

	// Status 9 (0x9): RebirthWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionRebirthWait,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonRebirthWaitProcUpdate,			// Proc Update
		ftCommonRebirthWaitProcInterrupt,		// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonRebirthCommonProcMap			// Proc Map
	},

	// Status 10 (0xA): Wait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWait,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonWaitProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 11 (0xB): WalkSlow
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWalkSlow,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonWalkProcInterrupt,				// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 12 (0xC): WalkMiddle
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWalkMiddle,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonWalkProcInterrupt,				// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 13 (0xD): WalkFast
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWalkFast,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonWalkProcInterrupt,				// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 14 (0xE): WalkEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWalkEnd,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 15 (0xF): Dash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDash,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDashProcUpdate,					// Proc Update
		ftCommonDashProcInterrupt,				// Proc Interrupt
		ftCommonDashProcPhysics,				// Proc Physics
		ftCommonDashProcMap						// Proc Map
	},

	// Status 16 (0x10): Run
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionRun,						// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonRunProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyGroundVelTransferAir,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 17 (0x11): RunBrake
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionRunBrake,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonRunBrakeProcInterrupt,			// Proc Interrupt
		ftCommonRunBrakeProcPhysics,			// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 18 (0x12): Turn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionTurn,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonTurnProcUpdate,					// Proc Update
		ftCommonTurnProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 19 (0x13): TurnRun
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionTurnRun,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonTurnRunProcUpdate,				// Proc Update
		ftCommonTurnRunProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 20 (0x14): KneeBend
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionKneeBend,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonKneeBendProcUpdate,				// Proc Update
		ftCommonKneeBendProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 21 (0x15): GuardKneeBend
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionGuardKneeBend,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonKneeBendProcUpdate,				// Proc Update
		ftCommonKneeBendProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 22 (0x16): JumpF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionJumpF,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		ftCommonJumpProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 23 (0x17): JumpB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionJumpB,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		ftCommonJumpProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 24 (0x18): JumpAerialF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionJumpAerialF,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 25 (0x19): JumpAerialB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionJumpAerialB,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonJumpAerialProcUpdate,			// Proc Update
		ftCommonJumpAerialProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 26 (0x1A): Fall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFall,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonFallProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 27 (0x1B): FallAerial
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFallAerial,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonFallProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 28 (0x1C): Squat
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSquat,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonSquatProcUpdate,				// Proc Update
		ftCommonSquatProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 29 (0x1D): SquatWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSquatWait,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonSquatWaitProcUpdate,			// Proc Update
		ftCommonSquatWaitProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 30 (0x1E): SquatRv
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSquatRv,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonSquatRvProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 31 (0x1F): LandingLight
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingLight,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonLandingProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 32 (0x20): LandingHeavy
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingHeavy,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonLandingProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 33 (0x21): Pass
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionPass,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		ftCommonPassProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 34 (0x22): GuardPass
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionGuardPass,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		ftCommonPassProcInterrupt,				// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 35 (0x23): OttottoWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionOttottoWait,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonOttottoProcInterrupt,			// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonOttottoProcMap					// Proc Map
	},

	// Status 36 (0x24): Ottotto
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionOttotto,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonOttottoProcUpdate,				// Proc Update
		ftCommonOttottoProcInterrupt,			// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonOttottoProcMap					// Proc Map
	},

	// Status 37 (0x25): DamageHi1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageHi1,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 38 (0x26): DamageHi2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageHi2,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		  	// Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 39 (0x27): DamageHi3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageHi3,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 40 (0x28): DamageN1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageN1,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 41 (0x29): DamageN2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageN2,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 42 (0x2A): DamageN3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageN3,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 43 (0x2B): DamageLw1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageLw1,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 44 (0x2C): DamageLw2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageLw2,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 45 (0x2D): DamageLw3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageLw3,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 46 (0x2E): DamageAir1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageAir1,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 47 (0x2F): DamageAir2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageAir2,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 48 (0x30): DamageAir3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageAir3,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 49 (0x31): DamageE1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageE,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageCommonProcUpdate,		    // Proc Update
		ftCommonDamageCommonProcInterrupt,	    // Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		mpCommonUpdateFighterKinetics			// Proc Map
	},

	// Status 50 (0x32): DamageE2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageE,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageAirCommonProcUpdate,		// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 51 (0x33): DamageFlyHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFlyHi,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageAirCommonProcUpdate,		// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 52 (0x34): DamageFlyN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFlyN,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageAirCommonProcUpdate,		// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 53 (0x35): DamageFlyLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFlyLw,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageAirCommonProcUpdate,		// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 54 (0x36): DamageFlyTop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFlyTop,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageAirCommonProcUpdate,		// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 55 (0x37): DamageFlyRoll
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFlyRoll,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDamageAirCommonProcUpdate,		// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 56 (0x38): WallDamage
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionWallDamage,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonWallDamageProcUpdate,			// Proc Update
		ftCommonDamageAirCommonProcInterrupt,	// Proc Interrupt
		ftCommonDamageCommonProcPhysics,		// Proc Physics
		ftCommonDamageAirCommonProcMap			// Proc Map
	},

	// Status 57 (0x39): DamageFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFall,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonDamageFallProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		ftCommonDamageFallProcMap				// Proc Map
	},

	// Status 58 (0x3A): FallSpecial
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFallSpecial,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonFallSpecialProcInterrupt,		// Proc Interrupt
		ftCommonFallSpecialProcPhysics,			// Proc Physics
		ftCommonFallSpecialProcMap				// Proc Map
	},

	// Status 59 (0x3B): LandingFallSpecial
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingFallSpecial,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonLandingProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 60 (0x3C): Twister
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionTwister,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonTwisterProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonTwisterProcPhysics,				// Proc Physics
		mpCommonProcFighterProject				// Proc Map
	},

	// Status 61 (0x3D): TaruCann
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonTaruCannProcUpdate,				// Proc Update
		ftCommonTaruCannProcInterrupt,			// Proc Interrupt
		ftCommonTaruCannProcPhysics,			// Proc Physics
		mpCommonProcFighterProject				// Proc Map
	},

	// Status 62 (0x3E): DokanStart
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDokanStart,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDokanStartProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonDokanStartProcPhysics,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 63 (0x3F): DokanWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDokanWaitProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonDokanWaitProcMap				// Proc Map
	},

	// Status 64 (0x40): DokanEnd
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDokanEnd,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDokanEndProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 65 (0x41): DokanWalk
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDokanWalk,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDokanEndProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 66 (0x42): StopCeil
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionStopCeil,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		mpCommonProcFighterCliffFloorCeil		// Proc Map
	},

	// Status 67 (0x43): DownBounceD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownBounceD,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDownBounceProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 68 (0x44): DownBounceU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownBounceU,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDownBounceProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 69 (0x45): DownWaitD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-2,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDownWaitProcUpdate,				// Proc Update
		ftCommonDownWaitProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 70 (0x46): DownWaitU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-2,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonDownWaitProcUpdate,				// Proc Update
		ftCommonDownWaitProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 71 (0x47): DownStandD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownStandD,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonDownStandProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 72 (0x48): DownStandU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownStandU,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonDownStandProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 73 (0x49): PassiveStandF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionPassiveStandF,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 74 (0x4A): PassiveStandB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionPassiveStandB,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 75 (0x4B): DownForwardD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownForwardD,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 76 (0x4C): DownForwardU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownForwardU,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 77 (0x4D): DownBackD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownBackD,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 78 (0x4E): DownBackU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownBackU,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 79 (0x4F): DownAttackD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownAttackD,				// Script ID
		nFTMotionAttackIDDownAttackD,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDDownAttackD,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 80 (0x50): DownAttackU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDownAttackU,				// Script ID
		nFTMotionAttackIDDownAttackU,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDDownAttackU,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 81 (0x51): Passive
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionPassive,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 82 (0x52): ReboundWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonReboundWaitProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 83 (0x53): Rebound
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionRebound,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonReboundProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 84 (0x54): CliffCatch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffCatch,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCatchProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 85 (0x55): CliffWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffWait,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonCliffWaitProcInterrupt,			// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 86 (0x56): CliffQuick
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffQuick,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffQuickProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 87 (0x57): CliffClimbQuick1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffClimbQuick1,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffClimbQuick1ProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 88 (0x58): CliffClimbQuick2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffClimbQuick2,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCommon2ProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommon2ProcPhysics,		// Proc Physics
		ftCommonCliffClimbCommon2ProcMap		// Proc Map
	},

	// Status 89 (0x59): CliffSlow
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffSlow,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffSlowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 90 (0x5A): CliffClimbSlow1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffClimbSlow1,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffClimbSlow1ProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 91 (0x5B): CliffClimbSlow2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffClimbSlow2,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCommon2ProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommon2ProcPhysics,		// Proc Physics
		ftCommonCliffClimbCommon2ProcMap		// Proc Map
	},

	// Status 92 (0x5C): CliffAttackQuick1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffAttackQuick1,		// Script ID
		nFTMotionAttackIDCliffAttackQuick,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDCliffAttackQuick,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffAttackQuick1ProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 93 (0x5D): CliffAttackQuick2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffAttackQuick2,		// Script ID
		nFTMotionAttackIDCliffAttackQuick,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDCliffAttackQuick,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCommon2ProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommon2ProcPhysics,		// Proc Physics
		ftCommonCliffAttackEscape2ProcMap		// Proc Map
	},

	// Status 94 (0x5E): CliffAttackSlow1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffAttackSlow1,		// Script ID
		nFTMotionAttackIDCliffAttackSlow,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDCliffAttackSlow,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffAttackSlow1ProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 95 (0x5F): CliffAttackSlow2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffAttackSlow2,		// Script ID
		nFTMotionAttackIDCliffAttackSlow,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDCliffAttackSlow,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCommon2ProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommon2ProcPhysics,		// Proc Physics
		ftCommonCliffAttackEscape2ProcMap		// Proc Map
	},

	// Status 96 (0x60): CliffEscapeQuick1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffEscapeQuick1,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffEscapeQuick1ProcUpdate,	// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 97 (0x61): CliffEscapeQuick2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffEscapeQuick2,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCommon2ProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommon2ProcPhysics,		// Proc Physics
		ftCommonCliffAttackEscape2ProcMap		// Proc Map
	},

	// Status 98 (0x62): CliffEscapeSlow1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffEscapeSlow1,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffEscapeSlow1ProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommonProcPhysics,			// Proc Physics
		ftCommonCliffCommonProcMap				// Proc Map
	},

	// Status 99 (0x63): CliffEscapeSlow2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCliffEscapeSlow2,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCliffCommon2ProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCliffCommon2ProcPhysics,		// Proc Physics
		ftCommonCliffAttackEscape2ProcMap		// Proc Map
	},

	// Status 100 (0x64): LightGet
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightGet,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonGetProcUpdate,					// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonLightGetProcMap					// Proc Map
	},

	// Status 101 (0x65): HeavyGet
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyGet,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonGetProcUpdate,					// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyGetProcMap					// Proc Map
	},

	// Status 102 (0x66): LiftWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-2,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonLiftWaitProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyGetProcMap					// Proc Map
	},

	// Status 103 (0x67): LiftTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-2,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonLiftTurnProcUpdate,				// Proc Update
		ftCommonLiftTurnProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyGetProcMap					// Proc Map
	},

	// Status 104 (0x68): LightThrowDrop
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowDrop,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 105 (0x69): LightThrowDash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowDash,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 106 (0x6A): LightThrowF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowF,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 107 (0x6B): LightThrowB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowB,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 108 (0x6C): LightThrowHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowHi,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 109 (0x6D): LightThrowLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowLw,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 110 (0x6E): LightThrowF4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowF4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 111 (0x6F): LightThrowB4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowB4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 112 (0x70): LightThrowHi4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowHi4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 113 (0x71): LightThrowLw4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowLw4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 114 (0x72): LightThrowAirF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirF,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 115 (0x73): LightThrowAirB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirB,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 116 (0x74): LightThrowAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirHi,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 117 (0x75): LightThrowAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirLw,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 118 (0x76): LightThrowAirF4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirF4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 119 (0x77): LightThrowAirB4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirB4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 120 (0x78): LightThrowAirHi4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirHi4,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 121 (0x79): LightThrowAirLw4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLightThrowAirLw4,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		mpCommonProcFighterWaitOrLanding		// Proc Map
	},

	// Status 122 (0x7A): HeavyThrowF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowF,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyThrowProcMap				// Proc Map
	},

	// Status 123 (0x7B): HeavyThrowB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowB,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyThrowProcMap				// Proc Map
	},

	// Status 124 (0x7C): HeavyThrowF4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowF4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyThrowProcMap				// Proc Map
	},

	// Status 125 (0x7D): HeavyThrowB4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHeavyThrowB4,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDItemThrow,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonItemThrowProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonHeavyThrowProcMap				// Proc Map
	},

	// Status 126 (0x7E): SwordSwing1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSwordSwing1,				// Script ID
		nFTMotionAttackIDSwordSwing1,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSwordSwing1,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 127 (0x7F): SwordSwing3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSwordSwing3,				// Script ID
		nFTMotionAttackIDSwordSwing3,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSwordSwing3,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 128 (0x80): SwordSwing4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSwordSwing4,				// Script ID
		nFTMotionAttackIDSwordSwing4,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSwordSwing4,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 129 (0x81): SwordSwingDash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionSwordSwingDash,			// Script ID
		nFTMotionAttackIDSwordSwingDash,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDSwordSwingDash,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 130 (0x82): BatSwing1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionBatSwing1,				// Script ID
		nFTMotionAttackIDBatSwing1,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDBatSwing1,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 131 (0x83): BatSwing3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionBatSwing3,				// Script ID
		nFTMotionAttackIDBatSwing3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDBatSwing3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 132 (0x84): BatSwing4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionBatSwing4,				// Script ID
		nFTMotionAttackIDBatSwing4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDBatSwing4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 133 (0x85): BatSwingDash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionBatSwingDash,			// Script ID
		nFTMotionAttackIDBatSwingDash,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDBatSwingDash,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 134 (0x86): HarisenSwing1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHarisenSwing1,			// Script ID
		nFTMotionAttackIDHarisenSwing1,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHarisenSwing1,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHarisenSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 135 (0x87): HarisenSwing3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHarisenSwing3,			// Script ID
		nFTMotionAttackIDHarisenSwing3,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHarisenSwing3,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHarisenSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 136 (0x88): HarisenSwing4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHarisenSwing4,			// Script ID
		nFTMotionAttackIDHarisenSwing4,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHarisenSwing4,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHarisenSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 137 (0x89): HarisenSwingDash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHarisenSwingDash,		// Script ID
		nFTMotionAttackIDHarisenSwingDash,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHarisenSwingDash,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHarisenSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 138 (0x8A): StarRodSwing1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionStarRodSwing1,			// Script ID
		nFTMotionAttackIDStarRodSwing1,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDStarRodSwing1,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonStarRodSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 139 (0x8B): StarRodSwing3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionStarRodSwing3,			// Script ID
		nFTMotionAttackIDStarRodSwing3,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDStarRodSwing3,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonStarRodSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 140 (0x8C): StarRodSwing4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionStarRodSwing4,			// Script ID
		nFTMotionAttackIDStarRodSwing4,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDStarRodSwing4,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonStarRodSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 141 (0x8D): StarRodSwingDash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionStarRodSwingDash,		// Script ID
		nFTMotionAttackIDStarRodSwingDash,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDStarRodSwingDash,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonStarRodSwingProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 142 (0x8E): LGunShoot
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLGunShoot,				// Script ID
		nFTMotionAttackIDLGunShoot,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDLGunShoot,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonLGunShootProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonLGunShootProcMap				// Proc Map
	},

	// Status 143 (0x8F): LGunShootAir
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLGunShootAir,			// Script ID
		nFTMotionAttackIDLGunShoot,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDLGunShoot,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonLGunShootProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonLGunShootAirProcMap				// Proc Map
	},

	// Status 144 (0x90): FireFlowerShoot
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFireFlowerShoot,			// Script ID
		nFTMotionAttackIDFireFlowerShoot,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDFireFlowerShoot,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonFireFlowerShootProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonFireFlowerShootProcMap			// Proc Map
	},

	// Status 145 (0x91): FireFlowerShootAir
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFireFlowerShootAir,		// Script ID
		nFTMotionAttackIDFireFlowerShoot,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		TRUE,									// Is projectile?
		nFTStatusAttackIDFireFlowerShoot,		// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonFireFlowerShootProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonFireFlowerShootAirProcMap		// Proc Map
	},

	// Status 146 (0x92): HammerWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHammerWait,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHammer,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftHammerProcInterrupt,					// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftHammerProcMap							// Proc Map
	},

	// Status 147 (0x93): HammerWalk
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHammerWalk,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHammer,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonHammerWalkProcInterrupt,		// Proc Interrupt
		ftCommonWalkProcPhysics,				// Proc Physics
		ftHammerProcMap							// Proc Map
	},

	// Status 148 (0x94): HammerTurn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHammerWalk,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHammer,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHammerTurnProcUpdate,			// Proc Update
		ftCommonHammerTurnProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftHammerProcMap							// Proc Map
	},

	// Status 149 (0x95): HammerKneeBend
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHammerWalk,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHammer,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHammerKneeBendProcUpdate,		// Proc Update
		ftCommonHammerKneeBendProcInterrupt,	// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftHammerProcMap							// Proc Map
	},

	// Status 150 (0x96): HammerFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHammerWalk,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHammer,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonHammerFallProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyAirVelDriftFastFall,		// Proc Physics
		ftCommonHammerFallProcMap				// Proc Map
	},

	// Status 151 (0x97): HammerLanding
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionHammerWalk,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDHammer,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonHammerLandingProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftHammerProcMap							// Proc Map
	},

	// Status 152 (0x98): GuardOn
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionGuardOn,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonGuardOnProcUpdate,				// Proc Update
		ftCommonGuardCommonProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 153 (0x99): Guard
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonGuardProcUpdate,				// Proc Update
		ftCommonGuardCommonProcInterrupt,		// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 154 (0x9A): GuardOff
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionGuardOff,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonGuardOffProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 155 (0x9B): GuardSetOff
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonGuardSetOffProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 156 (0x9C): EscapeF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionEscapeF,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonEscapeProcUpdate,				// Proc Update
		ftCommonEscapeProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 157 (0x9D): EscapeB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionEscapeB,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonEscapeProcUpdate,				// Proc Update
		ftCommonEscapeProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 158 (0x9E): ShieldBreakFly
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShieldBreakFly,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonShieldBreakFlyProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftCommonShieldBreakFlyProcMap			// Proc Map
	},

	// Status 159 (0x9F): ShieldBreakFall
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShieldBreakFall,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelFriction,			// Proc Physics
		ftCommonShieldBreakFallProcMap			// Proc Map
	},

	// Status 160 (0xA0): ShieldBreakDownD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShieldBreakDownD,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonShieldBreakDownProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 161 (0xA1): ShieldBreakDownU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShieldBreakDownU,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonShieldBreakDownProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 162 (0xA2): ShieldBreakStandD
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShieldBreakStandD,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonShieldBreakStandProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 163 (0xA3): ShieldBreakStandU
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShieldBreakStandU,		// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonShieldBreakStandProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 164 (0xA4): FuraFura
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFuraFura,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonFuraFuraProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 165 (0xA5): FuraSleep
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionFuraSleep,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonFuraSleepProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnGroundBreak		// Proc Map
	},

	// Status 166 (0xA6): Catch
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCatch,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCatchProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonCatchProcMap					// Proc Map
	},

	// Status 167 (0xA7): CatchPull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCatchPull,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonCatchPullProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonCatchProcMap					// Proc Map
	},

	// Status 168 (0xA8): CatchWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-2,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonCatchWaitProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		ftCommonCatchProcMap					// Proc Map
	},

	// Status 169 (0xA9): ThrowF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrowF,					// Script ID
		nFTMotionAttackIDThrowF,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowF,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrowProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		ftCommonCatchProcMap					// Proc Map
	},

	// Status 170 (0xAA): ThrowB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrowB,					// Script ID
		nFTMotionAttackIDThrowB,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDThrowB,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrowProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		ftCommonCatchProcMap					// Proc Map
	},

	// Status 171 (0xAB): CapturePulled
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCapturePulled,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCapturePulledProcPhysics,		// Proc Physics
		ftCommonCapturePulledProcMap			// Proc Map
	},

	// Status 172 (0xAC): CaptureWait
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-2,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCapturePulledProcPhysics,		// Proc Physics
		ftCommonCaptureWaitProcMap				// Proc Map
	},

	// Status 173 (0xAD): CaptureKirby
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionDamageFall,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCaptureKirbyProcPhysics,		// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 174 (0xAE): CaptureWaitKirby
	{
		/////////////////////////// Motion Info ////////////////////////////////
		-1,										// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonCaptureWaitKirbyProcInterrupt,	// Proc Interrupt
		NULL,									// Proc Physics
		ftCommonCaptureWaitKirbyProcMap			// Proc Map
	},

	// Status 175 (0xAF): ThrownKirbyStar
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownKirbyStar,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrownKirbyStarProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownKirbyStarProcPhysics,		// Proc Physics
		ftCommonThrownCommonStarProcMap			// Proc Map
	},

	// Status 176 (0xB0): ThrownCopyStar
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownCopyStar,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrownCopyStarProcUpdate,		// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownCopyStarProcPhysics,		// Proc Physics
		ftCommonThrownCommonStarProcMap			// Proc Map
	},

	// Status 177 (0xB1): CaptureYoshi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCapturePulled,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCaptureYoshiProcPhysics,		// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 178 (0xB2): YoshiEgg
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionYoshiEgg,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonYoshiEggProcUpdate,				// Proc Update
		ftCommonYoshiEggProcInterrupt,			// Proc Interrupt
		ftCommonYoshiEggProcPhysics,			// Proc Physics
		ftCommonYoshiEggProcMap					// Proc Map
	},

	// Status 179 (0xB3): CaptureCaptain
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionCaptureCaptain,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonCaptureCaptainProcPhysics,		// Proc Physics
		mpCommonUpdateFighterProjectFloor				// Proc Map
	},

	// Status 180 (0xB4): ThrownDonkeyUnk
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownDonkeyUnk,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		NULL,									// Proc Physics
		NULL									// Proc Map
	},

	// Status 181 (0xB5): ThrownDonkeyF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownDonkeyF,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrownProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 182 (0xB6): ThrownMarioB1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownMarioB1,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrownProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 183 (0xB7): ThrownUnk1
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownUnk1,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonThrownProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 184 (0xB8): Shouldered
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionShouldered,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		ftCommonCaptureShoulderedProcInterrupt,	// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 185 (0xB9): ThrownMarioB2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownMarioB2,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 186 (0xBA): ThrownCommon
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownCommon,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 187 (0xBB): ThrownUnk2
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownUnk2,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 188 (0xBC): ThrownUnk3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionThrownUnk3,				// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		NULL,									// Proc Update
		NULL,									// Proc Interrupt
		ftCommonThrownProcPhysics,				// Proc Physics
		ftCommonThrownProcMap					// Proc Map
	},

	// Status 189 (0xBD): Appeal
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAppeal,					// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		ftCommonAppealProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 190 (0xBE): Attack11
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttack11,				// Script ID
		nFTMotionAttackIDAttack11,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack11,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack11ProcUpdate,				// Proc Update
		ftCommonAttack11ProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 191 (0xBF): Attack12
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttack12,				// Script ID
		nFTMotionAttackIDAttack12,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttack12,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttack12ProcUpdate,				// Proc Update
		ftCommonAttack12ProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 192 (0xC0): AttackDash
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackDash,				// Script ID
		nFTMotionAttackIDAttackDash,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackDash,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelTransN,			// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 193 (0xC1): AttackS3Hi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS3Hi,				// Script ID
		nFTMotionAttackIDAttackS3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 194 (0xC2): AttackS3HiS
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS3HiS,				// Script ID
		nFTMotionAttackIDAttackS3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 195 (0xC3): AttackS3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS3,				// Script ID
		nFTMotionAttackIDAttackS3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 196 (0xC4): AttackS3LwS
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS3LwS,				// Script ID
		nFTMotionAttackIDAttackS3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 197 (0xC5): AttackS3Lw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS3Lw,				// Script ID
		nFTMotionAttackIDAttackS3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 198 (0xC6): AttackHi3F
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackHi3F,				// Script ID
		nFTMotionAttackIDAttackHi3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackHi3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 199 (0xC7): AttackHi3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackHi3,				// Script ID
		nFTMotionAttackIDAttackHi3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackHi3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 200 (0xC8): AttackHi3B
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackHi3B,				// Script ID
		nFTMotionAttackIDAttackHi3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackHi3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 201 (0xC9): AttackLw3
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackLw3,				// Script ID
		nFTMotionAttackIDAttackLw3,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackLw3,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackLw3ProcUpdate,			// Proc Update
		ftCommonAttackLw3ProcInterrupt,			// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 202 (0xCA): AttackS4Hi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS4Hi,				// Script ID
		nFTMotionAttackIDAttackS4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackS4ProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 203 (0xCB): AttackS4HiS
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS4HiS,				// Script ID
		nFTMotionAttackIDAttackS4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackS4ProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 204 (0xCC): AttackS4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS4,				// Script ID
		nFTMotionAttackIDAttackS4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackS4ProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 205 (0xCD): AttackS4LwS
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS4LwS,				// Script ID
		nFTMotionAttackIDAttackS4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackS4ProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 206 (0xCE): AttackS4Lw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackS4Lw,				// Script ID
		nFTMotionAttackIDAttackS4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackS4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackS4ProcUpdate,				// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 207 (0xCF): AttackHi4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackHi4,				// Script ID
		nFTMotionAttackIDAttackHi4,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackHi4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundFrictionOrTransN,	// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 208 (0xD0): AttackLw4
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackLw4,				// Script ID
		nFTMotionAttackIDAttackLw4,				// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		TRUE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackLw4,				// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonSetFighterFallOnEdgeBreak		// Proc Map
	},

	// Status 209 (0xD1): AttackAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackAirN,				// Script ID
		nFTMotionAttackIDAttackAirN,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonAttackAirProcMap				// Proc Map
	},

	// Status 210 (0xD2): AttackAirF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackAirF,				// Script ID
		nFTMotionAttackIDAttackAirF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonAttackAirProcMap				// Proc Map
	},

	// Status 211 (0xD3): AttackAirB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackAirB,				// Script ID
		nFTMotionAttackIDAttackAirB,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirB,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonAttackAirProcMap				// Proc Map
	},

	// Status 212 (0xD4): AttackAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackAirHi,				// Script ID
		nFTMotionAttackIDAttackAirHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetFall,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonAttackAirProcMap				// Proc Map
	},

	// Status 213 (0xD5): AttackAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionAttackAirLw,				// Script ID
		nFTMotionAttackIDAttackAirLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftCommonAttackAirLwProcUpdate,			// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyAirVelDrift,				// Proc Physics
		ftCommonAttackAirProcMap				// Proc Map
	},

	// Status 214 (0xD6): LandingAirN
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingAirN,			// Script ID
		nFTMotionAttackIDAttackAirN,		// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirN,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 215 (0xD7): LandingAirF
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingAirF,				// Script ID
		nFTMotionAttackIDAttackAirF,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirF,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 216 (0xD8): LandingAirB
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingAirB,				// Script ID
		nFTMotionAttackIDAttackAirB,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirB,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 217 (0xD9): LandingAirHi
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingAirHi,			// Script ID
		nFTMotionAttackIDAttackAirHi,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirHi,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 218 (0xDA): LandingAirLw
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingAirLw,			// Script ID
		nFTMotionAttackIDAttackAirLw,			// Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsAir,							// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDAttackAirLw,			// Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	},

	// Status 219 (0xDB): LandingAirNull
	{
		/////////////////////////// Motion Info ////////////////////////////////
		nFTCommonMotionLandingAirNull,			// Script ID
		nFTMotionAttackIDNone,				    // Motion attack ID

		/////////////////////////// Status Info ////////////////////////////////
		0,										// Unused
		FALSE,									// Is Smash attack?
		nMPKineticsGround,						// Grounded or aerial attack?
		FALSE,									// Is projectile?
		nFTStatusAttackIDNone,				    // Status attack ID

		//////////////////////// Process Callbacks /////////////////////////////
		ftAnimEndSetWait,		            	// Proc Update
		NULL,									// Proc Interrupt
		ftPhysicsApplyGroundVelFriction,		// Proc Physics
		mpCommonProcFighterOnCliffEdge			// Proc Map
	}
};

#endif
