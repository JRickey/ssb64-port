#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <gr/ground.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern alSoundEffect* func_800269C0_275C0(u16);
extern s32 dGRYamabukiMonsterAttackKind;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITFushigibanaItemDesc =
{
    nITKindFushigibana,                         // Item Kind
    &gGRCommonStruct.yamabuki.item_head,        // Pointer to item file data?
    &llGRYamabukiMapFushigibanaItemAttributes,  // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,                // Main matrix transformations
        nGCMatrixKindNull,                      // Secondary matrix transformations?
        0                                       // ???
    },

    nGMAttackStateNew,                          // Hitbox Update State
    itFushigibanaCommonProcUpdate,              // Proc Update
    NULL,                                       // Proc Map
    NULL,                                       // Proc Hit
    NULL,                                       // Proc Shield
    NULL,                                       // Proc Hop
    NULL,                                       // Proc Set-Off
    NULL,                                       // Proc Reflector
    NULL                                        // Proc Damage
};

WPDesc dITFushigibanaWeaponRazorWeaponDesc =
{
    0x03,                                       // Render flags?
    nWPKindFushigibanaRazor,                    // Weapon Kind
    &gGRCommonStruct.yamabuki.item_head,        // Pointer to item's loaded files?
    &llGRYamabukiMapFushigibanaRazorWeaponAttributes, // Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,             // Main matrix transformations
        nGCMatrixKindNull,                      // Secondary matrix transformations?
        0                                       // ???
    },

    itFushigibanaWeaponRazorProcUpdate,         // Proc Update
    NULL,                                       // Proc Map
    itFushigibanaWeaponRazorProcHit,            // Proc Hit
    itFushigibanaWeaponRazorProcHit,            // Proc Shield
    itFushigibanaWeaponRazorProcHop,            // Proc Hop
    itFushigibanaWeaponRazorProcHit,            // Proc Set-Off
    itFushigibanaWeaponRazorProcReflector,      // Proc Reflector
    itFushigibanaWeaponRazorProcHit             // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80184440
void itFushigibanaCommonUpdateMonsterEvent(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    ITMonsterEvent *ev = itGetMonsterEvent(dITFushigibanaItemDesc, &llGRYamabukiMapFushigibanaHitParties); // (ITMonsterEvent*) ((uintptr_t)*dITFushigibanaItemDesc.p_file + (intptr_t)&Fushigibana_Event);

    if (ip->multi == ev[ip->event_id].timer)
    {
        ip->attack_coll.angle            = ev[ip->event_id].angle;
        ip->attack_coll.damage           = ev[ip->event_id].damage;
        ip->attack_coll.size             = ev[ip->event_id].size;
        ip->attack_coll.knockback_scale  = ev[ip->event_id].knockback_scale;
        ip->attack_coll.knockback_weight = ev[ip->event_id].knockback_weight;
        ip->attack_coll.knockback_base   = ev[ip->event_id].knockback_base;
        ip->attack_coll.element          = ev[ip->event_id].element;
        ip->attack_coll.can_setoff       = ev[ip->event_id].can_setoff;
        ip->attack_coll.shield_damage    = ev[ip->event_id].shield_damage;
        ip->attack_coll.fgm_id          = ev[ip->event_id].fgm_id;

        ip->event_id++;

        if (ip->event_id == 2)
        {
            ip->event_id = 1;
        }
    }
    ip->multi++;

    if (ip->multi == ITFUSHIGIBANA_RETURN_WAIT)
    {
        Vec3f pos = DObjGetStruct(item_gobj)->translate.vec.f;

        pos.y = 0.0F;

        efManagerDustLightMakeEffect(&pos, -1, 1.0F);
    }
}

// 0x801845B4
sb32 itFushigibanaCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    Vec3f pos;

    dobj->translate.vec.f.x += ip->item_vars.fushigibana.offset.x;
    dobj->translate.vec.f.y += ip->item_vars.fushigibana.offset.y;

    itFushigibanaCommonUpdateMonsterEvent(item_gobj);

    pos = dobj->translate.vec.f;

    pos.x += ITFUSHIGIBANA_RAZOR_SPAWN_OFF_X;

    if 
    (
        (ip->item_vars.fushigibana.flags == GRYAMABUKI_MONSTER_WEAPON_INSTANT)                                                     || 
        ((ip->item_vars.fushigibana.flags & GRYAMABUKI_MONSTER_WEAPON_WAIT) && (dobj->anim_frame >= ITFUSHIGIBANA_RAZOR_SPAWN_BEGIN)) &&
        (dobj->anim_frame <= ITFUSHIGIBANA_RAZOR_SPAWN_END)
    )
    {
        dobj->mobj->texture_id_curr = 1;

        if (!ip->item_vars.fushigibana.razor_spawn_wait)
        {
            itFushigibanaWeaponRazorMakeWeapon(item_gobj, &pos);

            ip->item_vars.fushigibana.razor_spawn_wait = ITFUSHIGIBANA_RAZOR_SPAWN_WAIT;

            func_800269C0_275C0(nSYAudioFGMMonsterShoot);

            efManagerDustCollideMakeEffect(&pos);
        }
        if (ip->item_vars.fushigibana.razor_spawn_wait > 0)
        {
            ip->item_vars.fushigibana.razor_spawn_wait--;
        }
    }
    else dobj->mobj->texture_id_curr = 0;

    if (dobj->anim_wait == AOBJ_ANIM_NULL)
    {
        grYamabukiGateSetClosedWait();

        return TRUE;
    }
    return FALSE;
}

// 0x8018470C
GObj* itFushigibanaMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITFushigibanaItemDesc, pos, vel, flags);
    s32 unused;
    DObj *dobj;
    ITStruct *ip;

    if (item_gobj != NULL)
    {
        ip = itGetStruct(item_gobj);
        dobj = DObjGetStruct(item_gobj);

        ip->event_id = 0;

        ip->multi = 0;

        ip->item_vars.fushigibana.razor_spawn_wait = 0;
        ip->item_vars.fushigibana.offset = *pos;

        ip->is_allow_knockback = TRUE;

        ip->item_vars.fushigibana.flags = syUtilsRandIntRange(GRYAMABUKI_MONSTER_WEAPON_MAX);

        if ((dGRYamabukiMonsterAttackKind == ip->item_vars.fushigibana.flags) || (ip->item_vars.fushigibana.flags & dGRYamabukiMonsterAttackKind))
        {
            ip->item_vars.fushigibana.flags++;

            ip->item_vars.fushigibana.flags %= GRYAMABUKI_MONSTER_WEAPON_MAX;
        }
        if (ip->item_vars.fushigibana.flags == GRYAMABUKI_MONSTER_WEAPON_INSTANT)
        {
            dobj->mobj->texture_id_curr = 1;
        }
        dGRYamabukiMonsterAttackKind = ip->item_vars.fushigibana.flags;

        func_800269C0_275C0(nSYAudioVoiceYamabukiFushigibana);
    }
    return item_gobj;
}

// 0x80184820
sb32 itFushigibanaWeaponRazorProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->physics.vel_air.x += ITFUSHIGIBANA_RAZOR_ADD_VEL_X * wp->lr;

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80184874
sb32 itFushigibanaWeaponRazorProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerDamageSlashMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->attack_coll.damage, wp->lr);

    return TRUE;
}

// 0x801848BC
sb32 itFushigibanaWeaponRazorProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x) + F_CLC_DTOR32(180.0F);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    if (wp->physics.vel_air.x > 0.0F)
    {
        wp->lr = +1;
    }
    else wp->lr = -1;

    return FALSE;
}

// 0x80184970
sb32 itFushigibanaWeaponRazorProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);

    wpMainReflectorSetLR(wp, fp);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.z = syUtilsArcTan2(wp->physics.vel_air.y, wp->physics.vel_air.x) + F_CLC_DTOR32(180.0F);
    DObjGetStruct(weapon_gobj)->scale.vec.f.x = 1.0F;

    wp->lr = -wp->lr;

    return FALSE;
}

// 0x801849EC
GObj* itFushigibanaWeaponRazorMakeWeapon(GObj *item_gobj, Vec3f *pos)
{
    GObj *weapon_gobj = wpManagerMakeWeapon(item_gobj, &dITFushigibanaWeaponRazorWeaponDesc, pos, WEAPON_FLAG_PARENT_ITEM);
    DObj *dobj;
    WPStruct *wp;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    wp = wpGetStruct(weapon_gobj);

    wp->lr = -1;

    wp->physics.vel_air.x = ITFUSHIGIBANA_RAZOR_VEL_X;

    dobj = DObjGetStruct(weapon_gobj);

    dobj->translate.vec.f = *pos;

    wp->lifetime = ITFUSHIGIBANA_RAZOR_LIFETIME;

    return weapon_gobj;
}
