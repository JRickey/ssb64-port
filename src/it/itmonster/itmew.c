#include <it/item.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8018AC40
ITDesc dITMewItemDesc =
{
    nITKindMew,                             // Item Kind
    &gITManagerCommonData,                  // Pointer to item file data?
    &llITCommonDataMewItemAttributes,       // Offset of item attributes in file?

    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyRSca,         // Main matrix transformations
        nGCMatrixKindNull,                  // Secondary matrix transformations?
        0,                                  // ???
    },

    nGMAttackStateOff,                      // Hitbox Update State
    itMewCommonProcUpdate,                  // Proc Update
    itMewCommonProcMap,                     // Proc Map
    NULL,                                   // Proc Hit
    NULL,                                   // Proc Shield
    NULL,                                   // Proc Hop
    NULL,                                   // Proc Set-Off
    NULL,                                   // Proc Reflector
    NULL                                    // Proc Damage
};

// 0x8018AC74
ITStatusDesc dITMewStatusDescs[/* */] =
{
    // Status 0 (Neutral FLy)
    {
        itMewFlyProcUpdate,                 // Proc Update
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

enum itMewStatus
{
    nITMewStatusFly,
    nITMewStatusEnumCount
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8017EBE0
sb32 itMewFlyProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);
    Vec3f pos = DObjGetStruct(item_gobj)->translate.vec.f;

    if (ip->multi == 0)
    {
        return TRUE;
    }
    if (ip->item_vars.mew.esper_gfx_int == 0)
    {
        ip->item_vars.mew.esper_gfx_int = ITMEW_EFFECT_SPAWN_INT;

        efManagerHealSparklesMakeEffect(&pos);
    }
    ip->item_vars.mew.esper_gfx_int--;

    ip->multi--;

    ip->physics.vel_air.y += ITMEW_FLY_ADD_VEL_Y;

    return FALSE;
}

// 0x8017EC84
void itMewFlyInitVars(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    ip->multi = ITMEW_LIFETIME;

    if (syUtilsRandIntRange(2) != 0)
    {
        ip->physics.vel_air.x = ITMEW_STARTVEL_X;
    }
    else ip->physics.vel_air.x = -ITMEW_STARTVEL_X;
    
    ip->physics.vel_air.y = ITMEW_STARTVEL_Y;

    func_800269C0_275C0(nSYAudioFGMMewFly);

    if (ip->kind == nITKindMew)
    {
        func_800269C0_275C0(nSYAudioVoiceMBallMewAppear);
    }
    efManagerRippleMakeEffect(&DObjGetStruct(item_gobj)->translate.vec.f);

    ip->item_vars.mew.esper_gfx_int = 0;
}

// 0x8017ED20
void itMewFlySetStatus(GObj *item_gobj)
{
    itMewFlyInitVars(item_gobj);
    itMainSetStatus(item_gobj, dITMewStatusDescs, nITMewStatusFly);
}

// 0x8017ED54
sb32 itMewCommonProcUpdate(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (ip->multi == 0)
    {
        ip->physics.vel_air.y = 0.0F;

        itMewFlySetStatus(item_gobj);
    }
    ip->multi--;

    return FALSE;
}

// 0x8017EDA4
sb32 itMewCommonProcMap(GObj *item_gobj)
{
    ITStruct *ip = itGetStruct(item_gobj);

    if (itMapTestAllCollisionFlag(item_gobj, MAP_FLAG_FLOOR) != FALSE)
    {
        ip->physics.vel_air.y = 0.0F;
    }
    return FALSE;
}

// 0x8017EDE4
GObj* itMewMakeItem(GObj *parent_gobj, Vec3f *pos, Vec3f *vel, u32 flags)
{
    GObj *item_gobj = itManagerMakeItem(parent_gobj, &dITMewItemDesc, pos, vel, flags);

    if (item_gobj != NULL)
    {
        DObj *dobj = DObjGetStruct(item_gobj);
        ITStruct *ip = itGetStruct(item_gobj);

        ip->multi = ITMONSTER_RISE_STOP_WAIT;

        ip->physics.vel_air.x = ip->physics.vel_air.z = 0.0F;
        ip->physics.vel_air.y = ITMONSTER_RISE_VEL_Y; // Starting to think this is a macro

        dobj->translate.vec.f.y -= ip->attr->map_coll_bottom;
            
        // This ptr stuff is likely also a macro
        gcAddDObjAnimJoint(dobj, itGetMonsterAnimNode(ip, &llITCommonDataMewDataStart), 0.0F);
    }
    return item_gobj;
}
