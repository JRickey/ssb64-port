#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8015A7A0
void ftBossOkupunch1ProcUpdate(GObj *fighter_gobj)
{
    ftAnimEndCheckSetStatus(fighter_gobj, ftBossOkupunch2SetStatus);
}

// 0x8015A7C4
void ftBossOkupunch1SetStatus(GObj *fighter_gobj)
{
    FTStruct *boss_fp, *target_fp;
    s32 floor_line_id;
    s32 line_id;

    ftMainSetStatus(fighter_gobj, nFTBossStatusOkupunch1, 0.0F, 1.0F, FTSTATUS_PRESERVE_NONE);

    boss_fp = ftGetStruct(fighter_gobj);

    target_fp = ftGetStruct(boss_fp->passive_vars.boss.p->target_gobj);

    floor_line_id = target_fp->coll_data.floor_line_id;

    line_id = ((floor_line_id != -1) && floor_line_id != -2) ? floor_line_id : boss_fp->passive_vars.boss.p->default_line_id;

    ftBossCommonGetPositionCenter(line_id, &boss_fp->status_vars.boss.okupunch.pos);
}
