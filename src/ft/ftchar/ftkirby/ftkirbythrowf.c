#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8014A430
void ftKirbyThrowFProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftKirbyThrowFFallSetStatus);
}

// 0x8014A454
void ftKirbyThrowFProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if ((mpCommonCheckFighterLanding(fighter_gobj) != FALSE) && (fp->physics.vel_air.y < 0.0F))
    {
        ftKirbyThrowFLandingSetStatus(fighter_gobj);
    }
}

// 0x8014A4A8
void ftKirbyThrowFFallSetStatus(GObj *fighter_gobj)
{
    FTStruct *this_fp = ftGetStruct(fighter_gobj);
    GObj *catch_gobj = this_fp->catch_gobj;
    FTStruct *catch_fp = ftGetStruct(catch_gobj);

    ftMainSetStatus(fighter_gobj, nFTKirbyStatusThrowFFall, 0.0F, 1.0F, FTSTATUS_PRESERVE_TEXTUREPART);

    catch_fp->is_ignore_dead = FALSE;
}

// 0x8014A4F8
void ftKirbyThrowFLandingProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        ftPhysicsApplyGroundVelFriction(fighter_gobj);
    }
    else ftPhysicsApplyAirVelTransNAll(fighter_gobj);
}

// 0x8014A538
void ftKirbyThrowFLandingProcMap(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->ga == nMPKineticsGround)
    {
        ftCommonCatchProcMap(fighter_gobj);
    }
    else if ((mpCommonCheckFighterLanding(fighter_gobj) != FALSE) && (fp->physics.vel_air.y < 0.0F))
    {
        mpCommonSetFighterWaitOrLanding(fighter_gobj);
    }
}

// 0x8014A5AC
void ftKirbyThrowFLandingSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);
    ftMainSetStatus(fighter_gobj, nFTKirbyStatusThrowFLanding, 0.0F, 1.0F, FTSTATUS_PRESERVE_TEXTUREPART);
}
