#ifndef _WPPROCESS_H_
#define _WPPROCESS_H_

#include "wptypes.h"

// Update weapon's scale, rotate and translate vectors
void wpProcessUpdateHitOffsets(DObj *dobj, Vec3f *vec);

// Update weapon's hitbox positions
void wpProcessUpdateHitPositions(GObj *weapon_gobj);

// Update weapon's record of interacted targets	(Decrement rehit timer if available, clear targets if timer reaches 0)
void wpProcessUpdateAttackRecords(GObj *weapon_gobj);

// Main weapon logic GObj process (animation, physics, collision, map_bound
// check)
void wpProcessProcWeaponMain(GObj *weapon_gobj);

// Set type of interaction and record hit target when weapon's hitbox collides
// with another GObj
void wpProcessSetHitInteractStats(WPAttackColl *attack_coll, GObj *victim_gobj, s32 attack_type, u32 interact_mask);

// Unknown
void wpProcessUpdateHitInteractStats(WPStruct *this_wp, WPAttackColl *attack_coll, GObj *target_gobj, s32 attack_type, u32 attack_group_id);

// Set weapon VS weapon hitbox detection results
void wpProcessUpdateAttackStatWeapon
(
	WPStruct *this_wp,
	WPAttackColl *this_attack_coll,
	s32 this_attack_id,
	WPStruct *victim_wp,
	WPAttackColl *victim_attack_coll,
	s32 victim_attack_id,
	GObj *this_gobj,
	GObj *victim_gobj
);

// GObj process for detecting other weapon hitboxes
void wpProcessProcSearchHitWeapon(GObj *weapon_gobj);

// GObj process for running various hitbox collision routines
void wpProcessProcHitCollisions(GObj *weapon_gobj);

#endif
