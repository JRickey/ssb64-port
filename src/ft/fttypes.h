#ifndef _FTTYPES_H_
#define _FTTYPES_H_

#include <ssb_types.h>
#include <macros.h>
// #include <sys/objdisplay.h> // Probably shouldn't be included
// #include <PR/ultratypes.h>
#include <sys/obj.h>
#include <sys/controller.h>
#include <mp/map.h>
#include <ef/effect.h>
#include <gm/generic.h>
#include <gm/gmsound.h>

#include <ft/ftdef.h>

#include <ft/ftcommon.h>
/*
 * NOTE: <ft/ftchar.h> intentionally included *after* the core struct
 * definitions below. Character headers declare arrays like
 *     extern FTStatusDesc dFTMarioSpecialStatusDescs[];
 * which requires the element type to be complete. On MSVC the incomplete
 * type is accepted at extern-array scope, but clang (correctly) rejects it,
 * so we delay the include until FTStatusDesc/FTMotionDesc are defined. See
 * the secondary include at the bottom of this file.
 */
#ifdef PORT
#include <stddef.h>
#endif

// Structs
struct FTSpecialColl
{
    s32 kind;
    s32 joint_id;
    Vec3f offset;
    Vec3f size;
    s32 damage_resist;
};

struct FTItemPickup
{
    Vec2f pickup_offset_light;
    Vec2f pickup_range_light;
    Vec2f pickup_offset_heavy;
    Vec2f pickup_range_heavy;
};

struct FTThrownStatus
{
    s32 status1, status2;
};

struct FTThrownStatusArray
{
    FTThrownStatus ft_thrown[2];
};

union FTAnimDesc
{
    u32 word;

#if IS_BIG_ENDIAN
    struct
    {
        ub32 is_use_xrotn_joint : 1;        // 0x80000000
        ub32 is_use_transn_joint : 1;       // 0x40000000
        ub32 is_use_yrotn_joint : 1;        // 0x20000000
        ub32 is_enabled_joints : 24;        // 0x10000000 to 0x00000020
        ub32 is_use_submotion_script : 1;   // 0x00000010
        ub32 is_anim_joint : 1;             // 0x00000008
        ub32 is_have_translate_scale : 1;   // 0x00000004
        ub32 is_use_shieldpose : 1;         // 0x00000002
        ub32 is_use_animlocks : 1;          // 0x00000001
    } flags;
#else
    struct
    {
        ub32 is_use_animlocks : 1;          // 0x00000001
        ub32 is_use_shieldpose : 1;         // 0x00000002
        ub32 is_have_translate_scale : 1;   // 0x00000004
        ub32 is_anim_joint : 1;             // 0x00000008
        ub32 is_use_submotion_script : 1;   // 0x00000010
        ub32 is_enabled_joints : 24;        // 0x00000020 to 0x10000000
        ub32 is_use_yrotn_joint : 1;        // 0x20000000
        ub32 is_use_transn_joint : 1;       // 0x40000000
        ub32 is_use_xrotn_joint : 1;        // 0x80000000
    } flags;
#endif
};

struct FTMotionDesc
{
    u32 anim_file_id;       // Animation file ID
    intptr_t offset;        // Offset?
    FTAnimDesc anim_desc;   // Animation flags
};

struct FTMotionDescArray
{
    FTMotionDesc motion_desc[1]; // Array size = last animation ID?
};

struct FTFileSize
{
    size_t main;
    size_t mainmotion_largest_anim;
    size_t submotion_largest_anim;
};

struct FTData
{
    u32 file_main_id; // File size in bytes?
    u32 file_mainmotion_id;
    u32 file_submotion_id;
    u32 file_model_id;
    u32 file_shieldpose_id;
    u32 file_special1_id;
    u32 file_special2_id;
    u32 file_special3_id;
    u32 file_special4_id;
    size_t file_main_size;
    void **p_file_main; // Pointer to character's file?
    void **p_file_mainmotion;
    void **p_file_submotion;
    void **p_file_model;
    void **p_file_shieldpose;
    void **p_file_special1;
    void **p_file_special2;
    void **p_file_special3;
    void **p_file_special4;
    s32 *p_particle;
    uintptr_t particles_script_lo;
    uintptr_t particles_script_hi;
    uintptr_t particles_texture_lo;
    uintptr_t particles_texture_hi;
    intptr_t o_attributes; // Offset to fighter's attributes
    FTMotionDescArray *mainmotion;
    FTMotionDescArray *submotion;
    s32 mainmotion_array_count;
    s32 *submotion_array_count;
    size_t file_anim_size;
};

// Accessory part (e.g. Pikachu wizard hat and Jigglypuff bow)
struct FTAccessPart
{
    s32 joint_id;
#ifdef PORT
    u32 dl;
    u32 mobjsubs;
    u32 costume_matanim_joints;
#else
    Gfx *dl;
    MObjSub **mobjsubs;
    AObjEvent32 **costume_matanim_joints;
#endif
};

struct FTModelPart
{
#ifdef PORT
    u32 dl;
    u32 mobjsubs;
    u32 costume_matanim_joints;
    u32 main_matanim_joints;
#else
    Gfx *dl;
    MObjSub **mobjsubs;
    AObjEvent32 **costume_matanim_joints;
    AObjEvent32 **main_matanim_joints;
#endif
#if IS_BIG_ENDIAN
    u8 flags;
#else
    u8 pad[3];
    u8 flags;
#endif
};

struct FTCommonPart
{
#ifdef PORT
    u32 dobjdesc;                   // Relocation token
    u32 p_mobjsubs;                 // Relocation token
    u32 p_costume_matanim_joints;   // Relocation token
#else
    DObjDesc *dobjdesc;
    MObjSub ***p_mobjsubs;
    AObjEvent32 ***p_costume_matanim_joints;
#endif
#if IS_BIG_ENDIAN
    u8 flags;
#else
    u8 pad[3];
    u8 flags;
#endif
};

struct FTCommonPartContainer
{
    FTCommonPart commonparts[2];
};

#ifdef PORT
#define FTACCESSPART_GET_DL(accesspart) ((Gfx*)PORT_RESOLVE((accesspart)->dl))
#define FTACCESSPART_GET_MOBJSUBS(accesspart) ((MObjSub**)PORT_RESOLVE((accesspart)->mobjsubs))
#define FTACCESSPART_GET_COSTUME_MATANIM_JOINTS(accesspart) ((AObjEvent32**)PORT_RESOLVE((accesspart)->costume_matanim_joints))
#define FTMODELPART_GET_DL(modelpart) ((Gfx*)PORT_RESOLVE((modelpart)->dl))
#define FTMODELPART_GET_MOBJSUBS(modelpart) ((MObjSub**)PORT_RESOLVE((modelpart)->mobjsubs))
#define FTMODELPART_GET_COSTUME_MATANIM_JOINTS(modelpart) ((AObjEvent32**)PORT_RESOLVE((modelpart)->costume_matanim_joints))
#define FTMODELPART_GET_MAIN_MATANIM_JOINTS(modelpart) ((AObjEvent32**)PORT_RESOLVE((modelpart)->main_matanim_joints))
#define FTMODELPARTCONTAINER_GET_DESC(container, index) ((FTModelPartDesc*)PORT_RESOLVE((container)->modelparts_desc[index]))
#define FTPARTS_GET_FLAGS(commonpart) ((commonpart)->flags)
#define FTPARTS_GET_DOBJDESC(commonpart) ((DObjDesc*)PORT_RESOLVE((commonpart)->dobjdesc))
#define FTPARTS_GET_MOBJSUBS(commonpart) ((MObjSub***)PORT_RESOLVE((commonpart)->p_mobjsubs))
#define FTPARTS_GET_COSTUME_MATANIM_JOINTS(commonpart) ((AObjEvent32***)PORT_RESOLVE((commonpart)->p_costume_matanim_joints))
_Static_assert(sizeof(FTAccessPart) == 16, "FTAccessPart must be 16 bytes to match file data layout");
_Static_assert(sizeof(FTModelPart) == 20, "FTModelPart must be 20 bytes to match file data layout");
_Static_assert(sizeof(FTCommonPart) == 16, "FTCommonPart must be 16 bytes to match file data layout");
#else
#define FTACCESSPART_GET_DL(accesspart) ((accesspart)->dl)
#define FTACCESSPART_GET_MOBJSUBS(accesspart) ((accesspart)->mobjsubs)
#define FTACCESSPART_GET_COSTUME_MATANIM_JOINTS(accesspart) ((accesspart)->costume_matanim_joints)
#define FTMODELPART_GET_DL(modelpart) ((modelpart)->dl)
#define FTMODELPART_GET_MOBJSUBS(modelpart) ((modelpart)->mobjsubs)
#define FTMODELPART_GET_COSTUME_MATANIM_JOINTS(modelpart) ((modelpart)->costume_matanim_joints)
#define FTMODELPART_GET_MAIN_MATANIM_JOINTS(modelpart) ((modelpart)->main_matanim_joints)
#define FTMODELPARTCONTAINER_GET_DESC(container, index) ((container)->modelparts_desc[index])
#define FTPARTS_GET_FLAGS(commonpart) ((commonpart)->flags)
#define FTPARTS_GET_DOBJDESC(commonpart) ((commonpart)->dobjdesc)
#define FTPARTS_GET_MOBJSUBS(commonpart) ((commonpart)->p_mobjsubs)
#define FTPARTS_GET_COSTUME_MATANIM_JOINTS(commonpart) ((commonpart)->p_costume_matanim_joints)
#endif

struct FTModelPartDesc
{
    FTModelPart modelparts[1][2];
};

struct FTModelPartContainer
{
#ifdef PORT
    u32 modelparts_desc[FTPARTS_JOINT_NUM_MAX - nFTPartsJointCommonStart];
#else
    FTModelPartDesc *modelparts_desc[FTPARTS_JOINT_NUM_MAX - nFTPartsJointCommonStart];
#endif
};

struct FTModelPartStatus
{
    s8 modelpart_id_base, modelpart_id_curr;
};

struct FTTexturePart
{
    u8 joint_id;
    u8 detail[2];
};

struct FTTexturePartContainer
{
    FTTexturePart textureparts[2];
};

struct FTTexturePartStatus
{
    s8 texture_id_base, texture_id_curr;
};

struct FTMotionFlags
{
#ifdef PORT
    // LP64/LE fix: positional C initializers (`{motion_id_val, attack_id_val}`)
    // would write the wrong bitfield on LE — the LE declaration has `attack_id`
    // declared first for correct runtime bit layout, but that makes the first
    // positional init value land in `attack_id` instead of `motion_id`.  Using
    // plain fields keeps the source-order positional init working.  Embedding
    // struct (`FTStatusDesc`) is never read from file data so the size bump
    // (2 → 4 bytes) is harmless.
    s16 motion_id;
    u16 attack_id;
#elif IS_BIG_ENDIAN
    s16 motion_id : 10;
    u16 attack_id : 6;
#else
    u16 attack_id : 6;
    s16 motion_id : 10;
#endif
};

struct FTMotionScript
{
	f32 script_wait;
	u32 *p_script;
	s32 script_id;
#ifdef PORT
	/* On IDO/N64, void* is 4 bytes, so the original `void *p_goto[1]`
	 * + `s32 loop_count[4]` formed a contiguous 20-byte region where
	 * p_goto[N] for N>0 cleanly aliased loop_count[N-1].  On LP64
	 * void* is 8 bytes, so p_goto[1] (offset 32) aliases TWO loop_count
	 * slots and the strides no longer line up: a 4-byte write to
	 * loop_count[1] clobbers only the *upper* half of an 8-byte
	 * return address sitting in p_goto[1], leaving p_script with
	 * (loop_count[1] << 32) | retaddr_low — exactly the 0x3xxxxxxxx
	 * fault we hit on Yoshi-egg-on-Giant-DK (motion 153, script_id=3).
	 * Give p_goto its own dedicated slots so subroutine and loop state
	 * don't share storage.  See docs/bugs/. */
	void *p_goto[4];
#else
	void *p_goto[1];
#endif
	s32 loop_count[4];
};

struct FTMotionEventDefault // Event with no arguments
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 value : 26;
#else
	u32 value : 26;
	u32 opcode : 6;
#endif
};

struct FTMotionEventDouble // Event with no arguments
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 pad1 : 26;
	u32 pad2 : 32;
#else
	u32 pad1 : 26;
	u32 opcode : 6;
	u32 pad2 : 32;
#endif
};

struct FTMotionEventMakeAttack1
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 attack_id: 3;
	u32 group_id : 3;
	s32 joint_id : 7;
	u32 damage : 8;
	ub32 can_rebound : 1;
	u32 element : 4;
#else
	u32 element : 4;
	ub32 can_rebound : 1;
	u32 damage : 8;
	s32 joint_id : 7;
	u32 group_id : 3;
	u32 attack_id: 3;
	u32 opcode : 6;
#endif
};

struct FTMotionEventMakeAttack2
{
#if IS_BIG_ENDIAN
	u32 size : 16;
	s32 off_x : 16;
#else
	s32 off_x : 16;
	u32 size : 16;
#endif
};

struct FTMotionEventMakeAttack3
{
#if IS_BIG_ENDIAN
	s32 off_y : 16;
	s32 off_z : 16;
#else
	s32 off_z : 16;
	s32 off_y : 16;
#endif
};

struct FTMotionEventMakeAttack4
{
#if IS_BIG_ENDIAN
	s32 angle : 10;
	u32 knockback_scale : 10;
	u32 knockback_weight : 10;
	u32 is_hit_ground_air : 2;
#else
	u32 is_hit_ground_air : 2;
	u32 knockback_weight : 10;
	u32 knockback_scale : 10;
	s32 angle : 10;
#endif
};

struct FTMotionEventMakeAttack5
{
#if IS_BIG_ENDIAN
	s32 shield_damage : 8;
	u32 fgm_level : 3;
	u32 fgm_kind : 4;
	u32 knockback_base : 10;
	u32 pad : 7;
#else
	/* LE mirror: the 25 declared bits don't fill a u32, so a naive reversal
	 * would start `knockback_base` at bit 0 and leave the unused 7 bits at
	 * the TOP of the word — the opposite of where BE leaves them (bottom).
	 * Every field would then land at the wrong physical position.
	 * Leading `pad : 7` pushes the real fields up into the same bit
	 * positions the BE layout produces. */
	u32 pad : 7;
	u32 knockback_base : 10;
	u32 fgm_kind : 4;
	u32 fgm_level : 3;
	s32 shield_damage : 8;
#endif
};
_Static_assert(sizeof(struct FTMotionEventMakeAttack5) == 4, "FTMotionEventMakeAttack5 must be 4 bytes — pad bits missing or bitfield packing broken");

struct FTMotionEventMakeAttack
{
	FTMotionEventMakeAttack1 s1;
	FTMotionEventMakeAttack2 s2;
	FTMotionEventMakeAttack3 s3;
	FTMotionEventMakeAttack4 s4;
	FTMotionEventMakeAttack5 s5;
};

struct FTMotionEventSetAttackOffset1
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 attack_id: 3;
	s32 off_x : 16;
	u32 pad : 7;
#else
	u32 pad : 7;    /* see FTMotionEventMakeAttack5 for rationale */
	s32 off_x : 16;
	u32 attack_id: 3;
	u32 opcode : 6;
#endif
};
_Static_assert(sizeof(struct FTMotionEventSetAttackOffset1) == 4, "FTMotionEventSetAttackOffset1 must be 4 bytes");

struct FTMotionEventSetAttackOffset2
{
#if IS_BIG_ENDIAN
	s32 off_y : 16;
	s32 off_z : 16;
#else
	s32 off_z : 16;
	s32 off_y : 16;
#endif
};

struct FTMotionEventSetAttackOffset
{
	FTMotionEventSetAttackOffset1 s1;
	FTMotionEventSetAttackOffset2 s2;
};

struct FTMotionEventSetAttackCollDamage
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 attack_id: 3;
	u32 damage : 8;
	u32 pad : 15;
#else
	u32 pad : 15;   /* see FTMotionEventMakeAttack5 for rationale */
	u32 damage : 8;
	u32 attack_id: 3;
	u32 opcode : 6;
#endif
};
_Static_assert(sizeof(struct FTMotionEventSetAttackCollDamage) == 4, "FTMotionEventSetAttackCollDamage must be 4 bytes");

struct FTMotionEventSetAttackCollSize
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 attack_id: 3;
	u32 size : 16;
	u32 pad : 7;
#else
	u32 pad : 7;    /* see FTMotionEventMakeAttack5 for rationale */
	u32 size : 16;
	u32 attack_id: 3;
	u32 opcode : 6;
#endif
};
_Static_assert(sizeof(struct FTMotionEventSetAttackCollSize) == 4, "FTMotionEventSetAttackCollSize must be 4 bytes");

struct FTMotionEventSetAttackCollSound
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 attack_id: 3;
	u32 fgm_level : 3;
	u32 pad : 20;
#else
	u32 pad : 20;   /* see FTMotionEventMakeAttack5 for rationale */
	u32 fgm_level : 3;
	u32 attack_id: 3;
	u32 opcode : 6;
#endif
};
_Static_assert(sizeof(struct FTMotionEventSetAttackCollSound) == 4, "FTMotionEventSetAttackCollSound must be 4 bytes");

struct FTMotionEventSetThrow1
{
	u32 opcode : 6;
};

struct FTMotionEventSetThrow2
{
#ifdef PORT
	u32 throw_desc;     // Relocation token — use PORT_RESOLVE()
#else
	FTThrowHitDesc* throw_desc;
#endif
};

struct FTMotionEventSetThrow
{
	FTMotionEventSetThrow1 s1;
	FTMotionEventSetThrow2 s2;
};

struct FTMotionEventMakeEffect1
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	s32 joint_id : 7;
	u32 effect_id : 9;
	u32 flag : 10;
#else
	u32 flag : 10;
	u32 effect_id : 9;
	s32 joint_id : 7;
	u32 opcode : 6;
#endif
};

struct FTMotionEventMakeEffect2
{
#if IS_BIG_ENDIAN
	s32 off_x : 16;
	s32 off_y : 16;
#else
	s32 off_y : 16;
	s32 off_x : 16;
#endif
};

struct FTMotionEventMakeEffect3
{
#if IS_BIG_ENDIAN
	s32 off_z : 16;
	s32 rng_x : 16;
#else
	s32 rng_x : 16;
	s32 off_z : 16;
#endif
};

struct FTMotionEventMakeEffect4
{
#if IS_BIG_ENDIAN
	s32 rng_y : 16;
	s32 rng_z : 16;
#else
	s32 rng_z : 16;
	s32 rng_y : 16;
#endif
};

struct FTMotionEventMakeEffect
{
	FTMotionEventMakeEffect1 s1;
	FTMotionEventMakeEffect2 s2;
	FTMotionEventMakeEffect3 s3;
	FTMotionEventMakeEffect4 s4;
};

struct FTMotionEventSetHitStatusPartID
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	s32 joint_id : 7;
	u32 hitstatus : 19;
#else
	u32 hitstatus : 19;
	s32 joint_id : 7;
	u32 opcode : 6;
#endif
};

struct FTMotionEventSetDamageCollPartID1
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	s32 joint_id : 7;
	u32 pad : 19;
#else
	u32 pad : 19;   /* see FTMotionEventMakeAttack5 for rationale */
	s32 joint_id : 7;
	u32 opcode : 6;
#endif
};
_Static_assert(sizeof(struct FTMotionEventSetDamageCollPartID1) == 4, "FTMotionEventSetDamageCollPartID1 must be 4 bytes");

struct FTMotionEventSetDamageCollPartID2
{
#if IS_BIG_ENDIAN
	s32 off_x : 16;
	s32 off_y : 16;
#else
	s32 off_y : 16;
	s32 off_x : 16;
#endif
};

struct FTMotionEventSetDamageCollPartID3
{
#if IS_BIG_ENDIAN
	s32 off_z : 16;
	s32 size_x : 16;
#else
	s32 size_x : 16;
	s32 off_z : 16;
#endif
};

struct FTMotionEventSetDamageCollPartID4
{
#if IS_BIG_ENDIAN
	s32 size_y : 16;
	s32 size_z : 16;
#else
	s32 size_z : 16;
	s32 size_y : 16;
#endif
};

struct FTMotionEventSetDamageCollPartID
{
	FTMotionEventSetDamageCollPartID1 s1;
	FTMotionEventSetDamageCollPartID2 s2;
	FTMotionEventSetDamageCollPartID3 s3;
	FTMotionEventSetDamageCollPartID4 s4;
};

struct FTMotionEventSubroutine1
{
	u32 opcode : 6;
};

struct FTMotionEventSubroutine2
{
#ifdef PORT
	u32 p_goto;         // Relocation token — use PORT_RESOLVE()
#else
	void* p_goto;
#endif
};

struct FTMotionEventSubroutine
{
	FTMotionEventSubroutine1 s1;
	FTMotionEventSubroutine2 s2;
};

struct FTMotionEventSetDamageThrown1
{
	u32 opcode : 6;
};

struct FTMotionEventSetDamageThrown2
{
#ifdef PORT
	u32 p_subroutine;   // Relocation token — use PORT_RESOLVE()
#else
	void* p_subroutine;
#endif
};

struct FTMotionDamageScript
{
#ifdef PORT
	u32 p_script[2][nFTKindEnumCount];   // Relocation tokens — use PORT_RESOLVE()
#else
	void* p_script[2][nFTKindEnumCount];
#endif
};

struct FTMotionEventSetDamageThrown
{
	FTMotionEventSetDamageThrown1 s1;
	FTMotionEventSetDamageThrown2 s2;
};

struct FTMotionEventGoto1
{
	u32 opcode : 6;
};

struct FTMotionEventGoto2
{
#ifdef PORT
	u32 p_goto;         // Relocation token — use PORT_RESOLVE()
#else
	void* p_goto;
#endif
};

struct FTMotionEventGoto
{
	FTMotionEventGoto1 s1;
	FTMotionEventGoto2 s2;
};

struct FTMotionEventParallel1
{
	u32 opcode : 6;
};

struct FTMotionEventParallel2
{
#ifdef PORT
	u32 p_goto;         // Relocation token — use PORT_RESOLVE()
#else
	void* p_goto;
#endif
};

struct FTMotionEventParallel
{
	FTMotionEventParallel1 s1;
	FTMotionEventParallel2 s2;
};

struct FTMotionEventSetModelPartID
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	s32 joint_id : 7;
	s32 modelpart_id : 19;
#else
	s32 modelpart_id : 19;
	s32 joint_id : 7;
	u32 opcode : 6;
#endif
};

struct FTMotionEventSetTexturePartID
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 texturepart_id : 6;
	u32 frame : 20;
#else
	u32 frame : 20;
	u32 texturepart_id : 6;
	u32 opcode : 6;
#endif
};

struct FTMotionEventSetColAnimID
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 colanim_id : 8;
	u32 length : 18;
#else
	u32 length : 18;
	u32 colanim_id : 8;
	u32 opcode : 6;
#endif
};

struct FTMotionEventSetSlopeContour
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 pad : 23;
	u32 flags : 3;
#else
	u32 flags : 3;
	u32 pad : 23;
	u32 opcode : 6;
#endif
};

struct FTMotionEventSetAfterImage
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 is_itemswing : 8;
	s32 drawstatus : 18;
#else
	s32 drawstatus : 18;
	u32 is_itemswing : 8;
	u32 opcode : 6;
#endif
};

struct FTMotionEventMakeRumble
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 length : 13;
	u32 rumble_id : 13;
#else
	u32 rumble_id : 13;
	u32 length : 13;
	u32 opcode : 6;
#endif
};

struct FTMotionEventStopRumble
{
#if IS_BIG_ENDIAN
	u32 opcode : 6;
	u32 rumble_id : 26;
#else
	u32 rumble_id : 26;
	u32 opcode : 6;
#endif
};

struct FTStatusDesc
{
    FTMotionFlags mflags;
    GMStatFlags sflags;

    void (*proc_update)(GObj*);
    void (*proc_interrupt)(GObj*);
    void (*proc_physics)(GObj*);
    void (*proc_map)(GObj*);
};

/*
 * Character headers need FTStatusDesc / FTMotionDesc to be complete for their
 * `extern FTStatusDesc dFTMarioSpecialStatusDescs[]` style declarations; they
 * also provide the fighter-specific *StatusVars types used in the union below.
 */
#include <ft/ftchar.h>

struct FTOpeningDesc
{
    s32 motion_id;
    void (*proc_update)(GObj*);
};

struct FTThrowHitDesc
{
    s32 status_id;
    s32 damage;
    s32 angle;
    s32 knockback_scale;
    s32 knockback_weight;
    s32 knockback_base;
    s32 element;
};

struct FTThrowReleaseDesc
{
    s32 angle;
    s32 knockback_scale;
    s32 knockback_weight;
    s32 knockback_base;
};

struct FTDesc
{
    s32 fkind;
    Vec3f pos;
    s32 lr;
    u8 team;
    u8 player;
    u8 detail;
    u8 costume;
    u8 shade;
    u8 handicap;
    u8 level;
    u8 stock_count;
    u8 unk_rebirth_0x1C;
    u8 unk_rebirth_0x1D;
    u8 team_order;                  // Order number of fighter if member of "VS *character* Team" 
    ub32 is_skip_entry : 1;         // If TRUE, fighter gets spawned in Wait or Fall state, otherwise use entry state
    ub32 is_skip_shadow_setup : 1;  // If TRUE, fighter's shadow is not initialized
    ub32 is_magnify_ignore : 1;       // If TRUE, fighter's magnifying glass is not rendered?
    s32 copy_kind;                  // Kirby's copy ID on spawn
    s32 damage;
    s32 pkind;
    SYController *controller;       // Pointer to player's controller input struct
    u16 button_mask_a;
    u16 button_mask_b;
    u16 button_mask_z;
    u16 button_mask_l;
    void *figatree_heap;                // Pointer to animation bank?
    void *proc_display;
};

struct FTAttackMatrix
{
    sb32 unk_fthitmtx_0x0;
    Mtx44f mtx;
    f32 unk_fthitmtx_0x44;
};

struct FTAttackColl
{
    s32 attack_state;
    u32 group_id;
    s32 joint_id;
    s32 damage;
    s32 element;
    DObj *joint;
    Vec3f offset;
    f32 size;
    s32 angle;
    s32 knockback_scale;
    s32 knockback_weight;
    s32 knockback_base;
    s32 shield_damage;
    u32 fgm_level : 3;
    u32 fgm_kind : 4;
    ub32 is_hit_air : 1;
    ub32 is_hit_ground : 1;
    ub32 can_rebound : 1;
    ub32 is_scale_pos : 1;
    u32 motion_attack_id : 6;
    u16 motion_count;
    u16 stat_count;
    Vec3f pos_curr;
    Vec3f pos_prev;
    GMAttackRecord attack_records[GMATTACKREC_NUM_MAX];
    FTAttackMatrix attack_matrix;
};

struct FTDamageCollDesc
{
    s32 joint_id;
    s32 placement;
    sb32 is_grabbable;
    Vec3f offset;
    Vec3f size;
};

struct FTDamageColl
{
    s32 hitstatus;
    s32 joint_id;
    DObj *joint;
    s32 placement;              // 0 = low, 1 = middle, 2 = high
    sb32 is_grabbable;
    Vec3f offset;
    Vec3f size;
};

struct FTHitLog
{
    s32 attacker_object_class;
    void *attack_coll;
    s32 attack_id;
    GObj *attacker_gobj;
    FTDamageColl *damage_coll; // Victim fighter's hurtbox
    u8 attacker_player;
    s32 attacker_player_num;
};

struct FTItemThrow
{
    sb32 is_smash_throw : 1;
    s32 vel_scale : 10;
    s32 angle : 11;
    u32 damage_scale : 10;
};

struct FTItemSwing
{
    u32 anim_speed : 10;
};

struct FTParts
{
    s32 transform_update_mode;              // Update mode of DObj's transformations?
    // 0 = free transformation updates
    // 1 = lock transformation updates
    // 2 = ???
    // 3 = ???
    union
    {
        struct
        {
            u8 unk_dobjtrans_0x4;
            u8 unk_dobjtrans_0x5;
            u8 unk_dobjtrans_0x6;
            u8 unk_dobjtrans_0x7;
        };
        s32 unk_dobjtrans_word;
    };

    FTParts *next;
    u8 flags;
    u8 joint_id;
    ub8 is_have_anim;
    u8 unk_dobjtrans_0xF;
    Mtx44f unk_dobjtrans_0x10;
    Mtx44f mtx_translate;
    Vec3f vec_scale; // ???
    Mtx44f unk_dobjtrans_0x9C;
    GObj *gobj;
};

struct FTHiddenPart
{
    s32 root_joint_id;
    s32 parent_joint_id;
    s32 partindex_0x8;
    s32 joint_kind;
};

// Skeleton joints for electric shock effects?
struct FTSkeleton
{
    union
    {
#ifdef PORT
        u32 dl;        // Relocation token
        u32 dls;       // Relocation token for a 2-entry display-list array
#else
        Gfx *dl;        // Single array of display lists
        Gfx **dls;      // Pointer to two arrays of display lists
#endif
    };
#if IS_BIG_ENDIAN
    u8 flags;
#else
    u8 pad[3];
    u8 flags;
#endif
};

#ifdef PORT
#define FTSKELETON_GET_DL(skeleton) ((Gfx*)PORT_RESOLVE((skeleton)->dl))
#define FTSKELETON_GET_DLS(skeleton) (PORT_RESOLVE((skeleton)->dls))
_Static_assert(sizeof(FTSkeleton) == 8, "FTSkeleton must be 8 bytes to match file data layout");
#else
#define FTSKELETON_GET_DL(skeleton) ((skeleton)->dl)
#define FTSKELETON_GET_DLS(skeleton) ((skeleton)->dls)
#endif

struct FTShadow
{
    Vtx vtx1[8];
    Vtx vtx2[8];
    s32 player;
    s32 unused;
};

struct FTCostume
{
    u8 royal[4];
    u8 team[3];
    u8 develop;
};

struct FTDemoDesc
{
    s32 fkind;
    s32 costume;
    s32 shade;
};

struct FTAfterImage
{
    s16 translate_x;
    s16 translate_y;
    s16 translate_z;
    Vec3f vec;
};

struct FTCamera
{
    FTStruct *target_fp;
    Vec3f target_pos;
    f32 unk_ftcobj_0x10;
};

struct FTSprites
{
#ifdef PORT
    u32 stock_sprite;   // Relocation token — use PORT_RESOLVE()
    u32 stock_luts;     // Relocation token — resolves to array of u32 LUT tokens
    u32 emblem;         // Relocation token
#else
    Sprite *stock_sprite;
    int **stock_luts;
    Sprite *emblem;
#endif
};

#ifdef PORT
_Static_assert(sizeof(FTSprites) == 12, "FTSprites must be 12 bytes to match file data layout");
#endif

struct FTComputer
{
    u8 objective;                           // CPU player's current objective
    u8 objective_base;                      // CPU player's default objective
    u8 input_kind;                          // Stick + Button combination CPU player is set to execute
    u8 behavior;                            // CPU player's general behavior
    u8 unk_ftcom_0x4;                       // Unused?
    u8 trait;                               // CPU player will determine behavior based on this
    u8 unk_ftcom_0x6;                       // Unused?
    u8 input_wait;                          // "Controller command wait timer"
    u8 *p_command;                          // Controller input commands
    sb32 (*proc_com)(GObj*);                // "Main behavior routine"
    u16 jump_wait;                          // CPU player will wait this many frames before jumping on "jump" behavior?
    u16 item_track_wait;                    // CPU player will wait this many frames before targeting an item?
    u16 behavior_change_wait;               // CPU player will wait this many frames before changing behavior?
    u16 unk_ftcom_0x16;                     // Unused?
    u16 walk_stop_wait;                     // CPU player will wait this many frames before stopping after walking for 2.5 seconds?
    u16 fighter_follow_since;               // CPU player has followed its opponent for this many frames?
    u16 fighter_follow_wait;                // CPU player will wait this many frames before following target?
    u16 fighter_follow_end;                 // CPU player will stop following target after this many frames?
    u16 unk_ftcom_0x20;                     // ???
    u16 target_find_wait;
    u16 wiggle_wait;                        // CPU player will wait this many frames before finding a new target?
    u16 target_damage_percent;              // ???
    u16 attack_count;                       // Number of times CPU player successfully dealt damage?
    u16 appeal_attempt_frames;              // CPU player will attempt to taunt while this is not 0
    u16 stand_stop_wait;                    // CPU player will wait this many frames before breaking out of idle behavior
    GObj *target_gobj;                      // CPU player's target's GObj
    u8 item_throw_wait;                     // CPU player will wait this many frames before throwing held item?
    u8 unk_ftcom_0x35;
    u8 unk_ftcom_0x36;                      // Unused?
    u8 input_repeat_count;                  // Number of times CPU player has repeated the same input combination?
    u8 unk_ftcom_0x38;
    u8 stickn_button_a_count;               // Jab
    u8 sticktilts_button_a_count;           // Forward Tilt
    u8 sticksmashs_button_a_count;          // Forward Smash
    u8 sticktilthi_button_a_count;          // Up Tilt
    u8 sticksmashhi_button_a_count;         // Up Tilt / Up Aerial / Down Tilt / Down Aerial?
    u8 sticktiltlw_button_a_count;          // Down Tilt
    u8 sticksmashlw_button_a_count;         // Down Smash / Down Aerial
    u8 sticksmashs_button_b_count;          // Neutral Special
    u8 sticksmashhi_button_b_count;         // Up Special
    u8 sticksmashlw_button_b_count;         // Down Special
    u8 stickn_button_z_button_a_count;      // Grab
    ub32 unk_ftcom_0x44;                    // Unused?
    ub32 ftcom_flags_0x48_b0 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b1 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b2 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b3 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b4 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b5 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b6 : 1;           // Unused?
    ub32 ftcom_flags_0x48_b7 : 1;           // Unused?
    ub32 is_within_vertical_bounds : 1;     // ???
    ub32 ftcom_flags_0x49_b1 : 1;
    ub32 ftcom_flags_0x49_b2 : 1;           // Never set but compared against to check if CPU player should act appropriately VS fighters with reflector/absorb moves?
    ub32 ftcom_flags_0x49_b3 : 1;
    ub32 is_counterattack : 1;              // Whether CPU player will act just before landing from DamageFall?
    ub32 is_shield_item_weapon : 1;         // CPU player will attempt to shield if it detects an incoming weapon or item
    ub32 is_opponent_ra : 1;                // Whether CPU player's opponent can reflect or absorb?
    ub32 is_attempt_specialhi_recovery : 1; // Whether CPU will attempt to recover with Up Speciial
    ub32 ftcom_flags_0x4A_b0 : 1;
    ub32 ftcom_flags_0x4A_b1 : 1;
    ub32 is_stop_stand : 1;                 // Whether CPU player should leave standing / idle state
    Vec2f cliff_left_pos;                   // Nearest left ledge?
    Vec2f cliff_right_pos;                  // Nearest right ledge?
    s32 target_line_id;                     // CPU's target's line ID
    Vec2f target_pos;                       // CPU's target's position
    f32 target_dist;                        // FLOAT_MAX when offstage
    void *target_user;                      // FTStruct* most of the time, but can be ITStruct* too when looking for nearest item
    Vec2f origin_pos;                       // CPU player's TopN position at creation?
    Vec2f edge_pos;                         // CPU player's patrol range? (ends at edges of floor_line_id?)
    Vec2f stand_pos;                        // ??? Is this where the CPU player is supposed to stand when idling?
    s32 floor_line_id;                      // Some kind of collision line ID
    f32 dash_predict;                       // CPU player uses this to predict when it's appropriate to dash / run?
    f32 jump_predict;                       // CPU player uses this to predict what jump height to go for?
};

struct FTComputerAttack
{
    s32 input_kind;
    s32 hit_start_frame;
    s32 hit_end_frame;
    f32 detect_near_x;
    f32 detect_far_x;
    f32 detect_near_y;
    f32 detect_far_y;
};

struct FTPlayerInput
{
    u16 button_hold;        // Held buttons
    u16 button_tap;         // Newly pressed buttons
    u16 button_release;     // Newly released buttons
    Vec2b stick_range;      // Current stick range
    Vec2b stick_prev;       // Previous stick range
};

struct FTComputerInput
{
    u16 button_inputs;      // All buttons
    Vec2b stick_range;      // Computer player stick range
};

union FTKeyEvent
{
    u16 halfword;

#if IS_BIG_ENDIAN
    struct
    {
        u16 opcode : 4;
        u16 param : 12;
    } command;
#else
    struct
    {
        u16 param : 12;
        u16 opcode : 4;
    } command;
#endif

    Vec2b stick_range;
};

struct FTKey
{
    s32 input_wait;
    FTKeyEvent *script;
};

struct FTAttributes
{
    f32 size;                                       // Model size
    f32 walkslow_anim_length;                       // Length of slow walk animation
    f32 walkmiddle_anim_length;                     // Length of medium walk animation
    f32 walkfast_anim_length;                       // Length of fast walk animation
    f32 throw_walkslow_anim_length;                 // Length of cargo slow walk animation
    f32 throw_walkmiddle_anim_length;               // Length of cargo medium walk animation
    f32 throw_walkfast_anim_length;                 // Length of cargo fast walk animation
    f32 rebound_anim_length;                        // Length of rebound animation
    f32 walk_speed_mul;
    f32 traction;
    f32 dash_speed;
    f32 dash_decel;
    f32 run_speed;
    f32 kneebend_anim_length;                       // Jump-squat frames
    f32 jump_vel_x;
    f32 jump_height_mul;
    f32 jump_height_base;
    f32 jumpaerial_vel_x;
    f32 jumpaerial_height;
    f32 air_accel;
    f32 air_speed_max_x;
    f32 air_friction;
    f32 gravity;
    f32 tvel_base;                                  // Normal terminal velocity
    f32 tvel_fast;                                  // Fast-fall terminal velocity
    s32 jumps_max;                                  // Number of jumps
    f32 weight;                                     // Or, rather, "knockback multiplier"
    f32 attack1_followup_frames;                    // Jab combo connection frames
    f32 dash_to_run;                                // Frames before dash transitions to run?
    f32 shield_size;                                // Size of fighter's shield (does not affect shield health)
    f32 shield_break_vel_y;                         // Y-Velocity of fighter's shield break
    f32 shadow_size;                                // Size of fighter's shadow
    f32 jostle_width;                               // ???
    f32 jostle_x;                                   // ???
    sb32 is_metallic;                               // Used to determine whether the character makes blue sparks or gray metal dust particles when hit
    f32 cam_offset_y;
    f32 closeup_camera_zoom;
    f32 camera_zoom;
    f32 camera_zoom_base;
    MPObjectColl map_coll;                          // Map collision diamond dimensions
    Vec2f cliffcatch_coll;                          // Ledge grab box dimensions
    u16 dead_fgm_ids[2];                            // KO voices
    u16 deadup_sfx;                                 // Star-KO voice
    u16 damage_sfx;
    u16 smash_sfx[3];                               // Random Smash SFX
    FTItemPickup item_pickup;
    u16 itemthrow_vel_scale;                        // Scale of thrown item velocity in %
    u16 itemthrow_damage_scale;                     // Scale of thrown item damage in %
    u16 heavyget_sfx;
    f32 halo_size;                                  // Respawn platform size?
    SYColorRGBA shade_color[3];
    SYColorRGBA fog_color;
#if IS_BIG_ENDIAN
    ub32 is_have_attack11    : 1;
    ub32 is_have_attack12    : 1;
    ub32 is_have_attackdash  : 1;
    ub32 is_have_attacks3    : 1;
    ub32 is_have_attackhi3   : 1;
    ub32 is_have_attacklw3   : 1;
    ub32 is_have_attacks4    : 1;
    ub32 is_have_attackhi4   : 1;
    ub32 is_have_attacklw4   : 1;
    ub32 is_have_attackairn  : 1;
    ub32 is_have_attackairf  : 1;
    ub32 is_have_attackairb  : 1;
    ub32 is_have_attackairhi : 1;
    ub32 is_have_attackairlw : 1;
    ub32 is_have_specialn    : 1;
    ub32 is_have_specialairn : 1;
    ub32 is_have_specialhi   : 1;
    ub32 is_have_specialairhi: 1;
    ub32 is_have_speciallw   : 1;
    ub32 is_have_specialairlw: 1;
    ub32 is_have_catch       : 1;                   // Whether fighter has a grab
    ub32 is_have_voice       : 1;                   // Whether fighter can play FGMs marked "voice"
#else
    // LE: LSB-first bitfield allocation — reverse field order and add
    // explicit padding so each field maps to the same bit position as on BE
    // (the u32 value is identical after bswap32).
    ub32 : 10;                                      // bits 0-9 (padding — matches BE trailing bits)
    ub32 is_have_voice       : 1;                   // Whether fighter can play FGMs marked "voice"
    ub32 is_have_catch       : 1;                   // Whether fighter has a grab
    ub32 is_have_specialairlw: 1;
    ub32 is_have_speciallw   : 1;
    ub32 is_have_specialairhi: 1;
    ub32 is_have_specialhi   : 1;
    ub32 is_have_specialairn : 1;
    ub32 is_have_specialn    : 1;
    ub32 is_have_attackairlw : 1;
    ub32 is_have_attackairhi : 1;
    ub32 is_have_attackairb  : 1;
    ub32 is_have_attackairf  : 1;
    ub32 is_have_attackairn  : 1;
    ub32 is_have_attacklw4   : 1;
    ub32 is_have_attackhi4   : 1;
    ub32 is_have_attacks4    : 1;
    ub32 is_have_attacklw3   : 1;
    ub32 is_have_attackhi3   : 1;
    ub32 is_have_attacks3    : 1;
    ub32 is_have_attackdash  : 1;
    ub32 is_have_attack12    : 1;
    ub32 is_have_attack11    : 1;
#endif
    FTDamageCollDesc damage_coll_descs[11];         // Hurtbox array setup
    Vec3f hit_detect_range;                         // This is a radius around the fighter within which hitbox detection can occur
#ifdef PORT
    u32 setup_parts;                                // Relocation token — use PORT_RESOLVE()
    u32 animlock;                                   // Relocation token
#else
    u32 *setup_parts;                               // Pointer to two sets of flags marking joints that should be initialized on fighter creation
    u32 *animlock;                                  // Pointer to two sets of flags marking joints that should not be animated; ignores joints 0 through 3
#endif
    s32 effect_joint_ids[5];                        // The game will cycle through these joints when applying certain particles such as electricity and flames
    sb32 cliff_status_ga[5];                        // Bool for whether fighter is grounded or airborne during each cliff state
    s32 unused_0x2CC;                               // ???
#ifdef PORT
    u32 hiddenparts;                                // Relocation token
    u32 commonparts_container;                      // Relocation token
    u32 dobj_lookup;                                // Relocation token
    u32 shield_anim_joints[8];                      // Relocation tokens — one for each ordinal direction
#else
    FTHiddenPart *hiddenparts;                      // Hidden fighter body parts?
    FTCommonPartContainer *commonparts_container;   // Base fighter body parts
    DObjDesc *dobj_lookup;                          // I don't actually know how this works at the moment
    AObjEvent32 **shield_anim_joints[8];            // One for each ordinal direction
#endif
    s32 joint_rfoot_id;                             // Right foot joint
    f32 joint_rfoot_rotate;                         // Amount of bend applied to right foot on slope contour?
    s32 joint_lfoot_id;                             // Left foot joint
    f32 joint_lfoot_rotate;                         // Amount of bend applied to left foot on slope contour?
    u8 filler_0x30C[0x31C - 0x30C];
    f32 unk_0x31C;
    f32 unk_0x320;
#ifdef PORT
    u32 translate_scales;                           // Relocation token
    u32 modelparts_container;                       // Relocation token
    u32 accesspart;                                 // Relocation token
    u32 textureparts_container;                     // Relocation token
#else
    Vec3f *translate_scales;                        // A set of scaling vectors to modify the translation vector of a given joint?
    FTModelPartContainer *modelparts_container;     // Passive model parts controlled via motion events or code
    FTAccessPart *accesspart;                       // Headgear accessory (Pikachu wizard hat or Jigglypuff bow)
    FTTexturePartContainer *textureparts_container; // These are generally facial expressions, controlled via motion events
#endif
    s32 joint_itemheavy_id;                         // Joint for holding heavy items
#ifdef PORT
    u32 thrown_status;                              // Relocation token
#else
    FTThrownStatusArray *thrown_status;             // Array of thrown status IDs (forward- and back throw) to use for thrown fighters
#endif
    s32 joint_itemlight_id;                         // Joint for holding light items
#ifdef PORT
    u32 sprites;                                    // Relocation token
    u32 skeleton;                                   // Relocation token
#else
    FTSprites *sprites;                             // Stock sprites, stock palettes and emblem sprites
    FTSkeleton **skeleton;                          // Electric damage skeleton model data
#endif
};

#ifdef PORT
_Static_assert(sizeof(FTAttributes) == 0x348, "FTAttributes must be 0x348 bytes to match file data layout");
_Static_assert(offsetof(FTAttributes, dead_fgm_ids) == 0xB4, "dead_fgm_ids offset mismatch");
_Static_assert(offsetof(FTAttributes, deadup_sfx) == 0xB8, "deadup_sfx offset mismatch");
_Static_assert(offsetof(FTAttributes, smash_sfx) == 0xBC, "smash_sfx offset mismatch");
_Static_assert(offsetof(FTAttributes, itemthrow_vel_scale) == 0xE4, "itemthrow_vel_scale offset mismatch");
_Static_assert(offsetof(FTAttributes, heavyget_sfx) == 0xE8, "heavyget_sfx offset mismatch");
_Static_assert(offsetof(FTAttributes, shade_color) == 0xF0, "shade_color offset mismatch");
_Static_assert(offsetof(FTAttributes, fog_color) == 0xFC, "fog_color offset mismatch");
_Static_assert(offsetof(FTAttributes, unused_0x2CC) == 0x2CC, "unused_0x2CC offset mismatch");
_Static_assert(offsetof(FTAttributes, filler_0x30C) == 0x30C, "filler_0x30C offset mismatch");
#endif

// Main fighter struct
struct FTStruct
{
    FTStruct *next;                     // Next free FTStruct
    GObj *fighter_gobj;                 // Fighter's GObj
    s32 fkind;                          // Fighter ID
    u8 team;                            // Fighter's team
    u8 player;                          // Controller port assigned to this fighter
    u8 detail_curr;                     // Hi-Poly = 1, Low-Poly = 2
    u8 detail_base;                     // Hi-Poly = 1, Low-Poly = 2
    u8 costume;                         // Costume ID
    u8 shade;                           // i.e. When multiple instances of the same character costume are in-game
    u8 handicap;                        // Handicap value
    u8 level;                           // CPU player difficulty
    s8 stock_count;                     // Remaining stocks, runs out at -1 for some reason
    u8 team_order;                      // Order number if this fighter is a "VS *character* Team" member; used to check for bonuses such as Yoshi Rainbow
    u8 dl_link;                         // Rendering layer in which the fighter is drawn
    s32 player_num;                     // Player's number in the order they were created

    u32 status_total_tics;              // Tics spent in this action state

    s32 pkind;                          // Control type: 0 = HMN, 1 = CPU, 2 = N/A

    s32 status_id;                      // ID of current status
    s32 motion_id;                      // ID of motion script to use

    s32 percent_damage;                 // Fighter's damage
    s32 damage_resist;                  // Resits a specific amount of % damage before breaking, effectively damage-based armor
    s32 shield_health;                  // Current shield health
    f32 unk_ft_0x38;                    // Initialized to 0.0F, but never used
    s32 unk_ft_0x3C;                    // Initialized to 0, but never used
    u32 hitlag_tics;                    // Remaining hitlag tics
    s32 lr;                             // Facing direction; -1 = left, 0 = center, 1 = right

    struct FTPhysics
    {
        Vec3f vel_air;                  // Aerial self-induced velocity
        Vec3f vel_damage_air;           // Aerial knockback velocity
        Vec3f vel_ground;               // Grounded self-induced velocity
        f32 vel_damage_ground;          // Grounded knockback velocity
        f32 vel_jostle_x;               // X-Velocity from being pushed by another fighter
        f32 vel_jostle_z;               // Z-Velocity from being pushed by another fighter

    } physics;

    MPCollData coll_data;               // Collision data

    u8 jumps_used;                      // Jumps used out of maximum available jumps
    u8 unk_ft_0x149;                    // ???
    sb32 ga;                            // Ground / Air bool

    f32 attack1_followup_frames;        // Frames of input window to get the next jab combo
    s32 attack1_status_id;              // Current jab status ID
    s32 attack1_input_count;            // Number of A presses required for rapid jab; 1-frame A presses count as 2 inputs
    s32 cliffcatch_wait;                // Wait this many frames before fighter can grab a ledge again
    s32 tics_since_last_z;              // Frames since last Z-press, resets to 65536 on action state change
    s32 acid_wait;                      // Wait this many frames before fighter can be hurt by Planet Zebes acid again
    s32 twister_wait;                   // Wait this many frames before fighter can be picked up by the Hyrule Tornado again
    s32 tarucann_wait;                  // Wait this many frames before fighter can enter Barrel Cannon again
    s32 damagefloor_wait;               // Wait this many frames before fighter can be hurt by damaging floors again (e.g. Mario's Board the Platforms stage)
    s32 playertag_wait;                 // Wait this many frames before fighter's player indicator is shown again; tag is shown when this reaches 1 or is overridden by position on stage

    s32 card_anim_frame_id;             // Index of fighter's role on 1P Stage Card scene? (e.g. player character, opponent, ally etc.)

    union FTCommandVars
    {
        // 
        struct FTCommandFlags
        {
            u32 flag0;
            u32 flag1;
            u32 flag2;
            u32 flag3;

        } flags;

        struct FTItemThrowFlags
        {
            sb32 is_throw_item;
            u8 unk1;
            u32 damage : 24;
            u8 unk2;
            u32 vel : 12;
            s32 angle : 12;

        } item_throw;

    } motion_vars;

    ub32 is_attack_active : 1;          // Whether fighter has active hitboxes
    ub32 is_hitstatus_nodamage : 1;     // Whether fighter's hurtbox state is impervious
    ub32 is_damage_coll_modify : 1;     // Whether fighter is using custom temporary hurtboxes
    ub32 is_modelpart_modify : 1;       // Whether fighter is using a specific non-default model part
    ub32 is_texturepart_modify : 1;     // Whether fighter is using a specific non-default face texture
    ub32 is_reflect : 1;                // Whether fighter's reflection collision sphere is active
    s32 reflect_lr : 2;                 // Direction of incoming hitbox subject to reflection
    ub32 is_absorb : 1;                 // Whether fighter's absorption collision sphere is active
    s32 absorb_lr : 2;                  // Direction of incoming hitbox subject to absorption
    ub32 is_goto_attack100 : 1;         // Whether rapid jab follow-up is confirmed
    ub32 is_fastfall : 1;               // Whether fighter is fast-falling
    ub32 is_magnify_show : 1;           // Whether fighter's magnifying glass is visible
    ub32 is_limit_map_bounds : 1;       // Effectively keeps the player at the blast zone and ignores the dead action state if TRUE
    ub32 is_invisible : 1;              // Whether fighter is invisible
    ub32 is_shadow_hide : 1;            // Whether fighter's shadow should be hidden
    ub32 is_rebirth : 1;                // Whether fighter is in any of the revive states after getting KO'd
    ub32 is_magnify_ignore : 1;         // Skip rendering magnifying glass if TRUE?
    ub32 is_playertag_hide : 1;         // Skip rendering player indicator if TRUE
    ub32 is_playertag_bossend : 1;      // Also skips rendering player indicator? Used only in "Master Hand defeated" cinematic from what I can tell so far
    ub32 is_effect_skip : 1;            // Toggled ON when fighter enters a pipe on Mushroom Kingdom; prevents particle script events from being executed
    u32 effect_joint_array_id : 4;      // Goes up to 5 by default; index of the array from effect_joint_ids from FTAttributes which houses the actual joint ID
    ub32 is_shield : 1;                 // Whether fighter's shield collision sphere is active
    ub32 is_effect_attach : 1;          // Whether fighter has an effect attached to one of their joints
    ub32 is_jostle_ignore : 1;          // Whether fighter is immune to jostling
    ub32 is_have_translate_scale : 1;   // Whether fighter is using translation vector transformation scales for animations
    ub32 is_control_disable : 1;        // Fighter cannot be controlled if TRUE; enabled when training mode menu is up
    ub32 is_hitstun : 1;                // Whether fighter is in hitstun
    u32 slope_contour : 3;              // "Slope Contour" behavior flags: 0x1 = left foot, 0x2 = right foot, 0x4 = full body rotation relative to slanted surfaces
    ub32 is_use_animlocks : 1;          // If TRUE, fighter will ignore animation of specified joints
    ub32 is_muted : 1;                  // This is set to TRUE only in the Characters menu; prevents motion script sound effects from playing
    ub32 unk_ft_0x190_b5 : 1;           // Unused
    ub32 is_item_show : 1;              // Whether fighter's item is visible
    ub32 is_cliff_hold : 1;             // Whether fighter is holding onto a ledge
    ub32 is_events_forward : 1;         // Is this flag's sole purpose to fast-forward GFX in the moveset event parser?
    ub32 is_ghost : 1;                  // If TRUE, fighter does not check for hit collisions and cannot be KO'd
    ub32 is_damage_resist : 1;          // Whether fighter's damage-based armor is active
    ub32 is_menu_ignore : 1;            // Fighter cannot bring up training or pause menu if TRUE
    u32 camera_mode : 4;                // Camera behavior for this fighter
    ub32 is_special_interrupt : 1;      // Whether move can be interrupted by Link's boomerang? Have not seen this used anywhere else
    ub32 is_ignore_dead : 1;            // Ignore dead action states altogether
    ub32 is_catchstatus : 1;            // Whether fighter's current state is marked as a grab
    ub32 is_catch_or_capture : 1;       // My brain stops working everytime I try to understand what this does
    ub32 is_use_fogcolor : 1;           // Whether fighter is using fog-based color overlay
    ub32 is_shield_catch : 1;           // Set to TRUE when fighter grabs after getting shield poked; there is a check for this flag that halves throw damage if TRUE
    ub32 is_knockback_paused : 1;       // Whether fighter's knockback is paused until hitlag is over?

    u8 capture_immune_mask;             // Fighter is immune to these grab types
    u8 catch_mask;                      // Fighter's current grab type

    FTAnimDesc anim_desc;               // Fighter's animation flags
    Vec3f anim_vel;                     // Fighter's animation-induced velocity

    Vec2f magnify_pos;                  // Fighter's magnifying glass position

    struct FTInputStruct
    {
        SYController *controller;       // Controller inputs
        u16 button_mask_a;              // Button mapped to N64 A-Button
        u16 button_mask_b;              // Button mapped to N64 B-Button
        u16 button_mask_z;              // Button mapped to N64 Z-Trigger
        u16 button_mask_l;              // Button mapped to N64 L-Trigger
        FTPlayerInput pl;               // Human player inputs
        FTComputerInput cp;             // Computer player inputs

    } input;

    FTComputer computer;                // Computer player struct

    Vec2f damage_coll_size;             // Width and height of fighter's hurtbox; calculated from distance of TopN position to farthest hurtbox multiplied by 0.55

    u8 tap_stick_x;                     // Frames control stick has been tapped
    u8 tap_stick_y;                     // Frames control stick has been tapped
    u8 hold_stick_x;                    // Frames control stick has been tapped or held
    u8 hold_stick_y;                    // Frames control stick has been tapped or held

    s32 breakout_wait;                  // Frames until fighter breaks out of shield break / sleep / Cargo Throw
    s8 breakout_lr;                     // Whether victim is mashing left or right
    s8 breakout_ud;                     // Whether victim is mashing up or down

    u8 shuffle_frame_index;             // Ranges from 0-3; position of fighter's model vibration is adjusted based on this index when receiving hitlag
    u8 shuffle_index_max;               // How many iterations the frame index increments before looping back to 0;
    ub8 is_shuffle_electric;            // Fighter vibrates horizontally rather than vertically if hit by an electric attack
    u16 shuffle_tics;                   // Model shift timer

    GObj *throw_gobj;                   // GObj of opponent that threw this fighter
    s32 throw_fkind;                    // Kind of opponent that threw this fighter
    u8 throw_team;                      // Team of opponent that threw this fighter
    u8 throw_player;                    // Port of opponent that threw this fighter
    s32 throw_player_num;               // Player number of opponent that threw this fighter

    u32 motion_attack_id;               // Used to group status_ids for the stale queue
    u16 motion_count;                   // Multihits do not stale if motion_attack_id and motion_count are equal to the previous staling queue entry
    GMStatFlags stat_flags;             // Move statistics for determining what 1P Game bonus stats are awarded
    u16 stat_count;                     // Used to keep track of attacks for 1P Game bonus stats

    FTAttackColl attack_colls[4];       // Fighter's hitboxes

    s32 invincible_tics;                // Remaining tics of invincibility
    s32 intangible_tics;                // Remaining tics of intangibility
    s32 special_hitstatus;              // Type of custom, timed hit collision state
    s32 star_invincible_tics;           // Remaining tics of Starman invincibility
    s32 star_hitstatus;                 // Type of Starman hit collision state
    s32 hitstatus;                      // Type of passive hit collision state

    FTDamageColl damage_colls[11];

    f32 unk_ft_0x7A0;                   // Unused
    f32 hitlag_mul;                     // Hitlag multiplier; 1.5x for both attacker and defender if the connecting hitbox's element is electric
    f32 shield_heal_wait;               // How long before fighter's shield health recovers by 1 unit
    s32 unk_ft_0x7AC;                   // Unused

    s32 attack_damage;
    f32 attack_knockback;
    u16 attack_count;                   // Number of times this fighter successfully dealt damage 
    s32 attack_shield_push;             // Used to calculate shield/rebound pushback
    f32 attack_rebound;                 // Actually 2x staled damage?
    s32 hit_lr;
    s32 shield_damage;
    s32 shield_damage_total;            // shield_damage + hitbox damage + hitbox shield damage, does not persist?
    s32 shield_lr;                      // Direction in which shield knockback is set to occur
    s32 shield_player;                  // Port of player hitting this fighter's shield
    s32 reflect_damage;
    s32 damage_lag;                     // Used to calculate hitlag?
    f32 damage_knockback;
    f32 knockback_resist_passive;       // Passive armor, always active (?)
    f32 knockback_resist_status;        // Resist this many units of knockback, effectively knockback-based armor
    f32 damage_knockback_stack;         // Knockback stacking?
    s32 damage_queue;                   // Used to calculate knockback?
    s32 damage_angle;
    s32 damage_element;
    s32 damage_lr;
    s32 damage_index;
    s32 damage_joint_id;
    s32 damage_player_num;
    s32 damage_player;                  // Port index of damaging fighter
    u16 damage_count;                   // Number of times fighter has taken damage; initialized to 0 and incremented, but never used?
    s32 damage_kind;
    s32 damage_heal;                    // Percent damage to heal
    f32 damage_mul;
    s32 damage_object_class;            // Fighter, Weapon, Item or Ground
    s32 damage_object_kind;             // FTKind, WPKind, ITKind, envKind
    GMStatFlags damage_stat_flags;
    u16 damage_stat_count;

    f32 public_knockback;               // Knockback value used for crowd reactions

    GObj *search_gobj;                  // GObj this fighter found when searching for grabbable fighters
    f32 search_gobj_dist;               // Distance to found fighter GObj
    void (*proc_catch)(GObj*);          // Run this callback on grabbing attacker
    void (*proc_capture)(GObj*, GObj*); // Run this callback on grabbed victim
    GObj *catch_gobj;                   // GObj this fighter has caught
    GObj *capture_gobj;                 // GObj this fighter is captured by

    FTThrowHitDesc *throw_desc;         // Pointer to throw description

    GObj *item_gobj;                    // Item GObj this fighter is holding

    FTSpecialColl *special_coll;        // Fighter's special collision (This is a sphere! Not a box!)

    Vec3f entry_pos;                    // Fighter's initial spawn position

    f32 camera_zoom_frame;              // Maximum size of fighter's camera range?
    f32 camera_zoom_range;              // Multiplier of fighter's camera range?

    FTMotionScript motion_scripts[2][2];// Fighter's move scripts; [i][0] is played before, then [i][1] once all 'proc_whatevers' have been executed 

    DObj *joints[FTPARTS_JOINT_NUM_MAX];// Fighter's joints (DObjs)

    // -1 = hidden, 0 and up = draw model part ID
    FTModelPartStatus modelpart_status[FTPARTS_JOINT_NUM_MAX - nFTPartsJointCommonStart];

    FTTexturePartStatus texturepart_status[2];

    FTData *data;                   // Fighter's file data setup
    FTAttributes *attr;             // Fighter's unique attributes

    void **figatree;                // Main animation
    void **figatree_heap;           // Extern heap to load animations into

    void (*proc_update)(GObj*);     // Update process
    void (*proc_accessory)(GObj*);  // Runs at the beginning of status change if events aren't queued and whenever fighter isn't in hitlag
    void (*proc_interrupt)(GObj*);  // Interrupt process
    void (*proc_physics)(GObj*);    // Physics process
    void (*proc_map)(GObj*);        // Map collisions process
    void (*proc_slope)(GObj*);      // Slope Contour process
    void (*proc_damage)(GObj*);     // Runs when fighter is damaged
    void (*proc_trap)(GObj*);       // Used only by Yoshi Egg?
    void (*proc_shield)(GObj*);     // Runs when hitbox contact with a shield is made
    void (*proc_hit)(GObj*);        // Runs when hitbox contact with a hurtbox is made
    void (*proc_passive)(GObj*);    // Runs regardless of hitlag during the Update / Interrupt main process
    void (*proc_lagupdate)(GObj*);  // Runs on every tic when fighter is in hitlag
    void (*proc_lagstart)(GObj*);   // Runs when fighter enters hitlag
    void (*proc_lagend)(GObj*);     // Runs when fighter's hitlag is over
    void (*proc_status)(GObj*);     // Runs only on status change

    alSoundEffect *p_sfx;
    u16 sfx_id;
    alSoundEffect *p_voice;
    u16 voice_id;
    alSoundEffect *p_loop_sfx;
    u16 loop_sfx_id;

    GMColAnim colanim;

    SYColorRGBA fog_color;          // Used only by Master Hand, when in the background on the -Z plane?
    SYColorRGBA shade_color;        // Shade colors of character costume

    FTKey key;                      // Automatic input sequence struct

    struct FTAfterImageInfo
    {
        ub8 is_itemswing;
        s8 drawstatus;
        u8 desc_id;
        FTAfterImage desc[3];

    } afterimage;

    // Variables independent of current status ID
    union FTPassiveVars
    {
        FTMarioPassiveVars      mario;
        FTDonkeyPassiveVars     donkey;
        FTSamusPassiveVars      samus;
        FTLinkPassiveVars       link;
        FTCaptainPassiveVars    captain;
        FTKirbyPassiveVars      kirby;
        FTPikachuPassiveVars    pikachu;
        FTPurinPassiveVars      purin;
        FTNessPassiveVars       ness;
        FTBossPassiveVars       boss;

    } passive_vars;

    s32 hammer_tics;

    // Variables exclusive to current status ID or groups of status IDs
    union FTStatusVars
    {
        FTCommonStatusVars      common;
        FTMarioStatusVars       mario;
        FTFoxStatusVars         fox;
        FTDonkeyStatusVars      donkey;
        FTSamusStatusVars       samus;
        FTLinkStatusVars        link;
        FTYoshiStatusVars       yoshi;
        FTCaptainStatusVars     captain;
        FTKirbyStatusVars       kirby;
        FTPikachuStatusVars     pikachu;
        FTNessStatusVars        ness;
        FTBossStatusVars        boss;

    } status_vars;

    s32 display_mode;
};

#endif
