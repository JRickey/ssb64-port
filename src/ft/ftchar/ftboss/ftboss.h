#ifndef _FTBOSS_H_
#define _FTBOSS_H_

#include "ftbossfunctions.h"

#define FTBOSS_ATTACK_WAIT_MAX 120				// Master Hand will wait at most this many frames before initiating his next attack
#define FTBOSS_ATTACK_WAIT_LEVEL_DIV 100			// Divide this value by CPU level

#define FTBOSS_HARAU_VEL_X 50.0F					// Velocity of sweep ("shooing") attack?

#define FTBOSS_OKUHIKOUKI_VEL_ADD 40.0F	

extern FTStatusDesc dFTBossSpecialStatusDescs[/* */];

extern void *gFTDataBossMain;
extern void *gFTDataBossMainMotion;
extern void *gFTDataBossModel;
extern s32 gFTDataBossParticleBankID;

typedef enum ftBossMotion
{
	nFTBossMotionDefault = nFTCommonMotionSpecialStart,
	nFTBossMotionHippataku,	// Horizontal slap
	nFTBossMotionHarau,		// Sweep
	nFTBossMotionOkuhikouki1, // Finger rocket
	nFTBossMotionOkuhikouki2,
	nFTBossMotionOkuhikouki3,
	nFTBossMotionWalk,
	nFTBossMotionWalkLoop,
	nFTBossMotionWalkWait,
	nFTBossMotionWalkShoot,	// Finger walk hit
	nFTBossMotionGootsubusuUp,	// Fist slam 
	nFTBossMotionGootsubusuWait,
	nFTBossMotionGootsubusuEnd,
	nFTBossMotionGootsubusuDown,
	nFTBossMotionTsutsuku1,	// Pointer poke
	nFTBossMotionTsutsuku3,
	nFTBossMotionTsutsuku2,
	nFTBossMotionDrill,
	nFTBossMotionOkukouki,
	nFTBossMotionYubideppou1, // Finger gun
	nFTBossMotionYubideppou3,
	nFTBossMotionYubideppou2,
	nFTBossMotionOkupunch1,	// Fist rocket
	nFTBossMotionOkupunch2,
	nFTBossMotionOkupunch3,
	nFTBossMotionOkutsubushi, // Vertical hand slap
	nFTBossMotionDeadLeft,
	nFTBossMotionDeadCenter,
	nFTBossMotionDeadRight,
	nFTBossMotionAppear

} ftBossMotion;

typedef enum ftBossStatus
{
	nFTBossStatusDefault = nFTCommonStatusSpecialStart,
	nFTBossStatusWait,
	nFTBossStatusMove,
	nFTBossStatusHippataku,	// Horizontal slap
	nFTBossStatusHarau,		// Sweep
	nFTBossStatusOkuhikouki1,
	nFTBossStatusOkuhikouki2,
	nFTBossStatusOkuhikouki3,
	nFTBossStatusWalk,
	nFTBossStatusWalkLoop,
	nFTBossStatusWalkWait,
	nFTBossStatusWalkShoot,	// Finger walk hit
	nFTBossStatusGootsubusuUp,	// Fist slam 
	nFTBossStatusGootsubusuWait,
	nFTBossStatusGootsubusuEnd,
	nFTBossStatusGootsubusuDown,
	nFTBossStatusTsutsuku1,	// Pointer poke
	nFTBossStatusTsutsuku3,
	nFTBossStatusTsutsuku2,
	nFTBossStatusDrill,
	nFTBossStatusOkukouki,
	nFTBossStatusYubideppou1, // Finger gun
	nFTBossStatusYubideppou3,
	nFTBossStatusYubideppou2,
	nFTBossStatusOkupunch1,	// Fist rocket
	nFTBossStatusOkupunch2,
	nFTBossStatusOkupunch3,
	nFTBossStatusOkutsubushi, // Vertical hand slap
	nFTBossStatusOkutsubushiStart,
	nFTBossStatusDeadLeft,
	nFTBossStatusDeadCenter,
	nFTBossStatusDeadRight,
	nFTBossStatusAppear

} ftBossStatus;

typedef struct ftBossInfo
{
    GObj *target_gobj; // (Fighter) GObj pointer of Master Hand's target
    s32 current_line_id;
    s32 default_line_id;
    f32 wait_div;
    u32 wait_timer; // Wait this many frames until next attack
    s8 status_id;
    s8 status_id_random;
    u8 status_id_guard; // Struct probably ends here

} ftBossInfo;

typedef struct FTBossPassiveVars
{
    ftBossInfo *p, s;

} FTBossPassiveVars;

typedef struct ftBossWaITStatusVars
{
    Vec3f pos;

} ftBossWaITStatusVars;

typedef struct ftBossMoveStatusVars
{
    void (*proc_setstatus)(GObj*);
    Vec3f vel;
    f32 magnitude;

} ftBossMoveStatusVars;

typedef struct ftBossOkhuikoukiStatusVars
{
    Vec3f pos;

} ftBossOkuhikoukiStatusVars;

typedef struct ftBossGootsubusuStatusVars
{
    s16 wait_timer;
    f32 edgeleft_pos_x;
    f32 edgeright_pos_x;

} ftBossGootsubusuStatusVars;

typedef struct ftBossTsutsukuStatusVars
{
    s16 wait_timer;

} ftBossTsutsukuStatusVars;

typedef struct ftBossDrillStatusVars
{
    s16 follow_timer; // Follow player for this many frames?
    f32 edgeleft_pos_x;
    f32 edgeright_pos_x;

} ftBossDrillStatusVars;

typedef struct ftBossYubideppouStatusVars
{
    s16 wait_timer;
    s16 bullet_count;
    u8 shoot_timer;

} ftBossYubideppouStatusVars;

typedef struct ftBossOkupunchStatusVars
{
    Vec3f pos;

} ftBossOkupunchStatusVars;

typedef struct ftBossOkutsubushiStatusVars
{
    Vec3f pos;
    u16 follow_timer;

} ftBossOkutsubushiStatusVars;

typedef struct ftBossDeadStatusVars
{
    s16 dead_timer;

} ftBossDeadStatusVars;

typedef union FTBossStatusVars
{
    ftBossWaITStatusVars wait;
    ftBossMoveStatusVars move;
    ftBossOkuhikoukiStatusVars okuhikouki;
    ftBossGootsubusuStatusVars gootsubu;
	ftBossTsutsukuStatusVars tsutsuku;
    ftBossDrillStatusVars drill;
    ftBossYubideppouStatusVars yubideppou;
    ftBossOkupunchStatusVars okupunch;
    ftBossOkutsubushiStatusVars okutsubushi;
    ftBossDeadStatusVars dead;

} FTBossStatusVars;

#endif
