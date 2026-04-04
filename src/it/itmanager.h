#ifndef _ITMANAGER_H_
#define _ITMANAGER_H_

#include "ittypes.h"
#include <wp/wptypes.h>
#include <ft/fttypes.h>

extern intptr_t lITManagerParticleScriptBankLo;  // 0x00B1BCA0
extern intptr_t lITManagerParticleScriptBankHi;  // 0x00B1BDE0
extern intptr_t lITManagerParticleTextureBankLo; // 0x00B1BDE0
extern intptr_t lITManagerParticleTextureBankHi; // 0x00B1E640

// Allocate memory for item structs and initialize global item-related data
void itManagerInitItems();

// Get memory region for item struct and advance global region pointer
ITStruct* itManagerGetNextStructAlloc();

// Roll back local and global item struct pointer by one unit
void itManagerSetPrevStructAlloc(ITStruct* ip);

// Unknown, something to do with setting up item models?
void itManagerSetupItemDObjs(GObj* gobj, DObjDesc* dobjdesc, DObj** dobjs, u8 transform_kind);

// Create new item
GObj* itManagerMakeItem(GObj* parent_gobj, ITDesc* item_desc, Vec3f* pos, Vec3f* vel, u32 flags);

// Create item and init spawn GFX + spin rotation if common item (indexes 0 - 19)
GObj* itManagerMakeItemSetupCommon(GObj* parent_gobj, s32 index, Vec3f* pos, Vec3f* vel, u32 spawn_flags);

// Get current item user_data to see if != NULL
ITStruct* itManagerGetCurrentAlloc();

// Set wait timer for next item spawn
void itManagerSetItemSpawnWait();

// GObj process for hidden object that randomly spawns items - argument is unused
void itManagerAppearActorProcUpdate(GObj* item_gobj);

// Set up item spawn positions?
GObj* itManagerMakeAppearActor();

// Set up item spawn positions part 2?
void itManagerSetupContainerDrops();

// Set up global Pokémon spawn variables
void itManagerInitMonsterVars();

// Make immediate item of ID
GObj* itManagerMakeItemKind(GObj* parent_gobj, s32 kind, Vec3f* pos, Vec3f* vel, u32 flags);

#endif
