#include <ft/fighter.h>
#include <it/item.h>
#include <gr/ground.h>
#include <if/interface.h>
#include <sc/scene.h>
#include <sys/video.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018EEC0
s32 dSC1PBonusStageUnused0x8018EEC0 = 0;

// 0x8018EEC4 - column 0 = start offset, column 1 = DObjDesc*, column 2 = AObjEvent32**
GRBonusTarget dSC1PBonusStageTargetDescs[/* */] =
{
	// Bonus1Mario
	{ 
		&llGRBonus1MarioMapTargetsStart,
		&llGRBonus1MarioMapTargetsDObjDesc,
		&llGRBonus1MarioMapTargetsAnimJoint
	},

	// Bonus1Fox
	{ 
		&llGRBonus1FoxMapTargetsStart,
		&llGRBonus1FoxMapTargetsDObjDesc,
		&llGRBonus1FoxMapTargetsAnimJoint,
	},

	// Bonus1Donkey
	{ 
		&llGRBonus1DonkeyMapTargetsStart,
		&llGRBonus1DonkeyMapTargetsDObjDesc,
		&llGRBonus1DonkeyMapTargetsAnimJoint
	},

	// Bonus1Samus
	{ 
		&llGRBonus1SamusMapTargetsStart,
		&llGRBonus1SamusMapTargetsDObjDesc,
		&llGRBonus1SamusMapTargetsAnimJoint
	},

	// Bonus1Luigi
	{ 
		&llGRBonus1LuigiMapTargetsStart,
		&llGRBonus1LuigiMapTargetsDObjDesc,
		&llGRBonus1LuigiMapTargetsAnimJoint
	},

	// Bonus1Link
	{ 
		&llGRBonus1LinkMapTargetsStart,
		&llGRBonus1LinkMapTargetsDObjDesc,
		&llGRBonus1LinkMapTargetsAnimJoint
	},

	// Bonus1Yoshi
	{ 
		&llGRBonus1YoshiMapTargetsStart,
		&llGRBonus1YoshiMapTargetsDObjDesc,
		&llGRBonus1YoshiMapTargetsAnimJoint
	},

	// Bonus1Captain
	{ 
		&llGRBonus1CaptainMapTargetsStart,
		&llGRBonus1CaptainMapTargetsDObjDesc,
		&llGRBonus1CaptainMapTargetsAnimJoint
	},

	// Bonus1Kirby
	{ 
		&llGRBonus1KirbyMapTargetsStart,
		&llGRBonus1KirbyMapTargetsDObjDesc,
		&llGRBonus1KirbyMapTargetsAnimJoint
	},

	// Bonus1Pikachu
	{ 
		&llGRBonus1PikachuMapTargetsStart,
		&llGRBonus1PikachuMapTargetsDObjDesc,
		&llGRBonus1PikachuMapTargetsAnimJoint
	},

	// Bonus1Purin
	{ 
		&llGRBonus1PurinMapTargetsStart,
		&llGRBonus1PurinMapTargetsDObjDesc,
		&llGRBonus1PurinMapTargetsAnimJoint
	},

	// Bonus1Ness
	{ 
		&llGRBonus1NessMapTargetsStart,
		&llGRBonus1NessMapTargetsDObjDesc,
		&llGRBonus1NessMapTargetsAnimJoint
	}
};

// 0x8018EF54 - column 0 = DObjDesc offset, column 1 = AObjEvent32 offset
intptr_t dSC1PBonusStageBumperDescs[/* */][2] =
{
	// Mario
	{
		0x0,
		0x0
	},

	// Fox
	{
		&llGRBonus2FoxMapBumpersDObjDesc,
		&llGRBonus2FoxMapBumpersAnimJoint
	},

	// Donkey
	{
		0x0,
		0x0
	},

	// Samus
	{
		&llGRBonus2SamusMapBumpersDObjDesc,
		&llGRBonus2SamusMapBumpersAnimJoint
	},

	// Luigi
	{
		0x0,
		0x0
	},

	// Link
	{
		0x0,
		0x0
	},

	// Yoshi
	{
		0x0,
		0x0
	},

	// Captain
	{
		0x0,
		0x0
	},

	// Kirby
	{
		&llGRBonus2KirbyMapBumpersDObjDesc,
		&llGRBonus2KirbyMapBumpersAnimJoint
	},

	// Pikachu
	{
		0x0,
		0x0
	},

	// Purin
	{
		&llGRBonus2PurinMapBumpersDObjDesc,
		&llGRBonus2PurinMapBumpersAnimJoint
	},

	// Ness
	{
		&llGRBonus2NessMapBumpersDObjDesc,
		&llGRBonus2NessMapBumpersAnimJoint
	}
};

// 0x8018EFB4 - column 0 = DObjDesc***, column 1 = AObjEvent32**, column 2 = MObjSub***, column 3 = AObjEvent32***
intptr_t dSC1PBonusStagePlatformDescs[/* */][4] =
{
	// Small
	{
		&llBonus2CommonPlatformSmallDObjDesc,
		&llBonus2CommonPlatformSmallAnimJoint,
		&llBonus2CommonPlatformSmallMObjSub,
		&llBonus2CommonPlatformSmallMatAnimJoint
	},

	// Medium
	{
		&llBonus2CommonPlatformMediumDObjDesc,
		&llBonus2CommonPlatformMediumAnimJoint,
		&llBonus2CommonPlatformMediumMObjSub,
		&llBonus2CommonPlatformMediumMatAnimJoint
	},

	// Large
	{
		&llBonus2CommonPlatformLargeDObjDesc,
		&llBonus2CommonPlatformLargeAnimJoint,
		&llBonus2CommonPlatformLargeMObjSub,
		&llBonus2CommonPlatformLargeMatAnimJoint
	}
};

// 0x8018EFE4 - column 0 = DObjDesc*, column 1 = AObjEvent32**
intptr_t dSC1PBonusStageBoardedPlatformDescs[/* */][2] =
{
	// Small
	{
		&llBonus2CommonBoardedPlatformSmallDObjDesc,
		&llBonus2CommonBoardedPlatformSmallAnimJoint
	},

	// Medium
	{
		&llBonus2CommonBoardedPlatformMediumDObjDesc,
		&llBonus2CommonBoardedPlatformMediumAnimJoint
	},

	// Large
	{
		&llBonus2CommonBoardedPlatformLargeDObjDesc,
		&llBonus2CommonBoardedPlatformLargeAnimJoint
	}
};

// 0x8018EFFC - last one is apparently 0.554000020027F but precision is lost
f32 dSC1PBonusStageTimerUnitLengths[/* */] =
{
	I_MIN_TO_TICS(10),
	I_MIN_TO_TICS(1),
	I_SEC_TO_TICS(10),
	I_SEC_TO_TICS(1),
	I_SEC_TO_TICS(0.1),
	277.0F / 500.0F
};

// 0x8018F014
s32 dSC1PBonusStageTimerDigitPositions[/* */] = { 207, 222, 240, 255, 273, 288 };

// 0x8018F02C
s32 dSC1PBonusStagePlayerInterfacePositions[/* */] = { 55, 55, 55, 55 };

// 0x8018F03C
SYColorRGBA dSC1PBonusStageFadeColor = { 0x00, 0x00, 0x00, 0x00 };

// 0x8018F040
Lights1 dSC1PBonusStageLights1 = gdSPDefLights1(0x20, 0x20, 0x20, 0xFF, 0xFF, 0xFF, 0x32, 0x32, 0x32);

// 0x8018F058
Gfx dSC1PBonusStageDisplayList[/* */] =
{
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPSetLights1(dSC1PBonusStageLights1),
	gsSPEndDisplayList()
};

// 0x8018F080
SYVideoSetup dSC1PBonusStageVideoSetup = SYVIDEO_SETUP_DEFAULT();

// 0x8018F09C
SYTaskmanSetup dSC1PBonusStageTaskmanSetup =
{
    // Task Manager Buffer Setup
    {
        0,                          // ???
        sc1PBonusStageFuncUpdate,   // Update function
        scManagerFuncDraw,          // Frame draw function
        &ovl6_BSS_END,             	// Allocatable memory pool start
        0,                          // Allocatable memory pool size
        1,                          // ???
        2,                          // Number of contexts?
        sizeof(Gfx) * 7680,         // Display List Buffer 0 Size
        sizeof(Gfx) * 2560,         // Display List Buffer 1 Size
        0,                          // Display List Buffer 2 Size
        0,                          // Display List Buffer 3 Size
        0xD000,                     // Graphics Heap Size
        2,                          // ???
        0xC000,                     // RDP Output Buffer Size
        sc1PBonusStageFuncLights,   // Pre-render function
        syControllerFuncRead,       // Controller I/O function
    },

    0,                              // Number of GObjThreads
    sizeof(u64) * 192,              // Thread stack size
    0,                              // Number of thread stacks
    0,                              // ???
    0,                              // Number of GObjProcesses
    0,                              // Number of GObjs
    sizeof(GObj),                   // GObj size
    0,                              // Number of XObjs
    dLBCommonFuncMatrixList,        // Matrix function list
    NULL,                           // DObjVec eject function
    0,                              // Number of AObjs
    0,                              // Number of MObjs
    0,                              // Number of DObjs
    sizeof(DObj),                   // DObj size
    0,                              // Number of SObjs
    sizeof(SObj),                   // SObj size
    0,                              // Number of CObjs
    sizeof(CObj),                 	// CObj size
    
    sc1PBonusStageFuncStart         // Task start function
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018F1A0
void *gSC1PBonusStageItemFile;

// 0x8018F1A8
s32 gSC1PBonusStagePad0x8018F1A4[2];

// 0x8018F1B0
SCBattleState sSC1PBonusStageBattleState;

// 0x8018F3A0
u8 sSC1PBonusStageTimerDigits[6];

// 0x8018F3A8
sb32 sSC1PBonusStageIsTimeUp;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8018D0C0
void func_ovl6_8018D0C0(void)
{
	return;
}

// 0x8018D0C8
void func_ovl6_8018D0C8(void)
{
	return;
}

// 0x8018D0D0
void sc1PBonusStageFuncUpdate(void)
{
	ifCommonBattleUpdateInterfaceAll();
}

// 0x8018D0F0
void sc1PBonusStageInitVars(void)
{
	s32 player;
	s32 fkind;

	gSCManagerSceneData.is_reset = FALSE;

	sSC1PBonusStageBattleState = dSCManagerDefaultBattleState;
	gSCManagerBattleState = &sSC1PBonusStageBattleState;

	gSCManagerBattleState->game_type = nSCBattleGameTypeBonus;
	gSCManagerBattleState->game_rules = SCBATTLE_GAMERULE_BONUS | SCBATTLE_GAMERULE_TIME;
	gSCManagerBattleState->is_show_score = FALSE;
	gSCManagerBattleState->pl_count = 1;
	gSCManagerBattleState->cp_count = 0;

	if (gSCManagerSceneData.scene_prev == nSCKind1PGame)
	{
		fkind = gSCManagerSceneData.fkind;

		gSCManagerBattleState->time_limit = SCBATTLE_TIMELIMIT_INFINITE;

		if (gSCManagerSceneData.spgame_stage == nSC1PGameStageBonus1)
		{
			if (gSCManagerSceneData.spgame_time_limit != SCBATTLE_TIMELIMIT_INFINITE)
			{
				gSCManagerBattleState->time_limit = 2;
			}
			gSCManagerBattleState->gkind = fkind + nGRKindBonus1Start;
		}
		else
		{
			if (gSCManagerSceneData.spgame_time_limit != SCBATTLE_TIMELIMIT_INFINITE)
			{
				gSCManagerBattleState->time_limit = 2;
			}
			gSCManagerBattleState->gkind = fkind + nGRKindBonus2Start;
		}
	}
	else
	{
		fkind = gSCManagerSceneData.bonus_fkind;

		gSCManagerBattleState->time_limit = SCBATTLE_TIMELIMIT_INFINITE;

		if (gSCManagerSceneData.scene_prev == nSCKind1PBonus1Players)
		{
			gSCManagerBattleState->gkind = fkind + nGRKindBonus1Start;
		}
		else gSCManagerBattleState->gkind = fkind + nGRKindBonus2Start;
	}
	for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
	{
		if (player == gSCManagerSceneData.player)
		{
			gSCManagerBattleState->players[player].pkind = nFTPlayerKindMan;
			gSCManagerBattleState->players[player].fkind = fkind;

			if (gSCManagerSceneData.scene_prev == nSCKind1PGame)
			{
				gSCManagerBattleState->players[player].costume = gSCManagerSceneData.costume;
			}
			else gSCManagerBattleState->players[player].costume = gSCManagerSceneData.bonus_costume;

			gSCManagerBattleState->players[player].color = player;
		}
		else gSCManagerBattleState->players[player].pkind = nFTPlayerKindNot;
	}
}

// 0x8018D330
void sc1PBonusStageBonus1LoadFile(void)
{
	gSC1PBonusStageItemFile = lbRelocGetExternHeapFile((u32)&ll_253_FileID, syTaskmanMalloc(lbRelocGetFileSize((u32)&ll_253_FileID), 0x10));
}

// 0x8018D374
void sc1PBonusStageMakeTargets(void)
{
	GRBonusTarget *target = &dSC1PBonusStageTargetDescs[gSCManagerBattleState->gkind - nGRKindBonus1Start];
	AObjEvent32 **anim_joints;
	DObjDesc *dobjdesc;
	Vec3f vel;

	vel.x = vel.y = vel.z = 0.0F;

	dobjdesc = lbRelocGetFileData(DObjDesc*, ((uintptr_t)gMPCollisionGroundData->gr_desc[1].dobjdesc - target->start), target->dobjdesc);
	anim_joints = lbRelocGetFileData(AObjEvent32**, ((uintptr_t)gMPCollisionGroundData->gr_desc[1].dobjdesc - target->start), target->anim_joint);

	gGRCommonStruct.bonus1.target_count = 0;

	dobjdesc++, anim_joints++;

	while (dobjdesc->id != DOBJ_ARRAY_MAX)
	{
		GObj *item_gobj = itManagerMakeItemSetupCommon(NULL, nITKindTarget, &dobjdesc->translate, &vel, ITEM_FLAG_PARENT_GROUND);

		if (*anim_joints != NULL)
		{
			gcAddDObjAnimJoint(DObjGetStruct(item_gobj), *anim_joints, 0.0F);
			gcPlayAnimAll(item_gobj);
		}
		dobjdesc++, anim_joints++, gGRCommonStruct.bonus1.target_count++;
	}
	if (gGRCommonStruct.bonus1.target_count != SCBATTLE_BONUSGAME_TASK_MAX)
	{
		syDebugPrintf("Error : not %d targets!\n", SCBATTLE_BONUSGAME_TASK_MAX);

		while (TRUE);
	}
}

// 0x8018D4C4
void sc1PBonusStageUpdateTargetInterface(void)
{
	SObj *sobj = SObjGetStruct(gGRCommonStruct.bonus1.interface_gobj);
	s32 i;

	for (i = 0; i < gGRCommonStruct.bonus1.target_count; i++)
	{
		sobj = sobj->next;
	}
	gcEjectSObj(sobj);
}

// 0x8018D510
void sc1PBonusStageUpdateTargetCount(void)
{
	gGRCommonStruct.bonus1.target_count--;

	sc1PBonusStageUpdateTargetInterface();

	if (gGRCommonStruct.bonus1.target_count == 0)
	{
		if
		(
			(gSCManagerSceneData.scene_prev != nSCKind1PGame) &&
			(gSCManagerBackupData.spgame_records[gSCManagerSceneData.bonus_fkind].bonus1_task_count == SCBATTLE_BONUSGAME_TASK_MAX) &&
			(gSCManagerBattleState->time_passed < gSCManagerBackupData.spgame_records[gSCManagerSceneData.bonus_fkind].bonus1_time)
		)
		{
			ifCommonAnnounceCompleteInitInterface(nSYAudioVoiceAnnounceNewRecord);
		}
		else ifCommonAnnounceCompleteInitInterface(nSYAudioVoiceAnnounceComplete);

		ifCommonBattleEndAddSoundQueueID(nSYAudioFGMBonus1TargetBreak);
	}
}

// 0x8018D5C8
void sc1PBonusStageMakeBonus1Ground(void)
{
	sc1PBonusStageMakeTargets();
}

// 0x8018D5E8
void sc1PBonusStageBonus2LoadFile(void)
{
	gGRCommonStruct.bonus2.file = lbRelocGetExternHeapFile((u32)&llBonus2CommonFileID, syTaskmanMalloc(lbRelocGetFileSize((u32)&llBonus2CommonFileID), 0x10));
}

// 0x8018D62C
s32 sc1PBonusStageGetPlatformKind(s32 line_id)
{
	Vec3f pos_left;
	Vec3f pos_right;

	mpCollisionGetFloorEdgeL(line_id, &pos_left);
	mpCollisionGetFloorEdgeR(line_id, &pos_right);

	if ((pos_right.x - pos_left.x) <= 750.0F)
	{
		return 0;
	}
	else if ((pos_right.x - pos_left.x) <= 1050.0F)
	{
		return 1;
	}
	else return 2;
}

// 0x8018D6A8
void sc1PBonusStageInitPlatforms(s32 line_id)
{
	DObj *dobj;
	s32 id;

	id = mpCollisionSetDObjNoID(line_id);
	dobj = gMPCollisionYakumonoDObjs->dobjs[id];
	id = sc1PBonusStageGetPlatformKind(line_id);

	lbCommonSetupTreeDObjs
	(
		dobj,
		lbRelocGetFileData(DObjDesc*, gGRCommonStruct.bonus2.file, dSC1PBonusStagePlatformDescs[id][0]),
		NULL,
		0x44,
		nGCMatrixKindNull,
		nGCMatrixKindNull
	);
	lbCommonAddMObjForTreeDObjs(dobj->child, lbRelocGetFileData(MObjSub***, gGRCommonStruct.bonus2.file, dSC1PBonusStagePlatformDescs[id][2]));

	lbCommonAddTreeDObjsAnimAll
	(
		dobj->child,
		lbRelocGetFileData(AObjEvent32**, gGRCommonStruct.bonus2.file, dSC1PBonusStagePlatformDescs[id][1]),
		lbRelocGetFileData(AObjEvent32***, gGRCommonStruct.bonus2.file, dSC1PBonusStagePlatformDescs[id][3]),
		0.0F
	);
	lbCommonPlayTreeDObjsAnim(dobj->child);

	dobj->child->user_data.s = id | 0x8000;
}

// 0x8018D794
void sc1PBonusStageMakePlatforms(void)
{
	s32 line_count;
	s32 line_ids[100];
	s32 yakumono_id;
	s32 i;

	line_count = mpCollisionGetLineCountType(nMPLineKindFloor);
	gGRCommonStruct.bonus2.platform_count = 0;
	mpCollisionGetLineIDsTypeCount(nMPLineKindFloor, line_count, line_ids);

	for (i = 0; i < line_count; i++)
	{
		if ((mpCollisionGetVertexFlagsLineID(line_ids[i]) & MAP_VERTEX_MAT_MASK) == nMPMaterialDetect)
		{
			yakumono_id = mpCollisionSetDObjNoID(line_ids[i]);

			if (gMPCollisionYakumonoDObjs->dobjs[yakumono_id]->anim_joint.event32 == NULL)
			{
				mpCollisionSetYakumonoOnID(yakumono_id);
			}
			sc1PBonusStageInitPlatforms(line_ids[i]);

			gGRCommonStruct.bonus2.platform_count++;
		}
	}
}

// 0x8018D890
void sc1PBonusStageUpdatePlatformInterface(void)
{
	SObj* sobj = SObjGetStruct(gGRCommonStruct.bonus2.interface_gobj);
	s32 i;

	for (i = 0; i < gGRCommonStruct.bonus2.platform_count; i++)
	{
		sobj = sobj->next;
	}
	gcEjectSObj(sobj);
}

// 0x8018D8DC
void sc1PBonusStageUpdatePlatformCount(DObj *dobj)
{
	s32 id = dobj->child->user_data.s & ~0x8000;

	gcEjectDObj(dobj->child);

	lbCommonSetupTreeDObjs
	(
		dobj,
		lbRelocGetFileData(DObjDesc*, gGRCommonStruct.bonus2.file, dSC1PBonusStageBoardedPlatformDescs[id][0]),
		NULL,
		0x44,
		0,
		0
	);
	lbCommonAddDObjAnimJointAll
	(
		dobj->child,
		lbRelocGetFileData(AObjEvent32**, gGRCommonStruct.bonus2.file, dSC1PBonusStageBoardedPlatformDescs[id][1]),
		0.0F
	);
	lbCommonPlayTreeDObjsAnim(dobj->child);

	gGRCommonStruct.bonus2.platform_count--;

	sc1PBonusStageUpdatePlatformInterface();
	func_800269C0_275C0(nSYAudioFGMBonus2PlatformLanding);

	if (gGRCommonStruct.bonus2.platform_count == 0)
	{
		if
		(
			(gSCManagerSceneData.scene_prev != nSCKind1PGame) &&
			(gSCManagerBackupData.spgame_records[gSCManagerSceneData.bonus_fkind].bonus2_task_count == SCBATTLE_BONUSGAME_TASK_MAX) &&
			(gSCManagerBattleState->time_passed < gSCManagerBackupData.spgame_records[gSCManagerSceneData.bonus_fkind].bonus2_time)
		)
		{
			ifCommonAnnounceCompleteInitInterface(nSYAudioVoiceAnnounceNewRecord);
		}
		else ifCommonAnnounceCompleteInitInterface(nSYAudioVoiceAnnounceComplete);

		ifCommonBattleEndAddSoundQueueID(nSYAudioFGMBonus2PlatformLanding);
	}
}

// 0x8018DA2C
void sc1PBonusStageBonus2ProcUpdate(GObj *ground_gobj)
{
	GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

	while (fighter_gobj != NULL)
	{
		FTStruct *fp = ftGetStruct(fighter_gobj);

		if ((fp->ga == nMPKineticsGround) && ((fp->coll_data.floor_flags & MAP_VERTEX_MAT_MASK) == nMPMaterialDetect))
		{
			DObj *dobj = gMPCollisionYakumonoDObjs->dobjs[mpCollisionSetDObjNoID(fp->coll_data.floor_line_id)];

			if (dobj->child->user_data.s != nMPYakumonoStatusNone)
			{
				sc1PBonusStageUpdatePlatformCount(dobj);
			}
		}
		fighter_gobj = fighter_gobj->link_next;
	}
}

// 0x8018DAE0
void sc1PBonusStageMakeBonus2Ground(void)
{
	gcAddGObjProcess
	(
		gcMakeGObjSPAfter
		(
			nGCCommonKindGround,
			NULL,
			nGCCommonLinkIDGround,
			GOBJ_PRIORITY_DEFAULT
		),
		sc1PBonusStageBonus2ProcUpdate,
		nGCProcessKindFunc,
		4
	);
}

// 0x8018DB24
void sc1PBonusStageMakeBumpers(void)
{
	void *file;
	DObjDesc *dobjdesc;
	AObjEvent32 **anim_joints;
	Vec3f vel;

	if (gMPCollisionGroundData->map_nodes != NULL)
	{
		file = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - dSC1PBonusStageBumperDescs[gSCManagerBattleState->gkind - nGRKindBonus2Start][0]);

		vel.x = vel.y = vel.z = 0.0F;

		dobjdesc = lbRelocGetFileData(DObjDesc*, file, dSC1PBonusStageBumperDescs[gSCManagerBattleState->gkind - nGRKindBonus2Start][0]);
		anim_joints = lbRelocGetFileData(AObjEvent32**, file, dSC1PBonusStageBumperDescs[gSCManagerBattleState->gkind - nGRKindBonus2Start][1]);

		dobjdesc++, anim_joints++;

		while (dobjdesc->id != DOBJ_ARRAY_MAX)
		{
			GObj *item_gobj = itManagerMakeItemSetupCommon(NULL, nITKindGBumper, &dobjdesc->translate, &vel, ITEM_FLAG_PARENT_GROUND);

			if (*anim_joints != NULL)
			{
				gcAddDObjAnimJoint(DObjGetStruct(item_gobj), *anim_joints, 0.0F);
				gcPlayAnimAll(item_gobj);
			}
			dobjdesc++, anim_joints++;
		}
	}
}

// 0x8018DC38
void sc1PBonusStageInitBonus2(void)
{
	sc1PBonusStageMakeBumpers();
	sc1PBonusStageBonus2LoadFile();
	sc1PBonusStageMakePlatforms();
	sc1PBonusStageMakeBonus2Ground();
}

// 0x8018DC70
void sc1PBonusStageInterfaceThreadUpdate(GObj *interface_gobj)
{
	gcSleepCurrentGObjThread(60);
	ifCommonAnnounceGoMakeInterface();
	ifCommonPlayerDamageSetShowInterface();
	func_800269C0_275C0(nSYAudioVoiceAnnounceGo);
	ifCommonAnnounceGoSetStatus();
	gcEjectGObj(NULL);
	gcSleepCurrentGObjThread(1);
}

// 0x8018DCC4
void sc1PBonusStageMakeInterface(void)
{
	gcAddGObjProcess
	(
		gcMakeGObjSPAfter
		(
			nGCCommonKindInterface,
			NULL,
			nGCCommonLinkIDInterfaceActor,
			GOBJ_PRIORITY_DEFAULT
		),
		sc1PBonusStageInterfaceThreadUpdate,
		nGCProcessKindThread,
		5
	);
	gSCManagerBattleState->game_status = nSCBattleGameStatusWait;
}

// 0x8018DD14
void sc1PBonusStageInitCamera(void)
{
	s32 player;

	for (player = 0; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
	{
		if (gSCManagerBattleState->players[player].pkind == nFTPlayerKindNot)
		{
			continue;
		}
		if (gSCManagerBattleState->gkind >= nGRKindBonus2Start)
		{
			gmCameraSetStatusPlayerFollow(gSCManagerBattleState->players[player].fighter_gobj, 0.0F, F_CLC_DTOR32(-15.0F), 9000.0F, 0.3F, 31.5F);
		}
		else gmCameraSetStatusPlayerFollow(gSCManagerBattleState->players[player].fighter_gobj, 0.0F, F_CLC_DTOR32(-9.0F), 9000.0F, 0.3F, 31.5F);

		break;
	}
}

// 0x8018DDE0
void sc1PBonusStageMakeTargetSprites(void)
{
	GObj *interface_gobj;
	SObj *sobj;
	void *file;
	s32 i;

	file = lbRelocGetExternHeapFile(&llSC1PStageClear3FileID, syTaskmanMalloc(lbRelocGetFileSize(&llSC1PStageClear3FileID), 0x10));

	gGRCommonStruct.bonus1.interface_gobj = interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; i < gGRCommonStruct.bonus1.target_count; i++)
	{
		sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, file, &llSC1PStageClear3TargetSprite));
		sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

		sobj->pos.x = -(sobj->sprite.width / 2) + (((sobj->sprite.width + 3) * i) + 30);
		sobj->pos.y = 30 - (sobj->sprite.height / 2);
	}
}

// 0x8018DF3C
void sc1PBonusStageMakePlatformSprites(void)
{
	GObj *interface_gobj;
	SObj *sobj;
	void *file;
	s32 i;

	file = lbRelocGetExternHeapFile(&llSC1PStageClear3FileID, syTaskmanMalloc(lbRelocGetFileSize(&llSC1PStageClear3FileID), 0x10));

	gGRCommonStruct.bonus2.interface_gobj = interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);
	gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);

	for (i = 0; i < gGRCommonStruct.bonus2.platform_count; i++)
	{
		sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, file, &llSC1PStageClear3PlatformSprite));
		sobj->sprite.attr = SP_TEXSHUF | SP_TRANSPARENT;

		sobj->pos.x = -(sobj->sprite.width / 2) + (((sobj->sprite.width + 3) * i) + 30);
		sobj->pos.y = 30 - (sobj->sprite.height / 2);
	}
}

// 0x8018E098
void sc1PBonusStageMakeTaskSprites(void)
{
	if (gSCManagerBattleState->gkind >= nGRKindBonus2Start)
	{
		sc1PBonusStageMakePlatformSprites();
	}
	else sc1PBonusStageMakeTargetSprites();
}

// 0x8018E0E0
void sc1PBonusStageGetPlayerStartPosition(Vec3f *pos)
{
	s32 mapobj;

	mpCollisionGetMapObjIDsKind(nMPMapObjKind1PGamePlayer, &mapobj);
	mpCollisionGetMapObjPositionID(mapobj, pos);
}

// 0x8018E114
void sc1PBonusStageTimerProcUpdate(GObj *interface_gobj)
{
	s32 digit;
	u32 itime;
	f32 ftime;
	SObj *sobj;
	s32 i;

	itime = gSCManagerBattleState->time_passed;
	sobj = SObjGetStruct(interface_gobj);

	if (itime > I_TIME_TO_TICS(0, 59, 59, 59))
	{
		itime = I_TIME_TO_TICS(0, 59, 59, 59);
	}
	ftime = itime;

	for (i = 0; i < ARRAY_COUNT(sSC1PBonusStageTimerDigits); i++)
	{
		digit = ftime / dSC1PBonusStageTimerUnitLengths[i];
		ftime -= digit * dSC1PBonusStageTimerUnitLengths[i];

		if (digit != sSC1PBonusStageTimerDigits[i])
		{
			sobj->sprite = *lbRelocGetFileData(Sprite*, gGMCommonFiles[3], dIFCommonTimerDigitSpriteOffsets[digit]);
			sobj->pos.x = dSC1PBonusStageTimerDigitPositions[i] - (sobj->sprite.width * 0.5F);
			sobj->pos.y = 30.0F - (sobj->sprite.height * 0.5F);

			sSC1PBonusStageTimerDigits[i] = digit;

			if (i == 0)
			{
				sobj->sprite.attr &= ~SP_HIDDEN;
			}
		}
		sobj = sobj->next;
	}
}

#if defined(REGION_US)
// 0x8018E298
void sc1PBonusStageSetTimeUp(void)
{
	sSC1PBonusStageIsTimeUp = TRUE;
}

// 0x8018E2A8
void sc1PBonusStageTimeUpProcUpdate(GObj *interface_gobj)
{
	if (sSC1PBonusStageIsTimeUp != FALSE)
	{
		ifCommonAnnounceFailureInitInterface();
		sSC1PBonusStageIsTimeUp = FALSE;
		gcEjectGObj(interface_gobj);
	}
}

// 0x8018E2E8
void sc1PBonusStageMakeTimeUp(void)
{
	sSC1PBonusStageIsTimeUp = FALSE;

	if (gSCManagerSceneData.scene_prev == nSCKind1PGame)
	{
		gcAddGObjProcess
		(
			gcMakeGObjSPAfter
			(
				nGCCommonKindInterface,
				NULL,
				nGCCommonLinkIDInterface,
				GOBJ_PRIORITY_DEFAULT
			),
			sc1PBonusStageTimeUpProcUpdate,
			nGCProcessKindFunc,
			0
		);
	}
}
#endif

// 0x8018E344
void sc1PBonusStageMakeTimer(void)
{
	GObj *interface_gobj;
	SObj *sobj;
	s32 i;

	if (gSCManagerSceneData.scene_prev != nSCKind1PGame)
	{
		ifCommonTimerMakeInterface(NULL);
		ifCommonTimerSetAttr();

		interface_gobj = gcMakeGObjSPAfter(nGCCommonKindInterface, NULL, nGCCommonLinkIDInterface, GOBJ_PRIORITY_DEFAULT);

		gcAddGObjDisplay(interface_gobj, lbCommonDrawSObjAttr, 23, GOBJ_PRIORITY_DEFAULT, ~0);

		for (i = 0; i < ARRAY_COUNT(sSC1PBonusStageTimerDigits); i++)
		{
			sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], &llIFCommonTimerDigit0Sprite));

			sobj->pos.x = dSC1PBonusStageTimerDigitPositions[i] - (sobj->sprite.width * 0.5F);
			sobj->pos.y = 30.0F - (sobj->sprite.height * 0.5F);

			sSC1PBonusStageTimerDigits[i] = 0;
		}
		sobj = SObjGetStruct(interface_gobj);

		sobj->sprite.attr |= SP_HIDDEN;

		sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], &llIFCommonTimerSymbolSecSprite));
		sobj->pos.x = (s32) (231.0F - (sobj->sprite.width * 0.5F));
		sobj->pos.y = (s32) (20.0F - (sobj->sprite.height * 0.5F));

		sobj = lbCommonMakeSObjForGObj(interface_gobj, lbRelocGetFileData(Sprite*, gGMCommonFiles[3], &llIFCommonTimerSymbolCSecSprite));
		sobj->pos.x = (s32) (264.0F - (sobj->sprite.width * 0.5F));
		sobj->pos.y = (s32) (20.0F - (sobj->sprite.height * 0.5F));

		gcAddGObjProcess(interface_gobj, sc1PBonusStageTimerProcUpdate, nGCProcessKindFunc, 5);
		return;
	}
	else
	{
#if defined(REGION_US)
		ifCommonTimerMakeInterface(sc1PBonusStageSetTimeUp);
#else
		ifCommonTimerMakeInterface(ifCommonAnnounceFailureInitInterface);
#endif
		ifCommonTimerMakeDigits();
	}
}

// 0x8018E5D8
void sc1PBonusStageSetPlayerInterfacePositions(void)
{
	gIFCommonPlayerInterface.player_pos_x = dSC1PBonusStagePlayerInterfacePositions;
	gIFCommonPlayerInterface.player_pos_y = 210;
}

// 0x8018E5F8
void sc1PBonusStageFuncStart(void)
{
	s32 unused[3];
	s32 player;
	GObj* fighter_gobj;
	FTDesc desc;
	SYColorRGBA color;

	sc1PBonusStageInitVars();
	sc1PBonusStageSetupFiles();
	sc1PBonusStageBonus1LoadFile();
	gcMakeDefaultCameraGObj(nGCCommonLinkIDCamera, GOBJ_PRIORITY_DEFAULT, 100, COBJ_FLAG_ZBUFFER, GPACK_RGBA8888(0x00, 0x00, 0x00, 0xFF));
	efParticleInitAll();
	ftParamInitGame();
	mpCollisionInitGroundData();
	gmCameraSetViewportDimensions(10, 10, 310, 230);
	gmCameraMakeWallpaperCamera();
	grWallpaperMakeDecideKind();
	gmCameraMakeBattleCamera();
	itManagerInitItems();
	grCommonSetupInitAll();
	ftManagerAllocFighter(FTDATA_FLAG_MAINMOTION, 4);
	wpManagerAllocWeapons();
	efManagerInitEffects();
	ifScreenFlashMakeInterface(0xFF);
	gmRumbleMakeActor();
	ftPublicMakeActor();

	for (player = 0, desc = dFTManagerDefaultFighterDesc; player < ARRAY_COUNT(gSCManagerBattleState->players); player++)
	{
		if (gSCManagerBattleState->players[player].pkind == nFTPlayerKindNot)
		{
			continue;
		}
		ftManagerSetupFilesAllKind(gSCManagerBattleState->players[player].fkind);

		desc.fkind = gSCManagerBattleState->players[player].fkind;

		sc1PBonusStageGetPlayerStartPosition(&desc.pos);

		desc.lr = (desc.pos.x >= 0.0F) ? -1 : +1;

		desc.team = 0;
		desc.player = player;
		desc.detail = nFTPartsDetailHigh;
		desc.costume = gSCManagerBattleState->players[player].costume;
		desc.pkind = gSCManagerBattleState->players[player].pkind;
		desc.controller = &gSYControllerDevices[player];
		desc.figatree_heap = ftManagerAllocFigatreeHeapKind(gSCManagerBattleState->players[player].fkind);
		desc.is_skip_entry = TRUE;

		fighter_gobj = ftManagerMakeFighter(&desc);

		ftParamInitPlayerBattleStats(player, fighter_gobj);

		break;
	}

	ftManagerSetupFilesPlayablesAll();
	ifCommonBattleSetGameStatusWait();
	gmCameraScreenFlashMakeCamera();
	gmCameraMakeInterfaceCamera();
	gmCameraMakeEffectCamera();
	ifCommonPlayerTagMakeInterface();
	sc1PBonusStageSetPlayerInterfacePositions();
	ifCommonPlayerDamageInitInterface();
	ifCommonPlayerStockInitInterface();
	sc1PBonusStageMakeTaskSprites();
	sc1PBonusStageMakeInterface();
	mpCollisionSetPlayBGM();
	func_800269C0_275C0(nSYAudioVoicePublicExcited);
	sc1PBonusStageMakeTimer();
	sc1PBonusStageInitCamera();

	color = dSC1PBonusStageFadeColor;

	lbFadeMakeActor(nGCCommonKindTransition, nGCCommonLinkIDTransition, 10, &color, 12, TRUE, NULL);
#if defined(REGION_US)
	sc1PBonusStageMakeTimeUp();
#endif
}

// 0x8018E8D0
void sc1PBonusStageSetBonusStats(s32 tasks_remain)
{
	gSC1PManagerTotalDamage += gSCManagerBattleState->players[gSCManagerSceneData.player].total_damage_all;

	if (tasks_remain != 0)
	{
		gSCManagerSceneData.spgame_time_remain = 0;
		gSCManagerSceneData.bonus_get_mask[0] = 0;
		gSCManagerSceneData.bonus_get_mask[1] = 0;
		gSCManagerSceneData.bonus_get_mask[2] = 0;
	}
	else
	{
		gSCManagerSceneData.spgame_time_remain = I_TICS_TO_SEC(gSCManagerBattleState->time_remain + 59);
		gSCManagerSceneData.bonus_get_mask[0] = SC1PGAME_BONUS_MASK0_PERFECT;
		gSCManagerSceneData.bonus_get_mask[1] = 0;
		gSCManagerSceneData.bonus_get_mask[2] = 0;
	}
}

// 0x8018E95C
void sc1PBonusStageWriteBackup(sb32 is_tasks_fail, s32 fkind)
{
	if (gSCManagerSceneData.is_reset == FALSE)
	{
		if (gSCManagerBattleState->gkind <= nGRKindBonus1End)
		{
			if (is_tasks_fail != FALSE)
			{
				if (gSCManagerBackupData.spgame_records[fkind].bonus1_task_count < gSCManagerSceneData.bonus_tasks_complete)
				{
					gSCManagerBackupData.spgame_records[fkind].bonus1_task_count = gSCManagerSceneData.bonus_tasks_complete;

					lbBackupWrite();
				}
			}
			else
			{
				gSCManagerBackupData.spgame_records[fkind].bonus1_task_count = SCBATTLE_BONUSGAME_TASK_MAX;

				if (gSCManagerBattleState->time_passed < gSCManagerBackupData.spgame_records[fkind].bonus1_time)
				{
					gSCManagerBackupData.spgame_records[fkind].bonus1_time = gSCManagerBattleState->time_passed;

					lbBackupWrite();
				}
			}
		}
		else if (is_tasks_fail != FALSE)
		{
			if (gSCManagerBackupData.spgame_records[fkind].bonus2_task_count < gSCManagerSceneData.bonus_tasks_complete)
			{
				gSCManagerBackupData.spgame_records[fkind].bonus2_task_count = gSCManagerSceneData.bonus_tasks_complete;

				lbBackupWrite();
			}
		}
		else
		{
			gSCManagerBackupData.spgame_records[fkind].bonus2_task_count = SCBATTLE_BONUSGAME_TASK_MAX;

			if (gSCManagerBattleState->time_passed < gSCManagerBackupData.spgame_records[fkind].bonus2_time)
			{
				gSCManagerBackupData.spgame_records[fkind].bonus2_time = gSCManagerBattleState->time_passed;

				lbBackupWrite();
			}
		}
	}
}

// 0x8018EA80
void sc1PBonusStageFuncLights(Gfx **dls)
{
	gSPSetGeometryMode(dls[0]++, G_LIGHTING);

	ftDisplayLightsDrawReflect(dls, gMPCollisionLightAngleX, gMPCollisionLightAngleY);
}

// 0x8018EACC
void sc1PBonusStageStartScene(void)
{
	u16 bonus_complete_fkinds;
	s32 tasks_remain;
	u32 tasks_complete;
	s32 i;

	dSC1PBonusStageVideoSetup.zbuffer = SYVIDEO_ZBUFFER_START(320, 240, 0, 10, u16);
	syVideoInit(&dSC1PBonusStageVideoSetup);

	dSC1PBonusStageTaskmanSetup.scene_setup.arena_size = (size_t) ((uintptr_t)&gSYFramebufferSets - (uintptr_t)&ovl6_BSS_END);
	dSC1PBonusStageTaskmanSetup.func_start = sc1PBonusStageFuncStart;

	syTaskmanStartTask(&dSC1PBonusStageTaskmanSetup);
	syAudioStopBGMAll();

	while (syAudioCheckBGMPlaying(0) != FALSE)
	{
		continue;
	}

	syAudioSetBGMVolume(0, 0x7800);
	func_800266A0_272A0();
	gmRumbleInitPlayers();

	if (gSCManagerBattleState->game_status != nSCBattleGameStatusPause)
	{
		tasks_remain = (gSCManagerBattleState->gkind <= nGRKindBonus1End) ? 
		gGRCommonStruct.bonus1.target_count :
		gGRCommonStruct.bonus2.platform_count;

		tasks_complete = SCBATTLE_BONUSGAME_TASK_MAX - tasks_remain;

		if (tasks_remain > 0); // Bruh

		gSCManagerSceneData.bonus_tasks_complete = tasks_complete;

		switch (gSCManagerSceneData.scene_prev)
		{
		case nSCKind1PGame:
			sc1PBonusStageSetBonusStats(tasks_remain);
			sc1PBonusStageWriteBackup(tasks_remain, gSCManagerSceneData.fkind);
			break;

		default:
			sc1PBonusStageWriteBackup(tasks_remain, gSCManagerSceneData.bonus_fkind);

			if (gSCManagerSceneData.scene_prev == nSCKind1PBonus1Players)
			{
				gSCManagerSceneData.scene_curr = nSCKind1PBonus1Players;

				if (tasks_complete == SCBATTLE_BONUSGAME_TASK_MAX)
				{
					if (!(gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_LUIGI))
					{
						for (bonus_complete_fkinds = i = 0; i < ARRAY_COUNT(gSCManagerBackupData.spgame_records); i++)
						{
							if (gSCManagerBackupData.spgame_records[i].bonus1_task_count == SCBATTLE_BONUSGAME_TASK_MAX)
							{
								bonus_complete_fkinds |= (1 << i);
							}
						}
						if ((bonus_complete_fkinds & LBBACKUP_CHARACTER_MASK_STARTER) == LBBACKUP_CHARACTER_MASK_STARTER)
						{
							gSCManagerSceneData.fkind = gSCManagerSceneData.bonus_fkind;
							gSCManagerSceneData.costume = gSCManagerSceneData.bonus_costume;

							gSCManagerSceneData.spgame_stage = nSC1PGameStageLuigi;
							gSCManagerSceneData.scene_curr = nSCKind1PGame;

							break;
						}
					}
					if (sc1PManagerCheckUnlockSoundTest() != FALSE)
					{
						gSCManagerSceneData.unlock_messages[0] = nLBBackupUnlockSoundTest;
						gSCManagerSceneData.scene_curr = nSCKindMessage;
					}
					break;
				}
			}
			else
			{
				gSCManagerSceneData.scene_curr = nSCKind1PBonus2Players;

				if ((tasks_complete == SCBATTLE_BONUSGAME_TASK_MAX) && (sc1PManagerCheckUnlockSoundTest() != FALSE))
				{
					gSCManagerSceneData.unlock_messages[0] = nLBBackupUnlockSoundTest;
					gSCManagerSceneData.scene_curr = nSCKindMessage;
				}
			}
			break;
		}
		gSCManagerSceneData.scene_prev = nSCKind1PBonusStage;
	}
}
