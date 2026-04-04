#include <it/item.h>
#include <gr/ground.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITMarumineItemDesc = 
{
    nITKindMarumine,                        // Item Kind
    &gGRCommonStruct.yamabuki.item_head,    // Pointer to item file data?
    &llGRYamabukiMapMarumineItemAttributes,  // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTra,                   // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itMarumineCommonProcUpdate,             // Proc Update
    NULL,                                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITMarumineStatusDescs[/* */] = 
{
    // Status 0 (Neutral Explosion)
    {
        itMarumineExplodeProcUpdate,        // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        NULL                                // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itMarumineStatus
{
    nITMarumineStatusExplode,
    nITMarumineStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801837A0
void itMarumineExplodeMakeEffectGotoSetStatus(GObj *item_gobj)
{
    s32 unused;
    LBParticle *pc;
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    ip->damage_coll.hitstatus = nGMHitStatusNone;

    pc = efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);

    if (pc != NULL)
    {
        pc->xf->scale.x = ITMARUMINE_EXPLODE_EFFECT_SCALE;
        pc->xf->scale.y = ITMARUMINE_EXPLODE_EFFECT_SCALE;
        pc->xf->scale.z = ITMARUMINE_EXPLODE_EFFECT_SCALE;
    }
    efManagerQuakeMakeEffect(1);

    DObjGetStruct(item_gobj)->flags = DOBJ_FLAG_HIDDEN;

    ip->attack_coll.fgm_id = nSYAudioFGMExplodeL;

    itMainRefreshAttackColl(item_gobj);
    itMarumineExplodeSetStatus(item_gobj);
}

// 0x80183830
void itMarumineExplodeUpdateAttackEvent(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    ITAttackEvent *ev = itGetAttackEvent(dITMarumineItemDesc, &llGRYamabukiMapMarumineAttackEvents); // (ITAttackEvent*) ((uintptr_t)*dITMarumineItemDesc.p_file + (intptr_t)&llGRYamabukiMapMarumineAttackEvents);

    if (ip->multi == ev[ip->event_id].timer)
    {
        ip->attack_coll.angle  = ev[ip->event_id].angle;
        ip->attack_coll.damage = ev[ip->event_id].damage;
        ip->attack_coll.size   = ev[ip->event_id].size;

        ip->attack_coll.can_reflect = FALSE;
        ip->attack_coll.can_shield = FALSE;

        ip->attack_coll.element = nGMHitElementFire;

        ip->attack_coll.can_setoff = FALSE;

        ip->event_id++;

        if (ip->event_id == 4)
        {
            ip->event_id = 3;
        }
    }
}

// 0x80183914
sb32 itMarumineCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->translate.vec.f.x += ip->item_vars.marumine.offset.x;
    dobj->translate.vec.f.y += ip->item_vars.marumine.offset.y;

    if (dobj->anim_wait == AOBJ_ANIM_NULL)
    {
        itMainRefreshAttackColl(item_gobj);
        itMainClearOwnerStats(item_gobj);

        ip->item_vars.marumine.offset.x = 0.0F;
        ip->item_vars.marumine.offset.y = 0.0F;

        itMarumineExplodeMakeEffectGotoSetStatus(item_gobj);
        func_800269C0_275C0(nSYAudioFGMExplodeL);
    }
    return FALSE;
}

// 0x801839A8
sb32 itMarumineExplodeProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->translate.vec.f.x += ip->item_vars.marumine.offset.x;
    dobj->translate.vec.f.y += ip->item_vars.marumine.offset.y;

    itMarumineExplodeUpdateAttackEvent(item_gobj);

    ip->multi++;

    if (ip->multi == ITMARUMINE_EXPLODE_LIFETIME)
    {
        grYamabukiGateSetClosedWait();

        return TRUE;
    }
    else return FALSE;
}

// 0x80183A20
void itMarumineExplodeSetStatus(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = 0;

    ip->attack_coll.throw_mul = 1.0F;

    ip->event_id = 0;

    itMarumineExplodeUpdateAttackEvent(item_gobj);
    itMainSetStatus(item_gobj, dITMarumineStatusDescs, nITMarumineStatusExplode);
}

// 0x80183A74
GObj* itMarumineMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITMarumineItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);
        DObj *dobj = DObjGetStruct(item_gobj);

        ip->item_vars.marumine.offset = *pos;

        ip->is_allow_knockback = TRUE;

        gcAddXObjForDObjFixed(dobj, 0x46, 0);
        func_800269C0_275C0(nSYAudioVoiceYamabukiMarumine);
    }
    return item_gobj;
}
