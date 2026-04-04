#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015AA10
void ftBossOkupunch3ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossWaitSetStatus);
}

// 0x8015AA34
void ftBossOkupunch3SetStatus(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f *translate;

    fp->lr = -1;

    ftMainSetStatus(fighter_gobj, nFTBossStatusOkupunch3, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftMainPlayAnimEventsAll(fighter_gobj);

    translate = &DObjGetStruct(fighter_gobj)->translate.vec.f;

    translate->x = fp->status_vars.boss.okupunch.pos.x;
    translate->y = fp->status_vars.boss.okupunch.pos.y + 6000.0F;
    translate->z = 0.0F;
}
