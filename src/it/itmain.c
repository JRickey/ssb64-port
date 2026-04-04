#include <it/item.h>
#include <ft/fighter.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern alSoundEffect* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80189520
void (*dITMainProcDroppedList[/* */])(GObj*) =
{
    // Containers
    itBoxDroppedSetStatus,              // Box
    itTaruDroppedSetStatus,             // Barrel
    itCapsuleDroppedSetStatus,          // Capsule
    itEggDroppedSetStatus,              // Egg
    
    // Usable items
    itTomatoDroppedSetStatus,           // Maxim Tomato
    itHeartDroppedSetStatus,            // Heart Container
    NULL,                               // Star Man
    itSwordDroppedSetStatus,            // Beam Sword
    itBatDroppedSetStatus,              // Home Run Bat
    itHarisenDroppedSetStatus,          // Fan
    itStarRodDroppedSetStatus,          // Star Rod
    itLGunDroppedSetStatus,             // Ray Gun
    itFFlowerDroppedSetStatus,          // Fire Flower
    itHammerDroppedSetStatus,           // Hammer
    itMSBombDroppedSetStatus,           // Motion-sensor Bomb
    itBombHeiDroppedSetStatus,          // Bob-omb
    itNBumperDroppedSetStatus,          // Normal Bumper
    itGShellDroppedSetStatus,           // Green Shell
    itRShellDroppedSetStatus,           // Red Shell
    itMBallDroppedSetStatus,            // Poké Ball

    // Fighter items
    NULL,                               // Ness PK Fire
    itLinkBombDroppedSetStatus          // Link Bomb
};

// 0x80189578
void (*dITMainProcThrownList[/* */])(GObj*) =
{
    // Containers
    itBoxThrownSetStatus,               // Box
    itTaruThrownSetStatus,              // Barrel
    itCapsuleThrownSetStatus,           // Capsule
    itEggThrownSetStatus,               // Egg
    
    // Usable items
    NULL,                               // Maxim Tomato
    NULL,                               // Heart Container
    NULL,                               // Star Man
    itSwordThrownSetStatus,             // Beam Sword
    itBatThrownSetStatus,               // Home Run Bat
    itHarisenThrownSetStatus,           // Fan
    itStarRodThrownSetStatus,           // Star Rod
    itLGunThrownSetStatus,              // Ray Gun
    itFFlowerThrownSetStatus,           // Fire Flower
    itHammerThrownSetStatus,            // Hammer
    itMSBombThrownSetStatus,            // Motion-sensor Bomb
    itBombHeiThrownSetStatus,           // Bob-omb
    itNBumperThrownSetStatus,           // Normal Bumper
    itGShellThrownSetStatus,            // Green Shell
    itRShellThrownSetStatus,            // Red Shell
    itMBallThrownSetStatus,             // Poké Ball

    // Fighter items
    NULL,                               // Ness PK Fire
    itLinkBombThrownSetStatus           // Link Bomb
};

// 0x801895D0
void (*dITMainProcHoldList[/* */])(GObj*) =
{
    // Containers
    itBoxHoldSetStatus,                // Box
    itTaruHoldSetStatus,               // Barrel
    itCapsuleHoldSetStatus,            // Capsule
    itEggHoldSetStatus,                // Egg

    // Usable items
    NULL,                              // Maxim Tomato
    NULL,                              // Heart Container
    NULL,                              // Star Man
    itSwordHoldSetStatus,              // Beam Sword
    itBatHoldSetStatus,                // Home Run Bat
    itHarisenHoldSetStatus,            // Fan
    itStarRodHoldSetStatus,            // Star Rod
    itLGunHoldSetStatus,               // Ray Gun
    itFFlowerHoldSetStatus,            // Fire Flower
    itHammerHoldSetStatus,             // Hammer
    itMSBombHoldSetStatus,             // Motion-sensor Bomb
    itBombHeiHoldSetStatus,            // Bob-omb
    itNBumperHoldSetStatus,            // Normal Bumper
    itGShellHoldSetStatus,             // Green Shell
    itRShellHoldSetStatus,             // Red Shell
    itMBallHoldSetStatus,              // Poké Ball

    // Fighter items
    NULL,                              // Ness PK Fire
    itLinkBombHoldSetStatus            // Link Bomb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80172310
void itMainSetCommonSpin(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->spin_step = (ip->attr->spin_speed != 0) ? (F_PCT_TO_DEC(ip->attr->spin_speed) * ITEM_SPIN_SPEED_COMMON) : 0.0F;

    if (ip->lr == -1)
    {
        ip->spin_step = -ip->spin_step;
    }
}

// 0x80172394
void itMainSetAppearSpin(GObj *item_gobj, sb32 slow_or_fast)
{
    // slow_or_fast = 0 if slow, 1 if fast

    ITStruct *ip = itGetStruct(item_gobj);

    if (slow_or_fast == 0)
    {
        if (ip->attr->spin_speed != 0)
        {
            ip->spin_step = F_PCT_TO_DEC(ip->attr->spin_speed) * ITEM_SPIN_SPEED_APPEAR_SLOW;
        }
        else ip->spin_step = 0.0F;
    }
    else if (ip->attr->spin_speed != 0)
    {
        ip->spin_step = F_PCT_TO_DEC(ip->attr->spin_speed) * ITEM_SPIN_SPEED_APPEAR_FAST;
    }
    else ip->spin_step = 0.0F;
}

// 0x8017245C
void itMainSetThrownSpin(GObj *item_gobj, Vec3f *vel, sb32 is_smash_throw)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->spin_step = (is_smash_throw != FALSE) ? ITEM_SPIN_SPEED_SMASH_THROW : ITEM_SPIN_SPEED_NORMAL_THROW;

    if (vel->x < 0) // Facing direction, sort of
    {
        ip->spin_step = -ip->spin_step;
    }
    ip->spin_step = (ip->attr->spin_speed != 0) ? (F_PCT_TO_DEC(ip->attr->spin_speed) * ip->spin_step) : 0.0F;
}

// 0x80172508
void itMainSetSpinVelLR(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->lr = (ip->physics.vel_air.x >= 0.0F) ? +1 : -1;

    itMainSetCommonSpin(item_gobj);
}

// 0x80172558
void itMainApplyGravityClampTVel(ITStruct *ip, f32 gravity, f32 terminal_velocity)
{
    ip->physics.vel_air.y -= gravity;

    if (lbCommonMag2D(&ip->physics.vel_air) > terminal_velocity)
    {
        lbCommonNormDist2D(&ip->physics.vel_air);
        lbCommonScale2D(&ip->physics.vel_air, terminal_velocity);
    }
}

// 0x801725BC
void itMainResetPlayerVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->owner_gobj = NULL;
    ip->team = ITEM_TEAM_DEFAULT;
    ip->player = ITEM_PORT_DEFAULT;
    ip->handicap = ITEM_HANDICAP_DEFAULT;
    ip->player_num = 0;
    ip->attack_coll.throw_mul = ITEM_THROW_DEFAULT;

    ip->display_mode = gITManagerDisplayMode;
}

// 0x801725F8
void itMainClearAttackRecord(ITStruct *ip)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(ip->attack_coll.attack_records); i++)
    {
        GMAttackRecord *record = &ip->attack_coll.attack_records[i];

        record->victim_gobj = NULL;

        record->victim_flags.is_interact_hurt = record->victim_flags.is_interact_shield = record->victim_flags.is_interact_reflect = FALSE;

        record->victim_flags.timer_rehit = 0;

        record->victim_flags.group_id = 7;
    }
}

// 0x8017275C
void itMainRefreshAttackColl(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainClearAttackRecord(ip);

    ip->attack_coll.attack_state = nGMAttackStateNew;

    itProcessUpdateAttackPositions(item_gobj);
}

// 0x8017279C
void itMainClearOwnerStats(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_damage_all = TRUE;

    ip->owner_gobj = NULL;

    ip->team = ITEM_TEAM_DEFAULT;
}

// 0x801727BC
void itMainCopyDamageStats(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->owner_gobj = ip->damage_gobj;
    ip->team = ip->damage_team;
    ip->player = ip->damage_port;
    ip->player_num = ip->player_num; // Could potentially cause a bug? Didn't they mean damage_player_num?
    ip->handicap = ip->damage_handicap;
    ip->display_mode = ip->damage_display_mode;
}

// 0x801727F4
s32 itMainGetDamageOutput(ITStruct *ip)
{
    s32 damage;

    if (ip->is_thrown)
    {
        f32 mag = syVectorMag3D(&ip->physics.vel_air) * 0.1F;

        damage = (ip->attack_coll.damage + mag) * ip->attack_coll.throw_mul;
    }
    else damage = ip->attack_coll.damage;

    return (damage * ip->attack_coll.stale) + 0.999F;
}

// 0x80172890
sb32 itMainCheckShootNoAmmo(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (((ip->kind == nITKindStarRod) || (ip->kind == nITKindLGun) || (ip->kind == nITKindFFlower)) && (ip->multi == 0))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x801728D4
void itMainDestroyItem(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if ((ip->is_hold) && (ip->owner_gobj != NULL))
    {
        FTStruct *fp = ftGetStruct(ip->owner_gobj);

        fp->item_gobj = NULL;

        ftParamSetHammerParams(ip->owner_gobj);
    }
    else if ((ip->kind < nITKindGroundMonsterStart) || (ip->kind > nITKindGroundMonsterEnd))
    {
        efManagerDustExpandLargeMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);
    }
    if (ip->arrow_gobj != NULL)
    {
        gcEjectGObj(ip->arrow_gobj);
    }
    itManagerSetPrevStructAlloc(ip);
    gcEjectGObj(item_gobj);
}

// 0x80172984 - Link's Bomb erroneously redeclares this without stat_flags and stat_count
void itMainSetFighterRelease(GObj *item_gobj, Vec3f *vel, f32 throw_mul, u16 stat_flags, u16 stat_count)
{
    ITStruct *ip = itGetStruct(item_gobj);
    GObj *fighter_gobj = ip->owner_gobj;
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    s32 joint_id;

    lbCommonEjectTreeDObj(DObjGetStruct(item_gobj));

    pos.x = pos.y = pos.z = 0.0F;

    joint_id = (ip->weight == nITWeightHeavy) ? fp->attr->joint_itemheavy_id : fp->attr->joint_itemlight_id;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[joint_id], &pos);

    DObjGetStruct(item_gobj)->translate.vec.f.x = pos.x;
    DObjGetStruct(item_gobj)->translate.vec.f.y = pos.y;
    DObjGetStruct(item_gobj)->translate.vec.f.z = 0.0F;

    mpCommonRunItemCollisionDefault(item_gobj, fp->coll_data.p_translate, &fp->coll_data);

    fp->item_gobj = NULL;

    ip->is_hold = FALSE;

    ip->physics.vel_air = *vel;

    syVectorScale3D(&ip->physics.vel_air, ip->vel_scale);

    ip->times_thrown++;
    ip->is_thrown = TRUE;

    ip->attack_coll.throw_mul = throw_mul;

    ip->attack_coll.stat_flags = *(GMStatFlags*)&stat_flags;
    ip->attack_coll.stat_count = stat_count;

    ftParamSetHammerParams(fighter_gobj);
    itMainRefreshAttackColl(item_gobj);
}

// 0x80172AEC
void itMainSetFighterDrop(GObj *item_gobj, Vec3f *vel, f32 throw_mul)
{
    ITStruct *ip = itGetStruct(item_gobj);
    GObj *owner_gobj = ip->owner_gobj;
    FTStruct *fp = ftGetStruct(owner_gobj);

    if (dITMainProcDroppedList[ip->kind] != NULL)
    {
        dITMainProcDroppedList[ip->kind](item_gobj);
    }
    itMainSetFighterRelease(item_gobj, vel, throw_mul, nFTStatusAttackIDItemThrow, fp->stat_count);

    func_800269C0_275C0(ip->drop_sfx);
}

// 0x80172B78
void itMainSetFighterThrow(GObj *item_gobj, Vec3f *vel, f32 throw_mul, sb32 is_smash_throw)
{
    ITStruct *ip = itGetStruct(item_gobj);
    GObj *owner_gobj = ip->owner_gobj;
    FTStruct *fp = ftGetStruct(owner_gobj);

    if (ip->weight == nITWeightLight)
    {
        if (is_smash_throw != FALSE)
        {
            ftParamMakeRumble(fp, 6, 0);
        }
    }
    else ftParamMakeRumble(fp, (is_smash_throw != FALSE) ? 9 : 6, 0);

    if (dITMainProcThrownList[ip->kind] != NULL)
    {
        dITMainProcThrownList[ip->kind](item_gobj);
    }
    itMainSetFighterRelease(item_gobj, vel, throw_mul, fp->stat_flags.halfword, fp->stat_count);

    efManagerSparkleWhiteScaleMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f, 1.0F);

    func_800269C0_275C0((is_smash_throw != FALSE) ? ip->smash_sfx : ip->throw_sfx);

    itMainSetThrownSpin(item_gobj, vel, is_smash_throw);
}

// 0x80172CA4
void itMainSetFighterHold(GObj *item_gobj, GObj *fighter_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *joint;
    s32 unused;
    Vec3f pos;
    s32 joint_id;

    fp->item_gobj = item_gobj;
    ip->owner_gobj = fighter_gobj;

    ip->is_allow_pickup = FALSE;
    ip->is_hold = TRUE;

    ip->team = fp->team;
    ip->player = fp->player;
    ip->handicap = fp->handicap;
    ip->player_num = fp->player_num;

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    ip->display_mode = fp->display_mode;

    itMapSetAir(ip);

    joint = gcAddDObjForGObj(item_gobj, NULL);

    joint->sib_prev->sib_next = NULL;
    joint->sib_prev = NULL;
    joint->child = DObjGetStruct(item_gobj);

    DObjGetStruct(item_gobj)->parent = joint;

    item_gobj->obj = joint;

#if defined(REGION_US)
    gcAddXObjForDObjFixed(joint, 0x52, 0);
#else
    gcAddXObjForDObjFixed(joint, 0x51, 0);
#endif

    joint_id = (ip->weight == nITWeightHeavy) ? fp->attr->joint_itemheavy_id : fp->attr->joint_itemlight_id;

    joint->user_data.p = fp->joints[joint_id];

    pos.x = 0.0F;
    pos.y = 0.0F;
    pos.z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[joint_id], &pos);
    efManagerItemGetSwirlProcUpdate(&pos);
    gcSetDObjTransformsForGObj(item_gobj, ip->attr->data);

    if (dITMainProcHoldList[ip->kind] != NULL)
    {
        dITMainProcHoldList[ip->kind](item_gobj);
    }
    ftParamLinkResetShieldModelParts(fighter_gobj);

    if (ip->weight == nITWeightLight)
    {
        func_800269C0_275C0(nSYAudioFGMItemGet);
    }
    else if (fp->attr->heavyget_sfx != nSYAudioFGMVoiceEnd)
    {
        func_800269C0_275C0(fp->attr->heavyget_sfx);
    }
    ftParamMakeRumble(fp, 6, 0);

    ip->pickup_wait = ITEM_PICKUP_WAIT_DEFAULT;
}

// 0x80172E74
void itMainSetGroundAllowPickup(GObj *item_gobj) // Airborne item becomes grounded?
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->physics.vel_air.x = ip->physics.vel_air.y = ip->physics.vel_air.z = 0.0F;

    ip->is_allow_pickup = TRUE;

    ip->times_landed = 0;

    itMainResetPlayerVars(item_gobj);
    itMapSetGround(ip);
}

// 0x80172EC8
void itMainSetStatus(GObj *item_gobj, ITStatusDesc *status_desc, s32 status_id)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->proc_update    = status_desc[status_id].proc_update;
    ip->proc_map       = status_desc[status_id].proc_map;
    ip->proc_hit       = status_desc[status_id].proc_hit;
    ip->proc_shield    = status_desc[status_id].proc_shield;
    ip->proc_hop       = status_desc[status_id].proc_hop;
    ip->proc_setoff    = status_desc[status_id].proc_setoff;
    ip->proc_reflector = status_desc[status_id].proc_reflector;
    ip->proc_damage    = status_desc[status_id].proc_damage;

    ip->is_thrown = FALSE;

    ip->attack_coll.stat_flags.attack_id = nFTStatusAttackIDNull;
    ip->attack_coll.stat_flags.is_smash_attack = ip->attack_coll.stat_flags.ga = ip->attack_coll.stat_flags.is_projectile = FALSE;

    ip->attack_coll.stat_count = ftParamGetStatUpdateCount();
}

// 0x80172F98
sb32 itMainCheckSetColAnimID(GObj *item_gobj, s32 colanim_id, s32 duration)
{
    ITStruct *ip = itGetStruct(item_gobj);

    return ftParamCheckSetColAnimID(&ip->colanim, colanim_id, duration);
}

// 0x80172FBC
void itMainClearColAnim(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ftParamResetColAnim(&ip->colanim);
}

// 0x80172FE0
void itMainVelSetRebound(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->physics.vel_air.x *= -0.06F;
    ip->physics.vel_air.y = (ip->physics.vel_air.y * -0.3F) + 25.0F;
}

// 0x8017301C - Binary search function to get item ID for container drop?
s32 itMainSearchRandomWeight(s32 random, ITRandomWeights *weights, u32 min, u32 max) // Recursive!
{
    if (max == (min + 1))
    {
        return min;
    }
    else
    {
        s32 avg = (s32) (min + max) / 2;

        if (random < weights->blocks[avg])
        {
            itMainSearchRandomWeight(random, weights, min, avg);
        }
        else if (random < weights->blocks[avg + 1])
        {
            return avg;
        }
        else itMainSearchRandomWeight(random, weights, avg, max);
    }
}

// 0x80173090
s32 itMainGetWeightedItemKind(ITRandomWeights *weights)
{
    return weights->kinds[itMainSearchRandomWeight(syUtilsRandIntRange(weights->weights_sum), weights, 0, weights->valids_num)];
}

// 0x801730D4
sb32 itMainMakeContainerItem(GObj *parent_gobj)
{
    s32 unused;
    s32 kind;
    Vec3f vel; // Item's spawn velocity when falling out of a container

    if (gITManagerRandomWeights.weights_sum != 0)
    {
        kind = itMainGetWeightedItemKind(&gITManagerRandomWeights);

        if (kind <= nITKindCommonEnd)
        {
            vel.x = 0.0F;

            // Quite ridiculous especially since llITCommonDataContainerVelocitiesY is 0
            vel.y = *(f32*) ((intptr_t)&llITCommonDataContainerVelocitiesY + ((uintptr_t) &((f32*)gITManagerCommonData)[kind]));
            vel.z = 0;

            if
            (
                itManagerMakeItemSetupCommon
                (
                    parent_gobj,
                    kind,
                    &DObjGetStruct(parent_gobj)->translate.vec.f,
                    &vel,
                    (ITEM_FLAG_COLLPROJECT | ITEM_FLAG_PARENT_ITEM)
                )
                != NULL
            )
            {
                itMainSetAppearSpin(parent_gobj, TRUE);
            }
            return TRUE;
        }
    }
    return FALSE;
}

// 0x80173180
void itMainUpdateAttackEvent(GObj *item_gobj, ITAttackEvent *ev)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == ev[ip->event_id].timer)
    {
        ip->attack_coll.angle  = ev[ip->event_id].angle;
        ip->attack_coll.damage = ev[ip->event_id].damage;
        ip->attack_coll.size   = ev[ip->event_id].size;

        ip->event_id++;

        if (ip->event_id == 4)
        {
            ip->event_id = 3;
        }
    }
}

// 0x80173228
GObj* itMainMakeMonster(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    GObj *monster_gobj;
    ITStruct *mp;
    s32 i, j;
    s32 index;
    s32 unused;
    Vec3f vel;

    vel.x = 0.0F;
    vel.y = 16.0F;
    vel.z = 0.0F;

    // Is this checking to spawn Mew? Can only spawn once at least one character has been unlocked.
    if
    (
        (gSCManagerBackupData.unlock_mask & LBBACKUP_UNLOCK_MASK_NEWCOMERS) &&
        (syUtilsRandIntRange(151) == 0) &&
        (gITManagerMonsterData.monster_curr != nITKindMew) &&
        (gITManagerMonsterData.monster_prev != nITKindMew)
    )
    {
        index = nITKindMew;
    }
    else
    {
        for (i = j = nITKindMBallCommonStart; i <= nITKindMBallCommonEnd; i++) // Pokémon IDs
        {
            if ((i != gITManagerMonsterData.monster_curr) && (i != gITManagerMonsterData.monster_prev))
            {
                gITManagerMonsterData.monster_id[j - nITKindMBallMonsterStart] = i;
                j++;
            }
        }
        index = gITManagerMonsterData.monster_id[syUtilsRandIntRange(gITManagerMonsterData.monsters_num)];
    }
    if (gITManagerMonsterData.monsters_num != 10)
    {
        gITManagerMonsterData.monsters_num--;
    }
    gITManagerMonsterData.monster_prev = gITManagerMonsterData.monster_curr;
    gITManagerMonsterData.monster_curr = index;

    monster_gobj = itManagerMakeItemKind(item_gobj, index, &DObjGetStruct(item_gobj)->translate.vec.f, &vel, (ITEM_FLAG_COLLPROJECT | ITEM_FLAG_PARENT_ITEM));

    if (monster_gobj != NULL)
    {
        mp = itGetStruct(monster_gobj);

        mp->owner_gobj = ip->owner_gobj;
        mp->team = ip->team;
        mp->player = ip->player;
        mp->handicap = ip->handicap;
        mp->player_num = ip->player_num;
        mp->display_mode = ip->display_mode;

        if (gSCManagerBattleState->game_type == nSCBattleGameType1PGame)
        {
            if ((mp->player == gSCManagerSceneData.player) && (mp->kind == nITKindMew))
            {
                gSC1PGameBonusMewCatcher = TRUE;
            }
        }
    }
    return monster_gobj;
}

// 0x801733E4
sb32 itMainCommonProcHop(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    syVectorRotateAbout3D(&ip->physics.vel_air, &ip->shield_collide_dir, ip->shield_collide_angle * 2);
    itMainSetSpinVelLR(item_gobj);

    return FALSE;
}

// 0x80173434
sb32 itMainCommonProcReflector(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    FTStruct *fp = ftGetStruct(ip->owner_gobj);

    if ((ip->physics.vel_air.x * fp->lr) < 0.0F)
    {
        ip->physics.vel_air.x = -ip->physics.vel_air.x;
    }
    return FALSE;
}
