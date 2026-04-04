#include <ft/fighter.h>
#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTNESS_SPECIALHISTART_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)
#define FTNESS_SPECIALHIHOLD_STATUS_FLAGS (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_LOOPSFX | FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)
#define FTNESS_SPECIALHIEND_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)
#define FTNESS_SPECIALHI_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80153C50
void ftNessSpecialHiDecThunderTimers(FTStruct *fp)
{
    if (fp->status_vars.ness.specialhi.pkjibaku_delay != 0)
    {
        fp->status_vars.ness.specialhi.pkjibaku_delay--;
    }
    if (fp->passive_vars.ness.is_thunder_destroy & TRUE)
    {
        if (fp->status_vars.ness.specialhi.pkthunder_end_delay != 0)
        {
            fp->status_vars.ness.specialhi.pkthunder_end_delay--;
        }
    }
}

// 0x80153C88
void ftNessSpecialHiMakePKThunder(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    Vec3f vel;

    pos.x = 0.0F;
    pos.y = 0.0F;
    pos.z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[FTNESS_PKTHUNDER_SPAWN_JOINT], &pos);

    pos.z = 0.0F;

    vel.z = 0.0F;
    vel.y = FTNESS_PKTHUNDER_SPAWN_VEL_Y;
    vel.x = 0.0F;

    fp->status_vars.ness.specialhi.pkthunder_gobj = wpNessPKThunderHeadMakeWeapon(fighter_gobj, &pos, &vel);
}

// 0x80153CFC
sb32 ftNessSpecialHiCheckCollidePKThunder(GObj *fighter_gobj)
{
    f32 ft_pos_x;
    f32 ft_pos_y;
    f32 wp_pos_x;
    f32 wp_pos_y;
    f32 collide_x;
    f32 collide_y;
    GObj *pkthunder_gobj;
    WPStruct *ip;
    FTStruct *fp = ftGetStruct(fighter_gobj);

    pkthunder_gobj = fp->status_vars.ness.specialhi.pkthunder_gobj;

    if (fp->status_vars.ness.specialhi.pkjibaku_delay != 0)
    {
        return FALSE;
    }
    if ((fp->passive_vars.ness.is_thunder_destroy & TRUE) || (pkthunder_gobj == NULL))
    {
        return FALSE;
    }
    ip = wpGetStruct(pkthunder_gobj);

    ft_pos_x = DObjGetStruct(fighter_gobj)->translate.vec.f.x;
    wp_pos_x = DObjGetStruct(pkthunder_gobj)->translate.vec.f.x;

    collide_x = (ft_pos_x < wp_pos_x) ? -(ft_pos_x - wp_pos_x) : (ft_pos_x - wp_pos_x);

    if (collide_x < FTNESS_PKTHUNDER_COLLIDE_X)
    {
        ft_pos_y = DObjGetStruct(fighter_gobj)->translate.vec.f.y + 150.0F;
        wp_pos_y = DObjGetStruct(pkthunder_gobj)->translate.vec.f.y;

        collide_y = (ft_pos_y < wp_pos_y) ? -(ft_pos_y - wp_pos_y) : (ft_pos_y - wp_pos_y);

        if (collide_y < FTNESS_PKTHUNDER_COLLIDE_Y)
        {
            ip->weapon_vars.pkthunder.status = nWPNessPKThunderStatusCollide;

            fp->status_vars.ness.specialhi.pkthunder_pos = DObjGetStruct(pkthunder_gobj)->translate.vec.f;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80153E04
void ftNessSpecialHiStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftNessSpecialHiHoldSetStatus);
}

// 0x80153E28
void ftNessSpecialAirHiStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftNessSpecialAirHiHoldSetStatus);
}

// 0x80153E4C
void ftNessSpecialHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.ness.specialhi.pkthunder_gravity_delay != 0)
    {
        fp->status_vars.ness.specialhi.pkthunder_gravity_delay--;
    }
    ftPhysicsApplyGroundVelFriction(fighter_gobj);
}

// 0x80153E80
void ftNessSpecialAirHiProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->status_vars.ness.specialhi.pkthunder_gravity_delay != 0)
    {
        fp->status_vars.ness.specialhi.pkthunder_gravity_delay--;
    }
    else ftPhysicsApplyGravityClampTVel(fp, 0.5F, attr->tvel_base);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x80153EF8
void ftNessSpecialHiStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialHiStartSwitchStatusAir);
}

// 0x80153F1C
void ftNessSpecialAirHiStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirHiStartSwitchStatusGround);
}

// 0x80153F40
void ftNessSpecialAirHiStartSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiStart, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALHISTART_STATUS_FLAGS);
}

// 0x80153F80
void ftNessSpecialHiStartSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiStart, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALHISTART_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80153FCC
void ftNessSpecialHiInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.ness.specialhi.pkjibaku_delay = FTNESS_PKJIBAKU_DELAY;
    fp->status_vars.ness.specialhi.pkthunder_end_delay = FTNESS_PKTHUNDER_END_DELAY;
    fp->status_vars.ness.specialhi.pkthunder_gravity_delay = FTNESS_PKTHUNDER_GRAVITY_DELAY;
    fp->passive_vars.ness.is_thunder_destroy = FALSE;
    fp->passive_vars.ness.pkthunder_trail_id = 0;
}

// 0x80153FF0
void ftNessSpecialHiStartSetStatus(GObj *fighter_gobj)
{
    ftNessSpecialHiInitStatusVars(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80154030
void ftNessSpecialAirHiStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialHiInitStatusVars(fighter_gobj);

    fp->physics.vel_air.y = 0.0F;
    fp->physics.vel_air.x /= 2;

    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80154098
void ftNessSpecialHiUpdatePKThunder(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *weapon_gobj = fp->status_vars.ness.specialhi.pkthunder_gobj;

    if (weapon_gobj == NULL)
    {
        fp->passive_vars.ness.is_thunder_destroy |= TRUE;
    }
    fp->passive_vars.ness.pkthunder_trail_id++;

    if (fp->passive_vars.ness.pkthunder_trail_id >= FTNESS_PKTHUNDER_TRAIL_POS_COUNT)
    {
        fp->passive_vars.ness.pkthunder_trail_id = 0;
    }
    ftNessSpecialHiDecThunderTimers(fp);
}

// 0x801540EC
void ftNessSpecialHiHoldProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialHiUpdatePKThunder(fighter_gobj);

    if ((fp->status_vars.ness.specialhi.pkjibaku_delay <= 0) && (fp->status_vars.ness.specialhi.pkthunder_end_delay <= 0) && (fp->passive_vars.ness.is_thunder_destroy & TRUE))
    {
        ftNessSpecialHiEndSetStatus(fighter_gobj);
    }

    else if (ftNessSpecialHiCheckCollidePKThunder(fighter_gobj) != FALSE)
    {
        ftNessSpecialHiJibakuSetStatus(fighter_gobj);
    }
}

// 0x8015416C
void ftNessSpecialAirHiHoldProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialHiUpdatePKThunder(fighter_gobj);

    if ((fp->status_vars.ness.specialhi.pkjibaku_delay <= 0) && (fp->status_vars.ness.specialhi.pkthunder_end_delay <= 0) && (fp->passive_vars.ness.is_thunder_destroy & TRUE))
    {
        ftNessSpecialAirHiEndSetStatus(fighter_gobj);
    }

    else if (ftNessSpecialHiCheckCollidePKThunder(fighter_gobj) != FALSE)
    {
        ftNessSpecialAirHiJibakuSetStatus(fighter_gobj);
    }
}

// 0x801541EC
void ftNessSpecialHiHoldProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialHiHoldSwitchStatusAir);
}

// 0x80154210
void ftNessSpecialAirHiHoldProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirHiHoldSwitchStatusGround);
}

// 0x80154234
void ftNessSpecialHiSetPKThunderDestroy(GObj *fighter_gobj) // Unused
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *weapon_gobj = fp->status_vars.ness.specialhi.pkthunder_gobj;

    if (!(fp->passive_vars.ness.is_thunder_destroy & TRUE) && (weapon_gobj != NULL))
    {
        WPStruct *wp = wpGetStruct(weapon_gobj);

        wp->weapon_vars.pkthunder.status = nWPNessPKThunderStatusDestroy;
    }
}

// 0x80154268
void ftNessSpecialAirHiHoldSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiHold, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALHIHOLD_STATUS_FLAGS);
}

// 0x801542A8
void ftNessSpecialHiHoldSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiHold, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALHIHOLD_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x801542F4
void ftNessSpecialHiHoldInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialHiMakePKThunder(fighter_gobj);

    if (!(fp->is_effect_attach) && (efManagerNessPKThunderWaveMakeEffect(fighter_gobj) != NULL))
    {
        fp->is_effect_attach = TRUE;
    }
    fp->jumps_used = fp->attr->jumps_max;
}

// 0x8015435C
void ftNessSpecialHiHoldSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiHold, 0.0F, 1.0F, FTSTATUS_PRESERVE_LOOPSFX);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialHiHoldInitStatusVars(fighter_gobj);
}

// 0x801543A0
void ftNessSpecialAirHiHoldSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiHold, 0.0F, 1.0F, FTSTATUS_PRESERVE_LOOPSFX);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialHiHoldInitStatusVars(fighter_gobj);
}

// 0x801543E4
void ftNessSpecialAirHiEndProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTNESS_PKTHUNDER_FALLSPECIAL_DRIFT, FALSE, TRUE, TRUE, FTNESS_PKTHUNDER_LANDING_LAG, FALSE);
    }
}

// 0x80154438
void ftNessSpecialHiEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialHiEndSwitchStatusAir);
}

// 0x8015445C
void ftNessSpecialAirHiEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirHiEndSwitchStatusGround);
}

// 0x80154480
void ftNessSpecialAirHiEndSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiEnd, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALHIEND_STATUS_FLAGS);
}

// 0x801544C0
void ftNessSpecialHiEndSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiEnd, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALHIEND_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x8015450C
void ftNessSpecialHiClearProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_damage = NULL;
}

// 0x80154518
void ftNessSpecialHiEndSetStatus(GObj *fighter_gobj)
{
    ftNessSpecialHiClearProcDamage(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80154558
void ftNessSpecialAirHiEndSetStatus(GObj *fighter_gobj)
{
    ftNessSpecialHiClearProcDamage(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80154598
void ftNessSpecialHiCollideWallPhysics(GObj *fighter_gobj, MPCollData *coll_data)
{
    // The developers overengineered PK Thunder's physics by unnecessarily multiplying vel_air.x and pkjibaku_angle with fp->lr back and forth...
    // While this function could probably be reimplemented without getting rid of the fp->lr solution, I feel like this is the better way to do it.
    // Not only does this get rid of the need to use fp->lr, but it also fixes the janky wall angle reflections.
    // (shoutouts to ChatGPT)

    /*
    f32 pkjibaku_angle, wall_angle, angle_diff;
    FTStruct *fp = ftGetStruct(fighter_gobj);

    pkjibaku_angle = fp->status_vars.ness.specialhi.pkjibaku_angle;

    while (pkjibaku_angle > F_CST_DTOR32(180.0F))
    {
        pkjibaku_angle -= F_CST_DTOR32(360.0F);
    }
    while (pkjibaku_angle < F_CST_DTOR32(-180.0F))
    {
        pkjibaku_angle += F_CST_DTOR32(360.0F);
    }
    if (coll_data->mask_curr & MAP_FLAG_LWALL)
    {
        wall_angle = syUtilsArcTan2(coll_data->lwall_angle.y, coll_data->lwall_angle.x);
    }
    else if (coll_data->mask_curr & MAP_FLAG_RWALL)
    {
        wall_angle = syUtilsArcTan2(coll_data->rwall_angle.y, coll_data->rwall_angle.x);
    }
    else wall_angle = F_CST_DTOR32(0.0F);

    while (wall_angle > F_CST_DTOR32(180.0F))
    {
        wall_angle -= F_CST_DTOR32(360.0F);
    }
    while (wall_angle < F_CST_DTOR32(-180.0F))
    {
        wall_angle += F_CST_DTOR32(360.0F);
    }
    angle_diff = pkjibaku_angle - wall_angle;

    while (angle_diff > F_CST_DTOR32(180.0F))
    {
        angle_diff -= F_CST_DTOR32(360.0F);
    }
    while (angle_diff < F_CST_DTOR32(-180.0F))
    {
        angle_diff += F_CST_DTOR32(360.0F);
    }
    wall_angle += (angle_diff > 0.0F) ? F_CST_DTOR32(90.0F) : F_CST_DTOR32(-90.0F);

    while (wall_angle > F_CST_DTOR32(180.0F))
    {
        wall_angle -= F_CST_DTOR32(360.0F);
    }
    while (wall_angle < F_CST_DTOR32(-180.0F))
    {
        wall_angle += F_CST_DTOR32(360.0F);
    }
    syVectorRotate3D(&fp->physics.vel_air, SYVECTOR_AXIS_Z, wall_angle - fp->status_vars.ness.specialhi.pkjibaku_angle);

    fp->status_vars.ness.specialhi.pkjibaku_angle = syUtilsArcTan2(fp->physics.vel_air.y, fp->physics.vel_air.x);
    */

    s32 unused;
    f32 angle_old, angle_new;
    FTStruct *fp = ftGetStruct(fighter_gobj);

    angle_old = (fp->lr == +1) ? fp->status_vars.ness.specialhi.pkjibaku_angle : (F_CST_DTOR32(180.0F) - fp->status_vars.ness.specialhi.pkjibaku_angle);

    if (angle_old > F_CST_DTOR32(360.0F))
    {
        angle_old -= F_CST_DTOR32(360.0F);
    }
    if (coll_data->mask_curr & MAP_FLAG_LWALL)
    {
        angle_new = syUtilsArcTan2(coll_data->lwall_angle.y, coll_data->lwall_angle.x);

        if (angle_new > F_CST_DTOR32(360.0F))
        {
            angle_new -= F_CST_DTOR32(360.0F);
        }
        angle_new = ((angle_old + F_CST_DTOR32(180.0F)) < angle_new) ? (angle_new + F_CST_DTOR32(90.0F)) : (angle_new + F_CST_DTOR32(-90.0F));
    }
    if (coll_data->mask_curr & MAP_FLAG_RWALL)
    {
        angle_new = syUtilsArcTan2(coll_data->rwall_angle.y, coll_data->rwall_angle.x);

        if (angle_new > F_CST_DTOR32(360.0F))
        {
            angle_new -= F_CST_DTOR32(360.0F);
        }
        angle_new = ((angle_new + F_CST_DTOR32(180.0F)) < angle_old) ? (angle_new + F_CST_DTOR32(90.0F)) : (angle_new + F_CST_DTOR32(-90.0F));
    }
    // WARNING: Even though this function only runs if Ness collides with an LWALL/RWALL, angle_new is uninitialized if both conditions fail somehow. Cosmic bit-flip?
    syVectorRotate3D(&fp->physics.vel_air, SYVECTOR_AXIS_Z, angle_new - (fp->status_vars.ness.specialhi.pkjibaku_angle * fp->lr));

    fp->status_vars.ness.specialhi.pkjibaku_angle = syUtilsArcTan2(fp->physics.vel_air.y, fp->physics.vel_air.x * fp->lr);
}

// 0x80154758
void ftNessSpecialHiUpdateModelPitch(GObj *fighter_gobj) // Update joint's X rotation axis
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    
    // PK Thunder's velocity sign can sometimes end up becoming the inverse of the LR sign, causing Ness' model rotation to corrupt.
    // Solution:

    /* 
    fp->joints[4]->rotate.vec.f.x = 
    (syUtilsArcTan2(((fp->physics.vel_air.x * fp->lr) < 0.0F) ? -fp->physics.vel_air.x : fp->physics.vel_air.x, fp->physics.vel_air.y) * fp->lr) - F_CST_DTOR32(90.0F); 
    */

    fp->joints[4]->rotate.vec.f.x = (syUtilsArcTan2(fp->physics.vel_air.x, fp->physics.vel_air.y) * fp->lr) - F_CST_DTOR32(90.0F);

    ftParamsUpdateFighterPartsTransformAll(fp->joints[4]);
}

// 0x801547B8
void ftNessSpecialHiJibakuProcUpdate(GObj *fighter_gobj) // Grounded PK Thunder Blast
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.ness.specialhi.pkjibaku_anim_length--;

    if (fp->status_vars.ness.specialhi.pkjibaku_anim_length == 0)
    {
        ftNessSpecialHiEndSetStatus(fighter_gobj);
    }
}

// 0x801547EC
void ftNessSpecialAirHiJibakuProcUpdate(GObj *fighter_gobj) // Aerial PK Thunder Blast
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.ness.specialhi.pkjibaku_anim_length--;

    if (fp->status_vars.ness.specialhi.pkjibaku_anim_length == 0)
    {
        ftNessSpecialAirHiEndSetStatus(fighter_gobj);
    }
}

// 0x80154820
void ftNessSpecialHiJibakuProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->physics.vel_ground.x > 0.0F)
    {
        fp->physics.vel_ground.x -= FTNESS_PKJIBAKU_DECELERATE;
    }

    ftPhysicsApplyGroundVelTransferAir(fighter_gobj);
    ftNessSpecialHiUpdateModelPitch(fighter_gobj);
}

// 0x80154874
void ftNessSpecialAirHiJibakuProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 vel_x_bak = fp->physics.vel_air.x;
    f32 vel_y_bak = fp->physics.vel_air.y;

    fp->physics.vel_air.x -= (FTNESS_PKJIBAKU_DECELERATE * __cosf(fp->status_vars.ness.specialhi.pkjibaku_angle) * fp->lr);
    fp->physics.vel_air.y -= (FTNESS_PKJIBAKU_DECELERATE * __sinf(fp->status_vars.ness.specialhi.pkjibaku_angle));

    if (ABSF(fp->physics.vel_air.x) > ABSF(vel_x_bak))
    {
        fp->physics.vel_air.x = vel_x_bak;
    }
    if (ABSF(fp->physics.vel_air.y) > ABSF(vel_y_bak))
    {
        fp->physics.vel_air.y = vel_y_bak;
    }
    ftNessSpecialHiUpdateModelPitch(fighter_gobj);
}

// 0x801549B0
sb32 ftNessSpecialHiProcPass(GObj *fighter_gobj) // Check if Ness can pass through platforms, TRUE actually denies?
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    if (!(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) || (fp->status_vars.ness.specialhi.pkjibaku_anim_length <= FTNESS_PKJIBAKU_PASS_FRAME_END))
    {
        return TRUE;
    }
    else return FALSE;
}

// 0x801549FC
void ftNessSpecialHiJibakuProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterOnFloor(fighter_gobj) == FALSE)
    {
        if (fp->coll_data.mask_curr & (MAP_FLAG_RWALL | MAP_FLAG_LWALL))
        {
            mpCommonSetFighterAir(fp);
            ftNessSpecialAirHiEndSetStatus(fighter_gobj);
        }
        else ftNessSpecialHiJibakuSwitchStatusAir(fighter_gobj);
    }
    else if (fp->coll_data.mask_curr & (MAP_FLAG_CEIL | MAP_FLAG_RWALL | MAP_FLAG_LWALL))
    {
        fp->physics.vel_ground.x = 0.0F;
        ftCommonDownBounceSetStatus(fighter_gobj);
    }
}

// 0x80154A8C
void ftNessSpecialAirHiJibakuProcMap(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);
    f32 unused;
    Vec3f pos;

    if (mpCommonCheckFighterPassCliff(fighter_gobj, ftNessSpecialHiProcPass) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else if (syVectorAngleDiff3D(&fp->coll_data.floor_angle, &fp->physics.vel_air) > FTNESS_PKJIBAKU_HALT_ANGLE)
        {
            fp->physics.vel_air.x = 0.0F;
            fp->physics.vel_air.y = 0.0F;

            mpCommonSetFighterGround(fp);
            ftCommonDownBounceSetStatus(fighter_gobj);
        }
        else ftNessSpecialAirHiJibakuSwitchStatusGround(fighter_gobj);

        return;
    }

    pos.x = DObjGetStruct(fighter_gobj)->translate.vec.f.x;
    pos.y = DObjGetStruct(fighter_gobj)->translate.vec.f.y;
    pos.z = 0.0F;

    if (fp->coll_data.mask_curr & MAP_FLAG_CEIL)
    {
        if (syVectorAngleDiff3D(&fp->coll_data.ceil_angle, &fp->physics.vel_air) > FTNESS_PKJIBAKU_HALT_ANGLE)
        {
            pos.y += fp->coll_data.map_coll.top;

            ftNessSpecialAirHiJibakuBoundSetStatus(fighter_gobj, &fp->coll_data.ceil_angle, &pos);
        }
    }
    if (fp->coll_data.mask_curr & MAP_FLAG_LWALL)
    {
        if (syVectorAngleDiff3D(&fp->coll_data.lwall_angle, &fp->physics.vel_air) > FTNESS_PKJIBAKU_HALT_ANGLE)
        {
            pos.x += fp->coll_data.map_coll.width;
            pos.y += fp->coll_data.map_coll.center;

            ftNessSpecialAirHiJibakuBoundSetStatus(fighter_gobj, &fp->coll_data.lwall_angle, &pos);
        }
        else ftNessSpecialHiCollideWallPhysics(fighter_gobj, &fp->coll_data);
    }
    if (fp->coll_data.mask_curr & MAP_FLAG_RWALL)
    {
        if (syVectorAngleDiff3D(&fp->coll_data.rwall_angle, &fp->physics.vel_air) > FTNESS_PKJIBAKU_HALT_ANGLE)
        {
            pos.x -= fp->coll_data.map_coll.width;
            pos.y += fp->coll_data.map_coll.center;

            ftNessSpecialAirHiJibakuBoundSetStatus(fighter_gobj, &fp->coll_data.rwall_angle, &pos);
        }
        else ftNessSpecialHiCollideWallPhysics(fighter_gobj, &fp->coll_data);
    }
}

// 0x80154CBC
void ftNessSpecialAirHiJibakuSwitchStatusGround(GObj *fighter_gobj)
{
    f32 frame_begin;

    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));

    frame_begin = fighter_gobj->anim_frame;

    if (frame_begin == 0.0F)
    {
        frame_begin = 1.0F;
    }
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiJibaku, frame_begin, 1.0F, FTNESS_SPECIALHI_STATUS_FLAGS); // Change Action State to PK Thunder Blast
}

// 0x80154D1C
void ftNessSpecialHiJibakuSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 frame_begin;

    mpCommonSetFighterAir(fp);

    frame_begin = fighter_gobj->anim_frame;

    if (frame_begin == 0.0F)
    {
        frame_begin = 1.0F;
    }
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiJibaku, frame_begin, 1.0F, FTNESS_SPECIALHI_STATUS_FLAGS);

    fp->status_vars.ness.specialhi.pkjibaku_angle = syUtilsArcTan2(fp->physics.vel_air.y, fp->physics.vel_air.x * fp->lr);

    fp->jumps_used = fp->attr->jumps_max;
}

// 0x80154DBC
void ftNessSpecialHiJibakuInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.ness.specialhi.pkjibaku_anim_length = FTNESS_PKJIBAKU_ANIM_LENGTH;

    ftNessSpecialHiUpdateModelPitch(fighter_gobj);

    fp->proc_damage = NULL;

    fp->jumps_used = fp->attr->jumps_max;
}

// 0x80154DFC
void ftNessSpecialHiJibakuSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 angle_diff;
    s32 unused;
    Vec3f pos;

    if (fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) goto setair;
    
    pos.x = DObjGetStruct(fighter_gobj)->translate.vec.f.x - fp->status_vars.ness.specialhi.pkthunder_pos.x;
    pos.y = (DObjGetStruct(fighter_gobj)->translate.vec.f.y + 150.0F) - fp->status_vars.ness.specialhi.pkthunder_pos.y;
    pos.z = 0.0F;

    angle_diff = syVectorAngleDiff3D(&fp->coll_data.floor_angle, &pos);

    if (angle_diff < F_CST_DTOR32(90.0F))
    {
        goto setair;
    }
    else if (angle_diff > F_CST_DTOR32(155.0F))
    {
        goto setdown; // 2.3561945F
    }
    fp->lr = (pos.x >= 0) ? +1 : -1;

    if (pos.x < 0)
    {
        pos.x = -pos.x;
    }
    fp->physics.vel_ground.x = FTNESS_PKJIBAKU_VEL;

    ftNessSpecialHiJibakuInitStatusVars(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialHiJibaku, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
    ftMainPlayAnimEventsAll(fighter_gobj);
    return;  
   
setair:
    mpCommonSetFighterAir(fp);       
    ftNessSpecialAirHiJibakuSetStatus(fighter_gobj);       
    return;
    
setdown:
    ftCommonDownBounceSetStatus(fighter_gobj);
}

// 0x80154F54
void ftNessSpecialAirHiJibakuSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *dobj = DObjGetStruct(fighter_gobj);
    f32 dist_x = dobj->translate.vec.f.x - fp->status_vars.ness.specialhi.pkthunder_pos.x;
    f32 dist_y = (dobj->translate.vec.f.y + 150.0F) - fp->status_vars.ness.specialhi.pkthunder_pos.y;

    fp->lr = (dist_x >= 0.0F) ? +1 : -1;

    fp->status_vars.ness.specialhi.pkjibaku_angle = syUtilsArcTan2(dist_y, fp->lr * dist_x);

    fp->physics.vel_air.x = (__cosf(fp->status_vars.ness.specialhi.pkjibaku_angle) * FTNESS_PKJIBAKU_VEL * fp->lr);
    fp->physics.vel_air.y = (__sinf(fp->status_vars.ness.specialhi.pkjibaku_angle) * FTNESS_PKJIBAKU_VEL);

    ftNessSpecialHiJibakuInitStatusVars(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiJibaku, 0.0F, 1.0F, FTSTATUS_PRESERVE_COLANIM);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80155058
void ftNessSpecialAirHiJibakuBoundProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        ftCommonFallSpecialSetStatus(fighter_gobj, FTNESS_PKTHUNDER_FALLSPECIAL_DRIFT, FALSE, TRUE, TRUE, FTNESS_PKTHUNDER_LANDING_LAG, FALSE);
    }
}

// 0x801550AC
void ftNessSpecialAirHiJibakuBoundProcMap(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    if (mpCommonCheckFighterCliff(fighter_gobj) != FALSE)
    {
        if (fp->coll_data.mask_stat & MAP_FLAG_CLIFF_MASK)
        {
            ftCommonCliffCatchSetStatus(fighter_gobj);
        }
        else
        {
            mpCommonSetFighterGround(fp);
            ftCommonDownBounceSetStatus(fighter_gobj);
        }
    }
}

// 0x80155114
void ftNessSpecialAirHiJibakuBoundSetStatus(GObj *fighter_gobj, Vec3f *angle, Vec3f *pos)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);
    Vec3f *vel = &fp->physics.vel_air;

    lbCommonReflect2D(vel, angle);
    lbCommonScale2D(vel, FTNESS_PKJIBAKU_REBOUND_VEL_MAG);
    lbCommonMag2D(vel);
    ftPhysicsClampAirVelXMax(fp);

    fp->lr = (fp->physics.vel_air.x < 0.0F) ? +1 : -1;

    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirHiBound, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    efManagerImpactWaveMakeEffect(pos, SYVECTOR_AXIS_Z, syUtilsArcTan2(-angle->x, angle->y));
    efManagerQuakeMakeEffect(2);
}
