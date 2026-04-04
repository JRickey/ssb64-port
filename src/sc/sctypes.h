#ifndef _SCTYPES_H_
#define _SCTYPES_H_

#include <ssb_types.h>
#include <macros.h>
#include <sys/obj.h>
#include <sc/scdef.h>

#include <gm/generic.h>

struct SC1PGameComputer
{
	ub8 is_team_attack;
	u8 item_appearance_rate;
	u8 enemy_level[nSC1PGameDifficultyEnumCount];
	u8 enemy_handicap[nSC1PGameDifficultyEnumCount];
	u8 ally_level[nSC1PGameDifficultyEnumCount];
	u8 ally_handicap[nSC1PGameDifficultyEnumCount];
};

struct SC1PGameStage
{
	u8 screenflash_alpha;
	u8 gkind;
	u32 item_toggles;
	u8 opponent_count;            // Number of enemy players to spawn?
	u8 fkind[2];
	u8 opponent_behavior;
	u8 ally_count;
	u8 ally_behavior;
};

struct SC1PGameFighter
{
	s32 mapobj_kind; // Spawn position?
	void *figatree;
	s32 copy_kind;
	s32 team_order;
	sb32 is_skip_entry;
	sb32 is_magnify_ignore;
	u8 cp_trait;
	f32 camera_frame_mul;
};

struct SC1PGameStats
{
	s32 team_order;
	s32 damage_status_id;
	s32 damage_player;
	s32 damage_object_class;
	s32 damage_object_kind;
	GMStatFlags damage_stat_flags;
	u16 damage_stat_count;
};

struct SC1PGameBossPlan
{
    s32 unk_sc1pbossplan_0x0;
    u8 dl_link;
    u32 camera_tag;
    Vec3f pos;
};

struct SC1PGameBossAnim
{
    intptr_t o_anim_joint;
    intptr_t o_matanim_joint;
    f32 anim_speed;
};

struct SC1PGameBossEffect
{
    void (*proc_update)(GObj*);
    void (*proc_display)(GObj*);
    intptr_t o_dobjdesc;
    intptr_t o_mobjsub;
};

struct SC1PGameBossWallpaper
{
    s32 loop_count;
    s32 effect_count;
    s32 anim_count;
    s32 plan_count;
    s32 dobj_color_id;
    s32 color_id;
    s32 change_wait_base;
    s32 change_damage_min;
    sb32 is_random_wallpaper;
    SC1PGameBossEffect *bosseffect;
    SC1PGameBossAnim *bossanim;
    SC1PGameBossPlan *bossplan;
};

struct SC1PGameBossMain
{
    sb32 is_skip_wallpaper_change;
    s32 wallpaper_id;
    s32 change_wait;
    void *file_head;
    SC1PGameBossWallpaper *bosswallpaper;
    s32 bossplayer;
};

struct SC1PStageClearStats
{
	s32 bonus_array_id;
	s32 bonus_id;
};

struct SC1PStageClearScore
{
	intptr_t offset;
	s32 points;
};

struct SC1PTrainingModeSprites
{
	Vec2h pos;
	Sprite *sprite;
};

struct SC1PTrainingModeFiles
{
	s32 file_id;
	intptr_t offset;
	SYColorRGB fog_color;
};

struct SC1PTrainingModeMenu
{
	s32 main_menu_option;					  // Option selected in the main training mode menu
											  // (vertically)
	s32 damage;								  // Total combo damage accumulated
	s32 combo;								  // Combo count
	s32 item_hold;							  // Training Mode Item ID of item currently held by player
	s32 item_menu_option;					  // Option selected in "Item" settings
	s32 cp_menu_option;						  // Option selected in "CP" settings
	s32 speed_menu_option;					  // Option selected in "Speed" settings
	s32 view_menu_option;					  // Option selected in "View" settings
	s32 dummy;							 	  // Dummy fighter's port ID
	SC1PTrainingModeSprites *display_label_sprites; // "DAMAGE", "COMBO", "ENEMY", "SPEED" text
	Sprite **display_option_sprites;
	SC1PTrainingModeSprites *menu_label_sprites; // Orange text describing what each option is?
	Sprite **menu_option_sprites;
	SC1PTrainingModeSprites *unk_trainmenu_0x34;
	SC1PTrainingModeSprites *unk_trainmenu_0x38;
	GObj *damage_display_gobj; // Interface GObj of damage stat display
	GObj *combo_display_gobj;  // Interface GObj of combo stat display
	GObj *cp_display_gobj;	   // Interface GObj of CP behavior display
	GObj *speed_display_gobj;  // Interface GObj of speed display
	GObj *item_display_gobj;   // Interface GObj of item display
	GObj *menu_label_gobj;	   // Interface GObj of main menu options descriptions
							   // (orange text)
	GObj *cursor_gobj;
	GObj *cp_option_gobj;	// Interface GObj of main menu CP options
	GObj *item_option_gobj; // Interface GObj of main menu Item options
	GObj *speed_option_gobj;
	GObj *view_option_gobj;
	GObj *arrow_option_gobj;
	SObj *hscroll_option_sobj[4];
	GObj *unk_trainmenu_0x7C;
	GObj *combo0;
	SObj *vscroll_option_sobj[6][2];
	u32 cursor_ulx, cursor_uly;
	u32 cursor_lrx, cursor_lry;
	u16 button_hold;
	u16 button_tap;
	u16 button_queue;
	s32 rapid_scroll_wait;
	u8 damage_reset_wait;	 // Wait this many frames before resetting combo damage
	u8 combo_reset_wait;	 // Wait this many frames before resetting combo count
	ub8 exit_or_reset;		 // 0 = exit, 1 = reset
	u8 lagtic_wait;		 // Wait this many frames before duplicate/lag frame is
							 // applied? Used for 2/3 speed with a setting of 1
	u8 frameadvance_wait;	 // Wait this many frames before advancing to the next
							 // frame
	u8 item_spawn_wait;		 // Cooldown before new item can be summoned
	u16 magnify_wait;		 // Cooldown before magnifying glass is shown again after
							 // switching back from Close-Up view
	ub8 is_read_menu_inputs; // Menu navigation inputs are ignored if FALSE
	s32 unknown[2];
};

struct SCStaffrollMatrix
{
	u8 filler_0x0[0xC];
	f32 unk_gmcreditsmtx_0xC;
	f32 unk_gmcreditsmtx_0x10;
	f32 unk_gmcreditsmtx_0x14;
};

struct SCStaffrollText
{
	s32 character_start;        // Where to begin reading text from in main character array
	s32 character_count;        // Number of characters in credits role card to display
};

struct SCStaffrollSprite
{
	u8 width;
	u8 height;
	intptr_t offset;
};

struct SCStaffrollStaff
{
	u8 filler_0x0[0x4];
	s32 staff_id;
};

struct SCStaffrollName
{
	SCStaffrollName *next;
	s32 name_id;
	sb32 job_or_name;   // 0 = job (e.g. Director), 1 = name (e.g. Masahiro Sakurai)
	f32 offset_x;
	f32 unkgmcreditsstruct0x10;
	f32 interpolation;
	s32 status;
	s32 unkgmcreditsstruct0x1C;
};

struct SCStaffrollJob
{
	s32 prefix_id;    // e.g. "Chief" -> Chief Programmers
	s32 job_id;       // Job text to use
	s32 staff_count;  // Number of staff members to roll until new job is shown
};

struct SCStaffrollSetup
{
	f32 unk_gmcreditsunk_0x0;
	DObj *dobj;
	f32 spacing;
	f32 unk_gmcreditsunk_0xC;
	f32 unk_gmcreditsunk_0x10;
};

struct SCStaffrollProjection
{
	Vec3f pv0;
	Vec3f pv1;
	Vec3f pv2;
	Vec3f pv3;
	f32 px0;
	f32 py0;
	f32 px1;
	f32 py1;
	f32 px2;
	f32 py2;
	f32 px3;
	f32 py3;
};

struct SCExplainMain
{
    SObj *textbox_sobj;
    GObj *stick_gobj;
    GObj *spark_gobj;
    GObj *rgb_gobj;
    SObj *phase_sobj0;
    SObj *phase_sobj1;
    SObj *phase_sobj2;
    SObj *phase_sobj3;
    SObj *phase_sobj4;
    SObj *phase_sobj5;
    s32 phase_advance_wait;
    s32 phase;
    u8 unk_scexplainif_0x30;
    u8 stick_status;
};

struct SCExplainArgs
{
    u16 sprite_pos_x;
    u8 sprite_pos_y;
    u8 sprite_status;
};

struct SCExplainPhase
{
    u16 phase_time;                     // Time the given explanation phase of the How to Play tutorial should last
    u16 unused;
    u8 textbox_pos_x;
    u8 textbox_pos_y;
    Sprite *sprite;
    SCExplainArgs control_stick_args;
    SCExplainArgs phase_args0;
    SCExplainArgs phase_args1;
    SCExplainArgs phase_args2;
    SCExplainArgs phase_args3;
    SCExplainArgs phase_args4;
    SCExplainArgs rgb_overlay_args;
    SCExplainArgs phase_args5;
};

struct SCAutoDemoProc
{
	u16 focus_end_wait;
	void (*func_change)();
	void (*func_focus)();
};

struct SCBattleResults
{
	s32 tko;
	s32 kos;
	u8 player_or_team;
	u8 unk_battleres_0x9;
	ub8 is_human;
};

struct SCPlayerData
{
	u8 level;		   		                        // Actually begins at 1 instead of 0
	u8 handicap;	   		                        // Master Hand's handicap value is 35!?
	u8 pkind;	   			                        // 0 = HMN, 1 = CPU, 2 = none (?), 5 = How to Play (?)
	u8 fkind;
	u8 team;
	u8 player; 				                        // Identical to team if team battle is on?
	u8 costume;
	u8 shade; 				                        // Fog overlay applied when several players use the same costume
	u8 color;		                        		// Used for emblems, player tags, and possibly more
	ub8 is_single_stockicon;                        // Whether player's stock is permanent or a limited amount
	u8 tag;			                        		// Player tag sprite index (i.e. 1P, 2P, 3P, 4P, CP or heart)
	s8 stock_count;			                        // -1 = player has no stocks
	ub8 is_spgame_enemy;		                        // Respawn flag of multi-man enemy teams (Yoshi, Kirby, Fighting Polygons) in 1P mode
	u8 place;			                        	// Player's placement in battle results
	s32 falls;
	s32 score;								        // Caps at positive 999, crashes if way too low in the negatives
	s32 total_kos_players[GMCOMMON_PLAYERS_MAX];    // KOs scored on other players
	s32 unk_pblock_0x28;
	s32 unk_pblock_0x2C;
	s32 total_selfdestructs;					    // Applied when damaging player's ID is -1 or GMCOMMON_PLAYERS_MAX
	s32 total_damage_given;						    // Total damage dealt to all players
	s32 total_damage_all;						    // Damage received from all hazards
	s32 total_damage_players[GMCOMMON_PLAYERS_MAX]; // Total damage received from each player present
	s32 stock_damage_all;						    // All damage received per current stock, from any hazard
	s32 combo_damage_foe;						    // Total damage from consecutive hits dealt by foes (Resets when hitstun ends)
	s32 combo_count_foe;						    // Number of consecutive hits received from foes (Resets when hitstun ends)
	GObj *fighter_gobj;							    // Pointer to player's fighter GObj
	u32 stale_id;							  	    // Current position in stale queue?
	
	struct SCPlayerStale
	{
		u16 attack_id;
		u16 motion_count;

	} stale_info[5];
};

struct SCBattleState
{
	u8 game_type;
	u8 gkind;
	ub8 is_team_battle;
	u8 game_rules; 									// Series of flags; 0x1 = time, 0x2 = stock
	u8 pl_count;									// Number of human players registered
	u8 cp_count;									// Number of computer players registered
	u8 time_limit;									// Time limit (in seconds)
	u8 stocks;	 									// Number of stocks set in game rules
	u8 handicap; 									// 0 = OFF, 1 = ON, 2 = AUTO
	ub8 is_team_attack;	 							// 0 = OFF, 1 = ON
	ub8 is_stage_select; 							// 0 = OFF, 1 = ON
	u8 damage_ratio;								// From 50% to 200%
	u32 item_toggles; 								// Bits = item's on/off switch from match settings
	ub8 is_reset_players;							// Reset player slots when entering VS Mode character select screen
	u8 game_status;
	u32 time_remain;								// Tics remaining until timeout
	u32 time_passed; 								// Current match tic, counts up from 0
	u8 item_appearance_rate;						// Item appearance rate setting 
	ub32 is_show_score : 1;							// Displays score when a fighter falls
	ub32 is_not_teamshadows : 1;					// If FALSE, shadows are colored based on players' team affiliation, otherwise use default shadow color
	SCPlayerData players[GMCOMMON_PLAYERS_MAX]; 	// Holds data for each player
};

struct SCCommonData
{
	u8 scene_curr;									// Current scene
	u8 scene_prev;									// Previous scene
	u8 unlock_messages[nLBBackupUnlockEnumCount];	// Queued unlock messages
	u8 challenger_fkind;							// Opponent being fought in "Challenger Approaching" battle
	u16 demo_mask_prev;								// Mask of previously demo'd fighters
	u8 demo_first_fkind;							// First auto-demo fighter to focus on?
	u8 demo_fkind[2];								// Auto-demo fighters to focus on
	u8 gkind;										// Stage selected
	ub8 is_suddendeath;								// Whether current battle is a Sudden Death?
	ub8 is_continue;								// Whether continue has been selected
	ub8 is_reset; 									// A + B + R + Z button combination done
	u8 player;										// 1P Game player port
	u8 fkind;										// 1P Game player's character
	u8 costume;										// 1P Game player's costume
	u8 spgame_time_limit;							// 1P Game global time limit
	u8 spgame_stage; 								// 1P Game stage (0 = VS Link, 1 = VS Yoshi Team, etc.)
	u8 ally_players[2];								// Ally player ports
	u32 spgame_time_remain; 						// Timer of current 1P Game stage in seconds
	u32 spgame_score;								// 1P Game score
	u32 continues_used;								// 1P Game number of continues used
	u32 bonus_count;	   							// Number of bonuses player acquired throughout 1P Game
	u32 bonus_get_mask[3]; 							// Different bonuses the player has accumulated in current 1P Game Stage
													// Last member is total Bonus Stage task count
	u8 bonus_tasks_complete;						// Number of Bonus Stage tasks completed so far in current session
	u8 bonus_fkind;									// Bonus Stage character
	u8 bonus_costume;								// Bonus Stage costume
	u8 training_man_fkind;							// Training Mode player's character
	u8 training_man_costume;						// Training Mode player's costume
	u8 training_com_fkind;							// Training Mode CPU character
	u8 training_com_costume;						// Training Mode CPU costume
	ub8 is_extend_demo_wait;						// Wait longer for auto-demo to start?
	u8 demo_gkind_order;							// Current index of stage array to pick for auto-demo
	u8 maps_vsmode_gkind;							// VS Mode stage selected
	u8 maps_training_gkind;							// Training Mode stage selected
	u8 challenger_level_drop;						// Subtract from default CP level
	ub8 is_title_anim_viewed;						// Has the title screen animation been viewed?
};

#endif
