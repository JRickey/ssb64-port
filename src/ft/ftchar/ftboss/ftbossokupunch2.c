#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015A840
void ftBossOkupunch2ProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        gmCameraSetStatusDefault();
        ftBossOkupunch3SetStatus(fighter_gobj);
        ftBossCommonSetDisableFogColor(fighter_gobj);
    }
}

// 0x8015A890
void ftBossOkupunch2ProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp;
    f32 dist_x;

    ftPhysicsSetAirVelTransN(fighter_gobj);

    fp = ftGetStruct(fighter_gobj);

    dist_x = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.x - DObjGetStruct(fighter_gobj)->translate.vec.f.x;

    if (ABSF(dist_x) > 40.0F)
    {
        f32 vel_x = (dist_x > 0.0F) ? 40.0F : -40.0F;

        fp->physics.vel_air.x += vel_x;
    }
    else fp->physics.vel_air.x += dist_x;
    
    ftBossCommonUpdateFogColor(fighter_gobj);
}

// 0x8015A950
void ftBossOkupunch2SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f *translate;
    Vec3f sp34;
    Vec3f sp28;

    fp->lr = 0;

    ftMainSetStatus(fighter_gobj, nFTBossStatusOkupunch2, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;

    translate->x = fp->status_vars.boss.okupunch.pos.x;
    translate->y = fp->status_vars.boss.okupunch.pos.y;
    translate->z = -15000.0F;

    sp34.x = 0.0F;
    sp34.y = 0.0F;
    sp34.z = 0.0F;

    sp28.x = 0.0F;
    sp28.y = 1000.0F;
    sp28.z = 7000.0F;

    gmCameraSetStatusMapZoom(&sp34, &sp28);
    ftBossCommonSetUseFogColor(fighter_gobj);
}
