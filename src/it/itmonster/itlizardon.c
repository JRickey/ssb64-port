#include <it/item.h>
#include <wp/weapon.h>
#include <ft/fighter.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018AD30
ITDesc dITLizardonItemDesc = 
{
    nITKindLizardon,                        // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataLizardonItemAttributes,  // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindNull,                   // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0,                                   // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itLizardonCommonProcUpdate,             // Proc Update
    itLizardonCommonProcMap,                // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018AD64
ITStatusDesc dITLizardonStatusDescs[/* */] = 
{
    // Status 0 (Unused Fall)
    {
        itLizardonFallUnusedProcUpdate,     // Proc Update
        itLizardonFallUnusedProcMap,        // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 1 (Air Fall)
    {
        itLizardonFallProcUpdate,           // Proc Update
        itLizardonFallProcMap,              // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    },

    // Status 2 (Neutral Attack)
    {
        itLizardonAttackProcUpdate,         // Proc Update
        itLizardonAttackProcMap,            // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// 0x8018ADC4
WPDesc dITLizardonWeaponFlameWeaponDesc = 
{
    0x00,                                   // Render flags?
    nWPKindLizardonFlame,                   // Weapon Kind
    &gITManagerCommonData,                    // Pointer to character's loaded files?
    &llITCommonDataLizardonFlameWeaponAttributes,// Offset of weapon attributes in loaded files

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,          // Main matrix transformations
        nGCMatrixKindNull,                   // Secondary matrix transformations?
        0,                                  // ???
    },

    itLizardonWeaponFlameProcUpdate,        // Proc Update
    itLizardonWeaponFlameProcMap,           // Proc Map
    itLizardonWeaponFlameProcHit,           // Proc Hit
    itLizardonWeaponFlameProcHit,           // Proc Shield
    NULL,                                   // Proc Hop
    itLizardonWeaponFlameProcHit,           // Proc Set-Off
    itLizardonWeaponFlameProcReflector,     // Proc Reflector
    NULL                                    // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itLizardonStatus
{
    nITLizardonStatusFallUnused,            // Unused
    nITLizardonStatusFall,
    nITLizardonStatusAttack,
    nITLizardonStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017F470
sb32 itLizardonFallUnusedProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITLIZARDON_GRAVITY, ITLIZARDON_TVEL);

    return FALSE;
}

// 0x8017F49C
sb32 itLizardonFallUnusedProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMapCheckLanding(item_gobj, ITLIZARDON_MAP_REBOUND_COMMON, ITLIZARDON_MAP_REBOUND_GROUND, itLizardonAttackSetStatus);

    if (ip->coll_data.mask_curr & MAP_FLAG_FLOOR)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x8017F49C
sb32 itLizardonFallUnusedSetStatus(GObj *item_gobj) // Unused
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->is_allow_pickup = FALSE;

    itMapSetAir(ip);
    itMainSetStatus(item_gobj, dITLizardonStatusDescs, nITLizardonStatusFallUnused);
}

// 0x8017F53C
sb32 itLizardonFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMainApplyGravityClampTVel(ip, ITLIZARDON_GRAVITY, ITLIZARDON_TVEL);

    return FALSE;
}

// 0x8017F568
sb32 itLizardonFallProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    itMapTestAllCheckCollEnd(item_gobj);

    if (ip->coll_data.mask_curr & MAP_FLAG_FLOOR)
    {
        ip->physics.vel_air.y = 0.0F;

        itLizardonAttackSetStatus(item_gobj);
        itLizardonAttackInitVars(item_gobj);
    }
    return FALSE;
}

// 0x8017F5C4
void itLizardonFallSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITLizardonStatusDescs, nITLizardonStatusFall);
}

// 0x8017F5EC
sb32 itLizardonAttackProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    Vec3f pos = dobj->translate.vec.f;

    if (ip->kind == nITKindLizardon)
    {
        pos.y += ITLIZARDON_LIZARDON_FLAME_OFF_Y;

        pos.x += (ITLIZARDON_LIZARDON_FLAME_OFF_X * ip->lr);
    }
    else pos.x += (ITLIZARDON_OTHER_FLAME_OFF_X * ip->lr);
    
    if (ip->item_vars.lizardon.flame_spawn_wait == 0)
    {
        itLizardonAttackMakeFlame(item_gobj, &pos, ip->lr);

        ip->item_vars.lizardon.flame_spawn_wait = ITLIZARDON_FLAME_SPAWN_WAIT;
    }
    ip->item_vars.lizardon.flame_spawn_wait--;

    if (ip->multi == 0)
    {
        return TRUE;
    }
    if (ip->item_vars.lizardon.turn_wait == 0)
    {
        ip->item_vars.lizardon.turn_wait = ITLIZARDON_TURN_WAIT;

        ip->lr = -ip->lr;

        pos = dobj->translate.vec.f;

        pos.y += ip->attr->map_coll_bottom;

        pos.x += (ip->attr->map_coll_width + ITLIZARDON_DUST_OFF_X) * -ip->lr;

        efManagerDustHeavyMakeEffect(&pos, -ip->lr);

        if (ip->kind == nITKindPippi)
        {
            dobj->rotate.vec.f.y += F_CST_DTOR32(180.0F);
        }
    }
    ip->item_vars.lizardon.turn_wait--;

    ip->multi--;

    return FALSE;
}

// 0x8017F7E8
sb32 itLizardonAttackProcMap(GObj *item_gobj)
{
    itMapCheckLRWallProcNoFloor(item_gobj, itLizardonFallSetStatus);

    return FALSE;
}

// 0x8017F810
void itLizardonAttackInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);
    s32 unused[2];
    void *addr;
    Vec3f pos;

    ip->item_vars.lizardon.turn_wait = ITLIZARDON_TURN_WAIT;

    pos = dobj->translate.vec.f;

    ip->item_vars.lizardon.pos = pos;
    ip->item_vars.lizardon.flame_spawn_wait = 0;

    ip->lr = -1;

    if (ip->kind == nITKindLizardon)
    {
        addr = (void*) ((uintptr_t)ip->attr->data - (intptr_t)&llITCommonDataLizardonDataStart);

        gcAddDObjAnimJoint(dobj, lbRelocGetFileData(AObjEvent32*, addr, &llITCommonDataLizardonAnimJoint), 0.0F);
        gcAddMObjMatAnimJoint(dobj->mobj, lbRelocGetFileData(AObjEvent32*, addr, &llITCommonDataLizardonMatAnimJoint), 0.0F);
        gcPlayAnimAll(item_gobj);
    }
}

// 0x8017F8E4
void itLizardonAttackSetStatus(GObj *item_gobj)
{
    itMainSetStatus(item_gobj, dITLizardonStatusDescs, nITLizardonStatusAttack);
}

// 0x8017F90C
sb32 itLizardonCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        ip->multi = ITLIZARDON_LIFETIME;

        ip->physics.vel_air.y = 0.0F;

        if (ip->kind == nITKindLizardon)
        {
            func_800269C0_275C0(nSYAudioVoiceMBallLizardonAppear);
        }
        itLizardonFallSetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017F98C
sb32 itLizardonCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x8017F9CC
GObj* itLizardonMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITLizardonItemDesc, pos, vel, flags);
    DObj *dobj;
    ITStruct *ip;

    if (item_gobj != NULL)
    {
        dobj = DObjGetStruct(item_gobj);

        gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);
        gcAddXObjForDObjFixed(dobj, 0x48, 0);

        dobj->translate.vec.f = *pos;

        ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y;

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;

        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataLizardonDataStart), 0.0F);
    }
    return item_gobj;
}

// 0x8017FACC
sb32 itLizardonWeaponFlameProcUpdate(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    if (wpMainDecLifeCheckExpire(wp) != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8017FAF8
sb32 itLizardonWeaponFlameProcMap(GObj *weapon_gobj)
{
    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else return FALSE;
}

// 0x8017FB3C
sb32 itLizardonWeaponFlameProcHit(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return FALSE;
}

// 0x8017FB74
sb32 itLizardonWeaponFlameProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);
    Vec3f *translate;

    wp->lifetime = ITLIZARDON_FLAME_LIFETIME;

    wpMainReflectorSetLR(wp, fp);

    translate = &DObjGetStruct(weapon_gobj)->translate.vec.f;

    lbParticleMakePosVel(gITManagerParticleBankID | LBPARTICLE_MASK_GENLINK(0), 2, translate->x, translate->y, 0.0F, wp->physics.vel_air.x, wp->physics.vel_air.y, 0.0F);
    lbParticleMakePosVel(gITManagerParticleBankID | LBPARTICLE_MASK_GENLINK(0), 0, translate->x, translate->y, 0.0F, wp->physics.vel_air.x, wp->physics.vel_air.y, 0.0F);

    return FALSE;
}

// 0x8017FC38
GObj* itLizardonWeaponFlameMakeWeapon(GObj *item_gobj, Vec3f *pos, Vec3f *vel)
{
    GObj *weapon_gobj = wpManagerMakeWeapon(item_gobj, &dITLizardonWeaponFlameWeaponDesc, pos, WEAPON_FLAG_PARENT_ITEM);
    WPStruct *ip;

    if (weapon_gobj == NULL)
    {
        return NULL;
    }
    ip = wpGetStruct(weapon_gobj);

    ip->physics.vel_air = *vel;

    ip->lifetime = ITLIZARDON_FLAME_LIFETIME;

    lbParticleMakePosVel(gITManagerParticleBankID | LBPARTICLE_MASK_GENLINK(0), 2, pos->x, pos->y, 0.0F, ip->physics.vel_air.x, ip->physics.vel_air.y, 0.0F); // This needs to return something in v0 to match
    lbParticleMakePosVel(gITManagerParticleBankID | LBPARTICLE_MASK_GENLINK(0), 0, pos->x, pos->y, 0.0F, ip->physics.vel_air.x, ip->physics.vel_air.y, 0.0F);

    return weapon_gobj;
}

// 0x8017FD2C
void itLizardonAttackMakeFlame(GObj *item_gobj, Vec3f *pos, s32 lr)
{
    ITStruct *ip = itGetStruct(item_gobj);
    Vec3f vel;

    vel.x = __cosf(ITLIZARDON_FLAME_ANGLE) * ITLIZARDON_FLAME_VEL * lr;
    vel.y = __sinf(ITLIZARDON_FLAME_ANGLE) * ITLIZARDON_FLAME_VEL;
    vel.z = 0.0F;

    itLizardonWeaponFlameMakeWeapon(item_gobj, pos, &vel);

    func_800269C0_275C0(nSYAudioFGMLizardonFlame);
}
