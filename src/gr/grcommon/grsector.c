#include <gr/ground.h>
#include <ft/fighter.h>
#include <wp/weapon.h>
#include <sc/scene.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern void syInterpQuad(void*, void*, f32);
extern void syInterpCubic(void*, void*, f32);

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012E940
intptr_t dGRSectorArwingSectorDescs[/* */] =
{
    &llGRSectorMapArwing0SectorDesc,
    &llGRSectorMapArwing1SectorDesc,
    &llGRSectorMapArwing2SectorDesc,
    &llGRSectorMapArwing3SectorDesc,
    &llGRSectorMapArwing4SectorDesc,
    &llGRSectorMapArwing5SectorDesc,
    &llGRSectorMapArwing6SectorDesc,
    &llGRSectorMapArwing7SectorDesc
};

// 0x8012E960
intptr_t dGRSectorArwingAnimJoints[/* */] =
{
    &llGRSectorMapArwing0AnimJoint,
    &llGRSectorMapArwing1AnimJoint,
    &llGRSectorMapArwing2AnimJoint,
    &llGRSectorMapArwing3AnimJoint,
    &llGRSectorMapArwing4AnimJoint,
    &llGRSectorMapArwing5AnimJoint
};

// 0x8012E978
s16 dGRSectorArwingMapPositionsX[/* */] =
{
    -3000,
        0,
     9000
}; // Arwing collision vertex / spawn positions? There should be another 0x0000, but the next array is u8, does it still get padded?

// 0x8012E980
u8 dGRSectorArwingLaserCounts[/* */] =
{
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x00,
    0x00,
    0x00
};

// 0x8012E988
u8 dGRSectorArwingPilotIDs[/* */] =
{
    0x01,
    0x01,
    0x02,
    0x03,
    0x04,
    0x04,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x02,
    0x03,
    0x04,
    0x04,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x03,
    0x04,
    0x04,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x02,
    0x04,
    0x04,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x02,
    0x03,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x02,
    0x03,
    0x04,
    0x04,
    0x00
};

// 0x8012E9C0
u8 dGRSectorArwingPilotWaitTimers[/* */][2] =
{
    {  0,  7 },
    {  7,  9 },
    { 16, 10 },
    { 26, 10 },
    { 36,  9 },
    { 45, 10 }
};

// 0x8012E9CC
DObjTransformTypes dGRSectorArwingTransformKinds[/* */] =
{
#if defined(REGION_US)
    { 0x53, nGCMatrixKindNull, 0x00 },
#else
    { 0x52, nGCMatrixKindNull, 0x00 },
#endif
    { nGCMatrixKindTraRotRpyR, nGCMatrixKindNull, 0x00 },
    { nGCMatrixKindTra, 0x2C, 0x01 },
    { nGCMatrixKindTra, 0x2C, 0x01 },
    { nGCMatrixKindTra, 0x2C, 0x00 },
    { nGCMatrixKindTra, 0x2C, 0x00 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x01 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x00 },
    { nGCMatrixKindTra, 0x2C, 0x00 },
    { nGCMatrixKindTra, nGCMatrixKindNull, 0x00 },
    { nGCMatrixKindTra, 0x2C, 0x01 },
    { nGCMatrixKindNull, nGCMatrixKindNull, 0x00 } // This might just be padding
};

// 0x8012E9F0
WPDesc dGRSectorArwingWeaponLaser2DWeaponDesc =
{
    0,                                          // Render flags?
    nWPKindArwingLaser2D,                       // Weapon Kind
    &gGRCommonStruct.sector.weapon_head,        // Pointer to character's loaded files?
    &llGRSectorMapArwingLaser2DWeaponAttributes,    // Offset of weapon attributes in loaded files
    
    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,                // Main matrix transformations
        nGCMatrixKindNull,                      // Secondary matrix transformations?
        0                                       // ???
    },

    NULL,                                       // Proc Update
    grSectorArwingWeaponLaser2DProcMap,         // Proc Map
    grSectorArwingWeaponLaser2DProcHit,         // Proc Hit
    grSectorArwingWeaponLaser2DProcHit,         // Proc Shield
    grSectorArwingWeaponLaser2DProcHop,         // Proc Hop
    grSectorArwingWeaponLaser2DProcHit,         // Proc Set-Off
    grSectorArwingWeaponLaser2DProcReflector,   // Proc Reflector
    grSectorArwingWeaponLaser2DProcHit          // Proc Absorb
};

// 0x8012EA24
WPDesc dGRSectorArwingWeaponLaser3DWeaponDesc =
{
    0,                                          // Render flags?
    nWPKindArwingLaser3D,                       // Weapon Kind
    &gGRCommonStruct.sector.weapon_head,        // Pointer to character's loaded files?
    &llGRSectorMapArwingLaser3DWeaponAttributes,    // Offset of weapon attributes in loaded files
    
    // DObj transformation struct
    {
        nGCMatrixKindTraRotRpyR,                // Main matrix transformations
        nGCMatrixKindNull,                      // Secondary matrix transformations?
        0                                       // ???
    },

    NULL,                                       // Proc Update
    grSectorArwingWeaponLaser3DProcMap,         // Proc Map
    grSectorArwingWeaponLaser3DProcHit,         // Proc Hit
    grSectorArwingWeaponLaser3DProcHit,         // Proc Shield
    grSectorArwingWeaponLaser3DProcHit,         // Proc Hop
    grSectorArwingWeaponLaser3DProcHit,         // Proc Set-Off
    grSectorArwingWeaponLaser3DProcHit,         // Proc Reflector
    grSectorArwingWeaponLaser3DProcAbsorb       // Proc Absorb
};

// // // // // // // // // // // //
//                               //
//          ENUMERATORS          //
//                               //
// // // // // // // // // // // //

enum grSectorArwingStatus
{
    nGRSectorArwingStatusSleep,
    nGRSectorArwingStatusWait,
    nGRSectorArwingStatusPatrol
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80106730
void func_ovl2_80106730(DObj *arg0, Vec3f *vec1, Vec3f *vec2, Vec3f *vec3)
{
    DObj *sp54 = gGRCommonStruct.sector.map_dobjs[11];
    AObj *aobj = arg0->aobj;
    f32 vlen;

    while (aobj != NULL)
    {
        if ((aobj->kind != nGCAnimKindNone) && !(arg0->parent_gobj->flags & GOBJ_FLAG_NOANIM) && (aobj->track == nGCAnimTrackTraI))
        {
            vlen = gcGetAObjValue(aobj);

            if (vlen < 0.0F)
            {
                vlen = 0.0F;
            }
            else if (vlen > 1.0F)
            {
                vlen = 1.0F;
            }
            syInterpQuad(vec1, aobj->interpolate, vlen);
        }
        aobj = aobj->next;
    }
    if ((sp54->anim_wait != AOBJ_ANIM_NULL) && (gGRCommonStruct.sector.arwing_laser_count == 0))
    {
        aobj = sp54->aobj;

        while (aobj != NULL)
        {
            if ((aobj->kind != nGCAnimKindNone) && !(arg0->parent_gobj->flags & GOBJ_FLAG_NOANIM) && (aobj->track == nGCAnimTrackTraI))
            {
                syInterpCubic(vec3, aobj->interpolate, vlen);
            }
            aobj = aobj->next;
        }
        syVectorNorm3D(vec3);
    }
    lbCommonCross3D(vec3, vec1, vec2);
    lbCommonCross3D(vec1, vec2, vec3);
    
    syVectorNorm3D(vec1);
    syVectorNorm3D(vec2);
    syVectorNorm3D(vec3);
}

// 0x80106904
sb32 grSectorArwingLaser3DFuncMatrix(Mtx *mtx, DObj *dobj, Gfx **dls)
{
    f32 sx;
    Vec3f sp80;
    Vec3f sp74;
    Vec3f sp68;
    Mtx44f f;
    f32 tx;
    f32 ty;
    f32 tz;

    tx = dobj->translate.vec.f.x;
    ty = dobj->translate.vec.f.y;
    tz = dobj->translate.vec.f.z;

    sp80.x = -1.0F;
    sp80.y = 0.0F;
    sp80.z = 0.0F;
    sp68.x = 0.0F;
    sp68.y = 1.0F;
    sp68.z = 0.0F;

    if (gGRCommonStruct.sector.arwing_laser_count == 2)
    {
        sp74.x = sp74.y = 0.0F;
        sp74.z = 1;
    }
    else func_ovl2_80106730(dobj, &sp80, &sp74, &sp68);

    f[0][0] = sp74.x; // sp28
    f[0][1] = sp74.y; // sp2C
    f[0][2] = sp74.z; // sp30
    f[1][0] = sp68.x; // sp38
    f[1][1] = sp68.y; // sp3C
    f[1][2] = sp68.z; // sp40
    f[2][0] = sp80.x; // sp48
    f[2][1] = sp80.y; // sp4C
    f[2][2] = sp80.z; // sp50

    f[0][3] = f[1][3] = f[2][3] = 0.0F;                     // sp34, sp44, sp54

    f[3][0] = tx + gGRCommonStruct.sector.arwing_target_x;  // sp58
    f[3][1] = ty;                                           // sp5C
    f[3][2] = tz;                                           // sp60

    f[3][3] = 1.0F;                                         // sp64

    guMtxF2L(f, mtx);

    return 0;
}

// 0x80106A40
void grSectorArwingAddAnim(DObj *dobj, AObjEvent32 *anim_joint, f32 unused)
{
    if (anim_joint != NULL)
    {
        gcAddDObjAnimJoint(dobj, anim_joint, 0.0F);

        dobj->is_anim_root = FALSE;

        gcParseDObjAnimJoint(dobj);
        gcPlayDObjAnimJoint(dobj);
    }
    else
    {
        dobj->anim_wait = AOBJ_ANIM_NULL;
        dobj->is_anim_root = FALSE;
    }
}

// 0x80106A98
void grSectorArwingUpdateSleep(void)
{
    if (gSCManagerBattleState->game_status != nSCBattleGameStatusWait)
    {
        gGRCommonStruct.sector.arwing_status = nGRSectorArwingStatusWait;
    }
}

// 0x80106AC0
void grSectorArwingUpdateWait(void)
{
    s32 random;

    if (gGRCommonStruct.sector.arwing_appear_timer != 0)
    {
        gGRCommonStruct.sector.arwing_appear_timer--;
    }
    else
    {
        gGRCommonStruct.sector.arwing_target_x = 0.0F;

        if (gGRCommonStruct.sector.arwing_type_cycle != 0)
        {
            random = syUtilsRandIntRange(5);

            if (random == 4)
            {
                gGRCommonStruct.sector.arwing_target_x = dGRSectorArwingMapPositionsX[syUtilsRandIntRange(ARRAY_COUNT(dGRSectorArwingMapPositionsX))];
            }
            gGRCommonStruct.sector.arwing_type_cycle--;
            gGRCommonStruct.sector.arwing_state_timer = syUtilsRandIntRange(540) + 180;
            gGRCommonStruct.sector.arwing_pilot_curr = -1;
        }
        else
        {
            random = syUtilsRandIntRange(3) + 5;

            gGRCommonStruct.sector.unk_sector_0x4C = ((syUtilsRandUShort() % 2) != 0) ? random - 5 : -1;

            gGRCommonStruct.sector.arwing_type_cycle = 3;
            gGRCommonStruct.sector.arwing_pilot_curr = -2;
        }
        gGRCommonStruct.sector.arwing_flight_pattern = random;
        gGRCommonStruct.sector.arwing_status = 2;
        gGRCommonStruct.sector.unk_sector_0x4E = 0x3C;

        gGRCommonStruct.sector.map_dobjs[1]->translate.vec.f.x =
        gGRCommonStruct.sector.map_dobjs[1]->translate.vec.f.y =
        gGRCommonStruct.sector.map_dobjs[1]->translate.vec.f.z = 0.0F;

        gGRCommonStruct.sector.map_dobjs[1]->rotate.vec.f.x =
        gGRCommonStruct.sector.map_dobjs[1]->rotate.vec.f.y =
        gGRCommonStruct.sector.map_dobjs[1]->rotate.vec.f.z = 0.0F;

        gGRCommonStruct.sector.is_arwing_line_collision = FALSE;
        gGRCommonStruct.sector.is_arwing_line_active = TRUE;
        gGRCommonStruct.sector.is_arwing_z_collision = FALSE;
        gGRCommonStruct.sector.arwing_laser_ammo = 0;

        func_800269C0_275C0(nSYAudioFGMSectorAmbient1);
    }
}

// 0x80106C28
void grSectorArwingDecideZNear(void)
{
    if (ABSF(gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.z) < 200.0F)
    {
        gGRCommonStruct.sector.is_arwing_z_near = TRUE;
    }
    else gGRCommonStruct.sector.is_arwing_z_near = FALSE;
}

// 0x80106C88
void func_ovl2_80106C88(void)
{
    switch (gGRCommonStruct.sector.arwing_state_timer)
    {
    case 0:
        gGRCommonStruct.sector.is_arwing_line_active = FALSE;
        break;

    case 88:
        gGRCommonStruct.sector.is_arwing_line_active = TRUE;
        break;
    }
}

// 0x80106CC4
void func_ovl2_80106CC4(void)
{
    switch (gGRCommonStruct.sector.arwing_state_timer)
    {
    case 0:
        gGRCommonStruct.sector.is_arwing_line_active = FALSE;
        break;

    case 178:
        gGRCommonStruct.sector.is_arwing_line_active = TRUE;
        break;
    }
}

// 0x80106D00
void func_ovl2_80106D00(void)
{
    if (gGRCommonStruct.sector.arwing_state_timer == 0)
    {
        gGRCommonStruct.sector.map_dobjs[7]->anim_wait = AOBJ_ANIM_NULL;
        gGRCommonStruct.sector.map_dobjs[7]->flags = DOBJ_FLAG_NONE;
        gGRCommonStruct.sector.map_dobjs[9]->anim_wait = AOBJ_ANIM_NULL;
        gGRCommonStruct.sector.map_dobjs[9]->flags = DOBJ_FLAG_HIDDEN;

        grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[8], lbRelocGetFileData(AObjEvent32*, gGRCommonStruct.sector.map_file, &llFoxSpecial3_2EB4_AnimJoint), 0.0F);
    }
    else if (gGRCommonStruct.sector.map_dobjs[8]->anim_wait == AOBJ_ANIM_NULL)
    {
        gGRCommonStruct.sector.map_dobjs[7]->flags = DOBJ_FLAG_HIDDEN;
        gGRCommonStruct.sector.map_dobjs[9]->flags = DOBJ_FLAG_NONE;
    }
    if (gGRCommonStruct.sector.map_dobjs[1]->anim_wait == AOBJ_ANIM_NULL)
    {
        gGRCommonStruct.sector.map_dobjs[0]->anim_wait = AOBJ_ANIM_NULL;
    }
}

// 0x80106DD8
void func_ovl2_80106DD8(void)
{
    if (gGRCommonStruct.sector.arwing_pilot_curr != -2)
    {
        if (gGRCommonStruct.sector.arwing_pilot_curr >= 0)
        {
            switch (gGRCommonStruct.sector.arwing_pilot_curr)
            {
            case 1:
                func_ovl2_80106C88();
                break;

            case 4:
                func_ovl2_80106CC4();
                break;

            case 5:
                func_ovl2_80106D00();
                break;
            }
            if (gGRCommonStruct.sector.map_dobjs[1]->anim_wait == AOBJ_ANIM_NULL)
            {
                gGRCommonStruct.sector.arwing_pilot_curr = -1;
                gGRCommonStruct.sector.arwing_state_timer = 120;
            }
            else gGRCommonStruct.sector.arwing_state_timer++;
        }
        else
        {
            gGRCommonStruct.sector.arwing_state_timer--;

            if (gGRCommonStruct.sector.arwing_state_timer == 0)
            {
                u8 *random = &dGRSectorArwingPilotWaitTimers[gGRCommonStruct.sector.arwing_pilot_prev][0];
                s32 pilot_id = dGRSectorArwingPilotIDs[random[0] + syUtilsRandIntRange(random[1])];

                if (pilot_id != 0)
                {
                    grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[1], (AObjEvent32*) ((intptr_t)dGRSectorArwingAnimJoints[pilot_id] + (uintptr_t)gGRCommonStruct.sector.map_head), 0.0F);
                }
                gGRCommonStruct.sector.arwing_pilot_prev = gGRCommonStruct.sector.arwing_pilot_curr = pilot_id;
            }
        }
    }
}

// 0x80106F2
s32 grSectorArwingPrepareLaserCount(void)
{
    if (syUtilsRandIntRange(3) >= 3)
    {
        return 2;
    }
    else return 4;
}

// 0x80106F5C
s32 grSectorArwingGetLaserAmmoCount(void)
{
    GObj *fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    f32 pos_x = gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.x + gGRCommonStruct.sector.arwing_target_x;
    f32 pos_y = gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.y + gGRCommonStruct.sector.map_dobjs[1]->translate.vec.f.y;

    while (fighter_gobj != NULL)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (gGRCommonStruct.sector.arwing_laser_count == 2)
        {
            DObj *joint = fp->joints[nFTPartsJointTopN];

            if (joint->translate.vec.f.x < pos_x)
            {
                if ((joint->translate.vec.f.y < (pos_y + 300.0F)) && (joint->translate.vec.f.y > (pos_y + -500.0F)))
                {
                    return grSectorArwingPrepareLaserCount();
                }
            }
        }
        fighter_gobj = fighter_gobj->link_next;
    }
    return 0;
}

// 0x80107030
sb32 grSectorArwingWeaponLaser2DProcMap(GObj *weapon_gobj)
{
    if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
    {
        efManagerDustExpandSmallMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, 1.0F);

        return TRUE;
    }
    else return FALSE;
}

// 0x80107074
sb32 grSectorArwingWeaponLaser2DProcHit(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    efManagerImpactShockMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f, wp->attack_coll.damage);

    return TRUE;
}

// 0x801070A4
void func_ovl2_801070A4(Vec3f *rotate, Vec3f *direction, Vec3f *vec3, Vec3f *vec4)
{
    if ((vec3->z == -1.0F) || (vec3->z == 1.0F))
    {
        if (vec3->z == -1.0F)
        {
            rotate->y = F_CST_DTOR32(90.0F);
            rotate->x = syUtilsArcTan2(vec4->x, vec4->y);
        }
        else
        {
            rotate->y = F_CST_DTOR32(-90.0F);
            rotate->x = syUtilsArcTan2(-vec4->x, vec4->y);
        }
        rotate->z = 0.0F;
    }
    else
    {
        rotate->y = syUtilsArcSin(-vec3->z);
        rotate->x = syUtilsArcTan2(vec4->z, direction->z);
        rotate->z = syUtilsArcTan2(vec3->y, vec3->x);
    }
}

// 0x8010719C
void func_ovl2_8010719C(Vec3f *vel, Vec3f *rotate)
{
    Vec3f sp2C;
    Vec3f sp20;
    f32 unused;
    f32 rot_z;

    sp20.x = 0.0F;

    rot_z = gGRCommonStruct.sector.map_dobjs[1]->rotate.vec.f.z + F_CST_DTOR32(90.0F);

    sp20.y = __sinf(rot_z);
    sp20.z = __cosf(rot_z);

    lbCommonCross3D(&sp20, vel, &sp2C);
    lbCommonCross3D(vel, &sp2C, &sp20);
    syVectorNorm3D(&sp2C);
    syVectorNorm3D(&sp20);
    func_ovl2_801070A4(rotate, vel, &sp2C, &sp20);
}

// 0x80107238
sb32 grSectorArwingWeaponLaser2DProcHop(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f vel;

    syVectorRotateAbout3D(&wp->physics.vel_air, &wp->shield_collide_dir, wp->shield_collide_angle * 2);

    vel = wp->physics.vel_air;

    syVectorNorm3D(&vel);
    func_ovl2_8010719C(&vel, &DObjGetStruct(weapon_gobj)->rotate.vec.f);

    return FALSE;
}

// 0x801072C0
sb32 grSectorArwingWeaponLaser2DProcReflector(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    FTStruct *fp = ftGetStruct(wp->owner_gobj);
    Vec3f vel;

    wpMainReflectorSetLR(wp, fp);

    vel = wp->physics.vel_air;

    syVectorNorm3D(&vel);
    func_ovl2_8010719C(&vel, &DObjGetStruct(weapon_gobj)->rotate.vec.f);

    return FALSE;
}

// 0x80107330
void grSectorArwingWeaponLaser2DMakeWeapon(void)
{
    GObj *weapon_gobj;
    WPStruct *wp;
    Vec3f sp54;
    Vec3f sp48;
    Vec3f pos;
    Vec3f rotate;
    Vec3f vel;
    f32 zero = 0.0F;

    sp54.x = gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.x + gGRCommonStruct.sector.arwing_target_x;
    sp54.y = gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.y + gGRCommonStruct.sector.map_dobjs[1]->translate.vec.f.y;

    sp48 = gGRCommonStruct.sector.map_dobjs[2]->translate.vec.f;

    syVectorRotate3D(&sp48, SYVECTOR_AXIS_Z, gGRCommonStruct.sector.map_dobjs[1]->rotate.vec.f.z);

    pos.x = (sp54.x - sp48.z) - 566.0F;
    pos.y = sp54.y + sp48.y;
    pos.z = zero + sp48.x;

    weapon_gobj = wpManagerMakeWeapon(NULL, &dGRSectorArwingWeaponLaser2DWeaponDesc, &pos, WEAPON_FLAG_PARENT_GROUND);

    if (weapon_gobj != NULL)
    {
        wp = wpGetStruct(weapon_gobj);

        wp->physics.vel_air.x = -230.0F;

        vel.y = vel.z = 0.0F;
        vel.x = -1.0F;

        func_ovl2_8010719C(&vel, &rotate);

        DObjGetStruct(weapon_gobj)->rotate.vec.f = rotate;

        sp48 = gGRCommonStruct.sector.map_dobjs[3]->translate.vec.f;

        syVectorRotate3D(&sp48, 4, gGRCommonStruct.sector.map_dobjs[1]->rotate.vec.f.z);

        pos.x = (sp54.x - sp48.z) - 566.0F;
        pos.y = sp54.y + sp48.y;
        pos.z = zero + sp48.x;

        weapon_gobj = wpManagerMakeWeapon(NULL, &dGRSectorArwingWeaponLaser2DWeaponDesc, &pos, WEAPON_FLAG_PARENT_GROUND);

        if (weapon_gobj != NULL)
        {
            wp = wpGetStruct(weapon_gobj);

            wp->physics.vel_air.x = -230.0F;

            DObjGetStruct(weapon_gobj)->rotate.vec.f = rotate;
        }
    }
}

// 0x80107518
sb32 grSectorArwingWeaponLaserExplodeProcUpdate(GObj *weapon_gobj)
{
    if (wpMainDecLifeCheckExpire(wpGetStruct(weapon_gobj)) != FALSE)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x80107544
void grSectorArwingWeaponLaserExplodeInitVars(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->lifetime = 16;

    wp->attack_coll.can_reflect = FALSE;
    wp->attack_coll.can_absorb = TRUE;
    wp->attack_coll.can_shield = FALSE;

    wp->physics.vel_air.x = wp->physics.vel_air.y = wp->physics.vel_air.z = 0.0F;

    wp->attack_coll.size = 200.0F;

    DObjGetStruct(weapon_gobj)->dl = NULL;

    wpMainClearAttackRecord(wp);

    wp->proc_update = grSectorArwingWeaponLaserExplodeProcUpdate;

    wp->proc_map        =
    wp->proc_hit        =
    wp->proc_shield     =
    wp->proc_hop        =
    wp->proc_setoff     =
    wp->proc_hop        =
    wp->proc_reflector  =  NULL;
}

// 0x801075E0
sb32 grSectorArwingWeaponLaser3DProcMap(GObj *weapon_gobj)
{
    DObj *dobj = DObjGetStruct(weapon_gobj);

    if (ABSF(dobj->translate.vec.f.z) < 1000.0F)
    {
        if (wpMapTestAllCheckCollEnd(weapon_gobj) != FALSE)
        {
            func_800269C0_275C0(nSYAudioFGMExplodeS);
            efManagerSparkleWhiteMultiExplodeMakeEffect(&dobj->translate.vec.f);
            grSectorArwingWeaponLaserExplodeInitVars(weapon_gobj);
        }
    }
    return FALSE;
}

// 0x80107670
sb32 grSectorArwingWeaponLaser3DProcHit(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMultiExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);
    grSectorArwingWeaponLaserExplodeInitVars(weapon_gobj);

    return FALSE;
}

// 0x801076B0
sb32 grSectorArwingWeaponLaser3DProcAbsorb(GObj *weapon_gobj)
{
    func_800269C0_275C0(nSYAudioFGMExplodeS);
    efManagerSparkleWhiteMultiExplodeMakeEffect(&DObjGetStruct(weapon_gobj)->translate.vec.f);

    return TRUE;
}

// 0x801076E8
void grSectorArwingWeaponLaser3DMakeWeapon(void)
{
    GObj *weapon_gobj;
    GObj *fighter_gobj;
    s32 random;
    s32 player;
    FTStruct *fp;
    WPStruct *wp;
    Vec3f wp_pos;
    Vec3f ft_pos;
    Vec3f sp94;
    Vec3f sp88;
    Vec3f sp7C;
    Vec3f wp_angle;
    Mtx44f mtx;
    DObj *dobj;

    dobj = gGRCommonStruct.sector.map_dobjs[0];

    func_ovl2_80106730(dobj, &sp94, &sp88, &sp7C);

    mtx[0][0] = sp88.x; // sp30
    mtx[0][1] = sp88.y; // sp34
    mtx[0][2] = sp88.z; // sp38

    mtx[1][0] = sp7C.x; // sp40
    mtx[1][1] = sp7C.y; // sp44
    mtx[1][2] = sp7C.z; // sp48

    mtx[2][0] = sp94.x; // sp50
    mtx[2][1] = sp94.y; // sp54
    mtx[2][2] = sp94.z; // sp58

    mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0F;// sp3C, sp4C, sp5C

    mtx[3][0] = dobj->translate.vec.f.x + gGRCommonStruct.sector.arwing_target_x; // sp60
    mtx[3][1] = dobj->translate.vec.f.y; // sp64
    mtx[3][2] = dobj->translate.vec.f.z;

    mtx[3][3] = 1.0F;

    wp_pos.y = 0.0F;
    wp_pos.x = 0;

    wp_pos.z = 666.0F;

    gmCollisionGetWorldPosition(mtx, &wp_pos);

    random = syUtilsRandIntRange(gSCManagerBattleState->pl_count + gSCManagerBattleState->cp_count);

    fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    for (player = 0; player < random; player++)
    {
        fighter_gobj = fighter_gobj->link_next;
    }
    fp = ftGetStruct(fighter_gobj);

    if ((fp->coll_data.floor_line_id == -1) || (fp->coll_data.floor_line_id == -2))
    {
        ft_pos.x = ft_pos.y = ft_pos.z = 0;
    }
    else
    {
        ft_pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;

        ft_pos.y += fp->coll_data.floor_dist;
    }
    wp_angle.x = ft_pos.x - wp_pos.x;
    wp_angle.y = ft_pos.y - wp_pos.y;
    wp_angle.z = ft_pos.z - wp_pos.z;

    syVectorNorm3D(&wp_angle);

    weapon_gobj = wpManagerMakeWeapon(NULL, &dGRSectorArwingWeaponLaser3DWeaponDesc, &wp_pos, WEAPON_FLAG_PARENT_GROUND);

    if (weapon_gobj != NULL)
    {
        wp = wpGetStruct(weapon_gobj);

        wp->physics.vel_air.x = wp_angle.x * 230.0F;
        wp->physics.vel_air.y = wp_angle.y * 230.0F;
        wp->physics.vel_air.z = wp_angle.z * 230.0F;

        func_ovl2_8010719C(&wp_angle, &DObjGetStruct(weapon_gobj)->rotate.vec.f);
    }
}

// 0x80107910
void func_ovl2_80107910(void)
{
    if (gGRCommonStruct.sector.arwing_laser_count == 2)
    {
        grSectorArwingWeaponLaser2DMakeWeapon();
    }
    else grSectorArwingWeaponLaser3DMakeWeapon();

    func_800269C0_275C0(nSYAudioFGMSectorArwingLaser);
}

// 0x80107958
void func_ovl2_80107958(void)
{
    s32 ammo;
    void *mh1, *mh2;

    if (gGRCommonStruct.sector.arwing_laser_ammo == 0)
    {
        ammo = 0;

        if (gGRCommonStruct.sector.arwing_pilot_curr == -2)
        {
            if 
            (
                ((gGRCommonStruct.sector.unk_sector_0x4C == 0) && (gGRCommonStruct.sector.arwing_appear_timer == 5)) || 
                ((gGRCommonStruct.sector.unk_sector_0x4C == 1) && (gGRCommonStruct.sector.arwing_appear_timer == 5))
            )
            {
                ammo = grSectorArwingPrepareLaserCount();
            }
        }
        else if (gGRCommonStruct.sector.is_arwing_z_near == 0)
        {
            gGRCommonStruct.sector.unk_sector_0x4E = 60;
            gGRCommonStruct.sector.arwing_laser_ammo = 0;
        }
        else
        {
            gGRCommonStruct.sector.unk_sector_0x4E--;

            if (gGRCommonStruct.sector.unk_sector_0x4E == 0)
            {
                ammo = grSectorArwingGetLaserAmmoCount();

                gGRCommonStruct.sector.unk_sector_0x4E = 60;
            }
        }
        if (ammo != 0)
        {
            gGRCommonStruct.sector.arwing_laser_ammo = ammo;
            gGRCommonStruct.sector.arwing_laser_timer = 0;
            gGRCommonStruct.sector.unk_sector_0x52 = 0;
        }
    }
    else
    {
        if (gGRCommonStruct.sector.arwing_laser_timer == 0)
        {
            if (gGRCommonStruct.sector.unk_sector_0x52 == 0)
            {
                mh1 = gGRCommonStruct.sector.map_head;

                grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[4], lbRelocGetFileData(AObjEvent32*, mh1, &llFoxSpecial3_1B84_AnimJoint), 0.0F);
                grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[5], lbRelocGetFileData(AObjEvent32*, mh1, &llFoxSpecial3_1B84_AnimJoint), 0.0F);

                gGRCommonStruct.sector.unk_sector_0x52++;
            }
            else if (gGRCommonStruct.sector.map_dobjs[4]->anim_wait == AOBJ_ANIM_NULL)
            {
                mh2 = gGRCommonStruct.sector.map_head;

                func_ovl2_80107910();

                grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[2], lbRelocGetFileData(AObjEvent32*, mh2, &llFoxSpecial3_1B34_AnimJoint), 0.0F);
                grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[3], lbRelocGetFileData(AObjEvent32*, mh2, &llFoxSpecial3_1B34_AnimJoint), 0.0F);

                gGRCommonStruct.sector.arwing_laser_timer = 30;
                gGRCommonStruct.sector.arwing_laser_ammo--;
            }
        }
        if (gGRCommonStruct.sector.arwing_laser_timer)
        {
            gGRCommonStruct.sector.arwing_laser_timer--;
        }
        if (gGRCommonStruct.sector.arwing_laser_ammo == 0)
        {
            gGRCommonStruct.sector.unk_sector_0x4E = 240;
        }
    }
}

// 0x80107B30
void func_ovl2_80107B30(void)
{
    if ((gGRCommonStruct.sector.map_dobjs[8]->anim_wait == AOBJ_ANIM_NULL) && (gGRCommonStruct.sector.map_dobjs[7]->flags == DOBJ_FLAG_NONE))
    {
        grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[8], lbRelocGetFileData(AObjEvent32*, gGRCommonStruct.sector.map_file, &llFoxSpecial3_2EB4_AnimJoint), 0.0F);

        func_800269C0_275C0(nSYAudioFGMSectorAmbient2);
    }
}

// 0x80107BA0
void grSectorArwingUpdateCollisions(void)
{
    Vec3f pos;

    if (gGRCommonStruct.sector.arwing_pilot_curr != -2)
    {
        if ((gGRCommonStruct.sector.is_arwing_line_active) && (gGRCommonStruct.sector.is_arwing_z_near))
        {
            pos.x = gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.x + gGRCommonStruct.sector.arwing_target_x;
            pos.y = gGRCommonStruct.sector.map_dobjs[0]->translate.vec.f.y + gGRCommonStruct.sector.map_dobjs[1]->translate.vec.f.y;
            pos.z = 0.0F;

            if ((gGRCommonStruct.sector.is_arwing_z_collision == FALSE) || (gGRCommonStruct.sector.is_arwing_line_collision == FALSE))
            {
                mpCollisionSetYakumonoOnID(1);
                mpCollisionSetYakumonoPosID(1, &pos);
            }
            mpCollisionSetYakumonoPosID(1, &pos);
        }
        if (!(gGRCommonStruct.sector.is_arwing_line_active) || !(gGRCommonStruct.sector.is_arwing_z_near))
        {
            if ((gGRCommonStruct.sector.is_arwing_z_collision != FALSE) && (gGRCommonStruct.sector.is_arwing_line_collision != FALSE))
            {
                mpCollisionSetYakumonoOffID(1);
            }
        }
        gGRCommonStruct.sector.is_arwing_line_collision = gGRCommonStruct.sector.is_arwing_line_active;
        gGRCommonStruct.sector.is_arwing_z_collision = gGRCommonStruct.sector.is_arwing_z_near;
    }
}

// 0x80107CA0
void grSectorArwingUpdatePatrol(void)
{
    grSectorArwingDecideZNear();
    func_ovl2_80106DD8();
    func_ovl2_80107958();
    func_ovl2_80107B30();
    grSectorArwingUpdateCollisions();

    if (gGRCommonStruct.sector.map_dobjs[0]->anim_wait == AOBJ_ANIM_NULL)
    {
        gGRCommonStruct.sector.map_gobj->flags = GOBJ_FLAG_HIDDEN;

        gGRCommonStruct.sector.arwing_appear_timer = syUtilsRandIntRange(1140) + 960;
        gGRCommonStruct.sector.arwing_status = nGRSectorArwingStatusWait;

        mpCollisionSetYakumonoOffID(1);
    }
    else gGRCommonStruct.sector.arwing_appear_timer++;
}

// 0x80107D50
void func_ovl2_80107D50(void)
{
    GObj *map_gobj;
    GRSectorDesc *desc;

    if (gGRCommonStruct.sector.arwing_flight_pattern != -1)
    {
        map_gobj = gGRCommonStruct.sector.map_gobj;
        gGRCommonStruct.sector.arwing_laser_count = dGRSectorArwingLaserCounts[gGRCommonStruct.sector.arwing_flight_pattern];

        desc = (GRSectorDesc*) ((intptr_t)dGRSectorArwingSectorDescs[gGRCommonStruct.sector.arwing_flight_pattern] + (uintptr_t)gGRCommonStruct.sector.map_head);

        grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[0], desc->anim_joint_0x0, 0.0F);
        grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[7], desc->anim_joint_0x1C, 0.0F);
        grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[9], desc->anim_joint_0x24, 0.0F);
        grSectorArwingAddAnim(gGRCommonStruct.sector.map_dobjs[11], desc->anim_joint_0x2C, 0.0F);

        gGRCommonStruct.sector.arwing_flight_pattern = -1;
        map_gobj->flags = GOBJ_FLAG_NONE;
    }
}

// 0x80107E08
void grSectorProcUpdate(GObj *ground_gobj)
{
    switch (gGRCommonStruct.sector.arwing_status)
    {
    case nGRSectorArwingStatusSleep:
        grSectorArwingUpdateSleep();
        break;

    case nGRSectorArwingStatusWait:
        grSectorArwingUpdateWait();
        break;

    case nGRSectorArwingStatusPatrol:
        grSectorArwingUpdatePatrol();
        break;
    }
    func_ovl2_80107D50();
}

// 0x80107E7C
void grSectorInitAll(void)
{
    GObj *map_gobj;
    void *map_file;

    gGRCommonStruct.sector.map_head = (void*) ((uintptr_t)gMPCollisionGroundData->map_nodes - (intptr_t)&llGRSectorMapMapHead);

    map_file = lbRelocGetForceStatusBufferFile((intptr_t)&llFoxSpecial3FileID);

    gGRCommonStruct.sector.map_file = map_file;

    map_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    gGRCommonStruct.sector.map_gobj = map_gobj;

    gcAddGObjDisplay(map_gobj, gcDrawDObjTreeDLLinksForGObj, 6, GOBJ_PRIORITY_DEFAULT, ~0);
    grModelSetupGroundDObjs(map_gobj, lbRelocGetFileData(DObjDesc*, map_file, &llFoxSpecial3EntryArwingDObjDesc), gGRCommonStruct.sector.map_dobjs, dGRSectorArwingTransformKinds);
    gcAddGObjProcess(map_gobj, gcPlayAnimAll, nGCProcessKindFunc, 5);

    gGRCommonStruct.sector.arwing_status = 0;
    gGRCommonStruct.sector.arwing_flight_pattern = -1;
    gGRCommonStruct.sector.arwing_appear_timer = 600;
    gGRCommonStruct.sector.arwing_type_cycle = 3;
    gGRCommonStruct.sector.arwing_pilot_curr = -1;
    gGRCommonStruct.sector.arwing_pilot_prev = 0;
    gGRCommonStruct.sector.arwing_target_x = 0.0F;

    map_gobj->flags = GOBJ_FLAG_HIDDEN;

    gcAddDObjAnimJoint(gGRCommonStruct.sector.map_dobjs[10], lbRelocGetFileData(AObjEvent32*, map_file, &llFoxSpecial3_2E74_AnimJoint), 0.0F);
    gcPlayAnimAll(map_gobj);
    mpCollisionSetYakumonoOffID(1);
    gGRCommonStruct.sector.weapon_head = (void*) ((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRSectorMapMapHeader);
}

// 0x80107FCC
GObj* grSectorMakeGround(void)
{
    GObj *map_gobj = gcMakeGObjSPAfter(nGCCommonKindGround, NULL, nGCCommonLinkIDGround, GOBJ_PRIORITY_DEFAULT);

    grSectorInitAll();
    gcAddGObjProcess(map_gobj, grSectorProcUpdate, nGCProcessKindFunc, 4);

    return map_gobj;
}
