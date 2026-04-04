#include <ft/fighter.h>
#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015BB50
void ftFoxSpecialNProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    if (fp->motion_vars.flags.flag0 != 0)
    {
        fp->motion_vars.flags.flag0 = 0;

        pos.x = FTFOX_BLASTER_SPAWN_OFF_X;
        pos.y = 0.0F;
        pos.z = 0.0F;

        gmCollisionGetFighterPartsWorldPosition(fp->joints[FTFOX_BLASTER_HOLD_JOINT], &pos);
        wpFoxBlasterMakeWeapon(fighter_gobj, &pos);
    }
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        mpCommonSetFighterWaitOrFall(fighter_gobj);
    }
}

// 0x8015BBD8
void ftFoxSpecialNProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->motion_vars.flags.flag1 != 0) && (fp->input.pl.button_tap & fp->input.button_mask_b))
    {
        if (fp->ga == nMPKineticsAir)
        {
            ftFoxSpecialAirNSetStatus(fighter_gobj);
        }
        else ftFoxSpecialNSetStatus(fighter_gobj);

        ftParamSetMotionID(fp, nFTMotionAttackIDSpecialN);
        ftParamSetStatUpdate(fp, fp->stat_flags.halfword);
        ftParamUpdate1PGameAttackStats(fp, 0);
    }
}

// 0x8015BC68
void ftFoxSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag0 = 0;
    fp->motion_vars.flags.flag1 = 0;
}

// 0x8015BC78
void ftFoxSpecialNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftFoxSpecialNInitStatusVars(fighter_gobj);
}

// 0x8015BCB8
void ftFoxSpecialAirNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTFoxStatusSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_FASTFALL);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftFoxSpecialNInitStatusVars(fighter_gobj);
}
