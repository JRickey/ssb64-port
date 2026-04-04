#include <ft/fighter.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x801886E0
FTThrowHitDesc dFTCommonThrownNoDamageKnockback = { -1, 0, 361, 0, 0, 20, 0 };

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018CF80
s32 sFTCommonThrownScriptID;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014ADB0
void ftCommonThrownReleaseFighterLoseGrip(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *interact_gobj;
    FTStruct *interact_fp;
    Vec3f pos;

    if (this_fp->is_catch_or_capture)
    {
        interact_gobj = this_fp->catch_gobj;
    }
    else interact_gobj = this_fp->capture_gobj;

    interact_fp = ftGetStruct(interact_gobj);

    if ((this_fp->status_id >= nFTCommonStatusThrownStart) && (this_fp->status_id <= nFTCommonStatusThrownEnd))
    {
        pos.x = pos.y = pos.z = 0.0F;

        gmCollisionGetFighterPartsWorldPosition(this_fp->joints[4], &pos);

        pos.y -= 300.0F;

        DObjGetStruct(fighter_gobj)->translate.vec.f = pos;
    }
    mpCommonRunFighterCollisionDefault(fighter_gobj, &DObjGetStruct(interact_gobj)->translate.vec.f, &interact_fp->coll_data);

    if (this_fp->ga == nMPKineticsGround)
    {
        if ((this_fp->coll_data.floor_line_id == -1) || (this_fp->coll_data.floor_dist != 0.0F))
        {
            mpCommonSetFighterAir(this_fp);
        }
    }
}

// 0x8014AECC
void ftCommonThrownDecideFighterLoseGrip(GObj *fighter_gobj, GObj *interact_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    FTStruct *interact_fp = ftGetStruct(interact_gobj);

    if (this_fp->is_catch_or_capture)
    {
        ftCommonThrownReleaseFighterLoseGrip(fighter_gobj);
    }
    else ftCommonThrownReleaseFighterLoseGrip(interact_gobj);

    interact_fp->capture_gobj = NULL;
    this_fp->catch_gobj = NULL;
}

// 0x8014AF2C
void ftCommonThrownDecideDeadResult(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *interact_gobj = fp->catch_gobj;

    if (interact_gobj != NULL)
    {
        ftCommonThrownDecideFighterLoseGrip(fighter_gobj, interact_gobj);

        goto next;
    }
    else interact_gobj = fp->capture_gobj;

    if (interact_gobj != NULL)
    {
        ftCommonThrownDecideFighterLoseGrip(interact_gobj, fighter_gobj);

    next:
        mpCommonSetFighterWaitOrFall(fighter_gobj);
        mpCommonSetFighterWaitOrFall(interact_gobj);
    }
}

// 0x8014AF98
void ftCommonThrownProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamSetThrowParams(fp, fp->capture_gobj);

    fp->status_vars.common.damage.script_id = sFTCommonThrownScriptID;
}

// 0x8014AFD0
void ftCommonThrownReleaseThrownUpdateStats(GObj *fighter_gobj, s32 lr, s32 script_id, sb32 is_proc_status)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *capture_gobj = this_fp->capture_gobj;
    FTStruct *capture_fp = ftGetStruct(capture_gobj);
    FTThrowHitDesc *ft_throw;
    f32 knockback_final;
    s32 damage;
    f32 knockback_resist;
    f32 knockback_calc;

    knockback_resist = (this_fp->knockback_resist_status < this_fp->knockback_resist_passive) ? this_fp->knockback_resist_passive : this_fp->knockback_resist_status;

    sFTCommonThrownScriptID = script_id;

    if (this_fp->hitstatus != nGMHitStatusNormal)
    {
        ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusNormal);
    }
    if (!(this_fp->is_catch_or_capture))
    {
        ftCommonThrownReleaseFighterLoseGrip(fighter_gobj);
    }
    mpCommonSetFighterAir(this_fp);

    ft_throw = capture_fp->throw_desc;

    knockback_calc = ftParamGetCommonKnockback(this_fp->percent_damage, ft_throw->damage, ft_throw->damage, ft_throw->knockback_weight, ft_throw->knockback_scale, ft_throw->knockback_base, this_fp->attr->weight, capture_fp->handicap, this_fp->handicap);

    knockback_final = knockback_calc - knockback_resist;

    if (knockback_calc <= knockback_resist)
    {
        knockback_final = 0.0F;
    }
    damage = ftParamGetStaledDamage(capture_fp->player, ft_throw->damage, capture_fp->motion_attack_id, capture_fp->motion_count);

    if (capture_fp->is_shield_catch)
    {
        damage = ((damage * 0.5F) + 0.999F);
    }
    if (ftParamGetBestHitStatusAll(fighter_gobj) != nGMHitStatusNormal)
    {
        damage = 0;
    }
    if (is_proc_status != FALSE)
    {
        this_fp->proc_status = ftCommonThrownProcStatus;
    }
    ftCommonDamageInitDamageVars(fighter_gobj, ft_throw->status_id, damage, knockback_final, ft_throw->angle, lr, 1, ft_throw->element, capture_fp->player_num, TRUE, TRUE, TRUE);
    ftParamUpdate1PGameDamageStats(this_fp, capture_fp->player, nFTHitLogObjectFighter, capture_fp->fkind, capture_fp->stat_flags.halfword, capture_fp->stat_count);

    if (damage != 0)
    {
        ftParamUpdateDamage(this_fp, damage);
        ftParamUpdatePlayerBattleStats(capture_fp->player, this_fp->player, damage);
        ftParamUpdateStaleQueue(capture_fp->player, this_fp->player, capture_fp->motion_attack_id, capture_fp->motion_count);

        if ((s32) ((damage * 0.75F) + 4.0F) > 0)
        {
            ftParamMakeRumble(this_fp, 0, (s32) ((damage * 0.75F) + 4.0F));
        }
        if ((s32) ((damage * 0.5F) + 2.0F) > 0)
        {
            ftParamMakeRumble(capture_fp, 5, (s32) ((damage * 0.5F) + 2.0F));
        }
    }
    this_fp->capture_gobj = NULL;
}

// 0x8014B2AC
void ftCommonThrownUpdateDamageStats(FTStruct *this_fp)
{
    GObj *capture_gobj = this_fp->capture_gobj;
    FTStruct *capture_fp = ftGetStruct(capture_gobj);
    FTThrowHitDesc *ft_throw = &capture_fp->throw_desc[1];
    s32 damage = ftParamGetStaledDamage(capture_fp->player, ft_throw->damage, capture_fp->motion_attack_id, capture_fp->motion_count);

    ftParamUpdateDamage(this_fp, damage);
    ftParamUpdatePlayerBattleStats(capture_fp->player, this_fp->player, damage);
    ftParamUpdateStaleQueue(capture_fp->player, this_fp->player, capture_fp->motion_attack_id, capture_fp->motion_count);
}

// 0x8014B330
void ftCommonThrownSetStatusDamageRelease(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *capture_gobj = this_fp->capture_gobj;
    FTStruct *capture_fp = ftGetStruct(capture_gobj);
    FTThrowHitDesc *ft_throw;
    f32 knockback_final;
    s32 lr;
    s32 damage;
    f32 knockback_resist;
    f32 knockback_calc;

    knockback_resist = (this_fp->knockback_resist_status < this_fp->knockback_resist_passive) ? this_fp->knockback_resist_passive : this_fp->knockback_resist_status;

    if (this_fp->hitstatus != nGMHitStatusNormal)
    {
        ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusNormal);
    }
    if (!(this_fp->is_catch_or_capture))
    {
        ftCommonThrownReleaseFighterLoseGrip(fighter_gobj);
    }
    if (this_fp->ga == nMPKineticsAir)
    {
        mpCommonSetFighterAir(this_fp);
    }
    ft_throw = &capture_fp->throw_desc[1];

    knockback_calc = ftParamGetCommonKnockback(this_fp->percent_damage, ft_throw->damage, ft_throw->damage, ft_throw->knockback_weight, ft_throw->knockback_scale, ft_throw->knockback_base, this_fp->attr->weight, capture_fp->handicap, this_fp->handicap);

    knockback_final = knockback_calc - knockback_resist;

    if (knockback_calc <= knockback_resist)
    {
        knockback_final = 0.0F;
    }
    lr = (DObjGetStruct(fighter_gobj)->translate.vec.f.x < DObjGetStruct(capture_gobj)->translate.vec.f.x) ? +1 : -1;

    damage = ftParamGetStaledDamage(capture_fp->player, ft_throw->damage, capture_fp->motion_attack_id, capture_fp->motion_count);

    if (capture_fp->is_shield_catch)
    {
        damage = ((damage * 0.5F) + 0.999F);
    }
    if (ftParamGetBestHitStatusAll(fighter_gobj) != nGMHitStatusNormal)
    {
        damage = 0;
    }
    ftCommonDamageInitDamageVars(fighter_gobj, ft_throw->status_id, damage, knockback_final, ft_throw->angle, lr, 1, nGMHitElementNormal, capture_fp->player_num, FALSE, FALSE, TRUE);
    ftParamUpdate1PGameDamageStats(this_fp, capture_fp->player, nFTHitLogObjectFighter, capture_fp->fkind, capture_fp->stat_flags.halfword, capture_fp->stat_count);

    if (damage != 0)
    {
        ftParamUpdateDamage(this_fp, damage);
        ftParamUpdatePlayerBattleStats(capture_fp->player, this_fp->player, damage);
        ftParamUpdateStaleQueue(capture_fp->player, this_fp->player, capture_fp->motion_attack_id, capture_fp->motion_count);
    }
    this_fp->capture_gobj = NULL;
}

// 0x8014B5B4
void ftCommonThrownSetStatusNoDamageRelease(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTThrowHitDesc *ft_throw = &dFTCommonThrownNoDamageKnockback;
    f32 knockback_calc;
    f32 knockback_resist;
    f32 knockback_final;

    if (fp->knockback_resist_status < fp->knockback_resist_passive)
    {
        knockback_resist = fp->knockback_resist_passive;
    }
    else knockback_resist = fp->knockback_resist_status;

    DObjGetStruct(fighter_gobj)->translate.vec.f.z = 0.0F;

    knockback_calc = ftParamGetCommonKnockback(fp->percent_damage, ft_throw->damage, ft_throw->damage, ft_throw->knockback_weight, ft_throw->knockback_scale, ft_throw->knockback_base, fp->attr->weight, 9, fp->handicap);

    knockback_final = knockback_calc - knockback_resist;

    if (knockback_calc <= knockback_resist)
    {
        knockback_final = 0.0F;
    }
    ftCommonDamageInitDamageVars(fighter_gobj, ft_throw->status_id, 0, knockback_final, ft_throw->angle, fp->lr, 1, ft_throw->element, 0, TRUE, TRUE, FALSE);
    ftParamUpdate1PGameDamageStats(fp, GMCOMMON_PLAYERS_MAX, nFTHitLogObjectNone, 0, 0, 0);
}
