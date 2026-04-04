#include <wp/weapon.h>
#include <it/item.h>
#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x8018CFF0 - Points to next available weapon struct
WPStruct *sWPManagerStructsAllocFree;

// 0x8018CFF4
s32 sWPManagerDisplayMode;

// 0x8018CFF8
u32 sWPManagerGroupID;

// 0x8018CFFC
u8 sWPManagerPad0x8018CFFC[52];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801654B0
void wpManagerAllocWeapons(void)
{
    WPStruct *wp;
    s32 i;

    sWPManagerStructsAllocFree = wp = syTaskmanMalloc(sizeof(WPStruct) * WEAPON_ALLOC_MAX, 0x8);

    for (i = 0; i < (WEAPON_ALLOC_MAX - 1); i++)
    {
        wp[i].next = &wp[i + 1];
    }
    if (wp != NULL)
    {
        wp[i].next = NULL;
    }
    sWPManagerGroupID = 1;
    sWPManagerDisplayMode = nDBDisplayModeMaster;
}

// 0x80165558
WPStruct* wpManagerGetNextStructAlloc(void)
{
    WPStruct *new_weapon = sWPManagerStructsAllocFree;
    WPStruct *get_weapon;

    if (new_weapon == NULL)
    {
        return NULL;
    }
    get_weapon = new_weapon;

    sWPManagerStructsAllocFree = new_weapon->next;

    return get_weapon;
}

// 0x80165588
void wpManagerSetPrevStructAlloc(WPStruct *wp)
{
    wp->next = sWPManagerStructsAllocFree;

    sWPManagerStructsAllocFree = wp;
}

// 0x801655A0 - WARNING: Do NOT declare this with a void argument! PK Thunder passes unused arguments to this function.
u32 wpManagerGetGroupID()
{
    u32 group_id = sWPManagerGroupID++;

    if (sWPManagerGroupID == 0)
    {
        sWPManagerGroupID++;
    }
    return group_id;
}

// 0x801655C8
GObj* wpManagerMakeWeapon(GObj *parent_gobj, WPDesc *wp_desc, Vec3f *spawn_pos, u32 flags)
{
    GObj *weapon_gobj;
    void (*proc_display)(GObj*);
    WPAttributes *attr;
    WPStruct *wp;
    WPStruct *owner_wp;
    ITStruct *ip;
    FTStruct *fp;
    s32 unused[7];

    wp = wpManagerGetNextStructAlloc();

    if (wp == NULL)
    {
        return NULL;
    }
    weapon_gobj = gcMakeGObjSPAfter(nGCCommonKindWeapon, NULL, nGCCommonLinkIDWeapon, GOBJ_PRIORITY_DEFAULT);

    if (weapon_gobj == NULL)
    {
        wpManagerSetPrevStructAlloc(wp);
        return NULL;
    }
    attr = lbRelocGetFileData(WPAttributes*, *wp_desc->p_weapon, wp_desc->o_attributes); // I hope this is correct?
    weapon_gobj->user_data.p = wp;
    wp->weapon_gobj = weapon_gobj;
    wp->kind = wp_desc->kind;

    switch (flags & WEAPON_MASK_PARENT)
    {
    case WEAPON_FLAG_PARENT_FIGHTER: // Weapons spawned by fighters
        fp = ftGetStruct(parent_gobj);
        wp->owner_gobj = parent_gobj;
        wp->team = fp->team;
        wp->player = fp->player;
        wp->handicap = fp->handicap;
        wp->player_num = fp->player_num;
        wp->lr = fp->lr;

        wp->display_mode = fp->display_mode;

        wp->attack_coll.stale = ftParamGetStale(fp->player, fp->motion_attack_id, fp->motion_count);
        wp->attack_coll.motion_attack_id = fp->motion_attack_id;
        wp->attack_coll.motion_count = fp->motion_count;
        wp->attack_coll.stat_flags = fp->stat_flags;
        wp->attack_coll.stat_count = fp->stat_count;
        break;

    case WEAPON_FLAG_PARENT_WEAPON: // Weapons spawned by other weapons
        owner_wp = wpGetStruct(parent_gobj);
        wp->owner_gobj = owner_wp->owner_gobj;
        wp->team = owner_wp->team;
        wp->player = owner_wp->player;
        wp->handicap = owner_wp->handicap;
        wp->player_num = owner_wp->player_num;
        wp->lr = owner_wp->lr;

        wp->display_mode = owner_wp->display_mode;

        wp->attack_coll.stale = owner_wp->attack_coll.stale;
        wp->attack_coll.motion_attack_id = owner_wp->attack_coll.motion_attack_id;
        wp->attack_coll.motion_count = owner_wp->attack_coll.motion_count;
        wp->attack_coll.stat_flags = owner_wp->attack_coll.stat_flags;
        wp->attack_coll.stat_count = owner_wp->attack_coll.stat_count;
        break;

    case WEAPON_FLAG_PARENT_ITEM: // Weapons spawned by items
        ip = itGetStruct(parent_gobj);
        wp->owner_gobj = ip->owner_gobj;
        wp->team = ip->team;
        wp->player = ip->player;
        wp->handicap = ip->handicap;
        wp->player_num = ip->player_num;
        wp->lr = ip->lr;

        wp->display_mode = ip->display_mode;

        wp->attack_coll.stale = ip->attack_coll.stale;
        wp->attack_coll.motion_attack_id = ip->attack_coll.motion_attack_id;
        wp->attack_coll.motion_count = ip->attack_coll.motion_count;
        wp->attack_coll.stat_flags = ip->attack_coll.stat_flags;
        wp->attack_coll.stat_count = ip->attack_coll.stat_count;
        break;

    default: // Weapons spawned independently 
    case WEAPON_FLAG_PARENT_GROUND:
        wp->owner_gobj = NULL;
        wp->team = WEAPON_TEAM_DEFAULT;
        wp->player = WEAPON_PORT_DEFAULT;
        wp->handicap = WEAPON_HANDICAP_DEFAULT;
        wp->player_num = 0;
        wp->lr = +1;

        wp->display_mode = sWPManagerDisplayMode;

        wp->attack_coll.motion_attack_id = nFTMotionAttackIDNone;
        wp->attack_coll.stale = WEAPON_STALE_DEFAULT;
        wp->attack_coll.motion_count = ftParamGetMotionCount();
        wp->attack_coll.stat_flags.attack_id = nFTStatusAttackIDNone;
        wp->attack_coll.stat_flags.is_smash_attack = wp->attack_coll.stat_flags.ga = wp->attack_coll.stat_flags.is_projectile = 0;
        wp->attack_coll.stat_count = ftParamGetStatUpdateCount();
        break;
    }
    wp->attack_coll.attack_state = nGMAttackStateNew;
    
    wp->physics.vel_air.x = wp->physics.vel_air.y = wp->physics.vel_air.z = 0.0F;
    wp->physics.vel_ground = 0.0F;

    wp->attack_coll.damage = attr->damage;
    wp->attack_coll.element = attr->element;

    wp->attack_coll.offsets[0].x = attr->attack_offsets[0].x;
    wp->attack_coll.offsets[0].y = attr->attack_offsets[0].y;
    wp->attack_coll.offsets[0].z = attr->attack_offsets[0].z;
    wp->attack_coll.offsets[1].x = attr->attack_offsets[1].x;
    wp->attack_coll.offsets[1].y = attr->attack_offsets[1].y;
    wp->attack_coll.offsets[1].z = attr->attack_offsets[1].z;

    wp->attack_coll.size = attr->size * 0.5F;

    wp->attack_coll.angle = attr->angle;

    wp->attack_coll.knockback_scale = attr->knockback_scale;
    wp->attack_coll.knockback_weight = attr->knockback_weight;
    wp->attack_coll.knockback_base = attr->knockback_base;

    wp->attack_coll.can_setoff = attr->can_setoff;

    wp->attack_coll.shield_damage = attr->shield_damage;

    wp->attack_coll.fgm_id = attr->sfx;

    wp->attack_coll.priority = attr->priority;

    wp->attack_coll.can_rehit_item = attr->can_rehit_item;
    wp->attack_coll.can_rehit_fighter = attr->can_rehit_fighter;
    wp->attack_coll.can_rehit_shield = FALSE;

    wp->attack_coll.can_hop = attr->can_hop;
    wp->attack_coll.can_reflect = attr->can_reflect;
    wp->attack_coll.can_absorb = attr->can_absorb;
    wp->attack_coll.can_not_heal = FALSE;
    wp->attack_coll.can_shield = attr->can_shield;
    
    wp->attack_coll.attack_count = attr->attack_count;

    wp->attack_coll.interact_mask = GMHITCOLLISION_FLAG_ALL;

    wpMainClearAttackRecord(wp);

    wp->hit_normal_damage = 0;
    wp->hit_refresh_damage = 0;
    wp->hit_attack_damage = 0;
    wp->hit_shield_damage = 0;
    wp->reflect_gobj = NULL;
    wp->absorb_gobj = NULL;

    wp->is_hitlag_victim = FALSE;
    wp->is_hitlag_weapon = FALSE;
    wp->is_camera_follow = FALSE;

    wp->group_id = 0;

    wp->is_static_damage = FALSE;

    wp->p_sfx = NULL;
    wp->sfx_id = 0;

    wp->shield_collide_angle = 0.0F;

    wp->shield_collide_dir.x = wp->shield_collide_dir.y = wp->shield_collide_dir.z = 0.0F;

    if (wp_desc->flags & WEAPON_FLAG_DOBJDESC)
    {
        gcSetupCustomDObjs(weapon_gobj, attr->data, NULL, wp_desc->transform_types.tk1, wp_desc->transform_types.tk2, wp_desc->transform_types.tk3);

        proc_display = (wp_desc->flags & WEAPON_FLAG_DOBJLINKS) ? wpDisplayDObjTreeDLLinks : func_ovl3_80167618;
    }
    else
    {
        lbCommonInitDObj3Transforms(gcAddDObjForGObj(weapon_gobj, attr->data), wp_desc->transform_types.tk1, wp_desc->transform_types.tk2, wp_desc->transform_types.tk3);

        proc_display = (wp_desc->flags & WEAPON_FLAG_DOBJLINKS) ? wpDisplayDObjDLLinks : wpDisplayDLHead1;
    }
    gcAddGObjDisplay(weapon_gobj, proc_display, 14, GOBJ_PRIORITY_DEFAULT, ~0);

    if (attr->p_mobjsubs != NULL)
    {
        gcAddMObjAll(weapon_gobj, attr->p_mobjsubs);
    }
    if ((attr->anim_joints != NULL) || (attr->p_matanim_joints != NULL))
    {
        gcAddAnimAll(weapon_gobj, attr->anim_joints, attr->p_matanim_joints, 0.0F);
    }
    wp->coll_data.p_translate = &DObjGetStruct(weapon_gobj)->translate.vec.f;
    wp->coll_data.p_lr = &wp->lr;

    wp->coll_data.map_coll.top = attr->map_coll_top;
    wp->coll_data.map_coll.center = attr->map_coll_center;
    wp->coll_data.map_coll.bottom = attr->map_coll_bottom;
    wp->coll_data.map_coll.width = attr->map_coll_width;
    wp->coll_data.p_map_coll = &wp->coll_data.map_coll;

    wp->coll_data.ignore_line_id = -1;
    wp->coll_data.floor_line_id = -1;
    wp->coll_data.ceil_line_id = -1;
    wp->coll_data.lwall_line_id = -1;
    wp->coll_data.rwall_line_id = -1;

    wp->coll_data.update_tic = gMPCollisionUpdateTic;
    wp->coll_data.mask_curr = 0;

    wp->coll_data.vel_push.x = 0.0F;
    wp->coll_data.vel_push.y = 0.0F;
    wp->coll_data.vel_push.z = 0.0F;

    gcAddGObjProcess(weapon_gobj, wpProcessProcWeaponMain, nGCProcessKindFunc, 3);
    gcAddGObjProcess(weapon_gobj, wpProcessProcSearchHitWeapon, nGCProcessKindFunc, 1);
    gcAddGObjProcess(weapon_gobj, wpProcessProcHitCollisions, nGCProcessKindFunc, 0);

    wp->proc_update    = wp_desc->proc_update;
    wp->proc_map       = wp_desc->proc_map;
    wp->proc_hit       = wp_desc->proc_hit;
    wp->proc_shield    = wp_desc->proc_shield;
    wp->proc_hop       = wp_desc->proc_hop;
    wp->proc_setoff    = wp_desc->proc_setoff;
    wp->proc_reflector = wp_desc->proc_reflector;
    wp->proc_absorb    = wp_desc->proc_absorb;
    wp->proc_dead      = NULL;

    wp->coll_data.pos_prev = DObjGetStruct(weapon_gobj)->translate.vec.f = *spawn_pos;

    if (flags & WEAPON_FLAG_COLLPROJECT)
    {
        switch (flags & WEAPON_MASK_PARENT)
        {
        default:
        case WEAPON_FLAG_PARENT_GROUND:
            break;

        case WEAPON_FLAG_PARENT_FIGHTER:
            mpCommonRunWeaponCollisionDefault(weapon_gobj, ftGetStruct(parent_gobj)->coll_data.p_translate, &ftGetStruct(parent_gobj)->coll_data);
            break;

        case WEAPON_FLAG_PARENT_WEAPON:
            mpCommonRunWeaponCollisionDefault(weapon_gobj, wpGetStruct(parent_gobj)->coll_data.p_translate, &wpGetStruct(parent_gobj)->coll_data);
            break;

        case WEAPON_FLAG_PARENT_ITEM:
            mpCommonRunWeaponCollisionDefault(weapon_gobj, itGetStruct(parent_gobj)->coll_data.p_translate, &itGetStruct(parent_gobj)->coll_data);
            break;
        }
    }
    wp->ga = nMPKineticsAir;

    wpProcessUpdateHitPositions(weapon_gobj);

    return weapon_gobj;
}
