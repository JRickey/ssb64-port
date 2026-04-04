#ifndef _EFTYPES_H_
#define _EFTYPES_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <gm/generic.h>

#include <ef/efdef.h>
#include <ef/efvars.h>

struct EFDesc
{
    u8 flags;
    u8 dl_link;
    void **file_head;
    DObjTransformTypes transform_types1;
    DObjTransformTypes transform_types2;
    void (*proc_update)(GObj*);
    void (*proc_display)(GObj*);
    intptr_t o_dobjsetup;
    intptr_t o_mobjsub;
    intptr_t o_anim_joint;
    intptr_t o_matanim_joint;
};

typedef struct EFGroundParam
{
    u16 effect_id;                      // Array index of effect desc to use
    u16 make_queue;                     // How many of this effect ID to spawn
    s32 lr;                             // 0 = random, -3 or 3 = ???
    u8 effect_weight;                   // Randomizer weight?

} EFGroundParam;

typedef struct EFGroundDesc
{
    f32 alt_high;                       // Maximum alt
    f32 alt_low;                        // Minimum alt
    f32 pos_z;                          // Z-Axis position
    f32 scale;                          // Scale
    u16 effect_status;                  // Effect status? Always -1?
    void (*proc_groundeffect)(GObj*);   // ???
    EFDesc effect_desc;           // Effect description

} EFGroundDesc;

typedef struct EFGroundData
{
    u8 params_num;                      // Number of elements in effect_params
    EFGroundParam *effect_params;       // Effect parameter descriptions
    intptr_t o_data;                    // ???
    EFGroundDesc *effect_descs;   // Ground effect creation descriptions

} EFGroundData;

typedef struct EFGroundActor
{
    s32 make_wait;
    u16 effect_id;
    u16 make_queue;             // How many of this effect should be spawned
    u8 effect_count;            
    u8 *effect_ids;
    s32 lr;                     // Number of effects spawned, multiplied by LR sign
    void *file_head;
    EFGroundData *effect_data;

} EFGroundActor;

struct EFStruct
{
    EFStruct *next;
    GObj *fighter_gobj;

    u16 bank_id;
    LBTransform *xf;

    ub32 is_pause_effect : 1;

    void (*proc_update)(GObj*);

    union efEffectVars // Effect vars union?
    {
        EFCommonEffectVarsCommon common;
        EFCommonEffectVarsContainer container;
        EFCommonEffectVarsDamageNormalHeavy damage_normal_heavy;
        EFCommonEffectVarsDustLight dust_light;
        EFCommonEffectVarsDustHeavy dust_heavy;
        EFCommonEffectVarsDamageFlyOrbs damage_fly_orbs;
        EFCommonEffectVarsDamageSpawnOrbs damage_spawn_orbs;
        EFCommonEffectVarsImpactWave impact_wave;
        EFCommonEffectVarsStarRodSpark star_rod_spark;
        EFCommonEffectVarsDamageFlySpark damage_fly_sparks;
        EFCommonEffectVarsDamageSpawnSpark damage_spawn_sparks;
        EFCommonEffectVarsDamageFlyMDust damage_fly_mdust;
        EFCommonEffectVarsDamageSpawnMDust damage_spawn_mdust;
        EFCommonEffectVarsQuake quake;
        EFCommonEffectVarsReflector reflector;
        EFCommonEffectVarsShield shield;
        EFCommonEffectVarsUnknown1 unknown1;
        EFCommonEffectVarsThunderTrail thunder_trail;
        EFCommonEffectVarsVulcanJab vulcan_jab;
        EFCommonEffectVarsPKThunder pkthunder;
        EFCommonEffectVarsYoshiEggLay yoshi_egg_lay;
        EFCommonEffectVarsCaptureKirbyStar capture_kirby_star;
        EFCommonEffectVarsLoseKirbyStar lose_kirby_star;

        EFGroundEffectVarsCommon ground_effect; // Used in efGroundCheckEffectInBounds

    } effect_vars;
};

#endif
