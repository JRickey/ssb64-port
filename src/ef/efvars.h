#ifndef _EFVARS_H_
#define _EFVARS_H_

#include <ssb_types.h>
#include <macros.h>
#include <sys/obj.h>
#include <ef/efdef.h>

#define EFCOMMON_FLAMELR_OFF_X_BASE 300.0F
#define EFCOMMON_FLAMELR_OFF_X_ADD (-150.0F)
#define EFCOMMON_FLAMELR_OFF_Y_BASE 200.0F
#define EFCOMMON_FLAMELR_OFF_Y_ADD (-150.0F)
#define EFCOMMON_FLAMELR_VEL_BASE 20.0F

#define EFCOMMON_FLAMERANDOM_ANGLE_BASE F_CLC_DTOR32(90.0F)
#define EFCOMMON_FLAMERANDOM_ANGLE_ADD F_CLC_DTOR32(45.0F)
#define EFCOMMON_FLAMERANDOM_VEL_BASE 15.0F

#define EFCOMMON_DUSTCOLL_OFF_BASE 300.0F
#define EFCOMMON_DUSTCOLL_OFF_ADD (-150.0F)
#define EFCOMMON_DUSTCOLL_ANGLE_BASE F_CLC_DTOR32(90.0F)
#define EFCOMMON_DUSTCOLL_ANGLE_ADD F_CLC_DTOR32(45.0F)
#define EFCOMMON_DUSTCOLL_VEL_BASE 15.0F

#define EFCOMMON_SHOCKSMALL_OFF_BASE 300.0F
#define EFCOMMON_SHOCKSMALL_OFF_ADD  (-150.0F)
#define EFCOMMON_SHOCKSMALL_SCALE_BASE 0.5F
#define EFCOMMON_SHOCKSMALL_SCALE_ADD 0.75F

#define EFCOMMON_DUSTNORMAL_LIFETIME 9
#define EFCOMMON_DUSTNORMAL_ANGLE_BASE F_CLC_DTOR32(30.0F)
#define EFCOMMON_DUSTNORMAL_ANGLE_ADD F_CLC_DTOR32(-15.0F)
#define EFCOMMON_DUSTNORMAL_OFF_Y 39.375F
#define EFCOMMON_DUSTNORMAL_VEL_BASE 36.0F
#define EFCOMMON_DUSTNORMAL_SCATTER (1.0F / (float)EFCOMMON_DUSTNORMAL_LIFETIME)

#define EFCOMMON_DUSTHEAVY_OFF_Y 126.0F

#define EFCOMMON_DUSTEXPANDLARGE_OFF_BASE 160.0F
#define EFCOMMON_DUSTEXPANDLARGE_OFF_SUB 80.0F
#define EFCOMMON_DUSTEXPANDLARGE_SCALE 3.0F

#define EFCOMMON_DUSTEXPANDSMALL_VEL_Y 5.0F
#define EFCOMMON_DUSTEXPANDSMALL_VEL_X 0.0F

#define EFCOMMON_DUSTDASH_SCALE_SMALL 1.0F
#define EFCOMMON_DUSTDASH_SCALE_LARGE 1.5F
#define EFCOMMON_DUSTDASH_OFF_Y 280.0F

#define EFCOMMON_DAMAGEFLYORBS_VEL_SUB 10.0F

#define EFCOMMON_DAMAGESPAWNORBS_LIFETIME_RANDOM_MOD 4
#define EFCOMMON_DAMAGESPAWNORBS_LIFETIME_ADD 12
#define EFCOMMON_DAMAGESPAWNORBS_SCALE_BASE 2
#define EFCOMMON_DAMAGESPAWNORBS_SCALE_ADD 3.0F
#define EFCOMMON_DAMAGESPAWNORBS_VEL_BASE 100.0F
#define EFCOMMON_DAMAGESPAWNORBS_VEL_ADD 120.0F
#define EFCOMMON_DAMAGESPAWNORBS_ANGLE_BASE F_CLC_DTOR32(60.0F)
#define EFCOMMON_DAMAGESPAWNORBS_ANGLE_ADD1 F_CLC_DTOR32(-30.0F)
#define EFCOMMON_DAMAGESPAWNORBS_ANGLE_ADD2 F_CLC_DTOR32(90.0F)

#define EFCOMMON_STARRODSPARK_ADD_TIMER 62
#define EFCOMMON_STARRODSPARK_SCALE 0.75F
#define EFCOMMON_STARRODSPARK_VEL_BASE 25.0F
#define EFCOMMON_STARRODSPARK_VEL_ADD (-0.4F)

#define EFCOMMON_DAMAGESPAWNSPARK_LIFETIME 8
#define EFCOMMON_DAMAGESPAWNSPARK_ADD_TIMER 250
#define EFCOMMON_DAMAGESPAWNSPARK_LIFETIME_MOD 4
#define EFCOMMON_DAMAGESPAWNSPARK_VEL_BASE 50.0F
#define EFCOMMON_DAMAGESPAWNSPARK_VEL_ADD 0.004F

#define EFCOMMON_DAMAGESPAWNMDUST_LIFETIME 8
#define EFCOMMON_DAMAGESPAWNMDUST_ADD_TIMER 250
#define EFCOMMON_DAMAGESPAWNMDUST_LIFETIME_MOD 4
#define EFCOMMON_DAMAGESPAWNMDUSTVEL_BASE 50.0F
#define EFCOMMON_DAMAGESPAWNMDUSTVEL_ADD 0.004F

#define EFCOMMON_QUAKE_MAGNITUDE 6500.0F

#define EFCOMMON_CAPTUREKIRBYSTAR_SPARK_TIMER_MOD 2
#define EFCOMMON_CAPTUREKIRBYSTAR_ROTATE_STEP F_CLC_DTOR32(10.0F)
#define EFCOMMON_CAPTUREKIRBYSTAR_SPARK_OFF_Y 210.0F
#define EFCOMMON_CAPTUREKIRBYSTAR_SPARK_SCATTER_X 125.0F
#define EFCOMMON_CAPTUREKIRBYSTAR_SPARK_SCATTER_Y 250.0F

#define EFCOMMON_LOSEKIRBYSTAR_LIFETIME 50
#define EFCOMMON_LOSEKIRBYSTAR_ROTATE_STEP F_CLC_DTOR32(10.0F)
#define EFCOMMON_LOSEKIRBYSTAR_GRAVITY 3.0F
#define EFCOMMON_LOSEKIRBYSTAR_TVEL (-90.0F)
#define EFCOMMON_LOSEKIRBYSTAR_VEL_X 20.0F
#define EFCOMMON_LOSEKIRBYSTAR_VEL_Y 80.0F
#define EFCOMMON_LOSEKIRBYSTAR_OFF_Y 210.0F

typedef struct EFCommonEffectVarsContainer
{
	s32 lifetime;
	s32 unk_ef_0x1C;

} EFCommonEffectVarsContainer;

typedef struct EFCommonEffectVarsDamageNormalHeavy
{
    Vec3f pos;
    u8 player;
    s32 size;

} EFCommonEffectVarsDamageNormalHeavy;

typedef struct EFCommonEffectVarsCommon
{
    Vec3f vel;
    s32 size;
    LBTransform *xf;

} EFCommonEffectVarsCommon;

typedef struct EFCommonEffectVarsDustLight
{
    Vec3f vel1, vel2;
    s32 lifetime;
    LBTransform *xf;

} EFCommonEffectVarsDustLight;

typedef struct EFCommonEffectVarsDustHeavy
{
    Vec3f pos;
    s32 lr;
    s32 anim_frame;
    LBTransform *xf;

} EFCommonEffectVarsDustHeavy;

typedef struct EFCommonEffectVarsDamageFlyOrbs
{
    Vec3f vel;
    s32 lifetime;

} EFCommonEffectVarsDamageFlyOrbs;

typedef struct EFCommonEffectVarsDamageSpawnOrbs
{
    s32 lifetime;
    Vec3f pos;

} EFCommonEffectVarsDamageSpawnOrbs;

typedef struct EFCommonEffectVarsImpactWave
{
    u8 index;
    f32 alpha;
    f32 decay;

} EFCommonEffectVarsImpactWave;

typedef struct EFCommonEffectVarsStarRodSpark
{
    Vec3f vel, add;
    s32 add_timer;

} EFCommonEffectVarsStarRodSpark;

typedef struct EFCommonEffectVarsDamageFlySpark
{
    Vec3f vel, add;
    s32 add_timer;

} EFCommonEffectVarsDamageFlySpark;

typedef struct EFCommonEffectVarsDamageSpawnSpark
{
    s32 lifetime;
    Vec3f pos;
    s32 lr;

} EFCommonEffectVarsDamageSpawnSpark;

typedef struct EFCommonEffectVarsDamageFlyMDust
{
    Vec3f vel, add;
    s32 add_timer;

} EFCommonEffectVarsDamageFlyMDust;

typedef struct EFCommonEffectVarsDamageSpawnMDust
{
    s32 lifetime;
    Vec3f pos;
    s32 lr;

} EFCommonEffectVarsDamageSpawnMDust;

typedef struct EFCommonEffectVarsQuake
{
    u8 priority;

} EFCommonEffectVarsQuake;

typedef struct EFCommonEffectVarsReflector
{
    s32 index, status;

} EFCommonEffectVarsReflector;

typedef struct EFCommonEffectVarsShield
{
    s32 player;
    sb32 is_damage_shield;

} EFCommonEffectVarsShield;

typedef struct EFCommonEffectVarsUnknown1
{
    s32 efvars_unk1_0x0;

} EFCommonEffectVarsUnknown1;

typedef struct EFCommonEffectVarsThunderTrail
{
    s32 lifetime;

} EFCommonEffectVarsThunderTrail;

typedef struct EFCommonEffectVarsVulcanJab
{
    s32 lifetime;
    Vec3f vel, add;

} EFCommonEffectVarsVulcanJab;

typedef struct EFCommonEffectVarsPKThunder
{
    s32 status;
    GObj *owner_gobj;

} EFCommonEffectVarsPKThunder;

typedef struct EFCommonEffectVarsYoshiEggLay
{
    s32 index, force_index;

} EFCommonEffectVarsYoshiEggLay;

typedef struct EFCommonEffectVarsCaptureKirbyStar
{
    s32 effect_timer;

} EFCommonEffectVarsCaptureKirbyStar;

typedef struct EFCommonEffectVarsLoseKirbyStar
{
    Vec2f vel;
    s16 lifetime;
    s16 lr;

} EFCommonEffectVarsLoseKirbyStar;

typedef struct EFGroundEffectVarsCommon
{
    Vec3f pos;
    s32 effect_status;
    f32 scale_step;
    s32 lr;
    void *anim_joint;
    void *matanim_joint;
    u8 lr_bool;

} EFGroundEffectVarsCommon;

#endif
