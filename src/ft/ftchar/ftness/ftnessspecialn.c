#include <ft/fighter.h>
#include <wp/weapon.h>
// // // // // // // // // // // //
//                               //
//             MACROS            //
//                               //
// // // // // // // // // // // //

#define FTNESS_SPECIALN_STATUS_FLAGS (FTSTATUS_PRESERVE_TEXTUREPART | FTSTATUS_PRESERVE_HITSTATUS | FTSTATUS_PRESERVE_EFFECT | FTSTATUS_PRESERVE_COLANIM)

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80153950
void ftNessSpecialNProcAccessory(GObj *fighter_gobj) // PK Fire setup
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    Vec3f vel;
    f32 angle;

    if (fp->motion_vars.flags.flag0 != FALSE) // Check if flag to summon PK Fire is true
    {
        fp->motion_vars.flags.flag0 = FALSE; // Revert to 0 if PK Fire is summoned, so it doesn't repeat on every frame of the move

        pos.x = 0.0F;
        pos.y = 0.0F;
        pos.z = 0.0F;

        gmCollisionGetFighterPartsWorldPosition(fp->joints[FTNESS_PKFIRE_SPAWN_JOINT], &pos);

        pos.x += FTNESS_PKFIRE_SPAWN_OFF_X * fp->lr;
        pos.y += FTNESS_PKFIRE_SPAWN_OFF_Y;
        pos.z = 0.0F;

        if (fp->ga == nMPKineticsAir)
        {
            vel.z = 0.0F;
            angle = FTNESS_PKFIRE_SPARK_ANGLE_AIR;
            vel.x = __cosf(FTNESS_PKFIRE_SPARK_ANGLE_AIR) * FTNESS_PKFIRE_SPARK_VEL_AIR * fp->lr;
            vel.y = __sinf(FTNESS_PKFIRE_SPARK_ANGLE_AIR) * FTNESS_PKFIRE_SPARK_VEL_AIR;
        }
        else
        {
            vel.z = 0.0F;
            angle = FTNESS_PKFIRE_SPARK_ANGLE_GROUND;
            vel.x = __cosf(FTNESS_PKFIRE_SPARK_ANGLE_GROUND) * FTNESS_PKFIRE_SPARK_VEL_GROUND * fp->lr;
            vel.y = __sinf(FTNESS_PKFIRE_SPARK_ANGLE_GROUND) * FTNESS_PKFIRE_SPARK_VEL_GROUND;
        }
        wpNessPKFireMakeWeapon(fighter_gobj, &pos, &vel, angle); // Spawn PK Fire
    }
}

// 0x80153AC0
void ftNessSpecialNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterOnEdge(fighter_gobj, ftNessSpecialNSwitchStatusAir);
}

// 0x80153AE4
void ftNessSpecialAirNProcMap(GObj *fighter_gobj)
{
    mpCommonProcFighterLanding(fighter_gobj, ftNessSpecialAirNSwitchStatusGround);
}

// 0x80153B08
void ftNessSpecialAirNSwitchStatusGround(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterGround(fp);

    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialN, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALN_STATUS_FLAGS); // Action State Change

    fp->proc_accessory = ftNessSpecialNProcAccessory;
}

// 0x80153B5C
void ftNessSpecialNSwitchStatusAir(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    mpCommonSetFighterAir(fp);

    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirN, fighter_gobj->anim_frame, 1.0F, FTNESS_SPECIALN_STATUS_FLAGS); // Action State Change

    ftPhysicsClampAirVelXMax(fp);

    fp->proc_accessory = ftNessSpecialNProcAccessory;
}

// 0x80153BB8
void ftNessSpecialNInitStatusVars(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->motion_vars.flags.flag0 = FALSE;
    fp->proc_accessory = ftNessSpecialNProcAccessory;
}

// 0x80153BD0
void ftNessSpecialNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialNInitStatusVars(fighter_gobj);
}

// 0x80153C10
void ftNessSpecialAirNSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTNessStatusSpecialAirN, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);
    ftNessSpecialNInitStatusVars(fighter_gobj);
}
