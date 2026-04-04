#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8016F280
void itProcessUpdateAttackPositions(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    s32 i;

    for (i = 0; i < ip->attack_coll.attack_count; i++)
    {
        switch (ip->attack_coll.attack_state)
        {
        case nGMAttackStateOff:
            break;

        case nGMAttackStateNew:
            ip->attack_coll.attack_pos[i].pos_curr.x = ip->attack_coll.offsets[i].x + DObjGetStruct(item_gobj)->translate.vec.f.x;
            ip->attack_coll.attack_pos[i].pos_curr.y = ip->attack_coll.offsets[i].y + DObjGetStruct(item_gobj)->translate.vec.f.y;
            ip->attack_coll.attack_pos[i].pos_curr.z = ip->attack_coll.offsets[i].z + DObjGetStruct(item_gobj)->translate.vec.f.z;

            ip->attack_coll.attack_state = nGMAttackStateTransfer;

            ip->attack_coll.attack_pos[i].unk_ithitpos_0x18 = FALSE;
            ip->attack_coll.attack_pos[i].unk_ithitpos_0x5C = 0;
            break;

        case nGMAttackStateTransfer:
            ip->attack_coll.attack_state = nGMAttackStateInterpolate;

        case nGMAttackStateInterpolate:
            ip->attack_coll.attack_pos[i].pos_prev = ip->attack_coll.attack_pos[i].pos_curr;

            ip->attack_coll.attack_pos[i].pos_curr.x = ip->attack_coll.offsets[i].x + DObjGetStruct(item_gobj)->translate.vec.f.x;
            ip->attack_coll.attack_pos[i].pos_curr.y = ip->attack_coll.offsets[i].y + DObjGetStruct(item_gobj)->translate.vec.f.y;
            ip->attack_coll.attack_pos[i].pos_curr.z = ip->attack_coll.offsets[i].z + DObjGetStruct(item_gobj)->translate.vec.f.z;

            ip->attack_coll.attack_pos[i].unk_ithitpos_0x18 = FALSE;
            ip->attack_coll.attack_pos[i].unk_ithitpos_0x5C = 0;
            break;
        }
    }
}

// 0x8016F3D4
void itProcessUpdateAttackRecords(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    GMAttackRecord *record;
    ITAttackColl *it_attack_coll;
    s32 i;

    it_attack_coll = &ip->attack_coll;

    if (it_attack_coll->attack_state != nGMAttackStateOff)
    {
        for (i = 0; i < ARRAY_COUNT(ip->attack_coll.attack_records); i++)
        {
            record = &it_attack_coll->attack_records[i];

            if (record->victim_gobj != NULL)
            {
                if (record->victim_flags.timer_rehit > 0)
                {
                    record->victim_flags.timer_rehit--;

                    if (record->victim_flags.timer_rehit <= 0)
                    {
                        record->victim_gobj = NULL;

                        record->victim_flags.is_interact_hurt = record->victim_flags.is_interact_shield = record->victim_flags.is_interact_reflect = FALSE;

                        record->victim_flags.group_id = 7;
                    }
                }
            }
        }
    }
}

// 0x8016F534
void itProcessProcItemMain(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->hitlag_tics > 0)
    {
        ip->hitlag_tics--;
    }
    if (ip->hitlag_tics <= 0)
    {
        gcPlayAnimAll(item_gobj);
    }
    if (ip->hitlag_tics <= 0)
    {
        if (ip->proc_update != NULL)
        {
            if (ip->proc_update(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
    }
    if (ip->is_allow_pickup)
    {
        ip->pickup_wait--;

        if (ip->pickup_wait <= ITEM_DESPAWN_FLASH_BEGIN_DEFAULT)
        {
            if (ip->pickup_wait == 0)
            {
                efManagerSparkleWhiteScaleMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f, 1.0F);

                itMainDestroyItem(item_gobj);
                return;
            }
            if ((ip->pickup_wait % 2) != 0) // Make item invisible on odd frames; when in doubt, simply do "& 1"
            {
                item_gobj->flags ^= GOBJ_FLAG_HIDDEN;
            }
        }
        if (ip->arrow_timer == 0)
        {
            ip->arrow_timer = ITEM_ARROW_FLASH_INT_DEFAULT;
        }
        ip->arrow_timer--;
    }
    else item_gobj->flags = GOBJ_FLAG_NONE;

    if (!(ip->is_hold))
    {
        Vec3f *translate = &DObjGetStruct(item_gobj)->translate.vec.f;

        ip->coll_data.pos_prev = *translate;

        if (ip->hitlag_tics == 0)
        {
            translate->x += ip->physics.vel_air.x;
            translate->y += ip->physics.vel_air.y;
            translate->z += ip->physics.vel_air.z;
        }
        ip->coll_data.pos_diff.x = translate->x - ip->coll_data.pos_prev.x;
        ip->coll_data.pos_diff.y = translate->y - ip->coll_data.pos_prev.y;
        ip->coll_data.pos_diff.z = translate->z - ip->coll_data.pos_prev.z;

        if ((ip->is_attach_surface) && (mpCollisionCheckExistLineID(ip->attach_line_id) != FALSE))
        {
            MPCollData *coll_data = &ip->coll_data;

            mpCollisionGetSpeedLineID(ip->attach_line_id, &ip->coll_data.vel_speed);

            translate->x += coll_data->vel_speed.x;
            translate->y += coll_data->vel_speed.y;
            translate->z += coll_data->vel_speed.z;
        }
        else if ((ip->ga == nMPKineticsGround) && (ip->coll_data.floor_line_id != -1) && (ip->coll_data.floor_line_id != -2) && (mpCollisionCheckExistLineID(ip->coll_data.floor_line_id) != FALSE))
        {
            mpCollisionGetSpeedLineID(ip->coll_data.floor_line_id, &ip->coll_data.vel_speed);

            translate->x += ip->coll_data.vel_speed.x;
            translate->y += ip->coll_data.vel_speed.y;
            translate->z += ip->coll_data.vel_speed.z;
        }
        else ip->coll_data.vel_speed.x = ip->coll_data.vel_speed.y = ip->coll_data.vel_speed.z = 0.0F;

        if ((translate->y < gMPCollisionGroundData->map_bound_bottom) || (translate->x > gMPCollisionGroundData->map_bound_right) || (translate->x < gMPCollisionGroundData->map_bound_left) || (translate->y > gMPCollisionGroundData->map_bound_top))
        {
            if ((ip->proc_dead == NULL) || (ip->proc_dead(item_gobj) != FALSE))
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
        if (ip->proc_map != NULL)
        {
            ip->coll_data.mask_prev = ip->coll_data.mask_curr;
            ip->coll_data.mask_curr = 0;
            ip->coll_data.is_coll_end = FALSE;
            ip->coll_data.mask_stat = 0;
            ip->coll_data.mask_unk = 0;

            if (ip->proc_map(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
        itProcessUpdateAttackPositions(item_gobj);
        itProcessUpdateAttackRecords(item_gobj);
    }
    itVisualsUpdateColAnim(item_gobj);
}

// 0x8016F930
void itProcessSetHitInteractStats(ITAttackColl *it_attack_coll, GObj *victim_gobj, s32 attack_type, u32 group_id)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(it_attack_coll->attack_records); i++)
    {
        if (victim_gobj == it_attack_coll->attack_records[i].victim_gobj) // Run this if the victim we're checking has already been hit
        {
            switch (attack_type)
            {
            case nGMHitTypeDamage:
                it_attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
                break;

            case nGMHitTypeShield:
                it_attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
                break;

            case nGMHitTypeShieldRehit:
                it_attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
                it_attack_coll->attack_records[i].victim_flags.timer_rehit = ITEM_REHIT_TIME_DEFAULT;
                break;

            case nGMHitTypeReflect:
                it_attack_coll->attack_records[i].victim_flags.is_interact_reflect = TRUE;
                it_attack_coll->attack_records[i].victim_flags.timer_rehit = ITEM_REHIT_TIME_DEFAULT;
                break;

            case nGMHitTypeAttack:
                it_attack_coll->attack_records[i].victim_flags.group_id = group_id;
                break;

            case nGMHitTypeDamageRehit:
                it_attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
                it_attack_coll->attack_records[i].victim_flags.timer_rehit = ITEM_REHIT_TIME_DEFAULT;
                break;

            default: 
                break;
            }
            break;
        }
    }
    if (i == ARRAY_COUNT(it_attack_coll->attack_records)) // Check if all victim slots have been filled
    {
        for (i = 0; i < ARRAY_COUNT(it_attack_coll->attack_records); i++) // Reset hit count and increment until there is an empty slot
        {
            if (it_attack_coll->attack_records[i].victim_gobj == NULL)
            {
                break;
            }
        }
        if (i == ARRAY_COUNT(it_attack_coll->attack_records))
        {
            i = 0; // Reset hit count again if all victim slots are full
        }
        it_attack_coll->attack_records[i].victim_gobj = victim_gobj; // Store victim's pointer to slot

        switch (attack_type)
        {
        case nGMHitTypeDamage:
            it_attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
            break;

        case nGMHitTypeShield:
            it_attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
            break;

        case nGMHitTypeShieldRehit:
            it_attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
            it_attack_coll->attack_records[i].victim_flags.timer_rehit = ITEM_REHIT_TIME_DEFAULT;
            break;

        case nGMHitTypeReflect:
            it_attack_coll->attack_records[i].victim_flags.is_interact_reflect = TRUE;
            it_attack_coll->attack_records[i].victim_flags.timer_rehit = ITEM_REHIT_TIME_DEFAULT;
            break;

        case nGMHitTypeAttack:
            it_attack_coll->attack_records[i].victim_flags.group_id = group_id;
            break;

        case nGMHitTypeDamageRehit:
            it_attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
            it_attack_coll->attack_records[i].victim_flags.timer_rehit = ITEM_REHIT_TIME_DEFAULT;
            break;

        default: 
            break;
        }
    }
}

// 0x8016FB18 - Item's hurtbox gets hit by a fighter
void itProcessUpdateDamageStatFighter(FTStruct *fp, FTAttackColl *attack_coll, ITStruct *ip, ITDamageColl *damage_coll, GObj *fighter_gobj, GObj *item_gobj)
{
    s32 damage;
    f32 damage_knockback;
    Vec3f pos;

    ftMainSetHitInteractStats(fp, attack_coll->group_id, item_gobj, nGMHitTypeDamage, 0, FALSE);

    damage = attack_coll->damage;

    if (fp->attack_damage < damage)
    {
        fp->attack_damage = damage;
    }
    if (damage_coll->hitstatus == nGMHitStatusNormal)
    {
        ip->damage_queue += damage;

        if (ip->damage_highest < damage)
        {
            ip->damage_highest = damage;
            ip->damage_angle = attack_coll->angle;
            ip->damage_element = attack_coll->element;

            ip->damage_lr = (DObjGetStruct(item_gobj)->translate.vec.f.x < DObjGetStruct(fighter_gobj)->translate.vec.f.x) ? +1 : -1;

            ip->damage_gobj = fighter_gobj;
            ip->damage_team = fp->team;
            ip->damage_port = fp->player;
            ip->damage_player_num = fp->player_num;
            ip->damage_handicap = fp->handicap;
            ip->damage_display_mode = fp->display_mode;
        }
        if (ip->is_allow_knockback)
        {
            damage_knockback = ftParamGetCommonKnockback
            (
                ip->percent_damage,
                ip->damage_queue,
                damage,
                attack_coll->knockback_weight,
                attack_coll->knockback_scale,
                attack_coll->knockback_base,
                1.0F,
                fp->handicap,
                ip->handicap
            );
            if (ip->damage_knockback < damage_knockback)
            {
                ip->damage_knockback = damage_knockback;
            }
        }
        gmCollisionGetFighterAttackItemDamagePosition(&pos, attack_coll, damage_coll, item_gobj);

        switch (attack_coll->element)
        {
        case nGMHitElementFire:
            efManagerDamageFireMakeEffect(&pos, attack_coll->damage);
            break;

        case nGMHitElementElectric:
            efManagerDamageElectricMakeEffect(&pos, attack_coll->damage);
            break;

        case nGMHitElementCoin:
            efManagerDamageCoinMakeEffect(&pos);
            break;

        case nGMHitElementSlash:
            efManagerDamageSlashMakeEffect(&pos, attack_coll->damage, gmCollisionGetDamageSlashRotation(fp, attack_coll));
            break;

        default:
            efManagerDamageNormalLightMakeEffect(&pos, fp->player, attack_coll->damage, 0);
            break;
        }
    }
    ftMainPlayHitSFX(fp, attack_coll);
}

// 0x8016FD4C
void itProcessUpdateAttackStatItem(ITStruct *this_ip, ITAttackColl *this_hit, s32 this_attack_id, ITStruct *victim_ip, ITAttackColl *victim_hit, s32 victim_attack_id, GObj *this_gobj, GObj *victim_gobj)
{
    s32 victim_hit_damage = itMainGetDamageOutput(victim_ip);
    s32 this_hit_damage = itMainGetDamageOutput(this_ip);
    Vec3f pos;
    s32 highest_priority;

    gmCollisionGetItemAttackItemAttackPosition(&pos, victim_hit, victim_attack_id, this_hit, this_attack_id);

    highest_priority = this_hit->priority;

    if (victim_hit->priority <= highest_priority)
    {
        itProcessSetHitInteractStats(victim_hit, this_gobj, nGMHitTypeAttack, 0);

        if (victim_ip->hit_attack_damage < victim_hit_damage)
        {
            victim_ip->hit_attack_damage = victim_hit_damage;
        }
        efManagerSetOffMakeEffect(&pos, victim_hit_damage);
    }
    highest_priority = victim_hit->priority;

    if (this_hit->priority <= highest_priority)
    {
        itProcessSetHitInteractStats(this_hit, victim_gobj, nGMHitTypeAttack, 0);

        if (this_ip->hit_attack_damage < this_hit_damage)
        {
            this_ip->hit_attack_damage = this_hit_damage;
        }
        efManagerSetOffMakeEffect(&pos, this_hit_damage);
    }
}

// 0x8016FE4C
void itProcessUpdateAttackStatWeapon(WPStruct *wp, WPAttackColl *wp_attack_coll, s32 wp_attack_id, ITStruct *ip, ITAttackColl *it_attack_coll, s32 it_attack_id, GObj *weapon_gobj, GObj *item_gobj)
{
    s32 wp_attack_coll_damage = wpMainGetStaledDamage(wp);
    s32 it_attack_coll_damage = itMainGetDamageOutput(ip);
    Vec3f pos;
    s32 highest_priority;

    gmCollisionGetWeaponAttackItemAttackPosition(&pos, wp_attack_coll, wp_attack_id, it_attack_coll, it_attack_id);

    highest_priority = wp_attack_coll->priority;

    if (it_attack_coll->priority <= highest_priority)
    {
        itProcessSetHitInteractStats(it_attack_coll, weapon_gobj, nGMHitTypeAttack, 0);

        if (ip->hit_attack_damage < it_attack_coll_damage)
        {
            ip->hit_attack_damage = it_attack_coll_damage;
        }
        efManagerSetOffMakeEffect(&pos, it_attack_coll_damage);
    }
    highest_priority = it_attack_coll->priority;

    if (wp_attack_coll->priority <= highest_priority)
    {
        wpProcessUpdateHitInteractStats(wp, wp_attack_coll, item_gobj, nGMHitTypeAttack, 0);

        if (wp->hit_attack_damage < wp_attack_coll_damage)
        {
            wp->hit_attack_damage = wp_attack_coll_damage;
        }
        efManagerSetOffMakeEffect(&pos, wp_attack_coll_damage);
    }
}

// 0x8016FF4C - Item's hurtbox gets hit by another item
void itProcessUpdateDamageStatItem(ITStruct *attack_ip, ITAttackColl *attack_coll, s32 attack_id, ITStruct *defend_ip, ITDamageColl *damage_coll, GObj *attack_gobj, GObj *defend_gobj)
{
    s32 damage;
    f32 knockback;
    sb32 is_rehit;
    f32 vel;
    Vec3f pos;
    s32 lr;
    s32 unused;

    damage = itMainGetDamageOutput(attack_ip);

    is_rehit = ((defend_ip->type == nITTypeDamage) && (attack_coll->can_rehit_item)) ? TRUE : FALSE;

    itProcessSetHitInteractStats(attack_coll, defend_gobj, (is_rehit != FALSE) ? nGMHitTypeDamageRehit : nGMHitTypeDamage, 0);

    if (is_rehit != FALSE)
    {
        if (attack_ip->hit_refresh_damage < damage)
        {
            attack_ip->hit_refresh_damage = damage;
        }
    }
    else if (attack_ip->hit_normal_damage < damage)
    {
        attack_ip->hit_normal_damage = damage;
    }
    vel = (attack_ip->physics.vel_air.x < 0.0F) ? -attack_ip->physics.vel_air.x : attack_ip->physics.vel_air.x;

    if (vel < 5.0F)
    {
        attack_ip->hit_lr = lr = (DObjGetStruct(defend_gobj)->translate.vec.f.x < DObjGetStruct(attack_gobj)->translate.vec.f.x) ? -1 : +1;
    }
    else
    {
        lr = (attack_ip->physics.vel_air.x < 0) ? -1 : +1;

        attack_ip->hit_lr = lr;
    }
    if (damage_coll->hitstatus == nGMHitStatusNormal)
    {
        defend_ip->damage_queue += damage;

        if (defend_ip->damage_highest < damage)
        {
            defend_ip->damage_highest = damage; // Last source of damage?
            defend_ip->damage_angle = attack_coll->angle;
            defend_ip->damage_element = attack_coll->element;

            vel = (attack_ip->physics.vel_air.x < 0.0F) ? -attack_ip->physics.vel_air.x : attack_ip->physics.vel_air.x;

            if (vel < 5.0F)
            {
                defend_ip->damage_lr = lr = (DObjGetStruct(defend_gobj)->translate.vec.f.x < DObjGetStruct(attack_gobj)->translate.vec.f.x) ? +1 : -1;
            }
            else
            {
                lr = (attack_ip->physics.vel_air.x < 0) ? +1 : -1;

                defend_ip->damage_lr = lr;
            }
            defend_ip->damage_gobj          = attack_ip->owner_gobj;
            defend_ip->damage_team          = attack_ip->team;
            defend_ip->damage_port          = attack_ip->player;
            defend_ip->damage_player_num = attack_ip->player_num;
            defend_ip->damage_handicap      = attack_ip->handicap;
            defend_ip->damage_display_mode  = attack_ip->display_mode;
        }
        if (defend_ip->is_allow_knockback)
        {
            knockback = ftParamGetCommonKnockback
            (
                defend_ip->percent_damage,
                defend_ip->damage_queue,
                damage,
                attack_coll->knockback_weight,
                attack_coll->knockback_scale,
                attack_coll->knockback_base,
                1.0F,
                attack_ip->handicap,
                defend_ip->handicap
            );
            if (defend_ip->damage_knockback < knockback)
            {
                defend_ip->damage_knockback = knockback;
            }
        }
        if (attack_ip->is_hitlag_victim)
        {
            gmCollisionGetItemAttackItemDamagePosition(&pos, attack_coll, attack_id, damage_coll, defend_gobj);

            switch (attack_coll->element)
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
                efManagerDamageNormalLightMakeEffect(&pos, attack_ip->player, damage, 0);
                break;
            }
        }
    }
    func_800269C0_275C0(attack_coll->fgm_id);
}

// 0x801702C8 - Item's hurtbox gets hit by a weapon
void itProcessUpdateDamageStatWeapon(WPStruct *wp, WPAttackColl *attack_coll, s32 attack_id, ITStruct *ip, ITDamageColl *damage_coll, GObj *weapon_gobj, GObj *item_gobj)
{
    s32 damage;
    s32 unused;
    s32 is_rehit;
    f32 knockback;
    Vec3f pos;
    f32 vel;
    s32 lr;

    damage = wpMainGetStaledDamage(wp);

    is_rehit = ((ip->type == nITTypeDamage) && (attack_coll->can_rehit_item)) ? TRUE : FALSE;

    wpProcessUpdateHitInteractStats(wp, attack_coll, item_gobj, ((is_rehit != FALSE) ? nGMHitTypeDamageRehit : nGMHitTypeDamage), 0);

    if (is_rehit != FALSE)
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
    if (damage_coll->hitstatus == nGMHitStatusNormal)
    {
        ip->damage_queue += damage;

        if (ip->damage_highest < damage)
        {
            ip->damage_highest = damage;
            ip->damage_angle = attack_coll->angle;
            ip->damage_element = attack_coll->element;

            vel = (wp->physics.vel_air.x < 0.0F) ? -wp->physics.vel_air.x : wp->physics.vel_air.x;

            if (vel < 5.0F)
            {
                ip->damage_lr = lr = (DObjGetStruct(item_gobj)->translate.vec.f.x < DObjGetStruct(weapon_gobj)->translate.vec.f.x) ? +1 : -1;
            }
            else
            {
                lr = (wp->physics.vel_air.x < 0) ? +1 : -1;

                ip->damage_lr = lr;
            }
            ip->damage_gobj         = wp->owner_gobj;
            ip->damage_team         = wp->team;
            ip->damage_port         = wp->player;
            ip->damage_player_num   = wp->player_num;
            ip->damage_handicap     = wp->handicap;
            ip->damage_display_mode = wp->display_mode;
        }
        if (ip->is_allow_knockback)
        {
            knockback = ftParamGetCommonKnockback
            (
                ip->percent_damage,
                ip->damage_queue,
                damage,
                attack_coll->knockback_weight,
                attack_coll->knockback_scale,
                attack_coll->knockback_base,
                1.0F,
                wp->handicap,
                ip->handicap
            );
            if (ip->damage_knockback < knockback)
            {
                ip->damage_knockback = knockback;
            }
        }
        if (wp->is_hitlag_victim)
        {
            gmCollisionGetWeaponAttackItemDamagePosition(&pos, attack_coll, attack_id, damage_coll, item_gobj);

            switch (attack_coll->element)
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
                efManagerDamageNormalLightMakeEffect(&pos, wp->player, damage, NULL);
                break;
            }
        }
    }
    func_800269C0_275C0(attack_coll->fgm_id);
}

// 0x801705C4
void itProcessSearchHitFighter(GObj *item_gobj) // Check fighters for hit detection
{
    GObj *fighter_gobj;
    GObj *owner_gobj;
    s32 i;
    s32 k;
    s32 unused;
    s32 j;
    GObj *other_gobj;
    u32 team;
    FTAttackColl *attack_coll;
    GMHitFlags fighter_victim_flags;
    ITDamageColl *damage_coll;
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp;

    if (ip->damage_coll.interact_mask & GMHITCOLLISION_FLAG_FIGHTER)
    {
        fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

        while (fighter_gobj != NULL)
        {           
            if ((fighter_gobj == ip->owner_gobj) && !(ip->is_damage_all)) goto next_gobj;

            fp = ftGetStruct(fighter_gobj);

            if ((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE) && (((fp->throw_gobj != NULL) ? fp->throw_team : fp->team) == ip->team) && !(ip->is_damage_all)) goto next_gobj;
            
            if (fp->is_catchstatus) goto next_gobj;
            
            k = 0;

            if ((ip->owner_gobj != NULL) && (fp->throw_gobj != NULL) && (fp->throw_gobj == ip->owner_gobj)) goto next_gobj;
            
            for (i = 0; i < ARRAY_COUNT(gFTMainIsDamageDetect); i++)
            {
                attack_coll = &fp->attack_colls[i];

                if (attack_coll->attack_state != nGMAttackStateOff)
                {
                    if ((ip->ga == nMPKineticsAir) && (attack_coll->is_hit_air) || (ip->ga == nMPKineticsGround) && (attack_coll->is_hit_ground))
                    {
                        fighter_victim_flags.is_interact_hurt = fighter_victim_flags.is_interact_shield = FALSE;

                        fighter_victim_flags.group_id = 7;

                        for (j = 0; j < ARRAY_COUNT(attack_coll->attack_records); j++)
                        {
                            if (item_gobj == attack_coll->attack_records[j].victim_gobj)
                            {
                                fighter_victim_flags = attack_coll->attack_records[j].victim_flags;

                                break;
                            }
                        }
                        if ((!(fighter_victim_flags.is_interact_hurt)) && (!(fighter_victim_flags.is_interact_shield)) && (fighter_victim_flags.group_id == 7))
                        {
                            gFTMainIsDamageDetect[i] = TRUE;
                            k++;

                            continue;
                        }
                    }
                }
                gFTMainIsDamageDetect[i] = FALSE;
            }
            if (k != 0)
            {
                for (i = 0; i < ARRAY_COUNT(fp->attack_colls); i++)
                {
                    damage_coll = &ip->damage_coll;

                    if (gFTMainIsDamageDetect[i] != FALSE)
                    {
                        if (ip->damage_coll.hitstatus == nGMHitStatusNone) break;

                        if (damage_coll->hitstatus == nGMHitStatusIntangible) continue;

                        if (gmCollisionCheckFighterAttackItemDamageCollide(&fp->attack_colls[i], damage_coll, item_gobj) != FALSE)
                        {
                            itProcessUpdateDamageStatFighter(fp, &fp->attack_colls[i], ip, damage_coll, fighter_gobj, item_gobj);
                        }
                    }
                }
            }
        next_gobj:
            fighter_gobj = fighter_gobj->link_next;
        }
    }
}

// 0x8017088C
void itProcessSearchHitItem(GObj *this_gobj) // Check other items for hit detection
{
    ITAttackColl *this_hit;
    ITStruct *other_ip;
    ITStruct *this_ip;
    GObj *other_gobj;
    ITAttackColl *other_hit;
    GMHitFlags these_flags, those_flags;
    s32 i, j, m, n;
    sb32 is_check_self;
    ITDamageColl *damage_coll;

    this_ip = itGetStruct(this_gobj);
    this_hit = &this_ip->attack_coll;

    if (this_ip->damage_coll.interact_mask & GMHITCOLLISION_FLAG_ITEM)
    {
        other_gobj = gGCCommonLinks[nGCCommonLinkIDItem];

        is_check_self = FALSE;

        while (other_gobj != NULL)
        {
            if (other_gobj == this_gobj)
            {
                is_check_self = TRUE;
            }
            else
            {
                other_ip = itGetStruct(other_gobj);
                other_hit = &other_ip->attack_coll;

                if ((this_ip->owner_gobj == other_ip->owner_gobj) && !(this_ip->is_damage_all)) goto next_gobj;
                
                if ((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE) && (this_ip->team == other_ip->team) && !(this_ip->is_damage_all)) goto next_gobj;

                if (other_hit->attack_state == nGMAttackStateOff) goto next_gobj;
                
                if (!(other_hit->interact_mask & GMHITCOLLISION_FLAG_ITEM)) goto next_gobj;
                
                those_flags.is_interact_hurt = those_flags.is_interact_shield = FALSE;

                those_flags.group_id = 7;

                for (m = 0; m < ARRAY_COUNT(other_hit->attack_records); m++) // IDO will flip you off if you don't use a new iterator here...
                {
                    if (this_gobj == other_hit->attack_records[m].victim_gobj)
                    {
                        those_flags = other_hit->attack_records[m].victim_flags;

                        break;
                    }
                }
                if ((those_flags.is_interact_hurt) || (those_flags.is_interact_shield) || (those_flags.group_id != 7)) goto next_gobj;

                if ((is_check_self != FALSE) && (this_hit->can_setoff) && (other_hit->can_setoff) && (this_ip->owner_gobj != other_ip->owner_gobj))
                {
                    if ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (this_ip->team != other_ip->team))
                    {
                        if (this_hit->attack_state != nGMAttackStateOff)
                        {
                            if (this_hit->interact_mask & GMHITCOLLISION_FLAG_ITEM)
                            {
                                these_flags.is_interact_hurt = these_flags.is_interact_shield = FALSE;

                                these_flags.group_id = 7;

                                for (n = 0; n < ARRAY_COUNT(this_hit->attack_records); n++)
                                {
                                    if (other_gobj == this_hit->attack_records[n].victim_gobj)
                                    {
                                        these_flags = this_hit->attack_records[n].victim_flags;

                                        break;
                                    }
                                }
                                if ((these_flags.is_interact_hurt) || (these_flags.is_interact_shield) || (these_flags.group_id != 7)) goto hurtbox_check;

                                for (i = 0; i < other_hit->attack_count; i++)
                                {
                                    for (j = 0; j < this_hit->attack_count; j++)
                                    {
                                        if (gmCollisionCheckItemAttacksCollide(other_hit, i, this_hit, j) != FALSE)
                                        {
                                            itProcessUpdateAttackStatItem(other_ip, other_hit, i, this_ip, this_hit, j, other_gobj, this_gobj);

                                            if (other_ip->hit_attack_damage != 0) goto next_gobj;

                                            else if (this_ip->hit_attack_damage != 0) goto hurtbox_check;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            hurtbox_check:
                for (i = 0; i < other_hit->attack_count; i++) // ...it also flips you off if you DON'T reuse 'i' here
                {
                    damage_coll = &this_ip->damage_coll;

                    if (this_ip->damage_coll.hitstatus == nGMHitStatusNone) break;

                    if (damage_coll->hitstatus == nGMHitStatusIntangible) continue;

                    if (gmCollisionCheckItemAttackDamageCollide(other_hit, i, damage_coll, this_gobj) != FALSE)
                    {
                        itProcessUpdateDamageStatItem(other_ip, other_hit, i, this_ip, damage_coll, other_gobj, this_gobj);

                        goto next_gobj;
                    }
                }
            }
        next_gobj:
            other_gobj = other_gobj->link_next;
        }
    }
}

// 0x80170C84
void itProcessSearchHitWeapon(GObj *item_gobj) // Check weapons for hit detection
{
    ITAttackColl *it_attack_coll;
    WPStruct *wp;
    ITStruct *ip;
    GObj *weapon_gobj;
    WPAttackColl *wp_attack_coll;
    GMHitFlags these_flags;
    GMHitFlags those_flags;
    s32 i, j, m, n;
    sb32 is_check_self;
    ITDamageColl *damage_coll;

    ip = itGetStruct(item_gobj);
    it_attack_coll = &ip->attack_coll;

    if (ip->damage_coll.interact_mask & GMHITCOLLISION_FLAG_WEAPON)
    {
        weapon_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

        while (weapon_gobj != NULL)
        {
            wp = wpGetStruct(weapon_gobj);
            wp_attack_coll = &wp->attack_coll;

            if ((ip->owner_gobj == wp->owner_gobj) && !(ip->is_damage_all)) goto next_gobj;

            if ((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE) && (ip->team == wp->team) && !(ip->is_damage_all)) goto next_gobj;

            if (wp_attack_coll->attack_state != nGMAttackStateOff)
            {
                if (wp_attack_coll->interact_mask & GMHITCOLLISION_FLAG_ITEM)
                {
                    those_flags.is_interact_hurt = those_flags.is_interact_shield = FALSE;

                    those_flags.group_id = 7;

                    for (m = 0; m < ARRAY_COUNT(wp_attack_coll->attack_records); m++) // IDO will flip you off if you don't use a new iterator here...
                    {
                        if (item_gobj == wp_attack_coll->attack_records[m].victim_gobj)
                        {
                            those_flags = wp_attack_coll->attack_records[m].victim_flags;

                            break;
                        }
                    }
                    if ((those_flags.is_interact_hurt) || (those_flags.is_interact_shield) || (those_flags.group_id != 7)) goto next_gobj;
                    
                    if ((it_attack_coll->can_setoff) && (wp_attack_coll->can_setoff) && (ip->owner_gobj != wp->owner_gobj))
                    {
                        if ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (ip->team != wp->team))
                        {
                            if (it_attack_coll->attack_state != nGMAttackStateOff)
                            {
                                if (it_attack_coll->interact_mask & GMHITCOLLISION_FLAG_WEAPON)
                                {
                                    these_flags.is_interact_hurt = these_flags.is_interact_shield = FALSE;

                                    these_flags.group_id = 7;

                                    for (n = 0; n < ARRAY_COUNT(it_attack_coll->attack_records); n++)
                                    {
                                        if (weapon_gobj == it_attack_coll->attack_records[n].victim_gobj)
                                        {
                                            these_flags = it_attack_coll->attack_records[n].victim_flags;

                                            break;
                                        }
                                    }

                                    if ((these_flags.is_interact_hurt) || (these_flags.is_interact_shield) || (these_flags.group_id != 7)) goto hurtbox_check;

                                    else for (i = 0; i < wp_attack_coll->attack_count; i++)
                                    {
                                        for (j = 0; j < it_attack_coll->attack_count; j++)
                                        {
                                            if (gmCollisionCheckWeaponAttackItemAttackCollide(wp_attack_coll, i, it_attack_coll, j) != FALSE)
                                            {
                                                itProcessUpdateAttackStatWeapon(wp, wp_attack_coll, i, ip, it_attack_coll, j, weapon_gobj, item_gobj);

                                                if (wp->hit_attack_damage != 0) goto next_gobj;

                                                else if (ip->hit_attack_damage != 0) goto hurtbox_check;
                                            }
                                        }
                                    }
                                }

                            }
                        }
                    }
                hurtbox_check:
                    for (i = 0; i < wp_attack_coll->attack_count; i++) // ...it also flips you off if you DON'T reuse 'i' here
                    {
                        damage_coll = &ip->damage_coll;

                        if (ip->damage_coll.hitstatus == nGMHitStatusNone) break;

                        else if (damage_coll->hitstatus == nGMHitStatusIntangible) continue;

                        else if (gmCollisionCheckWeaponAttackItemDamageCollide(wp_attack_coll, i, damage_coll, item_gobj) != FALSE)
                        {
                            itProcessUpdateDamageStatWeapon(wp, wp_attack_coll, i, ip, damage_coll, weapon_gobj, item_gobj);

                            break;
                        }
                    }
                }
            }
        next_gobj:
            weapon_gobj = weapon_gobj->link_next;
        }
    }
}

// Copy pasted everything from Item VS Item hit collision logic and it immediately matched 82% of Item VS Weapon, even the stack; apparently in a much similar fashion to HAL

// 0x80171080
void itProcessProcSearchHitAll(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (!(ip->is_hold))
    {
        itProcessSearchHitFighter(item_gobj);
        itProcessSearchHitItem(item_gobj);
        itProcessSearchHitWeapon(item_gobj);
    }
}

// 0x801710C4
void itProcessProcHitCollisions(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->damage_queue != 0)
    {
        ip->percent_damage += ip->damage_queue;

        if (ip->percent_damage > GMCOMMON_PERCENT_DAMAGE_MAX)
        {
            ip->percent_damage = GMCOMMON_PERCENT_DAMAGE_MAX;
        }
        ip->damage_lag = ip->damage_queue;

        if (ip->proc_damage != NULL)
        {
            if (ip->proc_damage(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
    }
    if ((ip->hit_normal_damage != 0) || (ip->hit_refresh_damage != 0))
    {
        if (ip->proc_hit != NULL)
        {
            if (ip->proc_hit(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
    }
    if (ip->hit_shield_damage != 0)
    {
        if ((ip->attack_coll.can_hop) && (ip->ga == nMPKineticsAir))
        {
            if (ip->shield_collide_angle < ITEM_HOP_ANGLE_DEFAULT)
            {
                ip->shield_collide_angle -= F_CST_DTOR32(90.0F);

                if (ip->shield_collide_angle < 0.0F)
                {
                    ip->shield_collide_angle = 0.0F;
                }
                if (ip->proc_hop != NULL)
                {
                    if (ip->proc_hop(item_gobj) != FALSE)
                    {
                        itMainDestroyItem(item_gobj);
                        return;
                    }
                }
                goto next_check;
            }
        }
        if (ip->proc_shield != NULL)
        {
            if (ip->proc_shield(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
    }
next_check:
    if (ip->hit_attack_damage != 0)
    {
        if (ip->proc_setoff != NULL)
        {
            if (ip->proc_setoff(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
    }
    if (ip->reflect_gobj != NULL)
    {
        FTStruct *fp;

        ip->owner_gobj = ip->reflect_gobj;

        fp = ftGetStruct(ip->reflect_gobj);

        ip->team = fp->team;
        ip->player = fp->player;
        ip->player_num = fp->player_num;
        ip->handicap = fp->handicap;
#if defined(REGION_US)
        ip->attack_coll.stat_flags = ip->reflect_stat_flags;
        ip->attack_coll.stat_count = ip->reflect_stat_count;
#endif

        if (ip->proc_reflector != NULL)
        {
            if (ip->proc_reflector(item_gobj) != FALSE)
            {
                itMainDestroyItem(item_gobj);
                return;
            }
        }
        if (!(ip->is_static_damage))
        {
            ip->attack_coll.damage = (ip->attack_coll.damage * ITEM_REFLECT_MUL_DEFAULT) + ITEM_REFLECT_ADD_DEFAULT;

            if (ip->attack_coll.damage > ITEM_REFLECT_MAX_DEFAULT)
            {
                ip->attack_coll.damage = ITEM_REFLECT_MAX_DEFAULT;
            }
        }
    }
    if (ip->damage_lag != 0)
    {
        ip->hitlag_tics = ftParamGetHitLag(ip->damage_lag, nFTCommonStatusWait, 1.0F); // Maybe 10 is the "none" status ID?
    }

    ip->hit_normal_damage = 0;
    ip->hit_refresh_damage = 0;
    ip->hit_attack_damage = 0;
    ip->hit_shield_damage = 0;

    ip->reflect_gobj = NULL;
    
    ip->damage_highest = 0;
    ip->damage_queue = 0;
    ip->damage_lag = 0;
    ip->damage_knockback = 0.0F;
}
