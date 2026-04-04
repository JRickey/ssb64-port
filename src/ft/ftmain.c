#include <ft/fighter.h>
#include <wp/weapon.h>
#include <it/item.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <sys/controller.h>

extern alSoundEffect* func_800269C0_275C0(u16);
extern void func_ovl0_800C9A38();

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// Hitbox sound effects
// Rows = SFX type
// Columns = SFX level

// 0x80128D00
u16 dFTMainHitCollisionFGMs[/* */][nGMHitLevelEnumCount] =
{
    { nSYAudioFGMPunchS,             nSYAudioFGMPunchM,             nSYAudioFGMPunchL             },    // Punch
    { nSYAudioFGMKickS,              nSYAudioFGMKickM,              nSYAudioFGMKickL              },    // Kick
    { nSYAudioFGMMarioSpecialHiCoin, nSYAudioFGMMarioSpecialHiCoin, nSYAudioFGMMarioSpecialHiCoin },    // Coin
    { nSYAudioFGMBurnS,              nSYAudioFGMBurnM,              nSYAudioFGMBurnL              },    // Burn
    { nSYAudioFGMShockS,             nSYAudioFGMShockM,             nSYAudioFGMShockL             },    // Shock
    { nSYAudioFGMSlashS,             nSYAudioFGMSlashM,             nSYAudioFGMSlashL             },    // Slash
    { nSYAudioFGMHarisenHit,         nSYAudioFGMHarisenHit,         nSYAudioFGMHarisenHit         },    // Fan / Slap
    { nSYAudioFGMPunchM,             nSYAudioFGMPunchL,             nSYAudioFGMBatHit             }     // Bat
};

// 0x80128D30
GRAttackColl dFTMainGroundHitCollisionAttributes[/* */] =
{
    {  4,  1, 361, 100, 100, 0, nGMHitElementFire  },
    {  5, 10,  90, 100, 200, 0, nGMHitElementFire  },
    {  6, 10,  90, 100, 100, 0, nGMHitElementFire  },
    {  7, 10, 361, 100,  80, 0, nGMHitElementSlash },
    {  8,  1,  90, 100, 100, 0, nGMHitElementFire  },
    {  9,  1,  90, 100, 100, 0, nGMHitElementFire  }
};

/*
    Fighter status files order:
    1. Mario: 0x80129F08
    2. Donkey Kong: 0x80129FBC
    3. Fox: 0x8012A214
    4. Samus: 0x8012A41C
    5. Luigi: 0x8015A4F8
    6. Link: 0x8012A5AC
    7. Kirby: 0x8012A700
    9. Captain Falcon: 0x8012AD7C
    8. Yoshi: 0x8012AEF8
    10. Pikachu: 0x8012B010
    11. Jigglypuff: 0x8012B178
    12. Ness: 0x8012B2B8
    13. Master Hand: 0x8012B4AC
*/

#include <ft/ftcommon/ftcommonstatus.h>
#include <ft/ftchar/ftmario/ftmariostatus.h>
#include <ft/ftchar/ftdonkey/ftdonkeystatus.h>
#include <ft/ftchar/ftfox/ftfoxstatus.h>
#include <ft/ftchar/ftsamus/ftsamusstatus.h>
#include <ft/ftchar/ftluigi/ftluigistatus.h>
#include <ft/ftchar/ftlink/ftlinkstatus.h>
#include <ft/ftchar/ftkirby/ftkirbystatus.h>
#include <ft/ftchar/ftcaptain/ftcaptainstatus.h>
#include <ft/ftchar/ftyoshi/ftyoshistatus.h>
#include <ft/ftchar/ftpikachu/ftpikachustatus.h>
#include <ft/ftchar/ftpurin/ftpurinstatus.h>
#include <ft/ftchar/ftness/ftnessstatus.h>
#include <ft/ftchar/ftboss/ftbossstatus.h>

// 0x8012B740
FTStatusDesc *dFTMainSpecialStatusDescs[/* */] =
{
    dFTMarioSpecialStatusDescs,
    dFTFoxSpecialStatusDescs,
    dFTDonkeySpecialStatusDescs,
    dFTSamusSpecialStatusDescs,
    dFTLuigiSpecialStatusDescs,
    dFTLinkSpecialStatusDescs,
    dFTYoshiSpecialStatusDescs,
    dFTCaptainSpecialStatusDescs,
    dFTKirbySpecialStatusDescs,
    dFTPikachuSpecialStatusDescs,
    dFTPurinSpecialStatusDescs,
    dFTNessSpecialStatusDescs,
    dFTBossSpecialStatusDescs,
    dFTMarioSpecialStatusDescs,
    dFTMarioSpecialStatusDescs,
    dFTFoxSpecialStatusDescs,
    dFTDonkeySpecialStatusDescs,
    dFTSamusSpecialStatusDescs,
    dFTLuigiSpecialStatusDescs,
    dFTLinkSpecialStatusDescs,
    dFTYoshiSpecialStatusDescs,
    dFTCaptainSpecialStatusDescs,
    dFTKirbySpecialStatusDescs,
    dFTPikachuSpecialStatusDescs,
    dFTPurinSpecialStatusDescs,
    dFTNessSpecialStatusDescs,
    dFTDonkeySpecialStatusDescs
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80131180 - stage moving hazards
GRObstacle sFTMainGroundObstacles[2];

// 0x80131190 - stage environmental obstacles
GRHazard sFTMainGroundHazards[1];

// 0x80131198
s32 sFTMainGroundObstaclesNum;

// 0x8013119C
s32 sFTMainGroundHazardsNum;

// 0x801311A0
sb32 gFTMainIsDamageDetect[GMCOMMON_PLAYERS_MAX];

// 0x801311B0
sb32 gFTMainIsAttackDetect[GMCOMMON_PLAYERS_MAX];

// 0x801311C0
s32 sFTMainHitLogID;

// 0x801311C4
s32 sFTMainPad0x801311C4;

// 0x801311C8 - 0x10 bytes of padding after this, for mpcoll .bss
FTHitLog sFTMainHitLogs[10];

// 0x801312E0
s32 sFTMainPad0x801312E0;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800DF0F0
void ftMainParseMotionEvent(GObj *fighter_gobj, FTStruct *fp, FTMotionScript *ms, u32 ev_kind)
{
    s32 unused1;
    s32 effect_id;
    s32 i, j;
    s32 unused2;
    FTAttackColl *attack_coll;
    s32 attack_id;
    s32 group_id;
    u32 sfx_id;
    s32 joint_id;
    Vec3f effect_offset;
    Vec3f effect_scatter;
    u32 flag;
    Vec3f damage_coll_offset;
    Vec3f damage_coll_size;
    FTAttributes *attr;
    FTMotionDamageScript *p_damage;
    s32 fkind;
    s32 script_id;
    s32 slope_contour;
    sb32 unused3;

    switch (ev_kind)
    {
    case nFTMotionEventEnd:
        ms->p_script = NULL;
        break;

    case nFTMotionEventSyncWait:
        ms->script_wait += ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventAsyncWait:
        ms->script_wait = ftMotionEventCast(ms, FTMotionEventDefault)->value - fighter_gobj->anim_frame;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventMakeAttackColl:
    case nFTMotionEventMakeAttackCollScaled:
        if (fp->pkind != nFTPlayerKindDemo)
        {
            attack_id = ftMotionEventCast(ms, FTMotionEventMakeAttack1)->attack_id;
            attack_coll = &fp->attack_colls[attack_id];

            if ((attack_coll->attack_state == nGMAttackStateOff) || (attack_coll->group_id != ftMotionEventCast(ms, FTMotionEventMakeAttack1)->group_id))
            {
                attack_coll->group_id = ftMotionEventCast(ms, FTMotionEventMakeAttack1)->group_id;
                attack_coll->attack_state = nGMAttackStateNew;
                fp->is_attack_active = TRUE;

                for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
                {
                    if ((i != attack_id) && (fp->attack_colls[i].attack_state != nGMAttackStateOff) && (attack_coll->group_id == fp->attack_colls[i].group_id))
                    {
                        for (j = 0; j < ARRAY_COUNT(attack_coll->attack_records); j++)
                        {
                            attack_coll->attack_records[j] = fp->attack_colls[i].attack_records[j];
                        }
                        break;
                    }
                }
                if (i == ARRAY_COUNT(fp->attack_colls))
                {
                    ftParamClearAttackRecordID(fp, attack_id);
                }
            }
            attack_coll->joint_id = ftParamGetJointID(fp, ftMotionEventCast(ms, FTMotionEventMakeAttack1)->joint_id);
            attack_coll->joint = fp->joints[attack_coll->joint_id];
            attack_coll->damage = ftMotionEventCast(ms, FTMotionEventMakeAttack1)->damage;
            attack_coll->can_rebound = ftMotionEventCast(ms, FTMotionEventMakeAttack1)->can_rebound;
            attack_coll->element = ftMotionEventCast(ms, FTMotionEventMakeAttack1)->element;

            ftMotionEventAdvance(ms, FTMotionEventMakeAttack1);

            attack_coll->size = ftMotionEventCast(ms, FTMotionEventMakeAttack2)->size * 0.5F;
            attack_coll->offset.x = ftMotionEventCast(ms, FTMotionEventMakeAttack2)->off_x;

            ftMotionEventAdvance(ms, FTMotionEventMakeAttack2);

            attack_coll->offset.y = ftMotionEventCast(ms, FTMotionEventMakeAttack3)->off_y;
            attack_coll->offset.z = ftMotionEventCast(ms, FTMotionEventMakeAttack3)->off_z;

            ftMotionEventAdvance(ms, FTMotionEventMakeAttack3);

            attack_coll->angle = ftMotionEventCast(ms, FTMotionEventMakeAttack4)->angle;
            attack_coll->knockback_scale = ftMotionEventCast(ms, FTMotionEventMakeAttack4)->knockback_scale;
            attack_coll->knockback_weight = ftMotionEventCast(ms, FTMotionEventMakeAttack4)->knockback_weight;

            attack_coll->is_hit_air = ftMotionEventCast(ms, FTMotionEventMakeAttack4)->is_hit_ground_air & 1;           // Why?
            attack_coll->is_hit_ground = (ftMotionEventCast(ms, FTMotionEventMakeAttack4)->is_hit_ground_air & 2) >> 1; // ???

            ftMotionEventAdvance(ms, FTMotionEventMakeAttack4);

            attack_coll->shield_damage = ftMotionEventCast(ms, FTMotionEventMakeAttack5)->shield_damage;

            attack_coll->fgm_level = ftMotionEventCast(ms, FTMotionEventMakeAttack5)->fgm_level;
            attack_coll->fgm_kind = ftMotionEventCast(ms, FTMotionEventMakeAttack5)->fgm_kind;

            attack_coll->knockback_base = ftMotionEventCast(ms, FTMotionEventMakeAttack5)->knockback_base;

            ftMotionEventAdvance(ms, FTMotionEventMakeAttack5);

            attack_coll->is_scale_pos = (ev_kind == nFTMotionEventMakeAttackCollScaled) ? TRUE : FALSE;

            attack_coll->motion_attack_id = fp->motion_attack_id;

            attack_coll->motion_count = fp->motion_count;

            attack_coll->damage = ftParamGetStaledDamage(fp->player, attack_coll->damage, attack_coll->motion_attack_id, attack_coll->motion_count);
        }
        else ftMotionEventAdvance(ms, FTMotionEventMakeAttack);
        break;

    case nFTMotionEventSetAttackCollOffset:
        attack_id = ftMotionEventCast(ms, FTMotionEventSetAttackOffset1)->attack_id;

        attack_coll = &fp->attack_colls[attack_id];

        attack_coll->offset.x = ftMotionEventCast(ms, FTMotionEventSetAttackOffset1)->off_x;

        ftMotionEventAdvance(ms, FTMotionEventSetAttackOffset1);

        attack_coll->offset.y = ftMotionEventCast(ms, FTMotionEventSetAttackOffset2)->off_y;
        attack_coll->offset.z = ftMotionEventCast(ms, FTMotionEventSetAttackOffset2)->off_z;

        ftMotionEventAdvance(ms, FTMotionEventSetAttackOffset2);
        break;

    case nFTMotionEventSetAttackCollDamage:
        if (fp->pkind != nFTPlayerKindDemo)
        {
            attack_id = ftMotionEventCast(ms, FTMotionEventSetAttackCollDamage)->attack_id;

            fp->attack_colls[attack_id].damage = ftMotionEventCast(ms, FTMotionEventSetAttackCollDamage)->damage;

            ftMotionEventAdvance(ms, FTMotionEventSetAttackCollDamage);

            fp->attack_colls[attack_id].damage = ftParamGetStaledDamage(fp->player, fp->attack_colls[attack_id].damage, fp->attack_colls[attack_id].motion_attack_id, fp->attack_colls[attack_id].motion_count);
        }
        else ftMotionEventAdvance(ms, FTMotionEventSetAttackCollDamage);
        break;

    case nFTMotionEventSetAttackCollSize:
        attack_id = ftMotionEventCast(ms, FTMotionEventSetAttackCollSize)->attack_id;

        fp->attack_colls[attack_id].size = ftMotionEventCast(ms, FTMotionEventSetAttackCollSize)->size * 0.5F;

        ftMotionEventAdvance(ms, FTMotionEventSetAttackCollSize);
        break;

    case nFTMotionEventSetAttackCollSoundLevel:
        attack_id = ftMotionEventCast(ms, FTMotionEventSetAttackCollSound)->attack_id;

        fp->attack_colls[attack_id].fgm_level = ftMotionEventCast(ms, FTMotionEventSetAttackCollSound)->fgm_level;

        ftMotionEventAdvance(ms, FTMotionEventSetAttackCollSound);
        break;

    case nFTMotionEventRefreshAttackCollID:
        attack_id = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);

        ftParamRefreshAttackCollID(fighter_gobj, attack_id);
        break;

    case nFTMotionEventClearAttackCollID:
        attack_id = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);

        fp->attack_colls[attack_id].attack_state = nGMAttackStateOff;
        break;

    case nFTMotionEventClearAttackCollAll:
        ftParamClearAttackCollAll(fighter_gobj);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetThrow:
        ftMotionEventAdvance(ms, FTMotionEventSetThrow1);

        fp->throw_desc = ftMotionEventCast(ms, FTMotionEventSetThrow2)->throw_desc;

        ftMotionEventAdvance(ms, FTMotionEventSetThrow2);
        break;

    case nFTMotionEventPlayFGMStoreInfo:
        if (!(fp->is_muted))
        {
            fp->p_sfx = func_800269C0_275C0(ftMotionEventCastAdvance(ms, FTMotionEventDefault)->value);

            fp->sfx_id = (fp->p_sfx != NULL) ? fp->p_sfx->sfx_id : 0;
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventPlayFGM:
        if (!(fp->is_muted))
        {
            func_800269C0_275C0(ftMotionEventCastAdvance(ms, FTMotionEventDefault)->value);
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventPlayLoopSFXStoreInfo:
        if (!(fp->is_muted))
        {
            ftParamPlayLoopSFX(fp, ftMotionEventCastAdvance(ms, FTMotionEventDefault)->value);
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventStopLoopSFX:
        ftParamStopLoopSFX(fp), ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventPlayVoiceStoreInfo:
        if (!(fp->is_muted) && (fp->attr->is_have_voice))
        {
            ftParamPlayVoice(fp, ftMotionEventCastAdvance(ms, FTMotionEventDefault)->value);
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventPlayLoopVoiceStoreInfo:
        if (!(fp->is_muted) && (fp->attr->is_have_voice))
        {
            ftParamPlayLoopSFX(fp, ftMotionEventCastAdvance(ms, FTMotionEventDefault)->value);
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventPlaySmashVoice:
        if (!(fp->is_muted))
        {
            ftParamPlayVoice(fp, fp->attr->smash_sfx[syUtilsRandIntRange(ARRAY_COUNT(fp->attr->smash_sfx))]);

            ftMotionEventAdvance(ms, FTMotionEventDefault);
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetAirJumpAdd:
        fp->ga = nMPKineticsAir;

        fp->physics.vel_air.z = DObjGetStruct(fighter_gobj)->translate.vec.f.z = 0.0F;

        fp->jumps_used++;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetAirJumpMax:
        attr = fp->attr;

        fp->ga = nMPKineticsAir;

        fp->physics.vel_air.z = DObjGetStruct(fighter_gobj)->translate.vec.f.z = 0.0F;

        fp->jumps_used = attr->jumps_max;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventEffect:
    case nFTMotionEventEffectItemHold:
        if (!(fp->is_effect_skip))
        {
            joint_id = ftParamGetJointID(fp, ftMotionEventCast(ms, FTMotionEventMakeEffect1)->joint_id);
            effect_id = ftMotionEventCast(ms, FTMotionEventMakeEffect1)->effect_id;
            flag = ftMotionEventCast(ms, FTMotionEventMakeEffect1)->flag;

            ftMotionEventAdvance(ms, FTMotionEventMakeEffect1);

            effect_offset.x = ftMotionEventCast(ms, FTMotionEventMakeEffect2)->off_x;
            effect_offset.y = ftMotionEventCast(ms, FTMotionEventMakeEffect2)->off_y;

            ftMotionEventAdvance(ms, FTMotionEventMakeEffect2);

            effect_offset.z = ftMotionEventCast(ms, FTMotionEventMakeEffect3)->off_z;
            effect_scatter.x = ftMotionEventCast(ms, FTMotionEventMakeEffect3)->rng_x;

            ftMotionEventAdvance(ms, FTMotionEventMakeEffect3);

            effect_scatter.y = ftMotionEventCast(ms, FTMotionEventMakeEffect4)->rng_y;
            effect_scatter.z = ftMotionEventCast(ms, FTMotionEventMakeEffect4)->rng_z;

            ftMotionEventAdvance(ms, FTMotionEventMakeEffect4);

            ftParamMakeEffect(fighter_gobj, effect_id, joint_id, &effect_offset, &effect_scatter, fp->lr, (ev_kind == nFTMotionEventEffectItemHold) ? TRUE : FALSE, flag);
        }
        else ftMotionEventAdvance(ms, FTMotionEventMakeEffect);
        break;

    case nFTMotionEventSetHitStatusPartAll:
        ftParamSetHitStatusPartAll(fighter_gobj, ftMotionEventCast(ms, FTMotionEventDefault)->value);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetHitStatusPartID:
        ftParamSetHitStatusPartID(fighter_gobj, ftParamGetJointID(fp, ftMotionEventCast(ms, FTMotionEventSetHitStatusPartID)->joint_id), ftMotionEventCast(ms, FTMotionEventSetHitStatusPartID)->hitstatus);

        ftMotionEventAdvance(ms, FTMotionEventSetHitStatusPartID);
        break;

    case nFTMotionEventSetHitStatusAll:
        ftParamSetHitStatusAll(fighter_gobj, ftMotionEventCast(ms, FTMotionEventDefault)->value);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventResetDamageCollPartAll:
        ftParamResetFighterDamageCollsAll(fighter_gobj);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetDamageCollPartID:
        joint_id = ftParamGetJointID(fp, ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID1)->joint_id);

        ftMotionEventAdvance(ms, FTMotionEventSetDamageCollPartID1);

        damage_coll_offset.x = ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID2)->off_x;
        damage_coll_offset.y = ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID2)->off_y;

        ftMotionEventAdvance(ms, FTMotionEventSetDamageCollPartID2);

        damage_coll_offset.z = ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID3)->off_z;
        damage_coll_size.x = ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID3)->size_x;

        ftMotionEventAdvance(ms, FTMotionEventSetDamageCollPartID3);

        damage_coll_size.y = ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID4)->size_y;
        damage_coll_size.z = ftMotionEventCast(ms, FTMotionEventSetDamageCollPartID4)->size_z;

        ftMotionEventAdvance(ms, FTMotionEventSetDamageCollPartID4);

        ftParamModifyDamageCollID(fighter_gobj, joint_id, &damage_coll_offset, &damage_coll_size);
        break;

    case nFTMotionEventLoopBegin:
        ms->p_goto[ms->script_id] = lbRelocGetFileData(void*, ms->p_script, sizeof(FTMotionEventDefault));

        ms->script_id++;

        ms->loop_count[ms->script_id++ - 1] = ftMotionEventCast(ms, FTMotionEventDefault)->value, ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventLoopEnd:
        if (--ms->loop_count[ms->script_id - 2] != 0)
        {
            ms->p_script = ms->p_goto[ms->script_id - 2];
        }
        else ftMotionEventAdvance(ms, FTMotionEventDefault), ms->script_id -= 2; // Seems fake, but also impossible to match otherwise???
        break;

    case nFTMotionEventSubroutine:
        ftMotionEventAdvance(ms, FTMotionEventSubroutine1);

        ms->p_goto[ms->script_id] = lbRelocGetFileData(void*, ms->p_script, sizeof(FTMotionEventSubroutine2));

        ms->script_id++;

        ms->p_script = ftMotionEventCast(ms, FTMotionEventSubroutine2)->p_goto;
        break;

    case nFTMotionEventSetDamageThrown:
        if (fp->throw_gobj != NULL)
        {
            fkind = fp->throw_fkind;

            ftMotionEventAdvance(ms, FTMotionEventSetDamageThrown1);

            p_damage = ftMotionEventCast(ms, FTMotionEventSetDamageThrown2)->p_subroutine;

            if (p_damage->p_script[fp->status_vars.common.damage.script_id][fkind] != NULL)
            {
                ms->p_goto[ms->script_id] = lbRelocGetFileData(void*, ms->p_script, sizeof(FTMotionEventSetDamageThrown2));

                ms->script_id++;

                ms->p_script = p_damage->p_script[fp->status_vars.common.damage.script_id][fkind];
            }
            else ftMotionEventAdvance(ms, FTMotionEventSetDamageThrown2);
        }
        else ftMotionEventAdvance(ms, FTMotionEventSetDamageThrown);
        break;

    case nFTMotionEventReturn:
        ms->p_script = ms->p_goto[--ms->script_id];
        break;

    case nFTMotionEventGoto:
        ftMotionEventAdvance(ms, FTMotionEventGoto1);

        ms->p_script = ftMotionEventCast(ms, FTMotionEventGoto2)->p_goto;
        break;

    case nFTMotionEventSetParallelScript:
        ftMotionEventAdvance(ms, FTMotionEventParallel1);

        if (fp->motion_scripts[0][1].p_script == NULL)
        {
            fp->motion_scripts[0][1].p_script = fp->motion_scripts[1][1].p_script = ftMotionEventCast(ms, FTMotionEventParallel2)->p_goto;
            fp->motion_scripts[0][1].script_wait = fp->motion_scripts[1][1].script_wait = DObjGetStruct(fighter_gobj)->anim_speed - fighter_gobj->anim_frame;
            fp->motion_scripts[0][1].script_id = fp->motion_scripts[1][1].script_id = 0;
        }
        ftMotionEventAdvance(ms, FTMotionEventParallel2);
        break;

    case nFTMotionEventPauseScript:
        ftMotionEventAdvance(ms, FTMotionEventDefault);

        ms->script_wait = F32_MAX;
        break;

    case nFTMotionEventSetModelPartID:
        ftParamSetModelPartID
        (
            fighter_gobj, 
            ftParamGetJointID(fp, ftMotionEventCast(ms, FTMotionEventSetModelPartID)->joint_id), 
            ftMotionEventCast(ms, FTMotionEventSetModelPartID)->modelpart_id
        );
        ftMotionEventAdvance(ms, FTMotionEventSetModelPartID);
        break;

    case nFTMotionEventResetModelPartAll:
        ftParamResetModelPartAll(fighter_gobj);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventHideModelPartAll:
        ftParamHideModelPartAll(fighter_gobj);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetTexturePartID:
        ftParamSetTexturePartID
        (
            fighter_gobj, 
            ftMotionEventCast(ms, FTMotionEventSetTexturePartID)->texturepart_id, 
            ftMotionEventCast(ms, FTMotionEventSetTexturePartID)->frame
        );
        ftMotionEventAdvance(ms, FTMotionEventSetTexturePartID);
        break;

    case nFTMotionEventSetColAnim:
        ftParamCheckSetFighterColAnimID
        (
            fighter_gobj, 
            ftMotionEventCast(ms, FTMotionEventSetColAnimID)->colanim_id, 
            ftMotionEventCast(ms, FTMotionEventSetColAnimID)->length
        );
        ftMotionEventAdvance(ms, FTMotionEventSetColAnimID);
        break;

    case nFTMotionEventResetColAnim:
        ftParamResetStatUpdateColAnim(fighter_gobj);

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetFlag0:
        fp->motion_vars.flags.flag0 = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetFlag1:
        fp->motion_vars.flags.flag1 = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetFlag2:
        fp->motion_vars.flags.flag2 = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetFlag3:
        fp->motion_vars.flags.flag3 = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetSlopeContour:
        slope_contour = fp->slope_contour;

        fp->slope_contour = ftMotionEventCastAdvance(ms, FTMotionEventSetSlopeContour)->flags;

        if (!(slope_contour & fp->slope_contour & FTSLOPECONTOUR_FLAG_FULL))
        {
            DObjGetStruct(fighter_gobj)->rotate.vec.f.x = F_CLC_DTOR32(0.0F);
        }
        break;

    case nFTMotionEventHideItem:
        fp->is_item_show = ftMotionEventCast(ms, FTMotionEventDefault)->value;

        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;

    case nFTMotionEventSetAfterImage:
        fp->afterimage.is_itemswing = ftMotionEventCast(ms, FTMotionEventSetAfterImage)->is_itemswing;
        fp->afterimage.drawstatus = ftMotionEventCast(ms, FTMotionEventSetAfterImage)->drawstatus;

        ftMotionEventAdvance(ms, FTMotionEventSetAfterImage);
        break;

    case nFTMotionEventMakeRumble:
        if (fp->pkind != nFTPlayerKindDemo)
        {
            ftParamMakeRumble
            (
                fp, 
                ftMotionEventCast(ms, FTMotionEventMakeRumble)->rumble_id, 
                ftMotionEventCast(ms, FTMotionEventMakeRumble)->length
            );
        }
        ftMotionEventAdvance(ms, FTMotionEventMakeRumble);
        break;

    case nFTMotionEventStopRumble:
        if (fp->pkind != nFTPlayerKindDemo)
        {
            gmRumbleStopRumbleID(fp->player, ftMotionEventCast(ms, FTMotionEventDefault)->value);
        }
        ftMotionEventAdvance(ms, FTMotionEventDefault);
        break;
    }
}

// 0x800E02A8 - Run all motion events. Effects are parsed only if events aren't queued.
// If events are not queued, fp->motion_scripts[0][i] is copied to fp->motion_scripts[1][i]
// for later execution in the Physics / Map update process.
void ftMainUpdateMotionEventsAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 i;

    for (i = 0; i < ARRAY_COUNT(fp->motion_scripts); i++)
    {
        FTMotionScript *ms = &fp->motion_scripts[0][i];
        u32 ev_kind;

        if (ms->p_script != NULL)
        {
            if (ms->script_wait != F32_MAX)
            {
                ms->script_wait -= DObjGetStruct(fighter_gobj)->anim_speed;
            }
            while (TRUE)
            {
                if (ms->p_script == NULL)
                {
                    break;
                }
                else
                {
                    if (ms->script_wait == F32_MAX)
                    {
                        if ((DObjGetStruct(fighter_gobj)->anim_speed <= fighter_gobj->anim_frame)) 
                        {
                            break;
                        }
                        else ms->script_wait = -fighter_gobj->anim_frame;
                    }
                    else if (ms->script_wait > 0.0F) 
                    {
                        break;
                    }
                    ev_kind = ftMotionEventCast(ms, FTMotionEventMakeEffect1)->opcode;
    
                    if ((ev_kind == nFTMotionEventEffect || ev_kind == nFTMotionEventEffectItemHold) && (fp->is_events_forward))
                    {
                        ftMotionEventAdvance(ms, FTMotionEventMakeEffect);
                    }
                    else ftMainParseMotionEvent(fighter_gobj, fp, ms, ev_kind);
                }
            }
        }
    }
    if (!(fp->is_events_forward))
    {
        for (i = 0; i < ARRAY_COUNT(fp->motion_scripts); i++)
        {
            fp->motion_scripts[1][i] = fp->motion_scripts[0][i];
        }
    }
}

// 0x800E0478 - Fast-forward most fighter-specific events and update core events (sync timer, async timer, goto, subroutine, etc.).
// fp->motion_scripts[0][i] is always moved to fp->motion_scripts[1][i] for later execution in the Physics / Map update process.
void ftMainUpdateMotionEventsForward(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 i;

    for (i = 0; i < ARRAY_COUNT(fp->motion_scripts[0]); i++)
    {
        FTMotionScript *ms = &fp->motion_scripts[0][i];
        u32 ev_kind;

        if (ms->p_script != NULL)
        {
            if (ms->script_wait != F32_MAX)
            {
                ms->script_wait -= DObjGetStruct(fighter_gobj)->anim_speed;
            }
            while (TRUE)
            {
                if (ms->p_script == NULL)
                {
                    break;
                }
                else
                {
                    if (ms->script_wait == F32_MAX)
                    {
                        if ((DObjGetStruct(fighter_gobj)->anim_speed <= fighter_gobj->anim_frame))
                        {
                            break;
                        }
                        else ms->script_wait = -fighter_gobj->anim_frame;
                    }
                    else if (ms->script_wait > 0.0F)
                    {
                        break;
                    }
                    ev_kind = ftMotionEventCast(ms, FTMotionEventDefault)->opcode;

                    switch (ev_kind)
                    {
                    case nFTMotionEventClearAttackCollID:
                    case nFTMotionEventClearAttackCollAll:
                    case nFTMotionEventSetAttackCollDamage:
                    case nFTMotionEventSetAttackCollSize:
                    case nFTMotionEventSetAttackCollSoundLevel:
                    case nFTMotionEventRefreshAttackCollID:
                    case nFTMotionEventPlayFGM:
                    case nFTMotionEventPlayLoopSFXStoreInfo:
                    case nFTMotionEventStopLoopSFX:
                    case nFTMotionEventPlayVoiceStoreInfo:
                    case nFTMotionEventPlayLoopVoiceStoreInfo:
                    case nFTMotionEventPlayFGMStoreInfo:
                    case nFTMotionEventPlaySmashVoice:
                    case nFTMotionEventSetFlag0:
                    case nFTMotionEventSetFlag1:
                    case nFTMotionEventSetFlag2:
                    case nFTMotionEventSetAirJumpAdd:
                    case nFTMotionEventSetAirJumpMax:
                    case nFTMotionEventSetColAnim:
                    case nFTMotionEventResetColAnim:
                    case nFTMotionEventMakeRumble:
                    case nFTMotionEventStopRumble:
                    case nFTMotionEventSetAfterImage:
                        ftMotionEventAdvance(ms, FTMotionEventDefault);
                        break;

                    case nFTMotionEventEffect:
                    case nFTMotionEventEffectItemHold:
                        ftMotionEventAdvance(ms, FTMotionEventMakeEffect);
                        break;

                    case nFTMotionEventMakeAttackColl:
                    case nFTMotionEventMakeAttackCollScaled:
                        ftMotionEventAdvance(ms, FTMotionEventMakeAttack);
                        break;

                    case nFTMotionEventSetAttackCollOffset:
                        ftMotionEventAdvance(ms, FTMotionEventSetAttackOffset);
                        break;

                    default:
                        ftMainParseMotionEvent(fighter_gobj, fp, ms, ev_kind);
                        break;
                    }
                }
            }
        }
    }
    for (i = 0; i < ARRAY_COUNT(fp->motion_scripts[0]); i++)
    {
        fp->motion_scripts[1][i] = fp->motion_scripts[0][i];
    }
}

// 0x800E0654 - Fast-forward all events except core, throw setup and effect spawn events.
// This makes it so that effects are run last, after all four main processes
// (proc_update, proc_interrupt, proc_physics, proc_map).
void ftMainUpdateMotionEventsForwardEffect(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 i;

    for (i = 0; i < ARRAY_COUNT(fp->motion_scripts[1]); i++)
    {
        FTMotionScript *ms = &fp->motion_scripts[1][i];
        u32 ev_kind;

        if (ms->p_script != NULL)
        {
            if (ms->script_wait != F32_MAX)
            {
                ms->script_wait -= DObjGetStruct(fighter_gobj)->anim_speed;
            }
            while (TRUE)
            {
                if (ms->p_script == NULL)
                {
                    break;
                }
                else
                {
                    if (ms->script_wait == F32_MAX)
                    {
                        if (DObjGetStruct(fighter_gobj)->anim_speed <= fighter_gobj->anim_frame)
                        {
                            break;
                        }
                        else ms->script_wait = -fighter_gobj->anim_frame;
                    }
                    else if (ms->script_wait > 0.0F)
                    {
                        break;
                    }
                    ev_kind = ftMotionEventCast(ms, FTMotionEventDefault)->opcode;

                    switch (ev_kind)
                    {
                    case nFTMotionEventEnd:
                    case nFTMotionEventSyncWait:
                    case nFTMotionEventAsyncWait:
                    case nFTMotionEventSetDamageThrown:
                    case nFTMotionEventLoopBegin:
                    case nFTMotionEventLoopEnd:
                    case nFTMotionEventSubroutine:
                    case nFTMotionEventReturn:
                    case nFTMotionEventGoto:
                    case nFTMotionEventPauseScript:
                    case nFTMotionEventEffect:
                    case nFTMotionEventEffectItemHold:
                        ftMainParseMotionEvent(fighter_gobj, fp, ms, ev_kind);
                        break;

                    case nFTMotionEventMakeAttackColl:
                    case nFTMotionEventMakeAttackCollScaled:
                        ftMotionEventAdvance(ms, FTMotionEventMakeAttack);
                        break;

                    case nFTMotionEventSetAttackCollOffset:
                    case nFTMotionEventSetThrow:
                    case nFTMotionEventSetParallelScript:
                        ftMotionEventAdvance(ms, FTMotionEventDouble);
                        break;

                    case nFTMotionEventSetDamageCollPartID:
                        ftMotionEventAdvance(ms, FTMotionEventSetDamageCollPartID);
                        break;

                    default:
                        ftMotionEventAdvance(ms, FTMotionEventDefault);
                        break;
                    }
                }
            }
        }
    }
}

// 0x800E07D4
void ftMainPlayAnim(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->anim_desc.flags.is_use_transn_joint)
    {
        fp->anim_vel = fp->joints[nFTPartsJointTransN]->translate.vec.f;
    }
    ftParamUpdateAnimKeys(fighter_gobj);
    ftParamsUpdateFighterPartsTransform(fp->joints[nFTPartsJointTopN]);
}

// 0x800E0830 - Play fighter animation and run motion scripts normally
void ftMainPlayAnimEventsAll(GObj *fighter_gobj)
{
    ftMainPlayAnim(fighter_gobj);
    ftMainUpdateMotionEventsAll(fighter_gobj);
}

// 0x800E0858 - Play fighter animation and fast-forward motion scripts to current frame
void ftMainPlayAnimEventsForward(GObj *fighter_gobj)
{
    ftMainPlayAnim(fighter_gobj);
    ftMainUpdateMotionEventsForward(fighter_gobj);
}

// 0x800E0880
sb32 ftMainUpdateColAnim(GMColAnim *colanim, GObj *fighter_gobj, sb32 is_muted, sb32 is_effect_skip)
{
    s32 i, j;
    FTStruct *fp;
    GMColScript *cs;
    void *p_script;
    s32 effect_id;
    s32 joint_id;
    u32 flag;
    GMColEventDefault *def;
    Vec3f effect_offset;
    Vec3f effect_scatter;
    u32 ev_kind;
    s32 blend_frames;

    for (i = 0; i < ARRAY_COUNT(colanim->cs); i++)
    {
        cs = &colanim->cs[i]; // What's the point bruh

        if ((colanim->cs[i].p_script != NULL) && (colanim->cs[i].color_event_timer != 0))
        {
            colanim->cs[i].color_event_timer--;
        }
        while ((colanim->cs[i].p_script != NULL) && (cs->color_event_timer == 0))
        {
            ev_kind = gmColEventCast(colanim->cs[i].p_script, GMColEventDefault)->opcode;

            switch (ev_kind)
            {
            case nGMColEventEnd:
                for (j = 0; j < ARRAY_COUNT(colanim->cs); j++)
                {
                    if ((j != i) && (colanim->cs[j].p_script != NULL))
                    {
                        break;
                    }
                }
                if (j == ARRAY_COUNT(colanim->cs))
                {
                    return TRUE;
                }
                else colanim->cs[i].p_script = NULL;
                break;

            case nGMColEventWait:
                colanim->cs[i].color_event_timer = gmColEventCast(colanim->cs[i].p_script, GMColEventDefault)->value,
                gmColEventAdvance(colanim->cs[i].p_script, GMColEventDefault);
                break;

            case nGMColEventGoto:
                gmColEventAdvance(colanim->cs[i].p_script, GMColEventGoto1);
                colanim->cs[i].p_script = gmColEventCast(colanim->cs[i].p_script, GMColEventGoto2)->p_goto;
                break;

            case nGMColEventLoopBegin:
                colanim->cs[i].p_subroutine[colanim->cs[i].script_id++] = lbRelocGetFileData(void*, colanim->cs[i].p_script, sizeof(GMColEventDefault));
                colanim->cs[i].p_subroutine[colanim->cs[i].script_id++] = gmColEventCast(colanim->cs[i].p_script, GMColEventDefault)->value,
                gmColEventAdvance(colanim->cs[i].p_script, GMColEventDefault);
                break;

            case nGMColEventLoopEnd:
                if (--colanim->cs[i].loop_count[colanim->cs[i].script_id - 2] != 0)
                {
                    colanim->cs[i].p_script = colanim->cs[i].p_subroutine[colanim->cs[i].script_id - 2];
                }
                else gmColEventAdvance(colanim->cs[i].p_script, GMColEventDefault), colanim->cs[i].script_id -= 2;
                break;

            case nGMColEventSubroutine:
                gmColEventAdvance(colanim->cs[i].p_script, GMColEventSubroutine1);
                colanim->cs[i].p_subroutine[colanim->cs[i].script_id++] = lbRelocGetFileData(void*, colanim->cs[i].p_script, sizeof(GMColEventSubroutine1));
                colanim->cs[i].p_script = gmColEventCast(colanim->cs[i].p_script, GMColEventSubroutine2)->p_subroutine;
                break;

            case nGMColEventReturn:
                colanim->cs[i].p_script = colanim->cs[i].p_subroutine[--colanim->cs[i].script_id];
                break;

            case nGMColEventSetParallelScript:
                gmColEventAdvance(colanim->cs[i].p_script, GMColEventParallel1);

                if (colanim->cs[1].p_script == NULL)
                {
                    colanim->cs[1].p_script = gmColEventCast(colanim->cs[i].p_script, GMColEventParallel2)->p_script;
                    colanim->cs[1].color_event_timer = 0;
                    colanim->cs[1].script_id = 0;
                }
                gmColEventAdvance(colanim->cs[i].p_script, GMColEventParallel2);
                break;

            case nGMColEventClearColorAll:
                colanim->is_use_color1 = colanim->is_use_color2 = colanim->skeleton_id = 0;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventDefault);
                break;

            case nGMColEventSetColor1:
                colanim->is_use_color1 = TRUE;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventSetRGBA1);

                colanim->color1.r = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->r;
                colanim->color1.g = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->g;
                colanim->color1.b = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->b;
                colanim->color1.a = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->a;

                colanim->color1.ir = colanim->color1.ig = colanim->color1.ib = colanim->color1.ia = 0;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventSetRGBA2);
                break;

            case nGMColEventSetColor2:
                colanim->is_use_color2 = TRUE;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventSetRGBA1);

                colanim->color2.r = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->r;
                colanim->color2.g = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->g;
                colanim->color2.b = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->b;
                colanim->color2.a = gmColEventCast(colanim->cs[i].p_script, GMColEventSetRGBA2)->a;

                colanim->color2.ir = colanim->color2.ig = colanim->color2.ib = colanim->color2.ia = 0;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventSetRGBA2);
                break;

            case nGMColEventBlendColor1:
                blend_frames = gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA1)->blend_frames;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventBlendRGBA1);

                colanim->color1.ir = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->r - colanim->color1.r) / blend_frames;
                colanim->color1.ig = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->g - colanim->color1.g) / blend_frames;
                colanim->color1.ib = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->b - colanim->color1.b) / blend_frames;
                colanim->color1.ia = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->a - colanim->color1.a) / blend_frames;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventBlendRGBA2);
                break;

            case nGMColEventBlendColor2:
                blend_frames = gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA1)->blend_frames;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventBlendRGBA1);

                colanim->color2.ir = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->r - colanim->color2.r) / blend_frames;
                colanim->color2.ig = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->g - colanim->color2.g) / blend_frames;
                colanim->color2.ib = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->b - colanim->color2.b) / blend_frames;
                colanim->color2.ia = (s32) (gmColEventCast(colanim->cs[i].p_script, GMColEventBlendRGBA2)->a - colanim->color2.a) / blend_frames;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventBlendRGBA2);
                break;

            case nGMColEventEffect:
            case nGMColEventEffectItemHoldOffset:
                if (is_effect_skip == FALSE)
                {
                    fp = ftGetStruct(fighter_gobj);

                    joint_id = ftParamGetJointID(fp, gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect1)->joint_id);
                    effect_id = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect1)->effect_id;
                    flag = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect1)->flag;

                    gmColEventAdvance(colanim->cs[i].p_script, GMColEventMakeEffect1);

                    effect_offset.x = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect2)->off_x;
                    effect_offset.y = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect2)->off_y;

                    gmColEventAdvance(colanim->cs[i].p_script, GMColEventMakeEffect2);

                    effect_offset.z = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect3)->off_z;
                    effect_scatter.x = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect3)->rng_x;

                    gmColEventAdvance(colanim->cs[i].p_script, GMColEventMakeEffect3);

                    effect_scatter.y = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect4)->rng_y;
                    effect_scatter.z = gmColEventCast(colanim->cs[i].p_script, GMColEventMakeEffect4)->rng_z;

                    gmColEventAdvance(colanim->cs[i].p_script, GMColEventMakeEffect4);

                    ftParamMakeEffect(fighter_gobj, effect_id, joint_id, &effect_offset, &effect_scatter, fp->lr, (ev_kind == nGMColEventEffectItemHoldOffset) ? TRUE : FALSE, flag);
                }
                else gmColEventAdvance(colanim->cs[i].p_script, GMColEventMakeEffect);
                break;

            case nGMColEventSetLight:
                colanim->is_use_light = TRUE;

                colanim->light_angle_x = gmColEventCast(colanim->cs[i].p_script, GMColEventSetLight)->light1;
                colanim->light_angle_y = gmColEventCast(colanim->cs[i].p_script, GMColEventSetLight)->light2;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventSetLight);
                break;

            case nGMColEventClearLight:
                colanim->is_use_light = FALSE;

                gmColEventAdvance(colanim->cs[i].p_script, GMColEventDefault);
                break;

            case nGMColEventPlayFGM:
                if (is_muted == FALSE)
                {
                    func_800269C0_275C0(gmColEventCastAdvance(colanim->cs[i].p_script, GMColEventDefault)->value);
                }
                else gmColEventAdvance(colanim->cs[i].p_script, GMColEventDefault);
                break;

            case nGMColEventSetSkeletonID:
                colanim->skeleton_id = gmColEventCastAdvance(colanim->cs[i].p_script, GMColEventDefault)->value;
                break;

            default:
                break;
            }
        }
    }
    if (colanim->is_use_color1)
    {
        colanim->color1.r += colanim->color1.ir;
        colanim->color1.g += colanim->color1.ig;
        colanim->color1.b += colanim->color1.ib;
        colanim->color1.a += colanim->color1.ia;
    }
    if (colanim->is_use_color2)
    {
        colanim->color2.r += colanim->color2.ir;
        colanim->color2.g += colanim->color2.ig;
        colanim->color2.b += colanim->color2.ib;
        colanim->color2.a += colanim->color2.ia;
    }
    if (colanim->length != 0)
    {
        colanim->length--;

        if (colanim->length == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 0x800E11C8
void ftMainRunUpdateColAnim(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    while (ftMainUpdateColAnim(&fp->colanim, fighter_gobj, fp->is_muted, fp->is_effect_skip) != FALSE)
    {
        ftParamResetStatUpdateColAnim(fighter_gobj);
    }
}

// 0x800E1260
void ftMainProcUpdateInterrupt(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *other_fp;
    FTAttributes *this_attr;
    FTAttributes *other_attr;
    FTPlayerInput *pl;
    FTComputerInput *cp;
    SYController *controller;
    GObj *other_gobj;
    f32 jostle_dist_x;
    f32 dist_z;
    sb32 is_check_self;
    sb32 is_jostle;
    u16 button_tap_mask;
    u16 button_hold;
    u16 button_hold_com;
    f32 this_jostle;

    if (!(this_fp->is_control_disable))
    {
        this_fp->status_total_tics++;
    }
    if (!(this_fp->is_control_disable))
    {
        pl = &this_fp->input.pl;

        pl->stick_prev.x = pl->stick_range.x;
        pl->stick_prev.y = pl->stick_range.y;

        switch (this_fp->pkind)
        {
        case nFTPlayerKindMan:
            controller = this_fp->input.controller;
            button_hold = controller->button_hold;

            if (button_hold & R_TRIG)
            {
                button_hold |= (A_BUTTON | Z_TRIG);
            }
            pl->stick_range.x = controller->stick_range.x;
            pl->stick_range.y = controller->stick_range.y;

            button_tap_mask = (button_hold ^ pl->button_hold) & button_hold;

            pl->button_tap = (this_fp->hitlag_tics != 0) ? pl->button_tap | button_tap_mask : button_tap_mask;

            button_tap_mask = (button_hold ^ pl->button_hold) & pl->button_hold;

            pl->button_release = (this_fp->hitlag_tics != 0) ? pl->button_release | button_tap_mask : button_tap_mask;

            pl->button_hold = button_hold;
            break;

        case nFTPlayerKindCom:
            ftComputerProcessAll(fighter_gobj);
            goto next;

        case nFTPlayerKindKey:
        case nFTPlayerKindGameKey:
            ftKeyProcessKeyEvents(fighter_gobj);

        next:
            cp = &this_fp->input.cp;
            button_hold_com = this_fp->input.cp.button_inputs;

            if (button_hold_com & R_TRIG)
            {
                button_hold_com |= (A_BUTTON | Z_TRIG);
            }
            pl->stick_range.x = cp->stick_range.x;
            pl->stick_range.y = cp->stick_range.y;

            button_tap_mask = (button_hold_com ^ pl->button_hold) & button_hold_com;

            pl->button_tap = (this_fp->hitlag_tics != 0) ? pl->button_tap | button_tap_mask : button_tap_mask;

            button_tap_mask = (button_hold_com ^ pl->button_hold) & pl->button_hold;

            pl->button_release = (this_fp->hitlag_tics != 0) ? pl->button_release | button_tap_mask : button_tap_mask;

            pl->button_hold = button_hold_com;

            break;
        }
        if (pl->stick_range.x > I_CONTROLLER_RANGE_MAX)
        {
            pl->stick_range.x = I_CONTROLLER_RANGE_MAX;
        }
        if (pl->stick_range.x < -I_CONTROLLER_RANGE_MAX)
        {
            pl->stick_range.x = -I_CONTROLLER_RANGE_MAX;
        }
        if (pl->stick_range.y > I_CONTROLLER_RANGE_MAX)
        {
            pl->stick_range.y = I_CONTROLLER_RANGE_MAX;
        }
        if (pl->stick_range.y < -I_CONTROLLER_RANGE_MAX)
        {
            pl->stick_range.y = -I_CONTROLLER_RANGE_MAX;
        }
        if (gSCManagerBackupData.error_flags & LBBACKUP_ERROR_HALFSTICKRANGE)
        {
            pl->stick_range.x *= 0.5F;
            pl->stick_range.y *= 0.5F;
        }
        if (pl->stick_range.x >= 20)
        {
            if (pl->stick_prev.x >= 20)
            {
                this_fp->tap_stick_x++, this_fp->hold_stick_x++;
            }
            else this_fp->tap_stick_x = this_fp->hold_stick_x = 1;
        }
        else if (pl->stick_range.x <= -20)
        {
            if (pl->stick_prev.x <= -20)
            {
                this_fp->tap_stick_x++, this_fp->hold_stick_x++;
            }
            else this_fp->tap_stick_x = this_fp->hold_stick_x = 1;
        }
        else this_fp->tap_stick_x = this_fp->hold_stick_x = FTINPUT_STICKBUFFER_TICS_MAX;

        if (this_fp->tap_stick_x > FTINPUT_STICKBUFFER_TICS_MAX)
        {
            this_fp->tap_stick_x = FTINPUT_STICKBUFFER_TICS_MAX;
        }
        if (this_fp->hold_stick_x > FTINPUT_STICKBUFFER_TICS_MAX)
        {
            this_fp->hold_stick_x = FTINPUT_STICKBUFFER_TICS_MAX;
        }
        if (pl->stick_range.y >= 20)
        {
            if (pl->stick_prev.y >= 20)
            {
                this_fp->tap_stick_y++, this_fp->hold_stick_y++;
            }
            else this_fp->tap_stick_y = this_fp->hold_stick_y = 1;
        }
        else if (pl->stick_range.y <= -20)
        {
            if (pl->stick_prev.y <= -20)
            {
                this_fp->tap_stick_y++, this_fp->hold_stick_y++;
            }
            else this_fp->tap_stick_y = this_fp->hold_stick_y = 1;
        }
        else this_fp->tap_stick_y = this_fp->hold_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;

        if (this_fp->tap_stick_y > FTINPUT_STICKBUFFER_TICS_MAX)
        {
            this_fp->tap_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
        }
        if (this_fp->hold_stick_y > FTINPUT_STICKBUFFER_TICS_MAX)
        {
            this_fp->hold_stick_y = FTINPUT_STICKBUFFER_TICS_MAX;
        }
    }
    if (this_fp->tics_since_last_z < FTINPUT_ZTRIGLAST_TICS_MAX)
    {
        this_fp->tics_since_last_z++;
    }
    if (this_fp->input.pl.button_tap & this_fp->input.button_mask_z)
    {
        this_fp->tics_since_last_z = 0;
    }
    if (this_fp->hitlag_tics != 0)
    {
        this_fp->hitlag_tics--;

        if (this_fp->hitlag_tics == 0)
        {
            this_fp->is_knockback_paused = FALSE;

            if (this_fp->proc_lagend != NULL)
            {
                this_fp->proc_lagend(fighter_gobj);
            }
        }
    }
    this_fp->is_events_forward = TRUE;

    if (this_fp->hitlag_tics == 0)
    {
        ftMainPlayAnimEventsAll(fighter_gobj);
    }
    ftMainRunUpdateColAnim(fighter_gobj);

    if (this_fp->intangible_tics != 0)
    {
        this_fp->intangible_tics--;

        if (this_fp->intangible_tics == 0)
        {
            this_fp->special_hitstatus = (this_fp->invincible_tics != FALSE) ? nGMHitStatusInvincible : nGMHitStatusNormal;

            if (this_fp->colanim.colanim_id == 0xA)
            {
                ftParamResetStatUpdateColAnim(fighter_gobj);
            }
        }
    }
    if (this_fp->invincible_tics != 0)
    {
        this_fp->invincible_tics--;

        if ((this_fp->invincible_tics == 0) && (this_fp->intangible_tics == 0))
        {
            this_fp->special_hitstatus = nGMHitStatusNormal;

            if (this_fp->colanim.colanim_id == 0xA)
            {
                ftParamResetStatUpdateColAnim(fighter_gobj);
            }
        }
    }
    if (this_fp->star_invincible_tics != 0)
    {
        this_fp->star_invincible_tics--;

        if (this_fp->star_invincible_tics == 0)
        {
            this_fp->star_hitstatus = nGMHitStatusNormal;

#if defined(REGION_US)
            if (this_fp->colanim.colanim_id == 0x4A)
#else
            if (this_fp->colanim.colanim_id == 0x49)
#endif
            {
                ftParamResetStatUpdateColAnim(fighter_gobj);
            }
        }
        else if (this_fp->star_invincible_tics == ITSTAR_WARN_BEGIN_FRAME)
        {
            ftParamTryUpdateItemMusic();
        }
    }
    if (this_fp->damage_heal != 0)
    {
        this_fp->damage_heal--;

        if (this_fp->percent_damage != 0)
        {
            this_fp->percent_damage--;

            func_800269C0_275C0(nSYAudioFGMPlayerHeal);

            gSCManagerBattleState->players[this_fp->player].stock_damage_all = this_fp->percent_damage;
        }
        if (this_fp->percent_damage == 0)
        {
            this_fp->damage_heal = 0;
        }
        if ((this_fp->damage_heal == 0) && (this_fp->colanim.colanim_id == 0x9))
        {
            ftParamResetStatUpdateColAnim(fighter_gobj);
        }
    }
    if ((this_fp->item_gobj != NULL) && (this_fp->status_id != nFTCommonStatusLightGet) && (itGetStruct(this_fp->item_gobj)->kind == nITKindHammer))
    {
        ftHammerUpdateStats(fighter_gobj);
    }
    if (this_fp->shuffle_tics != 0)
    {
        this_fp->shuffle_tics--;

        this_fp->shuffle_frame_index++;

        if (this_fp->shuffle_frame_index == this_fp->shuffle_index_max)
        {
            this_fp->shuffle_frame_index = 0;
        }
    }
    if (this_fp->proc_passive != NULL)
    {
        this_fp->proc_passive(fighter_gobj);
    }
    if (this_fp->hitlag_tics == 0)
    {
        if ((this_fp->playertag_wait > 1) && !(this_fp->is_control_disable))
        {
            this_fp->playertag_wait--;
        }
        if (this_fp->proc_update != NULL)
        {
            this_fp->proc_update(fighter_gobj);
        }
        if (this_fp->proc_interrupt != NULL)
        {
            this_fp->proc_interrupt(fighter_gobj);
        }
        if (!(this_fp->is_hitstun))
        {
            gSCManagerBattleState->players[this_fp->player].combo_damage_foe = 0;
            gSCManagerBattleState->players[this_fp->player].combo_count_foe = 0;
        }
        is_jostle = FALSE;

        this_fp->physics.vel_jostle_x = this_fp->physics.vel_jostle_z = 0.0F;

        if ((this_fp->ga == nMPKineticsGround) && !(this_fp->is_jostle_ignore))
        {
            other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

            is_check_self = FALSE;

            while (other_gobj != NULL)
            {
                other_fp = ftGetStruct(other_gobj);

                if ((fighter_gobj != other_gobj) && (other_fp->capture_gobj == NULL))
                {
                    if ((other_fp->ga == nMPKineticsGround) && (this_fp->coll_data.floor_line_id == other_fp->coll_data.floor_line_id))
                    {
                        this_attr = this_fp->attr;
                        other_attr = other_fp->attr;

                        this_jostle = this_fp->attr->jostle_width;

                        jostle_dist_x = (DObjGetStruct(fighter_gobj)->translate.vec.f.x + (this_attr->jostle_x * this_fp->lr)) - (DObjGetStruct(other_gobj)->translate.vec.f.x + (other_attr->jostle_x * other_fp->lr));

                        if (ABS(jostle_dist_x) < (this_jostle + other_attr->jostle_width))
                        {
                            is_jostle = TRUE;

                            if (jostle_dist_x == 0.0F)
                            {
                                this_fp->physics.vel_jostle_x += (6.75F * ((is_check_self != FALSE) ? -1 : 1));
                            }
                            else this_fp->physics.vel_jostle_x += (6.75F * ((jostle_dist_x < 0.0F) ? -1 : 1));

                            dist_z = DObjGetStruct(fighter_gobj)->translate.vec.f.z - DObjGetStruct(other_gobj)->translate.vec.f.z;

                            if (dist_z == 0.0F)
                            {
                                if (jostle_dist_x == 0.0F)
                                {
                                    this_fp->physics.vel_jostle_z += (3.0F * ((is_check_self != FALSE) ? -1 : 1));
                                }
                                else this_fp->physics.vel_jostle_z += (3.0F * ((jostle_dist_x < 0.0F) ? 1 : -1));
                            }
                            else this_fp->physics.vel_jostle_z += (3.0F * ((dist_z < 0.0F) ? -1 : 1));
                        }
                    }
                }
                else is_check_self = TRUE;

                other_gobj = other_gobj->link_next;
            }
            if ((is_jostle == FALSE) && (DObjGetStruct(fighter_gobj)->translate.vec.f.z != 0.0F))
            {
                this_fp->physics.vel_jostle_z = ((DObjGetStruct(fighter_gobj)->translate.vec.f.z < 0.0F) ? +1 : -1) * 3.0F;
            }
        }
    }
    this_fp->coll_data.vel_push.x = this_fp->coll_data.vel_push.y = this_fp->coll_data.vel_push.z = 0.0F;
}

// 0x800E1CF0
void ftMainClearGroundElementsAll(void)
{
    s32 i;

    sFTMainGroundObstaclesNum = sFTMainGroundHazardsNum = 0;

    for (i = 0; i < ARRAY_COUNT(sFTMainGroundObstacles); i++)
    {
        sFTMainGroundObstacles[i].gobj = NULL;
    }
    for (i = 0; i < ARRAY_COUNT(sFTMainGroundHazards); i++)
    {
        sFTMainGroundHazards[i].gobj = NULL;
    }
}

// 0x800E1D48
sb32 ftMainCheckAddGroundObstacle(GObj *gobj, sb32(*proc_update)(GObj*, GObj*, s32*))
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sFTMainGroundObstacles); i++)
    {
        if (sFTMainGroundObstacles[i].gobj == NULL)
        {
            sFTMainGroundObstacles[i].gobj = gobj;
            sFTMainGroundObstacles[i].proc_update = proc_update;
            sFTMainGroundObstaclesNum++;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x800E1D9C
void ftMainClearGroundObstacle(GObj *gobj)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sFTMainGroundObstacles); i++)
    {
        if (sFTMainGroundObstacles[i].gobj == gobj)
        {
            sFTMainGroundObstacles[i].gobj = NULL;
            sFTMainGroundObstaclesNum--;

            break;
        }
    }
}

// 0x800E1DE8
sb32 ftMainCheckAddGroundHazard(GObj *gobj, sb32(*proc_update)(GObj*, GObj*, GRAttackColl**, s32*))
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sFTMainGroundHazards); i++)
    {
        if (sFTMainGroundHazards[i].gobj == NULL)
        {
            sFTMainGroundHazards[i].gobj = gobj;
            sFTMainGroundHazards[i].proc_update = proc_update;

            sFTMainGroundHazardsNum++;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x800E1E3C - Remove map environment object by GObj pointer match
void ftMainClearHazard(GObj *gobj)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sFTMainGroundHazards); i++)
    {
        if (sFTMainGroundHazards[i].gobj == gobj)
        {
            sFTMainGroundHazards[i].gobj = NULL;
            sFTMainGroundHazardsNum--;

            break;
        }
    }
}

// 0x800E1E88
void ftMainSetHitHazard(GObj *gobj, GObj *fighter_gobj, FTStruct *fp, s32 kind)
{
    switch (kind)
    {
    case nGMHitEnvironmentTwister:
        ftCommonTwisterSetStatus(fighter_gobj, gobj);
        break;

    case nGMHitEnvironmentTaruCann:
        ftCommonTaruCannSetStatus(fighter_gobj, gobj);
        break;
    }
}

// 0x800E1EE8
void ftMainSearchHitHazard(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GRObstacle *gh = &sFTMainGroundObstacles[0];

    if (!(fp->is_ghost))
    {
        s32 i;

        if (!(fp->hitlag_tics))
        {
            if (fp->twister_wait)
            {
                fp->twister_wait--;
            }
            if (fp->tarucann_wait)
            {
                fp->tarucann_wait--;
            }
        }
        for (i = 0; i < sFTMainGroundObstaclesNum; i++, gh++)
        {
            if (gh->gobj != NULL)
            {
                s32 kind;

                if (gh->proc_update(gh->gobj, fighter_gobj, &kind) != FALSE)
                {
                    ftMainSetHitHazard(gh->gobj, fighter_gobj, fp, kind);
                }
            }
        }
    }
}

// 0x800E1FE0
void ftMainUpdateVelDamageGround(FTStruct *fp, f32 move)
{
    if (fp->physics.vel_damage_ground < 0.0F)
    {
        fp->physics.vel_damage_ground += move;

        if (fp->physics.vel_damage_ground > 0.0F)
        {
            fp->physics.vel_damage_ground = 0.0F;
        }
    }
    else
    {
        fp->physics.vel_damage_ground -= move;

        if (fp->physics.vel_damage_ground < 0.0F)
        {
            fp->physics.vel_damage_ground = 0.0F;
        }
    }
}

// 0x800E2048
void ftMainProcPhysicsMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f *topn_translate = &fp->joints[nFTPartsJointTopN]->translate.vec.f;
    Vec3f *coll_translate = &fp->coll_data.pos_prev;
    Vec3f *floor_angle = &fp->coll_data.floor_angle;
    Vec3f *vel_damage_air;
    s32 unused[2];
    f32 size_mul;
    f32 tan;
    f32 damage_angle;
    Vec2f vel_damage_new;
    s32 i;

    *coll_translate = *topn_translate;

    if (fp->hitlag_tics == 0)
    {
        if (fp->cliffcatch_wait != 0)
        {
            fp->cliffcatch_wait--;
        }
        if (fp->proc_physics != NULL)
        {
            fp->proc_physics(fighter_gobj);
        }
        vel_damage_air = &fp->physics.vel_damage_air;

        if ((vel_damage_air->x != 0.0F) || (vel_damage_air->y != 0.0F))
        {
            if (fp->ga == nMPKineticsAir)
            {
                damage_angle = syUtilsArcTan2(vel_damage_air->y, vel_damage_air->x);

                vel_damage_new.y = vel_damage_air->x;
                vel_damage_new.x = vel_damage_air->y;

                vel_damage_air->x -= (1.7F * __cosf(damage_angle));
                vel_damage_air->y -= (1.7F * __sinf(damage_angle));

                if (((vel_damage_air->x * vel_damage_new.y) < 0.0F) || ((vel_damage_air->y * vel_damage_new.x) < 0.0F))
                {
                    vel_damage_air->x = vel_damage_air->y = 0.0F;
                }
                fp->physics.vel_damage_ground = 0.0F;
            }
            else
            {
                if (fp->physics.vel_damage_ground == 0.0F)
                {
                    fp->physics.vel_damage_ground = fp->physics.vel_damage_air.x;
                }
                ftMainUpdateVelDamageGround(fp, dMPCollisionMaterialFrictions[fp->coll_data.floor_flags & MAP_VERTEX_MAT_MASK] * fp->attr->traction * 0.25F);

                vel_damage_air->x = (floor_angle->y * fp->physics.vel_damage_ground);
                vel_damage_air->y = (-floor_angle->x * fp->physics.vel_damage_ground);
            }
        }
        syVectorAdd3D(topn_translate, &fp->physics.vel_air);

        topn_translate->x += vel_damage_air->x;
        topn_translate->y += vel_damage_air->y;
    }
    if (fp->proc_lagupdate != NULL)
    {
        fp->proc_lagupdate(fighter_gobj);
    }
    syVectorDiff3D(&fp->coll_data.pos_diff, topn_translate, coll_translate);

    if ((fp->ga == nMPKineticsGround) && (fp->coll_data.floor_line_id != -1) && (fp->coll_data.floor_line_id != -2) && (mpCollisionCheckExistLineID(fp->coll_data.floor_line_id) != FALSE))
    {
        mpCollisionGetSpeedLineID(fp->coll_data.floor_line_id, &fp->coll_data.vel_speed);
        syVectorAdd3D(topn_translate, &fp->coll_data.vel_speed);
    }
    else fp->coll_data.vel_speed.x = fp->coll_data.vel_speed.y = fp->coll_data.vel_speed.z = 0.0F;

    ftCommonDeadCheckInterruptCommon(fighter_gobj);

    if ((fp->coll_data.pos_prev.y >= gMPCollisionGroundData->alt_warning) && (topn_translate->y < gMPCollisionGroundData->alt_warning) && (fp->fkind != nFTKindBoss))
    {
        func_800269C0_275C0(nSYAudioFGMAltitudeWarn);
    }
    if (fp->public_knockback != 0)
    {
        if ((fp->joints[nFTPartsJointTopN]->translate.vec.f.x > (gMPCollisionBounds.current.left + 450.0F)) && (fp->joints[nFTPartsJointTopN]->translate.vec.f.x < (gMPCollisionBounds.current.right - 450.0F)))
        {
            fp->public_knockback = 0.0F;
        }
    }
    if (fp->proc_map != NULL)
    {
        fp->coll_data.mask_prev = fp->coll_data.mask_curr;
        fp->coll_data.mask_curr = 0;
        fp->coll_data.is_coll_end = FALSE;
        fp->coll_data.mask_stat = 0;
        fp->coll_data.mask_unk = 0;

        fp->proc_map(fighter_gobj);

        if (fp->fkind == nFTKindKirby)
        {
            ftParamKirbyTryMakeMapStarEffect(fighter_gobj);
        }
    }
    if (fp->proc_slope != NULL)
    {
        fp->proc_slope(fighter_gobj);
    }
    ftParamsUpdateFighterPartsTransformAll(fp->joints[nFTPartsJointTopN]);

    if (fp->hitlag_tics == 0)
    {
        ftMainUpdateMotionEventsForwardEffect(fighter_gobj);
    }
    if (fp->hitlag_tics == 0)
    {
        if (fp->proc_accessory != NULL)
        {
            fp->proc_accessory(fighter_gobj);
        }
    }
    fp->is_events_forward = FALSE;

    for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
    {
        FTAttackColl *attack_coll = &fp->attack_colls[i];

        switch (attack_coll->attack_state)
        {
        case nGMAttackStateOff:
            break;

        case nGMAttackStateNew:
            attack_coll->pos_curr = attack_coll->offset;

            if (attack_coll->is_scale_pos)
            {
                size_mul = 1.0F / fp->attr->size;

                attack_coll->pos_curr.x *= size_mul;
                attack_coll->pos_curr.y *= size_mul;
                attack_coll->pos_curr.z *= size_mul;
            }
            gmCollisionGetFighterPartsWorldPosition(attack_coll->joint, &attack_coll->pos_curr);

            attack_coll->attack_state = nGMAttackStateTransfer;

            attack_coll->attack_matrix.unk_fthitmtx_0x0 = FALSE;
            attack_coll->attack_matrix.unk_fthitmtx_0x44 = 0.0F;
            break;

        case nGMAttackStateTransfer:
            attack_coll->attack_state = nGMAttackStateInterpolate;

            /* fallthrough */

        case nGMAttackStateInterpolate:
            attack_coll->pos_prev = attack_coll->pos_curr;
            attack_coll->pos_curr = attack_coll->offset;

            if (attack_coll->is_scale_pos)
            {
                size_mul = 1.0F / fp->attr->size;

                attack_coll->pos_curr.x *= size_mul;
                attack_coll->pos_curr.y *= size_mul;
                attack_coll->pos_curr.z *= size_mul;
            }
            gmCollisionGetFighterPartsWorldPosition(attack_coll->joint, &attack_coll->pos_curr);

            attack_coll->attack_matrix.unk_fthitmtx_0x0 = FALSE;
            attack_coll->attack_matrix.unk_fthitmtx_0x44 = 0.0F;

            break;
        }
    }
}

// 0x800E2604
void ftMainProcPhysicsMapDefault(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->capture_gobj == NULL || fp->is_catch_or_capture) && (fp->catch_gobj == NULL || !(fp->is_catch_or_capture)))
    {
        ftMainProcPhysicsMap(fighter_gobj);
    }
}

// 0x800E2660
void ftMainProcPhysicsMapCapture(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->capture_gobj != NULL && !(fp->is_catch_or_capture)) || (fp->catch_gobj != NULL && fp->is_catch_or_capture))
    {
        ftMainProcPhysicsMap(fighter_gobj);
    }
}

// 0x800E26BC
void ftMainSetHitInteractStats(FTStruct *fp, u32 attack_group_id, GObj *victim_gobj, s32 attack_type, u32 victim_group_id, sb32 ignore_damage_or_hit)
{
    s32 i, j;

    for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
    {
        if (i == ARRAY_COUNT(fp->attack_colls)); // WAT

        if ((fp->attack_colls[i].attack_state != nGMAttackStateOff) && (attack_group_id == fp->attack_colls[i].group_id))
        {
            for (j = 0; j < ARRAY_COUNT(fp->attack_colls[i].attack_records); j++)
            {
                if (victim_gobj == fp->attack_colls[i].attack_records[j].victim_gobj)
                {
                    switch (attack_type)
                    {
                    case nGMHitTypeDamage:
                        fp->attack_colls[i].attack_records[j].victim_flags.is_interact_hurt = TRUE;
                        break;

                    case nGMHitTypeShield:
                        fp->attack_colls[i].attack_records[j].victim_flags.is_interact_shield = TRUE;
                        break;

                    case nGMHitTypeAttack:
                        fp->attack_colls[i].attack_records[j].victim_flags.group_id = victim_group_id;
                        break;

                    default:
                        break;
                    }
                    break;
                }
            }
            if (j == ARRAY_COUNT(fp->attack_colls[i].attack_records))
            {
                for (j = 0; j < ARRAY_COUNT(fp->attack_colls[i].attack_records); j++)
                {
                    if (fp->attack_colls[i].attack_records[j].victim_gobj == NULL) break;
                }
                if (j == ARRAY_COUNT(fp->attack_colls[i].attack_records)) j = 0;

                fp->attack_colls[i].attack_records[j].victim_gobj = victim_gobj;

                switch (attack_type)
                {
                case nGMHitTypeDamage:
                    fp->attack_colls[i].attack_records[j].victim_flags.is_interact_hurt = TRUE;
                    break;

                case nGMHitTypeShield:
                    fp->attack_colls[i].attack_records[j].victim_flags.is_interact_shield = TRUE;
                    break;

                case nGMHitTypeAttack:
                    fp->attack_colls[i].attack_records[j].victim_flags.group_id = victim_group_id;
                    break;

                default:
                    break;
                }
            }
            if (ignore_damage_or_hit == 0)
            {
                gFTMainIsDamageDetect[i] = FALSE;
            }
            else gFTMainIsAttackDetect[i] = FALSE;
        }
    }
}

// 0x800E287C
void ftMainSetHitRebound(GObj *attacker_gobj, FTStruct *fp, FTAttackColl *attack_coll, GObj *victim_gobj)
{
    if (fp->attack_shield_push < attack_coll->damage)
    {
        fp->attack_shield_push = attack_coll->damage;

        if ((attack_coll->can_rebound) && (fp->ga == nMPKineticsGround))
        {
#if defined(REGION_US)
            fp->attack_rebound = (fp->attack_shield_push * 1.62F) + 4.0F;
#else
            fp->attack_rebound = (fp->attack_shield_push * 1.75F) + 4.0F;
#endif
            fp->hit_lr = (DObjGetStruct(attacker_gobj)->translate.vec.f.x < DObjGetStruct(victim_gobj)->translate.vec.f.x) ? +1 : -1;
        }
    }
}

// 0x800E2910
void ftMainUpdateAttackStatFighter(FTStruct *other_fp, FTAttackColl *other_hit, FTStruct *this_fp, FTAttackColl *this_hit, GObj *other_gobj, GObj *this_gobj)
{
    Vec3f impact_pos;

    gmCollisionGetFighterAttacksPosition(&impact_pos, this_hit, other_hit);

    if ((this_hit->damage - 10) < other_hit->damage)
    {
        ftMainSetHitInteractStats(this_fp, this_hit->group_id, other_gobj, nGMHitTypeAttack, other_hit->group_id, 1);
        ftMainSetHitRebound(this_gobj, this_fp, this_hit, other_gobj);
        efManagerSetOffMakeEffect(&impact_pos, this_hit->damage);

        if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (this_hit->damage >= 20) && (other_fp->player == gSCManagerSceneData.player))
        {
            gSC1PGameBonusGiantImpact = TRUE;
        }
    }
    if ((other_hit->damage - 10) < this_hit->damage)
    {
        ftMainSetHitInteractStats(other_fp, other_hit->group_id, this_gobj, nGMHitTypeAttack, this_hit->group_id, 0);
        ftMainSetHitRebound(other_gobj, other_fp, other_hit, this_gobj);
        efManagerSetOffMakeEffect(&impact_pos, other_hit->damage);

        if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (other_hit->damage >= 20) && (this_fp->player == gSCManagerSceneData.player))
        {
            gSC1PGameBonusGiantImpact = TRUE;
        }
    }
}

// 0x800E2A90
void ftMainUpdateShieldStatFighter(FTStruct *attacker_fp, FTAttackColl *attack_coll, FTStruct *victim_fp, GObj *attacker_gobj, GObj *victim_gobj)
{
    Vec3f impact_pos;

    ftMainSetHitInteractStats(attacker_fp, attack_coll->group_id, victim_gobj, nGMHitTypeShield, 0, 0);

    if (attacker_fp->attack_shield_push < attack_coll->damage)
    {
        attacker_fp->attack_shield_push = attack_coll->damage;
    }
    victim_fp->shield_damage_total += (attack_coll->damage + attack_coll->shield_damage);

    if (victim_fp->shield_damage < attack_coll->damage)
    {
        victim_fp->shield_damage = attack_coll->damage;

        victim_fp->shield_lr = (DObjGetStruct(victim_gobj)->translate.vec.f.x < DObjGetStruct(attacker_gobj)->translate.vec.f.x) ? +1 : -1;

        victim_fp->shield_player = attacker_fp->player;
    }
    gmCollisionGetFighterAttackShieldPosition(&impact_pos, attack_coll, victim_gobj, victim_fp->joints[nFTPartsJointYRotN]);
    efManagerSetOffMakeEffect(&impact_pos, attack_coll->damage);
}

// 0x800E2B88
void ftMainUpdateCatchStatFighter(FTStruct *attacker_fp, FTAttackColl *attack_coll, FTStruct *victim_fp, GObj *attacker_gobj, GObj *victim_gobj)
{
    f32 dist;

    ftMainSetHitInteractStats(attacker_fp, attack_coll->group_id, victim_gobj, nGMHitTypeDamage, 0, 1);

    if (DObjGetStruct(victim_gobj)->translate.vec.f.x < DObjGetStruct(attacker_gobj)->translate.vec.f.x)
    {
        dist = -(DObjGetStruct(victim_gobj)->translate.vec.f.x - DObjGetStruct(attacker_gobj)->translate.vec.f.x);
    }
    else dist = DObjGetStruct(victim_gobj)->translate.vec.f.x - DObjGetStruct(attacker_gobj)->translate.vec.f.x;

    if (dist < attacker_fp->search_gobj_dist)
    {
        attacker_fp->search_gobj_dist = dist;
        attacker_fp->search_gobj = victim_gobj;
    }
}

// 0x800E2C24
void ftMainPlayHitSFX(FTStruct *fp, FTAttackColl *attack_coll)
{
    if ((fp->p_sfx != NULL) && (fp->p_sfx->sfx_id != 0) && (fp->p_sfx->sfx_id == fp->sfx_id))
    {
        func_80026738_27338(fp->p_sfx);
    }
    fp->p_sfx = NULL, fp->sfx_id = 0;

    lbCommonMakePositionFGM(dFTMainHitCollisionFGMs[attack_coll->fgm_kind][attack_coll->fgm_level], fp->joints[nFTPartsJointTopN]->translate.vec.f.x);
}

// 0x800E2CC0
sb32 ftMainCheckGetUpdateDamage(FTStruct *fp, s32 *damage)
{
    if (fp->is_damage_resist)
    {
        fp->damage_resist -= *damage;

        if (fp->damage_resist <= 0)
        {
            fp->is_damage_resist = FALSE;

            *damage = -fp->damage_resist;
        }
    }
    if (!(fp->is_damage_resist))
    {
        fp->damage_queue += *damage;

        if (fp->damage_lag < *damage)
        {
            fp->damage_lag = *damage;
        }
        return TRUE;
    }
    else return FALSE;
}

// 0x800E2D44
void ftMainUpdateDamageStatFighter(FTStruct *attacker_fp, FTAttackColl *attack_coll, FTStruct *victim_fp, FTDamageColl *damage_coll, GObj *attacker_gobj, GObj *victim_gobj)
{
    s32 damage;
    s32 attacker_player;
    s32 attacker_player_num;
    s32 unused;
    Vec3f impact_pos;

    ftMainSetHitInteractStats(attacker_fp, attack_coll->group_id, victim_gobj, nGMHitTypeDamage, 0, FALSE);

    damage = ftParamGetCapturedDamage(victim_fp, attack_coll->damage);

    if (attacker_fp->attack_damage < damage)
    {
        attacker_fp->attack_damage = damage;
    }
    if
    (
        (victim_fp->special_hitstatus == nGMHitStatusNormal) &&
        (victim_fp->star_hitstatus == nGMHitStatusNormal) &&
        (victim_fp->hitstatus == nGMHitStatusNormal)      &&
        (damage_coll->hitstatus == nGMHitStatusNormal)
    )
    {
        if (ftMainCheckGetUpdateDamage(victim_fp, &damage) != FALSE)
        {
            if (attacker_fp->throw_gobj != NULL)
            {
                attacker_player = attacker_fp->throw_player;
                attacker_player_num = attacker_fp->throw_player_num;
            }
            else
            {
                attacker_player = attacker_fp->player;
                attacker_player_num = attacker_fp->player_num;
            }
            if (sFTMainHitLogID < ARRAY_COUNT(sFTMainHitLogs))
            {
                FTHitLog *hitlog = &sFTMainHitLogs[sFTMainHitLogID];

                hitlog->attacker_object_class = nFTHitLogObjectFighter;
                hitlog->attack_coll = attack_coll;
                hitlog->attacker_gobj = attacker_gobj;
                hitlog->damage_coll = damage_coll;
                hitlog->attacker_player = attacker_player;
                hitlog->attacker_player_num = attacker_player_num;

                sFTMainHitLogID++;
            }
            ftParamUpdatePlayerBattleStats(attacker_player, victim_fp->player, damage);
            ftParamUpdateStaleQueue(attacker_player, victim_fp->player, attack_coll->motion_attack_id, attack_coll->motion_count);
        }
        else
        {
            gmCollisionGetFighterAttackDamagePosition(&impact_pos, attack_coll, damage_coll);
            efManagerSetOffMakeEffect(&impact_pos, damage);
        }
    }
    else
    {
        gmCollisionGetFighterAttackDamagePosition(&impact_pos, attack_coll, damage_coll);
        efManagerSetOffMakeEffect(&impact_pos, damage);
    }
    ftMainPlayHitSFX(attacker_fp, attack_coll);
}

// 0x800E2F04
void ftMainUpdateAttackStatWeapon(WPStruct *ip, WPAttackColl *wp_attack_coll, s32 index, FTStruct *fp, FTAttackColl *ft_attack_coll, GObj *weapon_gobj, GObj *fighter_gobj)
{
    s32 damage = wpMainGetStaledDamage(ip);
    Vec3f impact_pos;

    gmCollisionGetWeaponAttackFighterAttackPosition(&impact_pos, wp_attack_coll, index, ft_attack_coll);

    if ((ft_attack_coll->damage - 10) < damage)
    {
        ftMainSetHitInteractStats(fp, ft_attack_coll->group_id, weapon_gobj, nGMHitTypeAttack, 0, TRUE);
        ftMainSetHitRebound(fighter_gobj, fp, ft_attack_coll, weapon_gobj);
        efManagerSetOffMakeEffect(&impact_pos, ft_attack_coll->damage);
    }

    if ((damage - 10) < ft_attack_coll->damage)
    {
        wpProcessUpdateHitInteractStats(ip, wp_attack_coll, fighter_gobj, nGMHitTypeAttack, ft_attack_coll->group_id);

        if (ip->hit_attack_damage < damage)
        {
            ip->hit_attack_damage = damage;
        }
        efManagerSetOffMakeEffect(&impact_pos, damage);

        if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && ((damage - 10) >= 10) && (fp->player == gSCManagerSceneData.player))
        {
            gSC1PGameBonusGiantImpact = TRUE;
        }
    }
}

// 0x800E3048
void ftMainUpdateShieldStatWeapon(WPStruct *wp, WPAttackColl *wp_attack_coll, s32 attack_id, FTStruct *fp, GObj *weapon_gobj, GObj *fighter_gobj, f32 angle, Vec3f *dir)
{
    s32 damage = wpMainGetStaledDamage(wp);
    Vec3f impact_pos;

    wpProcessUpdateHitInteractStats(wp, wp_attack_coll, fighter_gobj, (wp_attack_coll->can_rehit_shield) ? nGMHitTypeShieldRehit : nGMHitTypeShield, 0);

    if (wp->hit_shield_damage < damage)
    {
        wp->hit_shield_damage = damage;

        wp->shield_collide_angle = angle;

        wp->shield_collide_dir.x = 0.0F;
        wp->shield_collide_dir.y = 0.0F;
        wp->shield_collide_dir.z = (fp->lr == +1) ? -dir->x : dir->x;

        syVectorNorm3D(&wp->shield_collide_dir);
    }
    fp->shield_damage_total += damage + wp_attack_coll->shield_damage;

    if (fp->shield_damage < damage)
    {
        fp->shield_damage = damage;

        fp->shield_lr = (wp->physics.vel_air.x < 0.0F) ? +1 : -1;

        fp->shield_player = wp->player;
    }
    gmCollisionGetWeaponAttackShieldPosition(&impact_pos, wp_attack_coll, attack_id, fighter_gobj, fp->joints[nFTPartsJointYRotN]);
    efManagerSetOffMakeEffect(&impact_pos, wp_attack_coll->shield_damage + damage);
}

// 0x800E31B4
void ftMainUpdateReflectorStatWeapon(WPStruct *wp, WPAttackColl *wp_attack_coll, FTStruct *fp, GObj *fighter_gobj)
{
    s32 damage = wpMainGetStaledDamage(wp);

    wpProcessUpdateHitInteractStats(wp, wp_attack_coll, fighter_gobj, nGMHitTypeReflect, 0);

    if (fp->special_coll->damage_resist < damage)
    {
        if (wp_attack_coll->can_rehit_fighter)
        {
            if (wp->hit_refresh_damage < damage)
            {
                wp->hit_refresh_damage = damage;
            }
        }
        else if (wp->hit_normal_damage < damage)
        {
            wp->hit_normal_damage = damage;
        }
        fp->reflect_damage = damage;

        fp->reflect_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(wp->weapon_gobj)->translate.vec.f.x) ? +1 : -1;
    }
    else
    {
        wp->reflect_gobj = fighter_gobj;

#if defined(REGION_US)
        /*
         * Oversight: attack_id is not set to that of the reflecting fighter.
         * This causes whatever move the original attacker used to summon the weapon
         * to be added to the reflecting fighter's staling queue if valid.
         * 
         * e.g. Fox reflects Mario's Fire Ball -> Fox's Blaster stales, even though he hasn't used it.
         */
        wp->reflect_stat_flags = fp->stat_flags;
        wp->reflect_stat_count = fp->stat_count;
#endif

        fp->reflect_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(wp->weapon_gobj)->translate.vec.f.x) ? +1 : -1;
    }
}

// 0x800E3308
void ftMainUpdateAbsorbStatWeapon(WPStruct *wp, WPAttackColl *wp_attack_coll, FTStruct *fp, GObj *fighter_gobj)
{
    s32 damage = wpMainGetStaledDamage(wp);

    wpProcessUpdateHitInteractStats(wp, wp_attack_coll, fighter_gobj, nGMHitTypeAbsorb, 0);

    wp->absorb_gobj = fighter_gobj;

    fp->absorb_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(wp->weapon_gobj)->translate.vec.f.x) ? +1 : -1;

    if (!(wp_attack_coll->can_not_heal))
    {
        fp->percent_damage -= (s32)(damage * 2.0F);

        if (fp->percent_damage < 0)
        {
            fp->percent_damage = 0;
        }
        gSCManagerBattleState->players[fp->player].stock_damage_all = fp->percent_damage;
    }
}

// 0x800E3418
void ftMainUpdateDamageStatWeapon(WPStruct *wp, WPAttackColl *wp_attack_coll, s32 wp_attack_id, FTStruct *fp, FTDamageColl *damage_coll, GObj *weapon_gobj, GObj *fighter_gobj)
{
    s32 temp_damage = wpMainGetStaledDamage(wp);
    s32 damage;

    wpProcessUpdateHitInteractStats(wp, wp_attack_coll, fighter_gobj, (wp_attack_coll->can_rehit_fighter) ? nGMHitTypeDamageRehit : nGMHitTypeDamage, 0);

    damage = ftParamGetCapturedDamage(fp, temp_damage);

    if (wp_attack_coll->can_rehit_fighter)
    {
        if (wp->hit_refresh_damage < damage)
        {
            wp->hit_refresh_damage = damage;
        }
    }
    else if (wp->hit_normal_damage < damage)
    {
        wp->hit_normal_damage = damage;
    }
    if
    (
        (fp->special_hitstatus == nGMHitStatusNormal)   &&
        (fp->star_hitstatus == nGMHitStatusNormal)      &&
        (fp->hitstatus == nGMHitStatusNormal)           &&
        (damage_coll->hitstatus == nGMHitStatusNormal)  &&
        (ftMainCheckGetUpdateDamage(fp, &damage) != FALSE)
    )
    {
        if (sFTMainHitLogID < ARRAY_COUNT(sFTMainHitLogs))
        {
            FTHitLog *hitlog = &sFTMainHitLogs[sFTMainHitLogID];

            hitlog->attacker_object_class = nFTHitLogObjectWeapon;
            hitlog->attack_coll = wp_attack_coll;
            hitlog->attack_id = wp_attack_id;
            hitlog->attacker_gobj = weapon_gobj;
            hitlog->damage_coll = damage_coll;
            hitlog->attacker_player = wp->player;
            hitlog->attacker_player_num = wp->player_num;

            sFTMainHitLogID++;
        }
        ftParamUpdatePlayerBattleStats(wp->player, fp->player, damage);
        ftParamUpdateStaleQueue(wp->player, fp->player, wp_attack_coll->motion_attack_id, wp_attack_coll->motion_count);
    }
    func_800269C0_275C0(wp_attack_coll->fgm_id);
}

// 0x800E35BC
void ftMainUpdateAttackStatItem(ITStruct *ip, ITAttackColl *it_attack_coll, s32 it_attack_id, FTStruct *fp, FTAttackColl *ft_attack_coll, GObj *item_gobj, GObj *fighter_gobj)
{
    s32 damage = itMainGetDamageOutput(ip);
    Vec3f impact_pos;

    gmCollisionGetItemAttackFighterAttackPosition(&impact_pos, it_attack_coll, it_attack_id, ft_attack_coll);

    if ((ft_attack_coll->damage - 10) < damage)
    {
        ftMainSetHitInteractStats(fp, ft_attack_coll->group_id, item_gobj, nGMHitTypeAttack, 0, 1);
        ftMainSetHitRebound(fighter_gobj, fp, ft_attack_coll, item_gobj);
        efManagerSetOffMakeEffect(&impact_pos, ft_attack_coll->damage);
    }
    if ((damage - 10) < ft_attack_coll->damage)
    {
        itProcessSetHitInteractStats(it_attack_coll, fighter_gobj, nGMHitTypeAttack, ft_attack_coll->group_id);

        if (ip->hit_attack_damage < damage)
        {
            ip->hit_attack_damage = damage;
        }
        efManagerSetOffMakeEffect(&impact_pos, damage);

        if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && ((damage - 10) >= 10) && (fp->player == gSCManagerSceneData.player))
        {
            gSC1PGameBonusGiantImpact = TRUE;
        }
    }
}

// 0x800E36F8
void ftMainUpdateShieldStatItem(ITStruct *ip, ITAttackColl *it_attack_coll, s32 attack_id, FTStruct *fp, GObj *item_gobj, GObj *fighter_gobj, f32 angle, Vec3f *vec)
{
    s32 damage = itMainGetDamageOutput(ip);
    Vec3f impact_pos;

    itProcessSetHitInteractStats(it_attack_coll, fighter_gobj, (it_attack_coll->can_rehit_shield) ? nGMHitTypeShieldRehit : nGMHitTypeShield, 0);

    if (ip->hit_shield_damage < damage)
    {
        ip->hit_shield_damage = damage;

        ip->shield_collide_angle = angle;

        ip->shield_collide_dir.x = 0.0F;
        ip->shield_collide_dir.y = 0.0F;
        ip->shield_collide_dir.z = (fp->lr == +1) ? -vec->x : vec->x;

        syVectorNorm3D(&ip->shield_collide_dir);
    }
    fp->shield_damage_total += damage + it_attack_coll->shield_damage;

    if (fp->shield_damage < damage)
    {
        fp->shield_damage = damage;

        fp->shield_lr = (ip->physics.vel_air.x < 0.0F) ? +1 : -1;

        fp->shield_player = ip->player;
    }
    gmCollisionGetItemAttackShieldPosition(&impact_pos, it_attack_coll, attack_id, fighter_gobj, fp->joints[nFTPartsJointYRotN]);
    efManagerSetOffMakeEffect(&impact_pos, it_attack_coll->shield_damage + damage);
}

// 0x800E3860
void ftMainUpdateReflectorStatItem(ITStruct *ip, ITAttackColl *it_attack_coll, FTStruct *fp, GObj *fighter_gobj)
{
    s32 damage = itMainGetDamageOutput(ip);

    itProcessSetHitInteractStats(it_attack_coll, fighter_gobj, nGMHitTypeReflect, 0);

    if (fp->special_coll->damage_resist < damage)
    {
        if (it_attack_coll->can_rehit_fighter)
        {
            if (ip->hit_refresh_damage < damage)
            {
                ip->hit_refresh_damage = damage;
            }
        }
        else if (ip->hit_normal_damage < damage)
        {
            ip->hit_normal_damage = damage;
        }
        fp->reflect_damage = damage;

        fp->reflect_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(ip->item_gobj)->translate.vec.f.x) ? +1 : -1;
    }
    else
    {
        ip->reflect_gobj = fighter_gobj;

#if defined(REGION_US)
        /*
         * Oversight: attack_id is not set to that of the reflecting fighter.
         * This causes whatever move the original attacker used the item with
         * to be added to the reflecting fighter's staling queue if valid.
         * 
         * e.g. Fox reflects an item thrown from LightThrowF4 -> Fox's own LightThrowF4 stales, even though he hasn't used it.
         */
        ip->reflect_stat_flags = fp->stat_flags;
        ip->reflect_stat_count = fp->stat_count;
#endif

        fp->reflect_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(ip->item_gobj)->translate.vec.f.x) ? +1 : -1;
    }
}

// 0x800E39B0
void ftMainUpdateDamageStatItem(ITStruct *ip, ITAttackColl *it_attack_coll, s32 attack_id, FTStruct *fp, FTDamageColl *damage_coll, GObj *item_gobj, GObj *fighter_gobj)
{
    s32 damage_temp = itMainGetDamageOutput(ip);
    s32 damage;
    s32 hit_lr;

    itProcessSetHitInteractStats(it_attack_coll, fighter_gobj, (it_attack_coll->can_rehit_fighter) ? nGMHitTypeDamageRehit : nGMHitTypeDamage, 0);

    if (ip->type == nITTypeTouch)
    {
        switch (ip->kind)
        {
        case nITKindStar:
            it_attack_coll->attack_state = nGMAttackStateOff;
            ip->hit_normal_damage = 1;

            ftParamSetStarHitStatusInvincible(fp, ITSTAR_INVINCIBLE_TIME);
            ftParamTryPlayItemMusic(nSYAudioBGMStar);
            func_800269C0_275C0(nSYAudioFGMStarGet);
#if defined (REGION_US)
            if ((gSCManagerBattleState->game_type == nSCBattleGameType1PGame) && (fp->player == gSCManagerSceneData.player) && (gSC1PGameBonusStarCount < U8_MAX))
            {
                gSC1PGameBonusStarCount++;
            }
#endif
            break;

        case nITKindGLucky:
            ftParamSetHealDamage(fp, it_attack_coll->damage);
            break;
        }
    }
    else
    {
        damage = ftParamGetCapturedDamage(fp, damage_temp);

        if (it_attack_coll->can_rehit_fighter)
        {
            if (ip->hit_refresh_damage < damage)
            {
                ip->hit_refresh_damage = damage;
            }
        }
        else if (ip->hit_normal_damage < damage)
        {
            ip->hit_normal_damage = damage;
        }
        if (ABSF(ip->physics.vel_air.x) < 5.0F)
        {
            ip->hit_lr = hit_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(item_gobj)->translate.vec.f.x) ? -1 : +1;
        }
        else
        {
            hit_lr = (ip->physics.vel_air.x < 0) ? -1 : +1;

            ip->hit_lr = hit_lr;
        }
        if 
        (
            (fp->special_hitstatus == nGMHitStatusNormal)   &&
            (fp->star_hitstatus == nGMHitStatusNormal)      &&
            (fp->hitstatus == nGMHitStatusNormal)           &&
            (damage_coll->hitstatus == nGMHitStatusNormal)  &&
            (ftMainCheckGetUpdateDamage(fp, &damage) != FALSE)
        )
        {
            if (sFTMainHitLogID < ARRAY_COUNT(sFTMainHitLogs))
            {
                FTHitLog *hitlog = &sFTMainHitLogs[sFTMainHitLogID];

                hitlog->attacker_object_class = nFTHitLogObjectItem;
                hitlog->attack_coll = it_attack_coll;
                hitlog->attack_id = attack_id;
                hitlog->attacker_gobj = item_gobj;
                hitlog->damage_coll = damage_coll;
                hitlog->attacker_player = ip->player;
                hitlog->attacker_player_num = ip->player_num;

                sFTMainHitLogID++;
            }
            ftParamUpdatePlayerBattleStats(ip->player, fp->player, damage);
            ftParamUpdateStaleQueue(ip->player, fp->player, it_attack_coll->motion_attack_id, it_attack_coll->motion_count);
        }
        func_800269C0_275C0(it_attack_coll->fgm_id);
    }
}

// 0x800E3CAC
void ftMainUpdateDamageStatGround(GObj *special_gobj, GObj *fighter_gobj, FTStruct *fp, GRAttackColl *gr_attack_coll, s32 kind)
{
    s32 damage = ftParamGetCapturedDamage(fp, gr_attack_coll->damage);
    sb32 is_take_damage = ftMainCheckGetUpdateDamage(fp, &damage);

    if ((is_take_damage != FALSE) && (sFTMainHitLogID < ARRAY_COUNT(sFTMainHitLogs)))
    {
        FTHitLog *hitlog = &sFTMainHitLogs[sFTMainHitLogID];

        hitlog->attacker_object_class = nFTHitLogObjectGround;
        hitlog->attack_coll = gr_attack_coll;
        hitlog->attacker_gobj = special_gobj;

        sFTMainHitLogID++;
    }
    switch (kind)
    {
    case nGMHitEnvironmentAcid:
        fp->acid_wait = 30;

        func_800269C0_275C0(nSYAudioFGMFloorDamageFire);
        break;

    case nGMHitEnvironmentPowerBlock:
        if (is_take_damage != FALSE)
        {
            ftParamUpdatePlayerBattleStats(itGetStruct(special_gobj)->damage_port, fp->player, damage);
        }
        break;

    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        fp->damagefloor_wait = 16;

        if (kind == 7)
        {
            func_800269C0_275C0(nSYAudioFGMShockML);
        }
        else func_800269C0_275C0(nSYAudioFGMFloorDamageFire);
        break;

    default:
        break;
    }
}

// 0x800E3DD0
void ftMainGetBumperDamageAngle(GObj *fighter_gobj, GObj *attacker_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->damage_angle == 362)
    {
        f32 dist_x = DObjGetStruct(fighter_gobj)->translate.vec.f.x - DObjGetStruct(attacker_gobj)->translate.vec.f.x;
        f32 dist_y;

        fp->damage_lr = (dist_x < 0) ? +1 : -1;

        if (dist_x < 0.0F)
        {
            dist_x = -dist_x;
        }
        dist_y = (DObjGetStruct(fighter_gobj)->translate.vec.f.y + fp->coll_data.map_coll.center) - DObjGetStruct(attacker_gobj)->translate.vec.f.y;

        fp->damage_angle = (dist_x == 0) ? 0 : F_CLC_RTOD32(syUtilsArcTan(dist_y / dist_x));
    }
}

// 0x800E3EBC
void ftMainProcessHitCollisionStatsMain(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *attacker_fp;
    WPStruct *wp;
    ITStruct *ip;
    FTAttributes *attr = this_fp->attr;
    FTHitLog *hitlog;
    s32 i, j;
    f32 knockback_temp;
    f32 knockback;
    FTAttackColl *ft_attack_coll;
    WPAttackColl *wp_attack_coll;
    ITAttackColl *it_attack_coll;
    GRAttackColl *gr_attack_coll;
    Vec3f pos;
    s32 damage;
    u8 gr_handicap;
    GObj *attacker_gobj;
    s32 damage_lr;

    knockback = -1.0F;

    for (i = 0; i < sFTMainHitLogID; i++)
    {
        hitlog = &sFTMainHitLogs[i];

        switch (hitlog->attacker_object_class)
        {
        case nFTHitLogObjectFighter:
            ft_attack_coll = hitlog->attack_coll;
            attacker_fp = ftGetStruct(hitlog->attacker_gobj);

            knockback_temp = ftParamGetCommonKnockback
            (
                this_fp->percent_damage, 
                this_fp->damage_queue, ft_attack_coll->damage, 
                ft_attack_coll->knockback_weight, 
                ft_attack_coll->knockback_scale, 
                ft_attack_coll->knockback_base, 
                attr->weight, 
                attacker_fp->handicap, 
                this_fp->handicap
            );

            gmCollisionGetFighterAttackDamagePosition(&pos, ft_attack_coll, hitlog->damage_coll);

            switch (ft_attack_coll->element)
            {
            case nGMHitElementFire:
                efManagerDamageFireMakeEffect(&pos, ft_attack_coll->damage);
                break;

            case nGMHitElementElectric:
                efManagerDamageElectricMakeEffect(&pos, ft_attack_coll->damage);
                break;

            case nGMHitElementCoin:
                efManagerDamageCoinMakeEffect(&pos);
                break;

            case nGMHitElementSlash:
                efManagerDamageSlashMakeEffect(&pos, ft_attack_coll->damage, gmCollisionGetDamageSlashRotation(attacker_fp, ft_attack_coll));
                break;

            default:
                if (knockback_temp < 180.0F)
                {
                    efManagerDamageNormalLightMakeEffect(&pos, hitlog->attacker_player, ft_attack_coll->damage, 0);
                }
                else efManagerDamageNormalHeavyMakeEffect(&pos, hitlog->attacker_player, ft_attack_coll->damage);

                if (ft_attack_coll->fgm_level > 0) // Changed this to > 0 for now, makes a bit more sense to me since it only does this on moves with hit SFX levels greater than weak (0)
                {
                    efManagerDamageSpawnOrbsRandgcMakeEffect(&pos);

                    switch (this_fp->attr->is_metallic)
                    {
                    case FALSE:
                        efManagerDamageSpawnSparksRandgcMakeEffect(&pos, this_fp->lr);
                        break;

                    case TRUE:
                        efManagerDamageSpawnMDustRandgcMakeEffect(&pos, this_fp->lr);
                        break;
                    }
                }
                break;
            }
            break;

        case nFTHitLogObjectWeapon:
            wp_attack_coll = hitlog->attack_coll;
            wp = wpGetStruct(hitlog->attacker_gobj);
            damage = wpMainGetStaledDamage(wp);

            knockback_temp = ftParamGetCommonKnockback(this_fp->percent_damage, this_fp->damage_queue, damage, wp_attack_coll->knockback_weight, wp_attack_coll->knockback_scale, wp_attack_coll->knockback_base, attr->weight, wp->handicap, this_fp->handicap);

            if (wp->is_hitlag_victim)
            {
                gmCollisionGetWeaponAttackFighterDamagePosition(&pos, wp_attack_coll, hitlog->attack_id, hitlog->damage_coll);

                switch (wp_attack_coll->element)
                {
                case nGMHitElementFire:
                    efManagerDamageFireMakeEffect(&pos, damage);
                    break;

                case nGMHitElementElectric:
                    efManagerDamageElectricMakeEffect(&pos, damage);
                    break;

                case nGMHitElementCoin:
                    efManagerDamageCoinMakeEffect(&pos);
                    break;

                default:
                    if (knockback_temp < 180.0F)
                    {
                        efManagerDamageNormalLightMakeEffect(&pos, hitlog->attacker_player, damage, NULL);
                    }
                    else efManagerDamageNormalHeavyMakeEffect(&pos, hitlog->attacker_player, damage);
                    break;
                }
            }
            break;

        case nFTHitLogObjectItem:
            it_attack_coll = hitlog->attack_coll;
            ip = itGetStruct(hitlog->attacker_gobj);

            damage = itMainGetDamageOutput(ip);

            knockback_temp = ftParamGetCommonKnockback(this_fp->percent_damage, this_fp->damage_queue, damage, it_attack_coll->knockback_weight, it_attack_coll->knockback_scale, it_attack_coll->knockback_base, attr->weight, ip->handicap, this_fp->handicap);

            if (ip->is_hitlag_victim)
            {
                gmCollisionGetItemAttackFighterDamagePosition(&pos, it_attack_coll, hitlog->attack_id, hitlog->damage_coll);

                switch (it_attack_coll->element)
                {
                case nGMHitElementFire:
                    efManagerDamageFireMakeEffect(&pos, damage);
                    break;

                case nGMHitElementElectric:
                    efManagerDamageElectricMakeEffect(&pos, damage);
                    break;

                case nGMHitElementCoin:
                    efManagerDamageCoinMakeEffect(&pos);
                    break;

                default:
                    if (knockback_temp < 180.0F)
                    {
                        efManagerDamageNormalLightMakeEffect(&pos, hitlog->attacker_player, damage, NULL);
                    }
                    else efManagerDamageNormalHeavyMakeEffect(&pos, hitlog->attacker_player, damage);
                    break;
                }
            }
            break;

        case nFTHitLogObjectGround:
            gr_attack_coll = hitlog->attack_coll;

            if (gr_attack_coll->kind == nGMHitEnvironmentPowerBlock) // POW Block?
            {
                gr_handicap = itGetStruct(hitlog->attacker_gobj)->damage_handicap;
            }
            else gr_handicap = 9;

            knockback_temp = ftParamGetCommonKnockback(this_fp->percent_damage, this_fp->damage_queue, gr_attack_coll->damage, gr_attack_coll->knockback_weight, gr_attack_coll->knockback_scale, gr_attack_coll->knockback_base, attr->weight, gr_handicap, this_fp->handicap);
            break;

        default:
            break;
        }
        if (knockback < knockback_temp)
        {
            knockback = knockback_temp;

            j = i;
        }
    }
    hitlog = &sFTMainHitLogs[j];
    attacker_gobj = hitlog->attacker_gobj;

    switch (hitlog->attacker_object_class)
    {
    case nFTHitLogObjectFighter:
        ft_attack_coll = hitlog->attack_coll;
        attacker_fp = ftGetStruct(attacker_gobj);
        this_fp->damage_angle = ft_attack_coll->angle;
        this_fp->damage_element = ft_attack_coll->element;

        this_fp->damage_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(attacker_gobj)->translate.vec.f.x) ? +1 : -1;

        this_fp->damage_player_num = hitlog->attacker_player_num;

        ftParamUpdate1PGameDamageStats(this_fp, hitlog->attacker_player, hitlog->attacker_object_class, attacker_fp->fkind, attacker_fp->stat_flags.halfword & ~0x400, attacker_fp->stat_count);

        this_fp->damage_joint_id = hitlog->damage_coll->joint_id;
        this_fp->damage_index = hitlog->damage_coll->placement;

        if (this_fp->damage_element == nGMHitElementElectric)
        {
            attacker_fp->hitlag_mul = 1.5F;
        }
        break;

    case nFTHitLogObjectWeapon:
        wp_attack_coll = hitlog->attack_coll;
        wp = wpGetStruct(attacker_gobj);
        this_fp->damage_angle = wp_attack_coll->angle;
        this_fp->damage_element = wp_attack_coll->element;

        if (ABSF(wp->physics.vel_air.x) < 5.0F)
        {
            this_fp->damage_lr = damage_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(attacker_gobj)->translate.vec.f.x) ? +1 : -1;
        }
        else
        {
            damage_lr = (wp->physics.vel_air.x < 0) ? +1 : -1;

            this_fp->damage_lr = damage_lr;
        }
        if (this_fp->player == hitlog->attacker_player)
        {
            this_fp->damage_player_num = 0;

            ftParamUpdate1PGameDamageStats(this_fp, GMCOMMON_PLAYERS_MAX, hitlog->attacker_object_class, wp->kind, 0, 0);
        }
        else
        {
            this_fp->damage_player_num = hitlog->attacker_player_num;

            ftParamUpdate1PGameDamageStats(this_fp, hitlog->attacker_player, hitlog->attacker_object_class, wp->kind, wp_attack_coll->stat_flags.halfword, wp_attack_coll->stat_count);
        }
        this_fp->damage_joint_id = hitlog->damage_coll->joint_id;
        this_fp->damage_index = hitlog->damage_coll->placement;

        ftMainGetBumperDamageAngle(fighter_gobj, attacker_gobj);
        break;

    case nFTHitLogObjectItem:
        it_attack_coll = hitlog->attack_coll;
        ip = itGetStruct(attacker_gobj);

        this_fp->damage_angle = it_attack_coll->angle;
        this_fp->damage_element = it_attack_coll->element;

        if (ABSF(ip->physics.vel_air.x) < 5.0F)
        {
            this_fp->damage_lr = damage_lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(attacker_gobj)->translate.vec.f.x) ? +1 : -1;
        }
        else
        {
            damage_lr = (ip->physics.vel_air.x < 0) ? +1 : -1;

            this_fp->damage_lr = damage_lr;
        }

        if (this_fp->player == hitlog->attacker_player)
        {
            this_fp->damage_player_num = 0;

            ftParamUpdate1PGameDamageStats(this_fp, GMCOMMON_PLAYERS_MAX, hitlog->attacker_object_class, ip->kind, 0, 0);
        }
        else
        {
            this_fp->damage_player_num = hitlog->attacker_player_num;
            ftParamUpdate1PGameDamageStats(this_fp, hitlog->attacker_player, hitlog->attacker_object_class, ip->kind, it_attack_coll->stat_flags.halfword, it_attack_coll->stat_count);
        }
        this_fp->damage_joint_id = hitlog->damage_coll->joint_id;
        this_fp->damage_index = hitlog->damage_coll->placement;

        ftMainGetBumperDamageAngle(fighter_gobj, attacker_gobj);
        break;

    case nFTHitLogObjectGround:
        gr_attack_coll = hitlog->attack_coll;

        this_fp->damage_angle = gr_attack_coll->angle;
        this_fp->damage_element = gr_attack_coll->element;

        this_fp->damage_lr = this_fp->lr;

        switch (gr_attack_coll->kind)
        {
        case nGMHitEnvironmentAcid:
            this_fp->damage_player_num = 0;

            if (this_fp->damage_player == -1)
            {
                this_fp->damage_player = GMCOMMON_PLAYERS_MAX;
            }
            ftParamUpdate1PGameDamageStats(this_fp, this_fp->damage_player, hitlog->attacker_object_class, gr_attack_coll->kind, 0, 0);
            break;

        case nGMHitEnvironmentPowerBlock:
            ip = itGetStruct(attacker_gobj);

            this_fp->damage_player_num = ip->damage_player_num;

            ftParamUpdate1PGameDamageStats(this_fp, ip->damage_port, hitlog->attacker_object_class, gr_attack_coll->kind, 0, 0);

            break;

        default:
            this_fp->damage_player_num = 0;

            ftParamUpdate1PGameDamageStats(this_fp, GMCOMMON_PLAYERS_MAX, hitlog->attacker_object_class, gr_attack_coll->kind, 0, 0);
            break;
        }
        this_fp->damage_joint_id = 0;
        this_fp->damage_index = 0;

        break;

    default:
        break;
    }
    this_fp->damage_knockback = knockback;

    if (this_fp->damage_element == nGMHitElementElectric)
    {
        this_fp->hitlag_mul = 1.5F;
    }
}

// 0x800E4870 - My brain hurts
void ftMainSearchHitFighter(GObj *this_gobj)
{
    GObj *other_gobj;
    FTStruct *this_fp;
    FTStruct *other_fp;
    s32 i, j, k, l, m, n;
    GMHitFlags those_flags;
    GMHitFlags these_flags;
    FTAttackColl *this_attack_coll;
    f32 angle;
    FTAttackColl *other_attack_coll;
    FTDamageColl *damage_coll;
    sb32 is_check_self;

    this_fp = ftGetStruct(this_gobj);
    other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    is_check_self = FALSE;

    while (other_gobj != NULL)
    {
        if (other_gobj == this_gobj)
        {
            is_check_self = TRUE;

            goto next_gobj;
        }
        else if (other_gobj == this_fp->capture_gobj) goto next_gobj;

        other_fp = ftGetStruct(other_gobj);

        if (((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE)) && (((other_fp->throw_gobj != NULL) ? other_fp->throw_team : other_fp->team) == this_fp->team))
        {
            goto next_gobj;
        }
        if (!(other_fp->is_catchstatus))
        {
            if ((other_fp->throw_gobj != NULL) && (this_gobj == other_fp->throw_gobj))
            {
                goto next_gobj;
            }
            else
            {
                k = 0;

                for (i = 0; i < ARRAY_COUNT(other_fp->attack_colls); i++)
                {
                    other_attack_coll = &other_fp->attack_colls[i];

                    if (other_attack_coll->attack_state != nGMAttackStateOff)
                    {
                        if ((this_fp->ga == nMPKineticsAir) && (other_attack_coll->is_hit_air) || (this_fp->ga == nMPKineticsGround) && (other_attack_coll->is_hit_ground))
                        {
                            these_flags.is_interact_hurt = these_flags.is_interact_shield = FALSE;

                            these_flags.group_id = 7;

                            for (m = 0; m < ARRAY_COUNT(other_attack_coll->attack_records); m++)
                            {
                                if (this_gobj == other_attack_coll->attack_records[m].victim_gobj)
                                {
                                    these_flags = other_attack_coll->attack_records[m].victim_flags;

                                    break;
                                }
                            }
                            if ((!(these_flags.is_interact_hurt)) && (!(these_flags.is_interact_shield)) && (these_flags.group_id == 7))
                            {
                                gFTMainIsDamageDetect[i] = TRUE;

                                k++;

                                continue;
                            }
                        }
                    }
                    gFTMainIsDamageDetect[i] = FALSE;
                }
                if (k == 0) goto next_gobj;

                else
                {
                    k = 0;

                    if ((is_check_self != FALSE) && (this_gobj != other_fp->capture_gobj) && (other_fp->ga == nMPKineticsGround) && (this_fp->ga == nMPKineticsGround) && !(this_fp->is_catchstatus))
                    {
                        if ((this_fp->throw_gobj == NULL) || (other_gobj != this_fp->throw_gobj) && (((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE)) || (((other_fp->throw_gobj != NULL) ? other_fp->throw_team : other_fp->team) != this_fp->throw_team)))
                        {
                            l = 0;

                            for (i = 0; i < ARRAY_COUNT(this_fp->attack_colls); i++)
                            {
                                this_attack_coll = &this_fp->attack_colls[i];

                                if (this_attack_coll->attack_state != nGMAttackStateOff)
                                {
                                    if ((other_fp->ga == nMPKineticsAir) && (this_attack_coll->is_hit_air) || (other_fp->ga == nMPKineticsGround) && (this_attack_coll->is_hit_ground))
                                    {
                                        those_flags.is_interact_hurt = those_flags.is_interact_shield = FALSE;

                                        those_flags.group_id = 7;

                                        for (n = 0; n < ARRAY_COUNT(this_attack_coll->attack_records); n++)
                                        {
                                            if (other_gobj == this_attack_coll->attack_records[n].victim_gobj)
                                            {
                                                those_flags = this_attack_coll->attack_records[n].victim_flags;

                                                break;
                                            }
                                        }
                                        if ((!(those_flags.is_interact_hurt)) && (!(those_flags.is_interact_shield)) && (those_flags.group_id == 7))
                                        {
                                            gFTMainIsAttackDetect[i] = TRUE;

                                            l++;

                                            continue;
                                        }
                                    }
                                }
                                gFTMainIsAttackDetect[i] = FALSE;
                            }
                            if (l != 0)
                            {
                                for (i = 0; i < ARRAY_COUNT(other_fp->attack_colls); i++)
                                {
                                    other_attack_coll = &other_fp->attack_colls[i];

                                    if (gFTMainIsDamageDetect[i] == FALSE) 
                                    {
                                        continue;
                                    }
                                    else for (j = 0; j < ARRAY_COUNT(this_fp->attack_colls); j++)
                                    {
                                        this_attack_coll = &this_fp->attack_colls[j];

                                        if (gFTMainIsAttackDetect[j] == FALSE) 
                                        {
                                            continue;
                                        }
                                        else if (gmCollisionCheckFighterAttacksCollide(other_attack_coll, this_attack_coll) != FALSE)
                                        {
                                            ftMainUpdateAttackStatFighter(other_fp, other_attack_coll, this_fp, this_attack_coll, other_gobj, this_gobj);

                                            if (gFTMainIsDamageDetect[i] == FALSE)
                                            {
                                                break;
                                            }
                                            else continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    for (i = 0; i < ARRAY_COUNT(other_fp->attack_colls); i++)
                    {
                        other_attack_coll = &other_fp->attack_colls[i];

                        if (gFTMainIsDamageDetect[i] == FALSE) 
                        {
                            continue;
                        }
                        gFTMainIsDamageDetect[i] = gmCollisionCheckFighterInFighterRange(other_attack_coll, this_gobj);

                        if (gFTMainIsDamageDetect[i] != FALSE) 
                        {
                            k++;
                        }
                    }
                    if (k != 0)
                    {
                        if (this_fp->is_shield)
                        {
                            for (i = 0; i < ARRAY_COUNT(other_fp->attack_colls); i++)
                            {
                                other_attack_coll = &other_fp->attack_colls[i];

                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else if (gmCollisionCheckFighterAttackShieldCollide(other_attack_coll, this_gobj, this_fp->joints[nFTPartsJointYRotN], &angle) != FALSE)
                                {
                                    ftMainUpdateShieldStatFighter(other_fp, other_attack_coll, this_fp, other_gobj, this_gobj);
                                }
                            }
                        }
                        if ((this_fp->special_hitstatus != nGMHitStatusIntangible) && (this_fp->star_hitstatus != nGMHitStatusIntangible) && (this_fp->hitstatus != nGMHitStatusIntangible))
                        {
                            for (i = 0; i < ARRAY_COUNT(other_fp->attack_colls); i++)
                            {
                                other_attack_coll = &other_fp->attack_colls[i];

                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else for (j = 0; j < ARRAY_COUNT(this_fp->damage_colls); j++)
                                {
                                    damage_coll = &this_fp->damage_colls[j];

                                    if (damage_coll->hitstatus == nGMHitStatusNone) 
                                    {
                                        break;
                                    }
                                    if (damage_coll->hitstatus != nGMHitStatusIntangible)
                                    {
                                        if (gmCollisionCheckFighterAttackDamageCollide(other_attack_coll, damage_coll) != FALSE)
                                        {
                                            ftMainUpdateDamageStatFighter(other_fp, other_attack_coll, this_fp, damage_coll, other_gobj, this_gobj);

                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    next_gobj:
        other_gobj = other_gobj->link_next;
    }
}

// 0x800E4ED4
void ftMainSearchHitWeapon(GObj *fighter_gobj)
{
    GObj *weapon_gobj;
    s32 i, j, k, l, m, n;
    GMHitFlags fighter_flags;
    GMHitFlags weapon_flags;
    FTStruct *fp;
    WPStruct *wp;
    WPAttackColl *wp_attack_coll;
    f32 angle;
    Vec3f vec;
    FTDamageColl *damage_coll;
    FTAttackColl *ft_attack_coll;
    s32 unused1;

    fp = ftGetStruct(fighter_gobj);
    weapon_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

    while (weapon_gobj != NULL)
    {
        wp = wpGetStruct(weapon_gobj);
        wp_attack_coll = &wp->attack_coll;

        if ((fighter_gobj != wp->owner_gobj) && ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (fp->team != wp->team)) && (wp_attack_coll->attack_state != nGMAttackStateOff))
        {
            if (wp_attack_coll->interact_mask & GMHITCOLLISION_FLAG_FIGHTER)
            {
                weapon_flags.is_interact_hurt = weapon_flags.is_interact_shield = weapon_flags.is_interact_reflect = weapon_flags.is_interact_absorb = FALSE;

                weapon_flags.group_id = 7;

                for (m = 0; m < ARRAY_COUNT(wp_attack_coll->attack_records); m++)
                {
                    if (fighter_gobj == wp_attack_coll->attack_records[m].victim_gobj)
                    {
                        weapon_flags = wp_attack_coll->attack_records[m].victim_flags;

                        break;
                    }
                }
                if (!(weapon_flags.is_interact_hurt) && !(weapon_flags.is_interact_shield) && !(weapon_flags.is_interact_reflect) && !(weapon_flags.is_interact_absorb) && (weapon_flags.group_id == 7))
                {
                    if ((wp_attack_coll->can_setoff) && !(fp->is_catchstatus) && ((fp->throw_gobj == NULL) || (fp->throw_gobj != wp->owner_gobj) && ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (fp->throw_team != wp->team))))
                    {

                        if (!(fp->is_reflect) || !(wp_attack_coll->can_reflect))
                        {
                            k = 0;

                            for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
                            {
                                ft_attack_coll = &fp->attack_colls[i];

                                if ((ft_attack_coll->attack_state != nGMAttackStateOff) && ((wp->ga == nMPKineticsAir) && (ft_attack_coll->is_hit_air) || (wp->ga == nMPKineticsGround) && (ft_attack_coll->is_hit_ground)))
                                {
                                    fighter_flags.group_id = 7;

                                    for (n = 0; n < ARRAY_COUNT(ft_attack_coll->attack_records); n++)
                                    {
                                        if (weapon_gobj == ft_attack_coll->attack_records[n].victim_gobj)
                                        {
                                            fighter_flags = ft_attack_coll->attack_records[n].victim_flags;

                                            break;
                                        }
                                    }
                                    if (fighter_flags.group_id == 7)
                                    {
                                        gFTMainIsAttackDetect[i] = TRUE;

                                        k++;

                                        continue;
                                    }
                                }
                                gFTMainIsAttackDetect[i] = FALSE;
                            }
                            if (k != 0)
                            {
                                for (i = 0; i < wp_attack_coll->attack_count; i++)
                                {
                                    for (j = 0; j < ARRAY_COUNT(fp->attack_colls); j++)
                                    {
                                        ft_attack_coll = &fp->attack_colls[j];

                                        if (gFTMainIsAttackDetect[j] == FALSE) 
                                        {
                                            continue;
                                        }
                                        else if (gmCollisionCheckWeaponAttackFighterAttackCollide(wp_attack_coll, i, ft_attack_coll) != FALSE)
                                        {
                                            ftMainUpdateAttackStatWeapon(wp, wp_attack_coll, i, fp, ft_attack_coll, weapon_gobj, fighter_gobj);

                                            if (wp->hit_attack_damage != 0) 
                                            {
                                                goto next_gobj;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                    }
                    for (i = 0, l = 0; i < wp_attack_coll->attack_count; i++)
                    {
                        gFTMainIsDamageDetect[i] = gmCollisionCheckWeaponInFighterRange(wp_attack_coll, i, fighter_gobj);

                        if (gFTMainIsDamageDetect[i] != FALSE)
                        {
                            l++;
                        }
                    }
                    if (l != 0)
                    {
                        if ((fp->is_reflect) && (wp_attack_coll->can_reflect))
                        {
                            for (i = 0; i < wp_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE) continue;

                                else if (gmCollisionCheckWeaponAttackSpecialCollide(wp_attack_coll, i, fp, fp->special_coll) != FALSE)
                                {
                                    ftMainUpdateReflectorStatWeapon(wp, wp_attack_coll, fp, fighter_gobj);

                                    goto next_gobj;
                                }
                            }
                        }
                        if ((fp->is_absorb) && (wp_attack_coll->can_absorb))
                        {
                            for (i = 0; i < wp_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else if (gmCollisionCheckWeaponAttackSpecialCollide(wp_attack_coll, i, fp, fp->special_coll) != FALSE)
                                {
                                    ftMainUpdateAbsorbStatWeapon(wp, wp_attack_coll, fp, fighter_gobj);

                                    goto next_gobj;
                                }
                            }
                        }
                        if ((fp->is_shield) && (wp_attack_coll->can_shield))
                        {
                            for (i = 0; i < wp_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else if (gmCollisionCheckWeaponAttackShieldCollide(wp_attack_coll, i, fighter_gobj, fp->joints[nFTPartsJointYRotN], &angle, &vec) != FALSE)
                                {
                                    ftMainUpdateShieldStatWeapon(wp, wp_attack_coll, i, fp, weapon_gobj, fighter_gobj, angle, &vec);

                                    goto next_gobj;
                                }
                            }
                        }
                        if ((fp->special_hitstatus != nGMHitStatusIntangible) && (fp->star_hitstatus != nGMHitStatusIntangible) && (fp->hitstatus != nGMHitStatusIntangible))
                        {
                            for (i = 0; i < wp_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else for (j = 0; j < ARRAY_COUNT(fp->damage_colls); j++)
                                {
                                    damage_coll = &fp->damage_colls[j];

                                    if (damage_coll->hitstatus == nGMHitStatusNone) break;

                                    if (damage_coll->hitstatus != nGMHitStatusIntangible)
                                    {
                                        if (gmCollisionCheckWeaponAttackFighterDamageCollide(wp_attack_coll, i, damage_coll) != FALSE)
                                        {
                                            ftMainUpdateDamageStatWeapon(wp, wp_attack_coll, i, fp, damage_coll, weapon_gobj, fighter_gobj);

                                            goto next_gobj;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    next_gobj:
        weapon_gobj = weapon_gobj->link_next;
    }
}

// 0x800E55DC
void ftMainSearchHitItem(GObj *fighter_gobj)
{
    GObj *item_gobj;
    s32 i, j, k, l, m, n;
    GMHitFlags fighter_flags;
    GMHitFlags item_flags;
    FTStruct *fp;
    ITStruct *ip;
    ITAttackColl *it_attack_coll;
    f32 angle;
    Vec3f vec;
    FTDamageColl *damage_coll;
    FTAttackColl *ft_attack_coll;
    s32 unused1;

    fp = ftGetStruct(fighter_gobj);
    item_gobj = gGCCommonLinks[nGCCommonLinkIDItem];

    while (item_gobj != NULL)
    {
        ip = itGetStruct(item_gobj);
        it_attack_coll = &ip->attack_coll;

        if ((fighter_gobj != ip->owner_gobj) && ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (fp->team != ip->team)) && (it_attack_coll->attack_state != nGMAttackStateOff))
        {
            if (it_attack_coll->interact_mask & GMHITCOLLISION_FLAG_FIGHTER)
            {
                item_flags.is_interact_hurt = item_flags.is_interact_shield = item_flags.is_interact_reflect = FALSE;

                item_flags.group_id = 7;

                for (m = 0; m < ARRAY_COUNT(it_attack_coll->attack_records); m++)
                {
                    if (fighter_gobj == it_attack_coll->attack_records[m].victim_gobj)
                    {
                        item_flags = it_attack_coll->attack_records[m].victim_flags;

                        break;
                    }
                }
                if (!(item_flags.is_interact_hurt) && !(item_flags.is_interact_shield) && !(item_flags.is_interact_reflect) && (item_flags.group_id == 7))
                {
                    if ((it_attack_coll->can_setoff) && !(fp->is_catchstatus) && ((fp->throw_gobj == NULL) || (fp->throw_gobj != ip->owner_gobj) && ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (fp->throw_team != ip->team))))
                    {
                        if (!(fp->is_reflect) || !(it_attack_coll->can_reflect))
                        {
                            k = 0;

                            for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
                            {
                                ft_attack_coll = &fp->attack_colls[i];

                                if ((ft_attack_coll->attack_state != nGMAttackStateOff) && ((ip->ga == nMPKineticsAir) && (ft_attack_coll->is_hit_air) || (ip->ga == nMPKineticsGround) && (ft_attack_coll->is_hit_ground)))
                                {
                                    fighter_flags.group_id = 7;

                                    for (n = 0; n < ARRAY_COUNT(ft_attack_coll->attack_records); n++)
                                    {
                                        if (item_gobj == ft_attack_coll->attack_records[n].victim_gobj)
                                        {
                                            fighter_flags = ft_attack_coll->attack_records[n].victim_flags;

                                            break;
                                        }
                                    }
                                    if (fighter_flags.group_id == 7)
                                    {
                                        gFTMainIsAttackDetect[i] = TRUE;

                                        k++;

                                        continue;
                                    }
                                }
                                gFTMainIsAttackDetect[i] = FALSE;
                            }
                            if (k != 0)
                            {
                                for (i = 0; i < it_attack_coll->attack_count; i++)
                                {
                                    for (j = 0; j < ARRAY_COUNT(fp->attack_colls); j++)
                                    {
                                        ft_attack_coll = &fp->attack_colls[j];

                                        if (gFTMainIsAttackDetect[j] == FALSE) 
                                        {
                                            continue;
                                        }
                                        else if (gmCollisionCheckItemAttackFighterAttackCollide(it_attack_coll, i, ft_attack_coll) != FALSE)
                                        {
                                            ftMainUpdateAttackStatItem(ip, it_attack_coll, i, fp, ft_attack_coll, item_gobj, fighter_gobj);

                                            if (ip->hit_attack_damage != 0) goto next_gobj;
                                        }
                                    }
                                }
                            }
                        }

                    }
                    for (i = 0, l = 0; i < it_attack_coll->attack_count; i++)
                    {
                        gFTMainIsDamageDetect[i] = gmCollisionCheckItemInFighterRange(it_attack_coll, i, fighter_gobj);

                        if (gFTMainIsDamageDetect[i] != FALSE)
                        {
                            l++;
                        }
                    }
                    if (l != 0)
                    {
                        if ((fp->is_reflect) && (it_attack_coll->can_reflect))
                        {
                            for (i = 0; i < it_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else if (gmCollisionCheckItemAttackSpecialCollide(it_attack_coll, i, fp, fp->special_coll) != FALSE)
                                {
                                    ftMainUpdateReflectorStatItem(ip, it_attack_coll, fp, fighter_gobj);

                                    goto next_gobj;
                                }
                            }
                        }
                        if ((fp->is_shield) && (it_attack_coll->can_shield))
                        {
                            for (i = 0; i < it_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE)
                                {
                                    continue;
                                }
                                else if (gmCollisionCheckItemAttackShieldCollide(it_attack_coll, i, fighter_gobj, fp->joints[nFTPartsJointYRotN], &angle, &vec) != FALSE)
                                {
                                    ftMainUpdateShieldStatItem(ip, it_attack_coll, i, fp, item_gobj, fighter_gobj, angle, &vec);

                                    goto next_gobj;
                                }
                            }
                        }
                        if ((fp->special_hitstatus != nGMHitStatusIntangible) && (fp->star_hitstatus != nGMHitStatusIntangible) && (fp->hitstatus != nGMHitStatusIntangible))
                        {
                            for (i = 0; i < it_attack_coll->attack_count; i++)
                            {
                                if (gFTMainIsDamageDetect[i] == FALSE) 
                                {
                                    continue;
                                }
                                else for (j = 0; j < ARRAY_COUNT(fp->damage_colls); j++)
                                {
                                    damage_coll = &fp->damage_colls[j];

                                    if (damage_coll->hitstatus == nGMHitStatusNone) 
                                    {
                                        break;
                                    }
                                    if (damage_coll->hitstatus != nGMHitStatusIntangible)
                                    {
                                        if (gmCollisionCheckItemAttackFighterDamageCollide(it_attack_coll, i, damage_coll) != FALSE)
                                        {
                                            ftMainUpdateDamageStatItem(ip, it_attack_coll, i, fp, damage_coll, item_gobj, fighter_gobj);

                                            goto next_gobj;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    next_gobj:
        item_gobj = item_gobj->link_next;
    }
}

// 0x800E5C30
sb32 ftMainGetGroundHitObstacle(FTStruct *fp, GRAttackColl **p_gr_attack_coll)
{
    if ((fp->damagefloor_wait == 0) && (fp->ga == nMPKineticsGround) && (fp->coll_data.floor_line_id != -1) && (fp->coll_data.floor_line_id != -2))
    {
        switch (fp->coll_data.floor_flags & MAP_VERTEX_MAT_MASK)
        {
        case nMPMaterialFireWeakS1:
            *p_gr_attack_coll = &dFTMainGroundHitCollisionAttributes[0];
            return TRUE;

        case nMPMaterialFireStrongS1:
            *p_gr_attack_coll = &dFTMainGroundHitCollisionAttributes[1];
            return TRUE;

        case nMPMaterialFireWeakHi1:
            *p_gr_attack_coll = &dFTMainGroundHitCollisionAttributes[2];
            return TRUE;

        case nMPMaterialSpikes:
            *p_gr_attack_coll = &dFTMainGroundHitCollisionAttributes[3];
            return TRUE;

        case nMPMaterialFireWeakHi2:
            *p_gr_attack_coll = &dFTMainGroundHitCollisionAttributes[4];
            return TRUE;

        case nMPMaterialFireWeakHi3:
            *p_gr_attack_coll = &dFTMainGroundHitCollisionAttributes[5];
            return TRUE;

        default:
            return FALSE;
        }
    }
    else return FALSE;
}

// 0x800E5D20
void ftMainSearchGroundHit(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GRHazard *ge = &sFTMainGroundHazards[0];
    s32 i;
    GRAttackColl *gr_attack_coll;
    s32 kind;

    if (fp->hitlag_tics == 0)
    {
        if (fp->acid_wait != 0)
        {
            fp->acid_wait--;
        }
        if (fp->damagefloor_wait != 0)
        {
            fp->damagefloor_wait--;
        }
    }
    if (ftParamGetBestHitStatusAll(fighter_gobj) == nGMHitStatusNormal)
    {
        for (i = 0; i < sFTMainGroundHazardsNum; i++, ge++)
        {
            if ((ge->gobj != NULL) && (ge->proc_update(ge->gobj, fighter_gobj, &gr_attack_coll, &kind) != FALSE))
            {
                ftMainUpdateDamageStatGround(ge->gobj, fighter_gobj, fp, gr_attack_coll, kind);
            }
        }
        if (ftMainGetGroundHitObstacle(fp, &gr_attack_coll) != FALSE)
        {
            ftMainUpdateDamageStatGround(NULL, fighter_gobj, fp, gr_attack_coll, gr_attack_coll->kind);
        }
    }
}

// 0x800E5E58 - Meth
void ftMainSearchFighterCatch(GObj *this_gobj)
{
    GObj *other_gobj;
    FTStruct *this_fp;
    FTStruct *other_fp;
    s32 i, j, m;
    FTDamageColl *damage_coll;
    GMHitFlags catch_mask;
    FTAttackColl *attack_coll;

    this_fp = ftGetStruct(this_gobj);

    this_fp->search_gobj = NULL;
    this_fp->search_gobj_dist = F32_MAX;

    other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (other_gobj != NULL)
    {
        if (other_gobj == this_gobj) 
        {
            goto next_gobj;
        }
        other_fp = ftGetStruct(other_gobj);

        if (other_fp->is_ghost) 
        {
            goto next_gobj;
        }
        if (other_fp->fkind == nFTKindBoss)
        {
            goto next_gobj;
        }
        if ((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE) && (this_fp->team == other_fp->team)) 
        {
            goto next_gobj;
        }
        if (other_fp->capture_immune_mask & this_fp->catch_mask)
        {
            goto next_gobj;
        }
        if ((other_fp->special_hitstatus != nGMHitStatusNormal) || (other_fp->star_hitstatus != nGMHitStatusNormal) || (other_fp->hitstatus != nGMHitStatusNormal)) 
        {
            goto next_gobj;
        }
        for (i = 0; i < ARRAY_COUNT(this_fp->attack_colls) ^ 0; i++) // XOR 0 ???
        {
            attack_coll = &this_fp->attack_colls[i];

            if (attack_coll->attack_state == nGMAttackStateOff) 
            {
                continue;
            }
            if ((other_fp->ga == nMPKineticsAir) && !(attack_coll->is_hit_air) || (other_fp->ga == nMPKineticsGround) && !(attack_coll->is_hit_ground)) 
            {
                continue;
            }
            catch_mask.is_interact_hurt = catch_mask.is_interact_shield = FALSE;

            catch_mask.group_id = 7;

            for (m = 0; m < ARRAY_COUNT(attack_coll->attack_records); m++)
            {
                if (other_gobj == attack_coll->attack_records[m].victim_gobj)
                {
                    catch_mask = attack_coll->attack_records[m].victim_flags;

                    break;
                }
            }
            if ((catch_mask.is_interact_hurt) || (catch_mask.is_interact_shield) || (catch_mask.group_id != 7)) continue;

            for (j = 0; j < ARRAY_COUNT(other_fp->damage_colls); j++)
            {
                damage_coll = &other_fp->damage_colls[j];

                if (damage_coll->hitstatus == nGMHitStatusNone) 
                {
                    break;
                }
                if ((damage_coll->hitstatus != nGMHitStatusIntangible) && (damage_coll->hitstatus != nGMHitStatusInvincible))
                {
                    if ((damage_coll->is_grabbable != FALSE) && (gmCollisionCheckFighterAttackDamageCollide(attack_coll, damage_coll) != FALSE))
                    {
                        ftMainUpdateCatchStatFighter(this_fp, attack_coll, other_fp, this_gobj, other_gobj);

                        goto next_gobj;
                    }
                }
            }        
        }
    next_gobj:
        other_gobj = other_gobj->link_next;
    }
}

// 0x800E6100
void ftMainProcSearchCatch(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSearchHitHazard(fighter_gobj);

    if (fp->is_catchstatus)
    {
        ftMainSearchFighterCatch(fighter_gobj);

        if (fp->search_gobj != NULL)
        {
            fp->proc_catch(fighter_gobj);
            fp->proc_capture(fp->search_gobj, fighter_gobj);
        }
    }
}

// 0x800E6178
void ftMainProcSearchHitAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->is_ghost))
    {
        sFTMainHitLogID = 0;

        ftMainSearchHitFighter(fighter_gobj);
        ftMainSearchHitItem(fighter_gobj);
        ftMainSearchHitWeapon(fighter_gobj);
        ftMainSearchGroundHit(fighter_gobj);

        if (sFTMainHitLogID != 0)
        {
            ftMainProcessHitCollisionStatsMain(fighter_gobj);
        }
    }
}

// 0x800E61EC
void ftMainProcParams(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 damage;
    s32 status_id;
    f32 knockback_resist;
    sb32 is_shieldbreak;
    u32 hitlag_tics;
    sb32 is_knockback_paused;
    s32 element;

    damage = 0;
    is_shieldbreak = FALSE;
    status_id = fp->status_id;
    hitlag_tics = fp->hitlag_tics;
    is_knockback_paused = FALSE;

    if (fp->unk_ft_0x7AC != 0)
    {
        fp->unk_ft_0x3C += fp->unk_ft_0x7AC;
    }
    if (!(fp->is_shield) && (fp->shield_health < 55))
    {
        fp->shield_heal_wait--;

        if (fp->shield_heal_wait == 0.0F)
        {
            fp->shield_health++;

            fp->shield_heal_wait = 10.0F;
        }
    }
    fp->shield_health -= fp->shield_damage_total;

    if (fp->shield_health <= 0)
    {
        fp->shield_health = (fp->fkind == nFTKindYoshi) ? 30 : 30;

        is_shieldbreak = TRUE;
    }
    if (fp->damage_knockback != 0.0F)
    {
        if ((fp->status_id == nFTCommonStatusSquat) || (fp->status_id == nFTCommonStatusSquatWait))
        {
            fp->damage_knockback *= (2.0F / 3.0F);
        }
        if (fp->status_id == nFTCommonStatusTwister)
        {
            fp->damage_kind = nFTDamageKindColAnim;
        }
        if (fp->knockback_resist_status < fp->knockback_resist_passive)
        {
            knockback_resist = fp->knockback_resist_passive;
        }
        else knockback_resist = fp->knockback_resist_status;

        fp->damage_knockback -= knockback_resist;

        if (fp->damage_knockback <= 0)
        {
            fp->damage_knockback = 0;
        }
        ftParamUpdateDamage(fp, fp->damage_queue);

        if (fp->proc_trap != NULL)
        {
            fp->proc_trap(fighter_gobj);
        }
        if (fp->fkind != nFTKindBoss)
        {
            switch (fp->damage_kind)
            {
            case nFTDamageKindNone:
                break;

            case nFTDamageKindStatus:
                ftParamStopVoiceRunProcDamage(fighter_gobj);
                ftCommonDamageGotoDamageStatus(fighter_gobj);
                break;

            case nFTDamageKindColAnim:
                ftCommonDamageSetDamageColAnim(fighter_gobj);
                break;

            case nFTDamageKindCatch:
                ftParamStopVoiceRunProcDamage(fighter_gobj);
                ftCommonDamageUpdateCatchResist(fighter_gobj);
                break;

            default:
                ftCommonDamageUpdateMain(fighter_gobj);
                break;
            }
        }
        else
        {
            ftCommonDamageSetDamageColAnim(fighter_gobj);
            ftBossCommonUpdateDamageStats(fighter_gobj);
        }
        damage = fp->damage_lag;
        is_knockback_paused = TRUE;

        ftParamSetDamageShuffle(fp, (fp->damage_element == nGMHitElementElectric) ? TRUE : FALSE, damage, status_id, fp->hitlag_mul);

        if ((s32)((fp->damage_queue * 0.75F) + 4.0F) > 0)
        {
            ftParamMakeRumble(fp, 0, (s32)((fp->damage_queue * 0.75F) + 4.0F));
        }
    }
    else if (fp->shield_damage != 0)
    {
        if (is_shieldbreak != FALSE)
        {
            ftCommonShieldBreakFlyCommonSetStatus(fighter_gobj);
        }
        else ftCommonGuardSetOffSetStatus(fighter_gobj);

        damage = fp->shield_damage;
    }
    else if (fp->attack_shield_push != 0)
    {
        if (fp->proc_shield != NULL)
        {
            fp->proc_shield(fighter_gobj);
        }
        if ((fp->attack_rebound != 0) && (fp->catch_gobj == NULL) && (fp->capture_gobj == NULL))
        {
            ftParamStopVoiceRunProcDamage(fighter_gobj);
            ftCommonReboundWaitSetStatus(fighter_gobj);
        }
        damage = fp->attack_shield_push;
    }
    else if (fp->attack_damage != 0)
    {
        if (fp->proc_hit != NULL)
        {
            fp->proc_hit(fighter_gobj);
        }
        damage = fp->attack_damage;

        if (fp->stat_flags.attack_id == nFTStatusAttackIDBatSwing4)
        {
            ftParamMakeRumble(fp, 10, 0);
        }
        else if ((s32) ((fp->attack_damage * 0.5F) + 2.0F) > 0)
        {
            ftParamMakeRumble(fp, 5, (s32)((fp->attack_damage * 0.5F) + 2.0F));
        }
    }
    else if (fp->reflect_damage != 0)
    {
        ftCommonShieldBreakFlyReflectorSetStatus(fighter_gobj);
    }
    else if (fp->reflect_lr != 0)
    {
        switch (fp->special_coll->kind)
        {
        case nFTSpecialCollKindFoxReflector:
            ftFoxSpecialLwHitSetStatus(fighter_gobj);
            break;

        case nFTSpecialCollKindNessReflector:
            func_800269C0_275C0(nSYAudioFGMBatHit);
            break;
        }
    }
    else if (fp->absorb_lr != 0)
    {
        ftNessSpecialLwProcAbsorb(fighter_gobj);
    }
    if (damage != 0)
    {
        fp->hitlag_tics = ftParamGetHitLag(damage, status_id, fp->hitlag_mul);

        if ((fp->hitlag_tics != 0) && (is_knockback_paused != FALSE))
        {
            fp->is_knockback_paused = TRUE;
        }
        fp->input.pl.button_tap = fp->input.pl.button_release = 0;

        if (fp->proc_lagstart != NULL)
        {
            fp->proc_lagstart(fighter_gobj);
        }
    }
    fp->unk_ft_0x7AC = 0;
    fp->attack_damage = 0;
    fp->attack_shield_push = 0;
    fp->shield_damage = 0;
    fp->shield_damage_total = 0;
    fp->damage_lag = 0;
    fp->damage_queue = 0;
    fp->damage_kind = nFTDamageKindDefault;

    fp->reflect_lr = 0;
    fp->reflect_damage = 0;

    fp->absorb_lr = 0;

    fp->unk_ft_0x7A0 = 0;
    fp->attack_rebound = 0;
    fp->damage_knockback = 0;
    fp->hitlag_mul = 1.0F;

    if ((hitlag_tics == 0) && (fp->afterimage.drawstatus != -1))
    {
        switch (fp->afterimage.is_itemswing)
        {
        case FALSE:
            if ((fp->fkind == nFTKindLink) && (fp->modelpart_status[11 - nFTPartsJointCommonStart].modelpart_id_curr == 0))
            {
                FTParts *parts = fp->joints[11]->user_data.p;

                func_ovl2_800EDBA4(fp->joints[11]);

                fp->afterimage.desc[fp->afterimage.desc_id].translate_x = parts->mtx_translate[3][0];
                fp->afterimage.desc[fp->afterimage.desc_id].translate_y = parts->mtx_translate[3][1];
                fp->afterimage.desc[fp->afterimage.desc_id].translate_z = parts->mtx_translate[3][2];
                fp->afterimage.desc[fp->afterimage.desc_id].vec.x = parts->mtx_translate[2][0];
                fp->afterimage.desc[fp->afterimage.desc_id].vec.y = parts->mtx_translate[2][1];
                fp->afterimage.desc[fp->afterimage.desc_id].vec.z = parts->mtx_translate[2][2];

                if (fp->afterimage.desc_id == 2)
                {
                    fp->afterimage.desc_id = 0;
                }
                else fp->afterimage.desc_id++;

                if (fp->afterimage.drawstatus <= 2)
                {
                    fp->afterimage.drawstatus++;
                }
            }
            break;

        case TRUE:
            if ((fp->item_gobj != NULL) && (fp->is_item_show) && (itGetStruct(fp->item_gobj)->kind == nITKindSword))
            {
                s32 unused;
                Mtx44f mtx;

                func_ovl0_800C9A38(mtx, fp->joints[fp->attr->joint_itemlight_id]);

                fp->afterimage.desc[fp->afterimage.desc_id].translate_x = mtx[3][0];
                fp->afterimage.desc[fp->afterimage.desc_id].translate_y = mtx[3][1];
                fp->afterimage.desc[fp->afterimage.desc_id].translate_z = mtx[3][2];
                fp->afterimage.desc[fp->afterimage.desc_id].vec.x = mtx[1][0];
                fp->afterimage.desc[fp->afterimage.desc_id].vec.y = mtx[1][1];
                fp->afterimage.desc[fp->afterimage.desc_id].vec.z = mtx[1][2];

                syVectorNorm3D(&fp->afterimage.desc[fp->afterimage.desc_id].vec);

                if (fp->afterimage.desc_id == 2)
                {
                    fp->afterimage.desc_id = 0;
                }
                else fp->afterimage.desc_id++;

                if (fp->afterimage.drawstatus <= 2)
                {
                    fp->afterimage.drawstatus++;
                }
            }
            break;
        }
    }
}

// 0x800E69C4
void ftMainUpdateHiddenPartID(FTStruct *fp, s32 hiddenpart_id)
{
    FTHiddenPart *hiddenpart;
    void *dl;
    DObj *root_joint;
    DObj *child_joint;
    DObj *sibling_joint;
    FTAttributes *attr;
    FTCommonPart *commonpart;
    DObj *parent_joint;
    FTParts *parts;

    attr = fp->attr;
    hiddenpart = &attr->hiddenparts[hiddenpart_id];

    if (hiddenpart->root_joint_id >= nFTPartsJointCommonStart)
    {
        if (fp->detail_curr == nFTPartsDetailHigh)
        {
            commonpart = &fp->attr->commonparts_container->commonparts[0];
        }
        else if (attr->commonparts_container->commonparts[1].dobjdesc[hiddenpart->root_joint_id - nFTPartsJointCommonStart].dl != NULL)
        {
            commonpart = &attr->commonparts_container->commonparts[1];
        }
        else commonpart = &attr->commonparts_container->commonparts[0];
    }
    else commonpart = NULL;

    dl = (commonpart != NULL) ? commonpart->dobjdesc[hiddenpart->root_joint_id - nFTPartsJointCommonStart].dl : NULL;

    root_joint = gcAddDObjForGObj(fp->fighter_gobj, dl);
    root_joint->sib_prev->sib_next = NULL;
    root_joint->sib_prev = NULL;

    if (dl != NULL)
    {
        lbCommonAddMObjForFighterPartsDObj(root_joint, commonpart->p_mobjsubs[hiddenpart->root_joint_id - nFTPartsJointCommonStart], commonpart->p_costume_matanim_joints[hiddenpart->root_joint_id - nFTPartsJointCommonStart], NULL, fp->costume);
    }
    if (commonpart != NULL)
    {
        fp->modelpart_status[hiddenpart->root_joint_id - nFTPartsJointCommonStart].modelpart_id_base = fp->modelpart_status[hiddenpart->root_joint_id - nFTPartsJointCommonStart].modelpart_id_curr = (dl != NULL) ? 0 : -1;
    }
    parent_joint = fp->joints[hiddenpart->parent_joint_id];

    switch (hiddenpart->joint_kind)
    {
    case 0:
        if (parent_joint->child != NULL)
        {
            sibling_joint = parent_joint->child->sib_next;
            child_joint = parent_joint->child;

            while (sibling_joint != NULL)
            {
                child_joint = sibling_joint;
                sibling_joint = sibling_joint->sib_next;
            }
            child_joint->sib_next = root_joint;
            root_joint->sib_prev = child_joint;
        }
        else parent_joint->child = root_joint;
        root_joint->parent = parent_joint;
        break;

    case 1:
        if (parent_joint->child != NULL)
        {
            child_joint = parent_joint->child;
            child_joint->sib_prev = root_joint;
            root_joint->sib_next = child_joint;
        }
        parent_joint->child = root_joint;
        root_joint->parent = parent_joint;
        break;

    case 2:
        child_joint = parent_joint->child->sib_next;
        child_joint->sib_prev = root_joint;
        root_joint->sib_next = child_joint;
        child_joint = parent_joint->child;
        child_joint->sib_next = root_joint;
        root_joint->parent = parent_joint;
        root_joint->sib_prev = child_joint;
        break;

    case 3:
        if (parent_joint->child != NULL)
        {
            child_joint = parent_joint->child;
            root_joint->child = child_joint;

            do
            {
                child_joint->parent = root_joint;
                child_joint = child_joint->sib_next;
            }
            while (child_joint != NULL);
        }
        parent_joint->child = root_joint;

        root_joint->parent = parent_joint;
        break;
    }
    fp->joints[hiddenpart->root_joint_id] = root_joint;

    root_joint->user_data.p = parts = ftManagerGetNextPartsAlloc();

    parts->flags = attr->commonparts_container->commonparts[fp->detail_curr - nFTPartsDetailStart].flags;
    parts->joint_id = hiddenpart->root_joint_id;

    if (hiddenpart->partindex_0x8 != 0)
    {
        lbCommonInitDObj(root_joint, 0x4B, 0, 0, fp->unk_ft_0x149);
    }
}

// 0x800E6CE0
void ftMainAddHiddenPartID(FTStruct *fp, s32 hiddenpart_id)
{
    DObj *new_parent_joint;
    DObj *sibling_joint;
    FTHiddenPart *hiddenpart;
    DObj *root_joint;
    DObj *child_joint;
    DObj *new_child_joint;
    DObj *parent_joint;

    hiddenpart = &fp->attr->hiddenparts[hiddenpart_id];
    root_joint = fp->joints[hiddenpart->root_joint_id];

    if (hiddenpart->root_joint_id == nFTPartsJointTransN)
    {
        parent_joint = root_joint->parent;
        child_joint = root_joint->child;
        new_parent_joint = parent_joint;
        new_child_joint = child_joint;

        if (new_child_joint != NULL)
        {
            sibling_joint = root_joint->sib_prev;

            new_child_joint = child_joint;
            child_joint = sibling_joint;

            if (sibling_joint == NULL)
            {
                new_parent_joint->child = new_child_joint;
            }
            else
            {
                child_joint->sib_next = new_child_joint;
                new_child_joint->sib_prev = root_joint->sib_prev;
            }
            child_joint = new_child_joint->sib_next;
            new_child_joint->parent = new_parent_joint;

            while (child_joint != NULL)
            {
                new_child_joint = child_joint;
                child_joint->parent = new_parent_joint;
                child_joint = child_joint->sib_next;
            }

            new_child_joint->sib_next = root_joint->sib_next;
            new_parent_joint = root_joint->sib_next;

            if (new_parent_joint != NULL)
            {
                new_parent_joint->sib_prev = new_child_joint;
            }
        }
        else
        {
            sibling_joint = root_joint->sib_prev;

            if (sibling_joint == NULL)
            {
                new_parent_joint->child = root_joint->sib_next;
            }
            else sibling_joint->sib_next = root_joint->sib_next;

            new_parent_joint = root_joint->sib_next;

            if (new_parent_joint != NULL)
            {
                new_parent_joint->sib_prev = root_joint->sib_prev;
            }
        }
        root_joint->child = NULL;
        root_joint->sib_prev = NULL;
        root_joint->sib_next = NULL;
        root_joint->parent = NULL;

        new_parent_joint = fp->joints[hiddenpart->parent_joint_id];

        if (new_parent_joint->child != NULL)
        {
            new_child_joint = new_parent_joint->child;
            root_joint->child = new_parent_joint->child;

            do
            {
                new_child_joint->parent = root_joint;
                new_child_joint = new_child_joint->sib_next;
            } 
            while (new_child_joint != NULL);
        }
        new_parent_joint->child = root_joint;
        root_joint->parent = new_parent_joint;
    }
}

// 0x800E6E00
void ftMainEjectHiddenPartID(FTStruct *fp, s32 hiddenpart_id)
{
    FTHiddenPart *hiddenpart = &fp->attr->hiddenparts[hiddenpart_id];
    DObj *root_joint = fp->joints[hiddenpart->root_joint_id];
    DObj *parent_joint;
    DObj *child_joint;
    DObj *sibling_joint;
    DObj *new_sibling_joint;

    ftManagerSetPrevPartsAlloc(ftGetParts(root_joint));

    child_joint = root_joint->child;
    parent_joint = root_joint->parent;

    if (child_joint != NULL)
    {
        sibling_joint = child_joint;

        if (root_joint->sib_prev == NULL)
        {
            parent_joint->child = child_joint;
        }
        else
        {
            root_joint->sib_prev->sib_next = sibling_joint;
            sibling_joint->sib_prev = (child_joint = root_joint)->sib_prev;
        }
        new_sibling_joint = sibling_joint->sib_next;
        sibling_joint->parent = parent_joint;

        while (new_sibling_joint != NULL)
        {
            sibling_joint = new_sibling_joint;
            new_sibling_joint->parent = parent_joint;
            new_sibling_joint = new_sibling_joint->sib_next;
        }
        sibling_joint->sib_next = root_joint->sib_next;
        new_sibling_joint = root_joint->sib_next;

        if (new_sibling_joint != NULL)
        {
            new_sibling_joint->sib_prev = sibling_joint;
        }
    }
    else
    {
        if (root_joint->sib_prev == NULL)
        {
            parent_joint->child = root_joint->sib_next;
        }
        else root_joint->sib_prev->sib_next = root_joint->sib_next;
        
        if (root_joint->sib_next != NULL)
        {
            root_joint->sib_next->sib_prev = root_joint->sib_prev;
        }
    }
    fp->joints[hiddenpart->root_joint_id] = NULL;
    root_joint->sib_next = NULL;
    root_joint->sib_prev = NULL;
    root_joint->child = NULL;

    gcEjectDObj(root_joint);
}

// 0x800E6F24
void ftMainSetStatus(GObj *fighter_gobj, s32 status_id, f32 frame_begin, f32 anim_speed, u32 flags)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    intptr_t event_file_head;
    FTAttributes *attr = fp->attr;
    FTStatusDesc *status_struct;
    FTOpeningDesc *opening_struct;
    f32 anim_frame;
    s32 status_struct_id;
    s32 anim_desc_update;
    s32 anim_desc_bak;
    s32 unused1;
    GMStatFlags status_flags;
    GMStatFlags attack_flags;
    s32 unused2;
    s32 motion_id;
    void *event_script_ptr;
    DObjDesc *dobjdesc;
    s32 unused3;
    FTStatusDesc *status_desc;
    FTMotionDescArray *script_array;
    FTMotionDesc *motion_desc;
    DObj *joint;
    DObj *transn_parent;                // Parent of TrasnN_Joint
    DObj *transn_child;                 // Child of TransN_Joint
    s32 i;

    status_struct = NULL;
    opening_struct = NULL;

    status_flags = fp->stat_flags;

    if (fp->is_events_forward)
    {
        ftMainUpdateMotionEventsForwardEffect(fighter_gobj);

        if (fp->proc_accessory != NULL)
        {
            fp->proc_accessory(fighter_gobj);
        }
    }
    fp->status_id = status_id;

    if (fp->detail_curr != fp->detail_base)
    {
        ftParamSetModelPartDetailAll(fighter_gobj, fp->detail_base);
        
        fp->detail_curr = fp->detail_base;
    }
    if (!(flags & FTSTATUS_PRESERVE_HIT) && (fp->is_attack_active))
    {
        ftParamClearAttackCollAll(fighter_gobj);
    }
    if (!(flags & FTSTATUS_PRESERVE_THROWPOINTER))
    {
        fp->throw_gobj = NULL;
    }
    if (!(flags & FTSTATUS_PRESERVE_HITSTATUS))
    {
        if (fp->is_hitstatus_nodamage)
        {
            ftParamSetHitStatusPartAll(fighter_gobj, nGMHitStatusNormal);
        }
        if (fp->hitstatus != nGMHitStatusNormal)
        {
            ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusNormal);
        }
    }
    if (fp->is_damage_coll_modify)
    {
        ftParamResetFighterDamageCollsAll(fighter_gobj);
    }
    if (!(flags & FTSTATUS_PRESERVE_MODELPART) && (fp->is_modelpart_modify))
    {
        ftParamResetModelPartAll(fighter_gobj);
    }
    if (!(flags & FTSTATUS_PRESERVE_TEXTUREPART) && (fp->is_texturepart_modify))
    {
        ftParamResetTexturePartAll(fighter_gobj);
    }
    if (!(flags & FTSTATUS_PRESERVE_COLANIM) && (dGMColScriptsDescs[fp->colanim.colanim_id].is_unlocked != FALSE))
    {
        ftParamResetStatUpdateColAnim(fighter_gobj);
    }
    if (!(flags & FTSTATUS_PRESERVE_EFFECT) && (fp->is_effect_attach))
    {
        ftParamProcStopEffect(fighter_gobj);
    }
    fp->is_reflect = FALSE;
    fp->is_absorb = FALSE;
    fp->is_shield = FALSE;

    if (!(flags & FTSTATUS_PRESERVE_FASTFALL))
    {
        fp->is_fastfall = FALSE;
    }

    fp->is_invisible = FALSE;
    fp->is_shadow_hide = FALSE;
    fp->is_rebirth = FALSE;
    fp->is_playertag_hide = FALSE;
    fp->is_effect_skip = FALSE; // Not sure exactly what this is, but it prevents certain ColAnim events from running if true?

    if (fp->pkind != nFTPlayerKindDemo)
    {
        gmRumbleStopRumbleID(fp->player, 2);
        gmRumbleStopRumbleID(fp->player, 3);

        if (!(flags & FTSTATUS_PRESERVE_RUMBLE))
        {
            gmRumbleStopRumbleID(fp->player, 7);
        }
        fp->joints[nFTPartsJointTopN]->rotate.vec.f.y = fp->lr * F_CLC_DTOR32(90.0F);

        DObjGetStruct(fighter_gobj)->rotate.vec.f.z = 0.0F;

        fp->joints[nFTPartsJointTopN]->rotate.vec.f.x = DObjGetStruct(fighter_gobj)->rotate.vec.f.z;

        fp->physics.vel_air.z = 0.0F;
        fp->physics.vel_ground.z = 0.0F;
    }

    fp->is_jostle_ignore = FALSE;
    fp->is_hitstun = FALSE;

    fp->damage_mul = 1.0F;

    if ((fp->ga == nMPKineticsGround) && !(flags & FTSTATUS_PRESERVE_DAMAGEPLAYER))
    {
        fp->damage_player = -1;
    }
    if (!(flags & FTSTATUS_PRESERVE_SLOPECONTOUR))
    {
        fp->slope_contour = 0;
    }
    fp->coll_data.ignore_line_id = -1;

    fp->is_item_show = TRUE;
    fp->is_cliff_hold = FALSE;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_NONE);

    fp->is_ghost = FALSE;
    fp->is_damage_resist = FALSE;
    fp->is_menu_ignore = FALSE;

    if (fp->camera_mode != nFTCameraModeEntry)
    {
        fp->camera_mode = nFTCameraModeDefault;
    }
    fp->camera_zoom_range = 1.0F;

    if (!(flags & FTSTATUS_PRESERVE_PLAYERTAG))
    {
        fp->playertag_wait = 0;
    }
    fp->is_special_interrupt = FALSE;
    fp->is_catchstatus = FALSE;
    fp->is_ignore_dead = FALSE;

    if (!(flags & FTSTATUS_PRESERVE_SHUFFLETIME))
    {
        fp->shuffle_tics = 0;
    }
    if (!(flags & FTSTATUS_PRESERVE_LOOPSFX))
    {
        ftParamStopLoopSFX(fp);
    }
    fp->knockback_resist_status = 0.0F;
    fp->damage_knockback_stack = 0.0F;

    if (!(flags & FTSTATUS_PRESERVE_AFTERIMAGE))
    {
        fp->afterimage.drawstatus = -1;
    }
    if ((status_id != nFTCommonStatusWait) && (status_id != nFTCommonStatusWalkSlow) && (status_id != nFTCommonStatusWalkMiddle) && (status_id != nFTCommonStatusWalkFast))
    {
        fp->attack1_followup_frames = 0.0F;
    }
    if ((fp->pkind != nFTPlayerKindDemo) && (fp->dl_link != FTDISPLAY_DLLINK_DEFAULT))
    {
        ftParamMoveDLLink(fighter_gobj, 9);
    }
    fp->status_total_tics = 0;

    if (status_id >= FTSTAT_CHARDATA_START) // Check if Character Data status ID
    {
        status_id -= FTSTAT_CHARDATA_START;
    }
    if (status_id >= FTSTAT_OPENING1_START) // Check if Opening status ID 1
    {
        opening_struct = D_ovl1_80390D20[fp->fkind];
        status_struct_id = status_id - FTSTAT_OPENING1_START;
    }
    else if (status_id >= FTSTAT_OPENING2_START) // Check if Opening status ID 2
    {
        opening_struct = &D_ovl1_80390BE8;
        status_struct_id = status_id - FTSTAT_OPENING2_START;
    }
    else if (status_id >= nFTCommonStatusSpecialStart)
    {
        status_struct = dFTMainSpecialStatusDescs[fp->fkind];
        status_struct_id = status_id - nFTCommonStatusSpecialStart;
    }
    else if (status_id >= nFTCommonStatusActionStart)
    {
        status_struct = dFTCommonActionStatusDescs;
        status_struct_id = status_id - nFTCommonStatusActionStart;
    }
    else
    {
        status_struct = dFTCommonNullStatusDescs;
        status_struct_id = status_id;
    }
    status_desc = &status_struct[status_struct_id];

    if (fp->pkind != nFTPlayerKindDemo)
    {
        if ((status_struct[status_struct_id].mflags.attack_id == nFTMotionAttackIDNone) || (status_struct[status_struct_id].mflags.attack_id != fp->motion_attack_id))
        {
            ftParamSetMotionID(fp, status_struct[status_struct_id].mflags.attack_id);
        }
        attack_flags = status_desc->sflags;

        if ((attack_flags.attack_id == nFTStatusAttackIDNone) || (attack_flags.attack_id != fp->stat_flags.attack_id))
        {
            ftParamSetStatUpdate(fp, status_struct[status_struct_id].sflags.halfword);
        }
    }
    if (fp->proc_status != NULL)
    {
        fp->proc_status(fighter_gobj);
        fp->proc_status = NULL;
    }
    ftParamUpdate1PGameAttackStats(fp, status_flags.halfword);

    if (status_struct != NULL)
    {
        motion_id = status_struct[status_struct_id].mflags.motion_id;
        fp->motion_id = motion_id;
        script_array = fp->data->mainmotion;
    }
    else
    {
        motion_id = opening_struct[status_struct_id].motion_id - 0x10000;
        fp->motion_id = motion_id;
        script_array = fp->data->submotion;
    }
    if ((motion_id != -1) && (motion_id != -2))
    {
        motion_desc = &script_array->motion_desc[motion_id];

        if (motion_desc->anim_desc.flags.is_use_shieldpose)
        {
            fp->figatree = (void*) ((intptr_t)motion_desc->anim_file_id + (uintptr_t)fp->data->p_file_shieldpose);
        }
        else if (motion_desc->anim_file_id != 0)
        {
            lbRelocGetForceExternHeapFile(motion_desc->anim_file_id, (void*) fp->figatree_heap);
            fp->figatree = fp->figatree_heap;
        }
        else fp->figatree = NULL;
        
        if (fp->figatree != NULL)
        {
            anim_desc_bak =
            fp->anim_desc.word & ~(FTANIM_FLAG_SUBMOTION_SCRIPT | FTANIM_FLAG_ANIMJOINT | FTANIM_FLAG_TRANSLATE_SCALES | FTANIM_FLAG_SHIELDPOSE | FTANIM_FLAG_ANIMLOCKS);
            
            fp->anim_desc.word = motion_desc->anim_desc.word;

            anim_desc_update =
            fp->anim_desc.word & ~(FTANIM_FLAG_SUBMOTION_SCRIPT | FTANIM_FLAG_ANIMJOINT | FTANIM_FLAG_TRANSLATE_SCALES | FTANIM_FLAG_SHIELDPOSE | FTANIM_FLAG_ANIMLOCKS);

            for (i = 0; ((anim_desc_bak != 0) || (anim_desc_update != 0)); i++, anim_desc_update <<= 1, anim_desc_bak <<= 1)
            {
                if (!(anim_desc_bak & (1 << 31)))
                {
                    if (anim_desc_update & (1 << 31))
                    {
                        ftMainUpdateHiddenPartID(fp, i);
                    }
                }
                else if (anim_desc_update & (1 << 31))
                {
                    ftMainAddHiddenPartID(fp, i);
                }
                else ftMainEjectHiddenPartID(fp, i);
            }
            dobjdesc = attr->commonparts_container->commonparts[fp->detail_curr - nFTPartsDetailStart].dobjdesc;

            for (i = nFTPartsJointCommonStart; dobjdesc->id != DOBJ_ARRAY_MAX; i++, dobjdesc++)
            {
                joint = fp->joints[i];

                if (joint != NULL)
                {
                    joint->translate.vec.f = dobjdesc->translate;

                    joint->rotate.vec.f = dobjdesc->rotate;

                    joint->scale.vec.f = dobjdesc->scale;

                    joint->flags = DOBJ_FLAG_NONE;
                }
            }
            if (fp->anim_desc.flags.is_use_transn_joint)
            {
                joint = fp->joints[nFTPartsJointTransN];
                
                joint->translate.vec.f.x = joint->translate.vec.f.y = joint->translate.vec.f.z = 0.0F;

                joint->rotate.vec.f.z = 0.0F;

                joint->flags = DOBJ_FLAG_NONE;
            }
            if (fp->anim_desc.flags.is_use_xrotn_joint)
            {
                joint = fp->joints[nFTPartsJointXRotN];

                joint->translate.vec.f.x = joint->translate.vec.f.y = joint->translate.vec.f.z = 0.0F;
                
                joint->rotate.vec.f.x = joint->rotate.vec.f.y = joint->rotate.vec.f.z = 0.0F; 
                
                joint->scale.vec.f.x = joint->scale.vec.f.y = joint->scale.vec.f.z = 1.0F;

                joint->flags = DOBJ_FLAG_NONE;
            }
            if (fp->anim_desc.flags.is_use_yrotn_joint)
            {
                joint = fp->joints[nFTPartsJointYRotN];

                joint->translate.vec.f.x = joint->translate.vec.f.y = joint->translate.vec.f.z = 0.0F;

                joint->rotate.vec.f.x = joint->rotate.vec.f.y = joint->rotate.vec.f.z = 0.0F;

                joint->scale.vec.f.x = joint->scale.vec.f.y = joint->scale.vec.f.z = 1.0F;

                joint->flags = DOBJ_FLAG_NONE;
            }
            lbCommonAddFighterPartsFigatree(fp->joints[nFTPartsJointTopN]->child, fp->figatree, frame_begin);

            if (anim_speed != DObjGetStruct(fighter_gobj)->anim_speed)
            {
                gcSetAnimSpeed(fighter_gobj, anim_speed);
            }
            if (fp->anim_desc.flags.is_use_transn_joint)
            {
                joint = fp->joints[nFTPartsJointTransN];

                transn_parent = joint->parent;
                transn_child = joint->child;
                transn_parent->child = transn_child;
                transn_child->parent = transn_parent;
                transn_child->sib_next = joint;
                joint->sib_prev = transn_child;
                joint->parent = transn_child->parent;
                joint->child = NULL;
            }

            if (fp->is_use_animlocks)
            {
                if (!(fp->anim_desc.flags.is_use_animlocks))
                {
                    ftParamSetAnimLocks(fp);
                }
            }
            else if (fp->anim_desc.flags.is_use_animlocks)
            {
                ftParamClearAnimLocks(fp);
            }
            fp->is_use_animlocks = fp->anim_desc.flags.is_use_animlocks;

            if (attr->translate_scales != NULL)
            {
                if (fp->anim_desc.flags.is_have_translate_scale)
                {
                    fp->is_have_translate_scale = FALSE;
                }
                else fp->is_have_translate_scale = TRUE;
            }
        }
        if (status_struct != NULL)
        {
            if (motion_desc->offset != 0x80000000)
            {
                // Actually subaction scripts?
                if (fp->anim_desc.flags.is_use_submotion_script)
                {
                    event_file_head = *fp->data->p_file_submotion;

                    event_script_ptr = (void*) ((intptr_t)motion_desc->offset + (intptr_t)event_file_head);
                }
                else
                {
                    event_file_head = *fp->data->p_file_mainmotion;

                    event_script_ptr = (void*) ((intptr_t)motion_desc->offset + (intptr_t)event_file_head);
                }
            }
            else event_script_ptr = NULL;

            fp->motion_scripts[0][0].p_script = fp->motion_scripts[1][0].p_script = event_script_ptr;
        }
        else
        {
            if (motion_desc->offset != 0x80000000)
            {
                event_script_ptr = (void*) motion_desc->offset;
            }
            else event_script_ptr = NULL;

            fp->motion_scripts[0][0].p_script = fp->motion_scripts[1][0].p_script = event_script_ptr;
        }
        anim_frame = DObjGetStruct(fighter_gobj)->anim_speed - frame_begin;

        fp->motion_scripts[0][0].script_wait = fp->motion_scripts[1][0].script_wait = anim_frame;
        fp->motion_scripts[0][0].script_id = fp->motion_scripts[1][0].script_id = 0;

        for (i = 1; i < ARRAY_COUNT(fp->motion_scripts[0]); i++)
        {
            fp->motion_scripts[0][i].p_script = fp->motion_scripts[1][i].p_script = NULL;
        }
        if (frame_begin != 0.0F)
        {
            ftMainPlayAnimEventsForward(fighter_gobj);
        }
        else
        {
            ftMainPlayAnimEventsAll(fighter_gobj);
            ftMainRunUpdateColAnim(fighter_gobj);
        }
    }
    else for (i = 0; i < ARRAY_COUNT(fp->motion_scripts[0]); i++)
    {
        fp->motion_scripts[0][i].p_script = fp->motion_scripts[1][i].p_script = NULL;
    }
    if (fp->pkind != nFTPlayerKindDemo)
    {
        fp->proc_update = status_struct[status_struct_id].proc_update;
        fp->proc_interrupt = status_struct[status_struct_id].proc_interrupt;
        fp->proc_physics = status_struct[status_struct_id].proc_physics;
        fp->proc_map = status_struct[status_struct_id].proc_map;
        fp->proc_slope = mpCommonUpdateFighterSlopeContour;
        fp->proc_accessory = NULL;
        fp->proc_damage = NULL;
        fp->proc_trap = NULL;
        fp->proc_hit = NULL;
        fp->proc_shield = NULL;
        fp->proc_passive = NULL;
        fp->proc_lagupdate = NULL;
        fp->proc_lagstart = NULL;
        fp->proc_lagend = NULL;
    }
    else if (opening_struct != NULL)
    {
        fp->proc_update = opening_struct[status_struct_id].proc_update;
    }
    else fp->proc_update = NULL;
}
