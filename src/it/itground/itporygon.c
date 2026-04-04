#include <it/item.h>
#include <gr/ground.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITPorygonItemDesc = 
{
    nITKindPorygon,                         // Item Kind
    &gGRCommonStruct.yamabuki.item_head,    // Pointer to item file data?
    &llGRYamabukiMapPorygonItemAttributes,   // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,            // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateNew,                      // Hitbox Update State
    itPorygonCommonProcUpdate,              // Proc Update
    NULL,                                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80183B10
void itPorygonCommonUpdateMonsterEvent(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    ITMonsterEvent *ev = itGetMonsterEvent(dITPorygonItemDesc, &llGRYamabukiMapPorygonHitParties); // (ITMonsterEvent*) ((uintptr_t)*dITPorygonItemDesc.p_file + (intptr_t)&Porygon_Event);

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

    if (ip->multi == ITPORYGON_SHAKE_STOP_WAIT)
    {
        Vec3f pos = DObjGetStruct(item_gobj)->translate.vec.f;

        pos.y = 0.0F;

        efManagerDustLightMakeEffect(&pos, -1, 1.0F);
    }
}

// 0x80183C84
sb32 itPorygonCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    DObj *dobj = DObjGetStruct(item_gobj);

    dobj->translate.vec.f.x += ip->item_vars.porygon.offset.x;
    dobj->translate.vec.f.y += ip->item_vars.porygon.offset.y;

    itPorygonCommonUpdateMonsterEvent(item_gobj);

    if (dobj->anim_wait == AOBJ_ANIM_NULL)
    {
        grYamabukiGateSetClosedWait();

        return TRUE;
    }
    else return FALSE;
}

// 0x80183D00
GObj* itPorygonMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITPorygonItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        ip->item_vars.porygon.offset = *pos;

        ip->is_allow_knockback = TRUE;

        ip->multi = 0;

        ip->event_id = 0;

        func_800269C0_275C0(nSYAudioVoiceYamabukiPorygon);
    }
    return item_gobj;
}
