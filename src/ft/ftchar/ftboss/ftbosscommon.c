#include <ft/fighter.h>

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80157F60
void ftBossCommonInvertLR(GObj *fighter_gobj) // Turn Master Hand around?
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->lr = -fp->lr;

    fp->joints[nFTPartsJointTopN]->rotate.vec.f.y = fp->lr * F_CST_DTOR32(90.0F);
}

// 0x80157F90
void ftBossCommonCheckEdgeInvertLR(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    Vec3f pos_left;
    Vec3f pos_right;

    mpCollisionGetFloorEdgeL(fp->passive_vars.boss.p->current_line_id, &pos_left);

    mpCollisionGetFloorEdgeR(fp->passive_vars.boss.p->current_line_id, &pos_right);

    if (((((pos_left.x + pos_right.x) * 0.5F) - DObjGetStruct(fighter_gobj)->translate.vec.f.x) * fp->lr) < 0.0F)
    {
        ftBossCommonInvertLR(fighter_gobj);
    }
}

// 0x80158030
void ftBossCommonCheckPlayerInvertLR(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    if (((DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.x - DObjGetStruct(fighter_gobj)->translate.vec.f.x) * fp->lr) < 0.0F)
    {
        ftBossCommonInvertLR(fighter_gobj);
    }
}

// 0x80158094
void ftBossCommonRandEdgeLR(s32 line_id, Vec3f *pos)
{
    if ((syUtilsRandUShort() % 2) != 0)
    {
        mpCollisionGetFloorEdgeL(line_id, pos);
    }
    else mpCollisionGetFloorEdgeR(line_id, pos);
}

// 0x801580E0
void ftBossCommonGotoTargetEdge(GObj *fighter_gobj, Vec3f *pos)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTStruct *player_fp = ftGetStruct(fp->passive_vars.boss.p->target_gobj);
    ftBossInfo *boss = fp->passive_vars.boss.p;

    if ((player_fp->coll_data.floor_line_id != -1) && (player_fp->coll_data.floor_line_id != -2))
    {
        fp->passive_vars.boss.p->current_line_id = player_fp->coll_data.floor_line_id;
    }
    else
    {
        if ((fp->coll_data.floor_line_id != -1) && (player_fp->coll_data.floor_line_id != -2))
        {
            fp->passive_vars.boss.p->current_line_id = fp->coll_data.floor_line_id;
        }
        else fp->passive_vars.boss.p->current_line_id = fp->passive_vars.boss.p->default_line_id;
    }
    ftBossCommonRandEdgeLR(fp->passive_vars.boss.p->current_line_id, pos);

    pos->y += 100.0F;
}

// 0x8015817C
void ftBossCommonSetPosOffsetY(GObj *fighter_gobj, Vec3f *pos, f32 off_y)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    pos->x = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.x;
    pos->y = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.y + off_y;
    pos->z = 0.0F;
}

// 0x801581BC
void ftBossCommonSetPosAddVelPlayer(GObj *fighter_gobj, Vec3f *pos, f32 vel_x, f32 vel_y)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTStruct *fp_unk = ftGetStruct(fp->passive_vars.boss.p->target_gobj);
    Vec3f translate = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f;
    f32 x;
    f32 y;

    x = translate.x;

    translate.x += ((syUtilsRandUShort() % 2) != 0) ? vel_x : -vel_x;

    if (mpCollisionGetFCCommonFloor(fp_unk->coll_data.floor_line_id, &translate, &y, NULL, NULL) != FALSE)
    {
        pos->x = translate.x;
    }
    else
    {
        translate.x = (x < translate.x) ? x - vel_x : x + vel_x;

        pos->x = (mpCollisionGetFCCommonFloor(fp_unk->coll_data.floor_line_id, &translate, &y, NULL, NULL) != FALSE) ? translate.x : x;
    }
    pos->y = (translate.y + y + vel_y);
    pos->z = 0.0F;
}

// 0x80158310
void ftBossCommonSetPosAddVelAuto(GObj *fighter_gobj, Vec3f *pos, f32 vel_x, f32 vel_y)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    pos->x = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.x + (((syUtilsRandUShort() % 2) != 0) ? vel_x : -vel_x);
    pos->y = DObjGetStruct(fp->passive_vars.boss.p->target_gobj)->translate.vec.f.y + vel_y;
    pos->z = 0.0F;
}

// I have no idea what is going on here but at least it matches
// Maybe Master Hand movement values or player tracking???

// 0x8015839C
void ftBossCommonGetPositionCenter(s32 line_id, Vec3f *pos_input)
{
    Vec3f pos_left;
    Vec3f pos_right;
    f32 y;

    mpCollisionGetFloorEdgeL(line_id, &pos_left);
    mpCollisionGetFloorEdgeR(line_id, &pos_right);

    pos_input->x = (pos_left.x + pos_right.x) * 0.5F;
    pos_input->z = 0.0F;
    pos_input->y = 0.0F;

    mpCollisionGetFCCommonFloor(line_id, pos_input, &y, NULL, NULL);

    pos_input->y += y;
}

// 0x80158428
void ftBossCommonSetNextAttackWait(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->passive_vars.boss.p->wait_timer = ((syUtilsRandIntRange(FTBOSS_ATTACK_WAIT_MAX) + (FTBOSS_ATTACK_WAIT_LEVEL_DIV / fp->level)) / fp->passive_vars.boss.p->wait_div);
}

// 0x80158528
void ftBossCommonUpdateFogColor(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    f32 fog_dist;
    s32 fog_blend;

    if (fp->joints[nFTPartsJointTopN]->translate.vec.f.z > 0.0F)
    {
        fog_dist = 1.0F;
    }
    else fog_dist = (fp->joints[nFTPartsJointTopN]->translate.vec.f.z < -15000.0F) ? 0.0F : (fp->joints[nFTPartsJointTopN]->translate.vec.f.z - (-15000.0F)) / 15000.0F;
    
    fog_blend = (0x7F * fog_dist) + 0x80;

    fp->fog_color.r = (fog_blend > 0xFF) ? 0xFF : fog_blend;
    fp->fog_color.g = (fog_blend > 0xFF) ? 0xFF : fog_blend;
    fp->fog_color.b = (fog_blend > 0xFF) ? 0xFF : fog_blend;
}

// 0x80158604
void ftBossCommonSetUseFogColor(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->is_use_fogcolor = TRUE;
    fp->fog_color.a = 0xFF;
}

// 0x80158620
void ftBossCommonSetDisableFogColor(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);

    fp->is_use_fogcolor = FALSE;
}

// 0x80158634
void ftBossCommonSetDefaultLineID(GObj *fighter_gobj)
{
    FTStruct *fp;

    if (mpCollisionGetLineCountType(nMPLineKindFloor) == 0)
    {
        while (TRUE)
        {
            syDebugPrintf("nothing under cll id\n");
            scManagerRunPrintGObjStatus();
        }
    }
    fp = ftGetStruct(fighter_gobj);

    mpCollisionGetLineIDsTypeCount(nMPLineKindFloor, 1, &fp->passive_vars.boss.p->default_line_id);
}

// 0x801586A0
void ftBossCommonUpdateDamageStats(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    s32 status_id = fp->status_id;

    if ((status_id != nFTBossStatusDeadLeft) && (status_id != nFTBossStatusDeadCenter) && (status_id != nFTBossStatusDeadRight))
    {
        if (fp->percent_damage >= 300)
        {
            sc1PGameBossDefeatInitInterface(fighter_gobj);

            if (fp->lr == -1)
            {
                ftBossDeadLeftSetStatus(fighter_gobj);
            }
            else ftBossDeadRightSetStatus(fighter_gobj);
        }
        else if (fp->percent_damage >= 200)
        {
            fp->passive_vars.boss.p->wait_div = 1.5F;
        }
    }
}
