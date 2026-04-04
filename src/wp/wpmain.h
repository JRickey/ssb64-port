#ifndef _WPMAIN_H_
#define _WPMAIN_H_

#include "wptypes.h"
#include <ft/fttypes.h>

// Stop weapon's ongoing sound effect
void wpMainStopFGM(WPStruct* wp);

// Play sound effect if applicable
void wpMainPlayFGM(WPStruct* wp, u16 sfx_id);

// Set weapon's facing direction based on velocity
void wpMainVelSetLR(GObj* weapon_gobj);

// Set weapon's Yaw rotation based on velocity
void wpMainVelSetModelPitch(GObj* weapon_gobj);

// Decrement weapon's lifetime and return bool for whether weapon has expired
sb32 wpMainDecLifeCheckExpire(WPStruct* wp);

// Destroy weapon
void wpMainDestroyWeapon(GObj* weapon_gobj);

// Transfer weapon's ground velocity to aerial velocity
void wpMainVelGroundTransferAir(GObj* weapon_gobj);

// Update weapon's gravity and clamp to terminal velocity if greater than
void wpMainApplyGravityClampTVel(WPStruct* wp, f32 gravity, f32 terminal_velocity);

// Set weapon's facing direction when reflected
void wpMainReflectorSetLR(WPStruct* wp, FTStruct* fp);

// Apply stale multiplier to final hitbox damage output (+ 0.999% bonus)
s32 wpMainGetStaledDamage(WPStruct* wp);

// Reset weapon's record of previously hit targets
void wpMainClearAttackRecord(WPStruct* wp);

// Unknown
void wpMainReflectorRotateWeaponModel(GObj* weapon_gobj);

#endif
