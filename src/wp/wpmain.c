#include <wp/weapon.h>
#include <ft/fighter.h>

extern void func_80026738_27338(alSoundEffect*);
extern alSoundEffect* func_800269C0_275C0(u16);

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80167EB0
void wpMainStopFGM(WPStruct *wp) // Stop weapon's ongoing SFX
{
    if (wp->p_sfx != NULL)
    {
        if ((wp->p_sfx->sfx_id != 0) && (wp->p_sfx->sfx_id == wp->sfx_id))
        {
            func_80026738_27338(wp->p_sfx);
        }
    }
    wp->p_sfx = NULL;
    wp->sfx_id = 0;
}

// 0x80167F08
void wpMainPlayFGM(WPStruct *wp, u16 sfx_id) // Play sound effect for weapon
{
    if (wp->p_sfx != NULL)
    {
        wpMainStopFGM(wp);
    }
    wp->p_sfx = func_800269C0_275C0(sfx_id);

    if (wp->p_sfx != NULL)
    {
        wp->sfx_id = wp->p_sfx->sfx_id;
    }
    else wp->sfx_id = 0;
}

// 0x80167F68
void wpMainVelSetLR(GObj *weapon_gobj) // Set weapon's facing direction based on velocity
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->lr = (wp->physics.vel_air.x >= 0.0F) ? +1 : -1;
}

// 0x80167FA0
void wpMainVelSetModelPitch(GObj *weapon_gobj) // Set pitch rotation based on velocity
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    DObjGetStruct(weapon_gobj)->rotate.vec.f.y = (wp->physics.vel_air.x >= 0.0F) ? F_CST_DTOR32(90.0F) /* HALF_PI32 */ : F_CST_DTOR32(-90.0F);
}

// 0x80167FE8
sb32 wpMainDecLifeCheckExpire(WPStruct *wp) // Decrement lifetime and check whether item has expired
{
    wp->lifetime--;

    if (wp->lifetime == 0)
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x8016800C
void wpMainDestroyWeapon(GObj *weapon_gobj) // Destroy weapon?
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wpMainStopFGM(wp);                  // Stop weapon's SFX
    wpManagerSetPrevStructAlloc(wp);    // Eject weapon's user_data from memory?
    gcEjectGObj(weapon_gobj);           // Eject GObj from memory?
}

// 0x80168044
void wpMainVelGroundTransferAir(GObj *weapon_gobj) // Transfer weapon's base ground velocity to aerial velocity
{
    WPStruct *wp = wpGetStruct(weapon_gobj);

    wp->physics.vel_air.x = wp->lr * wp->coll_data.floor_angle.y * wp->physics.vel_ground;
    wp->physics.vel_air.y = wp->lr * -wp->coll_data.floor_angle.x * wp->physics.vel_ground;
}

// 0x80168088
void wpMainApplyGravityClampTVel(WPStruct *wp, f32 gravity, f32 terminal_velocity) // Subtract vertical velocity every frame and clamp to terminal velocity
{
    wp->physics.vel_air.y -= gravity;

    if (lbCommonMag2D(&wp->physics.vel_air) > terminal_velocity)
    {
        lbCommonNormDist2D(&wp->physics.vel_air);
        lbCommonScale2D(&wp->physics.vel_air, terminal_velocity);
    }
}

// 0x801680EC
void wpMainReflectorSetLR(WPStruct *wp, FTStruct *fp) // Invert direction on reflect
{
    if ((wp->physics.vel_air.x * fp->lr) < 0.0F)
    {
        wp->physics.vel_air.x = -wp->physics.vel_air.x;
    }
}

// 0x80168128
s32 wpMainGetStaledDamage(WPStruct *wp) // Return final damage after applying staling and bonus 0.999%
{
    return (wp->attack_coll.damage * wp->attack_coll.stale) + 0.999F;
}

// 0x80168158
void wpMainClearAttackRecord(WPStruct *wp) // Clear hit victims array
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(wp->attack_coll.attack_records); i++)
    {
        GMAttackRecord *record = &wp->attack_coll.attack_records[i];

        record->victim_gobj = NULL;

        record->victim_flags.is_interact_hurt = record->victim_flags.is_interact_shield = record->victim_flags.is_interact_reflect = record->victim_flags.is_interact_absorb = FALSE;

        record->victim_flags.timer_rehit = 0;

        record->victim_flags.group_id = 7;
    }
}

// 0x8016830C
void func_ovl3_8016830C(DObj *dobj, Vec3f *vec)
{
    s32 unused[4];
    DObj *current_dobj;
    Mtx44f sp9C;
    Mtx44f sp5C;
    s32 i, j;

    guMtxIdentF(sp5C);

    current_dobj = dobj;

    for (i = 0; i != 0x12; i++, current_dobj = current_dobj->parent)
    {
        for (j = 0; j < current_dobj->xobjs_num; j++)
        {
            XObj *xobj = current_dobj->xobjs[j];

            if (xobj->kind == nGCMatrixKindNull)
            {
                break;
            }
            else guMtxL2F(sp9C, &xobj->mtx), guMtxCatF(sp5C, sp9C, sp5C);
        }
        if (current_dobj->parent == DOBJ_PARENT_NULL)
        {
            break;
        }
    }
    guMtxXFMF(sp5C, 0.0F, 0.0F, 0.0F, &vec->x, &vec->y, &vec->z);
}

// 0x80168428
void wpMainReflectorRotateWeaponModel(GObj *weapon_gobj)
{
    WPStruct *wp = wpGetStruct(weapon_gobj);
    Vec3f vel = wp->physics.vel_air, direction, angle, *rotate;

    direction.x = 0;
    direction.y = 0;
    direction.z = (vel.x > 0.0F) ? -1 : +1;

    syVectorNorm3D(&vel);

    lbCommonCross3D(&vel, &direction, &angle);

    rotate = &DObjGetStruct(weapon_gobj)->rotate.vec.f;

    if (direction.z == -1)
    {
        rotate->y = F_CST_DTOR32(90.0F);
        rotate->x = syUtilsArcTan2(angle.x, angle.y);
    }
    else
    {
        rotate->y = F_CST_DTOR32(-90.0F);
        rotate->x = syUtilsArcTan2(-angle.x, angle.y);
    }
    rotate->z = 0.0F;
}
