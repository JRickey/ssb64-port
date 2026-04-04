#ifndef _ITMAP_H_
#define _ITMAP_H_

#include "ittypes.h"

// Process for Left/Right Wall collision; check and return whether item collided with ground
sb32 itMapProcLRWallCheckFloor(MPCollData* coll_data, GObj* item_gobj, u32 flags);

// Test for Left/Right Wall collision; return whether item collided with ground
sb32 itMapTestLRWallCheckFloor(GObj* item_gobj);

// Test for Left/Right Wall collision; if item does not collide with ground, run proc_map and return FALSE; if collision
// with ground occurs, return TRUE; put simply, it checks if the item remains in a grounded state
sb32 itMapCheckLRWallProcNoFloor(GObj* item_gobj, void (*proc_map)(GObj*));

// Process for all collisions; check and return whether all collision tests have ended for current tick (?)
sb32 itMapProcAllCheckCollEnd(MPCollData* coll_data, GObj* item_gobj, u32 flags);

// Test for all wall collisions; check and return whether all collision tests have ended for current tick (?)
sb32 itMapTestAllCheckCollEnd(GObj* item_gobj);

// Process for all collisions; check and return whether collision with specified type(s) is successful
sb32 itMapProcAllCheckCollisionFlag(MPCollData* coll_data, GObj* item_gobj, u32 coll_flags);

// Test for all collisions; check and return whether collision with specified type(s) is successful
sb32 itMapTestAllCollisionFlag(GObj* item_gobj, u32 flag);

// Check for specific item map collision and modify velocity on contact
sb32 itMapCheckCollideAllRebound(GObj* item_gobj, u32 check_flags, f32 mod_vel, Vec3f* pos);

// Set recoil velocity when item hits ground
void itMapSetGroundRebound(Vec3f* vel, Vec3f* floor_angle, f32 ground_rebound);

// Check if item should be destroyed upon landing after being dropped / thrown
sb32 itMapCheckDestroyDropped(GObj* item_gobj, f32 common_rebound, f32 ground_rebound,
									   void (*proc_status)(GObj*));

// Check if item is landing normally
sb32 itMapCheckLanding(GObj* item_gobj, f32 common_rebound, f32 ground_rebound, void (*proc_map)(GObj*));

// Check if item recoils from collision with any type of surface; run proc_map if true
sb32 itMapCheckMapReboundProcAll(GObj* item_gobj, f32 common_rebound, f32 ground_rebound, void (*proc_map)(GObj*));

// Check if item recoils from collision with ground
sb32 itMapCheckDestroyLanding(GObj* item_gobj, f32 common_rebound);

// Test for all collisions; run proc_map if true; always returns FALSE
sb32 itMapCheckMapProcAll(GObj* item_gobj, void (*proc_map)(GObj*));

// Unknown; unused?
sb32 func_ovl3_80173E9C(GObj* item_gobj, void (*proc_map)(GObj*));

// Test for all collisions; if item recoils from collision with ground, run proc_map; otherwise check all other surface
// types; always returns FALSE
sb32 itMapCheckMapReboundProcNoFloor(GObj* item_gobj, f32 common_rebound, void (*proc)(GObj*));

// Make item grounded
void itMapSetGround(ITStruct* ip);

// Make item airborne
void itMapSetAir(ITStruct* ip);

#endif
