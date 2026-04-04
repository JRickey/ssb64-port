#include <it/item.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

// WARNING: Intentionally erroneous declaration. Missing two u16 arguments after f32. HAL's mistake, not mine.
extern void itMainSetFighterRelease(GObj*, Vec3f*, f32);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dItLinkBombItemDesc =
{
	nITKindLinkBomb, 						// Item Kind
	&gFTDataLinkMain, 						// Pointer to item file data?
	&llLinkMainBombItemAttributes,			// Offset of item attributes in file?

	// DObj transformation struct
	{
		nGCMatrixKindTra,  					// Main matrix transformations
		nGCMatrixKindNull, 					// Secondary matrix transformations?
		0					  				// ???
	},

	nGMAttackStateOff, 						// Hitbox Update State
	itLinkBombHoldProcUpdate,				// Proc Update
	NULL,									// Proc Map
	NULL,									// Proc Hit
	NULL,									// Proc Shield
	NULL,									// Proc Hop
	NULL,									// Proc Set-Off
	NULL,									// Proc Reflector
	NULL									// Proc Damage
};

ITStatusDesc dItLinkBombStatusDescs[/* */] = 
{
	// Status 0 (Ground Wait)
	{
		itLinkBombWaitProcUpdate,	 		// Proc Update
		itLinkBombWaitProcMap,		 		// Proc Map
		NULL,						 		// Proc Hit
		NULL,						 		// Proc Shield
		NULL,						 		// Proc Hop
		NULL,						 		// Proc Set-Off
		NULL,						 		// Proc Reflector
		itLinkBombCommonProcDamage 			// Proc Damage
	},

	// Status 1 (Air Wait Fall)
	{
		itLinkBombFallProcUpdate,	 		// Proc Update
		itLinkBombFallProcMap,		 		// Proc Map
		NULL,						 		// Proc Hit
		NULL,						 		// Proc Shield
		NULL,						 		// Proc Hop
		NULL,						 		// Proc Set-Off
		NULL,						 		// Proc Reflector
		itLinkBombCommonProcDamage 			// Proc Damage
	},

	// Status 2 (Fighter Hold)
	{
		itLinkBombHoldProcUpdate, 			// Proc Update
		NULL,					   			// Proc Map
		NULL,					   			// Proc Hit
		NULL,					   			// Proc Shield
		NULL,					   			// Proc Hop
		NULL,					   			// Proc Set-Off
		NULL,					   			// Proc Reflector
		NULL					   			// Proc Damage
	},

	// Status 3 (Fighter Throw)
	{
		itLinkBombFallProcUpdate,	   		// Proc Update
		itLinkBombThrownProcMap,	   		// Proc Map
		itLinkBombThrownProcHit,	   		// Proc Hit
		itLinkBombCommonProcShield,  		// Proc Shield
		itMainCommonProcHop,	   			// Proc Hop
		NULL,						   		// Proc Set-Off
		itMainCommonProcReflector, 			// Proc Reflector
		itLinkBombCommonProcDamage   		// Proc Damage
	},

	// Status 4 (Fighter Drop)
	{
		itLinkBombDroppedProcUpdate,	   	// Proc Update
		itLinkBombThrownProcMap,	   		// Proc Map
		itLinkBombDroppedProcHit,		   	// Proc Hit
		itLinkBombCommonProcShield,  		// Proc Shield
		itMainCommonProcHop,	  		 	// Proc Hop
		NULL,						   		// Proc Set-Off
		itMainCommonProcReflector, 			// Proc Reflector
		itLinkBombDroppedProcDamage	   		// Proc Damage
	},

	// Status 5 (Neutral Explosion)
	{
		itLinkBombExplodeProcUpdate,  		// Proc Update
		NULL,						  		// Proc Map
		NULL,						  		// Proc Hit
		NULL,						  		// Proc Shield
		NULL,						  		// Proc Hop
		NULL,						  		// Proc Set-Off
		NULL,						  		// Proc Reflector
		NULL						  		// Proc Damage
	}
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itLinkBombStatus
{
	nITLinkBombStatusWait,
	nITLinkBombStatusFall,
	nITLinkBombStatusHold,
	nITLinkBombStatusThrown,
	nITLinkBombStatusDropped,
	nITLinkBombStatusExplode,
	nITLinkBombStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801859C0
void itLinkBombExplodeWaitUpdateScale(GObj *item_gobj)
{
	DObj *dobj = DObjGetStruct(item_gobj);
	ITStruct *ip = itGetStruct(item_gobj);

	if (ip->item_vars.linkbomb.scale_int == 0)
	{
		f32 *scales = (f32*) ((uintptr_t)*dItLinkBombItemDesc.p_file + (intptr_t)&llLinkMainBombBloatScales);
		s32 scale_id = (ip->item_vars.linkbomb.scale_id > ITLINKBOMB_SCALE_INDEX_REWIND) ?
					      (ITLINKBOMB_SCALE_INDEX_MAX - ip->item_vars.linkbomb.scale_id) :
					   								    ip->item_vars.linkbomb.scale_id;

		if (ip->is_hold)
		{
			dobj->child->scale.vec.f.x = dobj->child->scale.vec.f.y = scales[scale_id];
		}
		else dobj->scale.vec.f.x = dobj->scale.vec.f.y = scales[scale_id];

		ip->item_vars.linkbomb.scale_int = ITLINKBOMB_SCALE_INT;

		if (ip->item_vars.linkbomb.scale_id >= ITLINKBOMB_SCALE_INDEX_MAX)
		{
			ip->item_vars.linkbomb.scale_id = 0;
		}
		else ip->item_vars.linkbomb.scale_id += 1; // Doesn't match with "++" lol
	}
	ip->item_vars.linkbomb.scale_int--;
}

// 0x80185A80
void itLinkBombExplodeMakeEffectGotoSetStatus(GObj *item_gobj)
{
	s32 unused;
	DObj *dobj = DObjGetStruct(item_gobj);
	ITStruct *ip = itGetStruct(item_gobj);
	LBParticle *pc;

	itLinkBombCommonSetHitStatusNone(item_gobj);

	pc = efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);

	if (pc != NULL)
	{
		pc->xf->scale.x = ITLINKBOMB_EXPLODE_EFFECT_SCALE;
		pc->xf->scale.y = ITLINKBOMB_EXPLODE_EFFECT_SCALE;
		pc->xf->scale.z = ITLINKBOMB_EXPLODE_EFFECT_SCALE;
	}
	efManagerQuakeMakeEffect(1);

	DObjGetStruct(item_gobj)->flags = DOBJ_FLAG_HIDDEN;

	ip->attack_coll.fgm_id = nSYAudioFGMExplodeL;

	itMainRefreshAttackColl(item_gobj);
	itLinkBombExplodeSetStatus(item_gobj);
}

// 0x80185B18 - Called only by unused function
void func_ovl3_80185B18(GObj *item_gobj)
{
	s32 unused[5];
	ITStruct *ip = itGetStruct(item_gobj);
	ITAttributes *attr = ip->attr;
	DObj *dobj = DObjGetStruct(item_gobj);
	Vec3f pos = dobj->translate.vec.f;

	pos.y += attr->map_coll_bottom;

	efManagerDustHeavyDoubleMakeEffect(&pos, ip->lr, 1.0F);
}

// 0x80185B84
sb32 itLinkBombCommonProcDamage(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	if (ip->damage_queue >= ITLINKBOMB_HEALTH)
		itLinkBombExplodeInitVars(item_gobj);
	else
	{
		ip->lr = -ip->damage_lr;

		ip->physics.vel_air.x = -ip->damage_lr  *ITLINKBOMB_DAMAGE_RECOIL_VEL_X;
		ip->physics.vel_air.y = -ip->damage_lr  *ITLINKBOMB_DAMAGE_RECOIL_VEL_Y;
	}
	return FALSE;
}

// 0x80185BFC
sb32 itLinkBombThrownProcHit(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	if 
	(
		(ABSF(ip->physics.vel_air.x) > ITLINKBOMB_EXPLODE_THRESHOLD_VEL_X) || 
		(ABSF(ip->physics.vel_air.y) > ITLINKBOMB_EXPLODE_THRESHOLD_VEL_Y)
	)
	{
		itLinkBombExplodeInitVars(item_gobj);
	}
	else
	{
		ip->lr = -ip->hit_lr;

		ip->physics.vel_air.x = -ip->hit_lr * ITLINKBOMB_HIT_RECOIL_VEL_X;
		ip->physics.vel_air.y = ITLINKBOMB_HIT_RECOIL_VEL_Y;

		itLinkBombFallSetStatus(item_gobj);
	}
	return FALSE;
}

// 0x80185CD4
void itLinkBombCommonSetHitStatusNormal(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	ip->damage_coll.hitstatus = nGMHitStatusNormal;
}

// 0x80185CE4
void itLinkBombCommonSetHitStatusNone(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	ip->damage_coll.hitstatus = nGMHitStatusNone;
}

// 0x80185CF0
sb32 itLinkBombFallProcUpdate(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	itMainApplyGravityClampTVel(ip, ITLINKBOMB_GRAVITY, ITLINKBOMB_TVEL);

	if (ip->lifetime == 0)
	{
		itLinkBombExplodeInitVars(item_gobj);
	}
	if (ip->lifetime == ITLINKBOMB_BLOAT_BEGIN)
	{
		itMainCheckSetColAnimID(item_gobj, nGMColAnimItemLinkBombCritical, ITLINKBOMB_BLOAT_COLANIM_LENGTH);

		ip->item_vars.linkbomb.scale_id = 1;
	}
	if (ip->lifetime < ITLINKBOMB_BLOAT_BEGIN)
	{
		itLinkBombExplodeWaitUpdateScale(item_gobj);
	}
	ip->lifetime--;

	itVisualsUpdateSpin(item_gobj);

	return FALSE;
}

// 0x80185DCC
sb32 itLinkBombWaitProcUpdate(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	if (ip->physics.vel_air.x != 0.0F)
	{
		ip->physics.vel_air.x += (-1.0F) * ip->lr;
	}
	if (ABSF(ip->physics.vel_air.x) < 1.0F)
	{
		ip->physics.vel_air.x = 0;
	}
	if (ip->lifetime == 0)
	{
		itLinkBombExplodeInitVars(item_gobj);
	}
	if (ip->lifetime == ITLINKBOMB_BLOAT_BEGIN)
	{
		itMainCheckSetColAnimID(item_gobj, nGMColAnimItemLinkBombCritical, ITLINKBOMB_BLOAT_COLANIM_LENGTH);

		ip->item_vars.linkbomb.scale_id = 1;
	}
	if (ip->lifetime < ITLINKBOMB_BLOAT_BEGIN)
	{
		itLinkBombExplodeWaitUpdateScale(item_gobj);
	}
	ip->lifetime--;

	return FALSE;
}

// 0x80185F10
sb32 itLinkBombWaitProcMap(GObj *item_gobj)
{
	itMapCheckLRWallProcNoFloor(item_gobj, itLinkBombFallSetStatus);

	return FALSE;
}

// 0x80185F38
sb32 itLinkBombFallProcMap(GObj *item_gobj)
{
	itMapCheckDestroyDropped(item_gobj, ITLINKBOMB_MAP_REBOUND_COMMON, ITLINKBOMB_MAP_REBOUND_GROUND, itLinkBombWaitSetStatus);

	return FALSE;
}

// 0x80185F70
void itLinkBombWaitSetStatus(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	ip->attack_coll.attack_state = nGMAttackStateOff;

	ip->is_allow_pickup = TRUE;

	ip->times_landed = 0;

	ip->physics.vel_air.x = ip->physics.vel_air.y = ip->physics.vel_air.z = 0.0F;

	itMapSetGround(ip);
	itLinkBombCommonSetHitStatusNormal(item_gobj);
	itMainSetStatus(item_gobj, dItLinkBombStatusDescs, nITLinkBombStatusWait);
}

// 0x80185FD8
void itLinkBombFallSetStatus(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	ip->is_allow_pickup = FALSE;

	itMapSetAir(ip);
	itLinkBombCommonSetHitStatusNormal(item_gobj);
	itMainSetStatus(item_gobj, dItLinkBombStatusDescs, nITLinkBombStatusFall);
}

// 0x80186024
sb32 itLinkBombHoldProcUpdate(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);
	FTStruct *fp = ftGetStruct(ip->owner_gobj);

	if (fp->status_id == nFTCommonStatusDokanWait) // Kind of odd but sure
	{
		return FALSE;
	}
	else
	{
		if (ip->lifetime == 0)
		{
			/* 
			 * OK, WHAT? This function takes 5 arguments, but it doesn't match
			 * otherwise??? Did they actually redefine this? Passes pointer in a3
			 * instead of u16... Do we leave this out of the header and declare it
			 * erroneously in this file to match? 
			 */
			
			// Update 3/23/2023: itMainSetFighterRelease matches as variadic. No comment.
			// Update  7/2/2023: variadic match confirmed fake, so does this file really use an erroneous decleration?

			itMainSetFighterRelease(item_gobj, &ip->physics.vel_air, 1.0F);
			itMainClearOwnerStats(item_gobj);
			itLinkBombExplodeInitVars(item_gobj);
		}
		if (ip->lifetime == ITLINKBOMB_BLOAT_BEGIN)
		{
			itMainCheckSetColAnimID(item_gobj, nGMColAnimItemLinkBombCritical, ITLINKBOMB_BLOAT_COLANIM_LENGTH);

			ip->item_vars.linkbomb.scale_id = 1;
		}
		if (ip->lifetime < ITLINKBOMB_BLOAT_BEGIN)
		{
			itLinkBombExplodeWaitUpdateScale(item_gobj);
		}
		ip->lifetime--;

		return FALSE;
	}
}

// 0x8018611C
void itLinkBombHoldSetStatus(GObj *item_gobj)
{
	itLinkBombCommonSetHitStatusNone(item_gobj);
	itMainSetStatus(item_gobj, dItLinkBombStatusDescs, nITLinkBombStatusHold);
}

// 0x80186150
sb32 itLinkBombThrownProcMap(GObj *item_gobj)
{
	s32 unused;
	ITStruct *ip = itGetStruct(item_gobj);
	Vec3f vel = ip->physics.vel_air;

	if (itMapCheckMapReboundProcAll(item_gobj, 0.4F, 0.3F, itLinkBombFallSetStatus) != FALSE)
	{
		if ((ABSF(vel.x) > ITLINKBOMB_EXPLODE_THRESHOLD_VEL_X) || (ABSF(vel.y) > ITLINKBOMB_EXPLODE_THRESHOLD_VEL_Y))
		{
			itLinkBombExplodeInitVars(item_gobj);
		}
	}
	return FALSE;
}

// 0x80186224
void itLinkBombThrownSetStatus(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	itLinkBombCommonSetHitStatusNormal(item_gobj);

	ip->is_damage_all = TRUE;

	itMainSetStatus(item_gobj, dItLinkBombStatusDescs, nITLinkBombStatusThrown);
}

// 0x80186270
sb32 itLinkBombDroppedProcUpdate(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	if (ip->item_vars.linkbomb.drop_update_wait != 0)
	{
		ip->item_vars.linkbomb.drop_update_wait--;
	}
	else itLinkBombFallProcUpdate(item_gobj);

	return FALSE;
}

// 0x801862AC
sb32 itLinkBombDroppedProcHit(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	if (ip->item_vars.linkbomb.drop_update_wait == 0)
	{
		itLinkBombThrownProcHit(item_gobj);
	}
	return FALSE;
}

// 0x801862E0
sb32 itLinkBombDroppedProcDamage(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	if (ip->item_vars.linkbomb.drop_update_wait == 0)
	{
		itLinkBombCommonProcDamage(item_gobj);
	}
	return FALSE;
}

// 0x80186314
void itLinkBombDroppedSetStatus(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	itLinkBombCommonSetHitStatusNormal(item_gobj);

	ip->item_vars.linkbomb.drop_update_wait = 10;

	ip->is_damage_all = TRUE;

	itMainSetStatus(item_gobj, dItLinkBombStatusDescs, nITLinkBombStatusDropped);
}

void itLinkBombExplodeInitVars(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	ip->physics.vel_air.x = ip->physics.vel_air.y = ip->physics.vel_air.z = 0.0F;

	itMainClearOwnerStats(item_gobj);
	itLinkBombExplodeMakeEffectGotoSetStatus(item_gobj);
	func_800269C0_275C0(nSYAudioFGMExplodeL);
}

// 0x801863AC
void itLinkBombExplodeUpdateAttackEvent(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);
	ITAttackEvent *ev = itGetAttackEvent(dItLinkBombItemDesc, &llLinkMainBombAttackEvents);

	if (ip->multi == ev[ip->event_id].timer)
	{
		ip->attack_coll.angle = ev[ip->event_id].angle;
		ip->attack_coll.damage = ev[ip->event_id].damage;
		ip->attack_coll.size = ev[ip->event_id].size;

		ip->attack_coll.can_rehit_item = TRUE;
		ip->attack_coll.can_hop = FALSE;
		ip->attack_coll.can_reflect = FALSE;
		ip->attack_coll.can_setoff = FALSE;

		ip->attack_coll.element = nGMHitElementFire;

		ip->event_id++;

		if (ip->event_id == 4)
			ip->event_id = 3;
	}
}

// 0x80186498
sb32 itLinkBombCommonProcShield(GObj *item_gobj)
{
	itMainVelSetRebound(item_gobj);

	return FALSE;
}

// 0x801864BC
sb32 func_ovl3_801864BC(GObj *item_gobj) // Unused
{
	func_ovl3_80185B18(item_gobj);
	itLinkBombExplodeInitVars(item_gobj);

	return FALSE;
}

// 0x801864E8
void itLinkBombExplodeInitAttackColl(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	ip->multi = 0;
	ip->event_id = 0;

	ip->attack_coll.throw_mul = 1.0F;

	itLinkBombExplodeUpdateAttackEvent(item_gobj);
}

// 0x80186524
sb32 itLinkBombExplodeProcUpdate(GObj *item_gobj)
{
	ITStruct *ip = itGetStruct(item_gobj);

	itLinkBombExplodeUpdateAttackEvent(item_gobj);

	ip->multi++;

	if (ip->multi == ITLINKBOMB_EXPLODE_LIFETIME)
	{
		return TRUE;
	}
	else return FALSE;
}

// 0x8018656C
void itLinkBombExplodeSetStatus(GObj *item_gobj)
{
	itLinkBombExplodeInitAttackColl(item_gobj);
	itMainSetStatus(item_gobj, dItLinkBombStatusDescs, nITLinkBombStatusExplode);
}

// 0x801865A0
GObj *itLinkBombMakeItem(GObj *fighter_gobj, Vec3f *pos, Vec3f *vel)
{
	GObj *item_gobj = itManagerMakeItem(fighter_gobj, &dItLinkBombItemDesc, pos, vel, ITEM_FLAG_PARENT_FIGHTER);
	DObj *dobj;
	ITStruct *ip;

	if (item_gobj != NULL)
	{
		ip = itGetStruct(item_gobj);
		dobj = DObjGetStruct(item_gobj);

		gcAddXObjForDObjFixed(dobj, 0x2E, 0);
		gcAddXObjForDObjFixed(dobj->child, 0x2E, 0);

		ip->multi = 0;

		ip->lifetime = ITLINKBOMB_LIFETIME;

		ip->item_vars.linkbomb.scale_id = 0;
		ip->item_vars.linkbomb.scale_int = ITLINKBOMB_SCALE_INT;

		ip->attack_coll.can_rehit_shield = TRUE;

		ip->physics.vel_air.x = ip->physics.vel_air.y = ip->physics.vel_air.z = 0.0F;

		itMainSetFighterHold(item_gobj, fighter_gobj);
	}
	return item_gobj;
}
