#ifndef _FTPHYSICS_H_
#define _FTPHYSICS_H_

#include "ftdef.h"
#include <ssb_types.h>
#include <sys/objdef.h>

// Transfer all ground velocity into aerial velocity
void ftPhysicsSetGroundVelTransferAir(GObj* fighter_gobj);

// Clamp ground velocity
void ftPhysicsClampGroundVel(FTStruct* fp, f32 clamp);

// Apply friction to ground velocity
void ftPhysicsSetGroundVelFriction(FTStruct* fp, f32 friction);

// Apply and clamp ground velocity if stick range is greater than stick_x_min
void ftPhysicsApplyClampGroundVelStickRange(FTStruct* fp, s32 stick_x_min, f32 vel, f32 clamp);

// Set ground velocity to vel * absolute stick range; apply friction
void ftPhysicsSetGroundVelAbsStickRange(FTStruct* fp, f32 vel, f32 friction);

// Set ground velocity to vel * stick range; apply friction
void ftPhysicsSetGroundVelStickRange(FTStruct* fp, f32 vel, f32 friction);

// Run ftPhysicsSetGroundVelTransferAir; don't know why this is a separate
// function
void ftPhysicsApplyGroundVelTransferAir(GObj* fighter_gobj);

// Apply friction to ground velocity the transfer to air velocity?
void ftPhysicsApplyGroundVelFriction(GObj* fighter_gobj);

// Set velocity to update from TrasnN joint's translation tracks
void ftPhysicsApplyGroundVelTransN(GObj* fighter_gobj);

// Run ftPhysicsApplyGroundVelTransN if applicable, else run
// ftPhysicsApplyGroundVelFriction
void ftPhysicsApplyGroundFrictionOrTransN(GObj* fighter_gobj);

// Clamp maximum positive aerial Y velocity
void ftPhysicsClampAirVelY(FTStruct* fp, f32 clamp);

// Add to aerial Y velocity and clamp
void ftPhysicsAddClampAirVelY(FTStruct* fp, f32 vel, f32 clamp);

// Apply gravity and clamp to terminal velocity
void ftPhysicsApplyGravityClampTVel(FTStruct* fp, f32 gravity, f32 t_vel);

// Set aerial Y velocity to fast fall speed
void ftPhysicsApplyFastFall(FTStruct* fp, FTAttributes *attr);

// Check for fast fall input from player
void ftPhysicsCheckSetFastFall(FTStruct* fp);

// Runs ftPhysicsApplyGravityClampTVel
void ftPhysicsApplyGravityDefault(FTStruct* fp, FTAttributes *attr);

// Clamp aerial X velocity
void ftPhysicsClampAirVelX(FTStruct* fp, f32 clamp);

// Clamp maximum aerial X velocity to fighter's air_speed_max_x
void ftPhysicsClampAirVelXMax(FTStruct* fp);

// If horizontal air speed is greater than clamp value, decrement velocity
// by 1.0 at a time; return bool for whether decrement occurred
sb32 ftPhysicsCheckClampAirVelXDec(FTStruct* fp, f32 clamp);

// Run ftPhysicsCheckClampAirVelXDec with character's maximum horizontal air
// speed as clamp value
sb32 ftPhysicsCheckClampAirVelXDecMax(FTStruct* fp, FTAttributes *attr);

// Apply and clamp aerial X velocity if stick range is greater than stick_x_min
void ftPhysicsClampAirVelXStickRange(FTStruct* fp, s32 stick_range_min, f32 vel, f32 clamp);

// Apply and clamp aerial X velocity if stick range is greater than 8 units
void ftPhysicsClampAirVelXStickDefault(FTStruct* fp, FTAttributes *attr);

// Apply friction to aerial X velocity
void ftPhysicsApplyAirVelXFriction(FTStruct* fp, FTAttributes *attr);

// Standard air drift physics
void ftPhysicsApplyAirVelDrift(GObj* fighter_gobj);

// Air drift physics, fast fall allowed
void ftPhysicsApplyAirVelDriftFastFall(GObj* fighter_gobj);

// Air physics, no drift allowed
void ftPhysicsApplyAirVelFriction(GObj* fighter_gobj);

// Get velocity from TransN joint's translation tracks into float pointers Z, Y
// and X; X and Z are swapped for some reason
void ftPhysicsGetAirVelTransN(FTStruct* fp, f32* z, f32* y, f32* x);

// Set velocity to update from TransN joint's translation tracks
void ftPhysicsSetAirVelTransN(GObj* fighter_gobj);

// Apply TransN velocity to all three velocity axes
void ftPhysicsApplyAirVelTransNAll(GObj* fighter_gobj);

// Apply TransN velocity to Y and Z axes
void ftPhysicsApplyAirVelTransNYZ(GObj* fighter_gobj);

// Set all velocity to 0.0
void ftPhysicsStopVelAll(GObj* fighter_gobj);

#endif
