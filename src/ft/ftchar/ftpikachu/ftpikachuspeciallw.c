#include <ft/fighter.h>
#include <wp/weapon.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTPIKACHU_SPECIALLWSTART_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM)
#define FTPIKACHU_SPECIALLWLOOP_STATUS_FLAGS (FTSTATUS_PRESERVE_RUMBLE | FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)
#define FTPIKACHU_SPECIALLWHIT_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)
#define FTPIKACHU_SPECIALLWEND_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80151DB0
void ftPikachuSpecialLwMakeThunder(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    Vec3f vel;

    pos.x = 0.0F;
    pos.y = 0.0F;
    pos.z = 0.0F;

    gmCollisionGetFighterPartsWorldPosition(fp->joints[FTPIKACHU_THUNDER_SPAWN_JOINT], &pos);

    pos.y = gMPCollisionGroundData->map_bound_top - FTPIKACHU_THUNDER_SPAWN_OFF_Y;

    vel.x = 0.0F;
    vel.z = 0.0F;
    vel.y = FTPIKACHU_THUNDER_VEL_Y;

    fp->status_vars.pikachu.speciallw.thunder_gobj = wpPikachuThunderHeadMakeWeapon(fighter_gobj, &pos, &vel);
}

// 0x80151E44
void ftPikachuSpecialLwStartUpdateThunder(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag0 != 0)
    {
        ftPikachuSpecialLwMakeThunder(fighter_gobj);
    }
}

// 0x80151E74
void ftPikachuSpecialLwStartProcUpdate(GObj *fighter_gobj)
{
    ftPikachuSpecialLwStartUpdateThunder(fighter_gobj);
    ftAnimEndCheckSetStatus(fighter_gobj, ftPikachuSpecialLwLoopSetStatus);
}

// 0x80151EA4
void ftPikachuSpecialAirLwStartProcUpdate(GObj *fighter_gobj)
{
    ftPikachuSpecialLwStartUpdateThunder(fighter_gobj);
    ftAnimEndCheckSetStatus(fighter_gobj, ftPikachuSpecialAirLwLoopSetStatus);
}

// 0x80151ED4
void ftPikachuSpecialLwStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftPikachuSpecialLwStartSwitchStatusAir);
}

// 0x80151EF8
void ftPikachuSpecialAirLwStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPikachuSpecialAirLwStartSwitchStatusGround);
}

// 0x80151F1C
void ftPikachuSpecialAirLwStartSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwStart, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWSTART_STATUS_FLAGS);
}

// 0x80151F5C
void ftPikachuSpecialLwStartSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwStart, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWSTART_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80151FA8
void ftPikachuSpecialLwStartInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;
    fp->motion_vars.flags.flag0 = 0;

    fp->passive_vars.pikachu.is_thunder_destroy = FALSE;
}

// 0x80151FBC
void ftPikachuSpecialLwStartSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPikachuSpecialLwStartInitStatusVars(fighter_gobj);
}

// 0x80151FFC
void ftPikachuSpecialAirLwStartSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPikachuSpecialLwStartInitStatusVars(fighter_gobj);
}

// 0x8015203C
sb32 ftPikachuSpecialLwCheckCollideThunder(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    WPStruct *wp;
    DObj *fj;
    DObj *wj;
    GObj *thunder_gobj;
    f32 ft_temp_x;
    f32 ft_temp_y;
    f32 wp_temp_x;
    f32 wp_temp_y;
    f32 dist_x;
    f32 dist_y;

    thunder_gobj = fp->status_vars.pikachu.speciallw.thunder_gobj;

    if (thunder_gobj == NULL)
    {
        fp->passive_vars.pikachu.is_thunder_destroy |= TRUE;
    }

    if (fp->passive_vars.pikachu.is_thunder_destroy & TRUE)
    {
        return FALSE;
    }

    wp = wpGetStruct(thunder_gobj);

    fj = DObjGetStruct(fighter_gobj);
    wj = DObjGetStruct(thunder_gobj);

    ft_temp_x = fj->translate.vec.f.x;
    wp_temp_x = wj->translate.vec.f.x;

    dist_x = (ft_temp_x < wp_temp_x) ? -(ft_temp_x - wp_temp_x) : (ft_temp_x - wp_temp_x);

    if (dist_x < FTPIKACHU_THUNDER_COLLIDE_X)
    {
        ft_temp_y = fj->translate.vec.f.y;
        wp_temp_y = wj->translate.vec.f.y + FTPIKACHU_THUNDER_COLL_OFF_Y;

        dist_y = (ft_temp_y < wp_temp_y) ? -(ft_temp_y - wp_temp_y) : (ft_temp_y - wp_temp_y);

        if (dist_y < FTPIKACHU_THUNDER_COLLIDE_Y)
        {
            wp->weapon_vars.thunder.thunder_state = nWPPikachuThunderStatusCollide;

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80152124
void ftPikachuSpecialLwLoopProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftPikachuSpecialLwCheckCollideThunder(fighter_gobj) != FALSE)
    {
        ftPikachuSpecialLwHitSetStatus(fighter_gobj);
    }
    else if (fp->passive_vars.pikachu.is_thunder_destroy & TRUE)
    {
        ftPikachuSpecialLwEndSetStatus(fighter_gobj);
    }
    else if (fp->motion_vars.flags.flag1 != 0)
    {
        ftPikachuSpecialLwEndSetStatus(fighter_gobj);
    }
}

// 0x8015219C
void ftPikachuSpecialAirLwLoopProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (ftPikachuSpecialLwCheckCollideThunder(fighter_gobj) != FALSE)
    {
        ftPikachuSpecialAirLwHitSetStatus(fighter_gobj);
    }
    else if (fp->passive_vars.pikachu.is_thunder_destroy & TRUE)
    {
        ftPikachuSpecialAirLwEndSetStatus(fighter_gobj);
    }
    else if (fp->motion_vars.flags.flag1 != 0)
    {
        ftPikachuSpecialAirLwEndSetStatus(fighter_gobj);
    }
}

// 0x80152214
void ftPikachuSpecialLwLoopProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftPikachuSpecialLwLoopSwitchStatusAir);
}

// 0x80152238
void ftPikachuSpecialAirLwLoopProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPikachuSpecialAirLwLoopSwitchStatusGround);
}

// 0x8015225C
void ftPikachuSpecialLwProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *thunder_gobj = fp->status_vars.pikachu.speciallw.thunder_gobj;

    if (thunder_gobj == NULL)
    {
        fp->passive_vars.pikachu.is_thunder_destroy |= TRUE;
    }
    if (!(fp->passive_vars.pikachu.is_thunder_destroy & TRUE))
    {
        WPStruct *wp = wpGetStruct(thunder_gobj);

        wp->weapon_vars.thunder.thunder_state = nWPPikachuThunderStatusDestroy;
    }
}

// 0x8015229C
void ftPikachuSpecialAirLwLoopSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwLoop, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWLOOP_STATUS_FLAGS);
}

// 0x801522DC
void ftPikachuSpecialLwLoopSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwLoop, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWLOOP_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80152328
void ftPikachuSpecialLwLoopUpdateThunder(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag0 == 0)
    {
        ftPikachuSpecialLwMakeThunder(fighter_gobj);
    }
    fp->motion_vars.flags.flag0 = 0;

    fp->proc_damage = ftPikachuSpecialLwProcDamage;
}

// 0x8015236C
void ftPikachuSpecialLwLoopSetStatus(GObj *fighter_gobj)
{
    ftPikachuSpecialLwLoopUpdateThunder(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwLoop, 0.0F, 1.0F, FTSTATUS_PRESERVE_RUMBLE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x801523B0
void ftPikachuSpecialAirLwLoopSetStatus(GObj *fighter_gobj)
{
    ftPikachuSpecialLwLoopUpdateThunder(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwLoop, 0.0F, 1.0F, FTSTATUS_PRESERVE_RUMBLE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x801523F4
void ftPikachuSpecialLwHitProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftPikachuSpecialLwEndSetStatus(fighter_gobj);
    }
}

// 0x80152424
void ftPikachuSpecialAirLwHitProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftPikachuSpecialAirLwEndSetStatus(fighter_gobj);
    }
}

// 0x80152454
void ftPikachuSpecialAirLwHitProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    ftPhysicsApplyGravityClampTVel(fp, FTPIKACHU_THUNDER_HIT_GRAVITY, attr->tvel_base);

    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}

// 0x801524A4
void ftPikachuSpecialLwHitProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftPikachuSpecialLwHitSwitchStatusAir);
}

// 0x801524C8
void ftPikachuSpecialAirLwHitProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPikachuSpecialAirLwHitSwitchStatusGround);
}

// 0x801524EC
void ftPikachuSpecialAirLwHitSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwHit, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWHIT_STATUS_FLAGS);
}

// 0x8015252C
void ftPikachuSpecialLwHitSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwHit, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWHIT_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80152578
void ftPikachuSpecialLwHitInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag1 = 0;

    fp->proc_damage = NULL;
}

// 0x80152588
void ftPikachuSpecialLwHitSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwHit, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPikachuSpecialLwHitInitStatusVars(fighter_gobj);
}

// 0x801525C8
void ftPikachuSpecialAirLwHitSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwHit, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPikachuSpecialLwHitInitStatusVars(fighter_gobj);

    fp->physics.vel_air.y = FTPIKACHU_THUNDER_HITVEL_Y;
}

// 0x80152620
void ftPikachuSpecialAirLwEndProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonFallSetStatus);
}

// 0x80152644
void ftPikachuSpecialLwEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftPikachuSpecialLwEndSwitchStatusAir);
}

// 0x80152668
void ftPikachuSpecialAirLwEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPikachuSpecialAirLwEndSwitchStatusGround);
}

// 0x8015268C
void ftPikachuSpecialAirLwEndSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwEnd, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWEND_STATUS_FLAGS);
}

// 0x801526CC
void ftPikachuSpecialLwEndSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwEnd, fighter_gobj->anim_frame, 1.0F, FTPIKACHU_SPECIALLWEND_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80152718
void ftPikachuSpecialLwClearProcDamage(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_damage = NULL;
}

// 0x80152724
void ftPikachuSpecialLwEndSetStatus(GObj *fighter_gobj)
{
    ftPikachuSpecialLwClearProcDamage(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80152764
void ftPikachuSpecialAirLwEndSetStatus(GObj *fighter_gobj)
{
    ftPikachuSpecialLwClearProcDamage(fighter_gobj);
    ftMainSetStatus(fighter_gobj, nFTPikachuStatusSpecialAirLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
