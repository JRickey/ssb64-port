#ifndef _ITMAIN_H_
#define _ITMAIN_H_

#include "ittypes.h"

// Something to do with item spin rotation
void itMainSetCommonSpin(GObj *item_gobj);

// Something to do with item spin rotation
void itMainSetAppearSpin(GObj *item_gobj, sb32 slow_or_fast);

// Something to do with item spin rotation
void itMainSetThrownSpin(GObj *item_gobj, Vec3f *vel, sb32 is_smash_throw);

// Set item's facing direction and spin rotation based on velocity
void itMainSetSpinVelLR(GObj *item_gobj);

// Update item's gravity and clamp to terminal velocity if greater than
void itMainApplyGravityClampTVel(ITStruct *ip, f32 gravity, f32 terminal_velocity);

// Reset item's player-related variables
void itMainResetPlayerVars(GObj *item_gobj);

// Clear item's record of previously hit targets
void itMainClearAttackRecord(ITStruct *ip);

// Renew item's hitbox
void itMainRefreshAttackColl(GObj *item_gobj);

// Clear item's ownership status
void itMainClearOwnerStats(GObj *item_gobj);

// Get player-related variables from damaging entity
void itMainCopyDamageStats(GObj *item_gobj);

// Apply modifiers to final damage output (+ 0.999% bonus)
s32 itMainGetDamageOutput(ITStruct *ip);

// Check if shooting type item has run out of ammo
sb32 itMainCheckShootNoAmmo(GObj *item_gobj);

// Destroy item
void itMainDestroyItem(GObj *item_gobj);

// Remove item from player - this is more than likely not included as Link's bomb erroneously calls this without
// initializing stat_flags and stat_count
// void itMainSetFighterRelease(GObj *item_gobj, Vec3f *vel, f32 throw_mul, u16 stat_flags, u16 stat_count);

// Run item's drop routine
void itMainSetFighterDrop(GObj *item_gobj, Vec3f *vel, f32 throw_mul);

// Run item's throw routine
void itMainSetFighterThrow(GObj *item_gobj, Vec3f *vel, f32 throw_mul, sb32 is_smash_throw);

// Run item's pickup routine
void itMainSetFighterHold(GObj *item_gobj, GObj *fighter_gobj);

// Make item grounded, clear player variables and make it possible to pick up
void itMainSetGroundAllowPickup(GObj *item_gobj);

// Change item's action state
void itMainSetStatus(GObj *item_gobj, ITStatusDesc *status_desc, s32 status_id);

// Apply ColAnim to item and return whether it was successfully applied
sb32 itMainCheckSetColAnimID(GObj *item_gobj, s32 colanim_id, s32 duration);

// Reset item's ColAnim
void itMainClearColAnim(GObj *item_gobj);

// Runs when caller collides with a hurtbox
void itMainVelSetRebound(GObj *item_gobj);

// Binary search function to get item ID for container drop ?
s32 itMainSearchRandomWeight(s32 random, ITRandomWeights *weights, u32 min, u32 max);

// Get item ID for container
s32 itMainGetWeightedItemKind(ITRandomWeights *weights);

// Spawn new item from container?
sb32 itMainMakeContainerItem(GObj *gobj);

// Some basic item subaction event? Updates hitbox damage, size and angle; multi acts as asynchronous timer
void itMainUpdateAttackEvent(GObj *item_gobj, ITAttackEvent *ev);

// Create Pokémon
GObj *itMainMakeMonster(GObj *item_gobj);

// Default process run when item bounces off a shield (Common = status default)
sb32 itMainCommonProcHop(GObj *item_gobj);

// Default process run when item is reflected (Common = status default)
sb32 itMainCommonProcReflector(GObj *item_gobj);

#endif
