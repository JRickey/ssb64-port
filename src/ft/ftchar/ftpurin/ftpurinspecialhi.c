#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTPURIN_SPECIALHI_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM | FTSTATUS_PRESERVE_HIT)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80151550
void ftPurinSpecialHiProcUpdate(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (fp->motion_vars.flags.flag1 == 1)
    {
        if (efManagerPurinSingMakeEffect(fighter_gobj) != NULL)
        {
            fp->is_effect_attach = TRUE;
        }
        fp->motion_vars.flags.flag1 = 0;
    }
    if (fp->status_id == nFTPurinStatusSpecialHi)
    {
        ftAnimEndSetWait(fighter_gobj);
    }
    else ftAnimEndSetFall(fighter_gobj);
}

// 0x801515CC
void ftPurinSpecialHiProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftPurinSpecialHiSwitchStatusAir);
}

// 0x801515F0
void ftPurinSpecialAirHiProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPurinSpecialAirHiSwitchStatusGround);
}

// 0x80151614
void ftPurinSpecialAirHiSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialHi, fighter_gobj->anim_frame, 1.0F, FTPURIN_SPECIALHI_STATUS_FLAGS);
}

// 0x80151654
void ftPurinSpecialHiSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialAirHi, fighter_gobj->anim_frame, 1.0F, FTPURIN_SPECIALHI_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x801516A0
void ftPurinSpecialHiSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x801516D8
void ftPurinSpecialAirHiSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialAirHi, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
