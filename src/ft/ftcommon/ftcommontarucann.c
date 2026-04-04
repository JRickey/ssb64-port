#include <ft/fighter.h>
#include <it/item.h>
#include <gr/ground.h>
#include <sc/scene.h>
#include <reloc_data.h>

// 0x80143E10
void ftCommonTaruCannProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.common.tarucann.shoot_wait != 0)
    {
        fp->status_vars.common.tarucann.shoot_wait--;

        if (fp->status_vars.common.tarucann.shoot_wait == (FTCOMMON_TARUCANN_SHOOT_WAIT / 2))
        {
            func_800269C0_275C0(nSYAudioFGMJungleTaruCannShoot);
        }
        if (fp->status_vars.common.tarucann.shoot_wait == 0)
        {
            ftCommonTaruCannShootFighter(fighter_gobj);

            return;
        }
    }
    fp->status_vars.common.tarucann.release_wait++;

    if ((fp->status_vars.common.tarucann.release_wait >= FTCOMMON_TARUCANN_RELEASE_WAIT) && (fp->status_vars.common.tarucann.shoot_wait == 0))
    {
        fp->status_vars.common.tarucann.shoot_wait = FTCOMMON_TARUCANN_SHOOT_WAIT;

        grJungleTaruCannAddAnimShoot(fp->status_vars.common.tarucann.tarucann_gobj);
    }
}

// 0x80143EB0
void ftCommonTaruCannProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->status_vars.common.tarucann.shoot_wait == 0) && (fp->input.pl.button_tap & (fp->input.button_mask_a | fp->input.button_mask_b)))
    {
        fp->status_vars.common.tarucann.shoot_wait = FTCOMMON_TARUCANN_SHOOT_WAIT;

        grJungleTaruCannAddAnimShoot(fp->status_vars.common.tarucann.tarucann_gobj);
    }
}

// 0x80143F04
void ftCommonTaruCannProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    GObj *tarucann_gobj = fp->status_vars.common.tarucann.tarucann_gobj;

    DObjGetStruct(fighter_gobj)->translate.vec.f = DObjGetStruct(tarucann_gobj)->translate.vec.f;
}

// 0x80143F30
void ftCommonTaruCannSetStatus(GObj *fighter_gobj, GObj *tarucann_gobj)
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
    ftMainSetStatus(fighter_gobj, nFTCommonStatusTaruCann, 0.0F, 0.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftPhysicsStopVelAll(fighter_gobj);

    fp->status_vars.common.tarucann.shoot_wait = 0;
    fp->status_vars.common.tarucann.release_wait = 0;
    fp->status_vars.common.tarucann.tarucann_gobj = tarucann_gobj;

    ftParamSetHitStatusAll(fighter_gobj, nGMHitStatusIntangible);

    fp->is_invisible = TRUE;

    ftParamSetCaptureImmuneMask(fp, FTCATCHKIND_MASK_ALL);
    func_800269C0_275C0(nSYAudioFGMJungleTaruCannEnter);
}

// 0x80144038
void ftCommonTaruCannShootFighter(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTThrowHitDesc *tarucann = (FTThrowHitDesc*) (((uintptr_t)gMPCollisionGroundData - (intptr_t)&llGRJungleMapMapHeader) + (intptr_t)&llGRJungleMapTaruCannThrowHitDesc);
    f32 knockback;
    s32 angle;

    DObjGetStruct(fighter_gobj)->translate.vec.f.z = 0.0F;

    knockback = ftParamGetGroundHazardKnockback(fp->percent_damage, tarucann->damage, tarucann->damage, tarucann->knockback_weight, tarucann->knockback_scale, tarucann->knockback_base, fp->attr->weight, 9, 9);

    angle = ((I_CLC_RTOD32(grJungleTaruCannGetRotate()) * -fp->lr) + 90);
    angle -= (angle / 360) * 360;

    ftCommonDamageInitDamageVars(fighter_gobj, nFTCommonStatusDamageFlyRoll, tarucann->damage, knockback, angle, fp->lr, 0, tarucann->element, 0, TRUE, TRUE, FALSE);
    ftParamUpdate1PGameDamageStats(fp, GMCOMMON_PLAYERS_MAX, nFTHitLogObjectGround, nGMHitEnvironmentTaruCann, 0, 0);

    fp->playertag_wait = 0;
    fp->tarucann_wait = FTCOMMON_TARUCANN_PICKUP_WAIT;
}
