#ifndef _ITTYPES_H_
#define _ITTYPES_H_

#include <ssb_types.h>
#include <macros.h>
#include <sys/obj.h>
#include <ef/effect.h>
#include <mp/map.h>
#include <gm/gmsound.h>
#include <gm/generic.h>

#include <it/itdef.h>
#include <it/itvars.h>
#ifdef PORT
#include <stddef.h>
#endif

// Structs
struct ITMonsterData
{
	u8 monster_curr;
	u8 monster_prev;
	u8 monster_id[44];
	u8 monsters_num;
};

struct ITDesc
{
	s32 kind;
	void **p_file;
	intptr_t o_attributes;
	DObjTransformTypes transform_types;
	s32 attack_state;
	sb32 (*proc_update)(GObj*);
	sb32 (*proc_map)(GObj*);
	sb32 (*proc_hit)(GObj*);
	sb32 (*proc_shield)(GObj*);
	sb32 (*proc_hop)(GObj*);
	sb32 (*proc_setoff)(GObj*);
	sb32 (*proc_reflector)(GObj*);
	sb32 (*proc_damage)(GObj*);
};

struct ITStatusDesc
{
	sb32 (*proc_update)(GObj*);
	sb32 (*proc_map)(GObj*);
	sb32 (*proc_hit)(GObj*);
	sb32 (*proc_shield)(GObj*);
	sb32 (*proc_hop)(GObj*);
	sb32 (*proc_setoff)(GObj*);
	sb32 (*proc_reflector)(GObj*);
	sb32 (*proc_damage)(GObj*);
};

struct ITRandomWeights          // Random item drops struct
{
    u8 unused[0x8];				// ???
    u8 valids_num;             	// Maximum number of items that can be spawned (item is toggled ON && randomizer weight is non-zero)
    u8 *kinds;             		// Array of item IDs that can be spawned
    u16 weights_sum;     		// Sum of all toggled items' randomizer weights
    u16 *blocks;          		// Item block sizes
};

struct ITAppearActor
{
    u8 mapobjs_num;       		// Maximum number of item spawn points
    u8 *mapobjs;           		// Pointer to array of item map object IDs
    u32 spawn_wait;        		// Spawn a random new item when this reaches 0
    ITRandomWeights weights;	// Randomizer struct
};

struct ITAttackPos
{
	Vec3f pos_curr;
	Vec3f pos_prev;
	sb32 unk_ithitpos_0x18;
	Mtx44f mtx;
	f32 unk_ithitpos_0x5C;
};

struct ITAttackColl
{
	s32 attack_state;									// Hitbox's position update mode (0 = disabled, 1 = fresh, 2 = transfer, 3 = interpolate)
	s32 damage;											// Hitbox's base damage output
	f32 throw_mul;										// Might be swapped with stale
	f32 stale;											// Might be swapped with throw_mul
	s32 element;										// Hitbox's element
	Vec3f offsets[ITEM_ATKCOLL_NUM_MAX];				// Hitbox offset from TopN translation vector
	f32 size;											// Hitbox size
	s32 angle;											// Launch angle
	u32 knockback_scale;								// Knockback growth
	u32 knockback_weight;								// Weight-Dependent Set Knockback
	u32 knockback_base;									// Base knockback
	s32 shield_damage;									// Shield damage
	s32 priority;										// Priority?
	u8 interact_mask;									// Mask of object classes hitbox can interact with; 0x1 = fighters, 0x2 = weapons, 0x4 = items
	u16 fgm_id;											// Played when hitbox connects with a hurtbox
	ub32 can_setoff : 1;								// Item's hitbox can collide with other hitboxes
	ub32 can_rehit_item : 1;							// Item can rehit item after default rehit cooldown expires
	ub32 can_rehit_fighter : 1;							// Item can rehit fighter after default rehit cooldown expires
	ub32 can_rehit_shield : 1;							// Item can rehit shield after default rehit cooldown expires
	ub32 can_hop : 1;									// Item can bounce off shields
	ub32 can_reflect : 1;								// Item can be reflected
	ub32 can_shield : 1;								// Item can be shielded
	u32 motion_attack_id : 6;							// Attack ID copied from object that spawned this item
	u16 motion_count;									// Item's animation update number?
	GMStatFlags stat_flags;								// Item's status flags
	u16 stat_count;										// Item's status update number
	s32 attack_count;									// Item's hitbox count, up to two
	ITAttackPos attack_pos[ITEM_ATKCOLL_NUM_MAX];		// Item hitbox positions
	GMAttackRecord attack_records[GMATTACKREC_NUM_MAX];	// Item's record of attacked targets
};

struct ITAttackEvent 	// Miniature hitbox subaction event? Commonly Used by explosions.
{
#if IS_BIG_ENDIAN
	u8 timer;
	s32 angle : 10;
	u32 damage : 8;
	u32 : 14;
	u16 size;
#else
	// IDO packs `u8 timer` into the top byte of the same u32 storage
	// that holds the angle/damage/pad bitfields, giving sizeof=8 with
	// size at offset 0x06. Replicate that physical layout on LE by
	// promoting timer to a u32:8 bitfield and filling the remaining
	// 2 bytes of BE ":14 pad" (plus alignment) with an explicit u16 pad.
	// Callers use scalar reads (`ev->timer`), so promotion is safe.
	// angle is read-as-unsigned; use BITFIELD_SEXT10() at read sites.
	//
	// IMPORTANT: do NOT call portFixupStructU16(&ev[i], 0x04, 1) on this
	// struct. ROM data has size at struct offset 0x04 (high half of the BE
	// u32 word at 0x04..0x07); after pass1 BSWAP32 those bytes physically
	// land at offset 0x06 in LE memory, exactly where this struct's `size`
	// field lives — so the read is already correct. Rotating the half-words
	// at 0x04 actively breaks size by moving it back to offset 0x04 (where
	// the struct expects pad). Bob-omb / capsule / Mr. Saturn / box etc.
	// explosions all read size=0 if you do (see git 87c4fe8 leftover).
	u32 : 6;
	u32 damage : 8;
	u32 angle : 10;
	u32 timer : 8;
	u16 _pad_0x04;
	u16 size;
#endif
};

struct ITMonsterEvent	// Full-scale hitbox subaction event? Used by Venusaur and Porygon.
{
#if IS_BIG_ENDIAN
	u8 timer;
	s32 angle : 10;
	u32 damage : 8;
	u32 : 14;
	u16 size;
#else
	// IDO packs `u8 timer` into the top byte of the same u32 storage
	// as the bitfield. sizeof is 36 (not 40). See ITAttackEvent above
	// for the same pattern (and the same "do not portFixupStructU16
	// on offset 0x04" warning — pass1 BSWAP32 leaves size correctly
	// readable at struct offset 0x06).
	// angle is read-as-unsigned; use BITFIELD_SEXT10() at read sites.
	// Note: fgm_id at offset 0x20 IS at the low half of its u32 word, so
	// it does need portFixupStructU16(&ev[i], 0x20, 1) at the read site
	// (kept in itporygon.c / itfushigibana.c).
	u32 : 6;
	u32 damage : 8;
	u32 angle : 10;
	u32 timer : 8;
	u16 _pad_0x04;
	u16 size;
#endif
	u32 knockback_scale;
	u32 knockback_weight;
	u32 knockback_base;
	s32 element;
#if IS_BIG_ENDIAN
	ub32 can_setoff : 1;
	u32 : 31;
#else
	u32 : 31;
	u32 can_setoff : 1;
#endif
	s32 shield_damage;
	u16 fgm_id;
};

struct ITDamageColl						// DamageColl struct
{
	u8 interact_mask; 					// 0x1 = interact with fighters, 0x2 = interact with weapons, 0x4 = interact with other items
	s32 hitstatus;	  					// 0 = none, 1 = normal, 2 = invincible, 3 = intangible
	Vec3f offset;	  					// Offset added to TopN joint's translation vector
	Vec3f size;		  					// DamageColl size
};

/*
 * ITAttributes layout is IDO-packed on BE and 72 bytes (0x48) total — NOT the
 * 0x50 the upstream decomp declaration would imply under a modern compiler.
 * Ground truth: disassembly of itManagerMakeItem at VRAM 0x8016E174 in
 * baserom.us.z64, cross-checked against reloc file 251 item data.
 *
 * ROM offsets and packing rules IDO produced:
 *   0x00  4 × u32 reloc pointer tokens
 *   0x10  u32 bitfield:
 *           [31] is_display_xlu    [30] is_item_dobjs
 *           [29] is_display_colanim [28] is_give_hitlag
 *           [27] weight             [26..11] attack_offset0_x (signed 16)
 *           [10..0] pad
 *   0x14  attack_offset0_y (s16) + attack_offset0_z (s16)
 *   0x18  attack_offset1_x (s16) + attack_offset1_y (s16)
 *   0x1C  attack_offset1_z (s16) + damage_coll_offset.x (s16)
 *   0x20  damage_coll_offset.y (s16) + damage_coll_offset.z (s16)
 *   0x24  damage_coll_size.x (s16) + damage_coll_size.y (s16)
 *   0x28  damage_coll_size.z (s16) + map_coll_top (s16)
 *   0x2C  map_coll_center (s16) + map_coll_bottom (s16)
 *   0x30  map_coll_width (s16) + size (u16)
 *   0x34  u32 bitfield: angle:10 | kb_scale:10 | damage:8 | element:4
 *   0x38  u32 bitfield: kb_weight:10 | shield_damage:8 | attack_count:2 |
 *                       can_setoff:1 | hit_sfx:10 | :1 pad
 *   0x3C  u32 bitfield: priority:3 | can_rehit_item:1 | can_rehit_fighter:1 |
 *                       can_hop:1 | can_reflect:1 | can_shield:1 |
 *                       kb_base:10 | type:4 | hitstatus:4 |
 *                       unk_b6:1 | unk_b7:1 | :2 pad
 *   0x40  u32 bitfield: drop_sfx:10 | throw_sfx:10 | smash_sfx:10 | :2 pad
 *                       (identical across every item in file 251 — "default"
 *                       sfx set; per-item overrides live in item C code)
 *   0x44  u32 bitfield: vel_scale:9 | :7 pad | spin_speed:16
 *                       (IDO packs the plain `u16 spin_speed` from the
 *                       upstream decomp into the low 16 bits of the same u32
 *                       that holds vel_scale's 9-bit bitfield — the canonical
 *                       IDO pad-gap-packing rule)
 *
 * The upstream decomp declaration is missing the padding discipline required
 * to make a modern LE compiler produce the same layout, so we diverge here.
 * BE branch still matches upstream; LE branch places every bitfield at the
 * numerically identical bit position IDO produced. After pass1 BSWAP32, LE
 * reads of u32 values equal the ROM's original numeric u32, so bitfield
 * extraction lands on the right bits.
 *
 * For the half-word plain fields at 0x14..0x33, each u32 word holds two u16
 * halves whose bytes get swapped by pass1 BSWAP32. itmanager.c calls
 * portFixupStructU16(attr, 0x14, 8) to rotate those eight u32 words, putting
 * both halves back at the offsets the struct declares.
 */
struct ITAttributes
{
#ifdef PORT
	u32 data;                           // Relocation token — use PORT_RESOLVE()
	u32 p_mobjsubs;                     // Relocation token
	u32 anim_joints;                    // Relocation token
	u32 p_matanim_joints;               // Relocation token
#else
	void *data;
	MObjSub ***p_mobjsubs;
	AObjEvent32 **anim_joints;
	AObjEvent32 ***p_matanim_joints;
#endif

	// 0x10 u32: flags + attack_offset0_x
#if IS_BIG_ENDIAN
	ub32 is_display_xlu : 1;
	ub32 is_item_dobjs : 1;
	ub32 is_display_colanim : 1;
	ub32 is_give_hitlag : 1;
	ub32 weight : 1;
	s32 attack_offset0_x : 16;			// use BITFIELD_SEXT16() at read sites on LE
	u32 : 11;
#else
	u32 : 11;
	u32 attack_offset0_x : 16;
	u32 weight : 1;
	u32 is_give_hitlag : 1;
	u32 is_display_colanim : 1;
	u32 is_item_dobjs : 1;
	u32 is_display_xlu : 1;
#endif

	// 0x14..0x33 plain half-words. After pass1 BSWAP32 every u32 word here has
	// its two u16 halves swapped; itmanager.c:portFixupStructU16(attr, 0x14, 8)
	// undoes that.
	s16 attack_offset0_y;				// 0x14
	s16 attack_offset0_z;				// 0x16
	s16 attack_offset1_x;				// 0x18
	s16 attack_offset1_y;				// 0x1A
	s16 attack_offset1_z;				// 0x1C
	Vec3h damage_coll_offset;			// 0x1E..0x23 (Hurtbox offset)
	Vec3h damage_coll_size;				// 0x24..0x29 (Hurtbox size)
	s16 map_coll_top;					// 0x2A
	s16 map_coll_center;				// 0x2C
	s16 map_coll_bottom;				// 0x2E
	s16 map_coll_width;					// 0x30
	u16 size;							// 0x32  (hitbox radius, use * 0.5F)

	// 0x34 u32 bitfield: angle + kb_scale + damage + element
#if IS_BIG_ENDIAN
	s32 angle : 10;
	u32 knockback_scale : 10;
	u32 damage : 8;
	u32 element : 4;
#else
	u32 element : 4;
	u32 damage : 8;
	u32 knockback_scale : 10;
	u32 angle : 10;						// use BITFIELD_SEXT10() at read sites
#endif

	// 0x38 u32 bitfield: kb_weight + shield_damage + attack_count + can_setoff + hit_sfx + 1 pad
#if IS_BIG_ENDIAN
	u32 knockback_weight : 10;
	s32 shield_damage : 8;
	u32 attack_count : 2;
	ub32 can_setoff : 1;
	u32 hit_sfx : 10;
	u32 : 1;
#else
	u32 : 1;
	u32 hit_sfx : 10;
	u32 can_setoff : 1;
	u32 attack_count : 2;
	u32 shield_damage : 8;				// use BITFIELD_SEXT8() at read sites
	u32 knockback_weight : 10;
#endif

	// 0x3C u32 bitfield: priority + 5 flags + kb_base + type + hitstatus + 2 unk + 4 pad
	// (Upstream fields sum to 28 bits; IDO auto-pads the u32 to 32 with 4 trailing pad bits.)
#if IS_BIG_ENDIAN
	u32 priority : 3;
	ub32 can_rehit_item : 1;
	ub32 can_rehit_fighter : 1;
	ub32 can_hop : 1;
	ub32 can_reflect : 1;
	ub32 can_shield : 1;
	u32 knockback_base : 10;
	u32 type : 4;
	u32 hitstatus : 4;
	ub32 unk_atca_0x3C_b6 : 1;
	ub32 unk_atca_0x3C_b7 : 1;
	u32 : 4;
#else
	u32 : 4;
	u32 unk_atca_0x3C_b7 : 1;
	u32 unk_atca_0x3C_b6 : 1;
	u32 hitstatus : 4;
	u32 type : 4;
	u32 knockback_base : 10;
	u32 can_shield : 1;
	u32 can_reflect : 1;
	u32 can_hop : 1;
	u32 can_rehit_fighter : 1;
	u32 can_rehit_item : 1;
	u32 priority : 3;
#endif

	// 0x40 u32 bitfield: drop_sfx + throw_sfx + smash_sfx + 2 pad
#if IS_BIG_ENDIAN
	u32 drop_sfx : 10;
	u32 throw_sfx : 10;
	u32 smash_sfx : 10;
	u32 : 2;
#else
	u32 : 2;
	u32 smash_sfx : 10;
	u32 throw_sfx : 10;
	u32 drop_sfx : 10;
#endif

	// 0x44 u32 bitfield: vel_scale + 7 pad + spin_speed
	// (IDO packs the upstream's plain `u16 spin_speed` into this u32's low 16
	// bits — see the comment at the top of this struct.)
#if IS_BIG_ENDIAN
	u32 vel_scale : 9;
	u32 : 7;
	u32 spin_speed : 16;
#else
	u32 spin_speed : 16;
	u32 : 7;
	u32 vel_scale : 9;
#endif
};

#ifdef PORT
_Static_assert(sizeof(ITAttributes) == 0x48, "ITAttributes must be 72 bytes to match ROM stride (reloc file 251)");
_Static_assert(offsetof(ITAttributes, attack_offset0_y) == 0x14, "attack_offset0_y offset mismatch");
_Static_assert(offsetof(ITAttributes, damage_coll_offset) == 0x1E, "damage_coll_offset offset mismatch");
_Static_assert(offsetof(ITAttributes, size) == 0x32, "size offset mismatch");
#endif

struct ITStruct 						// Common items, stage hazards, fighter items and Pokémon
{
	ITStruct *next;    					// Memory region allocated for next ITStruct
	GObj* item_gobj;		 			// Item's GObj pointer
	GObj* owner_gobj;		 			// Item's owner
	s32 kind;			 				// Item ID
	s32 type;			 				// Item type
	u8 team;				 			// Item's team
	u8 player;				 			// Item's port index
	u8 handicap;			 			// Item's handicap
	s32 player_num;		 				// Item's player number
	s32 percent_damage;		 			// Item's damage
	u32 hitlag_tics;		 			// Item's hitlag
	s32 lr;					 			// Item's facing direction

	struct ITPhysics
	{
		f32 vel_ground; 				// Item's ground velocity
		Vec3f vel_air;					// Item's aerial velocity

	} physics;

	MPCollData coll_data;	   			// Item's collision data
	sb32 ga; 							// Ground or air bool

	ITAttackColl attack_coll;	 		// Item's hitbox
	ITDamageColl damage_coll; 			// Item's hurtbox

	s32 hit_normal_damage;				// Damage applied to entity this item has hit
	s32 hit_lr;						// Direction of outgoing attack?
	s32 hit_refresh_damage; 			// Damage applied to entity this item has hit, if rehit is possible?
	s32 hit_attack_damage;				// Damage item dealt to other attack
	s32 hit_shield_damage;	 	 		// Damage item dealt to shield

	f32 shield_collide_angle; 			// Angle at which item collided with shield?
	Vec3f shield_collide_dir; 			// Direction of incoming velocity vector?

	GObj* reflect_gobj;					// GObj that reflected this item
#if defined(REGION_US)
	GMStatFlags reflect_stat_flags; 	// Status flags of GObj reflecting this
	u16 reflect_stat_count;				// Status update count at the time the item is reflected
#endif

	s32 damage_highest;		  			// I don't know why there are at least two of these
	f32 damage_knockback;	  			// Item's calculated knockback
	s32 damage_queue;		  			// Used to calculate knockback?
	s32 damage_angle;		  			// Angle of attack that hit the item
	s32 damage_element;		  			// Element of attack that hit the item
	s32 damage_lr;			  			// Direction of incoming attack
	GObj *damage_gobj;		  			// GObj that last dealt damage to this item?
	u8 damage_team;			  			// Team of attacker
	u8 damage_port;			  			// Controller port of attacker
	s32 damage_player_num; 				// Player number of attacker
	u8 damage_handicap;		  			// Handicap of attacker
	s32 damage_display_mode;  			// Display mode of attacker which the item takes on
	s32 damage_lag;			  			// Used to calculate hitlag?

	s32 lifetime; 						// Item's duration in frames

	f32 vel_scale; 						// Scale item's velocity

	u16 drop_sfx;   					// SFX to play when item is dropped
	u16 throw_sfx;  					// SFX to play when item is tilt-thrown
	u16 smash_sfx; 						// SFX to play when item is smash-thrown

	ub32 is_allow_pickup : 1;	  		// Bool to check whether item can be picked up or not
	ub32 is_hold : 1;			  		// Whether item is held by a fighter

	u32 times_landed : 2;		  		// Number of times item has touched the ground when landing; used to tell how many times item should bounce up

	u32 times_thrown : 3;		  		// Number of times item has been dropped or thrown by players; overflows after 7
	ub32 weight : 1;			  		// 0 = item is heavy, 1 = item is light
	ub32 is_damage_all : 1;		  		// Item ignores ownership and can damage anything?
	ub32 is_attach_surface : 1;	  		// Item is "sticking" to a map collision line specified by attach_line_id
	ub32 is_thrown : 1;			  		// Apply magnitude and throw multiplier to damage output
	u16 attach_line_id;			  		// Map collision line ID that item is attached to
	u32 pickup_wait : 12;		  		// Number of frames item can last without being picked up (if applicable)
	ub32 is_allow_knockback : 1;  		// Item can receive knockback velocity
	ub32 is_unused_item_bool : 1; 		// Unused? Set various times, but no item function ever checks this
	ub32 is_static_damage : 1;	  		// Ignore reflect multiplier if TRUE

	ITAttributes *attr; 				// Pointer to standard attributes

	GMColAnim colanim; 					// Item's ColAnim struct

	ub32 is_hitlag_victim : 1; 			// Item can deal hitlag to target

	u16 multi;			  				// Multi-purpose variable; e.g. it is used as intangibility delay for Star Man and ammo count for Ray Gun

	u32 event_id : 4; 					// Item hitbox script index? When in doubt, make this u8 : 4

	f32 spin_step; 						// Item spin rotation step

	GObj *arrow_gobj; 					// Red arrow pickup indicator GObj
	u8 arrow_timer;	  					// Frequency of red arrow indicator flash

	union ITStatusVars 					// Item-specific state variables
	{
		// Common items
		ITCommonItemVarsTaru taru;
		ITCommonItemVarsBombHei bombhei;
		ITCommonItemVarsBumper bumper;
		ITCommonItemVarsShell shell;
		ITCommonItemVarsMBall mball;

		// Fighter items
		ITFighterItemVarsPKFire pkfire;
		ITFighterItemVarsLinkBomb linkbomb;

		// Stage items
		ITGroundItemVarsPakkun pakkun;
		ITGroundItemVarsTaruBomb tarubomb;
		ITGroundItemVarsGLucky glucky;
		ITGroundItemVarsMarumine marumine;
		ITGroundItemVarsHitokage hitokage;
		ITGroundItemVarsFushigibana fushigibana;
		ITGroundItemVarsPorygon porygon;

		// Poké Ball Pokémon
		ITMonsterItemVarsIwark iwark;
		ITMonsterItemVarsKabigon kabigon;
		ITMonsterItemVarsTosakinto tosakinto;
		ITMonsterItemVarsNyars nyars;
		ITMonsterItemVarsLizardon lizardon;
		ITMonsterItemVarsSpear spear;
		ITMonsterItemVarsKamex kamex;
		ITMonsterItemVarsMLucky mlucky;
		ITMonsterItemVarsStarmie starmie;
		ITMonsterItemVarsDogas dogas;
		ITMonsterItemVarsMew mew;

	} item_vars;

	s32 display_mode; 					// Item's display mode

	sb32 (*proc_update)(GObj*);	   		// Update general item information
	sb32 (*proc_map)(GObj*);	   		// Update item's map collision
	sb32 (*proc_hit)(GObj*);	   		// Runs when item collides with a hurtbox
	sb32 (*proc_shield)(GObj*);	   		// Runs when item collides with a shield
	sb32 (*proc_hop)(GObj*);	  		// Runs when item bounces off a shield
	sb32 (*proc_setoff)(GObj*);	   		// Runs when item's hitbox collides with another hitbox
	sb32 (*proc_reflector)(GObj*);		// Runs when item is reflected
	sb32 (*proc_damage)(GObj*);	   		// Runs when item takes damage
	sb32 (*proc_dead)(GObj*);	   		// Runs when item is in a blast zone
};

#endif
