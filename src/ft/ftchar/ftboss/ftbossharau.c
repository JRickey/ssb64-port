#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801593E0
void ftBossHarauResetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusHarau, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
}

// 0x8015940C
void ftBossHarauProcUpdate(GObj *fighter_gobj)
{
    if (fighter_gobj->anim_frame <= 0.0F)
    {
        FTStruct *fp = ftGetStruct(fighter_gobj);

        if (fp->coll_data.floor_line_id == -1)
        {
            ftBossWaitSetStatus(fighter_gobj);
        }
        else ftBossHarauResetStatus(fighter_gobj);
    }
}

// 0x8015946C
void ftBossHarauProcPhysics(GObj *fighter_gobj)
{
    FTStruct *fp;

    ftPhysicsApplyAirVelTransNAll(fighter_gobj);

    fp = ftGetStruct(fighter_gobj);

    fp->physics.vel_air.x += fp->lr * FTBOSS_HARAU_VEL_X;
}

// 0x801594BC
void ftBossHarauSetStatus(GObj *fighter_gobj)
{
    ftMainSetStatus(fighter_gobj, nFTBossStatusHarau, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);
    ftBossCommonCheckEdgeInvertLR(fighter_gobj);
}
