#include <ft/fighter.h>
#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801636D0
void ftLinkSpecialNDestroyBoomerang(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->passive_vars.link.boomerang_gobj != NULL)
    {
        wpMainDestroyWeapon(fp->passive_vars.link.boomerang_gobj);

        fp->passive_vars.link.boomerang_gobj = NULL;
    }
}

// 0x80163708
void ftLinkSpecialNMakeBoomerang(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    if (fp->motion_vars.flags.flag0 != 0)
    {
        fp->motion_vars.flags.flag0 = 0;

        pos.x = 0.0F;
        pos.y = 0.0F;
        pos.z = 0.0F;

        gmCollisionGetFighterPartsWorldPosition(fp->joints[FTLINK_BOOMERANG_SPAWN_JOINT], &pos);

        fp->passive_vars.link.boomerang_gobj = wpLinkBoomerangMakeWeapon(fighter_gobj, &pos);
    }
}

// 0x80163770
void ftLinkSpecialNProcUpdate(GObj *fighter_gobj)
{
    ftLinkSpecialNMakeBoomerang(fighter_gobj);
    ftAnimEndSetWait(fighter_gobj);
}

// 0x80163798
void ftLinkSpecialAirNProcUpdate(GObj *fighter_gobj)
{
    ftLinkSpecialNMakeBoomerang(fighter_gobj);
    ftAnimEndSetFall(fighter_gobj);
}

// 0x801637C0
void ftLinkSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftLinkSpecialNSwitchStatusAir);
}

// 0x801637E4
void ftLinkSpecialNEmptyProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftLinkSpecialNEmptySwitchStatusAir);
}

// 0x80163808
void ftLinkSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftLinkSpecialAirNSwitchStatusGround);
}

// 0x8016382C
void ftLinkSpecialAirNEmptyProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftLinkSpecialAirNEmptySwitchStatusGround);
}

// 0x80163850 - Check for Smash-B input and reset flag0
void ftLinkSpecialNProcStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag0 = 0;

    if ((ABS(fp->input.pl.stick_range.x) >= FTLINK_BOOMERANG_SMASH_STICK_MIN) && (fp->hold_stick_x < FTLINK_BOOMERANG_SMASH_BUFFER))
    {
        fp->status_vars.link.specialn.is_smash = TRUE;
#if defined(REGION_US)
        fp->stat_flags.is_smash_attack = TRUE;
#endif
    }
    else fp->status_vars.link.specialn.is_smash = FALSE;
}

// 0x801638AC
void ftLinkSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    mpCommonSetFighterAir(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirN, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_MODELPART);
}

// 0x801638EC
void ftLinkSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialN, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_MODELPART);
}

// 0x8016392C
void ftLinkSpecialNEmptySwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirNEmpty, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
    fp->is_special_interrupt = TRUE;
}

// 0x8016397C
void ftLinkSpecialAirNEmptySwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialNEmpty, fighter_gobj->anim_frame, 1.0F, FTSTATUS_PRESERVE_NONE);
    fp->is_special_interrupt = TRUE;
}

// 0x801639CC
void ftLinkSpecialNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftLinkSpecialNProcStatus;

    if (fp->passive_vars.link.boomerang_gobj != NULL)
    {
        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialNEmpty, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

        fp->is_special_interrupt = TRUE;
    }
    else ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80163A4C
void ftLinkSpecialAirNSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->proc_status = ftLinkSpecialNProcStatus;

    if (fp->passive_vars.link.boomerang_gobj != NULL)
    {
        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirNEmpty, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

        fp->is_special_interrupt = TRUE;
    }
    else ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x80163ACC
void ftLinkSpecialNGetSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsAir)
    {
        ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialAirNReturn, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    }
    else ftMainSetStatus(fighter_gobj, nFTLinkStatusSpecialNGet, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    ftMainPlayAnimEventsAll(fighter_gobj);
}
