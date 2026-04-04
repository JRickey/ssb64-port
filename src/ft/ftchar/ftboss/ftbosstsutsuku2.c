#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159F40
void ftBossTsutsuku2ProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f translate;
    Vec3f vel;
    f32 magnitude;

    fp->status_vars.boss.tsutsuku.wait_timer--;

    if (fp->status_vars.boss.tsutsuku.wait_timer == 0)
    {
        ftBossTsutsuku3SetStatus(fighter_gobj);
    }
    else
    {
        translate.x = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.x + (-fp->lr * 900.0F);
        translate.y = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.y + 300.0F;
        translate.z = 0.0F;

        syVectorDiff3D(&vel, &translate, &DObjGetStruct(fighter_gobj)->translate.vec.f);

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
}

// 0x8015A070
void ftBossTsutsuku2SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp;

    ftMainSetStatus(fighter_gobj, nFTBossStatusTsutsuku2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    fp = ftGetStruct(fighter_gobj);

    fp->status_vars.boss.tsutsuku.wait_timer = syUtilsRandIntRange(80) + 60;
}
