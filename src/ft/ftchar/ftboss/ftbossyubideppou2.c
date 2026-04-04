#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015A600
void ftBossYubideppou2UpdatePosition(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos;
    Vec3f vel;
    f32 magnitude;

    pos.x = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.x + (-fp->lr * 3000.0F);
    pos.y = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.y;
    pos.z = 0.0F;

    syVectorDiff3D(&vel, &pos, &DObjGetStruct(fighter_gobj)->translate.vec.f);

    magnitude = syVectorMag3D(&vel);

    if (magnitude < 5.0F)
    {
        fp->physics.vel_air.x = vel.x;
        fp->physics.vel_air.y = vel.y;
    }
    else
    {
        syVectorNorm3D(&vel);
        syVectorScale3D(&vel, magnitude * 0.1F);

        fp->physics.vel_air.x = vel.x;
        fp->physics.vel_air.y = vel.y;
    }
}

// 0x8015A6FC
void ftBossYubideppou2ProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->status_vars.boss.yubideppou.wait_timer--;

    if (fp->status_vars.boss.yubideppou.wait_timer == 0)
    {
        ftBossYubideppou3SetStatus(fighter_gobj);
    }
    else ftBossYubideppou2UpdatePosition(fighter_gobj);
}

// 0x8015A748
void ftBossYubideppou2SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp;

    ftMainSetStatus(fighter_gobj, nFTBossStatusYubideppou2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp = ftGetStruct(fighter_gobj);

    fp->status_vars.boss.yubideppou.wait_timer = syUtilsRandIntRange(120) + 60;
}
