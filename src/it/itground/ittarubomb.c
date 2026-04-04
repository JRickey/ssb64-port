#include <it/item.h>
#include <gr/ground.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern f32 syUtilsRandFloat();
extern f32 syUtilsArcTan2(f32, f32);

extern alSoundEffect *func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITTaruBombItemDesc =
{
    nITKindTaruBomb,                            // Item Kind
    &gGRCommonStruct.bonus3.item_head,          // Pointer to item file data?
    &llGRBonus3MapTaruBombItemAttributes,       // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,                 // Main matrix transformations
        nGCMatrixKindNull,                       // Secondary matrix transformations?
        0                                        // ???
    },

    nGMAttackStateNew,                          // Hitbox Update State
    itTaruBombFallProcUpdate,                   // Proc Update
    itTaruBombFallProcMap,                      // Proc Map
    itTaruBombCommonProcHit,                    // Proc Hit
    itTaruBombCommonProcHit,                    // Proc Shield
    NULL,                                       // Proc Hop
    itTaruBombCommonProcHit,                    // Proc Set-Off
    itTaruBombCommonProcHit,                    // Proc Reflector
    itTaruBombCommonProcDamage                  // Proc Damage
};

ITStatusDesc dITTaruBombStatusDescs[/* */] =
{
    // Status 0 (Air Wait Fall)
    {
        itTaruBombFallProcUpdate,               // Proc Update
        itTaruBombFallProcMap,                  // Proc Map
        itTaruBombCommonProcHit,                // Proc Hit
        itTaruBombCommonProcHit,                // Proc Shield
        NULL,                                   // Proc Hop
        itTaruBombCommonProcHit,                // Proc Set-Off
        itTaruBombCommonProcHit,                // Proc Reflector
        itTaruBombCommonProcDamage              // Proc Damage
    },

    // Status 1 (Neutral Explosion)
    {
        itTaruBombExplodeProcUpdate,            // Proc Update
        NULL,                                   // Proc Map
        NULL,                                   // Proc Hit
        NULL,                                   // Proc Shield
        NULL,                                   // Proc Hop
        NULL,                                   // Proc Set-Off
        NULL,                                   // Proc Reflector
        NULL                                    // Proc Damage
    },

    // Status 2 (Ground Roll)
    {
        itTaruBombRollProcUpdate,               // Proc Update
        itTaruBombRollProcMap,                  // Proc Map
        itTaruBombCommonProcHit,                // Proc Hit
        itTaruBombCommonProcHit,                // Proc Shield
        NULL,                                   // Proc Hop
        itTaruBombCommonProcHit,                // Proc Set-Off
        itTaruBombCommonProcHit,                // Proc Reflector
        itTaruBombCommonProcDamage              // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itTaruBombStatus
{
    nITTaruBombStatusFall,
    nITTaruBombStatusExplode,
    nITTaruBombStatusRoll,
    nITTaruBombStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80184A70
void itTaruBombContainerSmashUpdateEffect(GObj *effect_gobj) // RTTF bomb explode GFX process
{
    EFStruct *ep = efGetStruct(effect_gobj);
    DObj *dobj = DObjGetStruct(effect_gobj);

    ep->effect_vars.container.lifetime--;

    if (ep->effect_vars.container.lifetime == 0)
    {
        efManagerSetPrevStructAlloc(ep);
        gcEjectGObj(effect_gobj);
    }
    else while (dobj != NULL)
    {
        dobj->scale.vec.f.y -= 1.3F;

        dobj->translate.vec.f.x += dobj->scale.vec.f.x; // This makes no sense, seems this custom effect is very... custom
        dobj->translate.vec.f.y += dobj->scale.vec.f.y;
        dobj->translate.vec.f.z += dobj->scale.vec.f.z;

        dobj->rotate.vec.f.x += dobj->anim_wait; // ??? Seems to be rotation step, but only in this case? Otherwise -FLOAT32_MAX?
        dobj->rotate.vec.f.y += dobj->anim_speed;
        dobj->rotate.vec.f.z += dobj->anim_frame;

        dobj = dobj->sib_next;
    }
}

// 0x80184B44
void itTaruBombContainerSmashMakeEffect(Vec3f *pos)
{
    GObj *effect_gobj;
    EFStruct *ep = efManagerGetEffectNoForce();
    DObj *dobj;
    s32 i;
    Gfx *dl;

    if (ep != NULL)
    {
        effect_gobj = gcMakeGObjSPAfter(nGCCommonKindEffect, NULL, nGCCommonLinkIDEffect, GOBJ_PRIORITY_DEFAULT);

        if (effect_gobj != NULL)
        {
            gcAddGObjDisplay(effect_gobj, gcDrawDObjTreeForGObj, 11, GOBJ_PRIORITY_DEFAULT, ~0);

            dl = (Gfx*) ((*(uintptr_t*)((uintptr_t)*dITTaruBombItemDesc.p_file + dITTaruBombItemDesc.o_attributes) - (uintptr_t)&llGRBonus3MapTaruBombDataStart) + (uintptr_t)&llGRBonus3MapTaruBombEffectDisplayList);

            for (i = 0; i < ITTARUBOMB_EFFECT_COUNT; i++)
            {
                dobj = gcAddDObjForGObj(effect_gobj, dl);

                gcAddXObjForDObjFixed(dobj, nGCMatrixKindTraRotRpyR, 0);

                dobj->translate.vec.f = *pos;

                dobj->scale.vec.f.x = (syUtilsRandFloat() * 48.0F) + -24.0F;
                dobj->scale.vec.f.y = (syUtilsRandFloat() * 50.0F) + 10.0F;
                dobj->scale.vec.f.z = (syUtilsRandFloat() * 32.0F) + -16.0F;

                dobj->anim_wait = F_CLC_DTOR32((syUtilsRandFloat() * 100.0F) + -50.0F);
                dobj->anim_speed = F_CLC_DTOR32((syUtilsRandFloat() * 100.0F) + -50.0F);
                dobj->anim_frame = F_CLC_DTOR32((syUtilsRandFloat() * 100.0F) + -50.0F);
            }
            ep->effect_vars.container.lifetime = ITTARUBOMB_GFX_LIFETIME;

            effect_gobj->user_data.p = ep;

            gcAddGObjProcess(effect_gobj, itTaruBombContainerSmashUpdateEffect, 1, 3);
        }
    }
}

// 0x80184D74
sb32 itTaruBombFallProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj;

    itMainApplyGravityClampTVel(ip, ITTARUBOMB_GRAVITY, ITTARUBOMB_TVEL);

    dobj = DObjGetStruct(item_gobj);
    dobj->rotate.vec.f.z += ip->item_vars.tarubomb.roll_rotate_step;

    return FALSE;
}

// 0x80184DC4
sb32 itTaruBombCommonProcHit(GObj *item_gobj)
{
    func_800269C0_275C0(nSYAudioFGMTaruBombHit);
    itTaruBombContainerSmashMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);
    itTaruBombExplodeMakeEffectGotoSetStatus(item_gobj);

    return FALSE;
}

// 0x80184E04
sb32 itTaruBombCommonProcDamage(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->percent_damage >= ITTARUBOMB_HEALTH_MAX)
    {
        return itTaruBombCommonProcHit(item_gobj);
    }
    return FALSE;
} 

// 0x80184E44
void itTaruBombRollSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->physics.vel_air.y = 0.0F;

    itMainSetStatus(item_gobj, dITTaruBombStatusDescs, nITTaruBombStatusRoll);
}

// 0x80184E78
sb32 itTaruBombFallCheckCollideGround(GObj *item_gobj, f32 common_rebound)
{
    s32 unused;
    ITStruct *ip;
    sb32 is_collide_floor = itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR);

    if (itMapCheckCollideAllRebound(item_gobj, (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL), common_rebound, NULL) != FALSE)
    {
        itMainSetSpinVelLR(item_gobj);
    }
    if (is_collide_floor != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80184EDC
sb32 itTaruBombFallProcMap(GObj *item_gobj)
{
    if (itTaruBombFallCheckCollideGround(item_gobj, ITTARUBOMB_MAP_REBOUND_COMMON) != FALSE)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if (ip->physics.vel_air.y >= 90.0F) // Is it even possible to meet this condition? Didn't they mean <= inverse of this value?
        {
            itTaruBombCommonProcHit(item_gobj); // This causes the bomb to smash on impact when landing from too high; doesn't seem possible to trigger

            return TRUE;
        }
        else if (ip->physics.vel_air.y < 30.0F)
        {
            itTaruBombRollSetStatus(item_gobj);
        }
        else
        {
            lbCommonReflect2D(&ip->physics.vel_air, &ip->coll_data.floor_angle);

            ip->physics.vel_air.y *= 0.2F;

            itMainSetSpinVelLR(item_gobj);
        }
        func_800269C0_275C0(nSYAudioFGMTaruBombMap);
        itMainClearOwnerStats(item_gobj);
    }
    return FALSE;
}


// 0x80184FAC
void itTaruBombCommonSetMapCollisionBox(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    DObjGetStruct(item_gobj)->rotate.vec.f.x = F_CLC_DTOR32(90.0F);

    ip->coll_data.map_coll.top = ip->coll_data.map_coll.width;
    ip->coll_data.map_coll.bottom = -ip->coll_data.map_coll.width;
}

// 0x80184FD4
sb32 itTaruBombExplodeProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi++;

    if (ip->multi == ITTARUBOMB_EXPLODE_LIFETIME)
    {
        return TRUE;
    }
    else itMainUpdateAttackEvent(item_gobj, itGetAttackEvent(dITTaruBombItemDesc, &llGRBonus3MapTaruBombAttackEvents));

    return FALSE;
}

// 0x80185030
sb32 itTaruBombRollProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    f32 roll_rotate_step;
    f32 sqrt_vel;

    ip->physics.vel_air.x += (-(syUtilsArcTan2(ip->coll_data.floor_angle.y, ip->coll_data.floor_angle.x) - F_CLC_DTOR32(90.0F) /*HALF_PI32*/) * ITTARUBOMB_MUL_VEL_X);

    ip->lr = (ip->physics.vel_air.x >= 0.0F) ? +1 : -1;

    sqrt_vel = sqrtf(SQUARE(ip->physics.vel_air.x) + SQUARE(ip->physics.vel_air.y));

    roll_rotate_step = ((ip->lr == -1) ? ITTARUBOMB_ROLL_ROTATE_MUL : -ITTARUBOMB_ROLL_ROTATE_MUL) * sqrt_vel;

    ip->item_vars.tarubomb.roll_rotate_step = roll_rotate_step;

    DObjGetStruct(item_gobj)->rotate.vec.f.z += roll_rotate_step;

    return FALSE;
}

// 0x8018511C
sb32 itTaruBombRollProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestLRWallCheckFloor(item_gobj) == FALSE)
    {
        itMainSetStatus(item_gobj, dITTaruBombStatusDescs, nITTaruBombStatusFall);
    }
    else if (ip->coll_data.mask_curr & (MAP_FLAG_RWALL | MAP_FLAG_LWALL))
    {
        return itTaruBombCommonProcHit(item_gobj);
    }
    return FALSE;
}

// 0x8018518C
GObj* itTaruBombMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITTaruBombItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        ip->item_vars.tarubomb.roll_rotate_step = 0.0F;

        itTaruBombCommonSetMapCollisionBox(item_gobj);
    }
    return item_gobj;
}

// 0x801851F4
void itTaruBombExplodeInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = 0;
    ip->event_id = 0;

    ip->attack_coll.fgm_id = nSYAudioFGMExplodeL;

    ip->attack_coll.can_rehit_item = TRUE;
    ip->attack_coll.can_reflect = FALSE;

    ip->attack_coll.throw_mul = ITEM_THROW_DEFAULT;
    ip->attack_coll.element = nGMHitElementFire;

    ip->attack_coll.can_setoff = FALSE;

    ip->damage_coll.hitstatus = nGMHitStatusNone;

    itMainRefreshAttackColl(item_gobj);
    itMainUpdateAttackEvent(item_gobj, itGetAttackEvent(dITTaruBombItemDesc, &llGRBonus3MapTaruBombAttackEvents));
}

// 0x80185284
void itTaruBombExplodeSetStatus(GObj *item_gobj)
{
    itTaruBombExplodeInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITTaruBombStatusDescs, nITTaruBombStatusExplode);
}

// 0x801852B8
void itTaruBombExplodeMakeEffectGotoSetStatus(GObj *item_gobj)
{
    LBParticle *pc;
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    ip->attack_coll.attack_state = nGMAttackStateOff;

    ip->physics.vel_air.x = 0.0F;
    ip->physics.vel_air.y = 0.0F;
    ip->physics.vel_air.z = 0.0F;

    pc = efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);

    if (pc != NULL)
    {
        pc->xf->scale.x =
        pc->xf->scale.y =
        pc->xf->scale.z = ITTARUBOMB_EXPLODE_EFFECT_SCALE;
    }
    efManagerQuakeMakeEffect(1);

    DObjGetStruct(item_gobj)->flags = DOBJ_FLAG_HIDDEN;

    itTaruBombExplodeSetStatus(item_gobj);
}
