#include <wp/weapon.h>
#include <it/item.h>
#include <ft/fighter.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80165ED0
void wpProcessUpdateHitOffsets(DObj *dobj, Vec3f *offset)
{
    offset->x *= dobj->scale.vec.f.x;
    offset->y *= dobj->scale.vec.f.y;

    syVectorRotate3D(offset, SYVECTOR_AXIS_Z, dobj->rotate.vec.f.z);

    offset->x += dobj->translate.vec.f.x;
    offset->y += dobj->translate.vec.f.y;
    offset->z += dobj->translate.vec.f.z;
}

// 0x80165F60
void wpProcessUpdateHitPositions(GObj *weapon_gobj) // Update hitbox(es?)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    DObj *dobj = DObjGetStruct(weapon_gobj);
    s32 i;

    for (i = 0; i < wp->attack_coll.attack_count; i++)
    {
        WPAttackPos *positions = &wp->attack_coll.attack_pos[i];
        Vec3f *offset = &wp->attack_coll.offsets[i];
        Vec3f *translate = &dobj->translate.vec.f;

        switch (wp->attack_coll.attack_state)
        {
        case nGMAttackStateOff:
            break;

        case nGMAttackStateNew:
            positions->pos_curr = *offset;

            if ((offset->x == 0.0F) && (offset->y == 0.0F) && (offset->z == 0.0F))
            {
                positions->pos_curr.x += translate->x;
                positions->pos_curr.y += translate->y;
                positions->pos_curr.z += translate->z;
            }
            else wpProcessUpdateHitOffsets(dobj, &positions->pos_curr);
            
            wp->attack_coll.attack_state = nGMAttackStateTransfer;

            positions->unk_wphitpos_0x18 = FALSE;
            positions->unk_wphitpos_0x5C = 0;
            break;

        case nGMAttackStateTransfer:
            wp->attack_coll.attack_state = nGMAttackStateInterpolate;

            /* fallthrough */

        case nGMAttackStateInterpolate:
            positions->pos_prev = positions->pos_curr;
            positions->pos_curr = *offset;

            if ((offset->x == 0.0F) && (offset->y == 0.0F) && (offset->z == 0.0F))
            {
                positions->pos_curr.x += translate->x;
                positions->pos_curr.y += translate->y;
                positions->pos_curr.z += translate->z;
            }
            else wpProcessUpdateHitOffsets(dobj, &positions->pos_curr);
            
            positions->unk_wphitpos_0x18 = FALSE;
            positions->unk_wphitpos_0x5C = 0;
            break;
        }
    }
}

// 0x801661E0
void wpProcessUpdateAttackRecords(GObj *weapon_gobj) // Set hitbox victim array
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    GMAttackRecord *record;
    WPAttackColl *attack_coll;
    s32 i;

    attack_coll = &wp->attack_coll;

    if (attack_coll->attack_state != nGMAttackStateOff)
    {
        for (i = 0; i < ARRAY_COUNT(wp->attack_coll.attack_records); i++)
        {
            record = &attack_coll->attack_records[i];

            if (record->victim_gobj != NULL)
            {
                if (record->victim_flags.timer_rehit > 0)
                {
                    record->victim_flags.timer_rehit--;

                    if (record->victim_flags.timer_rehit <= 0)
                    {
                        record->victim_gobj = NULL;

                        record->victim_flags.is_interact_hurt = record->victim_flags.is_interact_shield = record->victim_flags.is_interact_reflect = record->victim_flags.is_interact_absorb = FALSE;

                        record->victim_flags.group_id = 7;
                    }
                }
            }
        }
    }
}

// 0x801662BC
void wpProcessProcWeaponMain(GObj *weapon_gobj) // Run item logic pass 1 (animation, physics, collision, despawn check)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (!(wp->is_hitlag_weapon))
    {
        Vec3f *translate;

        gcPlayAnimAll(weapon_gobj);

        if (wp->proc_update != NULL)
        {
            if (wp->proc_update(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
        translate = &DObjGetStruct(weapon_gobj)->translate.vec.f;

        wp->coll_data.pos_prev = *translate;

        translate->x += wp->physics.vel_air.x;
        translate->y += wp->physics.vel_air.y;
        translate->z += wp->physics.vel_air.z;

        wp->coll_data.pos_diff.x = translate->x - wp->coll_data.pos_prev.x;
        wp->coll_data.pos_diff.y = translate->y - wp->coll_data.pos_prev.y;
        wp->coll_data.pos_diff.z = translate->z - wp->coll_data.pos_prev.z;

        if
        (
            (wp->ga == nMPKineticsGround) &&
            (wp->coll_data.floor_line_id != -1) &&
            (wp->coll_data.floor_line_id != -2) &&
            (mpCollisionCheckExistLineID(wp->coll_data.floor_line_id) != FALSE)
        )
        {
            mpCollisionGetSpeedLineID(wp->coll_data.floor_line_id, &wp->coll_data.vel_speed);

            translate->x += wp->coll_data.vel_speed.x;
            translate->y += wp->coll_data.vel_speed.y;
            translate->z += wp->coll_data.vel_speed.z;
        }
        else wp->coll_data.vel_speed.x = wp->coll_data.vel_speed.y = wp->coll_data.vel_speed.z = 0.0F;

        if 
        (
            (translate->y < gMPCollisionGroundData->map_bound_bottom) ||
            (translate->x > gMPCollisionGroundData->map_bound_right)  ||
            (translate->x < gMPCollisionGroundData->map_bound_left)   ||
            (translate->y > gMPCollisionGroundData->map_bound_top)    ||
            (translate->z < -20000.0F)                                ||
            (translate->z > 20000.0F)
        )
        {
            if ((wp->proc_dead == NULL) || (wp->proc_dead(weapon_gobj) != FALSE))
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
        if (wp->proc_map != NULL)
        {
            wp->coll_data.mask_prev = wp->coll_data.mask_curr;
            wp->coll_data.mask_curr = 0;
            wp->coll_data.is_coll_end = FALSE;
            wp->coll_data.mask_stat = 0;
            wp->coll_data.mask_unk = 0;

            if (wp->proc_map(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
        wpProcessUpdateHitPositions(weapon_gobj);
        wpProcessUpdateAttackRecords(weapon_gobj);
    }
}

// 0x80166594
void wpProcessSetHitInteractStats(WPAttackColl *attack_coll, GObj *victim_gobj, s32 attack_type, u32 attack_group_id)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(attack_coll->attack_records); i++)
    {
        if (victim_gobj == attack_coll->attack_records[i].victim_gobj) // Run this if the victim we're checking has already been hit
        {
            switch (attack_type)
            {
            case nGMHitTypeDamage:
                attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
                break;

            case nGMHitTypeShield:
                attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
                break;

            case nGMHitTypeShieldRehit:
                attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
                attack_coll->attack_records[i].victim_flags.timer_rehit = WEAPON_REHIT_TIME_DEFAULT;
                break;

            case nGMHitTypeReflect:
                attack_coll->attack_records[i].victim_flags.is_interact_reflect = TRUE;
                attack_coll->attack_records[i].victim_flags.timer_rehit = WEAPON_REHIT_TIME_DEFAULT;
                break;

            case nGMHitTypeAbsorb:
                attack_coll->attack_records[i].victim_flags.is_interact_absorb = TRUE;
                break;

            case nGMHitTypeAttack:
                attack_coll->attack_records[i].victim_flags.group_id = attack_group_id;
                break;

            case nGMHitTypeDamageRehit:
                attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
                attack_coll->attack_records[i].victim_flags.timer_rehit = WEAPON_REHIT_TIME_DEFAULT;
                break;

            default: 
                break;
            }
            break;
        }
    }
    if (i == ARRAY_COUNT(attack_coll->attack_records)) // Check if all victim slots have been filled
    {
        for (i = 0; i < ARRAY_COUNT(attack_coll->attack_records); i++) // Reset hit count and increment until there is an empty slot
        {
            if (attack_coll->attack_records[i].victim_gobj == NULL)
            {
                break;
            }
        }
        if (i == ARRAY_COUNT(attack_coll->attack_records))
        {
            i = 0; // Reset hit count again if all victim slots are full
        }
        attack_coll->attack_records[i].victim_gobj = victim_gobj; // Store victim's pointer to slot

        switch (attack_type)
        {
        case nGMHitTypeDamage:
            attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
            break;

        case nGMHitTypeShield:
            attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
            break;

        case nGMHitTypeShieldRehit:
            attack_coll->attack_records[i].victim_flags.is_interact_shield = TRUE;
            attack_coll->attack_records[i].victim_flags.timer_rehit = WEAPON_REHIT_TIME_DEFAULT;
            break;

        case nGMHitTypeReflect:
            attack_coll->attack_records[i].victim_flags.is_interact_reflect = TRUE;
            attack_coll->attack_records[i].victim_flags.timer_rehit = WEAPON_REHIT_TIME_DEFAULT;
            break;

        case nGMHitTypeAbsorb:
            attack_coll->attack_records[i].victim_flags.is_interact_absorb = TRUE;
            break;

        case nGMHitTypeAttack:
            attack_coll->attack_records[i].victim_flags.group_id = attack_group_id;
            break;

        case nGMHitTypeDamageRehit:
            attack_coll->attack_records[i].victim_flags.is_interact_hurt = TRUE;
            attack_coll->attack_records[i].victim_flags.timer_rehit = WEAPON_REHIT_TIME_DEFAULT;
            break;

        default: 
            break;
        }
    }
}

// 0x8016679C
void wpProcessUpdateHitInteractStats(WPStruct *this_wp, WPAttackColl *attack_coll, GObj *victim_gobj, s32 attack_type, u32 attack_group_id)
{
    if (this_wp->group_id != 0)
    {
        GObj *other_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

        while (other_gobj != NULL)
        {
            WPStruct *other_wp = wpGetStruct(other_gobj);

            if (other_wp->group_id == this_wp->group_id)
            {
                wpProcessSetHitInteractStats(&other_wp->attack_coll, victim_gobj, attack_type, attack_group_id);
            }
            other_gobj = other_gobj->link_next;
        }
    }
    else wpProcessSetHitInteractStats(attack_coll, victim_gobj, attack_type, attack_group_id);
}

// 0x80166854
void wpProcessUpdateAttackStatWeapon
(
    WPStruct *this_wp,
    WPAttackColl *this_attack_coll,
    s32 this_attack_id,
    WPStruct *victim_wp,
    WPAttackColl *victim_attack_coll,
    s32 victim_attack_id,
    GObj *this_gobj,
    GObj *victim_gobj
)
{
    s32 this_attack_coll_damage = wpMainGetStaledDamage(this_wp);
    s32 victim_attack_coll_damage = wpMainGetStaledDamage(victim_wp);
    Vec3f pos;
    s32 priority_high;

    gmCollisionGetWeaponAttacksPosition(&pos, victim_attack_coll, victim_attack_id, this_attack_coll, this_attack_id);

    priority_high = this_attack_coll->priority;

    if (victim_attack_coll->priority <= priority_high)
    {
        wpProcessUpdateHitInteractStats(victim_wp, victim_attack_coll, this_gobj, nGMHitTypeAttack, 0);

        if (victim_wp->hit_attack_damage < victim_attack_coll_damage)
        {
            victim_wp->hit_attack_damage = victim_attack_coll_damage;
        }
        efManagerSetOffMakeEffect(&pos, victim_attack_coll_damage);
    }
    priority_high = victim_attack_coll->priority;

    if (this_attack_coll->priority <= priority_high)
    {
        wpProcessUpdateHitInteractStats(this_wp, this_attack_coll, victim_gobj, nGMHitTypeAttack, 0);

        if (this_wp->hit_attack_damage < this_attack_coll_damage)
        {
            this_wp->hit_attack_damage = this_attack_coll_damage;
        }
        efManagerSetOffMakeEffect(&pos, this_attack_coll_damage);
    }
}

// 0x80166954
void wpProcessProcSearchHitWeapon(GObj *this_gobj) // Scan for hitbox collision with other items
{
    GObj *other_gobj;
    WPStruct *this_wp, *other_wp;
    WPAttackColl *other_hit, *this_attack_coll;
    GMHitFlags these_flags, those_flags;
    s32 m, n, i, j;
    sb32 is_check_self;

    this_wp = wpGetStruct(this_gobj);
    this_attack_coll = &this_wp->attack_coll;

    if ((this_attack_coll->can_setoff) && (this_attack_coll->attack_state != nGMAttackStateOff) && (this_attack_coll->interact_mask & GMHITCOLLISION_FLAG_WEAPON))
    {
        other_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

        is_check_self = FALSE;

        while (other_gobj != NULL)
        {
            other_wp = wpGetStruct(other_gobj);
            other_hit = &other_wp->attack_coll;

            if (other_gobj == this_gobj)
            {
                is_check_self = TRUE; // Not really sure what to name this
            }
            else if (is_check_self != FALSE)
            {
                if (this_wp->owner_gobj == other_wp->owner_gobj)
                {
                    goto next_gobj;
                }
                if ((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE) && (this_wp->team == other_wp->team)) 
                {
                    goto next_gobj;
                }
                if ((other_hit->attack_state != nGMAttackStateOff) && (other_hit->can_setoff))
                {
                    if (other_hit->interact_mask & GMHITCOLLISION_FLAG_WEAPON)
                    {
                        those_flags.group_id = 7;

                        for (m = 0; m < ARRAY_COUNT(other_hit->attack_records); m++)
                        {
                            if (this_gobj == other_hit->attack_records[m].victim_gobj)
                            {
                                those_flags = other_hit->attack_records[m].victim_flags;

                                break;
                            }
                        }
                        if (those_flags.group_id == 7)
                        {
                            these_flags.group_id = 7;

                            for (n = 0; n < ARRAY_COUNT(this_attack_coll->attack_records); n++)
                            {
                                if (other_gobj == this_attack_coll->attack_records[n].victim_gobj)
                                {
                                    these_flags = this_attack_coll->attack_records[n].victim_flags;

                                    break;
                                }
                            }
                            if (these_flags.group_id == 7)
                            {
                                for (i = 0; i < other_hit->attack_count; i++)
                                {
                                    for (j = 0; j < this_attack_coll->attack_count; j++)
                                    {
                                        if (gmCollisionCheckWeaponAttacksCollide(other_hit, i, this_attack_coll, j) != FALSE)
                                        {
                                            wpProcessUpdateAttackStatWeapon(other_wp, other_hit, i, this_wp, this_attack_coll, j, other_gobj, this_gobj);

                                            goto next_gobj;
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
}

// 0x80166BE4
void wpProcessProcHitCollisions(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if ((wp->hit_normal_damage != 0) || (wp->hit_refresh_damage != 0))
    {
        if (wp->proc_hit != NULL)
        {
            if (wp->proc_hit(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
    }
    if (wp->hit_shield_damage != 0)
    {
        if ((wp->attack_coll.can_hop) && (wp->ga == nMPKineticsAir))
        {
            if (wp->shield_collide_angle < WEAPON_HOP_ANGLE_DEFAULT)
            {
                wp->shield_collide_angle -= F_CST_DTOR32(90.0F);

                if (wp->shield_collide_angle < 0.0F)
                {
                    wp->shield_collide_angle = 0.0F;
                }
                if (wp->proc_hop != NULL)
                {
                    if (wp->proc_hop(weapon_gobj) != FALSE)
                    {
                        wpMainDestroyWeapon(weapon_gobj);
                        return;
                    }
                }
                goto next_check;
            }
        }
        if (wp->proc_shield != NULL)
        {
            if (wp->proc_shield(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
    }
next_check:
    if (wp->hit_attack_damage != 0)
    {
        if (wp->proc_setoff != NULL)
        {
            if (wp->proc_setoff(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
    }
    if (wp->reflect_gobj != NULL)
    {
        FTStruct *fp;

        wp->owner_gobj = wp->reflect_gobj;

        fp = ftGetStruct(wp->reflect_gobj);

        wp->team = fp->team;
        wp->player = fp->player;
        wp->display_mode = fp->display_mode;
        wp->player_num = fp->player_num;
        wp->handicap = fp->handicap;
#if defined(REGION_US)
        wp->attack_coll.stat_flags = wp->reflect_stat_flags;
        wp->attack_coll.stat_count = wp->reflect_stat_count;
#endif

        if (wp->proc_reflector != NULL)
        {
            if (wp->proc_reflector(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
        if (!(wp->is_static_damage))
        {
            wp->attack_coll.damage = (wp->attack_coll.damage * WEAPON_REFLECT_MUL_DEFAULT) + WEAPON_REFLECT_ADD_DEFAULT;

            if (wp->attack_coll.damage > WEAPON_REFLECT_TIME_DEFAULT)
            {
                wp->attack_coll.damage = WEAPON_REFLECT_TIME_DEFAULT;
            }
        }
    }
    if (wp->absorb_gobj != NULL)
    {
        if (wp->proc_absorb != NULL)
        {
            if (wp->proc_absorb(weapon_gobj) != FALSE)
            {
                wpMainDestroyWeapon(weapon_gobj);
                return;
            }
        }
    }
    wp->hit_normal_damage = 0;
    wp->hit_refresh_damage = 0;
    wp->hit_attack_damage = 0;
    wp->hit_shield_damage = 0;
    wp->reflect_gobj = NULL;
}
