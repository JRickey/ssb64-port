#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015ADC0
void ftBossDeadCenterProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.boss.dead.dead_timer--;

    if (fp->status_vars.boss.dead.dead_timer == 0)
    {
        fp->proc_physics = NULL;
    }
}

// 0x8015ADE8
void ftBossDeadCenterSetStatus(GObj *fighter_gobj)
{
    FTStruct *fp;
    f32 vel_z;

    ftMainSetStatus(fighter_gobj, nFTBossStatusDeadCenter, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp = ftGetStruct(fighter_gobj);

    fp->physics.vel_air.x = 0.0F;
    fp->physics.vel_air.y = __sinf(F_CST_DTOR32(45.0F)) * 100.0F;

    vel_z = __cosf(F_CST_DTOR32(45.0F)) * 100.0F;

    fp->status_vars.boss.dead.dead_timer = 200;

    fp->physics.vel_air.z = -vel_z;
}
