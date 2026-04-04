#include <ft/fighter.h>
#include <wp/weapon.h>
#include <reloc_data.h>

// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTNESS_SPECIALLWSTART_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)
#define FTNESS_SPECIALLWHOLD_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)
#define FTNESS_SPECIALLWHIT_STATUS_FLAGS (FTSTATUS_PRESERVE_LOOPSFX | FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)
#define FTNESS_SPECIALLWEND_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801551E0
void ftNessSpecialLwCheckRelease(FTStruct *fp) // PSI Magnet interrupt?
{
    if (!(fp->input.pl.button_hold & fp->input.button_mask_b))
    {
        fp->status_vars.ness.speciallw.is_release = TRUE;
    }
}

// 0x80155204
void ftNessSpecialLwDecReleaseLag(FTStruct *fp)
{
    if (fp->status_vars.ness.speciallw.release_lag != 0)
    {
        fp->status_vars.ness.speciallw.release_lag--;
    }
}

// 0x8015521C
void ftNessSpecialLwProcAbsorb(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    DObj *joint = fp->joints[nFTPartsJointTopN];

    fp->lr = fp->absorb_lr;

    joint->rotate.vec.f.y += F_CST_DTOR32(-180.0F);

    ftParamsUpdateFighterPartsTransformAll(fp->joints[nFTPartsJointTopN]);

    if (fp->ga == nMPKineticsGround)
    {
        ftNessSpecialLwHitSetStatus(fighter_gobj);
    }
    else ftNessSpecialAirLwHitSetStatus(fighter_gobj);
}

// 0x801552A0
void ftNessSpecialLwStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftNessSpecialLwHoldSetStatus);
}

// 0x801552C4
void ftNessSpecialAirLwStartProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftNessSpecialAirLwHoldSetStatus);
}

// 0x801552E8
void ftNessSpecialLwProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->status_vars.ness.speciallw.gravity_delay != 0)
    {
        fp->status_vars.ness.speciallw.gravity_delay--;
    }
}

// 0x80155304
void ftNessSpecialAirLwProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTAttributes *attr = fp->attr;

    if (fp->status_vars.ness.speciallw.gravity_delay != 0)
    {
        fp->status_vars.ness.speciallw.gravity_delay--;
    }
    else ftPhysicsApplyGravityClampTVel(fp, 0.8F, attr->tvel_base);
    
    if (ftPhysicsCheckClampAirVelXDecMax(fp, attr) == FALSE)
    {
        ftPhysicsApplyAirVelXFriction(fp, attr);
    }
}
// 0x80155380
void ftNessSpecialLwStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialLwStartSwitchStatusAir);
}

// 0x801553A4
void ftNessSpecialAirLwStartProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirLwStartSwitchStatusGround);
}

// 0x801553C8
void ftNessSpecialAirLwStartSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwStart, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWSTART_STATUS_FLAGS);
}

// 0x80155408
void ftNessSpecialLwStartSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwStart, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWSTART_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80155454
void ftNessSpecialLw_InitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.ness.speciallw.release_lag = FTNESS_PSYCHICMAGNET_RELEASE_LAG;
    fp->status_vars.ness.speciallw.is_release = FALSE;
    fp->status_vars.ness.speciallw.gravity_delay = FTNESS_PSYCHICMAGNET_GRAVITY_DELAY;
}

// 0x80155470
void ftNessSpecialLwStartSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialLw_InitStatusVars(fighter_gobj);
}

// 0x801554B0
void ftNessSpecialAirLwStartSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwStart, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialLw_InitStatusVars(fighter_gobj);

    fp->physics.vel_air.x /= 2;
    fp->physics.vel_air.y = 0.0F;
}

// 0x80155518
void ftNessSpecialLw_UpdateReleaseLag(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialLwCheckRelease(fp);
    ftNessSpecialLwDecReleaseLag(fp);
}

// 0x80155544
void ftNessSpecialLwHoldProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialLw_UpdateReleaseLag(fighter_gobj);

    if ((fp->status_vars.ness.speciallw.release_lag <= 0) && (fp->status_vars.ness.speciallw.is_release != FALSE))
    {
        ftNessSpecialLwEndSetStatus(fighter_gobj);
    }
}

// 0x80155594
void ftNessSpecialAirLwHoldProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    ftNessSpecialLw_UpdateReleaseLag(fighter_gobj);

    if ((fp->status_vars.ness.speciallw.release_lag <= 0) && (fp->status_vars.ness.speciallw.is_release != FALSE))
    {
        ftNessSpecialAirLwEndSetStatus(fighter_gobj);
    }
}

// 0x801555E4
void ftNessSpecialLwHoldProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialLwHoldSwitchStatusAir);
}

// 0x80155608
void ftNessSpecialAirLwHoldProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirLwHoldSwitchStatusGround);
}

// 0x8015562C
void ftNessSpecialAirLwHoldSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwHold, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWHOLD_STATUS_FLAGS);

    fp->is_absorb = TRUE;
}

// 0x80155680
void ftNessSpecialLwHoldSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwHold, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWHOLD_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);

    fp->is_absorb = TRUE;
}

// 0x801556E0
void ftNessSpecialLwInitVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (!(fp->is_effect_attach))
    {
        if (efManagerNessPsychicMagnetMakeEffect(fighter_gobj) != NULL)
        {
            fp->is_effect_attach = TRUE;
        }
    }
    fp->is_absorb = TRUE;

    fp->special_coll = (FTSpecialColl*) ((uintptr_t)gFTNessFileMainMotion + (intptr_t)&llNessMainMotionLwAbsorbFTSpecialColl);

    fp->motion_vars.flags.flag1 = 0;
}

// 0x80155750
void ftNessSpecialLwHoldSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwHold, 0.0F, 1.0F, (FTSTATUS_PRESERVE_LOOPSFX | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialLwInitVars(fighter_gobj);
}

// 0x80155794
void ftNessSpecialAirLwHoldSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwHold, 0.0F, 1.0F, (FTSTATUS_PRESERVE_LOOPSFX | FTSTATUS_PRESERVE_EFFECT));
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialLwInitVars(fighter_gobj);
}

// 0x801557D8
void ftNessSpecialLwHitProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftNessSpecialLwHoldSetStatus(fighter_gobj);
    }
}

// 0x80155808
void ftNessSpecialAirLwHitProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 != 0)
    {
        ftNessSpecialAirLwHoldSetStatus(fighter_gobj);
    }
}

// 0x80155838
void ftNessSpecialLwHitProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialLwHitSwitchStatusAir);
}

// 0x8015585C
void ftNessSpecialAirLwHitProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirLwHitSwitchStatusGround);
}

// 0x80155880
void ftNessSpecialAirLwHitSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwHit, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWHIT_STATUS_FLAGS);
    fp->is_absorb = TRUE;
}

// 0x801558D4
void ftNessSpecialLwHitSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct* fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwHit, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWHIT_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);

    fp->is_absorb = TRUE;
}

// 0x80155934
void ftNessSpecialLwHitSetAbsorbTrue(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->is_absorb = TRUE;
}

// 0x80155948
void ftNessSpecialLwHitSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwHit, 0.0F, 1.0F, FTSTATUS_PRESERVE_EFFECT);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialLwHitSetAbsorbTrue(fighter_gobj);
}

// 0x8015598C
void ftNessSpecialAirLwHitSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwHit, 0.0F, 1.0F, FTSTATUS_PRESERVE_EFFECT);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialLwHitSetAbsorbTrue(fighter_gobj);
}

// 0x801559D0
void ftNessSpecialAirLwEndProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftCommonFallSetStatus);
}

// 0x801559F4
void ftNessSpecialLwEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialLwEndSwitchStatusAir);
}

// 0x80155A18
void ftNessSpecialAirLwEndProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterCliff(fighter_gobj, ftNessSpecialAirLwEndSwitchStatusGround);
}

// 0x80155A3C
void ftNessSpecialAirLwEndSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwEnd, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWEND_STATUS_FLAGS);
}

// 0x80155A7C
void ftNessSpecialLwEndSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwEnd, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALLWEND_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x80155AC8
void ftNessSpecialLwEndSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80155B00
void ftNessSpecialAirLwEndSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirLwEnd, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
