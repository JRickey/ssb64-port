#include <ft/fighter.h>
#include <it/item.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <reloc_data.h>

// 0x801439D0
void ftCommonTwisterProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.common.twister.release_wait++;

    if (fp->status_vars.common.twister.release_wait >= FTCOMMON_TORNADO_RELEASE_WAIT)
    {
        ftCommonTwisterShootFighter(fighter_gobj);
    }
}

// 0x80143A20
void ftCommonTwisterProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *tornado_gobj = fp->status_vars.common.twister.tornado_gobj;
    Vec3f pos = DObjGetStruct(tornado_gobj)->translate.vec.f;
    Vec3f vel;
    f32 mul;
    f32 angle_d;
    f32 mag;
    f32 unused[2];

    angle_d = (fp->status_vars.common.twister.release_wait * 0.016666668F);
    mul = (((400.0F * angle_d) + 100.0F) * 0.5F);

    pos.x += (mul * lbCommonCos(F_CLC_DTOR32(1800.0F * angle_d)));
    pos.z += (mul * lbCommonSin(F_CLC_DTOR32(1800.0F * angle_d)));
    pos.y += 500.0F * angle_d;

    syVectorDiff3D(&vel, &pos, &DObjGetStruct(fighter_gobj)->translate.vec.f);

    mag = syVectorMag3D(&vel);

    if (mag > 50.0F)
    {
        syVectorScale3D(&vel, 50.0F / mag);
    }
    fp->physics.vel_air = vel;

    DObjGetStruct(fighter_gobj)->rotate.vec.f.y = (fp->lr * F_CLC_DTOR32(90.0F)) + F_CLC_DTOR32(1800.0F * angle_d);
}

// 0x80143BC4
void ftCommonTwisterSetStatus(GObj *fighter_gobj, GObj *tornado_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftParamStopVoiceRunProcDamage(fighter_gobj);

    if ((fp->item_gobj != NULL) && (itGetStruct(fp->item_gobj)->weight == nITWeightHeavy))
    {
        ftSetupDropItem(fp);
    }
    if (fp->catch_gobj != NULL)
    {
        ftCommonThrownSetStatusDamageRelease(fp->catch_gobj);

        fp->catch_gobj = NULL;
    }
    else if (fp->capture_gobj != NULL)
    {
        ftCommonThrownDecideFighterLoseGrip(fp->capture_gobj, fighter_gobj);
    }
    if (fp->ga == nMPKineticsGround)
    {
        mpCommonSetFighterAir(fp);
    }
    ftMainSetStatus(fighter_gobj, nFTCommonStatusTwister, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPhysicsStopVelAll(fighter_gobj);

    fp->status_vars.common.twister.release_wait = 0;
    fp->status_vars.common.twister.tornado_gobj = tornado_gobj;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);
    func_800269C0_275C0(nSYAudioFGMHyruleTwisterTrapped);
}

// 0x80143CC4
void ftCommonTwisterShootFighter(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTThrowHitDesc *tornado = (FTThrowHitDesc*) (((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRHyruleMapMapHeader) + (intptr_t)&llGRHyruleMapTwisterThrowHitDesc);
    f32 knockback;
    s32 damage;

    DObjGetStruct(fighter_gobj)->translate.vec.f.z = 0.0F;

    knockback = ftParamGetCommonKnockback(fp->percent_damage, tornado->damage, tornado->damage, tornado->knockback_weight, tornado->knockback_scale, tornado->knockback_base, fp->attr->weight, 9, fp->handicap);

    if (ftParamGetBestHitStatusAll(fighter_gobj) != nGMHitStatusNormal)
    {
        damage = 0;
    }
    else damage = tornado->damage;

    ftCommonDamageInitDamageVars(fighter_gobj, -1, damage, knockback, tornado->angle, fp->lr, 0, tornado->element, 0, TRUE, TRUE, TRUE);
    ftParamUpdate1PGameDamageStats(fp, GMCOMMON_PLAYERS_MAX, nFTHitLogObjectGround, nGMHitEnvironmentTwister, 0, 0);

    if (damage != 0)
    {
        ftParamUpdateDamage(fp, damage);
    }
    fp->twister_wait = FTCOMMON_TORNADO_PICKUP_WAIT;
}
