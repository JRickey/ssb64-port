#include <it/item.h>
#include <gr/ground.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

ITDesc dITPowerBlockItemDesc =
{
    nITKindPowerBlock,                      // Item Kind
    &gGRCommonStruct.inishie.item_head,     // Pointer to item file data?
    &llGRInishieMapPowerBlockItemAttributes,// Offset of item attributes in file?

    // DObj transformation struct
    {
        0x44,                               // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0                                   // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itPowerBlockCommonProcUpdate,           // Proc Update
    NULL,                                   // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

ITStatusDesc dITPowerBlockStatusDescs[/* */] =
{
    // Status 0 (Neutral Wait)
    {
        NULL,                               // Proc Update
        NULL,                               // Proc Map
        NULL,                               // Proc Hit
        NULL,                               // Proc Shield
        NULL,                               // Proc Hop
        NULL,                               // Proc Set-Off
        NULL,                               // Proc Reflector
        itPowerBlockWaitProcDamage          // Proc Damage
    }
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum itPowerBlockStatus
{
    nITPowerBlockStatusWait,
    nITPowerBlockStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017C090
sb32 itPowerBlockCommonProcUpdate(GObj *item_gobj)
{
    if (DObjGetStruct(item_gobj)->anim_wait == AOBJ_ANIM_NULL)
    {
        itPowerBlockWaitSetStatus(item_gobj);
    }
    return FALSE;
}

// 0x8017C0D4
void itPowerBlockWaitSetStatus(GObj *item_gobj)
{
    ITStruct *ip;

    itMainSetStatus(item_gobj, dITPowerBlockStatusDescs, nITPowerBlockStatusWait);

    ip = itGetStruct(item_gobj), ip->damage_coll.hitstatus = nGMHitStatusNormal;
}

// 0x8017C110
sb32 itPowerBlockNDamageProcUpdate(GObj *item_gobj)
{
    if (DObjGetStruct(item_gobj)->anim_wait == AOBJ_ANIM_NULL)
    {
        grInishiePowerBlockSetWait();

        return TRUE;
    }
    else return FALSE;
}

// 0x8017C15C
sb32 itPowerBlockWaitProcDamage(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->proc_update = itPowerBlockNDamageProcUpdate;
    ip->damage_coll.hitstatus = nGMHitStatusNone;

    gcAddDObjAnimJoint(DObjGetStruct(item_gobj), itGetPData(ip, &llGRInishieMapPowerBlockDataStart, &llGRInishieMapPowerBlockAnimJoint), 0.0F);
    gcPlayAnimAll(item_gobj);
    func_800269C0_275C0(nSYAudioFGMInishiePowerBlock);
    efManagerQuakeMakeEffect(3);
    grInishiePowerBlockSetDamage();

    return FALSE;
}

// 0x8017C1E0
GObj* itPowerBlockMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITPowerBlockItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        ip->damage_coll.interact_mask = GMHITCOLLISION_FLAG_FIGHTER;
    }
    return item_gobj;
}
