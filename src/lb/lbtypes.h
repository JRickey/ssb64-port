#ifndef _LBTYPES_H_
#define _LBTYPES_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <gm/gmdef.h>
#include <lb/lbdef.h>

struct LBFileNode
{
    u32 id;
    void *addr;
};

union LBRelocDesc
{
    struct lbRelocInfo
    {
        u16 reloc;
        u16 words_num;

    } info;

    void *p;
};

struct LBRelocSetup
{
    uintptr_t table_addr;        // ROM address
    u32 table_files_num;         // Total number of files in table?
    void *file_heap;         
    size_t file_heap_size;
    LBFileNode *status_buffer;
    size_t status_buffer_size;
    LBFileNode *force_status_buffer;
    size_t force_status_buffer_size;
};

struct LBTableEntry
{
    ub32 is_compressed : 1;
    u32 data_offset : 31;
    u16 reloc_intern_offset;    // in words
    u16 compressed_size;        // in words
    u16 reloc_extern_offset;    // in words
    u16 decompressed_size;      // in words
};

struct LBInternBuffer
{
    uintptr_t rom_table_lo; // Start of file table
    u32 total_files_num;
    uintptr_t rom_table_hi; // End of file table
    void *heap_start;
    void *heap_ptr;
    void *heap_end;

    // "status buffer"
    s32 status_buffer_num;
    s32 status_buffer_max;
    LBFileNode *status_buffer;

    // "force status buffer"
    s32 force_status_buffer_num;
    s32 force_status_buffer_max;
    LBFileNode *force_status_buffer;
};

struct LBScriptDesc
{
    s32 scripts_num;        // Number of scripts in array?
    LBScript *scripts[1];   // Dynamic array
};

struct LBScript
{
    u16 kind;               // Generator kind
	u16 texture_id;         // Texture ID?
    u16 generator_lifetime; // Total frames?
    u16 particle_lifetime;  // Particle lifetime?
    u32 flags;              // Flags?
	f32 gravity;
    f32 friction;
    Vec3f vel;
    f32 unk_script_0x20;
    f32 unk_script_0x24;
    f32 update_rate;
    f32 size;
    u8 bytecode[1];         // Particle bytecode
};

struct LBTextureDesc
{
    s32 textures_num;
    LBTexture *textures[1];
};

struct LBTexture
{
    u32 count;
    s32 fmt, siz;
    s32 width, height;
    u32 flags;              // Palette flags; if (this value & 1), there is only one palette; otherwise each image has its own palette
    void *data[1];          // Offsets to image, then palette data
};

struct LBGenerator
{
    LBGenerator *next;
	u16 generator_id;
	u16 flags;
	u8 kind;
    u8 bank_id;
    u16 texture_id;
    u16 particle_lifetime;
	u16 generator_lifetime;
	u8 *bytecode;
    Vec3f pos;
    Vec3f vel;
    f32 gravity;
    f32 friction;
    f32 size;
    f32 unk_gn_0x38;
    f32 unk_gn_0x3C;
    f32 update_rate;
    f32 frame;
    DObj *dobj;
    LBTransform *xf;

    union LBGeneratorVars
    {
        struct LBGeneratorRotate
        {
            f32 base, target;

        } rotate;

        Vec3f move;

        struct LBGeneratorVortex
        {
            f32 f;
            u16 lifetime;

        } vortex;

    } generator_vars;
};

struct LBTransform
{
    LBTransform *next;
    Vec3f translate;
    Vec3f rotate;
    Vec3f scale;
    u8 transform_status;        // 0 = ???, 1 = prepare matrix for transformation, 2 = finished transformation
    u8 transform_id;
    u16 users_num;              // Number of other structs using this particular LBTransform struct?
    Mtx44f affine;              // Translate + rotate + scale matrix
    Mtx44f projection;          // Projection matrix
    f32 pc0_magnitude;          // Projection matrix column 0 magnitude
    f32 pc1_magnitude;          // Projection matrix column 1 magnitude
    void (*proc_dead)(LBTransform*);
    u16 generator_id;
    GObj *effect_gobj;
};

struct LBParticle
{
    LBParticle *next;
    u16 generator_id;
    u16 flags;
    u8 bank_id;
    u8 loop_count;              // How many times to loop specific segment of bytecode
    u8 texture_id;              // Texture series ID
    u8 frame_id;                // Animation frame of texture series
    ub16 bytecode_timer;        // Wait timer before next bytecode event is parsed
    u16 size_target_length;     // Time to LERP size_target variable
    u16 primcolor_target_length;// Time to LERP target primitive color
    u16 envcolor_target_length; // Time to LERP target environment color
    u8 *bytecode;               // Bytecode base
    u16 bytecode_csr;           // Bytecode cursor
    u16 return_ptr;             // Return to bytecode start + return_ptr on command
    u16 loop_ptr;               // Loop from bytecode start + loop_ptr
    u16 lifetime;               // Particle's duration; ends at 1, not 0, for some reason
    Vec3f pos;                  // Position
    Vec3f vel;                  // Velocity
    f32 gravity;                // Gravity?
    f32 friction;               // Friction?
    f32 size;
    f32 size_target;
    SYColorRGBA primcolor;
    SYColorRGBA target_primcolor;
    SYColorRGBA envcolor;
    SYColorRGBA target_envcolor;
    LBGenerator *gn;
    LBTransform *xf;
};

struct LBTransitionDesc
{
    u32 file_id;
    intptr_t o_dobjdesc;
    intptr_t o_anim_joint;
    s32 unk_lbtransition_0xC;
};

struct LBBackupVSRecord
{
	u16 ko_count[GMCOMMON_FIGHTERS_PLAYABLE_NUM];
	u32 time_used;                                  // In seconds
	u32 damage_given;
	u32 damage_taken;
	u16 unk;
	u16 selfdestructs;
	u16 games_played;
	u16 player_count_tally;
	u16 player_count_tallies[GMCOMMON_FIGHTERS_PLAYABLE_NUM];
	u16 played_against[GMCOMMON_FIGHTERS_PLAYABLE_NUM];
};

struct LBBackup1PRecord
{
	u32 spgame_hiscore;
	u32 spgame_continues;
	u32 spgame_total_bonuses;
	u8 spgame_best_difficulty;
	u32 bonus1_time;	        // Break the Targets high score
	u8 bonus1_task_count;       // Targets broken
	u32 bonus2_time;	        // Board the Platforms high score
	u8 bonus2_task_count;       // Platforms boarded
	ub8 is_spgame_complete;	    // Whether character has completed 1P Game or not
};

// Save Data structure
struct LBBackupData
{
	LBBackupVSRecord vs_records[GMCOMMON_FIGHTERS_PLAYABLE_NUM];
	ub8 is_allow_screenflash; 	// Toggle for enabling white screen flash when, for example, a character takes too much damage
							  	// Likely tied to the unused "background flash" option? It is always toggled ON, even after clearing save data
	ub8 sound_mono_or_stereo;   // Whether sound mode is mono or stereo
	s16 screen_adjust_h;		// Offset from screen center horizontally
	s16 screen_adjust_v;		// Offset from screen center vertically
	u8 characters_fkind;		// Last character viewed on Character Data menu
	u8 unlock_mask;             // Mask of unlocked features
	u16 fighter_mask; 		    // Mask of available characters
	u8 spgame_difficulty;       // Last 1P Game difficulty selected
	u8 spgame_stock_count;      // Last 1P Game stock count selected
	LBBackup1PRecord spgame_records[GMCOMMON_FIGHTERS_PLAYABLE_NUM];
	u16 ground_mask;   	        // Records mask of unique stages played in VS mode
	u8 vs_itemswitch_battles; 	// Records number of VS games played for Item Switch unlock
	u16 vs_total_battles;		// Total amount of VS games played?
	u8 error_flags;		   		// Some kind of anti-piracy/anti-tempering measure?
	u8 boot;                    // How many times the game has successfully booted to the title screen
	u16 signature;              // ??? expected to be 0x29A, never actually written however
	s32 checksum; 				// Checksum of save data
};

#endif
