#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80159500
void ftBossOkuhikouki1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossOkuhikouki2SetStatus);
}

// 0x80159524
void ftBossOkuhikouki1SetStatus(GObj *fighter_gobj)
{
    FTStruct *target_fp;
    FTStruct *boss_fp;
    s32 line_id;

    ftMainSetStatus(fighter_gobj, nFTBossStatusOkuhikouki1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    boss_fp = ftGetStruct(fighter_gobj);

    target_fp = ftGetStruct(boss_fp->passive_vars.boss.p->target_gobj);

    line_id = ((target_fp->coll_data.floor_line_id != -1) && (target_fp->coll_data.floor_line_id != -2)) ? target_fp->coll_data.floor_line_id : boss_fp->passive_vars.boss.p->default_line_id;

    ftBossCommonGetPositionCenter(line_id, &boss_fp->status_vars.boss.okuhikouki.pos);
}
