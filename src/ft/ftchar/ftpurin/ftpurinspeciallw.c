#include <ft/fighter.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTPURIN_SPECIALLW_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80151710
void ftPurinSpecialLwProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnFloor(fighter_gobj, ftPurinSpecialLwSwitchStatusAir);
}

// 0x80151734
void ftPurinSpecialAirLwProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftPurinSpecialAirLwSwitchStatusGround);
}

// 0x80151758
void ftPurinSpecialAirLwSwitchStatusGround(GObj *fighter_gobj)
{
    mpCommonSetFighterGround(ftGetStruct(fighter_gobj));
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialLw, fighter_gobj->anim_frame, 1.0F, FTPURIN_SPECIALLW_STATUS_FLAGS);
}

// 0x80151798
void ftPurinSpecialLwSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialAirLw, fighter_gobj->anim_frame, 1.0F, FTPURIN_SPECIALLW_STATUS_FLAGS);
    ftPhysicsClampAirVelXMax(fp);
}

// 0x801517E4
void ftPurinSpecialLwSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialLw, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}

// 0x8015181C
void ftPurinSpecialAirLwSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTPurinStatusSpecialAirLw, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
}
