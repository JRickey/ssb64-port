#include <ft/fighter.h>
#include <wp/weapon.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80156CB0
void ftKirbyCopyFoxSpecialNProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;

    if (fp->motion_vars.flags.flag0 != FALSE)
    {
        fp->motion_vars.flags.flag0 = FALSE;

        pos.x = FTKIRBY_COPYFOX_BLASTER_SPAWN_OFF_X;
        pos.y = 0.0F;
        pos.z = 0.0F;

        gmCollisionGetFighterPartsWorldPosition(fp->joints[FTKIRBY_COPYFOX_BLASTER_SPAWN_JOINT], &pos);
        wpFoxBlasterMakeWeapon(fighter_gobj, &pos);
    }
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        mpCommonSetFighterWaitOrFall(fighter_gobj);
    }
}

// 0x80156D38
void ftKirbyCopyFoxSpecialNProcInterrupt(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((fp->motion_vars.flags.flag1 != 0) && (fp->input.pl.button_tap & fp->input.button_mask_b))
    {
        if (fp->ga == nMPKineticsAir)
        {
            ftKirbyCopyFoxSpecialAirNSetStatus(fighter_gobj);
        }
        else ftKirbyCopyFoxSpecialNSetStatus(fighter_gobj);

        ftParamSetMotionID(fp, nFTMotionAttackIDSpecialNCopyFox);
        ftParamSetStatUpdate(fp, fp->stat_flags.halfword);
        ftParamUpdate1PGameAttackStats(fp, 0);
    }
}

// 0x80156DC8
void ftKirbyCopyFoxSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag0 = 0;
    fp->motion_vars.flags.flag1 = 0;
}

// 0x80156DD8
void ftKirbyCopyFoxSpecialNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyFoxSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyFoxSpecialNInitStatusVars(fighter_gobj);
}

// 0x80156E18
void ftKirbyCopyFoxSpecialAirNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusCopyFoxSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_FASTFALL);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftKirbyCopyFoxSpecialNInitStatusVars(fighter_gobj);
}
