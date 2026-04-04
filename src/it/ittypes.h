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
	u8 timer;
	s32 angle : 10;
	u32 damage : 8;
	u16 size;
};

struct ITMonsterEvent	// Full-scale hitbox subaction event? Used by Venusaur and Porygon.
{
	u8 timer;
	s32 angle : 10;
	u32 damage : 8;
	u16 size;
	u32 knockback_scale;
	u32 knockback_weight;
	u32 knockback_base;
	s32 element;
	ub32 can_setoff : 1;
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

struct ITAttributes
{
	void *data; 						// Either DObjDesc or displaylist?
	MObjSub ***p_mobjsubs;				// Array of MObjSubs for each MObj on each DObj
	AObjEvent32 **anim_joints;			// Array of AnimJoints for each DObj
	AObjEvent32 ***p_matanim_joints;	// Array of MatAnimJoints for each MObj on each DObj
	ub32 is_display_xlu : 1;			// If TRUE, use transparency renderer
	ub32 is_item_dobjs : 1;				// If TRUE, set up special item DObj node tree; otherwise set up common DObj node tree
	ub32 is_display_colanim : 1;		// If TRUE, use ColAnim renderer
	ub32 is_give_hitlag : 1;			// If TRUE, deal hitlag on contact
	ub32 weight : 1; 					// Heavy = 0, Light = 1
	s32 attack_offset0_x : 16;			// Hitbox ID0 offset X
	s32 attack_offset0_y : 16; 			// Hitbox ID0 offset Y
	s32 attack_offset0_z : 16;			// Hitbox ID0 offset Z
	s32 attack_offset1_x : 16;			// Hitbox ID1 offset X
	s32 attack_offset1_y : 16;			// Hitbox ID1 offset Y
	s32 attack_offset1_z : 16;			// Hitbox ID1 offset Z
	Vec3h damage_coll_offset;			// Hurtbox offset
	Vec3h damage_coll_size;				// Hurtbox size
	s16 map_coll_top;					// Map Collision Box top
	s16 map_coll_center;				// Map Collision Box center
	s16 map_coll_bottom;				// Map Collision Box bottom
	s16 map_coll_width;					// Map Collision Box width
	u16 size;							// Hitbox size
	s32 angle : 10;						// Hitbox launch angle
	u32 knockback_scale : 10;			// Hitbox knockback scale
	u32 damage : 8;						// Hitbox damage
	u32 element : 4;					// Hitbox element
	u32 knockback_weight : 10;			// Hitbox fixed knockback
	s32 shield_damage : 8;				// Hitbox shield damage
	u32 attack_count : 2;				// Number of hitboxes
	ub32 can_setoff : 1;				// Whether hitbox can clang or not
	u32 hit_sfx : 10;					// Hitbox FGM
	u32 priority : 3;					// Hitbox priority
	ub32 can_rehit_item : 1;			// Whether hitbox can rehit items
	ub32 can_rehit_fighter : 1;			// Whether hitbox can rehit fighters
	ub32 can_hop : 1;					// Whether item can deflect off shields
	ub32 can_reflect : 1;				// Whether item can be reflected
	ub32 can_shield : 1;				// Whether item can be shielded
	u32 knockback_base : 10;			// Hitbox base knockback
	u32 type : 4;						// Item's usage type
	u32 hitstatus : 4;					// Item's base hurtbox collision state
	ub32 unk_atca_0x3C_b6 : 1;			// Unused
	ub32 unk_atca_0x3C_b7 : 1;			// Unused
	u32 drop_sfx : 10;					// FGM to play when Item is dropped
	u32 throw_sfx : 10;					// FGM to play when item is thrown lightly
	u32 smash_sfx : 10;					// FGM to play when Item is smash-thrown
	u32 vel_scale : 9;					// Item thrown velocity scale (percentage)
	u16 spin_speed;						// Item model rotation speed (percentage)
};

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
